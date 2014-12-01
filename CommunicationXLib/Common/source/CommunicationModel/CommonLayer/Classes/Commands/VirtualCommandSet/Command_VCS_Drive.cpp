// Command_VCS_Drive.cpp: Implementierung der Klasse CCommand_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////
CCommand_VCS_Drive::CCommand_VCS_Drive()
{
}

CCommand_VCS_Drive::CCommand_VCS_Drive(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_Drive::~CCommand_VCS_Drive()
{
}

CCommandRoot*CCommand_VCS_Drive::CloneCommand()
{
    CCommand_VCS_Drive* pNewCommand;

    pNewCommand = new CCommand_VCS_Drive();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_Drive& CCommand_VCS_Drive::operator=(CCommand_VCS_Drive& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_Drive::InitCommand(DWORD p_ulCommandId)
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
    if(InitCommand_ProfileVelocityMode(p_ulCommandId)) return TRUE;
    if(InitCommand_StateMachine(p_ulCommandId)) return TRUE;
    if(InitCommand_VelocityMode(p_ulCommandId)) return TRUE;
    if(InitCommand_FirmwareDownload(p_ulCommandId)) return TRUE;
    if(InitCommand_MasterEncoderMode(p_ulCommandId)) return TRUE;
    if(InitCommand_StepDirectionMode(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_Configuration(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_CURRENT_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("GetCurrentRegulatorGain", DRIVE_GET_CURRENT_REGULATOR_GAIN);

                AddReturnParameter(0, "P", ODT_UINT16);
                AddReturnParameter(1, "I", ODT_UINT16);

                SetDefaultParameter_GetCurrentRegulatorGain();
                return TRUE;
            };
        case DRIVE_GET_POSITION_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("GetPositionRegulatorGain", DRIVE_GET_POSITION_REGULATOR_GAIN);

                AddReturnParameter(0, "P", ODT_UINT16);
                AddReturnParameter(1, "I", ODT_UINT16);
                AddReturnParameter(2, "D", ODT_UINT16);

                SetDefaultParameter_GetPositionRegulatorGain();
                return TRUE;
            };
        case DRIVE_GET_POSITION_REGULATOR_FEED_FORWARD:
            {
                CCommand_VCS::InitCommand("GetPositionRegulatorFeedForward", DRIVE_GET_POSITION_REGULATOR_FEED_FORWARD);

                AddReturnParameter(0, "velocityFeedForward", ODT_UINT16);
                AddReturnParameter(1, "accelerationFeedForward", ODT_UINT16);

                SetDefaultParameter_GetPositionRegulatorFeedForward();
                return TRUE;
            };
        case DRIVE_GET_VELOCITY_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("GetVelocityRegulatorGain", DRIVE_GET_VELOCITY_REGULATOR_GAIN);

                AddReturnParameter(0, "P", ODT_UINT16);
                AddReturnParameter(1, "I", ODT_UINT16);

                SetDefaultParameter_GetVelocityRegulatorGain();
                return TRUE;
            };
        case DRIVE_GET_MAX_FOLLOWING_ERROR:
            {
                CCommand_VCS::InitCommand("GetMaxFollowingError", DRIVE_GET_MAX_FOLLOWING_ERROR);

                AddReturnParameter(0, "MaxFollowingError", ODT_UINT32);

                SetDefaultParameter_GetMaxFollowingError();
                return TRUE;
            };
        case DRIVE_GET_MAX_PROFILE_VELOCITY:
            {
                CCommand_VCS::InitCommand("GetMaxProfileVelocity", DRIVE_GET_MAX_PROFILE_VELOCITY);

                AddReturnParameter(0, "MaxProfileVelocity", ODT_UINT32);

                SetDefaultParameter_GetMaxProfileVelocity();
                return TRUE;
            };
        case DRIVE_GET_VELOCITY_UNITS:
            {
                CCommand_VCS::InitCommand("GetVelocityUnits", DRIVE_GET_VELOCITY_UNITS);

                AddReturnParameter(0, "velocityDimension", ODT_UINT8);
                AddReturnParameter(1, "velocityNotation", ODT_UINT8);

                SetDefaultParameter_GetVelocityUnits();
                return TRUE;
            };
        case DRIVE_SET_CURRENT_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("SetCurrentRegulatorGain", DRIVE_SET_CURRENT_REGULATOR_GAIN);

                AddParameter(0, "P", ODT_UINT16);
                AddParameter(1, "I", ODT_UINT16);

                SetDefaultParameter_SetCurrentRegulatorGain();
                return TRUE;
            };
        case DRIVE_SET_POSITION_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("SetPositionRegulatorGain", DRIVE_SET_POSITION_REGULATOR_GAIN);

                AddParameter(0, "P", ODT_UINT16);
                AddParameter(1, "I", ODT_UINT16);
                AddParameter(2, "D", ODT_UINT16);

                SetDefaultParameter_SetPositionRegulatorGain();
                return TRUE;
            };
        case DRIVE_SET_POSITION_REGULATOR_FEED_FORWARD:
            {
                CCommand_VCS::InitCommand("SetPositionRegulatorFeedForward", DRIVE_SET_POSITION_REGULATOR_FEED_FORWARD);

                AddParameter(0, "velocityFeedForward", ODT_UINT16);
                AddParameter(1, "accelerationFeedForward", ODT_UINT16);

                SetDefaultParameter_SetPositionRegulatorFeedForward();
                return TRUE;
            };
        case DRIVE_SET_VELOCITY_REGULATOR_GAIN:
            {
                CCommand_VCS::InitCommand("SetVelocityRegulatorGain", DRIVE_SET_VELOCITY_REGULATOR_GAIN);

                AddParameter(0, "P", ODT_UINT16);
                AddParameter(1, "I", ODT_UINT16);

                SetDefaultParameter_SetVelocityRegulatorGain();
                return TRUE;
            };
        case DRIVE_SET_MAX_FOLLOWING_ERROR:
            {
                CCommand_VCS::InitCommand("SetMaxFollowingError", DRIVE_SET_MAX_FOLLOWING_ERROR);

                AddParameter(0, "MaxFollowingError", ODT_UINT32);

                SetDefaultParameter_SetMaxFollowingError();
                return TRUE;
            };
        case DRIVE_SET_MAX_PROFILE_VELOCITY:
            {
                CCommand_VCS::InitCommand("SetMaxProfileVelocity", DRIVE_SET_MAX_PROFILE_VELOCITY);

                AddParameter(0, "MaxProfileVelocity", ODT_UINT32);

                SetDefaultParameter_SetMaxProfileVelocity();
                return TRUE;
            };
        case DRIVE_SET_VELOCITY_UNITS:
            {
                CCommand_VCS::InitCommand("SetVelocityUnits", DRIVE_SET_VELOCITY_UNITS);

                AddParameter(0, "velocityDimensionIndex", ODT_UINT8);
                AddParameter(1, "velocityNotationIndex", ODT_INT8);

                SetDefaultParameter_SetVelocityUnits();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_ConfigurationMotor(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetMotorParameter", DRIVE_GET_MOTOR_PARAMETER);

                AddReturnParameter(0, "motorType", ODT_UINT16);
                AddReturnParameter(1, "continuousCurrent", ODT_UINT16);
                AddReturnParameter(2, "peakCurrent", ODT_UINT16);
                AddReturnParameter(3, "polePair", ODT_UINT8);
                AddReturnParameter(4, "thermalTimeConstant", ODT_UINT16);

                SetDefaultParameter_GetMotorParameter();
                return TRUE;
            };
        case DRIVE_GET_MOTOR_TYPE:
            {
                CCommand_VCS::InitCommand("GetMotorType", DRIVE_GET_MOTOR_TYPE);

                AddReturnParameter(0, "motorType", ODT_UINT16);

                SetDefaultParameter_GetMotorType();
                return TRUE;
            };
        case DRIVE_GET_DC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetDcMotorParameter", DRIVE_GET_DC_MOTOR_PARAMETER);

                AddReturnParameter(0, "nominalCurrent", ODT_UINT16);
                AddReturnParameter(1, "maxOutputCurrent", ODT_UINT16);
                AddReturnParameter(2, "thermalTimeConstant", ODT_UINT16);

                SetDefaultParameter_GetDcMotorParameter();
                return TRUE;
            };
        case DRIVE_GET_EC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetEcMotorParameter", DRIVE_GET_EC_MOTOR_PARAMETER);

                AddReturnParameter(0, "nominalCurrent", ODT_UINT16);
                AddReturnParameter(1, "maxOutputCurrent", ODT_UINT16);
                AddReturnParameter(2, "thermalTimeConstant", ODT_UINT16);
                AddReturnParameter(3, "nbOfPolePairs", ODT_UINT8);

                SetDefaultParameter_GetEcMotorParameter();
                return TRUE;
            };
        case DRIVE_SET_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetMotorParameter", DRIVE_SET_MOTOR_PARAMETER);

                AddParameter(0, "motorType", ODT_UINT16);
                AddParameter(1, "continuousCurrent", ODT_UINT16);
                AddParameter(2, "peakCurrent", ODT_UINT16);
                AddParameter(3, "polePair", ODT_UINT8);
                AddParameter(4, "thermalTimeConstant", ODT_UINT16);

                SetDefaultParameter_SetMotorParameter();
                return TRUE;
            };
        case DRIVE_SET_MOTOR_TYPE:
            {
                CCommand_VCS::InitCommand("SetMotorType", DRIVE_SET_MOTOR_TYPE);

                AddParameter(0, "motorType", ODT_UINT16);

                SetDefaultParameter_SetMotorType();
                return TRUE;
            };
        case DRIVE_SET_DC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetDcMotorParameter", DRIVE_SET_DC_MOTOR_PARAMETER);

                AddParameter(0, "nominalCurrent", ODT_UINT16);
                AddParameter(1, "maxOutputCurrent", ODT_UINT16);
                AddParameter(2, "thermalTimeConstant", ODT_UINT16);

                SetDefaultParameter_SetDcMotorParameter();
                return TRUE;
            };
        case DRIVE_SET_EC_MOTOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetEcMotorParameter", DRIVE_SET_EC_MOTOR_PARAMETER);

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

