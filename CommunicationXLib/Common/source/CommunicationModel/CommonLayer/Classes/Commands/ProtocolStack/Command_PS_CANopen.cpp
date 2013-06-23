// Command_PS_CANopen.cpp: Implementierung der Klasse CCommand_PS_CANopen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_CANopen.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_PS_CANopen::CCommand_PS_CANopen()
{
}

CCommand_PS_CANopen::CCommand_PS_CANopen(DWORD p_ulCommandId)
{
	InitCommand(p_ulCommandId);
}

CCommand_PS_CANopen::~CCommand_PS_CANopen()
{
}

CCommandRoot*CCommand_PS_CANopen::CloneCommand()
{
	CCommand_PS_CANopen* pNewCommand;

	pNewCommand = new CCommand_PS_CANopen();
	*pNewCommand = *this;

	return pNewCommand;
}

CCommand_PS_CANopen& CCommand_PS_CANopen::operator=(CCommand_PS_CANopen& p_rOther)
{
	if(this != &p_rOther)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&p_rOther);
	}

	return *this;
}

BOOL CCommand_PS_CANopen::InitCommand(DWORD p_ulCommandId)
{
	const DWORD DATA_SIZE = 4;
	const DWORD SEGMENT_DATA_SIZE = 7;

	ResetCommand();
	switch(p_ulCommandId)
	{
		case CANOPEN_INITIATE_SDO_DOWNLOAD:
			{
				CCommand_PS::InitCommand("Initiate SDO Download", CANOPEN_INITIATE_SDO_DOWNLOAD);
				AddParameter(0, "cobId Client->Server", ODT_UINT32);
				AddParameter(1, "cobId Server->Client", ODT_UINT32, FALSE, TRUE);
				AddParameter(2, "expeditedTransfer", ODT_BOOLEAN);
				AddParameter(3, "sizeIndicated", ODT_BOOLEAN);
				AddParameter(4, "nonValidNbOfBytes", ODT_UINT8);
				AddParameter(5, "index", ODT_UINT16);
				AddParameter(6, "subIndex", ODT_UINT8);
				AddParameter(7, "data", ODT_UINT8, DATA_SIZE);
				AddReturnParameter(0, "cobId Server->Client", ODT_UINT32);
				AddReturnParameter(1, "index", ODT_UINT16, FALSE);
				AddReturnParameter(2, "subIndex", ODT_UINT8, FALSE);
				AddReturnParameter(3, "abortCode", ODT_UINT32);
				SetDefaultParameter_InitiateSDODownload();
				return TRUE;
			};
		case CANOPEN_DOWNLOAD_SDO_SEGMENT:
			{
				CCommand_PS::InitCommand("Download SDO Segment", CANOPEN_DOWNLOAD_SDO_SEGMENT);
				AddParameter(0, "cobId Client->Server", ODT_UINT32);
				AddParameter(1, "cobId Server->Client", ODT_UINT32, FALSE, TRUE);
				AddParameter(2, "toggle", ODT_BOOLEAN);
				AddParameter(3, "nonValidNbOfBytes", ODT_UINT8);
				AddParameter(4, "noMoreSegments", ODT_BOOLEAN);
				AddParameter(5, "segData", ODT_UINT8, SEGMENT_DATA_SIZE);
				AddReturnParameter(0, "cobId Server->Client", ODT_UINT32);
				AddReturnParameter(1, "toggle", ODT_BOOLEAN);
				AddReturnParameter(2, "abortCode", ODT_UINT32);
				SetDefaultParameter_DownloadSDOSegment();
				return TRUE;
			};
		case CANOPEN_INITIATE_SDO_UPLOAD:
			{
				CCommand_PS::InitCommand("Initiate SDO Upload", CANOPEN_INITIATE_SDO_UPLOAD);
				AddParameter(0, "cobId Client->Server", ODT_UINT32);
				AddParameter(1, "cobId Server->Client", ODT_UINT32, FALSE, TRUE);
				AddParameter(2, "index", ODT_UINT16);
				AddParameter(3, "subIndex", ODT_UINT8);
				AddReturnParameter(0, "cobId Server->Client", ODT_UINT32);
				AddReturnParameter(1, "expeditedTransfer", ODT_BOOLEAN);
				AddReturnParameter(2, "sizeIndicated", ODT_BOOLEAN);
				AddReturnParameter(3, "nonValidNbOfBytes", ODT_UINT8);
				AddReturnParameter(4, "index", ODT_UINT16, FALSE);
				AddReturnParameter(5, "subIndex", ODT_UINT8, FALSE);
				AddReturnParameter(6, "data", ODT_UINT8, DATA_SIZE);
				AddReturnParameter(7, "abortCode", ODT_UINT32);
				SetDefaultParameter_InitiateSDOUpload();
				return TRUE;
			};
		case CANOPEN_UPLOAD_SDO_SEGMENT:
			{
				CCommand_PS::InitCommand("Upload SDO Segment", CANOPEN_UPLOAD_SDO_SEGMENT);
				AddParameter(0, "cobId Client->Server", ODT_UINT32);
				AddParameter(1, "cobId Server->Client", ODT_UINT32, FALSE, TRUE);
				AddParameter(2, "toggle", ODT_BOOLEAN);
				AddReturnParameter(0, "cob-Id Server->Client", ODT_UINT32);
				AddReturnParameter(1, "toggle", ODT_BOOLEAN);
				AddReturnParameter(2, "nonValidNbOfBytes", ODT_UINT8);
				AddReturnParameter(3, "noMoreSegments", ODT_BOOLEAN);
				AddReturnParameter(4, "segData", ODT_UINT8, SEGMENT_DATA_SIZE);
				AddReturnParameter(5, "abortCode", ODT_UINT32);
				SetDefaultParameter_UploadSDOSegment();
				return TRUE;
			};
		case CANOPEN_ABORT_SDO_TRANSFER:
			{
				CCommand_PS::InitCommand("Abort SDO Transfer", CANOPEN_ABORT_SDO_TRANSFER);
				AddParameter(0, "cobId Client->Server", ODT_UINT32);
				AddParameter(1, "index", ODT_UINT16);
				AddParameter(2, "subIndex", ODT_UINT8);
				AddParameter(3, "abortCode", ODT_UINT32);
				SetDefaultParameter_AbortSDOTransfer();
				return TRUE;
			};
		case CANOPEN_NETWORK_INDICATION:
			{
				CCommand_PS::InitCommand("Network Indication", CANOPEN_NETWORK_INDICATION);
				AddParameter(0, "cobId Client->Server", ODT_UINT32);
				AddParameter(1, "cobId Server->Client", ODT_UINT32, FALSE, TRUE);
				AddParameter(2, "targetNetworkId", ODT_UINT16);
				AddParameter(3, "targetNodeId", ODT_UINT8);
				AddReturnParameter(0, "cobId Server->Client", ODT_UINT32);
				AddReturnParameter(1, "targetNetworkId", ODT_UINT16);
				AddReturnParameter(2, "targetNodeId", ODT_UINT8);
				AddReturnParameter(3, "abortCode", ODT_UINT32);
				SetDefaultParameter_NetworkIndication();
				return TRUE;
			};
		case CANOPEN_SEND_NMT_SERVICE:
			{
				CCommand_PS::InitCommand("Send NMT Service", CANOPEN_SEND_NMT_SERVICE);
				AddParameter(0, "commandSpecifier", ODT_UINT8);
				AddParameter(1, "nodeId", ODT_UINT8);
				SetDefaultParameter_SendNMTService();
				return TRUE;
			};
		case CANOPEN_SEND_CAN_FRAME:
			{
				CCommand_PS::InitCommand("Send CAN Frame", CANOPEN_SEND_CAN_FRAME);
				AddParameter(0, "cobId", ODT_UINT32);
				AddParameter(1, "length", ODT_UINT8);
				AddParameter(2, "data", ODT_MEMORY_BLOCK);
				SetDefaultParameter_SendCANFrame();
				return TRUE;
			};
        case CANOPEN_READ_CAN_FRAME:
            {
                CCommand_PS::InitCommand("Read CAN Frame", CANOPEN_READ_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT32);
                AddParameter(1, "length", ODT_UINT8);
                AddParameter(2, "timeout", ODT_UINT32);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadCANFrame();
                return TRUE;
            };
		case CANOPEN_REQUEST_CAN_FRAME:
			{
				CCommand_PS::InitCommand("Request CAN Frame", CANOPEN_REQUEST_CAN_FRAME);
				AddParameter(0, "cobId", ODT_UINT32);
				AddParameter(1, "length", ODT_UINT8);
				AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
				SetDefaultParameter_RequestCANFrame();
				return TRUE;
			};
		case CANOPEN_SEND_LSS_FRAME:
			{
				CCommand_PS::InitCommand("Send LSS Frame", CANOPEN_SEND_LSS_FRAME);
				AddParameter(0, "data", ODT_MEMORY_BLOCK);
				SetDefaultParameter_SendLSSFrame();
				return TRUE;
			};
		case CANOPEN_READ_LSS_FRAME:
			{
				CCommand_PS::InitCommand("Read LSS Frame", CANOPEN_READ_LSS_FRAME);
				AddParameter(0, "timeout", ODT_UINT16);
				AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
				SetDefaultParameter_ReadLSSFrame();
				return TRUE;
			};
	}

	return FALSE;
}

