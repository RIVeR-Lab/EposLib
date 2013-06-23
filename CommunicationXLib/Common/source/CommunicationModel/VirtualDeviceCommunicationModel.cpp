// VirtualDeviceCommunicationModel.cpp: Implementierung der Klasse CVirtualDeviceCommunicationModel.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>

#include <CommunicationModel/VirtualDeviceCommunicationModel.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CVirtualDeviceCommunicationModel::CVirtualDeviceCommunicationModel()
    : m_hVCS_Handle(0)
    , m_pObjectDictionary(NULL)
{
}

CVirtualDeviceCommunicationModel::~CVirtualDeviceCommunicationModel()
{
    VCS_CloseVirtualDevice();
}

BOOL CVirtualDeviceCommunicationModel::VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_hVCS_Handle == 0)
    {
        oResult = CVirtualDeviceCommunicationModelBase::VCS_OpenVirtualDevice(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, &m_hVCS_Handle, p_pErrorInfo);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_OpenVirtualDevice(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_phVCSKeyHandle, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_CloseVirtualDevice(CErrorInfo* p_pErrorInfo)
{
    if(CVirtualDeviceCommunicationModelBase::VCS_CloseVirtualDevice(m_hVCS_Handle, p_pErrorInfo))
    {
        m_hVCS_Handle = (HANDLE)0;
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModel::VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_CloseVirtualDevice(p_hVCS_Handle, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_CloseAllVirtualDevices(CErrorInfo* p_pErrorInfo)
{
    if(CVirtualDeviceCommunicationModelBase::VCS_CloseAllVirtualDevices(p_pErrorInfo))
    {
        m_hVCS_Handle = (HANDLE)0;
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualDeviceCommunicationModel::IsVirtualDeviceOpen()
{
    return m_hVCS_Handle != 0;
}

BOOL CVirtualDeviceCommunicationModel::SetKeyHandle(HANDLE p_hKeyHandle, CErrorInfo* p_pErrorInfo)
{
    m_hVCS_Handle = p_hKeyHandle;

    return TRUE;
}

BOOL CVirtualDeviceCommunicationModel::GetKeyHandle(HANDLE& p_rhVCS_Handle)
{
    p_rhVCS_Handle = m_hVCS_Handle;
    return TRUE;
}

BOOL CVirtualDeviceCommunicationModel::GetLayerParameterStack(CLayerParameterStack& p_rAddressParameter, EAddressingMode addressingMode)
{
    //Must be overridden by device specific Communication Model
    return FALSE;
}

BOOL CVirtualDeviceCommunicationModel::DetectLayerParameter(CLayerParameterStack& p_rAddressParameter, BOOL p_oShowMsg)
{
    //Must be overridden by device specific Communication Model
    return FALSE;
}

BOOL CVirtualDeviceCommunicationModel::DetectLayerParameter(BOOL p_oShowMsg)
{
    //Must be overridden by device specific Communication Model
    return FALSE;
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetProtocolStackSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_GetProtocolStackSettings(m_hVCS_Handle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_GetProtocolStackSettings(p_hVCS_Handle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_SetProtocolStackSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_SetProtocolStackSettings(m_hVCS_Handle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_SetProtocolStackSettings(HANDLE p_hVCS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_SetProtocolStackSettings(p_hVCS_Handle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetVirtualDeviceName(CStdString* p_pstrVirtualDeviceName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_GetVirtualDeviceName(m_hVCS_Handle, p_pstrVirtualDeviceName, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetVirtualDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrVirtualDeviceName, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_GetVirtualDeviceName(p_hVCS_Handle, p_pstrVirtualDeviceName, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetDeviceName(CStdString* p_pstrDeviceName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_GetDeviceName(m_hVCS_Handle, p_pstrDeviceName, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetDeviceName(HANDLE p_hVCS_Handle, CStdString* p_pstrDeviceName, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_GetDeviceName(p_hVCS_Handle, p_pstrDeviceName, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetProtocolStackName(CStdString* p_pstrProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_GetProtocolStackName(m_hVCS_Handle, p_pstrProtocolStackName, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetProtocolStackName(HANDLE p_hVCS_Handle, CStdString* p_pstrProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_GetProtocolStackName(p_hVCS_Handle, p_pstrProtocolStackName, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetInterfaceName(CStdString* p_pstrInterfaceName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult =  CVirtualDeviceCommunicationModelBase::VCS_GetInterfaceName(m_hVCS_Handle, p_pstrInterfaceName, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetInterfaceName(HANDLE p_hVCS_Handle, CStdString* p_pstrInterfaceName, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_GetInterfaceName(p_hVCS_Handle, p_pstrInterfaceName, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetPortName(CStdString* p_pstrPortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_GetPortName(m_hVCS_Handle, p_pstrPortName, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_GetPortName(HANDLE p_hVCS_Handle, CStdString* p_pstrPortName, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::VCS_GetPortName(p_hVCS_Handle, p_pstrPortName, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::ExecuteCommand(p_pCommand, m_hVCS_Handle, p_hTransactionHandle);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hTransactionHandle)
{
    return CVirtualDeviceCommunicationModelBase::ExecuteCommand(p_pCommand, p_hVCS_Handle, p_hTransactionHandle);
}

BOOL CVirtualDeviceCommunicationModel::GetCommands(ELayer p_Layer, CStdString* p_pstrCommandInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::GetCommands(m_hVCS_Handle, p_Layer, p_pstrCommandInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::GetCommands(HANDLE p_hVCS_Handle, ELayer p_Layer, CStdString* p_pstrCommandInfo)
{
    return CVirtualDeviceCommunicationModelBase::GetCommands( p_hVCS_Handle, p_Layer, p_pstrCommandInfo);
}

BOOL CVirtualDeviceCommunicationModel::AbortCommands(bool p_oActive)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::AbortCommands(m_hVCS_Handle, p_oActive);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::AbortCommands(HANDLE p_hVCS_Handle, bool p_oActive)
{
    return CVirtualDeviceCommunicationModelBase::AbortCommands(p_hVCS_Handle, p_oActive);
}

BOOL CVirtualDeviceCommunicationModel::InvokeCommand(CCommandRoot *p_pCommand, HANDLE p_hJournalHandle)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::InvokeCommand(p_pCommand, m_hVCS_Handle, p_hJournalHandle);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::InvokeCommand(CCommandRoot *p_pCommand, HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle)
{
    return CVirtualDeviceCommunicationModelBase::InvokeCommand(p_pCommand, p_hVCS_Handle, p_hJournalHandle);
}

CJournalManagerBase** CVirtualDeviceCommunicationModel::GetJournalManagerMember()
{
    return &m_pJournalManager;
}

CVirtualCommandSet_Manager* CVirtualDeviceCommunicationModel::GetVirtualCommandSetManager()
{
    return m_pVirtualCommandSetManager;
}

HANDLE CVirtualDeviceCommunicationModel::GetVCSHandle()
{
    return m_hVCS_Handle;
}

HANDLE CVirtualDeviceCommunicationModel::StartTransaction(HANDLE p_hJournalHandle)
{
    HANDLE hResult = 0;

    hResult = CVirtualDeviceCommunicationModelBase::StartTransaction(m_hVCS_Handle, p_hJournalHandle);

    return hResult;
}

HANDLE CVirtualDeviceCommunicationModel::StartTransaction(HANDLE p_hVCS_Handle, HANDLE p_hJournalHandle)
{
    return CVirtualDeviceCommunicationModelBase::StartTransaction(p_hVCS_Handle, p_hJournalHandle);
}

BOOL CVirtualDeviceCommunicationModel::EnableTracing(CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::EnableTracing(m_hVCS_Handle, p_TracingFileName, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::EnableTracing(HANDLE p_hVCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::EnableTracing(p_hVCS_Handle, p_TracingFileName, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::DisableTracing(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::DisableTracing(m_hVCS_Handle, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::DisableTracing(HANDLE p_hVCS_Handle, CErrorInfo* p_pErrorInfo)
{
    return CVirtualDeviceCommunicationModelBase::DisableTracing(p_hVCS_Handle, p_pErrorInfo);
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationDlg(CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode, EAddressingMode p_AddressingMode)
{
    CLayerParameterStack addressParameter;
    BOOL oResult = FALSE;

    if(GetLayerParameterStack(addressParameter))
    {
        oResult = CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(m_hVCS_Handle, addressParameter, p_strInfoMessage, p_strRootVirtualDeviceName, p_strRootDeviceName, p_strRootProtocolStackName, p_eDialogMode);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CLayerParameterStack& p_rAddressParameter, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode, EAddressingMode p_AddressingMode)
{
    return CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(p_rhVCS_Handle, p_rAddressParameter, p_strInfoMessage, p_strRootVirtualDeviceName, p_strRootDeviceName, p_strRootProtocolStackName, p_eDialogMode, p_AddressingMode);
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationDlg(CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(m_hVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_strRootVirtualDeviceName, p_strRootDeviceName, p_strRootProtocolStackName, p_eDialogMode);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode)
{
    return CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(p_hVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_phKeyHandle, p_eDialogMode);
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationDlg(CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(m_hVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_phKeyHandle, p_eDialogMode);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationDlg(HANDLE& p_rhVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strRootVirtualDeviceName, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, EDialogMode p_eDialogMode)
{
    return CVirtualDeviceCommunicationModelBase::SearchCommunicationDlg(p_rhVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_strRootVirtualDeviceName, p_strRootDeviceName, p_strRootProtocolStackName, p_eDialogMode);
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationWholeCanIdRangeDlg(CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, WORD *p_pusNodeId, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::SearchCommunicationWholeCanIdRangeDlg(m_hVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_pusNodeId, p_phKeyHandle, p_eDialogMode);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::SearchCommunicationWholeCanIdRangeDlg(HANDLE p_hVCS_Handle, CCommandRoot* p_pSearchCommand, CStdString p_strInfoMessage, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, WORD *p_pusNodeId, HANDLE *p_phKeyHandle, EDialogMode p_eDialogMode)
{
    return CVirtualDeviceCommunicationModelBase::SearchCommunicationWholeCanIdRangeDlg(p_hVCS_Handle, p_pSearchCommand, p_strInfoMessage, p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_pusNodeId, p_phKeyHandle, p_eDialogMode);
}

BOOL CVirtualDeviceCommunicationModel::SetObjectDictionary(CObjectDictionary* p_pObjectDictionary)
{
    BOOL oResult = TRUE;

    m_pObjectDictionary = p_pObjectDictionary;

    return oResult;
}

CObjectDictionary* CVirtualDeviceCommunicationModel::GetObjectDictionary()
{
    return m_pObjectDictionary;
}

template <class T>
BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, T* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode)
{
	BOOL oResult(FALSE);
	DWORD ulNbOfBytesRead(0);
    DWORD ulNbOfBytesToRead(0);
    CObjectEntry* pObjectEntry(0);
    void* pVoidData(0);

    if(m_pObjectDictionary)
    {
        pObjectEntry = m_pObjectDictionary->SearchObject(p_usObjectIndex, p_ubObjectSubIndex);
        if(pObjectEntry)
        {
            if(m_objectEntryRangeCheck.CreateSafeGetVoidDataContainer(pObjectEntry, *p_pData, pVoidData, ulNbOfBytesToRead))
            {
                if(GetObject(p_usObjectIndex, p_ubObjectSubIndex, pVoidData, ulNbOfBytesToRead, &ulNbOfBytesRead, p_pulErrorCode, p_AddressingMode))
                {
                    //Check Number of Bytes read
                    if(ulNbOfBytesToRead != ulNbOfBytesRead)
                    {
                        if(p_pulErrorCode) *p_pulErrorCode = ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED;
                    }
                    else
                    {
                        m_objectEntryRangeCheck.GetSafeVoidDataContainerData(pObjectEntry, pVoidData, p_pData);
                        oResult = TRUE;
                    }
                }
                //delete the void data
                delete pVoidData;
            }
            else
            {
                if(p_pulErrorCode) *p_pulErrorCode = ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED;
            }
        }
    }

    return oResult;
};
template BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, char* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, BYTE* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, short* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, WORD* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, long* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, DWORD* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, __int64* p_pData, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);

template <class T>
BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, T p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode)
{
	BOOL oResult(FALSE);
    DWORD ulNbOfBytesWritten(0);
    DWORD ulNbOfBytesToWrite(0);
    CObjectEntry* pObjectEntry(0);
    void* pVoidData(0);

    if(m_pObjectDictionary)
    {
        pObjectEntry = m_pObjectDictionary->SearchObject(p_usObjectIndex, p_ubObjectSubIndex);
        if(pObjectEntry)
        {
            //Create void data container to set object data
            if(m_objectEntryRangeCheck.CreateSafeSetVoidDataContainer(pObjectEntry, p_data, pVoidData, ulNbOfBytesToWrite))
            {
                if(SetObject(p_usObjectIndex, p_ubObjectSubIndex, pVoidData, ulNbOfBytesToWrite, &ulNbOfBytesWritten, p_pulErrorCode, p_AddressingMode))
                {
                    if(ulNbOfBytesToWrite != ulNbOfBytesWritten)
                    {
                        if(p_pulErrorCode) *p_pulErrorCode = ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED;
                    }
                    else
                    {
                        oResult = TRUE;
                    }
                }
                //Delete the void data
                delete pVoidData;
            }
            else
            {
                if(p_pulErrorCode) *p_pulErrorCode = ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED;
            }
        }
    }

    return oResult;
}

template BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, char p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, BYTE p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, short p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, WORD p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, long p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, DWORD p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);
template BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, __int64 p_data, DWORD* p_pulErrorCode, EAddressingMode p_AddressingMode);

BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToRead, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode)
{
    DWORD ulNbOfBytesRead(0);
    BOOL oResult(FALSE);

    if(GetObject(p_usObjectIndex, p_ubObjectSubIndex, p_pData, p_ulNbOfBytesToRead, &ulNbOfBytesRead, p_pulErrorCode, p_AddressingMode))
    {
        //Check Number of Bytes read
        if(p_ulNbOfBytesToRead != ulNbOfBytesRead)
        {
            if(p_pulErrorCode) *p_pulErrorCode = ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED;
            oResult = FALSE;
        }
        else
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToWrite, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode)
{
    DWORD ulNbOfBytesWritten(0);
    BOOL oResult(FALSE);

    if(SetObject(p_usObjectIndex, p_ubObjectSubIndex, p_pData, p_ulNbOfBytesToWrite, &ulNbOfBytesWritten, p_pulErrorCode, p_AddressingMode))
    {
        if(p_ulNbOfBytesToWrite != ulNbOfBytesWritten)
        {
            if(p_pulErrorCode) *p_pulErrorCode = ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED;
            oResult = FALSE;
        }
        else
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::GetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToRead, DWORD *p_pulNbOfBytesRead, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode)
{
    BOOL oResult = FALSE;
    CCommand_VCS_CanOpen command(VCS_CANOPEN_GET_OBJECT);
    CLayerParameterStack addressParameter;
    HANDLE hTransactionHandle = NULL;

    if(p_pData)
    {
        //Set Parameter Data
        command.SetParameterData(0, &p_usObjectIndex, sizeof(p_usObjectIndex));
        command.SetParameterData(1, &p_ubObjectSubIndex, sizeof(p_ubObjectSubIndex));
        command.SetParameterData(2, &p_ulNbOfBytesToRead, sizeof(p_ulNbOfBytesToRead));

        //Address Parameter
        GetLayerParameterStack(addressParameter, p_AddressingMode);
        command.SetLayerParameterStack(addressParameter);

        //ExecuteCommand
        oResult = ExecuteCommand(&command, hTransactionHandle);

        //Get ReturnParameter Data
        command.GetReturnParameterData(0, p_pData, p_ulNbOfBytesToRead);
        command.GetReturnParameterData(1, p_pulNbOfBytesRead, sizeof(*p_pulNbOfBytesRead));

        //Get ErrorCode
        command.GetErrorCode(p_pulErrorCode);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::SetObject(WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void *p_pData, DWORD p_ulNbOfBytesToWrite, DWORD *p_pulNbOfBytesWritten, DWORD *p_pulErrorCode, EAddressingMode p_AddressingMode)
{
    BOOL oResult = FALSE;
    CCommand_VCS_CanOpen command(VCS_CANOPEN_SET_OBJECT);
    CLayerParameterStack addressParameter;
    HANDLE hTransactionHandle = NULL;

    if(p_pData)
    {
        //Set Parameter Data
        command.ResetStatus();
        command.SetParameterData(0, &p_usObjectIndex, sizeof(p_usObjectIndex));
        command.SetParameterData(1, &p_ubObjectSubIndex, sizeof(p_ubObjectSubIndex));
        command.SetParameterData(2, p_pData, p_ulNbOfBytesToWrite);
        command.SetParameterData(3, &p_ulNbOfBytesToWrite, sizeof(p_ulNbOfBytesToWrite));

        //Address Parameter
        GetLayerParameterStack(addressParameter, p_AddressingMode);
        command.SetLayerParameterStack(addressParameter);

        //ExecuteCommand
        oResult = ExecuteCommand(&command, hTransactionHandle);

        //Get ReturnParameter Data
        command.GetReturnParameterData(0, p_pulNbOfBytesWritten, sizeof(*p_pulNbOfBytesWritten));

        //Get ErrorCode
        command.GetErrorCode(p_pulErrorCode);
    }

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_OpenVirtualDevice(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, HANDLE *p_phVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_OpenVirtualDevice(p_strVirtualDeviceName, p_strDeviceName, p_hPS_BaseLayerHandle, p_phVCSKeyHandle, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_CloseVirtualDevice(HANDLE p_hVCS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_CloseVirtualDevice(p_hVCS_Handle, p_hPS_BaseLayerHandle, p_pErrorInfo);

    return oResult;
}

BOOL CVirtualDeviceCommunicationModel::VCS_CloseAllVirtualDevices(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    oResult = CVirtualDeviceCommunicationModelBase::VCS_CloseAllVirtualDevices(p_hPS_BaseLayerHandle, p_pErrorInfo);

    return oResult;
}

WORD CVirtualDeviceCommunicationModel::GetPositionAddr(EAddressingMode p_AddressingMode)
{
    return 0;
}
