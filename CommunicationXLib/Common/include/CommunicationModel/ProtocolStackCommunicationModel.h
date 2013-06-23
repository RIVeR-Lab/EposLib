// ProtocolStackCommunicationModel.h: Schnittstelle f�r die Klasse CProtocolStackCommunicationModel.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ProtocolStackCommunicationModel_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
#define AFX_ProtocolStackCommunicationModel_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/BaseClasses/ProtocolStackCommunicationModelBase.h>
#include <MmcTypeDefinition.h>

class CCommandRoot;

class CProtocolStackCommunicationModel: public CProtocolStackCommunicationModelBase
{
public:
    CProtocolStackCommunicationModel();
    virtual ~CProtocolStackCommunicationModel();

    //Initialization
    virtual BOOL PS_OpenProtocolStack(CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_OpenProtocolStack(CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseProtocolStack(CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseProtocolStack(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseAllProtocolStacks(CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL PS_OpenLayerProtocolStack(CStdString p_strProtocolStackName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_OpenLayerProtocolStack(CStdString p_strProtocolStackName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseLayerProtocolStack(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseLayerProtocolStack(HANDLE p_hPS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_CloseAllLayerProtocolStacks(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);

    //Hilfsfunktionen
    virtual BOOL PS_GetBaseLayerKeyHandle(HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetBaseLayerKeyHandle(HANDLE p_hPS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo = NULL);

    //Setting Funktionen
    virtual BOOL PS_GetProtocolStackSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetProtocolStackSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL PS_GetProtocolStackMode(WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackMode(HANDLE p_hPS_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetProtocolStackMode(WORD p_usModeIndex, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetProtocolStackMode(HANDLE p_hPS_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo = NULL);

    //Name Funktionen
    virtual BOOL PS_GetProtocolStackName(CStdString* pProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackName(HANDLE p_hPS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetInterfaceName(CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetInterfaceName(HANDLE p_hPS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetPortName(CStdString* p_pPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetPortName(HANDLE p_hPS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetBaseDeviceName(HANDLE p_hPS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo = NULL);

    //Address Parameter
    virtual BOOL IsProtocolStackOpen();
    virtual BOOL SetKeyHandle(HANDLE p_hKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetKeyHandle(HANDLE& p_rhPS_Handle);
    virtual BOOL GetLayerParameterStack(CLayerParameterStack& p_rAddressParameter, EAddressingMode p_eAddressingMode = AM_UNKNOWN_ADDRESSING);

    //IPrivateCommunication Funktionalit�t
    virtual BOOL PS_ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hTransactionHandle);
    virtual BOOL PS_ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle);
    virtual BOOL PS_GetCommands(ELayer p_eLayer, CStdString* p_pCommandInfo);
    virtual BOOL PS_GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo);
    virtual BOOL PS_AbortCommands(BOOL p_oActive);
    virtual BOOL PS_AbortCommands(HANDLE p_hHandle, BOOL p_oActive);

    //IPrivateDtm IJournalTransaction Functions
    virtual HANDLE StartTransaction(HANDLE p_hJournalHandle);
    virtual HANDLE StartTransaction(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle);

    CJournalManagerBase** GetJournalManagerMember();
    CProtocolStackManagerBase* GetProtocolStackManager();
    HANDLE GetPSHandle();

protected:
    HANDLE m_hPS_Handle;
};

#endif // !defined(AFX_ProtocolStackCommunicationModel_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
