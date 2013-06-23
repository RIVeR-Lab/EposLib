// CommandSetInputsPositionMarker_VCS_Drive.cpp: Implementierung der Klasse CCommandSetInputsPositionMarker_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetInputsPositionMarker_VCS_Drive.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetInputsPositionMarker_VCS_Drive::CCommandSetInputsPositionMarker_VCS_Drive()
{
    m_strCommandSetName = COMMAND_SET_INPUTS_POSITION_MARKER;

    m_pCommand_SetPositionMarkerParameter = NULL;
    m_pCommand_GetPositionMarkerParameter = NULL;
    m_pCommand_ActivatePositionMarker = NULL;
    m_pCommand_DeactivatePositionMarker = NULL;
    m_pCommand_ReadPositionMarkerCounter = NULL;
    m_pCommand_ReadPositionMarkerCapturedPosition = NULL;
    m_pCommand_ResetPositionMarkerCounter = NULL;

    InitCommands();
}

CCommandSetInputsPositionMarker_VCS_Drive::~CCommandSetInputsPositionMarker_VCS_Drive()
{
    DeleteCommands();
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::VCS_SetPositionMarkerParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPositionMarkerEdgeType, BYTE p_ubPositionMarkerMode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetPositionMarkerParameter)
    {
        //Set Parameter Data
        m_pCommand_SetPositionMarkerParameter->ResetStatus();
        m_pCommand_SetPositionMarkerParameter->SetParameterData(0, &p_ubPositionMarkerEdgeType , sizeof(p_ubPositionMarkerEdgeType));
        m_pCommand_SetPositionMarkerParameter->SetParameterData(1, &p_ubPositionMarkerMode, sizeof(p_ubPositionMarkerMode));

        //ExecuteCommand
        oResult = m_pCommand_SetPositionMarkerParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetPositionMarkerParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::VCS_GetPositionMarkerParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubPositionMarkerEdgeType, BYTE* p_pubPositionMarkerMode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetPositionMarkerParameter)
    {
        //Set Parameter Data
        m_pCommand_GetPositionMarkerParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetPositionMarkerParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetPositionMarkerParameter->GetReturnParameterData(0, p_pubPositionMarkerEdgeType, sizeof(*p_pubPositionMarkerEdgeType));
        m_pCommand_GetPositionMarkerParameter->GetReturnParameterData(1, p_pubPositionMarkerMode, sizeof(*p_pubPositionMarkerMode));

        //Get ErrorCode
        m_pCommand_GetPositionMarkerParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::VCS_ActivatePositionMarker(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalInputNumber, BOOL p_oPolarity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ActivatePositionMarker)
    {
        //Set Parameter Data
        m_pCommand_ActivatePositionMarker->ResetStatus();
        m_pCommand_ActivatePositionMarker->SetParameterData(0, &p_usDigitalInputNumber, sizeof(p_usDigitalInputNumber));
        m_pCommand_ActivatePositionMarker->SetParameterData(1, &p_oPolarity, sizeof(p_oPolarity));

        //ExecuteCommand
        oResult = m_pCommand_ActivatePositionMarker->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ActivatePositionMarker->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::VCS_DeactivatePositionMarker(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalInputNumber, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DeactivatePositionMarker)
    {
        //Set Parameter Data
        m_pCommand_DeactivatePositionMarker->ResetStatus();
        m_pCommand_DeactivatePositionMarker->SetParameterData(0, &p_usDigitalInputNumber, sizeof(p_usDigitalInputNumber));

        //ExecuteCommand
        oResult = m_pCommand_DeactivatePositionMarker->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DeactivatePositionMarker->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::VCS_ReadPositionMarkerCounter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pCount, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ReadPositionMarkerCounter)
    {
        //Set Parameter Data
        m_pCommand_ReadPositionMarkerCounter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_ReadPositionMarkerCounter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadPositionMarkerCounter->GetReturnParameterData(0, p_pCount, sizeof(*p_pCount));

        //Get ErrorCode
        m_pCommand_ReadPositionMarkerCounter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::VCS_ReadPositionMarkerCapturedPosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCountIndex, long* p_plCapturedPosition, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ReadPositionMarkerCapturedPosition)
    {
        //Set Parameter Data
        m_pCommand_ReadPositionMarkerCapturedPosition->ResetStatus();
        m_pCommand_ReadPositionMarkerCapturedPosition->SetParameterData(0, &p_usCountIndex, sizeof(p_usCountIndex));

        //ExecuteCommand
        oResult = m_pCommand_ReadPositionMarkerCapturedPosition->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadPositionMarkerCapturedPosition->GetReturnParameterData(1, p_plCapturedPosition, sizeof(*p_plCapturedPosition));

        //Get ErrorCode
        m_pCommand_ReadPositionMarkerCapturedPosition->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::VCS_ResetPositionMarkerCounter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ResetPositionMarkerCounter)
    {
        //Set Parameter Data
        m_pCommand_ResetPositionMarkerCounter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_ResetPositionMarkerCounter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ResetPositionMarkerCounter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetInputsPositionMarker_VCS_Drive::InitCommands()
{
    DeleteCommands();

    //Init SetPositionMarkerParameter
    m_pCommand_SetPositionMarkerParameter = new CCommand_VCS_Drive();
    m_pCommand_SetPositionMarkerParameter->InitCommand(DRIVE_SET_POSITION_MARKER_PARAMETER);

    //Init GetPositionMarkerParameter
    m_pCommand_GetPositionMarkerParameter = new CCommand_VCS_Drive();
    m_pCommand_GetPositionMarkerParameter->InitCommand(DRIVE_GET_POSITION_MARKER_PARAMETER);

    //Init ActivatePositionMarker
    m_pCommand_ActivatePositionMarker = new CCommand_VCS_Drive();
    m_pCommand_ActivatePositionMarker->InitCommand(DRIVE_ACTIVATE_POSITION_MARKER);

    //Init DeactivatePositionMarker
    m_pCommand_DeactivatePositionMarker = new CCommand_VCS_Drive();
    m_pCommand_DeactivatePositionMarker->InitCommand(DRIVE_DEACTIVATE_POSITION_MARKER);

    //Init ReadPositionMarkerCount
    m_pCommand_ReadPositionMarkerCounter = new CCommand_VCS_Drive();
    m_pCommand_ReadPositionMarkerCounter->InitCommand(DRIVE_READ_POSITION_MARKER_COUNTER);

    //Init ReadPositionMarkerCapturedPosition
    m_pCommand_ReadPositionMarkerCapturedPosition = new CCommand_VCS_Drive();
    m_pCommand_ReadPositionMarkerCapturedPosition->InitCommand(DRIVE_READ_POSITION_MARKER_CAPTURED_POSITION);

    //Init ResetPositionMarkerCount
    m_pCommand_ResetPositionMarkerCounter = new CCommand_VCS_Drive();
    m_pCommand_ResetPositionMarkerCounter->InitCommand(DRIVE_RESET_POSITION_MARKER_COUNTER);
}

void CCommandSetInputsPositionMarker_VCS_Drive::DeleteCommands()
{
    if(m_pCommand_SetPositionMarkerParameter)
    {
        delete m_pCommand_SetPositionMarkerParameter;
        m_pCommand_SetPositionMarkerParameter = NULL;
    }

    if(m_pCommand_GetPositionMarkerParameter)
    {
        delete m_pCommand_GetPositionMarkerParameter;
        m_pCommand_GetPositionMarkerParameter = NULL;
    }

    if(m_pCommand_ActivatePositionMarker)
    {
        delete m_pCommand_ActivatePositionMarker;
        m_pCommand_ActivatePositionMarker = NULL;
    }

    if(m_pCommand_DeactivatePositionMarker)
    {
        delete m_pCommand_DeactivatePositionMarker;
        m_pCommand_DeactivatePositionMarker = NULL;
    }

    if(m_pCommand_ReadPositionMarkerCounter)
    {
        delete m_pCommand_ReadPositionMarkerCounter;
        m_pCommand_ReadPositionMarkerCounter = NULL;
    }

    if(m_pCommand_ReadPositionMarkerCapturedPosition)
    {
        delete m_pCommand_ReadPositionMarkerCapturedPosition;
        m_pCommand_ReadPositionMarkerCapturedPosition = NULL;
    }

    if(m_pCommand_ResetPositionMarkerCounter)
    {
        delete m_pCommand_ResetPositionMarkerCounter;
        m_pCommand_ResetPositionMarkerCounter = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetInputsPositionMarker_VCS_Drive::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetPositionMarkerParameter && !m_pCommand_GetPositionMarkerParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetPositionMarkerParameter && !m_pCommand_SetPositionMarkerParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ActivatePositionMarker && !m_pCommand_ActivatePositionMarker->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DeactivatePositionMarker && !m_pCommand_DeactivatePositionMarker->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ReadPositionMarkerCounter && !m_pCommand_ReadPositionMarkerCounter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ReadPositionMarkerCapturedPosition && !m_pCommand_ReadPositionMarkerCapturedPosition->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ResetPositionMarkerCounter && !m_pCommand_ResetPositionMarkerCounter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetInputsPositionMarker_VCS_Drive::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetPositionMarkerParameter) m_pCommand_SetPositionMarkerParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetPositionMarkerParameter) m_pCommand_GetPositionMarkerParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ActivatePositionMarker) m_pCommand_ActivatePositionMarker->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DeactivatePositionMarker) m_pCommand_DeactivatePositionMarker->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ReadPositionMarkerCounter) m_pCommand_ReadPositionMarkerCounter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ReadPositionMarkerCapturedPosition) m_pCommand_ReadPositionMarkerCapturedPosition->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ResetPositionMarkerCounter) m_pCommand_ResetPositionMarkerCounter->InitJournalManager(p_pJournalManager);
}

