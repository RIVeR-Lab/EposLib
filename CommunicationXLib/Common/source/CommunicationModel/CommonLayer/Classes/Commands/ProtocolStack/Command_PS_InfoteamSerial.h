// Command_PS_InfoteamSerial.h: Schnittstelle f�r die Klasse CCommand_PS_InfoteamSerial.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_PS_InfoteamSerial_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_PS_InfoteamSerial_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include "Command_PS_InfoteamSerial_Definitions.h"

class CCommand_PS_InfoteamSerial : public CCommand_PS
{
public:
	CCommand_PS_InfoteamSerial();
	CCommand_PS_InfoteamSerial(DWORD dCommandId);
	virtual ~CCommand_PS_InfoteamSerial();

	BOOL InitCommand(DWORD dCommandId);

	CCommand_PS_InfoteamSerial& operator=(CCommand_PS_InfoteamSerial& other);
	CCommandRoot*CloneCommand();

private:
	void SetDefaultParameter_ProcessProtocol();
	void SetDefaultParameter_ProcessProtocolMaxon();
	void SetDefaultParameter_AbortProtocolMaxon();
	void SetDefaultParameter_SendChunk();
	void SetDefaultParameter_ReceiveData();
};

#endif // !defined(AFX_Command_PS_InfoteamSerial_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
