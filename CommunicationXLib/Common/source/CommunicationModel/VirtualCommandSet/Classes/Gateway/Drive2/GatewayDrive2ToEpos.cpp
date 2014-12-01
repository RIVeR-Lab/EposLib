// GatewayDrive2ToEpos.cpp: Implementierung der Klasse CGatewayDrive2ToEpos.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GatewayDrive2ToEpos.h"


#include <memory.h>
#include <malloc.h>
#include <Classes/MmcDataConversion.h>
#include <Process/MmcProcess.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_DataRecorder.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_InputsOutputs.h>

#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenCommunicationDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenDriveDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEposDef.h>
#include <Classes/TypeDefinitions.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CGatewayDrive2ToEpos::CGatewayDrive2ToEpos()
{
    InitErrorHandling();
}

CGatewayDrive2ToEpos::~CGatewayDrive2ToEpos()
{
}

CGateway* CGatewayDrive2ToEpos::Clone()
{
    CGatewayDrive2ToEpos* pClonedGateway;

    pClonedGateway = new CGatewayDrive2ToEpos();
    *pClonedGateway = *this;

    return (CGatewayVCStoDCS*)pClonedGateway;
}

CGatewayDrive2ToEpos& CGatewayDrive2ToEpos::operator=(CGatewayDrive2ToEpos& other)
{
    if(this != &other)
    {
        *((CGatewayVCStoEpos*)this) = *((CGatewayVCStoEpos*)&other);
    }

    return *this;
}

