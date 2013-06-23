#if !defined(AFX_EPOS_WINDLL_H__01686F71_F7EB_4512_A1BA_2BC01FF5CDD1__INCLUDED_)
#define AFX_EPOS_WINDLL_H__01686F71_F7EB_4512_A1BA_2BC01FF5CDD1__INCLUDED_

//#include <CommunicationModel/Common/Journal/JournalManager.h>

class CCommandRoot;
class CCommunicationModel;

HANDLE StartTransaction(HANDLE KeyHandle,HANDLE JournalHandle=NULL);
void StopTransaction(HANDLE hTransactionHandle);
void SetNodeId(CCommandRoot* pCommand,WORD wNodeId);

/////////////////////////////////////////////////////////////////////////////
//
class CEPOSLinuxSharedLibApp
{
public:
	CEPOSLinuxSharedLibApp();
	~CEPOSLinuxSharedLibApp();

	//CJournalManagerBase* GetJournalManager();
    CCommunicationModel* CreateCommunicationModel();
	BOOL DeleteCommunicationModel();

private:
	//CJournalManager m_JournalManager;
    CCommunicationModel* m_pCommunicationModel;
};

extern CEPOSLinuxSharedLibApp* m_pDllInstance;

#endif // !defined(AFX_EPOS_WINDLL_H__01686F71_F7EB_4512_A1BA_2BC01FF5CDD1__INCLUDED_)
