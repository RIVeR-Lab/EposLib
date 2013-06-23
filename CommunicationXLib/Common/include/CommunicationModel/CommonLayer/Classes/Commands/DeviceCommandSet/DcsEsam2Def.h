/////////////////////////////////////////////////////////////////////////
// DcsEsam2Def.h
//
// Project:             Dtm
// Author:              Roland Odermatt
// Date of Creation:    29.09.2009
//
// Contains all ESAM2 related CANopen definitions for the DCS.
//
// Copyright � 2009, maxon motor ag.
// All rights reserved.
/////////////////////////////////////////////////////////////////////////

#if !defined(DcsEsam2Def_H__20071217__INCLUDED_)
#define DcsEsam2Def_H__20071217__INCLUDED_

#include <MmcTypeDefinition.h>

//Default ProtocolStack Settings
const DWORD		DEFAULT_TRANSFER_RATE_RS232				= 115200;
const DWORD		DEFAULT_TRANSFER_RATE_USB				= 1000000;
const DWORD		DEFAULT_TRANSFER_RATE_CAN_OPEN			= 1000000;
const DWORD		DEFAULT_TIMEOUT							= 500;

//Command Ids
//Object Dictionary Access Commands
const DWORD     ESAM2_READ_OBJECT                       = 0x00030010;
const DWORD     ESAM2_WRITE_OBJECT                      = 0x00030011;
const DWORD     ESAM2_INITIATE_SEGMENT_READ             = 0x00030012;
const DWORD     ESAM2_INITIATE_SEGMENT_WRITE            = 0x00030013;
const DWORD     ESAM2_SEGMENT_READ                      = 0x00030014;
const DWORD     ESAM2_SEGMENT_WRITE                     = 0x00030015;
const DWORD     ESAM2_ABORT_SEGMENT_TRANSFER            = 0x00030016;

//Network Management Commands
const DWORD     ESAM2_SEND_NMT_SERVICE                  = 0x0003000E;

//General Gateway
const DWORD     ESAM2_SEND_CAN_FRAME                    = 0x00030020;
const DWORD     ESAM2_REQUEST_CAN_FRAME                 = 0x00030021;
const DWORD     ESAM2_READ_CAN_FRAME                    = 0x00030022;

//Layer Setting Services
const DWORD     ESAM2_SEND_LSS_FRAME                    = 0x00030030;
const DWORD     ESAM2_READ_LSS_FRAME                    = 0x00030031;

//OpenPCS History Data Commands
const DWORD		ESAM2_OPCS_HIST_GET_RANGE				= 0x00030040;
const DWORD		ESAM2_OPCS_HIST_GET_RANGE_RESULT		= 0x00030041;

//DeviceCommandSet
#pragma pack(push, 1)
union UEsam2ControlByte
{
    struct SControlByte
    {
        BYTE    ubToggle                :1;
        BYTE    ubLastDataSegment       :1;
    }structure;
    BYTE ubValue;
};
#pragma pack(pop)

//PLC2 Program Control
const BYTE      PLC_STOP_PROGRAM        = 0x00;
const BYTE      PLC_START_PROGRAM       = 0x01;
const BYTE      PLC_RESET_PROGRAM       = 0x02;
const BYTE      PLC_CLEAR_PROGRAM       = 0x03;
const BYTE      PLC_COLDSTART_PROGRAM   = 0x81;
const BYTE      PLC_WARMSTART_PROGRAM   = 0x82;
const BYTE      PLC_HOTSTART_PROGRAM    = 0x83;

//PLC2 Process Input/Output Access
enum EProcessInputType
{
    PIT_PROCESS_INPUT_INT8                      = 0,
    PIT_PROCESS_INPUT_UINT8                     = 1,
    PIT_PROCESS_INPUT_INT16                     = 2,
    PIT_PROCESS_INPUT_UINT16                    = 3,
    PIT_PROCESS_INPUT_INT32                     = 4,
    PIT_PROCESS_INPUT_UINT32                    = 5,
    PIT_PROCESS_INPUT_INT64                     = 6,
    PIT_PROCESS_INPUT_UINT64                    = 7
};

enum EProcessOutputType
{
    PIT_PROCESS_OUTPUT_INT8                     = 0,
    PIT_PROCESS_OUTPUT_UINT8                    = 1,
    PIT_PROCESS_OUTPUT_INT16                    = 2,
    PIT_PROCESS_OUTPUT_UINT16                   = 3,
    PIT_PROCESS_OUTPUT_INT32                    = 4,
    PIT_PROCESS_OUTPUT_UINT32                   = 5,
    PIT_PROCESS_OUTPUT_INT64                    = 6,
    PIT_PROCESS_OUTPUT_UINT64                   = 7
};

//PLC2 Process Image Access
struct SProcessInputImage
{
    signed __int8           ProcessInput_INT8[16];
    unsigned __int8         ProcessInput_UINT8[16];
    signed __int16          ProcessInput_INT16[16];
    unsigned __int16        ProcessInput_UINT16[16];
    signed __int32          ProcessInput_INT32[16];
    unsigned __int32        ProcessInput_UINT32[16];
};

