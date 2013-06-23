// ProtocolStackCommunicationModelBase.cpp: Implementierung der Klasse CProtocolStackCommunicationModelBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/BaseClasses/ProtocolStackCommunicationModelBase.h>

#include "../ProtocolStack/ProtocolStackDialogManager.h"
#include <CommunicationModel/Common/Journal/BaseClasses/JournalManagerBase.h>
#include <CommunicationModel/ProtocolStack/ProtocolStackManager.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CProtocolStackCommunicationModelBase::CProtocolStackCommunicationModelBase()
: m_pProtocolStackDialogManager(0)
, m_pProtocolStackManager(0)
, m_pJournalManager(0)
{
}

CProtocolStackCommunicationModelBase::~CProtocolStackCommunicationModelBase()
{
    PS_CloseAllProtocolStacks();

    DeleteProtocolStackManager();

    if(m_pJournalManager)
    {
        delete m_pJournalManager;
        m_pJournalManager = 0;
    }
}

BOOL CProtocolStackCommunicationModelBase::DeleteProtocolStackManager()
{
    if(m_pProtocolStackManager)
    {
        delete m_pProtocolStackManager;
        m_pProtocolStackManager = 0;
    }

    return TRUE;
}

BOOL CProtocolStackCommunicationModelBase::DeleteProtocolStackDialogManager()
{
    if(m_pProtocolStackDialogManager)
    {
        delete m_pProtocolStackDialogManager;
        m_pProtocolStackDialogManager = 0;
    }

    return TRUE;
}

