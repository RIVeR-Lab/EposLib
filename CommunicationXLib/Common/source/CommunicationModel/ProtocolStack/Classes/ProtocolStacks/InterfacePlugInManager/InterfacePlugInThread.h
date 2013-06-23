// InterfacePlugInThread.h: Schnittstelle f�r die Klasse CInterfacePlugInThread.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_InterfacePlugInThread_H__A1888084_8F67_4C84_926A_B394A75720BF__INCLUDED_)
#define AFX_InterfacePlugInThread_H__A1888084_8F67_4C84_926A_B394A75720BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Thread/MmcThread.h>

class CInterfacePlugInManager;
class CInterfacePlugInWnd;

class CInterfacePlugInThread : public CMmcThread
{
public:
	CInterfacePlugInThread();
	virtual ~CInterfacePlugInThread();
	BOOL Init(CInterfacePlugInManager* pPlugInManager);

	BOOL CreatePlugInWindow();
	BOOL DestroyPlugInWindow();

	unsigned int WINAPI ThreadFunc();

private:
	CInterfacePlugInManager* m_pPlugInManager;
	CInterfacePlugInWnd* m_pPlugInWnd;
};

#endif // !defined(AFX_InterfacePlugInThread_H__A1888084_8F67_4C84_926A_B394A75720BF__INCLUDED_)
