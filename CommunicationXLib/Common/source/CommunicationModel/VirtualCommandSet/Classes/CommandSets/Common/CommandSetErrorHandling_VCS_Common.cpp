// CommandSetErrorHandling_VCS_Common.cpp: Implementierung der Klasse CCommandSetErrorHandling_VCS_Common.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include "CommandSetErrorHandling_VCS_Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetErrorHandling_VCS_Common::CCommandSetErrorHandling_VCS_Common()
{
    m_strCommandSetName = COMMAND_SET_ERROR_HANDLING;

    m_pCommand_ResetDevice = NULL;
    m_pCommand_ClearDeviceErrors = NULL;
    m_pCommand_GetNbOfDeviceErrors = NULL;
    m_pCommand_GetDeviceErrorCode = NULL;
    m_pCommand_GotoSafeState = NULL;

    InitCommands();
}

CCommandSetErrorHandling_VCS_Common::~CCommandSetErrorHandling_VCS_Common()
{
    DeleteCommands();
}

BOOL CCommandSetErrorHandling_VCS_Common::VCS_ResetDevice(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ResetDevice)
    {
        //Set Parameter Data
        m_pCommand_ResetDevice->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_ResetDevice->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_ResetDevice->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetErrorHandling_VCS_Common::VCS_ClearDeviceErrors(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ClearDeviceErrors)
    {
        //Set Parameter Data
        m_pCommand_ClearDeviceErrors->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_ClearDeviceErrors->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_ClearDeviceErrors->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetErrorHandling_VCS_Common::VCS_GetNbOfDeviceErrors(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubNbOfDeviceErrors, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetNbOfDeviceErrors)
    {
        //Set Parameter Data
        m_pCommand_GetNbOfDeviceErrors->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetNbOfDeviceErrors->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetNbOfDeviceErrors->GetReturnParameterData(0, p_pubNbOfDeviceErrors, sizeof(*p_pubNbOfDeviceErrors));

        //Get ErrorCode
        m_pCommand_GetNbOfDeviceErrors->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetErrorHandling_VCS_Common::VCS_GetDeviceErrorCode(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubErrorNumber, DWORD* p_pulDeviceErrorCode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetDeviceErrorCode)
    {
        //Set Parameter Data
        m_pCommand_GetDeviceErrorCode->ResetStatus();
        m_pCommand_GetDeviceErrorCode->SetParameterData(0, &p_ubErrorNumber, sizeof(p_ubErrorNumber));

        //ExecuteCommand
        oResult = m_pCommand_GetDeviceErrorCode->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetDeviceErrorCode->GetReturnParameterData(0, p_pulDeviceErrorCode, sizeof(*p_pulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_GetDeviceErrorCode->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetErrorHandling_VCS_Common::VCS_GotoSafeState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GotoSafeState)
    {
        //Set Parameter Data
        m_pCommand_GotoSafeState->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GotoSafeState->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_GotoSafeState->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetErrorHandling_VCS_Common::InitCommands()
{
    DeleteCommands();

    //Init ResetDevice
    m_pCommand_ResetDevice = new CCommand_VCS_Common();
    m_pCommand_ResetDevice->InitCommand(COMMON_RESET_DEVICE);

    //Init ClearDeviceErrors
    m_pCommand_ClearDeviceErrors = new CCommand_VCS_Common();
    m_pCommand_ClearDeviceErrors->InitCommand(COMMON_CLEAR_DEVICE_ERRORS);

    //Init GetNbOfDeviceErrors
    m_pCommand_GetNbOfDeviceErrors = new CCommand_VCS_Common();
    m_pCommand_GetNbOfDeviceErrors->InitCommand(COMMON_GET_NB_OF_DEVICE_ERRORS);

    //Init GetDeviceErrorCode
    m_pCommand_GetDeviceErrorCode = new CCommand_VCS_Common();
    m_pCommand_GetDeviceErrorCode->InitCommand(COMMON_GET_DEVICE_ERROR_CODE);

    //Init GotoSafeState
    m_pCommand_GotoSafeState = new CCommand_VCS_Common();
    m_pCommand_GotoSafeState->InitCommand(COMMON_GOTO_SAFE_STATE);
}

void CCommandSetErrorHandling_VCS_Common::DeleteCommands()
{
    if(m_pCommand_ResetDevice)
    {
        delete m_pCommand_ResetDevice;
        m_pCommand_ResetDevice = NULL;
    }
    if(m_pCommand_ClearDeviceErrors)
    {
        delete m_pCommand_ClearDeviceErrors;
        m_pCommand_ClearDeviceErrors = NULL;
    }
    if(m_pCommand_GetNbOfDeviceErrors)
    {
        delete m_pCommand_GetNbOfDeviceErrors;
        m_pCommand_GetNbOfDeviceErrors = NULL;
    }
    if(m_pCommand_GetDeviceErrorCode)
    {
        delete m_pCommand_GetDeviceErrorCode;
        m_pCommand_GetDeviceErrorCode = NULL;
    }
    if(m_pCommand_GotoSafeState)
    {
        delete m_pCommand_GotoSafeState;
        m_pCommand_GotoSafeState = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetErrorHandling_VCS_Common::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_ResetDevice && !m_pCommand_ResetDevice->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ClearDeviceErrors && !m_pCommand_ClearDeviceErrors->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetNbOfDeviceErrors && !m_pCommand_GetNbOfDeviceErrors->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetDeviceErrorCode && !m_pCommand_GetDeviceErrorCode->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetErrorHandling_VCS_Common::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_ResetDevice) m_pCommand_ResetDevice->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ClearDeviceErrors) m_pCommand_ClearDeviceErrors->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetNbOfDeviceErrors) m_pCommand_GetNbOfDeviceErrors->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetDeviceErrorCode) m_pCommand_GetDeviceErrorCode->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GotoSafeState) m_pCommand_GotoSafeState->InitJournalManager(p_pJournalManager);
}

void CCommandSetErrorHandling_VCS_Common::ResetJournalManager()
{
    if(m_pCommand_ResetDevice) m_pCommand_ResetDevice->ResetJournalManager();
    if(m_pCommand_ClearDeviceErrors) m_pCommand_ClearDeviceErrors->ResetJournalManager();
    if(m_pCommand_GetNbOfDeviceErrors) m_pCommand_GetNbOfDeviceErrors->ResetJournalManager();
    if(m_pCommand_GetDeviceErrorCode) m_pCommand_GetDeviceErrorCode->ResetJournalManager();
    if(m_pCommand_GotoSafeState) m_pCommand_GotoSafeState->ResetJournalManager();
}

BOOL CCommandSetErrorHandling_VCS_Common::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_ResetDevice && !m_pCommand_ResetDevice->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ClearDeviceErrors && !m_pCommand_ClearDeviceErrors->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetNbOfDeviceErrors && !m_pCommand_GetNbOfDeviceErrors->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetDeviceErrorCode && !m_pCommand_GetDeviceErrorCode->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GotoSafeState && !m_pCommand_GotoSafeState->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}
