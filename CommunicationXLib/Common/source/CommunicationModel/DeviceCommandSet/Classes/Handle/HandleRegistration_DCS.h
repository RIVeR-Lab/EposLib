// HandleRegistration_DCS.h: Schnittstelle f�r die Klasse CHandleRegistration_DCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLEREGISTRATION_DCS_H__B81820C9_FF4F_4E77_ADD0_CD741B8CC5EF__INCLUDED_)
#define AFX_HANDLEREGISTRATION_DCS_H__B81820C9_FF4F_4E77_ADD0_CD741B8CC5EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDeviceBase;
class CMmcCriticalSectionEx;

class CHandleRegistration_DCS
{
public:
	CHandleRegistration_DCS();
	virtual ~CHandleRegistration_DCS();
	BOOL IsEqual(CDeviceBase* pDevice,CProtocolStackManagerBase* pManager,HANDLE hProtocolStackHandle);
	BOOL IsEmpty();

	void SetKeyHandle(HANDLE hDeviceCommandSetHandle);
	void Init(CDeviceBase* pDevice,CProtocolStackManagerBase* pManager,HANDLE phProtocolStackHandle);
	void DeleteRegistration();

	BOOL GetRegistrationValues(CDeviceBase** ppDevice,CProtocolStackManagerBase** ppManager,HANDLE* phProtocolStackHandle);
	HANDLE GetKeyHandle();

	//Critical Section
    BOOL DoLock(DWORD p_ulTimeout = -1);
    BOOL DoUnlock();
    BOOL IsLocked();
	bool AbortLock(bool oActive);

private:
	BOOL CreateCriticalSection();
	BOOL DeleteCriticalSection();

	BOOL GetTraceString(CStdString& p_rTraceString);

private:
	HANDLE m_Handle_DCS;
	CDeviceBase* m_pDevice;
	CProtocolStackManagerBase* m_pManager;
	HANDLE m_Handle_PS;
	CMmcCriticalSectionEx* m_pCriticalSection;
};

#endif // !defined(AFX_HANDLEREGISTRATION_DCS_H__B81820C9_FF4F_4E77_ADD0_CD741B8CC5EF__INCLUDED_)
