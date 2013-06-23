// HandleRegistration_VCS.cpp: Implementierung der Klasse CHandleRegistration_VCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <stdio.h>
#include <algorithm>

#include "../VirtualDevices/BaseClasses/VirtualDeviceBase.h"
#include "../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <Thread/MmcCriticalSection.h>

#include "HandleRegistration_VCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CHandleRegistration_VCS::CHandleRegistration_VCS()
{
	m_Handle_VCS = 0;
	m_pVirtualDevice = 0;
	m_pManager = 0;
	m_Handle_DCS = 0;
	m_pCriticalSection = 0;

	CreateCriticalSection();
}

CHandleRegistration_VCS::~CHandleRegistration_VCS()
{
	if(m_pVirtualDevice) delete m_pVirtualDevice;
	DeleteCriticalSection();
}

BOOL CHandleRegistration_VCS::CreateCriticalSection()
{
	BOOL oResult = FALSE;

	if(!m_pCriticalSection)
	{
		m_pCriticalSection = new CMmcCriticalSectionEx();
		oResult = TRUE;
	}

	return oResult;
}

BOOL CHandleRegistration_VCS::DeleteCriticalSection()
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		delete m_pCriticalSection;
		m_pCriticalSection = 0;
		oResult = TRUE;
	}

	return oResult;
}

HANDLE CHandleRegistration_VCS::GetKeyHandle()
{
	return m_Handle_VCS;
}

BOOL CHandleRegistration_VCS::GetRegistrationValues(CVirtualDeviceBase** ppVirtualDevice, CDeviceCommandSetManagerBase** ppManager, HANDLE* phDeviceCommandSetHandle)
{
	if(ppVirtualDevice) *ppVirtualDevice = m_pVirtualDevice;
	if(ppManager) *ppManager = m_pManager;
	if(phDeviceCommandSetHandle) *phDeviceCommandSetHandle = m_Handle_DCS;
	return TRUE;
}

BOOL CHandleRegistration_VCS::IsEqual(CVirtualDeviceBase* pVirtualDevice, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle)
{
	CStdString deviceName = _T("");
	CStdString deviceName2 = _T("");
	BOOL oResult = FALSE;

	if((m_pVirtualDevice && pVirtualDevice) && (m_pManager && pManager))
	{
		//Init
		oResult = TRUE;

		//VirtualDevice
		if(oResult && !m_pVirtualDevice->IsEqual(pVirtualDevice)) oResult = FALSE;

		//Device
		m_pManager->DCS_GetDeviceName(m_Handle_DCS, &deviceName);
		pManager->DCS_GetDeviceName(hDeviceCommandSetHandle, &deviceName2);

		if(oResult && deviceName.CompareNoCase(deviceName2) != 0) oResult = FALSE;

		if(oResult && (m_Handle_DCS != hDeviceCommandSetHandle)) oResult = FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_VCS::IsEmpty()
{
	return (!m_pVirtualDevice || !m_pManager || !m_Handle_DCS);
}

void CHandleRegistration_VCS::SetKeyHandle(HANDLE hVCS_Handle)
{
	m_Handle_VCS = hVCS_Handle;
}

void CHandleRegistration_VCS::Init(CVirtualDeviceBase* pVirtualDevice, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle)
{
	if(pVirtualDevice) m_pVirtualDevice = pVirtualDevice->Clone();
	m_pManager = pManager;
	m_Handle_DCS = hDeviceCommandSetHandle;
}

void CHandleRegistration_VCS::DeleteRegistration()
{
	m_Handle_VCS = 0;
	if(m_pVirtualDevice)
		delete m_pVirtualDevice;
	m_pVirtualDevice = 0;
	m_pManager = 0;
	m_Handle_DCS = 0;
}

BOOL CHandleRegistration_VCS::DoLock(DWORD p_ulTimeout)
{
	CStdString traceString = _T("");
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
#ifdef _DEBUG
		//TraceString
		GetTraceString(traceString);
#endif
		//Lock
		oResult = m_pCriticalSection->Lock(p_ulTimeout, traceString);
	}

	return oResult;
}

BOOL CHandleRegistration_VCS::DoUnlock()
{
	CStdString traceString = _T("");
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
#ifdef _DEBUG
		//TraceString
		GetTraceString(traceString);
#endif
		//Unlock
		oResult = m_pCriticalSection->Unlock(traceString);
	}

	return oResult;
}

BOOL CHandleRegistration_VCS::IsLocked()
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->IsLocked();
	}

	return oResult;
}

BOOL CHandleRegistration_VCS::AbortLock(BOOL oActive)
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->AbortLock(oActive==TRUE) ? TRUE : FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_VCS::GetTraceString(CStdString& p_rTraceString)
{
	BOOL oResult = TRUE;

#ifdef _DEBUG
	p_rTraceString.Format(_T("HandleRegistration VCS 0x%.8x"), (unsigned int)m_Handle_VCS);
#endif

    return oResult;
}
