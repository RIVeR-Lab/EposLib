// Command_VCS_Drive4.h: Schnittstelle für die Klasse CCommand_VCS_Drive4.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_Drive4_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_Drive4_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/Command_VCS.h"
#include "Command_VCS_Drive4_Definitions.h"

class CCommand_VCS_Drive4 : public CCommand_VCS
{
public:
    CCommand_VCS_Drive4();
    CCommand_VCS_Drive4(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_Drive4();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_Drive4& operator=(CCommand_VCS_Drive4& other);
    CCommandRoot*CloneCommand();

private:
	BOOL InitCommand_ErrorHandling(DWORD p_ulCommandId);
    
    // Error Handling
	void SetDefaultParameter_ResetDevice();
};

#endif // !defined(AFX_Command_VCS_Drive4_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
