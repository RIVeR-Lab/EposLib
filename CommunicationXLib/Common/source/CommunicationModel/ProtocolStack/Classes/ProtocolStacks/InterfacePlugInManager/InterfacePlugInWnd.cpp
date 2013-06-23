// InterfacePlugInWnd.cpp: Implementierung der Klasse CInterfacePlugInWnd.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <Process/MmcProcess.h>
#include <Log/MmcLogger.h>
#include "InterfacePlugInManager.h"
#include "InterfacePlugInWnd.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
#ifdef WINVER

CInterfacePlugInWnd::CInterfacePlugInWnd()
{
	m_pPlugInManager = NULL;
	m_oNotificationPending = FALSE;
	m_oDeviceAdded = FALSE;
	m_ulStartTime = 0;
}

CInterfacePlugInWnd::~CInterfacePlugInWnd()
{
	
}

BOOL CInterfacePlugInWnd::Init(CInterfacePlugInManager* pPlugInManager)
{
	BOOL oResult = FALSE;

	m_pPlugInManager = pPlugInManager;

	return oResult;
}

BOOL CInterfacePlugInWnd::CheckNotification()
{
	DWORD ulDelayTime = 0;
	BOOL oResult = FALSE;

	if(m_pPlugInManager)
	{
		if(m_oNotificationPending)
		{
			m_pPlugInManager->GetNotificationDelay(ulDelayTime);
			if(ulDelayTime < (MmcGetTickCount()-m_ulStartTime))
			{
				//Logging
				#if defined _Logging_UsbEnumeration || defined _Logging_HidEnumeration
					if(m_oDeviceAdded)
					{
						m_pPlugInManager->WriteToLogFile(_T("Event PlugIn"));
					}
					else
					{
						m_pPlugInManager->WriteToLogFile(_T("Event PlugOut"));
					}
				#endif

				//Update
				m_pPlugInManager->UpdateDevice();
				m_oNotificationPending = FALSE;
				m_ulStartTime = 0;
				oResult = TRUE;
			}
		}
	}

	return oResult;
}

BEGIN_MESSAGE_MAP(CInterfacePlugInWnd, CWnd)
	//{{AFX_MSG_MAP(CInterfacePlugInWnd)
	ON_MESSAGE(WM_DEVICECHANGE,OnMyDeviceChange) 
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()


LRESULT CInterfacePlugInWnd::OnMyDeviceChange(WPARAM wParam, LPARAM lParam)
{
  PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;

  switch (wParam)
  {
    case DBT_DEVICEARRIVAL: DeviceAdded(pHdr); break;
    case DBT_DEVICEREMOVECOMPLETE: DeviceRemoved(pHdr); break;
	default: break;
  }

  return 0;
}

BOOL CInterfacePlugInWnd::DeviceAdded(PDEV_BROADCAST_HDR p_pHdr)
{
	PDEV_BROADCAST_DEVICEINTERFACE pDevInf = NULL;
	PDEV_BROADCAST_PORT pDevPort = NULL;
	BOOL oResult = FALSE;

	if(p_pHdr)
	{
		if (p_pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
		{
			pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)p_pHdr;
			oResult = NotifyDeviceAdded();
		}
		else if(p_pHdr->dbch_devicetype == DBT_DEVTYP_PORT)
		{
			pDevPort = (PDEV_BROADCAST_PORT)p_pHdr;
			oResult = NotifyDeviceAdded();
		}
	}

	return oResult;
}

BOOL CInterfacePlugInWnd::DeviceRemoved(PDEV_BROADCAST_HDR p_pHdr)
{
	PDEV_BROADCAST_DEVICEINTERFACE pDevInf = NULL;
	PDEV_BROADCAST_PORT pDevPort = NULL;
	BOOL oResult = FALSE;

	if(p_pHdr)
	{
		if (p_pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
		{
			pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)p_pHdr;
			oResult = NotifyDeviceRemoved();
	     
		}
		else if(p_pHdr->dbch_devicetype == DBT_DEVTYP_PORT)
		{
			pDevPort = (PDEV_BROADCAST_PORT)p_pHdr;
			oResult = NotifyDeviceRemoved();
		}
	}

	return oResult;
}

BOOL CInterfacePlugInWnd::NotifyDeviceAdded()
{
	BOOL oResult = FALSE;

	if(m_pPlugInManager)
	{
		if(!m_oNotificationPending)
		{
			m_oNotificationPending = TRUE;
			m_oDeviceAdded = TRUE;
			m_ulStartTime = MmcGetTickCount();
		}
		oResult = TRUE;
	}

	return oResult;
}

BOOL CInterfacePlugInWnd::NotifyDeviceRemoved()
{
	BOOL oResult = FALSE;

	if(m_pPlugInManager)
	{
		if(!m_oNotificationPending)
		{
			m_oNotificationPending = TRUE;
			m_oDeviceAdded = FALSE;
			m_ulStartTime = MmcGetTickCount();
		}
		oResult = TRUE;
	}

	return oResult;
}

int CInterfacePlugInWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	RegisterNotification();

	return 0;
}

BOOL CInterfacePlugInWnd::RegisterNotification()
{
	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	BOOL oResult = TRUE;

	//Init NotificationFilter
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

	//Register all Device Class Guids
	for(int i=0; i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++) 
	{
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(),&NotificationFilter,DEVICE_NOTIFY_WINDOW_HANDLE);
		if(!hDevNotify )
		{
			oResult = FALSE;
		}
	}

	return oResult;
}
#endif //WINVER
