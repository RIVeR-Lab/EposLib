// CommandSetVelocityMode_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetVelocityMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetVelocityMode_VCS_Drive2_H__62E8CBA5_B246_49A5_89A2_26C1E50916BF__INCLUDED_)
#define AFX_CommandSetVelocityMode_VCS_Drive2_H__62E8CBA5_B246_49A5_89A2_26C1E50916BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetVelocityMode_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int *p_plVelocityMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetVelocityMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lVelocityMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ActivateAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, float fScaling, short sOffset, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DeactivateAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_EnableAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DisableAnalogVelocitySetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetVelocityMode_VCS_Drive2();
    virtual ~CCommandSetVelocityMode_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_GetVelocityMust;
    CCommand_VCS_Drive2* m_pCommand_SetVelocityMust;
    CCommand_VCS_Drive2* m_pCommand_ActivateAnalogVelocitySetpoint;
    CCommand_VCS_Drive2* m_pCommand_DeactivateAnalogVelocitySetpoint;
    CCommand_VCS_Drive2* m_pCommand_EnableAnalogVelocitySetpoint;
    CCommand_VCS_Drive2* m_pCommand_DisableAnalogVelocitySetpoint;
};

#endif // !defined(AFX_CommandSetVelocityMode_VCS_Drive2_H__62E8CBA5_B246_49A5_89A2_26C1E50916BF__INCLUDED_)
