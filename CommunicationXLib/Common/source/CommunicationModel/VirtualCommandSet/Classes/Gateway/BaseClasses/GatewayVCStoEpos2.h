// GatewayVCStoEpos2.h: Schnittstelle f�r die Klasse CGatewayVCStoEpos2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayVCStoEpos2_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_)
#define AFX_GatewayVCStoEpos2_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include "../BaseClasses/GatewayVCStoDCS.h"

class CDeviceCommandSetManagerBase;
class CCommand_DCS_Epos2;

class CGatewayVCStoEpos2 : public CGatewayVCStoDCS
{
public:
    CGatewayVCStoEpos2();
    virtual ~CGatewayVCStoEpos2();
    CGatewayVCStoEpos2& operator=(CGatewayVCStoEpos2& other);

protected:
    BOOL ReadInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, __int8* pbValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, short* piValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, long* plValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadUInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* puValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, WORD* pwValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD* pdValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadUInt64Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, unsigned __int64* pullValue, CErrorInfo* pErrorInfo = NULL);
	BOOL ReadStringObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, CStdString* pValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, void* p_pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo = NULL);

    BOOL WriteInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, __int8 bValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, short iValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, long lValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteUInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE uValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, WORD wValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteUInt64Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, unsigned __int64 ullValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, void* p_pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo = NULL);

	BOOL SetDisableState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, CErrorInfo* pErrorInfo = NULL);
	BOOL ReadDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD* pwDeviceState, CErrorInfo* pErrorInfo = NULL);
    BOOL CheckDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wState, CErrorInfo* pErrorInfo = NULL);

    //Funktionalit�t NetworkManagement
    BOOL DCS_SendNMTService(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wCommandSpecifier, CErrorInfo* pErrorInfo = NULL);

    //Funktionalit�t GeneralGateway
    BOOL DCS_SendCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_RequestCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_ReadCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, DWORD p_ulTimeout, CErrorInfo* pErrorInfo = NULL);

    //LayerSettingServices
    BOOL DCS_SendLSSFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_ReadLSSFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);

	//Device Parameter Functions
	BOOL GetNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

	//Device Timeout
    BOOL GetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, DWORD& p_rulTimeout);
    BOOL SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, DWORD p_ulTimeout);

	//Device TransferRate
    BOOL GetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, DWORD& p_rulTransferRate, CErrorInfo* p_pErrorInfo);
    BOOL SetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, DWORD p_ulTransferRate, CErrorInfo* p_pErrorInfo);

	//Parameter
	BOOL RestoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL RestoreFactoryParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL StoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL ResetDevice(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, BOOL p_oWaitForReboot, CErrorInfo* p_pErrorInfo);
	BOOL WaitForReboot(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);

private:
    BOOL ReadObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dMaxNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo = NULL);

private:
    //Funktionalit�t ObjectionDictionary Befehlssatz
    BOOL DCS_ReadObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_WriteObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_InitiateSegmentedRead(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_InitiateSegmentedWrite(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dObjectLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_SegmentedRead(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, BOOL oToggle, BOOL* poMoreSegments, BYTE* pSegmentBuffer, DWORD dSegmentBufferLength, DWORD* pdSegmentLengthRead, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_SegmentedWrite(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, BOOL oToggle, BOOL oMoreSegments, BYTE* pSegmentBuffer, DWORD dSegmentLength, DWORD* pdSegmentLengthWritten, CErrorInfo* pErrorInfo = NULL);

    BOOL DCS_AbortSegmentedTransfer(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo = NULL);

    void InitCommands_ObjectDictionary();
    void InitCommands_NetworkManagement();
    void InitCommands_GeneralGateway();
    void InitCommands_LayerSettingServices();
    void DeleteCommands_ObjectDictionary();
    void DeleteCommands_NetworkManagement();
    void DeleteCommands_GeneralGateway();
    void DeleteCommands_LayerSettingServices();

	//TransferRate Conversion
	BOOL CheckBitrateIndexCiaStandard(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE uNodeId, WORD wBaudrateIndex);
    BOOL CheckBitrateValueCiaStandard(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE uNodeId, DWORD ulBaudrate);
    BOOL CheckBitrateCiaStandard(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE uNodeId);
	DWORD ConvertBaudrateIndexToValue(WORD wBaudrateIndex);
	WORD ConvertBaudrateValueToIndex(DWORD dBaudrate);
	DWORD ConvertBitrateIndexToValue_CiaStandard(WORD wBaudrateIndex);
	WORD ConvertBitrateValueToIndex_CiaStandard(DWORD dBaudrate);
	DWORD ConvertBitrateIndexToValue_Maxon(WORD wBaudrateIndex);
    WORD ConvertBitrateValueToIndex_Maxon(DWORD dBaudrate);

private:
    CCommand_DCS_Epos2* m_pCommand_WriteObject;
    CCommand_DCS_Epos2* m_pCommand_InitiateSegmentedWrite;
    CCommand_DCS_Epos2* m_pCommand_SegmentedWrite;

    CCommand_DCS_Epos2* m_pCommand_ReadObject;
    CCommand_DCS_Epos2* m_pCommand_InitiateSegmentedRead;
    CCommand_DCS_Epos2* m_pCommand_SegmentedRead;
    CCommand_DCS_Epos2* m_pCommand_AbortSegmentedTransfer;

    CCommand_DCS_Epos2* m_pCommand_SendCANFrame;
    CCommand_DCS_Epos2* m_pCommand_RequestCANFrame;
    CCommand_DCS_Epos2* m_pCommand_ReadCANFrame;

    CCommand_DCS_Epos2* m_pCommand_SendNMTService;

    CCommand_DCS_Epos2* m_pCommand_SendLSSFrame;
    CCommand_DCS_Epos2* m_pCommand_ReadLSSFrame;

};

#endif // !defined(AFX_GatewayVCStoEpos2_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_)
