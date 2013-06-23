#if !defined(AFX_VIRTUALCOMMANDSET_MANAGER_H__CA8030A4_8ED9_40D4_AEC0_425C27339CA6__INCLUDED_)
#define AFX_VIRTUALCOMMANDSET_MANAGER_H__CA8030A4_8ED9_40D4_AEC0_425C27339CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/VirtualCommandSet/BaseClasses/VirtualCommandSet_ManagerBase.h>
#include <CommunicationModel/VirtualCommandSet/Classes/Handle/HandleRegistrationMap_VCS.h>
#include <MmcTypeDefinition.h>
#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

class CVirtualDeviceBase;
class CDeviceCommandSetManagerBase;
class CProtocolStackManagerBase;
class CInterfaceManagerBase;

class CVirtualDeviceEpos_VCS;
class CLayerParameterStack;
class CErrorInfo;

class CVirtualCommandSet_Manager  : public CVirtualCommandSet_ManagerBase
{
public:
//Internal Structure Functions
    virtual BOOL VCS_InitVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_InitBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdDWordArray& dBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_InitDefaultProtocolStackSettings(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, DWORD dBaudrate, DWORD dTimeout, CErrorInfo* p_pErrorInfo = NULL);

//Initialization
    virtual HANDLE VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseVirtualDevice(HANDLE hVCS_Handle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseAllVirtualDevices(CErrorInfo* p_pErrorInfo = NULL);

    virtual HANDLE VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, HANDLE hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseVirtualDevice(HANDLE hVCS_Handle, HANDLE hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_CloseAllVirtualDevices(HANDLE hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);

	virtual BOOL VCS_AreAllVirtualDevicesClosed();

//Parameter
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo = NULL);

//Hilfsfunktionen
    virtual BOOL VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CLayerParameterStack& p_rAddressParameter, HANDLE* p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetBaseLayerKeyHandle(HANDLE p_hVCS_Handle, HANDLE* p_phPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetDefaultProtocolStackSettings(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD* p_pdBaudrate, DWORD* p_pdTimeout, CErrorInfo* p_pErrorInfo = NULL);

//Selection Funktionen
    virtual BOOL VCS_GetVirtualDeviceNameSelection(CStdStringArray* pVirtualDeviceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceNameSelection(CStdString strVirtualDeviceName, CStdStringArray* pDeviceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdStringArray* pInterfaceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetPortNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdStringArray* pPortSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);

//Setting Funktionen
    virtual BOOL VCS_GetProtocolStackSettings(HANDLE hVCS_Handle, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_SetProtocolStackSettings(HANDLE hVCS_Handle, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

//Name Funktionen
    virtual BOOL VCS_GetVirtualDeviceName(HANDLE hVCS_Handle, CStdString* pVirtualDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceName(HANDLE hVCS_Handle, CStdString*  pDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackName(HANDLE hVCS_Handle, CStdString*  pProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceName(HANDLE hVCS_Handle, CStdString* pInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL VCS_GetPortName(HANDLE hVCS_Handle, CStdString* pPortName, CErrorInfo* p_pErrorInfo = NULL);

//Funktionalit�t
    virtual BOOL ExecuteCommand(CCommandRoot *pCommand, HANDLE hHandle, HANDLE hTransactionHandle);
    virtual BOOL GetCommands(HANDLE hHandle, ELayer eLayer, CStdString* pCommandInfo);
    virtual BOOL AbortCommands(HANDLE hHandle, BOOL oActive);

//JournalManager
    BOOL InitJournalManager(HANDLE hHandle, CJournalManagerBase *pJournalManager);
    BOOL ResetJournalManager(HANDLE hHandle);
    void InitJournalManager(CJournalManagerBase *pJournalManager);
    void ResetJournalManager();

//Tracing
    virtual BOOL EnableTracing(HANDLE hVCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DisableTracing(HANDLE hVCS_Handle, CErrorInfo* p_pErrorInfo = NULL);

//LayerManager
    virtual BOOL GetLayerManager(HANDLE hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle);
    virtual BOOL SetDeviceCommandSetManager(CStdString strVirtualDeviceName, CDeviceCommandSetManagerBase* pDeviceCommandSetManager);
    virtual BOOL GetDeviceCommandSetManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CDeviceCommandSetManagerBase** ppDeviceCommandSetManager);
    virtual BOOL SetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CProtocolStackManagerBase* pProtocolStackManager);
    virtual BOOL GetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CProtocolStackManagerBase** ppProtocolStackManager);
    virtual BOOL SetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CInterfaceManagerBase* pInterfaceManager);
    virtual BOOL GetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CInterfaceManagerBase** ppInterfaceManager);

    CVirtualCommandSet_Manager();
    CVirtualCommandSet_Manager(int p_lInstanceValue);
    CVirtualCommandSet_Manager(const CVirtualCommandSet_Manager& rObject);
    virtual ~CVirtualCommandSet_Manager();
    virtual CVirtualCommandSet_ManagerBase* Clone();

private:
    void DeleteVirtualDeviceList();
    BOOL InitErrorHandling();

    BOOL GetVirtualDevice(CStdString strVirtualDeviceName, CVirtualDeviceBase*& p_rpVirtualDevice, BOOL& p_roNewCreated, CErrorInfo* p_pErrorInfo);

    //Singleton RegistrationMap
    BOOL InitRegistrationMap();
    BOOL ReleaseRegistrationMap();

    BOOL FindCorrectManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager);
    BOOL FindVirtualDevice(CStdString strVirtualDeviceName, CVirtualDeviceBase** ppVirtualDevice);

private:
    CHandleRegistrationMap_VCS* m_pHandleRegistrationMap_VCS;
    std::list<CVirtualDeviceBase*> m_VirtualDeviceList;
};

#endif // !defined(AFX_VIRTUALCOMMANDSET_MANAGER_H__CA8030A4_8ED9_40D4_AEC0_425C27339CA6__INCLUDED_)
