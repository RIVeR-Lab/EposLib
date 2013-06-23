// CommandSetInitialisation_VCS_Plc2.cpp: Implementierung der Klasse CCommandSetInitialisation_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetInitialisation_VCS_Plc2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetInitialisation_VCS_Plc2::CCommandSetInitialisation_VCS_Plc2()
{
    m_strCommandSetName = COMMAND_SET_INITIALISATION;

    m_pCommand_GetBootupBehavior = NULL;
    m_pCommand_SetBootupBehavior = NULL;

    InitCommands();
}

CCommandSetInitialisation_VCS_Plc2::~CCommandSetInitialisation_VCS_Plc2()
{
    DeleteCommands();
}

BOOL CCommandSetInitialisation_VCS_Plc2::VCS_SetBootupBehavior(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProgramControl, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_SetBootupBehavior)
    {
        //Set Parameter Data
        m_pCommand_SetBootupBehavior->ResetStatus();
        m_pCommand_SetBootupBehavior->SetParameterData(0, &p_usProgramControl, sizeof(p_usProgramControl));

        //Execute Command
        oResult = m_pCommand_SetBootupBehavior->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetBootupBehavior->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInitialisation_VCS_Plc2::VCS_GetBootupBehavior(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusProgramControl, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_GetBootupBehavior)
    {
        //Execute Command
        oResult = m_pCommand_GetBootupBehavior->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetBootupBehavior->GetReturnParameterData(0, p_pusProgramControl, sizeof(*p_pusProgramControl));

        //Get ErrorCode
        m_pCommand_GetBootupBehavior->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetInitialisation_VCS_Plc2::InitCommands()
{
    DeleteCommands();

    //Init GetBootupBehavior
    m_pCommand_GetBootupBehavior = new CCommand_VCS_Plc2();
    m_pCommand_GetBootupBehavior->InitCommand(PLC2_GET_BOOTUP_BEHAVIOUR);

    //Init SetBootupBehavior
    m_pCommand_SetBootupBehavior = new CCommand_VCS_Plc2();
    m_pCommand_SetBootupBehavior->InitCommand(PLC2_SET_BOOTUP_BEHAVIOUR);
}

void CCommandSetInitialisation_VCS_Plc2::DeleteCommands()
{
    if(m_pCommand_GetBootupBehavior)
    {
        delete m_pCommand_GetBootupBehavior;
        m_pCommand_GetBootupBehavior = NULL;
    }

    if(m_pCommand_SetBootupBehavior)
    {
        delete m_pCommand_SetBootupBehavior;
        m_pCommand_SetBootupBehavior = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetInitialisation_VCS_Plc2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_SetBootupBehavior && !m_pCommand_SetBootupBehavior->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetBootupBehavior && !m_pCommand_GetBootupBehavior->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetInitialisation_VCS_Plc2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetBootupBehavior) m_pCommand_SetBootupBehavior->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetBootupBehavior) m_pCommand_GetBootupBehavior->InitJournalManager(p_pJournalManager);
}

void CCommandSetInitialisation_VCS_Plc2::ResetJournalManager()
{
    if(m_pCommand_SetBootupBehavior) m_pCommand_SetBootupBehavior->ResetJournalManager();
    if(m_pCommand_GetBootupBehavior) m_pCommand_GetBootupBehavior->ResetJournalManager();
}

BOOL CCommandSetInitialisation_VCS_Plc2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SetBootupBehavior && !m_pCommand_SetBootupBehavior->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetBootupBehavior && !m_pCommand_GetBootupBehavior->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