BOOL CCommand_VCS_Drive::InitCommand_ConfigurationSensor(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetEncoderParameter", DRIVE_GET_ENCODER_PARAMETER);

                AddReturnParameter(0, "counts", ODT_UINT16);
                AddReturnParameter(1, "positionSensorType", ODT_UINT16);

                SetDefaultParameter_GetEncoderParameter();
                return TRUE;
            };
        case DRIVE_GET_SENSOR_TYPE:
            {
                CCommand_VCS::InitCommand("GetSensorType", DRIVE_GET_SENSOR_TYPE);

                AddReturnParameter(0, "sensorType", ODT_UINT16);

                SetDefaultParameter_GetSensorType();
                return TRUE;
            };
        case DRIVE_GET_INC_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetIncEncoderParameter", DRIVE_GET_INC_ENCODER_PARAMETER);

                AddReturnParameter(0, "encoderResolution", ODT_UINT32);
                AddReturnParameter(1, "invertedPolarity", ODT_BOOLEAN);

                SetDefaultParameter_GetIncEncoderParameter();
                return TRUE;
            };
        case DRIVE_GET_HALL_SENSOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetEncoderParameter", DRIVE_GET_HALL_SENSOR_PARAMETER);

                AddReturnParameter(0, "invertedPolarity", ODT_BOOLEAN);

                SetDefaultParameter_GetHallSensorParameter();
                return TRUE;
            };
        case DRIVE_SET_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetEncoderParameter", DRIVE_SET_ENCODER_PARAMETER);

                AddParameter(0, "counts", ODT_UINT16);
                AddParameter(1, "positionSensorType", ODT_UINT16);

                SetDefaultParameter_SetEncoderParameter();
                return TRUE;
            };
        case DRIVE_SET_SENSOR_TYPE:
            {
                CCommand_VCS::InitCommand("SetSensorType", DRIVE_SET_SENSOR_TYPE);

                AddParameter(0, "sensorType", ODT_UINT16);

                SetDefaultParameter_SetSensorType();
                return TRUE;
            };
        case DRIVE_SET_INC_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetIncEncoderParameter", DRIVE_SET_INC_ENCODER_PARAMETER);

                AddParameter(0, "encoderResolution", ODT_UINT32);
                AddParameter(1, "invertedPolarity", ODT_BOOLEAN);

                SetDefaultParameter_SetIncEncoderParameter();
                return TRUE;
            };
        case DRIVE_SET_HALL_SENSOR_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetEncoderParameter", DRIVE_SET_HALL_SENSOR_PARAMETER);

                AddParameter(0, "invertedPolarity", ODT_BOOLEAN);

                SetDefaultParameter_SetHallSensorParameter();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_CurrentMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_CURRENT_MUST:
            {
                CCommand_VCS::InitCommand("GetCurrentMust", DRIVE_GET_CURRENT_MUST);

                AddReturnParameter(0, "currentMust", ODT_INT16);

                SetDefaultParameter_GetCurrentMust();
                return TRUE;
            };
        case DRIVE_SET_CURRENT_MUST:
            {
                CCommand_VCS::InitCommand("SetCurrentMust", DRIVE_SET_CURRENT_MUST);

                AddParameter(0, "currentMust", ODT_INT16);

                SetDefaultParameter_SetCurrentMust();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_FirmwareDownload(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_BASE_SECTOR_VERSION:
            {

                CCommand_VCS::InitCommand("GetBaseSectorVersion", DRIVE_GET_BASE_SECTOR_VERSION);

                AddReturnParameter(0, "baseSectorVersion", ODT_UINT16);

                SetDefaultParameter_GetBaseSectorVersion();
                return TRUE;
            };
        case DRIVE_GET_SERIAL_NUMBER:
            {

                CCommand_VCS::InitCommand("GetSerialNumber", DRIVE_GET_SERIAL_NUMBER);

                AddReturnParameter(0, "serialNumber", ODT_UINT64);

                SetDefaultParameter_GetSerialNumber();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_HomingMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_FIND_HOME:
            {
                CCommand_VCS::InitCommand("FindHome", DRIVE_FIND_HOME);
                AddParameter(0, "homingMethod", ODT_INT8);
                SetDefaultParameter_FindHome();
                return TRUE;
            };
        case DRIVE_GET_HOMING_PARAMETER:
            {

                CCommand_VCS::InitCommand("GetHomingParameter", DRIVE_GET_HOMING_PARAMETER);
                AddReturnParameter(0, "homingAcceleration", ODT_UINT32);
                AddReturnParameter(1, "speedSwitch", ODT_UINT32);
                AddReturnParameter(2, "speedIndex", ODT_UINT32);
                AddReturnParameter(3, "homeOffset", ODT_INT32);
                AddReturnParameter(4, "currentThreshold", ODT_UINT16);
                AddReturnParameter(5, "homePosition", ODT_INT32);
                SetDefaultParameter_GetHomingParameter();
                return TRUE;
            };
        case DRIVE_SET_HOMING_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetHomingParameter", DRIVE_SET_HOMING_PARAMETER);
                AddParameter(0, "homingAcceleration", ODT_UINT32);
                AddParameter(1, "speedSwitch", ODT_UINT32);
                AddParameter(2, "speedIndex", ODT_UINT32);
                AddParameter(3, "homeOffset", ODT_INT32);
                AddParameter(4, "currentThreshold", ODT_UINT16);
                AddParameter(5, "homePosition", ODT_INT32);
                SetDefaultParameter_SetHomingParameter();
                return TRUE;
            };
        case DRIVE_STOP_HOMING:
            {

                CCommand_VCS::InitCommand("StopHoming", DRIVE_STOP_HOMING);
                SetDefaultParameter_StopHoming();
                return TRUE;
            };
        case DRIVE_DEFINE_POSITION:
            {
                CCommand_VCS::InitCommand("DefinePosition", DRIVE_DEFINE_POSITION);
                AddParameter(0, "homePosition", ODT_INT32);
                SetDefaultParameter_DefinePosition();
                return TRUE;
            }
       case DRIVE_GET_HOMING_STATE:
            {
                CCommand_VCS::InitCommand("GetHomingState", DRIVE_GET_HOMING_STATE);
                AddReturnParameter(0, "homingAttained", ODT_BOOLEAN);
                AddReturnParameter(1, "homingError", ODT_BOOLEAN);
                SetDefaultParameter_GetHomingState();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_InputsOutputs(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_DIGITAL_INPUT_CONFIGURATION:
            {
                CCommand_VCS::InitCommand("DigitalInputConfiguration", DRIVE_DIGITAL_INPUT_CONFIGURATION);
                AddParameter(0, "inputNb", ODT_UINT16);
                AddParameter(1, "configuration", ODT_UINT16);
                AddParameter(2, "mask", ODT_BOOLEAN);
                AddParameter(3, "polarity", ODT_BOOLEAN);
                AddParameter(4, "executionMask", ODT_BOOLEAN);
                SetDefaultParameter_DigitalInputConfiguration();
                return TRUE;
            };
        case DRIVE_DIGITAL_OUTPUT_CONFIGURATION:
            {
                CCommand_VCS::InitCommand("DigitalOutputConfiguration", DRIVE_DIGITAL_OUTPUT_CONFIGURATION);
                AddParameter(0, "outputNb", ODT_UINT16);
                AddParameter(1, "configuration", ODT_UINT16);
                AddParameter(2, "state", ODT_BOOLEAN);
                AddParameter(3, "mask", ODT_BOOLEAN);
                AddParameter(4, "polarity", ODT_BOOLEAN);
                SetDefaultParameter_DigitalOutputConfiguration();
                return TRUE;
            };
        case DRIVE_ANALOG_INPUT_CONFIGURATION:
            {
                CCommand_VCS::InitCommand("AnalogInputConfiguration", DRIVE_ANALOG_INPUT_CONFIGURATION);
                AddParameter(0, "inputNb", ODT_UINT16);
                AddParameter(1, "configuration", ODT_UINT16);
                AddParameter(2, "executionMask", ODT_BOOLEAN);
                SetDefaultParameter_AnalogInputConfiguration();
                return TRUE;
            };
        case DRIVE_GET_ALL_DIGITAL_INPUTS:
            {
                CCommand_VCS::InitCommand("GetAllDigitalInputs", DRIVE_GET_ALL_DIGITAL_INPUTS);
                AddReturnParameter(0, "inputs", ODT_UINT16);
                SetDefaultParameter_GetAllDigitalInputs();
                return TRUE;
            };
        case DRIVE_GET_ALL_DIGITAL_OUTPUTS:
            {
                CCommand_VCS::InitCommand("GetAllDigitalOutputs", DRIVE_GET_ALL_DIGITAL_OUTPUTS);
                AddReturnParameter(0, "outputs", ODT_UINT16);
                SetDefaultParameter_GetAllDigitalOutputs();
                return TRUE;
            };
        case DRIVE_GET_ANALOG_INPUT:
            {
                CCommand_VCS::InitCommand("GetAnalogInput", DRIVE_GET_ANALOG_INPUT);
                AddParameter(0, "number", ODT_UINT16);
                AddReturnParameter(0, "analog", ODT_UINT16);
                SetDefaultParameter_GetAnalogInput();
                return TRUE;
            };
        case DRIVE_SET_ALL_DIGITAL_OUTPUTS:
            {
                CCommand_VCS::InitCommand("SetAllDigitalOutputs", DRIVE_SET_ALL_DIGITAL_OUTPUTS);
                AddParameter(0, "outputs", ODT_UINT16);
                SetDefaultParameter_SetAllDigitalOutputs();
                return TRUE;
            };
        case DRIVE_SET_ANALOG_OUTPUT:
            {
                CCommand_VCS::InitCommand("SetAnalogOutput", DRIVE_SET_ANALOG_OUTPUT);
                AddParameter(0, "number", ODT_UINT16);
                AddParameter(1, "outputValue", ODT_UINT16);
                SetDefaultParameter_SetAnalogOutput();
                return TRUE;
            };
        case DRIVE_SET_POSITION_MARKER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetPositionMarkerParameter", DRIVE_SET_POSITION_MARKER_PARAMETER);
                AddParameter(0, "number", ODT_UINT16);
                AddParameter(1, "positionMarkerEdgeType", ODT_UINT8);
                AddParameter(2, "positionMarkerMode", ODT_UINT8);
                SetDefaultParameter_SetPositionMarkerParameter();
                return TRUE;
            };
        case DRIVE_GET_POSITION_MARKER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetPositionMarkerParameter", DRIVE_GET_POSITION_MARKER_PARAMETER);
                AddReturnParameter(0, "positionMarkerEdgeType", ODT_UINT8);
                AddReturnParameter(1, "positionMarkerMode", ODT_UINT8);
                SetDefaultParameter_GetPositionMarkerParameter();
                return TRUE;
            };
        case DRIVE_ACTIVATE_POSITION_MARKER:
            {
                CCommand_VCS::InitCommand("ActivatePositionMarker", DRIVE_ACTIVATE_POSITION_MARKER);
                AddParameter(0, "number", ODT_UINT16);
                AddParameter(1, "polarity", ODT_BOOLEAN);
                SetDefaultParameter_ActivatePositionMarker();
                return TRUE;
            };
        case DRIVE_DEACTIVATE_POSITION_MARKER:
            {
                CCommand_VCS::InitCommand("DeactivatePositionMarker", DRIVE_DEACTIVATE_POSITION_MARKER);
                AddParameter(0, "number", ODT_UINT16);
                SetDefaultParameter_DeactivatePositionMarker();
                return TRUE;
            };
        case DRIVE_READ_POSITION_MARKER_COUNTER:
            {
                CCommand_VCS::InitCommand("ReadPositionMarkerCounter", DRIVE_READ_POSITION_MARKER_COUNTER);
                AddReturnParameter(0, "counter", ODT_UINT16);
                SetDefaultParameter_ReadPositionMarkerCounter();
                return TRUE;
            };
        case DRIVE_READ_POSITION_MARKER_CAPTURED_POSITION:
            {
                CCommand_VCS::InitCommand("ReadPositionMarkerCapturedPosition", DRIVE_READ_POSITION_MARKER_CAPTURED_POSITION);
                AddParameter(0, "counterIndex", ODT_UINT16);
                AddReturnParameter(0, "capturedPosition", ODT_INT32);
                SetDefaultParameter_ReadPositionMarkerCapturedPosition();
                return TRUE;
            };
        case DRIVE_RESET_POSITION_MARKER_COUNTER:
            {
                CCommand_VCS::InitCommand("ResetPositionMarkerCounter", DRIVE_RESET_POSITION_MARKER_COUNTER);
                SetDefaultParameter_ResetPositionMarkerCounter();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_MotionInfo(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_CURRENT_IS:
            {
                CCommand_VCS::InitCommand("GetCurrentIs", DRIVE_GET_CURRENT_IS);
                AddReturnParameter(0, "currentIs", ODT_INT16);
                SetDefaultParameter_GetCurrentIs();
                return TRUE;
            };
        case DRIVE_GET_CURRENT_IS_AVERAGED:
            {
                CCommand_VCS::InitCommand("GetCurrentIsAveraged", DRIVE_GET_CURRENT_IS_AVERAGED);
                AddReturnParameter(0, "CurrentIsAveraged", ODT_INT16);
                SetDefaultParameter_GetCurrentIsAveraged();
                return TRUE;
            };
        case DRIVE_GET_MOVEMENT_STATE:
            {
                CCommand_VCS::InitCommand("GetMovementState", DRIVE_GET_MOVEMENT_STATE);
                AddReturnParameter(0, "targetReached", ODT_BOOLEAN);
                SetDefaultParameter_GetMovementState();
                return TRUE;
            };
        case DRIVE_GET_POSITION_IS:
            {
                CCommand_VCS::InitCommand("GetPositionIs", DRIVE_GET_POSITION_IS);
                AddReturnParameter(0, "positionIs", ODT_INT32);
                SetDefaultParameter_GetPositionIs();
                return TRUE;
            };
        case DRIVE_GET_VELOCITY_IS:
            {
                CCommand_VCS::InitCommand("GetVelocityIs", DRIVE_GET_VELOCITY_IS);
                AddReturnParameter(0, "velocityIs", ODT_INT32);
                SetDefaultParameter_GetVelocityIs();
                return TRUE;
            };
        case DRIVE_GET_VELOCITY_IS_AVERAGED:
            {
                CCommand_VCS::InitCommand("GetVelocityIsAveraged", DRIVE_GET_VELOCITY_IS_AVERAGED);
                AddReturnParameter(0, "VelocityIsAveraged", ODT_INT32);
                SetDefaultParameter_GetVelocityIsAveraged();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_PositionMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_POSITION_MUST:
            {
                CCommand_VCS::InitCommand("GetPositionMust", DRIVE_GET_POSITION_MUST);
                AddReturnParameter(0, "positionMust", ODT_INT32);
                SetDefaultParameter_GetPositionMust();
                return TRUE;
            };

        case DRIVE_SET_POSITION_MUST:
            {
                CCommand_VCS::InitCommand("SetPositionMust", DRIVE_SET_POSITION_MUST);
                AddParameter(0, "positionMust", ODT_INT32);
                SetDefaultParameter_SetPositionMust();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_ProfilePositionMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_POSITION_PROFILE:
            {
                CCommand_VCS::InitCommand("GetPositionProfile", DRIVE_GET_POSITION_PROFILE);
                AddReturnParameter(0, "profileVelocity", ODT_UINT32);
                AddReturnParameter(1, "profileAcceleration", ODT_UINT32);
                AddReturnParameter(2, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_GetPositionProfile();
                return TRUE;
            };
        case DRIVE_GET_TARGET_POSITION:
            {
                CCommand_VCS::InitCommand("GetTargetPosition", DRIVE_GET_TARGET_POSITION);
                AddReturnParameter(0, "targetPosition", ODT_INT32);
                SetDefaultParameter_GetTargetPosition();
                return TRUE;
            };
        case DRIVE_HALT_POSITION_MOVEMENT:
            {
                CCommand_VCS::InitCommand("HaltPositionMovement", DRIVE_HALT_POSITION_MOVEMENT);
                AddParameter(0, "nodeID", ODT_UINT16, FALSE, FALSE);
                SetDefaultParameter_HaltPositionMovement();
                return TRUE;
            };
        case DRIVE_MOVE_TO_POSITION:
            {
                CCommand_VCS::InitCommand("MoveToPosition", DRIVE_MOVE_TO_POSITION);
                AddParameter(0, "targetPosition", ODT_INT32);
                AddParameter(1, "absolute", ODT_BOOLEAN);
                AddParameter(2, "immediately", ODT_BOOLEAN);
                SetDefaultParameter_MoveToPosition();
                return TRUE;
            };
        case DRIVE_SET_POSITION_PROFILE:
            {
                CCommand_VCS::InitCommand("SetPositionProfile", DRIVE_SET_POSITION_PROFILE);
                AddParameter(0, "profileVelocity", ODT_UINT32);
                AddParameter(1, "profileAcceleration", ODT_UINT32);
                AddParameter(2, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_SetPositionProfile();
                return TRUE;
            };
        case DRIVE_ENABLE_POSITION_WINDOW:
            {
                CCommand_VCS::InitCommand("EnablePositionWindow", DRIVE_ENABLE_POSITION_WINDOW);
                AddParameter(0, "positionWindow", ODT_UINT32);
                AddParameter(1, "positionWindowTime", ODT_UINT16);
                SetDefaultParameter_EnablePositionWindow();
                return TRUE;
            };
        case DRIVE_DISABLE_POSITION_WINDOW:
            {
                CCommand_VCS::InitCommand("DisablePositionWindow", DRIVE_DISABLE_POSITION_WINDOW);
                SetDefaultParameter_DisablePositionWindow();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_ProfileVelocityMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_TARGET_VELOCITY:
            {
                CCommand_VCS::InitCommand("GetTargetVelocity", DRIVE_GET_TARGET_VELOCITY);
                AddReturnParameter(0, "targetVelocity", ODT_INT32);
                SetDefaultParameter_GetTargetVelocity();
                return TRUE;
            };
        case DRIVE_GET_VELOCITY_PROFILE:
            {
                CCommand_VCS::InitCommand("GetVelocityProfile", DRIVE_GET_VELOCITY_PROFILE);
                AddReturnParameter(0, "profileAcceleration", ODT_UINT32);
                AddReturnParameter(1, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_GetVelocityProfile();
                return TRUE;
            };
        case DRIVE_HALT_VELOCITY_MOVEMENT:
            {
                CCommand_VCS::InitCommand("HaltVelocityMovement", DRIVE_HALT_VELOCITY_MOVEMENT);
                SetDefaultParameter_HaltVelocityMovement();
                return TRUE;
            };
        case DRIVE_MOVE_WITH_VELOCITY:
            {
                CCommand_VCS::InitCommand("MoveWithVelocity", DRIVE_MOVE_WITH_VELOCITY);
                AddParameter(0, "targetVelocity", ODT_INT32);
                SetDefaultParameter_MoveWithVelocity();
                return TRUE;
            };
        case DRIVE_SET_VELOCITY_PROFILE:
            {
                CCommand_VCS::InitCommand("SetVelocityProfile", DRIVE_SET_VELOCITY_PROFILE);
                AddParameter(0, "profileAcceleration", ODT_UINT32);
                AddParameter(1, "profileDeceleration", ODT_UINT32);
                SetDefaultParameter_SetVelocityProfile();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_StateMachine(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_CLEAR_FAULT:
            {
                CCommand_VCS::InitCommand("ClearFault", DRIVE_CLEAR_FAULT);
                SetDefaultParameter_ClearFault();
                return TRUE;
            };
        case DRIVE_GET_DISABLE_STATE:
            {
                CCommand_VCS::InitCommand("GetDisableState", DRIVE_GET_DISABLE_STATE);
                AddReturnParameter(0, "disabled", ODT_BOOLEAN);
                SetDefaultParameter_GetDisableState();
                return TRUE;
            };
        case DRIVE_GET_ENABLE_STATE:
            {
                CCommand_VCS::InitCommand("GetEnableState", DRIVE_GET_ENABLE_STATE);
                AddReturnParameter(0, "enabled", ODT_BOOLEAN);
                SetDefaultParameter_GetEnableState();
                return TRUE;
            };
        case DRIVE_GET_FAULT_STATE:
            {
                CCommand_VCS::InitCommand("GetFaultState", DRIVE_GET_FAULT_STATE);
                AddReturnParameter(0, "isInFault", ODT_BOOLEAN);
                SetDefaultParameter_GetFaultState();
                return TRUE;
            };
        case DRIVE_GET_OPERATION_MODE:
            {
                CCommand_VCS::InitCommand("GetOperationMode", DRIVE_GET_OPERATION_MODE);
                AddReturnParameter(0, "mode", ODT_INT8);
                SetDefaultParameter_GetOperationMode();
                return TRUE;
            };
        case DRIVE_GET_QUICK_STOP_STATE:
            {
                CCommand_VCS::InitCommand("GetQuickStopState", DRIVE_GET_QUICK_STOP_STATE);
                AddReturnParameter(0, "isQuickStoped", ODT_BOOLEAN);
                SetDefaultParameter_GetQuickStopState();
                return TRUE;
            };
        case DRIVE_SET_DISABLE_STATE:
            {
                CCommand_VCS::InitCommand("SetDisableState", DRIVE_SET_DISABLE_STATE);
                SetDefaultParameter_SetDisableState();
                return TRUE;
            };
        case DRIVE_SET_ENABLE_STATE:
            {
                CCommand_VCS::InitCommand("SetEnableState", DRIVE_SET_ENABLE_STATE);
                SetDefaultParameter_SetEnableState();
                return TRUE;
            };
        case DRIVE_SET_OPERATION_MODE:
            {
                CCommand_VCS::InitCommand("SetOperationMode", DRIVE_SET_OPERATION_MODE);
                AddParameter(0, "mode", ODT_INT8);
                SetDefaultParameter_SetOperationMode();
                return TRUE;
            };
        case DRIVE_SET_QUICK_STOP_STATE:
            {
                CCommand_VCS::InitCommand("SetQuickStopState", DRIVE_SET_QUICK_STOP_STATE);
                SetDefaultParameter_SetQuickStopState();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_VelocityMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_GET_VELOCITY_MUST:
            {
                CCommand_VCS::InitCommand("GetVelocityMust", DRIVE_GET_VELOCITY_MUST);
                AddReturnParameter(0, "velocityMust", ODT_INT32);
                SetDefaultParameter_GetVelocityMust();
                return TRUE;
            };

        case DRIVE_SET_VELOCITY_MUST:
            {
                CCommand_VCS::InitCommand("SetVelocityMust", DRIVE_SET_VELOCITY_MUST);
                AddParameter(0, "velocityMust", ODT_INT32);
                SetDefaultParameter_SetVelocityMust();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Drive::InitCommand_MasterEncoderMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_SET_MASTER_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetMasterEncoderParameter", DRIVE_SET_MASTER_ENCODER_PARAMETER);
                AddParameter(0, "scalingNumerator", ODT_UINT16);
                AddParameter(1, "scalingDenominator", ODT_UINT16);
                AddParameter(2, "polarity", ODT_UINT8);
                AddParameter(3, "maxVelocity", ODT_UINT32);
                AddParameter(4, "maxAcceleration", ODT_UINT32);
                SetDefaultParameter_SetMasterEncoderParameter();
                return TRUE;
            };

        case DRIVE_GET_MASTER_ENCODER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetMasterEncoderParameter", DRIVE_GET_MASTER_ENCODER_PARAMETER);
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

BOOL CCommand_VCS_Drive::InitCommand_StepDirectionMode(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE_SET_STEP_DIRECTION_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetStepDirectionParameter", DRIVE_SET_STEP_DIRECTION_PARAMETER);
                AddParameter(0, "scalingNumerator", ODT_UINT16);
                AddParameter(1, "scalingDenominator", ODT_UINT16);
                AddParameter(2, "polarity", ODT_UINT8);
                AddParameter(3, "maxVelocity", ODT_UINT32);
                AddParameter(4, "maxAcceleration", ODT_UINT32);
                SetDefaultParameter_SetStepDirectionParameter();
                return TRUE;
            };

        case DRIVE_GET_STEP_DIRECTION_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetStepDirectionParameter", DRIVE_GET_STEP_DIRECTION_PARAMETER);
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

void CCommand_VCS_Drive::SetDefaultParameter_GetCurrentRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usP, sizeof(usP));
    SetReturnParameterData(1, &usI, sizeof(usI));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetEncoderParameter()
{
    WORD usCounts = 500;
    WORD usPositionSensorType = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usCounts, sizeof(usCounts));
    SetReturnParameterData(1, &usPositionSensorType, sizeof(usPositionSensorType));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetSensorType()
{
    WORD usSensorType = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usSensorType, sizeof(usSensorType));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetIncEncoderParameter()
{
    WORD usEncoderResolution = 500;
    BOOL oInvertedPolarity = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usEncoderResolution, sizeof(usEncoderResolution));
    SetReturnParameterData(1, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetHallSensorParameter()
{
    BOOL oInvertedPolarity = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetMotorParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetMotorType()
{
    WORD usMotorType = 10;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usMotorType, sizeof(usMotorType));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetDcMotorParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetEcMotorParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetPositionRegulatorGain()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetPositionRegulatorFeedForward()
{
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
    SetReturnParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetVelocityRegulatorGain()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetMaxFollowingError()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetMaxProfileVelocity()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetVelocityUnits()
{
    BYTE ubVelocityDimension = 0xA4;
    char bVelocityNotation = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ubVelocityDimension, sizeof(ubVelocityDimension));
    SetReturnParameterData(1, &bVelocityNotation, sizeof(bVelocityNotation));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetCurrentRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;

    //Parameter
    SetParameterData(0, &usP, sizeof(usP));
    SetParameterData(1, &usI, sizeof(usI));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetEncoderParameter()
{
    WORD usCounts = 500;
    WORD usPositionSensorType = 1;

    //Parameter
    SetParameterData(0, &usCounts, sizeof(usCounts));
    SetParameterData(1, &usPositionSensorType, sizeof(usPositionSensorType));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetSensorType()
{
    WORD usSensorType = 1;

    //Parameter
    SetParameterData(0, &usSensorType, sizeof(usSensorType));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetIncEncoderParameter()
{
    WORD usEncoderResolution = 500;
    BOOL oInvertedPolarity = FALSE;

    //Parameter
    SetParameterData(0, &usEncoderResolution, sizeof(usEncoderResolution));
    SetParameterData(1, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetHallSensorParameter()
{
    BOOL oInvertedPolarity = FALSE;

    //Parameter
    SetParameterData(0, &oInvertedPolarity, sizeof(oInvertedPolarity));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetMotorParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_SetMotorType()
{
    WORD usMotorType = 10;

    //Parameter
    SetParameterData(0, &usMotorType, sizeof(usMotorType));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetDcMotorParameter()
{
    WORD usContinuousCurrent = 1;
    WORD usMaxOutputCurrent = 1;
    WORD usThermalTimeConstant = 40;

    //Parameter
    SetParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
    SetParameterData(1, &usMaxOutputCurrent, sizeof(usMaxOutputCurrent));
    SetParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetEcMotorParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_SetPositionRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;
    WORD usD = 100;

    //Parameter
    SetParameterData(0, &usP, sizeof(usP));
    SetParameterData(1, &usI, sizeof(usI));
    SetParameterData(2, &usD, sizeof(usD));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetPositionRegulatorFeedForward()
{
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    //Parameter
    SetParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
    SetParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetVelocityRegulatorGain()
{
    WORD usP = 100;
    WORD usI = 100;

    //Parameter
    SetParameterData(0, &usP, sizeof(usP));
    SetParameterData(1, &usI, sizeof(usI));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetMaxFollowingError()
{
    DWORD ulMaxFollowingError = 2000;

    //Parameter
    SetParameterData(0, &ulMaxFollowingError, sizeof(ulMaxFollowingError));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetMaxProfileVelocity()
{
    DWORD ulMaxProfileVelocity = 25000;

    //Parameter
    SetParameterData(0, &ulMaxProfileVelocity, sizeof(ulMaxProfileVelocity));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetVelocityUnits()
{
    BYTE ubVelocityDimension = 0xA4;
    char bVelocityNotation = 0;

    //Parameter
    SetParameterData(0, &ubVelocityDimension, sizeof(ubVelocityDimension));
    SetParameterData(1, &bVelocityNotation, sizeof(bVelocityNotation));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetCurrentMust()
{
    short sCurrentMust = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &sCurrentMust, sizeof(sCurrentMust));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetCurrentMust()
{
    short sCurrentMust = 0;

    //Parameter
    SetParameterData(0, &sCurrentMust, sizeof(sCurrentMust));
}

void CCommand_VCS_Drive::SetDefaultParameter_FindHome()
{
    __int8 bHomingMethod = 0;

    //Parameter
    SetParameterData(0, &bHomingMethod, sizeof(bHomingMethod));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetHomingParameter()
{
    DWORD ulHomingAcceleration = 0;
    DWORD ulSpeedSwitch = 0;
    DWORD ulSpeedIndex = 0;
    int lHomeOffset = 0;
    WORD usCurrentThreshold = 0;
    int lHomePosition = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ulHomingAcceleration, sizeof(ulHomingAcceleration));
    SetReturnParameterData(1, &ulSpeedSwitch, sizeof(ulSpeedSwitch));
    SetReturnParameterData(2, &ulSpeedIndex, sizeof(ulSpeedIndex));
    SetReturnParameterData(3, &lHomeOffset, sizeof(lHomeOffset));
    SetReturnParameterData(4, &usCurrentThreshold, sizeof(usCurrentThreshold));
    SetReturnParameterData(5, &lHomePosition, sizeof(lHomePosition));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetHomingParameter()
{
    DWORD ulHomingAcceleration = 0;
    DWORD ulSpeedSwitch = 0;
    DWORD ulSpeedIndex = 0;
    int lHomeOffset = 0;
    WORD usCurrentThreshold = 0;
    int lHomePosition = 0;

    //Parameter
    SetParameterData(0, &ulHomingAcceleration, sizeof(ulHomingAcceleration));
    SetParameterData(1, &ulSpeedSwitch, sizeof(ulSpeedSwitch));
    SetParameterData(2, &ulSpeedIndex, sizeof(ulSpeedIndex));
    SetParameterData(3, &lHomeOffset, sizeof(lHomeOffset));
    SetParameterData(4, &usCurrentThreshold, sizeof(usCurrentThreshold));
    SetParameterData(5, &lHomePosition, sizeof(lHomePosition));
}

void CCommand_VCS_Drive::SetDefaultParameter_StopHoming()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_DefinePosition()
{
    int lHomePosition = 0;

    //Parameter
    SetParameterData(0, &lHomePosition, sizeof(lHomePosition));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetHomingState()
{
    BOOL oHomingAttained = FALSE;
    BOOL oHomingError = FALSE;

    //ReturnParameter
    SetReturnParameterData(0, &oHomingAttained, sizeof(oHomingAttained));
    SetReturnParameterData(1, &oHomingError, sizeof(oHomingError));
}

void CCommand_VCS_Drive::SetDefaultParameter_DigitalInputConfiguration()
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

void CCommand_VCS_Drive::SetDefaultParameter_DigitalOutputConfiguration()
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

void CCommand_VCS_Drive::SetDefaultParameter_AnalogInputConfiguration()
{
    WORD usInputNb = 0;
    WORD usConfiguration = 0x0000;
    BOOL oExecutionMask = FALSE;

    //Parameter
    SetParameterData(0, &usInputNb, sizeof(usInputNb));
    SetParameterData(1, &usConfiguration, sizeof(usConfiguration));
    SetParameterData(2, &oExecutionMask, sizeof(oExecutionMask));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetAllDigitalInputs()
{
    WORD usInputs = 0x0000;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usInputs, sizeof(usInputs));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetAllDigitalOutputs()
{
    WORD usOutputs = 0x0000;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usOutputs, sizeof(usOutputs));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetAnalogInput()
{
    WORD usNumber = 0;
    WORD usAnalog = 0;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));

    //ReturnParameter
    SetReturnParameterData(0, &usAnalog, sizeof(usAnalog));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetAllDigitalOutputs()
{
    WORD usOutputs = 0x0000;

    //Parameter
    SetParameterData(0, &usOutputs, sizeof(usOutputs));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetAnalogOutput()
{
    WORD usNumber = 0;
    WORD usOutput = 0x0000;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
    SetParameterData(1, &usOutput, sizeof(usOutput));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetPositionMarkerParameter()
{
    BYTE ubPositionMarkerEdgeType = 0;
    BYTE ubPositionMarkerMode = 1;

    //Parameter
    SetParameterData(0, &ubPositionMarkerEdgeType, sizeof(ubPositionMarkerEdgeType));
    SetParameterData(1, &ubPositionMarkerMode, sizeof(ubPositionMarkerMode));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetPositionMarkerParameter()
{
    BYTE ubPositionMarkerEdgeType = 0;
    BYTE ubPositionMarkerMode = 1;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ubPositionMarkerEdgeType, sizeof(ubPositionMarkerEdgeType));
    SetReturnParameterData(1, &ubPositionMarkerMode, sizeof(ubPositionMarkerMode));
}

void CCommand_VCS_Drive::SetDefaultParameter_ActivatePositionMarker()
{
    WORD usNumber = 0;
    BOOL oPolarity = FALSE;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
    SetParameterData(1, &oPolarity, sizeof(oPolarity));
}

void CCommand_VCS_Drive::SetDefaultParameter_DeactivatePositionMarker()
{
    WORD usNumber = 0;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
}

void CCommand_VCS_Drive::SetDefaultParameter_ReadPositionMarkerCounter()
{
    WORD usCount = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usCount, sizeof(usCount));
}

void CCommand_VCS_Drive::SetDefaultParameter_ReadPositionMarkerCapturedPosition()
{
    WORD usCounterIndex = 0;
    int lCapturedPosition = 0;

    //Parameter
    SetParameterData(0, &usCounterIndex, sizeof(usCounterIndex));

    //ReturnParameter
    SetReturnParameterData(0, &lCapturedPosition, sizeof(lCapturedPosition));
}

void CCommand_VCS_Drive::SetDefaultParameter_ResetPositionMarkerCounter()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_GetCurrentIs()
{
    short sCurrentIs = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &sCurrentIs, sizeof(sCurrentIs));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetCurrentIsAveraged()
{
    short sCurrentIsAveraged = 0;

    //ReturnParameter
    SetReturnParameterData(0, &sCurrentIsAveraged, sizeof(sCurrentIsAveraged));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetMovementState()
{
    BOOL oTargetReached = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oTargetReached, sizeof(oTargetReached));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetPositionIs()
{
    int lPositionIs = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lPositionIs, sizeof(lPositionIs));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetVelocityIs()
{
    int lVelocityIs = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lVelocityIs, sizeof(lVelocityIs));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetVelocityIsAveraged()
{
    int lVelocityIsAveraged = 0;

    //ReturnParameter
    SetReturnParameterData(0, &lVelocityIsAveraged, sizeof(lVelocityIsAveraged));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetPositionMust()
{
    int lPositionMust = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lPositionMust, sizeof(lPositionMust));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetPositionMust()
{
    int lPositionMust = 0;

    //Parameter
    SetParameterData(0, &lPositionMust, sizeof(lPositionMust));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetPositionProfile()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetTargetPosition()
{
    int lTargetPosition = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lTargetPosition, sizeof(lTargetPosition));
}

void CCommand_VCS_Drive::SetDefaultParameter_HaltPositionMovement()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_MoveToPosition()
{
    int lTargetPosition = 1000;
    BOOL oAbsolute = FALSE;
    BOOL oImmediately = TRUE;

    //Parameter
    SetParameterData(0, &lTargetPosition, sizeof(lTargetPosition));
    SetParameterData(1, &oAbsolute, sizeof(oAbsolute));
    SetParameterData(2, &oImmediately, sizeof(oImmediately));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetPositionProfile()
{
    DWORD ulProfileVelocity = 1000;
    DWORD ulProfileAcceleration = 10000;
    DWORD ulProfileDeceleration = 10000;

    //Parameter
    SetParameterData(0, &ulProfileVelocity, sizeof(ulProfileVelocity));
    SetParameterData(1, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
    SetParameterData(2, &ulProfileDeceleration, sizeof(ulProfileDeceleration));
}

void CCommand_VCS_Drive::SetDefaultParameter_EnablePositionWindow()
{
    DWORD ulPositionWindow = ULONG_MAX;
    WORD usPositionWindowTime = 0;

    //Parameter
    SetParameterData(0, &ulPositionWindow, sizeof(ulPositionWindow));
    SetParameterData(1, &usPositionWindowTime, sizeof(usPositionWindowTime));
}

void CCommand_VCS_Drive::SetDefaultParameter_DisablePositionWindow()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_GetTargetVelocity()
{
    int lTargetVelocity = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetVelocityProfile()
{
    DWORD ulProfileAcceleration = 0;
    DWORD ulProfileDeceleration = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
    SetReturnParameterData(1, &ulProfileDeceleration, sizeof(ulProfileDeceleration));
}

void CCommand_VCS_Drive::SetDefaultParameter_HaltVelocityMovement()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_MoveWithVelocity()
{
    int lTargetVelocity = 0;

    //Parameter
    SetParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetVelocityProfile()
{
    DWORD ulProfileAcceleration = 0;
    DWORD ulProfileDeceleration = 0;

    //Parameter
    SetParameterData(0, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
    SetParameterData(1, &ulProfileDeceleration, sizeof(ulProfileDeceleration));
}

void CCommand_VCS_Drive::SetDefaultParameter_ClearFault()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_GetDisableState()
{
    BOOL oIsDisabled = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsDisabled, sizeof(oIsDisabled));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetEnableState()
{
    BOOL oIsEnabled = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsEnabled, sizeof(oIsEnabled));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetFaultState()
{
    BOOL oIsInFault = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsInFault, sizeof(oIsInFault));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetOperationMode()
{
    __int8 bMode = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &bMode, sizeof(bMode));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetQuickStopState()
{
    BOOL oIsQuickStoped = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oIsQuickStoped, sizeof(oIsQuickStoped));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetDisableState()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_SetEnableState()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_SetOperationMode()
{
    __int8 bMode = 0;

    //Parameter
    SetParameterData(0, &bMode, sizeof(bMode));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetQuickStopState()
{

    //Parameter
}

void CCommand_VCS_Drive::SetDefaultParameter_GetVelocityMust()
{
    int lVelocityMust = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &lVelocityMust, sizeof(lVelocityMust));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetVelocityMust()
{
    int lVelocityMust = 0;

    //Parameter
    SetParameterData(0, &lVelocityMust, sizeof(lVelocityMust));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetSerialNumber()
{
    unsigned __int64 ullSerialNb = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ullSerialNb, sizeof(ullSerialNb));
}

void CCommand_VCS_Drive::SetDefaultParameter_GetBaseSectorVersion()
{
    WORD usBaseSectorVersion = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usBaseSectorVersion, sizeof(usBaseSectorVersion));
}

void CCommand_VCS_Drive::SetDefaultParameter_SetStepDirectionParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetStepDirectionParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_SetMasterEncoderParameter()
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

void CCommand_VCS_Drive::SetDefaultParameter_GetMasterEncoderParameter()
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

