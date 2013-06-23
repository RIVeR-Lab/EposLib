// CommandSetPositionMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetPositionMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetPositionMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetPositionMode_VCS_Drive2::CCommandSetPositionMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_POSITION_MODE;

    m_pCommand_GetPositionMust = NULL;
    m_pCommand_SetPositionMust = NULL;
    m_pCommand_ActivateAnalogPositionSetpoint = NULL;
    m_pCommand_DeactivateAnalogPositionSetpoint = NULL;
    m_pCommand_EnableAnalogPositionSetpoint = NULL;
    m_pCommand_DisableAnalogPositionSetpoint = NULL;

    InitCommands();
}

CCommandSetPositionMode_VCS_Drive2::~CCommandSetPositionMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetPositionMode_VCS_Drive2::VCS_GetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short *p_psPositionMust, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetPositionMust)
    {
        //Set Parameter Data
        m_pCommand_GetPositionMust->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetPositionMust->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetPositionMust->GetReturnParameterData(0, p_psPositionMust, sizeof(p_psPositionMust));

        //Get ErrorCode
        m_pCommand_GetPositionMust->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetPositionMode_VCS_Drive2::VCS_SetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short p_sPositionMust, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetPositionMust)
    {
        //Set Parameter Data
        m_pCommand_SetPositionMust->ResetStatus();
        m_pCommand_SetPositionMust->SetParameterData(0, &p_sPositionMust, sizeof(p_sPositionMust));

        //ExecuteCommand
        oResult = m_pCommand_SetPositionMust->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetPositionMust->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetPositionMode_VCS_Drive2::VCS_ActivateAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, float fScaling, short sOffset, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ActivateAnalogPositionSetpoint)
    {
        //Set Parameter Data
        m_pCommand_ActivateAnalogPositionSetpoint->ResetStatus();
        m_pCommand_ActivateAnalogPositionSetpoint->SetParameterData(0, &p_usAnalogInputNumber, sizeof(p_usAnalogInputNumber));
        m_pCommand_ActivateAnalogPositionSetpoint->SetParameterData(1, &fScaling, sizeof(fScaling));
        m_pCommand_ActivateAnalogPositionSetpoint->SetParameterData(2, &sOffset, sizeof(sOffset));

        //ExecuteCommand
        oResult = m_pCommand_ActivateAnalogPositionSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ActivateAnalogPositionSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetPositionMode_VCS_Drive2::VCS_DeactivateAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DeactivateAnalogPositionSetpoint)
    {
        //Set Parameter Data
        m_pCommand_DeactivateAnalogPositionSetpoint->ResetStatus();
        m_pCommand_DeactivateAnalogPositionSetpoint->SetParameterData(0, &p_usAnalogInputNumber, sizeof(p_usAnalogInputNumber));

        //ExecuteCommand
        oResult = m_pCommand_DeactivateAnalogPositionSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DeactivateAnalogPositionSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetPositionMode_VCS_Drive2::VCS_EnableAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_EnableAnalogPositionSetpoint)
    {
        //Set Parameter Data
        m_pCommand_EnableAnalogPositionSetpoint->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_EnableAnalogPositionSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_EnableAnalogPositionSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetPositionMode_VCS_Drive2::VCS_DisableAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DisableAnalogPositionSetpoint)
    {
        //Set Parameter Data
        m_pCommand_DisableAnalogPositionSetpoint->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DisableAnalogPositionSetpoint->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DisableAnalogPositionSetpoint->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetPositionMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init GetPositionMust
    m_pCommand_GetPositionMust = new CCommand_VCS_Drive2();
    m_pCommand_GetPositionMust->InitCommand(DRIVE2_GET_POSITION_MUST);

    //Init SetPostionMust
    m_pCommand_SetPositionMust = new CCommand_VCS_Drive2();
    m_pCommand_SetPositionMust->InitCommand(DRIVE2_SET_POSITION_MUST);

    //Init ActivateAnalogPositionSetpoint
    m_pCommand_ActivateAnalogPositionSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_ActivateAnalogPositionSetpoint->InitCommand(DRIVE2_ACTIVATE_ANALOG_POSITION_SETPOINT);

    //Init DeactivateAnalogPositionSetpoint
    m_pCommand_DeactivateAnalogPositionSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_DeactivateAnalogPositionSetpoint->InitCommand(DRIVE2_DEACTIVATE_ANALOG_POSITION_SETPOINT);

    //Init EnableAnalogPositionSetpoint
    m_pCommand_EnableAnalogPositionSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_EnableAnalogPositionSetpoint->InitCommand(DRIVE2_ENABLE_ANALOG_POSITION_SETPOINT);

    //Init DisableAnalogPositionSetpoint
    m_pCommand_DisableAnalogPositionSetpoint = new CCommand_VCS_Drive2();
    m_pCommand_DisableAnalogPositionSetpoint->InitCommand(DRIVE2_DISABLE_ANALOG_POSITION_SETPOINT);
}

void CCommandSetPositionMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_GetPositionMust)
    {
        delete m_pCommand_GetPositionMust;
        m_pCommand_GetPositionMust = NULL;
    }

    if(m_pCommand_SetPositionMust)
    {
        delete m_pCommand_SetPositionMust;
        m_pCommand_SetPositionMust = NULL;
    }
    if(m_pCommand_ActivateAnalogPositionSetpoint)
    {
        delete m_pCommand_ActivateAnalogPositionSetpoint;
        m_pCommand_ActivateAnalogPositionSetpoint = NULL;
    }

    if(m_pCommand_DeactivateAnalogPositionSetpoint)
    {
        delete m_pCommand_DeactivateAnalogPositionSetpoint;
        m_pCommand_DeactivateAnalogPositionSetpoint = NULL;
    }

    if(m_pCommand_EnableAnalogPositionSetpoint)
    {
        delete m_pCommand_EnableAnalogPositionSetpoint;
        m_pCommand_EnableAnalogPositionSetpoint = NULL;
    }

    if(m_pCommand_DisableAnalogPositionSetpoint)
    {
        delete m_pCommand_DisableAnalogPositionSetpoint;
        m_pCommand_DisableAnalogPositionSetpoint = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetPositionMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetPositionMust && !m_pCommand_GetPositionMust->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetPositionMust && !m_pCommand_SetPositionMust->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ActivateAnalogPositionSetpoint && !m_pCommand_ActivateAnalogPositionSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DeactivateAnalogPositionSetpoint && !m_pCommand_DeactivateAnalogPositionSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_EnableAnalogPositionSetpoint && !m_pCommand_EnableAnalogPositionSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DisableAnalogPositionSetpoint && !m_pCommand_DisableAnalogPositionSetpoint->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetPositionMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetPositionMust) m_pCommand_GetPositionMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetPositionMust) m_pCommand_SetPositionMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ActivateAnalogPositionSetpoint) m_pCommand_ActivateAnalogPositionSetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DeactivateAnalogPositionSetpoint) m_pCommand_DeactivateAnalogPositionSetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_EnableAnalogPositionSetpoint) m_pCommand_EnableAnalogPositionSetpoint->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DisableAnalogPositionSetpoint) m_pCommand_DisableAnalogPositionSetpoint->InitJournalManager(p_pJournalManager);
}

void CCommandSetPositionMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetPositionMust) m_pCommand_GetPositionMust->ResetJournalManager();
    if(m_pCommand_SetPositionMust) m_pCommand_SetPositionMust->ResetJournalManager();
    if(m_pCommand_ActivateAnalogPositionSetpoint) m_pCommand_ActivateAnalogPositionSetpoint->ResetJournalManager();
    if(m_pCommand_DeactivateAnalogPositionSetpoint) m_pCommand_DeactivateAnalogPositionSetpoint->ResetJournalManager();
    if(m_pCommand_EnableAnalogPositionSetpoint) m_pCommand_EnableAnalogPositionSetpoint->ResetJournalManager();
    if(m_pCommand_DisableAnalogPositionSetpoint) m_pCommand_DisableAnalogPositionSetpoint->ResetJournalManager();
}

BOOL CCommandSetPositionMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetPositionMust && !m_pCommand_GetPositionMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetPositionMust && !m_pCommand_SetPositionMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ActivateAnalogPositionSetpoint && !m_pCommand_ActivateAnalogPositionSetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DeactivateAnalogPositionSetpoint && !m_pCommand_DeactivateAnalogPositionSetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_EnableAnalogPositionSetpoint && !m_pCommand_EnableAnalogPositionSetpoint->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DisableAnalogPositionSetpoint && !m_pCommand_DisableAnalogPositionSetpoint->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

