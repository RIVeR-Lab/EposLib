#pragma once

#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_DataRecorder;
class CLayerManagerBase;

class CCommandSetDataRecording_VCS_DataRecorder : public CCommandSetBase_VCS
{
public:
    //DataRecorder Setup
    BOOL VCS_SetDataRecordingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usScalingNumerator, WORD p_usScalingDenominator, BYTE p_ubPolarity, DWORD p_ulMaxVelocity, DWORD p_ulMaxAcceleration, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetDataRecordingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusScalingNumerator, WORD *p_pusScalingDenominator, BYTE *p_pubPolarity, DWORD *p_pulMaxVelocity, DWORD *p_pulMaxAcceleration, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_EnableTrigger(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubTriggerType, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DisableAllTriggers(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ActivateChannel(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubChannelNumber, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, BYTE p_ubObjectSize, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DeactivateAllChannels(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    //DataRecorder Status
    BOOL VCS_StartRecorder(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_StopRecorder(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ForceTrigger(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_IsRecorderRunning(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poRunning, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_IsRecorderTriggered(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poTriggered, CErrorInfo* p_pErrorInfo = NULL);

    //DataRecorder Data
    BOOL VCS_ReadChannelVectorSize(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulVectorSize, CErrorInfo* p_pErrorInfo = NULL);

    //DataRecorder Advanced Functions
    BOOL VCS_ReadDataBuffer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubDataBuffer, DWORD p_ulBufferSizeToRead, DWORD* p_pulBufferSizeRead, WORD* p_pusVectorStartOffset, WORD* p_pusMaxNbOfSamples, WORD* p_pusNbOfRecordedSamples, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetDataRecording_VCS_DataRecorder();
    virtual ~CCommandSetDataRecording_VCS_DataRecorder();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    //DataRecorder Setup
    CCommand_VCS_DataRecorder* m_pCommand_SetDataRecordingParameter;
    CCommand_VCS_DataRecorder* m_pCommand_GetDataRecordingParameter;
    CCommand_VCS_DataRecorder* m_pCommand_EnableTrigger;
    CCommand_VCS_DataRecorder* m_pCommand_DisableAllTriggers;
    CCommand_VCS_DataRecorder* m_pCommand_ActivateChannel;
    CCommand_VCS_DataRecorder* m_pCommand_DeactivateAllChannels;

    //DataRecorder Status
    CCommand_VCS_DataRecorder* m_pCommand_StartRecorder;
    CCommand_VCS_DataRecorder* m_pCommand_StopRecorder;
    CCommand_VCS_DataRecorder* m_pCommand_ForceTrigger;
    CCommand_VCS_DataRecorder* m_pCommand_IsRecorderRunning;
    CCommand_VCS_DataRecorder* m_pCommand_IsRecorderTriggered;

    //DataRecorder Data
    CCommand_VCS_DataRecorder* m_pCommand_ReadChannelVectorSize;

    //DataRecorder Advanced Functions
    CCommand_VCS_DataRecorder* m_pCommand_ReadDataBuffer;
};

