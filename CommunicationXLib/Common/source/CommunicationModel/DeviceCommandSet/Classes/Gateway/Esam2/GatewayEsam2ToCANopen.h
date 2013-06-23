// GatewayEsam2ToCANopen.h: Schnittstelle f�r die Klasse CGatewayEsam2ToCANopen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYESAM2TOCANOPEN_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GATEWAYESAM2TOCANOPEN_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayDCStoPS.h"

class CCommand_DCS;
class CCommand_PS_CANopen;
class CProtocolStackManagerBase;

class CGatewayEsam2ToCANopen : public CGatewayDCStoPS
{
public:
    CGatewayEsam2ToCANopen();
    virtual ~CGatewayEsam2ToCANopen();

    virtual CGateway* Clone();
    CGatewayEsam2ToCANopen& operator=(CGatewayEsam2ToCANopen& other);

    virtual BOOL ProcessCommand(CCommandRoot*p_pCommand, CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle);

private:
    void DeleteCommands();
    void InitCommands();

    //Object Dictionary
    BOOL Process_ReadObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_WriteObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_InitiateSegmentedRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_InitiateSegmentedWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SegmentRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SegmentWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_AbortSegmentedTransfer(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);

    //General Gateway
    BOOL Process_SendCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_ReadCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_RequestCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SendNMTService(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);

    //Layer Setting Services
    BOOL Process_SendLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_ReadLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle);

    //Device Parameter Functions
    BOOL GetNodeId(CCommand_DCS* p_pCommand, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);
	BOOL GetRemoteNetworkId(CCommand_DCS* p_pCommand, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);
	BOOL GetRemoteNodeId(CCommand_DCS* p_pCommand, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);

    //PS Layer
    BOOL PS_InitiateSDODownload(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, BOOL p_oExpeditedTransfer, BOOL p_oSizeIndicated, BYTE p_ubNonValidNbOfBytes, WORD p_usIndex, BYTE p_ubSubIndex, void* p_pDataBuffer, DWORD p_ulDataBufferLength, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_DownloadSDOSegment(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, BOOL p_oToggle, BYTE p_ubNonValidNbOfBytes, BOOL p_oNoMoreSegments, void* p_pSegDataBuffer, DWORD p_ulSegDataBufferLength, BOOL* p_poRetToggle, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo = NULL);
//    BOOL PS_DownloadSDOSegment(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, BOOL p_oToggle, BYTE p_ubNonValidNbOfBytes, BOOL p_oNoMoreSegments, void* p_pSegDataBuffer, DWORD p_ulSegDataBufferLength, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_InitiateSDOUpload(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, WORD p_usIndex, BYTE p_ubSubIndex, BOOL* p_poExpeditedTransfer, BOOL* p_poSizeIndicated, BYTE* p_pubNonValidNbOfBytes, void* p_pDataBuffer, DWORD p_ulDataBufferLength, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_UploadSDOSegment(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, BOOL p_oToggle, BOOL* p_poRetToggle, BYTE* p_pubNonValidNbOfBytes, BOOL* p_poNoMoreSegments, void* p_pSegDataBuffer, DWORD p_ulSegDataBufferLength, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_NetworkIndication(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, WORD p_usTargetNetworkId, BYTE p_ubTargetNodeId, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_AbortSDOTransfer(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulAbortCode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_SendNMTService(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, BYTE p_ubCommandSpecifier, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_SendCANFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobId, BYTE p_ubLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_ReadCANFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobId, BYTE p_ubLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_RequestCANFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobId, BYTE p_ubLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_SendLSSFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL PS_ReadLSSFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, WORD p_usTimeout, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo = NULL);

    //Layer Parameter Stack
    BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

    //Helper Functions
    BOOL EvaluateErrorCode(BOOL p_oResult, DWORD p_ulDeviceErrorCode, CErrorInfo* p_pComErrorInfo, CErrorInfo* p_pCmdErrorInfo);
    BOOL CalculateDefaultSdoCobIds(BYTE p_ubNodeId, DWORD* p_pulCobIdClientServer, DWORD* p_pulCobIdServerClient);
	BOOL GetRequestNmtParameter(WORD p_usNodeIdentifier, WORD p_usCmdSpecifier, BYTE& p_rubSubIndex, DWORD& p_rulWriteAccessValue, DWORD& p_rulTimeout);

    BOOL InitErrorHandling();

private:
    CCommand_PS_CANopen* m_pCommand_InitiateSDODownload;
    CCommand_PS_CANopen* m_pCommand_DownloadSDOSegment;
    CCommand_PS_CANopen* m_pCommand_InitiateSDOUpload;
    CCommand_PS_CANopen* m_pCommand_UploadSDOSegment;
    CCommand_PS_CANopen* m_pCommand_NetworkIndication;
    CCommand_PS_CANopen* m_pCommand_AbortSDOTransfer;
    CCommand_PS_CANopen* m_pCommand_SendNMTService;
    CCommand_PS_CANopen* m_pCommand_SendCANFrame;
    CCommand_PS_CANopen* m_pCommand_ReadCANFrame;
    CCommand_PS_CANopen* m_pCommand_RequestCANFrame;
    CCommand_PS_CANopen* m_pCommand_SendLSSFrame;
    CCommand_PS_CANopen* m_pCommand_ReadLSSFrame;

    WORD m_usIndex;
    BYTE m_ubSubIndex;
};

#endif // !defined(AFX_GATEWAYESAM2TOCANOPEN_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