struct SProcessOutputImage
{
    signed __int8           ProcessOutput_INT8[16];
    unsigned __int8         ProcessOutput_UINT8[16];
    signed __int16          ProcessOutput_INT16[16];
    unsigned __int16        ProcessOutput_UINT16[16];
    signed __int32          ProcessOutput_INT32[16];
    unsigned __int32        ProcessOutput_UINT32[16];
};

//Network Names
const CStdString	MASTER_NETWORK_NAME									= _T("Master Network CAN-M");
const CStdString	MASTER_NETWORK_NAME_SHORT							= _T("Master Network");
const WORD		MASTER_DEFAULT_NETWORK_ID							= 0;

const CStdString	INTERNAL_NETWORK_NAME								= _T("Internal Network CAN-I");
const CStdString	INTERNAL_NETWORK_NAME_SHORT							= _T("Internal Network");
const WORD		INTERNAL_DEFAULT_NETWORK_ID							= 1;
const BYTE		INTERNAL_DEFAULT_NODE_ID							= 0;

const CStdString	SLAVE_NETWORK_NAME									= _T("Slave Network CAN-S");
const CStdString	SLAVE_NETWORK_NAME_SHORT							= _T("Slave Network");
const WORD		SLAVE_DEFAULT_NETWORK_ID							= 2;
const BYTE		SLAVE_DEFAULT_NODE_ID								= 0;

//Master Network Definition of mandatory objects
const WORD      INDEX_PROGRAM_DATA                                  = 0x1F50;
const BYTE      SUBINDEX_PROGRAM_DATA                               = 0x01;

const WORD      INDEX_PROGRAM_CONTROL                               = 0x1F51;
const BYTE      SUBINDEX_PROGRAM_CONTROL                            = 0x01;

const WORD      INDEX_NODE_ID                                       = 0x2000;
const BYTE      SUBINDEX_NODE_ID                                    = 0x00;

const WORD      INDEX_CAN_BITRATE                                   = 0x2001;
const BYTE      SUBINDEX_CAN_BITRATE                                = 0x00;

const WORD      INDEX_BAUDRATE                                      = 0x2002;
const BYTE      SUBINDEX_BAUDRATE                                   = 0x00;

const WORD      INDEX_EPOS2_P_VERSION                               = 0x2003;
const BYTE      SUBINDEX_EPOS2_P_SOFTWARE_VERSION                   = 0x01;
const BYTE      SUBINDEX_EPOS2_P_HARDWARE_VERSION                   = 0x02;
const BYTE      SUBINDEX_EPOS2_P_APPLICATION_NUMBER                 = 0x03;
const BYTE      SUBINDEX_EPOS2_P_APPLICATION_VERSION                = 0x04;
const BYTE      SUBINDEX_EPOS2_P_BASE_VERSION                       = 0x05;

const WORD      INDEX_ESAM2_VERSION                                 = 0x2004;
const BYTE      SUBINDEX_ESAM2_SOFTWARE_VERSION                     = 0x01;
const BYTE      SUBINDEX_ESAM2_HARDWARE_VERSION                     = 0x02;
const BYTE      SUBINDEX_ESAM2_APPLICATION_NUMBER                   = 0x03;
const BYTE      SUBINDEX_ESAM2_APPLICATION_VERSION                  = 0x04;

const WORD      INDEX_RS232_FRAME_TIMEOUT                           = 0x2005;
const BYTE      SUBINDEX_RS232_FRAME_TIMEOUT                        = 0x00;

const WORD      INDEX_USB_FRAME_TIMEOUT                             = 0x2006;
const BYTE      SUBINDEX_USB_FRAME_TIMEOUT                          = 0x00;

const WORD      INDEX_CAN_FRAME_TIMEOUT                             = 0x2007;
const BYTE      SUBINDEX_CAN_FRAME_TIMEOUT                          = 0x00;

const WORD      INDEX_SERIAL_NUMBER                                 = 0x200B;
const BYTE      SUBINDEX_SERIAL_NUMBER                              = 0x00;

const WORD      INDEX_OPEN_PCS                                      = 0x2FF0;
const BYTE      SUBINDEX_OPEN_PCS_SEND_PACKAGE                      = 0x01;
const BYTE      SUBINDEX_OPEN_PCS_READ_PACKAGE                      = 0x02;

const WORD      INDEX_DIGITAL_INPUT_CONFIGURATION                   = 0x2070;
const BYTE      SUBINDEX_DIGITAL_INPUT_CONFIGURATION                = 0x00;

const WORD      INDEX_DIGITAL_INPUT_FUNCTIONALITIES                 = 0x2071;
const BYTE      SUBINDEX_DIGITAL_INPUT_STATE                        = 0x01;
const BYTE      SUBINDEX_DIGITAL_INPUT_MASK                         = 0x02;
const BYTE      SUBINDEX_DIGITAL_INPUT_POLARITY                     = 0x03;
const BYTE      SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK               = 0x04;

