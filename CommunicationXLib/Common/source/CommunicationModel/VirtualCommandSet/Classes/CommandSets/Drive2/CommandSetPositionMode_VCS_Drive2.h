// CommandSetPositionMode_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetPositionMode_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetPositionMode_VCS_Drive2_H__603688AB_31C7_42A8_A14C_616850E13B22__INCLUDED_)
#define AFX_CommandSetPositionMode_VCS_Drive2_H__603688AB_31C7_42A8_A14C_616850E13B22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetPositionMode_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short* p_psPositionMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetPositionMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short p_sPositionMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ActivateAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, float fScaling, short sOffset, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DeactivateAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_EnableAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DisableAnalogPositionSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetPositionMode_VCS_Drive2();
    virtual ~CCommandSetPositionMode_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_GetPositionMust;
    CCommand_VCS_Drive2* m_pCommand_SetPositionMust;
    CCommand_VCS_Drive2* m_pCommand_ActivateAnalogPositionSetpoint;
    CCommand_VCS_Drive2* m_pCommand_DeactivateAnalogPositionSetpoint;
    CCommand_VCS_Drive2* m_pCommand_EnableAnalogPositionSetpoint;
    CCommand_VCS_Drive2* m_pCommand_DisableAnalogPositionSetpoint;
};

#endif // !defined(AFX_CommandSetPositionMode_VCS_Drive2_H__603688AB_31C7_42A8_A14C_616850E13B22__INCLUDED_)
