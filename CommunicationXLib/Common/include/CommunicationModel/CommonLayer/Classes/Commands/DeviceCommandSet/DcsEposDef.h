/////////////////////////////////////////////////////////////////////////
// DcsEposDef.h
//
// Project:             Dtm
// Author:              Christian Herger
// Date of Creation:    17.12.2007
//
// Contains all EPOS related CANopen definitions for the DCS.
//
// Copyright � 2007, maxon motor ag.
// All rights reserved.
/////////////////////////////////////////////////////////////////////////

#if !defined(DcsEposDef_H__20071217__INCLUDED_)
#define DcsEposDef_H__20071217__INCLUDED_

#include <MmcTypeDefinition.h>

//Default ProtocolStack Settings
const DWORD		DEFAULT_TRANSFER_RATE_RS232				= 38400;
const DWORD		DEFAULT_TRANSFER_RATE_CAN_OPEN			= 1000000;
const DWORD		DEFAULT_TIMEOUT							= 500;

//Command Ids
//Object Dictionary Access Commands fuer alten RS232 Befehlssatz (Firmware SW 0x2000)
const DWORD     EPOS_READ_OD_OBJECT                     = 0x0000000A;
const DWORD     EPOS_READ_OD_OBJECT_NEXT_SEGMENT        = 0x0000000B;
const DWORD     EPOS_WRITE_OD_OBJECT                    = 0x0000000C;
const DWORD     EPOS_WRITE_OD_OBJECT_NEXT_SEGMENT       = 0x0000000D;

//Object Dictionary Access Commands //Funktionalit�t fuer neuen RS232 Befehlssatz (Gateway support, Firmware >= SW 0x2001)
const DWORD     EPOS_READ_OBJECT                        = 0x00000010;
const DWORD     EPOS_WRITE_OBJECT                       = 0x00000011;
const DWORD     EPOS_INITIATE_SEGMENT_READ              = 0x00000012;
const DWORD     EPOS_INITIATE_SEGMENT_WRITE             = 0x00000013;
const DWORD     EPOS_SEGMENT_READ                       = 0x00000014;
const DWORD     EPOS_SEGMENT_WRITE                      = 0x00000015;
const DWORD     EPOS_ABORT_SEGMENT_TRANSFER             = 0x00000016;

//Network Management Commands
const DWORD     EPOS_SEND_NMT_SERVICE                   = 0x0000000E;

//General Gateway
const DWORD     EPOS_SEND_CAN_FRAME                     = 0x00000020;
const DWORD     EPOS_REQUEST_CAN_FRAME                  = 0x00000021;
const DWORD     EPOS_READ_CAN_FRAME                     = 0x00000022;

//Layer Setting Services
const DWORD     EPOS_SEND_LSS_FRAME                     = 0x00000030;
const DWORD     EPOS_READ_LSS_FRAME                     = 0x00000031;

//DeviceCommandSet
#pragma pack(push,1)
union UEposControlByte
{
	struct SControlByte
	{
			BYTE	bLength			:6;
			BYTE	bToggle			:1;
			BYTE	bMoreSegments	:1;
	}structure;
	BYTE bValue;
};
#pragma pack(pop)

//Sensor Types
enum EPositionSensorType
{
	PST_ENCODER_3_CHANNEL = 1, 
	PST_ENCODER_2_CHANNEL = 2, 
	PST_HALL_SENSORS = 3
};

//Definition of mandatory EPOS objects (index/subindex)
const WORD		INDEX_NODE_ID									= 0x2000;
const BYTE		SUBINDEX_NODE_ID								= 0x00;

const WORD		INDEX_CAN_BITRATE								= 0x2001;
const BYTE		SUBINDEX_CAN_BITRATE							= 0x00;

const WORD		INDEX_BAUDRATE									= 0x2002;
const BYTE		SUBINDEX_BAUDRATE								= 0x00;

const WORD		INDEX_VERSION									= 0x2003;
const BYTE		SUBINDEX_SOFTWARE_VERSION						= 0x01;
const BYTE		SUBINDEX_HARDWARE_VERSION						= 0x02;
const BYTE		SUBINDEX_APPLICATION_NUMBER						= 0x03;
const BYTE		SUBINDEX_APPLICATION_VERSION					= 0x04;
const BYTE		SUBINDEX_BASE_VERSION							= 0x05;

const WORD		INDEX_SERIAL_NUMBER								= 0x2004;
const BYTE		SUBINDEX_SERIAL_NUMBER							= 0x00;

const WORD		INDEX_RS232_FRAME_TIMEOUT						= 0x2005;
const BYTE		SUBINDEX_RS232_FRAME_TIMEOUT					= 0x00;

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

const WORD      INDEX_POSITION_MARKER							= 0x2074;
const BYTE      SUBINDEX_POSITION_MARKER_CAPTURED_POSITION		= 0x01;
const BYTE		SUBINDEX_POSITION_MARKER_EDGE_TYPE				= 0x02;
const BYTE      SUBINDEX_POSITION_MARKER_MODE					= 0x03;
const BYTE      SUBINDEX_POSITION_MARKER_COUNTER				= 0x04;
const BYTE      SUBINDEX_POSITION_MARKER_HISTORY_1				= 0x05;
const BYTE      SUBINDEX_POSITION_MARKER_HISTORY_2				= 0x06;

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
const BYTE      SUBINDEX_DIGITAL_POSITION_SCALING_DENOMINATOR	= 0x03;
const BYTE      SUBINDEX_DIGITAL_POSITION_POLARITY		        = 0x04;

const WORD      INDEX_FIRMWARE_DOWNLOAD		        		    = 0x2FFF;
const BYTE      SUBINDEX_FIRMWARE_DOWNLOAD_START		        = 0x01;

#endif // !defined(DcsEposDef_H__20071217__INCLUDED_)
