/***************************************************************************

    HEADER FILE:    Command_VCS_PLC2_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_PLC2_Definitions.h
    Date:           27.08.2009

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_PLC2_Definitions_07102003)
#define Command_VCS_PLC2_Definitions_07102003

//Command Ids
//Initialisation
    const DWORD    PLC2_SET_BOOTUP_BEHAVIOUR                        = 0x00400001;
    const DWORD    PLC2_GET_BOOTUP_BEHAVIOUR                        = 0x00400002;

//Program Status
    const DWORD    PLC2_COLDSTART_PLC                               = 0x00400021;
    const DWORD    PLC2_WARMSTART_PLC                               = 0x00400022;
    const DWORD    PLC2_HOTSTART_PLC                                = 0x00400023;
    const DWORD    PLC2_STOP_PLC                                    = 0x00400024;
	const DWORD    PLC2_ERASE_PLC                                   = 0x00400026;
    const DWORD    PLC2_GET_PLC_STATUS                              = 0x00400025;

//Program Variable Access
    const DWORD    PLC2_INIT_VARIABLE_TABLE                         = 0x00400030;
    const DWORD    PLC2_SET_VARIABLE                                = 0x00400031;
    const DWORD    PLC2_GET_VARIABLE                                = 0x00400032;

//Process Input/Output Access
    const DWORD    PLC2_SET_PROCESS_INPUT                           = 0x00400040;
    const DWORD    PLC2_GET_PROCESS_OUTPUT                          = 0x00400041;
	const DWORD    PLC2_SET_PROCESS_INPUT_BIT                       = 0x00400042;
    const DWORD    PLC2_GET_PROCESS_OUTPUT_BIT                      = 0x00400043;

//Process Image Access
    const DWORD    PLC2_SET_PROCESS_INPUT_IMAGE                     = 0x00400050;
    const DWORD    PLC2_GET_PROCESS_OUTPUT_IMAGE                    = 0x00400051;

//GeneralGateway
    const DWORD    PLC2_REQUEST_CAN_FRAME                           = 0x00400060;
    const DWORD    PLC2_SEND_CAN_FRAME                              = 0x00400061;
    const DWORD    PLC2_READ_CAN_FRAME                              = 0x00400062;
    const DWORD    PLC2_SEND_NMT_SERVICE                            = 0x00400063;

//LayerSettingServices
    const DWORD    PLC2_SEND_LSS_FRAME                              = 0x00400070;
    const DWORD    PLC2_READ_LSS_FRAME                              = 0x00400071;

//FirmwareDownload
    const DWORD    PLC2_GET_BASE_SECTOR_VERSION                     = 0x00400080;
    const DWORD    PLC2_GET_SERIAL_NUMBER                           = 0x00400081;

#endif    //Command_VCS_PLC2_Definitions_07102003