BOOL CGatewayDrive2ToEpos::ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE h_Handle, HANDLE p_hTransactionHandle)
{
    CCommand_VCS* pCommand_VCS = NULL;
    CDeviceCommandSetManagerBase* pDeviceCommandSetManager = NULL;
    BOOL oResult = FALSE;

    if(CGateway::ProcessCommand(p_pCommand, p_pManager, h_Handle, p_hTransactionHandle))
    {
        if(CheckLayers(p_pCommand, p_pManager))
        {
            pCommand_VCS = (CCommand_VCS*)p_pCommand;
            pDeviceCommandSetManager = (CDeviceCommandSetManagerBase*)p_pManager;

            //Lock CriticalSection
            if(Lock(p_pCommand))
            {
                //Process Command
                switch(p_pCommand->GetCommandId())
                {
                    //Configuration
                    case DRIVE2_GET_CURRENT_REGULATOR_GAIN: oResult = Process_GetCurrentRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_POSITION_REGULATOR_GAIN: oResult = Process_GetPositionRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_POSITION_REGULATOR_FEED_FORWARD: oResult = Process_GetPositionRegulatorFeedForward(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_VELOCITY_REGULATOR_GAIN: oResult = Process_GetVelocityRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_MAX_FOLLOWING_ERROR: oResult = Process_GetMaxFollowingError(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_MAX_PROFILE_VELOCITY: oResult = Process_GetMaxProfileVelocity(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_CURRENT_REGULATOR_GAIN: oResult = Process_SetCurrentRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_POSITION_REGULATOR_GAIN: oResult = Process_SetPositionRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_POSITION_REGULATOR_FEED_FORWARD: oResult = Process_SetPositionRegulatorFeedForward(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_VELOCITY_REGULATOR_GAIN: oResult = Process_SetVelocityRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_MAX_FOLLOWING_ERROR: oResult = Process_SetMaxFollowingError(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_MAX_PROFILE_VELOCITY: oResult = Process_SetMaxProfileVelocity(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Configuration Motor
                    case DRIVE2_GET_MOTOR_PARAMETER: oResult = Process_GetMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_MOTOR_TYPE: oResult = Process_GetMotorType(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_DC_MOTOR_PARAMETER: oResult = Process_GetDcMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_EC_MOTOR_PARAMETER: oResult = Process_GetEcMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_MOTOR_PARAMETER: oResult = Process_SetMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_MOTOR_TYPE: oResult = Process_SetMotorType(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_DC_MOTOR_PARAMETER: oResult = Process_SetDcMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_EC_MOTOR_PARAMETER: oResult = Process_SetEcMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Configuration Sensor
                    case DRIVE2_GET_ENCODER_PARAMETER: oResult = Process_GetEncoderParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_SENSOR_TYPE: oResult = Process_GetSensorType(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_INC_ENCODER_PARAMETER: oResult = Process_GetIncEncoderParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_HALL_SENSOR_PARAMETER: oResult = Process_GetHallSensorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_ENCODER_PARAMETER: oResult = Process_SetEncoderParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_SENSOR_TYPE: oResult = Process_SetSensorType(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_INC_ENCODER_PARAMETER: oResult = Process_SetIncEncoderParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_HALL_SENSOR_PARAMETER: oResult = Process_SetHallSensorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //CurrentMode
                    case DRIVE2_GET_CURRENT_MUST: oResult = Process_GetCurrentMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_CURRENT_MUST: oResult = Process_SetCurrentMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //HomingMode
                    case DRIVE2_FIND_HOME: oResult = Process_FindHome(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_HOMING_PARAMETER: oResult = Process_GetHomingParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_HOMING_PARAMETER: oResult = Process_SetHomingParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_STOP_HOMING: oResult = Process_StopHoming(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_DEFINE_POSITION: oResult = Process_DefinePosition(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_HOMING_STATE: oResult = Process_GetHomingState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //InputOutputs
                    case IO_DIGITAL_INPUT_CONFIGURATION: oResult = Process_DigitalInputConfiguration(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_DIGITAL_OUTPUT_CONFIGURATION: oResult = Process_DigitalOutputConfiguration(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_GET_ALL_DIGITAL_INPUTS: oResult = Process_GetAllDigitalInputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_GET_ALL_DIGITAL_OUTPUTS: oResult = Process_GetAllDigitalOutputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_GET_ANALOG_INPUT: oResult = Process_GetAnalogInput(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_SET_ALL_DIGITAL_OUTPUTS: oResult = Process_SetAllDigitalOutputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    case DRIVE2_SET_POSITION_MARKER_PARAMETER: oResult = Process_SetPositionMarkerParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_POSITION_MARKER_PARAMETER: oResult = Process_GetPositionMarkerParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_ACTIVATE_POSITION_MARKER: oResult = Process_ActivatePositionMarker(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_DEACTIVATE_POSITION_MARKER: oResult = Process_DeactivatePositionMarker(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_READ_POSITION_MARKER_COUNTER: oResult = Process_ReadPositionMarkerCounter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_READ_POSITION_MARKER_CAPTURED_POSITION: oResult = Process_ReadPositionMarkerCapturedPosition(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_RESET_POSITION_MARKER_COUNTER: oResult = Process_ResetPositionMarkerCounter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //MotionInfo
                    case DRIVE2_GET_CURRENT_IS: oResult = Process_GetCurrentIs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_CURRENT_IS_AVERAGED: oResult = Process_GetCurrentIsAveraged(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_MOVEMENT_STATE: oResult = Process_GetMovementState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_POSITION_IS: oResult = Process_GetPositionIs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_VELOCITY_IS: oResult = Process_GetVelocityIs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_VELOCITY_IS_AVERAGED: oResult = Process_GetVelocityIsAveraged(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //PositionMode
                    case DRIVE2_GET_POSITION_MUST: oResult = Process_GetPositionMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_POSITION_MUST: oResult = Process_SetPositionMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //ProfilePositionMode
                    case DRIVE2_GET_POSITION_PROFILE: oResult = Process_GetPositionProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_TARGET_POSITION: oResult = Process_GetTargetPosition(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_HALT_POSITION_MOVEMENT: oResult = Process_HaltPositionMovement(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_MOVE_TO_POSITION: oResult = Process_MoveToPosition(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_POSITION_PROFILE: oResult = Process_SetPositionProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_ENABLE_POSITION_WINDOW: oResult = Process_EnablePositionWindow(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_DISABLE_POSITION_WINDOW: oResult = Process_DisablePositionWindow(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //ProfileVelocityMode
                    case DRIVE2_GET_TARGET_VELOCITY: oResult = Process_GetTargetVelocity(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_VELOCITY_PROFILE: oResult = Process_GetVelocityProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_HALT_VELOCITY_MOVEMENT: oResult = Process_HaltVelocityMovement(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_MOVE_WITH_VELOCITY: oResult = Process_MoveWithVelocity(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_VELOCITY_PROFILE: oResult = Process_SetVelocityProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //StateMachine
                    case DRIVE2_CLEAR_FAULT: oResult = Process_ClearFault(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_DISABLE_STATE: oResult = Process_GetDisableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_ENABLE_STATE: oResult = Process_GetEnableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_FAULT_STATE: oResult = Process_GetFaultState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_OPERATION_MODE: oResult = Process_GetOperationMode(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_QUICK_STOP_STATE: oResult = Process_GetQuickStopState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_DISABLE_STATE: oResult = Process_SetDisableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_ENABLE_STATE: oResult = Process_SetEnableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_OPERATION_MODE: oResult = Process_SetOperationMode(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_QUICK_STOP_STATE: oResult = Process_SetQuickStopState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //VelocityMode
                    case DRIVE2_GET_VELOCITY_MUST: oResult = Process_GetVelocityMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_SET_VELOCITY_MUST: oResult = Process_SetVelocityMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //FirmwareDownload
                    case DRIVE2_GET_BASE_SECTOR_VERSION: oResult = Process_GetBaseSectorVersion(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE2_GET_SERIAL_NUMBER: oResult = Process_GetSerialNumber(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Object Dictionary
                    case VCS_CANOPEN_GET_OBJECT: oResult = Process_GetObject(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_SET_OBJECT: oResult = Process_SetObject(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_RESTORE: oResult = Process_Restore(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_STORE: oResult = Process_Store(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //GeneralGateway
                    case VCS_CANOPEN_REQUEST_CAN_FRAME: oResult = Process_RequestCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_SEND_CAN_FRAME: oResult = Process_SendCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_READ_CAN_FRAME: oResult = Process_ReadCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_SEND_NMT_SERVICE: oResult = Process_SendNMTService(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //LayerSettingServices
                    case VCS_CANOPEN_SEND_LSS_FRAME: oResult = Process_SendLSSFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_READ_LSS_FRAME: oResult = Process_ReadLSSFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //DataRecorder Setup
                    case REC_SET_RECORDER_PARAMETER: oResult = Process_SetDataRecordingParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_GET_RECORDER_PARAMETER: oResult = Process_GetDataRecordingParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_ENABLE_TRIGGER: oResult = Process_EnableTrigger(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_DISABLE_ALL_TRIGGERS: oResult = Process_DisableAllTriggers(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_ACTIVATE_CHANNEL: oResult = Process_ActivateChannel(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_DEACTIVATE_ALL_CHANNELS: oResult = Process_DeactivateAllChannels(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //DataRecorder Status
                    case REC_START_RECORDER: oResult = Process_StartRecorder(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_STOP_RECORDER: oResult = Process_StopRecorder(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_FORCE_TRIGGER: oResult = Process_ForceTrigger(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_IS_RECORDER_RUNNING: oResult = Process_IsRecorderRunning(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case REC_IS_RECORDER_TRIGGERED: oResult = Process_IsRecorderTriggered(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //DataRecorder Data
                    case REC_READ_CHANNEL_VECTOR_SIZE: oResult = Process_ReadChannelVectorSize(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //DataRecorder Advanced Functions
                    case REC_READ_DATA_BUFFER: oResult = Process_ReadDataBuffer(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //ErrorHandling
                    case COMMON_RESET_DEVICE: oResult = Process_ResetDevice(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case COMMON_WAIT_FOR_REBOOT: oResult = Process_WaitForReboot(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case COMMON_CLEAR_DEVICE_ERRORS: oResult = Process_ClearDeviceErrors(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case COMMON_GET_NB_OF_DEVICE_ERRORS: oResult = Process_GetNbOfDeviceErrors(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case COMMON_GET_DEVICE_ERROR_CODE: oResult = Process_GetDeviceErrorCode(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case COMMON_GOTO_SAFE_STATE: oResult = Process_GotoSafeState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Version
                    case COMMON_GET_VERSION: oResult = Process_GetVersion(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case COMMON_GET_DEVICE_TYPE: oResult = Process_GetDeviceType(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

					//Persistence
					case COMMON_STORE: oResult = Process_Store(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case COMMON_RESTORE: oResult = Process_Restore(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                }

                //Unlock CriticalSection
                Unlock();
            }
        }
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_MoveToPosition(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lTargetPosition;
    BOOL oAbsolute;
    BOOL oImmediately;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameterdata
        p_pCommand->GetParameterData(0, &lTargetPosition, sizeof(lTargetPosition));
        p_pCommand->GetParameterData(1, &oAbsolute, sizeof(oAbsolute));
        p_pCommand->GetParameterData(2, &oImmediately, sizeof(oImmediately));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            //Send Targetposition
            usIndex = INDEX_TARGET_POSITION;
            usSubIndex = SUBINDEX_TARGET_POSITION;
            oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, lTargetPosition, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            if(oAbsolute)
            {
                if(oImmediately)
                    //Send obsolute and immediately
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, IMMEDIATE_ABS_POSITION, &errorInfo);
                else
                    //Send absolute
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ABSOLUTE_POSITION, &errorInfo);
            }
            else
            {
                if(oImmediately)
                    //Send relative and immediately
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, IMMEDIATE_REL_POSITION, &errorInfo);
                else
                    //Send relative
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, RELATIVE_POSITION, &errorInfo);
            }
        }
        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetCurrentRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usP = 0;
    WORD usI = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Current Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            usSubIndex = SUBINDEX_CURRENT_P_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usP, &errorInfo);
        }

        //Read Current Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            usSubIndex = SUBINDEX_CURRENT_I_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usI, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usP, sizeof(usP));
        p_pCommand->SetReturnParameterData(1, &usI, sizeof(usI));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetEncoderParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usCounts = 0;
    WORD usPositionSensorType = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read encoder resolution
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_ENCODER_PULSE_NUMBER;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usCounts, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_TYPE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usPositionSensorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usCounts, sizeof(usCounts));
        p_pCommand->SetReturnParameterData(1, &usPositionSensorType, sizeof(usPositionSensorType));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetSensorType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    CErrorInfo errorInfo;
    BYTE ubNodeId(0);
    WORD usSensorType = 0;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read sensor type
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_TYPE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usSensorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usSensorType, sizeof(usSensorType));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetIncEncoderParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_INC_ENCODER_POLARITY = 0x0000;

    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BOOL oInvertedPolarity = FALSE;
    CErrorInfo errorInfo;
    DWORD ulEncoderResolution = 0;
    BYTE ubNodeId(0);
    WORD usSensorPolarity = 0;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read data rate
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_ENCODER_PULSE_NUMBER;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulEncoderResolution, &errorInfo);
        }

        //Read sensor polarity
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usSensorPolarity, &errorInfo);

            if(oResult)
            {
                if(BIT_MASK_INC_ENCODER_POLARITY & usSensorPolarity)
                {
                    oInvertedPolarity = TRUE;
                }
                else
                {
                    oInvertedPolarity = FALSE;
                }
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulEncoderResolution, sizeof(ulEncoderResolution));
        p_pCommand->SetReturnParameterData(1, &oInvertedPolarity, sizeof(oInvertedPolarity));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetHallSensorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_HALL_SENSOR_POLARITY = 0x0001;

    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BOOL oInvertedPolarity = FALSE;
    CErrorInfo errorInfo;
    BYTE ubNodeId(0);
    WORD usSensorPolarity = 0;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read sensor polarity
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usSensorPolarity, &errorInfo);

            if(oResult)
            {
                if(BIT_MASK_HALL_SENSOR_POLARITY & usSensorPolarity)
                {
                    oInvertedPolarity = TRUE;
                }
                else
                {
                    oInvertedPolarity = FALSE;
                }
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oInvertedPolarity, sizeof(oInvertedPolarity));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usMotorType = 0;
    WORD usContinuousCurrent = 0;
    WORD usPeakCurrent = 0;
    BYTE ubPolePair = 0;
    WORD usThermalTimeConstant = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Motortype
        if(oResult)
        {
            usIndex = INDEX_MOTOR_TYPE;
            usSubIndex = SUBINDEX_MOTOR_TYPE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usMotorType, &errorInfo);
        }

        //Read continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usContinuousCurrent, &errorInfo);
        }

        //Read Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usPeakCurrent, &errorInfo);
        }

        //Read pole paar
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_POLE_PAIR_NUMBER;
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ubPolePair, &errorInfo);
        }

        //Read Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usThermalTimeConstant, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usMotorType, sizeof(usMotorType));
        p_pCommand->SetReturnParameterData(1, &usContinuousCurrent, sizeof(usContinuousCurrent));
        p_pCommand->SetReturnParameterData(2, &usPeakCurrent, sizeof(usPeakCurrent));
        p_pCommand->SetReturnParameterData(3, &ubPolePair, sizeof(ubPolePair));
        p_pCommand->SetReturnParameterData(4, &usThermalTimeConstant, sizeof(usThermalTimeConstant));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetMotorType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usMotorType = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Motortype
        if(oResult)
        {
            usIndex = INDEX_MOTOR_TYPE;
            usSubIndex = SUBINDEX_MOTOR_TYPE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usMotorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usMotorType, sizeof(usMotorType));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetDcMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usContinuousCurrent = 0;
    WORD usPeakCurrent = 0;
    WORD usThermalTimeConstant = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usContinuousCurrent, &errorInfo);
        }

        //Read Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usPeakCurrent, &errorInfo);
        }

        //Read Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usThermalTimeConstant, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
        p_pCommand->SetReturnParameterData(1, &usPeakCurrent, sizeof(usPeakCurrent));
        p_pCommand->SetReturnParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetEcMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usContinuousCurrent = 0;
    WORD usPeakCurrent = 0;
    BYTE ubPolePair = 0;
    WORD usThermalTimeConstant = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usContinuousCurrent, &errorInfo);
        }

        //Read Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usPeakCurrent, &errorInfo);
        }

        //Read Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usThermalTimeConstant, &errorInfo);
        }

        //Read pole pairs
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_POLE_PAIR_NUMBER;
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ubPolePair, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
        p_pCommand->SetReturnParameterData(1, &usPeakCurrent, sizeof(usPeakCurrent));
        p_pCommand->SetReturnParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
        p_pCommand->SetReturnParameterData(3, &ubPolePair, sizeof(ubPolePair));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetPositionRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usP = 0;
    WORD usI = 0;
    WORD usD = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Position Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_P_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usP, &errorInfo);
        }

        //Read Position Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_I_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usI, &errorInfo);
        }

        //Read Position Regulator D-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_D_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usD, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usP, sizeof(usP));
        p_pCommand->SetReturnParameterData(1, &usI, sizeof(usI));
        p_pCommand->SetReturnParameterData(2, &usD, sizeof(usD));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetPositionRegulatorFeedForward(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    CErrorInfo errorInfo;
    BYTE ubNodeId(0);
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read velocity feed forward
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_VELOCITY_FEED_FORWARD;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usVelocityFeedForward, &errorInfo);
        }

        //Read acceleration feed forward
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_ACCELERATION_FEED_FORWARD;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usAccelerationFeedForward, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
        p_pCommand->SetReturnParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetVelocityRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usP = 0;
    WORD usI = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Velocity Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            usSubIndex = SUBINDEX_VELOCITY_P_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usP, &errorInfo);
        }

        //Read Velocity Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            usSubIndex = SUBINDEX_VELOCITY_I_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usI, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usP, sizeof(usP));
        p_pCommand->SetReturnParameterData(1, &usI, sizeof(usI));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetCurrentRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usP;
    WORD usI;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usP, sizeof(usP));
        p_pCommand->GetParameterData(1, &usI, sizeof(usI));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write Current Regualator P-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            usSubIndex = SUBINDEX_CURRENT_P_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usP, &errorInfo);
        }

        //Write Current Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            usSubIndex = SUBINDEX_CURRENT_I_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usI, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetEncoderParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usCounts;
    WORD usPositionSensorType;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usCounts, sizeof(usCounts));
        p_pCommand->GetParameterData(1, &usPositionSensorType, sizeof(usPositionSensorType));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write encoder resolution
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_ENCODER_PULSE_NUMBER;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usCounts, &errorInfo);
        }

        //Write sensor type
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_TYPE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPositionSensorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetSensorType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    CErrorInfo errorInfo;
    BYTE ubNodeId(0);
    WORD usSensorType(0);

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usSensorType, sizeof(usSensorType));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write sensor type
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_TYPE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usSensorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetIncEncoderParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_INC_ENCODER_POLARITY_NORMAL = 0xFFFE;
    const WORD BIT_MASK_INC_ENCODER_POLARITY_INVERTED = 0x0001;

    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BOOL oInvertedPolarity = FALSE;
    CErrorInfo errorInfo;
    DWORD ulEncoderResolution(0);
    BYTE ubNodeId(0);
    WORD usSensorPolarity(0);

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ulEncoderResolution, sizeof(ulEncoderResolution));
        p_pCommand->GetParameterData(1, &oInvertedPolarity, sizeof(oInvertedPolarity));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read data rate
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_ENCODER_PULSE_NUMBER;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulEncoderResolution, &errorInfo);
        }

        //Write sensor polarity
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usSensorPolarity, &errorInfo);

            if(oResult)
            {
                if(oInvertedPolarity)
                {
                    usSensorPolarity |= BIT_MASK_INC_ENCODER_POLARITY_INVERTED;
                }
                else
                {
                    usSensorPolarity &= BIT_MASK_INC_ENCODER_POLARITY_NORMAL;
                }

                usIndex = INDEX_SENSOR_CONFIGURATION;
                usSubIndex = SUBINDEX_POSITION_SENSOR_POLARITY;
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usSensorPolarity, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetHallSensorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_HALL_SENSOR_POLARITY_NORMAL = 0xFFFD;
    const WORD BIT_MASK_HALL_SENSOR_POLARITY_INVERTED = 0x0002;

    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BOOL oInvertedPolarity = FALSE;
    CErrorInfo errorInfo;
    BYTE ubNodeId(0);
    WORD usSensorPolarity(0);

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &oInvertedPolarity, sizeof(oInvertedPolarity));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write sensor polarity
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            usSubIndex = SUBINDEX_POSITION_SENSOR_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usSensorPolarity, &errorInfo);

            if(oResult)
            {
                if(oInvertedPolarity)
                {
                    usSensorPolarity |= BIT_MASK_HALL_SENSOR_POLARITY_INVERTED;
                }
                else
                {
                    usSensorPolarity &= BIT_MASK_HALL_SENSOR_POLARITY_NORMAL;
                }

                usIndex = INDEX_SENSOR_CONFIGURATION;
                usSubIndex = SUBINDEX_POSITION_SENSOR_POLARITY;
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usSensorPolarity, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usMotorType;
    WORD usContinuousCurrent;
    WORD usPeakCurrent;
    BYTE ubPolePair;
    WORD usThermalTimeConstant;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usMotorType, sizeof(usMotorType));
        p_pCommand->GetParameterData(1, &usContinuousCurrent, sizeof(usContinuousCurrent));
        p_pCommand->GetParameterData(2, &usPeakCurrent, sizeof(usPeakCurrent));
        p_pCommand->GetParameterData(3, &ubPolePair, sizeof(ubPolePair));
        p_pCommand->GetParameterData(4, &usThermalTimeConstant, sizeof(usThermalTimeConstant));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write motor type
        if(oResult)
        {
            usIndex = INDEX_MOTOR_TYPE;
            usSubIndex = SUBINDEX_MOTOR_TYPE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usMotorType, &errorInfo);
        }

        //Write continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usContinuousCurrent, &errorInfo);
        }

        //Write Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPeakCurrent, &errorInfo);
        }

        //Write pole paar
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_POLE_PAIR_NUMBER;
            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ubPolePair, &errorInfo);
        }

        //Read Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usThermalTimeConstant, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetMotorType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usMotorType;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usMotorType, sizeof(usMotorType));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write motor type
        if(oResult)
        {
            usIndex = INDEX_MOTOR_TYPE;
            usSubIndex = SUBINDEX_MOTOR_TYPE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usMotorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetDcMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usContinuousCurrent;
    WORD usPeakCurrent;
    WORD usThermalTimeConstant;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
        p_pCommand->GetParameterData(1, &usPeakCurrent, sizeof(usPeakCurrent));
        p_pCommand->GetParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usContinuousCurrent, &errorInfo);
        }

        //Write Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPeakCurrent, &errorInfo);
        }

        //Write Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usThermalTimeConstant, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetEcMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usContinuousCurrent;
    WORD usPeakCurrent;
    BYTE ubPolePair;
    WORD usThermalTimeConstant;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usContinuousCurrent, sizeof(usContinuousCurrent));
        p_pCommand->GetParameterData(1, &usPeakCurrent, sizeof(usPeakCurrent));
        p_pCommand->GetParameterData(2, &usThermalTimeConstant, sizeof(usThermalTimeConstant));
        p_pCommand->GetParameterData(3, &ubPolePair, sizeof(ubPolePair));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usContinuousCurrent, &errorInfo);
        }

        //Write Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPeakCurrent, &errorInfo);
        }

        //Write Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usThermalTimeConstant, &errorInfo);
        }

        //Write pole paar
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            usSubIndex = SUBINDEX_POLE_PAIR_NUMBER;
            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ubPolePair, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetPositionRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usP;
    WORD usI;
    WORD usD;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usP, sizeof(usP));
        p_pCommand->GetParameterData(1, &usI, sizeof(usI));
        p_pCommand->GetParameterData(2, &usD, sizeof(usD));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Position Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_P_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usP, &errorInfo);
        }

        //Read Position Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_I_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usI, &errorInfo);
        }

        //Write Position Regulator D-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_D_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usD, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetPositionRegulatorFeedForward(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    CErrorInfo errorInfo;
    BYTE ubNodeId(0);
    WORD usAccelerationFeedForward = 0;
    WORD usVelocityFeedForward = 0;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->SetParameterData(0, &usVelocityFeedForward, sizeof(usVelocityFeedForward));
        p_pCommand->SetParameterData(1, &usAccelerationFeedForward, sizeof(usAccelerationFeedForward));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read velocity feed forward
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_VELOCITY_FEED_FORWARD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usVelocityFeedForward, &errorInfo);
        }

        //Read acceleration feed forward
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            usSubIndex = SUBINDEX_POSITION_ACCELERATION_FEED_FORWARD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usAccelerationFeedForward, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetVelocityRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usP;
    WORD usI;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usP, sizeof(usP));
        p_pCommand->GetParameterData(1, &usI, sizeof(usI));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Velocity Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            usSubIndex = SUBINDEX_VELOCITY_P_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usP, &errorInfo);
        }

        //Read Velocity Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            usSubIndex = SUBINDEX_VELOCITY_I_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usI, &errorInfo);
        }

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetCurrentMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    short iCurrentMust = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Current parameter must
        usIndex = INDEX_CURRENT_SETTING_VALUE;
        usSubIndex = SUBINDEX_CURRENT_SETTING_VALUE;
        if(oResult) oResult = ReadInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &iCurrentMust, &errorInfo);

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &iCurrentMust, sizeof(iCurrentMust));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_SetCurrentMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    short iCurrentMust;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &iCurrentMust, sizeof(iCurrentMust));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Current parameter must
        usIndex = INDEX_CURRENT_SETTING_VALUE;
        usSubIndex = SUBINDEX_CURRENT_SETTING_VALUE;
        if(oResult) oResult = WriteInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, iCurrentMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_RequestCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    WORD usCobID;
    WORD usLength;
    void* pData = NULL;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        pData = malloc(usLength);

        //RequestCANFrame
        if(oResult) oResult = DCS_RequestCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_SendCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    WORD usCobID;
    WORD usLength;
    void* pData = NULL;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        pData = malloc(usLength);
        memset(pData, 0, usLength);
        p_pCommand->GetParameterData(2, pData, usLength);

        //SendCANFrame
        if(oResult) oResult = DCS_SendCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ReadCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    WORD usCobID;
    WORD usLength;
    DWORD ulTimeout;
    void* pData = NULL;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        p_pCommand->GetParameterData(2, &ulTimeout, sizeof(ulTimeout));

        //ReadCANFrame
        pData = malloc(usLength);
        if(oResult) oResult = DCS_ReadCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usCobID, usLength, pData, ulTimeout, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_FindHome(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    WORD usStartHoming;
    BOOL oResult(FALSE);
	DWORD ulTimeout;

    BYTE ubNodeId(0);
    __int8 bHomingMethod;
    CErrorInfo errorInfo;

	p_pManager->DCS_GetProtocolStackSettings(p_hDCS_Handle, NULL, &ulTimeout);

   if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &bHomingMethod, sizeof(bHomingMethod));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Homing Method
        if(oResult)
        {
            usIndex = INDEX_HOMING_METHOD;
            usSubIndex = SUBINDEX_HOMING_METHOD;
            oResult = WriteInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, bHomingMethod, &errorInfo);
        }

        //Send 0->1 Bit Change
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            usStartHoming = 0x000F;//000F => Homing operation inturrept start = 0 / Enable operation = F
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usStartHoming, &errorInfo);
        }

		BOOL oHomingAttainedBitResetted(FALSE);
		WORD usHomingState(0);
		DWORD ulTimeStart;
		DWORD ulTimeBitTestRunning;

		// Start Time
		ulTimeStart = MmcGetTickCount();
		ulTimeBitTestRunning = 0;

		while(oResult && !oHomingAttainedBitResetted && (ulTimeBitTestRunning < ulTimeout))
		{
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usHomingState, &errorInfo);
            // Check homing attained (Bit 12)
            usHomingState &= HS_HOMING_ATTAINED;
            if(usHomingState == HS_HOMING_ATTAINED)
			{
                oHomingAttainedBitResetted = FALSE;
			}
            else
			{
                oHomingAttainedBitResetted = TRUE;
			}
			ulTimeBitTestRunning = MmcGetTickCount() - ulTimeStart;
		}

        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            usStartHoming = 0x001F;//001F => Homing operation start = 1 / Enable operation = F
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usStartHoming, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetHomingParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulHomingAcceleration = 0;
    DWORD ulSpeedSwitch = 0;
    DWORD ulSpeedIndex = 0;
    int lHomeOffset = 0;
    WORD usCurrentThreshold = 0;
    int lHomePosition = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //HomingAcceleration
        if(oResult)
        {
            usIndex = INDEX_HOMING_ACCELERATION;
            usSubIndex = SUBINDEX_HOMING_ACCELERATION;
            oResult= ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulHomingAcceleration, &errorInfo);
        }

        //SpeedSwitch
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            usSubIndex = SUBINDEX_SPEED_SEARCH_SWITCH;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulSpeedSwitch, &errorInfo);
        }

        //SpeedIndex
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            usSubIndex = SUBINDEX_SPEED_SEARCH_ZERO;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulSpeedIndex, &errorInfo);
        }

        //HomeOffset
        if(oResult)
        {
            usIndex = INDEX_HOME_OFFSET;
            usSubIndex = SUBINDEX_HOME_OFFSET;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, (DWORD*) &lHomeOffset, &errorInfo);
        }

        //CurrentThreshold
        if(oResult)
        {
            usIndex = INDEX_CURRENT_THRESHOLD;
            usSubIndex = SUBINDEX_CURRENT_THRESHOLD;
            ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usCurrentThreshold, &errorInfo);
        }

        //HomePosition
        if(oResult)
        {
            usIndex = INDEX_HOME_POSITION;
            usSubIndex = SUBINDEX_HOME_POSITION;
            if(!ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, (DWORD*) &lHomePosition, &errorInfo))
            {
                if(errorInfo.GetErrorCode() == k_Error_OldDeviceEpos_ObjNotExist)
                {
                    errorInfo.Reset();
                    lHomePosition = 0;
                    oResult = TRUE;
                }
                else
                {
                    oResult = FALSE;
                }
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulHomingAcceleration, sizeof(ulHomingAcceleration));
        p_pCommand->SetReturnParameterData(1, &ulSpeedSwitch, sizeof(ulSpeedSwitch));
        p_pCommand->SetReturnParameterData(2, &ulSpeedIndex, sizeof(ulSpeedIndex));
        p_pCommand->SetReturnParameterData(3, &lHomeOffset, sizeof(lHomeOffset));
        p_pCommand->SetReturnParameterData(4, &usCurrentThreshold, sizeof(usCurrentThreshold));
        p_pCommand->SetReturnParameterData(5, &lHomePosition, sizeof(lHomePosition));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetHomingParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulHomingAcceleration = 0;
    DWORD ulSpeedSwitch = 0;
    DWORD ulSpeedIndex = 0;
    int lHomeOffset = 0;
    WORD usCurrentThreshold = 0;
    int lHomePosition = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->ResetStatus();
        p_pCommand->GetParameterData(0, &ulHomingAcceleration, sizeof(ulHomingAcceleration));
        p_pCommand->GetParameterData(1, &ulSpeedSwitch, sizeof(ulSpeedSwitch));
        p_pCommand->GetParameterData(2, &ulSpeedIndex, sizeof(ulSpeedIndex));
        p_pCommand->GetParameterData(3, &lHomeOffset, sizeof(lHomeOffset));
        p_pCommand->GetParameterData(4, &usCurrentThreshold, sizeof(usCurrentThreshold));
        p_pCommand->GetParameterData(5, &lHomePosition, sizeof(lHomePosition));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //HomingAcceleration
        if(oResult)
        {
            usIndex = INDEX_HOMING_ACCELERATION;
            usSubIndex = SUBINDEX_HOMING_ACCELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulHomingAcceleration, &errorInfo);
        }

        //SpeedSwitch
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            usSubIndex = SUBINDEX_SPEED_SEARCH_SWITCH;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulSpeedSwitch, &errorInfo);
        }

        //SpeedIndex
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            usSubIndex = SUBINDEX_SPEED_SEARCH_ZERO;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulSpeedIndex, &errorInfo);
        }

        //HomeOffset
        if(oResult)
        {
            usIndex = INDEX_HOME_OFFSET;
            usSubIndex = SUBINDEX_HOME_OFFSET;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, (DWORD)lHomeOffset, &errorInfo);
        }

        //CurrentThreshold
        if(oResult)
        {
            usIndex = INDEX_CURRENT_THRESHOLD;
            usSubIndex = SUBINDEX_CURRENT_THRESHOLD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usCurrentThreshold, &errorInfo);
        }

        //HomePosition
        if(oResult)
        {
            usIndex = INDEX_HOME_POSITION;
            usSubIndex = SUBINDEX_HOME_POSITION;
            if(!WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, (DWORD)lHomePosition, &errorInfo))
            {
                if(errorInfo.GetErrorCode() == k_Error_OldDeviceEpos_ObjNotExist)
                {
                    errorInfo.Reset();
                    oResult = TRUE;
                }
                else
                {
                    oResult = FALSE;
                }
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_DefinePosition(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    CErrorInfo errorInfo;
    int lHomePosition = 0;
    BYTE ubNodeId(0);

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->ResetStatus();
        p_pCommand->GetParameterData(0, &lHomePosition, sizeof(lHomePosition));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //HomePosition
        if(oResult)
        {
            usIndex = INDEX_HOME_POSITION;
            usSubIndex = SUBINDEX_HOME_POSITION;
            if(!WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, (DWORD)lHomePosition, &errorInfo))
            {
                if(errorInfo.GetErrorCode() == k_Error_OldDeviceEpos_ObjNotExist)
                {
                    errorInfo.Reset();
                    oResult = TRUE;
                }
                else
                {
                    oResult = FALSE;
                }
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_StopHoming(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    WORD usStopHoming;
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Send 1->0 Bit Change
        usIndex = INDEX_CONTROL_WORD;
        usSubIndex = SUBINDEX_CONTROL_WORD;
        usStopHoming = 0x001F;//001F => Homing operation start = 1 / Enable operation = F
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usStopHoming, &errorInfo);

        if(oResult)
        {
            usStopHoming = 0x000F;//000F => Homing operation inturrept start = 0 / Enable operation = F
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usStopHoming, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetHomingState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    WORD usHomingState(0);
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    BOOL oHomingAttained;
    BOOL oHomingError;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        // Homing Attained
        if(oResult)
        {
            //Read Homing State
            usIndex = INDEX_STATUS_WORD;
            usSubIndex = SUBINDEX_STATUS_WORD;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usHomingState, &errorInfo);
            if(oResult)
            {
                // usDeviceState = usDeviceState & 0x7FF;

                // Check homing attained
                usHomingState &= HS_HOMING_ATTAINED;
                if(usHomingState == HS_HOMING_ATTAINED)
				{
                    oHomingAttained = TRUE;
				}
                else
				{
                    oHomingAttained = FALSE;
				}

				// Check homing error
                usHomingState &= HS_HOMING_ERROR;
                if(usHomingState == HS_HOMING_ERROR)
				{
                    oHomingError = TRUE;
				}
                else
				{
                    oHomingError = FALSE;
				}

            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oHomingAttained, sizeof(oHomingAttained));
        p_pCommand->SetReturnParameterData(1, &oHomingError, sizeof(oHomingError));

    }
	else
	{
		oResult = FALSE;
	}

    return oResult;

} // CGatewayDrive2ToEpos::Process_GetHomingState

BOOL CGatewayDrive2ToEpos::Process_DigitalInputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex, usSubIndex, usValue;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usDigitalInputNb;
    WORD usConfiguration;
    BOOL oMask;
    BOOL oPolarity;
    BOOL oExecutionMask;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usDigitalInputNb, sizeof(usDigitalInputNb));
        p_pCommand->GetParameterData(1, &usConfiguration, sizeof(usConfiguration));
        p_pCommand->GetParameterData(2, &oMask, sizeof(oMask));
        p_pCommand->GetParameterData(3, &oPolarity, sizeof(oPolarity));
        p_pCommand->GetParameterData(4, &oExecutionMask, sizeof(oExecutionMask));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write Digital Input Configuration
        usIndex = INDEX_DIGITAL_INPUT_CONFIGURATION;
        usSubIndex = usDigitalInputNb;
        usValue = usConfiguration;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oPolarity);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oExecutionMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_DigitalOutputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex, usSubIndex, usValue;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usDigitalOutputNb;
    WORD usConfiguration;
    BOOL oState;
    BOOL oMask;
    BOOL oPolarity;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usDigitalOutputNb, sizeof(usDigitalOutputNb));
        p_pCommand->GetParameterData(1, &usConfiguration, sizeof(usConfiguration));
        p_pCommand->GetParameterData(2, &oState, sizeof(oState));
        p_pCommand->GetParameterData(3, &oMask, sizeof(oMask));
        p_pCommand->GetParameterData(4, &oPolarity, sizeof(oPolarity));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write Digital Output Configuration
        usIndex = INDEX_DIGITAL_OUTPUT_CONFIGURATION;
        usSubIndex = usDigitalOutputNb;
        usValue = usConfiguration;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_OUTPUT_STATE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oState);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_OUTPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_OUTPUT_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oPolarity);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetAllDigitalInputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex, usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usInputs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read state of all digital inputs
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_STATE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usInputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usInputs, sizeof(usInputs));

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex, usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usOutputs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read state of all digital outputs
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_OUTPUT_STATE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usOutputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usOutputs, sizeof(usOutputs));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetAnalogInput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex, usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usNumber;
    WORD usAnalog = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usNumber, sizeof(usNumber));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read value of one analog input
        if(oResult)
        {
            usIndex = INDEX_ANALOG_INPUTS;
            usSubIndex = usNumber;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usAnalog, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usAnalog, sizeof(usAnalog));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex, usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usOutputs;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usOutputs, sizeof(usOutputs));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write state of all digital outputs
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_OUTPUT_STATE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usOutputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetPositionMarkerParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    BYTE ubPositionMarkerEdgeType = 0;
    BYTE ubPositionMarkerMode = 0;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ubPositionMarkerEdgeType, sizeof(ubPositionMarkerEdgeType));
        p_pCommand->GetParameterData(1, &ubPositionMarkerMode, sizeof(ubPositionMarkerMode));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Postition marker edge type
        if(oResult)
        {
            usIndex = INDEX_POSITION_MARKER;
            usSubIndex = SUBINDEX_POSITION_MARKER_EDGE_TYPE;
            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ubPositionMarkerEdgeType, &errorInfo);
        }

        //Postition marker mode
        if(oResult)
        {
            usIndex = INDEX_POSITION_MARKER;
            usSubIndex = SUBINDEX_POSITION_MARKER_MODE;
            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ubPositionMarkerMode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetPositionMarkerParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    BYTE ubPositionMarkerEdgeType = 0;
    BYTE ubPositionMarkerMode = 0;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Postition marker edge type
        if(oResult)
        {
            usIndex = INDEX_POSITION_MARKER;
            usSubIndex = SUBINDEX_POSITION_MARKER_EDGE_TYPE;
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ubPositionMarkerEdgeType, &errorInfo);
        }

        //Postition marker mode
        if(oResult)
        {
            usIndex = INDEX_POSITION_MARKER;
            usSubIndex = SUBINDEX_POSITION_MARKER_MODE;
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ubPositionMarkerMode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ubPositionMarkerEdgeType, sizeof(ubPositionMarkerEdgeType));
        p_pCommand->SetReturnParameterData(1, &ubPositionMarkerMode, sizeof(ubPositionMarkerMode));

    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ActivatePositionMarker(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD POSITION_MARKER_CONFIGURATION(0x0003);
    const WORD POSITION_MARKER_MASK(0x0008);

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usDigitalInputNumber;
    BOOL oPolarity;
    WORD usPolarity;
    WORD usExecutionMask;
    WORD usMask;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usDigitalInputNumber, sizeof(usDigitalInputNumber));
        p_pCommand->GetParameterData(1, &oPolarity, sizeof(oPolarity));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        // Digital input functionalities
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_CONFIGURATION;
            usSubIndex = usDigitalInputNumber;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, POSITION_MARKER_CONFIGURATION, &errorInfo);
        }

        //Set digital input mask
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usMask, &errorInfo);

            if(oResult)
            {
                usMask |= POSITION_MARKER_MASK;

                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usMask, &errorInfo);
            }
        }

        //Set digital input polarity
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usPolarity, &errorInfo);

            if(oResult)
            {
                if(oPolarity)
                {
                    usPolarity |= POSITION_MARKER_MASK;
                }
                else
                {
                    usPolarity &= 0xFFF7;
                }

                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPolarity, &errorInfo);
            }
        }

        //Set digital input execution mask
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usExecutionMask, &errorInfo);

            if(oResult)
            {
                usExecutionMask |= POSITION_MARKER_MASK;

                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usExecutionMask, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_DeactivatePositionMarker(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD POSITION_MARKER_MASK(0xFFF7);

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usDigitalInputNumber(0);
    WORD usConfiguration(15);
    WORD usMask(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usDigitalInputNumber, sizeof(usDigitalInputNumber));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set digital output configuration
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_CONFIGURATION;
            usSubIndex = usDigitalInputNumber;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usConfiguration, &errorInfo);

            while(!oResult && (usConfiguration != 0))
            {
                usConfiguration--;
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usConfiguration, &errorInfo);
            }
        }

        //Set digital input mask
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            usSubIndex = SUBINDEX_DIGITAL_INPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usMask, &errorInfo);

            if(oResult)
            {
                usMask |= POSITION_MARKER_MASK;

                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usMask, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ReadPositionMarkerCounter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usPositionMarkerCounter;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Position marker counter
        if(oResult)
        {
            usIndex = INDEX_POSITION_MARKER;
            usSubIndex = SUBINDEX_POSITION_MARKER_COUNTER;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usPositionMarkerCounter, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usPositionMarkerCounter, sizeof(usPositionMarkerCounter));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_ReadPositionMarkerCapturedPosition(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD POSITION_MARKER_OFFSET = 0x0004;

    BOOL oResult(TRUE);
    WORD usCounterIndex;
    WORD usIndex;
    WORD usSubIndex;

    BYTE ubNodeId(0);
    int lPositionMarkerCapturedPosition;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usCounterIndex, sizeof(usCounterIndex));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Position marker captured position
        if(oResult)
        {
            if(usCounterIndex > 0)
            {
                usIndex = INDEX_POSITION_MARKER;
                usSubIndex = POSITION_MARKER_OFFSET + usCounterIndex;
                oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lPositionMarkerCapturedPosition, &errorInfo);
            }
            else
            {
                usIndex = INDEX_POSITION_MARKER;
                usSubIndex = SUBINDEX_POSITION_MARKER_CAPTURED_POSITION;
                oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lPositionMarkerCapturedPosition, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lPositionMarkerCapturedPosition, sizeof(lPositionMarkerCapturedPosition));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_ResetPositionMarkerCounter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usPositionMarkerCounter = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Position marker counter
        if(oResult)
        {
            usIndex = INDEX_POSITION_MARKER;
            usSubIndex = SUBINDEX_POSITION_MARKER_COUNTER;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPositionMarkerCounter, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetCurrentIs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    short iCurrentIs;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //CurrentIs
        if(oResult)
        {
            usIndex = INDEX_CURRENT_ACTUAL_VALUE;
            usSubIndex = SUBINDEX_CURRENT_ACTUAL_VALUE;
            oResult = ReadInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &iCurrentIs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &iCurrentIs, sizeof(iCurrentIs));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetCurrentIsAveraged(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    short sCurrentIsAveraged;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        // Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        // CurrentIsAveraged
        if(oResult)
        {
            usIndex = INDEX_CURRENT_ACTUAL_VALUE_AVERAGED;
            usSubIndex = SUBINDEX_CURRENT_ACTUAL_VALUE_AVERAGED;
            oResult = ReadInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &sCurrentIsAveraged, &errorInfo);
        }

        // Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &sCurrentIsAveraged, sizeof(sCurrentIsAveraged));

        return oResult;
    }

    return FALSE;

} // CGatewayDrive2ToEpos::Process_GetCurrentIsAveraged

