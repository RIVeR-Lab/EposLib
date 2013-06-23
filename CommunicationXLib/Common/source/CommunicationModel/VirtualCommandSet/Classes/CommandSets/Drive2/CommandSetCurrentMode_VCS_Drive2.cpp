// CommandSetCurrentMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetCurrentMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetCurrentMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetCurrentMode_VCS_Drive2::CCommandSetCurrentMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_CURRENT_MODE;

    m_pCommand_GetCurrentMust = NULL;
    m_pCommand_SetCurrentMust = NULL;
    m_pCommand_ActivateAnalogCurrentSetpoint = NULL;
    m_pCommand_DeactivateAnalogCurrentSetpoint = NULL;
    m_pCommand_EnableAnalogCurrentSetpoint = NULL;
    m_pCommand_DisableAnalogCurrentSetpoint = NULL;

    InitCommands();
}

CCommandSetCurrentMode_VCS_Drive2::~CCommandSetCurrentMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetCurrentMode_VCS_Drive2::VCS_GetCurrentMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short* p_psCurrentMust, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetCurrentMust)
    {
        //Set Parameter Data
        m_pCommand_GetCurrentMust->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetCurrentMust->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetCurrentMust->GetReturnParameterData(0, p_psCurrentMust, sizeof(*p_psCurrentMust));

        //Get ErrorCode
        m_pCommand_GetCurrentMust->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetCurrentMode_VCS_Drive2::VCS_SetCurrentMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short p_sCurrentMust, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetCurrentMust)
    {
        //Set Parameter Data
        m_pCommand_SetCurrentMust->ResetStatus();
        m_pCommand_SetCurrentMust->SetParameterData(0, &p_sCurrentMust, sizeof(p_sCurrentMust));

        //ExecuteCommand
        oResult = m_pCommand_SetCurrentMust->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetCurrentMust->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetCurrentMode_VCS_Drive2::VCS_ActivateAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, float fScaling, short sOffset, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ActivateAnalogCurrentSetpoint)
    {
        //Set Parameter Data
        m_pCommand_ActivateAnalogCurrentSetpoint->ResetStatus();
        m_pCommand_ActivateAnalogCurrentSetpoint->SetParameterData(0, &p_usAnalogInputNumber, sizeof(p_usAnalogInputNumber));
        m_pCommand_ActivateAnalogCurrentSetpoint->SetParameterData(1, &fScaling, sizeof(fScaling));
        m_pCommand_ActivateAnalogCurrentSetpoint->SetParameterData(2, &sOffset, sizeof(sOffset));

        //ExecuteCommand
        oResult = m_pCommand_ActivateAnalogCurrentSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ActivateAnalogCurrentSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetCurrentMode_VCS_Drive2::VCS_DeactivateAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DeactivateAnalogCurrentSetpoint)
    {
        //Set Parameter Data
        m_pCommand_DeactivateAnalogCurrentSetpoint->ResetStatus();
        m_pCommand_DeactivateAnalogCurrentSetpoint->SetParameterData(0, &p_usAnalogInputNumber, sizeof(p_usAnalogInputNumber));

        //ExecuteCommand
        oResult = m_pCommand_DeactivateAnalogCurrentSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DeactivateAnalogCurrentSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetCurrentMode_VCS_Drive2::VCS_EnableAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_EnableAnalogCurrentSetpoint)
    {
        //Set Parameter Data
        m_pCommand_EnableAnalogCurrentSetpoint->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_EnableAnalogCurrentSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_EnableAnalogCurrentSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetCurrentMode_VCS_Drive2::VCS_DisableAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DisableAnalogCurrentSetpoint)
    {
        //Set Parameter Data
        m_pCommand_DisableAnalogCurrentSetpoint->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DisableAnalogCurrentSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DisableAnalogCurrentSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetCurrentMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init GetCurrentMust
    m_pCommand_GetCurrentMust = new CCommand_VCS_Drive2();
    m_pCommand_GetCurrentMust->InitCommand(DRIVE2_GET_CURRENT_MUST);

    //Init SetCurrentMust
    m_pCommand_SetCurrentMust = new CCommand_VCS_Drive2();
    m_pCommand_SetCurrentMust->InitCommand(DRIVE2_SET_CURRENT_MUST);

    //Init ActivateAnalogCurrentSetpoint
    m_pCommand_ActivateAnalogCurrentSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_ActivateAnalogCurrentSetpoint->InitCommand(DRIVE2_ACTIVATE_ANALOG_CURRENT_SETPOINT);

    //Init DeactivateAnalogCurrentSetpoint
    m_pCommand_DeactivateAnalogCurrentSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_DeactivateAnalogCurrentSetpoint->InitCommand(DRIVE2_DEACTIVATE_ANALOG_CURRENT_SETPOINT);

    //Init EnableAnalogCurrentSetpoint
    m_pCommand_EnableAnalogCurrentSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_EnableAnalogCurrentSetpoint->InitCommand(DRIVE2_ENABLE_ANALOG_CURRENT_SETPOINT);

    //Init DisableAnalogCurrentSetpoint
    m_pCommand_DisableAnalogCurrentSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_DisableAnalogCurrentSetpoint->InitCommand(DRIVE2_DISABLE_ANALOG_CURRENT_SETPOINT);
}

void CCommandSetCurrentMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_GetCurrentMust)
    {
        delete m_pCommand_GetCurrentMust;
        m_pCommand_GetCurrentMust = NULL;
    }

    if(m_pCommand_SetCurrentMust)
    {
        delete m_pCommand_SetCurrentMust;
        m_pCommand_SetCurrentMust = NULL;
    }

    if(m_pCommand_ActivateAnalogCurrentSetpoint)
    {
        delete m_pCommand_ActivateAnalogCurrentSetpoint;
        m_pCommand_ActivateAnalogCurrentSetpoint = NULL;
    }

    if(m_pCommand_DeactivateAnalogCurrentSetpoint)
    {
        delete m_pCommand_DeactivateAnalogCurrentSetpoint;
        m_pCommand_DeactivateAnalogCurrentSetpoint = NULL;
    }

    if(m_pCommand_EnableAnalogCurrentSetpoint)
    {
        delete m_pCommand_EnableAnalogCurrentSetpoint;
        m_pCommand_EnableAnalogCurrentSetpoint = NULL;
    }

    if(m_pCommand_DisableAnalogCurrentSetpoint)
    {
        delete m_pCommand_DisableAnalogCurrentSetpoint;
        m_pCommand_DisableAnalogCurrentSetpoint = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetCurrentMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetCurrentMust && !m_pCommand_GetCurrentMust->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetCurrentMust && !m_pCommand_SetCurrentMust->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ActivateAnalogCurrentSetpoint && !m_pCommand_ActivateAnalogCurrentSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DeactivateAnalogCurrentSetpoint && !m_pCommand_DeactivateAnalogCurrentSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_EnableAnalogCurrentSetpoint && !m_pCommand_EnableAnalogCurrentSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DisableAnalogCurrentSetpoint && !m_pCommand_DisableAnalogCurrentSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetCurrentMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetCurrentMust) m_pCommand_GetCurrentMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetCurrentMust) m_pCommand_SetCurrentMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ActivateAnalogCurrentSetpoint) m_pCommand_ActivateAnalogCurrentSetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DeactivateAnalogCurrentSetpoint) m_pCommand_DeactivateAnalogCurrentSetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_EnableAnalogCurrentSetpoint) m_pCommand_EnableAnalogCurrentSetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DisableAnalogCurrentSetpoint) m_pCommand_DisableAnalogCurrentSetpoint->InitJournalManager(p_pJournalManager);
}

void CCommandSetCurrentMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetCurrentMust) m_pCommand_GetCurrentMust->ResetJournalManager();
    if(m_pCommand_SetCurrentMust) m_pCommand_SetCurrentMust->ResetJournalManager();
    if(m_pCommand_ActivateAnalogCurrentSetpoint) m_pCommand_ActivateAnalogCurrentSetpoint->ResetJournalManager();
    if(m_pCommand_DeactivateAnalogCurrentSetpoint) m_pCommand_DeactivateAnalogCurrentSetpoint->ResetJournalManager();
    if(m_pCommand_EnableAnalogCurrentSetpoint) m_pCommand_EnableAnalogCurrentSetpoint->ResetJournalManager();
    if(m_pCommand_DisableAnalogCurrentSetpoint) m_pCommand_DisableAnalogCurrentSetpoint->ResetJournalManager();
}

BOOL CCommandSetCurrentMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetCurrentMust && !m_pCommand_GetCurrentMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetCurrentMust && !m_pCommand_SetCurrentMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ActivateAnalogCurrentSetpoint && !m_pCommand_ActivateAnalogCurrentSetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DeactivateAnalogCurrentSetpoint && !m_pCommand_DeactivateAnalogCurrentSetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_EnableAnalogCurrentSetpoint && !m_pCommand_EnableAnalogCurrentSetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DisableAnalogCurrentSetpoint && !m_pCommand_DisableAnalogCurrentSetpoint->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

