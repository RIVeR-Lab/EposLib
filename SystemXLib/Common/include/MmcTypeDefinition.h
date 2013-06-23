#ifndef ___TYPE_DEFINITIONS_
#define ___TYPE_DEFINITIONS_

#include <Thread/MmcCriticalSection.h>
#include <StdString.h>
#include <vector>
#include <list>

#ifdef _UNICODE
	typedef std::wiostream tMmcIoStream;
#else
	typedef std::iostream tMmcIoStream;
#endif

#ifndef WINVER

#define _NO_WINDOWS
#define USE_CURRENT_STL_IMPL

#include <string>
#include <Thread/MmcThread.h>
#include <limits.h>

#ifndef _tcslen
#define _tcslen strlen
#endif

#ifndef _stprintf
#define _stprintf sprintf
#endif

#ifndef _tcstoul
#define _tcstoul strtoul
#endif

#ifndef _tcstol
#define _tcstol strtol
#endif

#ifndef _tstof
#define _tstof atof
#endif

#ifndef _tcstok
#define _tcstok strtok
#endif //_tcstok

#define MB_OK				0x00000000L
#define MB_ICONINFORMATION	0x00000040L
#define _I64_MAX			LLONG_MAX /* maximum signed 64 bit value */
#define _I64_MIN			LLONG_MIN /* minimum signed 64 bit value */
#define _UI64_MAX			ULLONG_MAX /* maximum unsigned 64 bit value */
#define _I8_MAX				CHAR_MAX
#define _I8_MIN				CHAR_MIN

#ifndef BOOL
typedef int			BOOL;
#endif

#ifndef HANDLE
typedef void					*HANDLE;
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef DWORD
typedef unsigned int DWORD;
#endif

#ifndef DWORD_PTR
typedef DWORD DWORD_PTR;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef IJournal
#define IJournal void*
#endif

#ifndef T_JournalCallback
#define T_JournalCallback void*
#endif

#ifndef CArchive
class CArchive {};
#endif

#ifndef CStdStringArray
typedef std::vector<std::string> CStdStringArray;
#endif

#ifndef CStdDWordArray
typedef std::vector<DWORD> CStdDWordArray;
#endif

#ifndef CStdByteArray
typedef std::vector<unsigned char> CStdByteArray;
#endif

#ifndef _T
#define _T(x) x
#endif

#ifndef CObject
class CObject {};
#endif

#ifndef WINAPI
#define WINAPI
#endif

#ifndef __stdcall
#define __stdcall
#endif

#ifndef PVOID
typedef void					*PVOID;
#endif

#ifndef LPVOID
typedef PVOID					LPVOID;
#endif

#ifndef __int8
#define __int8 char
#endif

#ifndef __int16
#define __int16 short
#endif

#ifndef __int32
#define __int32 int
#endif

#ifndef __int64
#define __int64 long long
#endif

#ifndef ULONG_MAX
#define ULONG_MAX -1
#endif

#ifndef USHRT_MAX
#define USHRT_MAX (unsigned short)(-1)
#endif

#ifndef UINT_MAX
#define UINT_MAX (unsigned int)(-1)
#endif

#ifndef LONG
typedef unsigned int			LONG;
#endif

#ifndef LONGLONG
#define LONGLONG long long
#endif

#ifndef LONG_MAX
#define LONG_MAX 2147483647
#endif

#define IDS_COMM_X_ERROR_INTERNAL 0
#define IDS_COMM_X_ERROR_NULL_POINTER 0
#define IDS_COMM_X_ERROR_HANDLE_NOT_VALID 0
#define IDS_COMM_X_ERROR_BAD_VIRTUAL_DEVICE_NAME 0
#define IDS_COMM_X_ERROR_BAD_DEVICE_NAME 0
#define IDS_COMM_X_ERROR_BAD_PROTOCOL_STACK_NAME 0
#define IDS_COMM_X_ERROR_BAD_INTERFACE_NAME 0
#define IDS_COMM_X_ERROR_BAD_PORT_NAME 0
#define IDS_COMM_X_ERROR_LIBRARY_NOT_FOUND 0
#define IDS_COMM_X_ERROR_EXEC_COMMAND 0
#define IDS_COMM_X_ERROR_TIMEOUT 0
#define IDS_COMM_X_ERROR_BAD_PARAM 0
#define IDS_COMM_X_ERROR_COMMAND_ABORTED 0
#define IDS_COMM_X_ERROR_BUFFER_TOO_SMALL 0
#define IDS_COMM_X_ERROR_NO_COMM_FOUND 0
#define IDS_COMM_X_ERROR_FUNCTION_NOT_SUPPORTED 0
#define IDS_COMM_X_ERROR_PARAM_USED 0
#define IDS_COMM_X_ERROR_BAD_VIRTUAL_DEVICE_HANDLE 0
#define IDS_COMM_X_ERROR_BAD_DEVICE_HANDLE 0
#define IDS_COMM_X_ERROR_BAD_PROTOCOLSTACK_HANDLE 0
#define IDS_COMM_X_ERROR_BAD_INTERFACE_HANDLE 0
#define IDS_COMM_X_ERROR_BAD_PORT_HANDLE 0
#define IDS_COMM_X_ERROR_BAD_ADDRESS_PARAM 0
#define IDS_COMM_X_ERROR_BAD_VARIABLE_INFO_FILE 0
#define IDS_COMM_X_ERROR_VARIABLE_NAME_NOT_FOUND 0
#define IDS_COMM_X_ERROR_BAD_CONTROLLER_STATE 0
#define IDS_COMM_X_ERROR_BAD_FILE_CONTENT 0
#define IDS_COMM_X_ERROR_HID_WRITE_DATA	0
#define IDS_COMM_X_ERROR_HID_READ_DATA 0
#define IDS_COMM_X_ERROR_MAXON_SERIAL_V2_STUFFING 0
#define IDS_COMM_X_ERROR_MAXON_SERIAL_V2_DESTUFFING 0
#define IDS_COMM_X_ERROR_MAXON_SERIAL_V2_BAD_CRC_RECEIVED 0
#define IDS_COMM_X_ERROR_MAXON_SERIAL_V2_BAD_SIZE_RECEIVED 0
#define IDS_COMM_X_ERROR_MAXON_SERIAL_V2_BAD_SIZE_WRITTEN 0
#define IDS_COMM_X_ERROR_MAXON_SERIAL_V2_SEND_FRAME 0
#define IDS_COMM_X_ERROR_MAXON_SERIAL_V2_RECEIVE_FRAME 0
#define IDS_COMM_X_ERROR_VIRTUAL_DEVICE_BAD_SIZE_EXPECTED 0
#define IDS_COMM_X_ERROR_VIRTUAL_DEVICE_HOMING_ERROR 0
#define IDS_COMM_X_ERROR_VIRTUAL_DEVICE_BAD_SII_CATEGORY 0
#define IDS_COMM_X_ERROR_VIRTUAL_DEVICE_BAD_SII_STRING_TABLE 0

#else
	//WINDOWS
	#define CStdStringArray std::vector<CStdString>
	#define CStdWordArray std::vector<unsigned short>
	#define CStdDWordArray std::vector<unsigned long>
	#define CStdByteArray std::vector<unsigned char>
	#ifdef _MFC_VER
		typedef CCriticalSection CMmcCriticalSection;
		typedef CSingleLock CMmcSingleLock;
	#else
		class CMmcCriticalSection;
		class CMmcSingleLock;
	#endif
#endif //_WIN32

#endif //___TYPE_DEFINITIONS_
