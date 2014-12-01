// CommandSetOutputsPositionCompare_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetOutputsPositionCompare_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetOutputsPositionCompare_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetOutputsPositionCompare_VCS_Drive2::CCommandSetOutputsPositionCompare_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_OUTPUTS_POSITION_COMPARE;

    m_pCommand_SetPositionCompareParameter = NULL;
    m_pCommand_GetPositionCompareParameter = NULL;
    m_pCommand_ActivatePositionCompare = NULL;
    m_pCommand_DeactivatePositionCompare = NULL;
    m_pCommand_EnablePositionCompare = NULL;
    m_pCommand_DisablePositionCompare = NULL;
    m_pCommand_SetPositionCompareReferencePosition = NULL;

    InitCommands();
}

CCommandSetOutputsPositionCompare_VCS_Drive2::~CCommandSetOutputsPositionCompare_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::VCS_SetPositionCompareParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubOperationalMode, BYTE p_ubIntervalMode, BYTE p_ubDirectionDependency, WORD p_usIntervalWidth, WORD p_usIntervalRepetitions, WORD p_usPulseWidth, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetPositionCompareParameter)
    {
        //Set Parameter Data
        m_pCommand_SetPositionCompareParameter->ResetStatus();
        m_pCommand_SetPositionCompareParameter->SetParameterData(0, &p_ubOperationalMode, sizeof(p_ubOperationalMode));
        m_pCommand_SetPositionCompareParameter->SetParameterData(1, &p_ubIntervalMode, sizeof(p_ubIntervalMode));
        m_pCommand_SetPositionCompareParameter->SetParameterData(2, &p_ubDirectionDependency, sizeof(p_ubDirectionDependency));
        m_pCommand_SetPositionCompareParameter->SetParameterData(3, &p_usIntervalWidth, sizeof(p_usIntervalWidth));
        m_pCommand_SetPositionCompareParameter->SetParameterData(4, &p_usIntervalRepetitions, sizeof(p_usIntervalRepetitions));
        m_pCommand_SetPositionCompareParameter->SetParameterData(5, &p_usPulseWidth, sizeof(p_usPulseWidth));

        //ExecuteCommand
        oResult = m_pCommand_SetPositionCompareParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetPositionCompareParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::VCS_GetPositionCompareParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubOperationalMode, BYTE* p_pubIntervalMode, BYTE* p_pubDirectionDependency, WORD* p_pusIntervalWidth, WORD* p_pusIntervalRepetitions, WORD* p_pusPulseWidth, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetPositionCompareParameter)
    {
        //Set Parameter Data
        m_pCommand_GetPositionCompareParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetPositionCompareParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetPositionCompareParameter->GetReturnParameterData(0, p_pubOperationalMode, sizeof(*p_pubOperationalMode));
        m_pCommand_GetPositionCompareParameter->GetReturnParameterData(1, p_pubIntervalMode, sizeof(*p_pubIntervalMode));
        m_pCommand_GetPositionCompareParameter->GetReturnParameterData(2, p_pubDirectionDependency, sizeof(*p_pubDirectionDependency));
        m_pCommand_GetPositionCompareParameter->GetReturnParameterData(3, p_pusIntervalWidth, sizeof(*p_pusIntervalWidth));
        m_pCommand_GetPositionCompareParameter->GetReturnParameterData(4, p_pusIntervalRepetitions, sizeof(*p_pusIntervalRepetitions));
        m_pCommand_GetPositionCompareParameter->GetReturnParameterData(5, p_pusPulseWidth, sizeof(*p_pusPulseWidth));

        //Get ErrorCode
        m_pCommand_GetPositionCompareParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::VCS_ActivatePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalOutputNumber, BOOL p_oPolarity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ActivatePositionCompare)
    {
        //Set Parameter Data
        m_pCommand_ActivatePositionCompare->ResetStatus();
        m_pCommand_ActivatePositionCompare->SetParameterData(0, &p_usDigitalOutputNumber, sizeof(p_usDigitalOutputNumber));
        m_pCommand_ActivatePositionCompare->SetParameterData(1, &p_oPolarity, sizeof(p_oPolarity));

        //ExecuteCommand
        oResult = m_pCommand_ActivatePositionCompare->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ActivatePositionCompare->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::VCS_DeactivatePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalOutputNumber, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DeactivatePositionCompare)
    {
        //Set Parameter Data
        m_pCommand_DeactivatePositionCompare->ResetStatus();
        m_pCommand_DeactivatePositionCompare->SetParameterData(0, &p_usDigitalOutputNumber, sizeof(p_usDigitalOutputNumber));

        //ExecuteCommand
        oResult = m_pCommand_DeactivatePositionCompare->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DeactivatePositionCompare->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::VCS_EnablePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_EnablePositionCompare)
    {
        //Set Parameter Data
        m_pCommand_EnablePositionCompare->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_EnablePositionCompare->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_EnablePositionCompare->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::VCS_DisablePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DisablePositionCompare)
    {
        //Set Parameter Data
        m_pCommand_DisablePositionCompare->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DisablePositionCompare->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DisablePositionCompare->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::VCS_SetPositionCompareReferencePosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lReferencePosition, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetPositionCompareReferencePosition)
    {
        //Set Parameter Data
        m_pCommand_SetPositionCompareReferencePosition->ResetStatus();
        m_pCommand_SetPositionCompareReferencePosition->SetParameterData(0, &p_lReferencePosition, sizeof(p_lReferencePosition));

        //ExecuteCommand
        oResult = m_pCommand_SetPositionCompareReferencePosition->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetPositionCompareReferencePosition->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetOutputsPositionCompare_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init SetPositionCompareParameter
    m_pCommand_SetPositionCompareParameter = new CCommand_VCS_Drive2();
    m_pCommand_SetPositionCompareParameter->InitCommand(DRIVE2_SET_POSITION_COMPARE_PARAMETER);

    //Init GetPositionCompareParameter
    m_pCommand_GetPositionCompareParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetPositionCompareParameter->InitCommand(DRIVE2_GET_POSITION_COMPARE_PARAMETER);

    //Init ActivatePositionCompare
    m_pCommand_ActivatePositionCompare = new CCommand_VCS_Drive2();
    m_pCommand_ActivatePositionCompare->InitCommand(DRIVE2_ACTIVATE_POSITION_COMPARE);

    //Init DeactivatePositionCompare
    m_pCommand_DeactivatePositionCompare = new CCommand_VCS_Drive2();
    m_pCommand_DeactivatePositionCompare->InitCommand(DRIVE2_DEACTIVATE_POSITION_COMPARE);

    //Init EnablePositionCompare
    m_pCommand_EnablePositionCompare = new CCommand_VCS_Drive2();
    m_pCommand_EnablePositionCompare->InitCommand(DRIVE2_ENABLE_POSITION_COMPARE);

    //Init DisablePositionCompare
    m_pCommand_DisablePositionCompare = new CCommand_VCS_Drive2();
    m_pCommand_DisablePositionCompare->InitCommand(DRIVE2_DISABLE_POSITION_COMPARE);

    //Init SetPositionCompareReferencePosition
    m_pCommand_SetPositionCompareReferencePosition = new CCommand_VCS_Drive2();
    m_pCommand_SetPositionCompareReferencePosition->InitCommand(DRIVE2_SET_POSITION_COMPARE_REFERENCE_POSITION);
}

