// MaxonSerialV2Frame.cpp: Implementierung der Klasse CMaxonSerialV2Frame.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MaxonSerialV2Frame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <malloc.h>
#include <memory.h>
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMaxonSerialV2Frame::CMaxonSerialV2Frame():
m_kDleSize(1),
m_kStxSize(1),
m_kOpCodeSize(1),
m_kLenSize(1),
m_kCrcSize(2)
{
	m_pFrameBuffer = NULL;
	m_FrameBufferSize = 0;
}

CMaxonSerialV2Frame::~CMaxonSerialV2Frame()
{
	ResetFrame();
}

BYTE CMaxonSerialV2Frame::GetDleCharacter()
{
	const BYTE DLE_CHARACTER = 0x90;

	return DLE_CHARACTER;
}
BYTE CMaxonSerialV2Frame::GetStxCharacter()
{
	const BYTE STX_CHARACTER = 0x02;

	return STX_CHARACTER;
}

/******************************************************************************/
BOOL CMaxonSerialV2Frame::ResetFrame()
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
BOOL CMaxonSerialV2Frame::PrepareSendFrame(BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc)
{
	BYTE* pDest = NULL;
	DWORD dRoundedDataBufferLength = 0;
	void* pRoundedDataBuffer = NULL;

	//Delete Old Frame
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
		if(*pdDataBufferLength > 0) *puLen = (BYTE)(((*pdDataBufferLength+1)/2)); else *puLen = 0;

		//FrameBufferSize
		m_FrameBufferSize = m_kDleSize + m_kStxSize + m_kOpCodeSize + m_kLenSize + *pdDataBufferLength + m_kCrcSize;

		//Allocate memory
		m_pFrameBuffer = malloc(m_FrameBufferSize);
		memset(m_pFrameBuffer,0,m_FrameBufferSize);
		pDest = (BYTE*)m_pFrameBuffer;

		//Copy DLE to FrameBuffer
		*(pDest++) = GetDleCharacter();
		
		//Copy STX to FrameBuffer
		*(pDest++) = GetStxCharacter();

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
		if(!GetCRC(GetOpCodePointer(),m_FrameBufferSize-m_kDleSize-m_kStxSize-m_kCrcSize,pwCrc)) return FALSE;
		*((WORD*)pDest) = *((WORD*)pwCrc);

		return TRUE;
	}

	return FALSE;
}

BOOL CMaxonSerialV2Frame::PrepareStuffedSendFrame(BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc)
{
	BYTE* pDest = NULL;
	DWORD dRoundedDataBufferLength = 0;
	void* pRoundedDataBuffer = NULL;
	BYTE* pDummyBuffer = NULL;
	DWORD dDummyBufferLength = 0;
	BYTE* pStuffedBuffer = NULL;
	DWORD dStuffedBufferLength = 0;
	DWORD ulNumberOfStuffedBytes = 0;
	BOOL oResult = FALSE;

	//Delete Old Frame
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
		if(*pdDataBufferLength > 0) *puLen = (BYTE)(((*pdDataBufferLength+1)/2)); else *puLen = 0;

		//DummyBufferSize
		dDummyBufferLength = m_kOpCodeSize + m_kLenSize + *pdDataBufferLength + m_kCrcSize;

		//Allocate memory
		pDummyBuffer = (BYTE*)malloc(dDummyBufferLength);
		memset(pDummyBuffer,0,dDummyBufferLength);
		pDest = (BYTE*)pDummyBuffer;

		//Copy opCode to DummyBuffer
		*(pDest++) = uOpCode;

		//Copy len to DummyBuffer
		*(pDest++) = *puLen;

		//Copy data to DummyBuffer
		if(*ppDataBuffer)
		{
			memcpy(pDest,*ppDataBuffer,*pdDataBufferLength);
			pDest += *pdDataBufferLength;
		}

		//Calculate and Copy Crc to DummyBuffer
		if(!GetCRC(pDummyBuffer,dDummyBufferLength-m_kCrcSize,pwCrc)) return FALSE;
		*((WORD*)pDest) = *((WORD*)pwCrc);

		//Stuffing Buffer
		if(StuffingData(pDummyBuffer,dDummyBufferLength,pStuffedBuffer,dStuffedBufferLength,ulNumberOfStuffedBytes))
		{
			//FrameBuffer Size
			m_FrameBufferSize = m_kDleSize + m_kStxSize + dStuffedBufferLength;

			//Allocate memory
			m_pFrameBuffer = malloc(m_FrameBufferSize);
			memset(m_pFrameBuffer,0,m_FrameBufferSize);
			pDest = (BYTE*)m_pFrameBuffer;

			//Copy DLE to FrameBuffer
			*(pDest++) = GetDleCharacter();
			
			//Copy STX to FrameBuffer
			*(pDest++) = GetStxCharacter();

			//Copy data to FrameBuffer
			if(pStuffedBuffer)
			{
				memcpy(pDest,pStuffedBuffer,dStuffedBufferLength);
				pDest += dStuffedBufferLength;
				oResult = TRUE;
			}
		}

		if(pDummyBuffer) free(pDummyBuffer);
		if(pStuffedBuffer) free(pStuffedBuffer);
	}

	return oResult;
}

