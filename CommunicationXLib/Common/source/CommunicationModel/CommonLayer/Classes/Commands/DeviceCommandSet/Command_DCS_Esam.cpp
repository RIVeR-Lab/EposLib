// Command_DCS_Esam.cpp: Implementierung der Klasse CCommand_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_DCS_Esam::CCommand_DCS_Esam()
{
}

CCommand_DCS_Esam::CCommand_DCS_Esam(DWORD dCommandId)
{
	InitCommand(dCommandId);
}

CCommand_DCS_Esam::~CCommand_DCS_Esam()
{
}

//******************************************************************
CCommandRoot* CCommand_DCS_Esam::CloneCommand()
{
	CCommand_DCS_Esam* pNewCommand;

	pNewCommand = new CCommand_DCS_Esam();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_DCS_Esam& CCommand_DCS_Esam::operator=(CCommand_DCS_Esam& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}
//******************************************************************
BOOL CCommand_DCS_Esam::InitCommand(DWORD dCommandId)
{
	ResetCommand();
	if(InitCommand_ObjectDictionary(dCommandId)) return TRUE;
	if(InitCommand_NetworkManagement(dCommandId)) return TRUE;
	if(InitCommand_GeneralGateway(dCommandId)) return TRUE;
	if(InitCommand_LayerSettingServices(dCommandId)) return TRUE;
	if(InitCommand_DeviceControl(dCommandId)) return TRUE;

	return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Esam::InitCommand_ObjectDictionary(DWORD dCommandId)
{
	switch(dCommandId)
	{
		case ESAM_WRITE_OBJECT:
			{
				CCommand_DCS::InitCommand("WriteObject",ESAM_WRITE_OBJECT);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"index",ODT_UINT16);
				AddParameter(2,"subIndex",ODT_UINT8);
				AddParameter(3,"nodeId",ODT_UINT8);
				AddParameter(4,"data",ODT_MEMORY_BLOCK);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				SetDefaultParameter_WriteObject();
				return TRUE;
			};
		case ESAM_READ_OBJECT:
			{
				CCommand_DCS::InitCommand("ReadObject",ESAM_READ_OBJECT);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"index",ODT_UINT16);
				AddParameter(2,"subIndex",ODT_UINT8);
				AddParameter(3,"nodeId",ODT_UINT8);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				AddReturnParameter(1,"data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_ReadObject();
				return TRUE;
			};
		case ESAM_INITIATE_SEGMENT_WRITE:
			{
				CCommand_DCS::InitCommand("InitiateSegmentedWrite",ESAM_INITIATE_SEGMENT_WRITE);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"index",ODT_UINT16);
				AddParameter(2,"subIndex",ODT_UINT8);
				AddParameter(3,"nodeId",ODT_UINT8);
				AddParameter(4,"objectLength",ODT_UINT32);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				SetDefaultParameter_InitiateSegmentedWrite();
				return TRUE;
			};
		case ESAM_INITIATE_SEGMENT_READ:
			{
				CCommand_DCS::InitCommand("InitiateSegmentedRead",ESAM_INITIATE_SEGMENT_READ);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"index",ODT_UINT16);
				AddParameter(2,"subIndex",ODT_UINT8);
				AddParameter(3,"nodeId",ODT_UINT8);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				AddReturnParameter(1,"objectLength",ODT_UINT32);
				SetDefaultParameter_InitiateSegmentedRead();
				return TRUE;
			};
		case ESAM_SEGMENT_WRITE:
			{
				CCommand_DCS::InitCommand("SegmentWrite",ESAM_SEGMENT_WRITE);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"controlByte",ODT_UINT8);
				AddParameter(2,"data",ODT_MEMORY_BLOCK);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				SetDefaultParameter_SegmentedWrite();
				return TRUE;
			};
		case ESAM_SEGMENT_READ:
			{
				CCommand_DCS::InitCommand("SegmentRead",ESAM_SEGMENT_READ);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				AddReturnParameter(1,"controlByte",ODT_UINT8);
				AddReturnParameter(2,"data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_SegmentedRead();
				return TRUE;
			};

		case ESAM_ABORT_SEGMENT_TRANSFER:
			{
				CCommand_DCS::InitCommand("AbortSegmentedTransfer",ESAM_ABORT_SEGMENT_TRANSFER);
				AddParameter(0,"nodeId",ODT_UINT16,FALSE,FALSE);
				AddParameter(1,"index",ODT_UINT16);
				AddParameter(2,"subIndex",ODT_UINT8);
				AddParameter(3,"abortCode",ODT_UINT32);
				SetDefaultParameter_AbortSegmentedTransfer();
				return TRUE;
			};
	}

	return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Esam::InitCommand_NetworkManagement(DWORD dCommandId)
{
	switch(dCommandId)
	{
		case ESAM_SEND_NMT_SERVICE:
			{
				CCommand_DCS::InitCommand("SendNMTService",ESAM_SEND_NMT_SERVICE);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"nodeIdentifier",ODT_UINT16);
				AddParameter(2,"commandSpecifier",ODT_UINT16);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				SetDefaultParameter_SendNMTService();
				return TRUE;
			};
	}

	return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Esam::InitCommand_GeneralGateway(DWORD dCommandId)
{
	switch(dCommandId)
	{
		case ESAM_REQUEST_CAN_FRAME:
			{
				CCommand_DCS::InitCommand("RequestCANFrame",ESAM_REQUEST_CAN_FRAME);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"cobId",ODT_UINT16);
				AddParameter(2,"length",ODT_UINT16);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				AddReturnParameter(1,"data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_RequestCANFrame();
				return TRUE;
			};
		case ESAM_SEND_CAN_FRAME:
			{
				CCommand_DCS::InitCommand("SendCANFrame",ESAM_SEND_CAN_FRAME);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"cobId",ODT_UINT16);
				AddParameter(2,"length",ODT_UINT16);
				AddParameter(3,"data",ODT_MEMORY_BLOCK);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				SetDefaultParameter_SendCANFrame();
				return TRUE;
			};
        case ESAM_READ_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("ReadCANFrame", ESAM_READ_CAN_FRAME);
                AddParameter(0, "portNumber", ODT_UINT8);
                AddParameter(1, "cobId", ODT_UINT16);
                AddParameter(2, "length", ODT_UINT16);
                AddParameter(3, "timeout", ODT_UINT32);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadCANFrame();
                return TRUE;
            };
	}

	return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Esam::InitCommand_LayerSettingServices(DWORD dCommandId)
{
	switch(dCommandId)
	{
		case ESAM_SEND_LSS_FRAME:
			{
				CCommand_DCS::InitCommand("SendLSSFrame",ESAM_SEND_LSS_FRAME);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"data",ODT_MEMORY_BLOCK);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				SetDefaultParameter_SendLSSFrame();
				return TRUE;
			};
		case ESAM_READ_LSS_FRAME:
			{
				CCommand_DCS::InitCommand("ReadLSSFrame",ESAM_READ_LSS_FRAME);
				AddParameter(0,"portNumber",ODT_UINT8);
				AddParameter(1,"timeout",ODT_UINT16);
				AddReturnParameter(0,"errorCode",ODT_UINT32);
				AddReturnParameter(1,"data",ODT_MEMORY_BLOCK);
				SetDefaultParameter_ReadLSSFrame();
				return TRUE;
			};
	}

	return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Esam::InitCommand_DeviceControl(DWORD dCommandId)
{
	switch(dCommandId)
	{
		case ESAM_LOGIN:
				{
					CCommand_DCS::InitCommand("Login",ESAM_LOGIN);
					AddParameter(0,"maxReceiveSize",ODT_UINT16);
					AddReturnParameter(0,"errorCode",ODT_UINT8);
					SetDefaultParameter_Login();
					return TRUE;
				};
		case ESAM_LOGOUT:
				{
					CCommand_DCS::InitCommand("Logout",ESAM_LOGOUT);
					AddReturnParameter(0,"errorCode",ODT_UINT8);
					SetDefaultParameter_Logout();
					return TRUE;
				};

		case ESAM_SET_STATE:
				{
					CCommand_DCS::InitCommand("SetState",ESAM_SET_STATE);
					AddParameter(0,"runCommand",ODT_UINT8);
					AddReturnParameter(0,"errorCode",ODT_UINT8);
					SetDefaultParameter_SetState();
					return TRUE;
				};
	}

	return FALSE;
}

