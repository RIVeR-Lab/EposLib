// GatewayMaxonSerialV2ToUSB.h: Schnittstelle f�r die Klasse CGatewayMaxonSerialV2ToUSB.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayMaxonSerialV2ToUSB_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayMaxonSerialV2ToUSB_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "GatewayMaxonSerialV2ToI.h"

class CCommand_I_USB;
class CInterfaceManagerBase;

class CGatewayMaxonSerialV2ToUSB : public CGatewayMaxonSerialV2ToI
{
public:
	CGatewayMaxonSerialV2ToUSB();
	virtual ~CGatewayMaxonSerialV2ToUSB();

	virtual CGateway* Clone();
	CGatewayMaxonSerialV2ToUSB& operator=(CGatewayMaxonSerialV2ToUSB& other);

private:
	//Protocol Implementation
	BOOL Process_ProcessProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle);
	BOOL EvaluateWriteRetry(BOOL p_oSendResult,CErrorInfo& p_rSendErrorInfo,BOOL p_oReceiveResult,CErrorInfo& p_rReceiveErrorInfo,BOOL& p_roExecuteRetry,CErrorInfo& p_rErrorInfo);

	//Initialisation
	void DeleteCommands();
	void InitCommands();

	//Reading Writing Data 
	BOOL I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo = NULL);
	BOOL I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo = NULL);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

	//Error Handling
	BOOL InitErrorHandling();

private:
	CCommand_I_USB* m_pCommand_WriteData;
	CCommand_I_USB* m_pCommand_ReadData;
};



#endif // !defined(AFX_GatewayMaxonSerialV2ToUSB_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