/******************************************************************************/
BOOL CMaxonSerialV2Frame::PrepareReceiveFrame(BYTE uOpCode, BYTE uLen, DWORD p_ulRetDataBufferSize)
{
	DWORD ulDataSize(0);
	BYTE* pDest;

	//Delete old frame
	ResetFrame();

	//Data Size
	if(uLen > 0)
	{
		//Data Size defined by Len Parameter
		ulDataSize = (uLen)*sizeof(WORD);
	}
	else
	{
		//Data Size defined by prepared return data buffer (Len = Undefined)
		ulDataSize = p_ulRetDataBufferSize;
	}

	//Total number of bytes
	m_FrameBufferSize = (m_kDleSize + m_kStxSize);
	m_FrameBufferSize += ulDataSize;
	m_FrameBufferSize += (m_kOpCodeSize+m_kLenSize);
	m_FrameBufferSize += m_kCrcSize;

	//Allocate memory
	m_pFrameBuffer = malloc(m_FrameBufferSize);
	pDest = (BYTE*)m_pFrameBuffer;
	memset(pDest,0,m_FrameBufferSize);

	//Copy DLE to FrameBuffer
	*(pDest++) = GetDleCharacter();

	//Copy STX to FrameBuffer
	*(pDest++) = GetStxCharacter();

	//Copy opCode to FrameBuffer
	*(pDest++) = uOpCode;

	//Copy len to FrameBuffer
	*(pDest++) = uLen;

	return TRUE;
}

/******************************************************************************/
BOOL CMaxonSerialV2Frame::CopyReceivedData(BYTE* puOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc)
{
	//Copy OpCode
	if(puOpCode)
	{
		*puOpCode = *((BYTE*)GetOpCodePointer());
	}

	//Copy Len
	if(puLen)
	{
		*puLen = *((BYTE*)GetLenPointer());
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
BOOL CMaxonSerialV2Frame::GetCRC(void* pDataBuffer,DWORD nbOfBytes,WORD* pwCrc)
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
WORD CMaxonSerialV2Frame::CalcFieldCRC(WORD* field,WORD nbOfWords)
{
	//The crc sum is calculated word by word
	WORD shifter,c;
	WORD carry;
	WORD crc = 0;

	while(nbOfWords--)
	{
		shifter = 0x8000;
		c = *field++;
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
void* CMaxonSerialV2Frame::GetDlePointer()
{
	return m_pFrameBuffer;
}

/******************************************************************************/
void* CMaxonSerialV2Frame::GetStxPointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_kDleSize);
}

/******************************************************************************/
void* CMaxonSerialV2Frame::GetOpCodePointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_kDleSize + m_kStxSize);
}

/******************************************************************************/
void* CMaxonSerialV2Frame::GetLenPointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_kDleSize + m_kStxSize + m_kOpCodeSize);
}

/******************************************************************************/
void* CMaxonSerialV2Frame::GetDataPointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_kDleSize + m_kStxSize + m_kOpCodeSize + m_kLenSize);
}

/******************************************************************************/
void* CMaxonSerialV2Frame::GetCrcPointer()
{
	return (void*)((BYTE*)m_pFrameBuffer + m_FrameBufferSize - m_kCrcSize);
}

/******************************************************************************/
DWORD CMaxonSerialV2Frame::GetDleSize()
{
	return m_kDleSize;
}

/******************************************************************************/
DWORD CMaxonSerialV2Frame::GetStxSize()
{
	return m_kStxSize;
}

/******************************************************************************/
DWORD CMaxonSerialV2Frame::GetOpCodeSize()
{
	return m_kOpCodeSize;
}

/******************************************************************************/
DWORD CMaxonSerialV2Frame::GetLenSize()
{
	return m_kLenSize;
}

/******************************************************************************/
DWORD CMaxonSerialV2Frame::GetDataSize()
{
	return m_FrameBufferSize-m_kDleSize-m_kStxSize-m_kOpCodeSize-m_kLenSize-m_kCrcSize;
}