//*****************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_InitiateSegmentedRead()
{
	BYTE uPortNumber = 0;
	WORD wIndex = 0;
	BYTE uSubIndex = 0;
	BYTE uNodeId = 1;
	DWORD dErrorCode = 0;
	DWORD dObjectLength = 0;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wIndex,sizeof(wIndex));
	SetParameterData(2,&uSubIndex,sizeof(uSubIndex));
	SetParameterData(3,&uNodeId,sizeof(uNodeId));

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
	SetReturnParameterData(1,&dObjectLength,sizeof(dObjectLength));
}

//*****************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_InitiateSegmentedWrite()
{
	BYTE uPortNumber = 0;
	WORD wIndex = 0;
	BYTE uSubIndex = 0;
	BYTE uNodeId = 1;
	WORD objectLength = 0;
	DWORD dErrorCode = 0;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wIndex,sizeof(wIndex));
	SetParameterData(2,&uSubIndex,sizeof(uSubIndex));
	SetParameterData(3,&uNodeId,sizeof(uNodeId));
	SetParameterData(4,&objectLength,sizeof(objectLength));

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
}

//*****************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_ReadObject()
{
	BYTE uPortNumber = 0;
	WORD wIndex = 0;
	BYTE uSubIndex = 0;
	BYTE uNodeId = 1;

	DWORD dErrorCode = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wIndex,sizeof(wIndex));
	SetParameterData(2,&uSubIndex,sizeof(uSubIndex));
	SetParameterData(3,&uNodeId,sizeof(uNodeId));

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
	SetReturnParameterData(1,pData,0);
}

