// HandleRegistration_DCS.cpp: Implementierung der Klasse CHandleRegistration_DCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <stdio.h>
#include <algorithm>

#include "../Devices/BaseClasses/DeviceBase.h"
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>
#include <Thread/MmcCriticalSection.h>
#include "HandleRegistration_DCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CHandleRegistration_DCS::CHandleRegistration_DCS()
{
	m_Handle_DCS = 0;
	m_pDevice = 0;
	m_pManager = 0;
	m_Handle_PS = 0;
	m_pCriticalSection = 0;

	CreateCriticalSection();
}

CHandleRegistration_DCS::~CHandleRegistration_DCS()
{
	if(m_pDevice) delete m_pDevice;
	DeleteCriticalSection();
}

BOOL CHandleRegistration_DCS::CreateCriticalSection()
{
	BOOL oResult = FALSE;
	if(!m_pCriticalSection)
	{
		m_pCriticalSection = new CMmcCriticalSectionEx();
		oResult = TRUE;
	}

	return oResult;
}

BOOL CHandleRegistration_DCS::DeleteCriticalSection()
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

HANDLE CHandleRegistration_DCS::GetKeyHandle()
{
	return m_Handle_DCS;
}

BOOL CHandleRegistration_DCS::GetRegistrationValues(CDeviceBase** ppDevice, CProtocolStackManagerBase** ppManager, HANDLE* phProtocolStackHandle)
{
	if(ppDevice) *ppDevice = m_pDevice;
	if(ppManager) *ppManager = m_pManager;
	if(phProtocolStackHandle) *phProtocolStackHandle = m_Handle_PS;
	return TRUE;
}

BOOL CHandleRegistration_DCS::IsEmpty()
{
	return (!m_pDevice || !m_pManager || !m_Handle_PS);
}

void CHandleRegistration_DCS::SetKeyHandle(HANDLE hDeviceCommandSetHandle)
{
	m_Handle_DCS = hDeviceCommandSetHandle;
}

void CHandleRegistration_DCS::Init(CDeviceBase* pDevice, CProtocolStackManagerBase* pManager, HANDLE hProtocolStackHandle)
{
	if(pDevice) m_pDevice = pDevice->Clone();
	m_pManager = pManager;
	m_Handle_PS = hProtocolStackHandle;
}

void CHandleRegistration_DCS::DeleteRegistration()
{
	m_Handle_DCS = 0;
	if(m_pDevice) delete m_pDevice;
	m_pDevice = 0;
	m_pManager = 0;
	m_Handle_PS = 0;
}

BOOL CHandleRegistration_DCS::IsEqual(CDeviceBase* pDevice, CProtocolStackManagerBase* pManager, HANDLE hProtocolStackHandle)
{
	CStdString protocolStackName = _T("");
	CStdString protocolStackName2 = _T("");
	BOOL oResult = FALSE;

	if((m_pDevice && pDevice) && (m_pManager && pManager))
	{
		//Init
		oResult = TRUE;

		//Device
		if(oResult && !m_pDevice->IsEqual(pDevice)) oResult = FALSE;

		//ProtocolStack
		m_pManager->PS_GetProtocolStackName(m_Handle_PS, &protocolStackName);
		pManager->PS_GetProtocolStackName(hProtocolStackHandle, &protocolStackName2);

		if(oResult && protocolStackName.CompareNoCase(protocolStackName2) != 0) oResult = FALSE;
		if(oResult && (m_Handle_PS != hProtocolStackHandle)) oResult = FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_DCS::DoLock(DWORD p_ulTimeout)
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

BOOL CHandleRegistration_DCS::DoUnlock()
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

BOOL CHandleRegistration_DCS::IsLocked()
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->IsLocked();
	}

	return oResult;
}

bool CHandleRegistration_DCS::AbortLock(bool oActive)
{
	bool oResult = false;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->AbortLock(oActive);
	}

	return oResult;
}

BOOL CHandleRegistration_DCS::GetTraceString(CStdString& p_rTraceString)
{
	BOOL oResult = TRUE;

#ifdef _DEBUG
	p_rTraceString.Format(_T("HandleRegistration DCS 0x%.8x"), (unsigned int)m_Handle_DCS);
#endif

    return oResult;
}
