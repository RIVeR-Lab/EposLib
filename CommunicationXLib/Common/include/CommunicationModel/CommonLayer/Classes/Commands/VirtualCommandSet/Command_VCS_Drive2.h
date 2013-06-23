// Command_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommand_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_Drive2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_Drive2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include "Command_VCS_Drive2_Definitions.h"

class CCommand_VCS_Drive2 : public CCommand_VCS
{
public:
    CCommand_VCS_Drive2();
    CCommand_VCS_Drive2(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_Drive2();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_Drive2& operator=(CCommand_VCS_Drive2& other);
    CCommandRoot*CloneCommand();

private:
    BOOL InitCommand_Configuration(DWORD p_ulCommandId);
    BOOL InitCommand_ConfigurationMotor(DWORD p_ulCommandId);
    BOOL InitCommand_ConfigurationSensor(DWORD p_ulCommandId);
    BOOL InitCommand_CurrentMode(DWORD p_ulCommandId);
    BOOL InitCommand_HomingMode(DWORD p_ulCommandId);
    BOOL InitCommand_InputsOutputs(DWORD p_ulCommandId);
    BOOL InitCommand_MotionInfo(DWORD p_ulCommandId);
    BOOL InitCommand_PositionMode(DWORD p_ulCommandId);
    BOOL InitCommand_ProfilePositionMode(DWORD p_ulCommandId);
    BOOL InitCommand_InterpolatedPositionMode(DWORD p_ulCommandId);
    BOOL InitCommand_ProfileVelocityMode(DWORD p_ulCommandId);
    BOOL InitCommand_StateMachine(DWORD p_ulCommandId);
    BOOL InitCommand_VelocityMode(DWORD p_ulCommandId);
    BOOL InitCommand_FirmwareDownload(DWORD p_ulCommandId);
    BOOL InitCommand_MasterEncoderMode(DWORD p_ulCommandId);
    BOOL InitCommand_StepDirectionMode(DWORD p_ulCommandId);

    void SetDefaultParameter_GetCurrentRegulatorGain();
    void SetDefaultParameter_GetPositionRegulatorGain();
    void SetDefaultParameter_GetPositionRegulatorFeedForward();
    void SetDefaultParameter_GetVelocityRegulatorGain();
    void SetDefaultParameter_GetVelocityRegulatorFeedForward();
    void SetDefaultParameter_GetMaxFollowingError();
    void SetDefaultParameter_GetMaxProfileVelocity();
    void SetDefaultParameter_GetMaxAcceleration();
    void SetDefaultParameter_GetVelocityUnits();
    void SetDefaultParameter_SetCurrentRegulatorGain();
    void SetDefaultParameter_SetPositionRegulatorGain();
    void SetDefaultParameter_SetPositionRegulatorFeedForward();
    void SetDefaultParameter_SetVelocityRegulatorGain();
    void SetDefaultParameter_SetVelocityRegulatorFeedForward();
    void SetDefaultParameter_SetMaxFollowingError();
    void SetDefaultParameter_SetMaxProfileVelocity();
    void SetDefaultParameter_SetMaxAcceleration();
    void SetDefaultParameter_SetVelocityUnits();

    void SetDefaultParameter_SetEncoderParameter();
    void SetDefaultParameter_GetEncoderParameter();
    void SetDefaultParameter_SetSensorType();
    void SetDefaultParameter_GetSensorType();
    void SetDefaultParameter_SetIncEncoderParameter();
    void SetDefaultParameter_GetIncEncoderParameter();
    void SetDefaultParameter_SetHallSensorParameter();
    void SetDefaultParameter_GetHallSensorParameter();
    void SetDefaultParameter_SetSsiAbsEncoderParameter();
    void SetDefaultParameter_GetSsiAbsEncoderParameter();

    void SetDefaultParameter_SetMotorParameter();
    void SetDefaultParameter_SetMotorType();
    void SetDefaultParameter_SetDcMotorParameter();
    void SetDefaultParameter_SetEcMotorParameter();
    void SetDefaultParameter_GetMotorType();
    void SetDefaultParameter_GetMotorParameter();
    void SetDefaultParameter_GetDcMotorParameter();
    void SetDefaultParameter_GetEcMotorParameter();

    void SetDefaultParameter_GetCurrentMust();
    void SetDefaultParameter_SetCurrentMust();
    void SetDefaultParameter_ActivateAnalogCurrentSetpoint();
    void SetDefaultParameter_DeactivateAnalogCurrentSetpoint();
    void SetDefaultParameter_EnableAnalogCurrentSetpoint();
    void SetDefaultParameter_DisableAnalogCurrentSetpoint();

    void SetDefaultParameter_FindHome();
    void SetDefaultParameter_GetHomingParameter();
    void SetDefaultParameter_SetHomingParameter();
    void SetDefaultParameter_StopHoming();
    void SetDefaultParameter_DefinePosition();
	void SetDefaultParameter_GetHomingState();

    void SetDefaultParameter_DigitalInputConfiguration();
    void SetDefaultParameter_DigitalOutputConfiguration();
    void SetDefaultParameter_AnalogInputConfiguration();
    void SetDefaultParameter_GetAllDigitalInputs();
    void SetDefaultParameter_GetAllDigitalOutputs();
    void SetDefaultParameter_GetAnalogInput();
    void SetDefaultParameter_SetAllDigitalOutputs();
    void SetDefaultParameter_SetAnalogOutput();

    void SetDefaultParameter_SetPositionCompareParameter();
    void SetDefaultParameter_GetPositionCompareParameter();
    void SetDefaultParameter_ActivatePositionCompare();
    void SetDefaultParameter_DeactivatePositionCompare();
    void SetDefaultParameter_EnablePositionCompare();
    void SetDefaultParameter_DisablePositionCompare();
    void SetDefaultParameter_SetPositionCompareReferencePosition();
    void SetDefaultParameter_SetPositionMarkerParameter();
    void SetDefaultParameter_GetPositionMarkerParameter();
    void SetDefaultParameter_ActivatePositionMarker();
    void SetDefaultParameter_DeactivatePositionMarker();
    void SetDefaultParameter_ReadPositionMarkerCounter();
    void SetDefaultParameter_ReadPositionMarkerCapturedPosition();
    void SetDefaultParameter_ResetPositionMarkerCounter();

    void SetDefaultParameter_GetCurrentIs();
    void SetDefaultParameter_GetCurrentIsAveraged();
    void SetDefaultParameter_GetMovementState();
    void SetDefaultParameter_GetPositionIs();
    void SetDefaultParameter_GetVelocityIs();
    void SetDefaultParameter_GetVelocityIsAveraged();

    //PM
    void SetDefaultParameter_GetPositionMust();
    void SetDefaultParameter_SetPositionMust();
    void SetDefaultParameter_ActivateAnalogPositionSetpoint();
    void SetDefaultParameter_DeactivateAnalogPositionSetpoint();
    void SetDefaultParameter_EnableAnalogPositionSetpoint();
    void SetDefaultParameter_DisableAnalogPositionSetpoint();

    //PPM
    void SetDefaultParameter_GetPositionProfile();
    void SetDefaultParameter_GetTargetPosition();
    void SetDefaultParameter_HaltPositionMovement();
    void SetDefaultParameter_MoveToPosition();
    void SetDefaultParameter_SetPositionProfile();
    void SetDefaultParameter_EnablePositionWindow();
    void SetDefaultParameter_DisablePositionWindow();

    //PPM
    void SetDefaultParameter_GetTargetVelocity();
    void SetDefaultParameter_GetVelocityProfile();
    void SetDefaultParameter_HaltVelocityMovement();
    void SetDefaultParameter_MoveWithVelocity();
    void SetDefaultParameter_SetVelocityProfile();
    void SetDefaultParameter_EnableVelocityWindow();
    void SetDefaultParameter_DisableVelocityWindow();

    //IPM
    void SetDefaultParameter_SetIpmBufferParameter();
    void SetDefaultParameter_GetIpmBufferParameter();
    void SetDefaultParameter_ClearIpmBuffer();
    void SetDefaultParameter_GetFreeIpmBufferSize();
    void SetDefaultParameter_AddPvtValueToIpmBuffer();
    void SetDefaultParameter_StartIpmTrajectory();
    void SetDefaultParameter_StopIpmTrajectory();
    void SetDefaultParameter_GetIpmStatus();

    void SetDefaultParameter_ClearFault();
    void SetDefaultParameter_GetState();
    void SetDefaultParameter_GetDisableState();
    void SetDefaultParameter_GetEnableState();
    void SetDefaultParameter_GetFaultState();
    void SetDefaultParameter_GetOperationMode();
    void SetDefaultParameter_GetQuickStopState();
    void SetDefaultParameter_SendNMTService();
    void SetDefaultParameter_SetState();
    void SetDefaultParameter_SetDisableState();
    void SetDefaultParameter_SetEnableState();
    void SetDefaultParameter_SetOperationMode();
    void SetDefaultParameter_SetQuickStopState();

    void SetDefaultParameter_GetVelocityMust();
    void SetDefaultParameter_SetVelocityMust();
    void SetDefaultParameter_ActivateAnalogVelocitySetpoint();
    void SetDefaultParameter_DeactivateAnalogVelocitySetpoint();
    void SetDefaultParameter_EnableAnalogVelocitySetpoint();
    void SetDefaultParameter_DisableAnalogVelocitySetpoint();

    void SetDefaultParameter_GetSerialNumber();
    void SetDefaultParameter_GetBaseSectorVersion();

    //MEM
    void SetDefaultParameter_SetStepDirectionParameter();
    void SetDefaultParameter_GetStepDirectionParameter();

    //SDM
    void SetDefaultParameter_SetMasterEncoderParameter();
    void SetDefaultParameter_GetMasterEncoderParameter();
   
};

#endif // !defined(AFX_Command_VCS_Drive2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
