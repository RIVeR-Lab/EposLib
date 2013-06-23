#if !defined(AFX_DEVICESETTINGS_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_)
#define AFX_DEVICESETTINGS_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceSettings.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Window CDeviceSettings

class CProtocolStackSettings;

#include <list>
#include <MmcTypeDefinition.h>

typedef std::list<CProtocolStackSettings*> tProtocolStackSettingsList;

class CDeviceSettings
{
public:
	CDeviceSettings();
	virtual ~CDeviceSettings();

	void Init(CStdString strVirtualDeviceName,CStdString strDeviceName,CStdString strProtocolStackName, CStdString strInterfaceName,CStdString strPortName);
	CStdString GetVirtualDeviceName();
	CStdString GetDeviceName();
	CStdString GetProtocolStackName();
	CStdString GetInterfaceName();
	CStdString GetPortName();

	BOOL operator==(const CDeviceSettings& data) const;
	CDeviceSettings& operator=(CDeviceSettings& other);
	CDeviceSettings(const CDeviceSettings& other);
	void Reset();

	void AddProtocolStackSettings(CProtocolStackSettings* pProtocolStackSettings);
	BOOL GetProtocolStackSettingsList(tProtocolStackSettingsList** ppList);

	BOOL SetDefaultProtocolStackSettings(CProtocolStackSettings* pProtocolStackSettings);
	BOOL GetDefaultProtocolStackSettings(CProtocolStackSettings* pProtocolStackSettings);

private:
	void CreateDefaultProtocolStackSettings();
	void DeleteDefaultProtocolStackSettings();

	void DeleteProtocolStackSettingsList();
	void CopyProtocolStackSettingsList(tProtocolStackSettingsList* pSourceList);

private:
	CStdString m_strVirtualDeviceName;
	CStdString m_strDeviceName;
	CStdString m_strProtocolStackName;
	CStdString m_strInterfaceName;
	CStdString m_strPortName;

	CProtocolStackSettings* m_pDefaultProtocolStackSettings;
	tProtocolStackSettingsList m_ProtocolStackSettingsList;
};

typedef std::list<CDeviceSettings*> tDeviceSettingsList;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_DEVICESETTINGS_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_
