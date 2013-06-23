#pragma once

#ifdef _WIN32
	#include "MmcUsbHndlWin.h"
#else
	#include "MmcUsbHndlLinux.h"
#endif

class CMmcUsbHndl : public CMmcUsbHndlBase
{
public:
	CMmcUsbHndl();
	virtual ~CMmcUsbHndl();
};
