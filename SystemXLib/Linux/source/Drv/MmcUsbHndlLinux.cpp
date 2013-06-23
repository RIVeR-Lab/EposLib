#include <Drv/Usb/MmcUsbHndlLinux.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>

CMmcUsbHndlBase::CMmcUsbHndlBase()
{
	FT_SetVIDPID(0x403,0xa8b0);
}

CMmcUsbHndlBase::~CMmcUsbHndlBase()
{
}

CStdString CMmcUsbHndlBase::GetFtStatusDescription(unsigned long status)
{
	CStdString desc("Unknown");

	switch(status)
	{
		case FT_OK: desc = "FT_OK"; break;
		case FT_INVALID_HANDLE: desc = "FT_INVALID_HANDLE"; break;
		case FT_DEVICE_NOT_FOUND: desc = "FT_DEVICE_NOT_FOUND"; break;
		case FT_DEVICE_NOT_OPENED: desc = "FT_DEVICE_NOT_OPENED"; break;
		case FT_IO_ERROR: desc = "FT_IO_ERROR"; break;
		case FT_INSUFFICIENT_RESOURCES: desc = "FT_INSUFFICIENT_RESOURCES"; break;
		case FT_INVALID_PARAMETER: desc = "FT_INVALID_PARAMETER"; break;
		case FT_INVALID_BAUD_RATE: desc = "FT_INVALID_BAUD_RATE"; break;
		case FT_DEVICE_NOT_OPENED_FOR_ERASE: desc = "FT_DEVICE_NOT_OPENED_FOR_ERASE"; break;
		case FT_DEVICE_NOT_OPENED_FOR_WRITE: desc = "FT_DEVICE_NOT_OPENED_FOR_WRITE"; break;
		case FT_FAILED_TO_WRITE_DEVICE: desc = "FT_FAILED_TO_WRITE_DEVICE"; break;
		case FT_EEPROM_READ_FAILED: desc = "FT_EEPROM_READ_FAILED"; break;
		case FT_EEPROM_WRITE_FAILED: desc = "FT_EEPROM_WRITE_FAILED"; break;
		case FT_EEPROM_ERASE_FAILED: desc = "FT_EEPROM_ERASE_FAILED"; break;
		case FT_EEPROM_NOT_PRESENT: desc = "FT_EEPROM_NOT_PRESENT"; break;
		case FT_EEPROM_NOT_PROGRAMMED: desc = "FT_EEPROM_NOT_PROGRAMMED"; break;
		case FT_INVALID_ARGS: desc = "FT_INVALID_ARGS"; break;
		case FT_NOT_SUPPORTED: desc = "FT_NOT_SUPPORTED"; break;
		case FT_OTHER_ERROR: desc = "FT_OTHER_ERROR"; break;
		case FT_DEVICE_LIST_NOT_READY: desc = "FT_DEVICE_LIST_NOT_READY"; break;
		default: desc.Format("FT_Unknown - error code=%ld", status);
	}

	return desc;
}

