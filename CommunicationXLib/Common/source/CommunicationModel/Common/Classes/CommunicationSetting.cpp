// CommunicationSetting.cpp: Implementierung der Klasse CCommunicationSetting.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/Common/Classes/CommunicationSetting.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommunicationSetting::CCommunicationSetting()
{
	m_strVirtualDeviceName = "";
	m_strDeviceName = "";
	m_strProtocolStackName = "";
	m_strInterfaceName = "";
	m_strPortName = "";

	m_dBaudrate = 0;
	m_dTimeout = 0;
}

CCommunicationSetting::~CCommunicationSetting()
{
}

BOOL CCommunicationSetting::operator ==(const CCommunicationSetting& m)
{
	if(m_strVirtualDeviceName.CompareNoCase(m.m_strVirtualDeviceName) != 0) return FALSE;
	if(m_strDeviceName.CompareNoCase(m.m_strDeviceName) != 0) return FALSE;
	if(m_strProtocolStackName.CompareNoCase(m.m_strProtocolStackName) != 0) return FALSE;
	if(m_strInterfaceName.CompareNoCase(m.m_strInterfaceName) != 0) return FALSE;
	if(m_strPortName.CompareNoCase(m.m_strPortName) != 0) return FALSE;

	if(m_dBaudrate != m.m_dBaudrate) return FALSE;
	if(m_dTimeout != m.m_dTimeout) return FALSE;

	return TRUE;
}

CCommunicationSetting& CCommunicationSetting::operator=(CCommunicationSetting &other)
{
	if(this != &other)
	{
		m_strVirtualDeviceName = other.m_strVirtualDeviceName;
		m_strDeviceName = other.m_strDeviceName;
		m_strProtocolStackName = other.m_strProtocolStackName;
		m_strInterfaceName = other.m_strInterfaceName;
		m_strPortName = other.m_strPortName;

		m_dBaudrate = other.m_dBaudrate;
		m_dTimeout = other.m_dTimeout;
	}
	return *this;
}

void CCommunicationSetting::SetProtocolStackSettings(DWORD dBaudrate,DWORD dTimeout)
{
	m_dBaudrate = dBaudrate;
	m_dTimeout = dTimeout;
}

void CCommunicationSetting::GetProtocolStackSettings(DWORD* pdBaudrate,DWORD* pdTimeout)
{
	if(pdBaudrate) *pdBaudrate = m_dBaudrate;
	if(pdTimeout) *pdTimeout = m_dTimeout;
}

void CCommunicationSetting::SetDeviceSetting(CStdString strVirtualDeviceName, CStdString strDeviceName,CStdString strProtocolStackName,CStdString strInterfaceName,CStdString strPortName)
{
	m_strVirtualDeviceName = strVirtualDeviceName;
	m_strDeviceName = strDeviceName;
	m_strProtocolStackName = strProtocolStackName;
	m_strInterfaceName = strInterfaceName;
	m_strPortName = strPortName;
}

void CCommunicationSetting::GetDeviceSetting(CStdString *pStrVirtualDeviceName,CStdString *pStrDeviceName,CStdString *pStrProtocolStackName,CStdString *pStrInterfaceName,CStdString *pStrPortName)
{
	if(pStrVirtualDeviceName) *pStrVirtualDeviceName = m_strVirtualDeviceName;
	if(pStrDeviceName) *pStrDeviceName = m_strDeviceName;
	if(pStrProtocolStackName) *pStrProtocolStackName = m_strProtocolStackName;
	if(pStrInterfaceName) *pStrInterfaceName = m_strInterfaceName;
	if(pStrPortName) *pStrPortName = m_strPortName;
}

