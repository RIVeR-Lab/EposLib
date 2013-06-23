#pragma once

#include <MmcTypeDefinition.h>

class CProtocolStackBase;
class CInterfacePlugInThread;

class CInterfacePlugInManager
{
public:
	CInterfacePlugInManager(void);
	~CInterfacePlugInManager(void);
	BOOL Init(CProtocolStackBase* p_pProtocolStack);
	BOOL InitInstanceValue(int p_lInstanceValue);
	
	//NotificationDelay
	BOOL SetNotificationDelay(DWORD p_ulNotificationDelay);
	BOOL GetNotificationDelay(DWORD& p_rulNotificationDelay);
	
	//Register PlugIn Interfaces
	BOOL RegisterInterface(CStdString p_InterfaceName);
	BOOL UnregisterInterface(CStdString p_InterfaceName);

	//Update
	BOOL UpdateDevice();

	//Logging
	BOOL WriteToLogFile(CStdString p_Action);

private:
	BOOL CheckPlugInThread();
	BOOL FindRegisteredInterface(CStdString p_InterfaceName,int& p_riIndex);

	//Create
	BOOL CreatePlugInThread();
	BOOL DeletePlugInThread();

	//Start and Stop
	BOOL StartPlugInMonitoring();
	BOOL StopPlugInMonitoring();

	//Update
	BOOL UpdateDevice(CStdString p_InterfaceName);

private:
	CProtocolStackBase* m_pProtocolStack;
	CStdStringArray m_RegisteredInterfaces;
	CInterfacePlugInThread* m_pPlugInThread;
	DWORD m_ulNotificationDelay;
	int m_lInstanceValue;
};
