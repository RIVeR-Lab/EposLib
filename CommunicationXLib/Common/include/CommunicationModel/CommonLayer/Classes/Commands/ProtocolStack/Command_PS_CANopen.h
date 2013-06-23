// Command_PS_CANopen.h: Schnittstelle f�r die Klasse CCommand_PS_CANopen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_PS_CANopen_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_PS_CANopen_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_CANopen_Definitions.h>

class CCommand_PS_CANopen : public CCommand_PS
{
public:
	CCommand_PS_CANopen();
	CCommand_PS_CANopen(DWORD dCommandId);
	virtual ~CCommand_PS_CANopen();

	BOOL InitCommand(DWORD dCommandId);

	CCommand_PS_CANopen& operator=(CCommand_PS_CANopen& other);
	CCommandRoot* CloneCommand();

private:
	void SetDefaultParameter_InitiateSDODownload();
	void SetDefaultParameter_DownloadSDOSegment();
	void SetDefaultParameter_InitiateSDOUpload();
	void SetDefaultParameter_UploadSDOSegment();
	void SetDefaultParameter_AbortSDOTransfer();
	void SetDefaultParameter_NetworkIndication();
	void SetDefaultParameter_SendNMTService();
	void SetDefaultParameter_SendCANFrame();
    void SetDefaultParameter_ReadCANFrame();
	void SetDefaultParameter_RequestCANFrame();
	void SetDefaultParameter_SendLSSFrame();
	void SetDefaultParameter_ReadLSSFrame();
};

#endif // !defined(AFX_Command_PS_CANopen_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
