// Command_DCS.h: Schnittstelle f�r die Klasse CCommand_DCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMAND_DCS_H__09DDD854_CDDF_472A_906C_8CCF0CD3D43D__INCLUDED_)
#define AFX_COMMAND_DCS_H__09DDD854_CDDF_472A_906C_8CCF0CD3D43D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>

class CCommand_DCS: public CCommandRoot
{
public:
	CCommand_DCS();
	virtual ~CCommand_DCS();

	CCommand_DCS& operator=(CCommand_DCS& other);
	CCommandRoot* CloneCommand();

	void InitCommand(CStdString commandName,DWORD dCommandId);
	void ResetCommand();
};

#endif // !defined(AFX_COMMAND_DCS_H__09DDD854_CDDF_472A_906C_8CCF0CD3D43D__INCLUDED_)
