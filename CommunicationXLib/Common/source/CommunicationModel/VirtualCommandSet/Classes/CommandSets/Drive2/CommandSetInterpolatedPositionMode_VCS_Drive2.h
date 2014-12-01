#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetInterpolatedPositionMode_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_SetIpmBufferParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usUnderflowWarningLimit, WORD p_usOverflowWarningLimit, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetIpmBufferParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusUnderflowWarningLimit, WORD *p_pusOverflowWarningLimit, DWORD *p_pulMaxBufferSize, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ClearIpmBuffer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetFreeIpmBufferSize(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD *p_pulBufferSize, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_AddPvtValueToIpmBuffer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, int p_lPosition, int p_lVelocity, BYTE p_ubTime, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_StartIpmTrajectory(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_StopIpmTrajectory(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetIpmStatus(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poTrajectoryRunning, BOOL* p_poIsUnderflowWarning, BOOL* p_poIsOverflowWarning, BOOL* p_poIsVelocityWarning, BOOL* p_poIsAccelerationWarning, BOOL *p_poIsUnderflowError, BOOL* p_poIsOverflowError, BOOL* p_poIsVelocityError, BOOL* p_poIsAccelerationError, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetInterpolatedPositionMode_VCS_Drive2(void);
    virtual ~CCommandSetInterpolatedPositionMode_VCS_Drive2(void);

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_SetIpmBufferParameter;
    CCommand_VCS_Drive2* m_pCommand_GetIpmBufferParameter;
    CCommand_VCS_Drive2* m_pCommand_ClearIpmBuffer;
    CCommand_VCS_Drive2* m_pCommand_GetFreeIpmBufferSize;
    CCommand_VCS_Drive2* m_pCommand_AddPvtValueToIpmBuffer;
    CCommand_VCS_Drive2* m_pCommand_StartIpmTrajectory;
    CCommand_VCS_Drive2* m_pCommand_StopIpmTrajectory;
    CCommand_VCS_Drive2* m_pCommand_GetIpmStatus;
};

