/***************************************************************************

    HEADER FILE:    Command_VCS_Scanning_Definitions.h

****************************************************************************

    COPYRIGHT (C)   2010 maxon motor control

    Written by:     Roland Odermatt
    System:         Visual C++
    File:           Command_VCS_Scanning_Definitions.h
    Date:           25.07.2010

****************************************************************************

    Task:

***************************************************************************/

#if !defined(Command_VCS_Scanning_Definitions_07102003)
#define Command_VCS_Scanning_Definitions_07102003

//Command Ids
//Scanning Objects
    const DWORD    SCAN_READ_VENDOR_ID		                     = 0x00030001;
    const DWORD    SCAN_READ_DEVICE_TYPE	                     = 0x00030002;
    const DWORD    SCAN_READ_DEVICE_NAME                         = 0x00030003;
    const DWORD    SCAN_READ_PRODUCT_CODE                        = 0x00030004;
    const DWORD    SCAN_READ_REVISION_NUMBER                     = 0x00030005;
    const DWORD    SCAN_READ_SERIAL_NUMBER                       = 0x00030006;

//Scanning Configuration
    const DWORD    SCAN_READ_NODE_ID                             = 0x00030010;
    const DWORD    SCAN_WRITE_NODE_ID                            = 0x00030011;
	const DWORD    SCAN_READ_STATION_ID                          = 0x00030017;
    const DWORD    SCAN_WRITE_STATION_ID                         = 0x00030018;
	
	const DWORD    SCAN_READ_TRANSFER_RATE                       = 0x00030012;
    const DWORD    SCAN_WRITE_TRANSFER_RATE                      = 0x00030013;
	const DWORD    SCAN_STORE                                    = 0x00030014;
	const DWORD    SCAN_RESET_DEVICE                             = 0x00030015;
	const DWORD    SCAN_WAIT_FOR_REBOOT                          = 0x00030016;

	

#endif    //Command_VCS_Scanning_Definitions_07102003

