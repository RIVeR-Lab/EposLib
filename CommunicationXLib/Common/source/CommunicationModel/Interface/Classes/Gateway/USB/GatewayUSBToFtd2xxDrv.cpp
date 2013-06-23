// GatewayUSBToFtd2xxDrv.cpp: Implementierung der Klasse CGatewayUSBToFtd2xxDrv.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/Interface/Classes/Gateway/USB/GatewayUSBToFtd2xxDrv.h>



#include <malloc.h>
#include <memory.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/BaseClasses/Command_I.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_USB.h>
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include <CommunicationModel/Interface/Classes/Gateway/USB/Classes/RxBufferThread.h>
#include <CommunicationModel/Interface/Classes/Gateway/USB/Classes/Ftd2xxDeviceInfo.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayUSBToFtd2xxDrv::CGatewayUSBToFtd2xxDrv()
{
    m_oFirstInit = FALSE;

    m_pRxBufferThread = new CRxBufferThread();

    m_pUsbDeviceInfoHandling = 0;
	m_PortName = _T("");
	m_oUpdatePortOpened = TRUE;

    ResetRxBuffer();

    InitErrorHandling();
    InitBaudrateSelection();
    InitDefaultPortSettings();
}

CGatewayUSBToFtd2xxDrv::~CGatewayUSBToFtd2xxDrv()
{
    if(m_pRxBufferThread) delete m_pRxBufferThread;
}

BOOL CGatewayUSBToFtd2xxDrv::InitBaudrateSelection()
{
    m_BaudrateSelection.clear();
    m_BaudrateSelection.push_back(BAUDRATE_1000000);

    return TRUE;
}

BOOL CGatewayUSBToFtd2xxDrv::InitDefaultPortSettings()
{
    m_dDefaultBaudrate = BAUDRATE_1000000;
    m_dDefaultTimeout = 500;

    GetDefaultPortSettings(&m_dBaudrate, &m_dTimeout);

    return TRUE;
}

BOOL CGatewayUSBToFtd2xxDrv::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), TRUE);
    CCommand_I* pCommand_I;

    if(CGateway::ProcessCommand(pCommand, pManager, h_Handle, hTransactionHandle))
	{
		if(CheckLayers(pCommand, pManager))
		{
			pCommand_I = (CCommand_I*)pCommand;
			switch(pCommand->GetCommandId())
			{
				case USB_WRITE_DATA:    return Process_WriteData(pCommand_I);
				case USB_READ_DATA:     return Process_ReadData(pCommand_I);
				case USB_EE_READ:       return Process_EE_Read(pCommand_I);
				case USB_EE_PROGRAM:    return Process_EE_Program(pCommand_I);
				case USB_RESCAN:        return Process_Rescan(pCommand_I);
				case USB_RELOAD:        return Process_Reload(pCommand_I);
			}
		}
    }

    return FALSE;
}