BOOL CGatewayDrive2ToEpos::Process_GetMovementState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    WORD usDeviceState(0);
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    BOOL oTargetReached;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //TargetReached
        if(oResult)
        {
            //Read Device State
            usIndex = INDEX_STATUS_WORD;
            usSubIndex = SUBINDEX_STATUS_WORD;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usDeviceState, &errorInfo);
            if(oResult)
            {
                usDeviceState = usDeviceState & 0x7FF;

                //Check target reached
                usDeviceState &= DS_TARGET_REACHED;
                if(usDeviceState == DS_TARGET_REACHED)
                    oTargetReached = TRUE;
                else
                    oTargetReached = FALSE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oTargetReached, sizeof(oTargetReached));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetPositionIs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lPositionIs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //PositionIs
        if(oResult)
        {
            usIndex = INDEX_POSITION_ACTUAL_VALUE;
            usSubIndex = SUBINDEX_POSITION_ACTUAL_VALUE;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lPositionIs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lPositionIs, sizeof(lPositionIs));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetVelocityIs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lVelocityIs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //VeloctiyIs
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_ACTUAL_VALUE;
            usSubIndex = SUBINDEX_VELOCITY_ACTUAL_VALUE;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lVelocityIs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lVelocityIs, sizeof(lVelocityIs));

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetVelocityIsAveraged(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lVelocityIsAveraged = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        // Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        // VelocityIsAveraged
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_ACTUAL_VALUE_AVERAGED;
            usSubIndex = SUBINDEX_VELOCITY_ACTUAL_VALUE_AVERAGED;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lVelocityIsAveraged, &errorInfo);
        }

        // Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lVelocityIsAveraged, sizeof(lVelocityIsAveraged));

        return oResult;
    }

    return FALSE;

} // CGatewayDrive2ToEpos::Process_GetVelocityIsAveraged

