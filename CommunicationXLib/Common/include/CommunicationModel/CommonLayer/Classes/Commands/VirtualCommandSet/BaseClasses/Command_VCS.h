// Command_VCS.h: Schnittstelle f�r die Klasse CCommand_VCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_H__1FDA715C_31EA_453B_A400_47A2BC0E2173__INCLUDED_)
#define AFX_Command_VCS_H__1FDA715C_31EA_453B_A400_47A2BC0E2173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>

class CCommand_VCS: public CCommandRoot
{
public:
	CCommand_VCS();
	virtual ~CCommand_VCS();

	CCommand_VCS& operator=(CCommand_VCS& other);
	CCommandRoot* CloneCommand();

	void InitCommand(CStdString commandName,DWORD dCommandId);
	void ResetCommand();
};

#endif // !defined(AFX_Command_VCS_H__1FDA715C_31EA_453B_A400_47A2BC0E2173__INCLUDED_)
