#if !defined(AFX_OPENDEVICEDLG_H__27DA5F7F_CF26_4880_B8A7_67E4F48B1DCA__INCLUDED_)
#define AFX_OPENDEVICEDLG_H__27DA5F7F_CF26_4880_B8A7_67E4F48B1DCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// opendevicedlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld COpenDeviceDlg

class CDeviceSettings;
class CProtocolStackSettings;

#include <list>
#ifdef WINVER

#include <CommunicationModel/VirtualCommandSet/Common/DeviceSettings.h>

class COpenDeviceDlg : public CDialog
{
// Konstruktion
public:
    void GetParameter(CStdString *p_pStrVirtualDeviceName, CStdString *p_pStrDeviceName, CStdString *p_pStrProtocolStackName, CStdString *p_pStrInterfaceName, CStdString *p_pStrPortName, DWORD_PTR* p_pulBaudrate, DWORD_PTR* p_pulTimeout);
    void SetParameter(tDeviceSettingsList* p_pDeviceSettingsList, CStdStringArray *p_pstrVirtualDeviceNameArray, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, DWORD_PTR p_ulBaudrate, DWORD_PTR p_ulTimeout);

    COpenDeviceDlg(CWnd* p_pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
    //{{AFX_DATA(COpenDeviceDlg)
    CComboBox    m_ComboBaudrate;
    CComboBox    m_ComboInterfaceName;
    CComboBox    m_ComboProtocolStackName;
    CComboBox    m_ComboPortName;
    CComboBox    m_ComboDeviceName;
    DWORD_PTR    m_EditTimeout;
    //}}AFX_DATA

// �berschreibungen
    // Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
    //{{AFX_VIRTUAL(COpenDeviceDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
    //}}AFX_VIRTUAL

// Implementierung
protected:

    // Generierte Nachrichtenzuordnungsfunktionen
    //{{AFX_MSG(COpenDeviceDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelendokComboDeviceName();
    afx_msg void OnSelendokComboPortName();
    afx_msg void OnSelendokComboProtocolStackName();
    afx_msg void OnSelendokComboInterfaceName();
    virtual void OnCancel();
    afx_msg void OnSelendokComboBaudrate();
    afx_msg void OnKillfocusTimeout();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL InitControl();
    BOOL InitControl_DeviceName(CStdString p_strVirtualDeviceName, CStdString* p_pStrDeviceNameSel);
    BOOL InitControl_ProtocolStackName(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString* p_pStrProtocolStackNameSel);
    BOOL InitControl_InterfaceName(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString* p_pStrInterfaceNameSel);
    BOOL InitControl_PortName(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString strInterfaceName, CStdString* p_pStrPortNameSel);
    BOOL InitControl_BaudrateTimeout(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, DWORD_PTR* p_pulBaudrateSel, DWORD_PTR* p_pulTimeout);

    BOOL UpdateControl_DeviceName();
    BOOL UpdateControl_ProtocolStackName();
    BOOL UpdateControl_InterfaceName();
    BOOL UpdateControl_PortName();
    BOOL UpdateControl_Baudrate();
    BOOL UpdateControl_Timeout();

    CStdString GetNoDeviceString();
    CStdString GetNoProtocolStackString();
    CStdString GetNoInterfaceString();
    CStdString GetNoPortString();
    CStdString GetNoBaudrateString();

    BOOL FindDeviceSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CDeviceSettings** p_ppDeviceSettings);

    void DeleteDeviceSettingsList();
    void CopyDeviceSettingsList(tDeviceSettingsList* p_pSourceList);

    void GetVirtualDeviceNameAddictedToDeviceName();

    tDeviceSettingsList m_DeviceSettingsList;
    CStdStringArray m_strVirtualDeviceNameArray;
    CStdString m_strVirtualDeviceName;
    CStdString m_strDeviceName;
    CStdString m_strInterfaceName;
    CStdString m_strPortName;
    CStdString m_strProtocolStackName;
    DWORD_PTR m_ulBaudrate;
    DWORD_PTR m_ulTimeout;
};
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_OPENDEVICEDLG_H__27DA5F7F_CF26_4880_B8A7_67E4F48B1DCA__INCLUDED_
