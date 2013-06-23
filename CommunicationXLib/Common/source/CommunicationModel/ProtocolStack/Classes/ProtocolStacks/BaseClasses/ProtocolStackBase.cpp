// ProtocolStackBase.cpp: Implementierung der Klasse CProtocolStackBase.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <algorithm>

#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include "../../GatewayToInterface/BaseClasses/GatewayPStoI.h"
#include "../../GatewayToDevice/BaseClasses/GatewayPStoDCS.h"

#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include "../InterfacePlugInManager/InterfacePlugInManager.h"

#include "ProtocolStackBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CProtocolStackBase::CProtocolStackBase()
{
    m_lInstanceValue = 0;
    m_strProtocolStackName = "";

    m_pJournalManager = 0;
    m_pGatewayToInterface = 0;
    m_pGatewayToDevice = 0;
    m_dTimeout = 0;

    m_pPlugInManager = 0;
    m_pErrorHandling = 0;
    InitErrorHandling();
}

CProtocolStackBase::CProtocolStackBase(const CProtocolStackBase& p_rObject)
{
    //Instance Value
    m_lInstanceValue = p_rObject.m_lInstanceValue;

    //Timeout
    m_dTimeout = p_rObject.m_dTimeout;

    //Protokolstackname kopieren
    m_strProtocolStackName = p_rObject.m_strProtocolStackName;

    //JournalManager
    m_pJournalManager = p_rObject.m_pJournalManager;

    //Gateways
    if(p_rObject.m_pGatewayToInterface) m_pGatewayToInterface = (CGatewayPStoI*)p_rObject.m_pGatewayToInterface->Clone(); else m_pGatewayToInterface = 0;
    if(p_rObject.m_pGatewayToDevice) m_pGatewayToDevice = (CGatewayPStoDCS*)p_rObject.m_pGatewayToDevice->Clone(); else m_pGatewayToDevice = 0;

    m_pPlugInManager = 0;
    m_pErrorHandling = 0;
    InitErrorHandling();
}

