// Command_DCS_Esam2.h: Schnittstelle f�r die Klasse CCommand_DCS_Esam2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Command_DCS_Esam2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
#define AFX_Command_DCS_Esam2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>

class CCommand_DCS_Esam2 : public CCommand_DCS
{
public:
    CCommand_DCS_Esam2();
    CCommand_DCS_Esam2(DWORD p_ulCommandId);
    virtual ~CCommand_DCS_Esam2();

    BOOL InitCommand(DWORD p_ulCommandId);

    CCommand_DCS_Esam2& operator=(CCommand_DCS_Esam2& other);
    CCommandRoot*CloneCommand();

private:
    BOOL InitCommand_ObjectDictionary(DWORD p_ulCommandId);
    BOOL InitCommand_NetworkManagement(DWORD p_ulCommandId);
    BOOL InitCommand_GeneralGateway(DWORD p_ulCommandId);
    BOOL InitCommand_LayerSettingServices(DWORD p_ulCommandId);
	BOOL InitCommand_OpcsHistoryData(DWORD p_ulCommandId);

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

	//OpenPCS History Data Commands
	void SetDefaultParameter_OpcsHistGetRange();
	void SetDefaultParameter_OpcsHistGetRangeResult();
    
};

#endif // !defined(AFX_Command_DCS_Esam2_H__9B23BC0A_DD33_499E_B6F6_116AF15BE465__INCLUDED_)
