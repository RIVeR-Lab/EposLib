// CommandSetLayerSettingServices_DCS_Esam.cpp: Implementierung der Klasse CCommandSetLayerSettingServices_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetLayerSettingServices_DCS_Esam.h"

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

CCommandSetLayerSettingServices_DCS_Esam::CCommandSetLayerSettingServices_DCS_Esam()
{
    m_strCommandSetName = COMMAND_SET_LAYER_SETTING_SERVICES;

    m_pCommand_ReadLSSFrame = NULL;
    m_pCommand_SendLSSFrame = NULL;

    InitCommands();
}

CCommandSetLayerSettingServices_DCS_Esam::~CCommandSetLayerSettingServices_DCS_Esam()
{
    DeleteCommands();
}

BOOL CCommandSetLayerSettingServices_DCS_Esam::DCS_SendLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SendLSSFrame)
    {
        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_SendLSSFrame->SetParameterData(1, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = m_pCommand_SendLSSFrame->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendLSSFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(pErrorInfo);

    }

    return oResult;
}

BOOL CCommandSetLayerSettingServices_DCS_Esam::DCS_ReadLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadLSSFrame)
    {
        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_ReadLSSFrame->SetParameterData(1, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = m_pCommand_ReadLSSFrame->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_ReadLSSFrame->GetReturnParameterData(1, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(pErrorInfo);

    }

    return oResult;
}

void CCommandSetLayerSettingServices_DCS_Esam::InitCommands()
{
    DeleteCommands();

    //Init Command SendLSSFrame
    m_pCommand_SendLSSFrame = new CCommand_DCS_Esam();
    m_pCommand_SendLSSFrame->InitCommand(ESAM_SEND_LSS_FRAME);

    //Init Command ReadLSSFrame
    m_pCommand_ReadLSSFrame = new CCommand_DCS_Esam();
    m_pCommand_ReadLSSFrame->InitCommand(ESAM_SEND_LSS_FRAME);
}

void CCommandSetLayerSettingServices_DCS_Esam::DeleteCommands()
{
    if(m_pCommand_SendLSSFrame)
    {
        delete m_pCommand_SendLSSFrame;
        m_pCommand_SendLSSFrame = NULL;
    }

    if(m_pCommand_ReadLSSFrame)
    {
        delete m_pCommand_ReadLSSFrame;
        m_pCommand_ReadLSSFrame = NULL;
    }
}

void CCommandSetLayerSettingServices_DCS_Esam::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->InitJournalManager(pJournalManager);
}

void CCommandSetLayerSettingServices_DCS_Esam::ResetJournalManager()
{
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->ResetJournalManager();
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->ResetJournalManager();
}

BOOL CCommandSetLayerSettingServices_DCS_Esam::InitGateway(CGateway *pGateway)
{
    if(m_pCommand_SendLSSFrame && !m_pCommand_SendLSSFrame->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_ReadLSSFrame && !m_pCommand_ReadLSSFrame->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

CXXMLFile::CElementPart* CCommandSetLayerSettingServices_DCS_Esam::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
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
        if(m_pCommand_SendLSSFrame && !m_pCommand_SendLSSFrame->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ReadLSSFrame && !m_pCommand_ReadLSSFrame->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;

        return pElement;
    }

    return NULL;
}

