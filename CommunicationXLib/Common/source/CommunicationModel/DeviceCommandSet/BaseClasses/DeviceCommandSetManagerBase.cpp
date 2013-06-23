// DeviceCommandSetManagerBase.cpp: Implementierung der Klasse CDeviceCommandSetManagerBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "DeviceCommandSetManagerBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CDeviceCommandSetManagerBase::CDeviceCommandSetManagerBase()
{
    m_eLayer = DEVICE_COMMAND_SET_LAYER;
}

CDeviceCommandSetManagerBase::CDeviceCommandSetManagerBase(int p_lInstanceValue)
:    CLayerManagerBase(p_lInstanceValue)
{
    m_eLayer = DEVICE_COMMAND_SET_LAYER;
}

CDeviceCommandSetManagerBase::CDeviceCommandSetManagerBase(const CDeviceCommandSetManagerBase& rObject)
{
    m_eLayer = DEVICE_COMMAND_SET_LAYER;
}

CDeviceCommandSetManagerBase::~CDeviceCommandSetManagerBase()
{
}

CDeviceCommandSetManagerBase* CDeviceCommandSetManagerBase::Clone()
{
    return NULL;
}

BOOL CDeviceCommandSetManagerBase::DCS_InitAllDevices(CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_InitDevice(CStdString p_strDeviceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_InitDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_InitDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_InitBaudrateSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdDWordArray& p_ulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_InitDefaultProtocolStackSettings(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD p_ulBaudrateSel, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

HANDLE CDeviceCommandSetManagerBase::DCS_OpenDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CErrorInfo* p_pErrorInfo)
{
    return 0;
}

BOOL CDeviceCommandSetManagerBase::DCS_CloseDevice(HANDLE p_hDCS_Handle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_CloseAllDevices(CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

HANDLE CDeviceCommandSetManagerBase::DCS_OpenDevice(CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    return 0;
}

BOOL CDeviceCommandSetManagerBase::DCS_CloseDevice(HANDLE p_hDCS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_CloseAllDevices(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_AreAllDevicesClosed()
{
	return FALSE;
}

BOOL CDeviceCommandSetManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetKeyHandle(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_pDCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetBaseLayerKeyHandle(HANDLE p_hDCS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetDefaultProtocolStackSettings(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_IsDeviceNameSupported(CStdString p_strDeviceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetDeviceNameSelection(CStdStringArray* p_pDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetProtocolStackNameSelection(CStdString p_strDeviceName, CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetInterfaceNameSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetPortNameSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdStringArray* p_pPortNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetBaudrateSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CStdDWordArray* p_pBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetProtocolStackSettings(HANDLE p_hDCS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_SetProtocolStackSettings(HANDLE p_hDCS_Handle, DWORD p_ulBaudrateSel, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetDeviceName(HANDLE p_hDCS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetProtocolStackName(HANDLE p_hDCS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetInterfaceName(HANDLE p_hDCS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DCS_GetPortName(HANDLE p_hDCS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::GetLayerManager(HANDLE p_hHandle, ELayer p_eLayer, CLayerManagerBase** p_ppLayerManager, HANDLE* pLayerHandle)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::SetInterfaceManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CInterfaceManagerBase* p_pInterfaceManager)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::GetInterfaceManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CInterfaceManagerBase** p_ppInterfaceManager)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::SetProtocolStackManager(CStdString p_strDeviceName, CProtocolStackManagerBase* p_pProtocolStackManager)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::GetProtocolStackManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CProtocolStackManagerBase** p_ppProtocolStackManager)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::EnableTracing(HANDLE p_hDCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CDeviceCommandSetManagerBase::DisableTracing(HANDLE p_hDCS_Handle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}
