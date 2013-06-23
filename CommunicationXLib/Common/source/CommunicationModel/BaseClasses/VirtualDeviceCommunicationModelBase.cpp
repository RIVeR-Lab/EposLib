// VirtualDeviceCommunicationModelBase.cpp: Implementierung der Klasse CVirtualDeviceCommunicationModelBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <Storage/MmcFileInfo.h>

#ifdef WINVER
//#include <CommonWinLibRes.h>
#endif

#include <CommunicationModel/VirtualCommandSet/VirtualCommandSet_DialogManager.h>
#include <CommunicationModel/Common/Journal/BaseClasses/JournalManagerBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include <CommunicationModel/VirtualCommandSet/Dialogs/SearchCommunicationDlg.h>
#include <CommunicationModel/VirtualCommandSet/Dialogs/SearchCommunicationWholeCanIdRangeDlg.h>
#include <CommunicationModel/BaseClasses/VirtualDeviceCommunicationModelBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CVirtualDeviceCommunicationModelBase::CVirtualDeviceCommunicationModelBase()
: m_pVirtualCommandSetManager(NULL)
, m_pVirtualCommandSetDialogManager(NULL)
, m_pJournalManager(NULL)
{
}

CVirtualDeviceCommunicationModelBase::~CVirtualDeviceCommunicationModelBase()
{
    VCS_CloseAllVirtualDevices();

    DeleteVirtualCommandSetManager();
    DeleteVirtualCommandSetDialogManager();

    if(m_pJournalManager)
    {
        delete m_pJournalManager;
        m_pJournalManager = NULL;
    }
}

BOOL CVirtualDeviceCommunicationModelBase::DeleteVirtualCommandSetManager()
{
    if(m_pVirtualCommandSetManager)
    {
        delete m_pVirtualCommandSetManager;
        m_pVirtualCommandSetManager = NULL;
    }

    return TRUE;
}

