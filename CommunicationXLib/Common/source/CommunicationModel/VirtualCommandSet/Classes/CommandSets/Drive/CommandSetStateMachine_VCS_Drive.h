// CommandSetStateMachine_VCS_Drive.h: Schnittstelle f�r die Klasse CCommandSetStateMachine_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetStateMachine_VCS_Drive_H__C9618274_651E_43F9_944B_C9B1BE9690CB__INCLUDED_)
#define AFX_CommandSetStateMachine_VCS_Drive_H__C9618274_651E_43F9_944B_C9B1BE9690CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_VCS_Drive;
class CLayerManagerBase;

class CCommandSetStateMachine_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetFaultState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsInFault, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ClearFault(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_GetQuickStopState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsQuickStoped, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetQuickStopState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_GetEnableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsEnabled, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetEnableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_GetDisableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsDisabled, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetDisableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_GetOperationMode(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, __int8* p_pbMode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetOperationMode(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, __int8 p_bMode, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetStateMachine_VCS_Drive();
    virtual ~CCommandSetStateMachine_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive* m_pCommand_ClearFault;
    CCommand_VCS_Drive* m_pCommand_GetDisableState;
    CCommand_VCS_Drive* m_pCommand_GetEnableState;
    CCommand_VCS_Drive* m_pCommand_GetFaultState;
    CCommand_VCS_Drive* m_pCommand_GetOperationMode;
    CCommand_VCS_Drive* m_pCommand_GetQuickStopState;
    CCommand_VCS_Drive* m_pCommand_SetDisableState;
    CCommand_VCS_Drive* m_pCommand_SetEnableState;
    CCommand_VCS_Drive* m_pCommand_SetOperationMode;
    CCommand_VCS_Drive* m_pCommand_SetQuickStopState;
};

#endif // !defined(AFX_CommandSetStateMachine_VCS_Drive_H__C9618274_651E_43F9_944B_C9B1BE9690CB__INCLUDED_)
