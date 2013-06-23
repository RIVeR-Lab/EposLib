// Command_I.h: Schnittstelle f�r die Klasse CCommand_I.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_I_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_I_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>

class CCommand_I : public CCommandRoot
{
public:
	CCommand_I();
	virtual ~CCommand_I();

	void InitCommand(CStdString commandName,DWORD dCommandId);
	void ResetCommand();

	CCommand_I& operator=(CCommand_I& other);
	CCommandRoot* CloneCommand();

};

#endif // !defined(AFX_Command_I_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
