// Command_PS.cpp: Implementierung der Klasse CCommand_PS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommand_PS::CCommand_PS()
{
	ResetCommand();
}

CCommand_PS::~CCommand_PS()
{
}

//******************************************************************
CCommandRoot* CCommand_PS::CloneCommand()
{
	CCommand_PS* pNewCommand;

	pNewCommand = new CCommand_PS();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_PS& CCommand_PS::operator=(CCommand_PS& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}

//******************************************************************
void CCommand_PS::ResetCommand()
{
	CCommandRoot::ResetCommand();
	m_eLayer = PROTOCOL_STACK_LAYER;
}

//******************************************************************
void CCommand_PS::InitCommand(CStdString commandName,DWORD dCommandId)
{
	CCommandRoot::InitCommand(commandName,PROTOCOL_STACK_LAYER,dCommandId);
}
