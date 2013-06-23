// CommandSetVelocityMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetVelocityMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetVelocityMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetVelocityMode_VCS_Drive2::CCommandSetVelocityMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_VELOCITY_MODE;

    m_pCommand_GetVelocityMust = NULL;
    m_pCommand_SetVelocityMust = NULL;
    m_pCommand_ActivateAnalogVelocitySetpoint = NULL;
    m_pCommand_DeactivateAnalogVelocitySetpoint = NULL;
    m_pCommand_EnableAnalogVelocitySetpoint = NULL;
    m_pCommand_DisableAnalogVelocitySetpoint = NULL;

    InitCommands();
}

CCommandSetVelocityMode_VCS_Drive2::~CCommandSetVelocityMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetVelocityMode_VCS_Drive2::VCS_GetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long *p_plVelocityMust, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetVelocityMust)
    {
        //Set Parameter Data
        m_pCommand_GetVelocityMust->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetVelocityMust->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetVelocityMust->GetReturnParameterData(0, p_plVelocityMust, sizeof(*p_plVelocityMust));

        //Get ErrorCode
        m_pCommand_GetVelocityMust->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetVelocityMode_VCS_Drive2::VCS_SetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long p_lVelocityMust, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetVelocityMust)
    {
        //Set Parameter Data
        m_pCommand_SetVelocityMust->ResetStatus();
        m_pCommand_SetVelocityMust->SetParameterData(0, &p_lVelocityMust, sizeof(p_lVelocityMust));

        //ExecuteCommand
        oResult = m_pCommand_GetVelocityMust->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SetVelocityMust->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}
BOOL CCommandSetVelocityMode_VCS_Drive2::VCS_ActivateAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, float fScaling, short sOffset, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ActivateAnalogVelocitySetpoint)
    {
        //Set Parameter Data
        m_pCommand_ActivateAnalogVelocitySetpoint->ResetStatus();
        m_pCommand_ActivateAnalogVelocitySetpoint->SetParameterData(0, &p_usAnalogInputNumber, sizeof(p_usAnalogInputNumber));
        m_pCommand_ActivateAnalogVelocitySetpoint->SetParameterData(1, &fScaling, sizeof(fScaling));
        m_pCommand_ActivateAnalogVelocitySetpoint->SetParameterData(2, &sOffset, sizeof(sOffset));

        //ExecuteCommand
        oResult = m_pCommand_ActivateAnalogVelocitySetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ActivateAnalogVelocitySetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetVelocityMode_VCS_Drive2::VCS_DeactivateAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DeactivateAnalogVelocitySetpoint)
    {
        //Set Parameter Data
        m_pCommand_DeactivateAnalogVelocitySetpoint->ResetStatus();
        m_pCommand_DeactivateAnalogVelocitySetpoint->SetParameterData(0, &p_usAnalogInputNumber, sizeof(p_usAnalogInputNumber));

        //ExecuteCommand
        oResult = m_pCommand_DeactivateAnalogVelocitySetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DeactivateAnalogVelocitySetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetVelocityMode_VCS_Drive2::VCS_EnableAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_EnableAnalogVelocitySetpoint)
    {
        //Set Parameter Data
        m_pCommand_EnableAnalogVelocitySetpoint->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_EnableAnalogVelocitySetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_EnableAnalogVelocitySetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetVelocityMode_VCS_Drive2::VCS_DisableAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DisableAnalogVelocitySetpoint)
    {
        //Set Parameter Data
        m_pCommand_DisableAnalogVelocitySetpoint->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DisableAnalogVelocitySetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DisableAnalogVelocitySetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetVelocityMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init GetVelocityMust
    m_pCommand_GetVelocityMust = new CCommand_VCS_Drive2();
    m_pCommand_GetVelocityMust->InitCommand(DRIVE2_GET_VELOCITY_MUST);

    //Init SetVelocityMust
    m_pCommand_SetVelocityMust = new CCommand_VCS_Drive2;
    m_pCommand_SetVelocityMust->InitCommand(DRIVE2_SET_VELOCITY_MUST);
    //Init ActivateAnalogVelocitySetpoint
    m_pCommand_ActivateAnalogVelocitySetpoint = new CCommand_VCS_Drive2();
    m_pCommand_ActivateAnalogVelocitySetpoint->InitCommand(DRIVE2_ACTIVATE_ANALOG_VELOCITY_SETPOINT);

    //Init DeactivateAnalogVelocitySetpoint
    m_pCommand_DeactivateAnalogVelocitySetpoint = new CCommand_VCS_Drive2();
    m_pCommand_DeactivateAnalogVelocitySetpoint->InitCommand(DRIVE2_DEACTIVATE_ANALOG_VELOCITY_SETPOINT);

    //Init EnableAnalogVelocitySetpoint
    m_pCommand_EnableAnalogVelocitySetpoint = new CCommand_VCS_Drive2();
    m_pCommand_EnableAnalogVelocitySetpoint->InitCommand(DRIVE2_ENABLE_ANALOG_VELOCITY_SETPOINT);

    //Init DisableAnalogVelocitySetpoint
    m_pCommand_DisableAnalogVelocitySetpoint = new CCommand_VCS_Drive2();
    m_pCommand_DisableAnalogVelocitySetpoint->InitCommand(DRIVE2_DISABLE_ANALOG_VELOCITY_SETPOINT);
}

void CCommandSetVelocityMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_GetVelocityMust)
    {
        delete m_pCommand_GetVelocityMust;
        m_pCommand_GetVelocityMust = NULL;
    }
    if(m_pCommand_SetVelocityMust)
    {
        delete m_pCommand_SetVelocityMust;
        m_pCommand_SetVelocityMust = NULL;
    }

    if(m_pCommand_ActivateAnalogVelocitySetpoint)
    {
        delete m_pCommand_ActivateAnalogVelocitySetpoint;
        m_pCommand_ActivateAnalogVelocitySetpoint = NULL;
    }

    if(m_pCommand_DeactivateAnalogVelocitySetpoint)
    {
        delete m_pCommand_DeactivateAnalogVelocitySetpoint;
        m_pCommand_DeactivateAnalogVelocitySetpoint = NULL;
    }

    if(m_pCommand_EnableAnalogVelocitySetpoint)
    {
        delete m_pCommand_EnableAnalogVelocitySetpoint;
        m_pCommand_EnableAnalogVelocitySetpoint = NULL;
    }

    if(m_pCommand_DisableAnalogVelocitySetpoint)
    {
        delete m_pCommand_DisableAnalogVelocitySetpoint;
        m_pCommand_DisableAnalogVelocitySetpoint = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetVelocityMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;
    BOOL oCheckVisibility = FALSE;

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSet");
        pElement->AttributeToValue["Name"] = m_strCommandSetName;

        //Command Elements
        if(m_pCommand_GetVelocityMust && !m_pCommand_GetVelocityMust->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetVelocityMust && !m_pCommand_SetVelocityMust->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ActivateAnalogVelocitySetpoint && !m_pCommand_ActivateAnalogVelocitySetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DeactivateAnalogVelocitySetpoint && !m_pCommand_DeactivateAnalogVelocitySetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_EnableAnalogVelocitySetpoint && !m_pCommand_EnableAnalogVelocitySetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DisableAnalogVelocitySetpoint && !m_pCommand_DisableAnalogVelocitySetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetVelocityMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetVelocityMust) m_pCommand_GetVelocityMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetVelocityMust) m_pCommand_SetVelocityMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ActivateAnalogVelocitySetpoint) m_pCommand_ActivateAnalogVelocitySetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DeactivateAnalogVelocitySetpoint) m_pCommand_DeactivateAnalogVelocitySetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_EnableAnalogVelocitySetpoint) m_pCommand_EnableAnalogVelocitySetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DisableAnalogVelocitySetpoint) m_pCommand_DisableAnalogVelocitySetpoint->InitJournalManager(p_pJournalManager);
}

void CCommandSetVelocityMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetVelocityMust) m_pCommand_GetVelocityMust->ResetJournalManager();
    if(m_pCommand_SetVelocityMust) m_pCommand_SetVelocityMust->ResetJournalManager();
    if(m_pCommand_ActivateAnalogVelocitySetpoint) m_pCommand_ActivateAnalogVelocitySetpoint->ResetJournalManager();
    if(m_pCommand_DeactivateAnalogVelocitySetpoint) m_pCommand_DeactivateAnalogVelocitySetpoint->ResetJournalManager();
    if(m_pCommand_EnableAnalogVelocitySetpoint) m_pCommand_EnableAnalogVelocitySetpoint->ResetJournalManager();
    if(m_pCommand_DisableAnalogVelocitySetpoint) m_pCommand_DisableAnalogVelocitySetpoint->ResetJournalManager();
}

BOOL CCommandSetVelocityMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetVelocityMust && !m_pCommand_GetVelocityMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetVelocityMust && !m_pCommand_SetVelocityMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ActivateAnalogVelocitySetpoint && !m_pCommand_ActivateAnalogVelocitySetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DeactivateAnalogVelocitySetpoint && !m_pCommand_DeactivateAnalogVelocitySetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_EnableAnalogVelocitySetpoint && !m_pCommand_EnableAnalogVelocitySetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DisableAnalogVelocitySetpoint && !m_pCommand_DisableAnalogVelocitySetpoint->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

