// CommandSetStepDirectionMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetStepDirectionMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetStepDirectionMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetStepDirectionMode_VCS_Drive2::CCommandSetStepDirectionMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_STEP_DIRECTION_MODE;

    m_pCommand_GetStepDirectionParameter = NULL;
    m_pCommand_SetStepDirectionParameter = NULL;

    InitCommands();
}

CCommandSetStepDirectionMode_VCS_Drive2::~CCommandSetStepDirectionMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetStepDirectionMode_VCS_Drive2::VCS_GetStepDirectionParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusScalingNumerator, WORD *p_pusScalingDenominator, BYTE *p_pubPolarity, DWORD *p_pulMaxVelocity, DWORD *p_pulMaxAcceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetStepDirectionParameter)
    {
        //Set Parameter Data
        m_pCommand_GetStepDirectionParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetStepDirectionParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetStepDirectionParameter->GetReturnParameterData(0, p_pusScalingNumerator, sizeof(*p_pusScalingNumerator));
        m_pCommand_GetStepDirectionParameter->GetReturnParameterData(1, p_pusScalingDenominator, sizeof(*p_pusScalingDenominator));
        m_pCommand_GetStepDirectionParameter->GetReturnParameterData(2, p_pubPolarity, sizeof(*p_pubPolarity));
        m_pCommand_GetStepDirectionParameter->GetReturnParameterData(3, p_pulMaxVelocity, sizeof(*p_pulMaxVelocity));
        m_pCommand_GetStepDirectionParameter->GetReturnParameterData(4, p_pulMaxAcceleration, sizeof(*p_pulMaxAcceleration));

        //Get ErrorCode
        m_pCommand_GetStepDirectionParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetStepDirectionMode_VCS_Drive2::VCS_SetStepDirectionParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usScalingNumerator, WORD p_usScalingDenominator, BYTE p_ubPolarity, DWORD p_ulMaxVelocity, DWORD p_ulMaxAcceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetStepDirectionParameter)
    {
        //Set Parameter Data
        m_pCommand_SetStepDirectionParameter->ResetStatus();
        m_pCommand_SetStepDirectionParameter->SetParameterData(0, &p_usScalingNumerator, sizeof(p_usScalingNumerator));
        m_pCommand_SetStepDirectionParameter->SetParameterData(1, &p_usScalingDenominator, sizeof(p_usScalingDenominator));
        m_pCommand_SetStepDirectionParameter->SetParameterData(2, &p_ubPolarity, sizeof(p_ubPolarity));
        m_pCommand_SetStepDirectionParameter->SetParameterData(3, &p_ulMaxVelocity, sizeof(p_ulMaxVelocity));
        m_pCommand_SetStepDirectionParameter->SetParameterData(4, &p_ulMaxAcceleration, sizeof(p_ulMaxAcceleration));

        //ExecuteCommand
        oResult = m_pCommand_GetStepDirectionParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SetStepDirectionParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetStepDirectionMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init GetStepDirectionParameter
    m_pCommand_GetStepDirectionParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetStepDirectionParameter->InitCommand(DRIVE2_GET_STEP_DIRECTION_PARAMETER);

    //Init SetStepDirectionParameter
    m_pCommand_SetStepDirectionParameter = new CCommand_VCS_Drive2;
    m_pCommand_SetStepDirectionParameter->InitCommand(DRIVE2_SET_STEP_DIRECTION_PARAMETER);
}

void CCommandSetStepDirectionMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_GetStepDirectionParameter)
    {
        delete m_pCommand_GetStepDirectionParameter;
        m_pCommand_GetStepDirectionParameter = NULL;
    }
    if(m_pCommand_SetStepDirectionParameter)
    {
        delete m_pCommand_SetStepDirectionParameter;
        m_pCommand_SetStepDirectionParameter = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetStepDirectionMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetStepDirectionParameter && !m_pCommand_GetStepDirectionParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetStepDirectionParameter && !m_pCommand_SetStepDirectionParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetStepDirectionMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetStepDirectionParameter) m_pCommand_GetStepDirectionParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetStepDirectionParameter) m_pCommand_SetStepDirectionParameter->InitJournalManager(p_pJournalManager);
}

void CCommandSetStepDirectionMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetStepDirectionParameter) m_pCommand_GetStepDirectionParameter->ResetJournalManager();
    if(m_pCommand_SetStepDirectionParameter) m_pCommand_SetStepDirectionParameter->ResetJournalManager();
}

BOOL CCommandSetStepDirectionMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetStepDirectionParameter && !m_pCommand_GetStepDirectionParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetStepDirectionParameter && !m_pCommand_SetStepDirectionParameter->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

