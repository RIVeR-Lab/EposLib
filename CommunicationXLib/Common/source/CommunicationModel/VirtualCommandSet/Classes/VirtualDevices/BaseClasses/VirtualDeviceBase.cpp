// VirtualDeviceBase.cpp: Implementierung der Klasse CVirtualDeviceBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <algorithm>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include "../../Gateway/BaseClasses/GatewayVCStoDCS.h"
#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include "../../../../DeviceCommandSet/DeviceCommandSetManager.h"
#include "VirtualDeviceBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CVirtualDeviceBase::CVirtualDeviceBase()
{
    m_lInstanceValue = 0;
	m_strVirtualDeviceName = "";
    m_pGateway = 0;
    m_pJournalManager = 0;

    m_pErrorHandling = 0;
    InitErrorHandling();
}

CVirtualDeviceBase::CVirtualDeviceBase(const CVirtualDeviceBase& rObject)
{
    //Instance Value
	m_lInstanceValue = rObject.m_lInstanceValue;

	//Devicename kopieren
    m_strVirtualDeviceName = rObject.m_strVirtualDeviceName;

    //Gateway
    if(rObject.m_pGateway) m_pGateway = (CGatewayVCStoDCS*)rObject.m_pGateway->Clone(); else m_pGateway = 0;

    //JournalManager
    m_pJournalManager = rObject.m_pJournalManager;

    m_pErrorHandling = 0;
    InitErrorHandling();
}

BOOL CVirtualDeviceBase::InitInstanceValue(int p_lInstanceValue)
{
	BOOL oResult = TRUE;

	m_lInstanceValue = p_lInstanceValue;

	return oResult;
}

BOOL CVirtualDeviceBase::InitErrorHandling()
{
    DeleteErrorHandling();
    m_pErrorHandling = new CErrorHandling();
    return TRUE;
}

BOOL CVirtualDeviceBase::DeleteErrorHandling()
{
    if(m_pErrorHandling)
    {
        delete m_pErrorHandling;
        m_pErrorHandling = 0;
    }

    return TRUE;
}

CVirtualDeviceBase::~CVirtualDeviceBase()
{
    DeleteErrorHandling();
    DeleteGateway();
    DeleteDCS_ManagerList();
}

