// ErrorHandling.cpp: Implementierung der Klasse CErrorHandling.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>
#include <Ui/MmcMessageDialog.h>
#include <MmcTypeDefinition.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CErrorHandling::CErrorHandling()
{
    m_pErrorProducer = 0;
}

CErrorHandling::~CErrorHandling()
{
    DeleteErrorProducer();
}

void CErrorHandling::DeleteErrorProducer()
{
    if(m_pErrorProducer)
    {
        delete m_pErrorProducer;
        m_pErrorProducer = 0;
    }
}

BOOL CErrorHandling::GetErrorDescription(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    if(p_pDescription)
    {
        if(GetErrorDescription_GeneralError(p_ulErrorCode, p_pDescription)) return TRUE;
        if(GetErrorDescription_I(p_ulErrorCode, p_pDescription)) return TRUE;
        if(GetErrorDescription_PS(p_ulErrorCode, p_pDescription)) return TRUE;
        if(GetErrorDescription_DCS(p_ulErrorCode, p_pDescription)) return TRUE;
        if(GetErrorDescription_VCS(p_ulErrorCode, p_pDescription)) return TRUE;

        if(p_pDescription) *p_pDescription = "Unknown Error";
    }

    return FALSE;
}

BOOL CErrorHandling::GetErrorDescription_I(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_I_OpeningInterface:    *p_pDescription = "Error opening interface"; return TRUE;
        case k_Error_I_ClosingInterface:    *p_pDescription = "Error closing interface"; return TRUE;
        case k_Error_I_InterfaceNotOpen:    *p_pDescription = "Interface is not open"; return TRUE;
        case k_Error_I_OpeningPort:         *p_pDescription = "Error opening port"; return TRUE;
        case k_Error_I_ClosingPort:         *p_pDescription = "Error closing port"; return TRUE;
        case k_Error_I_PortNotOpen:         *p_pDescription = "Port is not open"; return TRUE;
        case k_Error_I_ResetPort:           *p_pDescription = "Error resetting port"; return TRUE;
        case k_Error_I_SetPortSettings:     *p_pDescription = "Error configuring port settings"; return TRUE;
        case k_Error_I_SetPortMode:         *p_pDescription = "Error configuring port mode"; return TRUE;
    }

    if(GetErrorDescription_I_RS232(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_I_CAN(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_I_USB(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_I_HID(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_I_Socket(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_I_TwinCAT(p_ulErrorCode, p_pDescription)) return TRUE;

    return FALSE;
}

BOOL CErrorHandling::GetErrorDescription_PS(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    if(GetErrorDescription_PS_MaxonSerialV1(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_PS_CANopen(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_PS_InfoteamSerial(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_PS_MaxonSerialV2(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_PS_EtherCAT(p_ulErrorCode, p_pDescription)) return TRUE;

    return FALSE;
}

BOOL CErrorHandling::GetErrorDescription_DCS(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    if(GetErrorDescription_DCS_DeviceEpos(p_ulErrorCode, p_pDescription)) return TRUE;
    if(GetErrorDescription_DCS_OldDeviceEpos(p_ulErrorCode, p_pDescription)) return TRUE;

    return FALSE;
}

BOOL CErrorHandling::GetErrorDescription_VCS(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    if(GetErrorDescription_VCS_VirtualDevice(p_ulErrorCode, p_pDescription)) return TRUE;

    return FALSE;
}

BOOL CErrorHandling::GetErrorDescription_GeneralError(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_NoError:                         *p_pDescription = _T(""); return TRUE;
        case k_Error_Internal:                  *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_INTERNAL); return TRUE;
        case k_Error_NullPointer:               *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_NULL_POINTER); return TRUE;
        case k_Error_HandleNotValid:            *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_HANDLE_NOT_VALID); return TRUE;
        case k_Error_BadVirtualDeviceName:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_VIRTUAL_DEVICE_NAME); return TRUE;
        case k_Error_BadDeviceName:             *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_DEVICE_NAME); return TRUE;
        case k_Error_BadProtocolStackName:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_PROTOCOL_STACK_NAME); return TRUE;
        case k_Error_BadInterfaceName:          *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_INTERFACE_NAME); return TRUE;
        case k_Error_BadPortName:               *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_PORT_NAME); return TRUE;
        case k_Error_LibraryNotLoaded:          *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_LIBRARY_NOT_FOUND); return TRUE;
        case k_Error_ExecutingCommand:          *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_EXEC_COMMAND); return TRUE;
        case k_Error_Timeout:                   *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_TIMEOUT); return TRUE;
        case k_Error_BadParameter:              *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_PARAM); return TRUE;
        case k_Error_CommandAbortedByUser:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_COMMAND_ABORTED); return TRUE;
        case ERROR_BUFFER_TOO_SMALL:            *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BUFFER_TOO_SMALL); return TRUE;
        case k_Error_NoCommunicationFound:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_NO_COMM_FOUND); return TRUE;
        case k_Error_FunctionNotSupported:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_FUNCTION_NOT_SUPPORTED); return TRUE;
        case ERROR_PARAMETER_ALREADY_USED:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_PARAM_USED); return TRUE;
        case ERROR_BAD_VIRTUAL_DEVICE_HANDLE:   *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_VIRTUAL_DEVICE_HANDLE); return TRUE;
        case k_Error_BadDeviceHandle:           *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_DEVICE_HANDLE); return TRUE;
        case k_Error_BadProtocolStackHandle:    *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_PROTOCOLSTACK_HANDLE); return TRUE;
        case k_Error_BadInterfaceHandle:        *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_INTERFACE_HANDLE); return TRUE;
        case k_Error_BadPortHandle:             *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_PORT_HANDLE); return TRUE;
        case k_Error_BadAddressParameter:       *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_ADDRESS_PARAM); return TRUE;
        case k_Error_BadVariableInfoFile:       *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_VARIABLE_INFO_FILE); return TRUE;
        case k_Error_VariableNameNotFound:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_VARIABLE_NAME_NOT_FOUND); return TRUE;
        case ERROR_BAD_DEVICE_STATE:            *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_CONTROLLER_STATE); return TRUE;
        case ERROR_BAD_FILE_CONTENT:            *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_BAD_FILE_CONTENT); return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_I_RS232(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_RS232_WriteData:           *p_pDescription = "Error writing RS232 Data"; return TRUE;
        case k_Error_RS232_ReadData:            *p_pDescription = "Error reading RS232 Data"; return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_I_USB(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_USB_WriteData:             *p_pDescription = "Error writing USB Data"; return TRUE;
        case k_Error_USB_ReadData:              *p_pDescription = "Error reading USB Data"; return TRUE;
        case k_Error_USB_Rescan:                *p_pDescription = "Error Rescanning USB Devices"; return TRUE;
        case k_Error_USB_Reload:                *p_pDescription = "Error Reloading USB Devices"; return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_I_HID(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_HID_WriteData:             *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_HID_WRITE_DATA); return TRUE;
        case k_Error_HID_ReadData:              *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_HID_READ_DATA); return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_I_CAN(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_CAN_ReceiveCanFrame:       *p_pDescription = "Error receiving CAN Frame"; return TRUE;
        case k_Error_CAN_TransmitCanFrame:      *p_pDescription = "Error transmitting CAN Frame"; return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_I_Socket(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    BOOL oResult(FALSE);

    switch(p_ulErrorCode)
    {
        case ERROR_SOCKET_WRITE_DATA:       *p_pDescription = "Error writing EtherCAT Frame"; oResult = TRUE; break;
        case ERROR_SOCKET_READ_DATA:        *p_pDescription = "Error reading EtherCAT Frame"; oResult = TRUE; break;
        default: ;
    }

    return oResult;
}

BOOL CErrorHandling::GetErrorDescription_I_TwinCAT(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    BOOL oResult(FALSE);

    switch(p_ulErrorCode)
    {
        case ERROR_TWINCAT_WRITE_DATA:      *p_pDescription = "Error writing TwinCAT-ADS Frame"; oResult = TRUE; break;
        case ERROR_TWINCAT_READ_DATA:       *p_pDescription = "Error reading TwinCAT-ADS Frame"; oResult = TRUE; break;
        default: ;
    }

    return oResult;
}

BOOL CErrorHandling::GetErrorDescription_PS_MaxonSerialV1(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_MaxonSerialV1_NegAckReceived:      *p_pDescription = "Maxon Serial V1 Protocol - Negative Acknowledge received"; return TRUE;
        case k_Error_MaxonSerialV1_BadCrcReceived:      *p_pDescription = "Maxon Serial V1 Protocol - Bad CRC received"; return TRUE;
        case k_Error_MaxonSerialV1_BadDataSizeReceived: *p_pDescription = "Maxon Serial V1 Protocol - Bad Data received"; return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_PS_InfoteamSerial(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_InfoteamSerial_BadAckReceived:         *p_pDescription = "Infoteam Serial Protocol - Bad Acknowledge received"; return TRUE;
        case k_Error_InfoteamSerial_RepAckReceived:         *p_pDescription = "Infoteam Serial Protocol - Repeat Acknowledge received"; return TRUE;
        case k_Error_InfoteamSerial_BadCrcReceived:         *p_pDescription = "Infoteam Serial Protocol - Bad CRC received"; return TRUE;
        case k_Error_InfoteamSerial_BadDataSizeReceived:    *p_pDescription = "Infoteam Serial Protocol - Bad Data received"; return TRUE;
        case k_Error_InfoteamSerial_ChunkSizeTooHigh:       *p_pDescription = "Infoteam Serial Protocol - Chunk Size too hihg"; return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_PS_MaxonSerialV2(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_MaxonSerialV2_Stuffing:                *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_MAXON_SERIAL_V2_STUFFING); return TRUE;
        case k_Error_MaxonSerialV2_Destuffing:              *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_MAXON_SERIAL_V2_DESTUFFING); return TRUE;
        case k_Error_MaxonSerialV2_BadCrcReceived:          *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_MAXON_SERIAL_V2_BAD_CRC_RECEIVED); return TRUE;
        case k_Error_MaxonSerialV2_BadDataSizeReceived:     *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_MAXON_SERIAL_V2_BAD_SIZE_RECEIVED); return TRUE;
        case k_Error_MaxonSerialV2_BadDataSizeWritten:      *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_MAXON_SERIAL_V2_BAD_SIZE_WRITTEN); return TRUE;
        case k_Error_MaxonSerialV2_SendFrame:               *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_MAXON_SERIAL_V2_SEND_FRAME); return TRUE;
        case k_Error_MaxonSerialV2_ReceiveFrame:            *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_MAXON_SERIAL_V2_RECEIVE_FRAME); return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_PS_EtherCAT(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    BOOL oResult(FALSE);

    switch(p_ulErrorCode)
    {
        case ERROR_ETHERCAT_SEND_FRAME:                     *p_pDescription = "EtherCAT - Failed sent frame"; oResult = TRUE; break;
        case ERROR_ETHERCAT_RECEIVE_FRAME:                  *p_pDescription = "EtherCAT - Failed receive frame"; oResult = TRUE; break;
		case ERROR_ETHERCAT_FRAME_NOT_RECEIVED:				*p_pDescription = "EtherCAT - Frame not received"; oResult = TRUE; break;
        default: ;
    }

    return oResult;
}

