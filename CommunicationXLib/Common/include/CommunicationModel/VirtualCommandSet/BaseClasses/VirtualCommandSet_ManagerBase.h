// VirtualCommandSet_ManagerBase.h: Schnittstelle f�r die Klasse CVirtualCommandSet_ManagerBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualCommandSet_ManagerBase_H__6935E87D_3EC5_4E37_B0E5_6B803D3C4A60__INCLUDED_)
#define AFX_VirtualCommandSet_ManagerBase_H__6935E87D_3EC5_4E37_B0E5_6B803D3C4A60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/LayerManagerBase.h>
#include <MmcTypeDefinition.h>

class CInterfaceManagerBase;
class CProtocolStackManagerBase;
class CDeviceCommandSetManagerBase;
class CErrorInfo;
class CJournalManagerBase;

class CVirtualCommandSet_ManagerBase : public CLayerManagerBase
{
public:
//Internal Structure Functions
    virtual BOOL VCS_InitVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_InitBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdDWordArray& dBaudrateSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_InitDefaultProtocolStackSettings(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, DWORD dBaudrate, DWORD dTimeout, CErrorInfo* pErrorInfo = NULL);

//Initialization
    virtual HANDLE VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL VCS_CloseVirtualDevice(HANDLE hVCS_Handle, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL VCS_CloseAllVirtualDevices(CErrorInfo* pErrorInfo = NULL);
	
	virtual HANDLE VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, HANDLE hPS_BaseLayerHandle, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL VCS_CloseVirtualDevice(HANDLE hVCS_Handle, HANDLE hPS_BaseLayerHandle, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL VCS_CloseAllVirtualDevices(HANDLE hPS_BaseLayerHandle, CErrorInfo* pErrorInfo = NULL);

	virtual BOOL VCS_AreAllVirtualDevicesClosed();

//Parameter
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);

//Hilfsfunktionen
    virtual BOOL VCS_GetKeyHandle(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, HANDLE* pVCSKeyHandle, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetDefaultProtocolStackSettings(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo = NULL);

//Selection Funktionen
    virtual BOOL VCS_GetVirtualDeviceNameSelection(CStdStringArray* pVirtualDeviceNameSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceNameSelection(CStdString strVirtualDeviceName, CStdStringArray* pDeviceNameSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdStringArray* pInterfaceNameSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetPortNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdStringArray* pPortSel, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* pErrorInfo = NULL);

//Setting Funktionen
    virtual BOOL VCS_GetProtocolStackSettings(HANDLE hVCS_Handle, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_SetProtocolStackSettings(HANDLE hVCS_Handle, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo = NULL);

//Name Funktionen
    virtual BOOL VCS_GetVirtualDeviceName(HANDLE hVCS_Handle, CStdString* pVirtualDeviceName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetDeviceName(HANDLE hVCS_Handle, CStdString* pDeviceName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetProtocolStackName(HANDLE hVCS_Handle, CStdString* pProtocolStackName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetInterfaceName(HANDLE hVCS_Handle, CStdString* pInterfaceName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL VCS_GetPortName(HANDLE hVCS_Handle, CStdString* pPortName, CErrorInfo* pErrorInfo = NULL);

//Funktionalit�t
	virtual BOOL ExecuteCommand(CCommandRoot *pCommand, HANDLE hHandle, HANDLE hTransactionHandle);

	virtual BOOL GetCommands(HANDLE hHandle,ELayer eLayer,CStdString* pCommandInfo);

//Tracing
    virtual BOOL EnableTracing(HANDLE hVCS_Handle, CStdString p_TracingFileName, CErrorInfo* pErrorInfo = NULL);
    virtual BOOL DisableTracing(HANDLE hVCS_Handle, CErrorInfo* pErrorInfo = NULL);

//LayerManager
    virtual BOOL GetLayerManager(HANDLE hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle);
    virtual BOOL SetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CInterfaceManagerBase* pInterfaceManager);
    virtual BOOL GetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CInterfaceManagerBase** ppInterfaceManager);
    virtual BOOL SetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CProtocolStackManagerBase* pProtocolStackManager);
    virtual BOOL GetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CProtocolStackManagerBase** ppProtocolStackManager);
    virtual BOOL SetDeviceCommandSetManager(CStdString strVirtualDeviceName, CDeviceCommandSetManagerBase* pDeviceCommandSetManager);
    virtual BOOL GetDeviceCommandSetManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CDeviceCommandSetManagerBase** ppDeviceCommandSetManager);

    CVirtualCommandSet_ManagerBase();
	CVirtualCommandSet_ManagerBase(int p_lInstanceValue);
    CVirtualCommandSet_ManagerBase(const CVirtualCommandSet_ManagerBase& rObject);
    virtual ~CVirtualCommandSet_ManagerBase();
    virtual CVirtualCommandSet_ManagerBase* Clone();
};

#endif // !defined(AFX_VirtualCommandSet_ManagerBase_H__6935E87D_3EC5_4E37_B0E5_6B803D3C4A60__INCLUDED_)
