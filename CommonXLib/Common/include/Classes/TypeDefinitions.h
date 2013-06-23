////////////////////////////////////////////////////////////////////
// TypeDefinitions.h
//
// Project:             DtmEpos2.sln
// Author:              Roland Odermatt
// Date of Creation:    10.10.2007
//
// This file contains DtmEpos2 specific type definitions
//
// Copyright (C) 2007,  maxon motor ag
// All rights reserved
////////////////////////////////////////////////////////////////////

#if !defined(TypeDefinitions_10102007)
#define TypeDefinitions_10102007

#include <string>

enum EAccessType
{
    AT_READ_WRITE,
    AT_READ_WRITE_READ,
    AT_READ_WRITE_WRITE,
    AT_WRITE_ONLY,
    AT_READ_ONLY,
    AT_READ_ONLY_CONST
};

enum EConnectionMode
{
    CM_RS232    = 0,
    CM_CAN_OPEN
};

enum EObjectDataType
{
    ODT_UNKNOWN         = 0,
    ODT_BOOLEAN         = 1,
    ODT_INT8            = 2,
    ODT_INT16           = 3,
    ODT_INT32           = 4,

    ODT_UINT8           = 5,
    ODT_UINT16          = 6,
    ODT_UINT32          = 7,

    ODT_INT64           = 21,
    ODT_UINT64          = 27,

    ODT_FLOAT           = 8,
    ODT_STRING          = 9,
    ODT_DOMAIN          = 15,
    ODT_MEMORY_BLOCK    = 10
};

enum EOperationMode
{
    OM_UNKNOWN_MODE                 = 0,
    OM_PROFILE_POSITION_MODE        = 1,
    OM_PROFILE_VELOCITY_MODE        = 3,
    OM_PROFILE_TORQUE_MODE          = 4,
    OM_HOMING_MODE                  = 6,
    OM_INTERPOLATED_POSITION_MODE   = 7,
    OM_CYCLIC_SYNC_POSITION_MODE    = 8,
    OM_CYCLIC_SYNC_VELOCITY_MODE    = 9,
    OM_CYCLIC_SYNC_TORQUE_MODE      = 10,
    OM_POSITION_MODE                = -1,
    OM_VELOCITY_MODE                = -2,
    OM_CURRENT_MODE                 = -3,
    OM_DIAGNOSTIC_MODE              = -4,
    OM_MASTER_ENCODER_MODE          = -5,
    OM_STEP_DIRECTION_MODE          = -6,
    OM_POSITION_TUNING_MODE         = -11,
    OM_CURRENT_TUNING_MODE          = -12
};

//--- Data Recorder Definitions --------------------------------------
const std::string RDA_FILE_VERSION = "rdaFileVersion_1.00";

const unsigned short MAX_DATA_RECORDER_BUFFER_SIZE           = 1024;
const unsigned short MAX_NB_OF_DATA_RECORDER_VARIABLES       = 4;

struct SRecorderConfiguration
{
	unsigned short usMovementTrigger:     1;
	unsigned short usErrorTrigger:        1;
	unsigned short usDigInputTrigger:     1;
	unsigned short usEndOfProfileTrigger: 1;
	unsigned short usReserved:            11;
	unsigned short usAddressMode:         1;
};

enum ERecorderStatus
{
    RS_STOPPED          = 0x0000,
    RS_RUNNING          = 0x0001,
    RS_TRIGGERED1       = 0x0002,
    RS_TRIGGERED2       = 0x0003,
};

enum EDataRecorderTriggerType
{
    DRT_MOVEMENT_START_TRIGGER      = 1,    //bit 1
    DRT_ERROR_TRIGGER               = 2,    //bit 2
    DRT_DIGITAL_INPUT_TRIGGER       = 4,    //bit 3
    DRT_MOVEMENT_END_TRIGGER        = 8     //bit 4
};

enum EDataType
{
    DT_BYTE,
    DT_WORD,
    DT_INT,
    DT_LWORD,
    DT_LINT,
    DT_FLOAT,
    DT_MEMORY_BLOCK,
    DT_BITS8,
    DT_BITS16,
    DT_BITS32,
    DT_BOOL,
    DT_UNKNOWN
};

enum EObjectValueStringFormat
{
    OVF_UNKNOWN         = 0,
    OVF_DEC             = 1,
    OVF_HEX             = 2,
};
#endif

