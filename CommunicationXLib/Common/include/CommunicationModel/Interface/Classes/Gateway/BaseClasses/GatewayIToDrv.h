// GatewayIToDrv.h: Schnittstelle für die Klasse CGatewayIToDrv.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayIToDrv_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
#define AFX_GatewayIToDrv_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../../CommonLayer/Classes/Gateway/Gateway.h"
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>

class CCommand_PS;
class CInterfaceManagerBase;

class CGatewayIToDrv: public CGateway
{
public:
	virtual WORD GetNbOfAvailableBoards();
    virtual BOOL AddPort(CStdString p_PortName);
    virtual BOOL DeletePort(CStdString p_PortName);

	CGatewayIToDrv();
	virtual ~CGatewayIToDrv();
	virtual CGateway* Clone();
	virtual CGatewayIToDrv& operator=(CGatewayIToDrv& p_rOther);

//Interne Struktur Funktionen
	virtual BOOL InitPort(WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver);
	virtual BOOL InitGateway();
	virtual BOOL InitBaudrateSelection(CStdDWordArray& p_rulBaudrateSel);
	virtual BOOL InitDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout);
	virtual BOOL UpdatePort(tPortList& p_rOpenPortList);

//Initialisation
	virtual BOOL OpenInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL CloseInterface(CErrorInfo* p_pErrorInfo = 0);

	virtual BOOL OpenPort(CStdString p_PortName, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL ReopenPort(CStdString p_PortName, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL ClosePort(CErrorInfo* p_pErrorInfo = 0);

//Hilfsfunktionen
	virtual BOOL ResetPort(CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL GetDefaultPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL IsPortNameSupported(CStdString p_PortName);
    virtual BOOL SetDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = 0);

//Selection Funktionen
	virtual BOOL GetPortNameSelection(CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL GetBaudrateSelection(CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL GetPortModeSelection(CStdStringArray* p_pPortModeSel, CErrorInfo* p_pErrorInfo = 0);

//Setting Funktionen
	virtual BOOL SetPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL GetPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = 0);

	virtual BOOL SetPortMode(WORD p_usPortMode, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL GetPortMode(WORD* p_pusPortMode, CErrorInfo* p_pErrorInfo = 0);

//Name Funktionen
	virtual BOOL InitInterfacePortName(CStdString* p_pInterfacePortName, WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver);
	virtual BOOL InitInterfaceName(CStdString* p_pInterfaceName, WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver);

//Funktionalität
	virtual BOOL ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE p_h_Handle, HANDLE hTransactionHandle);

//Tracing
	virtual BOOL EnableTracing(CStdString p_FileName, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL DisableTracing(CErrorInfo* p_pErrorInfo = 0);

protected:
	void AddPortList(CStdString p_PortName);
	void DeletePortList();
    void RemoveAtPortList(CStdString p_PortName);

	//Tracing
	BOOL Trace_Open(CStdString p_FileName);
	BOOL Trace_WriteData(BOOL p_oResult, void* p_pDataBuffer, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CStdString p_Description = _T(""));
	BOOL Trace_ReadData(BOOL p_oResult, void* p_pDataBuffer, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CStdString p_Description = _T(""));
	BOOL Trace_Settings(DWORD p_ulBaudrate, DWORD p_ulTimeout);
	BOOL Trace_Result(BOOL p_oResult, CStdString p_Description = _T(""));
	BOOL Trace_Close();

private:
	//Tracing
	BOOL Trace_FormatData(CStdString p_Function, BOOL p_oResult, void* p_pDataBuffer, DWORD p_ulNbOfBytesToDo, DWORD* p_pulNbOfBytesDone, BOOL p_oShowFailedData, CStdString p_Description, CStdString& p_rTraceLine);
	BOOL Trace_WriteLine(CStdString p_TraceLine);

protected:
	CStdString m_strInterfaceName;
	CStdString m_strInterfacePortName;
	CStdStringArray m_strPortArray;
	CStdDWordArray m_BaudrateSelection;

	DWORD m_dDefaultBaudrate;
	DWORD m_dDefaultTimeout;

	DWORD m_dBaudrate;
	DWORD m_dTimeout;

	CStdString m_PortName;
	WORD	m_wPortMode;

private:
	//Tracing
	CStdString		m_TraceFileName;
	std::ofstream	m_TraceFile;
	BOOL			m_oTraceFileOpen;
	DWORD			m_ulTraceLineCount;
	DWORD			m_ulStartTime;
	DWORD			m_ulLastTime;
	BOOL			m_oTracingEnabled;
};

#endif // !defined(AFX_GatewayIToDrv_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
