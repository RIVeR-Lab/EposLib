// GatewayPlc2ToEsam2.cpp: Implementierung der Klasse CGatewayPlc2ToEsam2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayPlc2ToEsam2.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_InputsOutputs_Definitions.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Scanning.h>
#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenCommunicationDef.h>
#include "Classes/VariableInfoFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayPlc2ToEsam2::CGatewayPlc2ToEsam2()
: m_pVariableInfoFile(NULL)
{
    InitErrorHandling();

    //Create variable info file
//    if(!m_pVariableInfoFile) m_pVariableInfoFile = new CVariableInfoFile();
}

CGatewayPlc2ToEsam2::~CGatewayPlc2ToEsam2()
{
    DeleteVariableInfoFile();
}

CGateway* CGatewayPlc2ToEsam2::Clone()
{
    CGatewayPlc2ToEsam2* pClonedGateway;

    pClonedGateway = new CGatewayPlc2ToEsam2();
    *pClonedGateway = *this;

    return (CGatewayVCStoDCS*)pClonedGateway;
}

CGatewayPlc2ToEsam2& CGatewayPlc2ToEsam2::operator=(CGatewayPlc2ToEsam2& other)
{
    if(this != &other)
    {
        *((CGatewayVCStoEsam2*)this) = *((CGatewayVCStoEsam2*)&other);
    }

    return *this;
}