BOOL CProtocolStackCommunicationModelBase::PS_OpenProtocolStackDlg(CStdStringArray *p_pstrProtocolStackNameArray, HANDLE *p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager && m_pProtocolStackDialogManager && p_pPSKeyHandle)
    {
        *p_pPSKeyHandle = m_pProtocolStackDialogManager->OpenProtocolStackDlg(m_pProtocolStackManager, p_pstrProtocolStackNameArray, p_pErrorInfo);
        oResult = (*p_pPSKeyHandle != 0);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    HANDLE hResult(0);
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager && p_phPSKeyHandle)
    {
       hResult = m_pProtocolStackManager->PS_OpenProtocolStack(p_ProtocolStackName, p_InterfaceName, p_PortName, p_pErrorInfo);
       if(hResult)
       {
           *p_phPSKeyHandle = hResult;
           oResult = TRUE;
       }
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_CloseProtocolStack(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_CloseProtocolStack(p_hPS_Handle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_CloseAllProtocolStacks(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_CloseAllProtocolStacks(p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_OpenLayerProtocolStackDlg(HANDLE p_hCommunication, HANDLE *p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager && m_pProtocolStackDialogManager && p_pPSKeyHandle)
    {
        *p_pPSKeyHandle = m_pProtocolStackDialogManager->OpenLayerProtocolStackDlg(m_pProtocolStackManager, p_hCommunication, p_pErrorInfo);
        oResult = (*p_pPSKeyHandle != 0);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_OpenLayerProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    HANDLE hResult(0);
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager && p_phPSKeyHandle)
    {
       hResult = m_pProtocolStackManager->PS_OpenProtocolStack(p_ProtocolStackName, p_DeviceName, p_hPS_BaseLayerHandle, p_pErrorInfo);
       if(hResult)
       {
           *p_phPSKeyHandle = hResult;
           oResult = TRUE;
       }
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_CloseLayerProtocolStack(HANDLE p_hPS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_CloseProtocolStack(p_hPS_Handle, p_hPS_BaseLayerHandle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_CloseAllLayerProtocolStacks(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_CloseAllProtocolStacks(p_hPS_BaseLayerHandle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_AreAllProtocolStacksClosed()
{
	BOOL oResult(FALSE);

	if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_AreAllProtocolStacksClosed();
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetKeyHandle(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetKeyHandle(p_ProtocolStackName, p_InterfaceName, p_PortName, p_pPSKeyHandle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetLayerKeyHandle(CStdString p_ProtocolStackName, CStdString p_DeviceName, CLayerParameterStack &p_rAddressParameter, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetLayerKeyHandle(p_ProtocolStackName, p_DeviceName, p_rAddressParameter, p_pPSKeyHandle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetBaseLayerKeyHandle(HANDLE p_hPS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetBaseLayerKeyHandle(p_hPS_Handle, p_pPS_BaseLayerKeyHandle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetDefaultProtocolStackSettings(p_ProtocolStackName, p_InterfaceName, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_SetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_SetDefaultProtocolStackSettings(p_ProtocolStackName, p_InterfaceName, p_ulBaudrate, p_ulTimeout, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_IsProtocolStackNameSupported(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_IsProtocolStackNameSupported(p_ProtocolStackName, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetErrorInfo(DWORD p_ulErrorCode, CStdString* p_pStrErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CErrorHandling::GetErrorDescription(p_ulErrorCode, p_pStrErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetDriverInfo(CStdString* p_pStrDriverName, CStdString* p_pStrDriverVersion)
{
    return FALSE;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetProtocolStackNameSelection(CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetProtocolStackNameSelection(p_pProtocolStackNameSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetProtocolStackModeSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pProtocolStackModeSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetProtocolStackModeSelection(p_ProtocolStackName, p_pProtocolStackModeSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetInterfaceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetInterfaceNameSelection(p_ProtocolStackName, p_pInterfaceNameSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetPortNameSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetPortNameSelection(p_ProtocolStackName, p_InterfaceName, p_pPortSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetBaudrateSelection(p_ProtocolStackName, p_InterfaceName, p_PortName, p_pulBaudrateSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetRemoteProtocolStackNameSelection(CStdStringArray* p_pRemoteProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetRemoteProtocolStackNameSelection(p_pRemoteProtocolStackNameSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetDeviceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetDeviceNameSelection(p_ProtocolStackName, p_pDeviceNameSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetProtocolStackNameSelection(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetProtocolStackNameSelection(p_ProtocolStackName, p_DeviceName, p_pProtocolStackNameSel, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetProtocolStackSettings(p_hPS_Handle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_SetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_SetProtocolStackSettings(p_hPS_Handle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetProtocolStackMode(HANDLE p_hPS_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetProtocolStackMode(p_hPS_Handle, p_pusModeIndex, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_SetProtocolStackMode(HANDLE p_hPS_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_SetProtocolStackMode(p_hPS_Handle, p_usModeIndex, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetProtocolStackName(HANDLE p_hPS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetProtocolStackName(p_hPS_Handle, p_pProtocolStackName, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetInterfaceName(HANDLE p_hPS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetInterfaceName(p_hPS_Handle, p_pInterfaceName, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetPortName(HANDLE p_hPS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetPortName(p_hPS_Handle, p_pPortName, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetDeviceName(HANDLE p_hPS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_GetDeviceName(p_hPS_Handle, p_pDeviceName, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->ExecuteCommand(p_pCommand, p_hHandle, p_hTransactionHandle);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->GetCommands(p_hHandle, p_eLayer, p_pCommandInfo);
    }

    return oResult;
}

HANDLE CProtocolStackCommunicationModelBase::StartTransaction(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle)
{
    HANDLE hHandle(0);

    if(m_pJournalManager)
    {
        hHandle = m_pProtocolStackManager->StartTransaction(p_hPS_Handle, p_hJournalHandle);
        if(hHandle == 0)
        {
            hHandle = m_pJournalManager->StartTransaction(p_hPS_Handle, p_hJournalHandle);
        }
    }

    return hHandle;
}

BOOL CProtocolStackCommunicationModelBase::StopTransaction(HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    if(m_pJournalManager)
    {
        if(!m_pProtocolStackManager->StopTransaction(p_hTransactionHandle))
        {
            oResult = m_pJournalManager->StopTransaction(p_hTransactionHandle);
        }
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::StartCommandTime(CJournalTime* p_pThisTime)
{
    BOOL oResult(FALSE);

    if(m_pJournalManager)
    {
        oResult = m_pJournalManager->StartCommandTime(p_pThisTime);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::StopCommandTime(CJournalTime* p_pThisTime)
{
    BOOL oResult(FALSE);

    if(m_pJournalManager)
    {
        oResult = m_pJournalManager->StopCommandTime(p_pThisTime);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::TransactionEvent_VCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    BOOL oResult(FALSE);

    if(m_pJournalManager)
    {
        if(!m_pProtocolStackManager->TransactionEvent_VCSLayer(p_hTransactionHandle, p_pCommand))
        {
            oResult = m_pJournalManager->TransactionEvent_VCSLayer(p_hTransactionHandle, p_pCommand);
        }
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::TransactionEvent_DCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    BOOL oResult(FALSE);

    if(m_pJournalManager)
    {
        if(!m_pProtocolStackManager->TransactionEvent_DCSLayer(p_hTransactionHandle, p_pCommand))
        {
            oResult = m_pJournalManager->TransactionEvent_DCSLayer(p_hTransactionHandle, p_pCommand);
        }
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::TransactionEvent_ProtocolStackLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    BOOL oResult(FALSE);

    if(m_pJournalManager)
    {
        if(!m_pProtocolStackManager->TransactionEvent_ProtocolStackLayer(p_hTransactionHandle, p_pCommand))
        {
            oResult = m_pJournalManager->TransactionEvent_ProtocolStackLayer(p_hTransactionHandle, p_pCommand);
        }
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::TransactionEvent_InterfaceLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    BOOL oResult(FALSE);

    if(m_pJournalManager)
    {
        if(!m_pProtocolStackManager->TransactionEvent_InterfaceLayer(p_hTransactionHandle, p_pCommand))
        {
            oResult = m_pJournalManager->TransactionEvent_InterfaceLayer(p_hTransactionHandle, p_pCommand);
        }
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_AbortCommands(HANDLE p_hHandle, BOOL p_oActive)
{
    BOOL oResult(TRUE);

    if(m_pProtocolStackManager && !m_pProtocolStackManager->AbortCommands(p_hHandle, p_oActive)) oResult = FALSE;

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::LockCommunication()
{
    DWORD ulTimeout(-1);

    CStdString strTrace(_T("ProtocolStackCommunicationModelBase"));

    return m_CriticalSectionEx.Lock(ulTimeout, strTrace);
}

BOOL CProtocolStackCommunicationModelBase::UnlockCommunication()
{
    CStdString strTrace(_T("ProtocolStackCommunicationModelBase"));

    return m_CriticalSectionEx.Unlock(strTrace);
}

BOOL CProtocolStackCommunicationModelBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->SetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->SetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_Value, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->GetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->GetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_rValue, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_AddPort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_AddPort(p_ProtocolStackName, p_InterfaceName, p_PortName, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModelBase::PS_DeletePort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pProtocolStackManager)
    {
        oResult = m_pProtocolStackManager->PS_DeletePort(p_ProtocolStackName, p_InterfaceName, p_PortName, p_pErrorInfo);
    }

    return oResult;
}
