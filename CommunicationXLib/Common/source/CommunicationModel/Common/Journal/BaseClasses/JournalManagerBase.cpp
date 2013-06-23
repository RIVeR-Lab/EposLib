// JournalManagerBase.cpp: Implementierung der Klasse CJournalManagerBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/Common/Journal/BaseClasses/JournalManagerBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CJournalManagerBase::CJournalManagerBase()
{
}

CJournalManagerBase::~CJournalManagerBase()
{
}

//*********************************************************************
HANDLE CJournalManagerBase::StartTransaction(HANDLE hKeyHandle, HANDLE hJournalHandle)
{
    return (HANDLE)0;
}

//*********************************************************************
BOOL CJournalManagerBase::StopTransaction(HANDLE hTransactionHandle)
{
    return FALSE;
}

//*********************************************************************
/*HANDLE CJournalManagerBase::StartJournal(HANDLE hKeyHandle, T_JournalCallback journalCallback)
{
    return NULL;
}

//*********************************************************************
HANDLE CJournalManagerBase::StartJournal(HANDLE hKeyHandle, CStdString fileName)
{
    return NULL;
}

//*********************************************************************
HANDLE CJournalManagerBase::StartJournal(HANDLE hKeyHandle, IJournalEvents* pIJournalEvents)
{
    return NULL;
}

//*********************************************************************
BOOL CJournalManagerBase::StopJournal(HANDLE hJournalHandle)
{
    return FALSE;
}*/

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent(ELayer eLayer, HANDLE hTransactionHandle, CStdString eventData)
{
    switch(eLayer)
    {
        case NO_LAYER                   : return FALSE;
        case INTERFACE_LAYER            : return TransactionEvent_InterfaceLayer(hTransactionHandle, eventData);
        case PROTOCOL_STACK_LAYER       : return TransactionEvent_ProtocolStackLayer(hTransactionHandle, eventData);
        case DEVICE_COMMAND_SET_LAYER   : return TransactionEvent_DCSLayer(hTransactionHandle, eventData);
        case VIRTUAL_COMMAND_SET_LAYER  : return TransactionEvent_VCSLayer(hTransactionHandle, eventData);
    }

    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent(ELayer eLayer, HANDLE hTransactionHandle, CCommandRoot* pCommand)
{
    switch(eLayer)
    {
        case NO_LAYER                   : return FALSE;
        case INTERFACE_LAYER            : return TransactionEvent_InterfaceLayer(hTransactionHandle, pCommand);
        case PROTOCOL_STACK_LAYER       : return TransactionEvent_ProtocolStackLayer(hTransactionHandle, pCommand);
        case DEVICE_COMMAND_SET_LAYER   : return TransactionEvent_DCSLayer(hTransactionHandle, pCommand);
        case VIRTUAL_COMMAND_SET_LAYER  : return TransactionEvent_VCSLayer(hTransactionHandle, pCommand);
    }

    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_VCSLayer(HANDLE hTransactionHandle, CStdString eventData)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_DCSLayer(HANDLE hTransactionHandle, CStdString eventData)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_ProtocolStackLayer(HANDLE hTransactionHandle, CStdString eventData)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_InterfaceLayer(HANDLE hTransactionHandle, CStdString eventData)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_VCSLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_DCSLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_ProtocolStackLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::TransactionEvent_InterfaceLayer(HANDLE hTransactionHandle, CCommandRoot* pCommand)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::StartJournalRunTime()
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::StopJournalRunTime()
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::StartCommandTime(CJournalTime *pThisTime)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::StopCommandTime(CJournalTime *pThisTime)
{
    return FALSE;
}

//*********************************************************************
BOOL CJournalManagerBase::IsActiveJournalListEmpty()
{
	return FALSE;
}
