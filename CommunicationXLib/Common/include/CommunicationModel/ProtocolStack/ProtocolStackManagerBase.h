// ProtocolStackManagerBase.h: Schnittstelle f�r die Klasse CProtocolStackManagerBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOLSTACKMANAGERBASE_H__74C339B5_6C6F_4FF5_B82F_5A13CFFA07B4__INCLUDED_)
#define AFX_PROTOCOLSTACKMANAGERBASE_H__74C339B5_6C6F_4FF5_B82F_5A13CFFA07B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/LayerManagerBase.h>

class CProtocolStackBase;
class CJournalManagerBase;
class CInterfaceManagerBase;
class CProtocolStackManagerBase;
class CJournalTime;
class CLayerParameterStack;
struct IJournalEvents;

class CProtocolStackManagerBase : public CLayerManagerBase
{
public:
    virtual void Init() {m_strClassType = _T("CProtocolStackManagerBase");}

    //Internal Structure Funktionen
    virtual BOOL PS_InitAllProtocolStacks(CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL PS_InitProtocolStack(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_InitProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_InitProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdString p_BaseLayerProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_InitBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdDWordArray& p_ulBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_InitDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_AddPort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_DeletePort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);

//Initialisation
    virtual HANDLE PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseProtocolStack(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseAllProtocolStacks(CErrorInfo* p_pErrorInfo = NULL);

    virtual HANDLE PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseProtocolStack(HANDLE p_hPS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseAllProtocolStacks(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);

	virtual BOOL PS_AreAllProtocolStacksClosed();

//Parameter
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo = NULL);

//Hilfsfunktionen
    virtual BOOL PS_GetKeyHandle(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetLayerKeyHandle(CStdString p_ProtocolStackName, CStdString p_DeviceName, CLayerParameterStack &p_rAddressParameter, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetBaseLayerKeyHandle(HANDLE p_hPS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_IsProtocolStackNameSupported(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);

//Selection Funktionen
    virtual BOOL PS_GetProtocolStackNameSelection(CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetInterfaceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetPortNameSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackModeSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pProtocolStackModeSel, CErrorInfo* p_pErrorInfo = NULL);

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
    virtual BOOL PS_GetDeviceName(HANDLE p_hPS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetInterfaceName(HANDLE p_hPS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetPortName(HANDLE p_hPS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo = NULL);

//Funktionalit�t
    virtual BOOL ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle);
    virtual BOOL GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo);

//JournalManager
    virtual BOOL InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* p_pJournalManager);
    virtual BOOL ResetJournalManager(HANDLE p_hHandle);
    virtual void InitJournalManager(CJournalManagerBase* p_pJournalManager);
    virtual void ResetJournalManager();

//IPrivateDtm IJournal Functions
    virtual HANDLE StartJournal(HANDLE p_hPS_Handle, IJournalEvents* p_pIJournalEvents) = 0;
    virtual BOOL StopJournal(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle) = 0;

//IPrivateDtm IJournalTransaction Functions
    virtual HANDLE StartTransaction(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle);
    virtual BOOL StopTransaction(HANDLE p_hTransactionHandle);
    virtual BOOL StartCommandTime(CJournalTime* p_pThisTime);
    virtual BOOL StopCommandTime(CJournalTime* p_pThisTime);
    virtual BOOL TransactionEvent_VCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);
    virtual BOOL TransactionEvent_DCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);
    virtual BOOL TransactionEvent_ProtocolStackLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);
    virtual BOOL TransactionEvent_InterfaceLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand);

//Tracing
    virtual BOOL EnableTracing(HANDLE p_hI_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DisableTracing(HANDLE p_hI_Handle, CErrorInfo* p_pErrorInfo = NULL);

//LayerManager
    virtual BOOL GetLayerManager(HANDLE p_hHandle, ELayer p_eLayer, CLayerManagerBase** p_ppLayerManager, HANDLE* pLayerHandle);
    virtual BOOL SetInterfaceManager(CStdString p_ProtocolStackName, CInterfaceManagerBase* p_pInterfaceManager);
    virtual BOOL GetInterfaceManager(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CInterfaceManagerBase** p_ppInterfaceManager);
    virtual BOOL SetProtocolStackManager(CStdString p_ProtocolStackName, CStdString p_DeviceName, CProtocolStackManagerBase* p_pProtocolStackManager);
    virtual BOOL GetProtocolStackManager(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdString p_BaseLayerProtocolStackName, CProtocolStackManagerBase** p_ppProtocolStackManager);

    CProtocolStackManagerBase();
    CProtocolStackManagerBase(int p_lInstanceValue);
    CProtocolStackManagerBase(const CProtocolStackManagerBase& rObject);
    virtual ~CProtocolStackManagerBase();
    virtual CProtocolStackManagerBase* Clone();
};

#endif // !defined(AFX_PROTOCOLSTACKMANAGERBASE_H__74C339B5_6C6F_4FF5_B82F_5A13CFFA07B4__INCLUDED_)