/******************************************************************************/
DWORD CMaxonSerialV2Frame::GetCrcSize()
{
	return m_kCrcSize;
}

/******************************************************************************/
BOOL CMaxonSerialV2Frame::CheckCRC()
{
	WORD calculatedCRC;

	if(GetCRC(GetOpCodePointer(),m_FrameBufferSize-m_kDleSize-m_kStxSize-m_kCrcSize,&calculatedCRC))
	{
		return (calculatedCRC == *((WORD*)GetCrcPointer()));
	}

	return FALSE;
}

/******************************************************************************/
DWORD CMaxonSerialV2Frame::GetFrameSize()
{
	return m_FrameBufferSize;
}

/******************************************************************************/
void* CMaxonSerialV2Frame::GetFramePointer()
{
	return m_pFrameBuffer;
}

BOOL CMaxonSerialV2Frame::StuffingData(BYTE* p_pData, DWORD p_ulDataLength, BYTE*& p_rpStuffedData, DWORD& p_rStuffedDataLength, DWORD& p_rNumberOfStuffedBytes)
{
	BYTE* pData = NULL;
	BYTE* pStuffedData = NULL;
	BOOL oResult = FALSE;

	if(p_pData)
	{
		if(GetNumberOfStuffingBytes(p_pData,p_ulDataLength,p_rNumberOfStuffedBytes))
		{
			//Prepare Buffer
			p_rStuffedDataLength = p_ulDataLength + p_rNumberOfStuffedBytes;
			p_rpStuffedData = (BYTE*)malloc(p_rStuffedDataLength);
			
			//Stuffing
			pData = p_pData;
			pStuffedData = p_rpStuffedData;
			for(DWORD i=0;i<p_ulDataLength;i++)
			{
				if(IsStuffingByte(*pData)) *pStuffedData++ = *pData;
				*pStuffedData++ = *pData++;
			}

			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CMaxonSerialV2Frame::DestuffingData(BYTE* p_pData, DWORD& p_rulDataLength, DWORD& p_rNumberOfStuffedBytes, BOOL& p_roStuffingBytePending)
{
	BYTE* pBuffer = NULL;
	BYTE* pData = p_pData;
	BYTE* pBufferData = NULL;
	DWORD ulDataLength = 0;
	BOOL oResult = FALSE;

	if(p_pData && (p_rulDataLength > 0))
	{
		//Prepare Buffer
		pBuffer = (BYTE*)malloc(p_rulDataLength);
		memcpy(pBuffer,p_pData,p_rulDataLength);

		//Initialisation
		pBufferData = pBuffer;
		pData = p_pData;
		
		//Destuffing
		oResult = TRUE;
		p_rNumberOfStuffedBytes = 0;
		for(DWORD i=0;i<p_rulDataLength;i++)
		{
			if(p_roStuffingBytePending)
			{
				//Last Byte was stuffing byte
				if(IsStuffingByte(*pBufferData))
				{
					//Stuffing Byte is doubled
					*pData++ = *pBufferData++;
					p_rNumberOfStuffedBytes++;
					ulDataLength++;
					p_roStuffingBytePending = FALSE;
				}
				else
				{
					//Stuffing Byte is not doubled
					oResult = FALSE;
					break;
				}
			}
			else
			{
				//Last Byte was not stuffing byte
				if(IsStuffingByte(*pBufferData))
				{
					//First Stuffing Byte found
					pBufferData++;
					p_roStuffingBytePending = TRUE;
				}
				else
				{
					//Normal Byte found
					*pData++ = *pBufferData++;
					ulDataLength++;
				}
			}
		}

		//Free Buffer
		if(pBuffer) free(pBuffer);

		//Update DataLength
		if(oResult)	p_rulDataLength = ulDataLength;
	}

	return oResult;
}

BOOL CMaxonSerialV2Frame::GetNumberOfStuffingBytes(BYTE* p_pData, DWORD p_ulDataLength, DWORD& p_rNumberOfStuffedBytes)
{
	BYTE* pData = p_pData;
	BOOL oResult = FALSE;

	if(p_pData)
	{
		oResult = TRUE;
		p_rNumberOfStuffedBytes = 0;
		for(DWORD i=0;i<p_ulDataLength;i++)
		{
			if(IsStuffingByte(*pData)) p_rNumberOfStuffedBytes++;
			pData++;
		}
	}

	return oResult;
}

BOOL CMaxonSerialV2Frame::IsStuffingByte(BYTE p_uData)
{
	BOOL oResult = FALSE;

	if(p_uData == GetDleCharacter())
	{
		oResult = TRUE;
	}

	return oResult;
}

