// GatewayMaxonSerialV1ToStandardI.h: Schnittstelle f�r die Klasse CGatewayMaxonSerialV1ToI.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayMaxonSerialV1ToStandardI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayMaxonSerialV1ToStandardI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayPStoI.h"
class CMaxonSerialV1Frame;
class CInterfaceManagerBase;

class CGatewayMaxonSerialV1ToI : public CGatewayPStoI
{
public:
	CGatewayMaxonSerialV1ToI();
	virtual ~CGatewayMaxonSerialV1ToI();

	virtual CGateway* Clone();
	CGatewayMaxonSerialV1ToI& operator=(CGatewayMaxonSerialV1ToI& other);

	BOOL ProcessCommand(CCommandRoot*pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle);

private:
	//Protocol Implementation
	virtual BOOL Process_ProcessProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	virtual BOOL Process_SendFrame(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	virtual BOOL Process_AbortProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);

	//Sending Receiving Frame
	BOOL SendFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc,CErrorInfo* pErrorInfo = NULL);
	BOOL ReceiveFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,BYTE* puOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc,DWORD dTimeout,CErrorInfo* pErrorInfo = NULL);

	//Reading Writing Data 
	virtual BOOL I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo = NULL);

private:
	CMaxonSerialV1Frame* m_pSendingFrame;
	CMaxonSerialV1Frame* m_pReceivingFrame;
};


#endif // !defined(AFX_GatewayMaxonSerialV1ToStandardI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
