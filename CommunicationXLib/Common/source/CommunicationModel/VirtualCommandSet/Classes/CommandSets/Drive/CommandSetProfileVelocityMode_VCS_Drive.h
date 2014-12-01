// CommandSetProfileVelocityMode_VCS_Drive.h: Schnittstelle f�r die Klasse CCommandSetProfileVelocityMode_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetProfileVelocityMode_VCS_Drive_H__4AFFBD0A_881A_4930_AFF1_015C16D5B33B__INCLUDED_)
#define AFX_CommandSetProfileVelocityMode_VCS_Drive_H__4AFFBD0A_881A_4930_AFF1_015C16D5B33B__INCLUDED_

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

class CCommandSetProfileVelocityMode_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetTargetVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int *p_plTargetVelocity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetVelocityProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pdProfileAcceleration, DWORD* p_pdProfileDeceleration, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_HaltVelocityMovement(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_MoveWithVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lTargetVelocity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetVelocityProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulProfileAcceleration, DWORD p_ulProfileDeceleration, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetProfileVelocityMode_VCS_Drive();
    virtual ~CCommandSetProfileVelocityMode_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive* m_pCommand_GetTargetVelocity;
    CCommand_VCS_Drive* m_pCommand_GetVelocityProfile;
    CCommand_VCS_Drive* m_pCommand_HaltVelocityMovement;
    CCommand_VCS_Drive* m_pCommand_MoveWithVelocity;
    CCommand_VCS_Drive* m_pCommand_SetVelocityProfile;
};

#endif // !defined(AFX_CommandSetProfileVelocityMode_VCS_Drive_H__4AFFBD0A_881A_4930_AFF1_015C16D5B33B__INCLUDED_)
