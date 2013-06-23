// JournalManagerBase.h: Schnittstelle f�r die Klasse CJournalManagerBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JournalManagerBase_H__9F36D41F_0DF4_4C91_AE54_36209F8786F5__INCLUDED_)
#define AFX_JournalManagerBase_H__9F36D41F_0DF4_4C91_AE54_36209F8786F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <MmcTypeDefinition.h>

class CJournalTime;
class CCommandRoot;
struct IJournalEvents;

class CJournalManagerBase
{
public:
    //Journal run time
    virtual BOOL StopJournalRunTime();
    virtual BOOL StartJournalRunTime();
    virtual BOOL StartCommandTime(CJournalTime* pThisTime);
    virtual BOOL StopCommandTime(CJournalTime* pThisTime);
	virtual BOOL IsActiveJournalListEmpty();

    //IJournal
    virtual BOOL StopJournal(HANDLE hJournalHandle) = 0;
    virtual HANDLE StartJournal(HANDLE hKeyHandle, T_JournalCallback journalCallback) = 0;
    virtual HANDLE StartJournal(HANDLE hKeyHandle, CStdString fileName) = 0;
    virtual HANDLE StartJournal(HANDLE hKeyHandle, IJournalEvents* pIJournalEvents) = 0;

    //IJournalTransaction
    virtual HANDLE StartTransaction(HANDLE hKeyHandle, HANDLE hJournalHandle);
    virtual BOOL StopTransaction(HANDLE hTransactionHandle);

    //TransactionEvents
    BOOL TransactionEvent(ELayer eLayer, HANDLE hTransactionHandle, CStdString eventData);
    virtual BOOL TransactionEvent_VCSLayer(HANDLE hTransactionHandle, CStdString eventData);
    virtual BOOL TransactionEvent_DCSLayer(HANDLE hTransactionHandle, CStdString eventData);
    virtual BOOL TransactionEvent_ProtocolStackLayer(HANDLE hTransactionHandle, CStdString eventData);
    virtual BOOL TransactionEvent_InterfaceLayer(HANDLE hTransactionHandle, CStdString eventData);

    BOOL TransactionEvent(ELayer eLayer, HANDLE hTransactionHandle, CCommandRoot* pCommand);
    virtual BOOL TransactionEvent_VCSLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand);
    virtual BOOL TransactionEvent_DCSLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand);
    virtual BOOL TransactionEvent_ProtocolStackLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand);
    virtual BOOL TransactionEvent_InterfaceLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand);

    CJournalManagerBase();
    virtual ~CJournalManagerBase();
};

#endif // !defined(AFX_JournalManagerBase_H__9F36D41F_0DF4_4C91_AE54_36209F8786F5__INCLUDED_)