BOOL CVirtualDeviceCommunicationModelBase::DeleteVirtualCommandSetDialogManager()
{
    if(m_pVirtualCommandSetDialogManager)
    {
        delete m_pVirtualCommandSetDialogManager;
        m_pVirtualCommandSetDialogManager = NULL;
    }

    return TRUE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_OpenVirtualDeviceDlg(CStdStringArray *p_pstrVirtualDeviceNameArray, HANDLE *p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pVirtualCommandSetManager && m_pVirtualCommandSetDialogManager && p_pVCSKeyHandle)
    {
        *p_pVCSKeyHandle = m_pVirtualCommandSetDialogManager->OpenVirtualDeviceDlg(m_pVirtualCommandSetManager, p_pstrVirtualDeviceNameArray, p_pErrorInfo);
        oResult = (*p_pVCSKeyHandle != 0);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    HANDLE hResult(0);
    BOOL oResult(FALSE);

    if(m_pVirtualCommandSetManager && p_phVCSKeyHandle)
    {
        //Check Compatibility
		if(p_strProtocolStackName == PROTOCOL_EPOS2_USB) p_strProtocolStackName = PROTOCOL_MAXON_SERIAL_V2;

		//OpenVirtualDevice
        hResult = m_pVirtualCommandSetManager->VCS_OpenVirtualDevice(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_pErrorInfo);
        if(hResult != 0)
        {
            *p_phVCSKeyHandle = hResult;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        if(m_pVirtualCommandSetManager->VCS_CloseVirtualDevice(p_hVCS_Handle, p_pErrorInfo))
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_CloseAllVirtualDevices(CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        if(m_pVirtualCommandSetManager->VCS_CloseAllVirtualDevices(p_pErrorInfo))
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_AreAllVirtualDevicesClosed()
{
	BOOL oResult(FALSE);

	if(m_pVirtualCommandSetManager)
    {
        oResult = m_pVirtualCommandSetManager->VCS_AreAllVirtualDevicesClosed();
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetKeyHandle(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_phVCSKeyHandle, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CLayerParameterStack& p_rAddressParameter, HANDLE* p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetKeyHandle(p_strVirtualDeviceName, p_strDeviceName, p_rAddressParameter, p_pVCSKeyHandle, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetBaseLayerKeyHandle(HANDLE p_hVCS_Handle, HANDLE* p_phPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetBaseLayerKeyHandle(p_hVCS_Handle, p_phPS_BaseLayerKeyHandle, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetVirtualDeviceNameSelection(CStdStringArray* p_pastrVirtualDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetVirtualDeviceNameSelection(p_pastrVirtualDeviceNameSel, p_pErrorInfo);
    }

    return FALSE;
}
BOOL CVirtualDeviceCommunicationModelBase::VCS_GetDeviceNameSelection(CStdString p_strVirtualDeviceName, CStdStringArray* p_pastrDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetDeviceNameSelection(p_strVirtualDeviceName, p_pastrDeviceNameSel, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetProtocolStackNameSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdStringArray* p_pastrProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetProtocolStackNameSelection(p_strVirtualDeviceName, p_strDeviceName, p_pastrProtocolStackNameSel, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetInterfaceNameSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdStringArray* p_pastrInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetInterfaceNameSelection(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_pastrInterfaceNameSel, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetPortNameSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdStringArray* p_pastrPortSel, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetPortNameSelection(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_pastrPortSel, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetBaudrateSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CStdDWordArray* p_paulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetBaudrateSelection(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_paulBaudrateSel, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetProtocolStackSettings(p_hVCS_Handle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetDefaultProtocolStackSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetDefaultProtocolStackSettings(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_SetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_SetProtocolStackSettings(p_hVCS_Handle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetVirtualDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrVirtualDeviceName, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetVirtualDeviceName(p_hVCS_Handle, p_pstrVirtualDeviceName, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrDeviceName, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetDeviceName(p_hVCS_Handle, p_pstrDeviceName, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetProtocolStackName(HANDLE p_hVCS_Handle, CStdString* p_pstrProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetProtocolStackName(p_hVCS_Handle, p_pstrProtocolStackName, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetInterfaceName(HANDLE p_hVCS_Handle, CStdString* p_pstrInterfaceName, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetInterfaceName(p_hVCS_Handle, p_pstrInterfaceName, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetPortName(HANDLE p_hVCS_Handle, CStdString* p_pstrPortName, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_GetPortName(p_hVCS_Handle, p_pstrPortName, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    if(m_pVirtualCommandSetManager)
    {
        oResult = m_pVirtualCommandSetManager->ExecuteCommand(p_pCommand, p_hVCS_Handle, p_hTransactionHandle);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::GetCommands(HANDLE p_hVCS_Handle, ELayer p_Layer, CStdString* p_pstrCommandInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->GetCommands(p_hVCS_Handle, p_Layer, p_pstrCommandInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::AbortCommands(HANDLE p_hVCS_Handle, bool p_oActive)
{
    if(m_pVirtualCommandSetManager && !m_pVirtualCommandSetManager->AbortCommands(p_hVCS_Handle, p_oActive)) return FALSE;
    if(!m_CriticalSectionEx.AbortLock(p_oActive)) return FALSE;

    return TRUE;
}

BOOL CVirtualDeviceCommunicationModelBase::InvokeCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle)
{
    BOOL oResult(FALSE);
    HANDLE hTransactionHandle;

    if(m_pJournalManager)
    {
        hTransactionHandle = m_pJournalManager->StartTransaction(p_hVCS_Handle, p_hJournalHandle);

        oResult = m_pVirtualCommandSetManager->ExecuteCommand(p_pCommand, p_hVCS_Handle, hTransactionHandle);

        m_pJournalManager->StopTransaction(hTransactionHandle);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::InitJournalManager(HANDLE p_hVCS_Handle, CJournalManagerBase* p_pJournalManager)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->InitJournalManager(p_hVCS_Handle, p_pJournalManager);
    }

    return FALSE;
}

HANDLE CVirtualDeviceCommunicationModelBase::StartTransaction(HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->StartTransaction(p_hVCS_Handle, p_hJournalHandle);
    }

    return NULL;
}

BOOL CVirtualDeviceCommunicationModelBase::StopTransaction(HANDLE p_hTransactionHandle)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->StopTransaction(p_hTransactionHandle);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::StartCommandTime(CJournalTime* p_pThisTime)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->StartCommandTime(p_pThisTime);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::StopCommandTime(CJournalTime* p_pThisTime)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->StopCommandTime(p_pThisTime);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::TransactionEvent_VCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->TransactionEvent_VCSLayer(p_hTransactionHandle, p_pCommand);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::TransactionEvent_DCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->TransactionEvent_DCSLayer(p_hTransactionHandle, p_pCommand);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::TransactionEvent_ProtocolStackLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->TransactionEvent_ProtocolStackLayer(p_hTransactionHandle, p_pCommand);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::TransactionEvent_InterfaceLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    if(m_pJournalManager)
    {
        return m_pJournalManager->TransactionEvent_InterfaceLayer(p_hTransactionHandle, p_pCommand);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_GetErrorInfo(DWORD p_ulErrorCode, CStdString* p_pStrDescription)
{
    return CErrorHandling::GetErrorDescription(p_ulErrorCode, p_pStrDescription);
}

BOOL CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CLayerParameterStack& p_rAddressParameter, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode, EAddressingMode p_AddressingMode)
{
    CCommand_VCS_Common command(COMMON_GET_DEVICE_TYPE);
    BOOL oResult(FALSE);

    //Address Parameter
    command.SetLayerParameterStack(p_rAddressParameter);

    oResult = SearchCommunicationDlg(p_rhVCS_Handle, &command, p_strInfoMessage, p_strRootVirtualDeviceName, p_strRootDeviceName, p_strRootProtocolStackName, p_eDialogMode);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode)
{
#ifdef WINVER
    CSearchCommunicationDlg dlg;

    if(m_pVirtualCommandSetManager && p_pSearchCommand)
    {
        //Init
        dlg.Init(m_pVirtualCommandSetManager, p_rhVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_eDialogMode);
        dlg.InitRootCommunicationSetting(p_strRootVirtualDeviceName, p_strRootDeviceName, p_strRootProtocolStackName, "", "");

        //Search Settings
        INT_PTR nRet = dlg.DoModal();

        //GetHandle
        dlg.GetHandle(&p_rhVCS_Handle);
        return dlg.SearchSucceeded();
    }
#endif
    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode)
{
#ifdef WINVER
    CSearchCommunicationDlg dlg;

    if(m_pVirtualCommandSetManager && p_pSearchCommand && p_phKeyHandle)
    {
        //Init
        dlg.Init(m_pVirtualCommandSetManager, p_hVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_eDialogMode);
        dlg.InitRootCommunicationSetting(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName);

        //Search Settings
        INT_PTR nRet = dlg.DoModal();

        //GetHandle
        dlg.GetHandle(p_phKeyHandle);

        return dlg.SearchSucceeded();
    }
#endif
    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::SearchCommunicationWholeCanIdRangeDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, WORD *p_pusNodeId, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode)
{
#ifdef WINVER
    CSearchCommunicationWholeCanIdRangeDlg dlg;

    if(m_pVirtualCommandSetManager && p_pSearchCommand && p_phKeyHandle)
    {
        //Init
        dlg.Init(m_pVirtualCommandSetManager, p_hVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_eDialogMode);
        dlg.InitRootCommunicationSetting(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName);

        //Search Settings
        INT_PTR nRet = dlg.DoModal();

        //GetHandle
        dlg.GetHandle(p_phKeyHandle);
        dlg.GetNodeId(p_pusNodeId);

        return dlg.SearchSucceeded();
    }
#endif
    return FALSE;
}

bool CVirtualDeviceCommunicationModelBase::VCS_GetDriverInfo(CStdString* p_pstrLibraryName, CStdString* p_pstrLibraryVersion)
{
    if(p_pstrLibraryName && p_pstrLibraryVersion)
    {
		*p_pstrLibraryName = CMmcFileInfo::GetProductName("CommonLib.lib");
        if(p_pstrLibraryName->IsEmpty())
        {
            return false;
        }

        *p_pstrLibraryVersion = CMmcFileInfo::GetProductVersion("CommonLib.lib");
        if(p_pstrLibraryVersion->IsEmpty())
        {
            return false;
        }

        return true;
    }

    return false;
}

BOOL CVirtualDeviceCommunicationModelBase::LockCommunication()
{
    DWORD ulTimeout(-1);
    CStdString strTrace(_T("VirtualDeviceCommunicationModelBase"));

    return m_CriticalSectionEx.Lock(ulTimeout, strTrace);
}

BOOL CVirtualDeviceCommunicationModelBase::UnlockCommunication()
{
    CStdString strTrace(_T("VirtualDeviceCommunicationModelBase"));

    return m_CriticalSectionEx.Unlock(strTrace);
}

CMmcCriticalSectionEx* CVirtualDeviceCommunicationModelBase::GetCriticalSection()
{
    return &m_CriticalSectionEx;
}

BOOL CVirtualDeviceCommunicationModelBase::EnableTracing(HANDLE p_hVCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->EnableTracing(p_hVCS_Handle, p_TracingFileName, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::DisableTracing(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->DisableTracing(p_hVCS_Handle, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);
    HANDLE hResult(0);

    if(m_pVirtualCommandSetManager && p_phVCSKeyHandle)
    {
        hResult = m_pVirtualCommandSetManager->VCS_OpenVirtualDevice(p_strVirtualDeviceName, p_strDeviceName, p_hPS_BaseLayerHandle, p_pErrorInfo);

        if(hResult != 0)
        {
            *p_phVCSKeyHandle = hResult;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_CloseVirtualDevice(p_hVCS_Handle, p_hPS_BaseLayerHandle, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_CloseAllVirtualDevices(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->VCS_CloseAllVirtualDevices(p_hPS_BaseLayerHandle, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->SetParameter(p_eParameterType, p_hVCS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->SetParameter(p_eParameterType, p_hVCS_Handle, p_Name, p_Value, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->GetParameter(p_eParameterType, p_hVCS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    if(m_pVirtualCommandSetManager)
    {
        return m_pVirtualCommandSetManager->GetParameter(p_eParameterType, p_hVCS_Handle, p_Name, p_rValue, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_OpenDriveDlg(CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrDriveNameArray, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pVirtualCommandSetManager && m_pVirtualCommandSetDialogManager && p_pVCSKeyHandle)
    {
        *p_pVCSKeyHandle = m_pVirtualCommandSetDialogManager->OpenDriveDlg(m_pVirtualCommandSetManager, p_strVirtualDeviceName, p_pstrDriveNameArray, p_hPS_BaseLayerHandle, p_pErrorInfo);
        oResult = (*p_pVCSKeyHandle != 0);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModelBase::VCS_OpenPlcDlg(CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrPlcNameArray, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pVirtualCommandSetManager && m_pVirtualCommandSetDialogManager && p_pVCSKeyHandle)
    {
        *p_pVCSKeyHandle = m_pVirtualCommandSetDialogManager->OpenPlcDlg(m_pVirtualCommandSetManager, p_strVirtualDeviceName, p_pstrPlcNameArray, p_hPS_BaseLayerHandle, p_pErrorInfo);
        oResult = (*p_pVCSKeyHandle != 0);
    }

    return oResult;
}
