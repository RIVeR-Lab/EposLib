// DeviceSettings.cpp: Implementierungsdatei
//
#include "stdafx.h"


#include <CommunicationModel/VirtualCommandSet/Common/ProtocolStackSettings.h>
#include <CommunicationModel/VirtualCommandSet/Common/DeviceSettings.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceSettings

CDeviceSettings::CDeviceSettings()
{
	m_pDefaultProtocolStackSettings = NULL;
	CreateDefaultProtocolStackSettings();

	Reset();
}

CDeviceSettings::CDeviceSettings(const CDeviceSettings& other)
{
	if(this != &other)
	{
		*((CDeviceSettings*)this) = *((CDeviceSettings*)&other);

		m_strVirtualDeviceName = other.m_strVirtualDeviceName;
		m_strProtocolStackName = other.m_strProtocolStackName;
		m_strPortName = other.m_strPortName;
		m_strDeviceName = other.m_strDeviceName;
		m_strInterfaceName = other.m_strInterfaceName;
	}
}

void CDeviceSettings::DeleteProtocolStackSettingsList()
{
	CProtocolStackSettings* pSettings;

	for( tProtocolStackSettingsList::iterator it = m_ProtocolStackSettingsList.begin();
			it != m_ProtocolStackSettingsList.end(); it++ )
	{
		pSettings = (*it);
		delete pSettings;
	}

	m_ProtocolStackSettingsList.clear();
}

void CDeviceSettings::CreateDefaultProtocolStackSettings()
{
	if(!m_pDefaultProtocolStackSettings)
	{
		m_pDefaultProtocolStackSettings = new CProtocolStackSettings();
	}
}

void CDeviceSettings::DeleteDefaultProtocolStackSettings()
{
	if(m_pDefaultProtocolStackSettings)
	{
		delete m_pDefaultProtocolStackSettings;
		m_pDefaultProtocolStackSettings = NULL;
	}
}

void CDeviceSettings::AddProtocolStackSettings(CProtocolStackSettings* pProtocolStackSettings)
{
	if(pProtocolStackSettings!=NULL)
		m_ProtocolStackSettingsList.push_back(pProtocolStackSettings);
}

BOOL CDeviceSettings::GetProtocolStackSettingsList(tProtocolStackSettingsList** ppList)
{
	if(ppList)
	{
		*ppList = &m_ProtocolStackSettingsList;
		return TRUE;
	}

	return FALSE;
}

CDeviceSettings::~CDeviceSettings()
{
	DeleteProtocolStackSettingsList();
	DeleteDefaultProtocolStackSettings();
}

CDeviceSettings& CDeviceSettings::operator=(CDeviceSettings& other)
{
	if(this != &other)
	{
		m_strVirtualDeviceName = other.m_strVirtualDeviceName;
		m_strProtocolStackName = other.m_strProtocolStackName;
		m_strPortName = other.m_strPortName;
		m_strDeviceName = other.m_strDeviceName;
		m_strInterfaceName = other.m_strInterfaceName;

		CopyProtocolStackSettingsList(&other.m_ProtocolStackSettingsList);
		if(m_pDefaultProtocolStackSettings && other.m_pDefaultProtocolStackSettings)
		{
			*m_pDefaultProtocolStackSettings = *other.m_pDefaultProtocolStackSettings;
		}
	}

	return *this;
}

void CDeviceSettings::Reset()
{
	m_strProtocolStackName = " ";
	m_strPortName = " ";
	m_strVirtualDeviceName = " ";
	m_strDeviceName = " ";
	m_strInterfaceName = " ";
}

BOOL CDeviceSettings::operator==(const CDeviceSettings& data) const
{
	return ((m_strVirtualDeviceName == data.m_strVirtualDeviceName) && (m_strDeviceName == data.m_strDeviceName) && (m_strProtocolStackName == data.m_strProtocolStackName) && (m_strInterfaceName == data.m_strInterfaceName) && (m_strPortName == data.m_strPortName));
}

void CDeviceSettings::Init(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName)
{
	m_strVirtualDeviceName = strVirtualDeviceName;
	m_strDeviceName = strDeviceName;
	m_strProtocolStackName = strProtocolStackName;
	m_strInterfaceName = strInterfaceName;
	m_strPortName = strPortName;
}

CStdString CDeviceSettings::GetVirtualDeviceName()
{
	return m_strVirtualDeviceName;
}

CStdString CDeviceSettings::GetDeviceName()
{
	return m_strDeviceName;
}

CStdString CDeviceSettings::GetProtocolStackName()
{
	return m_strProtocolStackName;
}

CStdString CDeviceSettings::GetInterfaceName()
{
	return m_strInterfaceName;
}

CStdString CDeviceSettings::GetPortName()
{
	return m_strPortName;
}

void CDeviceSettings::CopyProtocolStackSettingsList(tProtocolStackSettingsList* pSourceList)
{
	CProtocolStackSettings* pNewSettings = NULL;

	DeleteProtocolStackSettingsList();
	if(pSourceList)
	{
		for( tProtocolStackSettingsList::iterator it = pSourceList->begin();
				it != pSourceList->end(); it++ )
		{
			CProtocolStackSettings* pSettings = (*it);
			if(pSettings)
			{
				pNewSettings = new CProtocolStackSettings();
				*pNewSettings = *pSettings;
				m_ProtocolStackSettingsList.push_back(pNewSettings);
			}
		}
	}
}

BOOL CDeviceSettings::SetDefaultProtocolStackSettings(CProtocolStackSettings* pProtocolStackSettings)
{
	if(pProtocolStackSettings && m_pDefaultProtocolStackSettings)
	{
		*m_pDefaultProtocolStackSettings = *pProtocolStackSettings;
		return TRUE;
	}

	return FALSE;
}

BOOL CDeviceSettings::GetDefaultProtocolStackSettings(CProtocolStackSettings* pProtocolStackSettings)
{
	if(pProtocolStackSettings && m_pDefaultProtocolStackSettings)
	{
		*pProtocolStackSettings = *m_pDefaultProtocolStackSettings;
		return TRUE;
	}

	return FALSE;
}
