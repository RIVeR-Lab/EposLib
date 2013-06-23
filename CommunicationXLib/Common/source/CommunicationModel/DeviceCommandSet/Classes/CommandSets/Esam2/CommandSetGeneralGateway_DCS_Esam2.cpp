// CommandSetGeneralGateway_DCS_Esam2.cpp: Implementierung der Klasse CCommandSetGeneralGateway_DCS_Esam2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetGeneralGateway_DCS_Esam2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommandSetGeneralGateway_DCS_Esam2::CCommandSetGeneralGateway_DCS_Esam2()
{
    m_strCommandSetName = COMMAND_SET_GENERAL_GATEWAY;

    m_pCommand_RequestCANFrame = NULL;
    m_pCommand_SendCANFrame = NULL;
    m_pCommand_ReadCANFrame = NULL;

    InitCommands();
}

CCommandSetGeneralGateway_DCS_Esam2::~CCommandSetGeneralGateway_DCS_Esam2()
{
    DeleteCommands();
}

BOOL CCommandSetGeneralGateway_DCS_Esam2::DCS_SendCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, WORD wCobID, WORD wLength, void* pData, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SendCANFrame)
    {
        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
        m_pCommand_SendCANFrame->SetParameterData(1, &wCobID, sizeof(wCobID));
        m_pCommand_SendCANFrame->SetParameterData(2, &wLength, sizeof(wLength));
        m_pCommand_SendCANFrame->SetParameterData(3, pData, wLength);

        //Execute Command
        oResult = m_pCommand_SendCANFrame->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendCANFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetGeneralGateway_DCS_Esam2::DCS_ReadCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, WORD wCobID, WORD wLength, void* pData, DWORD p_ulTimeOut, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ReadCANFrame)
    {
        //Set Parameter Data
        m_pCommand_ReadCANFrame->ResetStatus();
        m_pCommand_ReadCANFrame->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
        m_pCommand_ReadCANFrame->SetParameterData(1, &wCobID, sizeof(wCobID));
        m_pCommand_ReadCANFrame->SetParameterData(2, &wLength, sizeof(wLength));
        m_pCommand_ReadCANFrame->SetParameterData(3, &p_ulTimeOut, sizeof(p_ulTimeOut));

        //Execute Command
        oResult = m_pCommand_ReadCANFrame->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, pData, wLength);

        //Get ErrorCode
        m_pCommand_ReadCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetGeneralGateway_DCS_Esam2::DCS_RequestCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, WORD wCobID, WORD wLength, void* pData, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_RequestCANFrame)
    {
        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
        m_pCommand_RequestCANFrame->SetParameterData(1, &wCobID, sizeof(wCobID));
        m_pCommand_RequestCANFrame->SetParameterData(2, &wLength, sizeof(wLength));

        //Execute Command
        oResult = m_pCommand_RequestCANFrame->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_RequestCANFrame->GetReturnParameterData(1, pData, wLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

void CCommandSetGeneralGateway_DCS_Esam2::InitCommands()
{
    DeleteCommands();

    //Init Command Request
    m_pCommand_RequestCANFrame = new CCommand_DCS_Esam2();
    m_pCommand_RequestCANFrame->InitCommand(ESAM2_REQUEST_CAN_FRAME);

    //Init Command Send
    m_pCommand_SendCANFrame = new CCommand_DCS_Esam2();
    m_pCommand_SendCANFrame->InitCommand(ESAM2_SEND_CAN_FRAME);

    //Init Command Read
    m_pCommand_ReadCANFrame = new CCommand_DCS_Esam2();
    m_pCommand_ReadCANFrame->InitCommand(ESAM2_READ_CAN_FRAME);
}

void CCommandSetGeneralGateway_DCS_Esam2::DeleteCommands()
{
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

    if(m_pCommand_ReadCANFrame)
    {
        delete m_pCommand_ReadCANFrame;
        m_pCommand_ReadCANFrame = NULL;
    }
}

void CCommandSetGeneralGateway_DCS_Esam2::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->InitJournalManager(pJournalManager);
    if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadCANFrame) m_pCommand_ReadCANFrame->InitJournalManager(pJournalManager);
}

void CCommandSetGeneralGateway_DCS_Esam2::ResetJournalManager()
{
    if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->ResetJournalManager();
    if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->ResetJournalManager();
    if(m_pCommand_ReadCANFrame) m_pCommand_ReadCANFrame->ResetJournalManager();
}

BOOL CCommandSetGeneralGateway_DCS_Esam2::InitGateway(CGateway *pGateway)
{
    if(m_pCommand_RequestCANFrame && !m_pCommand_RequestCANFrame->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_SendCANFrame && !m_pCommand_SendCANFrame->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_ReadCANFrame && !m_pCommand_ReadCANFrame->InitGateway(pGateway)) return FALSE;

    return TRUE;
}

CXXMLFile::CElementPart* CCommandSetGeneralGateway_DCS_Esam2::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
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
        if(m_pCommand_SendCANFrame && !m_pCommand_SendCANFrame->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_RequestCANFrame && !m_pCommand_RequestCANFrame->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ReadCANFrame && !m_pCommand_ReadCANFrame->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;

        return pElement;
    }

    return NULL;
}

