#pragma once

#include <CommunicationModel/Common/Classes/CommunicationSetting.h>

class CDriveCommunicationSetting : public CCommunicationSetting
{
public:
    //Communication
    BOOL SetCommunicationSetting(HANDLE p_hBaseLayer, CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName);
    BOOL GetCommunicationSetting(HANDLE* p_phBaseLayer, CStdString* p_pProtocolStackName, CStdString* p_pInterfaceName, CStdString* p_pPortName);

    //Gateway
    BOOL GetGatewaySetting(HANDLE* p_phGateway, CStdString* p_pGatewayProtocolStackName, CStdString* p_pGatewayName);
    BOOL SetGatewaySetting(HANDLE p_hGateway, CStdString p_GatewayProtocolStackName, CStdString p_GatewayName);

    BOOL SetGatewayProtocolStackSettings(DWORD p_ulGatewayBaudrate, DWORD p_ulGatewayTimeout);
    BOOL GetGatewayProtocolStackSettings(DWORD* p_pulGatewayBaudrate, DWORD* p_pulGatewayTimeout);

    //Device
    BOOL GetDeviceSetting(HANDLE* p_phDrive, CStdString* p_pVirtualDeviceName, CStdString* p_pDeviceName, BYTE* p_pubNodeId);
    BOOL SetDeviceSetting(HANDLE p_hDrive, CStdString p_VirtualDeviceName, CStdString p_DeviceName, BYTE p_ubNodeId);

    BOOL operator ==(const CDriveCommunicationSetting& p_rEqual);
    CDriveCommunicationSetting& operator=(CDriveCommunicationSetting &p_rAssign);

    CDriveCommunicationSetting(void);
    virtual ~CDriveCommunicationSetting(void);

private:
    BYTE m_ubNodeId;
    CStdString m_GatewayName;
    CStdString m_GatewayProtocolStackName;
    CStdString m_VirtualDeviceName;
    DWORD m_ulGatewayBaudrate;
    DWORD m_ulGatewayTimeout;
    HANDLE m_hBaseLayer;
    HANDLE m_hDrive;
    HANDLE m_hGateway;
};
