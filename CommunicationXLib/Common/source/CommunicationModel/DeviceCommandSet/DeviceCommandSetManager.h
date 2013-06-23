#if !defined(AFX_DEVICECOMMANDSET_H__E49EE49E_830C_4CED_8A20_94D3825CCEBD__INCLUDED_)
#define AFX_DEVICECOMMANDSET_H__E49EE49E_830C_4CED_8A20_94D3825CCEBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/DeviceCommandSetManagerBase.h"
#include "Classes/Handle/HandleRegistrationMap_DCS.h"

class CInterfaceManagerBase;
class CProtocolStackManagerBase;
class CDeviceBase;

class CDeviceCommandSetManager : public CDeviceCommandSetManagerBase
{
public:
//Internal Structure Funktionen
    virtual BOOL DCS_InitAllDevices(CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_InitDevice(CStdString p_strDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_InitDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_InitDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_InitBaudrateSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdDWordArray& p_ulBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_InitDefaultProtocolStackSettings(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD p_ulBaudrateSel, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);

//Initialisation
    virtual HANDLE DCS_OpenDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_CloseDevice(HANDLE p_hDCS_Handle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_CloseAllDevices(CErrorInfo* p_pErrorInfo = NULL);

    virtual HANDLE DCS_OpenDevice(CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_CloseDevice(HANDLE p_hDCS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_CloseAllDevices(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo = NULL);

	virtual BOOL DCS_AreAllDevicesClosed();

//Parameter
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo = NULL);

//Hilfsfunktionen
    virtual BOOL DCS_GetKeyHandle(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_pDCSKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetBaseLayerKeyHandle(HANDLE p_hDCS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetDefaultProtocolStackSettings(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_IsDeviceNameSupported(CStdString p_strDeviceName, CErrorInfo* p_pErrorInfo = NULL);

//Selection Funktionen
    virtual BOOL DCS_GetDeviceNameSelection(CStdStringArray* p_pDeviceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetProtocolStackNameSelection(CStdString p_strDeviceName, CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetInterfaceNameSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetPortNameSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdStringArray* p_pPortNameSel, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetBaudrateSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CStdDWordArray* p_pBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);

//Setting Funktionen
    virtual BOOL DCS_GetProtocolStackSettings(HANDLE p_hDCS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_SetProtocolStackSettings(HANDLE p_hDCS_Handle, DWORD p_ulBaudrateSel, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

//Name Funktionen
    virtual BOOL DCS_GetDeviceName(HANDLE p_hDCS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetProtocolStackName(HANDLE p_hDCS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetInterfaceName(HANDLE p_hDCS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DCS_GetPortName(HANDLE p_hDCS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo = NULL);

//Funktionalit�t
    virtual BOOL ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle);
    virtual BOOL GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo);
    virtual BOOL AbortCommands(HANDLE p_hHandle, bool oActive);

//Tracing
    virtual BOOL EnableTracing(HANDLE p_hPS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL DisableTracing(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo = NULL);

//LayerManager
    virtual BOOL GetLayerManager(HANDLE p_hHandle, ELayer p_eLayer, CLayerManagerBase** p_ppLayerManager, HANDLE* pLayerHandle);
    virtual BOOL SetInterfaceManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CInterfaceManagerBase* p_pInterfaceManager);
    virtual BOOL GetInterfaceManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CInterfaceManagerBase** p_ppInterfaceManager);
    virtual BOOL SetProtocolStackManager(CStdString p_strDeviceName, CProtocolStackManagerBase* p_pProtocolStackManager);
    virtual BOOL GetProtocolStackManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CProtocolStackManagerBase** p_ppProtocolStackManager);

//JournalManager
    BOOL InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* pJournalManager);
    BOOL ResetJournalManager(HANDLE p_hHandle);
    void InitJournalManager(CJournalManagerBase* pJournalManager);
    void ResetJournalManager();

    CDeviceCommandSetManager();
    CDeviceCommandSetManager(int p_lInstanceValue);
    CDeviceCommandSetManager(const CDeviceCommandSetManager& rObject);
    virtual ~CDeviceCommandSetManager();
    virtual CDeviceCommandSetManagerBase* Clone();

private:
    BOOL InitErrorHandling();
    void DeleteDeviceList();

    BOOL GetDevice(CStdString p_strDeviceName, CDeviceBase*& p_rpDevice, BOOL& p_roNewCreated, CErrorInfo* p_pErrorInfo);

    //Singleton RegistrationMap
    BOOL InitRegistrationMap();
    BOOL ReleaseRegistrationMap();

    BOOL FindDeviceIndex(CStdString p_strDeviceName, short* piDeviceIndex);
    BOOL FindDevice(CStdString p_strDeviceName, CDeviceBase** ppDevice);
    BOOL FindCorrectManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CProtocolStackManagerBase** ppManager);
    BOOL FindCorrectManager(CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, CProtocolStackManagerBase** ppManager);

private:
    CHandleRegistrationMap_DCS* m_pHandleRegistrationMap_DCS;
    std::list<CDeviceBase*> m_DeviceList;
};

#endif // !defined(AFX_DEVICECOMMANDSET_H__E49EE49E_830C_4CED_8A20_94D3825CCEBD__INCLUDED_)
