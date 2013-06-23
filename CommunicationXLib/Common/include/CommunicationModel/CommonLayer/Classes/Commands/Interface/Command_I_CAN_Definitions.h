/***************************************************************************

	HEADER FILE:		Command_I_CAN_Definitions.h

/***************************************************************************

	COPYRIGHT (C)	2005 maxon motor control

	Written by:			Odermatt Roland
	System:				Visual C++
	File:				Command_I_CAN_Definitions.h
	Date:				28.10.2005

****************************************************************************

	Task:

***************************************************************************/

#if !defined(Command_I_CAN_Definitions_28102005)
#define Command_I_CAN_Definitions_28102005

const DWORD CAN_TRANSMIT_CAN_FRAME				= 0x00010001;
const DWORD CAN_RECEIVE_CAN_FRAME				= 0x00010002;
const DWORD CAN_RECEIVE_FILTERED_CAN_FRAME		= 0x00010003;

const DWORD CAN_TRANSMIT_CAN_FRAME_EX			= 0x00010004;
const DWORD CAN_RECEIVE_CAN_FRAME_EX			= 0x00010005;
const DWORD CAN_RECEIVE_FILTERED_CAN_FRAME_EX	= 0x00010006;

#endif	//Command_I_CAN_Definitions_28102005

