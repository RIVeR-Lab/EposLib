/////////////////////////////////////////////////////////////////////////
// DcsEsamDef.h
//
// Project:             Dtm
// Author:              Christian Herger
// Date of Creation:    17.12.2007
//
// Contains all ESAM related CANopen definitions for the DCS.
//
// Copyright � 2007, maxon motor ag.
// All rights reserved.
/////////////////////////////////////////////////////////////////////////

#if !defined(DcsEsamDef_H__20071217__INCLUDED_)
#define DcsEsamDef_H__20071217__INCLUDED_

#include <MmcTypeDefinition.h>

//Default ProtocolStack Settings
const DWORD		DEFAULT_TRANSFER_RATE_RS232				= 115200;
const DWORD		DEFAULT_TIMEOUT							= 500;

//Command Ids
//Object Dictionary Access Commands
const DWORD     ESAM_READ_OBJECT                        = 0x00010010;
const DWORD     ESAM_WRITE_OBJECT                       = 0x00010011;
const DWORD     ESAM_INITIATE_SEGMENT_READ              = 0x00010012;
const DWORD     ESAM_INITIATE_SEGMENT_WRITE             = 0x00010013;
const DWORD     ESAM_SEGMENT_READ                       = 0x00010014;
const DWORD     ESAM_SEGMENT_WRITE                      = 0x00010015;
const DWORD     ESAM_ABORT_SEGMENT_TRANSFER             = 0x00010016;

//Network Management Commands
const DWORD     ESAM_SEND_NMT_SERVICE                   = 0x0001000E;

//General Gateway
const DWORD     ESAM_SEND_CAN_FRAME                     = 0x00010020;
const DWORD     ESAM_REQUEST_CAN_FRAME                  = 0x00010021;
const DWORD     ESAM_READ_CAN_FRAME                     = 0x00010022;

//Layer Setting Services 
const DWORD     ESAM_SEND_LSS_FRAME                     = 0x00010030;
const DWORD     ESAM_READ_LSS_FRAME                     = 0x00010031;

//DeviceControl
const DWORD     ESAM_LOGIN                              = 0x00010040;
const DWORD     ESAM_LOGOUT                             = 0x00010041;
const DWORD     ESAM_SET_STATE                          = 0x00010042;

//CANopen Port Configuration
const BYTE      CANOPEN_PORT_INTERNAL                   = 0x01;
const BYTE      CANOPEN_PORT                            = 0x02;

//DeviceCommandSet
#pragma pack(push,1)
union UEsamControlByte
{
    struct SControlByte
    {
        BYTE    ubNbOfDataBytes      :7;
        BYTE    ubLastDataSegment    :1;
    }structure;
    BYTE ubValue;
};
#pragma pack(pop)

//Definition of mandatory ESAM objects (index/subindex)
const WORD		INDEX_BAUDRATE									= 0x2002;
const BYTE		SUBINDEX_BAUDRATE								= 0x00;

const WORD		INDEX_MISC_CONFIGURATION						= 0x2008;
const BYTE		SUBINDEX_MISC_CONFIGURATION						= 0x00;

const WORD		INDEX_ENCODER_COUNTER							= 0x2020;
const BYTE		SUBINDEX_ENCODER_COUNTER						= 0x00;

const WORD		INDEX_ENCODER_COUNTER_INDEX						= 0x2021;
const BYTE		SUBINDEX_ENCODER_COUNTER_INDEX					= 0x00;

const WORD		INDEX_HALLSENSOR_PATTERN						= 0x2022;
const BYTE		SUBINDEX_HALLSENSOR_PATTERN						= 0x00;

const WORD		INDEX_INTERNAL_OBJECT_ROTOR_ANGEL				= 0x2023;
const BYTE		SUBINDEX_INTERNAL_OBJECT_ROTOR_ANGEL			= 0x00;

const WORD		INDEX_SYSTEM_STATUS								= 0x2024;
const BYTE		SUBINDEX_SYSTEM_STATUS							= 0x00;

const WORD		INDEX_ENTER_SERVICE_MODE						= 0x2025;
const BYTE		SUBINDEX_ENTER_SERVICE_MODE						= 0x00;

const WORD		INDEX_MEMORY									= 0x2026;
const BYTE		SUBINDEX_MEMORY_ADDRESS							= 0x01;
const BYTE		SUBINDEX_MEMORY_VALUE							= 0x02;

const WORD		INDEX_CURRENT_ACTUAL_VALUE_AVERAGED				= 0x2027;
const BYTE		SUBINDEX_CURRENT_ACTUAL_VALUE_AVERAGED			= 0x00;

const WORD		INDEX_VELOCITY_ACTUAL_VALUE_AVERAGED			= 0x2028;
const BYTE		SUBINDEX_VELOCITY_ACTUAL_VALUE_AVERAGED			= 0x00;

const WORD		INDEX_CURRENT_SETTING_VALUE						= 0x2030;
const BYTE		SUBINDEX_CURRENT_SETTING_VALUE					= 0x00;

const WORD		INDEX_POSITION_SETTING_VALUE					= 0x2062;
const BYTE		SUBINDEX_POSITION_SETTING_VALUE					= 0x00;

const WORD		INDEX_VELOCITY_SETTING_VALUE					= 0x206B;
const BYTE		SUBINDEX_VELOCITY_SETTING_VALUE					= 0x00;

