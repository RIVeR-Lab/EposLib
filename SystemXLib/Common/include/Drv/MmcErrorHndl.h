#ifndef __MMC_ERROR_HNDL__H
#define __MMC_ERROR_HNDL__H

enum EDrvErrorCode
{
	DEC_NOERROR,
	DEC_LIBRARY_NOT_LOADED,
	DEC_PORT_NOT_OPENED,
	DEC_SET_PORT_MODE_FAILED,
	DEC_READ_DATA_FAILED,
	DEC_WRITE_DATA_FAILED
};

class CMmcErrorHndl
{
public:
	CMmcErrorHndl()
	{
		m_eLastError = DEC_NOERROR;
	}
	EDrvErrorCode GetLastError()
	{
		return m_eLastError;
	}
protected:
	EDrvErrorCode m_eLastError;
};

#endif //__MMC_ERROR_HNDL__H