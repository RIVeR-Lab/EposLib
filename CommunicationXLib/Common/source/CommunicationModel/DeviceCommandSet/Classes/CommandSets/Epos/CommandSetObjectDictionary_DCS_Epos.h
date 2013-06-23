// CommandSetObjectDictionary_DCS_Epos.h: Schnittstelle f�r die Klasse CCommandSetObjectDictionary_DCS_Epos.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetObjectDictionary_DCS_Epos_H__97E8AE61_3CC8_4950_899B_85C927B35923__INCLUDED_)
#define AFX_CommandSetObjectDictionary_DCS_Epos_H__97E8AE61_3CC8_4950_899B_85C927B35923__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_DCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_DCS_Epos;
class CLayerManagerBase;

class CCommandSetObjectDictionary_DCS_Epos : public CCommandSetBase_DCS
{
public:
//Funktionalit�t fuer alten RS232 Befehlssatz (Firmware SW 0x2000)
	BOOL DCS_ReadODObject(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,BYTE* pDataBuffer,WORD wMaxNumberOfBytesToRead,WORD* pwNumberOfBytesRead,WORD* pwSegmentedRead,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_ReadODObjectNextSegment(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,BYTE* pDataBuffer,WORD wMaxNumberOfBytesToRead,WORD* pwNumberOfBytesRead,WORD* pwSegmentedRead,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_WriteODObject(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,BYTE* pDataBuffer,WORD wNumberOfBytesToWrite,BOOL oSignedDataType,WORD* pwNumberOfBytesWritten,WORD* pwSegmentedWrite,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_WriteODObjectNextSegment(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,BYTE* pDataBuffer,WORD wNumberOfBytesToWrite,BOOL oSignedDataType,WORD* pwNumberOfBytesWritten,WORD* pwSegmentedWrite,CErrorInfo* pErrorInfo = NULL);

//Funktionalit�t fuer neuen RS232 Befehlssatz (Gateway support, Firmware >= SW 0x2001)
	BOOL DCS_ReadObject(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,BYTE* pObjectBuffer,DWORD dObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_WriteObject(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,BYTE* pObjectBuffer,DWORD dObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_InitiateSegmentedRead(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_InitiateSegmentedWrite(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,DWORD dObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SegmentedRead(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,BOOL oToggle,BOOL* poMoreSegments,BYTE* pSegmentBuffer,DWORD dSegmentBufferLength,DWORD* pdSegmentLengthRead,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SegmentedWrite(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,BOOL oToggle,BOOL oMoreSegments,BYTE* pSegmentBuffer,DWORD dSegmentLength,DWORD* pdSegmentLengthWritten,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_AbortSegmentedTransfer(CLayerManagerBase* pManager,HANDLE hHandle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,DWORD dAbortCode,CErrorInfo* pErrorInfo = NULL);

	CCommandSetObjectDictionary_DCS_Epos();
	virtual ~CCommandSetObjectDictionary_DCS_Epos();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

    virtual BOOL InitGateway(CGateway *pGateway);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement);

private:
	void DeleteCommands();
	void InitCommands();

	CCommand_DCS_Epos* m_pCommand_WriteODObject;
	CCommand_DCS_Epos* m_pCommand_WriteODObjectNextSegment;
	CCommand_DCS_Epos* m_pCommand_WriteObject;
	CCommand_DCS_Epos* m_pCommand_InitiateSegmentedWrite;
	CCommand_DCS_Epos* m_pCommand_SegmentedWrite;

	CCommand_DCS_Epos* m_pCommand_ReadODObject;
	CCommand_DCS_Epos* m_pCommand_ReadODObjectNextSegment;
	CCommand_DCS_Epos* m_pCommand_ReadObject;
	CCommand_DCS_Epos* m_pCommand_InitiateSegmentedRead;
	CCommand_DCS_Epos* m_pCommand_SegmentedRead;

	CCommand_DCS_Epos* m_pCommand_AbortSegmentedTransfer;
};

#endif // !defined(AFX_CommandSetObjectDictionary_DCS_Epos_H__97E8AE61_3CC8_4950_899B_85C927B35923__INCLUDED_)
