// ProtocolStack_MaxonSerialV1.h: Schnittstelle f�r die Klasse CProtocolStack_MaxonSerialV1.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ProtocolStack_MaxonSerialV1_H__333868EC_E8A1_45CD_9521_6FD75C6E562B__INCLUDED_)
#define AFX_ProtocolStack_MaxonSerialV1_H__333868EC_E8A1_45CD_9521_6FD75C6E562B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "BaseClasses/ProtocolStackBase.h"

class CCommand_PS_MaxonSerialV1;
class CGateway;
class CJournalManagerBase;
class CLayerManagerBase;
class CInterfaceManagerBase;

class CProtocolStack_MaxonSerialV1 : public CProtocolStackBase
{
public:
    CProtocolStack_MaxonSerialV1();
    CProtocolStack_MaxonSerialV1(const CProtocolStack_MaxonSerialV1& rObject);
    virtual ~CProtocolStack_MaxonSerialV1();
    virtual CProtocolStackBase* Clone();

	//Initialisation
	BOOL InitProtocolStack(CErrorInfo* pErrorInfo = NULL);
    BOOL InitProtocolStack(CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);
	BOOL InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStackName, CErrorInfo* pErrorInfo = NULL);
    BOOL InitGatewayToInterface(CStdString strInterfaceName);
	BOOL InitGatewayToDevice(CStdString strDeviceName);

	//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

    //Funktionalit�t
    BOOL GetCommands(CStdString* pCommandInfo);

    BOOL PS_ProcessProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_SendFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
	BOOL PS_AbortProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo = NULL);

protected:
    BOOL CreatePlugInManager();

private:
    void DeleteCommands();
    void InitCommands();
    void InitCommands(CGateway* pGateway);
    BOOL VerifyGatewayToInterface(CStdString strInterfaceName);
	BOOL VerifyGatewayToDevice(CStdString strDeviceName);
    BOOL InitErrorHandling();
    BOOL InitBaudrateSelection(CInterfaceManagerBase* pInterfaceManager, CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);
    BOOL InitDefaultProtocolStackSettings(CInterfaceManagerBase* pInterfaceManager, CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);

	//ParameterSet
	BOOL InitParameterSet();

private:
    CCommand_PS_MaxonSerialV1* m_pCommand_ProcessProtocol;
    CCommand_PS_MaxonSerialV1* m_pCommand_SendFrame;
	CCommand_PS_MaxonSerialV1* m_pCommand_AbortProtocol;
};


#endif // !defined(AFX_ProtocolStack_MaxonSerialV1_H__333868EC_E8A1_45CD_9521_6FD75C6E562B__INCLUDED_)
