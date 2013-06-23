#pragma once
#ifdef WINVER
#include "afxwin.h"

class CProtocolStackSettings;

class COpenDriveDlg : public CDialog
{
    DECLARE_DYNAMIC(COpenDriveDlg)

public:
    void GetParameter(CStdString *p_pStrDriveName, BYTE *p_pubNodeId);
    void SetParameter(CStdStringArray *p_pstrDriveNameArray);

    COpenDriveDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~COpenDriveDlg();

// Dialog Data
    CComboBox m_ComboDriveName;
    CComboBox m_ComboNodeId;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelendokCombodrivename();
    afx_msg void OnCbnSelendokCombonodeid();
    virtual BOOL DestroyWindow();
    virtual BOOL OnInitDialog();

protected:
    virtual void OnCancel();

private:
    BOOL InitControl();
    BOOL InitControl_DriveName(CStdString* p_pStrDriveNameSel);
    BOOL UpdateControl_DriveName();
    CStdString GetNoDriveString();

    BOOL InitControl_NodeId();
    BOOL UpdateControl_NodeId();

    BYTE m_ubNodeId;
    CStdStringArray m_strDriveNameArray;
    CStdString m_strDriveName;
};
#endif
