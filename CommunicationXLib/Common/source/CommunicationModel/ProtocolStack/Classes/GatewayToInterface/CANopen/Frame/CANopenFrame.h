// CANopenFrame.h: Schnittstelle f�r die Klasse CCANopenFrame.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANOPENFRAME_H__AE9DFEBF_E4CD_4B15_9706_71D18ABDE334__INCLUDED_)
#define AFX_CANOPENFRAME_H__AE9DFEBF_E4CD_4B15_9706_71D18ABDE334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

class CCANopenFrame
{
public:
	CCANopenFrame();
	virtual ~CCANopenFrame();

	void* GetCobIdPointer();
	DWORD GetCobIdSize();
	void* GetRtrPointer();
	DWORD GetRtrSize();
	void* GetDlcPointer();
	DWORD GetDlcSize();
	void* GetCanDataPointer();
	DWORD GetCanDataSize();

	void SetCobId(DWORD dCobId);
	DWORD GetCobId();
	BOOL GetRtr();
	BYTE GetDlc();

	BOOL PrepareSendFrame_InitiateSDODownload(DWORD dCobId,BOOL oExpeditedTransfer,BOOL oSizeIndicated,BYTE uNonValidNbOfBytes,WORD wIndex,BYTE uSubIndex,void* pDataBuffer,DWORD dDataBufferLength);
	BOOL PrepareSendFrame_DownloadSDOSegment(DWORD dCobId,BOOL oToggle,BYTE uNonValidNbOfBytes,BOOL oNoMoreSegments,void* pSegDataBuffer,DWORD dSegDataBufferLength);
	BOOL PrepareSendFrame_InitiateSDOUpload(DWORD dCobId,WORD wIndex,BYTE uSubIndex);
	BOOL PrepareSendFrame_UploadSDOSegment(DWORD dCobId,BOOL oToggle);
	BOOL PrepareSendFrame_NetworkIndication(DWORD dCobId,WORD wTargetNetworkId,BYTE uTargetNodeId);
	BOOL PrepareSendFrame_AbortSDOTransfer(DWORD dCobId,WORD wIndex,BYTE uSubIndex,DWORD dAbortCode);
	BOOL PrepareSendFrame_SendNMTService(BYTE uCommandSpecifier,BYTE uNodeId);
	BOOL PrepareSendFrame_SendCANFrame(DWORD dCobId,BYTE uLength,void* pDataBuffer,DWORD dDataBufferLength);
	BOOL PrepareSendFrame_RequestCANFrame(DWORD dCobId,BYTE uLength);

	BOOL CopyReceivedData_InitiateSDODownload(DWORD* pdCobId,WORD* pwIndex,BYTE* puSubIndex,DWORD* pdAbortCode);
	BOOL CopyReceivedData_DownloadSDOSegment(DWORD* pdCobId,BOOL* poToggle,DWORD* pdAbortCode);
	BOOL CopyReceivedData_InitiateSDOUpload(DWORD* pdCobId,BOOL* poExpeditedTransfer,BOOL* poSizeIndicated,BYTE* puNonValidNbOfBytes,WORD* pwIndex,BYTE* puSubIndex,void** ppDataBuffer,DWORD* pdDataBufferLength,DWORD* pdAbortCode);
	BOOL CopyReceivedData_UploadSDOSegment(DWORD* pdCobId,BOOL* poToggle,BYTE* puNonValidNbOfBytes,BOOL* poNoMoreSegments,void** ppSegDataBuffer,DWORD* pdSegDataBufferLength,DWORD* pdAbortCode);
	BOOL CopyReceivedData_NetworkIndication(DWORD* pdCobId, WORD* pwTargetNetworkId, BYTE* puTargetNodeId, DWORD* pdAbortCode);
	BOOL CopyReceivedData_RequestCANFrame(void** ppDataBuffer,DWORD* pdDataBufferLength);
	BOOL CopyReceivedData_ReceivedCANFrame(void** ppDataBuffer,DWORD* pdDataBufferLength);

