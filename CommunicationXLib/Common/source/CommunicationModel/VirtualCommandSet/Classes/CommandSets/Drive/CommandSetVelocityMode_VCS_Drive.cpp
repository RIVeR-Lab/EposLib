// CommandSetVelocityMode_VCS_Drive.cpp: Implementierung der Klasse CCommandSetVelocityMode_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandSetVelocityMode_VCS_Drive.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommandSetVelocityMode_VCS_Drive::CCommandSetVelocityMode_VCS_Drive()
{
    m_strCommandSetName = COMMAND_SET_VELOCITY_MODE;

    m_pCommand_GetVelocityMust = NULL;
    m_pCommand_SetVelocityMust = NULL;

    InitCommands();
}

CCommandSetVelocityMode_VCS_Drive::~CCommandSetVelocityMode_VCS_Drive()
{
    DeleteCommands();
}

BOOL CCommandSetVelocityMode_VCS_Drive::VCS_GetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long *p_plVelocityMust, CErrorInfo* p_pErrorInfo)
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

BOOL CCommandSetVelocityMode_VCS_Drive::VCS_SetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long p_lVelocityMust, CErrorInfo* p_pErrorInfo)
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

void CCommandSetVelocityMode_VCS_Drive::InitCommands()
{
    DeleteCommands();

    //Init GetVelocityMust
    m_pCommand_GetVelocityMust = new CCommand_VCS_Drive();
    m_pCommand_GetVelocityMust->InitCommand(DRIVE_GET_VELOCITY_MUST);

    //Init SetVelocityMust
    m_pCommand_SetVelocityMust = new CCommand_VCS_Drive;
    m_pCommand_SetVelocityMust->InitCommand(DRIVE_SET_VELOCITY_MUST);
}

void CCommandSetVelocityMode_VCS_Drive::DeleteCommands()
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
}

CXXMLFile::CElementPart* CCommandSetVelocityMode_VCS_Drive::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
    }

    return pElement;
}

void CCommandSetVelocityMode_VCS_Drive::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetVelocityMust) m_pCommand_GetVelocityMust->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetVelocityMust) m_pCommand_SetVelocityMust->InitJournalManager(p_pJournalManager);
}

void CCommandSetVelocityMode_VCS_Drive::ResetJournalManager()
{
    if(m_pCommand_GetVelocityMust) m_pCommand_GetVelocityMust->ResetJournalManager();
    if(m_pCommand_SetVelocityMust) m_pCommand_SetVelocityMust->ResetJournalManager();
}

BOOL CCommandSetVelocityMode_VCS_Drive::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetVelocityMust && !m_pCommand_GetVelocityMust->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetVelocityMust && !m_pCommand_SetVelocityMust->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

