// CommandSetDataRecording_VCS_DataRecorder.cpp: Implementierung der Klasse CCommandSetDataRecording_VCS_DataRecorder.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_DataRecorder.h>
#include "CommandSetDataRecording_VCS_DataRecorder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetDataRecording_VCS_DataRecorder::CCommandSetDataRecording_VCS_DataRecorder()
{
    m_strCommandSetName = COMMAND_SET_DATA_RECORDING;

    //DataRecorder Setup
    m_pCommand_SetDataRecordingParameter = NULL;
    m_pCommand_GetDataRecordingParameter = NULL;
    m_pCommand_EnableTrigger = NULL;
    m_pCommand_DisableAllTriggers = NULL;
    m_pCommand_ActivateChannel = NULL;
    m_pCommand_DeactivateAllChannels = NULL;

    //DataRecorder Status
    m_pCommand_StartRecorder = NULL;
    m_pCommand_StopRecorder = NULL;
    m_pCommand_ForceTrigger = NULL;
    m_pCommand_IsRecorderRunning = NULL;
    m_pCommand_IsRecorderTriggered = NULL;

    //DataRecorder Data
    m_pCommand_ReadChannelVectorSize = NULL;

    //DataRecorder Advanced Functions
    m_pCommand_ReadDataBuffer = NULL;

    InitCommands();
}

