// ErrorCode_Defintions.h: Schnittstelle für die Klasse CErrorCode_Defintions.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ErrorCode_Defintions_H__DD421E4C_9091_4F06_9358_CB291C3646E7__INCLUDED_)
#define AFX_ErrorCode_Defintions_H__DD421E4C_9091_4F06_9358_CB291C3646E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***********************************************************************************/
//0x1xxxxxxx    Allgemeine Fehler

//0x2xxxxxxx    Interface Layer
//0x21xxxxxx    Interface RS232
//0x22xxxxxx    Interface CAN
//0x23xxxxxx    Interface USB
//0x24xxxxxx    Interface HID
//0x25xxxxxx    Interface Socket
//0x26xxxxxx    Interface TwinCAT

//0x3xxxxxxx    ProtocolStack Layer
//0x31xxxxxx    ProtocolStack EposRS232
//0x32xxxxxx    ProtocolStack CANopen

//0x4xxxxxxx    DeviceCommandSet Layer
//0x41xxxxxx    Device Epos (+ Fehler definiert in Epos Firmware 0x0xxxxxxx)

//0x5xxxxxxx    VirtualCommandSet Layer
//0x51xxxxxx    VirtualDevice
/************************************************************************************/

#define k_NoError                                                       0x00000000

//Allgemeine Fehler (0x1xxxxxxx)
#define k_Error_Internal                                                0x10000001
#define k_Error_NullPointer                                             0x10000002
#define k_Error_HandleNotValid                                          0x10000003
#define k_Error_BadVirtualDeviceName                                    0x10000004
#define k_Error_BadDeviceName                                           0x10000005
#define k_Error_BadProtocolStackName                                    0x10000006
#define k_Error_BadInterfaceName                                        0x10000007
#define k_Error_BadPortName                                             0x10000008
#define k_Error_LibraryNotLoaded                                        0x10000009
#define k_Error_ExecutingCommand                                        0x1000000A
#define k_Error_Timeout                                                 0x1000000B
#define k_Error_BadParameter                                            0x1000000C
#define k_Error_CommandAbortedByUser                                    0x1000000D
#define ERROR_BUFFER_TOO_SMALL                                          0x1000000E
#define k_Error_NoCommunicationFound                                    0x1000000F
#define k_Error_FunctionNotSupported                                    0x10000010
#define ERROR_PARAMETER_ALREADY_USED                                    0x10000011
#define ERROR_BAD_VIRTUAL_DEVICE_HANDLE                                 0x10000012
#define k_Error_BadDeviceHandle                                         0x10000013
#define k_Error_BadProtocolStackHandle                                  0x10000014
#define k_Error_BadInterfaceHandle                                      0x10000015
#define k_Error_BadPortHandle                                           0x10000016
#define k_Error_BadAddressParameter                                     0x10000017
#define k_Error_BadVariableInfoFile                                     0x10000018
#define k_Error_VariableNameNotFound                                    0x10000019
#define ERROR_BAD_DEVICE_STATE                                          0x10000020
#define ERROR_BAD_FILE_CONTENT                                          0x10000021

//InterfaceLayer (0x2xxxxxxx)
#define k_Error_I_OpeningInterface                                      0x20000001
#define k_Error_I_ClosingInterface                                      0x20000002
#define k_Error_I_InterfaceNotOpen                                      0x20000003
#define k_Error_I_OpeningPort                                           0x20000004
#define k_Error_I_ClosingPort                                           0x20000005
#define k_Error_I_PortNotOpen                                           0x20000006
#define k_Error_I_ResetPort                                             0x20000007
#define k_Error_I_SetPortSettings                                       0x20000008
#define k_Error_I_SetPortMode                                           0x20000009
//Interface RS232 (0x21xxxxxx)
#define k_Error_RS232_WriteData                                         0x21000001
#define k_Error_RS232_ReadData                                          0x21000002
//Interface CAN (0x22xxxxxx)
#define k_Error_CAN_ReceiveCanFrame                                     0x22000001
#define k_Error_CAN_TransmitCanFrame                                    0x22000002
//Interface USB (0x23xxxxxx)
#define k_Error_USB_WriteData                                           0x23000001
#define k_Error_USB_ReadData                                            0x23000002
#define k_Error_USB_Rescan                                              0x23000003
#define k_Error_USB_Reload                                              0x23000004
//Interface HID (0x24xxxxxx)
#define k_Error_HID_WriteData                                           0x24000001
#define k_Error_HID_ReadData                                            0x24000002
//Interface Socket (0x25xxxxxx)
#define ERROR_SOCKET_WRITE_DATA                                         0x25000001
#define ERROR_SOCKET_READ_DATA                                          0x25000002
//Interface Socket (0x26xxxxxx)
#define ERROR_TWINCAT_WRITE_DATA                                        0x26000001
#define ERROR_TWINCAT_READ_DATA                                         0x26000002
//Interface IPC (0x27xxxxxx)
const DWORD ERROR_IPC_WRITE_DATA                                        = 0x27000001;
const DWORD ERROR_IPC_READ_DATA                                         = 0x27000002;