BOOL CVirtualDeviceBase::InitVirtualDevice(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualDeviceBase::InitVirtualDevice(CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualDeviceBase::VCS_GetVirtualDeviceName(CStdString* pVirtualDeviceName)
{
    if(pVirtualDeviceName)
    {
        *pVirtualDeviceName = m_strVirtualDeviceName;
        return TRUE;
    }

    return FALSE;
}

void CVirtualDeviceBase::DeleteDCS_ManagerList()
{
    CDeviceCommandSetManagerBase* pManager = 0;
    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
    {
        pManager = (*it);
        delete pManager;
    }
    m_DeviceCommandSetManagerList.clear();
}

void CVirtualDeviceBase::DeleteGateway()
{
    if(m_pGateway)
    {
        delete m_pGateway;
        m_pGateway = 0;
    }
}

BOOL CVirtualDeviceBase::AddManager(CDeviceCommandSetManagerBase* pManager)
{
    BOOL oResult = FALSE;

    if(pManager)
    {
        m_DeviceCommandSetManagerList.push_back(pManager);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CVirtualDeviceBase::GetFirstManager(CDeviceCommandSetManagerBase** ppManager)
{
    CDeviceCommandSetManagerBase* pListManager = 0;
    BOOL oResult = FALSE;

    std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin();

    if(it != m_DeviceCommandSetManagerList.end())
    {
    	pListManager = (*it);
		if(pListManager)
		{
			if(ppManager) *ppManager = pListManager;
			oResult = TRUE;
		}
    }

    return oResult;
}

BOOL CVirtualDeviceBase::FindCorrectManager(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager)
{
    if(ppManager)
    {
        for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
        {
        	CDeviceCommandSetManager* pListManager = (CDeviceCommandSetManager*)(*it);
            if(pListManager)
            {
                if(pListManager->DCS_IsDeviceNameSupported(strDeviceName))
                {
                    if(ppManager) *ppManager = pListManager;
                    return TRUE;
                }
            }
        }

        return FindCorrectLibrary(strDeviceName, ppManager);
    }

    return FALSE;
}

BOOL CVirtualDeviceBase::FindCorrectLibrary(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager)
{
    if(ppManager)
    {
        *ppManager = 0;
        return FALSE;
    }

    return FALSE;
}

BOOL CVirtualDeviceBase::VCS_GetDeviceNameSelection(CStdStringArray* pDeviceNameSel, CErrorInfo* pErrorInfo)
{
    CStdStringArray selArray;

    if(pDeviceNameSel)
    {
        pDeviceNameSel->clear();

        for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
		{
			CDeviceCommandSetManagerBase* pManager = (*it);
			if(pManager)
			{
				if(pManager->DCS_GetDeviceNameSelection(&selArray))
					AppendSelectionArray(pDeviceNameSel, &selArray);
			}
		}

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CVirtualDeviceBase::AppendSelectionArray(CStdStringArray* pDestArray, CStdStringArray* pSourceArray)
{
    CStdString strDestName;
    CStdString strSourceName;
    BOOL oFound;

    if(pDestArray && pSourceArray)
    {
    	for(CStdStringArray::iterator it = pSourceArray->begin(); it != pSourceArray->end(); it++)
        {
            oFound = FALSE;
            strSourceName = (*it);
            for(CStdStringArray::iterator it2 = pDestArray->begin(); it2 != pDestArray->end(); it2++)
			{
            	strDestName = (*it2);

            	if(strSourceName.CompareNoCase(strDestName) == 0)
				{
					oFound = TRUE;
					break;
				}
			}

            if(!oFound)
                pDestArray->push_back(strSourceName);
        }

        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualDeviceBase::VCS_GetProtocolStackNameSelection(CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(strDeviceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    //GetProtocolStackNameSelection
    if(!pManager->DCS_GetProtocolStackNameSelection(strDeviceName, pProtocolStackNameSel, pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CVirtualDeviceBase::VCS_GetInterfaceNameSelection(CStdString strDeviceName, CStdString strProtocolStackName, CStdStringArray* pInterfaceNameSel, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(strDeviceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    //GetInterfaceNameSelection
    if(!pManager->DCS_GetInterfaceNameSelection(strDeviceName, strProtocolStackName, pInterfaceNameSel, pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CVirtualDeviceBase::VCS_GetPortNameSelection(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdStringArray* pPortSel, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(strDeviceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    //GetPortNameSelection
    if(!pManager->DCS_GetPortNameSelection(strDeviceName, strProtocolStackName, strInterfaceName, pPortSel, pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CVirtualDeviceBase::VCS_GetBaudrateSelection(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(strDeviceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    if(!pManager->DCS_GetBaudrateSelection(strDeviceName, strProtocolStackName, strInterfaceName, strPortName, pdBaudrateSel, pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CVirtualDeviceBase::GetCommands(CStdString* pCommandInfo)
{
    return FALSE;
}

void CVirtualDeviceBase::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    m_pJournalManager = pJournalManager;

    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
	{
		pManager = (*it);
		pManager->InitJournalManager(pJournalManager);
	}
}

void CVirtualDeviceBase::ResetJournalManager()
{
    CDeviceCommandSetManagerBase* pManager = 0;

    m_pJournalManager = 0;

    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
	{
    	pManager = (*it);
    	pManager->ResetJournalManager();
	}
}

BOOL CVirtualDeviceBase::VerifyGateway(CStdString strVirtualDeviceName)
{
    return FALSE;
}

BOOL CVirtualDeviceBase::InitGateway(CStdString strVirtualDeviceName)
{
    return FALSE;
}

BOOL CVirtualDeviceBase::InitBaudrateSelection(CStdString strDeviceName, CStdString strProtocolStack, CStdString strInterfaceName, CStdDWordArray& dBaudrateSel)
{
    CDeviceCommandSetManagerBase* pDeviceCommandSetManager = 0;

    BOOL oResult = TRUE;

    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
	{
    	pDeviceCommandSetManager = (*it);
    	if(pDeviceCommandSetManager)
		{
			if(!pDeviceCommandSetManager->DCS_InitBaudrateSelection(strDeviceName, strProtocolStack, strInterfaceName, dBaudrateSel))
			{
				oResult = FALSE;
			}
		}
	}

    return oResult;
}

BOOL CVirtualDeviceBase::InitDefaultProtocolStackSettings(CStdString strDeviceName, CStdString strProtocolStack, CStdString strInterfaceName, DWORD dBaudrate, DWORD dTimeout)
{
    CDeviceCommandSetManagerBase* pDeviceCommandSetManager = 0;

    BOOL oResult = TRUE;

    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
   	{
       	pDeviceCommandSetManager = (*it);
       	if(pDeviceCommandSetManager)
		{
			if(!pDeviceCommandSetManager->DCS_InitDefaultProtocolStackSettings(strDeviceName, strProtocolStack, strInterfaceName, dBaudrate, dTimeout))
			{
				oResult = FALSE;
			}
		}
   	}

    return oResult;
}

CVirtualDeviceBase* CVirtualDeviceBase::Clone()
{
    return 0;
}

BOOL CVirtualDeviceBase::IsEqual(CVirtualDeviceBase* p_pVirtualDevice)
{
	BOOL oResult = FALSE;

	if(p_pVirtualDevice)
	{
		oResult = TRUE;
		if(oResult && (m_strVirtualDeviceName != p_pVirtualDevice->m_strVirtualDeviceName)) oResult = FALSE;
		if(oResult && (m_lInstanceValue != p_pVirtualDevice->m_lInstanceValue)) oResult = FALSE;
	}

	return oResult;
}

BOOL CVirtualDeviceBase::ExecuteCommand(CCommandRoot*pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle)
{
    BOOL oResult;

    if(pCommand)
    {
        //Init References
        pCommand->InitGateway(m_pGateway);
        pCommand->InitJournalManager(m_pJournalManager);

        //Execute
        oResult = pCommand->Execute(pDeviceCommandSetManager, hDCS_Handle, hTransactionHandle);

        //Reset References
        pCommand->ResetGateway();
        pCommand->ResetJournalManager();

        return oResult;
    }

    return FALSE;
}

BOOL CVirtualDeviceBase::AbortCommands(BOOL oActive)
{
    if(m_pGateway)
    {
        return m_pGateway->AbortCommands(oActive);
    }

    return TRUE;
}

BOOL CVirtualDeviceBase::AreParameterEqual(CVirtualDeviceBase* p_pVirtualDevice)
{
	BOOL oResult = FALSE;

	if(p_pVirtualDevice && m_pGateway)
	{
		oResult = m_pGateway->AreParameterEqual(p_pVirtualDevice->m_pGateway);
	}

	return oResult;
}

BOOL CVirtualDeviceBase::SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->SetParameter(p_Name, p_pValue, p_ulSize);
	}

	return oResult;
}

BOOL CVirtualDeviceBase::SetParameter(CStdString p_Name, CStdString p_Value)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->SetParameter(p_Name, p_Value);
	}

	return oResult;
}

BOOL CVirtualDeviceBase::GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->GetParameter(p_Name, p_pValue, p_ulSize);
	}

	return oResult;
}

BOOL CVirtualDeviceBase::GetParameter(CStdString p_Name, CStdString& p_rValue)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->GetParameter(p_Name, p_rValue);
	}

	return oResult;
}

BOOL CVirtualDeviceBase::Lock(DWORD p_ulTimeout)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->Lock(p_ulTimeout);
	}

	return oResult;
}

BOOL CVirtualDeviceBase::Unlock()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->Unlock();
	}

	return oResult;
}

BOOL CVirtualDeviceBase::IsLocked()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->IsLocked();
	}

	return oResult;
}
