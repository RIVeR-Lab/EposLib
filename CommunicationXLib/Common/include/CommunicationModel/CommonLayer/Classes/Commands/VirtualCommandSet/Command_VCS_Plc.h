// Command_VCS_Plc.h: Schnittstelle f�r die Klasse CCommand_VCS_Plc.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_Plc_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_Plc_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include "Command_VCS_Plc_Definitions.h"

class CCommand_VCS_Plc : public CCommand_VCS
{
public:
    CCommand_VCS_Plc();
    CCommand_VCS_Plc(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_Plc();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_Plc& operator=(CCommand_VCS_Plc& other);
    CCommandRoot*CloneCommand();

private:
	BOOL InitCommand_GeneralGateway(DWORD p_ulCommandId);
    BOOL InitCommand_LayerSettingServices(DWORD p_ulCommandId);
        
    //General Gateway
	void SetDefaultParameter_SendCANFrame();
    void SetDefaultParameter_RequestCANFrame();
    void SetDefaultParameter_ReadCANFrame();
	void SetDefaultParameter_SendNMTService();

	//Layer Setting Services
    void SetDefaultParameter_SendLSSFrame();
    void SetDefaultParameter_ReadLSSFrame();
 
};

#endif // !defined(AFX_Command_VCS_Plc_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
