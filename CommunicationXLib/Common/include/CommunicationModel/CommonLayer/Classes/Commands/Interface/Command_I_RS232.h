// Command_I_RS232.h: Schnittstelle f�r die Klasse CCommand_I_RS232.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_I_RS232_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_I_RS232_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/Command_I.h"
#include "Command_I_RS232_Definitions.h"

class CCommand_I_RS232 : public CCommand_I
{
public:
	CCommand_I_RS232();
	CCommand_I_RS232(DWORD dCommandId);
	virtual ~CCommand_I_RS232();

	BOOL InitCommand(DWORD dCommandId);

	CCommand_I_RS232& operator=(CCommand_I_RS232& other);
	CCommandRoot* CloneCommand();

private:
	void SetDefaultParameter_ReadData();
	void SetDefaultParameter_WriteData();
};

#endif // !defined(AFX_Command_I_RS232_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
