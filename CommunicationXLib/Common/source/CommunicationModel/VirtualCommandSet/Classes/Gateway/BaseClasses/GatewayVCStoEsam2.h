// GatewayVCStoEsam2.h: Schnittstelle f�r die Klasse CGatewayVCStoEsam2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayVCStoEsam2_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_)
#define AFX_GatewayVCStoEsam2_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayVCStoDCS.h"

class CDeviceCommandSetManagerBase;
class CCommand_VCS;
class CCommand_DCS_Esam2;

class CGatewayVCStoEsam2 : public CGatewayVCStoDCS
{
public:
    CGatewayVCStoEsam2();
    virtual ~CGatewayVCStoEsam2();
    CGatewayVCStoEsam2& operator=(CGatewayVCStoEsam2& other);

protected:
    //ObjectDictionary
    BOOL ReadInt8Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int8* p_pbValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, short* p_psValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, int* p_plValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int64* p_pullValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadUInt8Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pubValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadUInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, WORD* p_pusValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadUInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD* p_pulValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadUInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, unsigned __int64* p_pullValue, CErrorInfo* p_pErrorInfo = NULL);
	BOOL ReadStringObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, CStdString* p_pValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL ReadDataBufferObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, void*& p_rpData, DWORD& p_rulNumberOfBytesToRead, DWORD* p_pulNumberOfBytesRead, CErrorInfo* p_pErrorInfo = NULL);

    BOOL WriteInt8Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int8 p_bValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, short p_sValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, int p_lValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int64 p_ullValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteUInt8Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE p_ubValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteUInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, WORD p_usValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteUInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteUInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, unsigned __int64 p_ullValue, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteDataBufferObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, void* p_pData, DWORD p_ulNumberOfBytesToWrite, DWORD* p_pulNumberOfBytesWritten, CErrorInfo* p_pErrorInfo = NULL);

    //NetworkManagement
    BOOL DCS_SendNMTService(CLayerManagerBase *p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usCommandSpecifier, CErrorInfo* p_pErrorInfo = NULL);

    //General Gateway
    BOOL DCS_SendCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_RequestCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_ReadCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usCobID, WORD p_usLength, void* p_pData, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);

    //Layer Setting Services
    BOOL DCS_SendLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_ReadLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usTimeout, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo = NULL);

    //Device Parameter Functions
    BOOL GetAddressParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, WORD& p_rNetworkId, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);
    BOOL GetNetworkId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, WORD& p_rNetworkId, CErrorInfo* p_pErrorInfo);

    //Layer Parameter Stack
    BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

    //Device Timeout
    BOOL GetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD& p_rulTimeout);
    BOOL SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD p_ulTimeout);

	//Device TransferRate
    BOOL GetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD& p_rulTransferRate, CErrorInfo* p_pErrorInfo);
    BOOL SetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD p_ulTransferRate, CErrorInfo* p_pErrorInfo);

	//Parameter
	BOOL RestoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL RestoreFactoryParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL StoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL ResetDevice(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, BOOL p_oWaitForReboot, CErrorInfo* p_pErrorInfo);
	BOOL WaitForReboot(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);

    //WaitForTermination
    BOOL WaitForTermination(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo);

    BOOL ReadODObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE*& p_rpDataBuffer, DWORD& p_rulNumberOfBytesToRead, DWORD* p_pulNumberOfBytesRead, CErrorInfo* p_pErrorInfo = NULL);
    BOOL WriteODObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pDataBuffer, DWORD p_ulNumberOfBytesToWrite, DWORD* p_pulNumberOfBytesWritten, CErrorInfo* p_pErrorInfo = NULL);

private:
    BOOL DCS_ReadObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pObjectBuffer, DWORD p_ulObjectBufferLength, DWORD* p_pulObjectLengthRead, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_InitiateSegmentedRead(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD* p_pulObjectLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_SegmentedRead(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL p_oToggle, BOOL* p_poLastDataSegment, BYTE* p_pSegmentBuffer, DWORD p_ulSegmentBufferLength, DWORD* p_pulSegmentLengthRead, CErrorInfo* p_pErrorInfo =  NULL);
    BOOL DCS_WriteObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pObjectBuffer, DWORD p_ulObjectLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_InitiateSegmentedWrite(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulObjectLength, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_SegmentedWrite(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL p_oToggle, BOOL p_oMoreSegments, BYTE* p_pSegmentBuffer, DWORD p_ulSegmentLength, DWORD* p_pulSegmentLengthWritten, CErrorInfo* p_pErrorInfo = NULL);
    BOOL DCS_AbortSegmentedTransfer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulAbortCode, CErrorInfo* p_pErrorInfo = NULL);

    void InitCommands_ObjectDictionary();
    void InitCommands_GeneralGateway();
    void InitCommands_LayerSettingServices();
    void DeleteCommands_ObjectDictionary();
    void DeleteCommands_GeneralGateway();
    void DeleteCommands_LayerSettingServices();

	//TransferRate Conversion
	DWORD ConvertBaudrateIndexToValue(WORD wBaudrateIndex);
	WORD ConvertBaudrateValueToIndex(DWORD dBaudrate);
	DWORD ConvertBitrateIndexToValue_CiaStandard(WORD wBaudrateIndex);
	WORD ConvertBitrateValueToIndex_CiaStandard(DWORD dBaudrate);

private:
    CCommand_DCS_Esam2* m_pCommand_WriteObject;
    CCommand_DCS_Esam2* m_pCommand_InitiateSegmentedWrite;
    CCommand_DCS_Esam2* m_pCommand_SegmentedWrite;
    CCommand_DCS_Esam2* m_pCommand_ReadObject;
    CCommand_DCS_Esam2* m_pCommand_InitiateSegmentedRead;
    CCommand_DCS_Esam2* m_pCommand_SegmentedRead;
    CCommand_DCS_Esam2* m_pCommand_AbortSegmentedTransfer;

    CCommand_DCS_Esam2* m_pCommand_SendCANFrame;
    CCommand_DCS_Esam2* m_pCommand_RequestCANFrame;
    CCommand_DCS_Esam2* m_pCommand_ReadCANFrame;
    CCommand_DCS_Esam2* m_pCommand_SendNMTService;

    CCommand_DCS_Esam2* m_pCommand_SendLSSFrame;
    CCommand_DCS_Esam2* m_pCommand_ReadLSSFrame;
};

#endif // !defined(AFX_GatewayVCStoEsam2_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_)
