/***************************************************************************

	HEADER FILE:		Command_PS_InfoteamSerial_Definitions.h

/***************************************************************************

	COPYRIGHT (C)	2005 maxon motor control

	Written by:			Odermatt Roland
	System:				Visual C++
	File:				Command_PS_InfoteamSerial_Definitions.h
	Date:				28.10.2005

****************************************************************************

	Task:

***************************************************************************/

#if !defined(Command_PS_InfoteamSerial_Definitions_28102005)
#define Command_PS_InfoteamSerial_Definitions_28102005

//EsamRS232 Protocol
const DWORD INFOTEAM_SERIAL_PROCESS_PROTOCOL			= 0x00020001;
const DWORD INFOTEAM_SERIAL_PROCESS_PROTOCOL_MAXON		= 0x00020002;
const DWORD INFOTEAM_SERIAL_ABORT_PROTOCOL_MAXON		= 0x00020003;
const DWORD INFOTEAM_SERIAL_SEND_CHUNK					= 0x00020004;
const DWORD INFOTEAM_SERIAL_RECEIVE_DATA				= 0x00020005;

//LastChunkFlag
#define k_NormalChunk		0x00
#define k_FirstChunk		0xF0
#define k_LastChunk			0x0F
#define k_FirstAndLastChunk 0xFF

//ChunkSize
#define k_MaxChunkSize		132

//PackageType
#define k_PT_MAXON_CMD		0xF1
#define k_PT_MAXON_REC		0xF3

#define k_PT_INFOTEAM_SYNC	0x00
#define k_PT_INFOTEAM_CMD	0x01
#define k_PT_INFOTEAM_DATA	0x02
#define k_PT_INFOTEAM_REC	0x03

#endif	//Command_PS_InfoteamSerial_Definitions_28102005

