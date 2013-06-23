// MaxonSerialV1Frame.cpp: Implementierung der Klasse CMaxonSerialV1Frame.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MaxonSerialV1Frame.h"

#include <malloc.h>
#include <memory.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMaxonSerialV1Frame::CMaxonSerialV1Frame():
m_kLenSize(1),m_kCrcSize(2),m_kOpCodeSize(1)
{
	m_pFrameBuffer = NULL;
	m_FrameBufferSize = 0;
}

CMaxonSerialV1Frame::~CMaxonSerialV1Frame()
{
	ResetFrame();
}

/******************************************************************************/
BOOL CMaxonSerialV1Frame::ResetFrame()
{
	m_FrameBufferSize = 0;
	if(m_pFrameBuffer)
	{
		free(m_pFrameBuffer);
		m_pFrameBuffer = NULL;
		return TRUE;
	}

	return FALSE;
}

/******************************************************************************/
BOOL CMaxonSerialV1Frame::PrepareSendFrame(BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc)
{
	BYTE* pDest = NULL;
	DWORD dRoundedDataBufferLength;
	void* pRoundedDataBuffer;

	//Delete old frame
	ResetFrame();

	if(puLen && ppDataBuffer && pdDataBufferLength && pwCrc)
	{
		//Check DataBuffer (round to even byteSize)
		if(*pdDataBufferLength % 2)
		{
			//Round
			dRoundedDataBufferLength = *pdDataBufferLength+1;

			//Allocate new Memory
			pRoundedDataBuffer = malloc(dRoundedDataBufferLength);
			memset(pRoundedDataBuffer,0,dRoundedDataBufferLength);
			memcpy(pRoundedDataBuffer,*ppDataBuffer,*pdDataBufferLength);

			//Change DataBuffer
			if(*ppDataBuffer) free(*ppDataBuffer);
			*ppDataBuffer = pRoundedDataBuffer;
			*pdDataBufferLength = dRoundedDataBufferLength;
		}

		//Calculate Len
		if(*pdDataBufferLength > 0) *puLen = (BYTE)(((*pdDataBufferLength+1)/2)-1); else *puLen = 0;

		//FrameBufferSize
		m_FrameBufferSize = m_kOpCodeSize + m_kLenSize + *pdDataBufferLength + m_kCrcSize;

		//Allocate memory
		m_pFrameBuffer = malloc(m_FrameBufferSize);
		memset(m_pFrameBuffer,0,m_FrameBufferSize);
		pDest = (BYTE*)m_pFrameBuffer;

		//Copy opCode to FrameBuffer
		*(pDest++) = uOpCode;

		//Copy len to FrameBuffer
		*(pDest++) = *puLen;

		//Copy data to FrameBuffer
		if(*ppDataBuffer)
		{
			memcpy(pDest,*ppDataBuffer,*pdDataBufferLength);
			pDest += *pdDataBufferLength;
		}

		//Copy Crc to FrameBuffer
		if(!GetCRC(m_pFrameBuffer,m_FrameBufferSize-m_kCrcSize,pwCrc)) return FALSE;
		*((WORD*)pDest) = *((WORD*)pwCrc);

		return TRUE;
	}

	return FALSE;
}

/******************************************************************************/
BOOL CMaxonSerialV1Frame::PrepareReceiveFrame(BYTE uOpCode,BYTE uLen)
{
	BYTE* pDest;

	//Delete old frame
	ResetFrame();

	//Total number of bytes
	m_FrameBufferSize = (uLen+1)*sizeof(WORD);
	m_FrameBufferSize += (m_kOpCodeSize+m_kLenSize);
	m_FrameBufferSize += m_kCrcSize;

	//Allocate memory
	m_pFrameBuffer = malloc(m_FrameBufferSize);
	pDest = (BYTE*)m_pFrameBuffer;
	memset(pDest,0,m_FrameBufferSize);

	//Copy opCode to FrameBuffer
	*(pDest++) = uOpCode;

	//Copy len to FrameBuffer
	*(pDest++) = uLen;

	return TRUE;
}

