// Command_PS_MaxonSerialV1.h: Schnittstelle f�r die Klasse CCommand_PS_MaxonSerialV1.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_PS_MaxonSerialV1_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_PS_MaxonSerialV1_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include "Command_PS_MaxonSerialV1_Definitions.h"

class CCommand_PS_MaxonSerialV1 : public CCommand_PS
{
public:
	CCommand_PS_MaxonSerialV1();
	CCommand_PS_MaxonSerialV1(DWORD dCommandId);
	virtual ~CCommand_PS_MaxonSerialV1();

	BOOL InitCommand(DWORD dCommandId);

	CCommand_PS_MaxonSerialV1& operator=(CCommand_PS_MaxonSerialV1& other);
	CCommandRoot*CloneCommand();

private:
	void SetDefaultParameter_ProcessProtocol();
	void SetDefaultParameter_SendFrame();
	void SetDefaultParameter_AbortProtocol();
};
#endif // !defined(AFX_Command_PS_MaxonSerialV1_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
