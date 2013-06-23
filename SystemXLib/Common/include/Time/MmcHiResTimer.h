//////////////////////////////////////////////////////////////////////////////
// MmcHiResTimer.h
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
//  MOD: Dawid Sienkiewicz / maxon motor ag
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#ifndef MMCTIMER_H_DEF
#define MMCTIMER_H_DEF

#ifdef _WIN32
#include <time/MmcHiResTimerWin.h>
typedef CMmcHiResTimerWin CMmcHiResTimerCP;
#else
#include <Time/MmcHiResTimerLinux.h>
typedef CMmcHiResTimerLinux CMmcHiResTimerCP;
#endif

class CMmcHiResTimer : public CMmcHiResTimerCP
{
public:
	// default constructor
	CMmcHiResTimer() {}
	// default destructor
    virtual ~CMmcHiResTimer() {}
};

#endif // MMCTIMER_H_DEF
