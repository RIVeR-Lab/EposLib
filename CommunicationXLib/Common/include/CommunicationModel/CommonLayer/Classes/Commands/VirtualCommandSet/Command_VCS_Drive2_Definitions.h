/***************************************************************************

    HEADER FILE:    Command_VCS_Drive2_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_Drive2_Definitions.h
    Date:           25.10.2005

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_Drive2_Definitions_07102003)
#define Command_VCS_Drive2_Definitions_07102003

//Command Ids
//Configuration
    //Position Regulator
    const DWORD    DRIVE2_SET_POSITION_REGULATOR_GAIN                = 0x00200001;
    const DWORD    DRIVE2_GET_POSITION_REGULATOR_GAIN                = 0x00200002;
    const DWORD    DRIVE2_SET_POSITION_REGULATOR_FEED_FORWARD        = 0x00200003;
    const DWORD    DRIVE2_GET_POSITION_REGULATOR_FEED_FORWARD        = 0x00200004;

    //Velocity Regulator
    const DWORD    DRIVE2_SET_VELOCITY_REGULATOR_GAIN                = 0x00200010;
    const DWORD    DRIVE2_GET_VELOCITY_REGULATOR_GAIN                = 0x00200011;
    const DWORD    DRIVE2_SET_VELOCITY_REGULATOR_FEED_FORWARD        = 0x00200012;
    const DWORD    DRIVE2_GET_VELOCITY_REGULATOR_FEED_FORWARD        = 0x00200013;

    //Current Regulator
    const DWORD    DRIVE2_SET_CURRENT_REGULATOR_GAIN                 = 0x00200015;
    const DWORD    DRIVE2_GET_CURRENT_REGULATOR_GAIN                 = 0x00200016;

    //Safety
    const DWORD    DRIVE2_SET_MAX_FOLLOWING_ERROR                    = 0x00200020;
    const DWORD    DRIVE2_GET_MAX_FOLLOWING_ERROR                    = 0x00200021;
    const DWORD    DRIVE2_SET_MAX_PROFILE_VELOCITY                   = 0x00200022;
    const DWORD    DRIVE2_GET_MAX_PROFILE_VELOCITY                   = 0x00200023;
    const DWORD    DRIVE2_SET_MAX_ACCELERATION                       = 0x00200024;
    const DWORD    DRIVE2_GET_MAX_ACCELERATION                       = 0x00200025;

    //Units
    const DWORD    DRIVE2_SET_VELOCITY_UNITS                         = 0x00200028;
    const DWORD    DRIVE2_GET_VELOCITY_UNITS                         = 0x00200029;

    //Motor
    const DWORD    DRIVE2_GET_MOTOR_PARAMETER                        = 0x00200030;
    const DWORD    DRIVE2_SET_MOTOR_PARAMETER                        = 0x00200031;
    const DWORD    DRIVE2_SET_MOTOR_TYPE                             = 0x00200032;
    const DWORD    DRIVE2_GET_MOTOR_TYPE                             = 0x00200033;
    const DWORD    DRIVE2_SET_DC_MOTOR_PARAMETER                     = 0x00200034;
    const DWORD    DRIVE2_GET_DC_MOTOR_PARAMETER                     = 0x00200035;
    const DWORD    DRIVE2_SET_EC_MOTOR_PARAMETER                     = 0x00200036;
    const DWORD    DRIVE2_GET_EC_MOTOR_PARAMETER                     = 0x00200037;

    //Sensor
    const DWORD    DRIVE2_SET_ENCODER_PARAMETER                      = 0x00200040;
    const DWORD    DRIVE2_GET_ENCODER_PARAMETER                      = 0x00200041;
    const DWORD    DRIVE2_SET_SENSOR_TYPE                            = 0x00200042;
    const DWORD    DRIVE2_GET_SENSOR_TYPE                            = 0x00200043;
    const DWORD    DRIVE2_SET_INC_ENCODER_PARAMETER                  = 0x00200044;
    const DWORD    DRIVE2_GET_INC_ENCODER_PARAMETER                  = 0x00200045;
    const DWORD    DRIVE2_SET_HALL_SENSOR_PARAMETER                  = 0x00200046;
    const DWORD    DRIVE2_GET_HALL_SENSOR_PARAMETER                  = 0x00200047;
    const DWORD    DRIVE2_SET_SSI_ABS_ENCODER_PARAMETER              = 0x00200048;
    const DWORD    DRIVE2_GET_SSI_ABS_ENCODER_PARAMETER              = 0x00200049;

//CurrentMode
    const DWORD    DRIVE2_GET_CURRENT_MUST                           = 0x00200050;
    const DWORD    DRIVE2_SET_CURRENT_MUST                           = 0x00200051;
    const DWORD    DRIVE2_ACTIVATE_ANALOG_CURRENT_SETPOINT           = 0x00200052;
    const DWORD    DRIVE2_DEACTIVATE_ANALOG_CURRENT_SETPOINT         = 0x00200053;
    const DWORD    DRIVE2_ENABLE_ANALOG_CURRENT_SETPOINT             = 0x00200054;
    const DWORD    DRIVE2_DISABLE_ANALOG_CURRENT_SETPOINT            = 0x00200055;

//HomingMode
    const DWORD    DRIVE2_FIND_HOME                                  = 0x00200060;
    const DWORD    DRIVE2_GET_HOMING_PARAMETER                       = 0x00200061;
    const DWORD    DRIVE2_SET_HOMING_PARAMETER                       = 0x00200062;
    const DWORD    DRIVE2_STOP_HOMING                                = 0x00200063;
    const DWORD    DRIVE2_DEFINE_POSITION                            = 0x00200064;
    const DWORD    DRIVE2_GET_HOMING_STATE							 = 0x00200065;

//InputOutputs
    //Position Compare
    const DWORD    DRIVE2_SET_POSITION_COMPARE_PARAMETER             = 0x00200080;
    const DWORD    DRIVE2_GET_POSITION_COMPARE_PARAMETER             = 0x00200081;
    const DWORD    DRIVE2_ACTIVATE_POSITION_COMPARE                  = 0x00200082;
    const DWORD    DRIVE2_DEACTIVATE_POSITION_COMPARE                = 0x00200083;
    const DWORD    DRIVE2_ENABLE_POSITION_COMPARE                    = 0x00200084;
    const DWORD    DRIVE2_DISABLE_POSITION_COMPARE                   = 0x00200085;
    const DWORD    DRIVE2_SET_POSITION_COMPARE_REFERENCE_POSITION    = 0x00200086;

    //Position Marker
    const DWORD    DRIVE2_SET_POSITION_MARKER_PARAMETER              = 0x00200090;
    const DWORD    DRIVE2_GET_POSITION_MARKER_PARAMETER              = 0x00200091;
    const DWORD    DRIVE2_ACTIVATE_POSITION_MARKER                   = 0x00200092;
    const DWORD    DRIVE2_DEACTIVATE_POSITION_MARKER                 = 0x00200093;
    const DWORD    DRIVE2_READ_POSITION_MARKER_COUNTER               = 0x00200094;
    const DWORD    DRIVE2_READ_POSITION_MARKER_CAPTURED_POSITION     = 0x00200095;
    const DWORD    DRIVE2_RESET_POSITION_MARKER_COUNTER              = 0x00200096;

//MotionInfo
    const DWORD    DRIVE2_GET_CURRENT_IS                             = 0x00200100;
    const DWORD    DRIVE2_GET_MOVEMENT_STATE                         = 0x00200101;
    const DWORD    DRIVE2_GET_POSITION_IS                            = 0x00200102;
    const DWORD    DRIVE2_GET_VELOCITY_IS                            = 0x00200103;
    const DWORD    DRIVE2_GET_VELOCITY_IS_AVERAGED                   = 0x00200104;
    const DWORD    DRIVE2_GET_CURRENT_IS_AVERAGED                    = 0x00200105;

//PositionMode
    const DWORD    DRIVE2_GET_POSITION_MUST                          = 0x00200110;
    const DWORD    DRIVE2_SET_POSITION_MUST                          = 0x00200111;
    const DWORD    DRIVE2_ACTIVATE_ANALOG_POSITION_SETPOINT          = 0x00200112;
    const DWORD    DRIVE2_DEACTIVATE_ANALOG_POSITION_SETPOINT        = 0x00200113;
    const DWORD    DRIVE2_ENABLE_ANALOG_POSITION_SETPOINT            = 0x00200114;
    const DWORD    DRIVE2_DISABLE_ANALOG_POSITION_SETPOINT           = 0x00200115;

//ProfilePositionMode
    const DWORD    DRIVE2_GET_POSITION_PROFILE                       = 0x00200120;
    const DWORD    DRIVE2_GET_TARGET_POSITION                        = 0x00200121;
    const DWORD    DRIVE2_HALT_POSITION_MOVEMENT                     = 0x00200122;
    const DWORD    DRIVE2_MOVE_TO_POSITION                           = 0x00200123;
    const DWORD    DRIVE2_SET_POSITION_PROFILE                       = 0x00200124;
    const DWORD    DRIVE2_ENABLE_POSITION_WINDOW                     = 0x00200125;
    const DWORD    DRIVE2_DISABLE_POSITION_WINDOW                    = 0x00200126;

//ProfileVelocityMode
    const DWORD    DRIVE2_GET_TARGET_VELOCITY                        = 0x00200130;
    const DWORD    DRIVE2_GET_VELOCITY_PROFILE                       = 0x00200131;
    const DWORD    DRIVE2_HALT_VELOCITY_MOVEMENT                     = 0x00200132;
    const DWORD    DRIVE2_MOVE_WITH_VELOCITY                         = 0x00200133;
    const DWORD    DRIVE2_SET_VELOCITY_PROFILE                       = 0x00200134;
    const DWORD    DRIVE2_ENABLE_VELOCITY_WINDOW                     = 0x00200135;
    const DWORD    DRIVE2_DISABLE_VELOCITY_WINDOW                    = 0x00200136;

//Interpolated Position Mode
    const DWORD    DRIVE2_SET_IPM_BUFFER_PARAMETER                   = 0x00200140;
    const DWORD    DRIVE2_GET_IPM_BUFFER_PARAMETER                   = 0x00200141;
    const DWORD    DRIVE2_CLEAR_IPM_BUFFER                           = 0x00200142;
    const DWORD    DRIVE2_GET_FREE_IPM_BUFFER_SIZE                   = 0x00200143;
    const DWORD    DRIVE2_ADD_PVT_VALUE_TO_IPM_BUFFER                = 0x00200144;
    const DWORD    DRIVE2_START_IPM_TRAJECTORY                       = 0x00200145;
    const DWORD    DRIVE2_STOP_IPM_TRAJECTORY                        = 0x00200146;
    const DWORD    DRIVE2_GET_IPM_STATUS                             = 0x00200147;

//MasterEncoderMode
    const DWORD    DRIVE2_SET_MASTER_ENCODER_PARAMETER               = 0x00200150;
    const DWORD    DRIVE2_GET_MASTER_ENCODER_PARAMETER               = 0x00200151;

//StepDirectionMode
    const DWORD    DRIVE2_SET_STEP_DIRECTION_PARAMETER               = 0x00200160;
    const DWORD    DRIVE2_GET_STEP_DIRECTION_PARAMETER               = 0x00200161;

//StateMachine
    const DWORD    DRIVE2_CLEAR_FAULT                                = 0x00200170;
    const DWORD    DRIVE2_GET_DISABLE_STATE                          = 0x00200171;
    const DWORD    DRIVE2_GET_ENABLE_STATE                           = 0x00200172;
    const DWORD    DRIVE2_GET_FAULT_STATE                            = 0x00200173;
    const DWORD    DRIVE2_GET_OPERATION_MODE                         = 0x00200174;
    const DWORD    DRIVE2_GET_QUICK_STOP_STATE                       = 0x00200175;
    const DWORD    DRIVE2_SET_DISABLE_STATE                          = 0x00200176;
    const DWORD    DRIVE2_SET_ENABLE_STATE                           = 0x00200177;
    const DWORD    DRIVE2_SET_OPERATION_MODE                         = 0x00200178;
    const DWORD    DRIVE2_SET_QUICK_STOP_STATE                       = 0x00200179;

//VelocityMode
    const DWORD    DRIVE2_GET_VELOCITY_MUST                          = 0x00200200;
    const DWORD    DRIVE2_SET_VELOCITY_MUST                          = 0x00200201;
    const DWORD    DRIVE2_ACTIVATE_ANALOG_VELOCITY_SETPOINT          = 0x00200202;
    const DWORD    DRIVE2_DEACTIVATE_ANALOG_VELOCITY_SETPOINT        = 0x00200203;
    const DWORD    DRIVE2_ENABLE_ANALOG_VELOCITY_SETPOINT            = 0x00200204;
    const DWORD    DRIVE2_DISABLE_ANALOG_VELOCITY_SETPOINT           = 0x00200205;

//FirmwareDownload
    const DWORD    DRIVE2_GET_BASE_SECTOR_VERSION                    = 0x00200220;
    const DWORD    DRIVE2_GET_SERIAL_NUMBER                          = 0x00200221;


#endif    //Command_VCS_Drive2_Definitions_07102003

