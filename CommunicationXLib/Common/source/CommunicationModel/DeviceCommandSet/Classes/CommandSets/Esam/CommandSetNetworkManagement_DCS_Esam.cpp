// CommandSetNetworkManagement_DCS_Esam.cpp: Implementierung der Klasse CCommandSetNetworkManagement_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetNetworkManagement_DCS_Esam.h"

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

CCommandSetNetworkManagement_DCS_Esam::CCommandSetNetworkManagement_DCS_Esam()
{
    m_strCommandSetName = COMMAND_SET_NETWORK_MANAGEMENT;

    m_pCommand_SendNMTService = NULL;

    InitCommands();
}

CCommandSetNetworkManagement_DCS_Esam::~CCommandSetNetworkManagement_DCS_Esam()
{
    DeleteCommands();
}

BOOL CCommandSetNetworkManagement_DCS_Esam::DCS_SendNMTService(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wNodeId, WORD wCommandSpecifier, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;
    DWORD dDeviceErrorCode = 0;

    if(m_pCommand_SendNMTService)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_SendNMTService->SetParameterData(1, &wNodeId, sizeof(wNodeId));
        m_pCommand_SendNMTService->SetParameterData(2, &wCommandSpecifier, sizeof(wCommandSpecifier));

        //Execute Command
        oResult = m_pCommand_SendNMTService->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendNMTService->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(pErrorInfo);

    }

    return oResult;
}

void CCommandSetNetworkManagement_DCS_Esam::InitCommands()
{
    DeleteCommands();

    //Init Command SendNMTService
    m_pCommand_SendNMTService = new CCommand_DCS_Esam();
    m_pCommand_SendNMTService->InitCommand(ESAM_SEND_NMT_SERVICE);
}

void CCommandSetNetworkManagement_DCS_Esam::DeleteCommands()
{
    if(m_pCommand_SendNMTService)
    {
        delete m_pCommand_SendNMTService;
        m_pCommand_SendNMTService = NULL;
    }
}

void CCommandSetNetworkManagement_DCS_Esam::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->InitJournalManager(pJournalManager);
}

void CCommandSetNetworkManagement_DCS_Esam::ResetJournalManager()
{
    if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->ResetJournalManager();
}

BOOL CCommandSetNetworkManagement_DCS_Esam::InitGateway(CGateway *pGateway)
{
    if(m_pCommand_SendNMTService && !m_pCommand_SendNMTService->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

CXXMLFile::CElementPart* CCommandSetNetworkManagement_DCS_Esam::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
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
        if(m_pCommand_SendNMTService && !m_pCommand_SendNMTService->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;

        return pElement;
    }

    return NULL;
}


