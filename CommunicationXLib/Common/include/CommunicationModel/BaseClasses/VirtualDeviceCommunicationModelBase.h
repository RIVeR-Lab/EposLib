// VirtualDeviceCommunicationModelBase.h: Schnittstelle f�r die Klasse CVirtualDeviceCommunicationModelBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualDeviceCommunicationModelBase_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
#define AFX_VirtualDeviceCommunicationModelBase_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/VirtualCommandSet/Dialogs/SearchCommunicationDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorCode_Definitions.h>
#include <CommunicationModel/VirtualCommandSet/VirtualCommandSet_Manager.h>
#include <Thread/MmcCriticalSection.h>

class CVirtualCommandSet_DialogManager;
class CJournalManagerBase;
class CLayerParameterStack;

class CVirtualDeviceCommunicationModelBase
{
public:
    CVirtualDeviceCommunicationModelBase();
    virtual ~CVirtualDeviceCommunicationModelBase();

    //Synchronisation
    CMmcCriticalSectionEx* GetCriticalSection();

    //Initialisation
    virtual BOOL VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseAllVirtualDevices(CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseAllVirtualDevices(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);

	virtual BOOL VCS_AreAllVirtualDevicesClosed();

    //Hilfsfunktionen
    virtual BOOL VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CLayerParameterStack& p_rAddressParameter, HANDLE* p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetBaseLayerKeyHandle(HANDLE p_hVCS_Handle, HANDLE* p_phPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetErrorInfo(DWORD p_ulErrorCode, CStdString* p_pStrDescription);
    virtual bool VCS_GetDriverInfo(CStdString* p_pstrLibraryName, CStdString* p_pstrLibraryVersion);
    virtual BOOL VCS_GetDefaultProtocolStackSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);

    //Selection Funktionen
	virtual BOOL VCS_GetVirtualDeviceNameSelection(CStdStringArray* p_pastrVirtualDeviceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceNameSelection(CStdString p_strVirtualDeviceName, CStdStringArray* p_pastrDeviceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackNameSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdStringArray* p_pastrProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceNameSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdStringArray* p_pastrInterfaceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetPortNameSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdStringArray* p_pastrPortSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetBaudrateSelection(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CStdDWordArray* p_paulBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);

    //Setting Funktionen
    virtual BOOL VCS_GetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_SetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

    //Name Funktionen
    virtual BOOL VCS_GetVirtualDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrVirtualDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackName(HANDLE p_hVCS_Handle, CStdString* p_pstrProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceName(HANDLE p_hVCS_Handle, CStdString* p_pstrInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetPortName(HANDLE p_hVCS_Handle, CStdString* p_pstrPortName, CErrorInfo* p_pErrorInfo = NULL);

    //Funktionalität
    virtual BOOL ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hTransactionHandle);
    virtual BOOL GetCommands(HANDLE p_hVCS_Handle, ELayer p_Layer, CStdString* p_pstrCommandInfo);
    virtual BOOL AbortCommands(HANDLE p_hVCS_Handle, bool p_oActive);
    virtual BOOL InvokeCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle);
    virtual BOOL InitJournalManager(HANDLE p_hVCS_Handle, CJournalManagerBase* p_pJournalManager);

    //Parameter
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo = NULL);

    //IJournalTransaction Functions
    virtual HANDLE StartTransaction(HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle);
    virtual BOOL StopTransaction(HANDLE p_hTransactionHandle);
    virtual BOOL StartCommandTime(CJournalTime* p_pThisTime);
    virtual BOOL StopCommandTime(CJournalTime* p_pThisTime);
    virtual BOOL TransactionEvent_VCSLayer(HANDLE p_hTransactionHandle, CCommandRoot* p_pCommand);
    virtual BOOL TransactionEvent_DCSLayer(HANDLE p_hTransactionHandle, CCommandRoot* p_pCommand);
    virtual BOOL TransactionEvent_ProtocolStackLayer(HANDLE p_hTransactionHandle, CCommandRoot* p_pCommand);
    virtual BOOL TransactionEvent_InterfaceLayer(HANDLE p_hTransactionHandle, CCommandRoot* p_pCommand);

    //Tracing
    virtual BOOL EnableTracing(HANDLE p_hVCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DisableTracing(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo = NULL);

    //Search Communication
    virtual BOOL SearchCommunicationDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);
    virtual BOOL SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);
    virtual BOOL SearchCommunicationWholeCanIdRangeDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, WORD *p_pusNodeId, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);
    virtual BOOL SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CLayerParameterStack& p_rAddressParameter, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);

    //Dialogs
    virtual BOOL VCS_OpenVirtualDeviceDlg(CStdStringArray *p_pstrVirtualDeviceNameArray, HANDLE *p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo);
    virtual BOOL VCS_OpenDriveDlg(CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrDriveNameArray, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_OpenPlcDlg(CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrPlcNameArray, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);

protected:
    BOOL DeleteVirtualCommandSetManager();
    BOOL DeleteVirtualCommandSetDialogManager();

    //Synchronization
    BOOL LockCommunication();
    BOOL UnlockCommunication();

protected:
    CVirtualCommandSet_Manager* m_pVirtualCommandSetManager;
    CVirtualCommandSet_DialogManager* m_pVirtualCommandSetDialogManager;
    CJournalManagerBase* m_pJournalManager;

    //Synchronization of internal Functions, ActiveX controls and GatewayChannel
	CMmcCriticalSectionEx m_CriticalSectionEx;
};

#endif // !defined(AFX_VirtualDeviceCommunicationModelBase_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
