// Command_VCS_Scanning.h: Schnittstelle für die Klasse CCommand_VCS_Scanning.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_VCS_Scanning_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_VCS_Scanning_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Scanning_Definitions.h>

class CCommand_VCS_Scanning : public CCommand_VCS
{
public:
    CCommand_VCS_Scanning();
    CCommand_VCS_Scanning(DWORD p_ulCommandId);
    virtual ~CCommand_VCS_Scanning();

    BOOL InitCommand(DWORD p_ulCommandId);
    CCommand_VCS_Scanning& operator=(CCommand_VCS_Scanning& other);
    CCommandRoot*CloneCommand();

private:
    BOOL InitCommand_Identification(DWORD p_ulCommandId);
	BOOL InitCommand_Configuration(DWORD p_ulCommandId);
	    
    //Data Recorder
    void SetDefaultParameter_ReadVendorId();
    void SetDefaultParameter_ReadDeviceType();
    void SetDefaultParameter_ReadDeviceName();
    void SetDefaultParameter_ReadProductCode();
	void SetDefaultParameter_ReadRevisionNumber();
    void SetDefaultParameter_ReadSerialNumber();
    void SetDefaultParameter_ReadNodeId();
    void SetDefaultParameter_WriteNodeId();
	void SetDefaultParameter_ReadStationId();
    void SetDefaultParameter_WriteStationId();
    void SetDefaultParameter_ReadTransferRate();
    void SetDefaultParameter_WriteTransferRate();
    void SetDefaultParameter_Store();
    void SetDefaultParameter_ResetDevice();
	void SetDefaultParameter_WaitForReboot();

};

#endif // !defined(AFX_Command_VCS_Scanning_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
