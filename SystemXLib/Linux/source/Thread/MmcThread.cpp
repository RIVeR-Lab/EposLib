// ThreadBase.cpp: implementation of the CThreadBase class.
//
//////////////////////////////////////////////////////////////////////
#include <Thread/MmcThread.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMmcThread::CMmcThread()
{
	m_Running = 0;
}

CMmcThread::~CMmcThread()
{
	Stop();
}
