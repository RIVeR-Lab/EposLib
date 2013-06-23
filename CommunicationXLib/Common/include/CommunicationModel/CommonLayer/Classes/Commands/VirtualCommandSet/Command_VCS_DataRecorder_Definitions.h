/***************************************************************************

    HEADER FILE:    Command_VCS_DataRecorder_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2005 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_DataRecorder_Definitions.h
    Date:           25.10.2005

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_DataRecorder_Definitions_07102003)
#define Command_VCS_DataRecorder_Definitions_07102003

//Command Ids
//DataRecorder Setup
    const DWORD    REC_SET_RECORDER_PARAMETER                     = 0x00020000;
    const DWORD    REC_GET_RECORDER_PARAMETER                     = 0x00020001;
    const DWORD    REC_ENABLE_TRIGGER                             = 0x00020002;
    const DWORD    REC_DISABLE_ALL_TRIGGERS                       = 0x00020003;
    const DWORD    REC_ACTIVATE_CHANNEL                           = 0x00020004;
    const DWORD    REC_DEACTIVATE_ALL_CHANNELS                    = 0x00020005;

//DataRecorder Status
    const DWORD    REC_START_RECORDER                             = 0x00020010;
    const DWORD    REC_STOP_RECORDER                              = 0x00020011;
    const DWORD    REC_FORCE_TRIGGER                              = 0x00020012;
    const DWORD    REC_IS_RECORDER_RUNNING                        = 0x00020013;
    const DWORD    REC_IS_RECORDER_TRIGGERED                      = 0x00020014;

//DataRecorder Data
    const DWORD    REC_READ_CHANNEL_VECTOR_SIZE                   = 0x00020020;
    const DWORD    REC_READ_CHANNEL_DATA_VECTOR                   = 0x00020021;

//DataRecorder Advanced
    const DWORD    REC_READ_DATA_BUFFER                           = 0x00020030;


#endif    //Command_VCS_DataRecorder_Definitions_07102003