void CCommandSetOutputsPositionCompare_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_SetPositionCompareParameter)
    {
        delete m_pCommand_SetPositionCompareParameter;
        m_pCommand_SetPositionCompareParameter = NULL;
    }

    if(m_pCommand_GetPositionCompareParameter)
    {
        delete m_pCommand_GetPositionCompareParameter;
        m_pCommand_GetPositionCompareParameter = NULL;
    }

    if(m_pCommand_ActivatePositionCompare)
    {
        delete m_pCommand_ActivatePositionCompare;
        m_pCommand_ActivatePositionCompare = NULL;
    }

    if(m_pCommand_DeactivatePositionCompare)
    {
        delete m_pCommand_DeactivatePositionCompare;
        m_pCommand_DeactivatePositionCompare = NULL;
    }

    if(m_pCommand_EnablePositionCompare)
    {
        delete m_pCommand_EnablePositionCompare;
        m_pCommand_EnablePositionCompare = NULL;
    }

    if(m_pCommand_DisablePositionCompare)
    {
        delete m_pCommand_DisablePositionCompare;
        m_pCommand_DisablePositionCompare = NULL;
    }

    if(m_pCommand_SetPositionCompareReferencePosition)
    {
        delete m_pCommand_SetPositionCompareReferencePosition;
        m_pCommand_SetPositionCompareReferencePosition = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetOutputsPositionCompare_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;
    BOOL oCheckVisibility = FALSE;

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSubSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_GetPositionCompareParameter && !m_pCommand_GetPositionCompareParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetPositionCompareParameter && !m_pCommand_SetPositionCompareParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetPositionCompareReferencePosition && !m_pCommand_SetPositionCompareReferencePosition->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ActivatePositionCompare && !m_pCommand_ActivatePositionCompare->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DeactivatePositionCompare && !m_pCommand_DeactivatePositionCompare->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_EnablePositionCompare && !m_pCommand_EnablePositionCompare->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DisablePositionCompare && !m_pCommand_DisablePositionCompare->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetOutputsPositionCompare_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetPositionCompareParameter) m_pCommand_SetPositionCompareParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetPositionCompareParameter) m_pCommand_GetPositionCompareParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ActivatePositionCompare) m_pCommand_ActivatePositionCompare->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DeactivatePositionCompare) m_pCommand_DeactivatePositionCompare->InitJournalManager(p_pJournalManager);
    if(m_pCommand_EnablePositionCompare) m_pCommand_EnablePositionCompare->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DisablePositionCompare) m_pCommand_DisablePositionCompare->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetPositionCompareReferencePosition) m_pCommand_SetPositionCompareReferencePosition->InitJournalManager(p_pJournalManager);
}