//*****************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_SegmentedRead()
{
	BYTE uPortNumber = 0;
	DWORD dErrorCode = 0;
	BYTE uControlByte = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
	SetReturnParameterData(1,&uControlByte,sizeof(uControlByte));
	SetReturnParameterData(2,pData,0);
}

//*****************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_SegmentedWrite()
{
	BYTE uPortNumber = 0;
	BYTE uControlByte = 0;
	void* pData = NULL;
	DWORD dErrorCode = 0;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&uControlByte,sizeof(uControlByte));
	SetParameterData(2,pData,0);

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
}

//*****************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_AbortSegmentedTransfer()
{
	WORD wNodeId = 1;
	WORD wIndex = 0;
	BYTE uSubIndex = 0;
	DWORD dAbortCode = 0;

	//Parameter
	SetParameterData(0,&wNodeId,sizeof(wNodeId));
	SetParameterData(1,&wIndex,sizeof(wIndex));
	SetParameterData(2,&uSubIndex,sizeof(uSubIndex));
	SetParameterData(3,&dAbortCode,sizeof(dAbortCode));
}

//*****************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_WriteObject()
{
	BYTE uPortNumber = 0;
	WORD wIndex = 0;
	BYTE uSubIndex = 0;
	BYTE uNodeId = 1;
	void* pData = NULL;
	DWORD dErrorCode = 0;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wIndex,sizeof(wIndex));
	SetParameterData(2,&uSubIndex,sizeof(uSubIndex));
	SetParameterData(3,&uNodeId,sizeof(uNodeId));
	SetParameterData(4,pData,0);

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_SendNMTService()
{
	BYTE uPortNumber = 0;
	WORD wNodeId = 1;
	WORD wCmdSpecifier = 0;
	DWORD dErrorCode = 0;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wNodeId,sizeof(wNodeId));
	SetParameterData(2,&wCmdSpecifier,sizeof(wCmdSpecifier));

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_RequestCANFrame()
{
	BYTE uPortNumber = 0;
	WORD wCobId = 0;
	WORD wLength= 0;
	DWORD dErrorCode = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wCobId,sizeof(wCobId));
	SetParameterData(2,&wLength,sizeof(wLength));

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
	SetReturnParameterData(1,pData,0);
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_SendCANFrame()
{
	BYTE uPortNumber = 0;
	WORD wCobId = 0;
	WORD wLength = 0;
	void* pData = NULL;
	DWORD dErrorCode = 0;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wCobId,sizeof(wCobId));
	SetParameterData(2,&wLength,sizeof(wLength));
	SetParameterData(3,pData,0);

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_ReadCANFrame()
{
    BYTE uPortNumber = 0;
    WORD wCobId = 0;
    WORD wLength = 0;
    DWORD ulTimeout = 0;
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
    SetParameterData(1, &wCobId, sizeof(wCobId));
    SetParameterData(2, &wLength, sizeof(wLength));
    SetParameterData(3, &ulTimeout, sizeof(ulTimeout));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, pData, wLength);
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_SendLSSFrame()
{
	BYTE uPortNumber = 0;
	void* pData = NULL;
	DWORD dErrorCode = 0;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,pData,0);

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_ReadLSSFrame()
{
	BYTE uPortNumber = 0;
	WORD wTimeout = 10;
	DWORD dErrorCode = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0,&uPortNumber,sizeof(uPortNumber));
	SetParameterData(1,&wTimeout,sizeof(wTimeout));

	//ReturnParameter
	SetReturnParameterData(0,&dErrorCode,sizeof(dErrorCode));
	SetReturnParameterData(1,pData,0);
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_Login()
{
	WORD wMaxReceiveSize = 257;
	BYTE uErrorCode = 0;

	//Parameter
	SetParameterData(0,&wMaxReceiveSize,sizeof(wMaxReceiveSize));

	//ReturnParameter
	SetReturnParameterData(0,&uErrorCode,sizeof(uErrorCode));
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_Logout()
{
	BYTE uErrorCode = 0;

	//ReturnParameter
	SetReturnParameterData(0,&uErrorCode,sizeof(uErrorCode));
}

//**************************************************************************
void CCommand_DCS_Esam::SetDefaultParameter_SetState()
{
	BYTE uRunCommand = 0;
	BYTE uErrorCode = 0;

	//Parameter
	SetParameterData(0,&uRunCommand,sizeof(uRunCommand));

	//ReturnParameter
	SetReturnParameterData(0,&uErrorCode,sizeof(uErrorCode));
}

