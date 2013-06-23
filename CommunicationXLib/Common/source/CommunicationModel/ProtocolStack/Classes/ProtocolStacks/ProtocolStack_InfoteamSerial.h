// ProtocolStack_InfoteamSerial.h: Schnittstelle f�r die Klasse CProtocolStack_InfoteamSerial.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ProtocolStack_InfoteamSerial_H__333868EC_E8A1_45CD_9521_6FD75C6E562B__INCLUDED_)
#define AFX_ProtocolStack_InfoteamSerial_H__333868EC_E8A1_45CD_9521_6FD75C6E562B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "BaseClasses/ProtocolStackBase.h"

class CCommand_PS_InfoteamSerial;
class CGateway;
class CJournalManagerBase;
class CInterfaceManagerBase;

class CProtocolStack_InfoteamSerial : public CProtocolStackBase
{
public:
    CProtocolStack_InfoteamSerial();
    CProtocolStack_InfoteamSerial(const CProtocolStack_InfoteamSerial& rObject);
    virtual ~CProtocolStack_InfoteamSerial();
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

    BOOL PS_ProcessProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_ProcessProtocolMaxon(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo = NULL);
	BOOL PS_AbortProtocolMaxon(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo = NULL);

    BOOL PS_SendChunk(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dPackageSize, BOOL oFirstChunk, BOOL oLastChunk, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo = NULL);
    BOOL PS_ReceiveData(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo = NULL);

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
    CCommand_PS_InfoteamSerial* m_pCommand_ProcessProtocol;
    CCommand_PS_InfoteamSerial* m_pCommand_ProcessProtocolMaxon;
	CCommand_PS_InfoteamSerial* m_pCommand_AbortProtocolMaxon;
    CCommand_PS_InfoteamSerial* m_pCommand_SendChunk;
    CCommand_PS_InfoteamSerial* m_pCommand_ReceiveData;
};


#endif // !defined(AFX_ProtocolStack_InfoteamSerial_H__333868EC_E8A1_45CD_9521_6FD75C6E562B__INCLUDED_)
