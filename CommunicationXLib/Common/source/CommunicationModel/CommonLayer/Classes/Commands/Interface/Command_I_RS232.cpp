// Command_I_RS232.cpp: Implementierung der Klasse CCommand_I_RS232.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_RS232.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_I_RS232::CCommand_I_RS232()
{
}

CCommand_I_RS232::CCommand_I_RS232(DWORD dCommandId)
{
	InitCommand(dCommandId);
}

CCommand_I_RS232::~CCommand_I_RS232()
{
}

//******************************************************************
CCommandRoot* CCommand_I_RS232::CloneCommand()
{
	CCommand_I_RS232* pNewCommand;

	pNewCommand = new CCommand_I_RS232();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_I_RS232& CCommand_I_RS232::operator=(CCommand_I_RS232& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}

//******************************************************************
BOOL CCommand_I_RS232::InitCommand(DWORD dCommandId)
{
	ResetCommand();
	switch(dCommandId)
	{
		case RS232_WRITE_DATA:
			{
				CCommand_I::InitCommand("RS232 WriteData",RS232_WRITE_DATA);
				AddParameter(0,"data",ODT_MEMORY_BLOCK);
				AddReturnParameter(0,"nbOfBytesWritten",ODT_UINT32,FALSE);
				SetDefaultParameter_WriteData();
				return TRUE;
			};
		case RS232_READ_DATA:
			{
				CCommand_I::InitCommand("RS232 ReadData",RS232_READ_DATA);
				AddParameter(0,"nbOfBytesToRead",ODT_UINT32,FALSE,TRUE);
				AddReturnParameter(0,"data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_ReadData();
				return TRUE;
			};
	}

	return FALSE;
}

//**************************************************************************
void CCommand_I_RS232::SetDefaultParameter_ReadData()
{
	DWORD dNbOfBytesToRead = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&dNbOfBytesToRead,sizeof(dNbOfBytesToRead));

	//ReturnParameter
	SetReturnParameterData(0,pData,0);
}

//**************************************************************************
void CCommand_I_RS232::SetDefaultParameter_WriteData()
{
	void* pData = NULL;
	DWORD dNbOfBytesWritten = 0;

	//Parameter
	SetParameterData(0,pData,0);

	//ReturnParameter
    SetReturnParameterData(0, &dNbOfBytesWritten, sizeof(dNbOfBytesWritten));
}

