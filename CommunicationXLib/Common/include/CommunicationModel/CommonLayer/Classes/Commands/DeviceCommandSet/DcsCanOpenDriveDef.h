/////////////////////////////////////////////////////////////////////////
// DcsCanOpenDriveDef.h
//
// Project:             Dtm
// Author:              Christian Herger
// Date of Creation:    17.12.2007
//
// Contains all Drive related CANopen definitions for the DCS.
//
// Copyright © 2007, maxon motor ag.
// All rights reserved.
/////////////////////////////////////////////////////////////////////////

#if !defined(DcsCanOpenDriveDef_H__20071217__INCLUDED_)
#define DcsCanOpenDriveDef_H__20071217__INCLUDED_

enum EDeviceState
{
    DS_NOT_READY_TO_SWITCH_ON       = 0x0000,
    DS_SWITCH_ON_DISABLED           = 0x0040,
    DS_READY_TO_SWITCH_ON           = 0x0021,
    DS_SWITCHED_ON                  = 0x0023,
    DS_OPERATION_ENABLED            = 0x0027,
    DS_QUICK_STOP_ACTIV             = 0x0007,
    DS_FAULT_REACTION_ACTIV         = 0x000F,
    DS_FAULT                        = 0x0008,
    DS_TARGET_REACHED               = 0x0400
};

// Homing states
enum EHomingState
{
	HS_HOMING_IDLE_STATE			= 0x0000,
	HS_HOMING_ATTAINED				= 0x1000,
	HS_HOMING_ERROR					= 0x2000
};

//CANopen StateMachine
const WORD      DISABLE_OPERATION                       = 0x0007;
const WORD      DISABLE_VOLTAGE                         = 0x0000;
const WORD      ENABLE_OPERATION                        = 0x010F;
const WORD      FAULT_RESET                             = 0x0080;
const WORD      QUICK_STOP                              = 0x0002;
const WORD      SHUTDOWN                                = 0x0006;
const WORD      SWITCH_ON                               = 0x000F;

//Profile Position
const WORD      IMMEDIATE_ABS_POSITION                  = 0x003F;
const WORD      ABSOLUTE_POSITION                       = 0x001F;
const WORD      IMMEDIATE_REL_POSITION                  = 0x007F;
const WORD      RELATIVE_POSITION                       = 0x005F;

//Homing
const WORD      START_HOMING                            = 0x001F;
const WORD      STOP_HOMING                             = 0x011F;

//Profile Velocity
const WORD      MOVE_WITH_VELOCITY                      = 0x000F;

//Profile Interpolated Position
const WORD      ENABLE_INTERPOLATED_POSITION_MODE       = 0x001F;

//General
const WORD      STOP_PROFILE                            = 0x010F;
const WORD      START_PROFILE                           = 0x000F;


//Definition of mandatory drive objects (index/subindex)
const WORD      INDEX_CONTROL_WORD								= 0x6040;
const BYTE      SUBINDEX_CONTROL_WORD							= 0x00;

const WORD      INDEX_STATUS_WORD								= 0x6041;
const BYTE      SUBINDEX_STATUS_WORD							= 0x00;

const WORD      INDEX_FAULT_REACTION_OPTION_CODE                = 0x605E;
const BYTE      SUBINDEX_FAULT_REACTION_OPTION_CODE             = 0x00;

const WORD      INDEX_OPERATION_MODE							= 0x6060;
const BYTE      SUBINDEX_OPERATION_MODE							= 0x00;

const WORD      INDEX_OPERATION_MODE_DISPLAY					= 0x6061;
const BYTE      SUBINDEX_OPERATION_MODE_DISPLAY					= 0x00;

const WORD      INDEX_POSITION_DEMAND_VALUE						= 0x6062;
const BYTE      SUBINDEX_POSITION_DEMAND_VALUE					= 0x00;

const WORD      INDEX_POSITION_ACTUAL_VALUE						= 0x6064;
const BYTE      SUBINDEX_POSITION_ACTUAL_VALUE					= 0x00;

const WORD      INDEX_MAX_FOLLOWING_ERROR						= 0x6065;
const BYTE      SUBINDEX_MAX_FOLLOWING_ERROR					= 0x00;

const WORD      INDEX_TIME_OUT_FOLLOWING_ERROR					= 0x6066;
const BYTE      SUBINDEX_TIME_OUT_FOLLOWING_ERROR				= 0x00;

const WORD      INDEX_POSITION_WINDOW							= 0x6067;
const BYTE      SUBINDEX_POSITION_WINDOW						= 0x00;

