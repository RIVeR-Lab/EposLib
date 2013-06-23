#pragma once

#include <StdString.h>
#include <vector>
#include "../MmcErrorHndl.h"

using namespace std;

#define	INPUT_BUFFER_SIZE	1200
#define	OUTPUT_BUFFER_SIZE	1200

//RS232 Baudrate
#define k_9600Baud			9600
#define k_14400Baud			14400
#define k_19200Baud			19200
#define k_38400Baud			38400
#define k_57600Baud			57600
#define k_115200Baud		115200

class CMmcRs232HndlDef : public CMmcErrorHndl
{
	virtual vector<CStdString> InitPort() = 0;
	virtual bool OpenPort(CStdString strPortName) = 0;
	virtual bool ClosePort() = 0;
	virtual bool SetPortSettings(unsigned int dBaudrate, unsigned int dTimeout, bool oChangeOnly) = 0;
	virtual bool SetPortMode(unsigned short wPortMode) = 0;
	virtual bool SetTimeout(unsigned int dTimeout) = 0;
	virtual bool Purge() = 0;
	virtual bool Write(unsigned char* pDataBuffer, unsigned int nBytesToWrite, unsigned int *pBytesWritten) = 0;
	virtual bool Read(unsigned char* pDataBuffer, unsigned int nBytesToRead, unsigned int *pBytesRead) = 0;
	
protected:
	bool m_oFirstInit;
};
