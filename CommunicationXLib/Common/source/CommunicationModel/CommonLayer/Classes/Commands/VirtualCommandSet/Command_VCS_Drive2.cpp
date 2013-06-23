// Command_VCS_Drive2.cpp: Implementierung der Klasse CCommand_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_InputsOutputs.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommand_VCS_Drive2::CCommand_VCS_Drive2()
{
}

CCommand_VCS_Drive2::CCommand_VCS_Drive2(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_Drive2::~CCommand_VCS_Drive2()
{
}

CCommandRoot*CCommand_VCS_Drive2::CloneCommand()
{
    CCommand_VCS_Drive2* pNewCommand;

    pNewCommand = new CCommand_VCS_Drive2();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_Drive2& CCommand_VCS_Drive2::operator=(CCommand_VCS_Drive2& p_rOther)
{
    if(this != &p_rOther)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&p_rOther);
    }

    return *this;
}

BOOL CCommand_VCS_Drive2::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_Configuration(p_ulCommandId)) return TRUE;
    if(InitCommand_ConfigurationMotor(p_ulCommandId)) return TRUE;
    if(InitCommand_ConfigurationSensor(p_ulCommandId)) return TRUE;
    if(InitCommand_CurrentMode(p_ulCommandId)) return TRUE;
    if(InitCommand_HomingMode(p_ulCommandId)) return TRUE;
    if(InitCommand_InputsOutputs(p_ulCommandId)) return TRUE;
    if(InitCommand_MotionInfo(p_ulCommandId)) return TRUE;
    if(InitCommand_PositionMode(p_ulCommandId)) return TRUE;
    if(InitCommand_ProfilePositionMode(p_ulCommandId)) return TRUE;
    if(InitCommand_InterpolatedPositionMode(p_ulCommandId)) return TRUE;
    if(InitCommand_ProfileVelocityMode(p_ulCommandId)) return TRUE;
    if(InitCommand_StateMachine(p_ulCommandId)) return TRUE;
    if(InitCommand_VelocityMode(p_ulCommandId)) return TRUE;
    if(InitCommand_FirmwareDownload(p_ulCommandId)) return TRUE;
    if(InitCommand_MasterEncoderMode(p_ulCommandId)) return TRUE;
    if(InitCommand_StepDirectionMode(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_Configuration(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_CURRENT_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("GetCurrentRegulatorGain", DRIVE2_GET_CURRENT_REGULATOR_GAIN);
                AddReturnParameter(0, "P", ODT_UINT16);
                AddReturnParameter(1, "I", ODT_UINT16);
                SetDefaultParameter_GetCurrentRegulatorGain();
                return TRUE;
            };
        case DRIVE2_GET_POSITION_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("GetPositionRegulatorGain", DRIVE2_GET_POSITION_REGULATOR_GAIN);
                AddReturnParameter(0, "P", ODT_UINT16);
                AddReturnParameter(1, "I", ODT_UINT16);
                AddReturnParameter(2, "D", ODT_UINT16);
                SetDefaultParameter_GetPositionRegulatorGain();
                return TRUE;
            };
        case DRIVE2_GET_POSITION_REGULATOR_FEED_FORWARD:
            {
                CCommand_VCS::InitCommand("GetPositionRegulatorFeedForward", DRIVE2_GET_POSITION_REGULATOR_FEED_FORWARD);
                AddReturnParameter(0, "velocityFeedForward", ODT_UINT16);
                AddReturnParameter(1, "accelerationFeedForward", ODT_UINT16);
                SetDefaultParameter_GetPositionRegulatorFeedForward();
                return TRUE;
            };
        case DRIVE2_GET_VELOCITY_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("GetVelocityRegulatorGain", DRIVE2_GET_VELOCITY_REGULATOR_GAIN);
                AddReturnParameter(0, "P", ODT_UINT16);
                AddReturnParameter(1, "I", ODT_UINT16);
                SetDefaultParameter_GetVelocityRegulatorGain();
                return TRUE;
            };
        case DRIVE2_GET_VELOCITY_REGULATOR_FEED_FORWARD:
            {
                CCommand_VCS::InitCommand("GetVelocityRegulatorFeedForward", DRIVE2_GET_VELOCITY_REGULATOR_FEED_FORWARD);
                AddReturnParameter(0, "velocityFeedForward", ODT_UINT16);
                AddReturnParameter(1, "accelerationFeedForward", ODT_UINT16);
                SetDefaultParameter_GetVelocityRegulatorFeedForward();
                return TRUE;
            };
        case DRIVE2_GET_MAX_FOLLOWING_ERROR:
            {
                CCommand_VCS::InitCommand("GetMaxFollowingError", DRIVE2_GET_MAX_FOLLOWING_ERROR);
                AddReturnParameter(0, "MaxFollowingError", ODT_UINT32);
                SetDefaultParameter_GetMaxFollowingError();
                return TRUE;
            };
        case DRIVE2_GET_MAX_PROFILE_VELOCITY:
            {
                CCommand_VCS::InitCommand("GetMaxProfileVelocity", DRIVE2_GET_MAX_PROFILE_VELOCITY);
                AddReturnParameter(0, "MaxProfileVelocity", ODT_UINT32);
                SetDefaultParameter_GetMaxProfileVelocity();
                return TRUE;
            };
        case DRIVE2_GET_MAX_ACCELERATION:
            {
                CCommand_VCS::InitCommand("GetMaxAcceleration", DRIVE2_GET_MAX_ACCELERATION);
                AddReturnParameter(0, "MaxAcceleration", ODT_UINT32);
                SetDefaultParameter_GetMaxAcceleration();
                return TRUE;
            };
        case DRIVE2_GET_VELOCITY_UNITS:
            {
                CCommand_VCS::InitCommand("GetVelocityUnits", DRIVE2_GET_VELOCITY_UNITS);
                AddReturnParameter(0, "velocityDimension", ODT_UINT8);
                AddReturnParameter(1, "velocityNotation", ODT_UINT8);
                SetDefaultParameter_GetVelocityUnits();
                return TRUE;
            };
        case DRIVE2_SET_CURRENT_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("SetCurrentRegulatorGain", DRIVE2_SET_CURRENT_REGULATOR_GAIN);
                AddParameter(0, "P", ODT_UINT16);
                AddParameter(1, "I", ODT_UINT16);
                SetDefaultParameter_SetCurrentRegulatorGain();
                return TRUE;
            };
        case DRIVE2_SET_POSITION_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("SetPositionRegulatorGain", DRIVE2_SET_POSITION_REGULATOR_GAIN);
                AddParameter(0, "P", ODT_UINT16);
                AddParameter(1, "I", ODT_UINT16);
                AddParameter(2, "D", ODT_UINT16);
                SetDefaultParameter_SetPositionRegulatorGain();
                return TRUE;
            };
        case DRIVE2_SET_POSITION_REGULATOR_FEED_FORWARD:
            {
                CCommand_VCS::InitCommand("SetPositionRegulatorFeedForward", DRIVE2_SET_POSITION_REGULATOR_FEED_FORWARD);
                AddParameter(0, "velocityFeedForward", ODT_UINT16);
                AddParameter(1, "accelerationFeedForward", ODT_UINT16);
                SetDefaultParameter_SetPositionRegulatorFeedForward();
                return TRUE;
            };
        case DRIVE2_SET_VELOCITY_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("SetVelocityRegulatorGain", DRIVE2_SET_VELOCITY_REGULATOR_GAIN);
                AddParameter(0, "P", ODT_UINT16);
                AddParameter(1, "I", ODT_UINT16);
                SetDefaultParameter_SetVelocityRegulatorGain();
                return TRUE;
            };
        case DRIVE2_SET_VELOCITY_REGULATOR_FEED_FORWARD:
            {
                CCommand_VCS::InitCommand("SetVelocityRegulatorFeedForward", DRIVE2_SET_VELOCITY_REGULATOR_FEED_FORWARD);
                AddParameter(0, "velocityFeedForward", ODT_UINT16);
                AddParameter(1, "accelerationFeedForward", ODT_UINT16);
                SetDefaultParameter_SetVelocityRegulatorFeedForward();
                return TRUE;
            };
        case DRIVE2_SET_MAX_FOLLOWING_ERROR:
            {
                CCommand_VCS::InitCommand("SetMaxFollowingError", DRIVE2_SET_MAX_FOLLOWING_ERROR);
                AddParameter(0, "MaxFollowingError", ODT_UINT32);
                SetDefaultParameter_SetMaxFollowingError();
                return TRUE;
            };
        case DRIVE2_SET_MAX_PROFILE_VELOCITY:
            {
                CCommand_VCS::InitCommand("SetMaxProfileVelocity", DRIVE2_SET_MAX_PROFILE_VELOCITY);
                AddParameter(0, "MaxProfileVelocity", ODT_UINT32);
                SetDefaultParameter_SetMaxProfileVelocity();
                return TRUE;
            };
        case DRIVE2_SET_MAX_ACCELERATION:
            {
                CCommand_VCS::InitCommand("SetMaxAcceleration", DRIVE2_SET_MAX_ACCELERATION);
                AddParameter(0, "MaxAcceleration", ODT_UINT32);
                SetDefaultParameter_SetMaxAcceleration();
                return TRUE;
            };
        case DRIVE2_SET_VELOCITY_UNITS:
            {
                CCommand_VCS::InitCommand("SetVelocityUnits", DRIVE2_SET_VELOCITY_UNITS);
                AddParameter(0, "velocityDimensionIndex", ODT_UINT8);
                AddParameter(1, "velocityNotationIndex", ODT_INT8);
                SetDefaultParameter_SetVelocityUnits();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_ConfigurationMotor(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetMotorParameter", DRIVE2_GET_MOTOR_PARAMETER);
                AddReturnParameter(0, "motorType", ODT_UINT16);
                AddReturnParameter(1, "continuousCurrent", ODT_UINT16);
                AddReturnParameter(2, "peakCurrent", ODT_UINT16);
                AddReturnParameter(3, "polePair", ODT_UINT8);
                AddReturnParameter(4, "thermalTimeConstant", ODT_UINT16);
                SetDefaultParameter_GetMotorParameter();
                return TRUE;
            };
        case DRIVE2_GET_MOTOR_TYPE:
            {
                CCommand_VCS::InitCommand("GetMotorType", DRIVE2_GET_MOTOR_TYPE);
                AddReturnParameter(0, "motorType", ODT_UINT16);
                SetDefaultParameter_GetMotorType();
                return TRUE;
            };
        case DRIVE2_GET_DC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetDcMotorParameter", DRIVE2_GET_DC_MOTOR_PARAMETER);
                AddReturnParameter(0, "nominalCurrent", ODT_UINT16);
                AddReturnParameter(1, "maxOutputCurrent", ODT_UINT16);
                AddReturnParameter(2, "thermalTimeConstant", ODT_UINT16);
                SetDefaultParameter_GetDcMotorParameter();
                return TRUE;
            };
        case DRIVE2_GET_EC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetEcMotorParameter", DRIVE2_GET_EC_MOTOR_PARAMETER);
                AddReturnParameter(0, "nominalCurrent", ODT_UINT16);
                AddReturnParameter(1, "maxOutputCurrent", ODT_UINT16);
                AddReturnParameter(2, "thermalTimeConstant", ODT_UINT16);
                AddReturnParameter(3, "nbOfPolePairs", ODT_UINT8);
                SetDefaultParameter_GetEcMotorParameter();
                return TRUE;
            };
        case DRIVE2_SET_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetMotorParameter", DRIVE2_SET_MOTOR_PARAMETER);
                AddParameter(0, "motorType", ODT_UINT16);
                AddParameter(1, "continuousCurrent", ODT_UINT16);
                AddParameter(2, "peakCurrent", ODT_UINT16);
                AddParameter(3, "polePair", ODT_UINT8);
                AddParameter(4, "thermalTimeConstant", ODT_UINT16);
                SetDefaultParameter_SetMotorParameter();
                return TRUE;
            };
        case DRIVE2_SET_MOTOR_TYPE:
            {
                CCommand_VCS::InitCommand("SetMotorType", DRIVE2_SET_MOTOR_TYPE);
                AddParameter(0, "motorType", ODT_UINT16);
                SetDefaultParameter_SetMotorType();
                return TRUE;
            };
        case DRIVE2_SET_DC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetDcMotorParameter", DRIVE2_SET_DC_MOTOR_PARAMETER);
                AddParameter(0, "nominalCurrent", ODT_UINT16);
                AddParameter(1, "maxOutputCurrent", ODT_UINT16);
                AddParameter(2, "thermalTimeConstant", ODT_UINT16);
                SetDefaultParameter_SetDcMotorParameter();
                return TRUE;
            };
        case DRIVE2_SET_EC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetEcMotorParameter", DRIVE2_SET_EC_MOTOR_PARAMETER);
                AddParameter(0, "nominalCurrent", ODT_UINT16);
                AddParameter(1, "maxOutputCurrent", ODT_UINT16);
                AddParameter(2, "thermalTimeConstant", ODT_UINT16);
                AddParameter(3, "nbOfPolePairs", ODT_UINT8);
                SetDefaultParameter_SetEcMotorParameter();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_ConfigurationSensor(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetEncoderParameter", DRIVE2_GET_ENCODER_PARAMETER);
                AddReturnParameter(0, "counts", ODT_UINT16);
                AddReturnParameter(1, "positionSensorType", ODT_UINT16);
                SetDefaultParameter_GetEncoderParameter();
                return TRUE;
            };
        case DRIVE2_GET_SENSOR_TYPE:
            {
                CCommand_VCS::InitCommand("GetSensorType", DRIVE2_GET_SENSOR_TYPE);
                AddReturnParameter(0, "sensorType", ODT_UINT16);
                SetDefaultParameter_GetSensorType();
                return TRUE;
            };
        case DRIVE2_GET_INC_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetIncEncoderParameter", DRIVE2_GET_INC_ENCODER_PARAMETER);
                AddReturnParameter(0, "encoderResolution", ODT_UINT32);
                AddReturnParameter(1, "invertedPolarity", ODT_BOOLEAN);
                SetDefaultParameter_GetIncEncoderParameter();
                return TRUE;
            };
        case DRIVE2_GET_HALL_SENSOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetEncoderParameter", DRIVE2_GET_HALL_SENSOR_PARAMETER);
                AddReturnParameter(0, "invertedPolarity", ODT_BOOLEAN);
                SetDefaultParameter_GetHallSensorParameter();
                return TRUE;
            };
        case DRIVE2_GET_SSI_ABS_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetSsiAbsEncoderParameter", DRIVE2_GET_SSI_ABS_ENCODER_PARAMETER);
                AddReturnParameter(0, "dataRate", ODT_UINT16);
                AddReturnParameter(1, "nbOfMultiTurnDataBits", ODT_UINT16);
                AddReturnParameter(2, "nbOfSingleTurnDataBits", ODT_UINT16);
                AddReturnParameter(3, "invertedPolarity", ODT_BOOLEAN);
                SetDefaultParameter_GetSsiAbsEncoderParameter();
                return TRUE;
            };
        case DRIVE2_SET_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetEncoderParameter", DRIVE2_SET_ENCODER_PARAMETER);
                AddParameter(0, "counts", ODT_UINT16);
                AddParameter(1, "positionSensorType", ODT_UINT16);
                SetDefaultParameter_SetEncoderParameter();
                return TRUE;
            };
        case DRIVE2_SET_SENSOR_TYPE:
            {
                CCommand_VCS::InitCommand("SetSensorType", DRIVE2_SET_SENSOR_TYPE);
                AddParameter(0, "sensorType", ODT_UINT16);
                SetDefaultParameter_SetSensorType();
                return TRUE;
            };
        case DRIVE2_SET_INC_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetIncEncoderParameter", DRIVE2_SET_INC_ENCODER_PARAMETER);
                AddParameter(0, "encoderResolution", ODT_UINT32);
                AddParameter(1, "invertedPolarity", ODT_BOOLEAN);
                SetDefaultParameter_SetIncEncoderParameter();
                return TRUE;
            };
        case DRIVE2_SET_HALL_SENSOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetEncoderParameter", DRIVE2_SET_HALL_SENSOR_PARAMETER);
                AddParameter(0, "invertedPolarity", ODT_BOOLEAN);
                SetDefaultParameter_SetHallSensorParameter();
                return TRUE;
            };
        case DRIVE2_SET_SSI_ABS_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetSsiAbsEncoderParameter", DRIVE2_SET_SSI_ABS_ENCODER_PARAMETER);
                AddParameter(0, "dataRate", ODT_UINT16);
                AddParameter(1, "nbOfMultiTurnDataBits", ODT_UINT16);
                AddParameter(2, "nbOfSingleTurnDataBits", ODT_UINT16);
                AddParameter(3, "invertedPolarity", ODT_BOOLEAN);
                SetDefaultParameter_SetSsiAbsEncoderParameter();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_CurrentMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_CURRENT_MUST:
            {
                CCommand_VCS::InitCommand("GetCurrentMust", DRIVE2_GET_CURRENT_MUST);
                AddReturnParameter(0, "currentMust", ODT_INT16);
                SetDefaultParameter_GetCurrentMust();
                return TRUE;
            };
        case DRIVE2_SET_CURRENT_MUST:
            {
                CCommand_VCS::InitCommand("SetCurrentMust", DRIVE2_SET_CURRENT_MUST);
                AddParameter(0, "currentMust", ODT_INT16);
                SetDefaultParameter_SetCurrentMust();
                return TRUE;
            };
        case DRIVE2_ACTIVATE_ANALOG_CURRENT_SETPOINT:
            {
                CCommand_VCS::InitCommand("ActivateAnalogCurrentSetpoint", DRIVE2_ACTIVATE_ANALOG_CURRENT_SETPOINT);
                AddParameter(0, "analogInputNumber", ODT_UINT16);
                AddParameter(1, "scaling", ODT_FLOAT);
                AddParameter(2, "offset", ODT_INT16);
                SetDefaultParameter_ActivateAnalogCurrentSetpoint();
                return TRUE;
            };
        case DRIVE2_DEACTIVATE_ANALOG_CURRENT_SETPOINT:
            {
                CCommand_VCS::InitCommand("DeactivateAnalogCurrentSetpoint", DRIVE2_DEACTIVATE_ANALOG_CURRENT_SETPOINT);
                AddParameter(0, "analogInputNumber", ODT_UINT16);
                SetDefaultParameter_DeactivateAnalogCurrentSetpoint();
                return TRUE;
            };
        case DRIVE2_ENABLE_ANALOG_CURRENT_SETPOINT:
            {
                CCommand_VCS::InitCommand("EnableAnalogCurrentSetpoint", DRIVE2_ENABLE_ANALOG_CURRENT_SETPOINT);
                SetDefaultParameter_EnableAnalogCurrentSetpoint();
                return TRUE;
            };
        case DRIVE2_DISABLE_ANALOG_CURRENT_SETPOINT:
            {
                CCommand_VCS::InitCommand("DisableAnalogCurrentSetpoint", DRIVE2_DISABLE_ANALOG_CURRENT_SETPOINT);
                SetDefaultParameter_DisableAnalogCurrentSetpoint();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_FirmwareDownload(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_BASE_SECTOR_VERSION:
            {
                CCommand_VCS::InitCommand("GetBaseSectorVersion", DRIVE2_GET_BASE_SECTOR_VERSION);
                AddReturnParameter(0, "baseSectorVersion", ODT_UINT16);
                SetDefaultParameter_GetBaseSectorVersion();
                return TRUE;
            };
        case DRIVE2_GET_SERIAL_NUMBER:
            {
                CCommand_VCS::InitCommand("GetSerialNumber", DRIVE2_GET_SERIAL_NUMBER);
                AddReturnParameter(0, "serialNumber", ODT_UINT64);
                SetDefaultParameter_GetSerialNumber();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_HomingMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_FIND_HOME:
            {
                CCommand_VCS::InitCommand("FindHome", DRIVE2_FIND_HOME);
                AddParameter(0, "homingMethod", ODT_INT8);
                SetDefaultParameter_FindHome();
                return TRUE;
            };
        case DRIVE2_GET_HOMING_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetHomingParameter", DRIVE2_GET_HOMING_PARAMETER);
                AddReturnParameter(0, "homingAcceleration", ODT_UINT32);
                AddReturnParameter(1, "speedSwitch", ODT_UINT32);
                AddReturnParameter(2, "speedIndex", ODT_UINT32);
                AddReturnParameter(3, "homeOffset", ODT_INT32);
                AddReturnParameter(4, "currentThreshold", ODT_UINT16);
                AddReturnParameter(5, "homePosition", ODT_INT32);
                SetDefaultParameter_GetHomingParameter();
                return TRUE;
            };
        case DRIVE2_SET_HOMING_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetHomingParameter", DRIVE2_SET_HOMING_PARAMETER);
                AddParameter(0, "homingAcceleration", ODT_UINT32);
                AddParameter(1, "speedSwitch", ODT_UINT32);
                AddParameter(2, "speedIndex", ODT_UINT32);
                AddParameter(3, "homeOffset", ODT_INT32);
                AddParameter(4, "currentThreshold", ODT_UINT16);
                AddParameter(5, "homePosition", ODT_INT32);
                SetDefaultParameter_SetHomingParameter();
                return TRUE;
            };
        case DRIVE2_STOP_HOMING:
            {
                CCommand_VCS::InitCommand("StopHoming", DRIVE2_STOP_HOMING);
                SetDefaultParameter_StopHoming();
                return TRUE;
            };
        case DRIVE2_DEFINE_POSITION:
            {
                CCommand_VCS::InitCommand("DefinePosition", DRIVE2_DEFINE_POSITION);
                AddParameter(0, "homePosition", ODT_INT32);
                SetDefaultParameter_DefinePosition();
                return TRUE;
            }
       case DRIVE2_GET_HOMING_STATE:
            {
                CCommand_VCS::InitCommand("GetHomingState", DRIVE2_GET_HOMING_STATE);
                AddReturnParameter(0, "homingAttained", ODT_BOOLEAN);
                AddReturnParameter(1, "homingError", ODT_BOOLEAN);
                SetDefaultParameter_GetHomingState();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_InputsOutputs(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case IO_DIGITAL_INPUT_CONFIGURATION:
            {
                CCommand_VCS::InitCommand("DigitalInputConfiguration", IO_DIGITAL_INPUT_CONFIGURATION);
                AddParameter(0, "inputNb", ODT_UINT16);
                AddParameter(1, "configuration", ODT_UINT16);
                AddParameter(2, "mask", ODT_BOOLEAN);
                AddParameter(3, "polarity", ODT_BOOLEAN);
                AddParameter(4, "executionMask", ODT_BOOLEAN);
                SetDefaultParameter_DigitalInputConfiguration();
                return TRUE;
            };
        case IO_DIGITAL_OUTPUT_CONFIGURATION:
            {
                CCommand_VCS::InitCommand("DigitalOutputConfiguration", IO_DIGITAL_OUTPUT_CONFIGURATION);
                AddParameter(0, "outputNb", ODT_UINT16);
                AddParameter(1, "configuration", ODT_UINT16);
                AddParameter(2, "state", ODT_BOOLEAN);
                AddParameter(3, "mask", ODT_BOOLEAN);
                AddParameter(4, "polarity", ODT_BOOLEAN);
                SetDefaultParameter_DigitalOutputConfiguration();
                return TRUE;
            };
        case IO_ANALOG_INPUT_CONFIGURATION:
            {
                CCommand_VCS::InitCommand("AnalogInputConfiguration", IO_ANALOG_INPUT_CONFIGURATION);
                AddParameter(0, "inputNb", ODT_UINT16);
                AddParameter(1, "configuration", ODT_UINT16);
                AddParameter(2, "executionMask", ODT_BOOLEAN);
                SetDefaultParameter_AnalogInputConfiguration();
                return TRUE;
            };
        case IO_GET_ALL_DIGITAL_INPUTS:
            {
                CCommand_VCS::InitCommand("GetAllDigitalInputs", IO_GET_ALL_DIGITAL_INPUTS);
                AddReturnParameter(0, "inputs", ODT_UINT16);
                SetDefaultParameter_GetAllDigitalInputs();
                return TRUE;
            };
        case IO_GET_ALL_DIGITAL_OUTPUTS:
            {
                CCommand_VCS::InitCommand("GetAllDigitalOutputs", IO_GET_ALL_DIGITAL_OUTPUTS);
                AddReturnParameter(0, "outputs", ODT_UINT16);
                SetDefaultParameter_GetAllDigitalOutputs();
                return TRUE;
            };
        case IO_GET_ANALOG_INPUT:
            {
                CCommand_VCS::InitCommand("GetAnalogInput", IO_GET_ANALOG_INPUT);
                AddParameter(0, "number", ODT_UINT16);
                AddReturnParameter(0, "analog", ODT_UINT16);
                SetDefaultParameter_GetAnalogInput();
                return TRUE;
            };
        case IO_SET_ALL_DIGITAL_OUTPUTS:
            {
                CCommand_VCS::InitCommand("SetAllDigitalOutputs", IO_SET_ALL_DIGITAL_OUTPUTS);
                AddParameter(0, "outputs", ODT_UINT16);
                SetDefaultParameter_SetAllDigitalOutputs();
                return TRUE;
            };
        case IO_SET_ANALOG_OUTPUT:
            {
                CCommand_VCS::InitCommand("SetAnalogOutput", IO_SET_ANALOG_OUTPUT);
                AddParameter(0, "number", ODT_UINT16);
                AddParameter(1, "outputValue", ODT_UINT16);
                SetDefaultParameter_SetAnalogOutput();
                return TRUE;
            };
        case DRIVE2_SET_POSITION_COMPARE_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetPositionCompareParameter", DRIVE2_SET_POSITION_COMPARE_PARAMETER);
                AddParameter(0, "operationalMode", ODT_UINT8);
                AddParameter(1, "intervalMode", ODT_UINT8);
                AddParameter(2, "directionDependency", ODT_UINT8);
                AddParameter(3, "intervalWidth", ODT_UINT16);
                AddParameter(4, "intervalRepetitions", ODT_UINT16);
                AddParameter(5, "pulseWidth", ODT_UINT16);
                SetDefaultParameter_SetPositionCompareParameter();
                return TRUE;
            };
        case DRIVE2_GET_POSITION_COMPARE_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetPositionCompareParameter", DRIVE2_GET_POSITION_COMPARE_PARAMETER);
                AddReturnParameter(0, "operationalMode", ODT_UINT8);
                AddReturnParameter(1, "intervalMode", ODT_UINT8);
                AddReturnParameter(2, "directionDependency", ODT_UINT8);
                AddReturnParameter(3, "intervalWidth", ODT_UINT16);
                AddReturnParameter(4, "intervalRepetitions", ODT_UINT16);
                AddReturnParameter(5, "pulseWidth", ODT_UINT16);
                SetDefaultParameter_GetPositionCompareParameter();
                return TRUE;
            };
        case DRIVE2_ACTIVATE_POSITION_COMPARE:
            {
                CCommand_VCS::InitCommand("ActivatePositionCompare", DRIVE2_ACTIVATE_POSITION_COMPARE);
                AddParameter(0, "number", ODT_UINT16);
                AddParameter(1, "polarity", ODT_BOOLEAN);
                SetDefaultParameter_ActivatePositionCompare();
                return TRUE;
            };
        case DRIVE2_DEACTIVATE_POSITION_COMPARE:
            {
                CCommand_VCS::InitCommand("DeactivatePositionCompare", DRIVE2_DEACTIVATE_POSITION_COMPARE);
                AddParameter(0, "number", ODT_UINT16);
                SetDefaultParameter_DeactivatePositionCompare();
                return TRUE;
            };
        case DRIVE2_ENABLE_POSITION_COMPARE:
            {
                CCommand_VCS::InitCommand("EnablePositionCompare", DRIVE2_ENABLE_POSITION_COMPARE);
                SetDefaultParameter_EnablePositionCompare();
                return TRUE;
            };
        case DRIVE2_DISABLE_POSITION_COMPARE:
            {
                CCommand_VCS::InitCommand("DisablePositionCompare", DRIVE2_DISABLE_POSITION_COMPARE);
                SetDefaultParameter_DisablePositionCompare();
                return TRUE;
            };
        case DRIVE2_SET_POSITION_COMPARE_REFERENCE_POSITION:
            {
                CCommand_VCS::InitCommand("SetPositionCompareReferencePosition", DRIVE2_SET_POSITION_COMPARE_REFERENCE_POSITION);
                AddParameter(0, "referencePosition", ODT_INT32);
                SetDefaultParameter_SetPositionCompareReferencePosition();
                return TRUE;
            };
        case DRIVE2_SET_POSITION_MARKER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetPositionMarkerParameter", DRIVE2_SET_POSITION_MARKER_PARAMETER);
                AddParameter(0, "number", ODT_UINT16);
                AddParameter(1, "positionMarkerEdgeType", ODT_UINT8);
                AddParameter(2, "positionMarkerMode", ODT_UINT8);
                SetDefaultParameter_SetPositionMarkerParameter();
                return TRUE;
            };
        case DRIVE2_GET_POSITION_MARKER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetPositionMarkerParameter", DRIVE2_GET_POSITION_MARKER_PARAMETER);
                AddReturnParameter(0, "positionMarkerEdgeType", ODT_UINT8);
                AddReturnParameter(1, "positionMarkerMode", ODT_UINT8);
                SetDefaultParameter_GetPositionMarkerParameter();
                return TRUE;
            };
        case DRIVE2_ACTIVATE_POSITION_MARKER:
            {
                CCommand_VCS::InitCommand("ActivatePositionMarker", DRIVE2_ACTIVATE_POSITION_MARKER);
                AddParameter(0, "number", ODT_UINT16);
                AddParameter(1, "polarity", ODT_BOOLEAN);
                SetDefaultParameter_ActivatePositionMarker();
                return TRUE;
            };
        case DRIVE2_DEACTIVATE_POSITION_MARKER:
            {
                CCommand_VCS::InitCommand("DeactivatePositionMarker", DRIVE2_DEACTIVATE_POSITION_MARKER);
                AddParameter(0, "number", ODT_UINT16);
                SetDefaultParameter_DeactivatePositionMarker();
                return TRUE;
            };
        case DRIVE2_READ_POSITION_MARKER_COUNTER:
            {
                CCommand_VCS::InitCommand("ReadPositionMarkerCounter", DRIVE2_READ_POSITION_MARKER_COUNTER);
                AddReturnParameter(0, "counter", ODT_UINT16);
                SetDefaultParameter_ReadPositionMarkerCounter();
                return TRUE;
            };
        case DRIVE2_READ_POSITION_MARKER_CAPTURED_POSITION:
            {
                CCommand_VCS::InitCommand("ReadPositionMarkerCapturedPosition", DRIVE2_READ_POSITION_MARKER_CAPTURED_POSITION);
                AddParameter(0, "counterIndex", ODT_UINT16);
                AddReturnParameter(0, "capturedPosition", ODT_INT32);
                SetDefaultParameter_ReadPositionMarkerCapturedPosition();
                return TRUE;
            };
        case DRIVE2_RESET_POSITION_MARKER_COUNTER:
            {
                CCommand_VCS::InitCommand("ResetPositionMarkerCounter", DRIVE2_RESET_POSITION_MARKER_COUNTER);
                SetDefaultParameter_ResetPositionMarkerCounter();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_MotionInfo(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_CURRENT_IS:
            {
                CCommand_VCS::InitCommand("GetCurrentIs", DRIVE2_GET_CURRENT_IS);
                AddReturnParameter(0, "currentIs", ODT_INT16);
                SetDefaultParameter_GetCurrentIs();
                return TRUE;
            };
        case DRIVE2_GET_CURRENT_IS_AVERAGED:
            {
                CCommand_VCS::InitCommand("GetCurrentIsAveraged", DRIVE2_GET_CURRENT_IS_AVERAGED);
                AddReturnParameter(0, "CurrentIsAveraged", ODT_INT16);
                SetDefaultParameter_GetCurrentIsAveraged();
                return TRUE;
            };
        case DRIVE2_GET_MOVEMENT_STATE:
            {
                CCommand_VCS::InitCommand("GetMovementState", DRIVE2_GET_MOVEMENT_STATE);
                AddReturnParameter(0, "targetReached", ODT_BOOLEAN);
                SetDefaultParameter_GetMovementState();
                return TRUE;
            };
        case DRIVE2_GET_POSITION_IS:
            {
                CCommand_VCS::InitCommand("GetPositionIs", DRIVE2_GET_POSITION_IS);
                AddReturnParameter(0, "positionIs", ODT_INT32);
                SetDefaultParameter_GetPositionIs();
                return TRUE;
            };
        case DRIVE2_GET_VELOCITY_IS:
            {
                CCommand_VCS::InitCommand("GetVelocityIs", DRIVE2_GET_VELOCITY_IS);
                AddReturnParameter(0, "velocityIs", ODT_INT32);
                SetDefaultParameter_GetVelocityIs();
                return TRUE;
            };
        case DRIVE2_GET_VELOCITY_IS_AVERAGED:
            {
                CCommand_VCS::InitCommand("GetVelocityIsAveraged", DRIVE2_GET_VELOCITY_IS_AVERAGED);
                AddReturnParameter(0, "VelocityIsAveraged", ODT_INT32);
                SetDefaultParameter_GetVelocityIsAveraged();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_PositionMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_POSITION_MUST:
            {
                CCommand_VCS::InitCommand("GetPositionMust", DRIVE2_GET_POSITION_MUST);
                AddReturnParameter(0, "positionMust", ODT_INT32);
                SetDefaultParameter_GetPositionMust();
                return TRUE;
            };
        case DRIVE2_SET_POSITION_MUST:
            {
                CCommand_VCS::InitCommand("SetPositionMust", DRIVE2_SET_POSITION_MUST);
                AddParameter(0, "positionMust", ODT_INT32);
                SetDefaultParameter_SetPositionMust();
                return TRUE;
            };
        case DRIVE2_ACTIVATE_ANALOG_POSITION_SETPOINT:
            {
                CCommand_VCS::InitCommand("ActivateAnalogPositionSetpoint", DRIVE2_ACTIVATE_ANALOG_POSITION_SETPOINT);
                AddParameter(0, "analogInputNumber", ODT_UINT16);
                AddParameter(1, "scaling", ODT_FLOAT);
                AddParameter(2, "offset", ODT_INT32);
                SetDefaultParameter_ActivateAnalogPositionSetpoint();
                return TRUE;
            };
        case DRIVE2_DEACTIVATE_ANALOG_POSITION_SETPOINT:
            {
                CCommand_VCS::InitCommand("DeactivateAnalogPositionSetpoint", DRIVE2_DEACTIVATE_ANALOG_POSITION_SETPOINT);
                AddParameter(0, "analogInputNumber", ODT_UINT16);
                SetDefaultParameter_DeactivateAnalogPositionSetpoint();
                return TRUE;
            };
        case DRIVE2_ENABLE_ANALOG_POSITION_SETPOINT:
            {
                CCommand_VCS::InitCommand("EnableAnalogPositionSetpoint", DRIVE2_ENABLE_ANALOG_POSITION_SETPOINT);
                SetDefaultParameter_EnableAnalogPositionSetpoint();
                return TRUE;
            };
        case DRIVE2_DISABLE_ANALOG_POSITION_SETPOINT:
            {
                CCommand_VCS::InitCommand("DisableAnalogPositionSetpoint", DRIVE2_DISABLE_ANALOG_POSITION_SETPOINT);
                SetDefaultParameter_DisableAnalogPositionSetpoint();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_ProfilePositionMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_POSITION_PROFILE:
            {
                CCommand_VCS::InitCommand("GetPositionProfile", DRIVE2_GET_POSITION_PROFILE);
                AddReturnParameter(0, "profileVelocity", ODT_UINT32);
                AddReturnParameter(1, "profileAcceleration", ODT_UINT32);
                AddReturnParameter(2, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_GetPositionProfile();
                return TRUE;
            };
        case DRIVE2_GET_TARGET_POSITION:
            {
                CCommand_VCS::InitCommand("GetTargetPosition", DRIVE2_GET_TARGET_POSITION);
                AddReturnParameter(0, "targetPosition", ODT_INT32);
                SetDefaultParameter_GetTargetPosition();
                return TRUE;
            };
        case DRIVE2_HALT_POSITION_MOVEMENT:
            {
                CCommand_VCS::InitCommand("HaltPositionMovement", DRIVE2_HALT_POSITION_MOVEMENT);
                AddParameter(0, "nodeID", ODT_UINT16, FALSE, FALSE);
                SetDefaultParameter_HaltPositionMovement();
                return TRUE;
            };
        case DRIVE2_MOVE_TO_POSITION:
            {
                CCommand_VCS::InitCommand("MoveToPosition", DRIVE2_MOVE_TO_POSITION);
                AddParameter(0, "targetPosition", ODT_INT32);
                AddParameter(1, "absolute", ODT_BOOLEAN);
                AddParameter(2, "immediately", ODT_BOOLEAN);
                SetDefaultParameter_MoveToPosition();
                return TRUE;
            };
        case DRIVE2_SET_POSITION_PROFILE:
            {
                CCommand_VCS::InitCommand("SetPositionProfile", DRIVE2_SET_POSITION_PROFILE);
                AddParameter(0, "profileVelocity", ODT_UINT32);
                AddParameter(1, "profileAcceleration", ODT_UINT32);
                AddParameter(2, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_SetPositionProfile();
                return TRUE;
            };
        case DRIVE2_ENABLE_POSITION_WINDOW:
            {
                CCommand_VCS::InitCommand("EnablePositionWindow", DRIVE2_ENABLE_POSITION_WINDOW);
                AddParameter(0, "positionWindow", ODT_UINT32);
                AddParameter(1, "positionWindowTime", ODT_UINT16);
                SetDefaultParameter_EnablePositionWindow();
                return TRUE;
            };
        case DRIVE2_DISABLE_POSITION_WINDOW:
            {
                CCommand_VCS::InitCommand("DisablePositionWindow", DRIVE2_DISABLE_POSITION_WINDOW);
                SetDefaultParameter_DisablePositionWindow();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_InterpolatedPositionMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_SET_IPM_BUFFER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetIpmBufferParameter", DRIVE2_SET_IPM_BUFFER_PARAMETER);
                AddParameter(0, "underflowWarningLimit", ODT_UINT16);
                AddParameter(1, "overflowWarningLimit", ODT_UINT16);
                SetDefaultParameter_SetIpmBufferParameter();
                return TRUE;
            };
        case DRIVE2_GET_IPM_BUFFER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetIpmBufferParameter", DRIVE2_GET_IPM_BUFFER_PARAMETER);
                AddReturnParameter(0, "underflowWarningLimit", ODT_UINT16);
                AddReturnParameter(1, "overflowWarningLimit", ODT_UINT16);
                AddReturnParameter(2, "maxBufferSize", ODT_UINT32);
                SetDefaultParameter_GetIpmBufferParameter();
                return TRUE;
            };
        case DRIVE2_CLEAR_IPM_BUFFER:
            {
                CCommand_VCS::InitCommand("ClearIpmBuffer", DRIVE2_CLEAR_IPM_BUFFER);
                SetDefaultParameter_ClearIpmBuffer();
                return TRUE;
            };
        case DRIVE2_GET_FREE_IPM_BUFFER_SIZE:
            {
                CCommand_VCS::InitCommand("GetFreeIpmBufferSize", DRIVE2_GET_FREE_IPM_BUFFER_SIZE);
                AddReturnParameter(0, "bufferSize", ODT_UINT32);
                SetDefaultParameter_GetFreeIpmBufferSize();
                return TRUE;
            };
        case DRIVE2_ADD_PVT_VALUE_TO_IPM_BUFFER:
            {
                CCommand_VCS::InitCommand("AddPvtValueToIpmBuffer", DRIVE2_ADD_PVT_VALUE_TO_IPM_BUFFER);
                AddParameter(0, "position", ODT_UINT32);
                AddParameter(1, "velocity", ODT_UINT32);
                AddParameter(2, "time", ODT_UINT8);
                SetDefaultParameter_AddPvtValueToIpmBuffer();
                return TRUE;
            };
        case DRIVE2_START_IPM_TRAJECTORY:
            {
                CCommand_VCS::InitCommand("StartIpmTrajectory", DRIVE2_START_IPM_TRAJECTORY);
                SetDefaultParameter_StartIpmTrajectory();
                return TRUE;
            };
        case DRIVE2_STOP_IPM_TRAJECTORY:
            {
                CCommand_VCS::InitCommand("StopIpmTrajectory", DRIVE2_STOP_IPM_TRAJECTORY);
                SetDefaultParameter_StopIpmTrajectory();
                return TRUE;
            };
        case DRIVE2_GET_IPM_STATUS:
            {
                CCommand_VCS::InitCommand("GetIpmStatus", DRIVE2_GET_IPM_STATUS);
                AddReturnParameter(0, "trajectoryRunning", ODT_BOOLEAN);
                AddReturnParameter(1, "isUnderflowWarning", ODT_BOOLEAN);
                AddReturnParameter(2, "isOverflowWarning", ODT_BOOLEAN);
                AddReturnParameter(3, "isVelocityWarning", ODT_BOOLEAN);
                AddReturnParameter(4, "isAccelerationWarning", ODT_BOOLEAN);
                AddReturnParameter(5, "isUnderflowError", ODT_BOOLEAN);
                AddReturnParameter(6, "isOverflowError", ODT_BOOLEAN);
                AddReturnParameter(7, "isVelocityError", ODT_BOOLEAN);
                AddReturnParameter(8, "isAccelerationError", ODT_BOOLEAN);
                SetDefaultParameter_GetIpmStatus();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_ProfileVelocityMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_TARGET_VELOCITY:
            {
                CCommand_VCS::InitCommand("GetTargetVelocity", DRIVE2_GET_TARGET_VELOCITY);
                AddReturnParameter(0, "targetVelocity", ODT_INT32);
                SetDefaultParameter_GetTargetVelocity();
                return TRUE;
            };
        case DRIVE2_GET_VELOCITY_PROFILE:
            {
                CCommand_VCS::InitCommand("GetVelocityProfile", DRIVE2_GET_VELOCITY_PROFILE);
                AddReturnParameter(0, "profileAcceleration", ODT_UINT32);
                AddReturnParameter(1, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_GetVelocityProfile();
                return TRUE;
            };
        case DRIVE2_HALT_VELOCITY_MOVEMENT:
            {
                CCommand_VCS::InitCommand("HaltVelocityMovement", DRIVE2_HALT_VELOCITY_MOVEMENT);
                SetDefaultParameter_HaltVelocityMovement();
                return TRUE;
            };
        case DRIVE2_MOVE_WITH_VELOCITY:
            {
                CCommand_VCS::InitCommand("MoveWithVelocity", DRIVE2_MOVE_WITH_VELOCITY);
                AddParameter(0, "targetVelocity", ODT_INT32);
                SetDefaultParameter_MoveWithVelocity();
                return TRUE;
            };
        case DRIVE2_SET_VELOCITY_PROFILE:
            {
                CCommand_VCS::InitCommand("SetVelocityProfile", DRIVE2_SET_VELOCITY_PROFILE);
                AddParameter(0, "profileAcceleration", ODT_UINT32);
                AddParameter(1, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_SetVelocityProfile();
                return TRUE;
            };
        case DRIVE2_ENABLE_VELOCITY_WINDOW:
            {
                CCommand_VCS::InitCommand("EnableVelocityWindow", DRIVE2_ENABLE_VELOCITY_WINDOW);
                AddParameter(0, "velocityWindow", ODT_UINT32);
                AddParameter(1, "velocityWindowTime", ODT_UINT16);
                SetDefaultParameter_EnableVelocityWindow();
                return TRUE;
            };
        case DRIVE2_DISABLE_VELOCITY_WINDOW:
            {
                CCommand_VCS::InitCommand("DisableVelocityWindow", DRIVE2_DISABLE_VELOCITY_WINDOW);
                SetDefaultParameter_DisableVelocityWindow();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_StateMachine(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_CLEAR_FAULT:
            {
                CCommand_VCS::InitCommand("ClearFault", DRIVE2_CLEAR_FAULT);
                SetDefaultParameter_ClearFault();
                return TRUE;
            };
        case DRIVE2_GET_DISABLE_STATE:
            {
                CCommand_VCS::InitCommand("GetDisableState", DRIVE2_GET_DISABLE_STATE);
                AddReturnParameter(0, "disabled", ODT_BOOLEAN);
                SetDefaultParameter_GetDisableState();
                return TRUE;
            };
        case DRIVE2_GET_ENABLE_STATE:
            {
                CCommand_VCS::InitCommand("GetEnableState", DRIVE2_GET_ENABLE_STATE);
                AddReturnParameter(0, "enabled", ODT_BOOLEAN);
                SetDefaultParameter_GetEnableState();
                return TRUE;
            };
        case DRIVE2_GET_FAULT_STATE:
            {
                CCommand_VCS::InitCommand("GetFaultState", DRIVE2_GET_FAULT_STATE);
                AddReturnParameter(0, "isInFault", ODT_BOOLEAN);
                SetDefaultParameter_GetFaultState();
                return TRUE;
            };
        case DRIVE2_GET_OPERATION_MODE:
            {
                CCommand_VCS::InitCommand("GetOperationMode", DRIVE2_GET_OPERATION_MODE);
                AddReturnParameter(0, "mode", ODT_INT8);
                SetDefaultParameter_GetOperationMode();
                return TRUE;
            };
        case DRIVE2_GET_QUICK_STOP_STATE:
            {
                CCommand_VCS::InitCommand("GetQuickStopState", DRIVE2_GET_QUICK_STOP_STATE);
                AddReturnParameter(0, "isQuickStoped", ODT_BOOLEAN);
                SetDefaultParameter_GetQuickStopState();
                return TRUE;
            };
        case DRIVE2_SET_DISABLE_STATE:
            {
                CCommand_VCS::InitCommand("SetDisableState", DRIVE2_SET_DISABLE_STATE);
                SetDefaultParameter_SetDisableState();
                return TRUE;
            };
        case DRIVE2_SET_ENABLE_STATE:
            {
                CCommand_VCS::InitCommand("SetEnableState", DRIVE2_SET_ENABLE_STATE);
                SetDefaultParameter_SetEnableState();
                return TRUE;
            };
        case DRIVE2_SET_OPERATION_MODE:
            {
                CCommand_VCS::InitCommand("SetOperationMode", DRIVE2_SET_OPERATION_MODE);
                AddParameter(0, "mode", ODT_INT8);
                SetDefaultParameter_SetOperationMode();
                return TRUE;
            };
        case DRIVE2_SET_QUICK_STOP_STATE:
            {
                CCommand_VCS::InitCommand("SetQuickStopState", DRIVE2_SET_QUICK_STOP_STATE);
                SetDefaultParameter_SetQuickStopState();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_VelocityMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_GET_VELOCITY_MUST:
            {
                CCommand_VCS::InitCommand("GetVelocityMust", DRIVE2_GET_VELOCITY_MUST);
                AddReturnParameter(0, "velocityMust", ODT_INT32);
                SetDefaultParameter_GetVelocityMust();
                return TRUE;
            };
        case DRIVE2_SET_VELOCITY_MUST:
            {
                CCommand_VCS::InitCommand("SetVelocityMust", DRIVE2_SET_VELOCITY_MUST);
                AddParameter(0, "velocityMust", ODT_INT32);
                SetDefaultParameter_SetVelocityMust();
                return TRUE;
            };
        case DRIVE2_ACTIVATE_ANALOG_VELOCITY_SETPOINT:
            {
                CCommand_VCS::InitCommand("ActivateAnalogVelocitySetpoint", DRIVE2_ACTIVATE_ANALOG_VELOCITY_SETPOINT);
                AddParameter(0, "analogInputNumber", ODT_UINT16);
                AddParameter(1, "scaling", ODT_FLOAT);
                AddParameter(2, "offset", ODT_INT32);
                SetDefaultParameter_ActivateAnalogVelocitySetpoint();
                return TRUE;
            };
        case DRIVE2_DEACTIVATE_ANALOG_VELOCITY_SETPOINT:
            {
                CCommand_VCS::InitCommand("DeactivateAnalogVelocitySetpoint", DRIVE2_DEACTIVATE_ANALOG_VELOCITY_SETPOINT);
                AddParameter(0, "analogInputNumber", ODT_UINT16);
                SetDefaultParameter_DeactivateAnalogVelocitySetpoint();
                return TRUE;
            };
        case DRIVE2_ENABLE_ANALOG_VELOCITY_SETPOINT:
            {
                CCommand_VCS::InitCommand("EnableAnalogVelocitySetpoint", DRIVE2_ENABLE_ANALOG_VELOCITY_SETPOINT);
                SetDefaultParameter_EnableAnalogVelocitySetpoint();
                return TRUE;
            };
        case DRIVE2_DISABLE_ANALOG_VELOCITY_SETPOINT:
            {
                CCommand_VCS::InitCommand("DisableAnalogVelocitySetpoint", DRIVE2_DISABLE_ANALOG_VELOCITY_SETPOINT);
                SetDefaultParameter_DisableAnalogVelocitySetpoint();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_MasterEncoderMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_SET_MASTER_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetMasterEncoderParameter", DRIVE2_SET_MASTER_ENCODER_PARAMETER);
                AddParameter(0, "scalingNumerator", ODT_UINT16);
                AddParameter(1, "scalingDenominator", ODT_UINT16);
                AddParameter(2, "polarity", ODT_UINT8);
                AddParameter(3, "maxVelocity", ODT_UINT32);
                AddParameter(4, "maxAcceleration", ODT_UINT32);
                SetDefaultParameter_SetMasterEncoderParameter();
                return TRUE;
            };
        case DRIVE2_GET_MASTER_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetMasterEncoderParameter", DRIVE2_GET_MASTER_ENCODER_PARAMETER);
                AddReturnParameter(0, "scalingNumerator", ODT_UINT16);
                AddReturnParameter(1, "scalingDenominator", ODT_UINT16);
                AddReturnParameter(2, "polarity", ODT_UINT8);
                AddReturnParameter(3, "maxVelocity", ODT_UINT32);
                AddReturnParameter(4, "maxAcceleration", ODT_UINT32);
                SetDefaultParameter_GetMasterEncoderParameter();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive2::InitCommand_StepDirectionMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE2_SET_STEP_DIRECTION_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetStepDirectionParameter", DRIVE2_SET_STEP_DIRECTION_PARAMETER);
                AddParameter(0, "scalingNumerator", ODT_UINT16);
                AddParameter(1, "scalingDenominator", ODT_UINT16);
                AddParameter(2, "polarity", ODT_UINT8);
                AddParameter(3, "maxVelocity", ODT_UINT32);
                AddParameter(4, "maxAcceleration", ODT_UINT32);
                SetDefaultParameter_SetStepDirectionParameter();
                return TRUE;
            };

        case DRIVE2_GET_STEP_DIRECTION_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetStepDirectionParameter", DRIVE2_GET_STEP_DIRECTION_PARAMETER);
                AddReturnParameter(0, "scalingNumerator", ODT_UINT16);
                AddReturnParameter(1, "scalingDenominator", ODT_UINT16);
                AddReturnParameter(2, "polarity", ODT_UINT8);
                AddReturnParameter(3, "maxVelocity", ODT_UINT32);
                AddReturnParameter(4, "maxAcceleration", ODT_UINT32);
                SetDefaultParameter_GetStepDirectionParameter();
                return TRUE;
            };
    }

    return FALSE;
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetCurrentRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usP, sizeof(usP));
    SetReturnParameterData(1, &usI, sizeof(usI));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetEncoderParameter()
{
    WORD usCounts = 500;
    WORD usPositionSensorType = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usCounts, sizeof(usCounts));
    SetReturnParameterData(1, &usPositionSensorType, sizeof(usPositionSensorType));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetSensorType()
{
    WORD usSensorType = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usSensorType, sizeof(usSensorType));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetIncEncoderParameter()
{
    WORD usEncoderResolution = 500;
    BOOL oInvertedPolarity = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usEncoderResolution, sizeof(usEncoderResolution));
    SetReturnParameterData(1, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetHallSensorParameter()
{
    BOOL oInvertedPolarity = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetSsiAbsEncoderParameter()
{
    WORD usDataRate = 500;
    WORD usNbOfMultiTurnDataBits = 12;
    WORD usNbOfSingleTurnDataBits = 13;
    BOOL oInvertedPolarity = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usDataRate, sizeof(usDataRate));
    SetReturnParameterData(1, &usNbOfMultiTurnDataBits, sizeof(usNbOfMultiTurnDataBits));
    SetReturnParameterData(2, &usNbOfSingleTurnDataBits, sizeof(usNbOfSingleTurnDataBits));
    SetReturnParameterData(3, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetMotorParameter()
{
    WORD usMotorType = 10;
    WORD usContinuousCurrent = 1;
    WORD usMaxOutputCurrent = 1;
    BYTE ubNbOfPolePairs = 1;
    WORD usThermalTimeConstant = 40;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usMotorType, sizeof(usMotorType));
    SetReturnParameterData(1, &usContinuousCurrent, sizeof(usContinuousCurrent));
    SetReturnParameterData(2, &usMaxOutputCurrent, sizeof(usMaxOutputCurrent));
    SetReturnParameterData(3, &ubNbOfPolePairs, sizeof(ubNbOfPolePairs));
    SetReturnParameterData(4, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetMotorType()
{
    WORD usMotorType = 10;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usMotorType, sizeof(usMotorType));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetDcMotorParameter()
{
    WORD usContinuousCurrent = 1;
    WORD usMaxOutputCurrent = 1;
    WORD usThermalTimeConstant = 40;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
    SetReturnParameterData(1, &usMaxOutputCurrent, sizeof(usMaxOutputCurrent));
    SetReturnParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetEcMotorParameter()
{
    WORD usContinuousCurrent = 1;
    WORD usMaxOutputCurrent = 1;
    BYTE ubNbOfPolePairs = 1;
    WORD usThermalTimeConstant = 40;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
    SetReturnParameterData(1, &usMaxOutputCurrent, sizeof(usMaxOutputCurrent));
    SetReturnParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
    SetReturnParameterData(3, &ubNbOfPolePairs, sizeof(ubNbOfPolePairs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetPositionRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;
    WORD usD = 100;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usP, sizeof(usP));
    SetReturnParameterData(1, &usI, sizeof(usI));
    SetReturnParameterData(2, &usD, sizeof(usD));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetPositionRegulatorFeedForward()
{
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    //ReturnParameter
    SetReturnParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
    SetReturnParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetVelocityRegulatorGain()
{
    BYTE uPortNumber = 0;
    WORD wNodeId = 1;
    WORD usP = 100;
    WORD usI = 100;

    //Parameter
    SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
    SetParameterData(1, &wNodeId, sizeof(wNodeId));

    //ReturnParameter
    SetReturnParameterData(0, &usP, sizeof(usP));
    SetReturnParameterData(1, &usI, sizeof(usI));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetVelocityRegulatorFeedForward()
{
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    //ReturnParameter
    SetReturnParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
    SetReturnParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetMaxFollowingError()
{
    BYTE uPortNumber = 0;
    WORD wNodeId = 1;
    DWORD ulMaxFollowingError = 2000;

    //Parameter
    SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
    SetParameterData(1, &wNodeId, sizeof(wNodeId));

    //ReturnParameter
    SetReturnParameterData(0, &ulMaxFollowingError, sizeof(ulMaxFollowingError));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetMaxProfileVelocity()
{
    BYTE uPortNumber = 0;
    WORD wNodeId = 1;
    DWORD ulMaxProfileVelocity = 25000;

    //Parameter
    SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
    SetParameterData(1, &wNodeId, sizeof(wNodeId));

    //ReturnParameter
    SetReturnParameterData(0, &ulMaxProfileVelocity, sizeof(ulMaxProfileVelocity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetMaxAcceleration()
{
    BYTE uPortNumber = 0;
    WORD wNodeId = 1;
    DWORD ulMaxAcceleration = UINT_MAX;

    //Parameter
    SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
    SetParameterData(1, &wNodeId, sizeof(wNodeId));

    //ReturnParameter
    SetReturnParameterData(0, &ulMaxAcceleration, sizeof(ulMaxAcceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetVelocityUnits()
{
    BYTE ubVelocityDimension = 0xA4;
    char bVelocityNotation = 0;

    //ReturnParameter
    SetReturnParameterData(0, &ubVelocityDimension, sizeof(ubVelocityDimension));
    SetReturnParameterData(1, &bVelocityNotation, sizeof(bVelocityNotation));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetCurrentRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;

    //Parameter
    SetParameterData(0, &usP, sizeof(usP));
    SetParameterData(1, &usI, sizeof(usI));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetEncoderParameter()
{
    WORD usCounts = 500;
    WORD usPositionSensorType = 1;

    //Parameter
    SetParameterData(0, &usCounts, sizeof(usCounts));
    SetParameterData(1, &usPositionSensorType, sizeof(usPositionSensorType));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetSensorType()
{
    WORD usSensorType = 1;

    //Parameter
    SetParameterData(0, &usSensorType, sizeof(usSensorType));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetIncEncoderParameter()
{
    WORD usEncoderResolution = 500;
    BOOL oInvertedPolarity = FALSE;

    //Parameter
    SetParameterData(0, &usEncoderResolution, sizeof(usEncoderResolution));
    SetParameterData(1, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetHallSensorParameter()
{
    BOOL oInvertedPolarity = FALSE;

    //Parameter
    SetParameterData(0, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetSsiAbsEncoderParameter()
{
    WORD usDataRate = 500;
    WORD usNbOfMultiTurnDataBits = 12;
    WORD usNbOfSingleTurnDataBits = 13;
    BOOL oInvertedPolarity = FALSE;

    //Parameter
    SetParameterData(0, &usDataRate, sizeof(usDataRate));
    SetParameterData(1, &usNbOfMultiTurnDataBits, sizeof(usNbOfMultiTurnDataBits));
    SetParameterData(2, &usNbOfSingleTurnDataBits, sizeof(usNbOfSingleTurnDataBits));
    SetParameterData(3, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetMotorParameter()
{
    WORD usMotorType = 10;
    WORD usContinuousCurrent = 1;
    WORD usMaxOutputCurrent = 1;
    BYTE ubNbOfPolePairs = 1;
    WORD usThermalTimeConstant = 40;

    //Parameter
    SetParameterData(0, &usMotorType, sizeof(usMotorType));
    SetParameterData(1, &usContinuousCurrent, sizeof(usContinuousCurrent));
    SetParameterData(2, &usMaxOutputCurrent, sizeof(usMaxOutputCurrent));
    SetParameterData(3, &ubNbOfPolePairs, sizeof(ubNbOfPolePairs));
    SetParameterData(4, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetMotorType()
{
    WORD usMotorType = 10;

    //Parameter
    SetParameterData(0, &usMotorType, sizeof(usMotorType));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetDcMotorParameter()
{
    WORD usContinuousCurrent = 1;
    WORD usMaxOutputCurrent = 1;
    WORD usThermalTimeConstant = 40;

    //Parameter
    SetParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
    SetParameterData(1, &usMaxOutputCurrent, sizeof(usMaxOutputCurrent));
    SetParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetEcMotorParameter()
{
    WORD usContinuousCurrent = 1;
    WORD usMaxOutputCurrent = 1;
    BYTE ubNbOfPolePairs = 1;
    WORD usThermalTimeConstant = 40;

    //Parameter
    SetParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
    SetParameterData(1, &usMaxOutputCurrent, sizeof(usMaxOutputCurrent));
    SetParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
    SetParameterData(3, &ubNbOfPolePairs, sizeof(ubNbOfPolePairs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetPositionRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;
    WORD usD = 100;

    //Parameter
    SetParameterData(0, &usP, sizeof(usP));
    SetParameterData(1, &usI, sizeof(usI));
    SetParameterData(2, &usD, sizeof(usD));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetPositionRegulatorFeedForward()
{
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    //Parameter
    SetParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
    SetParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetVelocityRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;

    //Parameter
    SetParameterData(0, &usP, sizeof(usP));
    SetParameterData(1, &usI, sizeof(usI));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetVelocityRegulatorFeedForward()
{
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    //Parameter
    SetParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
    SetParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetMaxFollowingError()
{
    DWORD ulMaxFollowingError = 2000;

    //Parameter
    SetParameterData(0, &ulMaxFollowingError, sizeof(ulMaxFollowingError));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetMaxProfileVelocity()
{
    DWORD ulMaxProfileVelocity = 25000;

    //Parameter
    SetParameterData(0, &ulMaxProfileVelocity, sizeof(ulMaxProfileVelocity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetMaxAcceleration()
{
    DWORD ulMaxAcceleration = UINT_MAX;

    //Parameter
    SetParameterData(0, &ulMaxAcceleration, sizeof(ulMaxAcceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetVelocityUnits()
{
    BYTE ubVelocityDimension = 0xA4;
    char bVelocityNotation = 0;

    //Parameter
    SetParameterData(0, &ubVelocityDimension, sizeof(ubVelocityDimension));
    SetParameterData(1, &bVelocityNotation, sizeof(bVelocityNotation));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetCurrentMust()
{
    short sCurrentMust = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &sCurrentMust, sizeof(sCurrentMust));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetCurrentMust()
{
    short sCurrentMust = 0;

    //Parameter
    SetParameterData(0, &sCurrentMust, sizeof(sCurrentMust));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ActivateAnalogCurrentSetpoint()
{
    WORD usAnalogInputNumber = 0;
    float fScaling = 0;
    short sOffset = 0;

    //Parameter
    SetParameterData(0, &usAnalogInputNumber, sizeof(usAnalogInputNumber));
    SetParameterData(1, &fScaling, sizeof(fScaling));
    SetParameterData(2, &sOffset, sizeof(sOffset));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DeactivateAnalogCurrentSetpoint()
{
    WORD usAnalogInputNumber = 0;

    //Parameter
    SetParameterData(0, &usAnalogInputNumber, sizeof(usAnalogInputNumber));
}

void CCommand_VCS_Drive2::SetDefaultParameter_EnableAnalogCurrentSetpoint()
{

    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_DisableAnalogCurrentSetpoint()
{

    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_FindHome()
{
    __int8 bHomingMethod = 0;

    //Parameter
    SetParameterData(0, &bHomingMethod, sizeof(bHomingMethod));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetHomingParameter()
{
    DWORD ulHomingAcceleration = 0;
    DWORD ulSpeedSwitch = 0;
    DWORD ulSpeedIndex = 0;
    long lHomeOffset = 0;
    WORD usCurrentThreshold = 0;
    long lHomePosition = 0;

    //ReturnParameter
    SetReturnParameterData(0, &ulHomingAcceleration, sizeof(ulHomingAcceleration));
    SetReturnParameterData(1, &ulSpeedSwitch, sizeof(ulSpeedSwitch));
    SetReturnParameterData(2, &ulSpeedIndex, sizeof(ulSpeedIndex));
    SetReturnParameterData(3, &lHomeOffset, sizeof(lHomeOffset));
    SetReturnParameterData(4, &usCurrentThreshold, sizeof(usCurrentThreshold));
    SetReturnParameterData(5, &lHomePosition, sizeof(lHomePosition));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetHomingParameter()
{
    DWORD ulHomingAcceleration = 0;
    DWORD ulSpeedSwitch = 0;
    DWORD ulSpeedIndex = 0;
    long lHomeOffset = 0;
    WORD usCurrentThreshold = 0;
    long lHomePosition = 0;

    //Parameter
    SetParameterData(0, &ulHomingAcceleration, sizeof(ulHomingAcceleration));
    SetParameterData(1, &ulSpeedSwitch, sizeof(ulSpeedSwitch));
    SetParameterData(2, &ulSpeedIndex, sizeof(ulSpeedIndex));
    SetParameterData(3, &lHomeOffset, sizeof(lHomeOffset));
    SetParameterData(4, &usCurrentThreshold, sizeof(usCurrentThreshold));
    SetParameterData(5, &lHomePosition, sizeof(lHomePosition));
}

void CCommand_VCS_Drive2::SetDefaultParameter_StopHoming()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_DefinePosition()
{
    long lHomePosition = 0;

    //Parameter
    SetParameterData(0, &lHomePosition, sizeof(lHomePosition));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetHomingState()
{
    BOOL oHomingAttained = FALSE;
    BOOL oHomingError = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oHomingAttained, sizeof(oHomingAttained));
    SetReturnParameterData(1, &oHomingError, sizeof(oHomingError));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DigitalInputConfiguration()
{
    WORD usInputNb = 0;
    WORD usConfiguration = 0x0000;
    BOOL oMask = FALSE;
    BOOL oPolarity = FALSE;
    BOOL oExecutionMask = FALSE;

    //Parameter
    SetParameterData(0, &usInputNb, sizeof(usInputNb));
    SetParameterData(1, &usConfiguration, sizeof(usConfiguration));
    SetParameterData(2, &oMask, sizeof(oMask));
    SetParameterData(3, &oPolarity, sizeof(oPolarity));
    SetParameterData(4, &oExecutionMask, sizeof(oExecutionMask));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DigitalOutputConfiguration()
{
    WORD usOutputNb = 0;
    WORD usConfiguration = 0x0000;
    BOOL oState = FALSE;
    BOOL oMask = FALSE;
    BOOL oPolarity = FALSE;

    //Parameter
    SetParameterData(0, &usOutputNb, sizeof(usOutputNb));
    SetParameterData(1, &usConfiguration, sizeof(usConfiguration));
    SetParameterData(2, &oState, sizeof(oState));
    SetParameterData(3, &oMask, sizeof(oMask));
    SetParameterData(4, &oPolarity, sizeof(oPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_AnalogInputConfiguration()
{
    WORD usInputNb = 0;
    WORD usConfiguration = 0x0000;
    BOOL oExecutionMask = FALSE;

    //Parameter
    SetParameterData(0, &usInputNb, sizeof(usInputNb));
    SetParameterData(1, &usConfiguration, sizeof(usConfiguration));
    SetParameterData(2, &oExecutionMask, sizeof(oExecutionMask));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetAllDigitalInputs()
{
    WORD usInputs = 0x0000;

    //ReturnParameter
    SetReturnParameterData(0, &usInputs, sizeof(usInputs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetAllDigitalOutputs()
{
    WORD usOutputs = 0x0000;

    //ReturnParameter
    SetReturnParameterData(0, &usOutputs, sizeof(usOutputs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetAnalogInput()
{
    WORD usNumber = 0;
    WORD usAnalog = 0;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));

    //ReturnParameter
    SetReturnParameterData(0, &usAnalog, sizeof(usAnalog));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetAllDigitalOutputs()
{
    WORD usOutputs = 0x0000;

    //Parameter
    SetParameterData(0, &usOutputs, sizeof(usOutputs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetAnalogOutput()
{
    WORD usNumber = 0;
    WORD usOutput = 0x0000;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
    SetParameterData(1, &usOutput, sizeof(usOutput));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetPositionCompareParameter()
{
    BYTE ubOperationalMode = 0;
    BYTE ubIntervalMode = 0;
    BYTE ubDirectionDependency = 0;
    WORD usIntervalWidth = 0;
    WORD usIntervalRepetitions = 0;
    WORD usPulseWidth = 0;

    //Parameter
    SetParameterData(0, &ubOperationalMode, sizeof(ubOperationalMode));
    SetParameterData(1, &ubIntervalMode, sizeof(ubIntervalMode));
    SetParameterData(2, &ubDirectionDependency, sizeof(ubDirectionDependency));
    SetParameterData(3, &usIntervalWidth, sizeof(usIntervalWidth));
    SetParameterData(4, &usIntervalRepetitions, sizeof(usIntervalRepetitions));
    SetParameterData(5, &usPulseWidth, sizeof(usPulseWidth));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetPositionCompareParameter()
{
    BYTE ubOperationalMode = 0;
    BYTE ubIntervalMode = 0;
    BYTE ubDirectionDependency = 0;
    WORD usIntervalWidth = 0;
    WORD usIntervalRepetitions = 0;
    WORD usPulseWidth = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ubOperationalMode, sizeof(ubOperationalMode));
    SetReturnParameterData(1, &ubIntervalMode, sizeof(ubIntervalMode));
    SetReturnParameterData(2, &ubDirectionDependency, sizeof(ubDirectionDependency));
    SetReturnParameterData(3, &usIntervalWidth, sizeof(usIntervalWidth));
    SetReturnParameterData(4, &usIntervalRepetitions, sizeof(usIntervalRepetitions));
    SetReturnParameterData(5, &usPulseWidth, sizeof(usPulseWidth));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ActivatePositionCompare()
{
    WORD usNumber = 0;
    BOOL oPolarity = FALSE;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
    SetParameterData(1, &oPolarity, sizeof(oPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DeactivatePositionCompare()
{
    WORD usNumber = 0;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
}

void CCommand_VCS_Drive2::SetDefaultParameter_EnablePositionCompare()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_DisablePositionCompare()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetPositionCompareReferencePosition()
{
    long lReferencePosition = 0;

    //Parameter
    SetParameterData(0, &lReferencePosition, sizeof(lReferencePosition));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetPositionMarkerParameter()
{
    BYTE ubPositionMarkerEdgeType = 0;
    BYTE ubPositionMarkerMode = 1;

    //Parameter
    SetParameterData(0, &ubPositionMarkerEdgeType, sizeof(ubPositionMarkerEdgeType));
    SetParameterData(1, &ubPositionMarkerMode, sizeof(ubPositionMarkerMode));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetPositionMarkerParameter()
{
    BYTE ubPositionMarkerEdgeType = 0;
    BYTE ubPositionMarkerMode = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ubPositionMarkerEdgeType, sizeof(ubPositionMarkerEdgeType));
    SetReturnParameterData(1, &ubPositionMarkerMode, sizeof(ubPositionMarkerMode));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ActivatePositionMarker()
{
    WORD usNumber = 0;
    BOOL oPolarity = FALSE;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
    SetParameterData(1, &oPolarity, sizeof(oPolarity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DeactivatePositionMarker()
{
    WORD usNumber = 0;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ReadPositionMarkerCounter()
{
    WORD usCount = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usCount, sizeof(usCount));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ReadPositionMarkerCapturedPosition()
{
    WORD usCounterIndex = 0;
    long lCapturedPosition = 0;

    //Parameter
    SetParameterData(0, &usCounterIndex, sizeof(usCounterIndex));

    //ReturnParameter
    SetReturnParameterData(0, &lCapturedPosition, sizeof(lCapturedPosition));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ResetPositionMarkerCounter()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetCurrentIs()
{
    short sCurrentIs = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &sCurrentIs, sizeof(sCurrentIs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetCurrentIsAveraged()
{
    short sCurrentIsAveraged = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &sCurrentIsAveraged, sizeof(sCurrentIsAveraged));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetMovementState()
{
    BOOL oTargetReached = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oTargetReached, sizeof(oTargetReached));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetPositionIs()
{
    long lPositionIs = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lPositionIs, sizeof(lPositionIs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetVelocityIs()
{
    long lVelocityIs = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lVelocityIs, sizeof(lVelocityIs));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetVelocityIsAveraged()
{
    long lVelocityIsAveraged = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lVelocityIsAveraged, sizeof(lVelocityIsAveraged));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetPositionMust()
{
    long lPositionMust = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lPositionMust, sizeof(lPositionMust));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetPositionMust()
{
    long lPositionMust = 0;

    //Parameter
    SetParameterData(0, &lPositionMust, sizeof(lPositionMust));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ActivateAnalogPositionSetpoint()
{
    WORD usAnalogInputNumber = 0;
    float fScaling = 0;
    long lOffset = 0;

    //Parameter
    SetParameterData(0, &usAnalogInputNumber, sizeof(usAnalogInputNumber));
    SetParameterData(1, &fScaling, sizeof(fScaling));
    SetParameterData(2, &lOffset, sizeof(lOffset));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DeactivateAnalogPositionSetpoint()
{
    WORD usAnalogInputNumber = 0;

    //Parameter
    SetParameterData(0, &usAnalogInputNumber, sizeof(usAnalogInputNumber));
}

void CCommand_VCS_Drive2::SetDefaultParameter_EnableAnalogPositionSetpoint()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_DisableAnalogPositionSetpoint()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetPositionProfile()
{
    DWORD ulProfileVelocity = 1000;
    DWORD ulProfileAcceleration = 10000;
    DWORD ulProfileDeceleration = 10000;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ulProfileVelocity, sizeof(ulProfileVelocity));
    SetReturnParameterData(1, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
    SetReturnParameterData(2, &ulProfileDeceleration, sizeof(ulProfileDeceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetTargetPosition()
{
    long lTargetPosition = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lTargetPosition, sizeof(lTargetPosition));
}

void CCommand_VCS_Drive2::SetDefaultParameter_HaltPositionMovement()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_MoveToPosition()
{
    long lTargetPosition = 1000;
    BOOL oAbsolute = FALSE;
    BOOL oImmediately = TRUE;

    //Parameter
    SetParameterData(0, &lTargetPosition, sizeof(lTargetPosition));
    SetParameterData(1, &oAbsolute, sizeof(oAbsolute));
    SetParameterData(2, &oImmediately, sizeof(oImmediately));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetPositionProfile()
{
    DWORD ulProfileVelocity = 1000;
    DWORD ulProfileAcceleration = 10000;
    DWORD ulProfileDeceleration = 10000;

    //Parameter
    SetParameterData(0, &ulProfileVelocity, sizeof(ulProfileVelocity));
    SetParameterData(1, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
    SetParameterData(2, &ulProfileDeceleration, sizeof(ulProfileDeceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_EnablePositionWindow()
{
    DWORD ulPositionWindow = ULONG_MAX;
    WORD usPositionWindowTime = 0;

    //Parameter
    SetParameterData(0, &ulPositionWindow, sizeof(ulPositionWindow));
    SetParameterData(1, &usPositionWindowTime, sizeof(usPositionWindowTime));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DisablePositionWindow()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetTargetVelocity()
{
    long lTargetVelocity = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetVelocityProfile()
{
    DWORD ulProfileAcceleration = 0;
    DWORD ulProfileDeceleration = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
    SetReturnParameterData(1, &ulProfileDeceleration, sizeof(ulProfileDeceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_HaltVelocityMovement()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_MoveWithVelocity()
{
    long lTargetVelocity = 0;

    //Parameter
    SetParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetVelocityProfile()
{
    DWORD ulProfileAcceleration = 0;
    DWORD ulProfileDeceleration = 0;

    //Parameter
    SetParameterData(0, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
    SetParameterData(1, &ulProfileDeceleration, sizeof(ulProfileDeceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_EnableVelocityWindow()
{
    DWORD ulVelocityWindow = ULONG_MAX;
    WORD usVelocityWindowTime = 0;

    //Parameter
    SetParameterData(0, &ulVelocityWindow, sizeof(ulVelocityWindow));
    SetParameterData(1, &usVelocityWindowTime, sizeof(usVelocityWindowTime));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DisableVelocityWindow()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_ClearFault()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetState()
{
    WORD usState = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usState, sizeof(usState));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetDisableState()
{
    BOOL oIsDisabled = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsDisabled, sizeof(oIsDisabled));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetEnableState()
{
    BOOL oIsEnabled = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsEnabled, sizeof(oIsEnabled));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetFaultState()
{
    BOOL oIsInFault = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsInFault, sizeof(oIsInFault));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetOperationMode()
{
    __int8 bMode = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &bMode, sizeof(bMode));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetQuickStopState()
{
    BOOL oIsQuickStoped = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsQuickStoped, sizeof(oIsQuickStoped));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetState()
{
    WORD usState = 0;

    //Parameter
    SetParameterData(0, &usState, sizeof(usState));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetDisableState()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetEnableState()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetOperationMode()
{
    __int8 bMode = 0;

    //Parameter
    SetParameterData(0, &bMode, sizeof(bMode));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetQuickStopState()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetVelocityMust()
{
    long lVelocityMust = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lVelocityMust, sizeof(lVelocityMust));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetVelocityMust()
{
    long lVelocityMust = 0;

    //Parameter
    SetParameterData(0, &lVelocityMust, sizeof(lVelocityMust));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ActivateAnalogVelocitySetpoint()
{
    WORD usAnalogInputNumber = 0;
    float fScaling = 0;
    long lOffset = 0;

    //Parameter
    SetParameterData(0, &usAnalogInputNumber, sizeof(usAnalogInputNumber));
    SetParameterData(1, &fScaling, sizeof(fScaling));
    SetParameterData(2, &lOffset, sizeof(lOffset));
}

void CCommand_VCS_Drive2::SetDefaultParameter_DeactivateAnalogVelocitySetpoint()
{
    WORD usAnalogInputNumber = 0;

    //Parameter
    SetParameterData(0, &usAnalogInputNumber, sizeof(usAnalogInputNumber));
}

void CCommand_VCS_Drive2::SetDefaultParameter_EnableAnalogVelocitySetpoint()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_DisableAnalogVelocitySetpoint()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetSerialNumber()
{
    unsigned __int64 ullSerialNb = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ullSerialNb, sizeof(ullSerialNb));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetBaseSectorVersion()
{
    WORD usBaseSectorVersion = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usBaseSectorVersion, sizeof(usBaseSectorVersion));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetIpmBufferParameter()
{
    WORD usUnderflowWarningLimit = 4;
    WORD usOverflowWarningLimit = 60;

    //Parameter
    SetParameterData(0, &usUnderflowWarningLimit, sizeof(usUnderflowWarningLimit));
    SetParameterData(1, &usOverflowWarningLimit, sizeof(usOverflowWarningLimit));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetIpmBufferParameter()
{
    WORD usUnderflowWarningLimit = 4;
    WORD usOverflowWarningLimit = 60;
    DWORD ulMaxBufferSize = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usUnderflowWarningLimit, sizeof(usUnderflowWarningLimit));
    SetReturnParameterData(1, &usOverflowWarningLimit, sizeof(usOverflowWarningLimit));
    SetReturnParameterData(2, &ulMaxBufferSize, sizeof(ulMaxBufferSize));
}

void CCommand_VCS_Drive2::SetDefaultParameter_ClearIpmBuffer()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetFreeIpmBufferSize()
{
    DWORD ulBufferSize = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ulBufferSize, sizeof(ulBufferSize));
}

void CCommand_VCS_Drive2::SetDefaultParameter_AddPvtValueToIpmBuffer()
{
    long lPosition = 0;
    long lVelocity = 0;
    BYTE ubTime = 0;

    //Parameter
    SetParameterData(0, &lPosition, sizeof(lPosition));
    SetParameterData(1, &lVelocity, sizeof(lVelocity));
    SetParameterData(2, &ubTime, sizeof(ubTime));
}

void CCommand_VCS_Drive2::SetDefaultParameter_StartIpmTrajectory()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_StopIpmTrajectory()
{
    //Parameter
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetIpmStatus()
{
    WORD usBaseSectorVersion = 0;
    BOOL oTrajectoryRunning = FALSE;
    BOOL oIsUnderflowWarning = FALSE;
    BOOL oIsOverflowWarning = FALSE;
    BOOL oIsVelocityWarning = FALSE;
    BOOL oIsAccelerationWarning = FALSE;
    BOOL oIsUnderflowError = FALSE;
    BOOL oIsOverflowError = FALSE;
    BOOL oIsVelocityError = FALSE;
    BOOL oIsAccelerationError = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oTrajectoryRunning, sizeof(oTrajectoryRunning));
    SetReturnParameterData(1, &oIsUnderflowWarning, sizeof(oIsUnderflowWarning));
    SetReturnParameterData(2, &oIsOverflowWarning, sizeof(oIsOverflowWarning));
    SetReturnParameterData(3, &oIsVelocityWarning, sizeof(oIsVelocityWarning));
    SetReturnParameterData(4, &oIsAccelerationWarning, sizeof(oIsAccelerationWarning));
    SetReturnParameterData(5, &oIsUnderflowError, sizeof(oIsUnderflowError));
    SetReturnParameterData(6, &oIsOverflowError, sizeof(oIsOverflowError));
    SetReturnParameterData(7, &oIsVelocityError, sizeof(oIsVelocityError));
    SetReturnParameterData(8, &oIsAccelerationError, sizeof(oIsAccelerationError));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetStepDirectionParameter()
{
    BYTE ubPolarity = 0;
    DWORD ulMaxAcceleration = ULONG_MAX;
    DWORD ulMaxVelocity = 25000;
    WORD usScalingDenominator = 1;
    WORD usScalingNumerator = 1;

    //Parameter
    SetParameterData(0, &usScalingNumerator, sizeof(usScalingNumerator));
    SetParameterData(1, &usScalingDenominator, sizeof(usScalingDenominator));
    SetParameterData(2, &ubPolarity, sizeof(ubPolarity));
    SetParameterData(3, &ulMaxVelocity, sizeof(ulMaxVelocity));
    SetParameterData(4, &ulMaxAcceleration, sizeof(ulMaxAcceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetStepDirectionParameter()
{
    BYTE ubPolarity = 0;
    DWORD ulMaxAcceleration = ULONG_MAX;
    DWORD ulMaxVelocity = 25000;
    WORD usScalingDenominator = 1;
    WORD usScalingNumerator = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usScalingNumerator, sizeof(usScalingNumerator));
    SetReturnParameterData(1, &usScalingDenominator, sizeof(usScalingDenominator));
    SetReturnParameterData(2, &ubPolarity, sizeof(ubPolarity));
    SetReturnParameterData(3, &ulMaxVelocity, sizeof(ulMaxVelocity));
    SetReturnParameterData(4, &ulMaxAcceleration, sizeof(ulMaxAcceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_SetMasterEncoderParameter()
{
    BYTE ubPolarity = 0;
    DWORD ulMaxAcceleration = ULONG_MAX;
    DWORD ulMaxVelocity = 25000;
    WORD usScalingDenominator = 1;
    WORD usScalingNumerator = 1;

    //Parameter
    SetParameterData(0, &usScalingNumerator, sizeof(usScalingNumerator));
    SetParameterData(1, &usScalingDenominator, sizeof(usScalingDenominator));
    SetParameterData(2, &ubPolarity, sizeof(ubPolarity));
    SetParameterData(3, &ulMaxVelocity, sizeof(ulMaxVelocity));
    SetParameterData(4, &ulMaxAcceleration, sizeof(ulMaxAcceleration));
}

void CCommand_VCS_Drive2::SetDefaultParameter_GetMasterEncoderParameter()
{
    BYTE ubPolarity = 0;
    DWORD ulMaxAcceleration = ULONG_MAX;
    DWORD ulMaxVelocity = 25000;
    WORD usScalingDenominator = 1;
    WORD usScalingNumerator = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usScalingNumerator, sizeof(usScalingNumerator));
    SetReturnParameterData(1, &usScalingDenominator, sizeof(usScalingDenominator));
    SetReturnParameterData(2, &ubPolarity, sizeof(ubPolarity));
    SetReturnParameterData(3, &ulMaxVelocity, sizeof(ulMaxVelocity));
    SetReturnParameterData(4, &ulMaxAcceleration, sizeof(ulMaxAcceleration));
}

