// CommandSetMotionInfo_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetMotionInfo_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetMotionInfo_VCS_Drive2_H__8CD6D347_6DDA_47E7_A6FB_44593CA1E0E4__INCLUDED_)
#define AFX_CommandSetMotionInfo_VCS_Drive2_H__8CD6D347_6DDA_47E7_A6FB_44593CA1E0E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetMotionInfo_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetMovementState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poTargetReached, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetVelocityIs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long *p_plVelocityIs, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetPositionIs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long *p_plPositionIs, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetCurrentIs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short* p_psCurrentIs, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetMotionInfo_VCS_Drive2();
    virtual ~CCommandSetMotionInfo_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_GetCurrentIs;
    CCommand_VCS_Drive2* m_pCommand_GetMovementState;
    CCommand_VCS_Drive2* m_pCommand_GetPositionIs;
    CCommand_VCS_Drive2* m_pCommand_GetVelocityIs;
};

#endif // !defined(AFX_CommandSetMotionInfo_VCS_Drive2_H__8CD6D347_6DDA_47E7_A6FB_44593CA1E0E4__INCLUDED_)
