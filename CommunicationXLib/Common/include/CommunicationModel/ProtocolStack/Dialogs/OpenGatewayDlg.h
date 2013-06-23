#pragma once
#ifdef WINVER
#include "afxwin.h"
#endif
// COpenGatewayDlg dialog

class CGatewaySettings;
#ifdef WINVER
typedef std::list<CGatewaySettings*> tGatewaySettingsList;

class COpenGatewayDlg : public CDialog
{
    DECLARE_DYNAMIC(COpenGatewayDlg)

public:
    void GetParameter(CStdString *p_pGatewayName, CStdString *p_pGatewayProtocolStackName, BYTE *p_pubNodeId, WORD *p_pusRemoteNetworkId);
    void SetParameter(tGatewaySettingsList* p_pGatewaySettingsList);

    COpenGatewayDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~COpenGatewayDlg();

// Dialog Data
    CComboBox m_ComboGatewayName;
    CComboBox m_ComboGatewayProtocolStackName;
    CComboBox m_ComboNodeId;
    CComboBox m_ComboRemoteNetworkId;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelendokComboGatewayName();
    afx_msg void OnCbnSelendokComboGatewayProtocolStackName();
    afx_msg void OnCbnSelendokCombonodeid();
    afx_msg void OnCbnSelendokComboremotenetworkid();
    virtual BOOL DestroyWindow();
    virtual BOOL OnInitDialog();

protected:
    virtual void OnCancel();

private:
    BOOL InitControl();
    BOOL InitControl_GatewayName(CStdString* p_pGatewayNameSel);
    BOOL InitControl_GatewayProtocolStackName(CStdString* p_pGatewayProtocolStackNameSel);
    BOOL InitControl_NodeId();
    BOOL InitControl_RemoteNetworkId();

    BOOL UpdateControl_GatewayName();
    BOOL UpdateControl_GatewayProtocolStackName();
    BOOL UpdateControl_NodeId();
    BOOL UpdateControl_RemoteNetworkId();

    CStdString GetNoGatewayString();
    CStdString GetNoGatewayProtocolStackString();

    void CopyGatewaySettingsList(tGatewaySettingsList* p_pSourceList);
    void DeleteGatewaySettingsList();

    BYTE m_ubNodeId;
    CStdString m_GatewayName;
    CStdString m_GatewayProtocolStackName;
    tGatewaySettingsList m_GatewaySettingsList;
    WORD m_usRemoteNetworkId;
};
#endif
