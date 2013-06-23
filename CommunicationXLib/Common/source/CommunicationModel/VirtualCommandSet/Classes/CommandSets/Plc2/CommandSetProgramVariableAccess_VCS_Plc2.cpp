// CommandSetProgramVariableAccess_VCS_Plc2.cpp: Implementierung der Klasse CCommandSetProgramVariableAccess_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetProgramVariableAccess_VCS_Plc2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommandSetProgramVariableAccess_VCS_Plc2::CCommandSetProgramVariableAccess_VCS_Plc2()
{
    m_strCommandSetName = COMMAND_SET_PROGRAM_VARIABLE_ACCESS;

    m_pCommand_GetVariable = NULL;
    m_pCommand_SetVariable = NULL;

    InitCommands();
}

CCommandSetProgramVariableAccess_VCS_Plc2::~CCommandSetProgramVariableAccess_VCS_Plc2()
{
    DeleteCommands();
}

BOOL CCommandSetProgramVariableAccess_VCS_Plc2::VCS_SetVariable(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CStdString p_VariableName, void* p_pDataBuffer, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_SetVariable)
    {
        //Set Parameter Data
        m_pCommand_SetVariable->ResetStatus();
        m_pCommand_SetVariable->SetParameterData(0, &p_VariableName, sizeof(p_VariableName));
        m_pCommand_SetVariable->SetParameterData(1, &p_ulNbOfBytesToWrite, sizeof(p_ulNbOfBytesToWrite));
        m_pCommand_SetVariable->SetParameterData(2, p_pDataBuffer, p_ulNbOfBytesToWrite);

        //Execute Command
        oResult = m_pCommand_SetVariable->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SetVariable->GetReturnParameterData(0, p_pulNbOfBytesWritten, sizeof(*p_pulNbOfBytesWritten));

        //Get ErrorCode
        m_pCommand_SetVariable->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetProgramVariableAccess_VCS_Plc2::VCS_GetVariable(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CStdString p_VariableName, void* p_pDataBuffer, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_GetVariable)
    {
        //Set Parameter Data
        m_pCommand_GetVariable->ResetStatus();
        m_pCommand_GetVariable->SetParameterData(0, &p_VariableName, sizeof(p_VariableName));
        m_pCommand_GetVariable->SetParameterData(1, &p_ulNbOfBytesToRead, sizeof(p_ulNbOfBytesToRead));

        //Execute Command
        oResult = m_pCommand_GetVariable->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetVariable->GetReturnParameterData(0, p_pulNbOfBytesRead, sizeof(*p_pulNbOfBytesRead));
        m_pCommand_GetVariable->GetReturnParameterData(1, p_pDataBuffer, p_ulNbOfBytesToRead);

        //Get ErrorCode
        m_pCommand_GetVariable->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetProgramVariableAccess_VCS_Plc2::InitCommands()
{
    DeleteCommands();

    //Init GetVariable
    m_pCommand_GetVariable = new CCommand_VCS_Plc2();
    m_pCommand_GetVariable->InitCommand(PLC2_GET_BOOTUP_BEHAVIOUR);

    //Init SetVariable
    m_pCommand_SetVariable = new CCommand_VCS_Plc2();
    m_pCommand_SetVariable->InitCommand(PLC2_SET_BOOTUP_BEHAVIOUR);
}

void CCommandSetProgramVariableAccess_VCS_Plc2::DeleteCommands()
{
    if(m_pCommand_GetVariable)
    {
        delete m_pCommand_GetVariable;
        m_pCommand_GetVariable = NULL;
    }

    if(m_pCommand_SetVariable)
    {
        delete m_pCommand_SetVariable;
        m_pCommand_SetVariable = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetProgramVariableAccess_VCS_Plc2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_SetVariable && !m_pCommand_SetVariable->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetVariable && !m_pCommand_GetVariable->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetProgramVariableAccess_VCS_Plc2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetVariable) m_pCommand_SetVariable->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetVariable) m_pCommand_GetVariable->InitJournalManager(p_pJournalManager);
}

void CCommandSetProgramVariableAccess_VCS_Plc2::ResetJournalManager()
{
    if(m_pCommand_SetVariable) m_pCommand_SetVariable->ResetJournalManager();
    if(m_pCommand_GetVariable) m_pCommand_GetVariable->ResetJournalManager();
}

BOOL CCommandSetProgramVariableAccess_VCS_Plc2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SetVariable && !m_pCommand_SetVariable->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetVariable && !m_pCommand_GetVariable->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

