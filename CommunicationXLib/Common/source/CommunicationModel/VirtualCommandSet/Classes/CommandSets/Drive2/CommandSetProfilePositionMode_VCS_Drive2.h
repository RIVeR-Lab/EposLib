// CommandSetProfilePositionMode_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetProfilePositionMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetProfilePositionMode_VCS_Drive2_H__8D4BF5C9_156A_45AE_BCB2_B9E2F6097751__INCLUDED_)
#define AFX_CommandSetProfilePositionMode_VCS_Drive2_H__8D4BF5C9_156A_45AE_BCB2_B9E2F6097751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetProfilePositionMode_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_HaltPositionMovement(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_MoveToPosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lTargetPosition, BOOL p_oAbsolute, BOOL p_oImmediately, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetTargetPosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int *p_plTargetPosition, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetPositionProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulProfileVelocity, DWORD* p_pulProfileAcceleration, DWORD* p_pulProfileDeceleration, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetPositionProfile(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulProfileVelocity, DWORD p_ulProfileAcceleration, DWORD p_ulProfileDeceleration, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_EnablePositionWindow(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulPositionWindow, WORD p_usPositionWindowTime, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DisablePositionWindow(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetProfilePositionMode_VCS_Drive2();
    virtual ~CCommandSetProfilePositionMode_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_GetPositionProfile;
    CCommand_VCS_Drive2* m_pCommand_GetTargetPosition;
    CCommand_VCS_Drive2* m_pCommand_HaltPositionMovement;
    CCommand_VCS_Drive2* m_pCommand_MoveToPosition;
    CCommand_VCS_Drive2* m_pCommand_SetPositionProfile;
    CCommand_VCS_Drive2* m_pCommand_EnablePositionWindow;
    CCommand_VCS_Drive2* m_pCommand_DisablePositionWindow;
};

#endif // !defined(AFX_CommandSetProfilePositionMode_VCS_Drive2_H__8D4BF5C9_156A_45AE_BCB2_B9E2F6097751__INCLUDED_)
