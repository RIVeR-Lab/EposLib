/***************************************************************************

    HEADER FILE:    Command_VCS_Plc_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_Plc_Definitions.h
    Date:           27.08.2009

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_Plc_Definitions_07102003)
#define Command_VCS_Plc_Definitions_07102003

//Command Ids
//GeneralGateway
    const DWORD    PLC_REQUEST_CAN_FRAME                          = 0x00300050;
    const DWORD    PLC_SEND_CAN_FRAME                             = 0x00300051;
    const DWORD    PLC_READ_CAN_FRAME                             = 0x00300052;
	const DWORD    PLC_SEND_NMT_SERVICE                           = 0x00300177;

//LayerSettingServices
    const DWORD    PLC_SEND_LSS_FRAME                             = 0x00300210;
    const DWORD    PLC_READ_LSS_FRAME                             = 0x00300211;

#endif    //Command_VCS_Plc_Definitions_07102003

