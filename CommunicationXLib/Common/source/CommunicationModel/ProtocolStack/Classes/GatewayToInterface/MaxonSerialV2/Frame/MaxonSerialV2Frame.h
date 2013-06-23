// MaxonSerialV2Frame.h: Schnittstelle f�r die Klasse CMaxonSerialV2Frame.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MaxonSerialV2Frame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
#define AFX_MaxonSerialV2Frame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

#include <MmcTypeDefinition.h>

enum EReceiveFrameState
{
	RFS_WAITING_FOR_DLE, 
	RFS_WAITING_FOR_STX,
	RFS_WAITING_FOR_OPCODE,
	RFS_WAITING_FOR_LEN,
	RFS_WAITING_FOR_DATA_AND_CRC,
	RFS_FRAME_RECEIVED
};


class CMaxonSerialV2Frame
{
public:
	CMaxonSerialV2Frame();
	virtual ~CMaxonSerialV2Frame();

	BOOL ResetFrame();
	BOOL PrepareSendFrame(BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc);
	BOOL PrepareStuffedSendFrame(BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc);
	BOOL PrepareReceiveFrame(BYTE uOpCode,BYTE uLen, DWORD p_ulRetDataBufferSize);
	BOOL CopyReceivedData(BYTE* puOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc);

	BOOL CheckCRC();

	void* GetFramePointer();
	void* GetDlePointer();
	void* GetStxPointer();
	void* GetOpCodePointer();
	void* GetLenPointer();
	void* GetDataPointer();
	void* GetCrcPointer();
	
	DWORD GetFrameSize();
	DWORD GetDleSize();
	DWORD GetStxSize();
	DWORD GetOpCodeSize();
	DWORD GetLenSize();
	DWORD GetDataSize();
	DWORD GetCrcSize();

	static BYTE GetDleCharacter();
	static BYTE GetStxCharacter();

	//Stuffing
	static BOOL StuffingData(BYTE* p_pData, DWORD p_ulDataLength, BYTE*& p_rpStuffedData, DWORD& p_rStuffedDataLength, DWORD& p_rNumberOfStuffedBytes);
	static BOOL DestuffingData(BYTE* p_pData, DWORD& p_rulDataLength, DWORD& p_rNumberOfStuffedBytes, BOOL& p_roStuffingBytePending);
	static BOOL GetNumberOfStuffingBytes(BYTE* p_pData, DWORD p_ulDataLength, DWORD& p_rNumberOfStuffedBytes);
	static BOOL IsStuffingByte(BYTE p_uData);

private:
	BOOL GetCRC(void* pDataBuffer,DWORD nbOfBytes,WORD* pwCrc);
	WORD CalcFieldCRC(WORD* field,WORD nbOfWords);

private:
	const DWORD m_kDleSize;
	const DWORD m_kStxSize;
	const DWORD m_kOpCodeSize;
	const DWORD m_kLenSize;
	const DWORD m_kCrcSize;

	void* m_pFrameBuffer;
	DWORD m_FrameBufferSize;
};

#endif // !defined(AFX_MaxonSerialV2Frame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
