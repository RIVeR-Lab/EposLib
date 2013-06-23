#pragma once
#ifdef WINVER
#include "afxwin.h"

class CProtocolStackSettings;

class COpenPlcDlg : public CDialog
{
    DECLARE_DYNAMIC(COpenPlcDlg)

public:
    void GetParameter(CStdString *p_pStrPlcName, BYTE *p_pubNodeId);
    void SetParameter(CStdStringArray *p_pstrPlcNameArray);

    COpenPlcDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~COpenPlcDlg();

// Dialog Data
    CComboBox m_ComboPlcName;
    CComboBox m_ComboNodeId;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelendokComboplcname();
    afx_msg void OnCbnSelendokCombonodeid();
    virtual BOOL DestroyWindow();
    virtual BOOL OnInitDialog();

protected:
    virtual void OnCancel();

private:
    BOOL InitControl();
    BOOL InitControl_PlcName(CStdString* p_pStrPlcNameSel);
    BOOL UpdateControl_PlcName();

    CStdString GetNoPlcString();

    BOOL InitControl_NodeId();
    BOOL UpdateControl_NodeId();

    BYTE m_ubNodeId;
    CStdStringArray m_strPlcNameArray;
    CStdString m_strPlcName;
};
#endif
