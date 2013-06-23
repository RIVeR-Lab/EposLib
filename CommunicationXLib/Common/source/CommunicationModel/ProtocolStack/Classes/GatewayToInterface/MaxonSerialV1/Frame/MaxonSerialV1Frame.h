// MaxonSerialV1Frame.h: Schnittstelle f�r die Klasse CMaxonSerialV1Frame.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MaxonSerialV1Frame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
#define AFX_MaxonSerialV1Frame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <MmcTypeDefinition.h>

#define	OK_ACK					0x4F
#define	NEG_ACK					0x46

class CMaxonSerialV1Frame
{
public:
	CMaxonSerialV1Frame();
	virtual ~CMaxonSerialV1Frame();

	BOOL ResetFrame();
	BOOL PrepareSendFrame(BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc);
	BOOL PrepareReceiveFrame(BYTE uOpCode,BYTE uLen);
	BOOL CopyReceivedData(BYTE* puOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc);

	BOOL CheckCRC();

	void* GetFramePointer();
	void* GetCrcPointer();
	void* GetDataPointer();
	void* GetLengthPointer();
	void* GetOpCodePointer();

	DWORD GetFrameSize();
	DWORD GetOpCodeSize();
	DWORD GetLengthSize();
	DWORD GetDataSize();
	DWORD GetCrcSize();

private:
	BOOL GetCRC(void* pDataBuffer,DWORD nbOfBytes,WORD* pwCrc);

	WORD CalcFieldCRC(WORD* field,WORD nbOfWords);
	WORD SwapWord(WORD data);

	const DWORD m_kLenSize;
	const DWORD m_kOpCodeSize;
	const DWORD m_kCrcSize;

	void* m_pFrameBuffer;
	DWORD m_FrameBufferSize;
};


#endif // !defined(AFX_MaxonSerialV1Frame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
