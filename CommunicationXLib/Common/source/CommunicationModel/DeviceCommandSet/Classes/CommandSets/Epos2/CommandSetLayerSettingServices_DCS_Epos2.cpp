// CommandSetLayerSettingServices_DCS_Epos2.cpp: Implementierung der Klasse CCommandSetLayerSettingServices_DCS_Epos2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetLayerSettingServices_DCS_Epos2.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEpos2Def.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommandSetLayerSettingServices_DCS_Epos2::CCommandSetLayerSettingServices_DCS_Epos2()
{
    m_strCommandSetName = COMMAND_SET_LAYER_SETTING_SERVICES;

    m_pCommand_ReadLSSFrame = NULL;
    m_pCommand_SendLSSFrame = NULL;

    InitCommands();
}

CCommandSetLayerSettingServices_DCS_Epos2::~CCommandSetLayerSettingServices_DCS_Epos2()
{
    DeleteCommands();
}

BOOL CCommandSetLayerSettingServices_DCS_Epos2::DCS_SendLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SendLSSFrame)
    {
        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = m_pCommand_SendLSSFrame->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendLSSFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetLayerSettingServices_DCS_Epos2::DCS_ReadLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadLSSFrame)
    {
        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &wTimeout, sizeof(wTimeout));

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

void CCommandSetLayerSettingServices_DCS_Epos2::InitCommands()
{
    DeleteCommands();

    //Init Command SendLSSFrame
    m_pCommand_SendLSSFrame = new CCommand_DCS_Epos2();
    m_pCommand_SendLSSFrame->InitCommand(EPOS2_SEND_LSS_FRAME);

    //Init Command ReadLSSFrame
    m_pCommand_ReadLSSFrame = new CCommand_DCS_Epos2();
    m_pCommand_ReadLSSFrame->InitCommand(EPOS2_READ_LSS_FRAME);
}

void CCommandSetLayerSettingServices_DCS_Epos2::DeleteCommands()
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

void CCommandSetLayerSettingServices_DCS_Epos2::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->InitJournalManager(pJournalManager);
}

void CCommandSetLayerSettingServices_DCS_Epos2::ResetJournalManager()
{
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->ResetJournalManager();
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->ResetJournalManager();
}

BOOL CCommandSetLayerSettingServices_DCS_Epos2::InitGateway(CGateway *pGateway)
{
    if(m_pCommand_SendLSSFrame && !m_pCommand_SendLSSFrame->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_ReadLSSFrame && !m_pCommand_ReadLSSFrame->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

CXXMLFile::CElementPart* CCommandSetLayerSettingServices_DCS_Epos2::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
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

