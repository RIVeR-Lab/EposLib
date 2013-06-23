// InterfaceManagerBase.cpp: Implementierung der Klasse CInterfaceManagerBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <CommunicationModel/Interface/InterfaceManagerBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CInterfaceManagerBase::CInterfaceManagerBase()
{
	m_eLayer = INTERFACE_LAYER;
}

CInterfaceManagerBase::CInterfaceManagerBase(int p_lInstanceValue)
: CLayerManagerBase(p_lInstanceValue)
{
	m_eLayer = INTERFACE_LAYER;
}

CInterfaceManagerBase::CInterfaceManagerBase(const CInterfaceManagerBase& p_rObject)
{
	m_eLayer = INTERFACE_LAYER;
}

CInterfaceManagerBase::~CInterfaceManagerBase()
{
}

CInterfaceManagerBase* CInterfaceManagerBase::Clone()
{
	return NULL;
}

BOOL CInterfaceManagerBase::I_InitInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_UpdateInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_AddInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_DeleteInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_InitAllInterfaces(CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_InitBaudrateSelection(CStdString p_InterfaceName, CStdDWordArray& p_rulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_InitDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

HANDLE CInterfaceManagerBase::I_OpenInterface(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
	return 0;
}

BOOL CInterfaceManagerBase::I_CloseInterface(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_CloseAllInterfaces(CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_AreAllInterfacesClosed()
{
	return FALSE;
}

BOOL CInterfaceManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetKeyHandle(CStdString p_InterfaceName, CStdString p_PortName, HANDLE* pIKeyHandle, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_ResetInterface(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_SetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CInterfaceManagerBase::I_IsInterfaceNameSupported(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetInterfaceNameSelection(CStdStringArray* pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetPortNameSelection(CStdString p_InterfaceName, CStdStringArray* pPortSel, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetBaudrateSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetInterfaceModeSelection(CStdString p_InterfaceName, CStdStringArray* pInterfaceModeSel, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetPortModeSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdStringArray* pInterfaceModeSel, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetInterfaceSettings(HANDLE hI_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_SetInterfaceSettings(HANDLE hI_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetInterfaceMode(HANDLE hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_SetInterfaceMode(HANDLE hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetPortMode(HANDLE hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_SetPortMode(HANDLE hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetInterfaceName(HANDLE hI_Handle, CStdString* pInterfaceName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::I_GetPortName(HANDLE hI_Handle, CStdString* pPortName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::ExecuteCommand(CCommandRoot *pCommand, HANDLE hHandle, HANDLE hTransactionHandle)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::GetCommands(HANDLE p_hHandle, ELayer eLayer, CStdString* pCommandInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::GetLayerManager(HANDLE p_hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::GetLayerManager(CStdString strDeviceName, CStdString strProtocolStackName, ELayer eLayer, CLayerManagerBase** ppLayerManager)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::EnableTracing(HANDLE hI_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CInterfaceManagerBase::DisableTracing(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}
