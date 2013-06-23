#pragma once

#ifdef _WIN32
	#include "MmcRs232HndlWin.h"
#else
	#include "MmcRs232HndlLinux.h"
#endif

class CMmcRs232Hndl : public CMmcRs232HndlBase
{
public:
	CMmcRs232Hndl();
	virtual ~CMmcRs232Hndl();
};