void CCommand_PS_CANopen::SetDefaultParameter_InitiateSDODownload()
{
	DWORD dCobId = 0;
	BOOL oExpeditedTransfer = TRUE;
	BOOL oSizeIndicated = FALSE;
	BYTE uNonValidNbOfBytes = 0;
	WORD usIndex = 0;
	BYTE uSubIndex = 0;
	void* pData = NULL;
	DWORD dAbortCode = 0;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &dCobId, sizeof(dCobId));
	SetParameterData(2, &oExpeditedTransfer, sizeof(oExpeditedTransfer));
	SetParameterData(3, &oSizeIndicated, sizeof(oSizeIndicated));
	SetParameterData(4, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
	SetParameterData(5, &usIndex, sizeof(usIndex));
	SetParameterData(6, &uSubIndex, sizeof(uSubIndex));
	SetParameterData(7, pData, 0);

	//ReturnParameter
	SetReturnParameterData(0, &dCobId, sizeof(dCobId));
	SetReturnParameterData(1, &usIndex, sizeof(usIndex));
	SetReturnParameterData(2, &uSubIndex, sizeof(uSubIndex));
	SetReturnParameterData(3, &dAbortCode, sizeof(dAbortCode));
}

void CCommand_PS_CANopen::SetDefaultParameter_DownloadSDOSegment()
{
	DWORD dCobId = 0;
	BOOL oToggle = TRUE;
	BYTE uNonValidNbOfBytes = 0;
	BOOL oNoMoreSegments = FALSE;
	void* pSegData = NULL;
	DWORD dAbortCode = 0;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &dCobId, sizeof(dCobId));
	SetParameterData(2, &oToggle, sizeof(oToggle));
	SetParameterData(3, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
	SetParameterData(4, &oNoMoreSegments, sizeof(oNoMoreSegments));
	SetParameterData(5, pSegData, 0);

	//ReturnParameter
	SetReturnParameterData(0, &dCobId, sizeof(dCobId));
	SetReturnParameterData(1, &oToggle, sizeof(oToggle));
	SetReturnParameterData(2, &dAbortCode, sizeof(dAbortCode));
}

