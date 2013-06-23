// HandleRegistration_I.cpp: Implementierung der Klasse CHandleRegistration_I.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <stdio.h>

#include "../Interfaces/BaseClasses/InterfaceBase.h"
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>
#include <Thread/MmcCriticalSection.h>

#include "HandleRegistration_I.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CHandleRegistration_I::CHandleRegistration_I()
{
	m_hInterfaceHandle = 0;
	m_pInterface = NULL;
	m_pPort = NULL;
	m_pCriticalSection = NULL;

	CreateCriticalSection();
}

CHandleRegistration_I::~CHandleRegistration_I()
{
	DeleteCriticalSection();
}

BOOL CHandleRegistration_I::CreateCriticalSection()
{
	BOOL oResult = FALSE;

	if(!m_pCriticalSection)
	{
		m_pCriticalSection = new CMmcCriticalSectionEx();
		oResult = TRUE;
	}

	return oResult;
}
	
BOOL CHandleRegistration_I::DeleteCriticalSection()
{
	BOOL oResult = FALSE;
	if(m_pCriticalSection)
	{
		delete m_pCriticalSection;
		m_pCriticalSection = NULL;
		oResult = TRUE;
	}

	return oResult;
}

/********************************************************************/
HANDLE CHandleRegistration_I::GetKeyHandle()
{
	return m_hInterfaceHandle;
}

/********************************************************************/
BOOL CHandleRegistration_I::GetRegistrationValues(CInterfaceBase** ppInterface,CPortBase** ppPort)
{
	if(ppInterface) *ppInterface = m_pInterface;
	if(ppPort) *ppPort = m_pPort;

	return TRUE;
}

/********************************************************************/
BOOL CHandleRegistration_I::IsEqual(CInterfaceBase* pInterface, CStdString p_PortName)
{
	CStdString portName = _T("");
	BOOL oResult = FALSE;

	if((m_pInterface && pInterface) && m_pPort)
	{
		//Init
		oResult = TRUE;

		//Interface
		if(oResult && !m_pInterface->IsEqual(pInterface)) oResult = FALSE;

		//Port
		if(oResult && !m_pPort->IsEqual(p_PortName)) oResult = FALSE;
	}

	return oResult;
}

BOOL CHandleRegistration_I::IsEqual(CInterfaceBase* pInterface)
{
	BOOL oResult = FALSE;

	if(m_pInterface && pInterface)
	{
		//Init
		oResult = TRUE;

		//Interface
		if(oResult && !m_pInterface->IsEqual(pInterface)) oResult = FALSE;
	}

	return oResult;
}


/********************************************************************/
BOOL CHandleRegistration_I::IsEmpty()
{
	return ((m_pInterface == NULL) || (m_pPort == NULL));
}

/********************************************************************/
void CHandleRegistration_I::SetKeyHandle(HANDLE hInterfaceHandle)
{
	m_hInterfaceHandle = hInterfaceHandle;
}

/********************************************************************/
void CHandleRegistration_I::Init(CInterfaceBase* pInterface,CPortBase* pPort)
{
	if(pInterface && pPort)
	{
		m_pInterface = pInterface->Clone();
		m_pPort = pPort->Clone();
	}
}

/********************************************************************/
void CHandleRegistration_I::DeleteRegistration()
{
	if(m_pInterface)
		delete m_pInterface;
	m_pInterface = NULL;
	if(m_pPort)
		delete m_pPort;
	m_pPort = NULL;
	m_hInterfaceHandle = 0;
}


BOOL CHandleRegistration_I::DoLock(DWORD p_ulTimeout)
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
    
BOOL CHandleRegistration_I::DoUnlock()
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

BOOL CHandleRegistration_I::IsLocked()
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->IsLocked();
	}
	
	return oResult;
}

BOOL CHandleRegistration_I::AbortLock(BOOL oActive)
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->AbortLock(oActive==TRUE) ? TRUE : FALSE;
	}
	
	return oResult;
}

BOOL CHandleRegistration_I::GetTraceString(CStdString& p_rTraceString)
{
	BOOL oResult = TRUE;

#ifdef _DEBUG
	p_rTraceString.Format(_T("HandleRegistration I 0x%.8x"),(unsigned int)m_hInterfaceHandle);
#endif

    return oResult;
}
