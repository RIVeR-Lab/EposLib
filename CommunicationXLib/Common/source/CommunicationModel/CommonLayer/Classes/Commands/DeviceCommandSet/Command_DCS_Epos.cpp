// Command_DCS_Epos.cpp: Implementierung der Klasse CCommand_DCS_Epos.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEposDef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_DCS_Epos::CCommand_DCS_Epos()
{
}

CCommand_DCS_Epos::CCommand_DCS_Epos(DWORD dCommandId)
{
    InitCommand(dCommandId);
}

CCommand_DCS_Epos::~CCommand_DCS_Epos()
{
}

//******************************************************************
CCommandRoot* CCommand_DCS_Epos::CloneCommand()
{
    CCommand_DCS_Epos* pNewCommand;

    pNewCommand = new CCommand_DCS_Epos();
    *pNewCommand = *this;

    return pNewCommand;
}

//******************************************************************
CCommand_DCS_Epos& CCommand_DCS_Epos::operator=(CCommand_DCS_Epos& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}
//******************************************************************
BOOL CCommand_DCS_Epos::InitCommand(DWORD dCommandId)
{
    ResetCommand();
    if(InitCommand_ObjectDictionary(dCommandId)) return TRUE;
    if(InitCommand_NetworkManagement(dCommandId)) return TRUE;
    if(InitCommand_GeneralGateway(dCommandId)) return TRUE;
    if(InitCommand_LayerSettingServices(dCommandId)) return TRUE;

    return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Epos::InitCommand_ObjectDictionary(DWORD dCommandId)
{
    switch(dCommandId)
    {
        case EPOS_WRITE_OD_OBJECT:
            {
                CCommand_DCS::InitCommand("WriteODObject", EPOS_WRITE_OD_OBJECT);
                AddParameter(0, "nodeId", ODT_UINT16);
                AddParameter(1, "index", ODT_UINT16);
                AddParameter(2, "subIndex", ODT_UINT16);
                AddParameter(3, "nbOfBytesToWrite", ODT_UINT16);
                AddParameter(4, "segmentByteLength", ODT_UINT16);
                AddParameter(5, "data", ODT_MEMORY_BLOCK);
                AddParameter(6, "signedDataType", ODT_INT32, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT16);
                AddReturnParameter(1, "segmentedWrite", ODT_UINT16);
                AddReturnParameter(2, "nbOfBytesWritten", ODT_UINT16);
                SetDefaultParameter_WriteODObject();
                return TRUE;
            };
        case EPOS_READ_OD_OBJECT:
            {
                CCommand_DCS::InitCommand("ReadODObject", EPOS_READ_OD_OBJECT);
                AddParameter(0, "nodeId", ODT_UINT16);
                AddParameter(1, "index", ODT_UINT16);
                AddParameter(2, "subIndex", ODT_UINT16);
                AddParameter(3, "nbOfBytesToRead", ODT_UINT16, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT16);
                AddReturnParameter(1, "segmentedRead", ODT_UINT16);
                AddReturnParameter(2, "nbOfBytesRead", ODT_UINT16);
                AddReturnParameter(3, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadODObject();
                return TRUE;
            };
        case EPOS_WRITE_OD_OBJECT_NEXT_SEGMENT:
            {
                CCommand_DCS::InitCommand("WriteODObjectNextSegment", EPOS_WRITE_OD_OBJECT_NEXT_SEGMENT);
                AddParameter(0, "nodeId", ODT_UINT16, FALSE, FALSE);
                AddParameter(1, "nbOfBytesToWrite", ODT_UINT16);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                AddParameter(3, "signedDataType", ODT_INT32, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT16);
                AddReturnParameter(1, "segmentedWrite", ODT_UINT16);
                AddReturnParameter(2, "nbOfBytesWritten", ODT_UINT16);
                SetDefaultParameter_WriteODObjectNextSegment();
                return TRUE;
            };
        case EPOS_READ_OD_OBJECT_NEXT_SEGMENT:
            {
                CCommand_DCS::InitCommand("ReadODObjectNextSegment", EPOS_READ_OD_OBJECT_NEXT_SEGMENT);
                AddParameter(0, "nodeId", ODT_UINT16, FALSE, FALSE);
                AddParameter(1, "nbOfBytesToRead", ODT_UINT16, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT16);
                AddReturnParameter(1, "segmentedRead", ODT_UINT16);
                AddReturnParameter(2, "nbOfBytesRead", ODT_UINT16);
                AddReturnParameter(3, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadODObjectNextSegment();
                return TRUE;
            };
        case EPOS_WRITE_OBJECT:
            {
                CCommand_DCS::InitCommand("WriteObject", EPOS_WRITE_OBJECT);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddParameter(3, "data", ODT_MEMORY_BLOCK);
                AddParameter(4, "objectLength", ODT_UINT32, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_WriteObject();
                return TRUE;
            };
        case EPOS_READ_OBJECT:
            {
                CCommand_DCS::InitCommand("ReadObject", EPOS_READ_OBJECT);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddParameter(3, "objectLength", ODT_UINT32, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadObject();
                return TRUE;
            };
        case EPOS_INITIATE_SEGMENT_WRITE:
            {
                CCommand_DCS::InitCommand("InitiateSegmentedWrite", EPOS_INITIATE_SEGMENT_WRITE);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddParameter(3, "objectLength", ODT_UINT32);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_InitiateSegmentedWrite();
                return TRUE;
            };
        case EPOS_INITIATE_SEGMENT_READ:
            {
                CCommand_DCS::InitCommand("InitiateSegmentedRead", EPOS_INITIATE_SEGMENT_READ);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_InitiateSegmentedRead();
                return TRUE;
            };
        case EPOS_SEGMENT_WRITE:
            {
                CCommand_DCS::InitCommand("SegmentWrite", EPOS_SEGMENT_WRITE);
                AddParameter(0, "nodeId", ODT_UINT16, FALSE, FALSE);
                AddParameter(1, "controlByte", ODT_UINT8);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "controlByte", ODT_UINT8);
                AddReturnParameter(2, "dummyByte", ODT_UINT8);
                SetDefaultParameter_SegmentedWrite();
                return TRUE;
            };
        case EPOS_SEGMENT_READ:
            {
                CCommand_DCS::InitCommand("SegmentRead", EPOS_SEGMENT_READ);
                AddParameter(0, "nodeId", ODT_UINT16, FALSE, FALSE);
                AddParameter(1, "controlByte", ODT_UINT8);
                AddParameter(2, "dummyByte", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "controlByte", ODT_UINT8);
                AddReturnParameter(2, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SegmentedRead();
                return TRUE;
            };

        case EPOS_ABORT_SEGMENT_TRANSFER:
            {
                CCommand_DCS::InitCommand("AbortSegmentedTransfer", EPOS_ABORT_SEGMENT_TRANSFER);
                AddParameter(0, "nodeId", ODT_UINT16, FALSE, FALSE);
                AddParameter(1, "index", ODT_UINT16);
                AddParameter(2, "subIndex", ODT_UINT8);
                AddParameter(3, "abortCode", ODT_UINT32);
                SetDefaultParameter_AbortSegmentedTransfer();
                return TRUE;
            };
    }

    return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Epos::InitCommand_NetworkManagement(DWORD dCommandId)
{
    switch(dCommandId)
    {
        case EPOS_SEND_NMT_SERVICE:
            {
                CCommand_DCS::InitCommand("SendNMTService", EPOS_SEND_NMT_SERVICE);
                AddParameter(0, "nodeIdentifier", ODT_UINT16);
                AddParameter(1, "commandSpecifier", ODT_UINT16);
                SetDefaultParameter_SendNMTService();
                return TRUE;
            };
    }

    return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Epos::InitCommand_GeneralGateway(DWORD dCommandId)
{
    switch(dCommandId)
    {
        case EPOS_REQUEST_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("RequestCANFrame", EPOS_REQUEST_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_RequestCANFrame();
                return TRUE;
            };
        case EPOS_SEND_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("SendCANFrame", EPOS_SEND_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SendCANFrame();
                return TRUE;
            };
        case EPOS_READ_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("ReadCANFrame", EPOS_READ_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "timeout", ODT_UINT32);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadCANFrame();
                return TRUE;
            };
    }

    return FALSE;
}

//******************************************************************
BOOL CCommand_DCS_Epos::InitCommand_LayerSettingServices(DWORD dCommandId)
{
    switch(dCommandId)
    {
        case EPOS_SEND_LSS_FRAME:
            {
                CCommand_DCS::InitCommand("SendLSSFrame", EPOS_SEND_LSS_FRAME);
                AddParameter(0, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendLSSFrame();
                return TRUE;
            };
        case EPOS_READ_LSS_FRAME:
            {
                CCommand_DCS::InitCommand("ReadLSSFrame", EPOS_READ_LSS_FRAME);
                AddParameter(0, "timeout", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadLSSFrame();
                return TRUE;
            };
    }

    return FALSE;
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_InitiateSegmentedRead()
{
    BYTE uNodeId = 1;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wIndex, sizeof(wIndex));
    SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
    SetParameterData(2, &uNodeId, sizeof(uNodeId));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_InitiateSegmentedWrite()
{
    BYTE uNodeId = 1;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    WORD objectLength = 0;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wIndex, sizeof(wIndex));
    SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
    SetParameterData(2, &uNodeId, sizeof(uNodeId));
    SetParameterData(3, &objectLength, sizeof(objectLength));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_ReadObject()
{
    BYTE uNodeId = 1;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    DWORD dObjectLength = 0;
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wIndex, sizeof(wIndex));
    SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
    SetParameterData(2, &uNodeId, sizeof(uNodeId));
    SetParameterData(3, &dObjectLength, sizeof(dObjectLength));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, pData, 0);
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_ReadODObject()
{
    WORD wNodeId = 1;
    WORD wIndex = 0;
    WORD wSubIndex = 0;
    WORD wNbOfBytesToRead = 0;
    WORD* pData = NULL;
    WORD wNbOfBytesRead= 0;
    WORD wSegmentedRead = 0;
    WORD wErrorCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &wIndex, sizeof(wIndex));
    SetParameterData(2, &wSubIndex, sizeof(wSubIndex));
    SetParameterData(3, &wNbOfBytesToRead, sizeof(wNbOfBytesToRead));

    //ReturnParameter
    SetReturnParameterData(0, &wErrorCode, sizeof(wErrorCode));
    SetReturnParameterData(1, &wSegmentedRead, sizeof(wSegmentedRead));
    SetReturnParameterData(2, &wNbOfBytesRead, sizeof(wNbOfBytesRead));
    SetReturnParameterData(3, pData, 0);
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_ReadODObjectNextSegment()
{
    WORD wNodeId = 1;
    WORD wNbOfBytesToRead = 0;
    WORD* pData = NULL;
    WORD wNbOfBytesRead= 0;
    WORD wSegmentedRead = 0;
    WORD wErrorCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &wNbOfBytesToRead, sizeof(wNbOfBytesToRead));

    //ReturnParameter
    SetReturnParameterData(0, &wErrorCode, sizeof(wErrorCode));
    SetReturnParameterData(1, &wSegmentedRead, sizeof(wSegmentedRead));
    SetReturnParameterData(2, &wNbOfBytesRead, sizeof(wNbOfBytesRead));
    SetReturnParameterData(3, pData, 0);
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_SegmentedRead()
{
    WORD wNodeId = 1;
    BYTE uControlByte = 0;
    BYTE uDummyByte = 0;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &uControlByte, sizeof(uControlByte));
    SetParameterData(2, &uDummyByte, sizeof(uDummyByte));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, &uControlByte, sizeof(uControlByte));
    SetReturnParameterData(2, NULL, 0);
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_SegmentedWrite()
{
    WORD wNodeId = 1;
    BYTE uControlByte = 0;
    BYTE uDummyByte = 0;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &uControlByte, sizeof(uControlByte));
    SetParameterData(2, NULL, 0);

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, &uControlByte, sizeof(uControlByte));
    SetReturnParameterData(2, &uDummyByte, sizeof(uDummyByte));
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_WriteObject()
{
    BYTE uNodeId = 1;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    WORD objectLength = 0;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wIndex, sizeof(wIndex));
    SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
    SetParameterData(2, &uNodeId, sizeof(uNodeId));
    SetParameterData(3, NULL, 0);
    SetParameterData(4, &objectLength, sizeof(objectLength));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_WriteODObject()
{
    WORD wNodeId = 1;
    WORD wIndex = 0;
    WORD wSubIndex = 0;
    WORD nbOfBytesToWrite = 0;
    WORD segmentByteLength = 0;
    BOOL oSignedDataType = FALSE;
    WORD nbOfBytesWritten = 0;
    WORD segmentedWrite = 0;
    WORD wErrorCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &wIndex, sizeof(wIndex));
    SetParameterData(2, &wSubIndex, sizeof(wSubIndex));
    SetParameterData(3, &nbOfBytesToWrite, sizeof(nbOfBytesToWrite));
    SetParameterData(4, &segmentByteLength, sizeof(segmentByteLength));
    SetParameterData(5, NULL, 0);
    SetParameterData(6, &oSignedDataType, sizeof(oSignedDataType));

    //ReturnParameter
    SetReturnParameterData(0, &wErrorCode, sizeof(wErrorCode));
    SetReturnParameterData(1, &segmentedWrite, sizeof(segmentedWrite));
    SetReturnParameterData(2, &nbOfBytesWritten, sizeof(nbOfBytesWritten));
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_WriteODObjectNextSegment()
{
    WORD wNodeId = 1;
    WORD nbOfBytesToWrite = 0;
    BOOL oSignedDataType = FALSE;
    WORD nbOfBytesWritten = 0;
    WORD segmentedWrite = 0;
    WORD wErrorCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &nbOfBytesToWrite, sizeof(nbOfBytesToWrite));
    SetParameterData(2, NULL, 0);
    SetParameterData(3, &oSignedDataType, sizeof(oSignedDataType));

    //ReturnParameter
    SetReturnParameterData(0, &wErrorCode, sizeof(wErrorCode));
    SetReturnParameterData(1, &segmentedWrite, sizeof(segmentedWrite));
    SetReturnParameterData(2, &nbOfBytesWritten, sizeof(nbOfBytesWritten));
}

//*****************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_AbortSegmentedTransfer()
{
    WORD wNodeId = 1;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    DWORD dAbortCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &wIndex, sizeof(wIndex));
    SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
    SetParameterData(3, &dAbortCode, sizeof(dAbortCode));
}

//**************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_SendNMTService()
{
    WORD wNodeId = 1;
    WORD wCmdSpecifier = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &wCmdSpecifier, sizeof(wCmdSpecifier));
}

//**************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_RequestCANFrame()
{
    WORD wCobId = 0;
    WORD wLength= 0;
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wCobId, sizeof(wCobId));
    SetParameterData(1, &wLength, sizeof(wLength));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, pData, wLength);
}

//**************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_SendCANFrame()
{
    WORD wCobId = 0;
    WORD wLength = 0;
    void* pData = NULL;

    //Parameter
    SetParameterData(0, &wCobId, sizeof(wCobId));
    SetParameterData(1, &wLength, sizeof(wLength));
    SetParameterData(2, pData, wLength);
}

//**************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_ReadCANFrame()
{
    WORD wCobId = 0;
    WORD wLength = 0;
    DWORD ulTimeout = 0;
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wCobId, sizeof(wCobId));
    SetParameterData(1, &wLength, sizeof(wLength));
    SetParameterData(2, &ulTimeout, sizeof(ulTimeout));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, pData, wLength);
}

//**************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_SendLSSFrame()
{
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

//**************************************************************************
void CCommand_DCS_Epos::SetDefaultParameter_ReadLSSFrame()
{
    WORD wTimeout = 10;
    DWORD dErrorCode = 0;
    void* pData = NULL;

    //Parameter
    SetParameterData(0, &wTimeout, sizeof(wTimeout));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, pData, 0);
}