void CCommand_PS_CANopen::SetDefaultParameter_InitiateSDOUpload()
{
	DWORD dCobId = 0;
	BOOL oExpeditedTransfer = TRUE;
	BOOL oSizeIndicated = FALSE;
	BYTE uNonValidNbOfBytes = 0;
	WORD usIndex = 0;
	BYTE uSubIndex = 0;
	void* pData = NULL;
	DWORD dAbortCode = 0;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &dCobId, sizeof(dCobId));
	SetParameterData(2, &usIndex, sizeof(usIndex));
	SetParameterData(3, &uSubIndex, sizeof(uSubIndex));

	//ReturnParameter
	SetReturnParameterData(0, &dCobId, sizeof(dCobId));
	SetReturnParameterData(1, &oExpeditedTransfer, sizeof(oExpeditedTransfer));
	SetReturnParameterData(2, &oSizeIndicated, sizeof(oSizeIndicated));
	SetReturnParameterData(3, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
	SetReturnParameterData(4, &usIndex, sizeof(usIndex));
	SetReturnParameterData(5, &uSubIndex, sizeof(uSubIndex));
	SetReturnParameterData(6, pData, 0);
	SetReturnParameterData(7, &dAbortCode, sizeof(dAbortCode));
}

void CCommand_PS_CANopen::SetDefaultParameter_UploadSDOSegment()
{
	DWORD dCobId = 0;
	BOOL oToggle = TRUE;
	BYTE uNonValidNbOfBytes = 0;
	BOOL oNoMoreSegments = FALSE;
	void* pSegData = NULL;
	DWORD dAbortCode = 0;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &dCobId, sizeof(dCobId));
	SetParameterData(2, &oToggle, sizeof(oToggle));

	//ReturnParameter
	SetReturnParameterData(0, &dCobId, sizeof(dCobId));
	SetReturnParameterData(1, &oToggle, sizeof(oToggle));
	SetReturnParameterData(2, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
	SetReturnParameterData(3, &oNoMoreSegments, sizeof(oNoMoreSegments));
	SetReturnParameterData(4, pSegData, 0);
	SetReturnParameterData(5, &dAbortCode, sizeof(dAbortCode));
}

