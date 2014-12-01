// CommandSetProfilePositionMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetProfilePositionMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetProfilePositionMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetProfilePositionMode_VCS_Drive2::CCommandSetProfilePositionMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_PROFILE_POSITION_MODE;

    m_pCommand_GetPositionProfile = NULL;
    m_pCommand_GetTargetPosition = NULL;
    m_pCommand_HaltPositionMovement = NULL;
    m_pCommand_MoveToPosition = NULL;
    m_pCommand_SetPositionProfile = NULL;
    m_pCommand_EnablePositionWindow = NULL;
    m_pCommand_DisablePositionWindow = NULL;

    InitCommands();
}

CCommandSetProfilePositionMode_VCS_Drive2::~CCommandSetProfilePositionMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::VCS_GetPositionProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulProfileVelocity, DWORD* p_pulProfileAcceleration, DWORD* p_pulProfileDeceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetPositionProfile)
    {
        //Set Parameterdata
        m_pCommand_GetPositionProfile->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetPositionProfile->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameterdata
        m_pCommand_GetPositionProfile->GetReturnParameterData(0, p_pulProfileVelocity, sizeof(*p_pulProfileVelocity));
        m_pCommand_GetPositionProfile->GetReturnParameterData(1, p_pulProfileAcceleration, sizeof(*p_pulProfileAcceleration));
        m_pCommand_GetPositionProfile->GetReturnParameterData(2, p_pulProfileDeceleration, sizeof(*p_pulProfileDeceleration));

        //Get ErrorCode
        m_pCommand_GetPositionProfile->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::VCS_GetTargetPosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int *p_plTargetPosition, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetTargetPosition)
    {
        //Set Parameter Data
        m_pCommand_GetTargetPosition->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetTargetPosition->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetTargetPosition->GetReturnParameterData(0, p_plTargetPosition, sizeof(*p_plTargetPosition));

        //Get ErrorCode
        m_pCommand_GetTargetPosition->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::VCS_HaltPositionMovement(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_HaltPositionMovement)
    {
        //Set Parameterdata
        m_pCommand_HaltPositionMovement->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_HaltPositionMovement->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_HaltPositionMovement->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::VCS_MoveToPosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lTargetPosition, BOOL p_oAbsolute, BOOL p_oImmediately, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_MoveToPosition)
    {
        //Set Parameterdata
        m_pCommand_MoveToPosition->ResetStatus();
        m_pCommand_MoveToPosition->SetParameterData(0, &p_lTargetPosition, sizeof(p_lTargetPosition));
        m_pCommand_MoveToPosition->SetParameterData(1, &p_oAbsolute, sizeof(p_oAbsolute));
        m_pCommand_MoveToPosition->SetParameterData(2, &p_oImmediately, sizeof(p_oImmediately));

        //ExecuteCommand
        oResult = m_pCommand_MoveToPosition->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_MoveToPosition->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::VCS_SetPositionProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulProfileVelocity, DWORD p_ulProfileAcceleration, DWORD p_ulProfileDeceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetPositionProfile)
    {
        //Set Parameter Data
        m_pCommand_SetPositionProfile->ResetStatus();
        m_pCommand_SetPositionProfile->SetParameterData(0, &p_ulProfileVelocity, sizeof(p_ulProfileVelocity));
        m_pCommand_SetPositionProfile->SetParameterData(1, &p_ulProfileAcceleration, sizeof(p_ulProfileAcceleration));
        m_pCommand_SetPositionProfile->SetParameterData(2, &p_ulProfileDeceleration, sizeof(p_ulProfileDeceleration));

        //ExecuteCommand
        oResult = m_pCommand_SetPositionProfile->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetPositionProfile->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::VCS_EnablePositionWindow(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulPositionWindow, WORD p_usPositionWindowTime, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_EnablePositionWindow)
    {
        //Set Parameter Data
        m_pCommand_EnablePositionWindow->ResetStatus();
        m_pCommand_EnablePositionWindow->SetParameterData(0, &p_ulPositionWindow, sizeof(p_ulPositionWindow));
        m_pCommand_EnablePositionWindow->SetParameterData(1, &p_usPositionWindowTime, sizeof(p_usPositionWindowTime));

        //ExecuteCommand
        oResult = m_pCommand_EnablePositionWindow->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_EnablePositionWindow->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::VCS_DisablePositionWindow(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DisablePositionWindow)
    {
        //Set Parameter Data
        m_pCommand_DisablePositionWindow->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DisablePositionWindow->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DisablePositionWindow->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetProfilePositionMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init command GetPositionProfile
    m_pCommand_GetPositionProfile = new CCommand_VCS_Drive2();
    m_pCommand_GetPositionProfile->InitCommand(DRIVE2_GET_POSITION_PROFILE);

    //Init command GetTargetPosition
    m_pCommand_GetTargetPosition = new CCommand_VCS_Drive2();
    m_pCommand_GetTargetPosition->InitCommand(DRIVE2_GET_TARGET_POSITION);

    //Init command HaltPositionMovement
    m_pCommand_HaltPositionMovement = new CCommand_VCS_Drive2();
    m_pCommand_HaltPositionMovement->InitCommand(DRIVE2_HALT_POSITION_MOVEMENT);

    //Init command MoveToPosition
    m_pCommand_MoveToPosition = new CCommand_VCS_Drive2();
    m_pCommand_MoveToPosition->InitCommand(DRIVE2_MOVE_TO_POSITION);

    //Init SetPositionProfile
    m_pCommand_SetPositionProfile = new CCommand_VCS_Drive2();
    m_pCommand_SetPositionProfile->InitCommand(DRIVE2_SET_POSITION_PROFILE);

    //Init EnablePositionWindow
    m_pCommand_EnablePositionWindow = new CCommand_VCS_Drive2();
    m_pCommand_EnablePositionWindow->InitCommand(DRIVE2_ENABLE_POSITION_WINDOW);

    //Init DisablePositionWindow
    m_pCommand_DisablePositionWindow = new CCommand_VCS_Drive2();
    m_pCommand_DisablePositionWindow->InitCommand(DRIVE2_DISABLE_POSITION_WINDOW);
}

void CCommandSetProfilePositionMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_GetPositionProfile)
    {
        delete m_pCommand_GetPositionProfile;
        m_pCommand_GetPositionProfile = NULL;
    }

    if(m_pCommand_GetTargetPosition)
    {
        delete m_pCommand_GetTargetPosition;
        m_pCommand_GetTargetPosition = NULL;
    }

    if(m_pCommand_HaltPositionMovement)
    {
        delete m_pCommand_HaltPositionMovement;
        m_pCommand_HaltPositionMovement = NULL;
    }

    if(m_pCommand_MoveToPosition)
    {
        delete m_pCommand_MoveToPosition;
        m_pCommand_MoveToPosition = NULL;
    }

    if(m_pCommand_SetPositionProfile)
    {
        delete m_pCommand_SetPositionProfile;
        m_pCommand_SetPositionProfile = NULL;
    }

    if(m_pCommand_EnablePositionWindow)
    {
        delete m_pCommand_EnablePositionWindow;
        m_pCommand_EnablePositionWindow = NULL;
    }

    if(m_pCommand_DisablePositionWindow)
    {
        delete m_pCommand_DisablePositionWindow;
        m_pCommand_DisablePositionWindow = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetProfilePositionMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetPositionProfile && !m_pCommand_GetPositionProfile->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetTargetPosition && !m_pCommand_GetTargetPosition->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetPositionProfile && !m_pCommand_SetPositionProfile->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_MoveToPosition && !m_pCommand_MoveToPosition->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_HaltPositionMovement && !m_pCommand_HaltPositionMovement->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_EnablePositionWindow && !m_pCommand_EnablePositionWindow->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DisablePositionWindow && !m_pCommand_DisablePositionWindow->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetProfilePositionMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetPositionProfile) m_pCommand_GetPositionProfile->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetTargetPosition) m_pCommand_GetTargetPosition->InitJournalManager(p_pJournalManager);
    if(m_pCommand_HaltPositionMovement) m_pCommand_HaltPositionMovement->InitJournalManager(p_pJournalManager);
    if(m_pCommand_MoveToPosition) m_pCommand_MoveToPosition->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetPositionProfile) m_pCommand_SetPositionProfile->InitJournalManager(p_pJournalManager);
    if(m_pCommand_EnablePositionWindow) m_pCommand_EnablePositionWindow->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DisablePositionWindow) m_pCommand_DisablePositionWindow->InitJournalManager(p_pJournalManager);
}

void CCommandSetProfilePositionMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetPositionProfile) m_pCommand_GetPositionProfile->ResetJournalManager();
    if(m_pCommand_GetTargetPosition) m_pCommand_GetTargetPosition->ResetJournalManager();
    if(m_pCommand_HaltPositionMovement) m_pCommand_HaltPositionMovement->ResetJournalManager();
    if(m_pCommand_MoveToPosition) m_pCommand_MoveToPosition->ResetJournalManager();
    if(m_pCommand_SetPositionProfile) m_pCommand_SetPositionProfile->ResetJournalManager();
    if(m_pCommand_EnablePositionWindow) m_pCommand_EnablePositionWindow->ResetJournalManager();
    if(m_pCommand_DisablePositionWindow) m_pCommand_DisablePositionWindow->ResetJournalManager();
}

BOOL CCommandSetProfilePositionMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetPositionProfile && !m_pCommand_GetPositionProfile->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetTargetPosition && !m_pCommand_GetTargetPosition->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_HaltPositionMovement && !m_pCommand_HaltPositionMovement->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_MoveToPosition && !m_pCommand_MoveToPosition->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetPositionProfile && !m_pCommand_SetPositionProfile->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_EnablePositionWindow && !m_pCommand_EnablePositionWindow->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DisablePositionWindow && !m_pCommand_DisablePositionWindow->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