const WORD      INDEX_POSITION_WINDOW_TIME						= 0x6068;
const BYTE      SUBINDEX_POSITION_WINDOW_TIME					= 0x00;

const WORD      INDEX_VELOCITY_DEMAND_VALUE						= 0x606B;
const BYTE      SUBINDEX_VELOCITY_DEMAND_VALUE					= 0x00;

const WORD      INDEX_VELOCITY_ACTUAL_VALUE						= 0x606C;
const BYTE      SUBINDEX_VELOCITY_ACTUAL_VALUE					= 0x00;

const WORD      INDEX_VELOCITY_WINDOW							= 0x606D;
const BYTE      SUBINDEX_VELOCITY_WINDOW						= 0x00;

const WORD      INDEX_VELOCITY_WINDOW_TIME						= 0x606E;
const BYTE      SUBINDEX_VELOCITY_WINDOW_TIME					= 0x00;

const WORD      INDEX_TARGET_TORQUE                             = 0x6071;
const BYTE      SUBINDEX_TARGET_TORQUE                          = 0x00;

const WORD      INDEX_TORQUE_ACTUAL_VALUE                       = 0x6077;
const BYTE      SUBINDEX_TORQUE_ACTUAL_VALUE                    = 0x00;

const WORD      INDEX_CURRENT_ACTUAL_VALUE						= 0x6078;
const BYTE      SUBINDEX_CURRENT_ACTUAL_VALUE					= 0x00;

const WORD      INDEX_TARGET_POSITION							= 0x607A;
const BYTE      SUBINDEX_TARGET_POSITION						= 0x00;

const WORD      INDEX_HOME_OFFSET								= 0x607C;
const BYTE      SUBINDEX_HOME_OFFSET							= 0x00;

const WORD      INDEX_POSITION_LIMIT							= 0x607D;
const BYTE      SUBINDEX_MIN_POSITION_LIMIT						= 0x01;
const BYTE      SUBINDEX_MAX_POSITION_LIMIT						= 0x02;

const WORD      INDEX_MAX_PROFILE_VELOCITY						= 0x607F;
const BYTE      SUBINDEX_MAX_PROFILE_VELOCITY					= 0x00;

const WORD      INDEX_PROFILE_VELOCITY							= 0x6081;
const BYTE      SUBINDEX_PROFILE_VELOCITY						= 0x00;

const WORD      INDEX_PROFILE_ACCELERATION						= 0x6083;
const BYTE      SUBINDEX_PROFILE_ACCELERATION					= 0x00;

const WORD      INDEX_PROFILE_DECELERATION						= 0x6084;
const BYTE      SUBINDEX_PROFILE_DECELERATION					= 0x00;

const WORD      INDEX_QUICK_STOP_DECELERATION					= 0x6085;
const BYTE      SUBINDEX_QUICK_STOP_DECELERATION				= 0x00;

const WORD      INDEX_PROFILE_TYPE								= 0x6086;
const BYTE      SUBINDEX_PROFILE_TYPE							= 0x00;

const WORD      INDEX_POSITION_NOTATION							= 0x6089;
const BYTE      SUBINDEX_POSITION_NOTATION						= 0x00;

const WORD      INDEX_POSITION_DIMENSION						= 0x608A;
const BYTE      SUBINDEX_POSITION_DIMENSION						= 0x00;

const WORD      INDEX_VELOCITY_NOTATION							= 0x608B;
const BYTE      SUBINDEX_VELOCITY_NOTATION						= 0x00;

const WORD      INDEX_VELOCITY_DIMENSION						= 0x608C;
const BYTE      SUBINDEX_VELOCITY_DIMENSION						= 0x00;

const WORD      INDEX_ACCELERATION_NOTATION						= 0x608D;
const BYTE      SUBINDEX_ACCELERATION_NOTATION					= 0x00;

const WORD      INDEX_ACCELERATION_DIMENSION					= 0x608E;
const BYTE      SUBINDEX_ACCELERATION_DIMENSION					= 0x00;

const WORD      INDEX_HOMING_METHOD								= 0x6098;
const BYTE      SUBINDEX_HOMING_METHOD							= 0x00;

const WORD      INDEX_HOMING_SPEED								= 0x6099;
const BYTE      SUBINDEX_SPEED_SEARCH_SWITCH					= 0x01;
const BYTE      SUBINDEX_SPEED_SEARCH_ZERO						= 0x02;

const WORD      INDEX_HOMING_ACCELERATION						= 0x609A;
const BYTE      SUBINDEX_HOMING_ACCELERATION					= 0x00;

const WORD      INDEX_POSITION_OFFSET                           = 0x60B0;
const BYTE      SUBINDEX_POSITION_OFFSET                        = 0x00;

