// SearchCommunicationWholeCanIdRangeThread.h: Schnittstelle f�r die Klasse CSearchCommunicationWholeCanIdRangeThread.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHCOMMUNICATIONWHOLECANIDRANGETHREAD_H__27C6E0F2_14F2_11D4_9E9C_00A02436C4EF__INCLUDED_)
#define AFX_SEARCHCOMMUNICATIONWHOLECANIDRANGETHREAD_H__27C6E0F2_14F2_11D4_9E9C_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Thread/MmcThread.h>
#include "../SearchCommunicationWholeCanIdRangeDlg.h"

#define WM_NextCommunicationSetting WM_USER+1
#define WM_EndCommunicationSearch WM_USER+2
#define WM_UpdateProgress WM_USER+3

class CCommunicationSetting;
class CVirtualCommandSet_ManagerBase;
class CCommandRoot;

class CSearchCommunicationWholeCanIdRangeThread : public CMmcThread
{
public:
    void SetStartHandle(HANDLE p_hHandle);
    void Init(CVirtualCommandSet_ManagerBase* p_pManager, CCommandRoot* p_pCommand);
#ifdef WINVER
    void InitSearchCommunicationWholeCanIdRangeDlg(CSearchCommunicationWholeCanIdRangeDlg *p_pSearchCommunicationWholeCanIdRangeDlg);
#endif
    void InitCommunicationSettingList(std::list<CCommunicationSetting*>* p_pList);

    CSearchCommunicationWholeCanIdRangeThread();
    virtual ~CSearchCommunicationWholeCanIdRangeThread();

    unsigned int WINAPI ThreadFunc();

private:
    void DeleteTestCommand();
    void DeleteStartCommunicationSetting();

    BOOL StoreCommunicationSetting(HANDLE hp_Handle);
    BOOL OpenCommunicationSetting(CCommunicationSetting* p_pSetting, CCommunicationSetting* p_pLastSetting, HANDLE* p_phHandle);
    BOOL IsDeviceChangeNecessary(CCommunicationSetting* p_pSetting, CCommunicationSetting* p_pLastSetting);
    BOOL RestoreCommunicationSetting(HANDLE* p_phHandle);

private:
    CVirtualCommandSet_ManagerBase* m_pVirtualCommandSetManager;
    CCommandRoot* m_pTestCommand;
    std::list<CCommunicationSetting*>* m_pCommunicationSettingList;
#ifdef WINVER
    CSearchCommunicationWholeCanIdRangeDlg* m_pSearchCommunicationWholeCanIdRangeDlg;
#endif
    HANDLE m_hHandle;
    CCommunicationSetting* m_pStartCommunicationSetting;
};

#endif // !defined(AFX_SEARCHCOMMUNICATIONWHOLECANIDRANGETHREAD_H__27C6E0F2_14F2_11D4_9E9C_00A02436C4EF__INCLUDED_)
