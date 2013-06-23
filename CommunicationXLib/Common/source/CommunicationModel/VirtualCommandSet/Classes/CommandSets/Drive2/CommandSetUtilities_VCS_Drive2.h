// CommandSetUtilities_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetUtilities_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDSETUTILITIESS_VCS_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
#define AFX_CommandSetUtilities_VCS_Drive2_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetUtilities_VCS_Drive2 : public CCommandSetBase_VCS
{    
public:
    BOOL VCS_GetVersion(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, WORD p_usNodeId, WORD* p_pulHardwareVersion, WORD* p_pulSoftwareVersion, WORD* p_pulApplicationNumber, WORD* p_pulApplicationVersion, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_Restore(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, WORD p_usNodeId, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_Store(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, WORD p_usNodeId, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, WORD p_usNodeId, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void* p_pData, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, WORD p_usNodeId, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, void* p_pData, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetUtilities_VCS_Drive2();
    virtual ~CCommandSetUtilities_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_GetObject;
    CCommand_VCS_Drive2* m_pCommand_GetVersion;
    CCommand_VCS_Drive2* m_pCommand_Restore;
    CCommand_VCS_Drive2* m_pCommand_SetObject;
    CCommand_VCS_Drive2* m_pCommand_Store;
};

#endif // !defined(AFX_CommandSetUtilities_VCS_Drive2_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
