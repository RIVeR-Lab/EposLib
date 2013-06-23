// Command_VCS_Common.h: Schnittstelle für die Klasse CCommand_VCS_Common.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_Common_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_Common_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/Command_VCS.h"
#include "Command_VCS_Common_Definitions.h"

class CCommand_VCS_Common : public CCommand_VCS
{
public:
    CCommand_VCS_Common();
    CCommand_VCS_Common(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_Common();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_Common& operator=(CCommand_VCS_Common& other);
    CCommandRoot*CloneCommand();

private:
	BOOL InitCommand_ErrorHandling(DWORD p_ulCommandId);
    BOOL InitCommand_Version(DWORD p_ulCommandId);
	BOOL InitCommand_Persistence(DWORD p_ulCommandId);
    
    //Error Handling
	void SetDefaultParameter_ResetDevice();
	void SetDefaultParameter_WaitForReboot();
	void SetDefaultParameter_ClearDeviceErrors();
    void SetDefaultParameter_GetNbOfDeviceErrors();
    void SetDefaultParameter_GetDeviceErrorCode();
	void SetDefaultParameter_GotoSafeState();
           
	//Version
	void SetDefaultParameter_GetVersion();
	void SetDefaultParameter_GetDeviceType();

	//Persistence
	void SetDefaultParameter_Store();
    void SetDefaultParameter_Restore();
	void SetDefaultParameter_RestoreFactory();
};

#endif // !defined(AFX_Command_VCS_Common_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