CCommandSetDataRecording_VCS_DataRecorder::~CCommandSetDataRecording_VCS_DataRecorder()
{
    DeleteCommands();
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_GetDataRecordingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusScalingNumerator, WORD *p_pusScalingDenominator, BYTE *p_pubPolarity, DWORD *p_pulMaxVelocity, DWORD *p_pulMaxAcceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetDataRecordingParameter)
    {
        //Set Parameter Data
        m_pCommand_GetDataRecordingParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetDataRecordingParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetDataRecordingParameter->GetReturnParameterData(0, p_pusScalingNumerator, sizeof(p_pusScalingNumerator));
        m_pCommand_GetDataRecordingParameter->GetReturnParameterData(1, p_pusScalingDenominator, sizeof(p_pusScalingDenominator));
        m_pCommand_GetDataRecordingParameter->GetReturnParameterData(2, p_pubPolarity, sizeof(p_pubPolarity));
        m_pCommand_GetDataRecordingParameter->GetReturnParameterData(3, p_pulMaxVelocity, sizeof(p_pulMaxVelocity));
        m_pCommand_GetDataRecordingParameter->GetReturnParameterData(4, p_pulMaxAcceleration, sizeof(p_pulMaxAcceleration));

        //Get ErrorCode
        m_pCommand_GetDataRecordingParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_SetDataRecordingParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usScalingNumerator, WORD p_usScalingDenominator, BYTE p_ubPolarity, DWORD p_ulMaxVelocity, DWORD p_ulMaxAcceleration, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetDataRecordingParameter)
    {
        //Set Parameter Data
        m_pCommand_SetDataRecordingParameter->ResetStatus();
        m_pCommand_SetDataRecordingParameter->SetParameterData(0, &p_usScalingNumerator, sizeof(p_usScalingNumerator));
        m_pCommand_SetDataRecordingParameter->SetParameterData(1, &p_usScalingDenominator, sizeof(p_usScalingDenominator));
        m_pCommand_SetDataRecordingParameter->SetParameterData(2, &p_ubPolarity, sizeof(p_ubPolarity));
        m_pCommand_SetDataRecordingParameter->SetParameterData(3, &p_ulMaxVelocity, sizeof(p_ulMaxVelocity));
        m_pCommand_SetDataRecordingParameter->SetParameterData(4, &p_ulMaxAcceleration, sizeof(p_ulMaxAcceleration));

        //ExecuteCommand
        oResult = m_pCommand_GetDataRecordingParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetDataRecordingParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_EnableTrigger(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubTriggerType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_EnableTrigger)
    {
        //Set Parameter Data
        m_pCommand_EnableTrigger->ResetStatus();
        m_pCommand_EnableTrigger->SetParameterData(0, &p_ubTriggerType, sizeof(p_ubTriggerType));

        //ExecuteCommand
        oResult = m_pCommand_EnableTrigger->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_EnableTrigger->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_DisableAllTriggers(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DisableAllTriggers)
    {
        //Set Parameter Data
        m_pCommand_DisableAllTriggers->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DisableAllTriggers->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DisableAllTriggers->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_ActivateChannel(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubChannelNumber, WORD p_usObjectIndex, BYTE p_ubObjectSubIndex, BYTE p_ubObjectSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ActivateChannel)
    {
        //Set Parameter Data
        m_pCommand_ActivateChannel->ResetStatus();
        m_pCommand_ActivateChannel->SetParameterData(0, &p_ubChannelNumber, sizeof(p_ubChannelNumber));
        m_pCommand_ActivateChannel->SetParameterData(1, &p_usObjectIndex, sizeof(p_usObjectIndex));
        m_pCommand_ActivateChannel->SetParameterData(2, &p_ubObjectSubIndex, sizeof(p_ubObjectSubIndex));
        m_pCommand_ActivateChannel->SetParameterData(3, &p_ubObjectSize, sizeof(p_ubObjectSize));

        //ExecuteCommand
        oResult = m_pCommand_ActivateChannel->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ActivateChannel->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_DeactivateAllChannels(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_DeactivateAllChannels)
    {
        //Set Parameter Data
        m_pCommand_DeactivateAllChannels->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_DeactivateAllChannels->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_DeactivateAllChannels->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_StartRecorder(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_StartRecorder)
    {
        //Set Parameter Data
        m_pCommand_StartRecorder->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_StartRecorder->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_StartRecorder->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_StopRecorder(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_StopRecorder)
    {
        //Set Parameter Data
        m_pCommand_StopRecorder->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_StopRecorder->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_StopRecorder->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_ForceTrigger(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ForceTrigger)
    {
        //Set Parameter Data
        m_pCommand_ForceTrigger->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_ForceTrigger->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ForceTrigger->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_IsRecorderRunning(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poRunning, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_IsRecorderRunning)
    {
        //Set Parameter Data
        m_pCommand_IsRecorderRunning->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_IsRecorderRunning->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_IsRecorderRunning->GetReturnParameterData(0, p_poRunning, sizeof(p_poRunning));

        //Get ErrorCode
        m_pCommand_IsRecorderRunning->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_IsRecorderTriggered(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poTriggered, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_IsRecorderTriggered)
    {
        //Set Parameter Data
        m_pCommand_IsRecorderTriggered->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_IsRecorderTriggered->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_IsRecorderTriggered->GetReturnParameterData(0, p_poTriggered, sizeof(p_poTriggered));

        //Get ErrorCode
        m_pCommand_IsRecorderTriggered->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_ReadChannelVectorSize(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulVectorSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ReadChannelVectorSize)
    {
        //Set Parameter Data
        m_pCommand_ReadChannelVectorSize->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_ReadChannelVectorSize->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadChannelVectorSize->GetReturnParameterData(0, p_pulVectorSize, sizeof(p_pulVectorSize));

        //Get ErrorCode
        m_pCommand_ReadChannelVectorSize->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::VCS_ReadDataBuffer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubDataBuffer, DWORD p_ulBufferSizeToRead, DWORD* p_pulBufferSizeRead, WORD* p_pusVectorStartOffset, WORD* p_pusMaxNbOfSamples, WORD* p_pusNbOfRecordedSamples, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ReadDataBuffer)
    {
        //Set Parameter Data
        m_pCommand_ReadDataBuffer->ResetStatus();
        m_pCommand_ReadDataBuffer->SetParameterData(0, &p_ulBufferSizeToRead, sizeof(p_ulBufferSizeToRead));

        //ExecuteCommand
        oResult = m_pCommand_ReadDataBuffer->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadDataBuffer->GetReturnParameterData(0, p_pubDataBuffer, p_ulBufferSizeToRead);
        m_pCommand_ReadDataBuffer->GetReturnParameterData(1, p_pulBufferSizeRead, sizeof(p_pulBufferSizeRead));
        m_pCommand_ReadDataBuffer->GetReturnParameterData(2, p_pusVectorStartOffset, sizeof(p_pusVectorStartOffset));
        m_pCommand_ReadDataBuffer->GetReturnParameterData(3, p_pusMaxNbOfSamples, sizeof(p_pusMaxNbOfSamples));
        m_pCommand_ReadDataBuffer->GetReturnParameterData(4, p_pusNbOfRecordedSamples, sizeof(p_pusNbOfRecordedSamples));

        //Get ErrorCode
        m_pCommand_ReadDataBuffer->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetDataRecording_VCS_DataRecorder::InitCommands()
{
    DeleteCommands();

    //Init GetDataRecordingParameter
    m_pCommand_GetDataRecordingParameter = new CCommand_VCS_DataRecorder();
    m_pCommand_GetDataRecordingParameter->InitCommand(REC_GET_RECORDER_PARAMETER);

    //Init SetDataRecordingParameter
    m_pCommand_SetDataRecordingParameter = new CCommand_VCS_DataRecorder;
    m_pCommand_SetDataRecordingParameter->InitCommand(REC_SET_RECORDER_PARAMETER);

    //Init EnableTrigger
    m_pCommand_EnableTrigger = new CCommand_VCS_DataRecorder;
    m_pCommand_EnableTrigger->InitCommand(REC_ENABLE_TRIGGER);

    //Init DisableAllTriggers
    m_pCommand_DisableAllTriggers = new CCommand_VCS_DataRecorder;
    m_pCommand_DisableAllTriggers->InitCommand(REC_DISABLE_ALL_TRIGGERS);

    //Init ActivateChannel
    m_pCommand_ActivateChannel = new CCommand_VCS_DataRecorder;
    m_pCommand_ActivateChannel->InitCommand(REC_ACTIVATE_CHANNEL);

    //Init DeactivateAllChannels
    m_pCommand_DeactivateAllChannels = new CCommand_VCS_DataRecorder;
    m_pCommand_DeactivateAllChannels->InitCommand(REC_DEACTIVATE_ALL_CHANNELS);

    //Init StartRecorder
    m_pCommand_StartRecorder = new CCommand_VCS_DataRecorder;
    m_pCommand_StartRecorder->InitCommand(REC_START_RECORDER);

    //Init StopRecorder
    m_pCommand_StopRecorder = new CCommand_VCS_DataRecorder;
    m_pCommand_StopRecorder->InitCommand(REC_STOP_RECORDER);

    //Init ForceTrigger
    m_pCommand_ForceTrigger = new CCommand_VCS_DataRecorder;
    m_pCommand_ForceTrigger->InitCommand(REC_FORCE_TRIGGER);

    //Init IsRecorderRunning
    m_pCommand_IsRecorderRunning = new CCommand_VCS_DataRecorder;
    m_pCommand_IsRecorderRunning->InitCommand(REC_IS_RECORDER_RUNNING);

    //Init IsRecorderTriggered
    m_pCommand_IsRecorderTriggered = new CCommand_VCS_DataRecorder;
    m_pCommand_IsRecorderTriggered->InitCommand(REC_IS_RECORDER_TRIGGERED);

    //Init ReadChannelVectorSize
    m_pCommand_ReadChannelVectorSize = new CCommand_VCS_DataRecorder;
    m_pCommand_ReadChannelVectorSize->InitCommand(REC_READ_CHANNEL_VECTOR_SIZE);

    //Init ReadDataBuffer
    m_pCommand_ReadDataBuffer = new CCommand_VCS_DataRecorder;
    m_pCommand_ReadDataBuffer->InitCommand(REC_READ_DATA_BUFFER);
}

void CCommandSetDataRecording_VCS_DataRecorder::DeleteCommands()
{
    if(m_pCommand_GetDataRecordingParameter)
    {
        delete m_pCommand_GetDataRecordingParameter;
        m_pCommand_GetDataRecordingParameter = NULL;
    }
    if(m_pCommand_SetDataRecordingParameter)
    {
        delete m_pCommand_SetDataRecordingParameter;
        m_pCommand_SetDataRecordingParameter = NULL;
    }
    if(m_pCommand_EnableTrigger)
    {
        delete m_pCommand_EnableTrigger;
        m_pCommand_EnableTrigger = NULL;
    }
    if(m_pCommand_DisableAllTriggers)
    {
        delete m_pCommand_DisableAllTriggers;
        m_pCommand_DisableAllTriggers = NULL;
    }
    if(m_pCommand_ActivateChannel)
    {
        delete m_pCommand_ActivateChannel;
        m_pCommand_ActivateChannel = NULL;
    }
    if(m_pCommand_DeactivateAllChannels)
    {
        delete m_pCommand_DeactivateAllChannels;
        m_pCommand_DeactivateAllChannels = NULL;
    }
    if(m_pCommand_StartRecorder)
    {
        delete m_pCommand_StartRecorder;
        m_pCommand_StartRecorder = NULL;
    }
    if(m_pCommand_StopRecorder)
    {
        delete m_pCommand_StopRecorder;
        m_pCommand_StopRecorder = NULL;
    }
    if(m_pCommand_ForceTrigger)
    {
        delete m_pCommand_ForceTrigger;
        m_pCommand_ForceTrigger = NULL;
    }
    if(m_pCommand_IsRecorderRunning)
    {
        delete m_pCommand_IsRecorderRunning;
        m_pCommand_IsRecorderRunning = NULL;
    }
    if(m_pCommand_IsRecorderTriggered)
    {
        delete m_pCommand_IsRecorderTriggered;
        m_pCommand_IsRecorderTriggered = NULL;
    }
    if(m_pCommand_ReadChannelVectorSize)
    {
        delete m_pCommand_ReadChannelVectorSize;
        m_pCommand_ReadChannelVectorSize = NULL;
    }
    if(m_pCommand_ReadDataBuffer)
    {
        delete m_pCommand_ReadDataBuffer;
        m_pCommand_ReadDataBuffer = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetDataRecording_VCS_DataRecorder::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;
    BOOL oCheckVisibility = FALSE;

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //DataRecorder Setup
        if(m_pCommand_GetDataRecordingParameter && !m_pCommand_GetDataRecordingParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetDataRecordingParameter && !m_pCommand_SetDataRecordingParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_EnableTrigger && !m_pCommand_EnableTrigger->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DisableAllTriggers && !m_pCommand_DisableAllTriggers->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ActivateChannel && !m_pCommand_ActivateChannel->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_DeactivateAllChannels && !m_pCommand_DeactivateAllChannels->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;

        //DataRecorder Status
        if(m_pCommand_StartRecorder && !m_pCommand_StartRecorder->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_StopRecorder && !m_pCommand_StopRecorder->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ForceTrigger && !m_pCommand_ForceTrigger->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_IsRecorderRunning && !m_pCommand_IsRecorderRunning->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_IsRecorderTriggered && !m_pCommand_IsRecorderTriggered->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;

        //DataRecorder Data
        if(m_pCommand_ReadChannelVectorSize && !m_pCommand_ReadChannelVectorSize->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;

        //DataRecorder Advanced Functions
        if(m_pCommand_ReadDataBuffer && !m_pCommand_ReadDataBuffer->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetDataRecording_VCS_DataRecorder::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    //DataRecorder Setup
    if(m_pCommand_GetDataRecordingParameter) m_pCommand_GetDataRecordingParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetDataRecordingParameter) m_pCommand_SetDataRecordingParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_EnableTrigger) m_pCommand_EnableTrigger->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DisableAllTriggers) m_pCommand_DisableAllTriggers->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ActivateChannel) m_pCommand_ActivateChannel->InitJournalManager(p_pJournalManager);
    if(m_pCommand_DeactivateAllChannels) m_pCommand_DeactivateAllChannels->InitJournalManager(p_pJournalManager);

    //DataRecorder Status
    if(m_pCommand_StartRecorder) m_pCommand_StartRecorder->InitJournalManager(p_pJournalManager);
    if(m_pCommand_StopRecorder) m_pCommand_StopRecorder->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ForceTrigger) m_pCommand_ForceTrigger->InitJournalManager(p_pJournalManager);
    if(m_pCommand_IsRecorderRunning) m_pCommand_IsRecorderRunning->InitJournalManager(p_pJournalManager);
    if(m_pCommand_IsRecorderTriggered) m_pCommand_IsRecorderTriggered->InitJournalManager(p_pJournalManager);

    //DataRecorder Data
    if(m_pCommand_ReadChannelVectorSize) m_pCommand_ReadChannelVectorSize->InitJournalManager(p_pJournalManager);

    //DataRecorder Advanced Functions
    if(m_pCommand_ReadDataBuffer) m_pCommand_ReadDataBuffer->InitJournalManager(p_pJournalManager);
}

void CCommandSetDataRecording_VCS_DataRecorder::ResetJournalManager()
{
    //DataRecorder Setup
    if(m_pCommand_GetDataRecordingParameter) m_pCommand_GetDataRecordingParameter->ResetJournalManager();
    if(m_pCommand_SetDataRecordingParameter) m_pCommand_SetDataRecordingParameter->ResetJournalManager();
    if(m_pCommand_EnableTrigger) m_pCommand_EnableTrigger->ResetJournalManager();
    if(m_pCommand_DisableAllTriggers) m_pCommand_DisableAllTriggers->ResetJournalManager();
    if(m_pCommand_ActivateChannel) m_pCommand_ActivateChannel->ResetJournalManager();
    if(m_pCommand_DeactivateAllChannels) m_pCommand_DeactivateAllChannels->ResetJournalManager();

    //DataRecorder Status
    if(m_pCommand_StartRecorder) m_pCommand_StartRecorder->ResetJournalManager();
    if(m_pCommand_StopRecorder) m_pCommand_StopRecorder->ResetJournalManager();
    if(m_pCommand_ForceTrigger) m_pCommand_ForceTrigger->ResetJournalManager();
    if(m_pCommand_IsRecorderRunning) m_pCommand_IsRecorderRunning->ResetJournalManager();
    if(m_pCommand_IsRecorderTriggered) m_pCommand_IsRecorderTriggered->ResetJournalManager();

    //DataRecorder Data
    if(m_pCommand_ReadChannelVectorSize) m_pCommand_ReadChannelVectorSize->ResetJournalManager();

    //DataRecorder Advanced Functions
    if(m_pCommand_ReadDataBuffer) m_pCommand_ReadDataBuffer->ResetJournalManager();
}

BOOL CCommandSetDataRecording_VCS_DataRecorder::InitGateway(CGateway *p_pGateway)
{
    //DataRecorder Setup
    if(m_pCommand_GetDataRecordingParameter && !m_pCommand_GetDataRecordingParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetDataRecordingParameter && !m_pCommand_SetDataRecordingParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_EnableTrigger && !m_pCommand_EnableTrigger->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DisableAllTriggers && !m_pCommand_DisableAllTriggers->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ActivateChannel && !m_pCommand_ActivateChannel->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_DeactivateAllChannels && !m_pCommand_DeactivateAllChannels->InitGateway(p_pGateway)) return FALSE;

    //DataRecorder Status
    if(m_pCommand_StartRecorder && !m_pCommand_StartRecorder->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_StopRecorder && !m_pCommand_StopRecorder->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ForceTrigger && !m_pCommand_ForceTrigger->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_IsRecorderRunning && !m_pCommand_IsRecorderRunning->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_IsRecorderTriggered && !m_pCommand_IsRecorderTriggered->InitGateway(p_pGateway)) return FALSE;

    //DataRecorder Data
    if(m_pCommand_ReadChannelVectorSize && !m_pCommand_ReadChannelVectorSize->InitGateway(p_pGateway)) return FALSE;

    //DataRecorder Advanced Functions
    if(m_pCommand_ReadDataBuffer && !m_pCommand_ReadDataBuffer->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}
