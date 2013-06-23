/***************************************************************************

	HEADER FILE:		Command_PS_CANopen_Definitions.h

/***************************************************************************

	COPYRIGHT (C)	2005 maxon motor control

	Written by:			Odermatt Roland
	System:				Visual C++
	File:				Command_PS_CANopen_Definitions.h
	Date:				28.10.2005

****************************************************************************

	Task:

***************************************************************************/

#if !defined(Command_PS_CANopen_Definitions_28102005)
#define Command_PS_CANopen_Definitions_28102005

//SDO Services
const DWORD CANOPEN_INITIATE_SDO_DOWNLOAD		= 0x00000001;
const DWORD CANOPEN_DOWNLOAD_SDO_SEGMENT		= 0x00000002;

const DWORD CANOPEN_INITIATE_SDO_UPLOAD			= 0x00000003;
const DWORD CANOPEN_UPLOAD_SDO_SEGMENT			= 0x00000004;

const DWORD CANOPEN_ABORT_SDO_TRANSFER			= 0x00000005;

const DWORD CANOPEN_NETWORK_INDICATION			= 0x00000006;

//NMT Services
const DWORD CANOPEN_SEND_NMT_SERVICE			= 0x00000010;

//PDO Services
const DWORD CANOPEN_SEND_CAN_FRAME				= 0x00000020;
const DWORD CANOPEN_REQUEST_CAN_FRAME			= 0x00000021;
const DWORD CANOPEN_READ_CAN_FRAME				= 0x00000022;

//LSS Services
const DWORD CANOPEN_SEND_LSS_FRAME				= 0x00000030;
const DWORD CANOPEN_READ_LSS_FRAME				= 0x00000031;


#endif	//Command_PS_CANopen_Definitions_28102005

