// GatewayEposToMaxonSerialV1.h: Schnittstelle f�r die Klasse CGatewayEposToMaxonSerialV1.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYEPOSTOEPOSRS232_H__10BC918F_ECD4_465F_A861_6942A4B78334__INCLUDED_)
#define AFX_GATEWAYEPOSTOEPOSRS232_H__10BC918F_ECD4_465F_A861_6942A4B78334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayDCStoPS.h"
class CCommand_PS_MaxonSerialV1;

class CGatewayEposToMaxonSerialV1 : public CGatewayDCStoPS
{
public:
    CGatewayEposToMaxonSerialV1();
    virtual ~CGatewayEposToMaxonSerialV1();

    virtual CGateway* Clone();
    CGatewayEposToMaxonSerialV1& operator=(CGatewayEposToMaxonSerialV1& other);

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
    BOOL Process_SendNMTService(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_RequestCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadODObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadODObjectNextSegment(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_WriteODObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_WriteODObjectNextSegment(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);
    BOOL Process_AbortSegmentedTransfer(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle);

    BOOL PS_ProcessProtocol(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_SendFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
	BOOL PS_AbortProtocol(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo = NULL);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

    BOOL EvaluateErrorCode(BOOL oResult, DWORD dDeviceErrorCode, CErrorInfo* pComErrorInfo, CErrorInfo* pCmdErrorInfo);
    BOOL EvaluateOldErrorCode(BOOL oResult, WORD wDeviceErrorCode, CErrorInfo* pComErrorInfo, CErrorInfo* pCmdErrorInfo);

    BOOL CopyData(void*& pDest, void* pSource, DWORD sourceSize);
    BOOL CopyReturnData(void* pDest, DWORD destSize, void*& pSource, DWORD& sourceSize);

    BOOL InitErrorHandling();

private:
    CCommand_PS_MaxonSerialV1* m_pCommand_ProcessProtocol;
    CCommand_PS_MaxonSerialV1* m_pCommand_SendFrame;
	CCommand_PS_MaxonSerialV1* m_pCommand_AbortProtocol;
};


#endif // !defined(AFX_GATEWAYEPOSTOEPOSRS232_H__10BC918F_ECD4_465F_A861_6942A4B78334__INCLUDED_)