//ProtocolStackLayer (0x3xxxxxxx)
//ProtocolStack MaxonSerialV1 (0x31xxxxxx)
#define k_Error_MaxonSerialV1_NegAckReceived                            0x31000001
#define k_Error_MaxonSerialV1_BadCrcReceived                            0x31000002
#define k_Error_MaxonSerialV1_BadDataSizeReceived                       0x31000003
//ProtocolStack CANopen (0x32xxxxxx)
#define k_Error_CANopen_SdoReceiveFrameNotReceived                      0x32000001
#define k_Error_CANopen_RequestedCanFrameNotReceived                    0x32000002
#define k_Error_CANopen_CanFrameNotReceived                             0x32000003
//ProtocolStack EsamRS232 (0x33xxxxxx)
#define k_Error_InfoteamSerial_BadAckReceived                           0x33000001
#define k_Error_InfoteamSerial_RepAckReceived                           0x33000002
#define k_Error_InfoteamSerial_BadCrcReceived                           0x33000003
#define k_Error_InfoteamSerial_BadDataSizeReceived                      0x33000004
#define k_Error_InfoteamSerial_ChunkSizeTooHigh                         0x33000005
//ProtocolStack Epos2USB (0x34xxxxxx)
#define k_Error_MaxonSerialV2_Stuffing                                  0x34000001
#define k_Error_MaxonSerialV2_Destuffing                                0x34000002
#define k_Error_MaxonSerialV2_BadCrcReceived                            0x34000003
#define k_Error_MaxonSerialV2_BadDataSizeReceived                       0x34000004
#define k_Error_MaxonSerialV2_BadDataSizeWritten                        0x34000005
#define k_Error_MaxonSerialV2_SendFrame                                 0x34000006
#define k_Error_MaxonSerialV2_ReceiveFrame                              0x34000007
//ProtocolStack EtherCAT (0x35xxxxxx)
#define ERROR_ETHERCAT_SEND_FRAME                                       0x35000001
#define ERROR_ETHERCAT_RECEIVE_FRAME                                    0x35000002
#define ERROR_ETHERCAT_FRAME_NOT_RECEIVED                               0x35000003