BOOL CGatewayUSBToFtd2xxDrv::Process_WriteData(CCommand_I* pCommand)
{
    //*Constants I*
    const int PARAMETER_INDEX_DATA(0);
    const int RETURN_PARAMETER_INDEX_NB_OF_BYTES_WRITTEN(0);

    //*Variables I*
    //Parameter
    void* pDataBuffer(0);
    DWORD dDataBufferLength;
    //ReturnParameter
    DWORD dNbOfBytesWritten(0);

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Prepare DataBuffer
        dDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

        //Get I Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = WriteData(pDataBuffer, dDataBufferLength, &dNbOfBytesWritten, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_NB_OF_BYTES_WRITTEN, &dNbOfBytesWritten, sizeof(dNbOfBytesWritten));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::Process_ReadData(CCommand_I* pCommand)
{
    //*Constants I*
    const int PARAMETER_INDEX_NB_OF_BYTES_TO_READ(0);
    const int RETURN_PARAMETER_INDEX_DATA(0);

    //*Variables I*
    //Parameter
    DWORD dNbOfBytesToRead;

    //ReturnParameter
    void* pDataBuffer(0);
    DWORD dDataBufferLength;
    DWORD dNbOfBytesRead(0);

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Get I Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NB_OF_BYTES_TO_READ, &dNbOfBytesToRead, sizeof(dNbOfBytesToRead));

        //Prepare DataBuffer
        dDataBufferLength = dNbOfBytesToRead;
        if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

        //Execute Command
        oResult = ReadData(pDataBuffer, dNbOfBytesToRead, &dNbOfBytesRead, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pDataBuffer, dNbOfBytesRead);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::Process_EE_Read(CCommand_I* pCommand)
{
	BOOL oResult(FALSE);

    //*Constants I*
    const int RETURN_PARAMETER_INDEX_VENDOR_ID(0);
    const int RETURN_PARAMETER_INDEX_PRODUCT_ID(1);
    const int RETURN_PARAMETER_INDEX_MANUFACTURER(2);
    const int RETURN_PARAMETER_INDEX_DESCRIPTION(3);
    const int RETURN_PARAMETER_INDEX_SERIAL_NUMBER(4);
    const int RETURN_PARAMETER_INDEX_USE_VCP_DRIVER(5);

    //*Variables I*
    //ReturnParameter
    WORD usVendorId(0);
    WORD usProductId(0);
    TCHAR* pManufacturer(0);
    unsigned int ulManufacturerLength(0);
    TCHAR* pDescription(0);
    unsigned int ulDescriptionLength(0);
    TCHAR* pSerialNumber(0);
    unsigned int ulSerialNumberLength(0);
    bool oUseVcpDriver = false;

    CErrorInfo errorInfo;
    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Execute Command
        oResult = m_UsbHndl.EE_Read(usVendorId, usProductId, pManufacturer, ulManufacturerLength, pDescription, ulDescriptionLength, pSerialNumber, ulSerialNumberLength, oUseVcpDriver);

        //Set I ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_VENDOR_ID, &usVendorId, sizeof(usVendorId));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_PRODUCT_ID, &usProductId, sizeof(usProductId));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_MANUFACTURER, pManufacturer, ulManufacturerLength);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DESCRIPTION, pDescription, ulDescriptionLength);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_SERIAL_NUMBER, pSerialNumber, ulSerialNumberLength);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_USE_VCP_DRIVER, &oUseVcpDriver, sizeof(oUseVcpDriver));

        //Free Buffers
        if(pManufacturer) free(pManufacturer);
        if(pDescription) free(pDescription);
        if(pSerialNumber) free(pSerialNumber);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::Process_EE_Program(CCommand_I* pCommand)
{
	BOOL oResult(FALSE);

    //*Constants I*
    const int PARAMETER_INDEX_VENDOR_ID(0);
    const int PARAMETER_INDEX_PRODUCT_ID(1);
    const int PARAMETER_INDEX_MANUFACTURER(2);
    const int PARAMETER_INDEX_DESCRIPTION(3);
    const int PARAMETER_INDEX_SERIAL_NUMBER(4);
    const int PARAMETER_INDEX_USE_VCP_DRIVER(5);

    //*Variables I*
    //Parameter
    WORD usVendorId(0);
    WORD usProductId(0);
    TCHAR* pManufacturer(0);
    DWORD ulManufacturerLength(0);
    TCHAR* pDescription(0);
    DWORD ulDescriptionLength(0);
    TCHAR* pSerialNumber(0);
    DWORD ulSerialNumberLength(0);
    BOOL oUseVcpDriver(FALSE);

    CErrorInfo errorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Prepare Buffers
        ulManufacturerLength = pCommand->GetParameterLength(PARAMETER_INDEX_MANUFACTURER);
        if(ulManufacturerLength > 0) pManufacturer = (TCHAR*)malloc(ulManufacturerLength*sizeof(TCHAR)+1);
        if(pManufacturer) memset(pManufacturer, 0, ulManufacturerLength*sizeof(TCHAR)+1);

        ulDescriptionLength = pCommand->GetParameterLength(PARAMETER_INDEX_DESCRIPTION);
        if(ulDescriptionLength > 0) pDescription = (TCHAR*)malloc(ulDescriptionLength*sizeof(TCHAR)+1);
        if(pDescription) memset(pDescription, 0, ulDescriptionLength*sizeof(TCHAR)+1);

        ulSerialNumberLength = pCommand->GetParameterLength(PARAMETER_INDEX_SERIAL_NUMBER);
        if(ulSerialNumberLength > 0) pSerialNumber = (TCHAR*)malloc(ulSerialNumberLength*sizeof(TCHAR)+1);
        if(pSerialNumber) memset(pSerialNumber, 0, ulSerialNumberLength*sizeof(TCHAR)+1);

        //Get I Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_VENDOR_ID, &usVendorId, sizeof(usVendorId));
        pCommand->GetParameterData(PARAMETER_INDEX_PRODUCT_ID, &usProductId, sizeof(usProductId));
        pCommand->GetParameterData(PARAMETER_INDEX_MANUFACTURER, pManufacturer, ulManufacturerLength);
        pCommand->GetParameterData(PARAMETER_INDEX_DESCRIPTION, pDescription, ulDescriptionLength);
        pCommand->GetParameterData(PARAMETER_INDEX_SERIAL_NUMBER, pSerialNumber, ulSerialNumberLength);
        pCommand->GetParameterData(PARAMETER_INDEX_USE_VCP_DRIVER, &oUseVcpDriver, sizeof(oUseVcpDriver));

        //Execute Command
		oResult = m_UsbHndl.EE_Program(usVendorId, usProductId, pManufacturer, ulManufacturerLength, pDescription, ulDescriptionLength, pSerialNumber, ulSerialNumberLength, oUseVcpDriver==TRUE) ? TRUE : FALSE;

        //Set I ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);

        //Free Buffers
        if(pManufacturer) free(pManufacturer);
        if(pDescription) free(pDescription);
        if(pSerialNumber) free(pSerialNumber);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::Process_Rescan(CCommand_I* pCommand)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Execute Command
        oResult = Rescan(&errorInfo);

        //Set I ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::Process_Reload(CCommand_I* pCommand)
{
    //*Constants I*
    const int PARAMETER_INDEX_VENDOR_ID(0);
    const int PARAMETER_INDEX_PRODUCT_ID(1);

    //*Variables I*
    //Parameter
    WORD usVendorId(0);
    WORD usProductId(0);

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Get I Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_VENDOR_ID, &usVendorId, sizeof(usVendorId));
        pCommand->GetParameterData(PARAMETER_INDEX_PRODUCT_ID, &usProductId, sizeof(usProductId));

        //Execute Command
        oResult = Reload(usVendorId, usProductId, &errorInfo);

        //Set I ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

CGateway* CGatewayUSBToFtd2xxDrv::Clone()
{
    CGatewayUSBToFtd2xxDrv* pClonedGateway;

    pClonedGateway = new CGatewayUSBToFtd2xxDrv();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayUSBToFtd2xxDrv& CGatewayUSBToFtd2xxDrv::operator=(CGatewayUSBToFtd2xxDrv& other)
{
    if(this != &other)
    {
        *((CGatewayIToDrv*)this) = *((CGatewayIToDrv*)&other);
        m_pUsbDeviceInfoHandling = other.m_pUsbDeviceInfoHandling;
    }

    return *this;
}

BOOL CGatewayUSBToFtd2xxDrv::OpenPort(CStdString strPortName, CErrorInfo* pErrorInfo)
{
    const BOOL CHANGE_ONLY(FALSE);
    BOOL oResult(FALSE);

    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), TRUE);

	unsigned long lResult = 0;
	DWORD ulLocationId = 0;
    CStdString strSerialNumber;
    
    if(GetLocationId(strPortName, ulLocationId))
    {
    	if( GetSerialNumber(strPortName, strSerialNumber) )
    	{
			if(m_UsbHndl.Open(ulLocationId, strSerialNumber))
			{
				//Logging
				#ifdef _Logging_UsbEnumeration
					if(m_pUsbDeviceInfoHandling) m_pUsbDeviceInfoHandling->WriteToLogFile("OpenPort", strPortName, ulLocationId);
				#endif

				//Configure default settings for port
				if(ConfigurePortSettings(m_dBaudrate, m_dTimeout, CHANGE_ONLY, pErrorInfo, FALSE))
				{
					m_PortName = strPortName;
					if(m_oUpdatePortOpened) UpdatePortOpened(m_PortName, TRUE);
					oResult = TRUE;
				}
				else
				{
					ClosePort();
					oResult = FALSE;
				}
			}
			else
			{
				//Logging
				#ifdef _Logging_UsbEnumeration
					CStdString info;
					info.Format("OpenPort Failed (lResult = %i)", lResult);
					if(m_pUsbDeviceInfoHandling) m_pUsbDeviceInfoHandling->WriteToLogFile(info, strPortName, ulLocationId);
				#endif

				ClosePort();
				if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_OpeningPort, pErrorInfo);
				oResult = FALSE;
			}
    	}
    }
	else
	{
		//Logging
		#ifdef _Logging_UsbEnumeration
			if(m_pUsbDeviceInfoHandling) m_pUsbDeviceInfoHandling->WriteToLogFile("OpenPort No LocId", strPortName, ulLocationId);
		#endif
	}

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::ReopenPort(CStdString strPortName, CErrorInfo* pErrorInfo)
{
	CStdString info = _T("");
	BOOL oResult = FALSE;

	//Deactivate Update
	m_oUpdatePortOpened = FALSE;

	//Logging
	#ifdef _Logging_UsbEnumeration
		if(m_pUsbDeviceInfoHandling) m_pUsbDeviceInfoHandling->WriteToLogFile("ReopenPort", strPortName);
	#endif

	//Close
    if(ClosePort(pErrorInfo))
    {
        //Open
        if(!OpenPort(strPortName, pErrorInfo))
		{
			//Reload FTDI Driver
			bool bReloaded = m_UsbHndl.Reload();

			//Logging
			#ifdef _Logging_UsbEnumeration
				if(m_pUsbDeviceInfoHandling)
				{
					info.Format("Reload FTDI Driver (result = %s)", bReloaded ? "success" : "failed");
					m_pUsbDeviceInfoHandling->WriteToLogFile(info, strPortName);
				}
			#endif

			//Retry Open
			oResult = OpenPort(strPortName, pErrorInfo);
		}
		else
		{
			oResult = TRUE;
		}
    }

	//Reactivate Update
	m_oUpdatePortOpened = TRUE;

	if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
	return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::SetPortSettings(DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo)
{
    return ConfigurePortSettings(dBaudrate, dTimeout, oChangeOnly, pErrorInfo, TRUE);
}

BOOL CGatewayUSBToFtd2xxDrv::ConfigurePortSettings(DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo, BOOL oDoLock)
{
	BOOL oResult(TRUE);
    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), FALSE);

    //Lock
    if(oDoLock) lock.Lock();

    //Do only if changed
    if(!oChangeOnly || (dTimeout != m_dTimeout) || (dBaudrate != m_dBaudrate))
    {
        //Do only if changed
        if(oResult)
        {
            if(!oChangeOnly || (dTimeout != m_dTimeout))
            {
                //Set Timeout
                if(!SetTimeout(dTimeout))
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_SetPortSettings, pErrorInfo);
                    oResult = FALSE;
                }
            }
        }

        if(oResult)
        {
            if(!oChangeOnly || !m_oFirstInit)
            {
                //Configure PortMode
                if(!ConfigurePortMode(m_wPortMode, pErrorInfo, FALSE))
                {
                    oResult = FALSE;
                }
            }
        }

        //Do only if changed
        if(oResult)
        {
            if(!oChangeOnly || (dBaudrate != m_dBaudrate))
            {
                if(SetBaudrate(dBaudrate))
                {
                    //Baudrate is set
                    m_dBaudrate = dBaudrate;
                }
                else
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_SetPortSettings, pErrorInfo);
                    oResult = FALSE;
                }
            }
        }

        if(oResult)
        {
            if(!oChangeOnly || !m_oFirstInit)
            {
                //Purge the buffer
                if(!PurgeBuffer())
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_SetPortSettings, pErrorInfo);
                    oResult = FALSE;
                }
                else
                {
                    m_oFirstInit = TRUE;
                }
            }
        }
    }

    //Tracing
    Trace_Settings(dBaudrate, dTimeout);

    //Unlock
    if(oDoLock) lock.Unlock();

    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_SetPortSettings, pErrorInfo);
    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::SetPortMode(WORD wPortMode, CErrorInfo* pErrorInfo)
{
    return ConfigurePortMode(wPortMode, pErrorInfo, TRUE);
}

