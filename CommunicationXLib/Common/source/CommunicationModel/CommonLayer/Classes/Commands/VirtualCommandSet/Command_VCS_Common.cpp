// Command_VCS_Common.cpp: Implementierung der Klasse CCommand_VCS_Common.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////
CCommand_VCS_Common::CCommand_VCS_Common()
{
}

CCommand_VCS_Common::CCommand_VCS_Common(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_Common::~CCommand_VCS_Common()
{
}

CCommandRoot*CCommand_VCS_Common::CloneCommand()
{
    CCommand_VCS_Common *pNewCommand;

    pNewCommand = new CCommand_VCS_Common();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_Common& CCommand_VCS_Common::operator=(CCommand_VCS_Common& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_Common::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_ErrorHandling(p_ulCommandId)) return TRUE;
    if(InitCommand_Version(p_ulCommandId)) return TRUE;
    if(InitCommand_Persistence(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_Common::InitCommand_ErrorHandling(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case COMMON_RESET_DEVICE:
            {
                CCommand_VCS::InitCommand("ResetDevice", COMMON_RESET_DEVICE);
                AddParameter(0, "waitForReboot", ODT_BOOLEAN);
                SetDefaultParameter_ResetDevice();
                return TRUE;
            };
        case COMMON_WAIT_FOR_REBOOT:
            {
                CCommand_VCS::InitCommand("WaitForReboot", COMMON_WAIT_FOR_REBOOT);
                SetDefaultParameter_WaitForReboot();
                return TRUE;
            };
        case COMMON_CLEAR_DEVICE_ERRORS:
            {
                CCommand_VCS::InitCommand("ClearDeviceErrors", COMMON_CLEAR_DEVICE_ERRORS);
                SetDefaultParameter_ClearDeviceErrors();
                return TRUE;
            };
        case COMMON_GET_NB_OF_DEVICE_ERRORS:
            {
                CCommand_VCS::InitCommand("GetNbOfDeviceErrors", COMMON_GET_NB_OF_DEVICE_ERRORS);
                AddReturnParameter(0, "nbOfDeviceError", ODT_UINT8);
                SetDefaultParameter_GetNbOfDeviceErrors();
                return TRUE;
            };
        case COMMON_GET_DEVICE_ERROR_CODE:
            {
                CCommand_VCS::InitCommand("GetDeviceErrorCode", COMMON_GET_DEVICE_ERROR_CODE);
                AddParameter(0, "errorNumber", ODT_UINT8);
                AddReturnParameter(0, "deviceErrorCode", ODT_UINT32);
                SetDefaultParameter_GetDeviceErrorCode();
                return TRUE;
            };
        case COMMON_GOTO_SAFE_STATE:
            {
                CCommand_VCS::InitCommand("GotoSafeState", COMMON_GOTO_SAFE_STATE);
                SetDefaultParameter_GotoSafeState();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Common::InitCommand_Version(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case COMMON_GET_VERSION:
            {
                CCommand_VCS::InitCommand("GetVersion", COMMON_GET_VERSION);
                AddReturnParameter(0, "hardwareVersion", ODT_UINT16);
                AddReturnParameter(1, "softwareVersion", ODT_UINT16);
                AddReturnParameter(2, "applicationNumber", ODT_UINT16);
                AddReturnParameter(3, "applicationVersion", ODT_UINT16);
                SetDefaultParameter_GetVersion();
                return TRUE;
            };

        case COMMON_GET_DEVICE_TYPE:
            {
                CCommand_VCS::InitCommand("GetDeviceType", COMMON_GET_DEVICE_TYPE);
                AddReturnParameter(0, "deviceType", ODT_UINT32);
                SetDefaultParameter_GetDeviceType();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Common::InitCommand_Persistence(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case COMMON_STORE:
            {
                CCommand_VCS::InitCommand(_T("Store"), COMMON_STORE);
                SetDefaultParameter_Store();
                return TRUE;
            };

        case COMMON_RESTORE:
            {
                CCommand_VCS::InitCommand(_T("Restore"), COMMON_RESTORE);
                SetDefaultParameter_Restore();
                return TRUE;
            };

        case COMMON_RESTORE_FACTORY:
            {
                CCommand_VCS::InitCommand(_T("RestoreFactory"), COMMON_RESTORE_FACTORY);
                SetDefaultParameter_RestoreFactory();
                return TRUE;
            };
    }

    return FALSE;
}

void CCommand_VCS_Common::SetDefaultParameter_ResetDevice()
{
    BOOL oWaitForReboot = FALSE;

    //Parameter
    SetParameterData(0, &oWaitForReboot, sizeof(oWaitForReboot));
}

void CCommand_VCS_Common::SetDefaultParameter_WaitForReboot()
{
    //No Parameter
}

void CCommand_VCS_Common::SetDefaultParameter_ClearDeviceErrors()
{
    //No Parameter
}

void CCommand_VCS_Common::SetDefaultParameter_GetNbOfDeviceErrors()
{
    BYTE ubNbDeviceErrors = 0;

    //ReturnParameter
    SetReturnParameterData(0, &ubNbDeviceErrors, sizeof(ubNbDeviceErrors));
}

void CCommand_VCS_Common::SetDefaultParameter_GetDeviceErrorCode()
{
    BYTE ubErrorIndex = 0;
    DWORD ulDeviceErrorCode = 0;

    //Parameter
    SetParameterData(0, &ubErrorIndex, sizeof(ubErrorIndex));

    //ReturnParameter
    SetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
}

void CCommand_VCS_Common::SetDefaultParameter_GotoSafeState()
{
    //No Parameter
}

void CCommand_VCS_Common::SetDefaultParameter_GetVersion()
{
    WORD usHWVersion = 0x0000;
    WORD usSWVersion = 0x0000;
    WORD usAppNumber = 0x0000;
    WORD usAppVersion = 0x0000;

    //ReturnParameter
    SetReturnParameterData(0, &usHWVersion, sizeof(usHWVersion));
    SetReturnParameterData(1, &usSWVersion, sizeof(usSWVersion));
    SetReturnParameterData(2, &usAppNumber, sizeof(usAppNumber));
    SetReturnParameterData(3, &usAppVersion, sizeof(usAppVersion));
}

void CCommand_VCS_Common::SetDefaultParameter_GetDeviceType()
{
    DWORD ulDeviceType = 0;

    //ReturnParameter
    SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));
}

void CCommand_VCS_Common::SetDefaultParameter_Restore()
{
    //No Parameter
}

void CCommand_VCS_Common::SetDefaultParameter_RestoreFactory()
{
    //No Parameter
}

void CCommand_VCS_Common::SetDefaultParameter_Store()
{
    //No Parameter
}