BOOL CGatewayDrive2ToEpos::Process_GetPositionMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lPositionMust = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //PositionMust
        if(oResult)
        {
            usIndex = INDEX_POSITION_SETTING_VALUE;
            usSubIndex = SUBINDEX_POSITION_SETTING_VALUE;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lPositionMust, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lPositionMust, sizeof(lPositionMust));

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetPositionMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    int lPositionMust;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &lPositionMust, sizeof(lPositionMust));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //PositionMust
        usIndex = INDEX_POSITION_SETTING_VALUE;
        usSubIndex = SUBINDEX_POSITION_SETTING_VALUE;
        if(oResult) oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, lPositionMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetPositionProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulProfileVelocity;
    DWORD ulProfileAcceleration;
    DWORD ulProfileDeceleration;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->ResetStatus();

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_PROFILE_VELOCITY;
            usSubIndex = SUBINDEX_PROFILE_VELOCITY;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulProfileVelocity, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_PROFILE_ACCELERATION;
            usSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulProfileAcceleration, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_PROFILE_DECELERATION;
            usSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulProfileDeceleration, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulProfileVelocity, sizeof(ulProfileVelocity));
        p_pCommand->SetReturnParameterData(1, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
        p_pCommand->SetReturnParameterData(2, &ulProfileDeceleration, sizeof(ulProfileDeceleration));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetTargetPosition(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lTargetPosition;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_TARGET_POSITION;
            usSubIndex = SUBINDEX_TARGET_POSITION;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lTargetPosition, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lTargetPosition, sizeof(lTargetPosition));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_HaltPositionMovement(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        usIndex = INDEX_CONTROL_WORD;
        usSubIndex = SUBINDEX_CONTROL_WORD;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, STOP_PROFILE, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetPositionProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulProfileVelocity;
    DWORD ulProfileAcceleration;
    DWORD ulProfileDeceleration;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->ResetStatus();
        p_pCommand->GetParameterData(0, &ulProfileVelocity, sizeof(ulProfileVelocity));
        p_pCommand->GetParameterData(1, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
        p_pCommand->GetParameterData(2, &ulProfileDeceleration, sizeof(ulProfileDeceleration));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            //Send velocity
            usIndex = INDEX_PROFILE_VELOCITY;
            usSubIndex = SUBINDEX_PROFILE_VELOCITY;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulProfileVelocity, &errorInfo);
        }

        if(oResult)
        {
            //Send acceleration
            usIndex = INDEX_PROFILE_ACCELERATION;
            usSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulProfileAcceleration, &errorInfo);
        }

        if(oResult)
        {
            //Send deceleration
            usIndex = INDEX_PROFILE_DECELERATION;
            usSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulProfileDeceleration, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_EnablePositionWindow(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulPositionWindow;
    WORD usPositionWindowTime;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->ResetStatus();
        p_pCommand->GetParameterData(0, &ulPositionWindow, sizeof(ulPositionWindow));
        p_pCommand->GetParameterData(1, &usPositionWindowTime, sizeof(usPositionWindowTime));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            //Send position window
            usIndex = INDEX_POSITION_WINDOW;
            usSubIndex = SUBINDEX_POSITION_WINDOW;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulPositionWindow, &errorInfo);
        }

        if(oResult)
        {
            //Send position window time
            usIndex = INDEX_POSITION_WINDOW_TIME;
            usSubIndex = SUBINDEX_POSITION_WINDOW_TIME;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPositionWindowTime, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_DisablePositionWindow(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulPositionWindow = ULONG_MAX;
    WORD usPositionWindowTime = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->ResetStatus();

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            //Send position window
            usIndex = INDEX_POSITION_WINDOW;
            usSubIndex = SUBINDEX_POSITION_WINDOW;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulPositionWindow, &errorInfo);
        }

        if(oResult)
        {
            //Send position window time
            usIndex = INDEX_POSITION_WINDOW_TIME;
            usSubIndex = SUBINDEX_POSITION_WINDOW_TIME;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usPositionWindowTime, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetTargetVelocity(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lTargetVelocity = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read target velocity
        if(oResult)
        {
            usIndex = INDEX_TARGET_VELOCITY;
            usSubIndex = SUBINDEX_TARGET_VELOCITY;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lTargetVelocity, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetVelocityProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulProfileAcceleration = 0;
    DWORD ulProfileDeceleration = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read ProfileAcceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_ACCELERATION;
            usSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulProfileAcceleration, &errorInfo);
        }

        //Read ProfileDeceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_DECELERATION;
            usSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulProfileDeceleration, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
        p_pCommand->SetReturnParameterData(1, &ulProfileDeceleration, sizeof(ulProfileDeceleration));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_HaltVelocityMovement(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Send Stop
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, STOP_PROFILE, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_MoveWithVelocity(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    int lTargetVelocity;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Set Parameter data
        p_pCommand->GetParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Send target velocity
        if(oResult)
        {
            usIndex = INDEX_TARGET_VELOCITY;
            usSubIndex = SUBINDEX_TARGET_VELOCITY;
            oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, lTargetVelocity, &errorInfo);
        }

        //Move
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, MOVE_WITH_VELOCITY, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetVelocityProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulProfileAcceleration;
    DWORD ulProfileDeceleration;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
        p_pCommand->GetParameterData(1, &ulProfileDeceleration, sizeof(ulProfileDeceleration));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Acceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_ACCELERATION;
            usSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulProfileAcceleration, &errorInfo);
        }

        //Set Decceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_DECELERATION;
            usSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulProfileDeceleration, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_ClearFault(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Clear Fault
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, FAULT_RESET, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetDisableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_DISABLE = 0x004F;
    WORD usDeviceState(0);
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    BOOL oIsDisabled;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Disable state
        if(oResult)
        {
            oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &usDeviceState, &errorInfo);

            if(oResult)
            {
                usDeviceState &= BIT_MASK_DISABLE;
                if(usDeviceState == DS_SWITCH_ON_DISABLED)
                    oIsDisabled = TRUE;
                else
                    oIsDisabled = FALSE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oIsDisabled, sizeof(oIsDisabled));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetEnableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_ENABLE = 0x006F;
    WORD usDeviceState(0);
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    BOOL oIsEnabled;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read enable state
        if(oResult)
        {
            oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &usDeviceState, &errorInfo);

            if(oResult)
            {
                usDeviceState &= BIT_MASK_ENABLE;
                if(usDeviceState == DS_OPERATION_ENABLED)
                    oIsEnabled = TRUE;
                else
                    oIsEnabled = FALSE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oIsEnabled, sizeof(oIsEnabled));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetFaultState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_FAULT = 0x004F;
    WORD usDeviceState(0);
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    BOOL oIsInFault;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Fault state
        if(oResult)
        {
            oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &usDeviceState, &errorInfo);

            if(oResult)
            {
                usDeviceState &= BIT_MASK_FAULT;
                if(usDeviceState == DS_FAULT)
                    oIsInFault = TRUE;
                else
                    oIsInFault = FALSE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oIsInFault, sizeof(oIsInFault));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetOperationMode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    __int8 bMode = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Operation Mode
        if(oResult)
        {
            usIndex = INDEX_OPERATION_MODE_DISPLAY;
            usSubIndex = SUBINDEX_OPERATION_MODE_DISPLAY;
            oResult = ReadInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &bMode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &bMode, sizeof(bMode));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetQuickStopState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_QUICK_STOP = 0x006F;
    WORD usDeviceState(0);
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    BOOL oIsQuickStoped = FALSE;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Quick state
        if(oResult) oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &usDeviceState, &errorInfo);

        if(oResult)
        {
            usDeviceState &= BIT_MASK_QUICK_STOP;
            if(usDeviceState == DS_QUICK_STOP_ACTIV)
                oIsQuickStoped = TRUE;
            else
                oIsQuickStoped = FALSE;
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oIsQuickStoped, sizeof(oIsQuickStoped));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetNbOfDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    BYTE ubNbOfDeviceError = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Number of Device Error
        if(oResult)
        {
            usIndex = INDEX_ERROR_HISTORY;
            usSubIndex = SUBINDEX_NB_OF_ERRORS;
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ubNbOfDeviceError, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ubNbOfDeviceError, sizeof(ubNbOfDeviceError));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetDeviceErrorCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    BYTE ubErrorNumber(0);
    DWORD ulDeviceErrorCode(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ubErrorNumber, sizeof(ubErrorNumber));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Code from Device Error
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, INDEX_ERROR_HISTORY, ubErrorNumber, &ulDeviceErrorCode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GotoSafeState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    //Address Parameter
    BYTE ubNodeId = 0;

    CErrorInfo errorInfo;
    BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Disable State
        if(oResult) oResult = SetDisableState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    DWORD ulDeviceType = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Device Type
        if(oResult)
        {
            usIndex = INDEX_DEVICE_TYPE;
            usSubIndex = SUBINDEX_DEVICE_TYPE;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulDeviceType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SendNMTService(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    WORD usNodeIdentifer;
    WORD usCommandSpecifier;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usNodeIdentifer, sizeof(usNodeIdentifer));
        p_pCommand->GetParameterData(1, &usCommandSpecifier, sizeof(usCommandSpecifier));

        //SendNMTService
        if(oResult) oResult = DCS_SendNMTService(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNodeIdentifer, usCommandSpecifier, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ReadLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE = 8;

    BOOL oResult = FALSE;

    WORD usTimeout;
    DWORD ulErrorCode = 0;
    void* pDataBuffer = NULL;
    DWORD ulDataBufferLength;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Prepare DataBuffer
        ulDataBufferLength = MAX_FRAME_SIZE;
        pDataBuffer = malloc(ulDataBufferLength);
        memset(pDataBuffer, 0, ulDataBufferLength);

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usTimeout, sizeof(usTimeout));

        //ReadLSSFrame
        if(oResult) oResult = DCS_ReadLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usTimeout, pDataBuffer, ulDataBufferLength, &errorInfo);
        if(!oResult) ulErrorCode = errorInfo.GetErrorCode();

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
        p_pCommand->SetReturnParameterData(1, pDataBuffer, ulDataBufferLength);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_SendLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE = 8;

    BOOL oResult = FALSE;

    void* pDataBuffer = NULL;
    DWORD ulDataBufferLength;
    DWORD ulErrorCode = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Prepare DataBuffer
        ulDataBufferLength = p_pCommand->GetParameterLength(1);
        if(ulDataBufferLength > MAX_FRAME_SIZE) ulDataBufferLength = MAX_FRAME_SIZE;
        pDataBuffer = malloc(ulDataBufferLength);
        memset(pDataBuffer, 0, ulDataBufferLength);

        //Get Parameter Data
        p_pCommand->GetParameterData(0, pDataBuffer, ulDataBufferLength);

        //SendLSSFrame
        if(oResult) oResult = DCS_SendLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, pDataBuffer, ulDataBufferLength, &errorInfo);
        if(!oResult) ulErrorCode = errorInfo.GetErrorCode();

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
        if(pDataBuffer) free(pDataBuffer);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetDisableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    BYTE ubNodeId = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Disable State
        if(oResult) oResult = SetDisableState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_SetEnableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    WORD usDeviceState(0);
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Device State
        if(oResult) oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &usDeviceState, &errorInfo);
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            switch(usDeviceState)
            {
                case DS_OPERATION_ENABLED:
                    return TRUE;

                case DS_SWITCH_ON_DISABLED:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, SHUTDOWN, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, DS_READY_TO_SWITCH_ON, &errorInfo);

                case DS_READY_TO_SWITCH_ON:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, SWITCH_ON, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, DS_OPERATION_ENABLED, &errorInfo);

                case DS_SWITCHED_ON:
                case DS_QUICK_STOP_ACTIV:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ENABLE_OPERATION, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, DS_OPERATION_ENABLED, &errorInfo);
                    break;
                default:
                    oResult = FALSE;

            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetOperationMode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    __int8 bMode;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &bMode, sizeof(bMode));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Operation Mode;
        if(oResult)
        {
            usIndex = INDEX_OPERATION_MODE;
            usSubIndex = SUBINDEX_OPERATION_MODE;
            oResult = WriteInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, bMode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetQuickStopState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    WORD usDeviceState(0);
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Device State
        if(oResult) oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, &usDeviceState, &errorInfo);
        if(oResult)
        {
            if(((usDeviceState & DS_QUICK_STOP_ACTIV) || (usDeviceState & DS_OPERATION_ENABLED))&&(oResult))
            {
                if((usDeviceState & DS_OPERATION_ENABLED) == DS_OPERATION_ENABLED)
                {
                    //Set Quick Stop
                    usIndex = INDEX_CONTROL_WORD;
                    usSubIndex = SUBINDEX_CONTROL_WORD;
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, QUICK_STOP, &errorInfo);
                }
                else
                    oResult = TRUE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_ResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;
	BOOL oWaitForReboot(FALSE);
	BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &oWaitForReboot, sizeof(oWaitForReboot));

        //SendNMTService
        if(oResult) oResult = ResetDevice(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubNodeId, oWaitForReboot, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_WaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //WaitForReboot
        if(oResult) oResult = WaitForReboot(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ClearDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Clear Fault
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            usSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, FAULT_RESET, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usObjectIndex;
    BYTE ubObjectSubIndex;
    DWORD ulNbOfBytesToRead;
    void* pData = NULL;
    DWORD ulNbOfBytesRead = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usObjectIndex, sizeof(usObjectIndex));
        p_pCommand->GetParameterData(1, &ubObjectSubIndex, sizeof(ubObjectSubIndex));
        p_pCommand->GetParameterData(2, &ulNbOfBytesToRead, sizeof(ulNbOfBytesToRead));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //GetObject
        if(oResult)
        {
            pData = malloc(ulNbOfBytesToRead);
			memset(pData, 0, ulNbOfBytesToRead);

            oResult = ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
        }

        //Set Returnparameter
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, ulNbOfBytesToRead);
        p_pCommand->SetReturnParameterData(1, &ulNbOfBytesRead, sizeof(ulNbOfBytesRead));
        if(pData) free(pData);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usSoftwareVersion = 0;
    WORD usHardwareVersion = 0;
    WORD usApplicationNumber = 0;
    WORD usApplicationVersion = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read HardwareVersion
        if(oResult)
        {
            usIndex = INDEX_VERSION;
            usSubIndex = SUBINDEX_HARDWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usHardwareVersion, &errorInfo);
        }

        //Read SoftwareVersion
        if(oResult)
        {
            usIndex = INDEX_VERSION;
            usSubIndex = SUBINDEX_SOFTWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usSoftwareVersion, &errorInfo);
        }

        //Read ApplicationNumber
        if(oResult)
        {
            usIndex = INDEX_VERSION;
            usSubIndex = SUBINDEX_APPLICATION_NUMBER;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usApplicationNumber, &errorInfo);
        }

        //Read ApplicationVersion
        if(oResult)
        {
            usIndex = INDEX_VERSION;
            usSubIndex = SUBINDEX_APPLICATION_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usApplicationVersion, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usHardwareVersion, sizeof(usHardwareVersion));
        p_pCommand->SetReturnParameterData(1, &usSoftwareVersion, sizeof(usSoftwareVersion));
        p_pCommand->SetReturnParameterData(2, &usApplicationNumber, sizeof(usApplicationNumber));
        p_pCommand->SetReturnParameterData(3, &usApplicationVersion, sizeof(usApplicationVersion));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_Restore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD TIMEOUT = 1000;

    WORD usIndex;
    WORD usSubIndex;
    DWORD ulPassword = 0;
    DWORD ulTransferRate = 0;
    DWORD ulBackupTimeout = 0;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand && p_pManager)
    {
        //Backup Timeout
        p_pManager->DCS_GetProtocolStackSettings(p_hDCS_Handle, &ulTransferRate, &ulBackupTimeout);

        //Increase Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hDCS_Handle, ulTransferRate, TIMEOUT, TRUE);

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write load all parameters
        ulPassword = 'd';
        ulPassword *= 256;
        ulPassword += 'a';
        ulPassword *= 256;
        ulPassword += 'o';
        ulPassword *= 256;
        ulPassword += 'l';

        usIndex = INDEX_RESTORE_DEFAULT_PARAMETERS;
        usSubIndex = SUBINDEX_RESTORE_ALL_DEFAULT_PARAMETERS;
        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulPassword, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hDCS_Handle, ulTransferRate, ulBackupTimeout, TRUE);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    WORD usObjectIndex;
    BYTE ubObjectSubIndex;
    void* pData = NULL;
    DWORD ulNbOfBytesToWrite;
    DWORD ulNbOfBytesWritten = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usObjectIndex, sizeof(usObjectIndex));
        p_pCommand->GetParameterData(1, &ubObjectSubIndex, sizeof(ubObjectSubIndex));
        p_pCommand->GetParameterData(3, &ulNbOfBytesToWrite, sizeof(ulNbOfBytesToWrite));
        pData = malloc(ulNbOfBytesToWrite);
        p_pCommand->GetParameterData(2, pData, ulNbOfBytesToWrite);

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //SetObject
        if(oResult) oResult = WriteDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNbOfBytesWritten, sizeof(ulNbOfBytesWritten));
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_Store(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand && p_pManager)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Store
        if(oResult) oResult = StoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

	}

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetVelocityMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    int lVelocityMust = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //VelocityMust
        usIndex = INDEX_VELOCITY_SETTING_VALUE;
        usSubIndex = SUBINDEX_VELOCITY_SETTING_VALUE;
        if(oResult) oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &lVelocityMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lVelocityMust, sizeof(lVelocityMust));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_SetVelocityMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult = FALSE;

    BYTE ubNodeId(0);
    int lVelocityMust;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &lVelocityMust, sizeof(lVelocityMust));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //VelocityMust
        usIndex = INDEX_VELOCITY_SETTING_VALUE;
        usSubIndex = SUBINDEX_VELOCITY_SETTING_VALUE;
        if(oResult) oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, lVelocityMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

