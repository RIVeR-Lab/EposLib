#if !defined(AFX_SEARCHCOMMUNICATIONWHOLECANIDRANGEDLG_H__C4067AAA_3721_4CAC_A8B1_587EE6EEF63D__INCLUDED_)
#define AFX_SEARCHCOMMUNICATIONWHOLECANIDRANGEDLG_H__C4067AAA_3721_4CAC_A8B1_587EE6EEF63D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchCommunicationWholeCanIdRangeDlg.h : Header-Datei
//
#ifdef WINVER
#include <CommunicationModel/VirtualCommandSet/Dialogs/SearchCommunicationDefinitions.h>

class CVirtualCommandSet_ManagerBase;
class CCommunicationSetting;
class CSearchCommunicationWholeCanIdRangeThread;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSearchCommunicationWholeCanIdRangeDlg

class CSearchCommunicationWholeCanIdRangeDlg : public CDialog
{
// Konstruktion
public:
    CSearchCommunicationWholeCanIdRangeDlg(CWnd* pParent = NULL);   // Standardkonstruktor

    void Init(CVirtualCommandSet_ManagerBase* p_pManager, HANDLE p_hHandle, CCommandRoot* p_pCommand, CStdString p_strSearchingInfo, EDialogMode p_eDialogMode);
	void InitRootCommunicationSetting(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName);
    void InitStartCommunicationSetting(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, DWORD p_ulBaudrate, DWORD p_ulTimeout);

    BOOL SearchSucceeded();
    BOOL GetHandle(HANDLE* p_phHandle);
    BOOL GetNodeId(WORD* p_pusNodeId);
    BOOL GetCommunicationSetting(CStdString* p_StrVirtualDeviceName, CStdString* p_pStrDeviceName, CStdString* p_pStrProtocolStackName, CStdString* p_pStrInterfaceName, CStdString* p_pStrPortName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout);
    void SetCommunicationSetting(CCommunicationSetting* p_pSetting, HANDLE p_hHandle);

// Dialogfelddaten
    //{{AFX_DATA(CSearchCommunicationWholeCanIdRangeDlg)
    CProgressCtrl m_SearchProgress;
    //}}AFX_DATA

// �berschreibungen
    // Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
    //{{AFX_VIRTUAL(CSearchCommunicationWholeCanIdRangeDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
    //}}AFX_VIRTUAL

// Implementierung
protected:
    LRESULT OnNextCommunicationSetting(WPARAM p_ulParam, LPARAM p_lParam);
    LRESULT OnEndCommunicationSearch(WPARAM p_ulParam, LPARAM p_lParam);
    LRESULT OnUpdateProgress(WPARAM p_ulParam, LPARAM p_lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
    // Generierte Nachrichtenzuordnungsfunktionen
    //{{AFX_MSG(CSearchCommunicationWholeCanIdRangeDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void InitSearchingInfo(CStdString p_SearchingInfoString);
    void InitStartHandle(HANDLE p_hHandle);

    void CreateSearchCommunicationWholeCanIdRangeThread();
    void DeleteSearchCommunicationWholeCanIdRangeThread();

    void DeleteCommunicationSetting();
    void DeleteRootCommunicationSetting();
    void DeleteStartCommunicationSetting();

    void InitCommunicationSettingList();
    void InitProgressRange();
    void AddCommunicationSettings();
	void AddCommunicationSettings(CStdString p_strVirtualDeviceName);
    void AddCommunicationSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName);
    void AddCommunicationSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName);
    void AddCommunicationSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName);
    void AddCommunicationSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName);
	BOOL IsCommunicationSettingFiltered(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, DWORD p_ulBaudrate, DWORD p_ulTimeout);
	
    void DeleteCommunicationSettingList();

	CStdStringArray::iterator FindStartIndex(CStdStringArray* p_pNameArray, CStdString p_strName);
    CStdDWordArray::iterator FindStartIndex(CStdDWordArray* p_pValueArray, DWORD p_ulValue);
    CStdDWordArray::iterator FindMaxValueIndex(CStdDWordArray* p_pValueArray);

    void SortNameArray(CStdStringArray* p_pNameArray, CStdString p_strName);
    void SortValueArray(CStdDWordArray* p_pValueArray, DWORD p_ulValue);

    void UpdateDialogSize();
    void UpdateSearchSettingStrings(int p_lSettingIndex);
    void UpdateSearchSettingLineStrings(int p_lLineIndex, CStdString p_strTitle, CStdString p_strValue);
    void ShowMessage();
    CStdString GetMessageString();

	BOOL IsConfirmDlgEnabled();
	BOOL IsProgressDlgEnabled();

private:
    CVirtualCommandSet_ManagerBase* m_pVirtualCommandSetManager;

    CSearchCommunicationWholeCanIdRangeThread* m_pSearchCommunicationWholeCanIdRangeThread;

    CCommunicationSetting* m_pRootCommunicationSetting;
    BOOL m_oIsDeviceNameVisible;
    BOOL m_oIsProtocolStackNameVisible;
    BOOL m_oIsInterfaceNameVisible;
    BOOL m_oIsPortNameVisible;

    CStdString m_strSearchingInfoString;
	std::list<CCommunicationSetting*> m_CommunicationSettingList;
    CCommunicationSetting* m_pStartCommunicationSetting;

    BOOL m_oCommunicationFound;
    HANDLE m_hHandle;
    CCommunicationSetting* m_pCommunicationSetting;
    WORD m_usNodeId;

    EDialogMode m_eDialogMode;
};
#endif //WINVER

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_SEARCHCOMMUNICATIONWHOLECANIDRANGEDLG_H__C4067AAA_3721_4CAC_A8B1_587EE6EEF63D__INCLUDED_
