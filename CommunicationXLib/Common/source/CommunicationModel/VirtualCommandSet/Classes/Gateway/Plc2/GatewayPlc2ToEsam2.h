// GatewayPlc2ToEsam2.h: Schnittstelle f�r die Klasse CGatewayPlc2ToEsam2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYVIRTUALDEVICETOEsam_H__CDA83DEC_E294_4A30_9717_F224720856F2__INCLUDED_)
#define AFX_GATEWAYVIRTUALDEVICETOEsam_H__CDA83DEC_E294_4A30_9717_F224720856F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/GatewayVCStoEsam2.h"

class CCommandRoot;
class CCommand_VCS;
class CDeviceCommandSetManagerBase;
class CLayerManagerBase;
class CVariableInfoFile;

class CGatewayPlc2ToEsam2 : public CGatewayVCStoEsam2
{
public:
    CGatewayPlc2ToEsam2();
    virtual ~CGatewayPlc2ToEsam2();
    CGateway* Clone();
    CGatewayPlc2ToEsam2& operator=(CGatewayPlc2ToEsam2& other);

    BOOL ProcessCommand(CCommandRoot*p_pCommand, CLayerManagerBase* p_pManager, HANDLE h_Handle, HANDLE p_hTransactionHandle);

private:
    //Initialisation
    BOOL Process_SetBootupBehavior(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetBootupBehavior(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Program Status
    BOOL Process_GetPlcStatus(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_ColdstartPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_WarmstartPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_HotstartPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_StopPlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_ErasePlc(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Program Variable Access
    BOOL Process_InitVariableTable(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SetVariable(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetVariable(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Process Input/Output Access
    BOOL Process_SetProcessInput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetProcessOutput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SetProcessInputBit(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetProcessOutputBit(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Process Image Access
    BOOL Process_SetProcessInputImage(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetProcessOutputImage(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //General Gateway
    BOOL Process_RequestCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SendCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_ReadCANFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SendNMTService(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Layer Setting Services
    BOOL Process_ReadLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SendLSSFrame(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Object Dictionary
    BOOL Process_GetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SetObject(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_Restore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_Store(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_RestoreFactory(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Error Handling
    BOOL Process_ResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_WaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_ClearDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetNbOfDeviceErrors(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetDeviceErrorCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GotoSafeState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Version
    BOOL Process_GetVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //ErrorHandling
    BOOL InitErrorHandling();

    //Inputs/Outputs
    BOOL Process_DigitalInputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_DigitalOutputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_AnalogInputConfiguration(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetAllDigitalInputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetAnalogInput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SetAllDigitalOutputs(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SetAnalogOutput(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    //Firmware Download
    BOOL Process_GetBaseSectorVersion(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_GetSerialNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

	//Scanning
	BOOL Process_ReadVendorId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ReadDeviceType(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ReadDeviceName(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ReadProductCode(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ReadRevisionNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ReadSerialNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

	BOOL Process_ReadNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_WriteNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ReadTransferRate(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_WriteTransferRate(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ScanStore(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ScanResetDevice(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_ScanWaitForReboot(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);

    WORD GetNewBitMask(WORD p_usConfiguration, WORD p_usOldMask, BOOL p_oState);

private:
    BOOL DeleteVariableInfoFile();
    BOOL SetBitState(BYTE* p_pData, DWORD p_ulSize, BYTE p_ubBitElement, BYTE p_ubState, CErrorInfo* p_pErrorInfo);
    BOOL GetBitState(BYTE* p_pData, DWORD p_ulSize, BYTE p_ubBitElement, BYTE& p_rubState, CErrorInfo* p_pErrorInfo);

    CVariableInfoFile *m_pVariableInfoFile;
};


#endif // !defined(AFX_GATEWAYVIRTUALDEVICETOEsam_H__CDA83DEC_E294_4A30_9717_F224720856F2__INCLUDED_)