WORD CGatewayDrive2ToEpos::GetNewBitMask(WORD p_usConfiguration, WORD p_usOldMask, BOOL p_oState)
{
    WORD usNewMask;

    if(p_oState)
    {//bit set to 1
        usNewMask = 1;

        for(int i = 0; i < p_usConfiguration; i++)
            usNewMask <<= 1;

        usNewMask |= p_usOldMask;
    }
    else
    {//bit set to 0
        usNewMask = 0xFFFF;

        for(int i = 0; i < p_usConfiguration; i++)
            usNewMask <<= 1;
        usNewMask--;

        usNewMask &= p_usOldMask;
    }

    return usNewMask;
}

BOOL CGatewayDrive2ToEpos::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayDrive2ToEpos";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetBaseSectorVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    WORD usBaseSectorVersion = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read target velocity
        if(oResult)
        {
            usIndex = INDEX_VERSION;
            usSubIndex = SUBINDEX_BASE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usBaseSectorVersion, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usBaseSectorVersion, sizeof(usBaseSectorVersion));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_GetSerialNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult(TRUE);

    BYTE ubNodeId(0);
    __int64 llSerialNumber = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read target velocity
        if(oResult)
        {
            usIndex = INDEX_SERIAL_NUMBER;
            usSubIndex = SUBINDEX_SERIAL_NUMBER;
            oResult = ReadUInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &llSerialNumber, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &llSerialNumber, sizeof(llSerialNumber));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDrive2ToEpos::Process_SetMaxFollowingError(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    DWORD ulMaxFollowingError = 0;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ulMaxFollowingError, sizeof(ulMaxFollowingError));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //MaxFollowingError
        if(oResult)
        {
            usIndex = INDEX_MAX_FOLLOWING_ERROR;
            usSubIndex = SUBINDEX_MAX_FOLLOWING_ERROR;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulMaxFollowingError, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetMaxFollowingError(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    DWORD ulMaxFollowingError = 0;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //MaxFollowingError
        if(oResult)
        {
            usIndex = INDEX_MAX_FOLLOWING_ERROR;
            usSubIndex = SUBINDEX_MAX_FOLLOWING_ERROR;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulMaxFollowingError, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulMaxFollowingError, sizeof(ulMaxFollowingError));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_SetMaxProfileVelocity(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    DWORD ulMaxProfileVelocity = 0;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ulMaxProfileVelocity, sizeof(ulMaxProfileVelocity));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //MaxFollowingError
        if(oResult)
        {
            usIndex = INDEX_MAX_PROFILE_VELOCITY;
            usSubIndex = SUBINDEX_MAX_PROFILE_VELOCITY;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ulMaxProfileVelocity, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetMaxProfileVelocity(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    DWORD ulMaxProfileVelocity = 0;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //MaxFollowingError
        if(oResult)
        {
            usIndex = INDEX_MAX_PROFILE_VELOCITY;
            usSubIndex = SUBINDEX_MAX_PROFILE_VELOCITY;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &ulMaxProfileVelocity, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulMaxProfileVelocity, sizeof(ulMaxProfileVelocity));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_SetDataRecordingParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usSamplingPeriod(0);
    WORD usNbOfPrecedingSamples(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usSamplingPeriod, sizeof(usSamplingPeriod));
        p_pCommand->GetParameterData(1, &usNbOfPrecedingSamples, sizeof(usNbOfPrecedingSamples));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Sampling period
        if(oResult)
        {
            usIndex = INDEX_SAMPLING_PERIOD;
            usSubIndex = SUBINDEX_SAMPLING_PERIOD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usSamplingPeriod, &errorInfo);
        }

        //Number of preceding samples
        if(oResult)
        {
            usIndex = INDEX_NB_OF_PRECEDING_SAMPLES;
            usSubIndex = SUBINDEX_NB_OF_PRECEDING_SAMPLES;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usNbOfPrecedingSamples, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_GetDataRecordingParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usSamplingPeriod(0);
    WORD usNbOfPrecedingSamples(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Sampling period
        if(oResult)
        {
            usIndex = INDEX_SAMPLING_PERIOD;
            usSubIndex = SUBINDEX_SAMPLING_PERIOD;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usSamplingPeriod, &errorInfo);
        }

        //Number of preceding samples
        if(oResult)
        {
            usIndex = INDEX_NB_OF_PRECEDING_SAMPLES;
            usSubIndex = SUBINDEX_NB_OF_PRECEDING_SAMPLES;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usNbOfPrecedingSamples, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usSamplingPeriod, sizeof(usSamplingPeriod));
        p_pCommand->SetReturnParameterData(1, &usNbOfPrecedingSamples, sizeof(usNbOfPrecedingSamples));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_EnableTrigger(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    BYTE ubTriggerType(0);
    WORD usConfiguration(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ubTriggerType, sizeof(ubTriggerType));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Recorder configuration
        if(oResult)
        {
            usIndex = INDEX_RECORDER_CONFIGURATION;
            usSubIndex = SUBINDEX_RECORDER_CONFIGURATION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usConfiguration, &errorInfo);

            if(oResult)
            {
                switch(ubTriggerType)
                {
                    case DRT_MOVEMENT_START_TRIGGER:    usConfiguration |= 0x0001; break;
                    case DRT_ERROR_TRIGGER:             usConfiguration |= 0x0002; break;
                    case DRT_DIGITAL_INPUT_TRIGGER:     usConfiguration |= 0x0004; break;
                    case DRT_MOVEMENT_END_TRIGGER:      usConfiguration |= 0x0008; break;
                }

                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usConfiguration, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_DisableAllTriggers(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD TRIGGER_CONFIGURATION_OFF = 0xFFF0;
    const WORD TRIGGER_CONTROL_OFF = 0xFFFD;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usConfiguration(0);
    WORD usControl(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Recorder configuration
        if(oResult)
        {
            usIndex = INDEX_RECORDER_CONFIGURATION;
            usSubIndex = SUBINDEX_RECORDER_CONFIGURATION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usConfiguration, &errorInfo);

            if(oResult)
            {
                usConfiguration &= TRIGGER_CONFIGURATION_OFF;
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usConfiguration, &errorInfo);
            }
        }

        //Recorder control
        if(oResult)
        {
            usIndex = INDEX_RECORDER_CONTROL;
            usSubIndex = SUBINDEX_RECORDER_CONTROL;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usControl, &errorInfo);

            if(oResult)
            {
                usControl &= TRIGGER_CONTROL_OFF;
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usControl, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ActivateChannel(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    BYTE ubChannelNumber;
    WORD usObjectIndex;
    BYTE ubObjectSubIndex;
    BYTE ubObjectSize;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ubChannelNumber, sizeof(ubChannelNumber));
        p_pCommand->GetParameterData(1, &usObjectIndex, sizeof(usObjectIndex));
        p_pCommand->GetParameterData(2, &ubObjectSubIndex, sizeof(ubObjectSubIndex));
        p_pCommand->GetParameterData(3, &ubObjectSize, sizeof(ubObjectSize));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Channel object index
        if(oResult)
        {
            usIndex = INDEX_VARIABLE_INDEX;
            usSubIndex = ubChannelNumber;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usObjectIndex, &errorInfo);
        }

        //Channel object subindex
        if(oResult)
        {
            usIndex = INDEX_VARIABLE_SUB_INDEX;
            usSubIndex = ubChannelNumber;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ubObjectSubIndex, &errorInfo);
        }

        //Activate number of channels
        if(oResult)
        {
            usIndex = INDEX_NB_OF_SAMPLING_VARIABLES;
            usSubIndex = SUBINDEX_NB_OF_SAMPLING_VARIABLES;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, ubChannelNumber, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_DeactivateAllChannels(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usDefaultValue(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Number of sampling variables
        if(oResult)
        {
            usIndex = INDEX_NB_OF_SAMPLING_VARIABLES;
            usSubIndex = SUBINDEX_NB_OF_SAMPLING_VARIABLES;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usDefaultValue, &errorInfo);
        }

        for(BYTE ubChannelNumber = 0; ubChannelNumber < MAX_NB_OF_DATA_RECORDER_VARIABLES; ubChannelNumber++)
        {
            //Channel object index
            if(oResult)
            {
                usIndex = INDEX_VARIABLE_INDEX;
                usSubIndex = ubChannelNumber + 1;
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usDefaultValue, &errorInfo);
            }

            //Channel object subindex
            if(oResult)
            {
                usIndex = INDEX_VARIABLE_SUB_INDEX;
                usSubIndex = ubChannelNumber + 1;
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usDefaultValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_StartRecorder(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD START_RECORDER = 0x0001;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usRecorderControl(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Recorder control
        if(oResult)
        {
            usIndex = INDEX_RECORDER_CONTROL;
            usSubIndex = SUBINDEX_RECORDER_CONTROL;
            usRecorderControl = START_RECORDER;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usRecorderControl, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_StopRecorder(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD STOP_RECORDER = 0x0000;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usRecorderControl(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Recorder control
        if(oResult)
        {
            usIndex = INDEX_RECORDER_CONTROL;
            usSubIndex = SUBINDEX_RECORDER_CONTROL;
            usRecorderControl = STOP_RECORDER;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usRecorderControl, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ForceTrigger(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD FORCE_TRIGGER = 0x0002;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usRecorderControl(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Recorder control
        if(oResult)
        {
            usIndex = INDEX_RECORDER_CONTROL;
            usSubIndex = SUBINDEX_RECORDER_CONTROL;
            usRecorderControl = FORCE_TRIGGER;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, usRecorderControl, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_IsRecorderRunning(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD RECORDER_RUNNING = 0x0001;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    BOOL oRecorderRunning = FALSE;
    WORD usRecorderStatus;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Recorder status
        if(oResult)
        {
            usIndex = INDEX_RECORDER_STATUS;
            usSubIndex = SUBINDEX_RECORDER_STATUS;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usRecorderStatus, &errorInfo);

            if(oResult)
            {
                if(usRecorderStatus & RECORDER_RUNNING)
                {
                    oRecorderRunning = TRUE;
                }
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oRecorderRunning, sizeof(oRecorderRunning));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_IsRecorderTriggered(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD RECORDER_TRIGGERED = 0x0002;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    BOOL oRecorderTriggered = FALSE;
    WORD usRecorderStatus;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Recorder status
        if(oResult)
        {
            usIndex = INDEX_RECORDER_STATUS;
            usSubIndex = SUBINDEX_RECORDER_STATUS;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usRecorderStatus, &errorInfo);

            if(oResult)
            {
                if(usRecorderStatus & RECORDER_TRIGGERED)
                {
                    oRecorderTriggered = TRUE;
                }
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oRecorderTriggered, sizeof(oRecorderTriggered));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ReadChannelVectorSize(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    WORD usVectorSize(0);
    DWORD ulVectorSize(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Max number of samples
        if(oResult)
        {
            usIndex = INDEX_MAX_NB_OF_SAMPLES;
            usSubIndex = SUBINDEX_MAX_NB_OF_SAMPLES;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usVectorSize, &errorInfo);
        }

        ulVectorSize = usVectorSize;

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulVectorSize, sizeof(ulVectorSize));
    }

    return oResult;
}

BOOL CGatewayDrive2ToEpos::Process_ReadDataBuffer(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    WORD usIndex;
    BYTE ubNodeId(0);
    WORD usSubIndex;

    void* pubDataBuffer(NULL);
    DWORD ulBufferSizeToRead;
    DWORD ulBufferSizeRead(0);
    WORD usVectorStartOffset(0);
    WORD usMaxNbOfSamples(0);
    WORD usNbOfRecordedSamples(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ulBufferSizeToRead, sizeof(ulBufferSizeToRead));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read data buffer
        if(oResult)
        {
            pubDataBuffer = malloc(ulBufferSizeToRead);
            usIndex = INDEX_DATA_BUFFER;
            usSubIndex = SUBINDEX_DATA_BUFFER;
            oResult = ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, pubDataBuffer, ulBufferSizeToRead, &ulBufferSizeRead, &errorInfo);
        }

        //Vector start offset
        if(oResult)
        {
            usIndex = INDEX_VECTOR_START_OFFSET;
            usSubIndex = SUBINDEX_VECTOR_START_OFFSET;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usVectorStartOffset, &errorInfo);
        }

        //Max number of samples
        if(oResult)
        {
            usIndex = INDEX_MAX_NB_OF_SAMPLES;
            usSubIndex = SUBINDEX_MAX_NB_OF_SAMPLES;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usMaxNbOfSamples, &errorInfo);
        }

        //Number of recorded samples
        if(oResult)
        {
            usIndex = INDEX_NB_OF_RECORDED_SAMPLES;
            usSubIndex = SUBINDEX_NB_OF_RECORDED_SAMPLES;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (WORD)ubNodeId, usIndex, usSubIndex, &usNbOfRecordedSamples, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pubDataBuffer, ulBufferSizeToRead);
        p_pCommand->SetReturnParameterData(1, &ulBufferSizeRead, sizeof(ulBufferSizeRead));
        p_pCommand->SetReturnParameterData(2, &usVectorStartOffset, sizeof(usVectorStartOffset));
        p_pCommand->SetReturnParameterData(3, &usMaxNbOfSamples, sizeof(usMaxNbOfSamples));
        p_pCommand->SetReturnParameterData(4, &usNbOfRecordedSamples, sizeof(usNbOfRecordedSamples));

        if(pubDataBuffer) free(pubDataBuffer);
    }

    return oResult;
}


