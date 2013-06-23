// Command_VCS_Scanning.cpp: Implementierung der Klasse CCommand_VCS_Scanning.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Scanning.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////

CCommand_VCS_Scanning::CCommand_VCS_Scanning()
{
}

CCommand_VCS_Scanning::CCommand_VCS_Scanning(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_Scanning::~CCommand_VCS_Scanning()
{
}

CCommandRoot*CCommand_VCS_Scanning::CloneCommand()
{
    CCommand_VCS_Scanning* pNewCommand;

    pNewCommand = new CCommand_VCS_Scanning();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_Scanning& CCommand_VCS_Scanning::operator=(CCommand_VCS_Scanning& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_Scanning::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_Identification(p_ulCommandId)) return TRUE;
	if(InitCommand_Configuration(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_Scanning::InitCommand_Identification(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case SCAN_READ_VENDOR_ID:
            {
                CCommand_VCS::InitCommand("ReadVendorId", SCAN_READ_VENDOR_ID);
                AddReturnParameter(0, "vendorId", ODT_UINT32);
                SetDefaultParameter_ReadVendorId();
                return TRUE;
            };
		case SCAN_READ_DEVICE_TYPE:
            {
                CCommand_VCS::InitCommand("ReadDeviceType", SCAN_READ_DEVICE_TYPE);
                AddReturnParameter(0, "deviceType", ODT_UINT32);
                SetDefaultParameter_ReadDeviceType();
                return TRUE;
            };
		case SCAN_READ_DEVICE_NAME:
            {
                CCommand_VCS::InitCommand("ReadDeviceName", SCAN_READ_DEVICE_NAME);
                AddReturnParameter(0, "deviceName", ODT_STRING);
                SetDefaultParameter_ReadDeviceName();
                return TRUE;
            };
		case SCAN_READ_PRODUCT_CODE:
            {
                CCommand_VCS::InitCommand("ReadProductCode", SCAN_READ_PRODUCT_CODE);
                AddReturnParameter(0, "productCode", ODT_UINT32);
                SetDefaultParameter_ReadProductCode();
                return TRUE;
            };
		case SCAN_READ_REVISION_NUMBER:
            {
                CCommand_VCS::InitCommand("ReadRevisionNumber", SCAN_READ_REVISION_NUMBER);
                AddReturnParameter(0, "revisionNUmber", ODT_UINT32);
                SetDefaultParameter_ReadRevisionNumber();
                return TRUE;
            };
		case SCAN_READ_SERIAL_NUMBER:
            {
                CCommand_VCS::InitCommand("ReadSerialNumber", SCAN_READ_SERIAL_NUMBER);
                AddReturnParameter(0, "serialNumber", ODT_UINT32);
                SetDefaultParameter_ReadSerialNumber();
                return TRUE;
            };
        
    }

    return FALSE;
}

BOOL CCommand_VCS_Scanning::InitCommand_Configuration(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case SCAN_READ_NODE_ID:
            {
                CCommand_VCS::InitCommand("ReadNodeId", SCAN_READ_NODE_ID);
                AddReturnParameter(0, "nodeId", ODT_UINT32);
                SetDefaultParameter_ReadNodeId();
                return TRUE;
            };
		case SCAN_WRITE_NODE_ID:
            {
                CCommand_VCS::InitCommand("WriteNodeId", SCAN_WRITE_NODE_ID);
                AddParameter(0, "nodeId", ODT_UINT32);
                SetDefaultParameter_WriteNodeId();
                return TRUE;
            };
		case SCAN_READ_STATION_ID:
            {
                CCommand_VCS::InitCommand("ReadStationId", SCAN_READ_STATION_ID);
                AddReturnParameter(0, "stationId", ODT_UINT16);
                SetDefaultParameter_ReadStationId();
                return TRUE;
            };
		case SCAN_WRITE_STATION_ID:
            {
                CCommand_VCS::InitCommand("WriteStationId", SCAN_WRITE_STATION_ID);
                AddParameter(0, "stationId", ODT_UINT16);
                SetDefaultParameter_WriteStationId();
                return TRUE;
            };
		case SCAN_READ_TRANSFER_RATE:
            {
                CCommand_VCS::InitCommand("ReadTransferRate", SCAN_READ_TRANSFER_RATE);
                AddReturnParameter(0, "transferRate", ODT_UINT32);
                SetDefaultParameter_ReadTransferRate();
                return TRUE;
            };
		case SCAN_WRITE_TRANSFER_RATE:
            {
                CCommand_VCS::InitCommand("WriteTransferRate", SCAN_WRITE_TRANSFER_RATE);
                AddParameter(0, "transferRate", ODT_UINT32);
                SetDefaultParameter_WriteTransferRate();
                return TRUE;
            };
		case SCAN_STORE:
            {
                CCommand_VCS::InitCommand("Store", SCAN_STORE);
                SetDefaultParameter_Store();
                return TRUE;
            };
		case SCAN_RESET_DEVICE:
            {
                CCommand_VCS::InitCommand("ResetDevice", SCAN_RESET_DEVICE);
				AddParameter(0, "broadcast", ODT_BOOLEAN);
				AddParameter(1, "waitForReboot", ODT_BOOLEAN);
                SetDefaultParameter_ResetDevice();
                return TRUE;
            };
		case SCAN_WAIT_FOR_REBOOT:
            {
                CCommand_VCS::InitCommand("WaitForReboot", SCAN_WAIT_FOR_REBOOT);
                SetDefaultParameter_WaitForReboot();
                return TRUE;
            };
        
    }

    return FALSE;
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadVendorId()
{
    DWORD ulVendorId = 0;

    //Return Parameter
    SetReturnParameterData(0, &ulVendorId, sizeof(ulVendorId));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadDeviceType()
{
    DWORD ulDeviceType = 0;

    //Return Parameter
    SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadDeviceName()
{
    char deviceName[] = "";

    //Return Parameter
    SetReturnParameterData(0, deviceName, sizeof(deviceName));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadProductCode()
{
    DWORD ulProductCode = 0;

    //Return Parameter
    SetReturnParameterData(0, &ulProductCode, sizeof(ulProductCode));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadRevisionNumber()
{
    DWORD ulRevisionNumber = 0;

    //Return Parameter
    SetReturnParameterData(0, &ulRevisionNumber, sizeof(ulRevisionNumber));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadSerialNumber()
{
    DWORD ulSerialNumber = 0;

    //Return Parameter
    SetReturnParameterData(0, &ulSerialNumber, sizeof(ulSerialNumber));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadNodeId()
{
    DWORD ulNodeId = 0;

    //Return Parameter
    SetReturnParameterData(0, &ulNodeId, sizeof(ulNodeId));
}

void CCommand_VCS_Scanning::SetDefaultParameter_WriteNodeId()
{
    DWORD ulNodeId = 0;

    //Parameter
    SetParameterData(0, &ulNodeId, sizeof(ulNodeId));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadStationId()
{
    WORD usStationId = 0;

    //Return Parameter
    SetReturnParameterData(0, &usStationId, sizeof(usStationId));
}

void CCommand_VCS_Scanning::SetDefaultParameter_WriteStationId()
{
    WORD usStationId = 0;

    //Parameter
    SetParameterData(0, &usStationId, sizeof(usStationId));
}

void CCommand_VCS_Scanning::SetDefaultParameter_ReadTransferRate()
{
    DWORD ulTransferRate = 0;

    //Return Parameter
    SetReturnParameterData(0, &ulTransferRate, sizeof(ulTransferRate));
}

void CCommand_VCS_Scanning::SetDefaultParameter_WriteTransferRate()
{
    DWORD ulTransferRate = 0;

    //Parameter
    SetParameterData(0, &ulTransferRate, sizeof(ulTransferRate));
}

void CCommand_VCS_Scanning::SetDefaultParameter_Store()
{
	//No Parameters
}

void CCommand_VCS_Scanning::SetDefaultParameter_ResetDevice()
{
    BOOL oBroadcast = FALSE;
	BOOL oWaitForReboot = FALSE;

    //Parameter
    SetParameterData(0, &oBroadcast, sizeof(oBroadcast));
	SetParameterData(1, &oWaitForReboot, sizeof(oWaitForReboot));
}

void CCommand_VCS_Scanning::SetDefaultParameter_WaitForReboot()
{
    //No Parameters
}


