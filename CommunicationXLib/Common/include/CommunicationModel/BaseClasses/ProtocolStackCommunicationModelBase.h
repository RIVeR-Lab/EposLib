// ProtocolStackCommunicationModelBase.h: Schnittstelle für die Klasse CProtocolStackCommunicationModelBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ProtocolStackCommunicationModelBase_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
#define AFX_ProtocolStackCommunicationModelBase_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>
#include <Thread/MmcCriticalSection.h>

class CProtocolStackDialogManager;
class CProtocolStackManagerBase;
class CJournalManagerBase;

class CProtocolStackCommunicationModelBase
{
public:
    CProtocolStackCommunicationModelBase();
    virtual ~CProtocolStackCommunicationModelBase();

    //Initialisation
    virtual BOOL PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseProtocolStack(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseAllProtocolStacks(CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL PS_OpenLayerProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseLayerProtocolStack(HANDLE p_hPS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseAllLayerProtocolStacks(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);

	virtual BOOL PS_AreAllProtocolStacksClosed();

    //Hilfsfunktionen
    virtual BOOL PS_GetKeyHandle(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetLayerKeyHandle(CStdString p_ProtocolStackName, CStdString p_DeviceName, CLayerParameterStack &p_rAddressParameter, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetBaseLayerKeyHandle(HANDLE p_hPS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_IsProtocolStackNameSupported(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetErrorInfo(DWORD p_ulErrorCode, CStdString* p_pStrErrorInfo);
    virtual BOOL PS_GetDriverInfo(CStdString* p_pStrDriverName, CStdString* p_pStrDriverVersion);

    //Selection Funktionen
    virtual BOOL PS_GetProtocolStackNameSelection(CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackModeSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pProtocolStackModeSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetInterfaceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetPortNameSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL PS_GetRemoteProtocolStackNameSelection(CStdStringArray* p_pRemoteProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetDeviceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pDeviceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackNameSelection(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);

    //Setting Funktionen
    virtual BOOL PS_GetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL PS_GetProtocolStackMode(HANDLE p_hPS_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetProtocolStackMode(HANDLE p_hPS_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo = NULL);

    //Name Funktionen
    virtual BOOL PS_GetProtocolStackName(HANDLE p_hPS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetInterfaceName(HANDLE p_hPS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetPortName(HANDLE p_hPS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetDeviceName(HANDLE p_hPS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo = NULL);

    //IPrivateCommunication Funktionalität
    virtual BOOL PS_ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle);
    virtual BOOL PS_GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo);
    virtual BOOL PS_AbortCommands(HANDLE p_hHandle, BOOL p_oActive);

    //IPrivateDtm IJournalTransaction Functions
    virtual HANDLE StartTransaction(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle);
    virtual BOOL StopTransaction(HANDLE p_hTransactionHandle);
    virtual BOOL StartCommandTime(CJournalTime* p_pThisTime);
    virtual BOOL StopCommandTime(CJournalTime* p_pThisTime);
    virtual BOOL TransactionEvent_VCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);
    virtual BOOL TransactionEvent_DCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);
    virtual BOOL TransactionEvent_ProtocolStackLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);
    virtual BOOL TransactionEvent_InterfaceLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);

    //Parameter
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo = NULL);

    //Dialogs
    virtual BOOL PS_OpenProtocolStackDlg(CStdStringArray *p_pstrProtocolStackNameArray, HANDLE *p_pPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_OpenLayerProtocolStackDlg(HANDLE p_hCommunication, HANDLE *p_pPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);

    //Internal structure functions
    virtual BOOL PS_AddPort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_DeletePort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);

protected:
    BOOL DeleteProtocolStackManager();
    BOOL DeleteProtocolStackDialogManager();

    //Synchronisation
    BOOL LockCommunication();
    BOOL UnlockCommunication();

protected:
    CProtocolStackManagerBase* m_pProtocolStackManager;
    CProtocolStackDialogManager* m_pProtocolStackDialogManager;
    CJournalManagerBase* m_pJournalManager;

    //Synchronisation of internal Functions, ActiveX controls and GatewayChannel
    CMmcCriticalSectionEx m_CriticalSectionEx;
};

#endif // !defined(AFX_ProtocolStackCommunicationModelBase_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
