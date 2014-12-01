// CommandSetHomingMode_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetHomingMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetHomingMode_VCS_Drive2_H__D86438FA_A3E0_4E0F_B28B_56910B64E752__INCLUDED_)
#define AFX_CommandSetHomingMode_VCS_Drive2_H__D86438FA_A3E0_4E0F_B28B_56910B64E752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetHomingMode_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_StopHoming(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_FindHome(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, __int8 p_bHomingMethod, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetHomingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulHomingAcceleration, DWORD p_ulSpeedSwitch, DWORD p_ulSpeedIndex, int p_lHomeOffset, WORD p_usCurrentThreshold, int p_lHomePosition=0, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetHomingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD *p_pulHomingAcceleration, DWORD *p_pulSpeedSwitch, DWORD *p_pulSpeedIndex, int *p_plHomeOffset, WORD* p_pusCurrentThreshold, int *p_plHomePosition = NULL, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DefinePosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lHomePosition, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetHomingMode_VCS_Drive2();
    virtual ~CCommandSetHomingMode_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();
    CCommand_VCS_Drive2* m_pCommand_FindHome;
    CCommand_VCS_Drive2* m_pCommand_GetHomingParameter;
    CCommand_VCS_Drive2* m_pCommand_SetHomingParameter;
    CCommand_VCS_Drive2* m_pCommand_StopHoming;
    CCommand_VCS_Drive2* m_pCommand_DefinePosition;
};

#endif // !defined(AFX_CommandSetHomingMode_VCS_Drive2_H__D86438FA_A3E0_4E0F_B28B_56910B64E752__INCLUDED_)
