// ProtocolStackSettings.cpp: Implementierungsdatei
//
#include "stdafx.h"

#include <stdio.h>
#include <CommunicationModel/VirtualCommandSet/Common/ProtocolStackSettings.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProtocolStackSettings

CProtocolStackSettings::CProtocolStackSettings()
{
	Reset();
}

CProtocolStackSettings::CProtocolStackSettings(const CProtocolStackSettings& other)
{
	if(this != &other)
	{
		*((CProtocolStackSettings*)this) = *((CProtocolStackSettings*)&other);

		m_dBaudrate = other.m_dBaudrate;
		m_dTimeout = other.m_dTimeout;
	}
}

CProtocolStackSettings::~CProtocolStackSettings()
{
}

CProtocolStackSettings& CProtocolStackSettings::operator=(CProtocolStackSettings& other)
{
	if(this != &other)
	{
		m_dBaudrate = other.m_dBaudrate;
		m_dTimeout = other.m_dTimeout;
	}

	return *this;
}

void CProtocolStackSettings::Reset()
{
	m_dBaudrate = 0;
	m_dTimeout = 0;
}

BOOL CProtocolStackSettings::operator==(const CProtocolStackSettings& data) const
{
	return (m_dBaudrate == data.m_dBaudrate);
}


void CProtocolStackSettings::Init(DWORD dBaudrate,DWORD dTimeout)
{
	m_dBaudrate = dBaudrate;
	m_dTimeout = dTimeout;
}

DWORD CProtocolStackSettings::GetBaudrate()
{
	return m_dBaudrate;
}

CStdString CProtocolStackSettings::GetBaudrateString()
{
	std::string baudrate;
	char szBaudrate[32];

	sprintf( szBaudrate, "%i", (int)m_dBaudrate);
	baudrate = szBaudrate;

	return baudrate;
}

DWORD CProtocolStackSettings::GetTimeout()
{
	return m_dTimeout;
}
