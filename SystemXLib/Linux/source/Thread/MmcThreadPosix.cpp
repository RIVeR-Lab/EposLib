/*
 * ThreadBasePosix.cpp
 *
 *  Created on: 12.10.2010
 *      Author: dawid
 */
#include <Thread/MmcThreadPosix.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMmcThreadPosix::CMmcThreadPosix()
{
	m_Running = 0;
	m_thread = 0;
	m_ThreadID = 0;
}

CMmcThreadPosix::~CMmcThreadPosix()
{
	Stop();
}

unsigned int CMmcThreadPosix::ThreadFunc()
{
	while(m_Running)
	{
		//TODO
	}

	m_Running = false;
	return -1;
}

void* CMmcThreadPosix::BaseThreadFunc(void *pParam)
{
	CMmcThreadPosix* pThread = reinterpret_cast<CMmcThreadPosix*>(pParam);
   unsigned int iResult = -1;

	if(pThread)
	{
		pThread->m_ThreadID = pthread_self();

		//Wait until m_Running = TRUE
		CMmcSingleLock lock(&(pThread->m_Sync),false);
		lock.Lock();
		lock.Unlock();

   	   //Thread Func
   	   iResult = pThread?pThread->ThreadFunc():-1;

   	   //Reset
   	   pThread->m_ThreadID = 0;
   	}

   pthread_exit(0);
}

int CMmcThreadPosix::Start()
{
	int rc = 1;

	if(!m_Running)
	{
		pthread_attr_t attr;

		pthread_mutex_init(reinterpret_cast<pthread_mutex_t*>(&m_Sync), NULL);

		/* Initialize and set thread detached attribute */
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		rc = pthread_create(&m_thread, NULL, BaseThreadFunc, (void *)this);

		/* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);
	}

	return rc==0;
}

int CMmcThreadPosix::Stop()
{
	int rc = 0;

	if( m_thread != 0 )
		rc = pthread_cancel(m_thread);

	pthread_mutex_destroy(reinterpret_cast<pthread_mutex_t*>(&m_Sync));

	return (rc==0);
}

int CMmcThreadPosix::IsStillActive()
{
	int oResult = IsRunning();

	return oResult;
}

int CMmcThreadPosix::IsRunning()
{
	int rc = ENOENT;

	if( m_thread != 0 )
		rc = pthread_tryjoin_np(m_thread, NULL);

	return ( rc == EBUSY );
}

int CMmcThreadPosix::SetHighPriority()
{
	int which = PRIO_PROCESS;
	id_t pid;
	int priority = -20;
	int ret;

	pid = getpid();
	ret = setpriority(which, pid, priority);

	return ret==0;
}


