// HandleRegistration_PS.cpp: Implementierung der Klasse CHandleRegistration_PS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <stdio.h>
#include <algorithm>

#include "../ProtocolStacks/BaseClasses/ProtocolStackBase.h"
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include "../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <Thread/MmcCriticalSection.h>

#include "HandleRegistration_PS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CHandleRegistration_PS::CHandleRegistration_PS()
{
	m_hProtocolStackHandle = 0;
	m_pProtocol = 0;

	m_pInterfaceManager = 0;
	m_hInterfaceHandle = 0;
	m_pDeviceCommandSetManager = 0;
	m_hDeviceCommandSetHandle = 0;
	m_pCriticalSection = 0;

	CreateCriticalSection();
}

CHandleRegistration_PS::~CHandleRegistration_PS()
{
	if(m_pProtocol) delete m_pProtocol;
	DeleteCriticalSection();
}

BOOL CHandleRegistration_PS::CreateCriticalSection()
{
	BOOL oResult = FALSE;

	if(!m_pCriticalSection)
	{
		m_pCriticalSection = new CMmcCriticalSectionEx();
		oResult = TRUE;
	}

	return oResult;
}

BOOL CHandleRegistration_PS::DeleteCriticalSection()
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

HANDLE CHandleRegistration_PS::GetKeyHandle()
{
	return m_hProtocolStackHandle;
}

BOOL CHandleRegistration_PS::GetRegistrationValues(CProtocolStackBase** ppProtocol, CInterfaceManagerBase** ppManager, HANDLE* phInterfaceHandle)
{
	BOOL oResult = FALSE;

	if(m_pInterfaceManager)
	{
		if(ppProtocol) *ppProtocol = m_pProtocol;
		if(ppManager) *ppManager = m_pInterfaceManager;
		if(phInterfaceHandle) *phInterfaceHandle = m_hInterfaceHandle;
		oResult = TRUE;
	}

	return oResult;
}

BOOL CHandleRegistration_PS::GetRegistrationValues(CProtocolStackBase** ppProtocolStack, CDeviceCommandSetManagerBase** ppManager, HANDLE* phDeviceCommandSetHandle)
{
	BOOL oResult = FALSE;

	if(m_pDeviceCommandSetManager)
	{
		if(ppProtocolStack) *ppProtocolStack = m_pProtocol;
		if(ppManager) *ppManager = m_pDeviceCommandSetManager;
		if(phDeviceCommandSetHandle) *phDeviceCommandSetHandle = m_hDeviceCommandSetHandle;
		oResult = TRUE;
	}

	return oResult;
}

BOOL CHandleRegistration_PS::GetRegistrationValues(CProtocolStackBase** ppProtocolStack, CLayerManagerBase** ppManager, HANDLE* phHandle)
{
	BOOL oResult = FALSE;

	if(!oResult) oResult = GetRegistrationValues(ppProtocolStack, (CInterfaceManagerBase**)ppManager, phHandle);
	if(!oResult) oResult = GetRegistrationValues(ppProtocolStack, (CDeviceCommandSetManagerBase**)ppManager, phHandle);

	return oResult;
}

