/*
 * ThreadBasePosix.h
 *
 *  Created on: 12.10.2010
 *      Author: dawid
 */

#ifndef THREADBASEPOSIX_H_
#define THREADBASEPOSIX_H_

#include <pthread.h>
#include <errno.h>
#include <Thread/MmcCriticalSection.h>
#include <Thread/MmcThreadBase.h>

class CMmcThreadPosix : public CMmcThreadBase
{
public:
	CMmcThreadPosix();
	virtual ~CMmcThreadPosix();

public:
	virtual int Start();
	virtual int Stop();
	virtual int IsRunning();

	virtual unsigned int ThreadFunc();

protected:
	virtual int SetHighPriority();
	virtual int IsStillActive();

private:
	static void* BaseThreadFunc(void *threadid);

//private members
private:
	pthread_t			m_thread;
	unsigned long int	m_ThreadID;
};

#endif /* THREADBASEPOSIX_H_ */
