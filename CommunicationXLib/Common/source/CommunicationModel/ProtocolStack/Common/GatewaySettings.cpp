// GatewaySettings.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include <CommunicationModel/ProtocolStack/Common/GatewaySettings.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGatewaySettings

CGatewaySettings::CGatewaySettings()
{
    Reset();
}

CGatewaySettings::CGatewaySettings(const CGatewaySettings& other)
{
    if(this != &other)
    {
        *((CGatewaySettings*)this) = *((CGatewaySettings*)&other);

        m_GatewayProtocolStackName = other.m_GatewayProtocolStackName;
        m_GatewayName = other.m_GatewayName;
    }
}

CGatewaySettings::~CGatewaySettings()
{
}

CGatewaySettings& CGatewaySettings::operator=(CGatewaySettings& other)
{
    if(this != &other)
    {
        m_GatewayProtocolStackName = other.m_GatewayProtocolStackName;
        m_GatewayName = other.m_GatewayName;
    }

    return *this;
}

void CGatewaySettings::Reset()
{
    m_GatewayProtocolStackName = " ";
    m_GatewayName = " ";
}

BOOL CGatewaySettings::operator==(const CGatewaySettings& data) const
{
    return ((m_GatewayName == data.m_GatewayName) && (m_GatewayProtocolStackName == data.m_GatewayProtocolStackName));
}

void CGatewaySettings::Init(CStdString p_GatewayName, CStdString p_GatewayProtocolStackName)
{
    m_GatewayName = p_GatewayName;
    m_GatewayProtocolStackName = p_GatewayProtocolStackName;
}

CStdString CGatewaySettings::GetGatewayName()
{
    return m_GatewayName;
}

CStdString CGatewaySettings::GetGatewayProtocolStackName()
{
    return m_GatewayProtocolStackName;
}

