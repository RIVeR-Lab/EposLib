// EPOS_WinDLL.cpp : Legt die Initialisierungsroutinen f�r die DLL fest.
//
#include "CommunicationModel/CommunicationModel.h"
#include <CommunicationModel/Common/Journal/Time/JournalTime.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>

#include "Definitions.h"
#include "EposCmd.h"

CEPOSLinuxSharedLibApp *m_pDllInstance = 0;

//////////////////////////////////////////////////////////////////////
// CEPOS_WinDLLApp Konstruktion

CEPOSLinuxSharedLibApp::CEPOSLinuxSharedLibApp()
{
    m_pCommunicationModel = NULL;
}

//////////////////////////////////////////////////////////////////////
// Das einzige CEPOS_WinDLLApp-Objekt

CEPOSLinuxSharedLibApp::~CEPOSLinuxSharedLibApp()
{
    DWORD ulErrorCode;

    VCS_CloseAllDevices(&ulErrorCode);

	DeleteCommunicationModel();
}

/*CJournalManagerBase* CEPOSLinuxSharedLibApp::GetJournalManager()
{
	return &m_JournalManager;
}*/

CCommunicationModel *CEPOSLinuxSharedLibApp::CreateCommunicationModel()
{
    if(!m_pCommunicationModel)
	{
        m_pCommunicationModel = new CCommunicationModel();
	}

    return m_pCommunicationModel;
}

BOOL CEPOSLinuxSharedLibApp::DeleteCommunicationModel()
{
    if(m_pCommunicationModel)
	{
        delete m_pCommunicationModel;
        m_pCommunicationModel = NULL;
	}

    return TRUE;
}

HANDLE StartTransaction(HANDLE KeyHandle,HANDLE JournalHandle)
{
	HANDLE hTransactionHandle = NULL;
	CJournalManagerBase* pJournalManager = NULL;

	/*pJournalManager = m_pDllInstance->GetJournalManager();
	if(pJournalManager)
	{
		hTransactionHandle = pJournalManager->StartTransaction(KeyHandle,JournalHandle);
	}*/

	return hTransactionHandle;
}

void StopTransaction(HANDLE hTransactionHandle)
{
	/*CJournalManagerBase* pJournalManager = NULL;

	pJournalManager = m_pDllInstance->GetJournalManager();
	if(pJournalManager)
	{
		pJournalManager->StopTransaction(hTransactionHandle);
	}*/
}

void SetNodeId(CCommandRoot* pCommand,WORD wNodeId)
{
	CStdString name;
	int iNbOfParameter;

	if(pCommand)
	{
		iNbOfParameter = pCommand->GetNbOfParameter();
		for(int i=0;i<iNbOfParameter;i++)
		{
			name = pCommand->GetParameterName(i);
			if(name.CompareNoCase("NodeId") == 0)
			{
				pCommand->SetParameterData(i,&wNodeId,sizeof(wNodeId));
				break;
			}
		}
	}
}
