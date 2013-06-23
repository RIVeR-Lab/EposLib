// CommandSetGeneralGateway_VCS_CanOpen.h: Schnittstelle f�r die Klasse CCommandSetGeneralGateway_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetGeneralGatewayS_VCS_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
#define AFX_CommandSetGeneralGateway_VCS_CanOpen_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_VCS_CanOpen;
class CLayerManagerBase;

class CCommandSetGeneralGateway_VCS_CanOpen : public CCommandSetBase_VCS
{
public:
    BOOL VCS_RequestCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ReadCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SendCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SendNMTService(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNodeId, WORD p_usCommandSpecifier, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetGeneralGateway_VCS_CanOpen();
    virtual ~CCommandSetGeneralGateway_VCS_CanOpen();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_CanOpen* m_pCommand_ReadCANFrame;
    CCommand_VCS_CanOpen* m_pCommand_RequestCANFrame;
    CCommand_VCS_CanOpen* m_pCommand_SendCANFrame;
    CCommand_VCS_CanOpen* m_pCommand_SendNMTService;
};

#endif // !defined(AFX_CommandSetGeneralGateway_VCS_CanOpen_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