bool CMmcUsbHndlBase::GetDeviceInfoDetail(unsigned int dDeviceIndex,
										  unsigned int *pFlags,
										  unsigned int *pType,
										  unsigned int *pID,
										  unsigned int *pLocId,
										  char* strSerialNumberBuffer,
										  char* strDescriptionBuffer)
{

	if( !AreLibraryFunctionsLoaded() )
    {
        perror("Library not loaded");
        return false;
    }

	FT_HANDLE ftHandleTemp;

	FT_STATUS ftStatus = FT_GetDeviceInfoDetail((DWORD)dDeviceIndex,
												(DWORD*)pFlags,
												(DWORD*)pType,
												(DWORD*)pID,
												(DWORD*)pLocId,
												strSerialNumberBuffer,
												strDescriptionBuffer,
												&ftHandleTemp);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_GetDeviceInfoDetail";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::CreateDeviceInfoList(unsigned int *pdNbOfDevices)
{

	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_CreateDeviceInfoList((DWORD*)pdNbOfDevices);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_CreateDeviceInfoList";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::GetQueueStatus(unsigned int *pdAmountRxQueue)
{

	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_GetQueueStatus(m_Handle, (DWORD*)pdAmountRxQueue);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_GetQueueStatus";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::Reload(unsigned short usVendorId, unsigned short usProductId)
{
	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }
	
	perror("Reload - not implemented");

	return true;
}

bool CMmcUsbHndlBase::Rescan()
{
	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	return true;
}

bool CMmcUsbHndlBase::Open(unsigned int ulLocationId, CStdString p_SerialNumber)
{
	if( !AreLibraryFunctionsLoaded() )
	{
		perror("Library not loaded");
		return false;
	}

	FT_STATUS ftStatus = FT_OpenEx((void*)p_SerialNumber.GetBuffer(), FT_OPEN_BY_SERIAL_NUMBER, &m_Handle);

	p_SerialNumber.ReleaseBuffer();

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_OpenEx";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::Reload()
{
	if( !AreLibraryFunctionsLoaded() )
	{
		perror("Library not loaded");
        return false;
    }

	return true;
}

bool CMmcUsbHndlBase::Configure()
{

	if( !AreLibraryFunctionsLoaded() )
	{
		perror("Library not loaded");
        return false;
    }

	bool oResult = true;
	const BYTE LATENCY_TIMER(2);
    const DWORD INPUT_TRANSFER_SIZE(128);
    const DWORD OUTPUT_TRANSFER_SIZE(128);
    FT_STATUS ftStatus = FT_OK;

	//Data Characteristics:ByteSize, StopBit, Parity
    if(FT_OK != (ftStatus = FT_SetDataCharacteristics(m_Handle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE)))
    {
    	std::string errormsg = GetFtStatusDescription(ftStatus);
    	errormsg += ":FT_SetDataCharacteristics";
    	perror(errormsg.c_str());
        oResult = false;
    }

    //FlowControl: Dtr, Rts, Xon, Xoff
    if(FT_OK != (ftStatus = FT_SetFlowControl(m_Handle, FT_FLOW_NONE, 0, 0)))
    {
    	std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_SetFlowControl";
		perror(errormsg.c_str());
        oResult = false;
    }

    //LatencyTimer
    if(FT_OK != (ftStatus = FT_SetLatencyTimer(m_Handle, LATENCY_TIMER)))
    {
    	std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_SetLatencyTimer";
		perror(errormsg.c_str());
        oResult = false;
    }

    //TransferSize
    if(FT_OK != (ftStatus = FT_SetUSBParameters(m_Handle, INPUT_TRANSFER_SIZE, OUTPUT_TRANSFER_SIZE)))
    {
    	std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_SetUSBParameters";
		perror(errormsg.c_str());
        oResult = false;
    }

	return oResult;
}

bool CMmcUsbHndlBase::Close()
{

	if( !AreLibraryFunctionsLoaded() )
	{
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_Close(m_Handle);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_Close";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::SetTimeouts(unsigned int dReadTimeout, unsigned int dWriteTimeout)
{

	if( !AreLibraryFunctionsLoaded() )
	{
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_SetTimeouts(m_Handle, dReadTimeout, dWriteTimeout);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_GetStatus";
		perror(errormsg.c_str());
	}


	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::SetBaudrate(unsigned int ulBaudrate)
{

	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_SetBaudRate(m_Handle, ulBaudrate);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_GetStatus";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::GetStatus(unsigned int *pAmountInRxQueue, unsigned int *pAmountInTxQueue, unsigned int *pEventStatus)
{

	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_GetStatus(m_Handle, (DWORD*)pAmountInRxQueue, (DWORD*)pAmountInTxQueue, (DWORD*)pEventStatus);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_GetStatus";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::ReadByte( unsigned char *pData, unsigned int *pDataRead)
{

	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_Read(m_Handle, pData, 1, (DWORD*)pDataRead);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_SetUSBParameters";
		perror(errormsg.c_str());
	}

	return (FT_OK == ftStatus);
}

bool CMmcUsbHndlBase::Write(void *pDataBuffer, unsigned int dNumberOfBytesToWrite, unsigned int *pdNumberOfBytesWritten)
{

	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_Write(m_Handle, pDataBuffer, dNumberOfBytesToWrite, (DWORD*)pdNumberOfBytesWritten);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_Write";
		perror(errormsg.c_str());
	}

	if( *pdNumberOfBytesWritten <= 0 )
	{
		perror("Write processed successfully, but number of bytes written is 0");
		return false;
	}

	return true;
}

bool CMmcUsbHndlBase::Read(void *pDataBuffer, unsigned int dNumberOfBytesToRead, unsigned int *pdNumberOfBytesRead)
{

	if( !AreLibraryFunctionsLoaded() )
    {
		perror("Library not loaded");
        return false;
    }

	FT_STATUS ftStatus = FT_Read(m_Handle, pDataBuffer, dNumberOfBytesToRead, (DWORD*)pdNumberOfBytesRead);

	if( ftStatus != FT_OK )
	{
		std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_Read";
		perror(errormsg.c_str());
		return false;
	}

	if( *pdNumberOfBytesRead <= 0 )
	{
		//perror("Read processed successfully, but number of read bytes is 0");
		return false;
	}

	return true;
}

bool CMmcUsbHndlBase::EE_Read(unsigned short& p_rusVendorId, 
							  unsigned short& p_rusProductId, 
							  char*& p_rpManufacturer, 
							  unsigned int& p_rulManufacturerLength,
							  char*& p_rpDescription, 
							  unsigned int& p_rulDescriptionLength,
							  char*& p_rpSerialNumber, 
							  unsigned int& p_rulSerialNumberLength,
							  bool& p_roUseVcpDriver)
{
    const int BUFFER_SIZE(64);

    FT_PROGRAM_DATA ftData;
    TCHAR ManufacturerBuf[BUFFER_SIZE];
    TCHAR ManufacturerIdBuf[BUFFER_SIZE];
    TCHAR DescriptionBuf[BUFFER_SIZE];
    TCHAR SerialNumberBuf[BUFFER_SIZE];
    BOOL oResult(FALSE);
    FT_STATUS ftStatus = FT_OK;

    //ResetErrorInfo
    //m_ErrorInfo.Reset();

    //Prepare Structure
    ftData.Signature1 = 0x00000000;
    ftData.Signature2 = 0xffffffff;
    ftData.Version = 0x00000002; // EEPROM structure with FT232R extensions
    ftData.Manufacturer = ManufacturerBuf;
    ftData.ManufacturerId = ManufacturerIdBuf;
    ftData.Description = DescriptionBuf;
    ftData.SerialNumber = SerialNumberBuf;

    if(FT_OK == (ftStatus = FT_EE_Read(m_Handle, &ftData)))
    {
        //VendorId
        p_rusVendorId = ftData.VendorId;

        //ProductId
        p_rusProductId = ftData.ProductId;

        //Manufacturer
        p_rulManufacturerLength = strlen(ManufacturerBuf)+1;
        p_rpManufacturer = (char*)malloc(p_rulManufacturerLength*sizeof(char));
        memset(p_rpManufacturer, 0, p_rulManufacturerLength*sizeof(char));
        strcpy(p_rpManufacturer, ManufacturerBuf);

        //Description
        p_rulDescriptionLength = strlen(DescriptionBuf)+1;
        p_rpDescription = (char*)malloc(p_rulDescriptionLength*sizeof(char));
        memset(p_rpDescription, 0, p_rulDescriptionLength*sizeof(char));
        strcpy(p_rpDescription, DescriptionBuf);

        //SerialNumber
        p_rulSerialNumberLength = strlen(SerialNumberBuf)+1;
        p_rpSerialNumber = (char*)malloc(p_rulSerialNumberLength*sizeof(char));
        memset(p_rpSerialNumber, 0, p_rulSerialNumberLength*sizeof(char));
        strcpy(p_rpSerialNumber, SerialNumberBuf);

        //UseVcpDriver
        if(ftData.RIsD2XX) p_roUseVcpDriver = FALSE; else p_roUseVcpDriver = TRUE;

        oResult = TRUE;
    }
    else
    {
    	std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_EE_Read";
		perror(errormsg.c_str());

        oResult = FALSE;
    }
        
    return oResult;
}

bool CMmcUsbHndlBase::EE_Program(	unsigned short p_usVendorId, 
									unsigned short p_usProductId, 
									char* p_pManufacturer, 
									unsigned int p_ulManufacturerLength,
									char* p_pDescription, 
									unsigned int p_ulDescriptionLength,
									char* p_pSerialNumber, 
									unsigned int p_ulSerialNumberLength,
									bool p_oUseVcpDriver)
{
    const unsigned int BUFFER_SIZE(64);

    FT_PROGRAM_DATA ftData;
    TCHAR ManufacturerBuf[BUFFER_SIZE];
    TCHAR ManufacturerIdBuf[BUFFER_SIZE];
    TCHAR DescriptionBuf[BUFFER_SIZE];
    TCHAR SerialNumberBuf[BUFFER_SIZE];
    BOOL oResult(FALSE);
    FT_STATUS ftStatus = FT_OK;

    //ResetErrorInfo
    //m_ErrorInfo.Reset();

	//Prepare Structure
    ftData.Signature1 = 0x00000000;
    ftData.Signature2 = 0xffffffff;
    ftData.Version = 0x00000002; // EEPROM structure with FT232R extensions
    ftData.Manufacturer = ManufacturerBuf;
    ftData.ManufacturerId = ManufacturerIdBuf;
    ftData.Description = DescriptionBuf;
    ftData.SerialNumber = SerialNumberBuf;

    //Read EE
    if(FT_OK == (ftStatus = FT_EE_Read(m_Handle, &ftData)))
    {
        //VendorId
        ftData.VendorId = p_usVendorId;

        //ProductId
        ftData.ProductId = p_usProductId;

        //Manufacturer
        if(p_ulManufacturerLength < BUFFER_SIZE)
        {
            memset(ManufacturerBuf, 0, BUFFER_SIZE*sizeof(char));
            if(p_pManufacturer) strcpy(ManufacturerBuf, p_pManufacturer);
        }

        //Description
        if(p_ulDescriptionLength < BUFFER_SIZE)
        {
            memset(DescriptionBuf, 0, BUFFER_SIZE*sizeof(TCHAR));
            if(p_pDescription) strcpy(DescriptionBuf, p_pDescription);
        }

        //SerialNumber
        if(p_ulSerialNumberLength < BUFFER_SIZE)
        {
            memset(SerialNumberBuf, 0, BUFFER_SIZE*sizeof(TCHAR));
            if(p_pSerialNumber) strcpy(SerialNumberBuf, p_pSerialNumber);
        }

        //UseVcpDriver
        if(p_oUseVcpDriver) ftData.RIsD2XX = 0; else ftData.RIsD2XX = 1;

        //Program EE
        if(FT_OK == (ftStatus = FT_EE_Program(m_Handle, &ftData)))
            oResult = TRUE;
        else
        {
        	std::string errormsg = GetFtStatusDescription(ftStatus);
			errormsg += ":FT_EE_Program";
			perror(errormsg.c_str());

            oResult = FALSE;
        }
    }
    else
    {
    	std::string errormsg = GetFtStatusDescription(ftStatus);
		errormsg += ":FT_EE_Read";
		perror(errormsg.c_str());

        oResult = FALSE;
    }
      
    return oResult;
}