BOOL CGatewayPlc2ToEsam2::ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE h_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CCommand_VCS* pCommand_VCS(NULL);
    CDeviceCommandSetManagerBase* pDeviceCommandSetManager(NULL);

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
                    //General Gateway
                    case VCS_CANOPEN_REQUEST_CAN_FRAME: oResult = Process_RequestCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_SEND_CAN_FRAME: oResult = Process_SendCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_READ_CAN_FRAME: oResult = Process_ReadCANFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_SEND_NMT_SERVICE: oResult = Process_SendNMTService(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Layer Setting Services
                    case VCS_CANOPEN_SEND_LSS_FRAME: oResult = Process_SendLSSFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_READ_LSS_FRAME: oResult = Process_ReadLSSFrame(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Object Dictionary
                    case VCS_CANOPEN_GET_OBJECT: oResult = Process_GetObject(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_SET_OBJECT: oResult = Process_SetObject(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_RESTORE: oResult = Process_Restore(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case VCS_CANOPEN_STORE: oResult = Process_Store(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
					case VCS_CANOPEN_RESTORE_FACTORY: oResult = Process_RestoreFactory(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

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
                    case COMMON_RESTORE_FACTORY: oResult = Process_RestoreFactory(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Inputs and Outputs
                    case IO_DIGITAL_INPUT_CONFIGURATION: oResult = Process_DigitalInputConfiguration(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_DIGITAL_OUTPUT_CONFIGURATION: oResult = Process_DigitalOutputConfiguration(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_ANALOG_INPUT_CONFIGURATION: oResult = Process_AnalogInputConfiguration(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_GET_ALL_DIGITAL_INPUTS: oResult = Process_GetAllDigitalInputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_GET_ALL_DIGITAL_OUTPUTS: oResult = Process_GetAllDigitalOutputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_GET_ANALOG_INPUT: oResult = Process_GetAnalogInput(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_SET_ALL_DIGITAL_OUTPUTS: oResult = Process_SetAllDigitalOutputs(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case IO_SET_ANALOG_OUTPUT: oResult = Process_SetAnalogOutput(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Initialisation
                    case PLC2_SET_BOOTUP_BEHAVIOUR: oResult = Process_SetBootupBehavior(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_GET_BOOTUP_BEHAVIOUR: oResult = Process_GetBootupBehavior(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Program Status
                    case PLC2_GET_PLC_STATUS: oResult = Process_GetPlcStatus(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_COLDSTART_PLC: oResult = Process_ColdstartPlc(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_WARMSTART_PLC: oResult = Process_WarmstartPlc(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_HOTSTART_PLC: oResult = Process_HotstartPlc(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_STOP_PLC: oResult = Process_StopPlc(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_ERASE_PLC: oResult = Process_ErasePlc(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Program Variable Access
                    case PLC2_INIT_VARIABLE_TABLE: oResult = Process_InitVariableTable(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_SET_VARIABLE: oResult = Process_SetVariable(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_GET_VARIABLE: oResult = Process_GetVariable(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Process Input/Output Access
                    case PLC2_SET_PROCESS_INPUT: oResult = Process_SetProcessInput(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_GET_PROCESS_OUTPUT: oResult = Process_GetProcessOutput(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_SET_PROCESS_INPUT_BIT: oResult = Process_SetProcessInputBit(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_GET_PROCESS_OUTPUT_BIT: oResult = Process_GetProcessOutputBit(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //Process Image Access
                    case PLC2_SET_PROCESS_INPUT_IMAGE: oResult = Process_SetProcessInputImage(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_GET_PROCESS_OUTPUT_IMAGE: oResult = Process_GetProcessOutputImage(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

                    //FirmwareDownload
                    case PLC2_GET_BASE_SECTOR_VERSION: oResult = Process_GetBaseSectorVersion(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;
                    case PLC2_GET_SERIAL_NUMBER: oResult = Process_GetSerialNumber(pCommand_VCS, pDeviceCommandSetManager, h_Handle, p_hTransactionHandle); break;

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

BOOL CGatewayPlc2ToEsam2::Process_ScanWaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	WORD usNetworkId(0);
	BYTE ubNodeId(0);
    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Reset
        if(oResult) oResult = WaitForReboot(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    WORD usObjectIndex(0);
    BYTE ubObjectSubIndex(0);
    DWORD ulNbOfBytesToRead(0);
    void* pData(NULL);
    DWORD ulNbOfBytesRead(0);

    WORD usNetworkId(0);
    BYTE ubNodeId(0);

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
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //GetObject
        if(ulNbOfBytesToRead > 0)
		{
			pData = malloc(ulNbOfBytesToRead);
			memset(pData, 0, ulNbOfBytesToRead);
		}

        if(oResult) oResult = ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);

        //Set Returnparameter
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, ulNbOfBytesToRead);
        p_pCommand->SetReturnParameterData(1, &ulNbOfBytesRead, sizeof(ulNbOfBytesRead));
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usObjectIndex(0);
    BYTE ubObjectSubIndex(0);
    void* pData(NULL);
    DWORD ulNbOfBytesToWrite(0);
    DWORD ulNbOfBytesWritten(0);

    WORD usNetworkId(0);
    BYTE ubNodeId(0);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

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
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //SetObject
        if(oResult) oResult = WriteDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usObjectIndex, ubObjectSubIndex, pData, ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNbOfBytesWritten, sizeof(ulNbOfBytesWritten));
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_Restore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usNetworkId(0);
    BYTE ubNodeId(0);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand && p_pManager)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(oResult) oResult = RestoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_RestoreFactory(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usNetworkId(0);
    BYTE ubNodeId(0);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand && p_pManager)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(oResult) oResult = RestoreFactoryParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_Store(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usNetworkId(0);
    BYTE ubNodeId(0);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand && p_pManager)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

		if(oResult) oResult = StoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
	BOOL oWaitForReboot(FALSE);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &oWaitForReboot, sizeof(oWaitForReboot));

        //Reset
        if(oResult) oResult = ResetDevice(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, oWaitForReboot, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_WaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    WORD usNetworkId(0);
	BYTE ubNodeId(0);
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
		if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //WaitForReboot
        if(oResult) oResult = WaitForReboot(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ClearDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE NB_OF_ERRORS(0);

    WORD usNetworkId(0);
    BYTE ubNodeId(0);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write Number of Errors in Error History
        if(oResult) oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_ERROR_HISTORY, SUBINDEX_NB_OF_ERRORS, NB_OF_ERRORS, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetNbOfDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    BYTE ubNbOfDeviceError(0);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Number of Device Error
        if(oResult) oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_ERROR_HISTORY, SUBINDEX_NB_OF_ERRORS, &ubNbOfDeviceError, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ubNbOfDeviceError, sizeof(ubNbOfDeviceError));

    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetDeviceErrorCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BYTE ubErrorNumber(0);
    DWORD ulDeviceErrorCode(0);

    WORD usNetworkId(0);
    BYTE ubNodeId(0);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ubErrorNumber, sizeof(ubErrorNumber));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Code from Device Error
        if(oResult) oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_ERROR_HISTORY, ubErrorNumber, &ulDeviceErrorCode, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GotoSafeState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Goto SafeState
        oResult = TRUE;

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    DWORD ulDeviceType(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Device Type
        if(oResult) oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DEVICE_TYPE, SUBINDEX_DEVICE_TYPE, &ulDeviceType, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    WORD usIndex(0);
    BYTE ubSubIndex(0);

    WORD usSoftwareVersion(0);
    WORD usHardwareVersion(0);
    WORD usApplicationNumber(0);
    WORD usApplicationVersion(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read HardwareVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS2_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS2_P_HARDWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usHardwareVersion, &errorInfo);
        }

        //Read SoftwareVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS2_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS2_P_SOFTWARE_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usSoftwareVersion, &errorInfo);
        }

        //Read ApplicationNumber
        if(oResult)
        {
            usIndex = INDEX_EPOS2_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS2_P_APPLICATION_NUMBER;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usApplicationNumber, &errorInfo);
        }

        //Read ApplicationVersion
        if(oResult)
        {
            usIndex = INDEX_EPOS2_P_VERSION;
            ubSubIndex = SUBINDEX_EPOS2_P_APPLICATION_VERSION;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usApplicationVersion, &errorInfo);
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

BOOL CGatewayPlc2ToEsam2::Process_RequestCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usNetworkId(0);

    WORD usCobID(0);
    WORD usLength(0);
    void* pData(NULL);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        pData = malloc(usLength);

        //Get Address Parameter
        if(oResult) oResult = GetNetworkId(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, &errorInfo);

        //RequestCANFrame
        if(oResult) oResult = DCS_RequestCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SendCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usNetworkId(0);

    WORD usCobID(0);
    WORD usLength(0);
    void* pData(NULL);

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
        if(oResult) oResult = GetNetworkId(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, &errorInfo);

        //SendCANFrame
        if(oResult) oResult = DCS_SendCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, usCobID, usLength, pData, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usNetworkId(0);

    DWORD ulTimeout(0);
    WORD usCobID(0);
    WORD usLength(0);
    void* pData(NULL);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usCobID, sizeof(usCobID));
        p_pCommand->GetParameterData(1, &usLength, sizeof(usLength));
        p_pCommand->GetParameterData(2, &ulTimeout, sizeof(ulTimeout));

        //Get Address Parameter
        if(oResult) oResult = GetNetworkId(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, &errorInfo);

        //ReadCANFrame
        pData = malloc(usLength);
        if(oResult) oResult = DCS_ReadCANFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, usCobID, usLength, pData, ulTimeout, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, usLength);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE(8);

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usNetworkId(0);

    DWORD ulDataBufferLength;
    DWORD ulErrorCode(0);
    WORD usTimeout(0);
    void* pDataBuffer(NULL);

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
        if(oResult) oResult = GetNetworkId(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, &errorInfo);

        //ReadLSSFrame
        if(oResult) oResult = DCS_ReadLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, usTimeout, pDataBuffer, ulDataBufferLength, &errorInfo);
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

BOOL CGatewayPlc2ToEsam2::Process_SendLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_FRAME_SIZE(8);

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usNetworkId(0);

    DWORD ulDataBufferLength;
    DWORD ulErrorCode(0);
    void* pDataBuffer(NULL);

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
        if(oResult) oResult = GetNetworkId(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, &errorInfo);

        //SendLSSFrame
        if(oResult) oResult = DCS_SendLSSFrame(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, pDataBuffer, ulDataBufferLength, &errorInfo);
        if(!oResult) ulErrorCode = errorInfo.GetErrorCode();

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulErrorCode, sizeof(ulErrorCode));
        if(pDataBuffer) free(pDataBuffer);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SendNMTService(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usNetworkId(0);

    WORD usNodeIdentifier(0);
    WORD usCommandSpecifier(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usNodeIdentifier, sizeof(usNodeIdentifier));
        p_pCommand->GetParameterData(1, &usCommandSpecifier, sizeof(usCommandSpecifier));

        //Get Address Parameter
        if(oResult) oResult = GetNetworkId(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, &errorInfo);

        //SendNMTService
        if(oResult) oResult = DCS_SendNMTService(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, (BYTE)usNodeIdentifier, usCommandSpecifier, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayPlc2ToEsam2";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayPlc2ToEsam2::Process_DigitalInputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usIndex(0);
    BYTE ubSubIndex(0);
    WORD usValue(0);
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usDigitalInputNb;
    WORD usConfiguration;
    BOOL oMask;
    BOOL oPolarity;
    BOOL oExecutionMask;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usDigitalInputNb, sizeof(usDigitalInputNb));
        p_pCommand->GetParameterData(1, &usConfiguration, sizeof(usConfiguration));
        p_pCommand->GetParameterData(2, &oMask, sizeof(oMask));
        p_pCommand->GetParameterData(3, &oPolarity, sizeof(oPolarity));
        p_pCommand->GetParameterData(4, &oExecutionMask, sizeof(oExecutionMask));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write Digital Input Configuration
        usIndex = INDEX_DIGITAL_INPUT_CONFIGURATION;
        ubSubIndex = (BYTE)usDigitalInputNb;
        usValue = usConfiguration;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_INPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_INPUT_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oPolarity);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oExecutionMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_DigitalOutputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usIndex(0);
    BYTE ubSubIndex(0);
    WORD usValue(0);
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usDigitalOutputNb;
    WORD usConfiguration;
    BOOL oState;
    BOOL oMask;
    BOOL oPolarity;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usDigitalOutputNb, sizeof(usDigitalOutputNb));
        p_pCommand->GetParameterData(1, &usConfiguration, sizeof(usConfiguration));
        p_pCommand->GetParameterData(2, &oState, sizeof(oState));
        p_pCommand->GetParameterData(3, &oMask, sizeof(oMask));
        p_pCommand->GetParameterData(4, &oPolarity, sizeof(oPolarity));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write Digital Output Configuration
        usIndex = INDEX_DIGITAL_OUTPUT_CONFIGURATION;
        ubSubIndex = (BYTE)usDigitalOutputNb;
        usValue = usConfiguration;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_STATE;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oState);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        if(oResult)
        {
            usIndex = INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_DIGITAL_OUTPUT_POLARITY;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oPolarity);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_AnalogInputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    WORD usIndex(0);
    BYTE ubSubIndex(0);
    WORD usValue(0);
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usAnalogInputNb;
    WORD usConfiguration;
    BOOL oExecutionMask;
    CErrorInfo errorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usAnalogInputNb, sizeof(usAnalogInputNb));
        p_pCommand->GetParameterData(1, &usConfiguration, sizeof(usConfiguration));
        p_pCommand->GetParameterData(2, &oExecutionMask, sizeof(oExecutionMask));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write Analog Input Configuration
        usIndex = INDEX_ANALOG_INPUT_CONFIGURATION;
        ubSubIndex = (BYTE)usAnalogInputNb;
        usValue = usConfiguration;
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);

        if(oResult)
        {
            usIndex = INDEX_ANALOG_INPUT_FUNCTIONALITIES;
            ubSubIndex = SUBINDEX_ANALOG_INPUT_EXECUTION_MASK;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usValue, &errorInfo);
            if(oResult)
            {
                usValue = GetNewBitMask(usConfiguration, usValue, oExecutionMask);
                oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, usValue, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetAllDigitalInputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usInputs(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read state of all digital inputs
        if(oResult)
        {
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DIGITAL_INPUT_FUNCTIONALITIES, SUBINDEX_DIGITAL_INPUT_STATE, &usInputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usInputs, sizeof(usInputs));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);
    WORD usOutputs(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read state of all digital outputs
        if(oResult)
        {
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES, SUBINDEX_DIGITAL_OUTPUT_STATE, &usOutputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usOutputs, sizeof(usOutputs));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetAnalogInput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    WORD usIndex(0);
    BYTE ubSubIndex(0);
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usNumber;
    WORD usAnalog(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usNumber, sizeof(usNumber));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read value of one analog input
        if(oResult)
        {
            usIndex = INDEX_ANALOG_INPUTS;
            ubSubIndex = (BYTE)usNumber;
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, usIndex, ubSubIndex, &usAnalog, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usAnalog, sizeof(usAnalog));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);
    WORD usOutputs;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usOutputs, sizeof(usOutputs));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write state of all digital outputs
        if(oResult)
        {
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DIGITAL_OUTPUT_FUNCTIONALITIES, SUBINDEX_DIGITAL_OUTPUT_STATE, usOutputs, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SetAnalogOutput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);
    WORD usNumber;
    WORD usOutputValue;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usNumber, sizeof(usNumber));
        p_pCommand->GetParameterData(1, &usOutputValue, sizeof(usOutputValue));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write state of all digital outputs
        if(oResult)
        {
            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_ANALOG_OUTPUTS, SUBINDEX_ANALOG_OUTPUTS, usOutputValue, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

WORD CGatewayPlc2ToEsam2::GetNewBitMask(WORD p_usConfiguration, WORD p_usOldMask, BOOL p_oState)
{
    WORD usNewMask(0);

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

BOOL CGatewayPlc2ToEsam2::Process_SetBootupBehavior(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    WORD usProgramControl(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usProgramControl, sizeof(usProgramControl));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write program state
        if(oResult) oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_STARTUP_PROGRAM_CONTROL, SUBINDEX_STARTUP_PROGRAM_CONTROL, usProgramControl, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetBootupBehavior(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    WORD usProgramControl(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write program state
        if(oResult) oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_STARTUP_PROGRAM_CONTROL, SUBINDEX_STARTUP_PROGRAM_CONTROL, &usProgramControl, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usProgramControl, sizeof(usProgramControl));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetPlcStatus(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    BOOL oIsProgramAvailable(FALSE);
    BOOL oIsProgramRunning(FALSE);
    BYTE ubProgramState(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read program state
        if(oResult) oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROGRAM_CONTROL, SUBINDEX_PROGRAM_CONTROL, &ubProgramState, &errorInfo);

        if(oResult)
        {
            switch(ubProgramState)
            {
                case 0x00: oIsProgramRunning = FALSE; oIsProgramAvailable = TRUE; break;
                case 0x01: oIsProgramRunning = TRUE; oIsProgramAvailable = TRUE; break;
                case 0x02: oIsProgramRunning = FALSE; oIsProgramAvailable = TRUE; break;
                case 0x03: oIsProgramRunning = FALSE; oIsProgramAvailable = FALSE; break;
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &oIsProgramRunning, sizeof(oIsProgramRunning));
        p_pCommand->SetReturnParameterData(1, &oIsProgramAvailable, sizeof(oIsProgramAvailable));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ColdstartPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD TIMEOUT = 10000;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write program state
        if(oResult) oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROGRAM_CONTROL, SUBINDEX_PROGRAM_CONTROL, PLC_COLDSTART_PROGRAM, &errorInfo);

        //Wait for command termination (FW Bug Workaround)
        if(oResult) oResult = WaitForTermination(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, TIMEOUT, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_WarmstartPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD TIMEOUT = 10000;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write program state
        if(oResult) oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROGRAM_CONTROL, SUBINDEX_PROGRAM_CONTROL, PLC_WARMSTART_PROGRAM, &errorInfo);

        //Wait for command termination (FW Bug Workaround)
        if(oResult) oResult = WaitForTermination(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, TIMEOUT, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_HotstartPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD TIMEOUT = 10000;

	BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write program state
        if(oResult) oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROGRAM_CONTROL, SUBINDEX_PROGRAM_CONTROL, PLC_HOTSTART_PROGRAM, &errorInfo);

		//Wait for command termination (FW Bug Workaround)
		if(oResult) oResult = WaitForTermination(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, TIMEOUT, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_StopPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD TIMEOUT = 10000;

	BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write program state
        if(oResult) oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROGRAM_CONTROL, SUBINDEX_PROGRAM_CONTROL, PLC_STOP_PROGRAM, &errorInfo);

		//Wait for command termination (FW Bug Workaround)
		if(oResult) oResult = WaitForTermination(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, TIMEOUT, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ErasePlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD TIMEOUT = 30000;

	BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Write program state
        if(oResult) oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROGRAM_CONTROL, SUBINDEX_PROGRAM_CONTROL, PLC_CLEAR_PROGRAM, &errorInfo);

		//Wait for command termination (FW Bug Workaround)
		if(oResult) oResult = WaitForTermination(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, TIMEOUT, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_InitVariableTable(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    char *pbProjectPathName(NULL);
    WORD usLength(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        if(oResult) oResult = p_pCommand->GetParameterData(0, &usLength, sizeof(usLength));
        pbProjectPathName = (char*)(malloc(usLength));
        if(oResult) oResult = p_pCommand->GetParameterData(1, pbProjectPathName, usLength);

        //Delete variable infos
        DeleteVariableInfoFile();

        //Create variable info file
        if(!m_pVariableInfoFile) m_pVariableInfoFile = new CVariableInfoFile();

        //Load variable info file
        if(oResult) oResult = m_pVariableInfoFile->Init(pbProjectPathName);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

        //Free data
        if(pbProjectPathName) free(pbProjectPathName);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SetVariable(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    char* pVariableName(NULL);
    DWORD ulLength(0);
    DWORD ulNbOfBytesToWrite(0);
    DWORD ulNbOfBytesWritten(0);
    unsigned __int64 ullVariableSelectionAddress(0);
    void* pData(NULL);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        if(oResult) oResult = p_pCommand->GetParameterData(0, &ulLength, sizeof(ulLength));
        pVariableName = (char*)malloc(ulLength);
        if(oResult) oResult = p_pCommand->GetParameterData(1, pVariableName, ulLength);
        if(oResult) oResult = p_pCommand->GetParameterData(2, &ulNbOfBytesToWrite, sizeof(ulNbOfBytesToWrite));
        pData = malloc(ulNbOfBytesToWrite);
        if(oResult) oResult = p_pCommand->GetParameterData(3, pData, ulNbOfBytesToWrite);

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(m_pVariableInfoFile)
        {
            //Get selection address
            if(m_pVariableInfoFile->GetSelectionAddress(pVariableName, &ullVariableSelectionAddress))
            {
                //Write variable selection address
                if(oResult) oResult = WriteUInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA, SUBINDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA_ADDRESS, ullVariableSelectionAddress, &errorInfo);

                //Write variable value
                if(oResult) oResult = WriteDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA, SUBINDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA_DOMAIN, pData, ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
            }
            else
            {
                //Variable name not defined
                errorInfo.Init(k_Error_VariableNameNotFound);
                oResult = FALSE;
            }
        }
        else
        {
            //No variable file included
            errorInfo.Init(k_Error_BadVariableInfoFile);
            oResult = FALSE;
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNbOfBytesWritten, sizeof(ulNbOfBytesWritten));

        //Free buffer
        if(pVariableName) free(pVariableName);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetVariable(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    char* pVariableName(NULL);
    DWORD ulLength(0);
    DWORD ulNbOfBytesToRead(0);
    DWORD ulNbOfBytesRead(0);
    unsigned __int64 ullVariableSelectionAddress(0);
    void* pData(NULL);

    if(p_pCommand && m_pVariableInfoFile)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        if(oResult) oResult = p_pCommand->GetParameterData(0, &ulLength, sizeof(ulLength));
        pVariableName = (char*)malloc(ulLength);
        if(oResult) oResult = p_pCommand->GetParameterData(1, pVariableName, ulLength);
        if(oResult) oResult = p_pCommand->GetParameterData(2, &ulNbOfBytesToRead, sizeof(ulNbOfBytesToRead));
        pData = malloc(ulNbOfBytesToRead);
        if(oResult) oResult = p_pCommand->GetParameterData(2, pData, ulNbOfBytesToRead);

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(m_pVariableInfoFile)
        {
            //Get selection address
            if(m_pVariableInfoFile->GetSelectionAddress(pVariableName, &ullVariableSelectionAddress))
            {
                //Read variable selection address
                if(oResult) oResult = ReadUInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA, SUBINDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA_ADDRESS, &ullVariableSelectionAddress, &errorInfo);

                //Read variable value
                if(oResult) oResult = ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA, SUBINDEX_SERVICE_OBJECT_APPLICATION_PROGRAM_DATA_DOMAIN, pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
            }
            else
            {
                //Variable name not defined
                errorInfo.Init(k_Error_VariableNameNotFound);
                oResult = FALSE;
            }
        }
        else
        {
            //No variable file included
            errorInfo.Init(k_Error_BadVariableInfoFile);
            oResult = FALSE;
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, ulNbOfBytesToRead);
        p_pCommand->SetReturnParameterData(1, &ulNbOfBytesRead, sizeof(ulNbOfBytesRead));

        //Free buffer
        if(pVariableName) free(pVariableName);
        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SetProcessInput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    BYTE ubElementNumber(0);
    DWORD ulNbOfBytesToWrite(0);
    DWORD ulNbOfBytesWritten(0);
    void* pData(NULL);
    WORD usProcessInputType(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usProcessInputType, sizeof(usProcessInputType));
        p_pCommand->GetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
        p_pCommand->GetParameterData(2, &ulNbOfBytesToWrite, sizeof(ulNbOfBytesToWrite));
        pData = malloc(ulNbOfBytesToWrite);
        p_pCommand->GetParameterData(3, pData, ulNbOfBytesToWrite);

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(oResult)
        {
            if(ubElementNumber > 0)
            {
                if(PIT_PROCESS_INPUT_INT8 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT8, ubElementNumber, (BYTE*)&(*(__int8*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
                else if(PIT_PROCESS_INPUT_INT16 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT16, ubElementNumber, (BYTE*)&(*(short*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
                else if(PIT_PROCESS_INPUT_INT32 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT32, ubElementNumber, (BYTE*)&(*(long*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
                else if(PIT_PROCESS_INPUT_INT64 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT64, ubElementNumber, (BYTE*)&(*(__int64*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
                else if(PIT_PROCESS_INPUT_UINT8 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT8, ubElementNumber, (BYTE*)&(*(BYTE*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
                else if(PIT_PROCESS_INPUT_UINT16 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT16, ubElementNumber, (BYTE*)&(*(WORD*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
                else if(PIT_PROCESS_INPUT_UINT32 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT32, ubElementNumber, (BYTE*)&(*(DWORD*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
                else if(PIT_PROCESS_INPUT_UINT64 == usProcessInputType)
                {
                    oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT64, ubElementNumber, (BYTE*)&(*(unsigned __int64*)pData), ulNbOfBytesToWrite, &ulNbOfBytesWritten, &errorInfo);
                }
            }
            else
            {
                oResult = FALSE;
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNbOfBytesWritten, sizeof(ulNbOfBytesWritten));

        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetProcessOutput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    BYTE ubElementNumber(0);
    DWORD ulNbOfBytesToRead(0);
    DWORD ulNbOfBytesRead(0);
    void* pData(NULL);
    WORD usProcessOutputType(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usProcessOutputType, sizeof(usProcessOutputType));
        p_pCommand->GetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
        p_pCommand->GetParameterData(2, &ulNbOfBytesToRead, sizeof(ulNbOfBytesToRead));
        pData = malloc(ulNbOfBytesToRead);

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(oResult)
        {
            if(ubElementNumber > 0)
            {
                if(PIT_PROCESS_OUTPUT_INT8 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT8, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
                else if(PIT_PROCESS_OUTPUT_INT16 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT16, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
                else if(PIT_PROCESS_OUTPUT_INT32 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT32, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
                else if(PIT_PROCESS_OUTPUT_INT64 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT64, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
                else if(PIT_PROCESS_OUTPUT_UINT8 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT8, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
                else if(PIT_PROCESS_OUTPUT_UINT16 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT16, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
                else if(PIT_PROCESS_OUTPUT_UINT32 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT32, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
                else if(PIT_PROCESS_OUTPUT_UINT64 == usProcessOutputType)
                {
                    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT64, ubElementNumber, (BYTE*&)pData, ulNbOfBytesToRead, &ulNbOfBytesRead, &errorInfo);
                }
            }
            else
            {
                oResult = FALSE;
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNbOfBytesRead, sizeof(ulNbOfBytesRead));
        p_pCommand->SetReturnParameterData(1, pData, ulNbOfBytesToRead);

        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SetProcessInputBit(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    WORD usProcessInputType(0);
    BYTE ubElementNumber(0);
    BYTE ubBitNumber(0);
    BYTE ubBitState(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usProcessInputType, sizeof(usProcessInputType));
        p_pCommand->GetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
        p_pCommand->GetParameterData(2, &ubBitNumber, sizeof(ubBitNumber));
        p_pCommand->GetParameterData(3, &ubBitState, sizeof(ubBitState));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(oResult)
        {
            if(ubElementNumber > 0)
            {
                if(PIT_PROCESS_INPUT_INT8 == usProcessInputType)
                {
                    char bData = 0;

                    if(ReadInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT8, ubElementNumber, (char*)&bData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&bData, sizeof(bData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT8, ubElementNumber, (char)bData, &errorInfo);
                        }
                    }
                }
                else if(PIT_PROCESS_INPUT_INT16 == usProcessInputType)
                {
                    short sData = 0;

                    if(ReadInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT16, ubElementNumber, (short*)&sData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&sData, sizeof(sData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT16, ubElementNumber, (short)sData, &errorInfo);
                        }
                    }
                }
                else if(PIT_PROCESS_INPUT_INT32 == usProcessInputType)
                {
                    long lData = 0;

                    if(ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT32, ubElementNumber, (long*)&lData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&lData, sizeof(lData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT32, ubElementNumber, (long)lData, &errorInfo);
                        }
                    }
                }
                else if(PIT_PROCESS_INPUT_INT64 == usProcessInputType)
                {
                    __int64 llData = 0;

                    if(ReadInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT64, ubElementNumber, (__int64*)&llData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&llData, sizeof(llData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_INT64, ubElementNumber, (__int64)llData, &errorInfo);
                        }
                    }
                }
                else if(PIT_PROCESS_INPUT_UINT8 == usProcessInputType)
                {
                    BYTE ubData = 0;

                    if(ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT8, ubElementNumber, (BYTE*)&ubData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&ubData, sizeof(ubData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT8, ubElementNumber, (BYTE)ubData, &errorInfo);
                        }
                    }
                }
                else if(PIT_PROCESS_INPUT_UINT16 == usProcessInputType)
                {
                    WORD usData = 0;

                    if(ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT16, ubElementNumber, (WORD*)&usData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&usData, sizeof(usData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT16, ubElementNumber, (WORD)usData, &errorInfo);
                        }
                    }
                }
                else if(PIT_PROCESS_INPUT_UINT32 == usProcessInputType)
                {
                    DWORD ulData = 0;

                    if(ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT32, ubElementNumber, (DWORD*)&ulData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&ulData, sizeof(ulData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT32, ubElementNumber, (DWORD)ulData, &errorInfo);
                        }
                    }
                }
                else if(PIT_PROCESS_INPUT_UINT64 == usProcessInputType)
                {
                    unsigned __int64 ullData = 0;

                    if(ReadUInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT64, ubElementNumber, (unsigned __int64*)&ullData, &errorInfo))
                    {
                        if(SetBitState((BYTE*)&ullData, sizeof(ullData), ubBitNumber, ubBitState, &errorInfo))
                        {
                            oResult = WriteUInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_INPUT_UINT64, ubElementNumber, (unsigned __int64)ullData, &errorInfo);
                        }
                    }
                }
            }
            else
            {
                oResult = FALSE;
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);

    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetProcessOutputBit(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    WORD usProcessOutputType(0);
    BYTE ubElementNumber(0);
    BYTE ubBitNumber(0);
    BYTE ubBitState(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &usProcessOutputType, sizeof(usProcessOutputType));
        p_pCommand->GetParameterData(1, &ubElementNumber, sizeof(ubElementNumber));
        p_pCommand->GetParameterData(2, &ubBitNumber, sizeof(ubBitNumber));

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        if(oResult)
        {
            if(ubElementNumber > 0)
            {
                if(PIT_PROCESS_OUTPUT_INT8 == usProcessOutputType)
                {
                    char bData = 0;

                    if(ReadInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT8, ubElementNumber, (char*)&bData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&bData, sizeof(bData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
                else if(PIT_PROCESS_OUTPUT_INT16 == usProcessOutputType)
                {
                    short sData = 0;

                    if(ReadInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT16, ubElementNumber, (short*)&sData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&sData, sizeof(sData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
                else if(PIT_PROCESS_OUTPUT_INT32 == usProcessOutputType)
                {
                    long lData = 0;

                    if(ReadInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT32, ubElementNumber, (long*)&lData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&lData, sizeof(lData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
                else if(PIT_PROCESS_OUTPUT_INT64 == usProcessOutputType)
                {
                    __int64 llData = 0;

                    if(ReadInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_INT64, ubElementNumber, (__int64*)&llData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&llData, sizeof(llData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
                else if(PIT_PROCESS_OUTPUT_UINT8 == usProcessOutputType)
                {
                    BYTE ubData = 0;

                    if(ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT8, ubElementNumber, (BYTE*)&ubData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&ubData, sizeof(ubData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
                else if(PIT_PROCESS_OUTPUT_UINT16 == usProcessOutputType)
                {
                    WORD usData = 0;

                    if(ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT16, ubElementNumber, (WORD*)&usData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&usData, sizeof(usData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
                else if(PIT_PROCESS_OUTPUT_UINT32 == usProcessOutputType)
                {
                    DWORD ulData = 0;

                    if(ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT32, ubElementNumber, (DWORD*)&ulData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&ulData, sizeof(ulData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
                else if(PIT_PROCESS_OUTPUT_UINT64 == usProcessOutputType)
                {
                    unsigned __int64 ullData = 0;

                    if(ReadUInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_OUTPUT_UINT64, ubElementNumber, (unsigned __int64*)&ullData, &errorInfo))
                    {
                        oResult = GetBitState((BYTE*)&ullData, sizeof(ullData), ubBitNumber, ubBitState, &errorInfo);
                    }
                }
            }
            else
            {
                oResult = FALSE;
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, &errorInfo);
            }
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ubBitState, sizeof(ubBitState));

    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_SetProcessInputImage(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    DWORD ulSelectionStart(0);
    DWORD ulSelectionLength(0);
    DWORD ulSelectionLengthWritten(0);
    DWORD ulSelectedRange(0);
    void* pData(NULL);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ulSelectionStart, sizeof(ulSelectionStart));
        p_pCommand->GetParameterData(1, &ulSelectionLength, sizeof(ulSelectionLength));
        pData = malloc(ulSelectionLength);
        p_pCommand->GetParameterData(2, pData, ulSelectionLength);

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        ulSelectedRange = ulSelectionLength - ulSelectionStart;

        //Write image selected range
        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_IMAGE, SUBINDEX_PROCESS_IMAGE_RANGE_INPUTS, ulSelectedRange, &errorInfo);

        //Write variable image
        if(oResult) oResult = WriteDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_IMAGE, SUBINDEX_PROCESS_IMAGE_DOMAIN, pData, ulSelectionLength, &ulSelectionLengthWritten, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulSelectionLengthWritten, sizeof(ulSelectionLengthWritten));

        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_GetProcessOutputImage(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    DWORD ulSelectionStart(0);
    DWORD ulSelectionLength(0);
    DWORD ulSelectionLengthRead(0);
    DWORD ulSelectedRange(0);
    void* pData(NULL);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Parameter Data
        p_pCommand->GetParameterData(0, &ulSelectionStart, sizeof(ulSelectionStart));
        p_pCommand->GetParameterData(1, &ulSelectionLength, sizeof(ulSelectionLength));
        pData = malloc(ulSelectionLength);

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        ulSelectedRange = ulSelectionLength - ulSelectionStart;

        //Write variable selected range
        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_IMAGE, SUBINDEX_PROCESS_IMAGE_RANGE_OUTPUTS, ulSelectedRange, &errorInfo);

        //Read variable image
        if(oResult) oResult = ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_PROCESS_IMAGE, SUBINDEX_PROCESS_IMAGE_DOMAIN, pData, ulSelectionLength, &ulSelectionLengthRead, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, pData, ulSelectionLength);
        p_pCommand->SetReturnParameterData(1, &ulSelectionLengthRead, sizeof(ulSelectionLengthRead));

        if(pData) free(pData);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::DeleteVariableInfoFile()
{
    if(m_pVariableInfoFile)
    {
        m_pVariableInfoFile->DeleteVariableList();

        delete m_pVariableInfoFile;
        m_pVariableInfoFile = NULL;

        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayPlc2ToEsam2::Process_GetBaseSectorVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    WORD usBaseSectorVersion(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read target velocity
        if(oResult)
        {
            oResult = ReadUInt16Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_EPOS2_P_VERSION, SUBINDEX_EPOS2_P_BASE_VERSION, &usBaseSectorVersion, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &usBaseSectorVersion, sizeof(usBaseSectorVersion));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayPlc2ToEsam2::Process_GetSerialNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CErrorInfo errorInfo;

    BYTE ubNodeId(0);
    WORD usNetworkId(0);

    unsigned __int64 llSerialNumber(0);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read target velocity
        if(oResult)
        {
            oResult = ReadUInt64Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_SERIAL_NUMBER, SUBINDEX_SERIAL_NUMBER, &llSerialNumber, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &llSerialNumber, sizeof(llSerialNumber));

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadVendorId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    DWORD ulVendorId(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_VENDOR_ID, &ulVendorId, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulVendorId, sizeof(ulVendorId));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    DWORD ulDeviceType(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DEVICE_TYPE, SUBINDEX_DEVICE_TYPE, &ulDeviceType, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulDeviceType, sizeof(ulDeviceType));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadDeviceName(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    CStdString deviceName(_T(""));
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadStringObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DEVICE_NAME, SUBINDEX_DEVICE_NAME, &deviceName, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, deviceName);
		
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadProductCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    DWORD ulProductCode(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_PRODUCT_CODE, &ulProductCode, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulProductCode, sizeof(ulProductCode));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadRevisionNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    DWORD ulRevisionNumber(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_REVISION_NO, &ulRevisionNumber, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulRevisionNumber, sizeof(ulRevisionNumber));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadSerialNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    DWORD ulSerialNumber(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt32Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_DEVICE_IDENTIFICATION, SUBINDEX_SERIAL_NO, &ulSerialNumber, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulSerialNumber, sizeof(ulSerialNumber));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    BYTE ubNodeIdValue(0);
	DWORD ulNodeIdValue(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
            oResult = ReadUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_NODE_ID, SUBINDEX_NODE_ID, &ubNodeIdValue, &errorInfo);
        }

		//Convert Type
		ulNodeIdValue = (DWORD)ubNodeIdValue;

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulNodeIdValue, sizeof(ulNodeIdValue));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_WriteNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
    BYTE ubNodeIdValue(0);
	DWORD ulNodeIdValue(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &ulNodeIdValue, sizeof(ulNodeIdValue));

		//Convert Type
		ubNodeIdValue = (BYTE)ulNodeIdValue;

        //Read Object
        if(oResult)
        {
            oResult = WriteUInt8Object(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, INDEX_NODE_ID, SUBINDEX_NODE_ID, ubNodeIdValue, &errorInfo);
        }

		//Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ReadTransferRate(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
	DWORD ulTransferRate(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

        //Read Object
        if(oResult)
        {
			oResult = GetDeviceTransferRate(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, ulTransferRate, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
        p_pCommand->SetReturnParameterData(0, &ulTransferRate, sizeof(ulTransferRate));
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_WriteTransferRate(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	BYTE ubNodeId(0);
    WORD usNetworkId(0);
	DWORD ulTransferRate(0);
	CErrorInfo errorInfo;
	BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &ulTransferRate, sizeof(ulTransferRate));

        //Read Object
        if(oResult)
        {
			oResult = SetDeviceTransferRate(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, ulTransferRate, &errorInfo);
        }

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ScanStore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	WORD usNetworkId(0);
    BYTE ubNodeId(0);

    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(p_pCommand && p_pManager)
    {
        //Init
        oResult = TRUE;

        //Get Address Parameter
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

		if(oResult) oResult = StoreParameter(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::Process_ScanResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle)
{
	WORD usNetworkId(0);
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
        if(oResult) oResult = GetAddressParameter(p_pCommand, p_pManager, p_hDCS_Handle, usNetworkId, ubNodeId, &errorInfo);

		//Get Parameter Data
        p_pCommand->GetParameterData(0, &oBroadcast, sizeof(oBroadcast));
		p_pCommand->GetParameterData(1, &oWaitForReboot, sizeof(oWaitForReboot));

        //Reset
        if(oResult) oResult = ResetDevice(p_pManager, p_hDCS_Handle, p_hTransactionHandle, usNetworkId, ubNodeId, oWaitForReboot, &errorInfo);

        //Set ReturnParameter Data
        p_pCommand->SetStatus(oResult, &errorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::SetBitState(BYTE* p_pData, DWORD p_ulSize, BYTE p_ubBitElement, BYTE p_ubState, CErrorInfo* p_pErrorInfo)
{
    const DWORD BITS_PER_BYTE = 8;

    unsigned __int64 ullValue = 0;
    unsigned __int64 ullMask = 0;
    BOOL oResult = FALSE;

    if(p_pData && p_ulSize > 0)
    {
        if((p_ulSize * BITS_PER_BYTE) > p_ubBitElement)
        {
            //Initialisation
            memcpy(&ullValue, p_pData, p_ulSize);

            ullMask = 1 << p_ubBitElement;

            //Set State
            if(p_ubState > 0) ullValue |= ullMask; else ullValue &= ~ullMask;

            //Set Data
            memcpy(p_pData, &ullValue, p_ulSize);
            oResult = TRUE;
        }
        else
        {
            //Bit Element too high
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, p_pErrorInfo);
        }
    }
    else
    {
        //Bad Container Variable
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayPlc2ToEsam2::GetBitState(BYTE* p_pData, DWORD p_ulSize, BYTE p_ubBitElement, BYTE& p_rubState, CErrorInfo* p_pErrorInfo)
{
    const DWORD BITS_PER_BYTE = 8;

    unsigned __int64 ullValue = 0;
    unsigned __int64 ullMask = 0;
    BOOL oResult = FALSE;

    if(p_pData && p_ulSize > 0)
    {
        if((p_ulSize * BITS_PER_BYTE) > p_ubBitElement)
        {
            //Initialisation
            memcpy(&ullValue, p_pData, p_ulSize);

            ullMask = 1 << p_ubBitElement;

            //Get State
            if((ullValue & ullMask) > 0) p_rubState = 1; else p_rubState = 0;
            oResult = TRUE;
        }
        else
        {
            //Bit Element too high
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, p_pErrorInfo);
        }
    }
    else
    {
        //Bad Container Variable
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, p_pErrorInfo);
    }

    return oResult;
}

