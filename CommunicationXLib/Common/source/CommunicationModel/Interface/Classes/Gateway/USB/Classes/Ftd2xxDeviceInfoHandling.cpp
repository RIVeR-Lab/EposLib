#include "stdafx.h"
#include <CommunicationModel/Interface/Classes/Gateway/USB/Classes/Ftd2xxDeviceInfoHandling.h>

#include <stdio.h>
#include <memory.h>
#include <Process/MmcProcess.h>
#include <Log/MmcLogger.h>
#include <Storage/MmcFileInfo.h>
#include <CommunicationModel/Interface/Classes/Gateway/USB/Classes/Ftd2xxDeviceInfo.h>

CMmcCriticalSection CFtd2xxDeviceInfoHandling::m_CriticalSection;

CFtd2xxDeviceInfoHandling::CFtd2xxDeviceInfoHandling(void): 
	m_pRegistrySemaphore(0)
{
	CreateRegistrySemaphore();
}

CFtd2xxDeviceInfoHandling::CFtd2xxDeviceInfoHandling(int p_lInstanceValue) : CDeviceInfoHandlingBase(p_lInstanceValue),
	m_pRegistrySemaphore(0)
	

{
	CreateRegistrySemaphore();
}

CFtd2xxDeviceInfoHandling::~CFtd2xxDeviceInfoHandling(void)
{
	ResetDeviceInfoList();
	DeleteRegistrySemaphore();
}

