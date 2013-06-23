// DeviceBase.cpp: Implementierung der Klasse CDeviceBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <algorithm>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>
#include "../../Gateway/BaseClasses/GatewayDCStoPS.h"
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>

#include "DeviceBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CDeviceBase::CDeviceBase()
{
	m_lInstanceValue = 0;
	m_strDeviceName = "";
	m_pJournalManager = 0;
	m_pGateway = 0;

	m_pErrorHandling = 0;
	InitErrorHandling();
}

CDeviceBase::CDeviceBase(const CDeviceBase& rObject)
{
	//Instance Value
	m_lInstanceValue = rObject.m_lInstanceValue;

	//Devicename kopieren
	m_strDeviceName = rObject.m_strDeviceName;

	//JournalManager
	m_pJournalManager = rObject.m_pJournalManager;

	//Gateway
	if(rObject.m_pGateway) m_pGateway = (CGatewayDCStoPS*)rObject.m_pGateway->Clone(); else m_pGateway = 0;

	m_pErrorHandling = 0;
	InitErrorHandling();
}

CDeviceBase* CDeviceBase::Clone()
{
	return 0;
}

BOOL CDeviceBase::IsEqual(CDeviceBase* p_pDevice)
{
	BOOL oResult = FALSE;

	if(p_pDevice)
	{
		oResult = TRUE;
		if(oResult && (m_strDeviceName != p_pDevice->m_strDeviceName)) oResult = FALSE;
		if(oResult && (m_lInstanceValue != p_pDevice->m_lInstanceValue)) oResult = FALSE;
	}

	return oResult;
}

CDeviceBase::~CDeviceBase()
{
	DeleteErrorHandling();
	DeleteGateway();
	DeletePrototcolStackManagerList();
}

BOOL CDeviceBase::InitInstanceValue(int p_lInstanceValue)
{
	BOOL oResult = TRUE;

	m_lInstanceValue = p_lInstanceValue;

	return oResult;
}

BOOL CDeviceBase::InitErrorHandling()
{
	DeleteErrorHandling();
	m_pErrorHandling = new CErrorHandling();
	return TRUE;
}

BOOL CDeviceBase::DeleteErrorHandling()
{
	if(m_pErrorHandling)
	{
		delete m_pErrorHandling;
		m_pErrorHandling = 0;
	}

	return TRUE;
}

