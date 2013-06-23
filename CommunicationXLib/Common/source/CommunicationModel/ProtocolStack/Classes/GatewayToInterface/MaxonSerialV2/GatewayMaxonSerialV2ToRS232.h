// GatewayMaxonSerialV2ToRS232.h: Schnittstelle f�r die Klasse CGatewayMaxonSerialV2ToRS232.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayMaxonSerialV2ToRS232_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayMaxonSerialV2ToRS232_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>



#include "GatewayMaxonSerialV2ToI.h"

class CCommand_I_RS232;
class CInterfaceManagerBase;

class CGatewayMaxonSerialV2ToRS232 : public CGatewayMaxonSerialV2ToI
{
public:
	CGatewayMaxonSerialV2ToRS232();
	virtual ~CGatewayMaxonSerialV2ToRS232();

	virtual CGateway* Clone();
	CGatewayMaxonSerialV2ToRS232& operator=(CGatewayMaxonSerialV2ToRS232& other);

private:
	//Initialisation
	void DeleteCommands();
	void InitCommands();

	//Reading Writing Data 
	BOOL I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo = NULL);
	BOOL I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo = NULL);

	//Error Handling
	BOOL InitErrorHandling();

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

private:
	CCommand_I_RS232* m_pCommand_WriteData;
	CCommand_I_RS232* m_pCommand_ReadData;
};


#endif // !defined(AFX_GatewayMaxonSerialV2ToRS232_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
