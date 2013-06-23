// CommandSetHomingMode_VCS_Drive2.cpp: Implementierung der Klasse CCommandSetHomingMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandSetHomingMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommandSetHomingMode_VCS_Drive2::CCommandSetHomingMode_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_HOMING_MODE;

    m_pCommand_FindHome = NULL;
    m_pCommand_GetHomingParameter = NULL;
    m_pCommand_SetHomingParameter = NULL;
    m_pCommand_StopHoming = NULL;
    m_pCommand_DefinePosition = NULL;

    InitCommands();
}

CCommandSetHomingMode_VCS_Drive2::~CCommandSetHomingMode_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetHomingMode_VCS_Drive2::VCS_FindHome(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, __int8 p_bHomingMethod, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_FindHome)
    {
        //Set Parameter Data
        m_pCommand_FindHome->ResetStatus();
        m_pCommand_FindHome->SetParameterData(0, &p_bHomingMethod, sizeof(p_bHomingMethod));

        //ExecuteCommand
        oResult = m_pCommand_FindHome->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_FindHome->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetHomingMode_VCS_Drive2::VCS_GetHomingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD *p_pulHomingAcceleration, DWORD *p_pulSpeedSwitch, DWORD *p_pulSpeedIndex, long *p_plHomeOffset, WORD* p_pusCurrentThreshold, long *p_plHomePosition, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetHomingParameter)
    {
        //Set Parameter Data
        m_pCommand_GetHomingParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetHomingParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetHomingParameter->GetReturnParameterData(0, p_pulHomingAcceleration, sizeof(*p_pulHomingAcceleration));
        m_pCommand_GetHomingParameter->GetReturnParameterData(1, p_pulSpeedSwitch, sizeof(*p_pulSpeedSwitch));
        m_pCommand_GetHomingParameter->GetReturnParameterData(2, p_pulSpeedIndex, sizeof(*p_pulSpeedIndex));
        m_pCommand_GetHomingParameter->GetReturnParameterData(3, p_plHomeOffset, sizeof(*p_plHomeOffset));
        m_pCommand_GetHomingParameter->GetReturnParameterData(4, p_pusCurrentThreshold, sizeof(*p_pusCurrentThreshold));
        m_pCommand_GetHomingParameter->GetReturnParameterData(5, p_plHomePosition, sizeof(*p_plHomePosition));

        //Get ErrorCode
        m_pCommand_GetHomingParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetHomingMode_VCS_Drive2::VCS_SetHomingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulHomingAcceleration, DWORD p_ulSpeedSwitch, DWORD p_ulSpeedIndex, long p_lHomeOffset, WORD p_usCurrentThreshold, long p_lHomePosition, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetHomingParameter)
    {
        //Set Parameter Data
        m_pCommand_SetHomingParameter->ResetStatus();
        m_pCommand_SetHomingParameter->SetParameterData(0, &p_ulHomingAcceleration, sizeof(p_ulHomingAcceleration));
        m_pCommand_SetHomingParameter->SetParameterData(1, &p_ulSpeedSwitch, sizeof(p_ulSpeedSwitch));
        m_pCommand_SetHomingParameter->SetParameterData(2, &p_ulSpeedIndex, sizeof(p_ulSpeedIndex));
        m_pCommand_SetHomingParameter->SetParameterData(3, &p_lHomeOffset, sizeof(p_lHomeOffset));
        m_pCommand_SetHomingParameter->SetParameterData(4, &p_usCurrentThreshold, sizeof(p_usCurrentThreshold));
        m_pCommand_SetHomingParameter->SetParameterData(5, &p_lHomePosition, sizeof(p_lHomePosition));

        //ExecuteCommand
        oResult = m_pCommand_SetHomingParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetHomingParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetHomingMode_VCS_Drive2::VCS_StopHoming(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_StopHoming)
    {
        //Set Parameter Data
        m_pCommand_StopHoming->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_StopHoming->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_StopHoming->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetHomingMode_VCS_Drive2::VCS_DefinePosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long p_lHomePosition, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DefinePosition)
    {
        //Set Parameter Data
        m_pCommand_DefinePosition->ResetStatus();
        m_pCommand_DefinePosition->SetParameterData(0, &p_lHomePosition, sizeof(p_lHomePosition));

        //ExecuteCommand
        oResult = m_pCommand_DefinePosition->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DefinePosition->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetHomingMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init FindHome command
    m_pCommand_FindHome = new CCommand_VCS_Drive2();
    m_pCommand_FindHome->InitCommand(DRIVE2_FIND_HOME);

    //Init GetHomingParameter command
    m_pCommand_GetHomingParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetHomingParameter->InitCommand(DRIVE2_GET_HOMING_PARAMETER);

    //Init SetHomingParameter command
    m_pCommand_SetHomingParameter = new CCommand_VCS_Drive2();
    m_pCommand_SetHomingParameter->InitCommand(DRIVE2_SET_HOMING_PARAMETER);

    //Init Stophoming
    m_pCommand_StopHoming = new CCommand_VCS_Drive2();
    m_pCommand_StopHoming->InitCommand(DRIVE2_STOP_HOMING);

    //Init Define Position
    m_pCommand_DefinePosition = new CCommand_VCS_Drive2();
    m_pCommand_DefinePosition->InitCommand(DRIVE2_DEFINE_POSITION);
}

void CCommandSetHomingMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_FindHome)
    {
        delete m_pCommand_FindHome;
        m_pCommand_FindHome = NULL;
    }

    if(m_pCommand_GetHomingParameter)
    {
        delete m_pCommand_GetHomingParameter;
        m_pCommand_GetHomingParameter = NULL;
    }

    if(m_pCommand_SetHomingParameter)
    {
        delete m_pCommand_SetHomingParameter;
        m_pCommand_SetHomingParameter = NULL;
    }

    if(m_pCommand_StopHoming)
    {
        delete m_pCommand_StopHoming;
        m_pCommand_StopHoming = NULL;
    }

    if(m_pCommand_DefinePosition)
    {
        delete m_pCommand_DefinePosition;
        m_pCommand_DefinePosition = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetHomingMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetHomingParameter && !m_pCommand_GetHomingParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetHomingParameter && !m_pCommand_SetHomingParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_FindHome && !m_pCommand_FindHome->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_StopHoming && !m_pCommand_StopHoming->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DefinePosition && !m_pCommand_DefinePosition->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetHomingMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_FindHome) m_pCommand_FindHome->ResetJournalManager();
    if(m_pCommand_GetHomingParameter) m_pCommand_GetHomingParameter->ResetJournalManager();
    if(m_pCommand_SetHomingParameter) m_pCommand_SetHomingParameter->ResetJournalManager();
    if(m_pCommand_StopHoming) m_pCommand_StopHoming->ResetJournalManager();
    if(m_pCommand_DefinePosition) m_pCommand_DefinePosition->ResetJournalManager();
}

void CCommandSetHomingMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_FindHome) m_pCommand_FindHome->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetHomingParameter) m_pCommand_GetHomingParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetHomingParameter) m_pCommand_SetHomingParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_StopHoming) m_pCommand_StopHoming->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DefinePosition) m_pCommand_DefinePosition->InitJournalManager(p_pJournalManager);
}

BOOL CCommandSetHomingMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_FindHome && !m_pCommand_FindHome->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetHomingParameter && !m_pCommand_GetHomingParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetHomingParameter && !m_pCommand_SetHomingParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_StopHoming && !m_pCommand_StopHoming->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DefinePosition && !m_pCommand_DefinePosition->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

