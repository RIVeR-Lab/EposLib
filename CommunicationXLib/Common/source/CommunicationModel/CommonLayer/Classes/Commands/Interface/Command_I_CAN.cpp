// Command_I_CAN.cpp: Implementierung der Klasse CCommand_I_CAN.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_CAN.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommand_I_CAN::CCommand_I_CAN()
{
}

CCommand_I_CAN::CCommand_I_CAN(DWORD p_ulCommandId)
{
	InitCommand(p_ulCommandId);
}

CCommand_I_CAN::~CCommand_I_CAN()
{
}

CCommandRoot* CCommand_I_CAN::CloneCommand()
{
	CCommand_I_CAN* pNewCommand;

	pNewCommand = new CCommand_I_CAN();
	*pNewCommand = *this;

	return pNewCommand;
}

CCommand_I_CAN& CCommand_I_CAN::operator=(CCommand_I_CAN& p_rOther)
{
	if(this != &p_rOther)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&p_rOther);
	}

	return *this;
}

BOOL CCommand_I_CAN::InitCommand(DWORD p_ulCommandId)
{
	const DWORD DATA_ARRAY_SIZE = 8;

	ResetCommand();
	switch(p_ulCommandId)
	{
		case CAN_TRANSMIT_CAN_FRAME:
			{
				CCommand_I::InitCommand("Transmit CAN-Frame", CAN_TRANSMIT_CAN_FRAME);
				AddParameter(0, "cobId", ODT_UINT32);
				AddParameter(1, "rtr", ODT_BOOLEAN);
				AddParameter(2, "dlc", ODT_UINT8);
				AddParameter(3, "data", ODT_UINT8, DATA_ARRAY_SIZE);
				SetDefaultParameter_TransmitCanFrame();
				return TRUE;
			};
		case CAN_RECEIVE_CAN_FRAME:
			{
				CCommand_I::InitCommand("Receive CAN-Frame", CAN_RECEIVE_CAN_FRAME);
				AddReturnParameter(0, "cobId", ODT_UINT32);
				AddReturnParameter(1, "rtr", ODT_BOOLEAN);
				AddReturnParameter(2, "dlc", ODT_UINT8);
				AddReturnParameter(3, "data", ODT_UINT8, DATA_ARRAY_SIZE);
				SetDefaultParameter_ReceiveCanFrame();
				return TRUE;
			};
		case CAN_RECEIVE_FILTERED_CAN_FRAME:
			{
				CCommand_I::InitCommand("Receive CAN-Frame", CAN_RECEIVE_FILTERED_CAN_FRAME);
				AddParameter(0, "cobIdFilter", ODT_UINT32, FALSE, TRUE);
				AddParameter(1, "rtrFilter", ODT_BOOLEAN, FALSE, TRUE);
				AddReturnParameter(0, "cobId", ODT_UINT32);
				AddReturnParameter(1, "rtr", ODT_BOOLEAN);
				AddReturnParameter(2, "dlc", ODT_UINT8);
				AddReturnParameter(3, "data", ODT_UINT8, DATA_ARRAY_SIZE);
				SetDefaultParameter_ReceiveFilteredCanFrame();
				return TRUE;
			};

		case CAN_TRANSMIT_CAN_FRAME_EX:
			{
				CCommand_I::InitCommand("Transmit CAN-FrameEx", CAN_TRANSMIT_CAN_FRAME_EX);
				AddParameter(0, "cobId", ODT_UINT32);
				AddParameter(1, "rtr", ODT_BOOLEAN);
				AddParameter(2, "dlc", ODT_UINT8);
				AddParameter(3, "data", ODT_UINT8, DATA_ARRAY_SIZE);
				SetDefaultParameter_TransmitCanFrame();
				return TRUE;
			};
		case CAN_RECEIVE_CAN_FRAME_EX:
			{
				CCommand_I::InitCommand("Receive CAN-FrameEx", CAN_RECEIVE_CAN_FRAME_EX);
				AddReturnParameter(0, "cobId", ODT_UINT32);
				AddReturnParameter(1, "rtr", ODT_BOOLEAN);
				AddReturnParameter(2, "dlc", ODT_UINT8);
				AddReturnParameter(3, "data", ODT_UINT8, DATA_ARRAY_SIZE);
				SetDefaultParameter_ReceiveCanFrame();
				return TRUE;
			};
		case CAN_RECEIVE_FILTERED_CAN_FRAME_EX:
			{
				CCommand_I::InitCommand("Receive CAN-FrameEx", CAN_RECEIVE_FILTERED_CAN_FRAME_EX);
				AddParameter(0, "cobIdFilter", ODT_UINT32, FALSE, TRUE);
				AddParameter(1, "rtrFilter", ODT_BOOLEAN, FALSE, TRUE);
				AddReturnParameter(0, "cobId", ODT_UINT32);
				AddReturnParameter(1, "rtr", ODT_BOOLEAN);
				AddReturnParameter(2, "dlc", ODT_UINT8);
				AddReturnParameter(3, "data", ODT_UINT8, DATA_ARRAY_SIZE);
				SetDefaultParameter_ReceiveFilteredCanFrame();
				return TRUE;
			};
	}

	return FALSE;
}

void CCommand_I_CAN::SetDefaultParameter_TransmitCanFrame()
{
	DWORD dCobId = 0;
	BOOL oRtr = 0;
	BYTE uDlc = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &oRtr, sizeof(oRtr));
	SetParameterData(2, &uDlc, sizeof(uDlc));
	SetParameterData(3, pData, 0);
}

void CCommand_I_CAN::SetDefaultParameter_ReceiveFilteredCanFrame()
{
	DWORD dCobId = 0;
	BOOL oRtr = 0;
	BYTE uDlc = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &oRtr, sizeof(oRtr));

	//ReturnParameter
	SetReturnParameterData(0, &dCobId, sizeof(dCobId));
	SetReturnParameterData(1, &oRtr, sizeof(oRtr));
	SetReturnParameterData(2, &uDlc, sizeof(uDlc));
	SetReturnParameterData(3, pData, 0);
}

void CCommand_I_CAN::SetDefaultParameter_ReceiveCanFrame()
{
	DWORD dCobId = 0;
	BOOL oRtr = 0;
	BYTE uDlc = 0;
	void* pData = NULL;

	//ReturnParameter
	SetReturnParameterData(0, &dCobId, sizeof(dCobId));
	SetReturnParameterData(1, &oRtr, sizeof(oRtr));
	SetReturnParameterData(2, &uDlc, sizeof(uDlc));
	SetReturnParameterData(3, pData, 0);
}
