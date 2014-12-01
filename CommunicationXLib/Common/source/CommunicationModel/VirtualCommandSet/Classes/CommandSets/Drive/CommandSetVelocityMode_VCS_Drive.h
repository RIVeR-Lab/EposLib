// CommandSetVelocityMode_VCS_Drive.h: Schnittstelle f�r die Klasse CCommandSetVelocityMode_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetVelocityMode_VCS_Drive_H__62E8CBA5_B246_49A5_89A2_26C1E50916BF__INCLUDED_)
#define AFX_CommandSetVelocityMode_VCS_Drive_H__62E8CBA5_B246_49A5_89A2_26C1E50916BF__INCLUDED_

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

class CCommandSetVelocityMode_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int *p_plVelocityMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lVelocityMust, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetVelocityMode_VCS_Drive();
    virtual ~CCommandSetVelocityMode_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive* m_pCommand_GetVelocityMust;
    CCommand_VCS_Drive* m_pCommand_SetVelocityMust;
};

#endif // !defined(AFX_CommandSetVelocityMode_VCS_Drive_H__62E8CBA5_B246_49A5_89A2_26C1E50916BF__INCLUDED_)
