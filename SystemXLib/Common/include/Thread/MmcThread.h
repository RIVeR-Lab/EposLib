// ThreadBase.h: interface for the CThreadBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADBASE_H__75B786E3_6FE1_11D2_A07F_00001C3081B5__INCLUDED_)
#define AFX_THREADBASE_H__75B786E3_6FE1_11D2_A07F_00001C3081B5__INCLUDED_

#ifdef _WIN32
#include <Thread/MmcThreadWin.h>
typedef CMmcThreadWin CMmcThreadCP;
#else
#include <Thread/MmcThreadPosix.h>
typedef CMmcThreadPosix CMmcThreadCP;
#endif

class CMmcThread : public CMmcThreadCP
{
public:
	CMmcThread();
	virtual ~CMmcThread();
};

#endif // !defined(AFX_THREADBASE_H__75B786E3_6FE1_11D2_A07F_00001C3081B5__INCLUDED_)
