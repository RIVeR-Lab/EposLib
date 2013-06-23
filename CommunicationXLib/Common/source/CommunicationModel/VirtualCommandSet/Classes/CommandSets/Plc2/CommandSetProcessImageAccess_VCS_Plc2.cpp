// CommandSetProcessImageAccess_VCS_Plc2.cpp: Implementierung der Klasse CCommandSetProcessImageAccess_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetProcessImageAccess_VCS_Plc2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetProcessImageAccess_VCS_Plc2::CCommandSetProcessImageAccess_VCS_Plc2()
{
    m_strCommandSetName = COMMAND_SET_PROCESS_IMAGE_ACCESS;

    m_pCommand_GetProcessOutputImage = NULL;
    m_pCommand_SetProcessInputImage = NULL;

    InitCommands();
}

CCommandSetProcessImageAccess_VCS_Plc2::~CCommandSetProcessImageAccess_VCS_Plc2()
{
    DeleteCommands();
}

BOOL CCommandSetProcessImageAccess_VCS_Plc2::VCS_SetProcessInputImage(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulSelectionStart, DWORD p_ulSelectionLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_SetProcessInputImage)
    {
        //Set Parameter Data
        m_pCommand_SetProcessInputImage->ResetStatus();
        m_pCommand_SetProcessInputImage->SetParameterData(0, &p_ulSelectionStart, sizeof(p_ulSelectionStart));
        m_pCommand_SetProcessInputImage->SetParameterData(1, &p_ulSelectionLength, sizeof(p_ulSelectionLength));
        m_pCommand_SetProcessInputImage->SetParameterData(2, p_pDataBuffer, p_ulDataBufferLength);

        //Execute Command
        oResult = m_pCommand_SetProcessInputImage->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetProcessInputImage->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProcessImageAccess_VCS_Plc2::VCS_GetProcessOutputImage(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulSelectionStart, DWORD p_ulSelectionLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_GetProcessOutputImage)
    {
        //Set Parameter Data
        m_pCommand_GetProcessOutputImage->ResetStatus();
        m_pCommand_GetProcessOutputImage->SetParameterData(0, &p_ulSelectionStart, sizeof(p_ulSelectionStart));
        m_pCommand_GetProcessOutputImage->SetParameterData(1, &p_ulSelectionLength, sizeof(p_ulSelectionLength));

        //Execute Command
        oResult = m_pCommand_GetProcessOutputImage->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetProcessOutputImage->GetReturnParameterData(0, p_pDataBuffer, p_ulDataBufferLength);

        //Get ErrorCode
        m_pCommand_GetProcessOutputImage->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetProcessImageAccess_VCS_Plc2::InitCommands()
{
    DeleteCommands();

    //Init GetProcessOutputImage
    m_pCommand_GetProcessOutputImage = new CCommand_VCS_Plc2();
    m_pCommand_GetProcessOutputImage->InitCommand(PLC2_GET_PROCESS_OUTPUT_IMAGE);

    //Init SetProcessInputImage
    m_pCommand_SetProcessInputImage = new CCommand_VCS_Plc2();
    m_pCommand_SetProcessInputImage->InitCommand(PLC2_SET_PROCESS_INPUT_IMAGE);
}

void CCommandSetProcessImageAccess_VCS_Plc2::DeleteCommands()
{
    if(m_pCommand_GetProcessOutputImage)
    {
        delete m_pCommand_GetProcessOutputImage;
        m_pCommand_GetProcessOutputImage = NULL;
    }

    if(m_pCommand_SetProcessInputImage)
    {
        delete m_pCommand_SetProcessInputImage;
        m_pCommand_SetProcessInputImage = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetProcessImageAccess_VCS_Plc2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement(NULL);
    BOOL oCheckVisibility(FALSE);

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_SetProcessInputImage && !m_pCommand_SetProcessInputImage->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetProcessOutputImage && !m_pCommand_GetProcessOutputImage->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetProcessImageAccess_VCS_Plc2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetProcessInputImage) m_pCommand_SetProcessInputImage->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetProcessOutputImage) m_pCommand_GetProcessOutputImage->InitJournalManager(p_pJournalManager);
}

void CCommandSetProcessImageAccess_VCS_Plc2::ResetJournalManager()
{
    if(m_pCommand_SetProcessInputImage) m_pCommand_SetProcessInputImage->ResetJournalManager();
    if(m_pCommand_GetProcessOutputImage) m_pCommand_GetProcessOutputImage->ResetJournalManager();
}

BOOL CCommandSetProcessImageAccess_VCS_Plc2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SetProcessInputImage && !m_pCommand_SetProcessInputImage->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetProcessOutputImage && !m_pCommand_GetProcessOutputImage->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

