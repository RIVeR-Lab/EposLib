//////////////////////////////////////////////////////////////////////////////
// MmcHiResTimer.h
//////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H_WIN_DEF
#define TIMER_H_WIN_DEF

#include <windows.h>
#include <time/MmcHiResTimerBase.h>

class CMmcHiResTimerWin : public CMmcHiResTimerBase
{
public:
	CMmcHiResTimerWin();                        // default constructor
    virtual ~CMmcHiResTimerWin();               // default destructor

	double start();                             // start timer
    void   stop();                              // stop the timer
	void   reset();                             // reset the timer
    double getElapsedTime();                    // get elapsed time in second
    double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
    double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
    double getElapsedTimeInMicroSec();// get elapsed time in micro-second
	
protected:
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
};

#endif // TIMER_H_WIN_DEF
