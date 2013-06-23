// InterfacePlugInThread.cpp: Implementierung der Klasse CInterfacePlugInThread.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "InterfacePlugInManager.h"
#include "InterfacePlugInWnd.h"
#include "InterfacePlugInThread.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CInterfacePlugInThread::CInterfacePlugInThread()
{
	m_pPlugInManager = NULL;
	m_pPlugInWnd = NULL;
}

CInterfacePlugInThread::~CInterfacePlugInThread()
{
	DestroyPlugInWindow();
}

BOOL CInterfacePlugInThread::Init(CInterfacePlugInManager* pPlugInManager)
{
	BOOL oResult = FALSE;

	m_pPlugInManager = pPlugInManager;

	return oResult;
}

BOOL CInterfacePlugInThread::CreatePlugInWindow()
{
	BOOL oResult = FALSE;

	DestroyPlugInWindow();

#ifdef WINVER
	if(!m_pPlugInWnd)
	{
		//Create Object
		m_pPlugInWnd = new CInterfacePlugInWnd();
		if(m_pPlugInWnd)
		{
			//Create Window
			if(m_pPlugInWnd->CreateEx(NULL,_T("STATIC"),_T("DevicePlugInWnd"),0,CRect(0,0,5,5),NULL,0))
			{
				oResult = TRUE;
			}
		}
	}
#endif

	return oResult;
}
	
BOOL CInterfacePlugInThread::DestroyPlugInWindow()
{
	BOOL oResult = FALSE;

#ifdef WINVER
	CHandleMap* pMap = NULL;

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	if(m_pPlugInWnd)
	{
		//Destroy Window
		if(m_pPlugInWnd->FromHandlePermanent(m_pPlugInWnd->m_hWnd))
		{
			m_pPlugInWnd->DestroyWindow();
		}

		//Delete Object
		delete m_pPlugInWnd;
		m_pPlugInWnd = NULL;
		oResult = TRUE;
	}
#endif

	return oResult;
}

unsigned int CInterfacePlugInThread::ThreadFunc()
{
#ifdef WINVER
	CInterfacePlugInWnd plugInWnd;
	MSG msg;
	
	//Check Invisble Windows
	if(m_pPlugInWnd)
	{
		//Init
		m_pPlugInWnd->Init(m_pPlugInManager);

		//Process Messages
		while(m_Running)
		{
			if(PeekMessage(&msg,m_pPlugInWnd->GetSafeHwnd(),0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			//Release Thread
			Sleep(100);

			//Check if Notification is pending
			m_pPlugInWnd->CheckNotification();
		}
	}

	m_Running = false;
#endif

	return 0;
}



