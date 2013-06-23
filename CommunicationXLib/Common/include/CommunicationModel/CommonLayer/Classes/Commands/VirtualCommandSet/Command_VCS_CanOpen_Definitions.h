/***************************************************************************

    HEADER FILE:    Command_VCS_CanOpen_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_CanOpen_Definitions.h
    Date:           25.10.2005

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_CanOpen_Definitions_07102003)
#define Command_VCS_CanOpen_Definitions_07102003

#include <MmcTypeDefinition.h>

//Command Ids
//Object Dictionary
    const DWORD    VCS_CANOPEN_GET_OBJECT                           = 0x00010001;
    const DWORD    VCS_CANOPEN_SET_OBJECT                           = 0x00010002;
    const DWORD    VCS_CANOPEN_RESTORE                              = 0x00010003;
	const DWORD    VCS_CANOPEN_STORE                                = 0x00010004;
	const DWORD    VCS_CANOPEN_RESTORE_FACTORY                      = 0x00010005;

//GeneralGateway
    const DWORD    VCS_CANOPEN_REQUEST_CAN_FRAME                    = 0x00010010;
    const DWORD    VCS_CANOPEN_SEND_CAN_FRAME                       = 0x00010011;
    const DWORD    VCS_CANOPEN_READ_CAN_FRAME                       = 0x00010012;
	const DWORD    VCS_CANOPEN_SEND_NMT_SERVICE                     = 0x00010013;

//LayerSettingServices
    const DWORD    VCS_CANOPEN_SEND_LSS_FRAME                       = 0x00010020;
    const DWORD    VCS_CANOPEN_READ_LSS_FRAME                       = 0x00010021;


#endif    //Command_VCS_CanOpen_Definitions_07102003

