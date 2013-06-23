// Command_VCS_Plc.cpp: Implementierung der Klasse CCommand_VCS_Plc.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////
CCommand_VCS_Plc::CCommand_VCS_Plc()
{
}

CCommand_VCS_Plc::CCommand_VCS_Plc(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_Plc::~CCommand_VCS_Plc()
{
}

CCommandRoot*CCommand_VCS_Plc::CloneCommand()
{
    CCommand_VCS_Plc* pNewCommand;

    pNewCommand = new CCommand_VCS_Plc();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_Plc& CCommand_VCS_Plc::operator=(CCommand_VCS_Plc& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}
BOOL CCommand_VCS_Plc::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

	if(InitCommand_GeneralGateway(p_ulCommandId)) return TRUE;
    if(InitCommand_LayerSettingServices(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_Plc::InitCommand_GeneralGateway(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC_REQUEST_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("RequestCANFrame", PLC_REQUEST_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_RequestCANFrame();
                return TRUE;
            };
        case PLC_SEND_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("SendCANFrame", PLC_SEND_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SendCANFrame();
                return TRUE;
            };
        case PLC_READ_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("ReadCANFrame", PLC_READ_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "timeout", ODT_UINT32);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadCANFrame();
                return TRUE;
            };
        case PLC_SEND_NMT_SERVICE:
            {
                CCommand_VCS::InitCommand("SendNMTService", PLC_SEND_NMT_SERVICE);
                AddParameter(0, "nodeIdentifier", ODT_UINT16);
                AddParameter(1, "commandSpecifier", ODT_UINT16);
                SetDefaultParameter_SendNMTService();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_Plc::InitCommand_LayerSettingServices(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case PLC_SEND_LSS_FRAME:
            {
                CCommand_VCS::InitCommand("SendLSSFrame", PLC_SEND_LSS_FRAME);
                AddParameter(0, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendLSSFrame();
                return TRUE;
            };
        case PLC_READ_LSS_FRAME:
            {
                CCommand_VCS::InitCommand("ReadLSSFrame", PLC_READ_LSS_FRAME);
                AddParameter(0, "timeout", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadLSSFrame();
                return TRUE;
            };
    }

    return FALSE;
}

void CCommand_VCS_Plc::SetDefaultParameter_RequestCANFrame()
{
    WORD usCobId= 0;
    WORD usLength = 0;
    void* data = NULL;

    //Parameter
    SetParameterData(0, &usCobId, sizeof(usCobId));
    SetParameterData(1, &usLength, sizeof(usLength));

    //ReturnParameter
    SetReturnParameterData(0, data, usLength);
}

void CCommand_VCS_Plc::SetDefaultParameter_SendCANFrame()
{
    WORD usCobId= 0;
    WORD usLength = 0;
    void* data = NULL;

    //Parameter
    SetParameterData(0, &usCobId, sizeof(usCobId));
    SetParameterData(1, &usLength, sizeof(usLength));
    SetParameterData(2, data, usLength);
}

void CCommand_VCS_Plc::SetDefaultParameter_ReadCANFrame()
{
    WORD usCobId= 0;
    WORD usLength = 0;
    DWORD ulTimeout = 0;
    void* data = NULL;

    //Parameter
    SetParameterData(0, &usCobId, sizeof(usCobId));
    SetParameterData(1, &usLength, sizeof(usLength));
    SetParameterData(2, &ulTimeout, sizeof(ulTimeout));

    //ReturnParameter
    SetReturnParameterData(0, data, usLength);
}

void CCommand_VCS_Plc::SetDefaultParameter_SendNMTService()
{
    WORD usNodeIdentifier= 0;
    WORD usCommandSpecifier = 0;

    //Parameter
	SetParameterData(0, &usNodeIdentifier, sizeof(usNodeIdentifier));
    SetParameterData(1, &usCommandSpecifier, sizeof(usCommandSpecifier));
}

void CCommand_VCS_Plc::SetDefaultParameter_SendLSSFrame()
{
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

void CCommand_VCS_Plc::SetDefaultParameter_ReadLSSFrame()
{
    WORD usTimeout = 10;
    DWORD dErrorCode = 0;
    void* pData = NULL;

    //Parameter
    SetParameterData(0, &usTimeout, sizeof(usTimeout));

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
    SetReturnParameterData(1, pData, 0);
}

