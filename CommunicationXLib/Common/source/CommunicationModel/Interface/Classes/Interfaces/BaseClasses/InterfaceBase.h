// InterfaceBase.h: Schnittstelle f�r die Klasse CInterfaceBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACEBASE_H__8D42697B_4ADF_4F33_A594_245344C95BCB__INCLUDED_)
#define AFX_INTERFACEBASE_H__8D42697B_4ADF_4F33_A594_245344C95BCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <StdString.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>
#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/DeviceInfoHandlingBase.h>

class CJournalManagerBase;
class CCommandRoot;
class CGatewayIToDrv;
class CErrorInfo;
class CPortBase;

class CInterfaceBase
{
public:
	CInterfaceBase();
    CInterfaceBase(const CInterfaceBase& rObject);

	virtual void Init() = 0;
	bool IsKindOf(CStdString strClassType);
    CStdString GetClassType();

    virtual ~CInterfaceBase();
    virtual CInterfaceBase* Clone();
	virtual BOOL IsEqual(CInterfaceBase* p_pInterface);

    virtual BOOL InitInterface(WORD wBoardNumber = 0, WORD wNbBoardWithOldDriver = 0);
    virtual BOOL InitInterfaceMode(std::list<DWORD> p_rNbOfAvailableBoards, int p_Index, WORD & p_rwNbBoardWithOldDriver);
    virtual BOOL InitGateway();
    virtual BOOL InitBaudrateSelection(CStdDWordArray& dBaudrateSel);
    virtual BOOL InitDefaultInterfaceSettings(DWORD dBaudrate, DWORD dTimeout);
    virtual BOOL UpdateInterface(tPortList& p_rOpenPortList);

//Initialisation
    virtual BOOL I_OpenInterface(CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_OpenInterfacePort(CPortBase* pPort, CStdString strPortName, CErrorInfo* pErrorInfo = NULL);

    virtual BOOL I_CloseInterface(CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_CloseInterfacePort(CPortBase* pPort, CErrorInfo* pErrorInfo = NULL);

//Hilfsfunktionen
    virtual BOOL I_GetDefaultInterfaceSettings(DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_SetDefaultInterfaceSettings(DWORD dBaudrate, DWORD dTimeout, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_ResetInterface(CPortBase* pPort, CErrorInfo* pErrorInfo = NULL);

    virtual BOOL I_FindInterfacePortIndex(CStdString strPortName, short* piPortIndex);
    virtual BOOL I_FindPort(CStdString strInterfacePortName, CStdString strPortName, CPortBase** ppPort);
    virtual BOOL I_AddPort(CStdString p_PortName);
    virtual BOOL I_DeletePort(CStdString p_PortName);

//Selection Funktionen
    virtual BOOL I_GetPortNameSelection(CStdStringArray* pPortSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_GetInterfaceModeSelection(CStdStringArray* pInterfaceModeSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_GetBaudrateSelection(CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* pErrorInfo = NULL);

//Setting Funktionen
    virtual BOOL I_GetInterfaceSettings(CPortBase* pPort, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_SetInterfaceSettings(CPortBase* pPort, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo = NULL);

    virtual BOOL I_GetInterfaceMode(WORD* pwModeIndex, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_SetInterfaceMode(WORD wModeIndex, CErrorInfo* pErrorInfo = NULL);

//Name Funktionen
    virtual BOOL I_GetInterfaceName(CStdString* pInterfaceName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL I_GetPortName(CPortBase* pPort, CStdString* pPortName, CErrorInfo* pErrorInfo = NULL);

//Funktionalit�t
    virtual BOOL ExecuteCommand(CCommandRoot* pCommand, CPortBase* pPort, HANDLE hTransactionHandle);
    virtual BOOL AbortCommands(BOOL oActive);

//Tracing
    virtual BOOL EnableTracing(CPortBase* pPort, CStdString p_FileName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL DisableTracing(CPortBase* pPort, CErrorInfo* pErrorInfo = NULL);

//ErrorHandling
    virtual BOOL InitErrorHandling();
    virtual BOOL DeleteErrorHandling();

//Instance Value for Singletons
	virtual BOOL InitInstanceValue(int p_lInstanceValue);
    virtual BOOL InitInfoHandling(CDeviceInfoHandlingBase* p_pInfoHandling);
    virtual std::list<DWORD> GetNbOfAvailableBoards();

//Parameter
	BOOL AreParameterEqual(CInterfaceBase* p_pDevice);
    BOOL SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL SetParameter(CStdString p_Name, CStdString p_Value);
	BOOL GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL GetParameter(CStdString p_Name, CStdString& p_rValue);

//Critical Section
    BOOL Lock(DWORD p_ulTimeout = -1);
    BOOL Unlock();
    BOOL IsLocked();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

protected:
    BOOL AppendSelectionArray(CStdStringArray* pDestArray, CStdStringArray* pSourceArray);
    void DeleteGateway();

    virtual BOOL InitInterfaceName(WORD wBoardNumber = 0, WORD wNbBoardWithOldDriver = 0);
    virtual void DeletePortList();

protected:
	int						m_lInstanceValue;
    CErrorHandling*			m_pErrorHandling;
    CGatewayIToDrv*			m_pGateway;
    CJournalManagerBase*	m_pJournalManager;
    CStdString				m_strInterfaceName;
    std::list<CPortBase*>	m_PortList;
    WORD					m_wModeIndex;
	CStdString				m_strClassType;
};

#endif // !defined(AFX_INTERFACEBASE_H__8D42697B_4ADF_4F33_A594_245344C95BCB__INCLUDED_)
