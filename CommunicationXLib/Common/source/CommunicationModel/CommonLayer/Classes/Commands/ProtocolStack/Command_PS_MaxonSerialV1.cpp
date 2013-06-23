// Command_PS_MaxonSerialV1.cpp: Implementierung der Klasse CCommand_PS_MaxonSerialV1.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Command_PS_MaxonSerialV1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_PS_MaxonSerialV1::CCommand_PS_MaxonSerialV1()
{
}

CCommand_PS_MaxonSerialV1::CCommand_PS_MaxonSerialV1(DWORD dCommandId)
{
	InitCommand(dCommandId);
}

CCommand_PS_MaxonSerialV1::~CCommand_PS_MaxonSerialV1()
{
}

//******************************************************************
CCommandRoot*CCommand_PS_MaxonSerialV1::CloneCommand()
{
	CCommand_PS_MaxonSerialV1* pNewCommand;

	pNewCommand = new CCommand_PS_MaxonSerialV1();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_PS_MaxonSerialV1& CCommand_PS_MaxonSerialV1::operator=(CCommand_PS_MaxonSerialV1& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}

//******************************************************************
BOOL CCommand_PS_MaxonSerialV1::InitCommand(DWORD dCommandId)
{
	ResetCommand();
	switch(dCommandId)
	{
		case MAXON_SERIAL_V1_PROCESS_PROTOCOL:
			{
				CCommand_PS::InitCommand("Maxon Serial V1 Protocol",MAXON_SERIAL_V1_PROCESS_PROTOCOL);
				AddParameter(0,"OpCode",ODT_UINT8);
				AddParameter(1,"Len-1",ODT_UINT8);
				AddParameter(2,"Data",ODT_MEMORY_BLOCK);
				AddParameter(3,"Crc",ODT_UINT16,TRUE,FALSE);
				AddParameter(4,"KeepLock",ODT_UINT8,FALSE,FALSE);
				AddReturnParameter(0,"OpCode",ODT_UINT8);
				AddReturnParameter(1,"Len-1",ODT_UINT8);
				AddReturnParameter(2,"Data",ODT_MEMORY_BLOCK);
				AddReturnParameter(3,"Crc",ODT_UINT16);
				SetDefaultParameter_ProcessProtocol();
				return TRUE;
			};
		case MAXON_SERIAL_V1_SEND_FRAME:
			{
				CCommand_PS::InitCommand("Maxon Serial V1 SendFrame",MAXON_SERIAL_V1_SEND_FRAME);
				AddParameter(0,"OpCode",ODT_UINT8);
				AddParameter(1,"Len-1",ODT_UINT8);
				AddParameter(2,"Data",ODT_MEMORY_BLOCK);
				AddParameter(3,"Crc",ODT_UINT16,TRUE,FALSE);
				SetDefaultParameter_SendFrame();
				return TRUE;
			};
		case MAXON_SERIAL_V1_ABORT_PROTOCOL:
			{
				CCommand_PS::InitCommand("Maxon Serial V1 Abort Protocol",MAXON_SERIAL_V1_ABORT_PROTOCOL);
				SetDefaultParameter_AbortProtocol();
				return TRUE;
			};
	}

	return FALSE;
}

void CCommand_PS_MaxonSerialV1::SetDefaultParameter_ProcessProtocol()
{
	BYTE uOpCode = 0;
	BYTE uLen = 0;
	void* pData = NULL;
	WORD wCrc = 0;
	BYTE ubKeepLock = 0;

	//Parameter
	SetParameterData(0,&uOpCode,sizeof(uOpCode));
	SetParameterData(1,&uLen,sizeof(uLen));
	SetParameterData(2,pData,0);
	SetParameterData(3,&wCrc,sizeof(wCrc));
	SetParameterData(4,&ubKeepLock,sizeof(ubKeepLock));

	//Return Parameter
	SetReturnParameterData(0,&uOpCode,sizeof(uOpCode));
	SetReturnParameterData(1,&uLen,sizeof(uLen));
	SetReturnParameterData(2,pData,0);
	SetReturnParameterData(3,&wCrc,sizeof(wCrc));
}

void CCommand_PS_MaxonSerialV1::SetDefaultParameter_SendFrame()
{
	BYTE uOpCode = 0;
	BYTE uLen = 0;
	void* pData = NULL;
	WORD wCrc = 0;

	//Parameter
	SetParameterData(0,&uOpCode,sizeof(uOpCode));
	SetParameterData(1,&uLen,sizeof(uLen));
	SetParameterData(2,pData,0);
	SetParameterData(3,&wCrc,sizeof(wCrc));
}

void CCommand_PS_MaxonSerialV1::SetDefaultParameter_AbortProtocol()
{
	//No Parameter

	//No Return Parameter
}