BOOL CHandleRegistration_PS::IsEqual(CProtocolStackBase* pProtocol, CInterfaceManagerBase* pManager, HANDLE hInterfaceHandle)
{
	CStdString interfaceName = _T("");
	CStdString interfaceName2 = _T("");
	BOOL oResult = FALSE;

	if((m_pProtocol && pProtocol) && (m_pInterfaceManager && pManager))
	{
		//Init
		oResult = TRUE;

		//ProtocolStack
		if(oResult && !m_pProtocol->IsEqual(pProtocol)) oResult = FALSE;

		//Interface
		m_pInterfaceManager->I_GetInterfaceName(m_hInterfaceHandle, &interfaceName);
		pManager->I_GetInterfaceName(hInterfaceHandle, &interfaceName2);

		if(oResult && interfaceName.CompareNoCase(interfaceName2) != 0) oResult = FALSE;

		if(oResult && (m_hInterfaceHandle != hInterfaceHandle)) oResult = FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_PS::IsEqual(CProtocolStackBase* pProtocolStack, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle)
{
	CStdString deviceName = _T("");
	CStdString deviceName2 = _T("");
	BOOL oResult = FALSE;

	if((m_pProtocol && pProtocolStack) && (m_pDeviceCommandSetManager && pManager))
	{
		//Init
		oResult = TRUE;

		//ProtocolStack
		if(oResult && !m_pProtocol->IsEqual(pProtocolStack)) oResult = FALSE;

		//Device
		m_pDeviceCommandSetManager->DCS_GetDeviceName(m_hDeviceCommandSetHandle, &deviceName);
		pManager->DCS_GetDeviceName(hDeviceCommandSetHandle, &deviceName2);

		if(oResult && deviceName.CompareNoCase(deviceName2) != 0) oResult = FALSE;
		if(oResult && (m_hDeviceCommandSetHandle != hDeviceCommandSetHandle)) oResult = FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_PS::IsEmpty()
{
	BOOL oResult = TRUE;

	if(m_pProtocol)
	{
		if(m_pInterfaceManager && m_hInterfaceHandle) oResult = FALSE;
		if(m_pDeviceCommandSetManager && m_hDeviceCommandSetHandle) oResult = FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_PS::IsBasedOnInterface()
{
	return (m_pProtocol && m_pInterfaceManager && m_hInterfaceHandle);
}

BOOL CHandleRegistration_PS::IsBasedOnDevice()
{
	return (m_pProtocol && m_pDeviceCommandSetManager && m_hDeviceCommandSetHandle);
}

void CHandleRegistration_PS::SetKeyHandle(HANDLE hProtocolStackHandle)
{
	m_hProtocolStackHandle = hProtocolStackHandle;
}

void CHandleRegistration_PS::Init(CProtocolStackBase* pProtocol, CInterfaceManagerBase* pManager, HANDLE hInterfaceHandle)
{
	if(pProtocol && pManager)
	{
		m_pProtocol = pProtocol->Clone();
		m_pInterfaceManager = pManager;
		m_hInterfaceHandle = hInterfaceHandle;
	}
}

void CHandleRegistration_PS::Init(CProtocolStackBase* pProtocolStack, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle)
{
	if(pProtocolStack && pManager)
	{
		m_pProtocol = pProtocolStack->Clone();
		m_pDeviceCommandSetManager = pManager;
		m_hDeviceCommandSetHandle = hDeviceCommandSetHandle;
	}
}

void CHandleRegistration_PS::DeleteRegistration()
{
	if(m_pProtocol)
	{
		delete m_pProtocol;
		m_pProtocol = 0;
	}
	m_hProtocolStackHandle = 0;

	//Interface Layer
	m_pInterfaceManager = 0;
	m_hInterfaceHandle = 0;

	//Device Command Set Layer
	m_pDeviceCommandSetManager = 0;
	m_hDeviceCommandSetHandle = 0;
}

BOOL CHandleRegistration_PS::DoLock(DWORD p_ulTimeout)
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

BOOL CHandleRegistration_PS::DoUnlock()
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

BOOL CHandleRegistration_PS::IsLocked()
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->IsLocked();
	}

	return oResult;
}

BOOL CHandleRegistration_PS::AbortLock(BOOL oActive)
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->AbortLock(oActive==TRUE) ? TRUE : FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_PS::GetTraceString(CStdString& p_rTraceString)
{
	BOOL oResult = TRUE;
#ifdef _DEBUG
	p_rTraceString.Format(_T("HandleRegistration PS 0x%.8x"), (unsigned int)m_hProtocolStackHandle);
#endif
    return oResult;
}
