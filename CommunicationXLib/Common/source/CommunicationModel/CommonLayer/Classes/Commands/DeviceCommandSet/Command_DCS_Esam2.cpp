// Command_DCS_Esam2.cpp: Implementierung der Klasse CCommand_DCS_Esam2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommand_DCS_Esam2::CCommand_DCS_Esam2()
{
}

CCommand_DCS_Esam2::CCommand_DCS_Esam2(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_DCS_Esam2::~CCommand_DCS_Esam2()
{
}

CCommandRoot* CCommand_DCS_Esam2::CloneCommand()
{
    CCommand_DCS_Esam2* pNewCommand;

    pNewCommand = new CCommand_DCS_Esam2();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_DCS_Esam2& CCommand_DCS_Esam2::operator=(CCommand_DCS_Esam2& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}
BOOL CCommand_DCS_Esam2::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();
    if(InitCommand_ObjectDictionary(p_ulCommandId)) return TRUE;
    if(InitCommand_NetworkManagement(p_ulCommandId)) return TRUE;
    if(InitCommand_GeneralGateway(p_ulCommandId)) return TRUE;
    if(InitCommand_LayerSettingServices(p_ulCommandId)) return TRUE;
	if(InitCommand_OpcsHistoryData(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_DCS_Esam2::InitCommand_ObjectDictionary(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case ESAM2_WRITE_OBJECT:
            {
                CCommand_DCS::InitCommand("WriteObject", ESAM2_WRITE_OBJECT);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "nodeId", ODT_UINT8);
                AddParameter(2, "index", ODT_UINT16);
                AddParameter(3, "subIndex", ODT_UINT8);
                AddParameter(4, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_WriteObject();
                return TRUE;
            };
        case ESAM2_READ_OBJECT:
            {
                CCommand_DCS::InitCommand("ReadObject", ESAM2_READ_OBJECT);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "nodeId", ODT_UINT8);
                AddParameter(2, "index", ODT_UINT16);
                AddParameter(3, "subIndex", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadObject();
                return TRUE;
            };
        case ESAM2_INITIATE_SEGMENT_WRITE:
            {
                CCommand_DCS::InitCommand("InitiateSegmentedWrite", ESAM2_INITIATE_SEGMENT_WRITE);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "nodeId", ODT_UINT8);
                AddParameter(2, "index", ODT_UINT16);
                AddParameter(3, "subIndex", ODT_UINT8);
                AddParameter(4, "objectLength", ODT_UINT32);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_InitiateSegmentedWrite();
                return TRUE;
            };
        case ESAM2_INITIATE_SEGMENT_READ:
            {
                CCommand_DCS::InitCommand("InitiateSegmentedRead", ESAM2_INITIATE_SEGMENT_READ);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "nodeId", ODT_UINT8);
                AddParameter(2, "index", ODT_UINT16);
                AddParameter(3, "subIndex", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "objectLength", ODT_UINT32);
                SetDefaultParameter_InitiateSegmentedRead();
                return TRUE;
            };
        case ESAM2_SEGMENT_WRITE:
            {
                CCommand_DCS::InitCommand("SegmentWrite", ESAM2_SEGMENT_WRITE);
                AddParameter(0, "length", ODT_UINT8);
                AddParameter(1, "controlByte", ODT_UINT8);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "lengthWritten", ODT_UINT8);
                AddReturnParameter(2, "controlByte", ODT_UINT8);
                SetDefaultParameter_SegmentedWrite();
                return TRUE;
            };
        case ESAM2_SEGMENT_READ:
            {
                CCommand_DCS::InitCommand("SegmentRead", ESAM2_SEGMENT_READ);
                AddParameter(0, "controlByte", ODT_UINT8);
                AddParameter(1, "dummyByte", ODT_UINT8, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "length", ODT_UINT8);
                AddReturnParameter(2, "controlByte", ODT_UINT8);
                AddReturnParameter(3, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SegmentedRead();
                return TRUE;
            };

        case ESAM2_ABORT_SEGMENT_TRANSFER:
            {
                CCommand_DCS::InitCommand("AbortSegmentedTransfer", ESAM2_ABORT_SEGMENT_TRANSFER);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "nodeId", ODT_UINT8);
                AddParameter(2, "index", ODT_UINT16);
                AddParameter(3, "subIndex", ODT_UINT8);
                AddParameter(4, "abortCode", ODT_UINT32);
                SetDefaultParameter_AbortSegmentedTransfer();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_DCS_Esam2::InitCommand_NetworkManagement(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case ESAM2_SEND_NMT_SERVICE:
            {
                CCommand_DCS::InitCommand("SendNMTService", ESAM2_SEND_NMT_SERVICE);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "nodeIdentifier", ODT_UINT8);
                AddParameter(2, "commandSpecifier", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendNMTService();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_DCS_Esam2::InitCommand_GeneralGateway(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case ESAM2_REQUEST_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("RequestCANFrame", ESAM2_REQUEST_CAN_FRAME);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "cobId", ODT_UINT16);
                AddParameter(2, "length", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_RequestCANFrame();
                return TRUE;
            };
        case ESAM2_SEND_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("SendCANFrame", ESAM2_SEND_CAN_FRAME);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "cobId", ODT_UINT16);
                AddParameter(2, "length", ODT_UINT16);
                AddParameter(3, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendCANFrame();
                return TRUE;
            };
        case ESAM2_READ_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("ReadCANFrame", ESAM2_READ_CAN_FRAME);
                AddParameter(0, "networkId", ODT_UINT16);
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

BOOL CCommand_DCS_Esam2::InitCommand_LayerSettingServices(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case ESAM2_SEND_LSS_FRAME:
            {
                CCommand_DCS::InitCommand("SendLSSFrame", ESAM2_SEND_LSS_FRAME);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendLSSFrame();
                return TRUE;
            };
        case ESAM2_READ_LSS_FRAME:
            {
                CCommand_DCS::InitCommand("ReadLSSFrame", ESAM2_READ_LSS_FRAME);
                AddParameter(0, "networkId", ODT_UINT16);
                AddParameter(1, "timeout", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadLSSFrame();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_DCS_Esam2::InitCommand_OpcsHistoryData(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case ESAM2_OPCS_HIST_GET_RANGE:
            {
                CCommand_DCS::InitCommand("OpcsHistGetRange", ESAM2_OPCS_HIST_GET_RANGE);
                AddParameter(0, "nbOfHistElements", ODT_UINT16);
                AddParameter(1, "elementIds", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
				AddReturnParameter(1, "sizeOfHistData", ODT_UINT32);
                SetDefaultParameter_OpcsHistGetRange();
                return TRUE;
            };
        case ESAM2_OPCS_HIST_GET_RANGE_RESULT:
            {
                CCommand_DCS::InitCommand("OpcsHistGetRangeResult", ESAM2_OPCS_HIST_GET_RANGE_RESULT);
                AddParameter(0, "histDataOffset", ODT_UINT32);
                AddParameter(1, "sizeOfHistData", ODT_UINT16);
				AddParameter(2, "done", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "histData", ODT_MEMORY_BLOCK);
                SetDefaultParameter_OpcsHistGetRangeResult();
                return TRUE;
            };
    }

    return FALSE;
}

void CCommand_DCS_Esam2::SetDefaultParameter_InitiateSegmentedRead()
{
    WORD usNetworkId(0);
    BYTE ubNodeId(1);
    WORD usIndex(0);
    BYTE ubSubIndex(0);
    DWORD ulErrorCode(0);
    DWORD ulObjectLength(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &ubNodeId, sizeof(ubNodeId));
    SetParameterData(2, &usIndex, sizeof(usIndex));
    SetParameterData(3, &ubSubIndex, sizeof(ubSubIndex));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, &ulObjectLength, sizeof(ulObjectLength));
}

void CCommand_DCS_Esam2::SetDefaultParameter_InitiateSegmentedWrite()
{
    WORD usNetworkId(0);
    BYTE ubNodeId(1);
    WORD usIndex(0);
    BYTE ubSubIndex(0);
    WORD usObjectLength(0);
    DWORD ulErrorCode(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &ubNodeId, sizeof(ubNodeId));
    SetParameterData(2, &usIndex, sizeof(usIndex));
    SetParameterData(3, &ubSubIndex, sizeof(ubSubIndex));
    SetParameterData(4, &usObjectLength, sizeof(usObjectLength));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
}

void CCommand_DCS_Esam2::SetDefaultParameter_ReadObject()
{
    WORD usNetworkId(0);
    BYTE ubNodeId(1);
    WORD usIndex(0);
    BYTE ubSubIndex(0);

    DWORD ulErrorCode(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &ubNodeId, sizeof(ubNodeId));
    SetParameterData(2, &usIndex, sizeof(usIndex));
    SetParameterData(3, &ubSubIndex, sizeof(ubSubIndex));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, pData, 0);
}

void CCommand_DCS_Esam2::SetDefaultParameter_SegmentedRead()
{
    BYTE ubControlByte(0);
    BYTE ubDummyByte(0);

    DWORD ulErrorCode(0);
    BYTE ubLength(0);
    BYTE ubRetControlByte(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &ubControlByte, sizeof(ubControlByte));
    SetParameterData(1, &ubDummyByte, sizeof(ubDummyByte));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, &ubLength, sizeof(ubLength));
    SetReturnParameterData(2, &ubControlByte, sizeof(ubControlByte));
    SetReturnParameterData(3, pData, 0);
}

void CCommand_DCS_Esam2::SetDefaultParameter_SegmentedWrite()
{
    BYTE ubLength(0);
    BYTE ubControlByte(0);
    void* pData(NULL);

    DWORD ulErrorCode(0);
    BYTE ubLengthWritten(0);
    BYTE ubRetControlByte(0);

    //Parameter
    SetParameterData(0, &ubLength, sizeof(ubLength));
    SetParameterData(1, &ubControlByte, sizeof(ubControlByte));
    SetParameterData(2, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, &ubLengthWritten, sizeof(ubLengthWritten));
    SetReturnParameterData(2, &ubRetControlByte, sizeof(ubRetControlByte));
}

void CCommand_DCS_Esam2::SetDefaultParameter_AbortSegmentedTransfer()
{
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usIndex(0);
    BYTE ubSubIndex(0);
    DWORD ulAbortCode(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(0, &ubNodeId, sizeof(ubNodeId));
    SetParameterData(1, &usIndex, sizeof(usIndex));
    SetParameterData(2, &ubSubIndex, sizeof(ubSubIndex));
    SetParameterData(3, &ulAbortCode, sizeof(ulAbortCode));
}

void CCommand_DCS_Esam2::SetDefaultParameter_WriteObject()
{
    WORD usNetworkId(0);
    BYTE ubNodeId(1);
    WORD usIndex(0);
    BYTE ubSubIndex(0);
    void* pData(NULL);
    DWORD ulErrorCode(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &ubNodeId, sizeof(ubNodeId));
    SetParameterData(2, &usIndex, sizeof(usIndex));
    SetParameterData(3, &ubSubIndex, sizeof(ubSubIndex));
    SetParameterData(4, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
}

void CCommand_DCS_Esam2::SetDefaultParameter_SendNMTService()
{
    WORD usNetworkId(0);
    BYTE ubNodeId(1);
    BYTE ubCmdSpecifier(0);
    DWORD ulErrorCode(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &ubNodeId, sizeof(ubNodeId));
    SetParameterData(2, &ubCmdSpecifier, sizeof(ubCmdSpecifier));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
}

void CCommand_DCS_Esam2::SetDefaultParameter_RequestCANFrame()
{
    WORD usNetworkId(0);
    WORD usCobId(0);
    WORD usLength(0);
    DWORD ulErrorCode(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &usCobId, sizeof(usCobId));
    SetParameterData(2, &usLength, sizeof(usLength));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, pData, 0);
}

void CCommand_DCS_Esam2::SetDefaultParameter_SendCANFrame()
{
    WORD usNetworkId(0);
    WORD usCobId(0);
    WORD usLength(0);
    void* pData(NULL);
    DWORD ulErrorCode(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &usCobId, sizeof(usCobId));
    SetParameterData(2, &usLength, sizeof(usLength));
    SetParameterData(3, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
}

void CCommand_DCS_Esam2::SetDefaultParameter_ReadCANFrame()
{
    WORD usNetworkId(0);
    WORD usCobId(0);
    WORD usLength(0);
    DWORD ulTimeout(0);
    void* pData(NULL);
    DWORD ulErrorCode(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &usCobId, sizeof(usCobId));
    SetParameterData(2, &usLength, sizeof(usLength));
    SetParameterData(3, &ulTimeout, sizeof(ulTimeout));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, pData, usLength);
}

void CCommand_DCS_Esam2::SetDefaultParameter_SendLSSFrame()
{
    WORD usNetworkId(0);
    void* pData(NULL);
    DWORD ulErrorCode(0);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
}

void CCommand_DCS_Esam2::SetDefaultParameter_ReadLSSFrame()
{
    WORD usNetworkId(0);
    WORD usTimeout(10);
    DWORD ulErrorCode(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &usNetworkId, sizeof(usNetworkId));
    SetParameterData(1, &usTimeout, sizeof(usTimeout));

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, pData, 0);
}

void CCommand_DCS_Esam2::SetDefaultParameter_OpcsHistGetRange()
{
	WORD usNbOfHistElements(0);
    void* pHistElementIds(NULL);
	DWORD ulErrorCode(0);
	DWORD ulSizeOfHistData(0);

    //Parameter
    SetParameterData(0, &usNbOfHistElements, sizeof(usNbOfHistElements));
    SetParameterData(1, pHistElementIds, 0);

    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, &ulSizeOfHistData, sizeof(ulSizeOfHistData));
}
	
void CCommand_DCS_Esam2::SetDefaultParameter_OpcsHistGetRangeResult()
{
	DWORD ulHistDataOffset(0);
    WORD usSizeOfHistData(0);
	BYTE uDone(0);
	DWORD ulErrorCode(0);
	void* pHistData(NULL);

    //Parameter
    SetParameterData(0, &ulHistDataOffset, sizeof(ulHistDataOffset));
	SetParameterData(1, &usSizeOfHistData, sizeof(usSizeOfHistData));
	SetParameterData(2, &uDone, sizeof(uDone));
    
    //ReturnParameter
    SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
    SetReturnParameterData(1, pHistData, 0);
}

