#pragma once

#include <list>
#include <CommunicationModel/VirtualCommandSet/Common/DeviceSettings.h>

class CGatewaySettings;
class CProtocolStackManagerBase;

typedef std::list<CGatewaySettings*> tGatewaySettingsList;

class CProtocolStackDialogManager
{
public:
    HANDLE OpenProtocolStackDlg(CProtocolStackManagerBase* p_pProtocolStackManager, CStdStringArray *p_paProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    HANDLE OpenLayerProtocolStackDlg(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hCommunication, CErrorInfo* p_pErrorInfo = NULL);

    CProtocolStackDialogManager(void);
    virtual ~CProtocolStackDialogManager(void);

private:
    BOOL InitGatewaySettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, tGatewaySettingsList* p_pList);
    BOOL InitGatewaySettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_GatewayProtocolStackName, tGatewaySettingsList* p_pList);
    BOOL DeleteGatewaySettingsSelection(tGatewaySettingsList* p_pList);

    BOOL InitDeviceSettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, tDeviceSettingsList* p_pList);
    BOOL InitDeviceSettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName, tDeviceSettingsList* p_pList);
    BOOL InitDeviceSettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, tDeviceSettingsList* p_pList);
    BOOL DeleteDeviceSettingsSelection(tDeviceSettingsList* p_pList);

private:
    DWORD_PTR m_ulTimeout;
};
