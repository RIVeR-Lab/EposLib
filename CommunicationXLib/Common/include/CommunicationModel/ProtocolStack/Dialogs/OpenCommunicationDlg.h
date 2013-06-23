#pragma once
#ifdef WINVER
#include "afxwin.h"
#endif
// COpenCommunicationDlg dialog

class CDeviceSettings;
class CProtocolStackSettings;

#include <list>

#ifdef WINVER
class COpenCommunicationDlg : public CDialog
{
    DECLARE_DYNAMIC(COpenCommunicationDlg)

public:
    void GetParameter(CStdString *p_pStrProtocolStackName, CStdString *p_pStrInterfaceName, CStdString *p_pStrPortName, DWORD_PTR* p_pulBaudrate, DWORD_PTR* p_pulTimeout);
    void SetParameter(tDeviceSettingsList* p_pDeviceSettingsList, CStdStringArray *p_pstrProtocolStackNameArray, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, DWORD_PTR p_ulBaudrate, DWORD_PTR p_ulTimeout);

    COpenCommunicationDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~COpenCommunicationDlg();

// Dialog Data
    CComboBox m_ComboProtocolStackName;
    CComboBox m_ComboInterfaceName;
    CComboBox m_ComboPortName;
    CComboBox m_ComboBaudrate;
    DWORD_PTR m_EditTimeout;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnKillfocusTimeout();
    afx_msg void OnSelendokComboBaudrate();
    afx_msg void OnSelendokComboDeviceName();
    afx_msg void OnSelendokComboInterfaceName();
    afx_msg void OnSelendokComboPortName();
    afx_msg void OnSelendokComboProtocolStackName();
    virtual BOOL DestroyWindow();
    virtual BOOL OnInitDialog();

protected:
    virtual void OnCancel();

private:
    BOOL InitControl();
    BOOL InitControl_ProtocolStackName(CStdString* p_pStrProtocolStackNameSel);
    BOOL InitControl_InterfaceName(CStdString p_strProtocolStackName, CStdString* p_pStrInterfaceNameSel);
    BOOL InitControl_PortName(CStdString p_strProtocolStackName, CStdString strInterfaceName, CStdString* p_pStrPortNameSel);
    BOOL InitControl_BaudrateTimeout(CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, DWORD_PTR* p_pulBaudrateSel, DWORD_PTR* p_pulTimeout);

    BOOL UpdateControl_ProtocolStackName();
    BOOL UpdateControl_InterfaceName();
    BOOL UpdateControl_PortName();
    BOOL UpdateControl_Baudrate();
    BOOL UpdateControl_Timeout();

    CStdString GetNoProtocolStackString();
    CStdString GetNoInterfaceString();
    CStdString GetNoPortString();
    CStdString GetNoBaudrateString();

    BOOL FindDeviceSettings(CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CDeviceSettings** p_ppDeviceSettings);

    void DeleteDeviceSettingsList();
    void CopyDeviceSettingsList(tDeviceSettingsList* p_pSourceList);

    CStdStringArray m_strProtocolStackNameArray;
    CStdString m_strInterfaceName;
    CStdString m_strPortName;
    CStdString m_strProtocolStackName;
    DWORD_PTR m_ulBaudrate;
    DWORD_PTR m_ulTimeout;
    tDeviceSettingsList m_DeviceSettingsList;
};
#endif //WINVER