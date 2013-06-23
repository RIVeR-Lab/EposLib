// CommandSetProcessInputOutputAccess_VCS_Plc2.h: Schnittstelle f�r die Klasse CCommandSetProcessInputOutputAccess_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Plc2;
class CLayerManagerBase;

class CCommandSetProcessInputOutputAccess_VCS_Plc2 : public CCommandSetBase_VCS
{
public:

    CCommandSetProcessInputOutputAccess_VCS_Plc2();
    virtual ~CCommandSetProcessInputOutputAccess_VCS_Plc2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

    BOOL VCS_SetProcessInput(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessInputType, BYTE p_ubElementNumber, void* p_pDataBuffer, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CErrorInfo* p_pErrorInfo);
    BOOL VCS_GetProcessOutput(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessOutputType, BYTE p_ubElementNumber, void* p_pDataBuffer, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CErrorInfo* p_pErrorInfo);

	BOOL VCS_SetProcessInputBit(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessInputType, BYTE p_ubElementNumber, BYTE p_ubBitNumber, BYTE p_ubBitState, CErrorInfo* p_pErrorInfo);
    BOOL VCS_GetProcessOutputBit(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProcessOutputType, BYTE p_ubElementNumber, BYTE p_ubBitNumber, BYTE* p_pubBitState, CErrorInfo* p_pErrorInfo);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Plc2* m_pCommand_GetProcessOutput;
    CCommand_VCS_Plc2* m_pCommand_SetProcessInput;
	CCommand_VCS_Plc2* m_pCommand_GetProcessOutputBit;
    CCommand_VCS_Plc2* m_pCommand_SetProcessInputBit;
};