BOOL CFtd2xxDeviceInfoHandling::CreateRegistrySemaphore()
{
	const long k_NbOfAllowedClients = 1;

	CStdString strName = _T("Maxon-Ftd2xxDeviceInfoHandling");
	BOOL oResult = FALSE;

	if(!m_pRegistrySemaphore)
	{
		m_pRegistrySemaphore = new CMmcSemaphore(k_NbOfAllowedClients, k_NbOfAllowedClients, strName);
		oResult = TRUE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::DeleteRegistrySemaphore()
{
	if(m_pRegistrySemaphore)
	{
		delete m_pRegistrySemaphore;
		m_pRegistrySemaphore = 0;

		return TRUE;
	}

	return FALSE;
}

CMmcCriticalSection* CFtd2xxDeviceInfoHandling::GetSyncInstance()
{
	return &m_CriticalSection;
}

CFtd2xxDeviceInfoHandling& CFtd2xxDeviceInfoHandling::operator=(CFtd2xxDeviceInfoHandling& p_rOther)
{
	if(this != &p_rOther)
	{
		CopyDeviceInfoList(p_rOther.m_DeviceInfoList, m_DeviceInfoList);
	}

	return *this;
}

BOOL CFtd2xxDeviceInfoHandling::CopyDeviceInfos(CFtd2xxDeviceInfoHandling* p_pOther)
{
	BOOL oResult = FALSE;

	if(p_pOther)
	{
		oResult = CopyDeviceInfoList(p_pOther->m_DeviceInfoList, m_DeviceInfoList);
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::GetPortNames(CStdStringArray& p_rPortNames)
{
	BOOL oResult = TRUE;
	//Reset
	p_rPortNames.clear();

	//Sort List by PortName
	SortDeviceInfoListByPortName(m_DeviceInfoList);

	//Add Port Names
	for(tDeviceInfoList::iterator it = m_DeviceInfoList.begin(); it != m_DeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo != 0)
			p_rPortNames.push_back(pDeviceInfo->GetPortName());
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::GetSerialNumber(CStdString p_PortName, CStdString& p_SerialNumber)
{
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it=m_DeviceInfoList.begin();
			it!=m_DeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo!=0)
		{
			if(p_PortName==pDeviceInfo->GetPortName())
			{
				p_SerialNumber = pDeviceInfo->GetSerialNumber();
				oResult = TRUE;
				break;
			}
		}
	}

	return oResult;
}


BOOL CFtd2xxDeviceInfoHandling::GetLocationId(CStdString p_PortName, DWORD& p_rulLocationId)
{
	BOOL oResult = FALSE;

	//Check Port Names
	if(!oResult)
	{
		for(tDeviceInfoList::iterator it = m_DeviceInfoList.begin(); it != m_DeviceInfoList.end(); it++)
		{
			CFtd2xxDeviceInfo* pDeviceInfo = (*it);
			if(pDeviceInfo != 0)
			{
				if(p_PortName == pDeviceInfo->GetPortName())
				{
					p_rulLocationId = pDeviceInfo->GetLocId();
					oResult = TRUE;
					break;
				}
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::UpdateLocationId(tDeviceInfoList& p_rDeviceInfoList, CStdString p_PortName)
{
	CFtd2xxDeviceInfo* pDeviceInfo = 0;
	CFtd2xxDeviceInfo* pDeviceInfoToBeUpdated = 0;
	BOOL oResult = FALSE;

	if(FindDeviceInfoByPortName(p_PortName, m_DeviceInfoList, pDeviceInfoToBeUpdated) && pDeviceInfoToBeUpdated)
	{
		if(FindDeviceInfoBySerialNumber(*pDeviceInfoToBeUpdated, p_rDeviceInfoList, pDeviceInfo) && pDeviceInfo)
		{
			pDeviceInfoToBeUpdated->Init(pDeviceInfo->GetLocId(), pDeviceInfo->GetSerialNumber(), pDeviceInfo->GetDrvFlags());
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::InitDeviceInfos(tDeviceInfoList& p_rDeviceInfoList)
{
	CMmcSingleLock lock(m_pRegistrySemaphore, TRUE);

	tPortList openPortList;
	BOOL oResult = FALSE;

	//Read DeviceInfo List from Registry
	ReadFromRegistry();

	//Logging
	#ifdef _Logging_UsbEnumeration
		WriteToLogFile(_T("I-ReadFromFtdiDrv"), p_rDeviceInfoList);
		WriteToLogFile(_T("I-ReadFromReg"));
	#endif

	//Update Removed Devices
	if(UpdateRemovedDevices(p_rDeviceInfoList, openPortList))
	{
		//Update Added Devices
		if(UpdateAddedDevices(p_rDeviceInfoList, openPortList))
		{
			if(UpdateNewAddedDevices(p_rDeviceInfoList, openPortList))
			{
				//Write DeviceInfo List to Registry
				WriteToRegistry();

				//Logging
				#ifdef _Logging_UsbEnumeration
					WriteToLogFile(_T("I-WriteToReg"));
				#endif

				oResult = TRUE;
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::UpdateDeviceInfos(tDeviceInfoList& p_rDeviceInfoList, tPortList& p_rOpenPortList)
{
	CMmcSingleLock lock(m_pRegistrySemaphore, TRUE);

	BOOL oResult = FALSE;

	//Read DeviceInfo List from Registry
	ReadFromRegistry();

	//Logging
	#ifdef _Logging_UsbEnumeration
		WriteToLogFile(_T("U-ReadFromFtdiDrv"), p_rDeviceInfoList);
		WriteToLogFile(_T("U-ReadFromReg"));
	#endif

	//Update Removed Devices
	if(UpdateRemovedDevices(p_rDeviceInfoList, p_rOpenPortList))
	{
		//Update Added Devices
		if(UpdateAddedDevices(p_rDeviceInfoList, p_rOpenPortList))
		{
			if(UpdateNewAddedDevices(p_rDeviceInfoList, p_rOpenPortList))
			{
				//Write DeviceInfo List to Registry
				WriteToRegistry();

				//Logging
				#ifdef _Logging_UsbEnumeration
					WriteToLogFile(_T("U-WriteToReg"));
				#endif

				oResult = TRUE;
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::UpdatePortOpened(CStdString p_PortName, BOOL p_oOpened)
{
	CMmcSingleLock lock(m_pRegistrySemaphore, TRUE);

	BOOL oResult = FALSE;

	if(UpdateRegistry(p_PortName, p_oOpened))
	{
		oResult = ReadFromRegistry();
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::UpdateRemovedDevices(tDeviceInfoList& p_rFtdiDrvDeviceInfoList, tPortList& p_rOpenPortList)
{
	BOOL oResult = TRUE;

	//Check Ports
	for(tDeviceInfoList::iterator it = m_DeviceInfoList.begin(); it != m_DeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo != 0)
		{
			//Reset OpenedBy if process is not alive
			if(!MmcIsProcessAvailable(pDeviceInfo->GetOpenedByProcess()))
			{
				pDeviceInfo->SetOpenedByProcess(0);
				pDeviceInfo->SetOpenedByInstance(0);
				pDeviceInfo->SetOpenedBy(_T(""));
			}
			CFtd2xxDeviceInfo* pExistingDeviceInfo = 0;
			//Update List
			if(!IsPortExistingInList(*pDeviceInfo, p_rFtdiDrvDeviceInfoList, pExistingDeviceInfo))
			{
				// Reset PluggedIn Flag
				pDeviceInfo->SetPluggedIn(0);
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::UpdateAddedDevices(tDeviceInfoList& p_rFtdiDrvDeviceInfoList, tPortList& p_rOpenPortList)
{
	BOOL oResult = TRUE;
	BOOL oRestarted(FALSE);

	//Check Ports
	tDeviceInfoList::iterator it = p_rFtdiDrvDeviceInfoList.begin();
	while(it != p_rFtdiDrvDeviceInfoList.end())
	{
		//Init
		oRestarted = FALSE;

		CFtd2xxDeviceInfo* pFtdiDrvDeviceInfo = (*it);
		if(pFtdiDrvDeviceInfo)
		{
			CFtd2xxDeviceInfo* pExistingDeviceInfo = 0;
			if(IsPortExistingInList(*pFtdiDrvDeviceInfo, m_DeviceInfoList, pExistingDeviceInfo) && pExistingDeviceInfo)
			{
				//Port is plugged in
				if(!pExistingDeviceInfo->IsOpened())
				{
					//Port is not opened by any process
					pExistingDeviceInfo->Update(*pFtdiDrvDeviceInfo);
					pExistingDeviceInfo->SetPluggedIn(1);
				}
				else if(pExistingDeviceInfo->IsOpenedBy(MmcGetCurrentProcessId(), m_lInstanceValue))
				{
					if(!pExistingDeviceInfo->IsPluggedIn())
					{
						pExistingDeviceInfo->Update(*pFtdiDrvDeviceInfo);
						pExistingDeviceInfo->SetPluggedIn(1);

						//Reopen Port
						ReopenPort(pExistingDeviceInfo->GetPortName(), p_rOpenPortList);
					}
				}

				//Remove from List
				p_rFtdiDrvDeviceInfoList.remove(pFtdiDrvDeviceInfo);
				delete pFtdiDrvDeviceInfo;

				//restart check
				if(p_rFtdiDrvDeviceInfoList.size() > 0)
				{
					it = p_rFtdiDrvDeviceInfoList.begin();
					oRestarted = TRUE;
				}
				else
				{
					//Last Element removed
					break;
				}
			}
		}

		//Next DeviceInfo
		if(!oRestarted) it++;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::UpdateNewAddedDevices(tDeviceInfoList& p_rFtdiDrvDeviceInfoList, tPortList& p_rOpenPortList)
{
	CFtd2xxDeviceInfo* pFtdiDrvDeviceInfo = 0;
	CFtd2xxDeviceInfo* pFirstNonPluggedInDeviceInfo = 0;
	BOOL oResult = TRUE;

	//Check Ports
	for(tDeviceInfoList::iterator it = p_rFtdiDrvDeviceInfoList.begin(); it != p_rFtdiDrvDeviceInfoList.end(); it++)
	{
		pFtdiDrvDeviceInfo = (*it);
		if(pFtdiDrvDeviceInfo)
		{
			if(FindNonPluggedInDeviceInfoInList(m_DeviceInfoList, pFirstNonPluggedInDeviceInfo) && pFirstNonPluggedInDeviceInfo)
			{
				//Port is plugged in
				if(!pFirstNonPluggedInDeviceInfo->IsOpened())
				{
					//Port is not opened by any process
					pFirstNonPluggedInDeviceInfo->Update(*pFtdiDrvDeviceInfo);
					pFirstNonPluggedInDeviceInfo->SetPluggedIn(1);
				}
				else if(pFirstNonPluggedInDeviceInfo->IsOpenedBy(MmcGetCurrentProcessId(), m_lInstanceValue))
				{
					if(!pFirstNonPluggedInDeviceInfo->IsPluggedIn())
					{
						pFirstNonPluggedInDeviceInfo->Update(*pFtdiDrvDeviceInfo);
						pFirstNonPluggedInDeviceInfo->SetPluggedIn(1);

						//Reopen Port
						ReopenPort(pFirstNonPluggedInDeviceInfo->GetPortName(), p_rOpenPortList);
					}
				}
			}
			else
			{
				//Add new entry
				pFtdiDrvDeviceInfo->SetOpenedBy(_T(""));
				pFtdiDrvDeviceInfo->SetOpenedByProcess(0);
				pFtdiDrvDeviceInfo->SetOpenedByInstance(0);
				pFtdiDrvDeviceInfo->SetPluggedIn(1);
				AddToDeviceInfoList(*pFtdiDrvDeviceInfo);
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::ReopenPort(CStdString p_PortName, tPortList& p_rOpenPortList)
{
	BOOL oResult = FALSE;

	for(tPortList::iterator it = p_rOpenPortList.begin(); it != p_rOpenPortList.end(); it++)
	{
		CPortBase* pPort = (*it);
		if(pPort)
		{
			//Get PortName
			CStdString portName = _T("");
			if(pPort->GetPortName(&portName))
			{
				//Check PortName
				if(p_PortName.Compare(portName) == 0)
				{
					//Reopen Port
					oResult = pPort->ReopenPort();
					break;
				}
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::IsPortExistingInList(CFtd2xxDeviceInfo& p_rDeviceInfo, tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpExistingDeviceInfo)
{
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->IsEqual(p_rDeviceInfo))
		{
			p_rpExistingDeviceInfo = pDeviceInfo;
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::FindNonPluggedInDeviceInfoInList(tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpNonPluggedInDeviceInfo)
{
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(!pDeviceInfo->IsPluggedIn())
		{
			p_rpNonPluggedInDeviceInfo = pDeviceInfo;
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::FindDeviceInfo(CFtd2xxDeviceInfo& p_rDeviceInfo, tDeviceInfoList& p_rDeviceInfoList, int& p_rIndexFound)
{
	int iIndex = 0;
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
        CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->IsLocIdEqual(p_rDeviceInfo))
		{
			p_rIndexFound = iIndex;
			oResult = TRUE;
			break;
		}
		iIndex++;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::FindDeviceInfoByLocId(CFtd2xxDeviceInfo& p_rDeviceInfo, tDeviceInfoList& p_rDeviceInfoList, int& p_rIndexFound)
{
	int iIndex = 0;
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->IsLocIdEqual(p_rDeviceInfo))
		{
			p_rIndexFound = iIndex;
			oResult = TRUE;
			break;
		}
		iIndex++;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::FindDeviceInfoByLocId(CFtd2xxDeviceInfo& p_rDeviceInfo, tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpFoundDeviceInfo)
{
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->IsLocIdEqual(p_rDeviceInfo))
		{
			p_rpFoundDeviceInfo = pDeviceInfo;
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::FindDeviceInfoBySerialNumber(CFtd2xxDeviceInfo& p_rDeviceInfo, tDeviceInfoList& p_rDeviceInfoList, int& p_rIndexFound)
{
	int iIndex = 0;
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
			CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->IsSerialNumberEqual(p_rDeviceInfo))
		{
			p_rIndexFound = iIndex;
			oResult = TRUE;
			break;
		}
		iIndex++;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::FindDeviceInfoBySerialNumber(CFtd2xxDeviceInfo& p_rDeviceInfo, tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpFoundDeviceInfo)
{
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->IsSerialNumberEqual(p_rDeviceInfo))
		{
			p_rpFoundDeviceInfo = pDeviceInfo;
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::FindDeviceInfoByPortName(CStdString p_PortName, tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpDeviceInfo)
{
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->IsPortNameEqual(p_PortName))
		{
			p_rpDeviceInfo = pDeviceInfo;
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::AddToDeviceInfoList(CFtd2xxDeviceInfo& p_rDeviceInfo)
{
	CFtd2xxDeviceInfo* pDeviceInfo = 0;
	CStdString portName = _T("");
	BOOL oResult = FALSE;

	if(GetNextFreePortName(portName))
	{
		pDeviceInfo = new CFtd2xxDeviceInfo();
		pDeviceInfo->Init(p_rDeviceInfo);
		pDeviceInfo->SetPortName(portName);
		pDeviceInfo->SetPluggedIn(p_rDeviceInfo.GetPluggedIn());
		pDeviceInfo->SetOpenedByProcess(p_rDeviceInfo.GetOpenedByProcess());
		pDeviceInfo->SetOpenedByInstance(p_rDeviceInfo.GetOpenedByInstance());
		m_DeviceInfoList.push_back(pDeviceInfo);
		oResult = TRUE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::GetNextFreePortName(CStdString& p_rPortName)
{
	int lPortIndex = 0;
	char szPortName[32];
	BOOL oResult = FALSE;

	memset(szPortName, 0, 32);

	while(!oResult)
	{
		sprintf(szPortName, "USB%i", lPortIndex);

		if(!IsPortNameUsed(szPortName, m_DeviceInfoList))
		{
			p_rPortName = szPortName;
			oResult = TRUE;
		}
		lPortIndex++;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::IsPortNameUsed(CStdString p_PortName, tDeviceInfoList& p_rDeviceInfoList)
{
	CStdString portName = _T("");
	BOOL oResult = FALSE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo->GetPortName(portName))
		{
			if(portName.CompareNoCase(p_PortName) == 0)
			{
				oResult = TRUE;
				break;
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::DeleteFromDeviceInfoList(CFtd2xxDeviceInfo& p_rDeviceInfo)
{
	int iIndex = 0;
	BOOL oResult = TRUE;

	//Find By LocId and SerialNumber
	if(oResult && !FindDeviceInfo(p_rDeviceInfo, m_DeviceInfoList, iIndex)) oResult = FALSE;

	//Remove DeviceInfo from LockedDeviceInfoList
	if(oResult)
	{
		int index = 0;
		for(tDeviceInfoList::iterator it = m_DeviceInfoList.begin(); it != m_DeviceInfoList.end(); it++)
		{
			if(index++ == iIndex)
			{
				CFtd2xxDeviceInfo* pDeviceInfo = (*it);
				delete pDeviceInfo;

				m_DeviceInfoList.erase(it);
				break;
			}
		}
		oResult = FALSE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::SortDeviceInfoListByPortName(tDeviceInfoList& p_rDeviceInfoList)
{
	size_t n = 0;
	CFtd2xxDeviceInfo deviceInfo;
	CFtd2xxDeviceInfo* pDeviceInfo = 0;
	CFtd2xxDeviceInfo* pNextDeviceInfo = 0;
	BOOL oExchanged = FALSE;
	BOOL oResult = TRUE;

	if(p_rDeviceInfoList.size() > 0)
	{
		//Bubble Sort Algorithm
		n = p_rDeviceInfoList.size() - 1;
		do
		{
			oExchanged = FALSE;
			for(int i = 0; i < n; i++)
			{
				int index = 0;
				for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
				{
					if(index == i)
						pDeviceInfo = (*it);

					if(index == i+1)
					{
						pNextDeviceInfo = (*it);
						break;
					}

					index++;
				}

				//Compare DeviceInfos
				if(pDeviceInfo && pNextDeviceInfo)
				{
					//Compare PortName
					if(pDeviceInfo->GetPortName() > pNextDeviceInfo->GetPortName())
					{
						//Interchange Values
						deviceInfo = *pDeviceInfo;
						*pDeviceInfo = *pNextDeviceInfo;
						*pNextDeviceInfo = deviceInfo;

						oExchanged = TRUE;
					}
				}
			}
			n = n-1;
		}
		while(oExchanged);
	}
	else
	{
		oResult = TRUE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::SortDeviceInfoListByLocId(tDeviceInfoList& p_rDeviceInfoList)
{
	size_t n = 0;
	CFtd2xxDeviceInfo deviceInfo;
	CFtd2xxDeviceInfo* pDeviceInfo = 0;
	CFtd2xxDeviceInfo* pNextDeviceInfo = 0;
	BOOL oExchanged = FALSE;
	BOOL oResult = TRUE;

	//Bubble Sort Algorithm
	n = p_rDeviceInfoList.size() - 1;
	do
	{
		oExchanged = FALSE;
		for(int i = 0; i < n; i++)
		{
			int index = 0;
			//FindIndex
			for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
			{
				if(index == i)
					pDeviceInfo = (*it);

				if(index == i+1)
				{
					pNextDeviceInfo = (*it);
					break;
				}

				index++;
			}
			//Compare DeviceInfos
			if(pDeviceInfo && pNextDeviceInfo)
			{
				//Compare LocId
				if(pDeviceInfo->GetLocId() > pNextDeviceInfo->GetLocId())
				{
					//Interchange Values
					deviceInfo = *pDeviceInfo;
					*pDeviceInfo = *pNextDeviceInfo;
					*pNextDeviceInfo = deviceInfo;

					oExchanged = TRUE;
				}
			}
		}
		n = n-1;
	}
	while(oExchanged);

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::SortPortNames(CStdStringArray& p_rPortNames)
{
	size_t n = 0;
	CStdString portName = _T("");
	CStdString nextPortName = _T("");
	BOOL oExchanged = FALSE;
	BOOL oResult = TRUE;

	//Bubble Sort Algorithm
	n = p_rPortNames.size()-1;

	do
	{
		oExchanged = FALSE;
		for(int i = 0; i < n; i++)
		{
			//DeviceInfo
			portName = p_rPortNames.at(i);

			//Next DeviceInfo
			nextPortName = p_rPortNames.at(i+1);

			//Compare PortName
			if(portName > nextPortName)
			{
				//Interchange Values
				p_rPortNames[i]=nextPortName;
				p_rPortNames[i+1]=portName;
				oExchanged = TRUE;
			}
		}
		n = n-1;
	}
	while(oExchanged);

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::CopyDeviceInfoList(tDeviceInfoList& p_rSourceDeviceInfoList, tDeviceInfoList& p_rTargetDeviceInfoList)
{
	BOOL oResult = FALSE;

	if(DeleteDeviceInfoList(p_rTargetDeviceInfoList))
	{
		oResult = TRUE;
		for(tDeviceInfoList::iterator it = p_rSourceDeviceInfoList.begin(); it != p_rSourceDeviceInfoList.end(); it++)
		{
			CFtd2xxDeviceInfo* pDeviceInfo = (*it);
			if(pDeviceInfo != 0)
			{
				CFtd2xxDeviceInfo* pClonedDeviceInfo = pDeviceInfo->Clone();
				if(pClonedDeviceInfo)
					p_rTargetDeviceInfoList.push_back(pClonedDeviceInfo);
			}
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::DeleteDeviceInfoList(tDeviceInfoList& p_rDeviceInfoList)
{
	BOOL oResult = TRUE;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo!=0) delete pDeviceInfo;
	}

	p_rDeviceInfoList.clear();
	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::AddDeviceInfo(CStdString p_PortName, DWORD p_ulLocId, CStdString p_SerialNumber)
{
	CFtd2xxDeviceInfo* pDeviceInfo = 0;
	BOOL oResult = TRUE;

	pDeviceInfo = new CFtd2xxDeviceInfo();
	pDeviceInfo->Init(p_ulLocId, p_SerialNumber, 0);
	pDeviceInfo->SetPortName(p_PortName);

	m_DeviceInfoList.push_back(pDeviceInfo);

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::ResetDeviceInfoList()
{
	BOOL oResult = FALSE;

	oResult = DeleteDeviceInfoList(m_DeviceInfoList);

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::DeleteFromRegistry()
{
	CStdString subKey = _T("");

	//Init
	GetRegistryKey(subKey);

	CMmcUserSettings userSettings(subKey, FALSE);

	//Delete
	return userSettings.Delete(subKey);
}

BOOL CFtd2xxDeviceInfoHandling::WriteToRegistry()
{
	BOOL oResult = FALSE;
	CStdString subKey = _T("");
	long entryIndex = 0;
	CStdString entryName = "";
	BOOL oLocked = FALSE;

	//Init
	DeleteFromRegistry();

	GetRegistryKey(subKey);

	CMmcUserSettings userSettings(subKey, TRUE);

	//Init
	oResult = TRUE;
	entryIndex = 0;

	//DeviceInfo List
	for(tDeviceInfoList::iterator it = m_DeviceInfoList.begin(); it != m_DeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);

		entryName.Format(_T("DeviceInfo%i"), entryIndex++);

		oLocked = FALSE;

		oResult = WriteDeviceInfoToRegistry(&userSettings, entryName, pDeviceInfo);
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::ReadFromRegistry()
{
	bool oResult = true;
	CStdString subKey = _T("");
	CFtd2xxDeviceInfo* pDeviceInfo = 0;
	BOOL oLocked = FALSE;

    //Init
	GetRegistryKey(subKey);

	CMmcUserSettings userSettings(subKey, FALSE);

    //Clear list
    ResetDeviceInfoList();

	std::list<CMmcUserSettings*> children = userSettings.GetChildren();

	for(std::list<CMmcUserSettings*>::iterator it = children.begin(); it != children.end(); it++)
	{
		CMmcUserSettings* pChild = (*it);
		CFtd2xxDeviceInfo* pDeviceInfo = new CFtd2xxDeviceInfo();

		if(ReadDeviceInfoFromRegistry(pChild, pDeviceInfo))
		{
			//Device Info List
			m_DeviceInfoList.push_back(pDeviceInfo);
		}
		else
		{
			//Delete DeviceInfo
			delete pDeviceInfo;
			oResult = false;
			break;
		}

		delete pChild;
	}

    return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::UpdateRegistry(CStdString p_PortName, BOOL p_oOpened)
{
	BOOL oResult = FALSE;
	CStdString subKey = _T("");
	CFtd2xxDeviceInfo deviceInfo;

	//Init
	GetRegistryKey(subKey);

	CMmcUserSettings userSettings(subKey, FALSE);

	std::list<CMmcUserSettings*> children = userSettings.GetChildren();

	for(std::list<CMmcUserSettings*>::iterator it = children.begin(); it != children.end(); it++)
	{
		CMmcUserSettings* pChild = (*it);
		if(ReadDeviceInfoFromRegistry(pChild, &deviceInfo))
		{
			if(deviceInfo.GetPortName() == p_PortName)
			{
				//Update Opened Flag
				oResult = WriteOpenedToRegistry(pChild, p_oOpened);
			}
		}

		delete pChild;
	}

    return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::WriteDeviceInfoToRegistry(CMmcUserSettings *p_pUserSettings, CStdString p_ValueName, CFtd2xxDeviceInfo* p_pDeviceInfo)
{
	BOOL oResult = FALSE;
	CStdString portName = "";
	CStdString serialNumber = "";
	DWORD locationId = 0;
	DWORD pluggedIn = 0;
	DWORD openedByProcess = 0;
	DWORD openedByInstance = 0;
	CStdString openedBy;

	if(p_pDeviceInfo)
	{
		CMmcUserSettings userSettings(p_pUserSettings, p_ValueName, TRUE);

		//Init
		oResult = TRUE;
		p_pDeviceInfo->GetPortName(portName);
		serialNumber = p_pDeviceInfo->GetSerialNumber();
		locationId = p_pDeviceInfo->GetLocId();
		pluggedIn = p_pDeviceInfo->GetPluggedIn();
		openedByProcess = p_pDeviceInfo->GetOpenedByProcess();
		openedByInstance = p_pDeviceInfo->GetOpenedByInstance();
		openedBy = p_pDeviceInfo->GetOpenedBy();

		//Write
		if(oResult && !userSettings.Write("", portName)) oResult = FALSE;
		if(oResult && !userSettings.Write("SerialNumber", serialNumber)) oResult = FALSE;
		if(oResult && !userSettings.Write("LocationId", locationId)) oResult = FALSE;
		if(oResult && !userSettings.Write("PluggedIn", pluggedIn)) oResult = FALSE;
		if(oResult && !userSettings.Write("OpenedByProcess", openedByProcess)) oResult = FALSE;
		if(oResult && !userSettings.Write("OpenedByInstance", openedByInstance)) oResult = FALSE;
		if(oResult && !userSettings.Write("OpenedBy", openedBy)) oResult = FALSE;
	}
	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::WriteOpenedToRegistry(CMmcUserSettings *p_pUserSettings, BOOL p_oOpened)
{
	BOOL oResult = FALSE;

	if(p_pUserSettings != 0)
	{
		DWORD openedByProcess = 0;
		DWORD openedByInstance = 0;
		CStdString openedBy = "";

		//Init
		oResult = TRUE;

		//Open Key
		if(p_oOpened)
		{
			openedByProcess = MmcGetCurrentProcessId();
			openedByInstance = m_lInstanceValue;
			openedBy = MmcGetModuleName();
		}
		else
		{
			openedByProcess = 0;
			openedByInstance = 0;
			openedBy = "";
		}

		//Write
		if(oResult && !p_pUserSettings->Write("OpenedByProcess", openedByProcess)) oResult = FALSE;
		if(oResult && !p_pUserSettings->Write("OpenedByInstance", openedByInstance)) oResult = FALSE;
		if(oResult && !p_pUserSettings->Write("OpenedBy", openedBy)) oResult = FALSE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::ReadDeviceInfoFromRegistry(CMmcUserSettings *p_pUserSettings, CFtd2xxDeviceInfo* p_pDeviceInfo)
{
	BOOL oResult = FALSE;

    if(p_pDeviceInfo && p_pUserSettings)
	{
		//Read
		CStdString portName = p_pUserSettings->ReadString("");
		CStdString serialNumber = p_pUserSettings->ReadString("SerialNumber");
		CStdString openedBy = p_pUserSettings->ReadString("OpenedBy");

		DWORD locationId = p_pUserSettings->ReadULONG("LocationId");
		DWORD pluggedIn = p_pUserSettings->ReadULONG("PluggedIn");
		DWORD openedByProcess = p_pUserSettings->ReadULONG("OpenedByProcess");
		DWORD openedByInstance = p_pUserSettings->ReadULONG("OpenedByInstance");

		//Return Values
		p_pDeviceInfo->Init(locationId, serialNumber, 0);
		p_pDeviceInfo->SetPortName(portName);
		p_pDeviceInfo->SetPluggedIn(pluggedIn);
		p_pDeviceInfo->SetOpenedByProcess(openedByProcess);
		p_pDeviceInfo->SetOpenedByInstance(openedByInstance);
		p_pDeviceInfo->SetOpenedBy(openedBy);

		oResult = TRUE;
	}

    return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::GetRegistryKey(CStdString& p_rRegistryKey)
{
	CStdString subKey = _T("");
	BOOL oResult = FALSE;

	//Init
	subKey.Format(_T("%s\\%s"), REGISTRY_MAXON, REGISTRY_FTDI_USB_HANDLING);

	p_rRegistryKey = subKey;
	oResult = TRUE;

	return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::GetApplicationInfo(CStdString& p_rApplicationName, CStdString& p_rVersion)
{
	BOOL oResult = FALSE;
#ifdef WINVER
    const DWORD MAX_SIZE = 512;

    TCHAR fileName[MAX_SIZE];

	if(GetModuleFileName(0, fileName, MAX_SIZE) > 0)
    {
		p_rVersion = CMmcFileInfo::GetFileVersion(fileName);
		p_rApplicationName = ExtractApplicationName(fileName);
        oResult = TRUE;
    }
#endif
    return oResult;
}

BOOL CFtd2xxDeviceInfoHandling::GetModuleVersion(CStdString& p_rVersion)
{
    CStdString fileName = MmcGetModuleName();

	if(!fileName.IsEmpty())
	{
		p_rVersion = CMmcFileInfo::GetFileVersion(fileName);
		return TRUE;
	}

    return FALSE;
}

CStdString CFtd2xxDeviceInfoHandling::ExtractApplicationName(CStdString p_FileName)
{
	int iIndexStart = 0;
	int iIndexEnd = 0;
	CStdString appName(_T(""));

	iIndexStart = p_FileName.ReverseFind('\\');
	iIndexEnd = p_FileName.ReverseFind('.');
	if((iIndexStart != -1) && (iIndexEnd != -1))
	{
		if(iIndexStart < iIndexEnd)
		{
			appName = p_FileName.Mid(iIndexStart, iIndexEnd - iIndexStart);
			appName = appName.TrimLeft('\\');
		}
	}

	return appName;
}

BOOL CFtd2xxDeviceInfoHandling::WriteToLogFile(CStdString p_Action, tDeviceInfoList& p_rDeviceInfoList)
{
	CStdString infos;

	for(tDeviceInfoList::iterator it = p_rDeviceInfoList.begin(); it != p_rDeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo)
		{
			//Init
			CStdString portName;
			CStdString info;
			pDeviceInfo->GetPortName(portName);

			//Format
			infos.Format(_T("%s LocId %.8X SerNb %s Flags %.8X; "), portName, pDeviceInfo->GetLocId(), pDeviceInfo->GetSerialNumber(), pDeviceInfo->GetDrvFlags());
			infos += info;
		}
	}

	return CMmcLogger::Commit(m_lInstanceValue, p_Action, infos);
}

BOOL CFtd2xxDeviceInfoHandling::WriteToLogFile(CStdString p_Action)
{
	CStdString infos;

	//DeviceInfos
	for(tDeviceInfoList::iterator it = m_DeviceInfoList.begin();
			it != m_DeviceInfoList.end(); it++)
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
		if(pDeviceInfo)
		{
			//Init
			CStdString portName;
			CStdString info;
			pDeviceInfo->GetPortName(portName);

			//Format
			info.Format(_T("%s LocId %.8X SerNb %s PluggedIn %i OpenedByProcess %i OpenedByInstance %i OpenedBy %s; "), portName, pDeviceInfo->GetLocId(), pDeviceInfo->GetSerialNumber(), pDeviceInfo->IsPluggedIn(), pDeviceInfo->GetOpenedByProcess(), pDeviceInfo->GetOpenedByInstance(), pDeviceInfo->GetOpenedBy());
			infos += info;
		}
	}

	return CMmcLogger::Commit(m_lInstanceValue, p_Action, infos);
}

BOOL CFtd2xxDeviceInfoHandling::WriteToLogFile(CStdString p_Action, CStdString p_PortName, DWORD p_ulLocId)
{
	CStdString infos;

	//Infos
	infos.Format(_T("Port %s, LocId %.8X"), p_PortName, p_ulLocId);

	return CMmcLogger::Commit(m_lInstanceValue, p_Action, infos);
}

BOOL CFtd2xxDeviceInfoHandling::WriteToLogFile(CStdString p_Action, CStdString p_PortName)
{
	CStdString infos;

	//Infos
	infos.Format(_T("Port %s"), p_PortName);

	return CMmcLogger::Commit(m_lInstanceValue, p_Action, infos);
}

