// Command_PS_InfoteamSerial.cpp: Implementierung der Klasse CCommand_PS_InfoteamSerial.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Command_PS_InfoteamSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_PS_InfoteamSerial::CCommand_PS_InfoteamSerial()
{
}

CCommand_PS_InfoteamSerial::CCommand_PS_InfoteamSerial(DWORD dCommandId)
{
	InitCommand(dCommandId);
}

CCommand_PS_InfoteamSerial::~CCommand_PS_InfoteamSerial()
{
}

//******************************************************************
CCommandRoot*CCommand_PS_InfoteamSerial::CloneCommand()
{
	CCommand_PS_InfoteamSerial* pNewCommand;

	pNewCommand = new CCommand_PS_InfoteamSerial();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_PS_InfoteamSerial& CCommand_PS_InfoteamSerial::operator=(CCommand_PS_InfoteamSerial& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}

//******************************************************************
BOOL CCommand_PS_InfoteamSerial::InitCommand(DWORD dCommandId)
{
	ResetCommand();
	switch(dCommandId)
	{
		case INFOTEAM_SERIAL_PROCESS_PROTOCOL:
			{
				CCommand_PS::InitCommand("Infoteam Serial Protocol",INFOTEAM_SERIAL_PROCESS_PROTOCOL);
				AddParameter(0,"PackageSize",ODT_UINT32);
				AddParameter(1,"ChunkSize",ODT_UINT16);
				AddParameter(2,"LastChunkFlag",ODT_UINT8);
				AddParameter(3,"Checksum",ODT_UINT32);
				AddParameter(4,"Data",ODT_MEMORY_BLOCK);
				AddReturnParameter(0,"PackageSize",ODT_UINT32);
				AddReturnParameter(1,"Checksum",ODT_UINT32);
				AddReturnParameter(2,"Data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_ProcessProtocol();
				return TRUE;
			};

		case INFOTEAM_SERIAL_PROCESS_PROTOCOL_MAXON:
			{
				CCommand_PS::InitCommand("Infoteam Serial Protocol",INFOTEAM_SERIAL_PROCESS_PROTOCOL_MAXON);
				AddParameter(0,"PackageSize",ODT_UINT32);
				AddParameter(1,"ChunkSize",ODT_UINT16);
				AddParameter(2,"LastChunkFlag",ODT_UINT8);
				AddParameter(3,"Checksum",ODT_UINT32);
				AddParameter(4,"PackageType",ODT_UINT8);
				AddParameter(5,"OpCode",ODT_UINT8);
				AddParameter(6,"Data",ODT_MEMORY_BLOCK);
				AddParameter(7,"KeepLock",ODT_UINT8, FALSE, FALSE);
				AddReturnParameter(0,"PackageSize",ODT_UINT32);
				AddReturnParameter(1,"Checksum",ODT_UINT32);
				AddReturnParameter(2,"PackageType",ODT_UINT8);
				AddReturnParameter(3,"Data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_ProcessProtocolMaxon();
				return TRUE;
			};

		case INFOTEAM_SERIAL_ABORT_PROTOCOL_MAXON:
			{
				CCommand_PS::InitCommand("Infoteam Serial Abort Protocol",INFOTEAM_SERIAL_ABORT_PROTOCOL_MAXON);
				SetDefaultParameter_AbortProtocolMaxon();
				return TRUE;
			};

		case INFOTEAM_SERIAL_SEND_CHUNK:
			{
				CCommand_PS::InitCommand("Infoteam Serial Protocol SendChunk",INFOTEAM_SERIAL_SEND_CHUNK);
				AddParameter(0,"PackageSize",ODT_UINT32);
				AddParameter(1,"ChunkSize",ODT_UINT16);
				AddParameter(2,"LastChunkFlag",ODT_UINT8);
				AddParameter(3,"Checksum",ODT_UINT32);
				AddParameter(4,"Data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_SendChunk();
				return TRUE;
			};

		case INFOTEAM_SERIAL_RECEIVE_DATA:
			{
				CCommand_PS::InitCommand("Infoteam Serial Protocol ReceiveData",INFOTEAM_SERIAL_RECEIVE_DATA);
				AddReturnParameter(0,"PackageSize",ODT_UINT32);
				AddReturnParameter(1,"Checksum",ODT_UINT32);
				AddReturnParameter(2,"Data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_ReceiveData();
				return TRUE;
			};
	}

	return FALSE;
}

//**************************************************************************
void CCommand_PS_InfoteamSerial::SetDefaultParameter_ProcessProtocol()
{
	DWORD dPackageSize = 0;
	WORD wChunkSize = 0;
	BYTE uLastChunkFlag = 0;
	DWORD dChecksum = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&dPackageSize,sizeof(dPackageSize));
	SetParameterData(1,&wChunkSize,sizeof(wChunkSize));
	SetParameterData(2,&uLastChunkFlag,sizeof(uLastChunkFlag));
	SetParameterData(3,&dChecksum,sizeof(dChecksum));
	SetParameterData(4,pData,0);

	//Return Parameter
	SetReturnParameterData(0,&dPackageSize,sizeof(dPackageSize));
	SetReturnParameterData(1,&dChecksum,sizeof(dChecksum));
	SetReturnParameterData(2,pData,0);
}

//**************************************************************************
void CCommand_PS_InfoteamSerial::SetDefaultParameter_ProcessProtocolMaxon()
{
	DWORD dPackageSize = 0;
	WORD wChunkSize = 0;
	BYTE uLastChunkFlag = 0;
	DWORD dChecksum = 0;
	BYTE uPackageType = 0;
	BYTE uOpCode = 0;
	void* pData = NULL;
	BYTE ubKeepLock = 0;

	//Parameter
	SetParameterData(0,&dPackageSize,sizeof(dPackageSize));
	SetParameterData(1,&wChunkSize,sizeof(wChunkSize));
	SetParameterData(2,&uLastChunkFlag,sizeof(uLastChunkFlag));
	SetParameterData(3,&dChecksum,sizeof(dChecksum));
	SetParameterData(4,&uPackageType,sizeof(uPackageType));
	SetParameterData(5,&uOpCode,sizeof(uOpCode));
	SetParameterData(6,pData,0);
	SetParameterData(7,&ubKeepLock,sizeof(ubKeepLock));

	//Return Parameter
	SetReturnParameterData(0,&dPackageSize,sizeof(dPackageSize));
	SetReturnParameterData(1,&dChecksum,sizeof(dChecksum));
	SetReturnParameterData(2,&uPackageType,sizeof(uPackageType));
	SetReturnParameterData(3,pData,0);
}

void CCommand_PS_InfoteamSerial::SetDefaultParameter_AbortProtocolMaxon()
{
	//No Parameter
	
	//No Return Parameter
}

//**************************************************************************
void CCommand_PS_InfoteamSerial::SetDefaultParameter_SendChunk()
{
	DWORD dPackageSize = 0;
	WORD wChunkSize = 0;
	BYTE uLastChunkFlag = 0;
	DWORD dChecksum = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&dPackageSize,sizeof(dPackageSize));
	SetParameterData(1,&wChunkSize,sizeof(wChunkSize));
	SetParameterData(2,&uLastChunkFlag,sizeof(uLastChunkFlag));
	SetParameterData(3,&dChecksum,sizeof(dChecksum));
	SetParameterData(4,pData,0);
}

//**************************************************************************
void CCommand_PS_InfoteamSerial::SetDefaultParameter_ReceiveData()
{
	DWORD dPackageSize = 0;
	DWORD dChecksum = 0;
	void* pData = NULL;

	//Return Parameter
	SetReturnParameterData(0,&dPackageSize,sizeof(dPackageSize));
	SetReturnParameterData(1,&dChecksum,sizeof(dChecksum));
	SetReturnParameterData(2,pData,0);
}

