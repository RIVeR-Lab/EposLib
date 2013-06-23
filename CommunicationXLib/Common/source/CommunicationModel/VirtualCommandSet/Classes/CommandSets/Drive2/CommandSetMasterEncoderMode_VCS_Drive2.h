#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetMasterEncoderMode_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_SetMasterEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usScalingNumerator, WORD p_usScalingDenominator, BYTE p_ubPolarity, DWORD p_ulMaxVelocity, DWORD p_ulMaxAcceleration, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetMasterEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusScalingNumerator, WORD *p_pusScalingDenominator, BYTE *p_pubPolarity, DWORD *p_pulMaxVelocity, DWORD *p_pulMaxAcceleration, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetMasterEncoderMode_VCS_Drive2();
    virtual ~CCommandSetMasterEncoderMode_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_GetMasterEncoderParameter;
    CCommand_VCS_Drive2* m_pCommand_SetMasterEncoderParameter;
};


