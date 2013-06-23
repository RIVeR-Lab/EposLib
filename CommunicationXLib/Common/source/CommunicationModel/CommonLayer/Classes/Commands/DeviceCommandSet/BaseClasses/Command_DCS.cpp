// Command_DCS.cpp: Implementierung der Klasse CCommand_DCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommand_DCS::CCommand_DCS()
{
}

CCommand_DCS::~CCommand_DCS()
{
}

//******************************************************************
CCommandRoot* CCommand_DCS::CloneCommand()
{
	CCommand_DCS* pNewCommand;

	pNewCommand = new CCommand_DCS();
	*pNewCommand = *this;

	return pNewCommand;
}

//******************************************************************
CCommand_DCS& CCommand_DCS::operator=(CCommand_DCS& other)
{
	if(this != &other)
	{
		*((CCommandRoot*)this) = *((CCommandRoot*)&other);
	}

	return *this;
}
//******************************************************************
void CCommand_DCS::ResetCommand()
{
	CCommandRoot::ResetCommand();
	m_eLayer = DEVICE_COMMAND_SET_LAYER;
}

//******************************************************************
void CCommand_DCS::InitCommand(CStdString commandName,DWORD dCommandId)
{
	CCommandRoot::InitCommand(commandName,DEVICE_COMMAND_SET_LAYER,dCommandId);
}

