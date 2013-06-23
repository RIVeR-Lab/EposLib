// VirtualDeviceBase.h: Schnittstelle f�r die Klasse CVirtualDeviceBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualDeviceBase_H__7B04E854_C5C6_4BFB_9E92_3C0BEBF8438E__INCLUDED_)
#define AFX_VirtualDeviceBase_H__7B04E854_C5C6_4BFB_9E92_3C0BEBF8438E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>

class CJournalManagerBase;
class CDeviceCommandSetManagerBase;
class CProtocolStackManagerBase;
class CGatewayVCStoDCS;
class CCommandRoot;

class CVirtualDeviceBase
{
public:
	CVirtualDeviceBase();
    CVirtualDeviceBase(const CVirtualDeviceBase& rOjbect);
    virtual ~CVirtualDeviceBase();
    virtual CVirtualDeviceBase* Clone();
	virtual BOOL IsEqual(CVirtualDeviceBase* p_pVirtualDevice);

//Hilfsfunktionen
    virtual BOOL VCS_GetDeviceNameSelection(CStdStringArray* pDeviceNameSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackNameSelection(CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceNameSelection(CStdString strDeviceName, CStdString strProtocolStackName, CStdStringArray* pInterfaceNameSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetPortNameSelection(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdStringArray* pPortSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetBaudrateSelection(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* pErrorInfo = NULL);

    virtual BOOL VCS_GetVirtualDeviceName(CStdString* pVirtualDeviceName);

//Interne Funktionnen
    virtual BOOL AddManager(CDeviceCommandSetManagerBase* pManager);
    virtual BOOL FindCorrectManager(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager);
    virtual BOOL FindCorrectLibrary(CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager);
    virtual BOOL InitVirtualDevice(CErrorInfo* pErrorInfo = NULL);
    virtual BOOL InitVirtualDevice(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL InitGateway(CStdString strVirtualDeviceName);
    virtual BOOL InitBaudrateSelection(CStdString strDeviceName, CStdString strProtocolStack, CStdString strInterfaceName, CStdDWordArray& dBaudrateSel);
    virtual BOOL InitDefaultProtocolStackSettings(CStdString strDeviceName, CStdString strProtocolStack, CStdString strInterfaceName, DWORD dBaudrate, DWORD dTimeout);

//Funktionalit�t
    virtual BOOL ExecuteCommand(CCommandRoot*pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
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
	BOOL AreParameterEqual(CVirtualDeviceBase* p_pVirtualDevice);
    BOOL SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL SetParameter(CStdString p_Name, CStdString p_Value);
	BOOL GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL GetParameter(CStdString p_Name, CStdString& p_rValue);

//Critical Section
    BOOL Lock(DWORD p_ulTimeout = -1);
    BOOL Unlock();
    BOOL IsLocked();

protected:
    BOOL GetFirstManager(CDeviceCommandSetManagerBase** ppManager);
    void DeleteDCS_ManagerList();
    void DeleteGateway();
    std::list<CDeviceCommandSetManagerBase*> m_DeviceCommandSetManagerList;
    CStdString m_strVirtualDeviceName;
    CGatewayVCStoDCS* m_pGateway;
    CJournalManagerBase* m_pJournalManager;

    int m_lInstanceValue;
	CErrorHandling* m_pErrorHandling;

private:
    virtual BOOL VerifyGateway(CStdString strVirtualDeviceName);
    BOOL AppendSelectionArray(CStdStringArray* pDestArray, CStdStringArray* pSourceArray);
};

#endif // !defined(AFX_VirtualDeviceBase_H__7B04E854_C5C6_4BFB_9E92_3C0BEBF8438E__INCLUDED_)
