// InfoteamSerialReceivingFrame.h: Schnittstelle f�r die Klasse CInfoteamSerialReceivingFrame.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_InfoteamSerialReceivingFrame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
#define AFX_InfoteamSerialReceivingFrame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <MmcTypeDefinition.h>

class CInfoteamSerialReceivingFrame
{
public:
	CInfoteamSerialReceivingFrame();
	virtual ~CInfoteamSerialReceivingFrame();

	BOOL ResetFrame();
	BOOL PrepareReceiveFrame(DWORD dPackageSize, DWORD p_ulMaxPackageSize);
	BOOL CopyReceivedData(DWORD* pdPackageSize,DWORD* pdChecksum,void** ppDataBuffer,DWORD* pdDataBufferLength);

	BOOL CheckCRC();

	void* GetFramePointer();
	void* GetPackageSizePointer();
	void* GetChecksumPointer();
	void* GetDataPointer();

	DWORD GetFrameSize();
	DWORD GetPackageSizeSize();
	DWORD GetChecksumSize();
	DWORD GetDataSize();

private:
	DWORD ComputeCRC(void* pDataBuffer,DWORD dSize);

private:
	#pragma pack(push,1)
	typedef struct t_EsamFrameHeader
	{
		DWORD		dPackageSize;
		DWORD		dChecksum;
	};
	#pragma pack(pop)


	void* m_pFrameBuffer;
	DWORD m_dFrameBufferSize;
};


#endif // !defined(AFX_InfoteamSerialReceivingFrame_H__929BF06A_36AC_4794_A55E_A4517961309D__INCLUDED_)
