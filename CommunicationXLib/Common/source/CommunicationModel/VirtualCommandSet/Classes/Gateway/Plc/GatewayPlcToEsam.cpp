// GatewayPlcToEsam.cpp: Implementierung der Klasse CGatewayPlcToEsam.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GatewayPlcToEsam.h"


#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Scanning.h>
#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenCommunicationDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenDriveDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CGatewayPlcToEsam::CGatewayPlcToEsam()
{
    InitErrorHandling();
}

CGatewayPlcToEsam::~CGatewayPlcToEsam()
{
}

CGateway* CGatewayPlcToEsam::Clone()
{
    CGatewayPlcToEsam* pClonedGateway;

    pClonedGateway = new CGatewayPlcToEsam();
    *pClonedGateway = *this;

    return (CGatewayVCStoDCS*)pClonedGateway;
}

CGatewayPlcToEsam& CGatewayPlcToEsam::operator=(CGatewayPlcToEsam& other)
{
    if(this != &other)
    {
        *((CGatewayVCStoEsam*)this) = *((CGatewayVCStoEsam*)&other);
    }

    return *this;
}

BOOL CGatewayPlcToEsam::ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE h_Handle, HANDLE p_hTransactionHandle)
{
    CCommand_VCS* pCommand_VCS = NULL;
    CDeviceCommandSetManagerBase* pDeviceCommandSetManager = NULL;
	BOOL oResult = FALSE;

    if(CGateway::ProcessCommand(p_pCommand, p_pManager, h_Handle, p_hTransactionHandle))
	{
		if(CheckLayers(p_pCommand, p_pManager))
		{
			pCommand_VCS = (CCommand_VCS*)p_pCommand;
			pDeviceCommandSetManager = (CDeviceCommandSetManagerBase*)p_pManager;

			//Lock CriticalSection
			if(Lock(p_pCommand))
			{
				//Process Command
				switch(p_pCommand->GetCommandId())
				{
					//StateMachine
					case DRIVE_CLEAR_FAULT: oResult = Process_ClearFault(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case DRIVE_SET_DISABLE_STATE: oResult = Process_SetDisableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case DRIVE_SET_ENABLE_STATE: oResult = Process_SetEnableState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

					//GeneralGateway
					case VCS_CANOPEN_REQUEST_CAN_FRAME: oResult = Process_RequestCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case VCS_CANOPEN_SEND_CAN_FRAME: oResult = Process_SendCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case VCS_CANOPEN_READ_CAN_FRAME: oResult = Process_ReadCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case VCS_CANOPEN_SEND_NMT_SERVICE: oResult = Process_SendNMTService(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

					//LayerSettingServices
					case VCS_CANOPEN_SEND_LSS_FRAME: oResult = Process_SendLSSFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case VCS_CANOPEN_READ_LSS_FRAME: oResult = Process_ReadLSSFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

					//Object Dictionary
					case VCS_CANOPEN_GET_OBJECT: oResult = Process_GetObject(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case VCS_CANOPEN_SET_OBJECT: oResult = Process_SetObject(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case VCS_CANOPEN_RESTORE: oResult = Process_Restore(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break; 
					case VCS_CANOPEN_STORE: oResult = Process_Store(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break; 
					
					//ErrorHandling
					case COMMON_RESET_DEVICE: oResult = Process_ResetDevice(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case COMMON_WAIT_FOR_REBOOT: oResult = Process_WaitForReboot(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case COMMON_CLEAR_DEVICE_ERRORS: oResult = Process_ClearDeviceErrors(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case COMMON_GET_NB_OF_DEVICE_ERRORS: oResult = Process_GetNbOfDeviceErrors(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break; 
					case COMMON_GET_DEVICE_ERROR_CODE: oResult = Process_GetDeviceErrorCode(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break; 
					case COMMON_GOTO_SAFE_STATE: oResult = Process_GotoSafeState(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break; 

					//Version
					case COMMON_GET_VERSION: oResult = Process_GetVersion(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break; 
					case COMMON_GET_DEVICE_TYPE: oResult = Process_GetDeviceType(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break; 

					//Persistence
					case COMMON_STORE: oResult = Process_Store(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case COMMON_RESTORE: oResult = Process_Restore(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

					//Scanning
					case SCAN_READ_VENDOR_ID: oResult = Process_ReadVendorId(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_READ_DEVICE_TYPE: oResult = Process_ReadDeviceType(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_READ_DEVICE_NAME: oResult = Process_ReadDeviceName(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_READ_PRODUCT_CODE: oResult = Process_ReadProductCode(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_READ_REVISION_NUMBER: oResult = Process_ReadRevisionNumber(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_READ_SERIAL_NUMBER: oResult = Process_ReadSerialNumber(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

					case SCAN_READ_NODE_ID: oResult = Process_ReadNodeId(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_WRITE_NODE_ID: oResult = Process_WriteNodeId(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_READ_TRANSFER_RATE: oResult = Process_ReadTransferRate(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_WRITE_TRANSFER_RATE: oResult = Process_WriteTransferRate(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_STORE: oResult = Process_ScanStore(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_RESET_DEVICE: oResult = Process_ScanResetDevice(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case SCAN_WAIT_FOR_REBOOT: oResult = Process_ScanWaitForReboot(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
				}

				//Unlock CriticalSection
				Unlock();
			}
		}
	}

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_RequestCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;

    WORD usCobID;
    WORD usLength;
    void* pData = NULL;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        pData = malloc(usLength);

		//Get Address Parameter
		if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //RequestCANFrame
        if(oResult) oResult = DCS_RequestCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_SendCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;

	WORD usCobID;
    WORD usLength;
    void* pData = NULL;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        pData = malloc(usLength);
        memset(pData, 0, usLength);
        p_pCommand->GetParameterData(2, pData, usLength);

		//Get Address Parameter
		if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //SendCANFrame
        if(oResult) oResult = DCS_SendCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;
    
	WORD usCobID;
    WORD usLength;
    DWORD ulTimeout;
    void* pData = NULL;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        p_pCommand->GetParameterData(2, &ulTimeout, sizeof(ulTimeout));

		//Get Address Parameter
		if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //ReadCANFrame
        pData = malloc(usLength);
        if(oResult) oResult = DCS_ReadCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usCobID, usLength, pData, ulTimeout, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE = 8;

    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;

    WORD usTimeout;
    DWORD ulErrorCode = 0;
    void* pDataBuffer = NULL;
    DWORD ulDataBufferLength;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Prepare DataBuffer
        ulDataBufferLength = MAX_FRAME_SIZE;
        pDataBuffer = malloc(ulDataBufferLength);
        memset(pDataBuffer, 0, ulDataBufferLength);

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usTimeout, sizeof(usTimeout));

		//Get Address Parameter
		if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //ReadLSSFrame
        if(oResult) oResult = DCS_ReadLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usTimeout, pDataBuffer, ulDataBufferLength, &errorInfo);
        if(!oResult) ulErrorCode = errorInfo.GetErrorCode();

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
        p_pCommand->SetReturnParameterData(1, pDataBuffer, ulDataBufferLength);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_SendLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE = 8;

    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;

    void* pDataBuffer = NULL;
    DWORD ulDataBufferLength;
    DWORD ulErrorCode = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Prepare DataBuffer
        ulDataBufferLength = p_pCommand->GetParameterLength(1);
        if(ulDataBufferLength > MAX_FRAME_SIZE) ulDataBufferLength = MAX_FRAME_SIZE;
        pDataBuffer = malloc(ulDataBufferLength);
        memset(pDataBuffer, 0, ulDataBufferLength);

        //Get Parameter Data
        p_pCommand->GetParameterData(0, pDataBuffer, ulDataBufferLength);

		//Get Address Parameter
		if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //SendLSSFrame
        if(oResult) oResult = DCS_SendLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, pDataBuffer, ulDataBufferLength, &errorInfo);
        if(!oResult) ulErrorCode = errorInfo.GetErrorCode();

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
        if(pDataBuffer) free(pDataBuffer);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_SendNMTService(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;

    WORD usNodeIdentifier;
    WORD usCommandSpecifier;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &usNodeIdentifier, sizeof(usNodeIdentifier));
        p_pCommand->GetParameterData(1, &usCommandSpecifier, sizeof(usCommandSpecifier));

		//Get Address Parameter
		if(oResult) oResult = GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, &errorInfo);

        //SendNMTService
        if(oResult) oResult = DCS_SendNMTService(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, usNodeIdentifier, usCommandSpecifier, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }
    
	return oResult;
}

BOOL CGatewayPlcToEsam::Process_GetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

    //Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    WORD usObjectIndex;
    BYTE ubObjectSubIndex;
    DWORD ulNbOfBytesToRead;
    void* pData = NULL;
    DWORD ulNbOfBytesRead = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &usObjectIndex, sizeof(usObjectIndex));
        p_pCommand->GetParameterData(1, &ubObjectSubIndex, sizeof(ubObjectSubIndex));
        p_pCommand->GetParameterData(2, &ulNbOfBytesToRead, sizeof(ulNbOfBytesToRead));
      
		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //GetObject
        pData = malloc(ulNbOfBytesToRead);
		memset(pData, 0, ulNbOfBytesToRead);

        if(oResult) oResult = ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);

        //Set Returnparameter
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, ulNbOfBytesToRead);
        p_pCommand->SetReturnParameterData(1, &ulNbOfBytesRead, sizeof(ulNbOfBytesRead));
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_GetVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    //Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = FALSE;

    WORD usSoftwareVersion = 0;
    WORD usHardwareVersion = 0;
    WORD usApplicationNumber = 0;
    WORD usApplicationVersion = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
     
		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);
		
        //Read HardwareVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_HARDWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usHardwareVersion, &errorInfo);
        }

        //Read SoftwareVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_SOFTWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usSoftwareVersion, &errorInfo);
        }

        //Read ApplicationNumber
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_APPLICATION_NUMBER;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usApplicationNumber, &errorInfo);
        }

        //Read ApplicationVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS_P_APPLICATION_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, &usApplicationVersion, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usHardwareVersion, sizeof(usHardwareVersion));
        p_pCommand->SetReturnParameterData(1, &usSoftwareVersion, sizeof(usSoftwareVersion));
        p_pCommand->SetReturnParameterData(2, &usApplicationNumber, sizeof(usApplicationNumber));
        p_pCommand->SetReturnParameterData(3, &usApplicationVersion, sizeof(usApplicationVersion));
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_SetDisableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    WORD usDeviceState(0);
    BOOL oResult = FALSE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Device State
        if(oResult) oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            switch(usDeviceState)
            {
                case DS_FAULT:
                case DS_SWITCH_ON_DISABLED: oResult = TRUE; break;
                case DS_OPERATION_ENABLED:
                case DS_QUICK_STOP_ACTIV:
                case DS_SWITCHED_ON:
                case DS_READY_TO_SWITCH_ON:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, DISABLE_VOLTAGE, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, DS_SWITCH_ON_DISABLED, &errorInfo);
                    break;
                default:
                    oResult = FALSE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ClearFault(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usIndex;
    BYTE ubSubIndex;
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        
		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Clear Fault
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, FAULT_RESET, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayPlcToEsam::Process_SetEnableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{

    WORD usIndex;
    BYTE ubSubIndex;
    WORD usDeviceState(0);
    BOOL oResult = TRUE;

    BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Get Parameter Data
        

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Device State
        if(oResult) oResult = ReadDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &usDeviceState, &errorInfo);
        if(oResult)
        {
            usIndex = INDEX_CONTROL_WORD;
            ubSubIndex = SUBINDEX_CONTROL_WORD;
            switch(usDeviceState)
            {
                case DS_OPERATION_ENABLED:
                    return TRUE;

                case DS_SWITCH_ON_DISABLED:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, SHUTDOWN, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, DS_READY_TO_SWITCH_ON, &errorInfo);

                case DS_READY_TO_SWITCH_ON:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, SWITCH_ON, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, DS_OPERATION_ENABLED, &errorInfo);

                case SWITCH_ON:

                case DS_QUICK_STOP_ACTIV:
                    if(oResult)
                        oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, ENABLE_OPERATION, &errorInfo);
                    if(oResult)
                        oResult = CheckDeviceState(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, DS_OPERATION_ENABLED, &errorInfo);
                    break;
                default:
                    oResult = FALSE;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayPlcToEsam::Process_GetDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    DWORD ulDeviceType = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Device Type
        if(oResult) oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_DEVICE_TYPE, SUBINDEX_DEVICE_TYPE, &ulDeviceType, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));
    }
    
	return oResult;
}

BOOL CGatewayPlcToEsam::Process_Restore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    //Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

	CErrorInfo errorInfo;
    BOOL oResult = FALSE;

    if(p_pCommand && p_pManager)
    {
        //Init
		oResult = TRUE;

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        if(oResult) oResult = RestoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}


BOOL CGatewayPlcToEsam::Process_SetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;
   
    WORD usObjectIndex;
    BYTE ubObjectSubIndex;
    void* pData = NULL;
    DWORD ulNbOfBytesToWrite;
    DWORD ulNbOfBytesWritten = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &usObjectIndex, sizeof(usObjectIndex));
        p_pCommand->GetParameterData(1, &ubObjectSubIndex, sizeof(ubObjectSubIndex));
        p_pCommand->GetParameterData(3, &ulNbOfBytesToWrite, sizeof(ulNbOfBytesToWrite));
        pData = malloc(ulNbOfBytesToWrite);
        p_pCommand->GetParameterData(2, pData, ulNbOfBytesToWrite);
      
		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //SetObject
        if(oResult) oResult = WriteDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNbOfBytesWritten, sizeof(ulNbOfBytesWritten));
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_Store(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Store
		if(oResult) oResult = StoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;
	BOOL oWaitForReboot(FALSE);

	BOOL oResult = FALSE;
	CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &oWaitForReboot, sizeof(oWaitForReboot));

        //SendNMTService
        if(oResult) oResult = ResetDevice(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, oWaitForReboot, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_WaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubPortNumber = 0;
	BYTE ubNodeId(0);
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //WaitForReboot
        if(oResult) oResult = WaitForReboot(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ClearDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE NB_OF_ERRORS = 0;

	BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);
        
        //Write Number of Errors in Error History
        if(oResult) oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_ERROR_HISTORY, SUBINDEX_NB_OF_ERRORS, NB_OF_ERRORS, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}
			
BOOL CGatewayPlcToEsam::Process_GetNbOfDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    BYTE ubNbOfDeviceError = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Number of Device Error
        if(oResult) oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_ERROR_HISTORY, SUBINDEX_NB_OF_ERRORS, &ubNbOfDeviceError, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ubNbOfDeviceError, sizeof(ubNbOfDeviceError));

    }
    
	return oResult;
}
	
BOOL CGatewayPlcToEsam::Process_GetDeviceErrorCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;

	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    BYTE ubErrorIndex = 0;
    DWORD ulDeviceErrorCode = 0;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &ubErrorIndex, sizeof(ubErrorIndex));

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Code from Device Error
        if(oResult) oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_ERROR_HISTORY,ubErrorIndex+1, &ulDeviceErrorCode, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
    }
        
	return oResult;
}

BOOL CGatewayPlcToEsam::Process_GotoSafeState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Parameter Data
        
		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Goto SafeState
        oResult = TRUE;

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }
    
	return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadVendorId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    DWORD ulVendorId(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_VENDOR_ID, &ulVendorId, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulVendorId, sizeof(ulVendorId));
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    DWORD ulDeviceType(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_DEVICE_TYPE, SUBINDEX_DEVICE_TYPE, &ulDeviceType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadDeviceName(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    CStdString deviceName(_T(""));
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadStringObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_DEVICE_NAME, SUBINDEX_DEVICE_NAME, &deviceName, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, deviceName.GetBuffer(), deviceName.GetLength());
		
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadProductCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    DWORD ulProductCode(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_PRODUCT_CODE, &ulProductCode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulProductCode, sizeof(ulProductCode));
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadRevisionNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    DWORD ulRevisionNumber(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_REVISION_NO, &ulRevisionNumber, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulRevisionNumber, sizeof(ulRevisionNumber));
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadSerialNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    DWORD ulSerialNumber(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_SERIAL_NO, &ulSerialNumber, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulSerialNumber, sizeof(ulSerialNumber));
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    BYTE ubNodeIdValue(0);
	DWORD ulNodeIdValue(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_NODE_ID, SUBINDEX_NODE_ID, &ubNodeIdValue, &errorInfo);
        }

		//Convert Type
		ulNodeIdValue = (DWORD)ubNodeIdValue;

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNodeIdValue, sizeof(ulNodeIdValue));
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_WriteNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
    BYTE ubNodeIdValue(0);
	DWORD ulNodeIdValue(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &ulNodeIdValue, sizeof(ulNodeIdValue));

		//Convert Type
		ubNodeIdValue = (BYTE)ulNodeIdValue;

        //Read Object
        if(oResult)
        {
            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, INDEX_NODE_ID, SUBINDEX_NODE_ID, ubNodeIdValue, &errorInfo);
        }

		//Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ReadTransferRate(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BOOL oResult(FALSE);

	//No TransferRate

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_WriteTransferRate(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BOOL oResult(FALSE);

	//No TransferRate

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ScanStore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	//Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
		oResult = TRUE;

		//Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Store
		if(oResult) oResult = StoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ScanResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
    BYTE ubNodeId(0);
	BOOL oBroadcast(FALSE);
	BOOL oWaitForReboot(FALSE);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &oBroadcast, sizeof(oBroadcast));
		p_pCommand->GetParameterData(1, &oWaitForReboot, sizeof(oWaitForReboot));

        //Reset
        if(oResult) oResult = ResetDevice(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, oWaitForReboot, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlcToEsam::Process_ScanWaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubPortNumber(0);
	BYTE ubNodeId(0);
    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, ubPortNumber, ubNodeId, &errorInfo);

        //Reset
        if(oResult) oResult = WaitForReboot(p_pManager, p_hDCS_Handle, p_hTransactionHandle, ubPortNumber, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

WORD CGatewayPlcToEsam::GetNewBitMask(WORD p_usConfiguration, WORD p_usOldMask, BOOL p_oState)
{
    WORD usNewMask;

    if(p_oState)
    {//bit set to 1
        usNewMask = 1;

        for(int i = 0; i < p_usConfiguration; i++)
            usNewMask <<= 1;

        usNewMask |= p_usOldMask;
    }
    else
    {//bit set to 0
        usNewMask = 0xFFFF;

        for(int i = 0; i < p_usConfiguration; i++)
            usNewMask <<= 1;
        usNewMask--;

        usNewMask &= p_usOldMask;
    }

    return usNewMask;
}

BOOL CGatewayPlcToEsam::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayPlcToEsam";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

