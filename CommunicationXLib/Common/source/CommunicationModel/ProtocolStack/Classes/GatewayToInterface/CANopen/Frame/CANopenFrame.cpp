// CANopenFrame.cpp: Implementierung der Klasse CCANopenFrame.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <malloc.h>
#include <memory.h>
#include "CANopenFrame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCANopenFrame::CCANopenFrame()
{
	m_FrameBufferSize = sizeof(m_FrameBuffer);
	memset(&m_FrameBuffer, 0, sizeof(m_FrameBuffer));
}

CCANopenFrame::~CCANopenFrame()
{
}

void CCANopenFrame::ResetFrame()
{
	memset(&m_FrameBuffer, 0, sizeof(m_FrameBuffer));
	m_FrameBufferSize = sizeof(m_FrameBuffer);
}

void* CCANopenFrame::GetCobIdPointer()
{
	return (void*)&m_FrameBuffer.dId;
}

DWORD CCANopenFrame::GetCobIdSize()
{
	return (DWORD)sizeof(m_FrameBuffer.dId);
}

void* CCANopenFrame::GetRtrPointer()
{
	return (void*)&m_FrameBuffer.oRtr;
}

DWORD CCANopenFrame::GetRtrSize()
{
	return (DWORD)sizeof(m_FrameBuffer.oRtr);
}

void* CCANopenFrame::GetDlcPointer()
{
	return (void*)&m_FrameBuffer.wDlc;
}

DWORD CCANopenFrame::GetDlcSize()
{
	return (DWORD)sizeof(m_FrameBuffer.wDlc);
}

void* CCANopenFrame::GetCanDataPointer()
{
	return (void*)&m_FrameBuffer.nCanData;
}

DWORD CCANopenFrame::GetCanDataSize()
{
	return (DWORD)sizeof(m_FrameBuffer.nCanData);
}

BOOL CCANopenFrame::PrepareSendFrame_InitiateSDODownload(DWORD p_ulCobId, BOOL p_oExpeditedTransfer, BOOL p_oSizeIndicated, BYTE p_ubNonValidNbOfBytes, WORD p_usIndex, BYTE p_ubSubIndex, void* p_pDataBuffer, DWORD p_ulDataBufferLength)
{
	const int FRAME_SIZE = 8;
	const int COMMAND_SPECIFIER = 1;
	const unsigned long MAX_DATA_SIZE = 4;

    BOOL oResult(FALSE);

	ResetFrame();

	if(p_pDataBuffer)
	{
		//Limit Values
		if(p_ulDataBufferLength > MAX_DATA_SIZE) p_ulDataBufferLength = MAX_DATA_SIZE;
		if(p_ubNonValidNbOfBytes > MAX_DATA_SIZE) p_ubNonValidNbOfBytes = MAX_DATA_SIZE;
		if(!p_oExpeditedTransfer || !p_oSizeIndicated) p_ubNonValidNbOfBytes = 0;

		//Frame
		m_FrameBuffer.dId = p_ulCobId;
		m_FrameBuffer.oRtr = FALSE;
		m_FrameBuffer.wDlc = FRAME_SIZE;

		m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.ccs = COMMAND_SPECIFIER;
		m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.n = p_ubNonValidNbOfBytes;
		m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.e = p_oExpeditedTransfer;
		m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.s = p_oSizeIndicated;
		m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.index = p_usIndex;
		m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.subIndex = p_ubSubIndex;
		memcpy(m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.data, p_pDataBuffer, p_ulDataBufferLength);

		oResult = TRUE;
	}

	return oResult;
}

BOOL CCANopenFrame::PrepareSendFrame_AbortSDOTransfer(DWORD p_ulCobId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulAbortCode)
{
	const int FRAME_SIZE = 8;
	const int COMMAND_SPECIFIER = 4;

	ResetFrame();

	//Frame
	m_FrameBuffer.dId = p_ulCobId;
	m_FrameBuffer.oRtr = FALSE;
	m_FrameBuffer.wDlc = FRAME_SIZE;

	m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.cs = COMMAND_SPECIFIER;
	m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.index = p_usIndex;
	m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.subIndex = p_ubSubIndex;
	m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.abortCode = p_ulAbortCode;

	return TRUE;
}

