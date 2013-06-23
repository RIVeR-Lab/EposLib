// CommandSetObjectDictionary_VCS_CanOpen.cpp: Implementierung der Klasse CCommandSetObjectDictionary_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include "CommandSetObjectDictionary_VCS_CanOpen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetObjectDictionary_VCS_CanOpen::CCommandSetObjectDictionary_VCS_CanOpen()
{
    m_strCommandSetName = COMMAND_SET_OBJECTDICTIONARY;

    m_pCommand_GetObject = NULL;
    m_pCommand_SetObject = NULL;
	m_pCommand_Store = NULL;
    m_pCommand_Restore = NULL;
      
    InitCommands();
}

CCommandSetObjectDictionary_VCS_CanOpen::~CCommandSetObjectDictionary_VCS_CanOpen()
{
    DeleteCommands();
}

BOOL CCommandSetObjectDictionary_VCS_CanOpen::VCS_GetObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void* p_pData, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetObject)
    {
        //Set Parameter Data
        m_pCommand_GetObject->ResetStatus();
        m_pCommand_GetObject->SetParameterData(0, &p_usObjectIndex, sizeof(p_usObjectIndex));
        m_pCommand_GetObject->SetParameterData(1, &p_ubObjectSubIndex, sizeof(p_ubObjectSubIndex));
        m_pCommand_GetObject->SetParameterData(2, &p_ulNbOfBytesToRead, sizeof(p_ulNbOfBytesToRead));

        //ExecuteCommand
        oResult = m_pCommand_GetObject->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetObject->GetReturnParameterData(0, p_pData, p_ulNbOfBytesToRead);
        m_pCommand_GetObject->GetReturnParameterData(1, p_pulNbOfBytesRead, sizeof(*p_pulNbOfBytesRead));

        //Get ErrorCode
        m_pCommand_GetObject->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_VCS_CanOpen::VCS_SetObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void* p_pData, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetObject)
    {
        //Set Parameter Data
        m_pCommand_SetObject->ResetStatus();
        m_pCommand_SetObject->SetParameterData(0, &p_usObjectIndex, sizeof(p_usObjectIndex));
        m_pCommand_SetObject->SetParameterData(1, &p_ubObjectSubIndex, sizeof(p_ubObjectSubIndex));
        m_pCommand_SetObject->SetParameterData(2, p_pData, p_ulNbOfBytesToWrite);
        m_pCommand_SetObject->SetParameterData(3, &p_ulNbOfBytesToWrite, sizeof(p_ulNbOfBytesToWrite));

        //ExecuteCommand
        oResult = m_pCommand_SetObject->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SetObject->GetReturnParameterData(0, p_pulNbOfBytesWritten, sizeof(*p_pulNbOfBytesWritten));

        //Get ErrorCode
        m_pCommand_SetObject->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_VCS_CanOpen::VCS_Restore(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_Restore)
    {
        //Set Parameter Data
        m_pCommand_Restore->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_Restore->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_Restore->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_VCS_CanOpen::VCS_Store(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_Store)
    {
        //Set Parameter Data
        m_pCommand_Store->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_Store->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_Store->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetObjectDictionary_VCS_CanOpen::InitCommands()
{
    DeleteCommands();

    //Init GetObject
    m_pCommand_GetObject = new CCommand_VCS_CanOpen();
    m_pCommand_GetObject->InitCommand(VCS_CANOPEN_GET_OBJECT);

    //Init SetObject
    m_pCommand_SetObject = new CCommand_VCS_CanOpen();
    m_pCommand_SetObject->InitCommand(VCS_CANOPEN_SET_OBJECT);

	//Init Store
    m_pCommand_Store = new CCommand_VCS_CanOpen();
    m_pCommand_Store->InitCommand(VCS_CANOPEN_STORE);
	
	//Init Restore
    m_pCommand_Restore = new CCommand_VCS_CanOpen();
    m_pCommand_Restore->InitCommand(VCS_CANOPEN_RESTORE);
}

void CCommandSetObjectDictionary_VCS_CanOpen::DeleteCommands()
{
    if(m_pCommand_GetObject)
    {
        delete m_pCommand_GetObject;
        m_pCommand_GetObject = NULL;
    }
    if(m_pCommand_SetObject)
    {
        delete m_pCommand_SetObject;
        m_pCommand_SetObject = NULL;
    }
    if(m_pCommand_Store)
    {
        delete m_pCommand_Store;
        m_pCommand_Store = NULL;
    }
	if(m_pCommand_Restore)
    {
        delete m_pCommand_Restore;
        m_pCommand_Restore = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetObjectDictionary_VCS_CanOpen::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetObject && !m_pCommand_GetObject->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetObject && !m_pCommand_SetObject->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
		if(m_pCommand_Store && !m_pCommand_Store->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_Restore && !m_pCommand_Restore->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetObjectDictionary_VCS_CanOpen::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetObject) m_pCommand_GetObject->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetObject) m_pCommand_SetObject->InitJournalManager(p_pJournalManager);
	if(m_pCommand_Restore) m_pCommand_Restore->InitJournalManager(p_pJournalManager);
    if(m_pCommand_Store) m_pCommand_Store->InitJournalManager(p_pJournalManager);
}

void CCommandSetObjectDictionary_VCS_CanOpen::ResetJournalManager()
{
    if(m_pCommand_GetObject) m_pCommand_GetObject->ResetJournalManager();
    if(m_pCommand_SetObject) m_pCommand_SetObject->ResetJournalManager();
    if(m_pCommand_Restore) m_pCommand_Restore->ResetJournalManager();
    if(m_pCommand_Store) m_pCommand_Store->ResetJournalManager();
}

BOOL CCommandSetObjectDictionary_VCS_CanOpen::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetObject && !m_pCommand_GetObject->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetObject && !m_pCommand_SetObject->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_Restore && !m_pCommand_Restore->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_Store && !m_pCommand_Store->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}
