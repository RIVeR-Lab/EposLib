// CommandSetProfileVelocityMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetProfileVelocityMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetProfileVelocityMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetProfileVelocityMode_VCS_Drive2::CCommandSetProfileVelocityMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_PROFILE_VELOCITY_MODE;

    m_pCommand_GetTargetVelocity = NULL;
    m_pCommand_GetVelocityProfile = NULL;
    m_pCommand_HaltVelocityMovement = NULL;
    m_pCommand_MoveWithVelocity = NULL;
    m_pCommand_SetVelocityProfile = NULL;
    m_pCommand_EnableVelocityWindow = NULL;
    m_pCommand_DisableVelocityWindow = NULL;

    InitCommands();
}

CCommandSetProfileVelocityMode_VCS_Drive2::~CCommandSetProfileVelocityMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::VCS_GetTargetVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int *p_plTargetVelocity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetTargetVelocity)
    {
        //Set Parameter Data
        m_pCommand_GetTargetVelocity->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetTargetVelocity->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetTargetVelocity->GetReturnParameterData(0, p_plTargetVelocity, sizeof(*p_plTargetVelocity));

        //Get ErrorCode
        m_pCommand_GetTargetVelocity->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::VCS_GetVelocityProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pdProfileAcceleration, DWORD* p_pdProfileDeceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetVelocityProfile)
    {
        //Set Parameter Data
        m_pCommand_GetVelocityProfile->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetVelocityProfile->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetVelocityProfile->GetReturnParameterData(0, p_pdProfileAcceleration, sizeof(*p_pdProfileAcceleration));
        m_pCommand_GetVelocityProfile->GetReturnParameterData(1, p_pdProfileDeceleration, sizeof(*p_pdProfileDeceleration));

        //Get ErrorCode
        m_pCommand_GetVelocityProfile->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::VCS_HaltVelocityMovement(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_HaltVelocityMovement)
    {
        //Set Parameter Data
        m_pCommand_HaltVelocityMovement->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_HaltVelocityMovement->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_HaltVelocityMovement->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::VCS_MoveWithVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lTargetVelocity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_MoveWithVelocity)
    {
        //Set Parameter Data
        m_pCommand_MoveWithVelocity->ResetStatus();
        m_pCommand_MoveWithVelocity->SetParameterData(0, &p_lTargetVelocity, sizeof(p_lTargetVelocity));

        //ExecuteCommand
        oResult = m_pCommand_HaltVelocityMovement->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_MoveWithVelocity->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::VCS_SetVelocityProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulProfileAcceleration, DWORD p_ulProfileDeceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetVelocityProfile)
    {
        //Set Parameter Data
        m_pCommand_SetVelocityProfile->ResetStatus();
        m_pCommand_SetVelocityProfile->SetParameterData(0, &p_ulProfileAcceleration, sizeof(p_ulProfileAcceleration));
        m_pCommand_SetVelocityProfile->SetParameterData(1, &p_ulProfileDeceleration, sizeof(p_ulProfileDeceleration));

        //ExecuteCommand
        oResult = m_pCommand_HaltVelocityMovement->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetVelocityProfile->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::VCS_EnableVelocityWindow(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulVelocityWindow, WORD p_usVelocityWindowTime, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_EnableVelocityWindow)
    {
        //Set Parameter Data
        m_pCommand_EnableVelocityWindow->ResetStatus();
        m_pCommand_EnableVelocityWindow->SetParameterData(0, &p_ulVelocityWindow, sizeof(p_ulVelocityWindow));
        m_pCommand_EnableVelocityWindow->SetParameterData(1, &p_usVelocityWindowTime, sizeof(p_usVelocityWindowTime));

        //ExecuteCommand
        oResult = m_pCommand_EnableVelocityWindow->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_EnableVelocityWindow->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::VCS_DisableVelocityWindow(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DisableVelocityWindow)
    {
        //Set Parameter Data
        m_pCommand_DisableVelocityWindow->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DisableVelocityWindow->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DisableVelocityWindow->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetProfileVelocityMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init GetTargetVelocity
    m_pCommand_GetTargetVelocity = new CCommand_VCS_Drive2();
    m_pCommand_GetTargetVelocity->InitCommand(DRIVE2_GET_TARGET_VELOCITY);

    //Init GetVelocityProfile
    m_pCommand_GetVelocityProfile = new CCommand_VCS_Drive2();
    m_pCommand_GetVelocityProfile->InitCommand(DRIVE2_GET_VELOCITY_PROFILE);

    //Init HaltVelocityMovement
    m_pCommand_HaltVelocityMovement = new CCommand_VCS_Drive2();
    m_pCommand_HaltVelocityMovement->InitCommand(DRIVE2_HALT_VELOCITY_MOVEMENT);

    //Init MoveWithVelocity
    m_pCommand_MoveWithVelocity = new CCommand_VCS_Drive2();
    m_pCommand_MoveWithVelocity->InitCommand(DRIVE2_MOVE_WITH_VELOCITY);

    //Init SetVelocityProfile
    m_pCommand_SetVelocityProfile = new CCommand_VCS_Drive2();
    m_pCommand_SetVelocityProfile->InitCommand(DRIVE2_SET_VELOCITY_PROFILE);

    //Init EnableVelocityWindow
    m_pCommand_EnableVelocityWindow = new CCommand_VCS_Drive2();
    m_pCommand_EnableVelocityWindow->InitCommand(DRIVE2_ENABLE_VELOCITY_WINDOW);

    //Init DisableVelocityWindow
    m_pCommand_DisableVelocityWindow = new CCommand_VCS_Drive2();
    m_pCommand_DisableVelocityWindow->InitCommand(DRIVE2_DISABLE_VELOCITY_WINDOW);
}

void CCommandSetProfileVelocityMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_GetTargetVelocity)
    {
        delete m_pCommand_GetTargetVelocity;
        m_pCommand_GetTargetVelocity = NULL;
    }
    if(m_pCommand_GetVelocityProfile)
    {
        delete m_pCommand_GetVelocityProfile;
        m_pCommand_GetVelocityProfile = NULL;
    }
    if(m_pCommand_HaltVelocityMovement)
    {
        delete m_pCommand_HaltVelocityMovement;
        m_pCommand_HaltVelocityMovement = NULL;
    }
    if(m_pCommand_MoveWithVelocity)
    {
        delete m_pCommand_MoveWithVelocity;
        m_pCommand_MoveWithVelocity = NULL;
    }
    if(m_pCommand_SetVelocityProfile)
    {
        delete m_pCommand_SetVelocityProfile;
        m_pCommand_SetVelocityProfile = NULL;
    }

    if(m_pCommand_EnableVelocityWindow)
    {
        delete m_pCommand_EnableVelocityWindow;
        m_pCommand_EnableVelocityWindow = NULL;
    }

    if(m_pCommand_DisableVelocityWindow)
    {
        delete m_pCommand_DisableVelocityWindow;
        m_pCommand_DisableVelocityWindow = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetProfileVelocityMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetTargetVelocity && !m_pCommand_GetTargetVelocity->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetVelocityProfile && !m_pCommand_GetVelocityProfile->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetVelocityProfile && !m_pCommand_SetVelocityProfile->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_MoveWithVelocity && !m_pCommand_MoveWithVelocity->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_HaltVelocityMovement && !m_pCommand_HaltVelocityMovement->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_EnableVelocityWindow && !m_pCommand_EnableVelocityWindow->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DisableVelocityWindow && !m_pCommand_DisableVelocityWindow->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetProfileVelocityMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetTargetVelocity) m_pCommand_GetTargetVelocity->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetVelocityProfile) m_pCommand_GetVelocityProfile->InitJournalManager(p_pJournalManager);
    if(m_pCommand_HaltVelocityMovement) m_pCommand_HaltVelocityMovement->InitJournalManager(p_pJournalManager);
    if(m_pCommand_MoveWithVelocity) m_pCommand_MoveWithVelocity->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetVelocityProfile) m_pCommand_SetVelocityProfile->InitJournalManager(p_pJournalManager);
    if(m_pCommand_EnableVelocityWindow) m_pCommand_EnableVelocityWindow->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DisableVelocityWindow) m_pCommand_DisableVelocityWindow->InitJournalManager(p_pJournalManager);
}

void CCommandSetProfileVelocityMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetTargetVelocity) m_pCommand_GetTargetVelocity->ResetJournalManager();
    if(m_pCommand_GetVelocityProfile) m_pCommand_GetVelocityProfile->ResetJournalManager();
    if(m_pCommand_HaltVelocityMovement) m_pCommand_HaltVelocityMovement->ResetJournalManager();
    if(m_pCommand_MoveWithVelocity) m_pCommand_MoveWithVelocity->ResetJournalManager();
    if(m_pCommand_SetVelocityProfile) m_pCommand_SetVelocityProfile->ResetJournalManager();
    if(m_pCommand_EnableVelocityWindow) m_pCommand_EnableVelocityWindow->ResetJournalManager();
    if(m_pCommand_DisableVelocityWindow) m_pCommand_DisableVelocityWindow->ResetJournalManager();
}

BOOL CCommandSetProfileVelocityMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetTargetVelocity && !m_pCommand_GetTargetVelocity->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetVelocityProfile && !m_pCommand_GetVelocityProfile->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_HaltVelocityMovement && !m_pCommand_HaltVelocityMovement->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_MoveWithVelocity && !m_pCommand_MoveWithVelocity->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetVelocityProfile && !m_pCommand_SetVelocityProfile->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_EnableVelocityWindow && !m_pCommand_EnableVelocityWindow->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DisableVelocityWindow && !m_pCommand_DisableVelocityWindow->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

