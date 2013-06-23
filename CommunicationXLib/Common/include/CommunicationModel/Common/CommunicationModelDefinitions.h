/*********************************************************************

    HEADER FILE:        CommunicationModelDefinitions.h

**********************************************************************

    COPYRIGHT (C)       2003 maxon motor control

    Project:            EPOS Fremdsystemeinbindung
    Written by:         Romero Jorge
    System:             Visual C++
    File:               CommunicationModelDefinitions.h
    Date:               04.08.2003

**********************************************************************

    Task:

*********************************************************************/

#if !defined(CommunicationModelDefinitions_04082003)
#define CommunicationModelDefinitions_04082003

#include <MmcTypeDefinition.h>

//Tracing definitions
//#define _Logging_EtherCAT_Timing

//Addressing
const BYTE          NODE_ID_GATEWAY                             = 0x00;
const BYTE          NODE_ID_DEFAULT                             = 0x01;
const BYTE          NODE_ID_UNDEFINED                           = 0xFF;
const CStdString    NODE_ID_UNDEFINED_TEXT                      = _T("Invalid");

const DWORD         VENDOR_ID_MAXON                             = 0x000000FB;
const BYTE          STATION_ID_UNDEFINED                        = 0x00;

//Transfer rate
const DWORD         TRANSFER_RATE_DEFAULT                       = 1000000;
const DWORD         TRANSFER_RATE_AUTO                          = (DWORD)-1;
const CStdString    TRANSFER_RATE_AUTO_TEXT                     = _T("Auto Bitrate");

//Device addressing
enum EAddressingMode
{
    AM_UNKNOWN_ADDRESSING,  //AddressingMode is statically defined
    AM_DIRECT_ADDRESSING,   //Addressing via NodeId 0
    AM_NODE_ID_ADDRESSING   //Addressing via configured NodeId
};

enum ELayer
{
    NO_LAYER                    = 0,
    INTERFACE_LAYER             = 1,
    PROTOCOL_STACK_LAYER        = 2,
    DEVICE_COMMAND_SET_LAYER    = 3,
    VIRTUAL_COMMAND_SET_LAYER   = 4
};

enum EParameterType
{
    PT_PORT                     = 0,
    PT_INTERFACE                = 1,
    PT_PROTOCOL_STACK           = 2,
    PT_DEVICE                   = 3,
    PT_VIRTUAL_DEVICE           = 4
};

enum ECommandStatus
{
    CMD_IDLE                    = 0,
    CMD_SUCCESSFUL              = 1,
    CMD_FAILED                  = 2,
};

//IXXAT interface mode definitions
#define k_IXXAT_V2      0x0002
#define k_IXXAT_V3      0x0003

//FTDI USB definitions
const TCHAR REGISTRY_MAXON[]                    = _T("Software\\maxon motor ag");
const TCHAR REGISTRY_FTDI_USB_HANDLING[]        = _T("Ftd2xxDeviceInfoHandling");
const TCHAR REGISTRY_HID_USB_HANDLING[]         = _T("HidDeviceInfoHandling");
const TCHAR REGISTRY_MCP_USB_HANDLING[]         = _T("McpDeviceInfoHandling");

//EtherCAT command types
typedef enum ECommandType
{
    CT_NOP          = 0,
    CT_APRD         = 1,  //Auto Increment Physical Read
    CT_APWR         = 2,  //Auto Increment Physical Write
    CT_APRW         = 3,  //Auto Increment Physical Read/Write
    CT_FPRD         = 4,  //Fix Address Physical Read
    CT_FPWR         = 5,  //Fix Address Physical Write
    CT_FPRW         = 6,  //Fix Address Physical Read/Write
    CT_BRD          = 7,  //Broadcast Read
    CT_BWR          = 8,  //Broadcast Write
    CT_BRW          = 9,  //Broadcast read/write
    CT_LRD          = 10, //Logical Read
    CT_LWR          = 11, //Logical Write
    CT_LRW          = 12, //Logical Read/Write
    CT_ARMW         = 13, //Auto Increment Read Multiple Write
    CT_EXT          = 255,
};

