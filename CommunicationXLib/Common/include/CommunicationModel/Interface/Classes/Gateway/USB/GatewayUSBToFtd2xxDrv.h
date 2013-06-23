// GatewayUSBToFtd2xxDrv.h: Schnittstelle für die Klasse CGatewayUSBToFtd2xxDrv.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayUSBToFtd2xxDrv_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayUSBToFtd2xxDrv_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _TODO
#include <ftdi.h>
#endif

#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/GatewayIToDrv.h>
#include "Classes/Ftd2xxDeviceInfoHandling.h"
#include <Drv/Usb/MmcUsbHndl.h>

//baudrate selection
const DWORD     BAUDRATE_1000000    = 1000000;

class CCommand_I;
class CRxBufferThread;
class CInterfaceBase;
class CFtd2xxDeviceInfo;
class CFtd2xxDeviceInfoHandling;

class CGatewayUSBToFtd2xxDrv : public CGatewayIToDrv
{
public:
    CGatewayUSBToFtd2xxDrv();
    virtual ~CGatewayUSBToFtd2xxDrv();

    virtual CGateway* Clone();
    //Interne Struktur Funktionen
    virtual BOOL InitPort(WORD wBoardNumber, WORD wNbBoardWithOldDriver);
    virtual BOOL UpdatePort(tPortList& p_rOpenPortList);

//Initialisation
    virtual BOOL OpenPort(CStdString strPortName, CErrorInfo* pErrorInfo = 0);
	virtual BOOL ReopenPort(CStdString strPortName, CErrorInfo* pErrorInfo = 0);
    virtual BOOL ClosePort(CErrorInfo* pErrorInfo = 0);

//Hilfsfunktionen
    virtual BOOL ResetPort(CErrorInfo* pErrorInfo = 0);

//Setting Funktionen
    virtual BOOL SetPortSettings(DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo = 0);
    virtual BOOL GetPortSettings(DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo = 0);

    CGatewayUSBToFtd2xxDrv& operator=(CGatewayUSBToFtd2xxDrv& other);

virtual BOOL SetPortMode(WORD wPortMode, CErrorInfo* pErrorInfo = 0);
    virtual BOOL GetPortMode(WORD* pwPortMode, CErrorInfo* pErrorInfo = 0);

//Name Funktionen
    virtual BOOL InitInterfacePortName(CStdString* pStrInterfacePortName, WORD wBoardNumber, WORD wNbBoardWithOldDriver);
    virtual BOOL InitInterfaceName(CStdString* pStrInterfaceName, WORD wBoardNumber, WORD wNbBoardWithOldDriver);

//Funktionalität
    virtual BOOL ProcessCommand(CCommandRoot*pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

    BOOL WaitForRxData(DWORD dTimeout);
    BOOL UpdateRxBuffer();

    //UsbDeviceInfoHandling
    BOOL InitUsbDeviceInfoHandling(CFtd2xxDeviceInfoHandling* pUsbDeviceInfoHandling);

private:
    //Initialisation
    BOOL InitBaudrateSelection();
    BOOL InitDefaultPortSettings();
    BOOL InitErrorHandling();

    //Settings
    BOOL ConfigurePortSettings(DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo, BOOL oDoLock);
    BOOL ConfigurePortMode(WORD wPortMode, CErrorInfo* pErrorInfo, BOOL oDoLock);

    //Commands
    BOOL Process_ReadData(CCommand_I* pCommand);
    BOOL Process_WriteData(CCommand_I* pCommand);
    BOOL Process_EE_Read(CCommand_I* pCommand);
    BOOL Process_EE_Program(CCommand_I* pCommand);
    BOOL Process_Rescan(CCommand_I* pCommand);
    BOOL Process_Reload(CCommand_I* pCommand);

    BOOL PurgeBuffer();
    BOOL SetTimeout(DWORD p_ulTimeout);
    BOOL SetBaudrate(DWORD p_ulBaudrate);
    BOOL ReadData(void* p_pDataBuffer, DWORD p_dNbOfBytesToRead, DWORD* p_pdNbOfBytesRead, CErrorInfo* p_pErrorInfo = 0);
    BOOL WriteData(void* p_pDataBuffer, DWORD p_dNbOfBytesToWrite, DWORD* p_pdNbOfBytesWritten, CErrorInfo* p_pErrorInfo = 0);

    BOOL Rescan(CErrorInfo* pErrorInfo = 0);
    BOOL Reload(WORD p_usVendorId, WORD p_usProductId, CErrorInfo* pErrorInfo = 0);

    //DeviceInfos
    BOOL UpdatePortList(CFtd2xxDeviceInfoHandling* pDeviceInfoHandling);
    BOOL GetLocationId(CStdString p_PortName, DWORD& p_rulLocationId);
    BOOL GetSerialNumber(CStdString p_PortName, CStdString& p_serialNumber);
    BOOL UpdateLocationId(CFtd2xxDeviceInfoHandling* pDeviceInfoHandling, CStdString p_PortName, DWORD& p_rulLocationId);
    BOOL GetDeviceInfos(tDeviceInfoList& p_rDeviceInfoList);
    BOOL DeleteDeviceInfoList(tDeviceInfoList& p_rDeviceInfoList);
    BOOL UpdatePortOpened(CStdString p_PortName, BOOL p_oOpened);

    BOOL ResetRxBuffer();
    BOOL GetRxData(void* pBuffer, DWORD dNbOfBytesToRead, DWORD* pdNbOfBytesRead);

    //DriverStatus
    CStdString GetFtStatusDescription(unsigned long status);

private:
    BOOL m_oFirstInit;
    BOOL m_oUpdatePortOpened;

    //RxBuffer
    CRxBufferThread* m_pRxBufferThread;
    BYTE m_RxBuffer[10000];
    DWORD m_dRxBufferCount;
    int m_iGetIndex;
    int m_iPutIndex;

	CMmcUsbHndl	m_UsbHndl;
    CMmcCriticalSection m_Sync;

    //UsbDeviceInfoHandling
    CFtd2xxDeviceInfoHandling* m_pUsbDeviceInfoHandling;
};

#endif // !defined(AFX_GatewayUSBToFtd2xxDrv_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
