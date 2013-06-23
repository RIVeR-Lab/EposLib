// virtualcommandset_dialogmanager.h: Schnittstelle f�r die Klasse CVirtualCommandSet_DialogManager.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIRTUALCOMMANDSET_DIALOGMANAGER_H__B81BA008_5396_4C37_A80F_9A6D2844168F__INCLUDED_)
#define AFX_VIRTUALCOMMANDSET_DIALOGMANAGER_H__B81BA008_5396_4C37_A80F_9A6D2844168F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/VirtualCommandSet/Dialogs/SearchCommunicationDefinitions.h>
#include <CommunicationModel/VirtualCommandSet/Dialogs/OpenDeviceDlg.h>
#include <MmcTypeDefinition.h>
#include <CommunicationModel/VirtualCommandSet/Common/DeviceSettings.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>

class CVirtualCommandSet_Manager;
class CCommandRoot;

class CVirtualCommandSet_DialogManager
{
public:
    BOOL SearchCommunicationDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, HANDLE *p_pKeyHandle, CStdString p_strVirtualDeviceName, CCommandRoot* p_pSearchCommand, CStdString p_strSearchingInfo, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, CStdString p_strRootInterfaceName, CStdString p_strRootPortName, EDialogMode p_eDialogMode);
    HANDLE OpenDriveDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrDriveNameArray, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    HANDLE OpenPlcDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrPlcNameArray, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    HANDLE OpenVirtualDeviceDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdStringArray *p_pstrVirtualDeviceNameArray, CErrorInfo* p_pErrorInfo = NULL);

    CVirtualCommandSet_DialogManager();
    virtual ~CVirtualCommandSet_DialogManager();

private:
    BOOL InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, tDeviceSettingsList* p_pList);
    BOOL InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, tDeviceSettingsList* p_pList);
    BOOL InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, tDeviceSettingsList* p_pList);
    BOOL InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, tDeviceSettingsList* p_pList);
    BOOL InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, tDeviceSettingsList* p_pList);
    BOOL DeleteDeviceSettingsSelection(tDeviceSettingsList* p_pList);

private:
    DWORD_PTR m_ulTimeout;
    CStdStringArray m_strVirtualDeviceNameArray;
};

#endif // !defined(AFX_VIRTUALCOMMANDSET_DIALOGMANAGER_H__B81BA008_5396_4C37_A80F_9A6D2844168F__INCLUDED_)
