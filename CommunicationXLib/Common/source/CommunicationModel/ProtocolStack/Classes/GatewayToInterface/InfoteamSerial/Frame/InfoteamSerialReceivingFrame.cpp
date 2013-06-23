// InfoteamSerialReceivingFrame.cpp: Implementierung der Klasse CInfoteamSerialReceivingFrame.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "InfoteamSerialReceivingFrame.h"

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

CInfoteamSerialReceivingFrame::CInfoteamSerialReceivingFrame()
{
	m_pFrameBuffer = NULL;
	m_dFrameBufferSize = 0;
}

CInfoteamSerialReceivingFrame::~CInfoteamSerialReceivingFrame()
{
	ResetFrame();
}

/******************************************************************************/
BOOL CInfoteamSerialReceivingFrame::ResetFrame()
{
	if((m_dFrameBufferSize > 0) && (m_pFrameBuffer))
	{
		free(m_pFrameBuffer);
		m_dFrameBufferSize = 0;
		m_pFrameBuffer = NULL;
	}
	return TRUE;
}

/******************************************************************************/
BOOL CInfoteamSerialReceivingFrame::PrepareReceiveFrame(DWORD dPackageSize, DWORD p_ulMaxPackageSize)
{
	BOOL oResult(FALSE);
	t_EsamFrameHeader* pFrameHeader(0);

	//Delete old frame
	ResetFrame();

	if(p_ulMaxPackageSize >= dPackageSize)
	{
		//Allocate Memory
		m_dFrameBufferSize = sizeof(t_EsamFrameHeader) + dPackageSize;
		m_pFrameBuffer = malloc(m_dFrameBufferSize);
		memset(m_pFrameBuffer,0,m_dFrameBufferSize);

		//Header
		pFrameHeader = (t_EsamFrameHeader*)m_pFrameBuffer;
		pFrameHeader->dPackageSize = dPackageSize;
		oResult = TRUE;
	}
	else
	{
		//Package Size Check failed
		oResult = FALSE;
	}

	return oResult;
}

/******************************************************************************/
BOOL CInfoteamSerialReceivingFrame::CopyReceivedData(DWORD* pdPackageSize,DWORD* pdChecksum,void** ppDataBuffer,DWORD* pdDataBufferLength)
{
	t_EsamFrameHeader* pFrameHeader;
	BYTE* pDataSource;

	if(m_pFrameBuffer)
	{
		pFrameHeader = (t_EsamFrameHeader*)m_pFrameBuffer;

		//PackageSize
		if(pdPackageSize)
		{
			*pdPackageSize = pFrameHeader->dPackageSize;
		}

		//Checksum
		if(pdChecksum)
		{
			*pdChecksum = pFrameHeader->dChecksum;
		}

		//Data
		if(ppDataBuffer && pdDataBufferLength)
		{
			//Free Buffer
			if(*pdDataBufferLength > 0)
			{
				free(*ppDataBuffer);
				*pdDataBufferLength = 0;
				*ppDataBuffer = NULL;
			}

			//Copy Data
			if(m_dFrameBufferSize > sizeof(t_EsamFrameHeader))
			{
				*pdDataBufferLength = m_dFrameBufferSize-sizeof(t_EsamFrameHeader);
				*ppDataBuffer = malloc(*pdDataBufferLength);
				pDataSource = (BYTE*)m_pFrameBuffer + sizeof(t_EsamFrameHeader);
				memcpy(*ppDataBuffer,pDataSource,*pdDataBufferLength);
			}
		}

		return TRUE;
	}

	return FALSE;
}

