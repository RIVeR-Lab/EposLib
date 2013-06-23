/*
 * ThreadBaseCP.h
 *
 *  Created on: 12.10.2010
 *      Author: dawid
 */

#ifndef THREADBASECP_H_
#define THREADBASECP_H_

#include <Thread/MmcCriticalSection.h>

class CMmcThreadBase
{
public:
	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual int IsRunning() = 0;

protected:
	virtual int SetHighPriority() = 0;
	virtual int IsStillActive() = 0;

protected:
	int					m_Running;
	CMmcCriticalSection	m_Sync;
};

#endif /* THREADBASECP_H_ */
