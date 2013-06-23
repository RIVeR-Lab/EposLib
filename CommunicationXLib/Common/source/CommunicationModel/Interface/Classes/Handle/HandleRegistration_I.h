// HandleRegistration_I.h: Schnittstelle f�r die Klasse CHandleRegistration_I.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLEREGISTRATION_I_H__6FA6D6E4_52E0_461C_A99A_5C6809B1E707__INCLUDED_)
#define AFX_HANDLEREGISTRATION_I_H__6FA6D6E4_52E0_461C_A99A_5C6809B1E707__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Interfaces/BaseClasses/InterfaceBase.h"

class CPortBase;
class CMmcCriticalSectionEx;

class CHandleRegistration_I
{
public:
	CHandleRegistration_I();
	virtual ~CHandleRegistration_I();
	BOOL IsEqual(CInterfaceBase* pInterface);
	BOOL IsEqual(CInterfaceBase* pInterface, CStdString p_PortName);
	BOOL IsEmpty();

	void SetKeyHandle(HANDLE hInterfaceHandle);
	void Init(CInterfaceBase* pInterface,CPortBase* pPort);
	void DeleteRegistration();

	BOOL GetRegistrationValues(CInterfaceBase** ppInterface,CPortBase** ppPort);
	HANDLE GetKeyHandle();

	//Critical Section
    BOOL DoLock(DWORD p_ulTimeout = -1);
    BOOL DoUnlock();
    BOOL IsLocked();
	BOOL AbortLock(BOOL oActive);

private:
	BOOL CreateCriticalSection();
	BOOL DeleteCriticalSection();

	BOOL GetTraceString(CStdString& p_rTraceString);

private:
	HANDLE m_hInterfaceHandle;
	CInterfaceBase* m_pInterface;
	CPortBase* m_pPort;
	
	CMmcCriticalSectionEx* m_pCriticalSection;
};

#endif // !defined(AFX_HANDLEREGISTRATION_I_H__6FA6D6E4_52E0_461C_A99A_5C6809B1E707__INCLUDED_)