BOOL CCANopenFrame::PrepareSendFrame_DownloadSDOSegment(DWORD p_ulCobId, BOOL p_oToggle, BYTE p_ubNonValidNbOfBytes, BOOL p_oNoMoreSegments, void* p_pSegDataBuffer, DWORD p_ulSegDataBufferLength)
{
	const int FRAME_SIZE = 8;
	const int COMMAND_SPECIFIER = 0;
	const unsigned long MAX_DATA_SIZE = 7;

    BOOL oResult(FALSE);

	ResetFrame();

	if(p_pSegDataBuffer)
	{
		//Limit Values
		if(p_ulSegDataBufferLength > MAX_DATA_SIZE) p_ulSegDataBufferLength = MAX_DATA_SIZE;
		if(p_ubNonValidNbOfBytes > MAX_DATA_SIZE) p_ubNonValidNbOfBytes = MAX_DATA_SIZE;

		//Frame
		m_FrameBuffer.dId = p_ulCobId;
		m_FrameBuffer.oRtr = FALSE;
		m_FrameBuffer.wDlc = FRAME_SIZE;

		m_FrameBuffer.nCanData.sDownloadSDOSegment_SendFrame.ccs = COMMAND_SPECIFIER;
		m_FrameBuffer.nCanData.sDownloadSDOSegment_SendFrame.n = p_ubNonValidNbOfBytes;
		m_FrameBuffer.nCanData.sDownloadSDOSegment_SendFrame.c = p_oNoMoreSegments;
		m_FrameBuffer.nCanData.sDownloadSDOSegment_SendFrame.t = p_oToggle;
		memcpy(m_FrameBuffer.nCanData.sDownloadSDOSegment_SendFrame.data, p_pSegDataBuffer, p_ulSegDataBufferLength);

		oResult = TRUE;
	}

	return oResult;
}

BOOL CCANopenFrame::PrepareSendFrame_InitiateSDOUpload(DWORD p_ulCobId, WORD p_usIndex, BYTE p_ubSubIndex)
{
	const int FRAME_SIZE = 8;
	const int COMMAND_SPECIFIER = 2;

	ResetFrame();

	//Frame
	m_FrameBuffer.dId = p_ulCobId;
	m_FrameBuffer.oRtr = FALSE;
	m_FrameBuffer.wDlc = FRAME_SIZE;

	m_FrameBuffer.nCanData.sInitiateSDOUpload_SendFrame.ccs = COMMAND_SPECIFIER;
	m_FrameBuffer.nCanData.sInitiateSDOUpload_SendFrame.index = p_usIndex;
	m_FrameBuffer.nCanData.sInitiateSDOUpload_SendFrame.subIndex = p_ubSubIndex;

	return TRUE;
}

BOOL CCANopenFrame::PrepareSendFrame_UploadSDOSegment(DWORD p_ulCobId, BOOL p_oToggle)
{
	const int FRAME_SIZE = 8;
	const int COMMAND_SPECIFIER = 3;

	ResetFrame();

	//Frame
	m_FrameBuffer.dId = p_ulCobId;
	m_FrameBuffer.oRtr = FALSE;
	m_FrameBuffer.wDlc = FRAME_SIZE;

	m_FrameBuffer.nCanData.sUploadSDOSegment_SendFrame.ccs = COMMAND_SPECIFIER;
	m_FrameBuffer.nCanData.sUploadSDOSegment_SendFrame.t = p_oToggle;

	return TRUE;
}

BOOL CCANopenFrame::PrepareSendFrame_NetworkIndication(DWORD p_ulCobId, WORD p_usTargetNetworkId, BYTE p_ubTargetNodeId)
{
	const int FRAME_SIZE = 8;
	const int COMMAND_SPECIFIER = 7;

	ResetFrame();

	//Frame
	m_FrameBuffer.dId = p_ulCobId;
	m_FrameBuffer.oRtr = FALSE;
	m_FrameBuffer.wDlc = FRAME_SIZE;

	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.ccs = COMMAND_SPECIFIER;
	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.r = 0;
	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.m1 = p_usTargetNetworkId;
	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.m2 = p_ubTargetNodeId;
	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.reserved[0] = 0;
	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.reserved[1] = 0;
	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.reserved[2] = 0;
	m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.reserved[3] = 0;

	return TRUE;
}

