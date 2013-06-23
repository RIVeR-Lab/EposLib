// CommandSetProcessImageAccess_VCS_Plc2.h: Schnittstelle f�r die Klasse CCommandSetProcessImageAccess_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Plc2;
class CLayerManagerBase;

class CCommandSetProcessImageAccess_VCS_Plc2 : public CCommandSetBase_VCS
{
public:

    CCommandSetProcessImageAccess_VCS_Plc2();
    virtual ~CCommandSetProcessImageAccess_VCS_Plc2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

    BOOL VCS_SetProcessInputImage(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulSelectionStart, DWORD p_ulSelectionLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo);
    BOOL VCS_GetProcessOutputImage(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulSelectionStart, DWORD p_ulSelectionLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Plc2* m_pCommand_GetProcessOutputImage;
    CCommand_VCS_Plc2* m_pCommand_SetProcessInputImage;
};

