// GatewayDriveToEsam.cpp: Implementierung der Klasse CGatewayDriveToEsam.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GatewayDriveToEsam.h"



#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>

#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenCommunicationDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenDriveDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CGatewayDriveToEsam::CGatewayDriveToEsam()
{
    InitErrorHandling();
}

CGatewayDriveToEsam::~CGatewayDriveToEsam()
{
}

CGateway* CGatewayDriveToEsam::Clone()
{
    CGatewayDriveToEsam* pClonedGateway;

    pClonedGateway = new CGatewayDriveToEsam();
    *pClonedGateway = *this;

    return (CGatewayVCStoDCS*)pClonedGateway;
}

CGatewayDriveToEsam& CGatewayDriveToEsam::operator=(CGatewayDriveToEsam& other)
{
    if(this != &other)
    {
        *((CGatewayVCStoEsam*)this) = *((CGatewayVCStoEsam*)&other);
    }

    return *this;
}

BOOL CGatewayDriveToEsam::ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE h_Handle, HANDLE p_hTransactionHandle)
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
                    case DRIVE_GET_CURRENT_REGULATOR_GAIN: oResult = Process_GetCurrentRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_ENCODER_PARAMETER: oResult = Process_GetEncoderParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_MOTOR_PARAMETER: oResult = Process_GetMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_POSITION_REGULATOR_GAIN: oResult = Process_GetPositionRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_VELOCITY_REGULATOR_GAIN: oResult = Process_GetVelocityRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_CURRENT_REGULATOR_GAIN: oResult = Process_SetCurrentRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_ENCODER_PARAMETER: oResult = Process_SetEncoderParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_MOTOR_PARAMETER: oResult = Process_SetMotorParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_POSITION_REGULATOR_GAIN: oResult = Process_SetPositionRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_VELOCITY_REGULATOR_GAIN: oResult = Process_SetVelocityRegulatorGain(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //CurrentMode
                    case DRIVE_GET_CURRENT_MUST: oResult = Process_GetCurrentMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_CURRENT_MUST: oResult = Process_SetCurrentMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //HomingMode
                    case DRIVE_FIND_HOME: oResult = Process_FindHome(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_HOMING_PARAMETER: oResult = Process_GetHomingParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_HOMING_PARAMETER: oResult = Process_SetHomingParameter(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_STOP_HOMING: oResult = Process_StopHoming(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //InputOutputs
                    case DRIVE_DIGITAL_INPUT_CONFIGURATION: oResult = Process_DigitalInputConfiguration(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_DIGITAL_OUTPUT_CONFIGURATION: oResult = Process_DigitalOutputConfiguration(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_ALL_DIGITAL_INPUTS: oResult = Process_GetAllDigitalInputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_ALL_DIGITAL_OUTPUTS: oResult = Process_GetAllDigitalOutputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_ANALOG_INPUT: oResult = Process_GetAnalogInput(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_ALL_DIGITAL_OUTPUTS: oResult = Process_SetAllDigitalOutputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //MotionInfo
                    case DRIVE_GET_CURRENT_IS: oResult = Process_GetCurrentIs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_MOVEMENT_STATE: oResult = Process_GetMovementState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_POSITION_IS: oResult = Process_GetPositionIs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_VELOCITY_IS: oResult = Process_GetVelocityIs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //PositionMode
                    case DRIVE_GET_POSITION_MUST: oResult = Process_GetPositionMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_POSITION_MUST: oResult = Process_SetPositionMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //ProfilEsamitionMode
                    case DRIVE_GET_POSITION_PROFILE: oResult = Process_GetPositionProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_TARGET_POSITION: oResult = Process_GetTargetPosition(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_HALT_POSITION_MOVEMENT: oResult = Process_HaltPositionMovement(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_MOVE_TO_POSITION: oResult = Process_MoveToPosition(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_POSITION_PROFILE: oResult = Process_SetPositionProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //ProfileVelocityMode
                    case DRIVE_GET_TARGET_VELOCITY: oResult = Process_GetTargetVelocity(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_VELOCITY_PROFILE: oResult = Process_GetVelocityProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_HALT_VELOCITY_MOVEMENT: oResult = Process_HaltVelocityMovement(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_MOVE_WITH_VELOCITY: oResult = Process_MoveWithVelocity(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_VELOCITY_PROFILE: oResult = Process_SetVelocityProfile(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //StateMachine
                    case DRIVE_CLEAR_FAULT: oResult = Process_ClearFault(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_DISABLE_STATE: oResult = Process_GetDisableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_ENABLE_STATE: oResult = Process_GetEnableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_FAULT_STATE: oResult = Process_GetFaultState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_OPERATION_MODE: oResult = Process_GetOperationMode(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_GET_QUICK_STOP_STATE: oResult = Process_GetQuickStopState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_DISABLE_STATE: oResult = Process_SetDisableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_ENABLE_STATE: oResult = Process_SetEnableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_OPERATION_MODE: oResult = Process_SetOperationMode(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_QUICK_STOP_STATE: oResult = Process_SetQuickStopState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //VelocityMode
                    case DRIVE_GET_VELOCITY_MUST: oResult = Process_GetVelocityMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case DRIVE_SET_VELOCITY_MUST: oResult = Process_SetVelocityMust(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

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

BOOL CGatewayDriveToEsam::Process_MoveToPosition(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lTargetPosition;
    BOOL oAbsolute;
    BOOL oImmediately;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get parameter data
        p_pCommand->GetParameterData(0, &lTargetPosition, sizeof(lTargetPosition));
        p_pCommand->GetParameterData(1, &oAbsolute, sizeof(oAbsolute));
        p_pCommand->GetParameterData(2, &oImmediately, sizeof(oImmediately));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            //Send target position
            usIndex = INDEX_TARGET_POSITION;
            ubSubIndex = SUBINDEX_TARGET_POSITION;
            oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, lTargetPosition, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            if(oAbsolute)
            {
                if(oImmediately)
                    //Send absolute and immediately
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, IMMEDIATE_ABS_POSITION, &errorInfo);
                else
                    //Send absolute
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ABSOLUTE_POSITION, &errorInfo);
            }
            else
            {
                if(oImmediately)
                    //Send relative and immediately
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, IMMEDIATE_REL_POSITION, &errorInfo);
                else
                    //Send relative
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, RELATIVE_POSITION, &errorInfo);
            }
        }
        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetCurrentRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usP = 0;
    WORD usI = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Current Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            ubSubIndex = SUBINDEX_CURRENT_P_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usP, &errorInfo);
        }

        //Read Current Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            ubSubIndex = SUBINDEX_CURRENT_I_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usI, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usP, sizeof(usP));
        p_pCommand->SetReturnParameterData(1, &usI, sizeof(usI));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetEncoderParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usCounts = 0;
    WORD usPositionSensorType = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read encoder resolution
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            ubSubIndex = SUBINDEX_ENCODER_PULSE_NUMBER;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usCounts, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            ubSubIndex = SUBINDEX_POSITION_SENSOR_TYPE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usPositionSensorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usCounts, sizeof(usCounts));
        p_pCommand->SetReturnParameterData(1, &usPositionSensorType, sizeof(usPositionSensorType));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usMotorType = 0;
    WORD usContinuousCurrent = 0;
    WORD usPeakCurrent = 0;
    BYTE ubPolePair = 0;
    WORD usThermalTimeConstant = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Motortype
        if(oResult)
        {
            usIndex = INDEX_MOTOR_TYPE;
            ubSubIndex = SUBINDEX_MOTOR_TYPE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usMotorType, &errorInfo);
        }

        //Read continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usContinuousCurrent, &errorInfo);
        }

        //Read Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usPeakCurrent, &errorInfo);
        }

        //Read pole paar
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_POLE_PAIR_NUMBER;
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ubPolePair, &errorInfo);
        }

        //Read Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usThermalTimeConstant, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_GetPositionRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usP = 0;
    WORD usI = 0;
    WORD usD = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Position Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            ubSubIndex = SUBINDEX_POSITION_P_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usP, &errorInfo);
        }

        //Read Position Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            ubSubIndex = SUBINDEX_POSITION_I_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usI, &errorInfo);
        }

        //Read Position Regulator D-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            ubSubIndex = SUBINDEX_POSITION_D_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usD, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_GetVelocityRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usP = 0;
    WORD usI = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Velocity Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            ubSubIndex = SUBINDEX_VELOCITY_P_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usP, &errorInfo);
        }

        //Read Velocity Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            ubSubIndex = SUBINDEX_VELOCITY_I_GAIN;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usI, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usP, sizeof(usP));
        p_pCommand->SetReturnParameterData(1, &usI, sizeof(usI));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetCurrentRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write Current Regualator P-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            ubSubIndex = SUBINDEX_CURRENT_P_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usP, &errorInfo);
        }

        //Write Current Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_CURRENT_GAIN;
            ubSubIndex = SUBINDEX_CURRENT_I_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usI, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetEncoderParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write encoder resolution
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            ubSubIndex = SUBINDEX_ENCODER_PULSE_NUMBER;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usCounts, &errorInfo);
        }

        //Write sensor type
        if(oResult)
        {
            usIndex = INDEX_SENSOR_CONFIGURATION;
            ubSubIndex = SUBINDEX_POSITION_SENSOR_TYPE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usPositionSensorType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetMotorParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write motor type
        if(oResult)
        {
            usIndex = INDEX_MOTOR_TYPE;
            ubSubIndex = SUBINDEX_MOTOR_TYPE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usMotorType, &errorInfo);
        }

        //Write continuous current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_MAX_CON_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usContinuousCurrent, &errorInfo);
        }

        //Write Peak current
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_MAX_PEAK_CURRENT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usPeakCurrent, &errorInfo);
        }

        //Write pole paar
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_POLE_PAIR_NUMBER;
            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ubPolePair, &errorInfo);
        }

        //Read Thermal Time constant
        if(oResult)
        {
            usIndex = INDEX_MOTOR_DATA;
            ubSubIndex = SUBINDEX_THERMAL_TIME_CONSTANT;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usThermalTimeConstant, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetPositionRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Position Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            ubSubIndex = SUBINDEX_POSITION_P_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usP, &errorInfo);
        }

        //Read Position Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            ubSubIndex = SUBINDEX_POSITION_I_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usI, &errorInfo);
        }

        //Write Position Regulator D-Gain
        if(oResult)
        {
            usIndex = INDEX_POSITION_GAIN;
            ubSubIndex = SUBINDEX_POSITION_D_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usD, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetVelocityRegulatorGain(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Velocity Regulator P-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            ubSubIndex = SUBINDEX_VELOCITY_P_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usP, &errorInfo);
        }

        //Read Velocity Regulator I-Gain
        if(oResult)
        {
            usIndex = INDEX_VELOCITY_GAIN;
            ubSubIndex = SUBINDEX_VELOCITY_I_GAIN;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usI, &errorInfo);
        }

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetCurrentMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    short iCurrentMust = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Current parameter must
        usIndex = INDEX_CURRENT_SETTING_VALUE;
        ubSubIndex = SUBINDEX_CURRENT_SETTING_VALUE;
        if(oResult) oResult = ReadInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &iCurrentMust, &errorInfo);

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &iCurrentMust, sizeof(iCurrentMust));
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_SetCurrentMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Current parameter must
        usIndex = INDEX_CURRENT_SETTING_VALUE;
        ubSubIndex = SUBINDEX_CURRENT_SETTING_VALUE;
        if(oResult) oResult = WriteInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, iCurrentMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_RequestCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
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

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //RequestCANFrame
        if(oResult) oResult = DCS_RequestCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_SendCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
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

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //SendCANFrame
        if(oResult) oResult = DCS_SendCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_ReadCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
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

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //ReadCANFrame
        pData = malloc(usLength);
        if(oResult) oResult = DCS_ReadCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usCobID, usLength, pData, ulTimeout, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_ReadLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE = 8;

    BOOL oResult = FALSE;
    BYTE ubPortNumber(0);
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

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //ReadLSSFrame
        if(oResult) oResult = DCS_ReadLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usTimeout, pDataBuffer, ulDataBufferLength, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_SendLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE = 8;

    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    void* pDataBuffer = NULL;
    DWORD ulDataBufferLength;
    DWORD ulErrorCode = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Prepare DataBuffer
        ulDataBufferLength = p_pCommand->GetParameterLength(1);
        if(ulDataBufferLength > MAX_FRAME_SIZE) ulDataBufferLength = MAX_FRAME_SIZE;
        pDataBuffer = malloc(ulDataBufferLength);
        memset(pDataBuffer, 0, ulDataBufferLength);

        //Get Parameter Data
        p_pCommand->GetParameterData(0, pDataBuffer, ulDataBufferLength);

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //SendLSSFrame
        if(oResult) oResult = DCS_SendLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, pDataBuffer, ulDataBufferLength, &errorInfo);
        if(!oResult) ulErrorCode = errorInfo.GetErrorCode();

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
        if(pDataBuffer) free(pDataBuffer);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_FindHome(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    WORD usStartHoming;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    __int8 bHomingMethod;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &bHomingMethod, sizeof(bHomingMethod));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Homing Method
        if(oResult)
        {
            usIndex = INDEX_HOMING_METHOD;
            ubSubIndex = SUBINDEX_HOMING_METHOD;
            oResult = WriteInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, bHomingMethod, &errorInfo);
        }

        //Send 0->1 Bit Change
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            usStartHoming = 0x000F;//000F => Homing operation inturrept start = 0 / Enable operation = F
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usStartHoming, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            usStartHoming = 0x001F;//001F => Homing operation start = 1 / Enable operation = F
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usStartHoming, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetHomingParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //HomingAcceleration
        if(oResult)
        {
            usIndex = INDEX_HOMING_ACCELERATION;
            ubSubIndex = SUBINDEX_HOMING_ACCELERATION;
            oResult= ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulHomingAcceleration, &errorInfo);
        }

        //SpeedSwitch
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            ubSubIndex = SUBINDEX_SPEED_SEARCH_SWITCH;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulSpeedSwitch, &errorInfo);
        }

        //SpeedIndex
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            ubSubIndex = SUBINDEX_SPEED_SEARCH_ZERO;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulSpeedIndex, &errorInfo);
        }

        //HomeOffset
        if(oResult)
        {
            usIndex = INDEX_HOME_OFFSET;
            ubSubIndex = SUBINDEX_HOME_OFFSET;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, (DWORD*) &lHomeOffset, &errorInfo);
        }

        //CurrentThreshold
        if(oResult)
        {
            usIndex = INDEX_CURRENT_THRESHOLD;
            ubSubIndex = SUBINDEX_CURRENT_THRESHOLD;
            ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usCurrentThreshold, &errorInfo);
        }

        //HomePosition
        if(oResult)
        {
            usIndex = INDEX_HOME_POSITION;
            ubSubIndex = SUBINDEX_HOME_POSITION;
            if(!ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, (DWORD*) &lHomePosition, &errorInfo))
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

