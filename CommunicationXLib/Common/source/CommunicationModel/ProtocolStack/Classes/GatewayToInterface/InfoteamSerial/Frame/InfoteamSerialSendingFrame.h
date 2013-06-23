// InfoteamSerialSendingFrame.h: Schnittstelle f�r die Klasse CInfoteamSerialSendingFrame.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_InfoteamSerialSendingFrame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
#define AFX_InfoteamSerialSendingFrame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <MmcTypeDefinition.h>

class CInfoteamSerialSendingFrame
{
public:
	CInfoteamSerialSendingFrame();
	virtual ~CInfoteamSerialSendingFrame();

	BOOL ResetFrame();
	BOOL PrepareSendFrame(DWORD dPackageSize,WORD wChunkSize,BYTE uLastChunkFlag,DWORD* pdChecksum,void* pDataBuffer,DWORD dDataBufferLength);

	void* GetFramePointer();
	DWORD GetFrameSize();

private:
	DWORD ComputeCRC(void* pDataBuffer,DWORD dSize);

private:
	#pragma pack(push,1)
	typedef struct t_EsamFrameHeader
	{
		DWORD		dPackageSize;
		WORD		wChunkSize;
		BYTE		uLastChunkFlag;
		DWORD		dChecksum;
	};
	#pragma pack(pop)

	void* m_pFrameBuffer;
	DWORD m_dFrameBufferSize;
};


#endif // !defined(AFX_InfoteamSerialSendingFrame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
