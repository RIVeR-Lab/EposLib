// CommandSetProcessInputOutputAccess_VCS_Plc2.cpp: Implementierung der Klasse CCommandSetProcessInputOutputAccess_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetProcessInputOutputAccess_VCS_Plc2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetProcessInputOutputAccess_VCS_Plc2::CCommandSetProcessInputOutputAccess_VCS_Plc2()
{
    m_strCommandSetName = COMMAND_SET_PROCESS_INPUT_OUTPUT_ACCESS;

    m_pCommand_GetProcessOutput = NULL;
    m_pCommand_SetProcessInput = NULL;
	m_pCommand_GetProcessOutputBit = NULL;
    m_pCommand_SetProcessInputBit = NULL;

    InitCommands();
}

CCommandSetProcessInputOutputAccess_VCS_Plc2::~CCommandSetProcessInputOutputAccess_VCS_Plc2()
{
    DeleteCommands();
}

BOOL CCommandSetProcessInputOutputAccess_VCS_Plc2::VCS_SetProcessInput(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessInputType, BYTE p_ubElementNumber, void* p_pDataBuffer, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_SetProcessInput)
    {
        //Set Parameter Data
        m_pCommand_SetProcessInput->ResetStatus();
        m_pCommand_SetProcessInput->SetParameterData(0, &p_usProcessInputType, sizeof(p_usProcessInputType));
        m_pCommand_SetProcessInput->SetParameterData(1, &p_ubElementNumber, sizeof(p_ubElementNumber));
        m_pCommand_SetProcessInput->SetParameterData(2, &p_ulNbOfBytesToWrite, sizeof(p_ulNbOfBytesToWrite));
        m_pCommand_SetProcessInput->SetParameterData(3, p_pDataBuffer, p_ulNbOfBytesToWrite);

        //Execute Command
        oResult = m_pCommand_SetProcessInput->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SetProcessInput->GetReturnParameterData(0, p_pulNbOfBytesWritten, sizeof(*p_pulNbOfBytesWritten));

        //Get ErrorCode
        m_pCommand_SetProcessInput->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProcessInputOutputAccess_VCS_Plc2::VCS_GetProcessOutput(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessOutputType, BYTE p_ubElementNumber, void* p_pDataBuffer, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_GetProcessOutput)
    {
        //Set Parameter Data
        m_pCommand_GetProcessOutput->ResetStatus();
        m_pCommand_GetProcessOutput->SetParameterData(0, &p_usProcessOutputType, sizeof(p_usProcessOutputType));
        m_pCommand_GetProcessOutput->SetParameterData(1, &p_ubElementNumber, sizeof(p_ubElementNumber));
        m_pCommand_GetProcessOutput->SetParameterData(2, &p_ulNbOfBytesToRead, sizeof(p_ulNbOfBytesToRead));

        //Execute Command
        oResult = m_pCommand_GetProcessOutput->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetProcessOutput->GetReturnParameterData(0, p_pulNbOfBytesRead, sizeof(*p_pulNbOfBytesRead));
        m_pCommand_GetProcessOutput->GetReturnParameterData(1, p_pDataBuffer, p_ulNbOfBytesToRead);

        //Get ErrorCode
        m_pCommand_GetProcessOutput->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProcessInputOutputAccess_VCS_Plc2::VCS_SetProcessInputBit(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessInputType, BYTE p_ubElementNumber, BYTE p_ubBitNumber, BYTE p_ubBitState, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_SetProcessInputBit)
    {
        //Set Parameter Data
        m_pCommand_SetProcessInputBit->ResetStatus();
        m_pCommand_SetProcessInputBit->SetParameterData(0, &p_usProcessInputType, sizeof(p_usProcessInputType));
        m_pCommand_SetProcessInputBit->SetParameterData(1, &p_ubElementNumber, sizeof(p_ubElementNumber));
        m_pCommand_SetProcessInputBit->SetParameterData(2, &p_ubBitNumber, sizeof(p_ubBitNumber));
		m_pCommand_SetProcessInputBit->SetParameterData(2, &p_ubBitState, sizeof(p_ubBitState));
        
        //Execute Command
        oResult = m_pCommand_SetProcessInputBit->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetProcessInputBit->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProcessInputOutputAccess_VCS_Plc2::VCS_GetProcessOutputBit(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessOutputType, BYTE p_ubElementNumber, BYTE p_ubBitNumber, BYTE* p_pubBitState, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_GetProcessOutputBit)
    {
        //Set Parameter Data
        m_pCommand_GetProcessOutputBit->ResetStatus();
        m_pCommand_GetProcessOutputBit->SetParameterData(0, &p_usProcessOutputType, sizeof(p_usProcessOutputType));
        m_pCommand_GetProcessOutputBit->SetParameterData(1, &p_ubElementNumber, sizeof(p_ubElementNumber));
        m_pCommand_GetProcessOutputBit->SetParameterData(2, &p_ubBitNumber, sizeof(p_ubBitNumber));

        //Execute Command
        oResult = m_pCommand_GetProcessOutputBit->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetProcessOutputBit->GetReturnParameterData(0, p_pubBitState, sizeof(*p_pubBitState));

        //Get ErrorCode
        m_pCommand_GetProcessOutputBit->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}


void CCommandSetProcessInputOutputAccess_VCS_Plc2::InitCommands()
{
    DeleteCommands();

    //Init GetProcessOutput
    m_pCommand_GetProcessOutput = new CCommand_VCS_Plc2();
    m_pCommand_GetProcessOutput->InitCommand(PLC2_GET_PROCESS_OUTPUT);

    //Init SetProcessInput
    m_pCommand_SetProcessInput = new CCommand_VCS_Plc2();
    m_pCommand_SetProcessInput->InitCommand(PLC2_SET_PROCESS_INPUT);

	//Init GetProcessOutputBit
    m_pCommand_GetProcessOutputBit = new CCommand_VCS_Plc2();
    m_pCommand_GetProcessOutputBit->InitCommand(PLC2_GET_PROCESS_OUTPUT_BIT);

    //Init SetProcessInputBit
    m_pCommand_SetProcessInputBit = new CCommand_VCS_Plc2();
    m_pCommand_SetProcessInputBit->InitCommand(PLC2_SET_PROCESS_INPUT_BIT);
}

void CCommandSetProcessInputOutputAccess_VCS_Plc2::DeleteCommands()
{
    if(m_pCommand_GetProcessOutput)
    {
        delete m_pCommand_GetProcessOutput;
        m_pCommand_GetProcessOutput = NULL;
    }

    if(m_pCommand_SetProcessInput)
    {
        delete m_pCommand_SetProcessInput;
        m_pCommand_SetProcessInput = NULL;
    }

	if(m_pCommand_GetProcessOutputBit)
    {
        delete m_pCommand_GetProcessOutputBit;
        m_pCommand_GetProcessOutputBit = NULL;
    }

    if(m_pCommand_SetProcessInputBit)
    {
        delete m_pCommand_SetProcessInputBit;
        m_pCommand_SetProcessInputBit = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetProcessInputOutputAccess_VCS_Plc2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_SetProcessInput && !m_pCommand_SetProcessInput->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetProcessOutput && !m_pCommand_GetProcessOutput->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
		if(m_pCommand_SetProcessInputBit && !m_pCommand_SetProcessInputBit->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetProcessOutputBit && !m_pCommand_GetProcessOutputBit->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}


void CCommandSetProcessInputOutputAccess_VCS_Plc2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetProcessInput) m_pCommand_SetProcessInput->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetProcessOutput) m_pCommand_GetProcessOutput->InitJournalManager(p_pJournalManager);
	if(m_pCommand_SetProcessInputBit) m_pCommand_SetProcessInputBit->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetProcessOutputBit) m_pCommand_GetProcessOutputBit->InitJournalManager(p_pJournalManager);
}

void CCommandSetProcessInputOutputAccess_VCS_Plc2::ResetJournalManager()
{
    if(m_pCommand_SetProcessInput) m_pCommand_SetProcessInput->ResetJournalManager();
    if(m_pCommand_GetProcessOutput) m_pCommand_GetProcessOutput->ResetJournalManager();
	if(m_pCommand_SetProcessInputBit) m_pCommand_SetProcessInputBit->ResetJournalManager();
    if(m_pCommand_GetProcessOutputBit) m_pCommand_GetProcessOutputBit->ResetJournalManager();
}

BOOL CCommandSetProcessInputOutputAccess_VCS_Plc2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SetProcessInput && !m_pCommand_SetProcessInput->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetProcessOutput && !m_pCommand_GetProcessOutput->InitGateway(p_pGateway)) return FALSE;
	if(m_pCommand_SetProcessInputBit && !m_pCommand_SetProcessInputBit->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetProcessOutputBit && !m_pCommand_GetProcessOutputBit->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

