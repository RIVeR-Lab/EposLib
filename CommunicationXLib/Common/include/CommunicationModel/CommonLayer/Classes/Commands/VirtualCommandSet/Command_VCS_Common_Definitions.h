/***************************************************************************

    HEADER FILE:    Command_VCS_Common_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_Common_Definitions.h
    Date:           25.10.2005

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_Common_Definitions_07102003)
#define Command_VCS_Common_Definitions_07102003

//Command Ids
//Error Handling
    const DWORD    COMMON_RESET_DEVICE	                             = 0x00000010;
	const DWORD    COMMON_CLEAR_DEVICE_ERRORS                        = 0x00000011;
	const DWORD    COMMON_GET_NB_OF_DEVICE_ERRORS                    = 0x00000012;
    const DWORD    COMMON_GET_DEVICE_ERROR_CODE                      = 0x00000013;
	const DWORD    COMMON_GOTO_SAFE_STATE                            = 0x00000014;
	const DWORD    COMMON_WAIT_FOR_REBOOT                            = 0x00000015;

//Version
	const DWORD    COMMON_GET_VERSION                                = 0x00000021;
	const DWORD    COMMON_GET_DEVICE_TYPE                            = 0x00000022;

//Persistence
	const DWORD    COMMON_STORE										 = 0x00000030;
	const DWORD    COMMON_RESTORE									 = 0x00000031;
	const DWORD    COMMON_RESTORE_FACTORY                            = 0x00000032;

#endif    //Command_VCS_Common_Definitions_07102003