//DeviceCommandSet Layer (0x4xxxxxxx)
//DeviceEpos (0x41xxxxxx + Fehler definiert in Epos Firmware 0x0xxxxxxx)
#define ERROR_DEVICE_EPOS_TOGGLE                                        0x05030000
#define ERROR_DEVICE_EPOS_SDO_TIME_OUT                                  0x05040000
#define ERROR_DEVICE_EPOS_CLIENT_SERVER                                 0x05040001
#define ERROR_DEVICE_EPOS_INVALID_BLOCK_SIZE                            0x05040002
#define ERROR_DEVICE_EPOS_INVALID_SEQUENCE                              0x05040003
#define ERROR_DEVICE_EPOS_CRC_ERROR                                     0x05040004
#define ERROR_DEVICE_EPOS_OUT_OF_MEMORY                                 0x05040005
#define ERROR_DEVICE_EPOS_ACCESS                                        0x06010000
#define ERROR_DEVICE_EPOS_WRITE_ONLY                                    0x06010001
#define ERROR_DEVICE_EPOS_READ_ONLY                                     0x06010002
#define ERROR_DEVICE_EPOS_OBJECT_NOT_EXIST                              0x06020000
#define ERROR_DEVICE_EPOS_PDO_MAPPING                                   0x06040041
#define ERROR_DEVICE_EPOS_PDO_LENGTH                                    0x06040042
#define ERROR_DEVICE_EPOS_GENERAL_PARAMETER                             0x06040043
#define ERROR_DEVICE_EPOS_GENERAL_INTERN_INCOMPATIBILITY                0x06040047
#define ERROR_DEVICE_EPOS_HARDWARE                                      0x06060000
#define ERROR_DEVICE_EPOS_SERVICE_PARAMETER                             0x06070010
#define ERROR_DEVICE_EPOS_SERVICE_PARAMETER_TO_HIGH                     0x06070012
#define ERROR_DEVICE_EPOS_SERVICE_PARAMETER_TO_LOW                      0x06070013
#define ERROR_DEVICE_EPOS_SUB_OBJECT                                    0x06090011
#define ERROR_DEVICE_EPOS_VALUE_RANGE                                   0x06090030
#define ERROR_DEVICE_EPOS_VALUE_TO_HIGH                                 0x06090031
#define ERROR_DEVICE_EPOS_VALUE_TO_LOW                                  0x06090032
#define ERROR_DEVICE_EPOS_MAX_LESS_MIN                                  0x06090036
#define ERROR_DEVICE_EPOS_GENERAL                                       0x08000000
#define ERROR_DEVICE_EPOS_TRANSFER_OR_STORE                             0x08000020
#define ERROR_DEVICE_EPOS_LOCAL_CONTROL                                 0x08000021
#define ERROR_DEVICE_EPOS_WRONG_DEVICE                                  0x08000022

#define ERROR_DEVIC_EEPOS_UNKNOWN_NETWORK_ID                            0x0A000001
#define ERROR_DEVIC_EEPOS_UNKNOWN_NODE_ID                               0x0A000002

#define ERROR_DEVICE_EPOS_CAN_ID                                        0x0F00FFB9
#define ERROR_DEVICE_EPOS_SERVICE_MODE                                  0x0F00FFBC
#define ERROR_DEVICE_EPOS_PASSWORD                                      0x0F00FFBE
#define ERROR_DEVICE_EPOS_ILLEGAL_COMMAND                               0x0F00FFBF
#define ERROR_DEVICE_EPOS_WRONG_NMT_STATE                               0x0F00FFC0

#define ERROR_DEVICE_ESAM2_SEGMENTED_TRANSFER_REQUIRED_INIT_DONE        0x0F00FFC2
#define ERROR_DEVICE_ESAM2_DATAHISTORY_BUFFER_OVERRUN                   0x0F010072

#define ERROR_DEVICE_EPOS_COMMUNICATION_ABORTED                         0x0FFFFFF1
#define ERROR_DEVICE_EPOS_COMMUNICATION_OVERFLOW                        0x0FFFFFF2
#define ERROR_DEVICE_EPOS_SEGMENTED_COMMUNICATION_ERROR                 0x0FFFFFF9
#define ERROR_DEVICE_EPOS_WRONG_AXIS_NUMBER                             0x0FFFFFFA
#define ERROR_DEVICE_EPOS_WRONG_CAN_DEVICE                              0x0FFFFFFB
#define ERROR_DEVICE_EPOS_WRONG_CAN_PORT                                0x0FFFFFFC
#define ERROR_DEVICE_EPOS_WRONG_CALLING_PARAMETER                       0x0FFFFFFD
#define ERROR_DEVICE_EPOS_GENERAL_COMMUNICATION_ERROR                   0x0FFFFFFE
#define ERROR_DEVICE_EPOS_TIMEOUT                                       0x0FFFFFFF

