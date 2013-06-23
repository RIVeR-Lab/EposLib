// GatewayInfoteamSerialToRS232.h: Schnittstelle f�r die Klasse CGatewayInfoteamSerialToRS232.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayInfoteamSerialToRS232_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayInfoteamSerialToRS232_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "GatewayInfoteamSerialToI.h"

class CCommand_I_RS232;
class CInterfaceManagerBase;

class CGatewayInfoteamSerialToRS232 : public CGatewayInfoteamSerialToI
{
public:
	CGatewayInfoteamSerialToRS232();
	virtual ~CGatewayInfoteamSerialToRS232();

	virtual CGateway* Clone();
	CGatewayInfoteamSerialToRS232& operator=(CGatewayInfoteamSerialToRS232& other);

private:
	//Initialisation
	void DeleteCommands();
	void InitCommands();

	//Read Write Data
	BOOL I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo = NULL);
	BOOL I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo = NULL);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

	//Error Handling
	BOOL InitErrorHandling();

private:
	CCommand_I_RS232* m_pCommand_WriteData;
	CCommand_I_RS232* m_pCommand_ReadData;
};



#endif // !defined(AFX_GatewayInfoteamSerialToRS232_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