BOOL CGatewayUSBToFtd2xxDrv::ConfigurePortMode(WORD wPortMode, CErrorInfo* pErrorInfo, BOOL oDoLock)
{
    BOOL oResult(FALSE);

    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), FALSE);

    //Lock
    if(oDoLock) lock.Lock();

    if(m_wPortMode == 0)
		oResult = m_UsbHndl.Configure() ? TRUE : FALSE;

    //Unlock
    if(oDoLock) lock.Unlock();

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::SetTimeout(DWORD p_ulTimeout)
{
    const DWORD WRITE_TIMEOUT(200);

    DWORD dReadTimeout(0);
    DWORD dWriteTimeout(0);
    BOOL oResult(FALSE);

    //Prepare Parameter
    oResult = TRUE;
    dReadTimeout = p_ulTimeout;
    dWriteTimeout = WRITE_TIMEOUT;

    //SetTimeouts
	if(!m_UsbHndl.SetTimeouts(dReadTimeout, dWriteTimeout))
        oResult = FALSE;
    else
        m_dTimeout = p_ulTimeout;

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::SetBaudrate(DWORD p_ulBaudrate)
{
    return m_UsbHndl.SetBaudrate(p_ulBaudrate) ? TRUE : FALSE;
}

BOOL CGatewayUSBToFtd2xxDrv::PurgeBuffer()
{
    unsigned int dAmountInRxQueue(1);
    unsigned int dAmountInTxQueue(1);
    unsigned int dEventStatus(0);
    BYTE uData(0);
    unsigned int dDataRead(1);
    BOOL oResult(FALSE);

    oResult = TRUE;

    while(oResult && (dAmountInRxQueue > 0))
    {
		if(m_UsbHndl.GetStatus(&dAmountInRxQueue, &dAmountInTxQueue, &dEventStatus))
        {
            if(dAmountInRxQueue > 0)
            {
                //Read all Characters in Rx Buffer
                oResult = TRUE;
                while(dAmountInRxQueue > 0)
                {
                    if(m_UsbHndl.ReadByte(&uData, &dDataRead))
                    {
                        dAmountInRxQueue -= dDataRead;
                    }
                    else
                    {
                        oResult = FALSE;
                        break;
                    }
                }
            }
            else
            {
                //No Characters in Rx Buffer
                oResult = TRUE;
            }
        }
        else
        {
            //Failed reading Status of Rx Buffer
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::WriteData(void* p_pDataBuffer, DWORD p_dNbOfBytesToWrite, DWORD* p_pdNbOfBytesWritten, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_RETRY = 2;
	const DWORD MAX_SIZE = 62;

	DWORD dNumberOfBytesToWrite(0);
	unsigned int dNumberOfBytesWritten(0);
	DWORD dTotalNumberOfBytesToWrite(0);
	DWORD dTotalNumberOfByteWritten(0);
	void* pDataBuffer(0);
	DWORD ulRetryCount(0);
    BOOL oResult(FALSE);

    //ResetErrorInfo
    if(p_pErrorInfo) p_pErrorInfo->Reset();

    //Execute Command
    if(p_pDataBuffer && (p_dNbOfBytesToWrite > 0))
    {
        //Init
		dTotalNumberOfBytesToWrite = p_dNbOfBytesToWrite;
		oResult = TRUE;

		while(oResult && (dTotalNumberOfBytesToWrite > 0))
		{
			//Limit Size
			if(dTotalNumberOfBytesToWrite < MAX_SIZE) dNumberOfBytesToWrite = dTotalNumberOfBytesToWrite; else dNumberOfBytesToWrite = MAX_SIZE;
			pDataBuffer = (void*)((BYTE*)p_pDataBuffer + dTotalNumberOfByteWritten);

			//Write Data
			if(m_UsbHndl.Write(pDataBuffer, dNumberOfBytesToWrite, &dNumberOfBytesWritten) && (dNumberOfBytesWritten > 0))
			{
				dTotalNumberOfBytesToWrite -= dNumberOfBytesWritten;
				dTotalNumberOfByteWritten += dNumberOfBytesWritten;
				if(p_pdNbOfBytesWritten) *p_pdNbOfBytesWritten = dTotalNumberOfByteWritten;
				ulRetryCount = 0;
				oResult = TRUE;
			}
			else
			{
				if(ulRetryCount < MAX_RETRY)
				{
					//Retry
					ulRetryCount++;
					dTotalNumberOfBytesToWrite -= dNumberOfBytesWritten;
					dTotalNumberOfByteWritten += dNumberOfBytesWritten;
					if(p_pdNbOfBytesWritten) *p_pdNbOfBytesWritten = dTotalNumberOfByteWritten;
					oResult = TRUE;
				}
				else
				{
					if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_USB_WriteData, p_pErrorInfo);
					oResult = FALSE;
				}
			}
		}
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
        oResult = FALSE;
    }

    Trace_WriteData(oResult, p_pDataBuffer, p_dNbOfBytesToWrite, p_pdNbOfBytesWritten);

    return oResult;
}

CStdString CGatewayUSBToFtd2xxDrv::GetFtStatusDescription(unsigned long status)
{
	return m_UsbHndl.GetFtStatusDescription(status).c_str();
}

BOOL CGatewayUSBToFtd2xxDrv::ReadData(void* p_pDataBuffer, DWORD p_dNbOfBytesToRead, DWORD* p_pdNbOfBytesRead, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_RETRY = 2;
	const DWORD MAX_SIZE = 62;
	BOOL oResult(FALSE);
	DWORD dNumberOfBytesToRead(0);
	unsigned int dNumberOfBytesRead(0);
	DWORD dTotalNumberOfBytesToRead(0);
	DWORD dTotalNumberOfByteRead(0);
	void* pDataBuffer(0);
	DWORD ulRetryCount(0);
	long lResult = 0;

    //ResetErrorInfo
    if(p_pErrorInfo) p_pErrorInfo->Reset();

    //Execute Command
    if(p_pDataBuffer && (p_dNbOfBytesToRead > 0) && p_pdNbOfBytesRead)
    {
        //Init
		dTotalNumberOfBytesToRead = p_dNbOfBytesToRead;
		oResult = TRUE;

		while(oResult && (dTotalNumberOfBytesToRead > 0))
		{
			//Limit Size
			if(dTotalNumberOfBytesToRead < MAX_SIZE) dNumberOfBytesToRead = dTotalNumberOfBytesToRead; else dNumberOfBytesToRead = MAX_SIZE;
			pDataBuffer = (void*)((BYTE*)p_pDataBuffer + dTotalNumberOfByteRead);
			dNumberOfBytesRead = 0;

			//Write Data
			if(m_UsbHndl.Read(pDataBuffer, dNumberOfBytesToRead, &dNumberOfBytesRead) && (dNumberOfBytesRead > 0))
			{
				dTotalNumberOfBytesToRead -= dNumberOfBytesRead;
				dTotalNumberOfByteRead += dNumberOfBytesRead;
				if(p_pdNbOfBytesRead) *p_pdNbOfBytesRead = dTotalNumberOfByteRead;
				ulRetryCount = 0;
				oResult = TRUE;
			}
			else
			{
				if(ulRetryCount < MAX_RETRY)
				{
					//Retry
					ulRetryCount++;
					dTotalNumberOfBytesToRead -= dNumberOfBytesRead;
					dTotalNumberOfByteRead += dNumberOfBytesRead;
					if(p_pdNbOfBytesRead) *p_pdNbOfBytesRead = dTotalNumberOfByteRead;
					oResult = TRUE;
				}
				else
				{
					if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_USB_ReadData, p_pErrorInfo);
					oResult = FALSE;
				}
			}
		}
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
        oResult = FALSE;
    }

    Trace_ReadData(oResult, p_pDataBuffer, p_dNbOfBytesToRead, p_pdNbOfBytesRead);

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::Rescan(CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

    //ResetErrorInfo
    if(pErrorInfo) pErrorInfo->Reset();

    //Rescan
    if(m_UsbHndl.Rescan())
        oResult = TRUE;
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_USB_Rescan, pErrorInfo);
        oResult = FALSE;
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::Reload(WORD p_usVendorId, WORD p_usProductId, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

    //ResetErrorInfo
    if(pErrorInfo) pErrorInfo->Reset();

    //Reload
    if(m_UsbHndl.Reload(p_usVendorId, p_usProductId))
        oResult = TRUE;
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_USB_Reload, pErrorInfo);
        oResult = FALSE;
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::WaitForRxData(DWORD dTimeout)
{
    BOOL oResult(FALSE);

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::ResetRxBuffer()
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    BOOL oResult(TRUE);

    memset(m_RxBuffer, 0, sizeof(m_RxBuffer));
    m_dRxBufferCount = 0;
    m_iGetIndex = 0;
    m_iPutIndex = 0;

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::UpdateRxBuffer()
{
    CMmcSingleLock lock(&m_Sync, TRUE);

    BOOL oResult(FALSE);
    unsigned int dAmountRxQueue(0);
    unsigned int dRead(0);

    if(m_iPutIndex < 9000)
    {
        if(m_UsbHndl.GetQueueStatus(&dAmountRxQueue))
        {
            if(dAmountRxQueue > 0)
            {
                if(m_UsbHndl.Read(&m_RxBuffer[m_iPutIndex], dAmountRxQueue, &dRead) && (dRead == dAmountRxQueue))
                {
                    m_iPutIndex += dRead;
                    m_dRxBufferCount += dRead;
                }
            }

            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::GetRxData(void* pBuffer, DWORD dNbOfBytesToRead, DWORD* pdNbOfBytesRead)
{
	BOOL oResult(FALSE);
    CMmcSingleLock lock(&m_Sync, TRUE);

    if(pBuffer)
    {
        if(m_iPutIndex < 9000)
        {
            if(m_dRxBufferCount >= dNbOfBytesToRead)
            {
                memcpy(pBuffer, &m_RxBuffer[m_iGetIndex], dNbOfBytesToRead);
                m_iGetIndex += dNbOfBytesToRead;
                m_dRxBufferCount -= dNbOfBytesToRead;
                if(pdNbOfBytesRead) *pdNbOfBytesRead = dNbOfBytesToRead;
                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::GetPortSettings(DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    if(pdBaudrate) *pdBaudrate = m_dBaudrate;
    if(pdTimeout) *pdTimeout = m_dTimeout;
    return TRUE;
}

BOOL CGatewayUSBToFtd2xxDrv::GetPortMode(WORD* pwPortMode, CErrorInfo* pErrorInfo)
{
    if(pwPortMode)
    {
        *pwPortMode = m_wPortMode;
        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CGatewayUSBToFtd2xxDrv::ClosePort(CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);
    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), TRUE);

    if(m_UsbHndl.Close())
    {
        //Logging
		#ifdef _Logging_UsbEnumeration
			if(m_pUsbDeviceInfoHandling) m_pUsbDeviceInfoHandling->WriteToLogFile("ClosePort", m_PortName);
		#endif

		Trace_Close();

		if(m_oUpdatePortOpened) UpdatePortOpened(m_PortName, FALSE);

		m_PortName = _T("");
        oResult = TRUE;
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_ClosingPort, pErrorInfo);
        oResult = FALSE;
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::ResetPort(CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);
    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), TRUE);

    oResult = TRUE;

    if(!PurgeBuffer())
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_ResetPort, pErrorInfo);
        oResult = FALSE;
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::InitPort(WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), TRUE);

    tDeviceInfoList deviceInfoList;
    BOOL oResult(FALSE);

    if(m_pUsbDeviceInfoHandling)
    {
		//Get All physically available Ports
        if(GetDeviceInfos(deviceInfoList))
        {
			if(m_pUsbDeviceInfoHandling->InitDeviceInfos(deviceInfoList))
            {
                oResult = UpdatePortList(m_pUsbDeviceInfoHandling);
            }

            DeleteDeviceInfoList(deviceInfoList);
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::UpdatePort(tPortList& p_rOpenPortList)
{
    CMmcSingleLock lock(CFtd2xxDeviceInfoHandling::GetSyncInstance(), TRUE);
    tDeviceInfoList deviceInfoList;
    BOOL oResult(FALSE);

    if(m_pUsbDeviceInfoHandling)
    {
        //Get All physically available Ports
        if(GetDeviceInfos(deviceInfoList))
        {
            if(m_pUsbDeviceInfoHandling->UpdateDeviceInfos(deviceInfoList, p_rOpenPortList))
            {
                //Update PortList
                oResult = UpdatePortList(m_pUsbDeviceInfoHandling);
            }

            DeleteDeviceInfoList(deviceInfoList);
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::UpdatePortList(CFtd2xxDeviceInfoHandling* pDeviceInfoHandling)
{
    CStdStringArray portNames;
    CStdString portName(_T(""));
    BOOL oResult(FALSE);

    //Reset PortList
    DeletePortList();

    if(pDeviceInfoHandling)
    {
        //Add Ports
        if(pDeviceInfoHandling->GetPortNames(portNames))
        {
            oResult = TRUE;
            for(std::size_t i=0;i<portNames.size();i++)
            {
                portName = portNames.at(i);
                AddPortList(portName);
            }
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::InitInterfacePortName(CStdString* pStrInterfacePortName, WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    m_strInterfacePortName = INTERFACE_PORT_USB;
    if(pStrInterfacePortName) *pStrInterfacePortName = m_strInterfacePortName;
    return TRUE;
}

BOOL CGatewayUSBToFtd2xxDrv::InitInterfaceName(CStdString* pStrInterfaceName, WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    m_strInterfaceName = INTERFACE_USB;
    if(pStrInterfaceName) *pStrInterfaceName = m_strInterfaceName;
    return TRUE;
}

BOOL CGatewayUSBToFtd2xxDrv::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName(_T("GatewayUSBToFtd2xxDrv"));

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(INTERFACE_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayUSBToFtd2xxDrv::UpdatePortOpened(CStdString p_PortName, BOOL p_oOpened)
{
	BOOL oResult(FALSE);

    if(m_pUsbDeviceInfoHandling)
    {
        oResult = m_pUsbDeviceInfoHandling->UpdatePortOpened(p_PortName, p_oOpened);
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::GetSerialNumber(CStdString p_PortName, CStdString& p_serialNumber)
{
	BOOL oResult(FALSE);

	if(m_pUsbDeviceInfoHandling)
	{
		oResult = m_pUsbDeviceInfoHandling->GetSerialNumber(p_PortName, p_serialNumber);
	}

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::GetLocationId(CStdString p_PortName, DWORD& p_rulLocationId)
{
    BOOL oResult(FALSE);

    if(m_pUsbDeviceInfoHandling)
    {
        if(m_pUsbDeviceInfoHandling->GetLocationId(p_PortName, p_rulLocationId))
        {
            if(p_rulLocationId == 0)
            {
                //LocationId is not valid, update LocationId
                oResult = UpdateLocationId(m_pUsbDeviceInfoHandling, p_PortName, p_rulLocationId);
            }
            else
            {
                //LocationId is valid
                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::UpdateLocationId(CFtd2xxDeviceInfoHandling* pDeviceInfoHandling, CStdString p_PortName, DWORD& p_rulLocationId)
{
    tDeviceInfoList deviceInfoList;
    BOOL oResult(FALSE);

    if(pDeviceInfoHandling)
    {
        //Get All physically available Ports
        if(GetDeviceInfos(deviceInfoList))
        {
            //Update LocationId
            if(pDeviceInfoHandling->UpdateLocationId(deviceInfoList, p_PortName))
            {
                //Get Updated LocationId
                oResult = pDeviceInfoHandling->GetLocationId(p_PortName, p_rulLocationId);
            }

            DeleteDeviceInfoList(deviceInfoList);
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::GetDeviceInfos(tDeviceInfoList& p_rDeviceInfoList)
{
    const int BUFFER_SIZE(100);
    const DWORD PRODUCT_ID_DEFAULT_CHIP(0x04036001);
    const DWORD PRODUCT_ID(0x0403A8B0);
    BOOL oResult(FALSE);
    char strSerialNumberBuffer[BUFFER_SIZE];
    char strDescriptionBuffer[BUFFER_SIZE];
    unsigned int dNbOfDevices(0);
    DWORD dDeviceIndex(0);
    unsigned int Flags(0);
    unsigned int Type(0);
    unsigned int ID(0);
    unsigned int LocId(0);
    CFtd2xxDeviceInfo* pDeviceInfo(0);

    //Reset
    DeleteDeviceInfoList(p_rDeviceInfoList);

    //Enum Devices
    if (m_UsbHndl.CreateDeviceInfoList(&dNbOfDevices))
    {
        oResult = TRUE;
        for(dDeviceIndex=0; dDeviceIndex<dNbOfDevices; dDeviceIndex++)
        {
            if (m_UsbHndl.GetDeviceInfoDetail(dDeviceIndex, &Flags, &Type, &ID, &LocId, strSerialNumberBuffer, strDescriptionBuffer))
            {
                if((PRODUCT_ID == ID) || (PRODUCT_ID_DEFAULT_CHIP == ID))
                {
                    //Add DeviceInfo
                    pDeviceInfo = new CFtd2xxDeviceInfo();
                    pDeviceInfo->Init(LocId, strSerialNumberBuffer, Flags);

                    //Add to List
                    p_rDeviceInfoList.push_back(pDeviceInfo);
                }
            }
        }
    }

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::DeleteDeviceInfoList(tDeviceInfoList& p_rDeviceInfoList)
{
	BOOL oResult(FALSE);

	for( tDeviceInfoList::iterator it=p_rDeviceInfoList.begin();
				it!=p_rDeviceInfoList.end(); it++ )
	{
		CFtd2xxDeviceInfo* pDeviceInfo = (*it);
        delete pDeviceInfo;
    }

    p_rDeviceInfoList.clear();

    return oResult;
}

BOOL CGatewayUSBToFtd2xxDrv::InitUsbDeviceInfoHandling(CFtd2xxDeviceInfoHandling* pUsbDeviceInfoHandling)
{
    BOOL oResult(TRUE);

    m_pUsbDeviceInfoHandling = pUsbDeviceInfoHandling;

    return oResult;
}

