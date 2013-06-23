// Command_VCS_CanOpen.cpp: Implementierung der Klasse CCommand_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////

CCommand_VCS_CanOpen::CCommand_VCS_CanOpen()
{
}

CCommand_VCS_CanOpen::CCommand_VCS_CanOpen(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_CanOpen::~CCommand_VCS_CanOpen()
{
}

CCommandRoot*CCommand_VCS_CanOpen::CloneCommand()
{
    CCommand_VCS_CanOpen* pNewCommand;

    pNewCommand = new CCommand_VCS_CanOpen();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_CanOpen& CCommand_VCS_CanOpen::operator=(CCommand_VCS_CanOpen& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_CanOpen::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_ObjectDictionary(p_ulCommandId)) return TRUE;
    if(InitCommand_GeneralGateway(p_ulCommandId)) return TRUE;
    if(InitCommand_LayerSettingServices(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_CanOpen::InitCommand_ObjectDictionary(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case VCS_CANOPEN_GET_OBJECT:
            {
                CCommand_VCS::InitCommand("GetObject", VCS_CANOPEN_GET_OBJECT);
                AddParameter(0, "objectIndex", ODT_UINT16);
                AddParameter(1, "objectSubIndex", ODT_UINT8);
                AddParameter(2, "nbOfBytesToRead", ODT_UINT32);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(1, "nbOfBytesRead", ODT_UINT32);
                SetDefaultParameter_GetObject();
                return TRUE;
            };
        case VCS_CANOPEN_SET_OBJECT:
            {
                CCommand_VCS::InitCommand("SetObject", VCS_CANOPEN_SET_OBJECT);
                AddParameter(0, "objectIndex", ODT_UINT16);
                AddParameter(1, "objectSubIndex", ODT_UINT8);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                AddParameter(3, "nbOfBytesToWrite", ODT_UINT32);
                AddReturnParameter(0, "nbOfBytesWritten", ODT_UINT32);
                SetDefaultParameter_SetObject();
                return TRUE;
            };
        case VCS_CANOPEN_RESTORE:
            {
                CCommand_VCS::InitCommand("Restore", VCS_CANOPEN_RESTORE);
                SetDefaultParameter_Restore();
                return TRUE;
            };
        
        case VCS_CANOPEN_STORE:
            {
                CCommand_VCS::InitCommand("Store", VCS_CANOPEN_STORE);
                SetDefaultParameter_Store();
                return TRUE;
            };

		case VCS_CANOPEN_RESTORE_FACTORY:
            {
                CCommand_VCS::InitCommand("RestoreFactory", VCS_CANOPEN_RESTORE_FACTORY);
                SetDefaultParameter_RestoreFactory();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_CanOpen::InitCommand_GeneralGateway(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case VCS_CANOPEN_REQUEST_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("RequestCANFrame", VCS_CANOPEN_REQUEST_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_RequestCANFrame();
                return TRUE;
            };
        case VCS_CANOPEN_SEND_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("SendCANFrame", VCS_CANOPEN_SEND_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_SendCANFrame();
                return TRUE;
            };
        case VCS_CANOPEN_READ_CAN_FRAME:
            {
                CCommand_VCS::InitCommand("ReadCANFrame", VCS_CANOPEN_READ_CAN_FRAME);
                AddParameter(0, "cobId", ODT_UINT16);
                AddParameter(1, "length", ODT_UINT16);
                AddParameter(2, "timeout", ODT_UINT32);
                AddReturnParameter(0, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadCANFrame();
                return TRUE;
            };
        case VCS_CANOPEN_SEND_NMT_SERVICE:
            {
                CCommand_VCS::InitCommand("SendNMTService", VCS_CANOPEN_SEND_NMT_SERVICE);
                AddParameter(0, "nodeIdentifier", ODT_UINT16);
                AddParameter(1, "commandSpecifier", ODT_UINT16);
                SetDefaultParameter_SendNMTService();
                return TRUE;
            };
    }

    return FALSE;
}

BOOL CCommand_VCS_CanOpen::InitCommand_LayerSettingServices(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case VCS_CANOPEN_SEND_LSS_FRAME:
            {
                CCommand_VCS::InitCommand("SendLSSFrame", VCS_CANOPEN_SEND_LSS_FRAME);
                AddParameter(0, "data", ODT_MEMORY_BLOCK);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                SetDefaultParameter_SendLSSFrame();
                return TRUE;
            };
        case VCS_CANOPEN_READ_LSS_FRAME:
            {
                CCommand_VCS::InitCommand("ReadLSSFrame", VCS_CANOPEN_READ_LSS_FRAME);
                AddParameter(0, "timeout", ODT_UINT16);
                AddReturnParameter(0, "errorCode", ODT_UINT32);
                AddReturnParameter(1, "data", ODT_MEMORY_BLOCK);
                SetDefaultParameter_ReadLSSFrame();
                return TRUE;
            };
    }

    return FALSE;
}

void CCommand_VCS_CanOpen::SetDefaultParameter_GetObject()
{
    WORD usObjectIndex = 0x0000;
    BYTE ubObjectSubIndex = 0x00;
    DWORD dNbOfBytesToRead = 0;
    void* pData = NULL;
    DWORD dNbOfBytesRead = 0;

    //Parameter
    SetParameterData(0, &usObjectIndex, sizeof(usObjectIndex));
    SetParameterData(1, &ubObjectSubIndex, sizeof(ubObjectSubIndex));
    SetParameterData(2, &dNbOfBytesToRead, sizeof(dNbOfBytesToRead));

    //ReturnParameter
    SetReturnParameterData(0, pData, dNbOfBytesToRead);
    SetReturnParameterData(1, &dNbOfBytesRead, sizeof(dNbOfBytesRead));
}

void CCommand_VCS_CanOpen::SetDefaultParameter_SetObject()
{
    WORD usObjectIndex = 0x0000;
    BYTE ubObjectSubIndex = 0x00;
    void* pData = NULL;
    DWORD ulNbOfBytesToWrite = 0;
    DWORD ulNbOfBytesWritten = 0;

    //Parameter
    SetParameterData(0, &usObjectIndex, sizeof(usObjectIndex));
    SetParameterData(1, &ubObjectSubIndex, sizeof(ubObjectSubIndex));
    SetParameterData(2, pData, ulNbOfBytesToWrite);
    SetParameterData(3, &ulNbOfBytesToWrite, sizeof(ulNbOfBytesToWrite));

    //ReturnParameter
    SetReturnParameterData(0, &ulNbOfBytesWritten, sizeof(ulNbOfBytesWritten));
}

void CCommand_VCS_CanOpen::SetDefaultParameter_Restore()
{
    //No Parameter
}

void CCommand_VCS_CanOpen::SetDefaultParameter_RestoreFactory()
{
    //No Parameter
}

void CCommand_VCS_CanOpen::SetDefaultParameter_Store()
{
    //No Parameter
}

void CCommand_VCS_CanOpen::SetDefaultParameter_RequestCANFrame()
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

void CCommand_VCS_CanOpen::SetDefaultParameter_SendCANFrame()
{
    WORD usCobId= 0;
    WORD usLength = 0;
    void* data = NULL;

    //Parameter
    SetParameterData(0, &usCobId, sizeof(usCobId));
    SetParameterData(1, &usLength, sizeof(usLength));
    SetParameterData(2, data, usLength);
}

void CCommand_VCS_CanOpen::SetDefaultParameter_ReadCANFrame()
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

void CCommand_VCS_CanOpen::SetDefaultParameter_SendNMTService()
{
	WORD usNodeIdentifer = 0;
    WORD usCommandSpecifier = 0x0000;

    //Parameter
	SetParameterData(0, &usNodeIdentifer, sizeof(usNodeIdentifer));
    SetParameterData(1, &usCommandSpecifier, sizeof(usCommandSpecifier));
}

void CCommand_VCS_CanOpen::SetDefaultParameter_SendLSSFrame()
{
    void* pData = NULL;
    DWORD dErrorCode = 0;

    //Parameter
    SetParameterData(0, pData, 0);

    //ReturnParameter
    SetReturnParameterData(0, &dErrorCode, sizeof(dErrorCode));
}

void CCommand_VCS_CanOpen::SetDefaultParameter_ReadLSSFrame()
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