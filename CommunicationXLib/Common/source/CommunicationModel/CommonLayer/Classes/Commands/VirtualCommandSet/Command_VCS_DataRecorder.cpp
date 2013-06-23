// Command_VCS_DataRecorder.cpp: Implementierung der Klasse CCommand_VCS_DataRecorder.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_DataRecorder.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
/////////////////////////0////////////////////////////////////////////
CCommand_VCS_DataRecorder::CCommand_VCS_DataRecorder()
{
}

CCommand_VCS_DataRecorder::CCommand_VCS_DataRecorder(DWORD p_ulCommandId)
{
    InitCommand(p_ulCommandId);
}

CCommand_VCS_DataRecorder::~CCommand_VCS_DataRecorder()
{
}

CCommandRoot*CCommand_VCS_DataRecorder::CloneCommand()
{
    CCommand_VCS_DataRecorder* pNewCommand;

    pNewCommand = new CCommand_VCS_DataRecorder();
    *pNewCommand = *this;

    return pNewCommand;
}

CCommand_VCS_DataRecorder& CCommand_VCS_DataRecorder::operator=(CCommand_VCS_DataRecorder& other)
{
    if(this != &other)
    {
        *((CCommandRoot*)this) = *((CCommandRoot*)&other);
    }

    return *this;
}

BOOL CCommand_VCS_DataRecorder::InitCommand(DWORD p_ulCommandId)
{
    ResetCommand();

    if(InitCommand_DataRecording(p_ulCommandId)) return TRUE;

    return FALSE;
}