BOOL CGatewayDriveToEsam::Process_SetHomingParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //HomingAcceleration
        if(oResult)
        {
            usIndex = INDEX_HOMING_ACCELERATION;
            ubSubIndex = SUBINDEX_HOMING_ACCELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulHomingAcceleration, &errorInfo);
        }

        //SpeedSwitch
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            ubSubIndex = SUBINDEX_SPEED_SEARCH_SWITCH;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulSpeedSwitch, &errorInfo);
        }

        //SpeedIndex
        if(oResult)
        {
            usIndex = INDEX_HOMING_SPEED;
            ubSubIndex = SUBINDEX_SPEED_SEARCH_ZERO;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulSpeedIndex, &errorInfo);
        }

        //HomeOffset
        if(oResult)
        {
            usIndex = INDEX_HOME_OFFSET;
            ubSubIndex = SUBINDEX_HOME_OFFSET;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, (DWORD)lHomeOffset, &errorInfo);
        }

        //CurrentThreshold
        if(oResult)
        {
            usIndex = INDEX_CURRENT_THRESHOLD;
            ubSubIndex = SUBINDEX_CURRENT_THRESHOLD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usCurrentThreshold, &errorInfo);
        }

        //HomePosition
        if(oResult)
        {
            usIndex = INDEX_HOME_POSITION;
            ubSubIndex = SUBINDEX_HOME_POSITION;
            if(!WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, (DWORD)lHomePosition, &errorInfo))
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

