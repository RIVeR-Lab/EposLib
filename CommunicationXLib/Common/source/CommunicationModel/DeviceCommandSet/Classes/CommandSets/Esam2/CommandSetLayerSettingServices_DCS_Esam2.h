// CommandSetLayerSettingServices_DCS_Esam2.h: Schnittstelle f�r die Klasse CCommandSetLayerSettingServices_DCS_Esam2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetLayerSettingServices_DCS_Esam2_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
#define AFX_CommandSetLayerSettingServices_DCS_Esam2_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_DCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_DCS_Esam2;
class CLayerManagerBase;

class CCommandSetLayerSettingServices_DCS_Esam2 : public CCommandSetBase_DCS
{
public:
	BOOL DCS_SendLSSFrame(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNetworkId,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_ReadLSSFrame(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNetworkId,WORD wTimeout,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo = NULL);

	CCommandSetLayerSettingServices_DCS_Esam2();
	virtual ~CCommandSetLayerSettingServices_DCS_Esam2();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);

	CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElementPart* pParentElement);

private:
	void DeleteCommands();
	void InitCommands();

	CCommand_DCS_Esam2* m_pCommand_SendLSSFrame;
	CCommand_DCS_Esam2* m_pCommand_ReadLSSFrame;
};

#endif // !defined(AFX_CommandSetLayerSettingServices_DCS_Esam2_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
