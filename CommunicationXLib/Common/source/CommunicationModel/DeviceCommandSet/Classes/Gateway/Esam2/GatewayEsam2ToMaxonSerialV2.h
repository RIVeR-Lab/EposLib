// GatewayEsam2ToMaxonSerialV2.h: Schnittstelle f�r die Klasse CGatewayEsam2ToMaxonSerialV2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYESAM2TOMAXONSERIALV2_H__10BC918F_ECD4_465F_A861_6942A4B78334__INCLUDED_)
#define AFX_GATEWAYESAM2TOMAXONSERIALV2_H__10BC918F_ECD4_465F_A861_6942A4B78334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayDCStoPS.h"

class CCommand_PS_MaxonSerialV2;

class CGatewayEsam2ToMaxonSerialV2 : public CGatewayDCStoPS
{
public:
    CGatewayEsam2ToMaxonSerialV2();
    virtual ~CGatewayEsam2ToMaxonSerialV2();

    virtual CGateway* Clone();
    CGatewayEsam2ToMaxonSerialV2& operator=(CGatewayEsam2ToMaxonSerialV2& other);

    virtual BOOL ProcessCommand(CCommandRoot*p_pCommand, CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle);

private:
    void DeleteCommands();
    void InitCommands();

    //ObjectDictionary
    BOOL Process_ReadObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_WriteObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_InitiateSegmentedRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_InitiateSegmentedWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SegmentRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SegmentWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_AbortSegmentedTransfer(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);

    //GeneralGateway
    BOOL Process_SendCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_ReadCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_RequestCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SendNMTService(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);

    //Layer Setting Services
    BOOL Process_SendLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ReadLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);

	//OpenPCS History Data Commands
    BOOL Process_OpcsHistGetRange(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_OpcsHistGetRangeResult(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);

    //PS Layer
    BOOL PS_ProcessProtocol(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, BYTE p_ubOpCode, void* p_pDataBuffer, DWORD p_ulDataBufferLength, BYTE p_ubKeepLock, void** p_ppRetDataBuffer, DWORD* p_pulRetDataBufferLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_AbortProtocol(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    //Layer Parameter Stack
    BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

    //Helper Functions
    BOOL EvaluateErrorCode(BOOL p_oResult, DWORD p_ulDeviceErrorCode, CErrorInfo* p_pComErrorInfo, CErrorInfo* p_pCmdErrorInfo);
    BOOL CopyData(void*& p_pDest, void* p_pSource, DWORD p_ulSourceSize);
    BOOL CopyReturnData(void* p_pDest, DWORD p_ulDestSize, void*& p_pSource, DWORD& p_rulSourceSize);

    BOOL InitErrorHandling();

private:
    CCommand_PS_MaxonSerialV2* m_pCommand_ProcessProtocol;
    CCommand_PS_MaxonSerialV2* m_pCommand_AbortProtocol;
};


#endif // !defined(AFX_GATEWAYESAM2TOMAXONSERIALV2_H__10BC918F_ECD4_465F_A861_6942A4B78334__INCLUDED_)
