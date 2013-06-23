// VirtualCommandSet_ManagerBase.cpp: Implementierung der Klasse CVirtualCommandSet_ManagerBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/VirtualCommandSet/BaseClasses/VirtualCommandSet_ManagerBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CVirtualCommandSet_ManagerBase::CVirtualCommandSet_ManagerBase()
{
    m_eLayer = VIRTUAL_COMMAND_SET_LAYER;
}

CVirtualCommandSet_ManagerBase::CVirtualCommandSet_ManagerBase(int p_lInstanceValue)
: CLayerManagerBase(p_lInstanceValue)
{
    m_eLayer = VIRTUAL_COMMAND_SET_LAYER;
}

CVirtualCommandSet_ManagerBase::CVirtualCommandSet_ManagerBase(const CVirtualCommandSet_ManagerBase& rObject)
{
    m_eLayer = VIRTUAL_COMMAND_SET_LAYER;
}

CVirtualCommandSet_ManagerBase* CVirtualCommandSet_ManagerBase::Clone()
{
    return NULL;
}

CVirtualCommandSet_ManagerBase::~CVirtualCommandSet_ManagerBase()
{
}

BOOL CVirtualCommandSet_ManagerBase::VCS_InitVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_InitBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdDWordArray& dBaudrateSel, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_InitDefaultProtocolStackSettings(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, DWORD dBaudrate, DWORD dTimeout, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

HANDLE CVirtualCommandSet_ManagerBase::VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CErrorInfo* pErrorInfo)
{
    return 0;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_CloseVirtualDevice(HANDLE hVCS_Handle, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_CloseAllVirtualDevices(CErrorInfo* pErrorInfo)
{
    return FALSE;
}

HANDLE CVirtualCommandSet_ManagerBase::VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, HANDLE hPS_BaseLayerHandle, CErrorInfo* pErrorInfo)
{
	return 0;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_CloseVirtualDevice(HANDLE hVCS_Handle, HANDLE hPS_BaseLayerHandle, CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_CloseAllVirtualDevices(HANDLE hPS_BaseLayerHandle, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_AreAllVirtualDevicesClosed()
{
	return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo)
{
	return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetKeyHandle(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, HANDLE* pVCSKeyHandle, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetVirtualDeviceNameSelection(CStdStringArray* pVirtualDeviceNameSel, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetDeviceNameSelection(CStdString strVirtualDeviceName, CStdStringArray* pDeviceNameSel, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetProtocolStackNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetInterfaceNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdStringArray* pInterfaceNameSel, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetPortNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdStringArray* pPortSel, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetProtocolStackSettings(HANDLE hVCS_Handle, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_SetProtocolStackSettings(HANDLE hVCS_Handle, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetVirtualDeviceName(HANDLE hVCS_Handle, CStdString* pDeviceName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetDeviceName(HANDLE hVCS_Handle, CStdString* pDeviceName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetProtocolStackName(HANDLE hVCS_Handle, CStdString* pProtocolStackName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetInterfaceName(HANDLE hVCS_Handle, CStdString* pInterfaceName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetPortName(HANDLE hVCS_Handle, CStdString* pPortName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::ExecuteCommand(CCommandRoot *pCommand, HANDLE hHandle, HANDLE hTransactionHandle)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::GetCommands(HANDLE hHandle, ELayer eLayer, CStdString* pCommandInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::GetLayerManager(HANDLE hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::SetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CInterfaceManagerBase* pInterfaceManager)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::GetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CInterfaceManagerBase** ppInterfaceManager)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::SetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CProtocolStackManagerBase* pProtocolStackManager)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::GetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CProtocolStackManagerBase** ppProtocolStackManager)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::SetDeviceCommandSetManager(CStdString strVirtualDeviceName, CDeviceCommandSetManagerBase* pDeviceCommandSetManager)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::GetDeviceCommandSetManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CDeviceCommandSetManagerBase** ppDeviceCommandSetManager)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::VCS_GetDefaultProtocolStackSettings(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::EnableTracing(HANDLE hDCS_Handle, CStdString p_TracingFileName, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CVirtualCommandSet_ManagerBase::DisableTracing(HANDLE hDCS_Handle, CErrorInfo* pErrorInfo)
{
    return FALSE;
}
