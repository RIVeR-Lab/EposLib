// CommandSetStateMachine_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetStateMachine_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetStateMachine_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetStateMachine_VCS_Drive2::CCommandSetStateMachine_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_STATE_MACHINE;

    m_pCommand_ClearFault = NULL;
    m_pCommand_GetDisableState = NULL;
    m_pCommand_GetEnableState = NULL;
    m_pCommand_GetFaultState = NULL;
    m_pCommand_GetOperationMode = NULL;
    m_pCommand_GetQuickStopState = NULL;
    m_pCommand_SetDisableState = NULL;
    m_pCommand_SetEnableState = NULL;
    m_pCommand_SetOperationMode = NULL;
    m_pCommand_SetQuickStopState = NULL;

    InitCommands();
}

CCommandSetStateMachine_VCS_Drive2::~CCommandSetStateMachine_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_ClearFault(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ClearFault)
    {
        //Set Parameter Data
        m_pCommand_ClearFault->ResetStatus();
        m_pCommand_ClearFault->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_ClearFault->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ClearFault->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_GetDisableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsDisabled, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetDisableState)
    {
        //Set Parameter Data
        m_pCommand_GetDisableState->ResetStatus();
        m_pCommand_GetDisableState->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_GetDisableState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetDisableState->GetReturnParameterData(0, p_poIsDisabled, sizeof(*p_poIsDisabled));

        //Get ErrorCode
        m_pCommand_GetDisableState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_GetEnableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsEnabled, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetEnableState)
    {
        //Set Parameter Data
        m_pCommand_GetEnableState->ResetStatus();
        m_pCommand_GetEnableState->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_GetEnableState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetEnableState->GetReturnParameterData(0, p_poIsEnabled, sizeof(*p_poIsEnabled));

        //Get ErrorCode
        m_pCommand_GetEnableState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_GetFaultState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsInFault, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetFaultState)
    {
        //Set Parameter Data
        m_pCommand_GetFaultState->ResetStatus();
        m_pCommand_GetFaultState->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_GetFaultState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetFaultState->GetReturnParameterData(0, p_poIsInFault, sizeof(*p_poIsInFault));

        //Get ErrorCode
        m_pCommand_GetFaultState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_GetOperationMode(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, __int8* p_pbMode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetOperationMode)
    {
        //Set Parameter Data
        m_pCommand_GetOperationMode->ResetStatus();
        m_pCommand_GetOperationMode->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_GetOperationMode->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetOperationMode->GetReturnParameterData(0, p_pbMode, sizeof(*p_pbMode));

        //Get ErrorCode
        m_pCommand_GetOperationMode->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_GetQuickStopState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, BOOL* p_poIsQuickStoped, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetQuickStopState)
    {
        //Set Parameter Data
        m_pCommand_GetQuickStopState->ResetStatus();
        m_pCommand_GetQuickStopState->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_GetQuickStopState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetQuickStopState->GetReturnParameterData(0, p_poIsQuickStoped, sizeof(*p_poIsQuickStoped));

        //Get ErrorCode
        m_pCommand_GetQuickStopState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_SetDisableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetDisableState)
    {
        //Set Parameter Data
        m_pCommand_SetDisableState->ResetStatus();
        m_pCommand_SetDisableState->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_SetDisableState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetDisableState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_SetEnableState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetEnableState)
    {
        //Set Parameter Data
        m_pCommand_SetEnableState->ResetStatus();
        m_pCommand_SetEnableState->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_SetEnableState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetEnableState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_SetOperationMode(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, __int8 p_bMode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetOperationMode)
    {
        //Set Parameterdata
        m_pCommand_SetOperationMode->ResetStatus();
        m_pCommand_SetOperationMode->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));
        m_pCommand_SetOperationMode->SetParameterData(1, &p_bMode, sizeof(p_bMode));

        //ExecuteCommand
        oResult = m_pCommand_SetOperationMode->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetOperationMode->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStateMachine_VCS_Drive2::VCS_SetQuickStopState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetQuickStopState)
    {
        //Set Parameter Data
        m_pCommand_SetQuickStopState->ResetStatus();
        m_pCommand_SetQuickStopState->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));

        //ExecuteCommand
        oResult = m_pCommand_SetQuickStopState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetQuickStopState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetStateMachine_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init ClearFault
    m_pCommand_ClearFault = new CCommand_VCS_Drive2();
    m_pCommand_ClearFault->InitCommand(DRIVE2_CLEAR_FAULT);

    //Init GetDisableState
    m_pCommand_GetDisableState = new CCommand_VCS_Drive2();
    m_pCommand_GetDisableState->InitCommand(DRIVE2_GET_DISABLE_STATE);

    //Init GetEnableState
    m_pCommand_GetEnableState = new CCommand_VCS_Drive2();
    m_pCommand_GetEnableState->InitCommand(DRIVE2_GET_ENABLE_STATE);

    //Init GetFaultState
    m_pCommand_GetFaultState = new CCommand_VCS_Drive2();
    m_pCommand_GetFaultState->InitCommand(DRIVE2_GET_FAULT_STATE);

    //Init GetOperationMode
    m_pCommand_GetOperationMode = new CCommand_VCS_Drive2();
    m_pCommand_GetOperationMode->InitCommand(DRIVE2_GET_OPERATION_MODE);

    //Init GetQuickStopState
    m_pCommand_GetQuickStopState = new CCommand_VCS_Drive2();
    m_pCommand_GetQuickStopState->InitCommand(DRIVE2_GET_QUICK_STOP_STATE);

    //Init SetDisableState
    m_pCommand_SetDisableState = new CCommand_VCS_Drive2();
    m_pCommand_SetDisableState->InitCommand(DRIVE2_SET_DISABLE_STATE);

    //Init SetEnableState
    m_pCommand_SetEnableState = new CCommand_VCS_Drive2();
    m_pCommand_SetEnableState->InitCommand(DRIVE2_SET_ENABLE_STATE);

    //Init SetOperationMode
    m_pCommand_SetOperationMode = new CCommand_VCS_Drive2();
    m_pCommand_SetOperationMode->InitCommand(DRIVE2_SET_OPERATION_MODE);

    //Init SetQuickStopState
    m_pCommand_SetQuickStopState = new CCommand_VCS_Drive2();
    m_pCommand_SetQuickStopState->InitCommand(DRIVE2_SET_QUICK_STOP_STATE);
}

