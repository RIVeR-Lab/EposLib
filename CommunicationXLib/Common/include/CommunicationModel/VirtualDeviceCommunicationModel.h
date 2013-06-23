// VirtualDeviceCommunicationModel.h: Schnittstelle f�r die Klasse CVirtualDeviceCommunicationModel.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualDeviceCommunicationModel_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
#define AFX_VirtualDeviceCommunicationModel_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/BaseClasses/VirtualDeviceCommunicationModelBase.h>
#include <ObjectDictionary/ObjectEntryRangeCheck.h>
#include <ObjectDictionary/ObjectDictionary.h>

class CCommandRoot;
class CLayerParameterStack;
class CJournalManagerWrapper;

class CVirtualDeviceCommunicationModel: public CVirtualDeviceCommunicationModelBase
{
public:
    CVirtualDeviceCommunicationModel();
    virtual ~CVirtualDeviceCommunicationModel();

    //Initialisation
    virtual BOOL VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseVirtualDevice(CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseAllVirtualDevices(CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseAllVirtualDevices(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);

    //Setting Funktionen
    virtual BOOL VCS_GetProtocolStackSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_SetProtocolStackSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_SetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

    //Name Funktionen
    virtual BOOL VCS_GetVirtualDeviceName(CStdString* p_pstrVirtualDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetVirtualDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrVirtualDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceName(CStdString* p_pstrDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackName(CStdString* p_pstrProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackName(HANDLE p_hVCS_Handle, CStdString* p_pstrProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceName(CStdString* p_pstrInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceName(HANDLE p_hVCS_Handle, CStdString* p_pstrInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetPortName(CStdString* p_pstrPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetPortName(HANDLE p_hVCS_Handle, CStdString* p_pstrPortName, CErrorInfo* p_pErrorInfo = NULL);

    //Address Parameter
    virtual BOOL IsVirtualDeviceOpen();
    virtual BOOL SetKeyHandle(HANDLE p_hKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetKeyHandle(HANDLE& p_rhVCS_Handle);
    virtual BOOL GetLayerParameterStack(CLayerParameterStack& p_rAddressParameter, EAddressingMode addressingMode = AM_UNKNOWN_ADDRESSING);
    virtual BOOL DetectLayerParameter(CLayerParameterStack& p_rAddressParameter, BOOL p_oShowMsg);
    virtual BOOL DetectLayerParameter(BOOL p_oShowMsg);

    //Funktionalit�t
    virtual BOOL ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hTransactionHandle);
    virtual BOOL ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hTransactionHandle);
    virtual BOOL GetCommands(ELayer p_Layer, CStdString* p_pstrCommandInfo);
    virtual BOOL GetCommands(HANDLE p_hVCS_Handle, ELayer p_Layer, CStdString* p_pstrCommandInfo);
    virtual BOOL AbortCommands(bool p_oActive);
    virtual BOOL AbortCommands(HANDLE p_hVCS_Handle, bool p_oActive);
    virtual BOOL InvokeCommand(CCommandRoot *p_pCommand, HANDLE p_hJournalHandle);
    virtual BOOL InvokeCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle);

    //IJournalTransaction Functions
    virtual HANDLE StartTransaction(HANDLE p_hJournalHandle);
    virtual HANDLE StartTransaction(HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle);

    //Tracing
    virtual BOOL EnableTracing(CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL EnableTracing(HANDLE p_hVCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DisableTracing(CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DisableTracing(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo = NULL);

    //Dialog Funktionen
    virtual BOOL SearchCommunicationDlg(CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);
    virtual BOOL SearchCommunicationDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);

    virtual BOOL SearchCommunicationDlg(CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);
    virtual BOOL SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);

    virtual BOOL SearchCommunicationWholeCanIdRangeDlg(CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, WORD *p_pusNodeId, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);
    virtual BOOL SearchCommunicationWholeCanIdRangeDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, WORD *p_pusNodeId, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode = DLGM_PROGRESS_AND_CONFIRM_DLG);

    virtual BOOL SearchCommunicationDlg(CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);
    virtual BOOL SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CLayerParameterStack& p_rAddressParameter, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);

    //ObjectDictionary
    BOOL SetObjectDictionary(CObjectDictionary* p_pObjectDictionary);
    CObjectDictionary* GetObjectDictionary();
    template <class T> BOOL GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, T* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);
    template <class T> BOOL SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, T p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);
    BOOL GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToRead, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);
    BOOL SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToWrite, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);
    BOOL GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToRead, DWORD *p_pulNbOfBytesRead, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);
    BOOL SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToWrite, DWORD *p_pulNbOfBytesWritten, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);

    CJournalManagerBase** GetJournalManagerMember();
    CVirtualCommandSet_Manager* GetVirtualCommandSetManager();
    HANDLE GetVCSHandle();

    //Address
    virtual WORD GetPositionAddr(EAddressingMode p_AddressingMode = AM_UNKNOWN_ADDRESSING);

protected:
    HANDLE m_hVCS_Handle;

private:
    //ObjectDictionary
    CObjectDictionary* m_pObjectDictionary;
    CObjectEntryRangeCheck m_objectEntryRangeCheck;
};

#endif // !defined(AFX_VirtualDeviceCommunicationModel_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
