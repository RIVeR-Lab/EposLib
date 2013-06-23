/////////////////////////////////////////////////////////////////////////
// DcsEpos2Def.h
//
// Project:             Dtm
// Author:              Christian Herger
// Date of Creation:    17.12.2007
//
// Contains all EPOS2 related CANopen definitions for the DCS.
//
// Copyright © 2007, maxon motor ag.
// All rights reserved.
/////////////////////////////////////////////////////////////////////////

#if !defined(DcsEpos2Def_H__20071217__INCLUDED_)
#define DcsEpos2Def_H__20071217__INCLUDED_

#include <MmcTypeDefinition.h>

//Default ProtocolStack Settings
const DWORD		DEFAULT_TRANSFER_RATE_RS232				= 115200;
const DWORD		DEFAULT_TRANSFER_RATE_USB				= 1000000;
const DWORD		DEFAULT_TRANSFER_RATE_CAN_OPEN			= 1000000;
const DWORD		DEFAULT_TIMEOUT							= 500;

//Object Dictionary Access Commands
const DWORD     EPOS2_READ_OBJECT                       = 0x00020010;
const DWORD     EPOS2_WRITE_OBJECT                      = 0x00020011;
const DWORD     EPOS2_INITIATE_SEGMENT_READ             = 0x00020012;
const DWORD     EPOS2_INITIATE_SEGMENT_WRITE            = 0x00020013;
const DWORD     EPOS2_SEGMENT_READ                      = 0x00020014;
const DWORD     EPOS2_SEGMENT_WRITE                     = 0x00020015;
const DWORD     EPOS2_ABORT_SEGMENT_TRANSFER            = 0x00020016;

//Network Management Commands
const DWORD     EPOS2_SEND_NMT_SERVICE                  = 0x0002000E;

//General Gateway
const DWORD     EPOS2_SEND_CAN_FRAME                    = 0x00020020;
const DWORD     EPOS2_REQUEST_CAN_FRAME                 = 0x00020021;
const DWORD     EPOS2_READ_CAN_FRAME                    = 0x00020022;

//Layer Setting Services
const DWORD     EPOS2_SEND_LSS_FRAME                    = 0x00020030;
const DWORD     EPOS2_READ_LSS_FRAME                    = 0x00020031;

//RegulationTuning
const double    PI                                      = 3.14159265359;
const double    POSITION_GAIN_P_UNIT_FACTOR             = 100.53;
const double    POSITION_GAIN_I_UNIT_FACTOR             = 12.868;
const double    POSITION_GAIN_D_UNIT_FACTOR             = 12566.37;
const double    POSITION_FEED_FORWARD_FACTOR            = 15625;
const double    VELOCITY_GAIN_P_UNIT_FACTOR             = 50265.48;
const double    VELOCITY_GAIN_I_UNIT_FACTOR             = 201.062;
const double    VELOCITY_FEED_FORWARD_FACTOR            = 15625;
const double    FEED_FORWARD_FACTOR_CORRECTION          = 64;
const int       WORKAROUND_EPOS2_DUAL_LOOP_GAIN_FACTOR  = 3;

//Current units (SI)
const DWORD     CURRENT_UNIT_MILLI_AMPERE_INDEX         = 0xFD040000;
const DWORD     CURRENT_UNIT_CENTI_AMPERE_INDEX         = 0xFE040000;
const DWORD     CURRENT_UNIT_DECI_AMPERE_INDEX          = 0xFF040000;
const DWORD     CURRENT_UNIT_AMPERE_INDEX               = 0x00040000;

const CStdString   CURRENT_UNIT_MILLI_AMPERE_TEXT       = _T("mA");
const CStdString   CURRENT_UNIT_CENTI_AMPERE_TEXT       = _T("cA");
const CStdString   CURRENT_UNIT_DECI_AMPERE_TEXT        = _T("dA");
const CStdString   CURRENT_UNIT_AMPERE_TEXT             = _T("A");

//DeviceCommandSet
#pragma pack(push, 1)
union UEpos2ControlByte
{
    struct SControlByte
    {
            BYTE    bLength         :6;
            BYTE    bToggle         :1;
            BYTE    bMoreSegments   :1;
    }structure;
    BYTE bValue;
};
#pragma pack(pop)

const WORD      INDEX_NODE_ID									= 0x2000;
const BYTE      SUBINDEX_NODE_ID								= 0x00;

