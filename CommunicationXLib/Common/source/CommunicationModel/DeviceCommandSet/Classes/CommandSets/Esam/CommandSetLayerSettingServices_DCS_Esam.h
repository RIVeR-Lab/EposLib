// CommandSetLayerSettingServices_DCS_Esam.h: Schnittstelle f�r die Klasse CCommandSetLayerSettingServices_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetLayerSettingServices_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
#define AFX_CommandSetLayerSettingServices_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_DCS.h"
#include <Classes/XXMLFile.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>

class CGateway;
class CCommand_DCS_Esam;
class CLayerManagerBase;

class CCommandSetLayerSettingServices_DCS_Esam : public CCommandSetBase_DCS
{
public:
	BOOL DCS_SendLSSFrame(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_ReadLSSFrame(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,WORD wTimeout,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo = NULL);

	CCommandSetLayerSettingServices_DCS_Esam();
	virtual ~CCommandSetLayerSettingServices_DCS_Esam();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);

	CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElementPart* pParentElement);

private:
	void DeleteCommands();
	void InitCommands();

	CCommand_DCS_Esam* m_pCommand_SendLSSFrame;
	CCommand_DCS_Esam* m_pCommand_ReadLSSFrame;
};

#endif // !defined(AFX_CommandSetLayerSettingServices_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
