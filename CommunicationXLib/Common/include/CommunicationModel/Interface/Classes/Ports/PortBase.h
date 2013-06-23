// PortBase.h: Schnittstelle f�r die Klasse CPortBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTBASE_H__489D7B2F_6A13_49FA_BE0D_0FCFF7F656F1__INCLUDED_)
#define AFX_PORTBASE_H__489D7B2F_6A13_49FA_BE0D_0FCFF7F656F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>

class CJournalManagerBase;
class CGatewayIToDrv;
class CCommandRoot;
class CPortBase;

typedef std::list<CPortBase*> tPortList;

class CPortBase
{
public:
	CPortBase();
    CPortBase(const CPortBase& rObject);
    virtual ~CPortBase();
    virtual CPortBase* Clone();
	virtual BOOL IsEqual(CStdString p_PortName);

//Interne Struktur Funktionen
    virtual BOOL InitPort(WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver);
    virtual BOOL InitGateway(CGatewayIToDrv* p_pGateway);
    virtual BOOL InitBaudrateSelection(CStdDWordArray& p_rulBaudrateSel);
    virtual BOOL InitDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout);
    virtual BOOL UpdatePort(tPortList& p_rOpenPortList);
    virtual BOOL ReopenPort(CErrorInfo* p_pErrorInfo = NULL);

//Initialisation
    virtual BOOL OpenPort(CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL ClosePort(CErrorInfo* p_pErrorInfo = NULL);

//Hilfsfunktionen
    virtual BOOL GetDefaultPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL ResetPort(CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL IsPortNameSupported(CStdString p_PortName);
    virtual BOOL SetDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);

//Selection Funktionen
    virtual BOOL GetPortNameSelection(CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetBaudrateSelection(CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetPortModeSelection(CStdStringArray* p_pPortModeSel, CErrorInfo* p_pErrorInfo = NULL);

//Setting Funktionen
    virtual BOOL GetPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL SetPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL GetPortMode(WORD* p_pusPortMode, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL SetPortMode(WORD wPortMode, CErrorInfo* p_pErrorInfo = NULL);

//Name Funktionen
    virtual BOOL SetPortName(CStdString p_PortName);
    virtual BOOL GetPortName(CStdString* p_pPortName);
    virtual BOOL GetInterfacePortName(CStdString* p_pInterfacePortName);
    virtual BOOL InitInterfacePortName(WORD p_usBoardNumber = 0, WORD p_usNbBoardWithOldDriver = 0);

//Funktionalität
    virtual BOOL ExecuteCommand(CCommandRoot* p_pCommand, HANDLE p_hTransactionHandle);
    virtual BOOL GetCommands(CStdString* p_pCommandInfo);

//JournalManager
    virtual BOOL InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* p_pJournalManager);
    virtual BOOL ResetJournalManager(HANDLE p_hHandle);
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Tracing
    virtual BOOL EnableTracing(CStdString p_FileName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DisableTracing(CErrorInfo* p_pErrorInfo = NULL);

//ErrorHandling
    virtual BOOL InitErrorHandling();
    virtual BOOL DeleteErrorHandling();

//Parameter
	BOOL AreParameterEqual(CPortBase* p_pDevice);
    BOOL SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL SetParameter(CStdString p_Name, CStdString p_Value);
	BOOL GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL GetParameter(CStdString p_Name, CStdString& p_rValue);

//Critical Section
    BOOL Lock(DWORD p_ulTimeout = -1);
    BOOL Unlock();
    BOOL IsLocked();

protected:
    void DeleteGateway();

    CErrorHandling* m_pErrorHandling;
    CGatewayIToDrv* m_pGateway;
    CJournalManagerBase* m_pJournalManager;
    CStdString m_strInterfacePortName;
    CStdString m_strPortName;
};

#endif // !defined(AFX_PORTBASE_H__489D7B2F_6A13_49FA_BE0D_0FCFF7F656F1__INCLUDED_)
