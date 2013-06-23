// InterfacePlugInWnd.h: Schnittstelle f�r die Klasse CInterfacePlugInWnd.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_InterfacePlugInWnd_H__A1888084_8F67_4C84_926A_B394A75720BF__INCLUDED_)
#define AFX_InterfacePlugInWnd_H__A1888084_8F67_4C84_926A_B394A75720BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WINVER

#include <Dbt.h>

// Copy from HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\DeviceClasses
static const GUID GUID_DEVINTERFACE_LIST[] = 
{
	// GUID_DEVINTERFACE_USB_DEVICE
	{ 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

	//// GUID_DEVINTERFACE_COMPORT
	{ 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } },

	// GUID_DEVINTERFACE_HID, 
	{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
			 
};

class CInterfacePlugInManager;

class CInterfacePlugInWnd : public CWnd
{
public:
	CInterfacePlugInWnd();
	virtual ~CInterfacePlugInWnd();
	BOOL Init(CInterfacePlugInManager* pPlugInManager);
	BOOL CheckNotification();

protected:
	afx_msg LRESULT OnMyDeviceChange(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//{{AFX_MSG(CInterfacePlugInWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL RegisterNotification();
	BOOL DeviceAdded(PDEV_BROADCAST_HDR p_pHdr);
	BOOL DeviceRemoved(PDEV_BROADCAST_HDR p_pHdr);

	BOOL NotifyDeviceAdded();
	BOOL NotifyDeviceRemoved();

private:
	CInterfacePlugInManager* m_pPlugInManager;
	BOOL m_oNotificationPending;
	BOOL m_oDeviceAdded;
	DWORD m_ulStartTime;

};
#endif //WINVER

#endif // !defined(AFX_InterfacePlugInWnd_H__A1888084_8F67_4C84_926A_B394A75720BF__INCLUDED_)