BOOL CCANopenFrame::PrepareSendFrame_SendNMTService(BYTE p_ubCommandSpecifier, BYTE p_ubNodeId)
{
	const int COB_ID = 0;
	const int FRAME_SIZE = 2;

	ResetFrame();

	//Frame
	m_FrameBuffer.dId = COB_ID;
	m_FrameBuffer.oRtr = FALSE;
	m_FrameBuffer.wDlc = FRAME_SIZE;

	m_FrameBuffer.nCanData.aDataByte[0] = p_ubCommandSpecifier;
	m_FrameBuffer.nCanData.aDataByte[1] = p_ubNodeId;

	return TRUE;
}

BOOL CCANopenFrame::PrepareSendFrame_SendCANFrame(DWORD p_ulCobId, BYTE p_ubLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength)
{
	const unsigned long MAX_FRAME_SIZE = 8;

    BOOL oResult(FALSE);

	ResetFrame();

	if(p_pDataBuffer)
	{
		//Limit Values
		if(p_ulDataBufferLength > MAX_FRAME_SIZE) p_ulDataBufferLength = MAX_FRAME_SIZE;

		//Frame
		m_FrameBuffer.dId = p_ulCobId;
		m_FrameBuffer.oRtr = FALSE;
		m_FrameBuffer.wDlc = (WORD)p_ulDataBufferLength;
		memcpy(m_FrameBuffer.nCanData.aDataByte, p_pDataBuffer, p_ulDataBufferLength);

		oResult = TRUE;
	}

	return oResult;
}

BOOL CCANopenFrame::PrepareSendFrame_RequestCANFrame(DWORD p_ulCobId, BYTE p_ubLength)
{
	const int MAX_FRAME_SIZE = 8;

	ResetFrame();

	//Limit Values
	if(p_ubLength > MAX_FRAME_SIZE) p_ubLength = MAX_FRAME_SIZE;

	//Frame
	m_FrameBuffer.dId = p_ulCobId;
	m_FrameBuffer.oRtr = TRUE;
	m_FrameBuffer.wDlc = p_ubLength;

	return TRUE;
}

