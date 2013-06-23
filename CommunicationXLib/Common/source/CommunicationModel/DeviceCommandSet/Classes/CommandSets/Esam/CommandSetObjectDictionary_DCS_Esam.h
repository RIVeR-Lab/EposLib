// CommandSetObjectDictionary_DCS_Esam.h: Schnittstelle f�r die Klasse CCommandSetObjectDictionary_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetObjectDictionary_DCS_Esam_H__97E8AE61_3CC8_4950_899B_85C927B35923__INCLUDED_)
#define AFX_CommandSetObjectDictionary_DCS_Esam_H__97E8AE61_3CC8_4950_899B_85C927B35923__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_DCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_DCS_Esam;
class CLayerManagerBase;

class CCommandSetObjectDictionary_DCS_Esam : public CCommandSetBase_DCS
{
public:
	BOOL DCS_ReadObject(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,BYTE uNodeId,WORD wIndex,BYTE uSubIndex,BYTE* pObjectBuffer,DWORD dObjectBufferLength,DWORD* pdObjectLengthRead,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_InitiateSegmentedRead(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,BYTE uNodeId,WORD wIndex,BYTE uSubIndex,DWORD* pdObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SegmentedRead(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,BOOL* poLastDataSegment,BYTE* pSegmentBuffer,DWORD dSegmentBufferLength,DWORD* pdSegmentLengthRead,CErrorInfo* pErrorInfo = NULL);

	BOOL DCS_WriteObject(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,BYTE uNodeId,WORD wIndex,BYTE uSubIndex,BYTE* pObjectBuffer,DWORD dObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_InitiateSegmentedWrite(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,BYTE uNodeId,WORD wIndex,BYTE uSubIndex,DWORD dObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SegmentedWrite(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,BYTE uPortNumber,BYTE* pSegmentBuffer,DWORD dSegmentLength,CErrorInfo* pErrorInfo = NULL);

	BOOL DCS_AbortSegmentedTransfer(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,DWORD dAbortCode,CErrorInfo* pErrorInfo = NULL);

	CCommandSetObjectDictionary_DCS_Esam();
	virtual ~CCommandSetObjectDictionary_DCS_Esam();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);

	CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElementPart* pParentElement);

private:
	void DeleteCommands();
	void InitCommands();

	CCommand_DCS_Esam* m_pCommand_WriteObject;
	CCommand_DCS_Esam* m_pCommand_InitiateSegmentedWrite;
	CCommand_DCS_Esam* m_pCommand_SegmentedWrite;

	CCommand_DCS_Esam* m_pCommand_ReadObject;
	CCommand_DCS_Esam* m_pCommand_InitiateSegmentedRead;
	CCommand_DCS_Esam* m_pCommand_SegmentedRead;

	CCommand_DCS_Esam* m_pCommand_AbortSegmentedTransfer;
};

#endif // !defined(AFX_CommandSetObjectDictionary_DCS_Esam_H__97E8AE61_3CC8_4950_899B_85C927B35923__INCLUDED_)
