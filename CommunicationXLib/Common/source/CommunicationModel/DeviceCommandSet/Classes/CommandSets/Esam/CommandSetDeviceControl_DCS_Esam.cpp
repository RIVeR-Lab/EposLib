// CommandSetDeviceControl_DCS_Esam.cpp: Implementierung der Klasse CCommandSetDeviceControl_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetDeviceControl_DCS_Esam.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommandSetDeviceControl_DCS_Esam::CCommandSetDeviceControl_DCS_Esam()
{
    m_strCommandSetName = COMMAND_SET_DEVICE_CONTROL;

    m_pCommand_Login = NULL;
    m_pCommand_Logout = NULL;
    m_pCommand_SetState = NULL;

    InitCommands();
}

CCommandSetDeviceControl_DCS_Esam::~CCommandSetDeviceControl_DCS_Esam()
{
    DeleteCommands();
}

void CCommandSetDeviceControl_DCS_Esam::InitCommands()
{
    DeleteCommands();

    //Init Command Login
    m_pCommand_Login = new CCommand_DCS_Esam();
    m_pCommand_Login->InitCommand(ESAM_LOGIN);

    //Init Command Logout
    m_pCommand_Logout = new CCommand_DCS_Esam();
    m_pCommand_Logout->InitCommand(ESAM_LOGOUT);

    //Init Command SetState
    m_pCommand_SetState = new CCommand_DCS_Esam();
    m_pCommand_SetState->InitCommand(ESAM_SET_STATE);
}

void CCommandSetDeviceControl_DCS_Esam::DeleteCommands()
{
    if(m_pCommand_Login)
    {
        delete m_pCommand_Login;
        m_pCommand_Login = NULL;
    }

    if(m_pCommand_Logout)
    {
        delete m_pCommand_Logout;
        m_pCommand_Logout = NULL;
    }

    if(m_pCommand_SetState)
    {
        delete m_pCommand_SetState;
        m_pCommand_SetState = NULL;
    }
}

void CCommandSetDeviceControl_DCS_Esam::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_Login) m_pCommand_Login->InitJournalManager(pJournalManager);
    if(m_pCommand_Logout) m_pCommand_Logout->InitJournalManager(pJournalManager);
    if(m_pCommand_SetState) m_pCommand_SetState->InitJournalManager(pJournalManager);
}

void CCommandSetDeviceControl_DCS_Esam::ResetJournalManager()
{
    if(m_pCommand_Login) m_pCommand_Login->ResetJournalManager();
    if(m_pCommand_Logout) m_pCommand_Logout->ResetJournalManager();
    if(m_pCommand_SetState) m_pCommand_SetState->ResetJournalManager();
}

BOOL CCommandSetDeviceControl_DCS_Esam::InitGateway(CGateway *pGateway)
{
    if(m_pCommand_Login && !m_pCommand_Login->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_Logout && !m_pCommand_Logout->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_SetState && !m_pCommand_SetState->InitGateway(pGateway)) return FALSE;
    return TRUE;
}
CXXMLFile::CElementPart* CCommandSetDeviceControl_DCS_Esam::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
{
    CXXMLFile::CElement* pElement;
    BOOL oCheckVisibility = FALSE;

    if(pFile && pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
        pFile->SetText(pElement, "CommandSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_Login && !m_pCommand_Login->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_Logout && !m_pCommand_Logout->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetState && !m_pCommand_SetState->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;

        return pElement;
    }

    return NULL;
}

BOOL CCommandSetDeviceControl_DCS_Esam::DCS_SetState(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uRunCommand, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;
    BYTE uReturnCode;

    if(m_pCommand_SetState)
    {
        //Set Parameter Data
        m_pCommand_SetState->ResetStatus();
        m_pCommand_SetState->SetParameterData(0, &uRunCommand, sizeof(uRunCommand));

        //Execute Command
        oResult = m_pCommand_SetState->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SetState->GetReturnParameterData(0, &uReturnCode, sizeof(uReturnCode));

        //Get ErrorCode
        m_pCommand_SetState->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDeviceControl_DCS_Esam::DCS_Login(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo)
{
    const DWORD TIMEOUT = 1000;
    const WORD MAX_RECEIVE_SIZE = 257;

    BOOL oResult = FALSE;
    WORD wMaxReceiveSize = MAX_RECEIVE_SIZE;
    BYTE uReturnCode;

    if(m_pCommand_Login)
    {
        //Set Parameter Data
        m_pCommand_Login->ResetStatus();
        m_pCommand_Login->SetTimeout(TIMEOUT);
        m_pCommand_Login->SetParameterData(0, &wMaxReceiveSize, sizeof(wMaxReceiveSize));

        //Execute Command
        oResult = m_pCommand_Login->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_Login->GetReturnParameterData(0, &uReturnCode, sizeof(uReturnCode));

        //Get ErrorCode
        m_pCommand_Login->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDeviceControl_DCS_Esam::DCS_Logout(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;
    BYTE uReturnCode;

    if(m_pCommand_Logout)
    {
        //Set Parameter Data
        m_pCommand_Logout->ResetStatus();

        //Execute Command
        oResult = m_pCommand_Logout->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_Logout->GetReturnParameterData(0, &uReturnCode, sizeof(uReturnCode));

        //Get ErrorCode
        m_pCommand_Logout->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

