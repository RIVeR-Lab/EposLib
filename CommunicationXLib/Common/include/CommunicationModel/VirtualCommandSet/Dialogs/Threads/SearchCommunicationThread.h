// SearchCommunicationThread.h: Schnittstelle f�r die Klasse CSearchCommunicationThread.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHCOMMUNICATIONTHREAD_H__27C6E0F2_14F2_11D4_9E9C_00A02436C4EF__INCLUDED_)
#define AFX_SEARCHCOMMUNICATIONTHREAD_H__27C6E0F2_14F2_11D4_9E9C_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Thread/MmcThread.h>
#include "../../../Common/Classes/CommunicationSetting.h"
#include "../../../CommonLayer/Classes/Commands/Command/CommandRoot.h"

#define WM_NextCommunicationSetting WM_USER+1
#define WM_EndCommunicationSearch WM_USER+2

class CSearchCommunicationDlg;
class CVirtualCommandSet_ManagerBase;

class CSearchCommunicationThread : public CMmcThread
{
public:
	void SetStartHandle(HANDLE hHandle);
	void Init(CVirtualCommandSet_ManagerBase* pManager,CCommandRoot* pCommand);
	void InitSearchCommunicationDlg(CSearchCommunicationDlg *pSearchCommunicationDlg);
	void InitCommunicationSettingList(std::list<CCommunicationSetting*>* pList);

	CSearchCommunicationThread();
	virtual ~CSearchCommunicationThread();

	unsigned int WINAPI ThreadFunc();

private:
	void DeleteTestCommand();
	void DeleteStartCommunicationSetting();

	BOOL StoreCommunicationSetting(HANDLE hHandle);
	BOOL OpenCommunicationSetting(CCommunicationSetting* pSetting,CCommunicationSetting* pLastSetting,HANDLE* phHandle);
	BOOL IsDeviceChangeNecessary(CCommunicationSetting* pSetting,CCommunicationSetting* pLastSetting);
	BOOL RestoreCommunicationSetting(HANDLE* phHandle);

private:
	CVirtualCommandSet_ManagerBase* m_pVirtualCommandSetManager;	
	CCommandRoot* m_pTestCommand;
	std::list<CCommunicationSetting*>* m_pCommunicationSettingList;
	CSearchCommunicationDlg* m_pSearchCommunicationDlg;

	HANDLE m_hHandle;
	CCommunicationSetting* m_pStartCommunicationSetting;

};

#endif // !defined(AFX_SEARCHCOMMUNICATIONTHREAD_H__27C6E0F2_14F2_11D4_9E9C_00A02436C4EF__INCLUDED_)
