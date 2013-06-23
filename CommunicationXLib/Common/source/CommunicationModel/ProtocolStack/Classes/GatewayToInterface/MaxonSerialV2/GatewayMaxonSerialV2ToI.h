// GatewayMaxonSerialV2ToI.h: Schnittstelle f�r die Klasse CGatewayMaxonSerialV2ToI.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayMaxonSerialV2ToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayMaxonSerialV2ToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

#include <Time/MmcHiResTimer.h>
#include "../BaseClasses/GatewayPStoI.h"
#include "Frame/MaxonSerialV2Frame.h"

class CInterfaceManagerBase;

class CGatewayMaxonSerialV2ToI : public CGatewayPStoI
{
public:
	CGatewayMaxonSerialV2ToI();
	virtual ~CGatewayMaxonSerialV2ToI();

	virtual CGateway* Clone();
	CGatewayMaxonSerialV2ToI& operator=(CGatewayMaxonSerialV2ToI& other);

	BOOL ProcessCommand(CCommandRoot*pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle);

protected:
	//Sending Receiving Frame
	BOOL SendFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc,CErrorInfo* pErrorInfo = NULL);
	BOOL ReceiveFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,BYTE* puOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc,DWORD dTimeout,CErrorInfo* pErrorInfo = NULL);

private:
	//Protocol Implementation
	virtual BOOL Process_ProcessProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	virtual BOOL Process_AbortProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);

	//Receive StateMachine
	BOOL InitWaitingForDleState(EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruDle,DWORD& p_rulReadBlockSize);
	BOOL InitWaitingForStxState(EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruStx,DWORD& p_rulReadBlockSize);
	BOOL InitWaitingForOpCodeState(EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruOpCode,DWORD& p_rulReadBlockSize,BOOL& p_roEscapeState);
	BOOL InitWaitingForLenState(EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruLen,DWORD& p_rulReadBlockSize,BOOL p_roEscapeState);
	BOOL InitWaitingForDataAndCrcState(EReceiveFrameState& p_rReceiveFrameState,CMaxonSerialV2Frame* p_pReceivingFrame,BYTE p_uOpCode,BYTE p_uLen, DWORD p_ulRetDataBufferSize, BYTE*& p_rpData,DWORD& p_rulNbOfDataBytesToRead,DWORD& p_rulReadBlockSize,BOOL& p_roEscapeState);
	BOOL ProcessWaitingForDle(CInterfaceManagerBase* p_pInterfaceManager,HANDLE p_hI_Handle,HANDLE p_hTransactionHandle,EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruDle,DWORD& p_rulReadBlockSize,CErrorInfo* p_pErrorInfo);
	BOOL ProcessWaitingForStx(CInterfaceManagerBase* p_pInterfaceManager,HANDLE p_hI_Handle,HANDLE p_hTransactionHandle,EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruStx,DWORD& p_rulReadBlockSize,CErrorInfo* p_pErrorInfo);
	BOOL ProcessWaitingForOpCode(CInterfaceManagerBase* p_pInterfaceManager,HANDLE p_hI_Handle,HANDLE p_hTransactionHandle,EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruOpCode,DWORD& p_rulReadBlockSize,BOOL& p_roEscapeState,CErrorInfo* p_pErrorInfo);
	BOOL ProcessWaitingForLen(CInterfaceManagerBase* p_pInterfaceManager,HANDLE p_hI_Handle,HANDLE p_hTransactionHandle,EReceiveFrameState& p_rReceiveFrameState,BYTE& p_ruLen,DWORD& p_rulReadBlockSize,BOOL& p_roEscapeState,CErrorInfo* p_pErrorInfo);
	BOOL ProcessWaitingForDataAndCrc(CInterfaceManagerBase* p_pInterfaceManager,HANDLE p_hI_Handle,HANDLE p_hTransactionHandle,EReceiveFrameState& p_rReceiveFrameState,BYTE*& p_rpData,DWORD& p_rulNbOfDataBytesToRead,DWORD& p_rulReadBlockSize,BOOL& p_roEscapeState,CErrorInfo* p_pErrorInfo);
	BOOL GetReceivedData(CInterfaceManagerBase* p_pInterfaceManager,HANDLE p_hI_Handle,HANDLE p_hTransactionHandle,BYTE& p_ruData,DWORD p_ulReadBlockSize);

	//BlockBuffer
	BOOL InitBlockBuffer(DWORD p_ulReadBlockSize);
	BOOL UpdateBlockBuffer(CInterfaceManagerBase* p_pInterfaceManager,HANDLE p_hI_Handle,HANDLE p_hTransactionHandle,DWORD p_ulReadBlockSize);
	BOOL DeleteBlockBuffer();
	BOOL GetDataFromBlockBuffer(BYTE& p_ruData);

	//Reading Writing Data 
	virtual BOOL I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo = NULL);
	BOOL I_WriteStuffedData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo = NULL);

	//Timer
	bool StartTime(double* pStartCount);
	DWORD StopTime(double startCount);

private:
	CMaxonSerialV2Frame* m_pSendingFrame;
	CMaxonSerialV2Frame* m_pReceivingFrame;

	BYTE* m_pBlockBuffer;
	DWORD m_ulBlockBufferSize;
	DWORD m_ulGetBlockBufferIndex;
	CMmcHiResTimer m_timer;
};

#endif // !defined(AFX_GatewayMaxonSerialV2ToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
