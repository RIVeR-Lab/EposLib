/*
 * ThreadBaseWin.h
 *
 *  Created on: 12.10.2010
 *      Author: dawid
 */

#ifndef THREADBASEWIN_H_
#define THREADBASEWIN_H_

#include <afxmt.h>
#include "MmcCriticalSection.h"
#include "MmcThreadBase.h"

class CMmcThreadWin : public CMmcThreadBase
{
public:
	CMmcThreadWin();
	virtual ~CMmcThreadWin();

	//Start Stop
	int Start();
	int Stop();
	int IsRunning();

	//Thread Function
	virtual unsigned int WINAPI ThreadFunc();

protected:
	int SetHighPriority();
	int IsStillActive();

private:
	static unsigned __stdcall BaseThreadFunc(LPVOID pParam);

private:
	void* m_thread;
	unsigned int m_ThreadID;
};

#endif /* THREADBASEWIN_H_ */
