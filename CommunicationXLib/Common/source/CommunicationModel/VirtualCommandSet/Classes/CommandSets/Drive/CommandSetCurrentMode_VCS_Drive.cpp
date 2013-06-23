// CommandSetCurrentMode_VCS_Drive.cpp: Implementierung der Klasse CCommandSetCurrentMode_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetCurrentMode_VCS_Drive.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetCurrentMode_VCS_Drive::CCommandSetCurrentMode_VCS_Drive()
{
    m_strCommandSetName = COMMAND_SET_CURRENT_MODE;

    m_pCommand_GetCurrentMust = NULL;
    m_pCommand_SetCurrentMust = NULL;

    InitCommands();
}

CCommandSetCurrentMode_VCS_Drive::~CCommandSetCurrentMode_VCS_Drive()
{
    DeleteCommands();
}

BOOL CCommandSetCurrentMode_VCS_Drive::VCS_GetCurrentMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short* p_psCurrentMust, CErrorInfo* p_pErrorInfo)
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

BOOL CCommandSetCurrentMode_VCS_Drive::VCS_SetCurrentMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short p_sCurrentMust, CErrorInfo* p_pErrorInfo)
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

void CCommandSetCurrentMode_VCS_Drive::InitCommands()
{
    DeleteCommands();

    //Init GetCurrentMust
    m_pCommand_GetCurrentMust = new CCommand_VCS_Drive();
    m_pCommand_GetCurrentMust->InitCommand(DRIVE_GET_CURRENT_MUST);

    //Init SetCurrentMust
    m_pCommand_SetCurrentMust = new CCommand_VCS_Drive();
    m_pCommand_SetCurrentMust->InitCommand(DRIVE_SET_CURRENT_MUST);
}

void CCommandSetCurrentMode_VCS_Drive::DeleteCommands()
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
}

CXXMLFile::CElementPart* CCommandSetCurrentMode_VCS_Drive::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
    }

    return pElement;
}

void CCommandSetCurrentMode_VCS_Drive::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetCurrentMust) m_pCommand_GetCurrentMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetCurrentMust) m_pCommand_SetCurrentMust->InitJournalManager(p_pJournalManager);
}

void CCommandSetCurrentMode_VCS_Drive::ResetJournalManager()
{
    if(m_pCommand_GetCurrentMust) m_pCommand_GetCurrentMust->ResetJournalManager();
    if(m_pCommand_SetCurrentMust) m_pCommand_SetCurrentMust->ResetJournalManager();
}

BOOL CCommandSetCurrentMode_VCS_Drive::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetCurrentMust && !m_pCommand_GetCurrentMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetCurrentMust && !m_pCommand_SetCurrentMust->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

