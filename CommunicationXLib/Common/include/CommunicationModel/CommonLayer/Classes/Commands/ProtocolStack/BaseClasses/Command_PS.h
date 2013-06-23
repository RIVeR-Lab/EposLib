// Command_PS.h: Schnittstelle f�r die Klasse CCommand_PS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_PS_H__09DDD854_CDDF_472A_906C_8CCF0CD3D43D__INCLUDED_)
#define AFX_COMMAND_PS_H__09DDD854_CDDF_472A_906C_8CCF0CD3D43D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>

class CCommand_PS : public CCommandRoot
{
public:
	CCommand_PS();
	virtual ~CCommand_PS();

	void InitCommand(CStdString commandName,DWORD dCommandId);
	void ResetCommand();

	CCommand_PS& operator=(CCommand_PS& other);
	CCommandRoot* CloneCommand();
};

#endif // !defined(AFX_COMMAND_PS_H__09DDD854_CDDF_472A_906C_8CCF0CD3D43D__INCLUDED_)
