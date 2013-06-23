#if !defined(AFX_Port_USB_H__95811EF2_3171_4F25_B8FD_52A0035BF3C4__INCLUDED_)
#define AFX_Port_USB_H__95811EF2_3171_4F25_B8FD_52A0035BF3C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>

class CCommand_I_USB;
class CJournalManagerBase;

class CPort_USB : public CPortBase
{
public:
	BOOL operator==(const CPort_USB& rPort);
	CPort_USB& operator=(CPort_USB& rSetting);

	CPort_USB();
	CPort_USB(CPort_USB& rObject);
	virtual ~CPort_USB();
	CPortBase* Clone();

	//Funktionalit�t
	BOOL GetCommands(CStdString* pCommandInfo);
	BOOL I_ReadData(HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo=NULL);
	BOOL I_WriteData(HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo=NULL);

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

private:
	BOOL InitErrorHandling();
	void DeleteCommands();
	void InitCommands();
	void InitCommands(CGatewayIToDrv* pGateway);
	BOOL InitGateway(CGatewayIToDrv* pGateway);
	void ResetPortMembers();
	void SetDefault();

	//ParameterSet
	BOOL InitParameterSet();

private:
	CCommand_I_USB* m_pCommand_ReadData;
	CCommand_I_USB* m_pCommand_WriteData;
};


#endif // !defined(AFX_Port_USB_H__95811EF2_3171_4F25_B8FD_52A0035BF3C4__INCLUDED_)
