// ProtocolStackBase.h: Schnittstelle für die Klasse CProtocolStackBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOLSTACKBASE_H__3FC11BE3_0F17_4811_9C71_23290EBAC2FC__INCLUDED_)
#define AFX_PROTOCOLSTACKBASE_H__3FC11BE3_0F17_4811_9C71_23290EBAC2FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <MmcTypeDefinition.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>
#include "../InterfacePlugInManager/InterfacePlugInManager.h"

class CGatewayPStoI;
class CInterfaceManagerBase;
class CGatewayPStoDCS;
class CDeviceCommandSetManagerBase;
class CLayerManagerBase;
class CJournalManagerBase;
class CCommandRoot;

class CProtocolStackBase
{
public:
    CProtocolStackBase();
    CProtocolStackBase(const CProtocolStackBase& rObject);
    virtual ~CProtocolStackBase();
    virtual CProtocolStackBase* Clone();
    virtual BOOL IsEqual(CProtocolStackBase* p_pProtocolStack);

//Hilfsfunktionen
    virtual BOOL PS_GetInterfaceNameSelection(CStdStringArray* pInterfaceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetPortNameSelection(CStdString p_InterfaceName, CStdStringArray* pPortSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetBaudrateSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackModeSelection(CStdStringArray* pProtocolStackModeSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetDefaultProtocolStackSettings(CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetDefaultProtocolStackSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL PS_GetDeviceNameSelection(CStdStringArray* pDeviceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_GetProtocolStackNameSelection(CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);

    virtual BOOL PS_GetProtocolStackName(CStdString* pProtocolStackName);

    virtual BOOL PS_GetProtocolStackMode(WORD* pwModeIndex, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL PS_SetProtocolStackMode(WORD wModeIndex, CErrorInfo* p_pErrorInfo = NULL);

//Interne Funktionen
    virtual BOOL AddManager(CInterfaceManagerBase* pManager);
    virtual BOOL FindCorrectManager(CStdString p_InterfaceName, CInterfaceManagerBase** ppManager);
    virtual BOOL FindCorrectLibrary(CStdString p_InterfaceName, CInterfaceManagerBase** ppManager);
    virtual BOOL InitProtocolStack(CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL InitProtocolStack(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL UpdateProtocolStack(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL AddPort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DeletePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL InitGatewayToInterface(CStdString p_InterfaceName);

    virtual BOOL AddManager(CDeviceCommandSetManagerBase* pManager);
    virtual BOOL FindCorrectManager(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager);
    virtual BOOL FindCorrectLibrary(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager);
    virtual BOOL InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL InitGatewayToDevice(CStdString strDeviceName);

    virtual BOOL InitBaudrateSelection(CStdString p_InterfaceName, CStdDWordArray& p_rulBaudrateSel);
    virtual BOOL InitDefaultProtocolStackSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout);

    BOOL UpdateTimeout(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle);
	BOOL UpdateTimeout(CDeviceCommandSetManagerBase* pDeviceCommandSetManagerManager, HANDLE hDCS_Handle);
    BOOL SetTimeout(DWORD timeout);
    DWORD GetTimeout();
    DWORD GetEvaluatedTimeout(DWORD dCommandTimeout);
    BOOL SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, DWORD p_ulTimeout);

//Funktionalität
    virtual BOOL ExecuteCommand(CCommandRoot* pCommand, CLayerManagerBase* pLayerManager, HANDLE h_Handle, HANDLE hTransactionHandle);
    virtual BOOL GetCommands(CStdString* pCommandInfo);
    virtual BOOL AbortCommands(BOOL oActive);

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

//ErrorHandling
    virtual BOOL InitErrorHandling();
    virtual BOOL DeleteErrorHandling();

//Instance Value for Singletons
    virtual BOOL InitInstanceValue(int p_lInstanceValue);

//Parameter
    BOOL AreParameterEqual(CProtocolStackBase* p_pDevice);
    BOOL SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
    BOOL SetParameter(CStdString p_Name, CStdString p_Value);
    BOOL GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
    BOOL GetParameter(CStdString p_Name, CStdString& p_rValue);

//Critical Section
    BOOL Lock(DWORD p_ulTimeout = -1);
    BOOL Unlock();
    BOOL IsLocked();

//Logging
	BOOL WriteToLogFile(CStdString p_Action);

private:
    BOOL AppendSelectionArray(CStdStringArray* pDestArray, CStdStringArray* pSourceArray);
    virtual BOOL VerifyGatewayToInterface(CStdString p_InterfaceName);
    virtual BOOL VerifyGatewayToDevice(CStdString strDeviceName);

protected:
    BOOL CreatePlugInManager();
    BOOL DeletePlugInManager();

    BOOL GetFirstManager(CInterfaceManagerBase** ppManager);
    BOOL GetFirstManager(CDeviceCommandSetManagerBase** ppManager);

    void DeleteInterfaceManagerList();
    void DeleteDeviceCommandSetManagerList();
    void DeleteGatewayToInterface();
    void DeleteGatewayToDevice();

protected:
    CStdString m_strProtocolStackName;
    DWORD m_dTimeout;
    int m_lInstanceValue;

    CErrorHandling* m_pErrorHandling;
    CInterfacePlugInManager* m_pPlugInManager;
    CJournalManagerBase* m_pJournalManager;

    //ProtocolStack -> Interface
    CGatewayPStoI* m_pGatewayToInterface;
    std::list<CInterfaceManagerBase*> m_InterfaceManagerList;

    //ProtocolStack -> Device -> ProtocolStack (Router)
    CGatewayPStoDCS* m_pGatewayToDevice;
    std::list<CDeviceCommandSetManagerBase*> m_DeviceCommandSetManagerList;
};

#endif // !defined(AFX_PROTOCOLSTACKBASE_H__3FC11BE3_0F17_4811_9C71_23290EBAC2FC__INCLUDED_)
