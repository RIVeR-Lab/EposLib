// CommandSetMotionInfo_VCS_Drive.cpp: Implementierung der Klasse CCommandSetMotionInfo_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetMotionInfo_VCS_Drive.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetMotionInfo_VCS_Drive::CCommandSetMotionInfo_VCS_Drive()
{
    m_strCommandSetName = COMMAND_SET_MOTION_INFO;

    m_pCommand_GetCurrentIs = NULL;
    m_pCommand_GetMovementState = NULL;
    m_pCommand_GetPositionIs = NULL;
    m_pCommand_GetVelocityIs = NULL;

    InitCommands();
}

CCommandSetMotionInfo_VCS_Drive::~CCommandSetMotionInfo_VCS_Drive()
{
    DeleteCommands();
}

BOOL CCommandSetMotionInfo_VCS_Drive::VCS_GetCurrentIs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short* p_psCurrentIs, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetCurrentIs)
    {
        //Set Parameter Data
        m_pCommand_GetCurrentIs->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetCurrentIs->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetCurrentIs->GetReturnParameterData(0, p_psCurrentIs, sizeof(*p_psCurrentIs));

        //Get ErrorCode
        m_pCommand_GetCurrentIs->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetMotionInfo_VCS_Drive::VCS_GetMovementState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poTargetReached, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetMovementState)
    {
        //Set Parameter Data
        m_pCommand_GetMovementState->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetMovementState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetMovementState->GetReturnParameterData(0, p_poTargetReached, sizeof(*p_poTargetReached));

        //Get ErrorCode
        m_pCommand_GetMovementState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetMotionInfo_VCS_Drive::VCS_GetPositionIs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long *p_plPositionIs, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetPositionIs)
    {
        //Set Parameter Data
        m_pCommand_GetPositionIs->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetPositionIs->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetPositionIs->GetReturnParameterData(0, p_plPositionIs, sizeof(*p_plPositionIs));

        //Get ErrorCode
        m_pCommand_GetPositionIs->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetMotionInfo_VCS_Drive::VCS_GetVelocityIs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long *p_plVelocityIs, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetVelocityIs)
    {
        //Set Parameter Data
        m_pCommand_GetVelocityIs->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetVelocityIs->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetVelocityIs->GetReturnParameterData(0, p_plVelocityIs, sizeof(*p_plVelocityIs));

        //Get ErrorCode
        m_pCommand_GetVelocityIs->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetMotionInfo_VCS_Drive::InitCommands()
{
    DeleteCommands();

    //Init GetCurrentIs
    m_pCommand_GetCurrentIs = new CCommand_VCS_Drive();
    m_pCommand_GetCurrentIs->InitCommand(DRIVE_GET_CURRENT_IS);

    //Init GetMovementState
    m_pCommand_GetMovementState = new CCommand_VCS_Drive();
    m_pCommand_GetMovementState->InitCommand(DRIVE_GET_MOVEMENT_STATE);

    //Init GetPositionIs
    m_pCommand_GetPositionIs = new CCommand_VCS_Drive();
    m_pCommand_GetPositionIs->InitCommand(DRIVE_GET_POSITION_IS);

    //Init GetVelocityIs
    m_pCommand_GetVelocityIs = new CCommand_VCS_Drive();
    m_pCommand_GetVelocityIs->InitCommand(DRIVE_GET_VELOCITY_IS);
}

void CCommandSetMotionInfo_VCS_Drive::DeleteCommands()
{
    if(m_pCommand_GetCurrentIs)
    {
        delete m_pCommand_GetCurrentIs;
        m_pCommand_GetCurrentIs = NULL;
    }

    if(m_pCommand_GetMovementState)
    {
        delete m_pCommand_GetMovementState;
        m_pCommand_GetMovementState = NULL;
    }

    if(m_pCommand_GetPositionIs)
    {
        delete m_pCommand_GetPositionIs;
        m_pCommand_GetPositionIs = NULL;
    }

    if(m_pCommand_GetVelocityIs)
    {
        delete m_pCommand_GetVelocityIs;
        m_pCommand_GetVelocityIs = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetMotionInfo_VCS_Drive::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetCurrentIs && !m_pCommand_GetCurrentIs->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetPositionIs && !m_pCommand_GetPositionIs->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetVelocityIs && !m_pCommand_GetVelocityIs->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetMovementState && !m_pCommand_GetMovementState->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetMotionInfo_VCS_Drive::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetCurrentIs) m_pCommand_GetCurrentIs->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetMovementState) m_pCommand_GetMovementState->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetPositionIs) m_pCommand_GetPositionIs->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetVelocityIs) m_pCommand_GetVelocityIs->InitJournalManager(p_pJournalManager);
}

void CCommandSetMotionInfo_VCS_Drive::ResetJournalManager()
{
    if(m_pCommand_GetCurrentIs) m_pCommand_GetCurrentIs->ResetJournalManager();
    if(m_pCommand_GetMovementState) m_pCommand_GetMovementState->ResetJournalManager();
    if(m_pCommand_GetPositionIs) m_pCommand_GetPositionIs->ResetJournalManager();
    if(m_pCommand_GetVelocityIs) m_pCommand_GetVelocityIs->ResetJournalManager();
}

BOOL CCommandSetMotionInfo_VCS_Drive::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetCurrentIs && !m_pCommand_GetCurrentIs->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetMovementState && !m_pCommand_GetMovementState->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetPositionIs && !m_pCommand_GetPositionIs->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetVelocityIs && !m_pCommand_GetVelocityIs->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

