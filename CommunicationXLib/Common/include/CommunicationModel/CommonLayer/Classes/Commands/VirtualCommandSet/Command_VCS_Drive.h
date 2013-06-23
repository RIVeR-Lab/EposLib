// Command_VCS_Drive.h: Schnittstelle f�r die Klasse CCommand_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_Drive_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_Drive_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive_Definitions.h>

class CCommand_VCS_Drive : public CCommand_VCS
{
public:
    CCommand_VCS_Drive();
    CCommand_VCS_Drive(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_Drive();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_Drive& operator=(CCommand_VCS_Drive& other);
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
    BOOL InitCommand_ProfileVelocityMode(DWORD p_ulCommandId);
    BOOL InitCommand_StateMachine(DWORD p_ulCommandId);
    BOOL InitCommand_VelocityMode(DWORD p_ulCommandId);
    BOOL InitCommand_FirmwareDownload(DWORD p_ulCommandId);
    BOOL InitCommand_MasterEncoderMode(DWORD p_ulCommandId);
    BOOL InitCommand_StepDirectionMode(DWORD p_ulCommandId);

    //Configuration
	void SetDefaultParameter_GetCurrentRegulatorGain();
    void SetDefaultParameter_GetPositionRegulatorGain();
    void SetDefaultParameter_GetPositionRegulatorFeedForward();
    void SetDefaultParameter_GetVelocityRegulatorGain();
    void SetDefaultParameter_GetMaxFollowingError();
    void SetDefaultParameter_GetMaxProfileVelocity();
    void SetDefaultParameter_GetVelocityUnits();
    void SetDefaultParameter_SetCurrentRegulatorGain();
    void SetDefaultParameter_SetPositionRegulatorGain();
    void SetDefaultParameter_SetPositionRegulatorFeedForward();
    void SetDefaultParameter_SetVelocityRegulatorGain();
    void SetDefaultParameter_SetMaxFollowingError();
    void SetDefaultParameter_SetMaxProfileVelocity();
    void SetDefaultParameter_SetVelocityUnits();

    //Configuration Sensor
	void SetDefaultParameter_SetEncoderParameter();
    void SetDefaultParameter_GetEncoderParameter();
    void SetDefaultParameter_SetSensorType();
    void SetDefaultParameter_GetSensorType();
    void SetDefaultParameter_SetIncEncoderParameter();
    void SetDefaultParameter_GetIncEncoderParameter();
    void SetDefaultParameter_SetHallSensorParameter();
    void SetDefaultParameter_GetHallSensorParameter();
    
    //Configuration Motor
	void SetDefaultParameter_SetMotorParameter();
    void SetDefaultParameter_SetMotorType();
    void SetDefaultParameter_SetDcMotorParameter();
    void SetDefaultParameter_SetEcMotorParameter();
    void SetDefaultParameter_GetMotorType();
    void SetDefaultParameter_GetMotorParameter();
    void SetDefaultParameter_GetDcMotorParameter();
    void SetDefaultParameter_GetEcMotorParameter();

    //CM
	void SetDefaultParameter_GetCurrentMust();
    void SetDefaultParameter_SetCurrentMust();
    
    //HM
	void SetDefaultParameter_FindHome();
    void SetDefaultParameter_GetHomingParameter();
    void SetDefaultParameter_SetHomingParameter();
    void SetDefaultParameter_StopHoming();
    void SetDefaultParameter_DefinePosition();
	void SetDefaultParameter_GetHomingState();

    //Input Output
	void SetDefaultParameter_DigitalInputConfiguration();
    void SetDefaultParameter_DigitalOutputConfiguration();
    void SetDefaultParameter_AnalogInputConfiguration();
    void SetDefaultParameter_GetAllDigitalInputs();
    void SetDefaultParameter_GetAllDigitalOutputs();
    void SetDefaultParameter_GetAnalogInput();
    void SetDefaultParameter_SetAllDigitalOutputs();
    void SetDefaultParameter_SetAnalogOutput();

	//Position Marker
	void SetDefaultParameter_SetPositionMarkerParameter();
    void SetDefaultParameter_GetPositionMarkerParameter();
    void SetDefaultParameter_ActivatePositionMarker();
    void SetDefaultParameter_DeactivatePositionMarker();
    void SetDefaultParameter_ReadPositionMarkerCounter();
    void SetDefaultParameter_ReadPositionMarkerCapturedPosition();
    void SetDefaultParameter_ResetPositionMarkerCounter();

    //Movement State
	void SetDefaultParameter_GetCurrentIs();
    void SetDefaultParameter_GetCurrentIsAveraged();
    void SetDefaultParameter_GetMovementState();
    void SetDefaultParameter_GetPositionIs();
    void SetDefaultParameter_GetVelocityIs();
    void SetDefaultParameter_GetVelocityIsAveraged();

    //PM
    void SetDefaultParameter_GetPositionMust();
    void SetDefaultParameter_SetPositionMust();
    
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
    
    //StateMachine
	void SetDefaultParameter_ClearFault();
    void SetDefaultParameter_GetDisableState();
    void SetDefaultParameter_GetEnableState();
    void SetDefaultParameter_GetFaultState();
    void SetDefaultParameter_GetOperationMode();
    void SetDefaultParameter_GetQuickStopState();
    void SetDefaultParameter_SetDisableState();
    void SetDefaultParameter_SetEnableState();
    void SetDefaultParameter_SetOperationMode();
    void SetDefaultParameter_SetQuickStopState();

    //VM
	void SetDefaultParameter_GetVelocityMust();
    void SetDefaultParameter_SetVelocityMust();
    
    //Firmware Download
	void SetDefaultParameter_GetSerialNumber();
    void SetDefaultParameter_GetBaseSectorVersion();

    //MEM
    void SetDefaultParameter_SetStepDirectionParameter();
    void SetDefaultParameter_GetStepDirectionParameter();

    //SDM
    void SetDefaultParameter_SetMasterEncoderParameter();
    void SetDefaultParameter_GetMasterEncoderParameter();
   
};

#endif // !defined(AFX_Command_VCS_Drive_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