	static BOOL CheckReceivedCommandSpecifier(CCANopenFrame* pSendingFrame,CCANopenFrame* pReceivingFrame);
	static BOOL CheckReceivedMultiplexor(CCANopenFrame* pSendingFrame,CCANopenFrame* pReceivingFrame);
	static BOOL CheckReceivedTargetNetwork(CCANopenFrame* pSendingFrame,CCANopenFrame* pReceivingFrame);

private:
	void ResetFrame();

private:
	#pragma pack(push,1)
	typedef union t_CanData
	{
		BYTE aDataByte[8];

		struct t_InitiateSDODownload_SendFrame
		{
				BYTE	s			:1;
				BYTE	e			:1;
				BYTE	n			:2;
				BYTE	X			:1;
				BYTE	ccs			:3;
				WORD	index;
				BYTE	subIndex;
				BYTE	data[4];

		} sInitiateSDODownload_SendFrame;

		struct t_InitiateSDODownload_ReceiveFrame
		{
				BYTE	X			:5;
				BYTE	scs			:3;
				WORD	index;
				BYTE	subIndex;
				BYTE	reserved[4];

		} sInitiateSDODownload_ReceiveFrame;

		struct t_DownloadSDOSegment_SendFrame
		{
				BYTE	c			:1;
				BYTE	n			:3;
				BYTE	t			:1;
				BYTE	ccs			:3;
				BYTE	data[7];

		} sDownloadSDOSegment_SendFrame;

		struct t_DownloadSDOSegment_ReceiveFrame
		{
				BYTE	X			:4;
				BYTE	t			:1;
				BYTE	scs			:3;
				BYTE	reserved[7];

		} sDownloadSDOSegment_ReceiveFrame;

		struct t_InitiateSDOUpload_SendFrame
		{
				BYTE	X			:5;
				BYTE	ccs			:3;
				WORD	index;
				BYTE	subIndex;
				BYTE	reserved[4];

		} sInitiateSDOUpload_SendFrame;

		struct t_InitiateSDOUpload_ReceiveFrame
		{
				BYTE	s			:1;
				BYTE	e			:1;
				BYTE	n			:2;
				BYTE	X			:1;
				BYTE	scs			:3;
				WORD	index;
				BYTE	subIndex;
				BYTE	data[4];

		} sInitiateSDOUpload_ReceiveFrame;

		struct t_UploadSDOSegment_SendFrame
		{
				BYTE	X			:4;
				BYTE	t			:1;
				BYTE	ccs			:3;
				BYTE	reserved[7];

		} sUploadSDOSegment_SendFrame;

		struct t_UploadSDOSegment_ReceiveFrame
		{
				BYTE	c			:1;
				BYTE	n			:3;
				BYTE	t			:1;
				BYTE	scs			:3;
				BYTE	data[7];

		} sUploadSDOSegment_ReceiveFrame;

		struct t_NetworkIndication_SendFrame
		{
				BYTE	r			:5;
				BYTE	ccs			:3;
				WORD	m1;
				BYTE	m2;
				BYTE	reserved[4];

		} sNetworkIndication_SendFrame;

		struct t_NetworkIndication_ReceiveFrame
		{
				BYTE	r			:5;
				BYTE	scs			:3;
				WORD	m1;
				BYTE	m2;
				BYTE	data[4];

		} sNetworkIndication_ReceiveFrame;

		struct t_AbortTransfer_AbortFrame
		{
				BYTE	X			:5;
				BYTE	cs			:3;
				WORD	index;
				BYTE	subIndex;
				DWORD	abortCode;

		} sAbortTransfer_AbortFrame;

		struct t_SendNMTService_SendFrame
		{
				BYTE	CS;
				BYTE	nodeId;

		} sSendNMTService_SendFrame;

	} t_CanData;

	typedef struct t_CANopenFrame
	{
		DWORD		dId;			//11-Bit Identifier
		BOOL		oRtr;			//Remote Request Bit
		WORD		wDlc;			//Frame Length
		t_CanData	nCanData;		//8 Databytes
	};
	#pragma pack(pop)

	t_CANopenFrame m_FrameBuffer;
	DWORD m_FrameBufferSize;
};

#endif // !defined(AFX_CANOPENFRAME_H__AE9DFEBF_E4CD_4B15_9706_71D18ABDE334__INCLUDED_)
