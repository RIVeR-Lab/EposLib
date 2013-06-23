/***************************************************************************

    HEADER FILE:    Command_VCS_InputsOutputs_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Adrian Ineichen
    System:         Visual C++
    File:           Command_VCS_InputsOutputs_Definitions.h
    Date:           09.12.2009

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_InputsOutputs_Definitions_09122009)
#define Command_VCS_InputsOutputs_Definitions_09122009

//Command Ids

//InputOutputs
    //General
    const DWORD    IO_DIGITAL_INPUT_CONFIGURATION                = 0x00200070;
    const DWORD    IO_DIGITAL_OUTPUT_CONFIGURATION               = 0x00200071;
    const DWORD    IO_ANALOG_INPUT_CONFIGURATION                 = 0x00200072;
    const DWORD    IO_GET_ALL_DIGITAL_INPUTS                     = 0x00200073;
    const DWORD    IO_GET_ALL_DIGITAL_OUTPUTS                    = 0x00200074;
    const DWORD    IO_GET_ANALOG_INPUT                           = 0x00200075;
    const DWORD    IO_SET_ALL_DIGITAL_OUTPUTS                    = 0x00200076;
    const DWORD    IO_SET_ANALOG_OUTPUT                          = 0x00200077;

#endif    //Command_VCS_InputsOutputs_Definitions_09122009