#ifdef WINVER
typedef void (__stdcall *T_JournalCallback)(HANDLE, TCHAR*, int);
#endif

//Definitionen Layer Strings
const TCHAR VIRTUAL_COMMAND_SET_LAYER_STRING[]   = _T("Application Layer");
const TCHAR COMMAND_SET_LAYER_STRING[]           = _T("Device Layer");
const TCHAR PROTOCOL_STACK_LAYER_STRING[]        = _T("Protocol Layer");
const TCHAR INTERFACE_LAYER_STRING[]             = _T("Interface Layer");

//Definition VirtualDevices
const TCHAR VIRTUAL_DEVICE_DRIVE[]               = _T("DRIVE");
const TCHAR VIRTUAL_DEVICE_DRIVE2[]              = _T("DRIVE 2");
const TCHAR VIRTUAL_DEVICE_DRIVE3[]              = _T("DRIVE 3");
const TCHAR VIRTUAL_DEVICE_DRIVE4[]              = _T("DRIVE 4");
const TCHAR VIRTUAL_DEVICE_PLC[]                 = _T("PLC");
const TCHAR VIRTUAL_DEVICE_PLC2[]                = _T("PLC 2");
const TCHAR VIRTUAL_DEVICE_SERVO[]               = _T("SERVO");

//Definition Devices
const TCHAR DEVICE_EPOS[]                        = _T("EPOS");
const TCHAR DEVICE_ESAM[]                        = _T("EPOS P");
const TCHAR DEVICE_EPOS2[]                       = _T("EPOS2");
const TCHAR DEVICE_EPOS3[]                       = _T("EPOS3");
const TCHAR DEVICE_ESAM2[]                       = _T("EPOS2 P");
const TCHAR DEVICE_ESCON[]                       = _T("ESCON");
const TCHAR DEVICE_MAXPOS[]                      = _T("MAXPOS");

//Defintion ProtocolStacks
const TCHAR PROTOCOL_CAN_OPEN[]                  = _T("CANopen");
const TCHAR PROTOCOL_MAXON_SERIAL_V1[]           = _T("MAXON_RS232");
const TCHAR PROTOCOL_MAXON_SERIAL_V2[]           = _T("MAXON SERIAL V2");
const TCHAR PROTOCOL_INFOTEAM_SERIAL[]           = _T("INFOTEAM SERIAL");
const TCHAR PROTOCOL_ETHERCAT_COE[]              = _T("EtherCAT-CoE");

//Old Definitions ProtocolStacks (only used for compatibility)
const TCHAR PROTOCOL_EPOS2_USB[]                 = _T("EPOS2_USB");

//Definition Interfaces
const TCHAR INTERFACE_NI[]                       = _T("NI");
const TCHAR INTERFACE_IXXAT[]                    = _T("IXXAT");
const TCHAR INTERFACE_VECTOR[]                   = _T("Vector");
const TCHAR INTERFACE_RS232[]                    = _T("RS232");
const TCHAR INTERFACE_USB[]                      = _T("USB");
const TCHAR INTERFACE_HID[]                      = _T("HID");
const TCHAR INTERFACE_ETH[]                      = _T("ETH");
const TCHAR INTERFACE_SOCKET[]                   = _T("Socket");
const TCHAR INTERFACE_TWINCAT[]                  = _T("TwinCAT-ADS");
const TCHAR INTERFACE_IPC[]                      = _T("IPC");
const TCHAR INTERFACE_MCP[]                      = _T("MCP");

//Definition InterfacePorts
const TCHAR INTERFACE_PORT_NI[]                  = _T("NI");
const TCHAR INTERFACE_PORT_IXXAT[]               = _T("IXXAT");
const TCHAR INTERFACE_PORT_VECTOR[]              = _T("Vector");
const TCHAR INTERFACE_PORT_RS232[]               = _T("RS232");
const TCHAR INTERFACE_PORT_USB[]                 = _T("USB");
const TCHAR INTERFACE_PORT_HID[]                 = _T("HID");
const TCHAR INTERFACE_PORT_ETH[]                 = _T("ETH");
const TCHAR INTERFACE_PORT_SOCKET[]              = _T("Socket");
const TCHAR INTERFACE_PORT_TWINCAT[]             = _T("TwinCAT-ADS");
const TCHAR INTERFACE_PORT_IPC[]                 = _T("IPC");
const TCHAR INTERFACE_PORT_MCP[]                 = _T("MCP");

