// Command_DCS_Epos.h: Schnittstelle f�r die Klasse CCommand_DCS_Epos.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_DCS_Epos_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_DCS_Epos_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>

class CCommand_DCS_Epos : public CCommand_DCS
{
public:
	CCommand_DCS_Epos();
	CCommand_DCS_Epos(DWORD dCommandId);
	virtual ~CCommand_DCS_Epos();

	BOOL InitCommand(DWORD dCommandId);

	CCommand_DCS_Epos& operator=(CCommand_DCS_Epos& other);
	CCommandRoot* CloneCommand();

private:
	BOOL InitCommand_ObjectDictionary(DWORD dCommandId);
	BOOL InitCommand_NetworkManagement(DWORD dCommandId);
	BOOL InitCommand_GeneralGateway(DWORD dCommandId);
	BOOL InitCommand_LayerSettingServices(DWORD dCommandId);

	void SetDefaultParameter_WriteODObject();
	void SetDefaultParameter_WriteODObjectNextSegment();
	void SetDefaultParameter_WriteObject();
	void SetDefaultParameter_InitiateSegmentedWrite();
	void SetDefaultParameter_SegmentedWrite();
	void SetDefaultParameter_AbortSegmentedTransfer();

	void SetDefaultParameter_ReadODObject();
	void SetDefaultParameter_ReadODObjectNextSegment();
	void SetDefaultParameter_ReadObject();
	void SetDefaultParameter_InitiateSegmentedRead();
	void SetDefaultParameter_SegmentedRead();

	void SetDefaultParameter_SendNMTService();

	void SetDefaultParameter_SendCANFrame();
    void SetDefaultParameter_ReadCANFrame();
	void SetDefaultParameter_RequestCANFrame();

	void SetDefaultParameter_SendLSSFrame();
	void SetDefaultParameter_ReadLSSFrame();
};
#endif // !defined(AFX_Command_DCS_Epos_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
