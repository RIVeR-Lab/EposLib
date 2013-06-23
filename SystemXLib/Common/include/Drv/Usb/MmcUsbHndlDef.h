#pragma once

#include <StdString.h>
#include "../MmcErrorHndl.h"

class CMmcUsbHndlDef : public CMmcErrorHndl
{
	virtual bool Open(unsigned int ulLocationId, CStdString p_SerialNumber) = 0;
	virtual bool Close() = 0;
	virtual bool Reload() = 0;
	virtual bool Configure() = 0;
	virtual bool SetTimeouts(unsigned int dReadTimeout, unsigned int dWriteTimeout) = 0;
	virtual bool SetBaudrate(unsigned int ulBaudrate) = 0;
	virtual bool GetStatus(unsigned int *pAmountInRxQueue, unsigned int *pAmountInTxQueue, unsigned int *pEventStatus) = 0;
	virtual bool ReadByte( unsigned char *pData, unsigned int *pDataRead) = 0;
	virtual bool Read(void *pDataBuffer, unsigned int dNumberOfBytesToRead, unsigned int *pdNumberOfBytesRead) = 0;
	virtual bool Write(void *pDataBuffer, unsigned int dNumberOfBytesToWrite, unsigned int *pdNumberOfBytesWritten) = 0;

	virtual bool EE_Program(	unsigned short p_usVendorId, 
								unsigned short p_usProductId, 
								TCHAR* p_pManufacturer, 
								unsigned int p_ulManufacturerLength,
								TCHAR* p_pDescription, 
								unsigned int p_ulDescriptionLength,
								TCHAR* p_pSerialNumber, 
								unsigned int p_ulSerialNumberLength,
								bool p_oUseVcpDriver) = 0;

	virtual bool EE_Read(	unsigned short& p_rusVendorId, 
							unsigned short& p_rusProductId, 
							TCHAR*& p_rpManufacturer, 
							unsigned int& p_rulManufacturerLength,
							TCHAR*& p_rpDescription, 
							unsigned int& p_rulDescriptionLength,
							TCHAR*& p_rpSerialNumber, 
							unsigned int& p_rulSerialNumberLength,
							bool& p_roUseVcpDriver) = 0;
	virtual bool Rescan() = 0;
	virtual bool Reload(unsigned short usVendorId, unsigned short usProductId) = 0;
	virtual bool GetQueueStatus(unsigned int *pdAmountRxQueue) = 0;
	virtual bool CreateDeviceInfoList(unsigned int *pdNbOfDevices) = 0;
	virtual bool GetDeviceInfoDetail(unsigned int dDeviceIndex, unsigned int *pFlags, unsigned int *pType, unsigned int *pID, unsigned int *pLocId, char* strSerialNumberBuffer, char* strDescriptionBuffer) = 0;
	virtual CStdString GetFtStatusDescription(unsigned long status) = 0;
};