/******************************************************************************/
BOOL CMaxonSerialV1Frame::CopyReceivedData(BYTE* puOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc)
{
	//Copy OpCode
	if(puOpCode)
	{
		*puOpCode = *((BYTE*)GetOpCodePointer());
	}

	//Copy Len-1
	if(puLen)
	{
		*puLen = *((BYTE*)GetLengthPointer());
	}

	//Copy Data
	if(ppDataBuffer && pdDataBufferLength)
	{
		*pdDataBufferLength = GetDataSize();

		if(*ppDataBuffer) free(*ppDataBuffer);
		*ppDataBuffer = malloc(GetDataSize());
		memcpy(*ppDataBuffer,GetDataPointer(),GetDataSize());
	}

	//Copy Crc
	if(pwCrc)
	{
		*pwCrc = *((WORD*)GetCrcPointer());
	}

	return TRUE;
}

/******************************************************************************/
BOOL CMaxonSerialV1Frame::GetCRC(void* pDataBuffer,DWORD nbOfBytes,WORD* pwCrc)
{
	DWORD nbOfWords;

	if(pDataBuffer && pwCrc)
	{
		//Check Word Limits
		nbOfWords = nbOfBytes/sizeof(WORD);
		if(nbOfWords > USHRT_MAX) nbOfWords = USHRT_MAX;

		//Calculate CRC
		*pwCrc = CalcFieldCRC((WORD*)pDataBuffer,(WORD)nbOfWords);

		return TRUE;
	}

	return FALSE;
}

/******************************************************************************/
WORD CMaxonSerialV1Frame::CalcFieldCRC(WORD* field,WORD nbOfWords)
{
	//The crc sum is calculated word by word
	WORD shifter,c;
	WORD carry;
	WORD crc = 0;
	BOOL firstWord = TRUE;

	while(nbOfWords--)
	{
		shifter = 0x8000;
		c = *field++;
		if(firstWord)
		{
			c = SwapWord(c);
			firstWord = FALSE;
		}
		do
		{
			carry = crc & 0x8000;
			crc <<= 1;
			if(c & shifter) crc++;
			if(carry) crc ^= 0x1021;
			shifter >>= 1;
		} while(shifter);
	}

	shifter = 0x8000;
	c = 0;
	do
	{
		carry = crc & 0x8000;
		crc <<= 1;
		if(c & shifter) crc++;
		if(carry) crc ^= 0x1021;
		shifter >>= 1;
	} while(shifter);

	return crc;
}

/******************************************************************************/
WORD CMaxonSerialV1Frame::SwapWord(WORD data)
{
	WORD result;
	BYTE* p = (BYTE*)&data;
	BYTE* pp = (BYTE*)&result;

	*pp = *(p + 1);
	*(pp + 1) = *(p);

	return result;
}

/******************************************************************************/
void* CMaxonSerialV1Frame::GetOpCodePointer()
{
	return m_pFrameBuffer;
}

/******************************************************************************/
void* CMaxonSerialV1Frame::GetLengthPointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_kOpCodeSize);
}

/******************************************************************************/
void* CMaxonSerialV1Frame::GetDataPointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_kOpCodeSize + m_kLenSize);
}

/******************************************************************************/
void* CMaxonSerialV1Frame::GetCrcPointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_FrameBufferSize - m_kCrcSize);
}

/******************************************************************************/
DWORD CMaxonSerialV1Frame::GetOpCodeSize()
{
	return m_kOpCodeSize;
}

/******************************************************************************/
DWORD CMaxonSerialV1Frame::GetLengthSize()
{
	return m_kLenSize;
}

/******************************************************************************/
DWORD CMaxonSerialV1Frame::GetDataSize()
{
	return m_FrameBufferSize-m_kOpCodeSize-m_kLenSize-m_kCrcSize;
}

/******************************************************************************/
DWORD CMaxonSerialV1Frame::GetCrcSize()
{
	return m_kCrcSize;
}

/******************************************************************************/
BOOL CMaxonSerialV1Frame::CheckCRC()
{
	WORD calculatedCRC;

	if(GetCRC(m_pFrameBuffer,m_FrameBufferSize-m_kCrcSize,&calculatedCRC))
	{
		return (calculatedCRC == *((WORD*)GetCrcPointer()));
	}

	return FALSE;
}

/******************************************************************************/
DWORD CMaxonSerialV1Frame::GetFrameSize()
{
	return m_FrameBufferSize;
}

/******************************************************************************/
void* CMaxonSerialV1Frame::GetFramePointer()
{
	return m_pFrameBuffer;
}

