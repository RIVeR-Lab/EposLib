// GatewayVCStoEsam.h: Schnittstelle f�r die Klasse CGatewayVCStoEsam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYVCSTOEsam_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_)
#define AFX_GATEWAYVCSTOEsam_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayVCStoDCS.h"

class CDeviceCommandSetManagerBase;
class CCommand_DCS_Esam;
class CCommand_VCS;

class CGatewayVCStoEsam : public CGatewayVCStoDCS
{
public:
    CGatewayVCStoEsam();
    virtual ~CGatewayVCStoEsam();
    CGatewayVCStoEsam& operator=(CGatewayVCStoEsam& other);

protected:
    BOOL ReadInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, __int8* pbValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, short* piValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, int* plValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadUInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* puValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, WORD* pwValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD* pdValue, CErrorInfo* pErrorInfo = NULL);
	BOOL ReadStringObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE ubPortNumber, BYTE ubNodeId, WORD usIndex, BYTE ubSubIndex, CStdString* pValue, CErrorInfo* pErrorInfo = NULL);
    BOOL ReadDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, void* pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo = NULL);

    BOOL WriteInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, __int8 bValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, short iValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, int lValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteUInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE uValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, WORD wValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD dValue, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, void* pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo = NULL);

	BOOL SetDisableState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,BYTE uPortNumber, WORD wNodeId, CErrorInfo* pErrorInfo = NULL);
	BOOL ReadDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wNodeId, WORD* pwDeviceState, CErrorInfo* pErrorInfo = NULL);
    BOOL CheckDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wNodeId, WORD wState, CErrorInfo* pErrorInfo = NULL);

    //Funktionalit�t NetworkManagement
    BOOL DCS_SendNMTService(CLayerManagerBase *pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wNodeId, WORD wCommandSpecifier, CErrorInfo* pErrorInfo = NULL);

    //Funktionalit�t GeneralGateway
    BOOL DCS_SendCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wCobID, WORD wLength, void* pData, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_RequestCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wCobID, WORD wLength, void* pData, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_ReadCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wCobID, WORD wLength, void* pData, DWORD p_ulTimeout, CErrorInfo* pErrorInfo = NULL);

    //LayerSettingServices
    BOOL DCS_SendLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_ReadLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);

	//Device Parameter Functions
	BOOL GetAddressParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rPortNumber, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);
	BOOL GetPortNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rPortNumber, CErrorInfo* p_pErrorInfo);
	BOOL GetNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

	//Parameter
	BOOL RestoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL StoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);
	BOOL ResetDevice(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, BOOL p_oWaitForReboot, CErrorInfo* p_pErrorInfo);
	BOOL WaitForReboot(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo);

private:
    BOOL ReadODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pDataBuffer, DWORD dMaxNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo = NULL);
    BOOL WriteODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pDataBuffer, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo = NULL);

private:
    BOOL DCS_ReadObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectBufferLength, DWORD* pdObjectLengthRead, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_InitiateSegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD* pdObjectLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_SegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BOOL* poLastDataSegment, BYTE* pSegmentBuffer, DWORD dSegmentBufferLength, DWORD* pdSegmentLengthRead, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_WriteObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_InitiateSegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD dObjectLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_SegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE* pSegmentBuffer, DWORD dSegmentLength, CErrorInfo* pErrorInfo = NULL);
    BOOL DCS_AbortSegmentedTransfer(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo = NULL);

    void InitCommands_ObjectDictionary();
    void InitCommands_NetworkManagement();
    void InitCommands_GeneralGateway();
    void InitCommands_LayerSettingServices();
    void DeleteCommands_ObjectDictionary();
    void DeleteCommands_NetworkManagement();
    void DeleteCommands_GeneralGateway();
    void DeleteCommands_LayerSettingServices();

private:
    CCommand_DCS_Esam* m_pCommand_WriteObject;
    CCommand_DCS_Esam* m_pCommand_InitiateSegmentedWrite;
    CCommand_DCS_Esam* m_pCommand_SegmentedWrite;

    CCommand_DCS_Esam* m_pCommand_ReadObject;
    CCommand_DCS_Esam* m_pCommand_InitiateSegmentedRead;
    CCommand_DCS_Esam* m_pCommand_SegmentedRead;

    CCommand_DCS_Esam* m_pCommand_AbortSegmentedTransfer;

    CCommand_DCS_Esam* m_pCommand_SendCANFrame;
    CCommand_DCS_Esam* m_pCommand_RequestCANFrame;
    CCommand_DCS_Esam* m_pCommand_ReadCANFrame;

    CCommand_DCS_Esam* m_pCommand_SendNMTService;

    CCommand_DCS_Esam* m_pCommand_SendLSSFrame;
    CCommand_DCS_Esam* m_pCommand_ReadLSSFrame;
};

#endif // !defined(AFX_GATEWAYVCSTOEsam_H__A726F8EE_A68B_46A5_A5AF_C9A5429EEF1D__INCLUDED_)