const WORD      INDEX_CAN_BITRATE								= 0x2001;
const BYTE      SUBINDEX_CAN_BITRATE							= 0x00;

const WORD      INDEX_BAUDRATE									= 0x2002;
const BYTE      SUBINDEX_BAUDRATE								= 0x00;

const WORD      INDEX_VERSION									= 0x2003;
const BYTE      SUBINDEX_SOFTWARE_VERSION						= 0x01;
const BYTE      SUBINDEX_HARDWARE_VERSION						= 0x02;
const BYTE      SUBINDEX_APPLICATION_NUMBER						= 0x03;
const BYTE      SUBINDEX_APPLICATION_VERSION					= 0x04;
const BYTE      SUBINDEX_BASE_VERSION							= 0x05;

const WORD      INDEX_SERIAL_NUMBER								= 0x2004;
const BYTE      SUBINDEX_SERIAL_NUMBER							= 0x00;

const WORD      INDEX_RS232_FRAME_TIMEOUT						= 0x2005;
const BYTE      SUBINDEX_RS232_FRAME_TIMEOUT					= 0x00;

const WORD      INDEX_USB_FRAME_TIMEOUT							= 0x2006;
const BYTE      SUBINDEX_USB_FRAME_TIMEOUT						= 0x00;

const WORD      INDEX_MISC_CONFIGURATION						= 0x2008;
const BYTE      SUBINDEX_MISC_CONFIGURATION						= 0x00;

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

const WORD		INDEX_INTERNAL_OBJECT_INTERNAL_PARAMETERS		= 0x202C;
const BYTE		SUBINDEX_MAIN_POSITION_SENSOR_INC_PER_REVOLUTION= 0x01;
const BYTE		SUBINDEX_AUX_POSITION_SENSOR_INC_PER_REVOLUTION	= 0x03;

const WORD      INDEX_CURRENT_SETTING_VALUE						= 0x2030;
const BYTE      SUBINDEX_CURRENT_SETTING_VALUE					= 0x00;

const WORD      INDEX_CURRENT_DEMAND_VALUE						= 0x2031;
const BYTE      SUBINDEX_CURRENT_DEMAND_VALUE					= 0x00;

const WORD      INDEX_POSITION_SETTING_VALUE					= 0x2062;
const BYTE      SUBINDEX_POSITION_SETTING_VALUE					= 0x00;

const WORD      INDEX_VELOCITY_SETTING_VALUE					= 0x206B;
const BYTE      SUBINDEX_VELOCITY_SETTING_VALUE					= 0x00;

const WORD      INDEX_DIGITAL_INPUT_CONFIGURATION				= 0x2070;
const BYTE      SUBINDEX_DIGITAL_INPUT_CONFIGURATION			= 0x00;

const WORD      INDEX_DIGITAL_INPUT_FUNCTIONALITIES				= 0x2071;
const BYTE      SUBINDEX_DIGITAL_INPUT_STATE					= 0x01;
const BYTE      SUBINDEX_DIGITAL_INPUT_MASK						= 0x02;
const BYTE      SUBINDEX_DIGITAL_INPUT_POLARITY					= 0x03;
const BYTE      SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK			= 0x04;

const WORD      INDEX_POSITION_MARKER							= 0x2074;
const BYTE      SUBINDEX_POSITION_MARKER_CAPTURED_POSITION		= 0x01;
const BYTE      SUBINDEX_POSITION_MARKER_EDGE_TYPE				= 0x02;
const BYTE      SUBINDEX_POSITION_MARKER_MODE					= 0x03;
const BYTE      SUBINDEX_POSITION_MARKER_COUNTER				= 0x04;
const BYTE      SUBINDEX_POSITION_MARKER_HISTORY_1				= 0x05;
const BYTE      SUBINDEX_POSITION_MARKER_HISTORY_2				= 0x06;

const WORD      INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES			= 0x2078;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_STATE					= 0x01;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_MASK					= 0x02;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_POLARITY				= 0x03;

const WORD      INDEX_DIGITAL_OUTPUT_CONFIGURATION				= 0x2079;
const BYTE      SUBINDEX_DIGITAL_OUTPUT_CONFIGURATION			= 0x00;

