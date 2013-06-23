/*
 * MmcProcess.h
 *
 *  Created on: Oct 20, 2010
 *      Author: dawid
 */

#ifndef MMCPROCESS_H_
#define MMCPROCESS_H_

#include <StdString.h>

unsigned long MmcGetCurrentProcessId();
unsigned long MmcGetCurrentThreadId();
unsigned long MmcGetTickCount();
void		  MmcSleep(unsigned int milliseconds);
CStdString	  MmcGetModuleName();
bool		  MmcIsProcessAvailable(unsigned long pid);
unsigned long MmcGetLastError();

#endif /* MMCPROCESS_H_ */
