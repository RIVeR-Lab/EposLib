// InfoteamSerialSendingFrame.cpp: Implementierung der Klasse CInfoteamSerialSendingFrame.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "InfoteamSerialSendingFrame.h"

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

CInfoteamSerialSendingFrame::CInfoteamSerialSendingFrame()
{
	m_pFrameBuffer = NULL;
	m_dFrameBufferSize = 0;
}

CInfoteamSerialSendingFrame::~CInfoteamSerialSendingFrame()
{
	ResetFrame();
}

/******************************************************************************/
BOOL CInfoteamSerialSendingFrame::ResetFrame()
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
BOOL CInfoteamSerialSendingFrame::PrepareSendFrame(DWORD dPackageSize,WORD wChunkSize,BYTE uLastChunkFlag,DWORD* pdChecksum,void* pDataBuffer,DWORD dDataBufferLength)
{
	t_EsamFrameHeader* pFrameHeader;
	BYTE* pData = 0;

	//Delete old frame
	ResetFrame();

	if(pDataBuffer)
	{
		//Allocate Memory
		m_dFrameBufferSize = sizeof(t_EsamFrameHeader) + dDataBufferLength;
		m_pFrameBuffer = malloc(m_dFrameBufferSize);

		//Header
		pFrameHeader = (t_EsamFrameHeader*)m_pFrameBuffer;
		pFrameHeader->dPackageSize = dPackageSize;
		pFrameHeader->wChunkSize = wChunkSize;
		pFrameHeader->uLastChunkFlag = uLastChunkFlag;

		//Data
		if(dDataBufferLength > 0)
		{
			pData = (BYTE*)m_pFrameBuffer + sizeof(t_EsamFrameHeader);
			memcpy(pData,pDataBuffer,dDataBufferLength);
		}

		//Checksum
		pFrameHeader->dChecksum = ComputeCRC(pData,dDataBufferLength);
		if(pdChecksum) *pdChecksum = pFrameHeader->dChecksum;

		return TRUE;
	}

	return FALSE;
}

/******************************************************************************/
/*BOOL CInfoteamSerialSendingFrame::PrepareSendFrame(DWORD dPackageSize,WORD wChunkSize,BYTE uLastChunkFlag,DWORD* pdChecksum,void* pDataBuffer,DWORD dDataBufferLength)
{
	t_EsamFrameHeader* pFrameHeader;
	BYTE* pData;

	dPackageSize  = 5;
	wChunkSize = 5;
	uLastChunkFlag = 0xFF;
	dDataBufferLength = 5;
	BYTE data[5];

	data[0] = 1;
	data[1] = 1;
	data[2] = 2;
	data[3] = 1;
	data[4] = 1;

	//Delete old frame
	ResetFrame();

	if(pDataBuffer)
	{
		//Allocate Memory
		m_dFrameBufferSize = sizeof(t_EsamFrameHeader) + dDataBufferLength;
		m_pFrameBuffer = malloc(m_dFrameBufferSize);

		//Header
		pFrameHeader = (t_EsamFrameHeader*)m_pFrameBuffer;
		pFrameHeader->dPackageSize = dPackageSize;
		pFrameHeader->wChunkSize = wChunkSize;
		pFrameHeader->uLastChunkFlag = uLastChunkFlag;

		//Data
		if(dDataBufferLength > 0)
		{
			pData = (BYTE*)m_pFrameBuffer + sizeof(t_EsamFrameHeader);
			memcpy(pData,data,dDataBufferLength);
		}

		//Checksum
		pFrameHeader->dChecksum = ComputeCRC(pData,dDataBufferLength);
		if(pdChecksum) *pdChecksum = pFrameHeader->dChecksum;

		return TRUE;
	}

	return FALSE;
}*/

/******************************************************************************/
DWORD CInfoteamSerialSendingFrame::GetFrameSize()
{
	return m_dFrameBufferSize;
}

/******************************************************************************/
void* CInfoteamSerialSendingFrame::GetFramePointer()
{
	return m_pFrameBuffer;
}

/******************************************************************************/
DWORD CInfoteamSerialSendingFrame::ComputeCRC(void* pDataBuffer,DWORD dSize)
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