BOOL CCommand_VCS_DataRecorder::InitCommand_DataRecording(DWORD p_ulCommandId)
{
    switch(p_ulCommandId)
    {
        case REC_SET_RECORDER_PARAMETER:
            {
                CCommand_VCS::InitCommand("SetRecorderParameter", REC_SET_RECORDER_PARAMETER);

                AddParameter(0, "samplingPeriod", ODT_UINT16);
                AddParameter(1, "nbOfPrecedingSamples", ODT_UINT16);

                SetDefaultParameter_SetRecorderParameter();
                return TRUE;
            };
        case REC_GET_RECORDER_PARAMETER:
            {
                CCommand_VCS::InitCommand("GetRecorderParameter", REC_GET_RECORDER_PARAMETER);

                AddReturnParameter(0, "samplingPeriod", ODT_UINT16);
                AddReturnParameter(1, "nbOfPrecedingSamples", ODT_UINT16);

                SetDefaultParameter_GetRecorderParameter();
                return TRUE;
            };
        case REC_ENABLE_TRIGGER:
            {
                CCommand_VCS::InitCommand("EnableTrigger", REC_ENABLE_TRIGGER);

                AddParameter(0, "triggerType", ODT_UINT8);

                SetDefaultParameter_EnableTrigger();
                return TRUE;
            };
        case REC_DISABLE_ALL_TRIGGERS:
            {
                CCommand_VCS::InitCommand("DisableAllTrigger", REC_DISABLE_ALL_TRIGGERS);
                SetDefaultParameter_DisableAllTriggers();
                return TRUE;
            };
        case REC_ACTIVATE_CHANNEL:
            {
                CCommand_VCS::InitCommand("ActivateChannel", REC_ACTIVATE_CHANNEL);

                AddParameter(0, "channelNumber", ODT_UINT8);
                AddParameter(1, "objectIndex", ODT_UINT16);
                AddParameter(2, "objectSubIndex", ODT_UINT8);
                AddParameter(3, "objectSize", ODT_UINT8);

                SetDefaultParameter_ActivateChannel();
                return TRUE;
            };
        case REC_DEACTIVATE_ALL_CHANNELS:
            {
                CCommand_VCS::InitCommand("DeactivateAllChannels", REC_DEACTIVATE_ALL_CHANNELS);
                SetDefaultParameter_DeactivateAllChannels();
                return TRUE;
            };
        case REC_START_RECORDER:
            {
                CCommand_VCS::InitCommand("StartRecorder", REC_START_RECORDER);
                SetDefaultParameter_StartRecorder();
                return TRUE;
            };
        case REC_STOP_RECORDER:
            {
                CCommand_VCS::InitCommand("StopRecorder", REC_STOP_RECORDER);
                SetDefaultParameter_StopRecorder();
                return TRUE;
            };
        case REC_FORCE_TRIGGER:
            {
                CCommand_VCS::InitCommand("ForceTrigger", REC_FORCE_TRIGGER);
                SetDefaultParameter_ForceTrigger();
                return TRUE;
            };
        case REC_IS_RECORDER_RUNNING:
            {
                CCommand_VCS::InitCommand("IsRecorderRunning", REC_IS_RECORDER_RUNNING);

                AddReturnParameter(0, "running", ODT_BOOLEAN);

                SetDefaultParameter_IsRecorderRunning();
                return TRUE;
            };
        case REC_IS_RECORDER_TRIGGERED:
            {
                CCommand_VCS::InitCommand("IsRecorderTriggered", REC_IS_RECORDER_TRIGGERED);

                AddReturnParameter(0, "triggered", ODT_BOOLEAN);

                SetDefaultParameter_IsRecorderTriggered();
                return TRUE;
            };
        case REC_READ_CHANNEL_VECTOR_SIZE:
            {
                CCommand_VCS::InitCommand("ReadChannelVectorSize", REC_READ_CHANNEL_VECTOR_SIZE);

                AddReturnParameter(0, "vectorSize", ODT_UINT32);

                SetDefaultParameter_ReadChannelVectorSize();
                return TRUE;
            };
        case REC_READ_CHANNEL_DATA_VECTOR:
            {
                CCommand_VCS::InitCommand("ReadChannelDataVector", REC_READ_CHANNEL_DATA_VECTOR);

                AddParameter(0, "channelNumber", ODT_UINT8);
                AddParameter(1, "vectorSize", ODT_UINT32);
                AddReturnParameter(0, "dataVector", ODT_MEMORY_BLOCK);

                SetDefaultParameter_ReadChannelDataVector();
                return TRUE;
            };
        case REC_READ_DATA_BUFFER:
            {
                CCommand_VCS::InitCommand("ReadDataBuffer", REC_READ_DATA_BUFFER);

                AddParameter(0, "bufferSizeToRead", ODT_UINT32);
                AddReturnParameter(0, "dataBuffer", ODT_MEMORY_BLOCK);
                AddReturnParameter(1, "bufferSizeRead", ODT_UINT32);
                AddReturnParameter(2, "vectorStartOffset", ODT_UINT16);
                AddReturnParameter(3, "maxNbOfSamples", ODT_UINT16);
                AddReturnParameter(4, "nbOfRecordedSamples", ODT_UINT16);

                SetDefaultParameter_ReadDataBuffer();
                return TRUE;
            };
    }

    return FALSE;
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_SetRecorderParameter()
{

    WORD usSamplingPeriod = 10;
    WORD usNbOfPrecedingSamples = 0;

    //Parameter
    SetParameterData(0, &usSamplingPeriod, sizeof(usSamplingPeriod));
    SetParameterData(1, &usNbOfPrecedingSamples, sizeof(usNbOfPrecedingSamples));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_GetRecorderParameter()
{

    WORD usSamplingPeriod = 10;
    WORD usNbOfPrecedingSamples = 0;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &usSamplingPeriod, sizeof(usSamplingPeriod));
    SetReturnParameterData(1, &usNbOfPrecedingSamples, sizeof(usNbOfPrecedingSamples));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_EnableTrigger()
{

    BYTE ubTriggerType = 0;

    //Parameter
    SetParameterData(0, &ubTriggerType, sizeof(ubTriggerType));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_DisableAllTriggers()
{

    //Parameter
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_ActivateChannel()
{

    BYTE ubChannelNumber = 0;
    WORD usObjectIndex = 0;
    BYTE ObjectSubIndex = 0;
    BYTE ObjectSize = 0;

    //Parameter
    SetParameterData(0, &ubChannelNumber, sizeof(ubChannelNumber));
    SetParameterData(1, &usObjectIndex, sizeof(usObjectIndex));
    SetParameterData(2, &ObjectSubIndex, sizeof(ObjectSubIndex));
    SetParameterData(3, &ObjectSize, sizeof(ObjectSize));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_DeactivateAllChannels()
{

    //Parameter
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_StartRecorder()
{

    //Parameter
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_StopRecorder()
{

    //Parameter
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_ForceTrigger()
{

    //Parameter
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_IsRecorderRunning()
{

    BOOL oRunning = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oRunning, sizeof(oRunning));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_IsRecorderTriggered()
{

    BOOL oTriggered = FALSE;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &oTriggered, sizeof(oTriggered));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_ReadChannelVectorSize()
{

    DWORD ulVectorSize = 512;

    //Parameter

    //ReturnParameter
    SetReturnParameterData(0, &ulVectorSize, sizeof(ulVectorSize));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_ReadChannelDataVector()
{

    BYTE ubChannelNumber = 0;
    DWORD ulVectorSize = 0;
    BYTE* pDataVector = NULL;

    //Parameter
    SetParameterData(0, &ubChannelNumber, sizeof(ubChannelNumber));
    SetParameterData(1, &ulVectorSize, sizeof(ulVectorSize));

    //ReturnParameter
    SetReturnParameterData(0, pDataVector, ulVectorSize);
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_ReadDataBuffer()
{

    DWORD ulBufferSizeToRead = 0;
    DWORD ulBufferSizeRead = 0;
    BYTE* pDataBuffer = NULL;
    WORD usVectorStartOffset = 0;
    WORD usMaxNbOfSamples = 0;
    WORD usNbOfRecordedSamples = 0;

    //Parameter
    SetParameterData(0, &ulBufferSizeToRead, sizeof(ulBufferSizeToRead));

    //ReturnParameter
    SetReturnParameterData(0, pDataBuffer, ulBufferSizeToRead);
    SetReturnParameterData(1, &ulBufferSizeRead, ulBufferSizeRead);
    SetReturnParameterData(2, &usVectorStartOffset, sizeof(usVectorStartOffset));
    SetReturnParameterData(3, &usMaxNbOfSamples, sizeof(usMaxNbOfSamples));
    SetReturnParameterData(4, &usNbOfRecordedSamples, sizeof(usNbOfRecordedSamples));
}

void CCommand_VCS_DataRecorder::SetDefaultParameter_ExtractChannelDataVector()
{

    BYTE ubChannelNumber = 0;
    DWORD ulVectorSize = 0;
    BYTE* pDataVector = NULL;
    DWORD ulBufferSize = 0;
    BYTE* pDataBuffer = NULL;
    WORD usVectorStartOffset = 0;
    WORD usMaxNbOfSamples = 0;
    WORD usNbOfRecordedSamples = 0;

    //Parameter
    SetParameterData(0, &ubChannelNumber, sizeof(ubChannelNumber));
    SetParameterData(1, &ulBufferSize, sizeof(ulBufferSize));
    SetParameterData(2, &ulVectorSize, sizeof(ulVectorSize));
    SetParameterData(3, &usVectorStartOffset, sizeof(usVectorStartOffset));
    SetParameterData(4, &usMaxNbOfSamples, sizeof(usMaxNbOfSamples));
    SetParameterData(5, &usNbOfRecordedSamples, sizeof(usNbOfRecordedSamples));

    //ReturnParameter
    SetReturnParameterData(0, pDataBuffer, ulBufferSize);
    SetReturnParameterData(1, pDataVector, ulVectorSize);
}