BOOL CDeviceBase::InitDevice(CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CDeviceBase::InitDevice(CStdString strProtocolStack, CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CDeviceBase::InitDevice(CStdString strProtocolStack, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CDeviceBase::InitGateway(CStdString strProtocolStackName)
{
	return FALSE;
}

BOOL CDeviceBase::InitBaudrateSelection(CStdString strProtocolStack, CStdString strInterfaceName, CStdDWordArray& dBaudrateSel)
{
	BOOL oResult = TRUE;

	for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
	{
		CProtocolStackManagerBase* pProtocolStackManager = (*it);
		if(pProtocolStackManager)
		{
			if(!pProtocolStackManager->PS_InitBaudrateSelection(strProtocolStack, strInterfaceName, dBaudrateSel))
			{
				oResult = FALSE;
			}
		}
	}

	return oResult;
}

BOOL CDeviceBase::InitDefaultProtocolStackSettings(CStdString strProtocolStack, CStdString strInterfaceName, DWORD dBaudrate, DWORD dTimeout)
{
	BOOL oResult = TRUE;

	for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
	{
		CProtocolStackManagerBase* pProtocolStackManager = (*it);
		if(pProtocolStackManager)
		{
			if(!pProtocolStackManager->PS_InitDefaultProtocolStackSettings(strProtocolStack, strInterfaceName, dBaudrate, dTimeout))
			{
				oResult = FALSE;
			}
		}
	}

	return oResult;
}

BOOL CDeviceBase::VerifyGateway(CStdString strProtocolStackName)
{
	return FALSE;
}

BOOL CDeviceBase::DCS_GetDeviceName(CStdString* pDeviceName)
{
	if(pDeviceName)
	{
		*pDeviceName = m_strDeviceName;
		return TRUE;
	}

	return FALSE;
}

void CDeviceBase::DeletePrototcolStackManagerList()
{
	for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
	{
		CProtocolStackManagerBase* pProtocolStackManager = (*it);
		delete pProtocolStackManager;
		pProtocolStackManager = 0;
	}
	m_ProtocolStackManagerList.clear();
}

void CDeviceBase::DeleteGateway()
{
	if(m_pGateway)
	{
		delete m_pGateway;
		m_pGateway = 0;
	}
}

BOOL CDeviceBase::AddManager(CProtocolStackManagerBase* pManager)
{
	BOOL oResult = FALSE;

	if(pManager)
	{
		m_ProtocolStackManagerList.push_back(pManager);
		oResult = TRUE;
	}

	return oResult;
}

BOOL CDeviceBase::GetFirstManager(CProtocolStackManagerBase** ppManager)
{
	BOOL oResult = FALSE;

	std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin();
	if(it != m_ProtocolStackManagerList.end())
	{
		CProtocolStackManagerBase* pListManager = (*it);
		if(pListManager)
		{
			if(ppManager) *ppManager = pListManager;
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CDeviceBase::FindCorrectManager(CStdString strProtocolStackName, CProtocolStackManagerBase** ppManager)
{
	if(ppManager)
	{
		for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
		{
			CProtocolStackManagerBase* pListManager = (*it);
			if(pListManager)
			{
				if(pListManager->PS_IsProtocolStackNameSupported(strProtocolStackName))
				{
					*ppManager = pListManager;
					return TRUE;
				}
			}
		}

		return FindCorrectLibrary(strProtocolStackName, ppManager);
	}

	return FALSE;
}

BOOL CDeviceBase::FindCorrectManager(HANDLE hPS_BaseLayerHandle, CProtocolStackManagerBase** ppManager)
{
	CStdString protocolStackName = _T("");

	if(ppManager)
	{
		for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
		{
			CProtocolStackManagerBase* pListManager = (*it);
			if(pListManager)
			{
				if(pListManager->PS_GetProtocolStackName(hPS_BaseLayerHandle, &protocolStackName))
				{
					if(pListManager->PS_IsProtocolStackNameSupported(protocolStackName))
					{
						*ppManager = pListManager;
						return TRUE;
					}
				}
			}
		}

		return FindCorrectLibrary(hPS_BaseLayerHandle, ppManager);
	}

	return FALSE;
}

BOOL CDeviceBase::FindCorrectLibrary(CStdString strProtocolStackName, CProtocolStackManagerBase** ppManager)
{
	if(ppManager)
	{
		*ppManager = 0;
		return FALSE;
	}

	return FALSE;
}

BOOL CDeviceBase::FindCorrectLibrary(HANDLE hPS_BaseLayerHandle, CProtocolStackManagerBase** ppManager)
{
	if(ppManager)
	{
		*ppManager = 0;
		return FALSE;
	}

	return FALSE;
}

BOOL CDeviceBase::DCS_GetProtocolStackNameSelection(CStdStringArray* pProtocolStackNameSel, CErrorInfo* pErrorInfo)
{
	CProtocolStackManagerBase* pManager = 0;
	CStdStringArray selArray;

	if(pProtocolStackNameSel)
	{
		pProtocolStackNameSel->clear();

		for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
		{
			CProtocolStackManagerBase* pManager = (*it);
			if(pManager)
			{
				if(pManager->PS_GetProtocolStackNameSelection(&selArray))
				{
					AppendSelectionArray(pProtocolStackNameSel, &selArray);
				}
			}
		}
		return TRUE;
	}

	if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
	return FALSE;
}

BOOL CDeviceBase::AppendSelectionArray(CStdStringArray* pDestArray, CStdStringArray* pSourceArray)
{
	CStdString strDestName;
	CStdString strSourceName;
	BOOL oFound;

	if(pDestArray && pSourceArray)
	{
		for(std::size_t i=0;i<pSourceArray->size();i++)
		{
			oFound = FALSE;
			strSourceName = pSourceArray->at(i);
			for(std::size_t j=0;j<pDestArray->size();j++)
			{
				strDestName = pDestArray->at(j);

				if(strSourceName.CompareNoCase(strDestName) == 0)
				{
					oFound = TRUE;
					break;
				}
			}

			if(!oFound)
			{
				pDestArray->push_back(strSourceName);
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CDeviceBase::DCS_GetInterfaceNameSelection(CStdString strProtocolStackName, CStdStringArray* pInterfaceNameSel, CErrorInfo* pErrorInfo)
{
	CProtocolStackManagerBase* pManager = 0;

	//Find Manager
	if(!FindCorrectManager(strProtocolStackName, &pManager) || (pManager == 0))
	{
		if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, pErrorInfo);
		return FALSE;
	}

	//GetInterfaceNameSelection
	if(!pManager->PS_GetInterfaceNameSelection(strProtocolStackName, pInterfaceNameSel, pErrorInfo))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDeviceBase::DCS_GetPortNameSelection(CStdString strProtocolStackName, CStdString strInterfaceName, CStdStringArray* pPortSel, CErrorInfo* pErrorInfo)
{
	CProtocolStackManagerBase* pManager = 0;

	//Find Manager
	if(!FindCorrectManager(strProtocolStackName, &pManager) || (pManager == 0))
	{
		if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, pErrorInfo);
		return FALSE;
	}

	//GetPortNameSelection
	if(!pManager->PS_GetPortNameSelection(strProtocolStackName, strInterfaceName, pPortSel, pErrorInfo))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDeviceBase::DCS_GetBaudrateSelection(CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* pErrorInfo)
{
	CProtocolStackManagerBase* pManager = 0;

	//Find Manager
	if(!FindCorrectManager(strProtocolStackName, &pManager) || (pManager == 0))
	{
		if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, pErrorInfo);
		return FALSE;
	}

	//GetBaudrateSelection
	if(!pManager->PS_GetBaudrateSelection(strProtocolStackName, strInterfaceName, strPortName, pdBaudrateSel, pErrorInfo))
	{
		return FALSE;
	}

	return TRUE;
}

void CDeviceBase::InitJournalManager(CJournalManagerBase* pJournalManager)
{
	CProtocolStackManagerBase* pProtocol = 0;

	m_pJournalManager = pJournalManager;

	for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
	{
		CProtocolStackManagerBase* pManager = (*it);
		pManager->InitJournalManager(pJournalManager);
	}
}

void CDeviceBase::ResetJournalManager()
{
	m_pJournalManager = 0;

	for(std::list<CProtocolStackManagerBase*>::iterator it = m_ProtocolStackManagerList.begin(); it != m_ProtocolStackManagerList.end(); it++)
	{
		CProtocolStackManagerBase* pManager = (*it);
		pManager->ResetJournalManager();
	}
}

BOOL CDeviceBase::ExecuteCommand(CCommandRoot*pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
	BOOL oResult;

	if(pCommand)
	{
		//Init References
		pCommand->InitGateway(m_pGateway);
		pCommand->InitJournalManager(m_pJournalManager);

		//Execute
		oResult = pCommand->Execute(pProtocolStackManager, hPS_Handle, hTransactionHandle);

		//Reset References
		pCommand->ResetGateway();
		pCommand->ResetJournalManager();

		return oResult;
	}

	return FALSE;
}

BOOL CDeviceBase::AbortCommands(BOOL oActive)
{
	if(m_pGateway)
	{
		return m_pGateway->AbortCommands(oActive);
	}

	return TRUE;
}

BOOL CDeviceBase::GetCommands(CStdString* pCommandInfo)
{
	return FALSE;
}

BOOL CDeviceBase::AreParameterEqual(CDeviceBase* p_pDevice)
{
	BOOL oResult = FALSE;

	if(p_pDevice && m_pGateway)
	{
		oResult = m_pGateway->AreParameterEqual(p_pDevice->m_pGateway);
	}

	return oResult;
}

BOOL CDeviceBase::SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->SetParameter(p_Name, p_pValue, p_ulSize);
	}

	return oResult;
}

BOOL CDeviceBase::SetParameter(CStdString p_Name, CStdString p_Value)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->SetParameter(p_Name, p_Value);
	}

	return oResult;
}

BOOL CDeviceBase::GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->GetParameter(p_Name, p_pValue, p_ulSize);
	}

	return oResult;
}

BOOL CDeviceBase::GetParameter(CStdString p_Name, CStdString& p_rValue)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->GetParameter(p_Name, p_rValue);
	}

	return oResult;
}

BOOL CDeviceBase::Lock(DWORD p_ulTimeout)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->Lock(p_ulTimeout);
	}

	return oResult;
}

BOOL CDeviceBase::Unlock()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->Unlock();
	}

	return oResult;
}

BOOL CDeviceBase::IsLocked()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->IsLocked();
	}

	return oResult;
}

BOOL CDeviceBase::InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName)
{
	CStdStringArray interfaceNameSel;
	CStdString interfaceName(_T(""));
	BOOL oResult(FALSE);

	if(p_pProtocolStackManager)
	{
		if(p_pProtocolStackManager->PS_GetInterfaceNameSelection(p_ProtocolStackName, &interfaceNameSel))
		{
			//Init
			oResult = TRUE;
			for(CStdStringArray::iterator it = interfaceNameSel.begin(); it != interfaceNameSel.end(); it++)
			{
				interfaceName = (*it);
				if(!InitDefaultProtocolStackSettings(p_pProtocolStackManager, p_ProtocolStackName, interfaceName)) oResult = FALSE;
			}
		}
	}

	return oResult;
}

BOOL CDeviceBase::InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName)
{
	//No Initialisation
	return FALSE;
}