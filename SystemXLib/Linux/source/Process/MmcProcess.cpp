/*
 * MmcProcess.cpp
 *
 *  Created on: Oct 20, 2010
 *      Author: dawid
 */
#include <Process/MmcProcess.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>

unsigned long MmcGetLastError()
{
	return 0;
}

bool	MmcIsProcessAvailable(unsigned long p_ulProcessId)
{
	CStdString	fileName;
	bool		active = false;

	fileName.Format("/proc/%ld", p_ulProcessId);

	FILE* file = fopen(fileName, "r");

	if( file != 0 )
	{
		active = true;
		fclose(file);
	}

	return active;
}

unsigned long MmcGetTickCount()
{
	struct timeval tv;
	if(gettimeofday(&tv, NULL) != 0)
			return 0;

	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void MmcSleep(unsigned int microseconds)
{
	clock_t tick1=-1, tick2=-1;
	double dClock = -1;
	struct tms tStruct1, tStruct2;

	if((tick1 = times(&tStruct1))== -1) printf("Error time 1\n");
	usleep(microseconds);
	if((tick2 = times(&tStruct2))== -1) printf("Error time 2\n");
	if((dClock = sysconf(_SC_CLK_TCK)) < 0) printf("Error sysconf \n");

	return;
}

unsigned long MmcGetCurrentProcessId()
{
	return getpid();
}

unsigned long MmcGetCurrentThreadId()
{
	return pthread_self();
}

CStdString MmcGetModuleName()
{
	return "MmcSysLib";
}
