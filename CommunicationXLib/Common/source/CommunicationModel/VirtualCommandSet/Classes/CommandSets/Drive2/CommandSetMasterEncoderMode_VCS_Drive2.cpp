// CommandSetMasterEncoderMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetMasterEncoderMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandSetMasterEncoderMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetMasterEncoderMode_VCS_Drive2::CCommandSetMasterEncoderMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_MASTER_ENCODER_MODE;

    m_pCommand_GetMasterEncoderParameter = NULL;
    m_pCommand_SetMasterEncoderParameter = NULL;

    InitCommands();
}

CCommandSetMasterEncoderMode_VCS_Drive2::~CCommandSetMasterEncoderMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetMasterEncoderMode_VCS_Drive2::VCS_GetMasterEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusScalingNumerator, WORD *p_pusScalingDenominator, BYTE *p_pubPolarity, DWORD *p_pulMaxVelocity, DWORD *p_pulMaxAcceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetMasterEncoderParameter)
    {
        //Set Parameter Data
        m_pCommand_GetMasterEncoderParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetMasterEncoderParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetMasterEncoderParameter->GetReturnParameterData(0, p_pusScalingNumerator, sizeof(*p_pusScalingNumerator));
        m_pCommand_GetMasterEncoderParameter->GetReturnParameterData(1, p_pusScalingDenominator, sizeof(*p_pusScalingDenominator));
        m_pCommand_GetMasterEncoderParameter->GetReturnParameterData(2, p_pubPolarity, sizeof(*p_pubPolarity));
        m_pCommand_GetMasterEncoderParameter->GetReturnParameterData(3, p_pulMaxVelocity, sizeof(*p_pulMaxVelocity));
        m_pCommand_GetMasterEncoderParameter->GetReturnParameterData(4, p_pulMaxAcceleration, sizeof(*p_pulMaxAcceleration));

        //Get ErrorCode
        m_pCommand_GetMasterEncoderParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetMasterEncoderMode_VCS_Drive2::VCS_SetMasterEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usScalingNumerator, WORD p_usScalingDenominator, BYTE p_ubPolarity, DWORD p_ulMaxVelocity, DWORD p_ulMaxAcceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetMasterEncoderParameter)
    {
        //Set Parameter Data
        m_pCommand_SetMasterEncoderParameter->ResetStatus();
        m_pCommand_SetMasterEncoderParameter->SetParameterData(0, &p_usScalingNumerator, sizeof(p_usScalingNumerator));
        m_pCommand_SetMasterEncoderParameter->SetParameterData(1, &p_usScalingDenominator, sizeof(p_usScalingDenominator));
        m_pCommand_SetMasterEncoderParameter->SetParameterData(2, &p_ubPolarity, sizeof(p_ubPolarity));
        m_pCommand_SetMasterEncoderParameter->SetParameterData(3, &p_ulMaxVelocity, sizeof(p_ulMaxVelocity));
        m_pCommand_SetMasterEncoderParameter->SetParameterData(4, &p_ulMaxAcceleration, sizeof(p_ulMaxAcceleration));

        //ExecuteCommand
        oResult = m_pCommand_GetMasterEncoderParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SetMasterEncoderParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetMasterEncoderMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init GetMasterEncoderParameter
    m_pCommand_GetMasterEncoderParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetMasterEncoderParameter->InitCommand(DRIVE2_GET_MASTER_ENCODER_PARAMETER);

    //Init SetMasterEncoderParameter
    m_pCommand_SetMasterEncoderParameter = new CCommand_VCS_Drive2;
    m_pCommand_SetMasterEncoderParameter->InitCommand(DRIVE2_SET_MASTER_ENCODER_PARAMETER);
}

void CCommandSetMasterEncoderMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_GetMasterEncoderParameter)
    {
        delete m_pCommand_GetMasterEncoderParameter;
        m_pCommand_GetMasterEncoderParameter = NULL;
    }
    if(m_pCommand_SetMasterEncoderParameter)
    {
        delete m_pCommand_SetMasterEncoderParameter;
        m_pCommand_SetMasterEncoderParameter = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetMasterEncoderMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetMasterEncoderParameter && !m_pCommand_GetMasterEncoderParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetMasterEncoderParameter && !m_pCommand_SetMasterEncoderParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetMasterEncoderMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetMasterEncoderParameter) m_pCommand_GetMasterEncoderParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetMasterEncoderParameter) m_pCommand_SetMasterEncoderParameter->InitJournalManager(p_pJournalManager);
}

void CCommandSetMasterEncoderMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetMasterEncoderParameter) m_pCommand_GetMasterEncoderParameter->ResetJournalManager();
    if(m_pCommand_SetMasterEncoderParameter) m_pCommand_SetMasterEncoderParameter->ResetJournalManager();
}

BOOL CCommandSetMasterEncoderMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetMasterEncoderParameter && !m_pCommand_GetMasterEncoderParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetMasterEncoderParameter && !m_pCommand_SetMasterEncoderParameter->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

