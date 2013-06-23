// ProtocolStack_CANopen.h: Schnittstelle f�r die Klasse CProtocolStack_CANopen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOLSTACK_CANOPEN_H__2B7A9C3C_C0BE_463E_A886_D531EA748137__INCLUDED_)
#define AFX_PROTOCOLSTACK_CANOPEN_H__2B7A9C3C_C0BE_463E_A886_D531EA748137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/ProtocolStackBase.h"

class CCommand_PS_CANopen;
class CGateway;
class CJournalManagerBase;
class CInterfaceManagerBase;

class CProtocolStack_CANopen : public CProtocolStackBase
{
public:
    CProtocolStack_CANopen();
    CProtocolStack_CANopen(const CProtocolStack_CANopen& rObject);
    virtual ~CProtocolStack_CANopen();
    virtual CProtocolStackBase* Clone();

	//Initialisation
	BOOL InitProtocolStack(CErrorInfo* pErrorInfo = NULL);
    BOOL InitProtocolStack(CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);
	BOOL InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStack, CErrorInfo* pErrorInfo = NULL);
    BOOL InitGatewayToInterface(CStdString strInterfaceName);
	BOOL InitGatewayToDevice(CStdString strDeviceName);
    
	//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

	//Funktionalit�t
    BOOL GetCommands(CStdString* pCommandInfo);

    BOOL PS_InitiateSDODownload(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oExpeditedTransfer, BOOL oSizeIndicated, BYTE uNonValidNbOfBytes, WORD wIndex, BYTE uSubIndex, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_DownloadSDOSegment(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BYTE uNonValidNbOfBytes, BOOL oNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, BOOL* poRetToggle, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_InitiateSDOUpload(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, WORD wIndex, BYTE uSubIndex, BOOL* poExpeditedTransfer, BOOL* poSizeIndicated, BYTE* puNonValidNbOfBytes, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_UploadSDOSegment(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BOOL* poRetToggle, BYTE* puNonValidNbOfBytes, BOOL* poNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_AbortSDOTransfer(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, WORD wIndex, BYTE uSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo = NULL);
	BOOL PS_NetworkIndication(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, WORD wTargetNetworkId, BYTE uTargetNodeId, DWORD* pdAbortCode, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_SendNMTService(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uCommandSpecifier, BYTE uNodeId, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_SendCANFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_RequestCANFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_SendLSSFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_ReadLSSFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);

protected:
    BOOL CreatePlugInManager();

private:
    void DeleteCommands();
    void InitCommands();
    void InitCommands(CGateway* pGateway);
    BOOL VerifyGatewayToInterface(CStdString strInterfaceName);
	BOOL VerifyGatewayToDevice(CStdString strInterfaceName);
    BOOL InitErrorHandling();

	//ParameterSet
	BOOL InitParameterSet();

private:
    CCommand_PS_CANopen* m_pCommand_InitiateSDODownload;
    CCommand_PS_CANopen* m_pCommand_DownloadSDOSegment;
    CCommand_PS_CANopen* m_pCommand_InitiateSDOUpload;
    CCommand_PS_CANopen* m_pCommand_UploadSDOSegment;
    CCommand_PS_CANopen* m_pCommand_AbortSDOTransfer;
	CCommand_PS_CANopen* m_pCommand_NetworkIndication;
    CCommand_PS_CANopen* m_pCommand_SendNMTService;
    CCommand_PS_CANopen* m_pCommand_SendCANFrame;
    CCommand_PS_CANopen* m_pCommand_RequestCANFrame;
    CCommand_PS_CANopen* m_pCommand_SendLSSFrame;
    CCommand_PS_CANopen* m_pCommand_ReadLSSFrame;
};

#endif // !defined(AFX_PROTOCOLSTACK_CANOPEN_H__2B7A9C3C_C0BE_463E_A886_D531EA748137__INCLUDED_)
