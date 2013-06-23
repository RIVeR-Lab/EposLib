#pragma once

#include <Drv/Usb/linux/ftd2xx.h>
#include "MmcUsbHndlDef.h"

class CMmcUsbHndlBase : public CMmcUsbHndlDef
{
public:
	CMmcUsbHndlBase();
	virtual ~CMmcUsbHndlBase();

public:
	bool Open(unsigned int ulLocationId, CStdString p_SerialNumber);
	bool Close();
	bool Reload();
	bool Configure();
	bool SetTimeouts(unsigned int dReadTimeout, unsigned int dWriteTimeout);
	bool SetBaudrate(unsigned int ulBaudrate);
	bool GetStatus(unsigned int *pAmountInRxQueue, unsigned int *pAmountInTxQueue, unsigned int *pEventStatus);
	bool ReadByte( unsigned char *pData, unsigned int *pDataRead);
	bool Read(void *pDataBuffer, unsigned int dNumberOfBytesToRead, unsigned int *pdNumberOfBytesRead);
	bool Write(void *pDataBuffer, unsigned int dNumberOfBytesToWrite, unsigned int *pdNumberOfBytesWritten);

	bool EE_Program(	unsigned short p_usVendorId, 
						unsigned short p_usProductId, 
						TCHAR* p_pManufacturer, 
						unsigned int p_ulManufacturerLength,
						TCHAR* p_pDescription, 
						unsigned int p_ulDescriptionLength,
						TCHAR* p_pSerialNumber, 
						unsigned int p_ulSerialNumberLength,
						bool p_oUseVcpDriver);

	bool EE_Read(	unsigned short& p_rusVendorId, 
					unsigned short& p_rusProductId, 
					TCHAR*& p_rpManufacturer, 
					unsigned int& p_rulManufacturerLength,
					TCHAR*& p_rpDescription, 
					unsigned int& p_rulDescriptionLength,
					TCHAR*& p_rpSerialNumber, 
					unsigned int& p_rulSerialNumberLength,
					bool& p_roUseVcpDriver);
	bool Rescan();
	bool Reload(unsigned short usVendorId, unsigned short usProductId);
	bool GetQueueStatus(unsigned int *pdAmountRxQueue);
	bool CreateDeviceInfoList(unsigned int *pdNbOfDevices);
	bool GetDeviceInfoDetail(unsigned int dDeviceIndex, unsigned int *pFlags, unsigned int *pType, unsigned int *pID, unsigned int *pLocId, char* strSerialNumberBuffer, char* strDescriptionBuffer);
	CStdString GetFtStatusDescription(unsigned long status);

private:
	bool AreLibraryFunctionsLoaded() { return true; }

private:
	FT_HANDLE m_Handle;
};