void CCommand_PS_CANopen::SetDefaultParameter_AbortSDOTransfer()
{
	DWORD dCobId = 0;
	WORD usIndex = 0;
	BYTE uSubIndex = 0;
	DWORD dAbortCode = 0;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &usIndex, sizeof(usIndex));
	SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
	SetParameterData(3, &dAbortCode, sizeof(dAbortCode));
}

void CCommand_PS_CANopen::SetDefaultParameter_NetworkIndication()
{
	DWORD dCobId = 0;
	WORD wTargetNetworkId = 0;
	BYTE uTargetNodeId = 0;
	DWORD dAbortCode = 0;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &dCobId, sizeof(dCobId));
	SetParameterData(2, &wTargetNetworkId, sizeof(wTargetNetworkId));
	SetParameterData(3, &uTargetNodeId, sizeof(uTargetNodeId));

	//ReturnParameter
	SetReturnParameterData(0, &dCobId, sizeof(dCobId));
	SetReturnParameterData(1, &wTargetNetworkId, sizeof(wTargetNetworkId));
	SetReturnParameterData(2, &uTargetNodeId, sizeof(uTargetNodeId));
	SetReturnParameterData(3, &dAbortCode, sizeof(dAbortCode));
}

void CCommand_PS_CANopen::SetDefaultParameter_SendNMTService()
{
	BYTE uCommandSpecifier = 0;
	BYTE uNodeId = 0;

	//Parameter
	SetParameterData(0, &uCommandSpecifier, sizeof(uCommandSpecifier));
	SetParameterData(1, &uNodeId, sizeof(uNodeId));
}

void CCommand_PS_CANopen::SetDefaultParameter_SendCANFrame()
{
	DWORD dCobId = 0;
	BYTE uLength = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &uLength, sizeof(uLength));
    SetParameterData(2, pData, uLength);
}

void CCommand_PS_CANopen::SetDefaultParameter_ReadCANFrame()
{
    DWORD dCobId = 0;
    BYTE uLength = 0;
    void* pData = NULL;
    DWORD ulTimeout = 0;

    //Parameter
    SetParameterData(0, &dCobId, sizeof(dCobId));
    SetParameterData(1, &uLength, sizeof(uLength));
    SetParameterData(2, &ulTimeout, sizeof(ulTimeout));

    //ReturnParameter
    SetReturnParameterData(0, pData, uLength);
}

void CCommand_PS_CANopen::SetDefaultParameter_RequestCANFrame()
{
	DWORD dCobId = 0;
	BYTE uLength = 0;
	void* pData = NULL;

	//Parameter
	SetParameterData(0, &dCobId, sizeof(dCobId));
	SetParameterData(1, &uLength, sizeof(uLength));

	//ReturnParameter
    SetReturnParameterData(0, pData, uLength);
}

void CCommand_PS_CANopen::SetDefaultParameter_SendLSSFrame()
{
	void* pData=NULL;

	//Parameter
	SetParameterData(0, pData, 0);
}

void CCommand_PS_CANopen::SetDefaultParameter_ReadLSSFrame()
{
	WORD wTimeout = 100;

	//Parameter
	SetParameterData(0, &wTimeout, sizeof(wTimeout));
}


