#include "stdafx.h"

#include <Log/MmcLogger.h>
#include "../BaseClasses/ProtocolStackBase.h"
#include "InterfacePlugInThread.h"
#include "InterfacePlugInManager.h"

CInterfacePlugInManager::CInterfacePlugInManager(void):
	m_pProtocolStack(0),
	m_pPlugInThread(0),
	m_ulNotificationDelay(100),
	m_lInstanceValue(0)
{
	CreatePlugInThread();
}

CInterfacePlugInManager::~CInterfacePlugInManager(void)
{
	DeletePlugInThread();
}

BOOL CInterfacePlugInManager::CreatePlugInThread()
{
	BOOL oResult = FALSE;

	DeletePlugInThread();
	if(!m_pPlugInThread)
	{
		m_pPlugInThread = new CInterfacePlugInThread();
		m_pPlugInThread->Init(this);
		oResult = TRUE;
	}

	return oResult;
}
	
BOOL CInterfacePlugInManager::DeletePlugInThread()
{
	BOOL oResult = FALSE;

	if(m_pPlugInThread)
	{
		if(StopPlugInMonitoring())
		{
			delete m_pPlugInThread;
			m_pPlugInThread = NULL;
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CInterfacePlugInManager::Init(CProtocolStackBase* p_pProtocolStack)
{
	BOOL oResult = TRUE;

	m_pProtocolStack = p_pProtocolStack;

	return oResult;
}

BOOL CInterfacePlugInManager::InitInstanceValue(int p_lInstanceValue)
{
	m_lInstanceValue = p_lInstanceValue;
	return TRUE;
}

BOOL CInterfacePlugInManager::RegisterInterface(CStdString p_InterfaceName)
{
	int iIndex = 0;
	BOOL oResult = FALSE;

	if(FindRegisteredInterface(p_InterfaceName,iIndex))
	{
		//Already registered
		oResult = TRUE;
	}
	else
	{
		//Add Interface
		m_RegisteredInterfaces.push_back(p_InterfaceName);
		
		//Start PlugInThread if first Interface
		oResult = CheckPlugInThread();
	}

	return oResult;
}
	
BOOL CInterfacePlugInManager::UnregisterInterface(CStdString p_InterfaceName)
{
	int iIndex = 0;
	BOOL oResult = FALSE;

	if(FindRegisteredInterface(p_InterfaceName,iIndex))
	{
		//Remove Interface
		m_RegisteredInterfaces.erase(m_RegisteredInterfaces.begin()+iIndex);
		
		//Stop PlugInThread if last Interface
		oResult = CheckPlugInThread();
	}

	return oResult;
}

BOOL CInterfacePlugInManager::FindRegisteredInterface(CStdString p_InterfaceName,int& p_riIndex)
{
	int iIndex = 0;
	BOOL oResult = FALSE;

	for(std::size_t i = 0;i < m_RegisteredInterfaces.size();i++)
	{
		if(p_InterfaceName==m_RegisteredInterfaces.at(i))
		{
			p_riIndex = (int)i;
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}

BOOL CInterfacePlugInManager::StartPlugInMonitoring()
{
	BOOL oResult = FALSE;

	if(m_pPlugInThread)
	{
		if(!m_pPlugInThread->IsRunning())
		{
			if(m_pPlugInThread->CreatePlugInWindow())
			{
				//Start
				oResult = m_pPlugInThread->Start();
			}
		}
		else
		{
			//Already Running
			oResult = TRUE;
		}
	}

	return oResult;
}
	
BOOL CInterfacePlugInManager::StopPlugInMonitoring()
{
	BOOL oResult = FALSE;

	if(m_pPlugInThread)
	{
		if(m_pPlugInThread->IsRunning())
		{
			//Stop
			if(m_pPlugInThread->Stop())
			{
				oResult = m_pPlugInThread->DestroyPlugInWindow();
			}
		}
		else
		{
			//Not Running
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CInterfacePlugInManager::CheckPlugInThread()
{
	BOOL oResult = FALSE;

	if(m_pPlugInThread)
	{
		if(m_RegisteredInterfaces.size() > 0)
		{
			//Start Monitoring
			oResult = StartPlugInMonitoring();
		}
		else
		{
			//Stop Monitoring
			oResult = StopPlugInMonitoring();
		}
	}

	return oResult;
}

BOOL CInterfacePlugInManager::UpdateDevice()
{
	CStdString interfaceName = _T("");
	BOOL oResult = TRUE;

	for(size_t i = 0; i < m_RegisteredInterfaces.size();i++)
	{
		interfaceName = m_RegisteredInterfaces.at(i);
		oResult = UpdateDevice(interfaceName);
	}

	return oResult;
}

BOOL CInterfacePlugInManager::WriteToLogFile(CStdString p_Action)
{
	return CMmcLogger::Commit(m_lInstanceValue, p_Action);
}

BOOL CInterfacePlugInManager::UpdateDevice(CStdString p_InterfaceName)
{
	BOOL oResult = FALSE;

	if(m_pProtocolStack)
	{
		oResult = m_pProtocolStack->UpdateProtocolStack(p_InterfaceName);
	}
	
	return oResult;
}

BOOL CInterfacePlugInManager::SetNotificationDelay(DWORD p_ulNotificationDelay)
{
	BOOL oResult = FALSE;

	m_ulNotificationDelay = p_ulNotificationDelay;

	return oResult;
}
	
BOOL CInterfacePlugInManager::GetNotificationDelay(DWORD& p_rulNotificationDelay)
{
	BOOL oResult = FALSE;

	p_rulNotificationDelay = m_ulNotificationDelay;

	return oResult;
}
