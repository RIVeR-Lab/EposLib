#pragma once

#include "MmcRs232HndlDef.h"

class CMmcRs232HndlBase : public CMmcRs232HndlDef
{
public:
	CMmcRs232HndlBase();
	virtual ~CMmcRs232HndlBase();

public:
	vector<CStdString> InitPort();
	bool OpenPort(CStdString strPortName);
	bool ClosePort();
	bool SetPortSettings(unsigned int dBaudrate, unsigned int dTimeout, bool oChangeOnly);
	bool SetPortMode(unsigned short wPortMode);
	bool SetTimeout(unsigned int dTimeout);
	bool Purge();
	bool Write(unsigned char* pDataBuffer, unsigned int nBytesToWrite, unsigned int *pBytesWritten);
	bool Read(unsigned char* pDataBuffer, unsigned int nBytesToRead, unsigned int *pBytesRead);

private:
	unsigned long	m_dTimeout;
	unsigned short	m_wPortMode;
	unsigned long	m_dBaudrate;

private:
	int m_Handle;
};