const WORD      INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES                = 0x2078;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_STATE                       = 0x01;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_MASK                        = 0x02;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_POLARITY                    = 0x03;

const WORD      INDEX_DIGITAL_OUTPUT_CONFIGURATION                  = 0x2079;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_CONFIGURATION               = 0x00;

const WORD      INDEX_ANALOG_INPUT_CONFIGURATION                    = 0x207B;
const WORD      SUBINDEX_ANALOG_INPUT_CONFIGURATION                 = 0x00;

const WORD      INDEX_ANALOG_INPUTS                                 = 0x207C;
const WORD      SUBINDEX_ANALOG_INPUTS                              = 0x00;

const WORD      INDEX_ANALOG_INPUT_FUNCTIONALITIES                  = 0x207D;
const WORD      SUBINDEX_ANALOG_INPUT_EXECUTION_MASK                = 0x00;

const WORD      INDEX_ANALOG_OUTPUTS                                = 0x207E;
const BYTE      SUBINDEX_ANALOG_OUTPUTS                             = 0x00;

const WORD      INDEX_STARTUP_PROGRAM_CONTROL                       = 0x2F51;
const BYTE      SUBINDEX_STARTUP_PROGRAM_CONTROL                    = 0x00;

const WORD      INDEX_PROCESS_IMAGE                                 = 0x2F70;
const BYTE      SUBINDEX_PROCESS_IMAGE_RANGE_INPUTS                 = 0x01;
const BYTE      SUBINDEX_PROCESS_IMAGE_RANGE_OUTPUTS                = 0x02;
const BYTE      SUBINDEX_PROCESS_IMAGE_DOMAIN                       = 0x03;

const WORD      INDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA               = 0x2F71;
const BYTE      SUBINDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA_ADDRESS    = 0x01;
const BYTE      SUBINDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA_DOMAIN     = 0x02;

//Obsolete -> Remove after NetworkConfiguration Implementation ODR
const WORD      INDEX_INTERNAL_PRODUCER_HEARTBEAT_TIME              = 0x5017;
const BYTE      SUBINDEX_INTERNAL_PRODUCER_HEARTBEAT_TIME           = 0x00;

const WORD      INDEX_AXIS_00                                       = 0x5280;
const WORD      INDEX_AXIS_1F                                       = 0x529F;
const BYTE      SUBINDEX_AXIS_SLAVE_NO                              = 0x01;
const BYTE      SUBINDEX_AXIS_STATUS_WORD                           = 0x02;
const BYTE      SUBINDEX_AXIS_CONTROL_WORD                          = 0x03;
const BYTE      SUBINDEX_AXIS_OPERATION_MODE_DISPLAY                = 0x04;

const WORD      INDEX_PROCESS_INPUT_INT8                            = 0xA000;
const WORD      INDEX_PROCESS_INPUT_INT16                           = 0xA0C0;
const WORD      INDEX_PROCESS_INPUT_INT32                           = 0xA1C0;
const WORD      INDEX_PROCESS_INPUT_INT64                           = 0xA400;

const WORD      INDEX_PROCESS_INPUT_UINT8                           = 0xA040;
const WORD      INDEX_PROCESS_INPUT_UINT16                          = 0xA100;
const WORD      INDEX_PROCESS_INPUT_UINT32                          = 0xA200;
const WORD      INDEX_PROCESS_INPUT_UINT64                          = 0xA440;

const WORD      INDEX_PROCESS_INPUT_END                             = 0xA47F;

const WORD      INDEX_PROCESS_OUTPUT_INT8                           = 0xA480;
const WORD      INDEX_PROCESS_OUTPUT_INT16                          = 0xA540;
const WORD      INDEX_PROCESS_OUTPUT_INT32                          = 0xA640;
const WORD      INDEX_PROCESS_OUTPUT_INT64                          = 0xA880;

const WORD      INDEX_PROCESS_OUTPUT_UINT8                          = 0xA4C0;
const WORD      INDEX_PROCESS_OUTPUT_UINT16                         = 0xA580;
const WORD      INDEX_PROCESS_OUTPUT_UINT32                         = 0xA680;
const WORD      INDEX_PROCESS_OUTPUT_UINT64                         = 0xA8C0;

const WORD      INDEX_PROCESS_OUTPUT_END                            = 0xA8FF;

const WORD      INDEX_TX_PDO1_DEFAULT_VALUE                         = 0xA580;
const BYTE      SUBINDEX_TX_PDO1_DEFAULT_VALUE                      = 0x01;

//Internal & Slave  Network Definition of  mandatory objects
const WORD      SLAVE_NET_INDEX_NODE_ID                             = 0x5000;
const BYTE      SLAVE_NET_SUBINDEX_NODE_ID                          = 0x00;

const WORD      SLAVE_NET_INDEX_CAN_BITRATE                         = 0x5001;
const BYTE      SLAVE_NET_SUBINDEX_CAN_BITRATE                      = 0x00;

//Common Definition of mandatory objects



#endif // !defined(DcsEsam2Def_H__20071217__INCLUDED_)
