/***************************************************************************

    HEADER FILE:    Command_VCS_Drive_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_Drive_Definitions.h
    Date:           25.10.2005

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_Drive_Definitions_07102003)
#define Command_VCS_Drive_Definitions_07102003

//Command Ids
//Configuration
    //Position Regulator
    const DWORD    DRIVE_SET_POSITION_REGULATOR_GAIN                = 0x00100001;
    const DWORD    DRIVE_GET_POSITION_REGULATOR_GAIN                = 0x00100002;
    const DWORD    DRIVE_SET_POSITION_REGULATOR_FEED_FORWARD        = 0x00100003;
    const DWORD    DRIVE_GET_POSITION_REGULATOR_FEED_FORWARD        = 0x00100004;

    //Velocity Regulator
    const DWORD    DRIVE_SET_VELOCITY_REGULATOR_GAIN                = 0x00100005;
    const DWORD    DRIVE_GET_VELOCITY_REGULATOR_GAIN                = 0x00100006;

    //Current Regulator
    const DWORD    DRIVE_SET_CURRENT_REGULATOR_GAIN                 = 0x00100007;
    const DWORD    DRIVE_GET_CURRENT_REGULATOR_GAIN                 = 0x00100008;

    //Safety
    const DWORD    DRIVE_SET_MAX_FOLLOWING_ERROR                    = 0x00100009;
    const DWORD    DRIVE_GET_MAX_FOLLOWING_ERROR                    = 0x00100010;
    const DWORD    DRIVE_SET_MAX_PROFILE_VELOCITY                   = 0x00100011;
    const DWORD    DRIVE_GET_MAX_PROFILE_VELOCITY                   = 0x00100012;
    
    //Units
    const DWORD    DRIVE_SET_VELOCITY_UNITS                         = 0x00100018;
    const DWORD    DRIVE_GET_VELOCITY_UNITS                         = 0x00100019;

    //Motor
    const DWORD    DRIVE_GET_MOTOR_PARAMETER                        = 0x00100020;
    const DWORD    DRIVE_SET_MOTOR_PARAMETER                        = 0x00100021;
    const DWORD    DRIVE_SET_MOTOR_TYPE                             = 0x00100022;
    const DWORD    DRIVE_GET_MOTOR_TYPE                             = 0x00100023;
    const DWORD    DRIVE_SET_DC_MOTOR_PARAMETER                     = 0x00100024;
    const DWORD    DRIVE_GET_DC_MOTOR_PARAMETER                     = 0x00100025;
    const DWORD    DRIVE_SET_EC_MOTOR_PARAMETER                     = 0x00100026;
    const DWORD    DRIVE_GET_EC_MOTOR_PARAMETER                     = 0x00100027;

    //Sensor
    const DWORD    DRIVE_SET_ENCODER_PARAMETER                      = 0x00100030;
    const DWORD    DRIVE_GET_ENCODER_PARAMETER                      = 0x00100031;
    const DWORD    DRIVE_SET_SENSOR_TYPE                            = 0x00100032;
    const DWORD    DRIVE_GET_SENSOR_TYPE                            = 0x00100033;
    const DWORD    DRIVE_SET_INC_ENCODER_PARAMETER                  = 0x00100034;
    const DWORD    DRIVE_GET_INC_ENCODER_PARAMETER                  = 0x00100035;
    const DWORD    DRIVE_SET_HALL_SENSOR_PARAMETER                  = 0x00100036;
    const DWORD    DRIVE_GET_HALL_SENSOR_PARAMETER                  = 0x00100037;
    
//CurrentMode
    const DWORD    DRIVE_GET_CURRENT_MUST                           = 0x00100040;
    const DWORD    DRIVE_SET_CURRENT_MUST                           = 0x00100041;

//HomingMode
    const DWORD    DRIVE_FIND_HOME                                  = 0x00100060;
    const DWORD    DRIVE_GET_HOMING_PARAMETER                       = 0x00100061;
    const DWORD    DRIVE_SET_HOMING_PARAMETER                       = 0x00100062;
    const DWORD    DRIVE_STOP_HOMING                                = 0x00100063;
    const DWORD    DRIVE_DEFINE_POSITION                            = 0x00100064;
    const DWORD    DRIVE_GET_HOMING_STATE							= 0x00100065;

//InputOutputs
    //General
    const DWORD    DRIVE_DIGITAL_INPUT_CONFIGURATION                = 0x00100070;
    const DWORD    DRIVE_DIGITAL_OUTPUT_CONFIGURATION               = 0x00100071;
    const DWORD    DRIVE_ANALOG_INPUT_CONFIGURATION                 = 0x00100072;
    const DWORD    DRIVE_GET_ALL_DIGITAL_INPUTS                     = 0x00100073;
    const DWORD    DRIVE_GET_ALL_DIGITAL_OUTPUTS                    = 0x00100074;
    const DWORD    DRIVE_GET_ANALOG_INPUT                           = 0x00100075;
    const DWORD    DRIVE_SET_ALL_DIGITAL_OUTPUTS                    = 0x00100076;
    const DWORD    DRIVE_SET_ANALOG_OUTPUT                          = 0x00100077;

    //Position Marker
    const DWORD    DRIVE_SET_POSITION_MARKER_PARAMETER              = 0x00100090;
    const DWORD    DRIVE_GET_POSITION_MARKER_PARAMETER              = 0x00100091;
    const DWORD    DRIVE_ACTIVATE_POSITION_MARKER                   = 0x00100092;
    const DWORD    DRIVE_DEACTIVATE_POSITION_MARKER                 = 0x00100093;
    const DWORD    DRIVE_READ_POSITION_MARKER_COUNTER               = 0x00100094;
    const DWORD    DRIVE_READ_POSITION_MARKER_CAPTURED_POSITION     = 0x00100095;
    const DWORD    DRIVE_RESET_POSITION_MARKER_COUNTER              = 0x00100096;

//MotionInfo
    const DWORD    DRIVE_GET_CURRENT_IS                             = 0x00100100;
    const DWORD    DRIVE_GET_MOVEMENT_STATE                         = 0x00100101;
    const DWORD    DRIVE_GET_POSITION_IS                            = 0x00100102;
    const DWORD    DRIVE_GET_VELOCITY_IS                            = 0x00100103;
    const DWORD    DRIVE_GET_VELOCITY_IS_AVERAGED                   = 0x00100104;
    const DWORD    DRIVE_GET_CURRENT_IS_AVERAGED                    = 0x00100105;

//PositionMode
    const DWORD    DRIVE_GET_POSITION_MUST                          = 0x00100110;
    const DWORD    DRIVE_SET_POSITION_MUST                          = 0x00100111;

//ProfilePositionMode
    const DWORD    DRIVE_GET_POSITION_PROFILE                       = 0x00100120;
    const DWORD    DRIVE_GET_TARGET_POSITION                        = 0x00100121;
    const DWORD    DRIVE_HALT_POSITION_MOVEMENT                     = 0x00100122;
    const DWORD    DRIVE_MOVE_TO_POSITION                           = 0x00100123;
    const DWORD    DRIVE_SET_POSITION_PROFILE                       = 0x00100124;
    const DWORD    DRIVE_ENABLE_POSITION_WINDOW                     = 0x00100125;
    const DWORD    DRIVE_DISABLE_POSITION_WINDOW                    = 0x00100126;

//ProfileVelocityMode
    const DWORD    DRIVE_GET_TARGET_VELOCITY                        = 0x00100130;
    const DWORD    DRIVE_GET_VELOCITY_PROFILE                       = 0x00100131;
    const DWORD    DRIVE_HALT_VELOCITY_MOVEMENT                     = 0x00100132;
    const DWORD    DRIVE_MOVE_WITH_VELOCITY                         = 0x00100133;
    const DWORD    DRIVE_SET_VELOCITY_PROFILE                       = 0x00100134;

//MasterEncoderMode
    const DWORD    DRIVE_SET_MASTER_ENCODER_PARAMETER               = 0x00100150;
    const DWORD    DRIVE_GET_MASTER_ENCODER_PARAMETER               = 0x00100151;

//StepDirectionMode
    const DWORD    DRIVE_SET_STEP_DIRECTION_PARAMETER               = 0x00100160;
    const DWORD    DRIVE_GET_STEP_DIRECTION_PARAMETER               = 0x00100161;

//StateMachine
    const DWORD    DRIVE_CLEAR_FAULT                                = 0x00100170;
    const DWORD    DRIVE_GET_DISABLE_STATE                          = 0x00100171;
    const DWORD    DRIVE_GET_ENABLE_STATE                           = 0x00100172;
    const DWORD    DRIVE_GET_FAULT_STATE                            = 0x00100173;
    const DWORD    DRIVE_GET_OPERATION_MODE                         = 0x00100174;
    const DWORD    DRIVE_GET_QUICK_STOP_STATE                       = 0x00100175;
    const DWORD    DRIVE_SET_DISABLE_STATE                          = 0x00100176;
    const DWORD    DRIVE_SET_ENABLE_STATE                           = 0x00100177;
    const DWORD    DRIVE_SET_OPERATION_MODE                         = 0x00100178;
    const DWORD    DRIVE_SET_QUICK_STOP_STATE                       = 0x00100179;

//VelocityMode
    const DWORD    DRIVE_GET_VELOCITY_MUST                          = 0x00100200;
    const DWORD    DRIVE_SET_VELOCITY_MUST                          = 0x00100201;

//FirmwareDownload
    const DWORD    DRIVE_GET_BASE_SECTOR_VERSION                    = 0x00100220;
    const DWORD    DRIVE_GET_SERIAL_NUMBER                          = 0x00100221;


#endif    //Command_VCS_Drive_Definitions_07102003