//OldDeviceEpos (0x41xxxxxx + Fehler definiert in Epos Firmware 0x0xxxxxxx)
#define k_Error_OldDeviceEpos_DeviceOldMask                             0x0F000000
#define k_Error_OldDeviceEpos_ToggleErr                                 0x0F00FFFF
#define k_Error_OldDeviceEpos_SdoTimeOut                                0x0F00FFFE
#define k_Error_OldDeviceEpos_CcsErr                                    0x0F00FFFD
#define k_Error_OldDeviceEpos_InvalidBlockSize                          0x0F00FFFC
#define k_Error_OldDeviceEpos_InvalidSequenceNo                         0x0F00FFFB
#define k_Error_OldDeviceEpos_CrcErr                                    0x0F00FFFA
#define k_Error_OldDeviceEpos_OutOfMemory                               0x0F00FFF9
#define k_Error_OldDeviceEpos_AccessErr                                 0x0F00FFF8
#define k_Error_OldDeviceEpos_WriteOnly                                 0x0F00FFF7
#define k_Error_OldDeviceEpos_ReadOnly                                  0x0F00FFF6
#define k_Error_OldDeviceEpos_ObjNotExist                               0x0F00FFF5
#define k_Error_OldDeviceEpos_NoPdoObj                                  0x0F00FFF4
#define k_Error_OldDeviceEpos_PdoLengthErr                              0x0F00FFF3
#define k_Error_OldDeviceEpos_GeneralParamErr                           0x0F00FFF2
#define k_Error_OldDeviceEpos_GeneralInternIncomp                       0x0F00FFF1
#define k_Error_OldDeviceEpos_HWErr                                     0x0F00FFF0
#define k_Error_OldDeviceEpos_ServiceParamErr                           0x0F00FFEF
#define k_Error_OldDeviceEpos_ServiceParamTooHigh                       0x0F00FFEE
#define k_Error_OldDeviceEpos_ServiceParamTooLow                        0x0F00FFED
#define k_Error_OldDeviceEpos_SubIndxErr                                0x0F00FFEC
#define k_Error_OldDeviceEpos_ValueRange                                0x0F00FFEB
#define k_Error_OldDeviceEpos_ValTooHigh                                0x0F00FFEA
#define k_Error_OldDeviceEpos_ValTooLow                                 0x0F00FFE9
#define k_Error_OldDeviceEpos_MaxLessMin                                0x0F00FFE8
#define k_Error_OldDeviceEpos_GeneralErr                                0x0F00FFE7
#define k_Error_OldDeviceEpos_TransfOrStoreErr                          0x0F00FFE6
#define k_Error_OldDeviceEpos_LocalControlErr                           0x0F00FFE5
#define k_Error_OldDeviceEpos_WrongDeviceState                          0x0F00FFE4
#define k_Error_OldDeviceEpos_WrongNmtState                             0x0F00FFC0
#define k_Error_OldDeviceEpos_IllegalCommand                            0x0F00FFBF
#define k_Error_OldDeviceEpos_ErrorPassword                             0x0F00FFBE
#define k_Error_OldDeviceEpos_ErrorLength                               0x0F00FFBD
#define k_Error_OldDeviceEpos_ErrorService                              0x0F00FFBC

#define k_Error_OldDeviceEpos_NoMoreSegments                            0x0F00FFBB
#define k_Error_OldDeviceEpos_SdoAbort                                  0x0F00FFBA
#define k_Error_OldDeviceEpos_ErrorCanId                                0x0F00FFB9
#define k_Error_OldDeviceEpos_ErrorAdress                               0x0F00FFB8
#define k_Error_OldDeviceEpos_SubIndexDoesNotExist                      0x0F00FFB7

//VirtualCommandSetLayer (0x5xxxxxxx)
//VirtualDevice (0x51xxxxxx)
#define ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED                   0x51000001
#define ERROR_VIRTUAL_DEVICE_HOMING_ERROR								0x51000002
#define ERROR_VIRTUAL_DEVICE_BAD_SII_CATEGORY			                0x51000003
#define ERROR_VIRTUAL_DEVICE_BAD_SII_STRING_TABLE		                0x51000004

#endif // !defined(AFX_ErrorCode_Defintions_H__DD421E4C_9091_4F06_9358_CB291C3646E7__INCLUDED_)