void CCommandSetOutputsPositionCompare_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_SetPositionCompareParameter) m_pCommand_SetPositionCompareParameter->ResetJournalManager();
    if(m_pCommand_GetPositionCompareParameter) m_pCommand_GetPositionCompareParameter->ResetJournalManager();
    if(m_pCommand_ActivatePositionCompare) m_pCommand_ActivatePositionCompare->ResetJournalManager();
    if(m_pCommand_DeactivatePositionCompare) m_pCommand_DeactivatePositionCompare->ResetJournalManager();
    if(m_pCommand_EnablePositionCompare) m_pCommand_EnablePositionCompare->ResetJournalManager();
    if(m_pCommand_DisablePositionCompare) m_pCommand_DisablePositionCompare->ResetJournalManager();
    if(m_pCommand_SetPositionCompareReferencePosition) m_pCommand_SetPositionCompareReferencePosition->ResetJournalManager();
}

BOOL CCommandSetOutputsPositionCompare_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SetPositionCompareParameter && !m_pCommand_SetPositionCompareParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetPositionCompareParameter && !m_pCommand_GetPositionCompareParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ActivatePositionCompare && !m_pCommand_ActivatePositionCompare->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DeactivatePositionCompare && !m_pCommand_DeactivatePositionCompare->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_EnablePositionCompare && !m_pCommand_EnablePositionCompare->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DisablePositionCompare && !m_pCommand_DisablePositionCompare->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetPositionCompareReferencePosition && !m_pCommand_SetPositionCompareReferencePosition->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

