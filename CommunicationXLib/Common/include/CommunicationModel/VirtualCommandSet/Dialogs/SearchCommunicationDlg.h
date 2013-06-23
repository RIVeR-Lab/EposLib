#if !defined(AFX_SEARCHCOMMUNICATIONDLG_H__C4067AAA_3721_4CAC_A8B1_587EE6EEF63D__INCLUDED_)
#define AFX_SEARCHCOMMUNICATIONDLG_H__C4067AAA_3721_4CAC_A8B1_587EE6EEF63D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// searchcommunicationdlg.h : Header-Datei
//

#include <CommunicationModel/VirtualCommandSet/Dialogs/SearchCommunicationDefinitions.h>

class CCommandRoot;
class CCommunicationSetting;
class CSearchCommunicationThread;
class CVirtualCommandSet_ManagerBase;

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CSearchCommunicationDlg
#ifdef WINVER
class CSearchCommunicationDlg : public CDialog
{
// Konstruktion
public:
	CSearchCommunicationDlg(CWnd* pParent = NULL);   // Standardkonstruktor

	void Init(CVirtualCommandSet_ManagerBase* pManager,HANDLE hHandle,CCommandRoot* pCommand,CStdString strSearchingInfo,EDialogMode p_eDialogMode);
	void InitRootCommunicationSetting(CStdString strVirtualDeviceName, CStdString strDeviceName,CStdString strProtocolStackName,CStdString strInterfaceName,CStdString strPortName);
	void InitStartCommunicationSetting(CStdString strVirtualDeviceName, CStdString strDeviceName,CStdString strProtocolStackName,CStdString strInterfaceName,CStdString strPortName,DWORD dBaudrate,DWORD dTimeout);

	BOOL SearchSucceeded();
	BOOL GetHandle(HANDLE* phHandle);
	BOOL GetCommunicationSetting(CStdString* pStrVirtualDeviceName,CStdString* pStrDeviceName,CStdString* pStrProtocolStackName,CStdString* pStrInterfaceName,CStdString* pStrPortName,DWORD* pdBaudrate,DWORD* pdTimeout);
	void SetCommunicationSetting(CCommunicationSetting* pSetting, HANDLE hHandle);

// Dialogfelddaten
	//{{AFX_DATA(CSearchCommunicationDlg)
	CProgressCtrl m_SearchProgress;
	//}}AFX_DATA

// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CSearchCommunicationDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	LRESULT OnNextCommunicationSetting(WPARAM wParam,LPARAM lParam);
	LRESULT OnEndCommunicationSearch(WPARAM wParam,LPARAM lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CSearchCommunicationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitSearchingInfo(CStdString searchingInfoString);
	void InitStartHandle(HANDLE hHandle);

	void CreateSearchCommunicationThread();
	void DeleteSearchCommunicationThread();

	void DeleteCommunicationSetting();
	void DeleteRootCommunicationSetting();
	void DeleteStartCommunicationSetting();

	void InitCommunicationSettingList();
	void AddCommunicationSettings();
	void AddCommunicationSettings(CStdString strVirtualDeviceName);
	void AddCommunicationSettings(CStdString strVirtualDeviceName, CStdString strDeviceName);
	void AddCommunicationSettings(CStdString strVirtualDeviceName, CStdString strDeviceName,CStdString strProtocolStackName);
	void AddCommunicationSettings(CStdString strVirtualDeviceName, CStdString strDeviceName,CStdString strProtocolStackName,CStdString strInterfaceName);
	void AddCommunicationSettings(CStdString strVirtualDeviceName, CStdString strDeviceName,CStdString strProtocolStackName,CStdString strInterfaceName,CStdString strPortName);
	BOOL IsCommunicationSettingFiltered(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, DWORD p_ulBaudrate, DWORD p_ulTimeout);
	void DeleteCommunicationSettingList();

	CStdStringArray::iterator FindStartIndex(CStdStringArray* pNameArray,CStdString name);
	CStdDWordArray::iterator FindStartIndex(CStdDWordArray* pValueArray,DWORD value);
	CStdDWordArray::iterator FindMaxValueIndex(CStdDWordArray* pValueArray);

	void SortNameArray(CStdStringArray* pNameArray,CStdString name);
	void SortValueArray(CStdDWordArray* pValueArray,DWORD value);

	void UpdateDialogSize();
	void UpdateSearchSettingStrings(int iSettingIndex);
	void UpdateSearchSettingLineStrings(int iLineIndex,CStdString strTitle,CStdString strValue);
	void ShowMessage();
	CStdString GetMessageString();
	BOOL IsConfirmDlgEnabled();
	BOOL IsProgressDlgEnabled();

private:
	CVirtualCommandSet_ManagerBase* m_pVirtualCommandSetManager;
	
	CSearchCommunicationThread* m_pSearchCommunicationThread;

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

	EDialogMode m_eDialogMode;
};
#endif WINVER
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_SEARCHCOMMUNICATIONDLG_H__C4067AAA_3721_4CAC_A8B1_587EE6EEF63D__INCLUDED_
