// Command_DCS_Esam.h: Schnittstelle f�r die Klasse CCommand_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_DCS_Esam_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_DCS_Esam_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>

class CCommand_DCS_Esam : public CCommand_DCS
{
public:
	CCommand_DCS_Esam();
	CCommand_DCS_Esam(DWORD dCommandId);
	virtual ~CCommand_DCS_Esam();

	BOOL InitCommand(DWORD dCommandId);

	CCommand_DCS_Esam& operator=(CCommand_DCS_Esam& other);
	CCommandRoot* CloneCommand();

private:
	BOOL InitCommand_ObjectDictionary(DWORD dCommandId);
	BOOL InitCommand_NetworkManagement(DWORD dCommandId);
	BOOL InitCommand_GeneralGateway(DWORD dCommandId);
	BOOL InitCommand_LayerSettingServices(DWORD dCommandId);
	BOOL InitCommand_DeviceControl(DWORD dCommandId);

	//ObjectDictionary
	void SetDefaultParameter_WriteObject();
	void SetDefaultParameter_InitiateSegmentedWrite();
	void SetDefaultParameter_SegmentedWrite();
    void SetDefaultParameter_ReadObject();
	void SetDefaultParameter_InitiateSegmentedRead();
	void SetDefaultParameter_SegmentedRead();
	void SetDefaultParameter_AbortSegmentedTransfer();

	//Network Management
	void SetDefaultParameter_SendNMTService();

	//General Gateway
	void SetDefaultParameter_SendCANFrame();
    void SetDefaultParameter_ReadCANFrame();
	void SetDefaultParameter_RequestCANFrame();

	//Layer Setting Services
	void SetDefaultParameter_SendLSSFrame();
	void SetDefaultParameter_ReadLSSFrame();

	//Device Control
	void SetDefaultParameter_Login();
	void SetDefaultParameter_Logout();
	void SetDefaultParameter_SetState();

};

#endif // !defined(AFX_Command_DCS_Esam_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
