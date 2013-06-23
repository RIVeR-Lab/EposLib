// Command_VCS_Plc2.h: Schnittstelle f�r die Klasse CCommand_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_Plc2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_Plc2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include "Command_VCS_Plc2_Definitions.h"

class CCommand_VCS_Plc2 : public CCommand_VCS
{
public:
    CCommand_VCS_Plc2();
    CCommand_VCS_Plc2(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_Plc2();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_Plc2& operator=(CCommand_VCS_Plc2& other);
    CCommandRoot*CloneCommand();

private:
    BOOL InitCommand_Initialisation(DWORD p_ulCommandId);
    BOOL InitCommand_ProgramStatus(DWORD p_ulCommandId);
    BOOL InitCommand_ProgramVariableAccess(DWORD p_ulCommandId);
    BOOL InitCommand_ProcessInputOutputAccess(DWORD p_ulCommandId);
    BOOL InitCommand_ProcessImageAccess(DWORD p_ulCommandId);
    BOOL InitCommand_GeneralGateway(DWORD p_ulCommandId);
    BOOL InitCommand_LayerSettingServices(DWORD p_ulCommandId);
    BOOL InitCommand_FirmwareDownload(DWORD p_ulCommandId);

//Initialisation
    void SetDefaultParameter_SetBootupBehavior();
    void SetDefaultParameter_GetBootupBehavior();

//Program Status
    void SetDefaultParameter_ColdstartPlc();
    void SetDefaultParameter_WarmstartPlc();
    void SetDefaultParameter_HotstartPlc();
    void SetDefaultParameter_StopPlc();
    void SetDefaultParameter_ErasePlc();
    void SetDefaultParameter_GetPlcStatus();

//Program Variable Access
    void SetDefaultParameter_InitVariableTable();
    void SetDefaultParameter_SetVariable();
    void SetDefaultParameter_GetVariable();

//Process Input/Output Access
    void SetDefaultParameter_SetProcessInput();
    void SetDefaultParameter_GetProcessOutput();
    void SetDefaultParameter_SetProcessInputBit();
    void SetDefaultParameter_GetProcessOutputBit();

//Process Image Access
    void SetDefaultParameter_SetProcessInputImage();
    void SetDefaultParameter_GetProcessOutputImage();

//General Gateway
    void SetDefaultParameter_SendCANFrame();
    void SetDefaultParameter_RequestCANFrame();
    void SetDefaultParameter_ReadCANFrame();
    void SetDefaultParameter_SendNMTService();

//Layer Setting Services
    void SetDefaultParameter_SendLSSFrame();
    void SetDefaultParameter_ReadLSSFrame();

//Firmware Download
    void SetDefaultParameter_GetSerialNumber();
    void SetDefaultParameter_GetBaseSectorVersion();
};

#endif // !defined(AFX_Command_VCS_Plc2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
