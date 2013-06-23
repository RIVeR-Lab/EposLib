// Command_DCS_Epos2.cpp: Implementierung der Klasse CCommand_DCS_Epos2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEpos2Def.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
///////////////////////////////////////////////////////////////////////
CCommand_DCS_Epos2::CCommand_DCS_Epos2()
{
}

CCommand_DCS_Epos2::CCommand_DCS_Epos2(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_DCS_Epos2::~CCommand_DCS_Epos2()
{
}

CCommandRoot* CCommand_DCS_Epos2::CloneCommand()
{
    CCommand_DCS_Epos2* pNewCommand;

    pNewCommand = new CCommand_DCS_Epos2();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_DCS_Epos2& CCommand_DCS_Epos2::operator=(CCommand_DCS_Epos2& p_rOther)
{
    if(this != &p_rOther)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&p_rOther);
    }

    return *this;
}

BOOL CCommand_DCS_Epos2::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();
    if(InitCommand_ObjectDictionary(p_ulCommandId)) return TRUE;
    if(InitCommand_NetworkManagement(p_ulCommandId)) return TRUE;
    if(InitCommand_GeneralGateway(p_ulCommandId)) return TRUE;
    if(InitCommand_LayerSettingServices(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_DCS_Epos2::InitCommand_ObjectDictionary(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case EPOS2_WRITE_OBJECT:
            {
                CCommand_DCS::InitCommand("WriteObject", EPOS2_WRITE_OBJECT);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddParameter(3, "data", ODT_MEMORY_BLOCK);
                AddParameter(4, "objectLength", ODT_UINT32, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_WriteObject();
                return TRUE;
            };
        case EPOS2_READ_OBJECT:
            {
                CCommand_DCS::InitCommand("ReadObject", EPOS2_READ_OBJECT);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddParameter(3, "objectLength", ODT_UINT32, FALSE, FALSE);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadObject();
                return TRUE;
            };
        case EPOS2_INITIATE_SEGMENT_WRITE:
            {
                CCommand_DCS::InitCommand("InitiateSegmentedWrite", EPOS2_INITIATE_SEGMENT_WRITE);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddParameter(3, "objectLength", ODT_UINT32);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_InitiateSegmentedWrite();
                return TRUE;
            };
        case EPOS2_INITIATE_SEGMENT_READ:
            {
                CCommand_DCS::InitCommand("InitiateSegmentedRead", EPOS2_INITIATE_SEGMENT_READ);
                AddParameter(0, "index", ODT_UINT16);
                AddParameter(1, "subIndex", ODT_UINT8);
                AddParameter(2, "nodeId", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_InitiateSegmentedRead();
                return TRUE;
            };
        case EPOS2_SEGMENT_WRITE:
            {
                CCommand_DCS::InitCommand("SegmentWrite", EPOS2_SEGMENT_WRITE);
                AddParameter(0, "nodeId", ODT_UINT16, FALSE, FALSE);
                AddParameter(1, "controlByte", ODT_UINT8);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "controlByte", ODT_UINT8);
                AddReturnParameter(2, "dummyByte", ODT_UINT8);
                SetDefaultParameter_SegmentedWrite();
                return TRUE;
            };
        case EPOS2_SEGMENT_READ:
            {
                CCommand_DCS::InitCommand("SegmentRead", EPOS2_SEGMENT_READ);
                AddParameter(0, "nodeId", ODT_UINT16, FALSE, FALSE);
                AddParameter(1, "controlByte", ODT_UINT8);
                AddParameter(2, "dummyByte", ODT_UINT8);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "controlByte", ODT_UINT8);
                AddReturnParameter(2, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SegmentedRead();
                return TRUE;
            };

        case EPOS2_ABORT_SEGMENT_TRANSFER:
            {
                CCommand_DCS::InitCommand("AbortSegmentedTransfer", EPOS2_ABORT_SEGMENT_TRANSFER);
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

BOOL CCommand_DCS_Epos2::InitCommand_NetworkManagement(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case EPOS2_SEND_NMT_SERVICE:
            {
                CCommand_DCS::InitCommand("SendNMTService", EPOS2_SEND_NMT_SERVICE);
                AddParameter(0, "nodeIdentifier", ODT_UINT16);
                AddParameter(1, "commandSpecifier", ODT_UINT16);
                SetDefaultParameter_SendNMTService();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_DCS_Epos2::InitCommand_GeneralGateway(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case EPOS2_REQUEST_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("RequestCANFrame", EPOS2_REQUEST_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_RequestCANFrame();
                return TRUE;
            };
        case EPOS2_SEND_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("SendCANFrame", EPOS2_SEND_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendCANFrame();
                return TRUE;
            };
        case EPOS2_READ_CAN_FRAME:
            {
                CCommand_DCS::InitCommand("ReadCANFrame", EPOS2_READ_CAN_FRAME);
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

BOOL CCommand_DCS_Epos2::InitCommand_LayerSettingServices(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case EPOS2_SEND_LSS_FRAME:
            {
                CCommand_DCS::InitCommand("SendLSSFrame", EPOS2_SEND_LSS_FRAME);
                AddParameter(0, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendLSSFrame();
                return TRUE;
            };
        case EPOS2_READ_LSS_FRAME:
            {
                CCommand_DCS::InitCommand("ReadLSSFrame", EPOS2_READ_LSS_FRAME);
                AddParameter(0, "timeout", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadLSSFrame();
                return TRUE;
            };
    }

    return FALSE;
}

void CCommand_DCS_Epos2::SetDefaultParameter_InitiateSegmentedRead()
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

void CCommand_DCS_Epos2::SetDefaultParameter_InitiateSegmentedWrite()
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

void CCommand_DCS_Epos2::SetDefaultParameter_ReadObject()
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

void CCommand_DCS_Epos2::SetDefaultParameter_SegmentedRead()
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

void CCommand_DCS_Epos2::SetDefaultParameter_SegmentedWrite()
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

void CCommand_DCS_Epos2::SetDefaultParameter_WriteObject()
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

void CCommand_DCS_Epos2::SetDefaultParameter_AbortSegmentedTransfer()
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

void CCommand_DCS_Epos2::SetDefaultParameter_SendNMTService()
{
    WORD wNodeId = 1;
    WORD wCmdSpecifier = 0;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wNodeId, sizeof(wNodeId));
    SetParameterData(1, &wCmdSpecifier, sizeof(wCmdSpecifier));
}

void CCommand_DCS_Epos2::SetDefaultParameter_RequestCANFrame()
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

void CCommand_DCS_Epos2::SetDefaultParameter_SendCANFrame()
{
    WORD wCobId = 0;
    WORD wLength = 0;
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, &wCobId, sizeof(wCobId));
    SetParameterData(1, &wLength, sizeof(wLength));
    SetParameterData(2, pData, wLength);

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

void CCommand_DCS_Epos2::SetDefaultParameter_ReadCANFrame()
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

void CCommand_DCS_Epos2::SetDefaultParameter_SendLSSFrame()
{
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

void CCommand_DCS_Epos2::SetDefaultParameter_ReadLSSFrame()
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
