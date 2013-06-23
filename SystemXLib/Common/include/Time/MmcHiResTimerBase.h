//////////////////////////////////////////////////////////////////////////////
// MmcHiResTimerBase.h
//////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H_BASE_DEF
#define TIMER_H_BASE_DEF

class CMmcHiResTimerBase
{
public:
	CMmcHiResTimerBase();                        // default constructor
    virtual ~CMmcHiResTimerBase();               // default destructor

    virtual double start()  = 0;                            // start timer
    virtual void   stop() = 0;                              // stop the timer
	virtual void   reset() = 0;                             // reset the timer
    virtual double getElapsedTime() = 0;                    // get elapsed time in second
    virtual double getElapsedTimeInSec() = 0;               // get elapsed time in second (same as getElapsedTime)
    virtual double getElapsedTimeInMilliSec() = 0;          // get elapsed time in milli-second
    virtual double getElapsedTimeInMicroSec() = 0;// get elapsed time in micro-second

protected:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
    int    stopped;                             // stop flag 
};

#endif // TIMER_H_BASE_DEF
