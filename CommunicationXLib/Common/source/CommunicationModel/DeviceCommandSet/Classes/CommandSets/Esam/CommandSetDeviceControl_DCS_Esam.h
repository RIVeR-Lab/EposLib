// CommandSetDeviceControl_DCS_Esam.h: Schnittstelle f�r die Klasse CCommandSetDeviceControl_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetDeviceControl_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
#define AFX_CommandSetDeviceControl_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_DCS.h"
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_DCS_Esam;
class CLayerManagerBase;

class CCommandSetDeviceControl_DCS_Esam : public CCommandSetBase_DCS
{
public:
	BOOL DCS_Login(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,CErrorInfo* pErrorInfo);
	BOOL DCS_Logout(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,CErrorInfo* pErrorInfo);
	BOOL DCS_SetState(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uRunCommand,CErrorInfo* pErrorInfo);

	CCommandSetDeviceControl_DCS_Esam();
	virtual ~CCommandSetDeviceControl_DCS_Esam();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);
	CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElementPart* pParentElement);

private:
	void DeleteCommands();
	void InitCommands();

	CCommand_DCS_Esam* m_pCommand_Login;
	CCommand_DCS_Esam* m_pCommand_Logout;
	CCommand_DCS_Esam* m_pCommand_SetState;
};

#endif // !defined(AFX_CommandSetDeviceControl_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