//Definition CommandGroups
const TCHAR COMMAND_GROUP_STANDARD[]             = _T("Standard");

//Definition CommandSets
const TCHAR COMMAND_SET_CONFIGURATION[]                  = _T("Configuration");
const TCHAR COMMAND_SET_MOTOR_CONFIGURATION[]            = _T("Motor");
const TCHAR COMMAND_SET_SENSOR_CONFIGURATION[]           = _T("Sensor");
const TCHAR COMMAND_SET_CURRENT_MODE[]                   = _T("CurrentMode");
const TCHAR COMMAND_SET_HOMING_MODE[]                    = _T("HomingMode");
const TCHAR COMMAND_SET_INPUTS_OUTPUTS[]                 = _T("Input and Outputs");
const TCHAR COMMAND_SET_OUTPUTS_POSITION_COMPARE[]       = _T("Position Compare");
const TCHAR COMMAND_SET_INPUTS_POSITION_MARKER[]         = _T("Position Marker");
const TCHAR COMMAND_SET_MOTION_INFO[]                    = _T("MotionInfo");
const TCHAR COMMAND_SET_POSITION_MODE[]                  = _T("PositionMode");
const TCHAR COMMAND_SET_PROFILE_POSITION_MODE[]          = _T("ProfilePositionMode");
const TCHAR COMMAND_SET_PROFILE_VELOCITY_MODE[]          = _T("ProfileVelocityMode");
const TCHAR COMMAND_SET_INTERPOLATED_POSITION_MODE[]     = _T("InterpolatedPositionMode");
const TCHAR COMMAND_SET_STATE_MACHINE[]                  = _T("StateMachine");
const TCHAR COMMAND_SET_UTILITIES[]                      = _T("Utilities");
const TCHAR COMMAND_SET_VELOCITY_MODE[]                  = _T("VelocityMode");
const TCHAR COMMAND_SET_GENERAL_GATEWAY[]                = _T("CAN Commands");
const TCHAR COMMAND_SET_NETWORK_MANAGEMENT[]             = _T("NetworkManagement");
const TCHAR COMMAND_SET_OBJECT_DICTIONARY[]              = _T("ObjectDictionary");
const TCHAR COMMAND_SET_LAYER_SETTING_SERVICES[]         = _T("LayerSettingServices");
const TCHAR COMMAND_SET_DEVICE_CONTROL[]                 = _T("DeviceControl");
const TCHAR COMMAND_SET_MASTER_ENCODER_MODE[]            = _T("MasterEncoderMode");
const TCHAR COMMAND_SET_STEP_DIRECTION_MODE[]            = _T("StepDirectionMode");
const TCHAR COMMAND_SET_DATA_RECORDING[]                 = _T("DataRecording");
const TCHAR COMMAND_SET_OBJECTDICTIONARY[]               = _T("ObjectDictionary");
const TCHAR COMMAND_SET_ERROR_HANDLING[]                 = _T("ErrorHandling");
const TCHAR COMMAND_SET_VERSION[]                        = _T("Version");
const TCHAR COMMAND_SET_INITIALISATION[]                 = _T("Initialisation");
const TCHAR COMMAND_SET_PROGRAM_VARIABLE_ACCESS[]        = _T("ProgramVariableAccess");
const TCHAR COMMAND_SET_PROGRAM_STATUS[]                 = _T("ProgramStatus");
const TCHAR COMMAND_SET_PROCESS_INPUT_OUTPUT_ACCESS[]    = _T("ProcessInputOutputAccess");
const TCHAR COMMAND_SET_PROCESS_IMAGE_ACCESS[]           = _T("ProcessImageAccess");
const TCHAR COMMAND_SET_DOWNLOAD[]                       = _T("Download");

#endif    //CommunicationModelDefinitions_04082003
