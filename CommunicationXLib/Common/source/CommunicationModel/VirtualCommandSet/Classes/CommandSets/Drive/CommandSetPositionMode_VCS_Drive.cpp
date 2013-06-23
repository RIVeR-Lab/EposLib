// CommandSetPositionMode_VCS_Drive.cpp: Implementierung der Klasse CCommandSetPositionMode_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetPositionMode_VCS_Drive.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetPositionMode_VCS_Drive::CCommandSetPositionMode_VCS_Drive()
{
    m_strCommandSetName = COMMAND_SET_POSITION_MODE;

    m_pCommand_GetPositionMust = NULL;
    m_pCommand_SetPositionMust = NULL;

    InitCommands();
}

CCommandSetPositionMode_VCS_Drive::~CCommandSetPositionMode_VCS_Drive()
{
    DeleteCommands();
}

BOOL CCommandSetPositionMode_VCS_Drive::VCS_GetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short *p_psPositionMust, CErrorInfo* p_pErrorInfo)
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

BOOL CCommandSetPositionMode_VCS_Drive::VCS_SetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short p_sPositionMust, CErrorInfo* p_pErrorInfo)
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

void CCommandSetPositionMode_VCS_Drive::InitCommands()
{
    DeleteCommands();

    //Init GetPositionMust
    m_pCommand_GetPositionMust    = new CCommand_VCS_Drive();
    m_pCommand_GetPositionMust->InitCommand(DRIVE_GET_POSITION_MUST);

    //Init SetPostionMust
    m_pCommand_SetPositionMust    = new CCommand_VCS_Drive();
    m_pCommand_SetPositionMust->InitCommand(DRIVE_SET_POSITION_MUST);
}

void CCommandSetPositionMode_VCS_Drive::DeleteCommands()
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
}

CXXMLFile::CElementPart* CCommandSetPositionMode_VCS_Drive::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
    }

    return pElement;
}

void CCommandSetPositionMode_VCS_Drive::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetPositionMust) m_pCommand_GetPositionMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetPositionMust) m_pCommand_SetPositionMust->InitJournalManager(p_pJournalManager);
}

void CCommandSetPositionMode_VCS_Drive::ResetJournalManager()
{
    if(m_pCommand_GetPositionMust) m_pCommand_GetPositionMust->ResetJournalManager();
    if(m_pCommand_SetPositionMust) m_pCommand_SetPositionMust->ResetJournalManager();
}

BOOL CCommandSetPositionMode_VCS_Drive::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetPositionMust && !m_pCommand_GetPositionMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetPositionMust && !m_pCommand_SetPositionMust->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

