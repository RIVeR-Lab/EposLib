// GatewayEsamToCANopen.h: Schnittstelle f�r die Klasse CGatewayEsamToCANopen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayEsamToCANopen_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayEsamToCANopen_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayDCStoPS.h"

class CCommand_PS_CANopen;
class CCommand_DCS;
class CProtocolStackManagerBase;

class CGatewayEsamToCANopen : public CGatewayDCStoPS
{
public:
    CGatewayEsamToCANopen();
    virtual ~CGatewayEsamToCANopen();

    virtual CGateway* Clone();
    CGatewayEsamToCANopen& operator=(CGatewayEsamToCANopen& other);

    virtual BOOL ProcessCommand(CCommandRoot*pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

private:
    void DeleteCommands();
    void InitCommands();

    BOOL Process_ReadObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_WriteObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_InitiateSegmentedRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_InitiateSegmentedWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SegmentRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SegmentWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_AbortSegmentedTransfer(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendNMTService(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_RequestCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_Login(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_Logout(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SetState(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);

    BOOL PS_InitiateSDODownload(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oExpeditedTransfer, BOOL oSizeIndicated, BYTE uNonValidNbOfBytes, WORD wIndex, BYTE uSubIndex, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_DownloadSDOSegment(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BYTE uNonValidNbOfBytes, BOOL oNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, BOOL* poRetToggle, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_DownloadSDOSegment(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BYTE uNonValidNbOfBytes, BOOL oNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_InitiateSDOUpload(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, WORD wIndex, BYTE uSubIndex, BOOL* poExpeditedTransfer, BOOL* poSizeIndicated, BYTE* puNonValidNbOfBytes, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_UploadSDOSegment(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BOOL* poRetToggle, BYTE* puNonValidNbOfBytes, BOOL* poNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_AbortSDOTransfer(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, WORD wIndex, BYTE uSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_SendNMTService(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uCommandSpecifier, BYTE uNodeId, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_SendCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_ReadCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, DWORD p_ulTimeout, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_RequestCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

    BOOL EvaluateErrorCode(BOOL oResult, DWORD dDeviceErrorCode, CErrorInfo* pComErrorInfo, CErrorInfo* pCmdErrorInfo);
    BOOL CalculateDefaultSdoCobIds(BYTE uNodeId, DWORD* pdCobIdClientServer, DWORD* pdCobIdServerClient);

    BOOL InitErrorHandling();

private:
    CCommand_PS_CANopen* m_pCommand_InitiateSDODownload;
    CCommand_PS_CANopen* m_pCommand_DownloadSDOSegment;
    CCommand_PS_CANopen* m_pCommand_InitiateSDOUpload;
    CCommand_PS_CANopen* m_pCommand_UploadSDOSegment;
    CCommand_PS_CANopen* m_pCommand_AbortSDOTransfer;
    CCommand_PS_CANopen* m_pCommand_SendNMTService;
    CCommand_PS_CANopen* m_pCommand_SendCANFrame;
    CCommand_PS_CANopen* m_pCommand_ReadCANFrame;
    CCommand_PS_CANopen* m_pCommand_RequestCANFrame;

    BYTE m_uNodeId;
    WORD m_wIndex;
    BYTE m_uSubIndex;
    BOOL m_oToggle;
};

#endif // !defined(AFX_GatewayEsamToCANopen_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