const WORD		INDEX_DIGITAL_INPUT_CONFIGURATION				= 0x2070;
const BYTE		SUBINDEX_DIGITAL_INPUT_CONFIGURATION			= 0x00;

const WORD      INDEX_DIGITAL_INPUT_FUNCTIONALITIES				= 0x2071;
const BYTE		SUBINDEX_DIGITAL_INPUT_STATE					= 0x01;
const BYTE		SUBINDEX_DIGITAL_INPUT_MASK						= 0x02;
const BYTE		SUBINDEX_DIGITAL_INPUT_POLARITY					= 0x03;
const BYTE      SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK			= 0x04;

const WORD		INDEX_POSITION_MARKER							= 0x2074;
const BYTE		SUBINDEX_POSITION_MARKER_EDGE_TYPE				= 0x02;

const WORD      INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES			= 0x2078;
const BYTE		SUBINDEX_DIGITAL_OUTPUT_STATE					= 0x01;
const BYTE		SUBINDEX_DIGITAL_OUTPUT_MASK					= 0x02;
const BYTE		SUBINDEX_DIGITAL_OUTPUT_POLARITY				= 0x03;

const WORD		INDEX_DIGITAL_OUTPUT_CONFIGURATION				= 0x2079;
const BYTE		SUBINDEX_DIGITAL_OUTPUT_CONFIGURATION			= 0x00;

const WORD		INDEX_ANALOG_INPUTS								= 0x207C;
const BYTE		SUBINDEX_ANALOG_INPUTS							= 0x00;

const WORD		INDEX_CURRENT_THRESHOLD							= 0x2080;
const BYTE		SUBINDEX_CURRENT_THRESHOLD						= 0x00;

const WORD		INDEX_HOME_POSITION								= 0x2081;
const BYTE		SUBINDEX_HOME_POSITION							= 0x00;

const WORD      INDEX_SENSOR_CONFIGURATION						= 0x2210;
const BYTE      SUBINDEX_ENCODER_PULSE_NUMBER					= 0x01;
const BYTE      SUBINDEX_POSITION_SENSOR_TYPE					= 0x02;
const BYTE      SUBINDEX_POSITION_SENSOR_POLARITY				= 0x04;

const WORD      INDEX_DIGITAL_POSITION_INPUT        		    = 0x2300;
const BYTE      SUBINDEX_DIGITAL_POSITION_DESIRED_VALUE	        = 0x01;
const BYTE      SUBINDEX_DIGITAL_POSITION_SCALING_NUMERATOR     = 0x02;
const BYTE      SUBINDEX_DIGITAL_POSITION_SCALING_DENOMINATOR   = 0x03;
const BYTE      SUBINDEX_DIGITAL_POSITION_POLARITY		        = 0x04;

const WORD      INDEX_NODE_ID									= 0x5000;
const BYTE      SUBINDEX_NODE_ID								= 0x00;

const WORD		INDEX_CAN_BITRATE								= 0x5001;
const BYTE		SUBINDEX_CAN_BITRATE							= 0x00;

const WORD      INDEX_EPOS_P_VERSION							= 0x5003;
const BYTE      SUBINDEX_EPOS_P_SOFTWARE_VERSION				= 0x01;
const BYTE      SUBINDEX_EPOS_P_HARDWARE_VERSION				= 0x02;
const BYTE      SUBINDEX_EPOS_P_APPLICATION_NUMBER				= 0x03;
const BYTE      SUBINDEX_EPOS_P_APPLICATION_VERSION				= 0x04;

const WORD      INDEX_ESAM_VERSION								= 0x5004;
const BYTE      SUBINDEX_ESAM_SOFTWARE_VERSION					= 0x01;
const BYTE      SUBINDEX_ESAM_HARDWARE_VERSION					= 0x02;
const BYTE      SUBINDEX_ESAM_APPLICATION_NUMBER				= 0x03;
const BYTE      SUBINDEX_ESAM_APPLICATION_VERSION				= 0x04;

const WORD		INDEX_RS232_FRAME_TIMEOUT						= 0x5005;
const BYTE		SUBINDEX_RS232_FRAME_TIMEOUT					= 0x00;

const WORD      INDEX_INTERNAL_PRODUCER_HEARTBEAT_TIME			= 0x5017;
const BYTE      SUBINDEX_INTERNAL_PRODUCER_HEARTBEAT_TIME		= 0x00;

const WORD      INDEX_INTERNAL_STATUS_WORD						= 0x5041;
const BYTE      SUBINDEX_INTERNAL_STATUS_WORD					= 0x00;

const WORD      INDEX_AXIS_00									= 0x5280;
const WORD      INDEX_AXIS_1F									= 0x529F;
const BYTE      SUBINDEX_AXIS_SLAVE_NO							= 0x01;
const BYTE      SUBINDEX_AXIS_STATUS_WORD						= 0x02;
const BYTE      SUBINDEX_AXIS_CONTROL_WORD						= 0x03;

const WORD      INDEX_PROGRAM_CONTROL							= 0x5F51;
const BYTE      SUBINDEX_PROGRAM_CONTROL						= 0x00;

#endif // !defined(DcsEsamDef_H__20071217__INCLUDED_)