/******************************************************************************/
DWORD CInfoteamSerialReceivingFrame::ComputeCRC(void* pDataBuffer,DWORD dSize)
{
	if((0 < dSize) && (123*1024*1024 > dSize))
	{
		// Precompute CRC lookup table for 16-bit CRC.
		static DWORD vnCRC_Lookup[0x100];		// CRC lookup table for quick checksum computation.
		static BOOL	 bCRC_Lookup_Init = FALSE;	// Is CRC lookup initialized?
		if (!bCRC_Lookup_Init)
		{	 bCRC_Lookup_Init = TRUE;
			for (int indx=0; indx<0x100; indx++)
			{
				// In slow implementations (without lookup table), the CRC is
				// computed bit by bit. Hence, 8 iterations are necessary per
				// byte read. During each iteration, we do:
				//	1.	nCRC=nCRC*2 + bBitShiftedIn;
				//	2.	if (nCRC & 0x10000) nCRC ^= SSM_CRC_POLYNOMDEFAULT;
				//`Because of the constant "0x10000" (bit 16), the sequence of
				// XOR�s is independant of the initial low-order-byte. It is
				// dependent only on the high-order 8 bit in nCRC before the
				// loop. We simulate this: for all 0x100=256 possibilities for
				// the high-byte in the word, we create a table:
				DWORD nCRC = (indx<<8);
				// Compute operation of shifting 8 bits through....
				for (int iBit=0; iBit<8; iBit++)
				{
					nCRC *= 2;
					if ( nCRC & 0x10000 )	// Check bit 16 of (nCRC<<1).
						nCRC ^= 0x1021;	// 0x1021, polynomial.
														// 0x1021 is short for 0x11021.
														// 0x11021 is the CCITT-polynomial x^16+x^12+x^5+x^0.
				}

				nCRC &= 0xFFFF;							// Restrict to WORD (16 bit-CRC).
				vnCRC_Lookup[indx] = nCRC;
		}	}

		// Computation of the CRC using the table:
		DWORD nCRC = 0;
		BYTE* puDataBuffer = (BYTE*)pDataBuffer;
		for(nCRC = 0;dSize-- > 0;++puDataBuffer)
		{	//		vvvvvvv-------nCRC�s low-byte is shifted 8 bits into high-byte.
			nCRC = (nCRC<<8) ^ vnCRC_Lookup[	0xFF
											 &	((nCRC>>8) ^ *puDataBuffer)
										   ];//	  ^^^^^^^----nCRC�s highbyte determines behaviour.
		}
		nCRC &= 0xFFFF;					// 16-bit CRC.
		return nCRC;
	}

	return 0;
}

/******************************************************************************/
BOOL CInfoteamSerialReceivingFrame::CheckCRC()
{
	DWORD dwComputedChecksum;
	t_EsamFrameHeader* pFrameHeader;
	BYTE* pData;
	DWORD dDataSize;

	if(m_pFrameBuffer)
	{
		pFrameHeader = (t_EsamFrameHeader*)m_pFrameBuffer;
		pData = (BYTE*)m_pFrameBuffer + sizeof(t_EsamFrameHeader);
		dDataSize = m_dFrameBufferSize - sizeof(t_EsamFrameHeader);
		dwComputedChecksum = ComputeCRC(pData,dDataSize);
		return (dwComputedChecksum == pFrameHeader->dChecksum);
	}

	return FALSE;
}

void* CInfoteamSerialReceivingFrame::GetFramePointer()
{
	return m_pFrameBuffer;
}

void* CInfoteamSerialReceivingFrame::GetPackageSizePointer()
{
	t_EsamFrameHeader* pFrameHeader;

	if(m_pFrameBuffer)
	{
		pFrameHeader = (t_EsamFrameHeader*)m_pFrameBuffer;
		return &pFrameHeader->dPackageSize;
	}

	return NULL;
}

void* CInfoteamSerialReceivingFrame::GetChecksumPointer()
{
	t_EsamFrameHeader* pFrameHeader;

	if(m_pFrameBuffer)
	{
		pFrameHeader = (t_EsamFrameHeader*)m_pFrameBuffer;
		return &pFrameHeader->dChecksum;
	}

	return NULL;
}

void* CInfoteamSerialReceivingFrame::GetDataPointer()
{
	if(m_pFrameBuffer)
	{
		return ((BYTE*)m_pFrameBuffer + sizeof(t_EsamFrameHeader));
	}

	return NULL;
}

DWORD CInfoteamSerialReceivingFrame::GetFrameSize()
{
	return m_dFrameBufferSize;
}

DWORD CInfoteamSerialReceivingFrame::GetPackageSizeSize()
{
	return sizeof(DWORD);
}

DWORD CInfoteamSerialReceivingFrame::GetChecksumSize()
{
	return sizeof(DWORD);
}

DWORD CInfoteamSerialReceivingFrame::GetDataSize()
{
	return (m_dFrameBufferSize - sizeof(t_EsamFrameHeader));
}

