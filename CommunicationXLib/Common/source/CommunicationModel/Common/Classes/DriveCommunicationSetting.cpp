#include "stdafx.h"

#include "DriveCommunicationSetting.h"

CDriveCommunicationSetting::CDriveCommunicationSetting(void)
    : m_hBaseLayer(NULL)
    , m_hDrive(NULL)
    , m_hGateway(NULL)
    , m_ubNodeId(0)
    , m_ulGatewayBaudrate(1000000)
    , m_ulGatewayTimeout(500)
    , m_VirtualDeviceName(_T(""))
    , m_GatewayName(_T(""))
    , m_GatewayProtocolStackName(_T(""))
{
}

CDriveCommunicationSetting::~CDriveCommunicationSetting(void)
{
}

BOOL CDriveCommunicationSetting::operator==(const CDriveCommunicationSetting& p_rEqual)
{
    if(m_hBaseLayer != p_rEqual.m_hBaseLayer) return FALSE;
    if(m_hDrive != p_rEqual.m_hDrive) return FALSE;
    if(m_hGateway != p_rEqual.m_hGateway) return FALSE;

    if(m_ubNodeId != p_rEqual.m_ubNodeId) return FALSE;

    if(m_VirtualDeviceName != p_rEqual.m_VirtualDeviceName) return FALSE;
    if(m_GatewayName != p_rEqual.m_GatewayName) return FALSE;
    if(m_GatewayProtocolStackName != p_rEqual.m_GatewayProtocolStackName) return FALSE;

    if(m_ulGatewayBaudrate != p_rEqual.m_ulGatewayBaudrate) return FALSE;
    if(m_ulGatewayTimeout != p_rEqual.m_ulGatewayTimeout) return FALSE;

    if((*((CCommunicationSetting*)this)) == (*((CCommunicationSetting*)&p_rEqual))) return FALSE;

    return TRUE;
}

CDriveCommunicationSetting& CDriveCommunicationSetting::operator=(CDriveCommunicationSetting &p_rAssign)
{
    if(this != &p_rAssign)
    {
        m_hBaseLayer = p_rAssign.m_hBaseLayer;
        m_hDrive = p_rAssign.m_hDrive;
        m_hGateway = p_rAssign.m_hGateway;

        m_ubNodeId = p_rAssign.m_ubNodeId;

        m_VirtualDeviceName = p_rAssign.m_VirtualDeviceName;
        m_GatewayName = p_rAssign.m_GatewayName;
        m_GatewayProtocolStackName = p_rAssign.m_GatewayProtocolStackName;

        m_ulGatewayBaudrate = p_rAssign.m_ulGatewayBaudrate;
        m_ulGatewayTimeout = p_rAssign.m_ulGatewayTimeout;

        *((CCommunicationSetting*)this) = *((CCommunicationSetting*)&p_rAssign);
    }

    return *this;
}

BOOL CDriveCommunicationSetting::SetCommunicationSetting(HANDLE p_hBaseLayer, CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName)
{
    m_hBaseLayer = p_hBaseLayer;
    m_strProtocolStackName = p_ProtocolStackName;
    m_strInterfaceName = p_InterfaceName;
    m_strPortName = p_PortName;

    return TRUE;
}

BOOL CDriveCommunicationSetting::GetCommunicationSetting(HANDLE* p_phBaseLayer, CStdString* p_pProtocolStackName, CStdString* p_pInterfaceName, CStdString* p_pPortName)
{
    if(p_phBaseLayer) *p_phBaseLayer = m_hBaseLayer;
    if(p_pProtocolStackName) *p_pProtocolStackName = m_strProtocolStackName;
    if(p_pInterfaceName) *p_pInterfaceName = m_strInterfaceName;
    if(p_pPortName) *p_pPortName = m_strPortName;

    return TRUE;
}

BOOL CDriveCommunicationSetting::GetGatewaySetting(HANDLE* p_phGateway, CStdString* p_pGatewayProtocolStackName, CStdString* p_pGatewayName)
{
    if(p_phGateway) *p_phGateway = m_hGateway;
    if(p_pGatewayProtocolStackName) *p_pGatewayProtocolStackName = m_GatewayProtocolStackName;
    if(p_pGatewayName) *p_pGatewayName = m_GatewayName;

    return TRUE;
}

BOOL CDriveCommunicationSetting::SetGatewaySetting(HANDLE p_hGateway, CStdString p_GatewayProtocolStackName, CStdString p_GatewayName)
{
    m_hGateway = p_hGateway;
    m_GatewayName = p_GatewayName;
    m_GatewayProtocolStackName = p_GatewayProtocolStackName;

    return TRUE;
}

BOOL CDriveCommunicationSetting::SetGatewayProtocolStackSettings(DWORD p_ulGatewayBaudrate, DWORD p_ulGatewayTimeout)
{
    m_ulGatewayBaudrate = p_ulGatewayBaudrate;
    m_ulGatewayTimeout = p_ulGatewayTimeout;

    return TRUE;
}

BOOL CDriveCommunicationSetting::GetGatewayProtocolStackSettings(DWORD* p_pulGatewayBaudrate, DWORD* p_pulGatewayTimeout)
{
    if(p_pulGatewayBaudrate) *p_pulGatewayBaudrate = m_ulGatewayBaudrate;
    if(p_pulGatewayTimeout) *p_pulGatewayTimeout = m_ulGatewayTimeout;

    return TRUE;
}

BOOL CDriveCommunicationSetting::SetDeviceSetting(HANDLE p_hDrive, CStdString p_VirtualDeviceName, CStdString p_DeviceName, BYTE p_ubNodeId)
{
    m_hDrive = p_hDrive;
    m_VirtualDeviceName = p_VirtualDeviceName;
    m_strDeviceName = p_DeviceName;
    m_ubNodeId = p_ubNodeId;

    return TRUE;
}

BOOL CDriveCommunicationSetting::GetDeviceSetting(HANDLE* p_phDrive, CStdString* p_pVirtualDeviceName, CStdString* p_pDeviceName, BYTE* p_pubNodeId)
{
    if(p_phDrive) *p_phDrive = m_hDrive;
    if(p_pVirtualDeviceName) *p_pVirtualDeviceName = m_VirtualDeviceName;
    if(p_pDeviceName) *p_pDeviceName = m_strDeviceName;
    if(p_pubNodeId) *p_pubNodeId = m_ubNodeId;

    return TRUE;
}