const WORD      INDEX_POSITION_COMPARE_CONFIGURATION            = 0x207A;
const BYTE      SUBINDEX_POSITION_COMPARE_CONFIGURATION         = 0x01;
const BYTE      SUBINDEX_POSITION_COMPARE_REFERENCE_POSITION    = 0x02;
const BYTE      SUBINDEX_POSITION_COMPARE_INTERVAL_WIDTH        = 0x03;
const BYTE      SUBINDEX_POSITION_COMPARE_INTERVAL_REPETITIONS  = 0x04;
const BYTE      SUBINDEX_POSITION_COMPARE_PULSE_WIDTH           = 0x05;

const WORD      INDEX_ANALOG_INPUT_CONFIGURATION				= 0x207B;
const BYTE      SUBINDEX_ANALOG_INPUT_CONFIGURATION				= 0x00;

const WORD      INDEX_ANALOG_INPUTS								= 0x207C;
const BYTE      SUBINDEX_ANALOG_INPUTS							= 0x00;
const BYTE      SUBINDEX_ANALOG_INPUT_1							= 0x01;
const BYTE      SUBINDEX_ANALOG_INPUT_2							= 0x02;

const WORD      INDEX_ANALOG_INPUT_FUNCTIONALITIES				= 0x207D;
const BYTE      SUBINDEX_ANALOG_INPUT_EXECUTION_MASK			= 0x00;

const WORD      INDEX_ANALOG_OUTPUTS							= 0x207E;
const BYTE      SUBINDEX_ANALOG_OUTPUTS							= 0x00;
const BYTE      SUBINDEX_ANALOG_OUTPUT_1						= 0x00;

const WORD      INDEX_CURRENT_THRESHOLD							= 0x2080;
const BYTE      SUBINDEX_CURRENT_THRESHOLD						= 0x00;

const WORD      INDEX_HOME_POSITION								= 0x2081;
const BYTE      SUBINDEX_HOME_POSITION							= 0x00;

const WORD      INDEX_SI_UNIT_CURRENT                           = 0x20AC;
const BYTE      SUBINDEX_SI_UNIT_CURRENT                        = 0x00;

const WORD      INDEX_INTERPOLATION_DATA_RECORD                 = 0x20C1;
const BYTE      SUBINDEX_INTERPOLATION_DATA_RECORD              = 0x00;

const WORD      INDEX_INTERPOLATION_BUFFER                      = 0x20C4;
const BYTE      SUBINDEX_INTERPOLATION_BUFFER_STATUS            = 0x01;
const BYTE      SUBINDEX_INTERPOLATION_BUFFER_UNDERFLOW_WARNING = 0x02;
const BYTE      SUBINDEX_INTERPOLATION_BUFFER_OVERFLOW_WARNING  = 0x03;

const WORD      INDEX_HOLDING_BRAKE                             = 0x2100;
const BYTE      SUBINDEX_BRAKE_REACTION_RISE_TIME               = 0x01;
const BYTE      SUBINDEX_BRAKE_REACTION_FALL_TIME               = 0x02;
const BYTE      SUBINDEX_BRAKE_TIMEOUT                          = 0x03;

const WORD      INDEX_SENSOR_CONFIGURATION						= 0x2210;
const BYTE      SUBINDEX_ENCODER_PULSE_NUMBER					= 0x01;
const BYTE      SUBINDEX_POSITION_SENSOR_TYPE					= 0x02;
const BYTE      SUBINDEX_POSITION_SENSOR_POLARITY				= 0x04;

const WORD      INDEX_SSI_ENCODER_CONFIGURATION					= 0x2211;
const BYTE      SUBINDEX_SSI_ENCODER_DATA_RATE					= 0x01;
const BYTE      SUBINDEX_SSI_ENCODER_NUMBER_OF_DATA_BITS		= 0x02;
const BYTE      SUBINDEX_SSI_ENCODER_ACTUAL_POSITION			= 0x03;
const BYTE      SUBINDEX_SSI_ENCODER_ENCODING_TYPE				= 0x04;

const WORD      INDEX_ENCODER_2_CONFIGURATION					= 0x2212;
const BYTE      SUBINDEX_ENCODER_2_PULSE_NUMBER					= 0x01;

const WORD      INDEX_SIN_ENCODER_2_CONFIGURATION				= 0x2213;
const BYTE      SUBINDEX_SIN_ENCODER_2_PULSE_NUMBER				= 0x01;

const WORD      INDEX_CONTROLLER_STRUCTURE	    				= 0x2220;
const BYTE      SUBINDEX_CONTROLLER_STRUCTURE					= 0x00;

