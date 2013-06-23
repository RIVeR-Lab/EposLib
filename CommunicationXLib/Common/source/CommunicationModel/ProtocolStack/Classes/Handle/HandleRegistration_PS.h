// HandleRegistration_PS.h: Schnittstelle f�r die Klasse CHandleRegistration_PS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLEREGISTRATION_PS_H__DA43FDA2_3993_4A74_A71C_75FD26B455F2__INCLUDED_)
#define AFX_HANDLEREGISTRATION_PS_H__DA43FDA2_3993_4A74_A71C_75FD26B455F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <Thread/MmcCriticalSection.h>

class CProtocolStackBase;
class CInterfaceManagerBase;
class CHandleRegistration_PS
{
public:
	CHandleRegistration_PS();
	virtual ~CHandleRegistration_PS();
	BOOL IsEqual(CProtocolStackBase* pProtocolStack, CInterfaceManagerBase* pManager, HANDLE hInterfaceHandle);
	BOOL IsEqual(CProtocolStackBase* pProtocolStack, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle);
	BOOL IsEmpty();
	BOOL IsBasedOnInterface();
	BOOL IsBasedOnDevice();

	void SetKeyHandle(HANDLE hProtocolStackHandle);
	void Init(CProtocolStackBase* pProtocolStack, CInterfaceManagerBase* pManager,HANDLE hInterfaceHandle);
	void Init(CProtocolStackBase* pProtocolStack, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle);
	void DeleteRegistration();

	BOOL GetRegistrationValues(CProtocolStackBase** ppProtocolStack, CInterfaceManagerBase** ppManager, HANDLE* phInterfaceHandle);
	BOOL GetRegistrationValues(CProtocolStackBase** ppProtocolStack, CDeviceCommandSetManagerBase** ppManager, HANDLE* phDeviceCommandSetHandle);
	BOOL GetRegistrationValues(CProtocolStackBase** ppProtocolStack, CLayerManagerBase** ppManager, HANDLE* phHandle);
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
	HANDLE m_hProtocolStackHandle;
	CProtocolStackBase* m_pProtocol;
	
	//Interface Layer
	CInterfaceManagerBase* m_pInterfaceManager;
	HANDLE m_hInterfaceHandle;

	//DeviceCommandSet Layer
	CDeviceCommandSetManagerBase* m_pDeviceCommandSetManager;
	HANDLE m_hDeviceCommandSetHandle;
	CMmcCriticalSectionEx* m_pCriticalSection;
};

#endif // !defined(AFX_HANDLEREGISTRATION_PS_H__DA43FDA2_3993_4A74_A71C_75FD26B455F2__INCLUDED_)