void CCommandSetInputsPositionMarker_VCS_Drive::ResetJournalManager()
{
    if(m_pCommand_SetPositionMarkerParameter) m_pCommand_SetPositionMarkerParameter->ResetJournalManager();
    if(m_pCommand_GetPositionMarkerParameter) m_pCommand_GetPositionMarkerParameter->ResetJournalManager();
    if(m_pCommand_ActivatePositionMarker) m_pCommand_ActivatePositionMarker->ResetJournalManager();
    if(m_pCommand_DeactivatePositionMarker) m_pCommand_DeactivatePositionMarker->ResetJournalManager();
    if(m_pCommand_ReadPositionMarkerCounter) m_pCommand_ReadPositionMarkerCounter->ResetJournalManager();
    if(m_pCommand_ReadPositionMarkerCapturedPosition) m_pCommand_ReadPositionMarkerCapturedPosition->ResetJournalManager();
    if(m_pCommand_ResetPositionMarkerCounter) m_pCommand_ResetPositionMarkerCounter->ResetJournalManager();
}

BOOL CCommandSetInputsPositionMarker_VCS_Drive::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SetPositionMarkerParameter && !m_pCommand_SetPositionMarkerParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetPositionMarkerParameter && !m_pCommand_GetPositionMarkerParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ActivatePositionMarker && !m_pCommand_ActivatePositionMarker->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DeactivatePositionMarker && !m_pCommand_DeactivatePositionMarker->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ReadPositionMarkerCounter && !m_pCommand_ReadPositionMarkerCounter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ReadPositionMarkerCapturedPosition && !m_pCommand_ReadPositionMarkerCapturedPosition->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ResetPositionMarkerCounter && !m_pCommand_ResetPositionMarkerCounter->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

