// GatewayPlcToEsam.h: Schnittstelle f�r die Klasse CGatewayPlcToEsam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYVIRTUALDEVICETOEsam_H__CDA83DEC_E294_4A30_9717_F224720856F2__INCLUDED_)
#define AFX_GATEWAYVIRTUALDEVICETOEsam_H__CDA83DEC_E294_4A30_9717_F224720856F2__INCLUDED_

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BaseClasses/GatewayVCStoEsam.h"

class CCommandRoot;
class CCommand_VCS;
class CDeviceCommandSetManagerBase;
class CLayerManagerBase;

class CGatewayPlcToEsam : public CGatewayVCStoEsam
{    
public:
    CGatewayPlcToEsam();
    virtual ~CGatewayPlcToEsam();
    CGateway* Clone();
    CGatewayPlcToEsam& operator=(CGatewayPlcToEsam& other);

    BOOL ProcessCommand(CCommandRoot*p_pCommand, CLayerManagerBase* p_pManager, HANDLE h_Handle, HANDLE p_hTransactionHandle);

private:
	//State Machine
    BOOL Process_ClearFault(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
	BOOL Process_SetDisableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
    BOOL Process_SetEnableState(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle);
 
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

   
private:
    BOOL InitErrorHandling();
    WORD GetNewBitMask(WORD p_usConfiguration, WORD p_usOldMask, BOOL p_oState);
};


#endif // !defined(AFX_GATEWAYVIRTUALDEVICETOEsam_H__CDA83DEC_E294_4A30_9717_F224720856F2__INCLUDED_)