BOOL CCANopenFrame::CopyReceivedData_InitiateSDODownload(DWORD* p_pulCobId, WORD* p_pusIndex, BYTE* p_pubSubIndex, DWORD* p_pulAbortCode)
{
	const int COMMAND_SPECIFIER = 3;
	const int ABORT_COMMAND_SPECIFIER = 4;

    BOOL oResult(FALSE);

	if(p_pulCobId && p_pusIndex && p_pubSubIndex && p_pulAbortCode)
	{
		if(m_FrameBuffer.nCanData.sInitiateSDODownload_ReceiveFrame.scs == COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_pusIndex = m_FrameBuffer.nCanData.sInitiateSDODownload_ReceiveFrame.index;
			*p_pubSubIndex = m_FrameBuffer.nCanData.sInitiateSDODownload_ReceiveFrame.subIndex;
			*p_pulAbortCode = 0;

			oResult = TRUE;
		}
		else if(m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.cs == ABORT_COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_pusIndex = m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.index;
			*p_pubSubIndex = m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.subIndex;
			*p_pulAbortCode = m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.abortCode;

			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CCANopenFrame::CopyReceivedData_DownloadSDOSegment(DWORD* p_pulCobId, BOOL* p_poToggle, DWORD* p_pulAbortCode)
{
	const int COMMAND_SPECIFIER = 1;
	const int ABORT_COMMAND_SPECIFIER = 4;

    BOOL oResult(FALSE);

	if(p_pulCobId && p_poToggle && p_pulAbortCode)
	{
		if(m_FrameBuffer.nCanData.sDownloadSDOSegment_ReceiveFrame.scs == COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_poToggle = m_FrameBuffer.nCanData.sDownloadSDOSegment_ReceiveFrame.t;
			*p_pulAbortCode = 0;

			oResult = TRUE;
		}
		else if(m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.cs == ABORT_COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_pulAbortCode = m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.abortCode;

			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CCANopenFrame::CopyReceivedData_InitiateSDOUpload(DWORD* p_pulCobId, BOOL* p_poExpeditedTransfer, BOOL* p_poSizeIndicated, BYTE* p_pubNonValidNbOfBytes, WORD* p_pusIndex, BYTE* p_pubSubIndex, void** p_ppDataBuffer, DWORD* p_pulDataBufferLength, DWORD* p_pulAbortCode)
{
	const int DATA_SIZE = 4;
	const int COMMAND_SPECIFIER = 2;
	const int ABORT_COMMAND_SPECIFIER = 4;

    BOOL oResult(FALSE);

	if(p_pulCobId && p_poExpeditedTransfer && p_poSizeIndicated && p_pubNonValidNbOfBytes && p_pusIndex && p_pubSubIndex && p_ppDataBuffer && p_pulDataBufferLength && p_pulAbortCode)
	{
		if(m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.scs == COMMAND_SPECIFIER)
		{
			//Prepare DataBuffer
			*p_pulCobId = m_FrameBuffer.dId;
			*p_poExpeditedTransfer = m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.e;
			*p_poSizeIndicated = m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.s;
			*p_pubNonValidNbOfBytes = m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.n;
			*p_pusIndex = m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.index;
			*p_pubSubIndex = m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.subIndex;

			*p_pulDataBufferLength = DATA_SIZE;
			*p_ppDataBuffer = malloc(*p_pulDataBufferLength);
			memcpy(*p_ppDataBuffer, m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.data, *p_pulDataBufferLength);

			*p_pulAbortCode = 0;

			oResult = TRUE;
		}
		else if(m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.cs == ABORT_COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_poExpeditedTransfer = FALSE;
			*p_poSizeIndicated = FALSE;
			*p_pubNonValidNbOfBytes = 0;
			*p_pusIndex = 0;
			*p_pubSubIndex = 0;

			*p_pulDataBufferLength = DATA_SIZE;
			*p_ppDataBuffer = malloc(*p_pulDataBufferLength);
			memset(*p_ppDataBuffer, 0, *p_pulDataBufferLength);

			*p_pulAbortCode = m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.abortCode;

			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CCANopenFrame::CopyReceivedData_UploadSDOSegment(DWORD* p_pulCobId, BOOL* p_poToggle, BYTE* p_pubNonValidNbOfBytes, BOOL* p_poNoMoreSegments, void** p_ppSegDataBuffer, DWORD* p_pulSegDataBufferLength, DWORD* p_pulAbortCode)
{
	const int DATA_SIZE = 7;
	const int COMMAND_SPECIFIER = 0;
	const int ABORT_COMMAND_SPECIFIER = 4;

    BOOL oResult(FALSE);

	if(p_pulCobId && p_poToggle && p_pubNonValidNbOfBytes && p_poNoMoreSegments && p_ppSegDataBuffer && p_pulSegDataBufferLength && p_pulAbortCode)
	{
		if(m_FrameBuffer.nCanData.sUploadSDOSegment_ReceiveFrame.scs == COMMAND_SPECIFIER)
		{
			//Prepare DataBuffer
			*p_pulCobId = m_FrameBuffer.dId;
			*p_poToggle = m_FrameBuffer.nCanData.sUploadSDOSegment_ReceiveFrame.t;
			*p_pubNonValidNbOfBytes = m_FrameBuffer.nCanData.sUploadSDOSegment_ReceiveFrame.n;
			*p_poNoMoreSegments = m_FrameBuffer.nCanData.sUploadSDOSegment_ReceiveFrame.c;

			*p_pulSegDataBufferLength = DATA_SIZE;
			*p_ppSegDataBuffer = malloc(*p_pulSegDataBufferLength);
			memcpy(*p_ppSegDataBuffer, m_FrameBuffer.nCanData.sUploadSDOSegment_ReceiveFrame.data, *p_pulSegDataBufferLength);

			*p_pulAbortCode = 0;

			oResult = TRUE;
		}
		else if(m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.cs == ABORT_COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_poToggle = FALSE;
			*p_pubNonValidNbOfBytes = 0;
			*p_poNoMoreSegments = TRUE;

			*p_pulSegDataBufferLength = DATA_SIZE;
			*p_ppSegDataBuffer = malloc(*p_pulSegDataBufferLength);
			memset(*p_ppSegDataBuffer, 0, *p_pulSegDataBufferLength);

			*p_pulAbortCode = m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.abortCode;

			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CCANopenFrame::CopyReceivedData_NetworkIndication(DWORD* p_pulCobId, WORD* p_usTargetNetworkId, BYTE* p_pubTargetNodeId, DWORD* p_pulAbortCode)
{
	const int COMMAND_SPECIFIER = 7;
	const int ABORT_COMMAND_SPECIFIER = 4;

    BOOL oResult(FALSE);

	if(p_pulCobId && p_usTargetNetworkId && p_pubTargetNodeId && p_pulAbortCode)
	{
		if(m_FrameBuffer.nCanData.sNetworkIndication_ReceiveFrame.scs == COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_usTargetNetworkId = m_FrameBuffer.nCanData.sNetworkIndication_ReceiveFrame.m1;
			*p_pubTargetNodeId = m_FrameBuffer.nCanData.sNetworkIndication_ReceiveFrame.m2;
			*p_pulAbortCode = 0;

			oResult = TRUE;
		}
		else if(m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.cs == ABORT_COMMAND_SPECIFIER)
		{
			*p_pulCobId = m_FrameBuffer.dId;
			*p_usTargetNetworkId = 0;
			*p_pubTargetNodeId = 0;
			*p_pulAbortCode = m_FrameBuffer.nCanData.sAbortTransfer_AbortFrame.abortCode;

			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CCANopenFrame::CopyReceivedData_RequestCANFrame(void** p_ppDataBuffer, DWORD* p_pulDataBufferLength)
{
	const unsigned long DATA_SIZE = 8;

    BOOL oResult(FALSE);

	if(p_ppDataBuffer && p_pulDataBufferLength)
	{
		//Prepare DataBuffer
		*p_pulDataBufferLength = m_FrameBuffer.wDlc;
		if(*p_pulDataBufferLength > DATA_SIZE) *p_pulDataBufferLength = DATA_SIZE;
		*p_ppDataBuffer = malloc(*p_pulDataBufferLength);
		memcpy(*p_ppDataBuffer, m_FrameBuffer.nCanData.aDataByte, *p_pulDataBufferLength);

		oResult = TRUE;
	}

	return oResult;
}

BOOL CCANopenFrame::CopyReceivedData_ReceivedCANFrame(void** p_ppDataBuffer, DWORD* p_pulDataBufferLength)
{
	return CopyReceivedData_RequestCANFrame(p_ppDataBuffer, p_pulDataBufferLength);
}

DWORD CCANopenFrame::GetCobId()
{
	return m_FrameBuffer.dId;
}

BOOL CCANopenFrame::GetRtr()
{
	return (BOOL)m_FrameBuffer.oRtr;
}

BYTE CCANopenFrame::GetDlc()
{
	return (BYTE)m_FrameBuffer.wDlc;
}

void CCANopenFrame::SetCobId(DWORD p_ulCobId)
{
	m_FrameBuffer.dId = p_ulCobId;
}

BOOL CCANopenFrame::CheckReceivedCommandSpecifier(CCANopenFrame* p_pSendingFrame, CCANopenFrame* p_pReceivingFrame)
{
	const int COMMAND_SPECIFIER_RIGHT_SHIFT = 5;

	const int INITIATE_SDO_DOWNLOAD_SEND = 1;
	const int INITIATE_SDO_DOWNLOAD_RECEIVE = 3;
	const int DOWNLOAD_SDO_SEGMENT_SEND = 0;
	const int DOWNLOAD_SDO_SEGMENT_RECEIVE = 1;
	const int INITIATE_SDO_UPLOAD_SEND = 2;
	const int INITIATE_SDO_UPLOAD_RECEIVE = 2;
	const int UPLOAD_SDO_SEGMENT_SEND = 3;
	const int UPLOAD_SDO_SEGMENT_RECEIVE = 0;
	const int NETWORK_INDICATION_SEND = 7;
	const int NETWORK_INDICATION_RECEIVE = 7;
	const int ABORT = 4;

	BYTE sendCommandSpecifier;
	BYTE receiveCommandSpecifier;

	if(p_pSendingFrame && p_pReceivingFrame)
	{
		//Command Specifiers
		sendCommandSpecifier = p_pSendingFrame->m_FrameBuffer.nCanData.aDataByte[0] >> COMMAND_SPECIFIER_RIGHT_SHIFT;
		receiveCommandSpecifier = p_pReceivingFrame->m_FrameBuffer.nCanData.aDataByte[0] >> COMMAND_SPECIFIER_RIGHT_SHIFT;

		//Allow Abort
		if(receiveCommandSpecifier == ABORT) return TRUE;

		//Check Combinations
		switch(sendCommandSpecifier)
		{
			case INITIATE_SDO_DOWNLOAD_SEND: return (receiveCommandSpecifier == INITIATE_SDO_DOWNLOAD_RECEIVE);
			case DOWNLOAD_SDO_SEGMENT_SEND: return (receiveCommandSpecifier == DOWNLOAD_SDO_SEGMENT_RECEIVE);
			case INITIATE_SDO_UPLOAD_SEND: return (receiveCommandSpecifier == INITIATE_SDO_UPLOAD_RECEIVE);
			case UPLOAD_SDO_SEGMENT_SEND: return (receiveCommandSpecifier == UPLOAD_SDO_SEGMENT_RECEIVE);
			case NETWORK_INDICATION_SEND: return (receiveCommandSpecifier == NETWORK_INDICATION_RECEIVE);
		}
	}

	return FALSE;
}

BOOL CCANopenFrame::CheckReceivedMultiplexor(CCANopenFrame* p_pSendingFrame, CCANopenFrame* p_pReceivingFrame)
{
	const int COMMAND_SPECIFIER_RIGHT_SHIFT = 5;

	const int INITIATE_SDO_DOWNLOAD_SEND = 1;
	const int INITIATE_SDO_UPLOAD_SEND = 2;

	BYTE sendCommandSpecifier;

	if(p_pSendingFrame && p_pReceivingFrame)
	{
		//Command Specifiers
		sendCommandSpecifier = p_pSendingFrame->m_FrameBuffer.nCanData.aDataByte[0] >> COMMAND_SPECIFIER_RIGHT_SHIFT;

		//Check Index & SubIndex
		switch(sendCommandSpecifier)
		{
			case INITIATE_SDO_DOWNLOAD_SEND:
				{
                    return ((p_pSendingFrame->m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.index == p_pReceivingFrame->m_FrameBuffer.nCanData.sInitiateSDODownload_ReceiveFrame.index) &&
							(p_pSendingFrame->m_FrameBuffer.nCanData.sInitiateSDODownload_SendFrame.subIndex == p_pReceivingFrame->m_FrameBuffer.nCanData.sInitiateSDODownload_ReceiveFrame.subIndex));
				};

			case INITIATE_SDO_UPLOAD_SEND:
				{
                    return ((p_pSendingFrame->m_FrameBuffer.nCanData.sInitiateSDOUpload_SendFrame.index == p_pReceivingFrame->m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.index) &&
							(p_pSendingFrame->m_FrameBuffer.nCanData.sInitiateSDOUpload_SendFrame.subIndex == p_pReceivingFrame->m_FrameBuffer.nCanData.sInitiateSDOUpload_ReceiveFrame.subIndex));
				};

			default: return TRUE;
		}
	}

	return FALSE;
}

BOOL CCANopenFrame::CheckReceivedTargetNetwork(CCANopenFrame* p_pSendingFrame, CCANopenFrame* p_pReceivingFrame)
{
	const int COMMAND_SPECIFIER_RIGHT_SHIFT = 5;

	const int NETWORK_INDICATION_Send = 7;

	BYTE sendCommandSpecifier = 0;

	if(p_pSendingFrame && p_pReceivingFrame)
	{
		//Command Specifiers
		sendCommandSpecifier = p_pSendingFrame->m_FrameBuffer.nCanData.aDataByte[0] >> COMMAND_SPECIFIER_RIGHT_SHIFT;

		//Check Index & SubIndex
		switch(sendCommandSpecifier)
		{
			case NETWORK_INDICATION_Send:
				{
					return ((p_pSendingFrame->m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.m1 == p_pReceivingFrame->m_FrameBuffer.nCanData.sNetworkIndication_ReceiveFrame.m1) &&
							(p_pSendingFrame->m_FrameBuffer.nCanData.sNetworkIndication_SendFrame.m2 == p_pReceivingFrame->m_FrameBuffer.nCanData.sNetworkIndication_ReceiveFrame.m2));
				};

			default: return TRUE;
		}
	}

	return FALSE;
}