BOOL CErrorHandling::GetErrorDescription_PS_CANopen(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_CANopen_SdoReceiveFrameNotReceived:    *p_pDescription = "CANopen Protocol - SDO Response Frame not received"; return TRUE;
        case k_Error_CANopen_RequestedCanFrameNotReceived:  *p_pDescription = "CANopen Protocol - Requested Can Frame not received"; return TRUE;
        case k_Error_CANopen_CanFrameNotReceived:           *p_pDescription = "CANopen Protocol - Can Frame not received"; return TRUE;
        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_DCS_DeviceEpos(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case ERROR_DEVICE_EPOS_TOGGLE:                                      *p_pDescription = "Toggle Error"; return TRUE;
        case ERROR_DEVICE_EPOS_SDO_TIME_OUT:                                *p_pDescription = "SDO Protocol Timeout"; return TRUE;
        case ERROR_DEVICE_EPOS_CLIENT_SERVER:                               *p_pDescription = "Command Specifier not valid or unknown"; return TRUE;
        case ERROR_DEVICE_EPOS_INVALID_BLOCK_SIZE:                          *p_pDescription = "Invalid Block Size"; return TRUE;
        case ERROR_DEVICE_EPOS_INVALID_SEQUENCE:                            *p_pDescription = "Invalid Sequence Number"; return TRUE;
        case ERROR_DEVICE_EPOS_CRC_ERROR:                                   *p_pDescription = "Crc Error"; return TRUE;
        case ERROR_DEVICE_EPOS_OUT_OF_MEMORY:                               *p_pDescription = "Out of Memory"; return TRUE;
        case ERROR_DEVICE_EPOS_ACCESS:                                      *p_pDescription = "Access denied"; return TRUE;
        case ERROR_DEVICE_EPOS_WRITE_ONLY:                                  *p_pDescription = "Object is WriteOnly"; return TRUE;
        case ERROR_DEVICE_EPOS_READ_ONLY:                                   *p_pDescription = "Object is ReadOnly"; return TRUE;
        case ERROR_DEVICE_EPOS_OBJECT_NOT_EXIST:                            *p_pDescription = "Object does not exist"; return TRUE;
        case ERROR_DEVICE_EPOS_PDO_MAPPING:                                 *p_pDescription = "Error PDO Mapping"; return TRUE;
        case ERROR_DEVICE_EPOS_PDO_LENGTH:                                  *p_pDescription = "Error PDO Length"; return TRUE;
        case ERROR_DEVICE_EPOS_GENERAL_PARAMETER:                           *p_pDescription = "Parameter Error"; return TRUE;
        case ERROR_DEVICE_EPOS_GENERAL_INTERN_INCOMPATIBILITY :             *p_pDescription = "Internal Incompatibility"; return TRUE;
        case ERROR_DEVICE_EPOS_HARDWARE:                                    *p_pDescription = "Hardware Error"; return TRUE;
        case ERROR_DEVICE_EPOS_SERVICE_PARAMETER:                           *p_pDescription = "Service Parameter Error"; return TRUE;
        case ERROR_DEVICE_EPOS_SERVICE_PARAMETER_TO_HIGH:                   *p_pDescription = "Service Parameter is too high"; return TRUE;
        case ERROR_DEVICE_EPOS_SERVICE_PARAMETER_TO_LOW:                    *p_pDescription = "Service Parameter is too low"; return TRUE;
        case ERROR_DEVICE_EPOS_SUB_OBJECT:                                  *p_pDescription = "SubObject does not exist"; return TRUE;
        case ERROR_DEVICE_EPOS_VALUE_RANGE:                                 *p_pDescription = "Value Range Error"; return TRUE;
        case ERROR_DEVICE_EPOS_VALUE_TO_HIGH:                               *p_pDescription = "Value is too high"; return TRUE;
        case ERROR_DEVICE_EPOS_VALUE_TO_LOW:                                *p_pDescription = "Value is too low"; return TRUE;
        case ERROR_DEVICE_EPOS_MAX_LESS_MIN:                                *p_pDescription = "Max. Value is less than min. Value"; return TRUE;
        case ERROR_DEVICE_EPOS_GENERAL:                                     *p_pDescription = "General Error"; return TRUE;
        case ERROR_DEVICE_EPOS_TRANSFER_OR_STORE:                           *p_pDescription = "Error transfering or storing data"; return TRUE;
        case ERROR_DEVICE_EPOS_LOCAL_CONTROL:                               *p_pDescription = "Error Local Control"; return TRUE;
        case ERROR_DEVICE_EPOS_WRONG_DEVICE:                                *p_pDescription = "Wrong Device State"; return TRUE;

        case ERROR_DEVIC_EEPOS_UNKNOWN_NETWORK_ID:                          *p_pDescription = "Network Id is unknown"; return TRUE;
        case ERROR_DEVIC_EEPOS_UNKNOWN_NODE_ID:                             *p_pDescription = "Node Id is unknown"; return TRUE;

        case ERROR_DEVICE_EPOS_CAN_ID:                                      *p_pDescription = "Wrong CAN Id"; return TRUE;
        case ERROR_DEVICE_EPOS_SERVICE_MODE:                                *p_pDescription = "Service Mode is needed for this operation"; return TRUE;
        case ERROR_DEVICE_EPOS_PASSWORD:                                    *p_pDescription = "Wrong Password"; return TRUE;
        case ERROR_DEVICE_EPOS_ILLEGAL_COMMAND:                             *p_pDescription = "Illegal Command"; return TRUE;
        case ERROR_DEVICE_EPOS_WRONG_NMT_STATE:                             *p_pDescription = "Wrong NMT State"; return TRUE;
        case ERROR_DEVICE_ESAM2_SEGMENTED_TRANSFER_REQUIRED_INIT_DONE:      *p_pDescription = "Segmented Transfer Required"; return TRUE;
        case ERROR_DEVICE_ESAM2_DATAHISTORY_BUFFER_OVERRUN:                 *p_pDescription = "Data History Buffer Overrun: Please reduce sampling rate or/and number of variables"; return TRUE;

        case ERROR_DEVICE_EPOS_COMMUNICATION_ABORTED:                       *p_pDescription = "Communication Aborted"; return TRUE;
        case ERROR_DEVICE_EPOS_COMMUNICATION_OVERFLOW:                      *p_pDescription = "Communication Buffer Overflow"; return TRUE;
        case ERROR_DEVICE_EPOS_SEGMENTED_COMMUNICATION_ERROR:               *p_pDescription = "Segmented Transfer Error"; return TRUE;
        case ERROR_DEVICE_EPOS_WRONG_AXIS_NUMBER:                           *p_pDescription = "Wrong Axis Number"; return TRUE;
        case ERROR_DEVICE_EPOS_WRONG_CAN_DEVICE:                            *p_pDescription = "Wrong Node Id"; return TRUE;
        case ERROR_DEVICE_EPOS_WRONG_CAN_PORT:                              *p_pDescription = "Can Port is not valid"; return TRUE;
        case ERROR_DEVICE_EPOS_WRONG_CALLING_PARAMETER:                     *p_pDescription = "Calling Parameter Error"; return TRUE;
        case ERROR_DEVICE_EPOS_GENERAL_COMMUNICATION_ERROR:                 *p_pDescription = "General Communication Error"; return TRUE;
        case ERROR_DEVICE_EPOS_TIMEOUT:                                     *p_pDescription = "Communication Timeout"; return TRUE;

        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_DCS_OldDeviceEpos(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case k_Error_OldDeviceEpos_ToggleErr:               *p_pDescription = "Toggle Error"; return TRUE;
        case k_Error_OldDeviceEpos_SdoTimeOut:              *p_pDescription = "Timeout Error"; return TRUE;
        case k_Error_OldDeviceEpos_CcsErr:                  *p_pDescription = "Command Specifier Error"; return TRUE;
        case k_Error_OldDeviceEpos_InvalidBlockSize:        *p_pDescription = "Invalid Block Size"; return TRUE;
        case k_Error_OldDeviceEpos_InvalidSequenceNo:       *p_pDescription = "Invalid Sequence Number"; return TRUE;
        case k_Error_OldDeviceEpos_CrcErr:                  *p_pDescription = "Crc Error"; return TRUE;
        case k_Error_OldDeviceEpos_OutOfMemory:             *p_pDescription = "Out of Memory"; return TRUE;
        case k_Error_OldDeviceEpos_AccessErr:               *p_pDescription = "Access Error"; return TRUE;
        case k_Error_OldDeviceEpos_WriteOnly:               *p_pDescription = "Object is WriteOnly"; return TRUE;
        case k_Error_OldDeviceEpos_ReadOnly:                *p_pDescription = "Object is ReadOnly"; return TRUE;
        case k_Error_OldDeviceEpos_ObjNotExist:             *p_pDescription = "Object does not exist"; return TRUE;
        case k_Error_OldDeviceEpos_NoPdoObj:                *p_pDescription = "PDO Number Error"; return TRUE;
        case k_Error_OldDeviceEpos_PdoLengthErr:            *p_pDescription = "PDO Length Error"; return TRUE;
        case k_Error_OldDeviceEpos_GeneralParamErr:         *p_pDescription = "Parameter Error"; return TRUE;
        case k_Error_OldDeviceEpos_GeneralInternIncomp:     *p_pDescription = "Internal Incomp Error"; return TRUE;
        case k_Error_OldDeviceEpos_HWErr:                   *p_pDescription = "Hardware Error"; return TRUE;
        case k_Error_OldDeviceEpos_ServiceParamErr:         *p_pDescription = "Service Parameter Error"; return TRUE;
        case k_Error_OldDeviceEpos_ServiceParamTooHigh:     *p_pDescription = "Service Parameter too High"; return TRUE;
        case k_Error_OldDeviceEpos_ServiceParamTooLow:      *p_pDescription = "Service Parameter too Low"; return TRUE;
        case k_Error_OldDeviceEpos_SubIndxErr:              *p_pDescription = "SubIndex Error"; return TRUE;
        case k_Error_OldDeviceEpos_ValueRange:              *p_pDescription = "Value Range Error"; return TRUE;
        case k_Error_OldDeviceEpos_ValTooHigh:              *p_pDescription = "Value too High"; return TRUE;
        case k_Error_OldDeviceEpos_ValTooLow:               *p_pDescription = "Value too Low"; return TRUE;
        case k_Error_OldDeviceEpos_MaxLessMin:              *p_pDescription = "Max. less Min."; return TRUE;
        case k_Error_OldDeviceEpos_GeneralErr:              *p_pDescription = "General Error"; return TRUE;
        case k_Error_OldDeviceEpos_TransfOrStoreErr:        *p_pDescription = "Transfer or Store Error"; return TRUE;
        case k_Error_OldDeviceEpos_LocalControlErr:         *p_pDescription = "Local Control Error"; return TRUE;
        case k_Error_OldDeviceEpos_WrongDeviceState:        *p_pDescription = "Wrong Device Control State"; return TRUE;
        case k_Error_OldDeviceEpos_WrongNmtState:           *p_pDescription = "Wrong Nmt State"; return TRUE;
        case k_Error_OldDeviceEpos_IllegalCommand:          *p_pDescription = "Illegal Command"; return TRUE;
        case k_Error_OldDeviceEpos_ErrorPassword:           *p_pDescription = "Error Password"; return TRUE;
        case k_Error_OldDeviceEpos_ErrorLength:             *p_pDescription = "Error Length"; return TRUE;
        case k_Error_OldDeviceEpos_ErrorService:            *p_pDescription = "Error Service"; return TRUE;
        case k_Error_OldDeviceEpos_NoMoreSegments:          *p_pDescription = "No more segments"; return TRUE;
        case k_Error_OldDeviceEpos_SdoAbort:                *p_pDescription = "Sdo Abort"; return TRUE;
        case k_Error_OldDeviceEpos_ErrorCanId:              *p_pDescription = "Wrong CAN ID"; return TRUE;
        case k_Error_OldDeviceEpos_ErrorAdress:             *p_pDescription = "Error Address"; return TRUE;
        case k_Error_OldDeviceEpos_SubIndexDoesNotExist:    *p_pDescription = "SubIndex does not exist"; return TRUE;

        default: return FALSE;
    }
}

BOOL CErrorHandling::GetErrorDescription_VCS_VirtualDevice(DWORD p_ulErrorCode, CStdString *p_pDescription)
{
    switch(p_ulErrorCode)
    {
        case ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED: *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_VIRTUAL_DEVICE_BAD_SIZE_EXPECTED); return TRUE;
        case ERROR_VIRTUAL_DEVICE_HOMING_ERROR:             *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_VIRTUAL_DEVICE_HOMING_ERROR); return TRUE;
		case ERROR_VIRTUAL_DEVICE_BAD_SII_CATEGORY:         *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_VIRTUAL_DEVICE_BAD_SII_CATEGORY); return TRUE;
		case ERROR_VIRTUAL_DEVICE_BAD_SII_STRING_TABLE:     *p_pDescription = _LoadResourceCStdString(IDS_COMM_X_ERROR_VIRTUAL_DEVICE_BAD_SII_STRING_TABLE); return TRUE;

        default: return FALSE;
    }
}

BOOL CErrorHandling::InitErrorProducer(CErrorProducer *p_pErrorProducer)
{
    BOOL oResult(FALSE);
    DeleteErrorProducer();

    if(p_pErrorProducer)
    {
        m_pErrorProducer = p_pErrorProducer->Clone();

        oResult = TRUE;
    }

    return oResult;
}

BOOL CErrorHandling::GetErrorProducerInfos(ELayer& p_rLayer, CStdString& p_rClassName, CStdString& p_rCommandName)
{
    BOOL oResult = FALSE;

    if(m_pErrorProducer)
    {
        p_rLayer = m_pErrorProducer->GetLayer();
        p_rClassName = m_pErrorProducer->GetClassName();
        p_rCommandName = m_pErrorProducer->GetCommandName();
        oResult = TRUE;
    }

    return oResult;
}

void CErrorHandling::GetError(DWORD p_ulErrorCode, CErrorInfo* p_pErrorInfo)
{
    if(p_pErrorInfo)
    {
        p_pErrorInfo->Init(p_ulErrorCode, m_pErrorProducer);
    }
}
