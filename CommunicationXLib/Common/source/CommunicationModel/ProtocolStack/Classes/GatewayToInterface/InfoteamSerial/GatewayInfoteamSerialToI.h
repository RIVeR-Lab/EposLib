// GatewayInfoteamSerialToI.h: Schnittstelle f�r die Klasse CGatewayInfoteamSerialToI.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayInfoteamSerialToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayInfoteamSerialToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#define	OK_ACK					0xF0
#define	REP_ACK					0x0F

#include "../BaseClasses/GatewayPStoI.h"

class CInfoteamSerialSendingFrame;
class CInfoteamSerialReceivingFrame;
class CInterfaceManagerBase;

class CGatewayInfoteamSerialToI : public CGatewayPStoI
{
public:
	CGatewayInfoteamSerialToI();
	virtual ~CGatewayInfoteamSerialToI();

	virtual CGateway* Clone();
	CGatewayInfoteamSerialToI& operator=(CGatewayInfoteamSerialToI& other);

	BOOL ProcessCommand(CCommandRoot*pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle);

private:
	//Protocol Implementation
	BOOL Process_ProcessProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	BOOL Process_ProcessProtocolMaxon(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	BOOL Process_AbortProtocolMaxon(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	BOOL Process_SendChunk(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	BOOL Process_ReceiveData(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);

	//Send Receive Frames 
	BOOL SendFrameRepeated(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,DWORD dPackageSize,WORD wChunkSize,BYTE uLastChunkFlag,DWORD* pdChecksum,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo = NULL);
	BOOL ReceiveFrameRepeated(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle, DWORD p_ulMaxPackageSize, DWORD* pdRetPackageSize,DWORD* pdRetChecksum,void** ppRetDataBuffer,DWORD* pdRetDataBufferLength,DWORD dTimeout,CErrorInfo* pErrorInfo = NULL);

	BOOL SendFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,DWORD dPackageSize,WORD wChunkSize,BYTE uLastChunkFlag,DWORD* pdChecksum,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo = NULL);
	BOOL ReceiveFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle, DWORD p_ulMaxPackageSize, DWORD* pdRetPackageSize,DWORD* pdRetChecksum,void** ppRetDataBuffer,DWORD* pdRetDataBufferLength,DWORD dTimeout,BOOL oSendRepAck,CErrorInfo* pErrorInfo = NULL);

	//Read Write Data
	virtual BOOL I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo = NULL);

private:
	CInfoteamSerialSendingFrame* m_pSendingFrame;
	CInfoteamSerialReceivingFrame* m_pReceivingFrame;
};


#endif // !defined(AFX_GatewayInfoteamSerialToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
