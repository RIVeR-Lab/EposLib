// CommandSetPositionMode_VCS_Drive.h: Schnittstelle f�r die Klasse CCommandSetPositionMode_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetPositionMode_VCS_Drive_H__603688AB_31C7_42A8_A14C_616850E13B22__INCLUDED_)
#define AFX_CommandSetPositionMode_VCS_Drive_H__603688AB_31C7_42A8_A14C_616850E13B22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_VCS_Drive;
class CLayerManagerBase;

class CCommandSetPositionMode_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short* p_psPositionMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short p_sPositionMust, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetPositionMode_VCS_Drive();
    virtual ~CCommandSetPositionMode_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive* m_pCommand_GetPositionMust;
    CCommand_VCS_Drive* m_pCommand_SetPositionMust;
};

#endif // !defined(AFX_CommandSetPositionMode_VCS_Drive_H__603688AB_31C7_42A8_A14C_616850E13B22__INCLUDED_)
