// GatewayCANopenToEpos.h: Schnittstelle f�r die Klasse CGatewayCANopenToEpos.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayCANopenToEpos_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayCANopenToEpos_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayPStoDCS.h"
class CCommand_DCS_Epos;

class CGatewayCANopenToEpos : public CGatewayPStoDCS
{
public:
    CGatewayCANopenToEpos();
    virtual ~CGatewayCANopenToEpos();

    virtual CGateway* Clone();
    CGatewayCANopenToEpos& operator=(CGatewayCANopenToEpos& other);

    BOOL ProcessCommand(CCommandRoot*pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

	//Device Timeout
    BOOL SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, DWORD p_ulTimeout);

private:
    void InitCommands();
    void DeleteCommands();

    //PS Layer
	BOOL Process_InitiateSDODownload(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_DownloadSDOSegment(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_InitiateSDOUpload(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_UploadSDOSegment(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
	BOOL Process_NetworkIndication(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_AbortSDOTransfer(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendNMTService(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendCANFrame(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadCANFrame(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_RequestCANFrame(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendLSSFrame(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadLSSFrame(CCommand_PS* pCommand, CDeviceCommandSetManagerBase* pDeviceCommandSetManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle);

	//DCS Layer
	BOOL DCS_ReadObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,BYTE uSubIndex,BYTE* pObjectBuffer,DWORD dObjectBufferLength,DWORD* pdObjectLengthRead,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_InitiateSegmentedRead(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,BYTE uSubIndex,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SegmentedRead(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wNodeId,BOOL oToggle,BOOL* poLastDataSegment,void** ppSegmentBuffer,DWORD* pdSegmentBufferLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_WriteObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,BYTE uSubIndex,BYTE* pObjectBuffer,DWORD dObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_InitiateSegmentedWrite(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,BYTE uSubIndex,DWORD dObjectLength,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SegmentedWrite(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wNodeId,BOOL oMoreSegments,BOOL oToggle,void* pSegmentBuffer,DWORD dSegmentLength,BOOL* poRetToggle,DWORD* pdSegmentLengthWritten,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_AbortSegmentedTransfer(CDeviceCommandSetManagerBase* pManager,HANDLE hDCS_Handle,HANDLE hTransactionHandle,WORD wNodeId,WORD wIndex,WORD wSubIndex,DWORD dAbortCode,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SendNMTService(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wNodeId,WORD wCommandSpecifier,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_SendCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wCobID,WORD wLength,void* pData,CErrorInfo* pErrorInfo = NULL);
	BOOL DCS_RequestCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,WORD wCobID,WORD wLength,void* pData,CErrorInfo* pErrorInfo = NULL);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

	//Helper Functions
	WORD GetNodeId(DWORD dDefaultSdoCobIdClientServer);
	DWORD CheckDataBufferLength(BOOL oSizeIndicated,BYTE uNonValidNbOfBytes,DWORD dDataBufferLength,DWORD dMaxDataBufferLength);
	
	//Buffer Functions
	BOOL ResetBufferedData();
	BOOL IsBufferedDataAvailable(DWORD dSize,DWORD* pdSizeAvailable,BOOL* poNoMoreSegments);
	BOOL AddBufferedData(void* pDataBuffer,DWORD dDataBufferSize,BOOL oNoMoreSegments);
	BOOL GetBufferedData(void* pDataBuffer,DWORD dDataBufferSize,DWORD* pdDataRead,BOOL* poNoMoreSegments);
	BOOL ReverseBufferedData(DWORD dReverseSize);

	//Addressing
    BOOL GetAddressParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo);

private:
	CCommand_DCS_Epos* m_pCommand_WriteObject;
	CCommand_DCS_Epos* m_pCommand_InitiateSegmentedWrite;
	CCommand_DCS_Epos* m_pCommand_SegmentedWrite;
	CCommand_DCS_Epos* m_pCommand_ReadObject;
	CCommand_DCS_Epos* m_pCommand_InitiateSegmentedRead;
	CCommand_DCS_Epos* m_pCommand_SegmentedRead;
	CCommand_DCS_Epos* m_pCommand_AbortSegmentedTransfer;
	CCommand_DCS_Epos* m_pCommand_SendNMTService;
	CCommand_DCS_Epos* m_pCommand_RequestCANFrame;
	CCommand_DCS_Epos* m_pCommand_SendCANFrame;

	void* m_pBufferedData;
	DWORD m_dBufferedDataSize;
	DWORD m_dBufferedDataRead;
	BOOL m_oNoMoreSegments;
	BOOL m_oToggle;

	const DWORD k_MaxRS232DataSize;
	DWORD m_WriteSegmentSize;
    
};

#endif // !defined(AFX_GatewayCANopenToEpos_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
