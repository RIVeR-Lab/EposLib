// Command_VCS_InputsOutputs.h: Schnittstelle f�r die Klasse CCommand_VCS_InputsOutputs.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_VCS_INPUTSOUTPUTS_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_COMMAND_VCS_INPUTSOUTPUTS_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include "Command_VCS_InputsOutputs_Definitions.h"

class CCommand_VCS_InputsOutputs : public CCommand_VCS
{
public:
    CCommand_VCS_InputsOutputs();
    CCommand_VCS_InputsOutputs(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_InputsOutputs();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_InputsOutputs& operator=(CCommand_VCS_InputsOutputs& other);
    CCommandRoot*CloneCommand();

private:
    BOOL InitCommand_InputsOutputs(DWORD p_ulCommandId);

    void SetDefaultParameter_DigitalInputConfiguration();
    void SetDefaultParameter_DigitalOutputConfiguration();
    void SetDefaultParameter_AnalogInputConfiguration();
    void SetDefaultParameter_GetAllDigitalInputs();
    void SetDefaultParameter_GetAllDigitalOutputs();
    void SetDefaultParameter_GetAnalogInput();
    void SetDefaultParameter_SetAllDigitalOutputs();
    void SetDefaultParameter_SetAnalogOutput();
};

#endif // !defined(AFX_COMMAND_VCS_INPUTSOUTPUTS_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
