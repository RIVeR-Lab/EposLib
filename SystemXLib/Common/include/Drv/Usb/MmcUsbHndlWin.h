#pragma once

#include <afx.h>
#define SS_ANSI
#include <drv/usb/win32/ftd2xx.h>
#include <drv/usb/MmcUsbHndlDef.h>
#include <StdString.h>

//**** Interface functions for FTD2xx Driver *************
typedef FT_STATUS (CALLBACK* t_FT_ListDevices) (void*, void*, DWORD);
typedef FT_STATUS (CALLBACK* t_FT_CreateDeviceInfoList) (DWORD*);
typedef FT_STATUS (CALLBACK* t_FT_GetDeviceInfo) (FT_HANDLE, FT_DEVICE*, LPDWORD, PCHAR, PCHAR, LPVOID);
typedef FT_STATUS (CALLBACK* t_FT_GetDeviceInfoDetail) (DWORD, DWORD*, DWORD*, DWORD*, DWORD*, void*, void*, FT_HANDLE*);
typedef FT_STATUS (CALLBACK* t_FT_Open) (int, FT_HANDLE*);
typedef FT_STATUS (CALLBACK* t_FT_OpenEx) (void*, DWORD, FT_HANDLE*);
typedef FT_STATUS (CALLBACK* t_FT_Close) (FT_HANDLE);
typedef FT_STATUS (CALLBACK* t_FT_Read) (FT_HANDLE, void*, DWORD, DWORD*);
typedef FT_STATUS (CALLBACK* t_FT_Write) (FT_HANDLE, void*, DWORD, DWORD*);
typedef FT_STATUS (CALLBACK* t_FT_ResetDevice) (FT_HANDLE);
typedef FT_STATUS (CALLBACK* t_FT_SetBaudRate) (FT_HANDLE, DWORD);
typedef FT_STATUS (CALLBACK* t_FT_SetDivisor) (FT_HANDLE, DWORD);
typedef FT_STATUS (CALLBACK* t_FT_Purge) (FT_HANDLE, DWORD);
typedef FT_STATUS (CALLBACK* t_FT_SetTimeouts) (FT_HANDLE, DWORD, DWORD);
typedef FT_STATUS (CALLBACK* t_FT_SetDataCharacteristics)(FT_HANDLE, BYTE, BYTE, BYTE);
typedef FT_STATUS (CALLBACK* t_FT_SetFlowControl)(FT_HANDLE, WORD, BYTE, BYTE);
typedef FT_STATUS (CALLBACK* t_FT_SetEventNotification)(FT_HANDLE, DWORD, void*);
typedef FT_STATUS (CALLBACK* t_FT_SetChars)(FT_HANDLE, UCHAR, UCHAR, UCHAR, UCHAR);
typedef FT_STATUS (CALLBACK* t_FT_GetQueueStatus)(FT_HANDLE, DWORD*);
typedef FT_STATUS (CALLBACK* t_FT_GetStatus)(FT_HANDLE, DWORD*, DWORD*, DWORD*);
typedef FT_STATUS (CALLBACK* t_FT_GetLatencyTimer)(FT_HANDLE, BYTE*);
typedef FT_STATUS (CALLBACK* t_FT_SetLatencyTimer)(FT_HANDLE, BYTE);
typedef FT_STATUS (CALLBACK* t_FT_SetUSBParameters)(FT_HANDLE, DWORD, DWORD);
typedef FT_STATUS (CALLBACK* t_FT_EE_Read)(FT_HANDLE, PFT_PROGRAM_DATA);
typedef FT_STATUS (CALLBACK* t_FT_EE_Program) (FT_HANDLE, PFT_PROGRAM_DATA);
typedef FT_STATUS (CALLBACK* t_FT_Rescan) ();
typedef FT_STATUS (CALLBACK* t_FT_Reload) (WORD, WORD);
//**** Interface functions for FTD2xx Driver *************

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

	bool		EE_Program(	unsigned short p_usVendorId, 
							unsigned short p_usProductId, 
							TCHAR* p_pManufacturer, 
							unsigned int p_ulManufacturerLength, 
							TCHAR* p_pDescription, 
							unsigned int p_ulDescriptionLength, 
							TCHAR* p_pSerialNumber, 
							unsigned int p_ulSerialNumberLength, 
							bool p_oUseVcpDriver);

	bool		EE_Read(	unsigned short& p_rusVendorId, 
							unsigned short& p_rusProductId, 
							TCHAR*& p_rpManufacturer, 
							unsigned int& p_rulManufacturerLength, 
							TCHAR*& p_rpDescription, 
							unsigned int& p_rulDescriptionLength, 
							TCHAR*& p_rpSerialNumber, 
							unsigned int& p_rulSerialNumberLength, 
							bool& p_roUseVcpDriver);
	bool		Rescan();
	bool		Reload(unsigned short usVendorId, unsigned short usProductId);
	bool		GetQueueStatus(unsigned int *pdAmountRxQueue);
	bool		CreateDeviceInfoList(unsigned int *pdNbOfDevices);
	bool		GetDeviceInfoDetail(unsigned int dDeviceIndex, unsigned int *pFlags, unsigned int *pType, unsigned int *pID, unsigned int *pLocId, char* strSerialNumberBuffer, char* strDescriptionBuffer);
	CStdString GetFtStatusDescription(unsigned long status);

private:
	bool		LoadLibraryFunctions();
	bool		FreeLibraryFunctions();
	bool		AreLibraryFunctionsLoaded();
	bool		AreOptionalLibraryFunctionsLoaded();

private:
	HINSTANCE					m_hDrvDLL;
	FT_HANDLE					m_Handle;
	t_FT_Open					m_lpOpen;
    t_FT_GetDeviceInfo			m_lpGetDeviceInfo;
    t_FT_OpenEx					m_lpOpenEx;
    t_FT_Close					m_lpClose;
    t_FT_Read					m_lpRead;
    t_FT_Write					m_lpWrite;
    t_FT_ResetDevice			m_lpResetDevice;
    t_FT_SetBaudRate			m_lpSetBaudRate;
    t_FT_SetDivisor				m_lpSetDivisor;
    t_FT_Purge					m_lpPurge;
    t_FT_SetTimeouts			m_lpSetTimeouts;
    t_FT_SetDataCharacteristics m_lpSetDataCharacteristics;
    t_FT_SetFlowControl			m_lpSetFlowControl;
    t_FT_SetChars				m_lpSetChars;
    t_FT_SetEventNotification	m_lpSetEventNotification;
    t_FT_GetQueueStatus			m_lpGetQueueStatus;
    t_FT_GetStatus				m_lpGetStatus;
    t_FT_GetLatencyTimer		m_lpGetLatencyTimer;
    t_FT_SetLatencyTimer		m_lpSetLatencyTimer;
    t_FT_SetUSBParameters		m_lpSetUSBParameters;
    t_FT_EE_Read				m_lpEERead;
    t_FT_EE_Program				m_lpEEProgram;
    t_FT_Rescan					m_lpRescan;
    t_FT_Reload					m_lpReload;
	t_FT_ListDevices			m_lpListDevices;
    t_FT_CreateDeviceInfoList	m_lpCreateDeviceInfoList;
    t_FT_GetDeviceInfoDetail	m_lpGetDeviceInfoDetail;
	unsigned long				m_ulLastError;
};