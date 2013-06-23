/***************************************************************************

	HEADER FILE:		Command_I_USB_Definitions.h

***************************************************************************

	COPYRIGHT (C)	2005 maxon motor control

	Written by:			Odermatt Roland
	System:				Visual C++
	File:				Command_I_USB_Definitions.h
	Date:				28.10.2005

****************************************************************************

	Task:

***************************************************************************/

#if !defined(Command_I_USB_Definitions_28102005)
#define Command_I_USB_Definitions_28102005

const DWORD     USB_WRITE_DATA                 = 0x00020001;
const DWORD     USB_READ_DATA                  = 0x00020002;

const DWORD     USB_EE_READ                    = 0x00020003;
const DWORD     USB_EE_PROGRAM                 = 0x00020004;

const DWORD     USB_RESCAN                     = 0x00020010;
const DWORD     USB_RELOAD                     = 0x00020011;


#endif	//Command_I_USB_Definitions_28102005