const WORD      INDEX_VELOCITY_OFFSET                           = 0x60B1;
const BYTE      SUBINDEX_VELOCITY_OFFSET                        = 0x00;

const WORD      INDEX_TORQUE_OFFSET                             = 0x60B2;
const BYTE      SUBINDEX_TORQUE_OFFSET                          = 0x00;

const WORD      INDEX_INTERPOLATION_SUBMODE						= 0x60C0;
const BYTE      SUBINDEX_INTERPOLATION_SUBMODE					= 0x00;

const WORD      INDEX_INTERPOLATION_TIME_PERIOD					= 0x60C2;
const BYTE      SUBINDEX_INTERPOLATION_TIME_PERIOD_VALUE		= 0x01;
const BYTE      SUBINDEX_INTERPOLATION_TIME_INDEX				= 0x02;

const WORD      INDEX_INTERPOLATION_DATA_CONFIGURATION          = 0x60C4;
const BYTE      SUBINDEX_INTERPOLATION_DC_MAX_BUFFER_SIZE       = 0x01;
const BYTE      SUBINDEX_INTERPOLATION_DC_ACTUAL_BUFFER_SIZE    = 0x02;
const BYTE      SUBINDEX_INTERPOLATION_DC_BUFFER_ORGANISATION   = 0x03;
const BYTE      SUBINDEX_INTERPOLATION_DC_BUFFER_POSITION       = 0x04;
const BYTE      SUBINDEX_INTERPOLATION_DC_SIZE_OF_DATARECORD    = 0x05;
const BYTE      SUBINDEX_INTERPOLATION_DC_BUFFER_CLEAR          = 0x06;

const WORD      INDEX_MAX_ACCELERATION							= 0x60C5;
const BYTE      SUBINDEX_MAX_ACCELERATION						= 0x00;

const WORD      INDEX_CURRENT_GAIN								= 0x60F6;
const BYTE      SUBINDEX_CURRENT_P_GAIN							= 0x01;
const BYTE      SUBINDEX_CURRENT_I_GAIN							= 0x02;

const WORD      INDEX_VELOCITY_GAIN								= 0x60F9;
const BYTE      SUBINDEX_VELOCITY_P_GAIN						= 0x01;
const BYTE      SUBINDEX_VELOCITY_I_GAIN						= 0x02;
const BYTE      SUBINDEX_VELOCITY_VELOCITY_FEED_FORWARD			= 0x04;
const BYTE      SUBINDEX_VELOCITY_ACCELERATION_FEED_FORWARD		= 0x05;


const WORD      INDEX_POSITION_GAIN								= 0x60FB;
const BYTE      SUBINDEX_POSITION_P_GAIN						= 0x01;
const BYTE      SUBINDEX_POSITION_I_GAIN						= 0x02;
const BYTE      SUBINDEX_POSITION_D_GAIN						= 0x03;
const BYTE      SUBINDEX_POSITION_VELOCITY_FEED_FORWARD			= 0x04;
const BYTE      SUBINDEX_POSITION_ACCELERATION_FEED_FORWARD		= 0x05;

const WORD      INDEX_POSITION_DEMAND_VALUE_INTERPOLATED		= 0x60FC;
const BYTE      SUBINDEX_POSITION_DEMAND_VALUE_INTERPOLATED		= 0x00;

const WORD      INDEX_TARGET_VELOCITY							= 0x60FF;
const BYTE      SUBINDEX_TARGET_VELOCITY						= 0x00;

const WORD      INDEX_MOTOR_TYPE								= 0x6402;
const BYTE      SUBINDEX_MOTOR_TYPE								= 0x00;

const WORD      INDEX_MOTOR_DATA								= 0x6410;
const BYTE      SUBINDEX_MAX_CON_CURRENT						= 0x01;
const BYTE      SUBINDEX_MAX_PEAK_CURRENT						= 0x02;
const BYTE      SUBINDEX_POLE_PAIR_NUMBER						= 0x03;
const BYTE      SUBINDEX_MAX_SPEED_IN_CURRENT_MODE				= 0x04;
const BYTE      SUBINDEX_THERMAL_TIME_CONSTANT					= 0x05;
const BYTE      SUBINDEX_TORQUE_CONSTANT                        = 0x06;
//Old ObjectIndices for StartupWizard OldCompatibility Mode
const BYTE      SUBINDEX_OLD_ENCODER_PULSE_NUMBER				= 0x05;
const BYTE      SUBINDEX_OLD_THERMAL_TIME_CONSTANT				= 0x06;

#endif // !defined(DcsCanOpenDriveDef_H__20071217__INCLUDED_)
