// CommandSetGeneralGateway_VCS_CanOpen.cpp: Implementierung der Klasse CCommandSetGeneralGateway_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include "CommandSetGeneralGateway_VCS_CanOpen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetGeneralGateway_VCS_CanOpen::CCommandSetGeneralGateway_VCS_CanOpen()
{
    m_strCommandSetName = COMMAND_SET_GENERAL_GATEWAY;

    m_pCommand_ReadCANFrame = NULL;
    m_pCommand_RequestCANFrame = NULL;
    m_pCommand_SendCANFrame = NULL;
    m_pCommand_SendNMTService = NULL;

    InitCommands();
}

CCommandSetGeneralGateway_VCS_CanOpen::~CCommandSetGeneralGateway_VCS_CanOpen()
{
    DeleteCommands();
}

BOOL CCommandSetGeneralGateway_VCS_CanOpen::VCS_RequestCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_RequestCANFrame)
    {
        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_RequestCANFrame->SetParameterData(1, &p_usLength, sizeof(p_usLength));

        //ExecuteCommand
        oResult = m_pCommand_RequestCANFrame->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, p_pData, p_usLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetGeneralGateway_VCS_CanOpen::VCS_SendCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SendCANFrame)
    {
        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_SendCANFrame->SetParameterData(1, &p_usLength, sizeof(p_usLength));
        m_pCommand_SendCANFrame->SetParameterData(2, p_pData, p_usLength);

        //ExecuteCommand
        oResult = m_pCommand_SendCANFrame->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetGeneralGateway_VCS_CanOpen::VCS_ReadCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ReadCANFrame)
    {
        //Set Parameter Data
        m_pCommand_ReadCANFrame->ResetStatus();
        m_pCommand_ReadCANFrame->SetParameterData(0, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_ReadCANFrame->SetParameterData(1, &p_usLength, sizeof(p_usLength));
        m_pCommand_ReadCANFrame->SetParameterData(2, &p_ulTimeout, sizeof(p_ulTimeout));

        //ExecuteCommand
        oResult = m_pCommand_ReadCANFrame->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, p_pData, p_usLength);

        //Get ErrorCode
        m_pCommand_ReadCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetGeneralGateway_VCS_CanOpen::VCS_SendNMTService(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, WORD p_usCommandSpecifier, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SendNMTService)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &p_usNodeId, sizeof(p_usNodeId));
        m_pCommand_SendNMTService->SetParameterData(1, &p_usCommandSpecifier, sizeof(p_usCommandSpecifier));

        //ExecuteCommand
        oResult = m_pCommand_SendNMTService->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetGeneralGateway_VCS_CanOpen::InitCommands()
{
    DeleteCommands();

    //Init ReadCANFrame
    m_pCommand_ReadCANFrame = new CCommand_VCS_CanOpen();
    m_pCommand_ReadCANFrame->InitCommand(VCS_CANOPEN_READ_CAN_FRAME);

    //Init RequestCANFrame
    m_pCommand_RequestCANFrame = new CCommand_VCS_CanOpen();
    m_pCommand_RequestCANFrame->InitCommand(VCS_CANOPEN_REQUEST_CAN_FRAME);

    //Init SendCANFrame
    m_pCommand_SendCANFrame = new CCommand_VCS_CanOpen();
    m_pCommand_SendCANFrame->InitCommand(VCS_CANOPEN_SEND_CAN_FRAME);

    //Init SendNMTService
    m_pCommand_SendNMTService = new CCommand_VCS_CanOpen();
    m_pCommand_SendNMTService->InitCommand(VCS_CANOPEN_SEND_NMT_SERVICE);
}

void CCommandSetGeneralGateway_VCS_CanOpen::DeleteCommands()
{
    if(m_pCommand_ReadCANFrame)
    {
        delete m_pCommand_ReadCANFrame;
        m_pCommand_ReadCANFrame = NULL;
    }

    if(m_pCommand_RequestCANFrame)
    {
        delete m_pCommand_RequestCANFrame;
        m_pCommand_RequestCANFrame = NULL;
    }

    if(m_pCommand_SendCANFrame)
    {
        delete m_pCommand_SendCANFrame;
        m_pCommand_SendCANFrame = NULL;
    }

    if(m_pCommand_SendNMTService)
    {
        delete m_pCommand_SendNMTService;
        m_pCommand_SendNMTService = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetGeneralGateway_VCS_CanOpen::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_SendCANFrame && !m_pCommand_SendCANFrame->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_RequestCANFrame && !m_pCommand_RequestCANFrame->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SendNMTService && !m_pCommand_SendNMTService->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetGeneralGateway_VCS_CanOpen::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_ReadCANFrame) m_pCommand_ReadCANFrame->InitJournalManager(p_pJournalManager);
    if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->InitJournalManager(p_pJournalManager);
}

void CCommandSetGeneralGateway_VCS_CanOpen::ResetJournalManager()
{
    if(m_pCommand_ReadCANFrame) m_pCommand_ReadCANFrame->ResetJournalManager();
    if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->ResetJournalManager();
    if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->ResetJournalManager();
    if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->ResetJournalManager();
}

BOOL CCommandSetGeneralGateway_VCS_CanOpen::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_ReadCANFrame && !m_pCommand_ReadCANFrame->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_RequestCANFrame && !m_pCommand_RequestCANFrame->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SendCANFrame && !m_pCommand_SendCANFrame->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SendNMTService && !m_pCommand_SendNMTService->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}
