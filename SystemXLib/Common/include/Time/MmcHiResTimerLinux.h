//////////////////////////////////////////////////////////////////////////////
// MmcHiResTimer.h
//////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H_LINUX_DEF
#define TIMER_H_LINUX_DEF

#include <sys/time.h>
#include <Time/MmcHiResTimerBase.h>

class CMmcHiResTimerLinux : public CMmcHiResTimerBase
{
public:
	CMmcHiResTimerLinux();                      // default constructor
    virtual ~CMmcHiResTimerLinux();             // default destructor

    double start();                             // start timer
	void   stop();                              // stop the timer
	void   reset();                             // reset the timer
	double getElapsedTime();                    // get elapsed time in second
	double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
	double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
	double getElapsedTimeInMicroSec();// get elapsed time in micro-second

protected:
    timeval startCount;                         //
    timeval endCount;                           //
};

#endif // TIMER_H_LINUX_DEF
