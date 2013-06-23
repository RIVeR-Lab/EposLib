// HandleRegistration_VCS.h: Schnittstelle f�r die Klasse CHandleRegistration_VCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HandleRegistration_VCS_H__D4E70BD0_EC18_48DD_A3BF_573DF4D4B365__INCLUDED_)
#define AFX_HandleRegistration_VCS_H__D4E70BD0_EC18_48DD_A3BF_573DF4D4B365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <Thread/MmcCriticalSection.h>

class CDeviceCommandSetManagerBase;
class CVirtualDeviceBase;

class CHandleRegistration_VCS
{
public:
	CHandleRegistration_VCS();
	virtual ~CHandleRegistration_VCS();
	BOOL IsEqual(CVirtualDeviceBase* pVirtualDevice,CDeviceCommandSetManagerBase* pManager,HANDLE hDeviceCommandSetHandle);
	BOOL IsEmpty();

	void SetKeyHandle(HANDLE hVCS_Handle);
	void Init(CVirtualDeviceBase* pVirtualDevice,CDeviceCommandSetManagerBase* pManager,HANDLE hDeviceCommandSetHandle);
	void DeleteRegistration();

	BOOL GetRegistrationValues(CVirtualDeviceBase** ppVirtualDevice,CDeviceCommandSetManagerBase** ppManager,HANDLE* phDeviceCommandSetHandle);
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
	HANDLE m_Handle_VCS;
	CVirtualDeviceBase* m_pVirtualDevice;
	CDeviceCommandSetManagerBase* m_pManager;
	HANDLE m_Handle_DCS;
	
	CMmcCriticalSectionEx* m_pCriticalSection;
};

#endif // !defined(AFX_HandleRegistration_VCS_H__D4E70BD0_EC18_48DD_A3BF_573DF4D4B365__INCLUDED_)
