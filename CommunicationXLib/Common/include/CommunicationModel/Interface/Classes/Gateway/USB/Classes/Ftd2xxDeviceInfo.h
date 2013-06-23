#pragma once

#include <MmcTypeDefinition.h>

class CFtd2xxDeviceInfo
{
public:
	CFtd2xxDeviceInfo(void);
	~CFtd2xxDeviceInfo(void);
	CFtd2xxDeviceInfo& operator=(CFtd2xxDeviceInfo& other);
	CFtd2xxDeviceInfo* Clone();

	BOOL Init(DWORD p_ulLocId,CStdString p_SerialNumber, DWORD p_ulFlags);
	BOOL Init(CFtd2xxDeviceInfo& p_rDeviceInfo);
	BOOL Update(CFtd2xxDeviceInfo& p_rDeviceInfo);
	BOOL SetPortName(CStdString p_PortName);
	BOOL SetOpenedByProcess(DWORD p_ulOpenedByProcess);
	BOOL SetOpenedByInstance(DWORD p_ulOpenedByInstance);
	BOOL SetOpenedBy(CStdString p_ulOpenedBy);
	BOOL SetPluggedIn(DWORD p_ulPluggedIn);
	BOOL GetPortName(CStdString& p_rPortName);
	DWORD GetLocId();
	DWORD GetDrvFlags();
	DWORD GetOpenedByProcess();
	DWORD GetOpenedByInstance();
	CStdString GetOpenedBy();
	DWORD GetPluggedIn();
	CStdString GetSerialNumber();
	CStdString GetPortName();

	BOOL IsEqual(CFtd2xxDeviceInfo& p_rDeviceInfo);
	BOOL IsLocIdEqual(CFtd2xxDeviceInfo& p_rDeviceInfo);
	BOOL IsSerialNumberEqual(CFtd2xxDeviceInfo& p_rDeviceInfo);
	BOOL IsPortNameEqual(CStdString p_PortName);
	BOOL IsPluggedIn();
	BOOL IsOpened();
	BOOL IsOpenedBy(DWORD p_ulProcessId, DWORD m_ulInstanceValue);

private:
	//Driver Infos
	DWORD m_ulLocationId;
	CStdString m_SerialNumber;
	DWORD m_ulDrvFlags;
	
	//Registry Enumeration Infos
	CStdString m_PortName;
	DWORD m_ulPluggedIn;
	DWORD m_ulOpenedByProcess;
	DWORD m_ulOpenedByInstance;
	CStdString m_OpenedBy;
};

