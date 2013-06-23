// Command_I.cpp: Implementierung der Klasse CCommand_I.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/BaseClasses/Command_I.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0/////////////////////////////////////////////
CCommand_I::CCommand_I()
{
	ResetCommand();
}

CCommand_I::~CCommand_I()
{
}

//******************************************************************
void CCommand_I::ResetCommand()
{
	CCommandRoot::ResetCommand();
	m_eLayer = INTERFACE_LAYER;
}

//******************************************************************
void CCommand_I::InitCommand(CStdString commandName,DWORD dCommandId)
{
	CCommandRoot::InitCommand(commandName,INTERFACE_LAYER,dCommandId);
}

//******************************************************************
CCommandRoot* CCommand_I::CloneCommand()
{
	CCommand_I* pNewCommand;

	pNewCommand = new CCommand_I();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_I& CCommand_I::operator=(CCommand_I& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}
