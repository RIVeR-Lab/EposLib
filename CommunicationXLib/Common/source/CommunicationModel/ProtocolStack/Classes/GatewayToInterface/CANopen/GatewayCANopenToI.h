// GatewayCANopenToI.h: Schnittstelle f�r die Klasse CGatewayCANopenToI.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayCANopenToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
#define AFX_GatewayCANopenToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include "../BaseClasses/GatewayPStoI.h"
class CCommand_I_CAN;
class CCANopenFrame;

class CGatewayCANopenToI : public CGatewayPStoI
{
public:
    CGatewayCANopenToI();
    virtual ~CGatewayCANopenToI();

    virtual CGateway* Clone();
    CGatewayCANopenToI& operator=(CGatewayCANopenToI& other);

    BOOL ProcessCommand(CCommandRoot*pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

private:
    void InitCommands();
    void DeleteCommands();

    BOOL Process_InitiateSDODownload(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_DownloadSDOSegment(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_InitiateSDOUpload(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_UploadSDOSegment(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
	BOOL Process_NetworkIndication(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_AbortSDOTransfer(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendNMTService(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendCANFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadCANFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_RequestCANFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_SendLSSFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);
    BOOL Process_ReadLSSFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle);

    BOOL SendFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, CCANopenFrame* pFrame, CErrorInfo* pErrorInfo = NULL);
    BOOL ReceiveFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, CCANopenFrame* pFrame, DWORD dTimeout, CErrorInfo* pErrorInfo = NULL);

    BOOL CheckReceivedFrame(CCANopenFrame* pSendingFrame, CCANopenFrame* pReceivingFrame);

    BOOL I_TransmitCanFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, BOOL oRtr, BYTE uDlc, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL I_ReceiveCanFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD* pdCobId, BOOL* poRtr, BYTE* puDlc, void* pDataBuffer, DWORD dDataBufferLength, DWORD dTimeout, CErrorInfo* pErrorInfo = NULL);
    BOOL I_ReceiveFilteredCanFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdFilter, BOOL oRtrFilter, DWORD* pdCobId, BOOL* poRtr, BYTE* puDlc, void* pDataBuffer, DWORD dDataBufferLength, DWORD dTimeout, CErrorInfo* pErrorInfo = NULL);

	//Layer Parameter Stack
	BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

	//NetworkIndication
	BOOL SetNetworkIndication();
	BOOL ResetNetworkIndication();
	BOOL IsNetworkIndicationActive();
	BOOL IsNetworkIndicationThread();

private:
    CCANopenFrame* m_pSendingFrame;
    CCANopenFrame* m_pReceivingFrame;

    CCommand_I_CAN* m_pCommand_TransmitCanFrame;
    CCommand_I_CAN* m_pCommand_ReceiveCanFrame;
    CCommand_I_CAN* m_pCommand_ReceiveFilteredCanFrame;

	//NetworkIndication
	DWORD m_ulNetworkIndicationThreadId;
};

#endif // !defined(AFX_GatewayCANopenToI_H__89B9836A_BF97_42DF_8370_3E0AF46F87FE__INCLUDED_)
