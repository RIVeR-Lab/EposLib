// Command_I_USB.h: Schnittstelle f�r die Klasse CCommand_I_USB.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_I_USB_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_I_USB_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/Command_I.h"
#include "Command_I_USB_Definitions.h"

class CCommand_I_USB : public CCommand_I
{
public:
	CCommand_I_USB();
	CCommand_I_USB(DWORD dCommandId);
	virtual ~CCommand_I_USB();

	BOOL InitCommand(DWORD dCommandId);

	CCommand_I_USB& operator=(CCommand_I_USB& other);
	CCommandRoot* CloneCommand();

private:
	void SetDefaultParameter_ReadData();
	void SetDefaultParameter_WriteData();
	void SetDefaultParameter_EE_Read();
	void SetDefaultParameter_EE_Program();
	void SetDefaultParameter_Rescan();
	void SetDefaultParameter_Reload();

};

#endif // !defined(AFX_Command_I_USB_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
