// CommandSetProgramStatus_VCS_Plc2.cpp: Implementierung der Klasse CCommandSetProgramStatus_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetProgramStatus_VCS_Plc2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetProgramStatus_VCS_Plc2::CCommandSetProgramStatus_VCS_Plc2()
{
    m_strCommandSetName = COMMAND_SET_PROGRAM_STATUS;

    m_pCommand_GetPlcStatus = NULL;
    m_pCommand_ColdstartPlc = NULL;
    m_pCommand_WarmstartPlc = NULL;
    m_pCommand_HotstartPlc = NULL;
    m_pCommand_StopPlc = NULL;

    InitCommands();
}

CCommandSetProgramStatus_VCS_Plc2::~CCommandSetProgramStatus_VCS_Plc2()
{
    DeleteCommands();
}

BOOL CCommandSetProgramStatus_VCS_Plc2::VCS_GetPlcStatus(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poIsProgramRunning, BOOL* p_poIsProgramAvailable, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_GetPlcStatus)
    {
        //Execute Command
        oResult = m_pCommand_GetPlcStatus->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetPlcStatus->GetReturnParameterData(0, p_poIsProgramRunning, sizeof(*p_poIsProgramRunning));
        m_pCommand_GetPlcStatus->GetReturnParameterData(1, p_poIsProgramAvailable, sizeof(*p_poIsProgramAvailable));

        //Get ErrorCode
        m_pCommand_GetPlcStatus->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProgramStatus_VCS_Plc2::VCS_ColdstartPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_ColdstartPlc)
    {
        //Set Parameter Data
        m_pCommand_ColdstartPlc->ResetStatus();

        //Execute Command
        oResult = m_pCommand_ColdstartPlc->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ColdstartPlc->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProgramStatus_VCS_Plc2::VCS_WarmstartPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_WarmstartPlc)
    {
        //Set Parameter Data
        m_pCommand_WarmstartPlc->ResetStatus();

        //Execute Command
        oResult = m_pCommand_WarmstartPlc->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_WarmstartPlc->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProgramStatus_VCS_Plc2::VCS_HotstartPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_HotstartPlc)
    {
        //Set Parameter Data
        m_pCommand_HotstartPlc->ResetStatus();

        //Execute Command
        oResult = m_pCommand_HotstartPlc->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_HotstartPlc->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProgramStatus_VCS_Plc2::VCS_StopPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_StopPlc)
    {
        //Set Parameter Data
        m_pCommand_StopPlc->ResetStatus();

        //Execute Command
        oResult = m_pCommand_StopPlc->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_StopPlc->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetProgramStatus_VCS_Plc2::InitCommands()
{
    DeleteCommands();

    //Init GetPlcStatus
    m_pCommand_GetPlcStatus = new CCommand_VCS_Plc2();
    m_pCommand_GetPlcStatus->InitCommand(PLC2_GET_PLC_STATUS);

    //Init ColdstartPlc
    m_pCommand_ColdstartPlc = new CCommand_VCS_Plc2();
    m_pCommand_ColdstartPlc->InitCommand(PLC2_COLDSTART_PLC);

    //Init WarmstartPlc
    m_pCommand_WarmstartPlc = new CCommand_VCS_Plc2();
    m_pCommand_WarmstartPlc->InitCommand(PLC2_WARMSTART_PLC);

    //Init HotstartPlc
    m_pCommand_HotstartPlc = new CCommand_VCS_Plc2();
    m_pCommand_HotstartPlc->InitCommand(PLC2_HOTSTART_PLC);

    //Init StopPlc
    m_pCommand_StopPlc = new CCommand_VCS_Plc2();
    m_pCommand_StopPlc->InitCommand(PLC2_STOP_PLC);
}

void CCommandSetProgramStatus_VCS_Plc2::DeleteCommands()
{
    if(m_pCommand_GetPlcStatus)
    {
        delete m_pCommand_GetPlcStatus;
        m_pCommand_GetPlcStatus = NULL;
    }

    if(m_pCommand_ColdstartPlc)
    {
        delete m_pCommand_ColdstartPlc;
        m_pCommand_ColdstartPlc = NULL;
    }

    if(m_pCommand_WarmstartPlc)
    {
        delete m_pCommand_WarmstartPlc;
        m_pCommand_WarmstartPlc = NULL;
    }

    if(m_pCommand_HotstartPlc)
    {
        delete m_pCommand_HotstartPlc;
        m_pCommand_HotstartPlc = NULL;
    }

    if(m_pCommand_StopPlc)
    {
        delete m_pCommand_StopPlc;
        m_pCommand_StopPlc = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetProgramStatus_VCS_Plc2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement(NULL);
    BOOL oCheckVisibility(FALSE);

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_GetPlcStatus && !m_pCommand_GetPlcStatus->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ColdstartPlc && !m_pCommand_ColdstartPlc->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_WarmstartPlc && !m_pCommand_WarmstartPlc->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_HotstartPlc && !m_pCommand_HotstartPlc->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_StopPlc && !m_pCommand_StopPlc->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetProgramStatus_VCS_Plc2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetPlcStatus) m_pCommand_GetPlcStatus->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ColdstartPlc) m_pCommand_ColdstartPlc->InitJournalManager(p_pJournalManager);
    if(m_pCommand_WarmstartPlc) m_pCommand_WarmstartPlc->InitJournalManager(p_pJournalManager);
    if(m_pCommand_HotstartPlc) m_pCommand_HotstartPlc->InitJournalManager(p_pJournalManager);
    if(m_pCommand_StopPlc) m_pCommand_StopPlc->InitJournalManager(p_pJournalManager);
}

void CCommandSetProgramStatus_VCS_Plc2::ResetJournalManager()
{
    if(m_pCommand_GetPlcStatus) m_pCommand_GetPlcStatus->ResetJournalManager();
    if(m_pCommand_ColdstartPlc) m_pCommand_ColdstartPlc->ResetJournalManager();
    if(m_pCommand_WarmstartPlc) m_pCommand_WarmstartPlc->ResetJournalManager();
    if(m_pCommand_HotstartPlc) m_pCommand_HotstartPlc->ResetJournalManager();
    if(m_pCommand_StopPlc) m_pCommand_StopPlc->ResetJournalManager();
}

BOOL CCommandSetProgramStatus_VCS_Plc2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetPlcStatus && !m_pCommand_GetPlcStatus->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ColdstartPlc && !m_pCommand_ColdstartPlc->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_WarmstartPlc && !m_pCommand_WarmstartPlc->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_HotstartPlc && !m_pCommand_HotstartPlc->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_StopPlc && !m_pCommand_StopPlc->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