BOOL CGatewayDriveToEsam::Process_StopHoming(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    WORD usStopHoming;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Send 1->0 Bit Change
        usIndex = INDEX_CONTROL_WORD;
        ubSubIndex = SUBINDEX_CONTROL_WORD;
        usStopHoming = 0x001F;//001F => Homing operation start = 1 / Enable operation = F
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usStopHoming, &errorInfo);

        if(oResult)
        {
            usStopHoming = 0x000F;//000F => Homing operation inturrept start = 0 / Enable operation = F
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usStopHoming, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_DigitalInputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    WORD usValue;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write Digital Input Configuration
        usIndex = INDEX_DIGITAL_INPUT_CONFIGURATION;
        ubSubIndex = (BYTE)usDigitalInputNb;
        usValue = usConfiguration;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_INPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_INPUT_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oPolarity);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oExecutionMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_DigitalOutputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    WORD usValue;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write Digital Output Configuration
        usIndex = INDEX_DIGITAL_OUTPUT_CONFIGURATION;
        ubSubIndex = (BYTE)usDigitalOutputNb;
        usValue = usConfiguration;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_STATE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oState);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oPolarity);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetAllDigitalInputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usInputs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read state of all digital inputs
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_INPUT_STATE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usInputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usInputs, sizeof(usInputs));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usOutputs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read state of all digital outputs
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_STATE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usOutputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usOutputs, sizeof(usOutputs));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetAnalogInput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usNumber;
    WORD usAnalog = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usNumber, sizeof(usNumber));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read value of one analog input
        if(oResult)
        {
            usIndex = INDEX_ANALOG_INPUTS;
            ubSubIndex = (BYTE)usNumber;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usAnalog, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usAnalog, sizeof(usAnalog));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usOutputs;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usOutputs, sizeof(usOutputs));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Write state of all digital outputs
        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_STATE;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, usOutputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetCurrentIs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    short iCurrentIs;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //CurrentIs
        if(oResult)
        {
            usIndex = INDEX_CURRENT_ACTUAL_VALUE;
            ubSubIndex = SUBINDEX_CURRENT_ACTUAL_VALUE;
            oResult = ReadInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &iCurrentIs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &iCurrentIs, sizeof(iCurrentIs));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetMovementState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    BOOL oTargetReached;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //TargetReached
        if(oResult)
        {
            //Read Device State
            usIndex = INDEX_STATUS_WORD;
            ubSubIndex = SUBINDEX_STATUS_WORD;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usDeviceState, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_GetPositionIs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lPositionIs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //PositionIs
        usIndex = INDEX_POSITION_ACTUAL_VALUE;
        ubSubIndex = SUBINDEX_POSITION_ACTUAL_VALUE;
        if(oResult) oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &lPositionIs, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lPositionIs, sizeof(lPositionIs));
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_GetVelocityIs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lVelocityIs = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //VeloctiyIs
        usIndex = INDEX_VELOCITY_ACTUAL_VALUE;
        ubSubIndex = SUBINDEX_VELOCITY_ACTUAL_VALUE;
        if(oResult) oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &lVelocityIs, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lVelocityIs, sizeof(lVelocityIs));
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_GetPositionMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lPositionMust = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //PositionMust
        usIndex = INDEX_POSITION_SETTING_VALUE;
        ubSubIndex = SUBINDEX_POSITION_SETTING_VALUE;
        if(oResult) oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &lPositionMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lPositionMust, sizeof(lPositionMust));
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_SetPositionMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //PositionMust
        usIndex = INDEX_POSITION_SETTING_VALUE;
        ubSubIndex = SUBINDEX_POSITION_SETTING_VALUE;
        if(oResult) oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, lPositionMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetPositionProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_PROFILE_VELOCITY;
            ubSubIndex = SUBINDEX_PROFILE_VELOCITY;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulProfileVelocity, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_PROFILE_ACCELERATION;
            ubSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulProfileAcceleration, &errorInfo);
        }

        if(oResult)
        {
            usIndex = INDEX_PROFILE_DECELERATION;
            ubSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulProfileDeceleration, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_GetTargetPosition(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lTargetPosition;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_TARGET_POSITION;
            ubSubIndex = SUBINDEX_TARGET_POSITION;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &lTargetPosition, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lTargetPosition, sizeof(lTargetPosition));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_HaltPositionMovement(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        usIndex = INDEX_CONTROL_WORD;
        ubSubIndex = SUBINDEX_CONTROL_WORD;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, STOP_PROFILE, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetPositionProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult)
        {
            //Send velocity
            usIndex = INDEX_PROFILE_VELOCITY;
            ubSubIndex = SUBINDEX_PROFILE_VELOCITY;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulProfileVelocity, &errorInfo);
        }

        if(oResult)
        {
            //Send acceleration
            usIndex = INDEX_PROFILE_ACCELERATION;
            ubSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulProfileAcceleration, &errorInfo);
        }

        if(oResult)
        {
            //Send deceleration
            usIndex = INDEX_PROFILE_DECELERATION;
            ubSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulProfileDeceleration, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetTargetVelocity(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lTargetVelocity = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read target velocity
        if(oResult)
        {
            usIndex = INDEX_TARGET_VELOCITY;
            ubSubIndex = SUBINDEX_TARGET_VELOCITY;
            oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &lTargetVelocity, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetVelocityProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    DWORD ulProfileAcceleration = 0;
    DWORD ulProfileDeceleration = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read ProfileAcceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_ACCELERATION;
            ubSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulProfileAcceleration, &errorInfo);
        }

        //Read ProfileDeceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_DECELERATION;
            ubSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulProfileDeceleration, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulProfileAcceleration, sizeof(ulProfileAcceleration));
        p_pCommand->SetReturnParameterData(1, &ulProfileDeceleration, sizeof(ulProfileDeceleration));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_HaltVelocityMovement(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Send Stop
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, STOP_PROFILE, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_MoveWithVelocity(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lTargetVelocity;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Set Parameter data
        p_pCommand->GetParameterData(0, &lTargetVelocity, sizeof(lTargetVelocity));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Send target velocity
        if(oResult)
        {
            usIndex = INDEX_TARGET_VELOCITY;
            ubSubIndex = SUBINDEX_TARGET_VELOCITY;
            oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, lTargetVelocity, &errorInfo);
        }

        //Move
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, MOVE_WITH_VELOCITY, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetVelocityProfile(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Acceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_ACCELERATION;
            ubSubIndex = SUBINDEX_PROFILE_ACCELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulProfileAcceleration, &errorInfo);
        }

        //Set Decceleration
        if(oResult)
        {
            usIndex = INDEX_PROFILE_DECELERATION;
            ubSubIndex = SUBINDEX_PROFILE_DECELERATION;
            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ulProfileDeceleration, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_ClearFault(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Clear Fault
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, FAULT_RESET, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetDisableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_DISABLE = 0x004F;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    BOOL oIsDisabled;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Disable state
        if(oResult)
        {
            oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);

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

BOOL CGatewayDriveToEsam::Process_GetEnableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_ENABLE = 0x006F;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    BOOL oIsEnabled;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read enable state
        if(oResult)
        {
            oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);

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

BOOL CGatewayDriveToEsam::Process_GetFaultState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_FAULT = 0x004F;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    BOOL oIsInFault;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Fault state
        if(oResult)
        {
            oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);

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

BOOL CGatewayDriveToEsam::Process_GetOperationMode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    __int8 bMode = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Operation Mode
        if(oResult)
        {
            usIndex = INDEX_OPERATION_MODE_DISPLAY;
            ubSubIndex = SUBINDEX_OPERATION_MODE_DISPLAY;
            oResult = ReadInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &bMode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &bMode, sizeof(bMode));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetQuickStopState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const WORD BIT_MASK_QUICK_STOP = 0x006F;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    BOOL oIsQuickStoped = FALSE;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Quick state
        if(oResult)
        {
            oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);

            if(oResult)
            {
                usDeviceState &= BIT_MASK_QUICK_STOP;
                if(usDeviceState == DS_QUICK_STOP_ACTIV)
                    oIsQuickStoped = TRUE;
                else
                    oIsQuickStoped = FALSE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oIsQuickStoped, sizeof(oIsQuickStoped));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SendNMTService(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    WORD usNodeIdentifer;
    WORD usCommandSpecifier;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usNodeIdentifer, sizeof(usNodeIdentifer));
        p_pCommand->GetParameterData(1, &usCommandSpecifier, sizeof(usCommandSpecifier));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //SendNMTService
        if(oResult)
        {
            oResult = DCS_SendNMTService(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usNodeIdentifer, usCommandSpecifier, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetDisableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Disable State
        if(oResult) oResult = SetDisableState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_SetEnableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;
    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Device State
        if(oResult) oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            switch(usDeviceState)
            {
                case DS_OPERATION_ENABLED:
                    return TRUE;

                case DS_SWITCH_ON_DISABLED:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, SHUTDOWN, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, DS_READY_TO_SWITCH_ON, &errorInfo);

                case DS_READY_TO_SWITCH_ON:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, SWITCH_ON, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, DS_OPERATION_ENABLED, &errorInfo);

                case SWITCH_ON:

                case DS_QUICK_STOP_ACTIV:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ENABLE_OPERATION, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, DS_OPERATION_ENABLED, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_SetOperationMode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    __int8 bMode;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        p_pCommand->GetParameterData(0, &bMode, sizeof(bMode));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Operation Mode;
        if(oResult)
        {
            usIndex = INDEX_OPERATION_MODE;
            ubSubIndex = SUBINDEX_OPERATION_MODE;
            oResult = WriteInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, bMode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_SetQuickStopState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read Device State
        if(oResult) oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);
        if(oResult)
        {
            if(((usDeviceState & DS_QUICK_STOP_ACTIV) || (usDeviceState & DS_OPERATION_ENABLED))&&(oResult))
            {
                if((usDeviceState & DS_OPERATION_ENABLED) == DS_OPERATION_ENABLED)
                {
                    //Set Quick Stop
                    usIndex = INDEX_CONTROL_WORD;
                    ubSubIndex = SUBINDEX_CONTROL_WORD;
                    oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, QUICK_STOP, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_GetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = TRUE;
    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //GetObject
        if(oResult)
        {
            pData = malloc(ulNbOfBytesToRead);
			memset(pData, 0, ulNbOfBytesToRead),

            oResult = ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_GetVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    WORD usSoftwareVersion = 0;
    WORD usHardwareVersion = 0;
    WORD usApplicationNumber = 0;
    WORD usApplicationVersion = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Read HardwareVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_HARDWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usHardwareVersion, &errorInfo);
        }

        //Read SoftwareVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_SOFTWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usSoftwareVersion, &errorInfo);
        }

        //Read ApplicationNumber
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_APPLICATION_NUMBER;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usApplicationNumber, &errorInfo);
        }

        //Read ApplicationVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_APPLICATION_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usApplicationVersion, &errorInfo);
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

BOOL CGatewayDriveToEsam::Process_GetDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    DWORD ulDeviceType = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Device Type
        if(oResult)
        {
            usIndex = INDEX_DEVICE_TYPE;
            ubSubIndex = SUBINDEX_DEVICE_TYPE;
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &ulDeviceType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_Restore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand && p_pManager)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        if(oResult) oResult = RestoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_SetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //SetObject
        if(oResult) oResult = WriteDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNbOfBytesWritten, sizeof(ulNbOfBytesWritten));
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_Store(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand && p_pManager)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Store
        if(oResult) oResult = StoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_ResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
	BOOL oWaitForReboot(FALSE);
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &oWaitForReboot, sizeof(oWaitForReboot));

        //SendNMTService
        if(oResult) oResult = ResetDevice(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, oWaitForReboot, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_WaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubPortNumber(0);
	BYTE ubNodeId(0);
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
		if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //WaitForReboot
        if(oResult) oResult = WaitForReboot(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_ClearDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Clear Fault
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, FAULT_RESET, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetNbOfDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    WORD usSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;
    BYTE ubNbOfDeviceError = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Number of Device Error
        if(oResult)
        {
            usIndex = INDEX_ERROR_HISTORY;
            usSubIndex = SUBINDEX_NB_OF_ERRORS;
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (BYTE)ubPortNumber, ubNodeId, usIndex, (BYTE)usSubIndex, &ubNbOfDeviceError, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ubNbOfDeviceError, sizeof(ubNbOfDeviceError));

        return oResult;
    }
    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GetDeviceErrorCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubErrorNumber(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;
    DWORD ulDeviceErrorCode = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ubErrorNumber, sizeof(ubErrorNumber));

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Code from Device Error
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, (BYTE)ubPortNumber, ubNodeId, INDEX_ERROR_HISTORY, ubErrorNumber, &ulDeviceErrorCode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayDriveToEsam::Process_GotoSafeState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    //Address Parameter
    BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    CErrorInfo errorInfo;
    BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //Set Disable State
        if(oResult) oResult = SetDisableState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_GetVelocityMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    int lVelocityMust = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //VelocityMust
        usIndex = INDEX_VELOCITY_SETTING_VALUE;
        ubSubIndex = SUBINDEX_VELOCITY_SETTING_VALUE;
        if(oResult) oResult = ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &lVelocityMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &lVelocityMust, sizeof(lVelocityMust));
    }

    return oResult;
}

BOOL CGatewayDriveToEsam::Process_SetVelocityMust(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
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
        if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);
        if(oResult) oResult = GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, ubNodeId, &errorInfo);

        //VelocityMust
        usIndex = INDEX_VELOCITY_SETTING_VALUE;
        ubSubIndex = SUBINDEX_VELOCITY_SETTING_VALUE;
        if(oResult) oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, lVelocityMust, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

WORD CGatewayDriveToEsam::GetNewBitMask(WORD p_usConfiguration, WORD p_usOldMask, BOOL p_oState)
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

BOOL CGatewayDriveToEsam::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayDriveToEsam";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}


