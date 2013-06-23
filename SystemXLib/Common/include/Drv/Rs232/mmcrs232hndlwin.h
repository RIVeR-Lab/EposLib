#pragma once

#include <afx.h>
#define SS_ANSI
#include <drv/rs232/MmcRs232HndlDef.h>

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
	HANDLE	m_Handle;
	DWORD	m_dTimeout;
	unsigned short m_wPortMode;
	unsigned long m_dBaudrate;
protected:
	unsigned long GetIntervalTimeout(unsigned long dTimeout);
	void ConvertPortName(CStdString portName, TCHAR *winApiPortName, DWORD bufferSize);
};