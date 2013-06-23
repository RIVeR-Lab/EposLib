// GatewayRS232ToWin32.h: Schnittstelle für die Klasse CGatewayRS232ToWin32.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayRS232ToWin32_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayRS232ToWin32_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <Drv/Rs232/MmcRs232Hndl.h>
#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/GatewayIToDrv.h>

class CCommand_I;

class CGatewayRS232ToWin32 : public CGatewayIToDrv
{
public:
	CGatewayRS232ToWin32();
	virtual ~CGatewayRS232ToWin32();

	virtual CGateway* Clone();
	CGatewayRS232ToWin32& operator=(CGatewayRS232ToWin32& other);

//Interne Struktur Funktionen
	virtual BOOL InitPort(WORD wBoardNumber, WORD wNbBoardWithOldDriver);

//Initialisation
	virtual BOOL OpenPort(CStdString strPortName, CErrorInfo* pErrorInfo = 0);
	virtual BOOL ClosePort(CErrorInfo* pErrorInfo = 0);

//Hilfsfunktionen
	virtual BOOL ResetPort(CErrorInfo* pErrorInfo = 0);

//Setting Funktionen
	virtual BOOL SetPortSettings(DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo = 0);
	virtual BOOL GetPortSettings(DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo = 0);

	virtual BOOL SetPortMode(WORD wPortMode, CErrorInfo* pErrorInfo = 0);
	virtual BOOL GetPortMode(WORD* pwPortMode, CErrorInfo* pErrorInfo = 0);

//Selection
	virtual BOOL GetPortModeSelection(CStdStringArray* pPortModeSel, CErrorInfo* pErrorInfo = 0);

//Name Funktionen
	virtual BOOL InitInterfacePortName(CStdString* pStrInterfacePortName, WORD wBoardNumber, WORD wNbBoardWithOldDriver);
	virtual BOOL InitInterfaceName(CStdString* pStrInterfaceName, WORD wBoardNumber, WORD wNbBoardWithOldDriver);

//Funktionalität
	virtual BOOL ProcessCommand(CCommandRoot*pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

private:
	BOOL InitBaudrateSelection();
	BOOL InitDefaultPortSettings();
	BOOL InitErrorHandling();
	BOOL Process_ReadData(CCommand_I* pCommand);
	BOOL Process_WriteData(CCommand_I* pCommand);
	BOOL PurgeBuffer();
	BOOL ReadData(void* pDataBuffer, DWORD dNbOfBytesToRead, DWORD* pdNbOfBytesRead, CErrorInfo* pErrorInfo = 0);
	BOOL SetTimeout(DWORD dTimeout);
	DWORD GetIntervalTimeout(DWORD dTimeout);
	BOOL WriteData(void* pDataBuffer, DWORD dNbOfBytesToWrite, DWORD* pdNbOfBytesWritten, CErrorInfo* pErrorInfo = 0);
	void ConvertPortName(CStdString portName, char *winApiPortName, DWORD bufferSize);

	CMmcRs232Hndl m_Rs232Hndl;

	BOOL	m_oFirstInit;
};


#endif // !defined(AFX_GatewayRS232ToWin32_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
