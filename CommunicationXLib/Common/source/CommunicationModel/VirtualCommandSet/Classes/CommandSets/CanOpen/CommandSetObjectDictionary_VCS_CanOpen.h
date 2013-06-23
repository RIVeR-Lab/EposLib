// CommandSetObjectDictionary_VCS_CanOpen.h: Schnittstelle f�r die Klasse CCommandSetObjectDictionary_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDSETUTILITIESS_VCS_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
#define AFX_CommandSetObjectDictionary_VCS_CanOpen_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_VCS_CanOpen;
class CLayerManagerBase;

class CCommandSetObjectDictionary_VCS_CanOpen : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void* p_pData, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void* p_pData, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CErrorInfo* p_pErrorInfo = NULL);
	BOOL VCS_Store(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
	BOOL VCS_Restore(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetObjectDictionary_VCS_CanOpen();
    virtual ~CCommandSetObjectDictionary_VCS_CanOpen();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_CanOpen* m_pCommand_GetObject;
    CCommand_VCS_CanOpen* m_pCommand_SetObject;
    CCommand_VCS_CanOpen* m_pCommand_Store;
	CCommand_VCS_CanOpen* m_pCommand_Restore;
    

};

#endif // !defined(AFX_CommandSetObjectDictionary_VCS_CanOpen_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
