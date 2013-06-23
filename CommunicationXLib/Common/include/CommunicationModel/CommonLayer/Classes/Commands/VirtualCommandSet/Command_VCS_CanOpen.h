// Command_VCS_CanOpen.h: Schnittstelle für die Klasse CCommand_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_CanOpen_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_CanOpen_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/Command_VCS.h"
#include "Command_VCS_CanOpen_Definitions.h"

class CCommand_VCS_CanOpen : public CCommand_VCS
{
public:
    CCommand_VCS_CanOpen();
    CCommand_VCS_CanOpen(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_CanOpen();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_VCS_CanOpen& operator=(CCommand_VCS_CanOpen& other);
    CCommandRoot*CloneCommand();

private:
    BOOL InitCommand_ObjectDictionary(DWORD p_ulCommandId);
	BOOL InitCommand_GeneralGateway(DWORD p_ulCommandId);
	BOOL InitCommand_LayerSettingServices(DWORD p_ulCommandId);
	    
    //ObjectDictionary
	void SetDefaultParameter_GetObject();
    void SetDefaultParameter_SetObject();
	void SetDefaultParameter_Store();
    void SetDefaultParameter_Restore();
	void SetDefaultParameter_RestoreFactory();

	//General Gateway
	void SetDefaultParameter_SendCANFrame();
    void SetDefaultParameter_RequestCANFrame();
    void SetDefaultParameter_ReadCANFrame();
	void SetDefaultParameter_SendNMTService();

    //Layer Setting Services
	void SetDefaultParameter_SendLSSFrame();
    void SetDefaultParameter_ReadLSSFrame();

};

#endif // !defined(AFX_Command_VCS_CanOpen_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
