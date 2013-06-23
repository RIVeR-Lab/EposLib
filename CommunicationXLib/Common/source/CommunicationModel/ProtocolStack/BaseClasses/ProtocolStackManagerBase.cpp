// ProtocolStackManagerBase.cpp: Implementierung der Klasse CProtocolStackManagerBase.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CProtocolStackManagerBase::CProtocolStackManagerBase()
{
    Init();
    m_eLayer = PROTOCOL_STACK_LAYER;
}

CProtocolStackManagerBase::CProtocolStackManagerBase(int p_lInstanceValue)
: CLayerManagerBase(p_lInstanceValue)
{
    m_eLayer = PROTOCOL_STACK_LAYER;
}

CProtocolStackManagerBase::CProtocolStackManagerBase(const CProtocolStackManagerBase& rObject)
{
    m_eLayer = PROTOCOL_STACK_LAYER;
}

CProtocolStackManagerBase::~CProtocolStackManagerBase()
{
}

CProtocolStackManagerBase* CProtocolStackManagerBase::Clone()
{
    return 0;
}

BOOL CProtocolStackManagerBase::PS_InitAllProtocolStacks(CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_InitProtocolStack(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_InitProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_InitProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdString p_BaseLayerProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_InitBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdDWordArray& p_ulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_InitDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_AddPort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_DeletePort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

HANDLE CProtocolStackManagerBase::PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    return 0;
}

BOOL CProtocolStackManagerBase::PS_CloseProtocolStack(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_CloseAllProtocolStacks(CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

HANDLE CProtocolStackManagerBase::PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    return 0;
}

BOOL CProtocolStackManagerBase::PS_CloseProtocolStack(HANDLE p_hPS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_CloseAllProtocolStacks(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_AreAllProtocolStacksClosed()
{
	return FALSE;
}

BOOL CProtocolStackManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetKeyHandle(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetLayerKeyHandle(CStdString p_ProtocolStackName, CStdString p_DeviceName, CLayerParameterStack &p_rAddressParameter, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetBaseLayerKeyHandle(HANDLE p_hPS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_SetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_IsProtocolStackNameSupported(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetProtocolStackNameSelection(CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetRemoteProtocolStackNameSelection(CStdStringArray* p_pRemoteProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetDeviceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetProtocolStackNameSelection(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetInterfaceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetPortNameSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetProtocolStackModeSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pProtocolStackModeSel, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_SetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetProtocolStackMode(HANDLE p_hPS_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_SetProtocolStackMode(HANDLE p_hPS_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetProtocolStackName(HANDLE p_hPS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetDeviceName(HANDLE p_hPS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetInterfaceName(HANDLE p_hPS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::PS_GetPortName(HANDLE p_hPS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::GetLayerManager(HANDLE p_hHandle, ELayer p_eLayer, CLayerManagerBase** p_ppLayerManager, HANDLE* pLayerHandle)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::SetInterfaceManager(CStdString p_ProtocolStackName, CInterfaceManagerBase* p_pInterfaceManager)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::GetInterfaceManager(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CInterfaceManagerBase** p_ppInterfaceManager)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::SetProtocolStackManager(CStdString p_ProtocolStackName, CStdString p_DeviceName, CProtocolStackManagerBase* p_pProtocolStackManager)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::GetProtocolStackManager(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdString p_BaseLayerProtocolStackName, CProtocolStackManagerBase** p_ppProtocolStackManager)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::EnableTracing(HANDLE p_hI_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::DisableTracing(HANDLE p_hI_Handle, CErrorInfo* p_pErrorInfo)
{
    return FALSE;
}

void CProtocolStackManagerBase::InitJournalManager(CJournalManagerBase* p_pJournalManager)
{
}

void CProtocolStackManagerBase::ResetJournalManager()
{
}

BOOL CProtocolStackManagerBase::InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* p_pJournalManager)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::ResetJournalManager(HANDLE p_hHandle)
{
    return FALSE;
}

HANDLE CProtocolStackManagerBase::StartTransaction(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle)
{
    return 0;
}

BOOL CProtocolStackManagerBase::StopTransaction(HANDLE p_hTransactionHandle)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::StartCommandTime(CJournalTime* p_pThisTime)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::StopCommandTime(CJournalTime* p_pThisTime)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::TransactionEvent_VCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::TransactionEvent_DCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::TransactionEvent_ProtocolStackLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}

BOOL CProtocolStackManagerBase::TransactionEvent_InterfaceLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}
