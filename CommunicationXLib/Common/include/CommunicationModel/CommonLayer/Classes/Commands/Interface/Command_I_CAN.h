// Command_I_CAN.h: Schnittstelle für die Klasse CCommand_I_CAN.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_I_CAN_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_I_CAN_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/BaseClasses/Command_I.h>
#include "Command_I_CAN_Definitions.h"

class CCommand_I_CAN : public CCommand_I
{
public:
    CCommand_I_CAN();
    CCommand_I_CAN(DWORD p_ulCommandId);
    virtual ~CCommand_I_CAN();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_I_CAN& operator=(CCommand_I_CAN& p_rOther);
    CCommandRoot* CloneCommand();

private:
    void SetDefaultParameter_TransmitCanFrame();
    void SetDefaultParameter_ReceiveFilteredCanFrame();
    void SetDefaultParameter_ReceiveCanFrame();
};

#endif // !defined(AFX_Command_I_CAN_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
