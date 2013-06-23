// CommandSetLayerSettingServices_VCS_CanOpen.cpp: Implementierung der Klasse CCommandSetLayerSettingServices_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include "CommandSetLayerSettingServices_VCS_CanOpen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetLayerSettingServices_VCS_CanOpen::CCommandSetLayerSettingServices_VCS_CanOpen()
{
    m_strCommandSetName = COMMAND_SET_LAYER_SETTING_SERVICES;

    m_pCommand_ReadLSSFrame = NULL;
    m_pCommand_SendLSSFrame = NULL;

    InitCommands();
}

CCommandSetLayerSettingServices_VCS_CanOpen::~CCommandSetLayerSettingServices_VCS_CanOpen()
{
    DeleteCommands();
}

BOOL CCommandSetLayerSettingServices_VCS_CanOpen::VCS_SendLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SendLSSFrame)
    {
        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, p_pDataBuffer, p_ulDataBufferLength);

        //Execute Command
        oResult = m_pCommand_SendLSSFrame->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendLSSFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetLayerSettingServices_VCS_CanOpen::VCS_ReadLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usTimeout, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadLSSFrame)
    {
        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &p_usTimeout, sizeof(p_usTimeout));

        //Execute Command
        oResult = m_pCommand_ReadLSSFrame->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_ReadLSSFrame->GetReturnParameterData(1, p_pDataBuffer, p_ulDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetLayerSettingServices_VCS_CanOpen::InitCommands()
{
    DeleteCommands();

    //Init ReadLSSFrame
    m_pCommand_ReadLSSFrame = new CCommand_VCS_CanOpen();
    m_pCommand_ReadLSSFrame->InitCommand(VCS_CANOPEN_READ_LSS_FRAME);

    //Init SendLSSFrame
    m_pCommand_SendLSSFrame = new CCommand_VCS_CanOpen();
    m_pCommand_SendLSSFrame->InitCommand(VCS_CANOPEN_SEND_LSS_FRAME);
}

void CCommandSetLayerSettingServices_VCS_CanOpen::DeleteCommands()
{
    if(m_pCommand_ReadLSSFrame)
    {
        delete m_pCommand_ReadLSSFrame;
        m_pCommand_ReadLSSFrame = NULL;
    }

    if(m_pCommand_SendLSSFrame)
    {
        delete m_pCommand_SendLSSFrame;
        m_pCommand_SendLSSFrame = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetLayerSettingServices_VCS_CanOpen::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_SendLSSFrame && !m_pCommand_SendLSSFrame->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ReadLSSFrame && !m_pCommand_ReadLSSFrame->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetLayerSettingServices_VCS_CanOpen::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->InitJournalManager(p_pJournalManager);
}

void CCommandSetLayerSettingServices_VCS_CanOpen::ResetJournalManager()
{
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->ResetJournalManager();
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->ResetJournalManager();
}

BOOL CCommandSetLayerSettingServices_VCS_CanOpen::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SendLSSFrame && !m_pCommand_SendLSSFrame->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ReadLSSFrame && !m_pCommand_ReadLSSFrame->InitGateway(p_pGateway)) return FALSE;
    return TRUE;
}
