// CommandSetNetworkManagement_DCS_Esam.h: Schnittstelle f�r die Klasse CCommandSetNetworkManagement_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetNetworkManagement_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
#define AFX_CommandSetNetworkManagement_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_DCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_DCS_Esam;
class CLayerManagerBase;

class CCommandSetNetworkManagement_DCS_Esam : public CCommandSetBase_DCS
{
public:
    CCommandSetNetworkManagement_DCS_Esam();
    virtual ~CCommandSetNetworkManagement_DCS_Esam();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
    virtual void ResetJournalManager();

    virtual BOOL InitGateway(CGateway *pGateway);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement);

    BOOL DCS_SendNMTService(CLayerManagerBase *pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wNodeId, WORD wCommandSpecifier, CErrorInfo* pErrorInfo = NULL);

private:
    void DeleteCommands();
    void InitCommands();

    CCommand_DCS_Esam* m_pCommand_SendNMTService;
};

#endif // !defined(AFX_CommandSetNetworkManagement_DCS_Esam_H__EF064ED0_0546_4F1A_875E_6F64B26B530C__INCLUDED_)
