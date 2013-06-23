// DeviceBase.h: Schnittstelle f�r die Klasse CDeviceBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DeviceBase_H__FE1798F8_F4C2_42F8_885E_18A52376DE22__INCLUDED_)
#define AFX_DeviceBase_H__FE1798F8_F4C2_42F8_885E_18A52376DE22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>
#include <list>

class CJournalManagerBase;
class CProtocolStackManagerBase;
class CGatewayDCStoPS;
class CCommandRoot;

class CDeviceBase
{
public:
	CDeviceBase();
	CDeviceBase(const CDeviceBase& rObject);
	virtual ~CDeviceBase();
	virtual CDeviceBase* Clone();
	virtual BOOL IsEqual(CDeviceBase* p_pDevice);

//Hilfsfunktionen
	virtual BOOL DCS_GetProtocolStackNameSelection(CStdStringArray* pProtocolStackNameSel,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL DCS_GetInterfaceNameSelection(CStdString strProtocolStackName,CStdStringArray* pInterfaceNameSel,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL DCS_GetPortNameSelection(CStdString strProtocolStackName,CStdString strInterfaceName,CStdStringArray* pPortSel,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL DCS_GetBaudrateSelection(CStdString strProtocolStackName,CStdString strInterfaceName,CStdString strPortName,CStdDWordArray* pdBaudrateSel,CErrorInfo* pErrorInfo = NULL);

	virtual BOOL DCS_GetDeviceName(CStdString* pDeviceName);

//Interne Funktionnen
	virtual BOOL AddManager(CProtocolStackManagerBase* pManager);
	virtual BOOL FindCorrectManager(CStdString strProtocolStackName,CProtocolStackManagerBase** ppManager);
	virtual BOOL FindCorrectManager(HANDLE hPS_BaseLayerHandle, CProtocolStackManagerBase** ppManager);
	virtual BOOL FindCorrectLibrary(CStdString strProtocolStackName,CProtocolStackManagerBase** ppManager);
	virtual BOOL FindCorrectLibrary(HANDLE hPS_BaseLayerHandle, CProtocolStackManagerBase** ppManager);

	virtual BOOL InitDevice(CErrorInfo* pErrorInfo = NULL);
	virtual BOOL InitDevice(CStdString strProtocolStack,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL InitDevice(CStdString strProtocolStack,CStdString strInterfaceName,CErrorInfo* pErrorInfo = NULL);
	virtual BOOL InitGateway(CStdString strProtocolStackName);
	virtual BOOL InitBaudrateSelection(CStdString strProtocolStack,CStdString strInterfaceName,CStdDWordArray& dBaudrateSel);
	virtual BOOL InitDefaultProtocolStackSettings(CStdString strProtocolStack,CStdString strInterfaceName,DWORD dBaudrate,DWORD dTimeout);

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

//Funktionalit�t
	virtual BOOL ExecuteCommand(CCommandRoot*pCommand,CProtocolStackManagerBase* pProtocolStackManager,HANDLE hPS_Handle,HANDLE hTransactionHandle);
	virtual BOOL GetCommands(CStdString* pCommandInfo);
	virtual BOOL AbortCommands(BOOL oActive);

//ErrorHandling
	virtual BOOL InitErrorHandling();
	virtual BOOL DeleteErrorHandling();

//Instance Value for Singletons
	virtual BOOL InitInstanceValue(int p_lInstanceValue);

//Parameter
	BOOL AreParameterEqual(CDeviceBase* p_pDevice);
    BOOL SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL SetParameter(CStdString p_Name, CStdString p_Value);
	BOOL GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL GetParameter(CStdString p_Name, CStdString& p_rValue);

//Critical Section
    BOOL Lock(DWORD p_ulTimeout = -1);
    BOOL Unlock();
    BOOL IsLocked();
    
protected:
	BOOL GetFirstManager(CProtocolStackManagerBase** ppManager);
	void DeletePrototcolStackManagerList();
	void DeleteGateway();
	BOOL InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName);
	virtual BOOL InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName);

private:
	virtual BOOL VerifyGateway(CStdString strProtocolStackName);
	BOOL AppendSelectionArray(CStdStringArray* pDestArray,CStdStringArray* pSourceArray);

protected:
	CStdString m_strDeviceName;
	CJournalManagerBase* m_pJournalManager;

	std::list<CProtocolStackManagerBase*> m_ProtocolStackManagerList;
	CGatewayDCStoPS* m_pGateway;

	int m_lInstanceValue;
	CErrorHandling* m_pErrorHandling;
};

#endif // !defined(AFX_DeviceBase_H__FE1798F8_F4C2_42F8_885E_18A52376DE22__INCLUDED_)
