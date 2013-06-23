// Command_VCS.cpp: Implementierung der Klasse CCommand_VCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommand_VCS::CCommand_VCS()
{
}

CCommand_VCS::~CCommand_VCS()
{
}

//********************************************************************
CCommandRoot* CCommand_VCS::CloneCommand()
{
	CCommand_VCS* pNewCommand;

	pNewCommand = new CCommand_VCS();
	*pNewCommand = *this;

	return pNewCommand;
}

//********************************************************************
CCommand_VCS& CCommand_VCS::operator=(CCommand_VCS& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}

//******************************************************************
void CCommand_VCS::ResetCommand()
{
	CCommandRoot::ResetCommand();
	m_eLayer = VIRTUAL_COMMAND_SET_LAYER;
}

//******************************************************************
void CCommand_VCS::InitCommand(CStdString commandName,DWORD dCommandId)
{
	CCommandRoot::InitCommand(commandName,VIRTUAL_COMMAND_SET_LAYER,dCommandId);
}
