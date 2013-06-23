// Command_VCS_Drive4.cpp: Implementierung der Klasse CCommand_VCS_Drive4.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive4.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////
CCommand_VCS_Drive4::CCommand_VCS_Drive4()
{
}

CCommand_VCS_Drive4::CCommand_VCS_Drive4(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_Drive4::~CCommand_VCS_Drive4()
{
}

CCommandRoot*CCommand_VCS_Drive4::CloneCommand()
{
    CCommand_VCS_Drive4 *pNewCommand;

    pNewCommand = new CCommand_VCS_Drive4();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_Drive4& CCommand_VCS_Drive4::operator=(CCommand_VCS_Drive4& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_Drive4::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_ErrorHandling(p_ulCommandId)) 
    {
        return TRUE;
    }
    return FALSE;
}

BOOL CCommand_VCS_Drive4::InitCommand_ErrorHandling(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case DRIVE4_RESET_DEVICE:
            {
                CCommand_VCS::InitCommand("ResetDevice", DRIVE4_RESET_DEVICE);
                AddParameter(0, "waitForReboot", ODT_BOOLEAN);
                AddParameter(1, "rebootTimeout", ODT_BOOLEAN);
                SetDefaultParameter_ResetDevice();
                return TRUE;
            };
    }
    return FALSE;
}

void CCommand_VCS_Drive4::SetDefaultParameter_ResetDevice()
{
    BOOL oWaitForReboot(FALSE);
    DWORD ulRebootTimeout(0);

    // Parameter
    SetParameterData(0, &oWaitForReboot, sizeof(oWaitForReboot));
    SetParameterData(1, &ulRebootTimeout, sizeof(ulRebootTimeout));
}