void CCommandSetStateMachine_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_ClearFault)
    {
        delete m_pCommand_ClearFault;
        m_pCommand_ClearFault = NULL;
    }
    if(m_pCommand_GetDisableState)
    {
        delete m_pCommand_GetDisableState;
        m_pCommand_GetDisableState = NULL;
    }
    if(m_pCommand_GetEnableState)
    {
        delete m_pCommand_GetEnableState;
        m_pCommand_GetEnableState = NULL;
    }
    if(m_pCommand_GetFaultState)
    {
        delete m_pCommand_GetFaultState;
        m_pCommand_GetFaultState = NULL;
    }
    if(m_pCommand_GetOperationMode)
    {
        delete m_pCommand_GetOperationMode;
        m_pCommand_GetOperationMode = NULL;
    }
    if(m_pCommand_GetQuickStopState)
    {
        delete m_pCommand_GetQuickStopState;
        m_pCommand_GetQuickStopState = NULL;
    }
    if(m_pCommand_SetDisableState)
    {
        delete m_pCommand_SetDisableState;
        m_pCommand_SetDisableState = NULL;
    }
    if(m_pCommand_SetEnableState)
    {
        delete m_pCommand_SetEnableState;
        m_pCommand_SetEnableState = NULL;
    }
    if(m_pCommand_SetOperationMode)
    {
        delete m_pCommand_SetOperationMode;
        m_pCommand_SetOperationMode = NULL;
    }
    if(m_pCommand_SetQuickStopState)
    {
        delete m_pCommand_SetQuickStopState;
        m_pCommand_SetQuickStopState = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetStateMachine_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;
    BOOL oCheckVisibility = FALSE;

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_ClearFault && !m_pCommand_ClearFault->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetEnableState && !m_pCommand_GetEnableState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetDisableState && !m_pCommand_GetDisableState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetFaultState && !m_pCommand_GetFaultState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetOperationMode && !m_pCommand_GetOperationMode->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetQuickStopState && !m_pCommand_GetQuickStopState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetEnableState && !m_pCommand_SetEnableState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetDisableState && !m_pCommand_SetDisableState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetOperationMode && !m_pCommand_SetOperationMode->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetQuickStopState && !m_pCommand_SetQuickStopState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetStateMachine_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_ClearFault) m_pCommand_ClearFault->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetDisableState) m_pCommand_GetDisableState->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetEnableState) m_pCommand_GetEnableState->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetFaultState) m_pCommand_GetFaultState->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetOperationMode) m_pCommand_GetOperationMode->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetQuickStopState) m_pCommand_GetQuickStopState->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetDisableState) m_pCommand_SetDisableState->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetEnableState) m_pCommand_SetEnableState->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetOperationMode) m_pCommand_SetOperationMode->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetQuickStopState) m_pCommand_SetQuickStopState->InitJournalManager(p_pJournalManager);
}

void CCommandSetStateMachine_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_ClearFault) m_pCommand_ClearFault->ResetJournalManager();
    if(m_pCommand_GetDisableState) m_pCommand_GetDisableState->ResetJournalManager();
    if(m_pCommand_GetEnableState) m_pCommand_GetEnableState->ResetJournalManager();
    if(m_pCommand_GetFaultState) m_pCommand_GetFaultState->ResetJournalManager();
    if(m_pCommand_GetOperationMode) m_pCommand_GetOperationMode->ResetJournalManager();
    if(m_pCommand_GetQuickStopState) m_pCommand_GetQuickStopState->ResetJournalManager();
    if(m_pCommand_SetDisableState) m_pCommand_SetDisableState->ResetJournalManager();
    if(m_pCommand_SetEnableState) m_pCommand_SetEnableState->ResetJournalManager();
    if(m_pCommand_SetOperationMode) m_pCommand_SetOperationMode->ResetJournalManager();
    if(m_pCommand_SetQuickStopState) m_pCommand_SetQuickStopState->ResetJournalManager();
}

BOOL CCommandSetStateMachine_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_ClearFault && !m_pCommand_ClearFault->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetDisableState && !m_pCommand_GetDisableState->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetEnableState && !m_pCommand_GetEnableState->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetFaultState && !m_pCommand_GetFaultState->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetOperationMode && !m_pCommand_GetOperationMode->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetQuickStopState && !m_pCommand_GetQuickStopState->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetDisableState && !m_pCommand_SetDisableState->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetEnableState && !m_pCommand_SetEnableState->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetOperationMode && !m_pCommand_SetOperationMode->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetQuickStopState && !m_pCommand_SetQuickStopState->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

