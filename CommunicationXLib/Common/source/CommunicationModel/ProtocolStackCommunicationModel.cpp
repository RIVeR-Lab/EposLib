// ProtocolStackCommunicationModel.cpp: Implementierung der Klasse CProtocolStackCommunicationModel.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/ProtocolStackCommunicationModel.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CProtocolStackCommunicationModel::CProtocolStackCommunicationModel()
: m_hPS_Handle(0)
{
}

CProtocolStackCommunicationModel::~CProtocolStackCommunicationModel()
{
    PS_CloseProtocolStack();
}

BOOL CProtocolStackCommunicationModel::PS_OpenProtocolStack(CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_hPS_Handle == 0)
    {
        oResult = CProtocolStackCommunicationModelBase::PS_OpenProtocolStack(p_strProtocolStackName, p_strInterfaceName, p_strPortName, &m_hPS_Handle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_OpenProtocolStack(CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_OpenProtocolStack(p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_phPSKeyHandle, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_CloseProtocolStack(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(CProtocolStackCommunicationModelBase::PS_CloseProtocolStack(m_hPS_Handle, p_pErrorInfo))
    {
        m_hPS_Handle = 0;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_CloseProtocolStack(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_CloseProtocolStack(p_hPS_Handle, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_CloseAllProtocolStacks(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(CProtocolStackCommunicationModelBase::PS_CloseAllProtocolStacks(p_pErrorInfo))
    {
        m_hPS_Handle = 0;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_OpenLayerProtocolStack(CStdString p_strProtocolStackName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_hPS_Handle == 0)
    {
        oResult = CProtocolStackCommunicationModelBase::PS_OpenLayerProtocolStack(p_strProtocolStackName, p_strDeviceName, p_hPS_BaseLayerHandle, &m_hPS_Handle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_OpenLayerProtocolStack(CStdString p_strProtocolStackName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE* p_phPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_OpenLayerProtocolStack(p_strProtocolStackName, p_strDeviceName, p_hPS_BaseLayerHandle, p_phPSKeyHandle, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_CloseLayerProtocolStack(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(CProtocolStackCommunicationModelBase::PS_CloseLayerProtocolStack(m_hPS_Handle, p_hPS_BaseLayerHandle, p_pErrorInfo))
    {
        m_hPS_Handle = 0;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_CloseLayerProtocolStack(HANDLE p_hPS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_CloseLayerProtocolStack(p_hPS_Handle, p_hPS_BaseLayerHandle, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_CloseAllLayerProtocolStacks(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(CProtocolStackCommunicationModelBase::PS_CloseAllLayerProtocolStacks(p_hPS_BaseLayerHandle, p_pErrorInfo))
    {
        m_hPS_Handle = 0;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetBaseLayerKeyHandle(HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetBaseLayerKeyHandle(m_hPS_Handle, p_pPS_BaseLayerKeyHandle, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetBaseLayerKeyHandle(HANDLE p_hPS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetBaseLayerKeyHandle(p_hPS_Handle, p_pPS_BaseLayerKeyHandle, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetProtocolStackSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetProtocolStackSettings(m_hPS_Handle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_GetProtocolStackSettings(p_hPS_Handle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_SetProtocolStackSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_SetProtocolStackSettings(m_hPS_Handle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_SetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_SetProtocolStackSettings(p_hPS_Handle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_GetProtocolStackMode(WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetProtocolStackMode(m_hPS_Handle, p_pusModeIndex, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetProtocolStackMode(HANDLE p_hPS_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_GetProtocolStackMode(p_hPS_Handle, p_pusModeIndex, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_SetProtocolStackMode(WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_SetProtocolStackMode(m_hPS_Handle, p_usModeIndex, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_SetProtocolStackMode(HANDLE p_hPS_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_SetProtocolStackMode(p_hPS_Handle, p_usModeIndex, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_GetProtocolStackName(CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetProtocolStackName(m_hPS_Handle, p_pProtocolStackName, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetProtocolStackName(HANDLE p_hPS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_GetProtocolStackName(p_hPS_Handle, p_pProtocolStackName, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_GetInterfaceName(CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetInterfaceName(m_hPS_Handle, p_pInterfaceName, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetInterfaceName(HANDLE p_hPS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_GetInterfaceName(p_hPS_Handle, p_pInterfaceName, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_GetPortName(CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetPortName(m_hPS_Handle, p_pPortName, p_pErrorInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetPortName(HANDLE p_hPS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_GetPortName(p_hPS_Handle, p_pPortName, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::PS_GetBaseDeviceName(HANDLE p_hPS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo)
{
    return CProtocolStackCommunicationModelBase::PS_GetDeviceName(p_hPS_Handle, p_pDeviceName, p_pErrorInfo);
}

BOOL CProtocolStackCommunicationModel::IsProtocolStackOpen()
{
    return m_hPS_Handle != 0;
}

BOOL CProtocolStackCommunicationModel::SetKeyHandle(HANDLE p_hKeyHandle, CErrorInfo* p_pErrorInfo)
{
    m_hPS_Handle = p_hKeyHandle;

    return TRUE;
}

BOOL CProtocolStackCommunicationModel::GetKeyHandle(HANDLE& p_rhPS_Handle)
{
    p_rhPS_Handle = m_hPS_Handle;
    return TRUE;
}

BOOL CProtocolStackCommunicationModel::GetLayerParameterStack(CLayerParameterStack& p_rAddressParameter, EAddressingMode p_eAddressingMode)
{
    //Must be overridden by device specific Communication Model
    return FALSE;
}

BOOL CProtocolStackCommunicationModel::PS_ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_ExecuteCommand(p_pCommand, m_hPS_Handle, p_hTransactionHandle);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    return CProtocolStackCommunicationModelBase::PS_ExecuteCommand(p_pCommand, p_hHandle, p_hTransactionHandle);
}

BOOL CProtocolStackCommunicationModel::PS_GetCommands(ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_GetCommands(m_hPS_Handle, p_eLayer, p_pCommandInfo);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    return CProtocolStackCommunicationModelBase::PS_GetCommands(p_hHandle, p_eLayer, p_pCommandInfo);
}

BOOL CProtocolStackCommunicationModel::PS_AbortCommands(BOOL p_oActive)
{
    BOOL oResult(FALSE);

    oResult = CProtocolStackCommunicationModelBase::PS_AbortCommands(m_hPS_Handle, p_oActive);

    return oResult;
}

BOOL CProtocolStackCommunicationModel::PS_AbortCommands(HANDLE p_hHandle, BOOL p_oActive)
{
    return CProtocolStackCommunicationModelBase::PS_AbortCommands(p_hHandle, p_oActive);
}

CJournalManagerBase** CProtocolStackCommunicationModel::GetJournalManagerMember()
{
    return &m_pJournalManager;
}

CProtocolStackManagerBase* CProtocolStackCommunicationModel::GetProtocolStackManager()
{
    return m_pProtocolStackManager;
}

HANDLE CProtocolStackCommunicationModel::GetPSHandle()
{
    return m_hPS_Handle;
}

HANDLE CProtocolStackCommunicationModel::StartTransaction(HANDLE p_hJournalHandle)
{
    HANDLE hResult(0);

    hResult = CProtocolStackCommunicationModelBase::StartTransaction(m_hPS_Handle, p_hJournalHandle);

    return hResult;
}

HANDLE CProtocolStackCommunicationModel::StartTransaction(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle)
{
    return CProtocolStackCommunicationModelBase::StartTransaction(p_hPS_Handle, p_hJournalHandle);
}