BOOL CProtocolStackBase::CreatePlugInManager()
{
    BOOL oResult(FALSE);

    DeletePlugInManager();
    if(!m_pPlugInManager)
    {
        m_pPlugInManager = new CInterfacePlugInManager();
        m_pPlugInManager->Init(this);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStackBase::DeletePlugInManager()
{
    BOOL oResult(FALSE);

    if(m_pPlugInManager)
    {
        delete m_pPlugInManager;
        m_pPlugInManager = 0;
        oResult = TRUE;
    }

    return oResult;
}

CProtocolStackBase* CProtocolStackBase::Clone()
{
    return 0;
}

BOOL CProtocolStackBase::IsEqual(CProtocolStackBase* p_pProtocolStack)
{
    BOOL oResult(FALSE);

    if(p_pProtocolStack)
    {
        oResult = TRUE;
        if(oResult && (m_strProtocolStackName != p_pProtocolStack->m_strProtocolStackName)) oResult = FALSE;
        if(oResult && (m_lInstanceValue != p_pProtocolStack->m_lInstanceValue)) oResult = FALSE;
    }

    return oResult;
}

CProtocolStackBase::~CProtocolStackBase()
{
    DeletePlugInManager();
    DeleteErrorHandling();
    DeleteGatewayToInterface();
    DeleteGatewayToDevice();
    DeleteInterfaceManagerList();
    DeleteDeviceCommandSetManagerList();
}

BOOL CProtocolStackBase::InitInstanceValue(int p_lInstanceValue)
{
    BOOL oResult(TRUE);

    m_lInstanceValue = p_lInstanceValue;
    if(m_pPlugInManager) m_pPlugInManager->InitInstanceValue(p_lInstanceValue);

    return oResult;
}

BOOL CProtocolStackBase::InitErrorHandling()
{
    DeleteErrorHandling();
    m_pErrorHandling = new CErrorHandling();
    return TRUE;
}

BOOL CProtocolStackBase::DeleteErrorHandling()
{
    if(m_pErrorHandling)
    {
        delete m_pErrorHandling;
        m_pErrorHandling = 0;
    }

    return TRUE;
}

BOOL CProtocolStackBase::ExecuteCommand(CCommandRoot* pCommand, CLayerManagerBase* pLayerManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    BOOL oTimeoutValid = FALSE;
    BOOL oResult(FALSE);

    if(pCommand)
    {
        //Init References
        if(m_pGatewayToInterface) pCommand->InitGateway(m_pGatewayToInterface);
        if(m_pGatewayToDevice) pCommand->InitGateway(m_pGatewayToDevice);
        pCommand->InitJournalManager(m_pJournalManager);

        //Timeout
        oTimeoutValid = pCommand->IsTimeoutValid();
        if(!oTimeoutValid) pCommand->SetTimeout(m_dTimeout);

        //Execute
        oResult = pCommand->Execute(pLayerManager, h_Handle, hTransactionHandle);

        //Timeout
        if(!oTimeoutValid) pCommand->ResetTimeout();

        //Reset References
        pCommand->ResetGateway();
        pCommand->ResetJournalManager();
    }

    return oResult;
}

BOOL CProtocolStackBase::AbortCommands(BOOL oActive)
{
    BOOL oResult(TRUE);

    if(m_pGatewayToInterface)
    {
        if(!m_pGatewayToInterface->AbortCommands(oActive)) oResult = FALSE;
    }

    if(m_pGatewayToDevice)
    {
        if(!m_pGatewayToDevice->AbortCommands(oActive)) oResult = FALSE;
    }

    return oResult;
}

BOOL CProtocolStackBase::GetCommands(CStdString* pCommandInfo)
{
    return FALSE;
}

BOOL CProtocolStackBase::InitProtocolStack(CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackBase::InitProtocolStack(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackBase::InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackBase::UpdateProtocolStack(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;
    CInterfaceManagerBase* pManager = 0;

    //Is manager already existing
    if(FindCorrectManager(p_InterfaceName, &pManager) || GetFirstManager(&pManager))
    {
        if(VerifyGatewayToInterface(p_InterfaceName))
        {
            if(pManager && pManager->I_UpdateInterface(p_InterfaceName, p_pErrorInfo))
            {
                //Interface updated
                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::AddPort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;
    CInterfaceManagerBase* pManager = 0;

    //Find manager
    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        pManager = (*it);
        if(pManager)
        {
            if(pManager->I_IsInterfaceNameSupported(p_InterfaceName))
            {
                oResult = TRUE;
                break;
            }
        }
    }

    if(oResult)
    {
        if(VerifyGatewayToInterface(p_InterfaceName))
        {
            if(pManager)
            {
                oResult = pManager->I_AddInterfacePort(p_InterfaceName, p_PortName, p_pErrorInfo);
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::DeletePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;
    CInterfaceManagerBase* pManager = 0;

    //Find manager
    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        pManager = (*it);
        if(pManager)
        {
            if(pManager->I_IsInterfaceNameSupported(p_InterfaceName))
            {
                oResult = TRUE;
                break;
            }
        }
    }

    if(oResult)
    {
        if(VerifyGatewayToInterface(p_InterfaceName))
        {
            if(pManager)
            {
                oResult = pManager->I_DeleteInterfacePort(p_InterfaceName, p_PortName, p_pErrorInfo);
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::InitGatewayToInterface(CStdString p_InterfaceName)
{
    return FALSE;
}

BOOL CProtocolStackBase::InitGatewayToDevice(CStdString strDeviceName)
{
    return FALSE;
}

BOOL CProtocolStackBase::InitBaudrateSelection(CStdString p_InterfaceName, CStdDWordArray& p_rulBaudrateSel)
{
    BOOL oResult(TRUE);

    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        CInterfaceManagerBase* pInterfaceManager = (*it);
        if(pInterfaceManager)
        {
            if(!pInterfaceManager->I_InitBaudrateSelection(p_InterfaceName, p_rulBaudrateSel))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::InitDefaultProtocolStackSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout)
{
    BOOL oResult(TRUE);

    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        CInterfaceManagerBase* pInterfaceManager = (*it);
        if(pInterfaceManager)
        {
            if(!pInterfaceManager->I_InitDefaultInterfaceSettings(p_InterfaceName, p_ulBaudrate, p_ulTimeout))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::AddManager(CInterfaceManagerBase* pManager)
{
    BOOL oResult(FALSE);

    if(pManager)
    {
        m_InterfaceManagerList.push_back(pManager);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStackBase::AddManager(CDeviceCommandSetManagerBase* pManager)
{
    BOOL oResult(FALSE);

    if(pManager)
    {
        m_DeviceCommandSetManagerList.push_back(pManager);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStackBase::GetFirstManager(CInterfaceManagerBase** ppManager)
{
    BOOL oResult(FALSE);

    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        CInterfaceManagerBase* pListManager = (*it);
        if(pListManager)
        {
            if(ppManager) *ppManager = pListManager;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::GetFirstManager(CDeviceCommandSetManagerBase** ppManager)
{
    BOOL oResult(FALSE);

    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
    {
        CDeviceCommandSetManagerBase* pListManager = (*it);
        if(pListManager)
        {
            if(ppManager) *ppManager = pListManager;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::FindCorrectManager(CStdString p_InterfaceName, CInterfaceManagerBase** ppManager)
{
    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        CInterfaceManagerBase* pListManager = (*it);
        if(pListManager)
        {
            if(pListManager->I_IsInterfaceNameSupported(p_InterfaceName))
            {
                if(ppManager) *ppManager = pListManager;
                return TRUE;
            }
        }
    }

    return FindCorrectLibrary(p_InterfaceName, ppManager);
}

BOOL CProtocolStackBase::FindCorrectManager(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager)
{
    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
    {
        CDeviceCommandSetManagerBase* pListManager = (*it);
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

BOOL CProtocolStackBase::FindCorrectLibrary(CStdString p_InterfaceName, CInterfaceManagerBase** ppManager)
{
    if(ppManager)
    {
        *ppManager = 0;
        return FALSE;
    }

    return FALSE;
}

BOOL CProtocolStackBase::FindCorrectLibrary(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager)
{
    if(ppManager)
    {
        *ppManager = 0;
        return FALSE;
    }

    return FALSE;
}

void CProtocolStackBase::DeleteGatewayToInterface()
{
    if(m_pGatewayToInterface)
    {
        delete m_pGatewayToInterface;
        m_pGatewayToInterface = 0;
    }
}

void CProtocolStackBase::DeleteGatewayToDevice()
{
    if(m_pGatewayToDevice)
    {
        delete m_pGatewayToDevice;
        m_pGatewayToDevice = 0;
    }
}

void CProtocolStackBase::DeleteInterfaceManagerList()
{
    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        CInterfaceManagerBase* pManager = (*it);
        delete pManager;
    }

    m_InterfaceManagerList.clear();
}

void CProtocolStackBase::DeleteDeviceCommandSetManagerList()
{
    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end();it++)
    {
        CDeviceCommandSetManagerBase* pManager = (*it);
        delete pManager;
    }

    m_DeviceCommandSetManagerList.clear();
}

BOOL CProtocolStackBase::PS_GetProtocolStackName(CStdString* pProtocolStackName)
{
    if(pProtocolStackName)
    {
        *pProtocolStackName = m_strProtocolStackName;
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackBase::PS_GetInterfaceNameSelection(CStdStringArray* pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    CStdStringArray selArray;

    if(pInterfaceNameSel)
    {
        //Force PlugIn Manager Update
        if(m_pPlugInManager)
        {
            //Logging
            #ifdef _Logging_UsbEnumeration
                m_pPlugInManager->WriteToLogFile(_T("Event InterfaceNameSelection"));
            #endif

            //Update
            m_pPlugInManager->UpdateDevice();
        }

        //Enumerated update interfaces
        pInterfaceNameSel->clear();

        for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
        {
            CInterfaceManagerBase* pManager = (*it);
            if(pManager)
            {
                if(pManager->I_GetInterfaceNameSelection(&selArray))
                {
                    AppendSelectionArray(pInterfaceNameSel, &selArray);
                }
            }
        }
        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}

BOOL CProtocolStackBase::PS_GetDeviceNameSelection(CStdStringArray* pDeviceNameSel, CErrorInfo* p_pErrorInfo)
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
                {
                    AppendSelectionArray(pDeviceNameSel, &selArray);
                }
            }
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}

BOOL CProtocolStackBase::PS_GetProtocolStackNameSelection(CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(strDeviceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
        return FALSE;
    }

    //Get ProtocolStack Name Selection
    if(!pManager->DCS_GetProtocolStackNameSelection(strDeviceName, pProtocolStackNameSel, p_pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CProtocolStackBase::AppendSelectionArray(CStdStringArray* pDestArray, CStdStringArray* pSourceArray)
{
    CStdString strDestName;
    CStdString strSourceName;
    BOOL oFound;

    if(pDestArray && pSourceArray)
    {
        for(std::size_t i = 0; i < pSourceArray->size(); i++)
        {
            oFound = FALSE;
            strSourceName = pSourceArray->at(i);
            for(std::size_t j = 0; j < pDestArray->size(); j++)
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

BOOL CProtocolStackBase::VerifyGatewayToInterface(CStdString p_InterfaceName)
{
    return FALSE;
}

BOOL CProtocolStackBase::VerifyGatewayToDevice(CStdString strDeviceName)
{
    return FALSE;
}

BOOL CProtocolStackBase::PS_GetPortNameSelection(CStdString p_InterfaceName, CStdStringArray* pPortSel, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(p_InterfaceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
        return FALSE;
    }

    //GetPortNameSelection
    if(!pManager->I_GetPortNameSelection(p_InterfaceName, pPortSel, p_pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CProtocolStackBase::PS_GetBaudrateSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(p_InterfaceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    if(!pManager->I_GetBaudrateSelection(p_InterfaceName, p_PortName, pdBaudrateSel, p_pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CProtocolStackBase::PS_GetProtocolStackModeSelection(CStdStringArray* pProtocolStackModeSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackBase::PS_GetProtocolStackMode(WORD* pwModeIndex, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackBase::PS_SetProtocolStackMode(WORD wModeIndex, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

void CProtocolStackBase::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    m_pJournalManager = pJournalManager;

    //Interface
    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        CInterfaceManagerBase* pInterface = (*it);
        pInterface->InitJournalManager(pJournalManager);
    }

    //Device
    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
    {
        CDeviceCommandSetManagerBase* pDevice = (*it);
        pDevice->InitJournalManager(pJournalManager);
    }
}

void CProtocolStackBase::ResetJournalManager()
{
    m_pJournalManager = 0;

    //Interface
    for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
    {
        CInterfaceManagerBase* pInterface = (*it);
        pInterface->ResetJournalManager();
    }

    //Device
    for(std::list<CDeviceCommandSetManagerBase*>::iterator it = m_DeviceCommandSetManagerList.begin(); it != m_DeviceCommandSetManagerList.end(); it++)
    {
        CDeviceCommandSetManagerBase* pDevice = (*it);
        pDevice->ResetJournalManager();
    }
}

BOOL CProtocolStackBase::PS_GetDefaultProtocolStackSettings(CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(p_InterfaceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
        return FALSE;
    }

    //GetDefaultInterfaceSettings
    if(!pManager->I_GetDefaultInterfaceSettings(p_InterfaceName, p_pulBaudrate, p_pulTimeout, p_pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CProtocolStackBase::PS_SetDefaultProtocolStackSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager = 0;

    //Find Manager
    if(!FindCorrectManager(p_InterfaceName, &pManager) || (pManager == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
        return FALSE;
    }

    //SetDefaultInterfaceSettings
    if(!pManager->I_SetDefaultInterfaceSettings(p_InterfaceName, p_ulBaudrate, p_ulTimeout, p_pErrorInfo))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CProtocolStackBase::UpdateTimeout(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle)
{
    if(pInterfaceManager)
    {
        return pInterfaceManager->I_GetInterfaceSettings(hI_Handle, 0, &m_dTimeout);
    }
    return FALSE;
}

BOOL CProtocolStackBase::UpdateTimeout(CDeviceCommandSetManagerBase* pDeviceCommandSetManagerManager, HANDLE hDCS_Handle)
{
    if(pDeviceCommandSetManagerManager)
    {
        return pDeviceCommandSetManagerManager->DCS_GetProtocolStackSettings(hDCS_Handle, 0, &m_dTimeout);
    }

    return FALSE;
}

BOOL CProtocolStackBase::SetTimeout(DWORD timeout)
{
    m_dTimeout = timeout;

    return TRUE;
}

BOOL CProtocolStackBase::SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, DWORD p_ulTimeout)
{
    BOOL oResult(FALSE);

    //Device timeout
    if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->SetDeviceTimeout(p_pManager, p_hHandle, p_ulTimeout);
    }

    return oResult;
}

DWORD CProtocolStackBase::GetTimeout()
{
    return m_dTimeout;
}

DWORD CProtocolStackBase::GetEvaluatedTimeout(DWORD dCommandTimeout)
{
    //Evaluated if commandTimeout or ProtocolStack Timeout is valid
    if(dCommandTimeout == 0)
    {
        return m_dTimeout;
    }
    else
    {
        return dCommandTimeout;
    }
}

BOOL CProtocolStackBase::AreParameterEqual(CProtocolStackBase* p_pProtocolStack)
{
    BOOL oResult(FALSE);

    if(p_pProtocolStack)
    {
        if(m_pGatewayToInterface && p_pProtocolStack->m_pGatewayToInterface)
        {
            oResult = m_pGatewayToInterface->AreParameterEqual(p_pProtocolStack->m_pGatewayToInterface);
        }
        else if(m_pGatewayToDevice && p_pProtocolStack->m_pGatewayToDevice)
        {
            oResult = m_pGatewayToDevice->AreParameterEqual(p_pProtocolStack->m_pGatewayToDevice);
        }
    }

    return oResult;
}

BOOL CProtocolStackBase::SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
    BOOL oResult(FALSE);

    if(m_pGatewayToInterface)
    {
        oResult = m_pGatewayToInterface->SetParameter(p_Name, p_pValue, p_ulSize);
    }
    else if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->SetParameter(p_Name, p_pValue, p_ulSize);
    }

    return oResult;
}

BOOL CProtocolStackBase::SetParameter(CStdString p_Name, CStdString p_Value)
{
    BOOL oResult(FALSE);

    if(m_pGatewayToInterface)
    {
        oResult = m_pGatewayToInterface->SetParameter(p_Name, p_Value);
    }
    else if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->SetParameter(p_Name, p_Value);
    }

    return oResult;
}

BOOL CProtocolStackBase::GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
    BOOL oResult(FALSE);

    if(m_pGatewayToInterface)
    {
        oResult = m_pGatewayToInterface->GetParameter(p_Name, p_pValue, p_ulSize);
    }
    else if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->GetParameter(p_Name, p_pValue, p_ulSize);
    }

    return oResult;
}

BOOL CProtocolStackBase::GetParameter(CStdString p_Name, CStdString& p_rValue)
{
    BOOL oResult(FALSE);

    if(m_pGatewayToInterface)
    {
        oResult = m_pGatewayToInterface->GetParameter(p_Name, p_rValue);
    }
    else if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->GetParameter(p_Name, p_rValue);
    }

    return oResult;
}

BOOL CProtocolStackBase::Lock(DWORD p_ulTimeout)
{
    BOOL oResult(FALSE);

    if(m_pGatewayToInterface)
    {
        oResult = m_pGatewayToInterface->Lock(p_ulTimeout);
    }
    else if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->Lock(p_ulTimeout);
    }

    return oResult;
}

BOOL CProtocolStackBase::Unlock()
{
    BOOL oResult(FALSE);

    if(m_pGatewayToInterface)
    {
        oResult = m_pGatewayToInterface->Unlock();
    }
    else if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->Unlock();
    }

    return oResult;
}

BOOL CProtocolStackBase::IsLocked()
{
    BOOL oResult(FALSE);

    if(m_pGatewayToInterface)
    {
        oResult = m_pGatewayToInterface->IsLocked();
    }
    else if(m_pGatewayToDevice)
    {
        oResult = m_pGatewayToDevice->IsLocked();
    }

    return oResult;
}

BOOL CProtocolStackBase::WriteToLogFile(CStdString p_Action)
{
    BOOL oResult(FALSE);

    if(m_pPlugInManager)
    {
        oResult = m_pPlugInManager->WriteToLogFile(p_Action);
    }

    return oResult;
}
/*
BOOL CProtocolStackBase::PS_GetInterfaceNameSelection(CStdStringArray* pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    CStdStringArray selArray;

    if(pInterfaceNameSel)
    {
        //Force PlugIn Manager Update
        if(m_pPlugInManager)
        {
            //Logging
            #ifdef _Logging_UsbEnumeration
                m_pPlugInManager->WriteToLogFile(_T("Event InterfaceNameSelection"));
            #endif

            //Update
            m_pPlugInManager->UpdateDevice();
        }

        //Enumerated update interfaces
        pInterfaceNameSel->clear();

        for(std::list<CInterfaceManagerBase*>::iterator it = m_InterfaceManagerList.begin(); it != m_InterfaceManagerList.end(); it++)
        {
            CInterfaceManagerBase* pManager = (*it);
            if(pManager)
            {
                if(pManager->I_GetInterfaceNameSelection(&selArray))
                {
                    AppendSelectionArray(pInterfaceNameSel, &selArray);
                }
            }
        }
        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}*/