const WORD      INDEX_GEAR_CONFIGURATION                        = 0x2230;
const BYTE      SUBINDEX_GEAR_RATIO_NUMERATOR                   = 0x01;
const BYTE      SUBINDEX_GEAR_RATIO_DENOMINATOR                 = 0x02;
const BYTE      SUBINDEX_GEAR_MAX_ENTRY_SPEED                   = 0x03;

const WORD      INDEX_DIGITAL_POSITION_INPUT                    = 0x2300;
const BYTE      SUBINDEX_DIGITAL_POSITION_DESIRED_VALUE         = 0x01;
const BYTE      SUBINDEX_DIGITAL_POSITION_SCALING_NUMERATOR     = 0x02;
const BYTE      SUBINDEX_DIGITAL_POSITION_SCALING_DENOMINATOR   = 0x03;
const BYTE      SUBINDEX_DIGITAL_POSITION_POLARITY              = 0x04;
const BYTE      SUBINDEX_DIGITAL_POSITION_OFFSET                = 0x05;

const WORD      INDEX_ANALOG_CURRENT_SETPOINT_CONFIGURATION     = 0x2301;
const BYTE      SUBINDEX_ANALOG_CURRENT_SCALING                 = 0x01;
const BYTE      SUBINDEX_ANALOG_CURRENT_OFFSET                  = 0x02;
const BYTE      SUBINDEX_ANALOG_CURRENT_NOTATION_INDEX          = 0x03;
const BYTE      SUBINDEX_ANALOG_CURRENT_SETPOINT                = 0x04;

const WORD      INDEX_ANALOG_VELOCITY_SETPOINT_CONFIGURATION    = 0x2302;
const BYTE      SUBINDEX_ANALOG_VELOCITY_SCALING                = 0x01;
const BYTE      SUBINDEX_ANALOG_VELOCITY_OFFSET                 = 0x02;
const BYTE      SUBINDEX_ANALOG_VELOCITY_NOTATION_INDEX         = 0x03;
const BYTE      SUBINDEX_ANALOG_VELOCITY_SETPOINT               = 0x04;

const WORD      INDEX_ANALOG_POSITION_SETPOINT_CONFIGURATION    = 0x2303;
const BYTE      SUBINDEX_ANALOG_POSITION_SCALING                = 0x01;
const BYTE      SUBINDEX_ANALOG_POSITION_OFFSET                 = 0x02;
const BYTE      SUBINDEX_ANALOG_POSITION_NOTATION_INDEX         = 0x03;
const BYTE      SUBINDEX_ANALOG_POSITION_SETPOINT               = 0x04;

const WORD      INDEX_POSITION_OSCILLATOR_PARAMETER             = 0x2400;
const BYTE      SUBINDEX_CUT_OFF_FREQUENCY                      = 0x01;
const BYTE      SUBINDEX_TARGET_AMPLITUDE                       = 0x02;
const BYTE      SUBINDEX_POSITION_OSCILLATION_CONTROL           = 0x03;

const WORD      INDEX_POSITION_OSCILLATOR_VALUES                = 0x2401;
const BYTE      SUBINDEX_CYCLE_DURATION                         = 0x01;
const BYTE      SUBINDEX_CURRENT_SET_VALUE                      = 0x03;
const BYTE      SUBINDEX_POSITION_PEAK_VALUE                    = 0x04;
const BYTE      SUBINDEX_DYNAMIC_FRICTION_VALUE                 = 0x05;
const BYTE      SUBINDEX_CYCLE_NUMBERS_AND_STATE                = 0x06;
const BYTE      SUBINDEX_POSITION_PEAK_VALUE_AUX                = 0x08;
const BYTE      SUBINDEX_POSITION_PEAK_VALUE_DIFF               = 0x09;

const WORD      INDEX_CURRENT_OSCILLATOR_PARAMETER              = 0x2410;
const BYTE      SUBINDEX_CURRENT_OSCILLATION_CONTROL            = 0x01;
const BYTE      SUBINDEX_CURRENT_AMPLITUDE_VALUE                = 0x02;

const WORD      INDEX_CURRENT_OSCILLATOR_VALUES                 = 0x2411;
const BYTE      SUBINDEX_RESISTANCE_VALUE                       = 0x01;
const BYTE      SUBINDEX_INDUCTIVITY_VALUE                      = 0x02;
const BYTE      SUBINDEX_FREQUENCY_VALUE                        = 0x03;

#endif // !defined(DcsEpos2Def_H__20071217__INCLUDED_)
