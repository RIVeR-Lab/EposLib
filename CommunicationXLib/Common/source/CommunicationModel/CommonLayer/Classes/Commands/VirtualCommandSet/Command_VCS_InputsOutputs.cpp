// Command_VCS_InputsOutputs.cpp: Implementierung der Klasse CCommand_VCS_InputsOutputs.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_InputsOutputs.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////
CCommand_VCS_InputsOutputs::CCommand_VCS_InputsOutputs()
{
}

CCommand_VCS_InputsOutputs::CCommand_VCS_InputsOutputs(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_InputsOutputs::~CCommand_VCS_InputsOutputs()
{
}

CCommandRoot*CCommand_VCS_InputsOutputs::CloneCommand()
{
    CCommand_VCS_InputsOutputs* pNewCommand;

    pNewCommand = new CCommand_VCS_InputsOutputs();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_InputsOutputs& CCommand_VCS_InputsOutputs::operator=(CCommand_VCS_InputsOutputs& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_InputsOutputs::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_InputsOutputs(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_InputsOutputs::InitCommand_InputsOutputs(DWORD p_ulCommandId)
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
    }

    return FALSE;
}

void CCommand_VCS_InputsOutputs::SetDefaultParameter_DigitalInputConfiguration()
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

void CCommand_VCS_InputsOutputs::SetDefaultParameter_DigitalOutputConfiguration()
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

void CCommand_VCS_InputsOutputs::SetDefaultParameter_AnalogInputConfiguration()
{
    WORD usInputNb = 0;
    WORD usConfiguration = 0x0000;
    BOOL oExecutionMask = FALSE;

    //Parameter
    SetParameterData(0, &usInputNb, sizeof(usInputNb));
    SetParameterData(1, &usConfiguration, sizeof(usConfiguration));
    SetParameterData(2, &oExecutionMask, sizeof(oExecutionMask));
}

void CCommand_VCS_InputsOutputs::SetDefaultParameter_GetAllDigitalInputs()
{
    WORD usInputs = 0x0000;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usInputs, sizeof(usInputs));
}

void CCommand_VCS_InputsOutputs::SetDefaultParameter_GetAllDigitalOutputs()
{
    WORD usOutputs = 0x0000;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usOutputs, sizeof(usOutputs));
}

void CCommand_VCS_InputsOutputs::SetDefaultParameter_GetAnalogInput()
{
    WORD usNumber = 0;
    WORD usAnalog = 0;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));

    //ReturnParameter
    SetReturnParameterData(0, &usAnalog, sizeof(usAnalog));
}

void CCommand_VCS_InputsOutputs::SetDefaultParameter_SetAllDigitalOutputs()
{
    WORD usOutputs = 0x0000;

    //Parameter
    SetParameterData(0, &usOutputs, sizeof(usOutputs));
}

void CCommand_VCS_InputsOutputs::SetDefaultParameter_SetAnalogOutput()
{
    WORD usNumber = 0;
    WORD usOutput = 0x0000;

    //Parameter
    SetParameterData(0, &usNumber, sizeof(usNumber));
    SetParameterData(1, &usOutput, sizeof(usOutput));
}
