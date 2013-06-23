// Command_VCS_Plc2.cpp: Implementierung der Klasse CCommand_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////
CCommand_VCS_Plc2::CCommand_VCS_Plc2()
{
}

CCommand_VCS_Plc2::CCommand_VCS_Plc2(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_Plc2::~CCommand_VCS_Plc2()
{
}

CCommandRoot*CCommand_VCS_Plc2::CloneCommand()
{
    CCommand_VCS_Plc2* pNewCommand;

    pNewCommand = new CCommand_VCS_Plc2();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_Plc2& CCommand_VCS_Plc2::operator=(CCommand_VCS_Plc2& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_Plc2::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_Initialisation(p_ulCommandId)) return TRUE;
    if(InitCommand_ProgramStatus(p_ulCommandId)) return TRUE;
    if(InitCommand_ProgramVariableAccess(p_ulCommandId)) return TRUE;
    if(InitCommand_ProcessInputOutputAccess(p_ulCommandId)) return TRUE;
    if(InitCommand_ProcessImageAccess(p_ulCommandId)) return TRUE;
    if(InitCommand_GeneralGateway(p_ulCommandId)) return TRUE;
    if(InitCommand_LayerSettingServices(p_ulCommandId)) return TRUE;
    if(InitCommand_FirmwareDownload(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_Plc2::InitCommand_Initialisation(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_SET_BOOTUP_BEHAVIOUR:
            {
                CCommand_VCS::InitCommand("SetBootupBehavior", PLC2_SET_BOOTUP_BEHAVIOUR);
                AddParameter(0, "programControl", ODT_UINT16);
                SetDefaultParameter_SetBootupBehavior();
                return TRUE;
            };
        case PLC2_GET_BOOTUP_BEHAVIOUR:
            {
                CCommand_VCS::InitCommand("GetBootupBehavior", PLC2_GET_BOOTUP_BEHAVIOUR);
                AddReturnParameter(0, "programControl", ODT_UINT16);
                SetDefaultParameter_GetBootupBehavior();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Plc2::InitCommand_ProgramStatus(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_COLDSTART_PLC:
            {
                CCommand_VCS::InitCommand("ColdstartPlc", PLC2_COLDSTART_PLC);
                SetDefaultParameter_ColdstartPlc();
                return TRUE;
            };
        case PLC2_WARMSTART_PLC:
            {
                CCommand_VCS::InitCommand("WarmstartPlc", PLC2_WARMSTART_PLC);
                SetDefaultParameter_WarmstartPlc();
                return TRUE;
            };
        case PLC2_HOTSTART_PLC:
            {
                CCommand_VCS::InitCommand("HotstartPlc", PLC2_HOTSTART_PLC);
                SetDefaultParameter_HotstartPlc();
                return TRUE;
            };
        case PLC2_STOP_PLC:
            {
                CCommand_VCS::InitCommand("StopPlc", PLC2_STOP_PLC);
                SetDefaultParameter_StopPlc();
                return TRUE;
            };
        case PLC2_ERASE_PLC:
            {
                CCommand_VCS::InitCommand("ErasePlc", PLC2_ERASE_PLC);
                SetDefaultParameter_ErasePlc();
                return TRUE;
            };
        case PLC2_GET_PLC_STATUS:
            {
                CCommand_VCS::InitCommand("GetPlcStatus", PLC2_GET_PLC_STATUS);
                AddReturnParameter(0, "isProgramRunning", ODT_BOOLEAN);
                AddReturnParameter(1, "isProgramAvailable", ODT_BOOLEAN);
                SetDefaultParameter_GetPlcStatus();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Plc2::InitCommand_ProgramVariableAccess(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_INIT_VARIABLE_TABLE:
            {
                CCommand_VCS::InitCommand("InitVariableTable", PLC2_INIT_VARIABLE_TABLE);
                AddParameter(0, "length", ODT_UINT16);
                AddParameter(1, "projectPathName", ODT_MEMORY_BLOCK);
                SetDefaultParameter_InitVariableTable();
                return TRUE;
            };
        case PLC2_SET_VARIABLE:
            {
                CCommand_VCS::InitCommand("SetVariable", PLC2_SET_VARIABLE);
                AddParameter(0, "nameLength", ODT_UINT32);
                AddParameter(1, "variableName", ODT_MEMORY_BLOCK);
                AddParameter(2, "nbOfBytesToWrite", ODT_UINT32);
                AddParameter(3, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "nbOfBytesWritten", ODT_UINT32);
                SetDefaultParameter_SetVariable();
                return TRUE;
            };
        case PLC2_GET_VARIABLE:
            {
                CCommand_VCS::InitCommand("GetVariable", PLC2_GET_VARIABLE);
                AddParameter(0, "nameLength", ODT_UINT32);
                AddParameter(1, "variableName", ODT_MEMORY_BLOCK);
                AddParameter(2, "nbOfBytesToRead", ODT_UINT32);
                AddReturnParameter(0, "nbOfBytesRead", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_GetVariable();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Plc2::InitCommand_ProcessInputOutputAccess(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_SET_PROCESS_INPUT:
            {
                CCommand_VCS::InitCommand("SetProcessInput", PLC2_SET_PROCESS_INPUT);
                AddParameter(0, "processInputType", ODT_UINT16);
                AddParameter(1, "elementNumber", ODT_UINT8);
                AddParameter(2, "nbOfBytesToWrite", ODT_UINT32);
                AddParameter(3, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "nbOfBytesWritten", ODT_UINT32);
                SetDefaultParameter_SetProcessInput();
                return TRUE;
            };
        case PLC2_GET_PROCESS_OUTPUT:
            {
                CCommand_VCS::InitCommand("GetProcessOutput", PLC2_GET_PROCESS_OUTPUT);
                AddParameter(0, "processOutputType", ODT_UINT16);
                AddParameter(1, "elementNumber", ODT_UINT8);
                AddParameter(2, "nbOfBytesToRead", ODT_UINT32);
                AddReturnParameter(0, "nbOfBytesRead", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_GetProcessOutput();
                return TRUE;
            };
        case PLC2_SET_PROCESS_INPUT_BIT:
            {
                CCommand_VCS::InitCommand("SetProcessInputBit", PLC2_SET_PROCESS_INPUT_BIT);
                AddParameter(0, "processInputType", ODT_UINT16);
                AddParameter(1, "elementNumber", ODT_UINT8);
                AddParameter(2, "bitNumber", ODT_UINT8);
                AddParameter(3, "bitState", ODT_UINT8);
                SetDefaultParameter_SetProcessInputBit();
                return TRUE;
            };
        case PLC2_GET_PROCESS_OUTPUT_BIT:
            {
                CCommand_VCS::InitCommand("GetProcessOutputBit", PLC2_GET_PROCESS_OUTPUT_BIT);
                AddParameter(0, "processOutputType", ODT_UINT16);
                AddParameter(1, "elementNumber", ODT_UINT8);
                AddParameter(2, "bitNumber", ODT_UINT8);
                AddReturnParameter(0, "bitState", ODT_UINT8);
                SetDefaultParameter_GetProcessOutputBit();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Plc2::InitCommand_ProcessImageAccess(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_SET_PROCESS_INPUT_IMAGE:
            {
                CCommand_VCS::InitCommand("SetProcessInputImage", PLC2_SET_PROCESS_INPUT_IMAGE);
                AddParameter(0, "selectionStart", ODT_UINT32);
                AddParameter(1, "selectionLength", ODT_UINT32);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SetProcessInputImage();
                return TRUE;
            };
        case PLC2_GET_PROCESS_OUTPUT_IMAGE:
            {
                CCommand_VCS::InitCommand("GetProcessOutputImage", PLC2_GET_PROCESS_OUTPUT_IMAGE);
                AddParameter(0, "selectionStart", ODT_UINT32);
                AddParameter(1, "selectionLength", ODT_UINT32);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_GetProcessOutputImage();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Plc2::InitCommand_GeneralGateway(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_REQUEST_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("RequestCANFrame", PLC2_REQUEST_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_RequestCANFrame();
                return TRUE;
            };
        case PLC2_SEND_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("SendCANFrame", PLC2_SEND_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SendCANFrame();
                return TRUE;
            };
        case PLC2_READ_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("ReadCANFrame", PLC2_READ_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "timeout", ODT_UINT32);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadCANFrame();
                return TRUE;
            };
        case PLC2_SEND_NMT_SERVICE:
            {
                CCommand_VCS::InitCommand("SendNMTService", PLC2_SEND_NMT_SERVICE);
                AddParameter(0, "nodeIdentifier", ODT_UINT16);
                AddParameter(1, "commandSpecifier", ODT_UINT16);
                SetDefaultParameter_SendNMTService();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Plc2::InitCommand_LayerSettingServices(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_SEND_LSS_FRAME:
            {
                CCommand_VCS::InitCommand("SendLSSFrame", PLC2_SEND_LSS_FRAME);
                AddParameter(0, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendLSSFrame();
                return TRUE;
            };
        case PLC2_READ_LSS_FRAME:
            {
                CCommand_VCS::InitCommand("ReadLSSFrame", PLC2_READ_LSS_FRAME);
                AddParameter(0, "timeout", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadLSSFrame();
                return TRUE;
            };
    }

    return FALSE;
}

//Firmware Download
BOOL CCommand_VCS_Plc2::InitCommand_FirmwareDownload(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC2_GET_BASE_SECTOR_VERSION:
            {
                CCommand_VCS::InitCommand("GetBaseSectorVersion", PLC2_GET_BASE_SECTOR_VERSION);
                AddReturnParameter(0, "baseSectorVersion", ODT_UINT16);
                SetDefaultParameter_GetBaseSectorVersion();
                return TRUE;
            };
        case PLC2_GET_SERIAL_NUMBER:
            {
                CCommand_VCS::InitCommand("GetSerialNumber", PLC2_GET_SERIAL_NUMBER);
                AddReturnParameter(0, "serialNumber", ODT_UINT64);
                SetDefaultParameter_GetSerialNumber();
                return TRUE;
            };
    }

    return FALSE;
}

//Initialisation
void CCommand_VCS_Plc2::SetDefaultParameter_SetBootupBehavior()
{
    WORD usProgramControl(0);

    //Parameter
    SetParameterData(0, &usProgramControl, sizeof(usProgramControl));
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetBootupBehavior()
{
    WORD usProgramControl(0);

    //ReturnParameter
    SetReturnParameterData(0, &usProgramControl, sizeof(usProgramControl));
}

//Program Status
void CCommand_VCS_Plc2::SetDefaultParameter_ColdstartPlc()
{
}

void CCommand_VCS_Plc2::SetDefaultParameter_WarmstartPlc()
{
}

void CCommand_VCS_Plc2::SetDefaultParameter_HotstartPlc()
{
}

void CCommand_VCS_Plc2::SetDefaultParameter_StopPlc()
{
}

void CCommand_VCS_Plc2::SetDefaultParameter_ErasePlc()
{
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetPlcStatus()
{
    BOOL oIsProgramAvailable(FALSE);
    BOOL oIsProgramRunnig(FALSE);

    //ReturnParameter
    SetReturnParameterData(0, &oIsProgramRunnig, sizeof(oIsProgramRunnig));
    SetReturnParameterData(1, &oIsProgramAvailable, sizeof(oIsProgramAvailable));
}

//Program Variable Access
void CCommand_VCS_Plc2::SetDefaultParameter_InitVariableTable()
{
    DWORD ulDataLength(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, pData, ulDataLength);
}

void CCommand_VCS_Plc2::SetDefaultParameter_SetVariable()
{
	CStdString VariableName(_T(""));
    DWORD ulNbOfBytesToWrite(0);
    DWORD ulNbOfBytesWrite(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &VariableName, VariableName.size());
    SetParameterData(1, &ulNbOfBytesToWrite, sizeof(ulNbOfBytesToWrite));
    SetParameterData(2, pData, ulNbOfBytesToWrite);

    //ReturnParameter
    SetReturnParameterData(0, &ulNbOfBytesWrite, sizeof(ulNbOfBytesWrite));
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetVariable()
{
    CStdString VariableName(_T(""));
    DWORD ulNbOfBytesRead(0);
    DWORD ulNbOfBytesToRead(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &VariableName, VariableName.size());
    SetParameterData(1, &ulNbOfBytesToRead, sizeof(ulNbOfBytesToRead));

    //ReturnParameter
    SetReturnParameterData(0, &ulNbOfBytesRead, sizeof(ulNbOfBytesRead));
    SetReturnParameterData(1, pData, ulNbOfBytesToRead);
}

//Process Input/Output Access
void CCommand_VCS_Plc2::SetDefaultParameter_SetProcessInput()
{
    BYTE ubElementNumber(0);
    DWORD ulNbOfBytesToWrite(0);
    DWORD ulNbOfBytesWrite(0);
    void* pData(NULL);
    WORD usProcessInputType(0);

    //Parameter
    SetParameterData(0, &usProcessInputType, sizeof(usProcessInputType));
    SetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
    SetParameterData(2, &ulNbOfBytesToWrite, sizeof(ulNbOfBytesToWrite));
    SetParameterData(3, pData, ulNbOfBytesToWrite);

    //ReturnParameter
    SetReturnParameterData(0, &ulNbOfBytesWrite, sizeof(ulNbOfBytesWrite));
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetProcessOutput()
{
    BYTE ubElementNumber(0);
    DWORD ulNbOfBytesRead(0);
    DWORD ulNbOfBytesToRead(0);
    void* pData(NULL);
    WORD usProcessOutputType(0);

    //Parameter
    SetParameterData(0, &usProcessOutputType, sizeof(usProcessOutputType));
    SetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
    SetParameterData(2, &ulNbOfBytesToRead, sizeof(ulNbOfBytesToRead));

    //ReturnParameter
    SetReturnParameterData(0, &ulNbOfBytesRead, sizeof(ulNbOfBytesRead));
    SetReturnParameterData(1, pData, ulNbOfBytesToRead);
}

void CCommand_VCS_Plc2::SetDefaultParameter_SetProcessInputBit()
{
    WORD usProcessInputType(0);
    BYTE ubElementNumber(0);
    BYTE ubBitNumber(0);
    BYTE ubBitState(0);

    //Parameter
    SetParameterData(0, &usProcessInputType, sizeof(usProcessInputType));
    SetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
    SetParameterData(2, &ubBitNumber, sizeof(ubBitNumber));
    SetParameterData(3, &ubBitState, sizeof(ubBitState));

    //ReturnParameter
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetProcessOutputBit()
{
    WORD usProcessOutputType(0);
    BYTE ubElementNumber(0);
    BYTE ubBitNumber(0);
    BYTE ubBitState(0);

    //Parameter
    SetParameterData(0, &usProcessOutputType, sizeof(usProcessOutputType));
    SetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
    SetParameterData(2, &ubBitNumber, sizeof(ubBitNumber));

    //ReturnParameter
    SetReturnParameterData(0, &ubBitState, sizeof(ubBitState));
}

//Process Image Access
void CCommand_VCS_Plc2::SetDefaultParameter_SetProcessInputImage()
{
    DWORD ulSelectionStart(0);
    DWORD ulSelectionLength(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &ulSelectionStart, sizeof(ulSelectionStart));
    SetParameterData(1, &ulSelectionLength, sizeof(ulSelectionLength));
    SetParameterData(2, pData, ulSelectionLength);
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetProcessOutputImage()
{
    DWORD ulSelectionStart(0);
    DWORD ulSelectionLength(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &ulSelectionStart, sizeof(ulSelectionStart));
    SetParameterData(1, &ulSelectionLength, sizeof(ulSelectionLength));

    //ReturnParameter
    SetReturnParameterData(0, pData, ulSelectionLength);
}

//General Gateway
void CCommand_VCS_Plc2::SetDefaultParameter_RequestCANFrame()
{
    WORD usCobId(0);
    WORD usLength(0);
    void* data(NULL);

    //Parameter
    SetParameterData(0, &usCobId, sizeof(usCobId));
    SetParameterData(1, &usLength, sizeof(usLength));

    //ReturnParameter
    SetReturnParameterData(0, data, usLength);
}

void CCommand_VCS_Plc2::SetDefaultParameter_SendCANFrame()
{
    WORD usCobId(0);
    WORD usLength(0);
    void* data(NULL);

    //Parameter
    SetParameterData(0, &usCobId, sizeof(usCobId));
    SetParameterData(1, &usLength, sizeof(usLength));
    SetParameterData(2, data, usLength);
}

void CCommand_VCS_Plc2::SetDefaultParameter_ReadCANFrame()
{
    WORD usCobId(0);
    WORD usLength(0);
    DWORD ulTimeout(0);
    void* data(NULL);

    //Parameter
    SetParameterData(0, &usCobId, sizeof(usCobId));
    SetParameterData(1, &usLength, sizeof(usLength));
    SetParameterData(2, &ulTimeout, sizeof(ulTimeout));

    //ReturnParameter
    SetReturnParameterData(0, data, usLength);
}

void CCommand_VCS_Plc2::SetDefaultParameter_SendNMTService()
{
    WORD usNodeIdentifier(0);
    WORD usCommandSpecifier(0);

    //Parameter
    SetParameterData(0, &usNodeIdentifier, sizeof(usNodeIdentifier));
    SetParameterData(1, &usCommandSpecifier, sizeof(usCommandSpecifier));
}

void CCommand_VCS_Plc2::SetDefaultParameter_SendLSSFrame()
{
    void* pData(NULL);
    DWORD dErrorCode(0);

    //Parameter
    SetParameterData(0, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

void CCommand_VCS_Plc2::SetDefaultParameter_ReadLSSFrame()
{
    WORD usTimeout(10);
    DWORD dErrorCode(0);
    void* pData(NULL);

    //Parameter
    SetParameterData(0, &usTimeout, sizeof(usTimeout));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, pData, 0);
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetSerialNumber()
{
    unsigned __int64 ullSerialNb(0);

    //ReturnParameter
    SetReturnParameterData(0, &ullSerialNb, sizeof(ullSerialNb));
}

void CCommand_VCS_Plc2::SetDefaultParameter_GetBaseSectorVersion()
{
    WORD usBaseSectorVersion(0);

    //ReturnParameter
    SetReturnParameterData(0, &usBaseSectorVersion, sizeof(usBaseSectorVersion));
}

