// GatewayEsam2ToMaxonSerialV2.cpp: Implementierung der Klasse CGatewayEsam2ToMaxonSerialV2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayEsam2ToMaxonSerialV2.h"


#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>
#include "../../../../CommonLayer/Classes/Commands/ProtocolStack/Command_PS_MaxonSerialV2.h"
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayEsam2ToMaxonSerialV2::CGatewayEsam2ToMaxonSerialV2():
    m_pCommand_ProcessProtocol(NULL),
    m_pCommand_AbortProtocol(NULL)
{
    InitCommands();
    InitErrorHandling();
}

CGatewayEsam2ToMaxonSerialV2::~CGatewayEsam2ToMaxonSerialV2()
{
    DeleteCommands();
}

CGateway* CGatewayEsam2ToMaxonSerialV2::Clone()
{
    CGatewayEsam2ToMaxonSerialV2* pClonedGateway;

    pClonedGateway = new CGatewayEsam2ToMaxonSerialV2();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayEsam2ToMaxonSerialV2& CGatewayEsam2ToMaxonSerialV2::operator=(CGatewayEsam2ToMaxonSerialV2& other)
{
    if(this != &other)
    {
        *((CGatewayDCStoPS*)this) = *((CGatewayDCStoPS*)&other);
    }

    return *this;
}

BOOL CGatewayEsam2ToMaxonSerialV2::ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    CCommand_DCS* pCommand_DCS;
    CProtocolStackManagerBase* pProtocolStackManager;

    if(CGateway::ProcessCommand(p_pCommand, p_pManager, p_hHandle, p_hTransactionHandle))
    {
        if(CheckLayers(p_pCommand, p_pManager))
        {
            pCommand_DCS = (CCommand_DCS*)p_pCommand;
            pProtocolStackManager = (CProtocolStackManagerBase*)p_pManager;

            switch(p_pCommand->GetCommandId())
            {
                //OpenPCS History Data Commands
				case ESAM2_OPCS_HIST_GET_RANGE: return Process_OpcsHistGetRange(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
				case ESAM2_OPCS_HIST_GET_RANGE_RESULT: return Process_OpcsHistGetRangeResult(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);

				//Object Dictionary Access Commands
                case ESAM2_READ_OBJECT: return Process_ReadObject(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_WRITE_OBJECT: return Process_WriteObject(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_INITIATE_SEGMENT_READ: return Process_InitiateSegmentedRead(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_INITIATE_SEGMENT_WRITE: return Process_InitiateSegmentedWrite(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_SEGMENT_READ: return Process_SegmentRead(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_SEGMENT_WRITE: return Process_SegmentWrite(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_ABORT_SEGMENT_TRANSFER: return Process_AbortSegmentedTransfer(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);

                //General Gateway
                case ESAM2_SEND_CAN_FRAME: return Process_SendCANFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_REQUEST_CAN_FRAME: return Process_RequestCANFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_READ_CAN_FRAME: return Process_ReadCANFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_SEND_NMT_SERVICE: return Process_SendNMTService(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);

                //Layer Setting Services
                case ESAM2_SEND_LSS_FRAME: return Process_SendLSSFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_READ_LSS_FRAME: return Process_ReadLSSFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
               
            }
        }
    }

    return FALSE;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_ReadObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE MAX_OBJECT_LENGTH(4);

    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_NODE_ID(1);
    const int PARAMETER_INDEX_INDEX(2);
    const int PARAMETER_INDEX_SUB_INDEX(3);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int RETURN_PARAMETER_INDEX_DATA(1);

    //*Constants PS*
    const BYTE OP_CODE(0x40);
    const BYTE DATA_LENGTH(6);
    const BYTE RETURN_DATA_LENGTH(8);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usIndex(0);
    BYTE ubSubIndex(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    BYTE pubMaxDataBuffer[MAX_OBJECT_LENGTH];

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength(0);
    BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest = NULL;
    void* pSource = NULL;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &usIndex, sizeof(usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &ubSubIndex, sizeof(ubSubIndex));

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &ubNodeId, sizeof(ubNodeId));
        CopyData(pDest, &usIndex, sizeof(usIndex));
        CopyData(pDest, &ubSubIndex, sizeof(ubSubIndex));

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult)
        {
            if(ulRetDataBufferLength != RETURN_DATA_LENGTH)
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadDataSizeReceived, &comErrorInfo);
                oResult = FALSE;
            }
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(pubMaxDataBuffer, MAX_OBJECT_LENGTH, pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pubMaxDataBuffer, MAX_OBJECT_LENGTH);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        if(ulDeviceErrorCode != ERROR_DEVICE_ESAM2_SEGMENTED_TRANSFER_REQUIRED_INIT_DONE)
        {
            PS_AbortProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle);
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_WriteObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE MAX_OBJECT_LENGTH(4);

    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_NODE_ID(1);
    const int PARAMETER_INDEX_INDEX(2);
    const int PARAMETER_INDEX_SUB_INDEX(3);
    const int PARAMETER_INDEX_DATA(4);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);

    //*Constants PS*
    const BYTE OP_CODE(0x48);
    const BYTE DATA_LENGTH(10);
    const BYTE RETURN_DATA_LENGTH(4);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usIndex(0);
    BYTE ubSubIndex(0);
    BYTE pubMaxDataBuffer[MAX_OBJECT_LENGTH];
    DWORD ulObjectLength(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength(0);
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &usIndex, sizeof(usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &ubSubIndex, sizeof(ubSubIndex));

        //Check max objectLength
        ulObjectLength = p_pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(ulObjectLength > MAX_OBJECT_LENGTH) ulObjectLength = MAX_OBJECT_LENGTH;
        memset(pubMaxDataBuffer, 0, MAX_OBJECT_LENGTH);
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pubMaxDataBuffer, ulObjectLength);

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &ubNodeId, sizeof(ubNodeId));
        CopyData(pDest, &usIndex, sizeof(usIndex));
        CopyData(pDest, &ubSubIndex, sizeof(ubSubIndex));
        CopyData(pDest, &pubMaxDataBuffer, MAX_OBJECT_LENGTH);

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (ulRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_InitiateSegmentedRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_NODE_ID(1);
    const int PARAMETER_INDEX_INDEX(2);
    const int PARAMETER_INDEX_SUB_INDEX(3);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int RETURN_PARAMETER_INDEX_OBJECT_LENGTH(1);

    //*Constants PS*
    const BYTE OP_CODE(0x41);
    const BYTE DATA_LENGTH(6);
    const BYTE RETURN_DATA_LENGTH(8);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usIndex(0);
    BYTE ubSubIndex(0);

    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    DWORD ulObjectLength(0);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &usIndex, sizeof(usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &ubSubIndex, sizeof(ubSubIndex));

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &ubNodeId, sizeof(ubNodeId));
        CopyData(pDest, &usIndex, sizeof(usIndex));
        CopyData(pDest, &ubSubIndex, sizeof(ubSubIndex));


        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (ulRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&ulObjectLength, sizeof(ulObjectLength), pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_OBJECT_LENGTH, &ulObjectLength, sizeof(ulObjectLength));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        if(!oResult)
        {
            PS_AbortProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle);
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_InitiateSegmentedWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_NODE_ID(1);
    const int PARAMETER_INDEX_INDEX(2);
    const int PARAMETER_INDEX_SUB_INDEX(3);
    const int PARAMETER_INDEX_OBJECT_LENGTH(4);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);

    //*Constants PS*
    const BYTE OP_CODE(0x49);
    const BYTE DATA_LENGTH(10);
    const BYTE RETURN_DATA_LENGTH(4);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    BYTE ubNodeId(0);
    WORD usIndex(0);
    BYTE ubSubIndex(0);
    DWORD ulObjectLength(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &usIndex, sizeof(usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &ubSubIndex, sizeof(ubSubIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &ulObjectLength, sizeof(ulObjectLength));

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &ubNodeId, sizeof(ubNodeId));
        CopyData(pDest, &usIndex, sizeof(usIndex));
        CopyData(pDest, &ubSubIndex, sizeof(ubSubIndex));
        CopyData(pDest, &ulObjectLength, sizeof(ulObjectLength));

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (ulRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        if(!oResult)
        {
            PS_AbortProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle);
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_SegmentRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_CONTROL_BYTE(0);
    const int PARAMETER_INDEX_DUMMY_BYTE(1);

    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int RETURN_PARAMETER_INDEX_LENGTH(1);
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE(2);
    const int RETURN_PARAMETER_INDEX_DATA(3);

    //*Constants PS*
    const BYTE OP_CODE(0x42);
    const BYTE DATA_LENGTH(2);
    const DWORD MIN_RETURN_DATA_LENGTH(6);
    const DWORD MAX_RETURN_DATA_LENGTH(262);

    //*Variables DCS*
    //Parameter
    UEsam2ControlByte controlByte;
    controlByte.ubValue = 0;
    BYTE ubDummyByte(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    BYTE ubLength(0);
    UEsam2ControlByte retControlByte;
    retControlByte.ubValue = 0;
    void* pData(NULL);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(p_pCommand)
    {
        //Check CriticalSection
        if(!IsLocked(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.ubValue, sizeof(controlByte.ubValue));
        p_pCommand->GetParameterData(PARAMETER_INDEX_DUMMY_BYTE, &ubDummyByte, sizeof(ubDummyByte));

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &controlByte.ubValue, sizeof(controlByte.ubValue));
        CopyData(pDest, &ubDummyByte, sizeof(ubDummyByte));

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && ((ulRetDataBufferLength < MIN_RETURN_DATA_LENGTH)||(ulRetDataBufferLength > MAX_RETURN_DATA_LENGTH)))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&ubLength, sizeof(ubLength), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&retControlByte.ubValue, sizeof(retControlByte.ubValue), pSource, ulRetDataBufferLength);
        if(oResult) pData = pSource;

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Restore controlByte
        if(controlByte.structure.ubToggle != retControlByte.structure.ubToggle)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(ERROR_DEVICE_EPOS_TOGGLE, &cmdErrorInfo);
            oResult = FALSE;
        }
        if(ubLength > ulRetDataBufferLength) ubLength = (BYTE)ulRetDataBufferLength;

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_LENGTH, &ubLength, sizeof(ubLength));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pData, ubLength);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        if(retControlByte.structure.ubLastDataSegment || !oResult)
        {
            PS_AbortProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle);
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_SegmentWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE MAX_SEGMENT_LENGTH(255);

    //*Constants DCS*
    const int PARAMETER_INDEX_LENGTH(0);
    const int PARAMETER_INDEX_CONTROL_BYTE(1);
    const int PARAMETER_INDEX_DATA(2);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int PARAMETER_INDEX_LENGTH_WRITTEN(1);
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE(2);

    //*Constants PS*
    const BYTE OP_CODE(0x4A);
    const BYTE DATA_LENGTH(2);
    const BYTE RETURN_DATA_LENGTH(6);

    //*Variables DCS*
    //Parameter
    BYTE ubLength(0);
    UEsam2ControlByte controlByte;
    controlByte.ubValue = 0;
    void* pData(NULL);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    BYTE ubLengthWritten(0);
    UEsam2ControlByte retControlByte;
    retControlByte.ubValue = 0;

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(p_pCommand)
    {
        //Check CriticalSection
        if(!IsLocked(p_pCommand)) return FALSE;

        //GetParameterData
        p_pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &ubLength, sizeof(ubLength));
        p_pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.ubValue, sizeof(controlByte.ubValue));

        //Check Max SegmentLength
        if(ubLength > MAX_SEGMENT_LENGTH) ubLength = MAX_SEGMENT_LENGTH;
        pData = malloc(ubLength);
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pData, ubLength);

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH + ubLength;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &ubLength, sizeof(ubLength));
        CopyData(pDest, &controlByte.ubValue, sizeof(controlByte.ubValue));
        CopyData(pDest, pData, ubLength);

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (ulRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&ubLengthWritten, sizeof(ubLengthWritten), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&retControlByte.ubValue, sizeof(retControlByte.ubValue), pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Restore controlByte
        if(oResult && (controlByte.structure.ubToggle != retControlByte.structure.ubToggle))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(ERROR_DEVICE_EPOS_TOGGLE, &cmdErrorInfo);
            oResult = FALSE;
        }

        //SetReturnParameterData
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(PARAMETER_INDEX_LENGTH_WRITTEN, &ubLengthWritten, sizeof(ubLengthWritten));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));

        //Free DataBuffer
        if(pData) free(pData);
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        if(controlByte.structure.ubLastDataSegment || !oResult)
        {
            PS_AbortProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle);
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_AbortSegmentedTransfer(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    //Abort Protocol
    oResult = PS_AbortProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle);

    //Unlock CriticalSection
    Unlock();

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_SendCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH(8);

    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_COBID(1);
    const int PARAMETER_INDEX_LENGTH(2);
    const int PARAMETER_INDEX_DATA(3);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);

    //*Constants PS*
    const BYTE OP_CODE(0x50);
    const BYTE DATA_LENGTH(14);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    WORD usCobId(0);
    WORD usLength(0);
    BYTE pubMaxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD ulDeviceErrorCode(0);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength(0);
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest(NULL);
    void* pSource(NULL);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_COBID, &usCobId, sizeof(usCobId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &usLength, sizeof(usLength));

        //Limit Length
        if(usLength > MAX_FRAME_LENGTH) usLength = MAX_FRAME_LENGTH;
        memset(pubMaxFrameBuffer, 0, MAX_FRAME_LENGTH);
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pubMaxFrameBuffer, usLength);

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &usCobId, sizeof(usCobId));
        CopyData(pDest, &usLength, sizeof(usLength));
        CopyData(pDest, pubMaxFrameBuffer, MAX_FRAME_LENGTH);

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_ReadCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;
    DWORD ulDeviceErrorCode(k_Error_FunctionNotSupported);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(TRUE, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_RequestCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH(8);

    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_COBID(1);
    const int PARAMETER_INDEX_LENGTH(2);
    const int PARAMETER_INDEX_TIMEOUT(3);

    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int RETURN_PARAMETER_INDEX_DATA(1);

    //*Constants PS*
    const BYTE OP_CODE(0x51);
    const BYTE DATA_LENGTH(6);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    WORD usCobId(0);
    WORD usLength(0);
    DWORD ulTimeout(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    BYTE pubMaxFrameBuffer[MAX_FRAME_LENGTH];

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength(0);
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(8);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest(NULL);
    void* pSource(NULL);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_COBID, &usCobId, sizeof(usCobId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &usLength, sizeof(usLength));
        p_pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &ulTimeout, sizeof(ulTimeout));

        //Limit Length
        if(usLength > MAX_FRAME_LENGTH) usLength = MAX_FRAME_LENGTH;
        memset(pubMaxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &usCobId, sizeof(usCobId));
        CopyData(pDest, &usLength, sizeof(usLength));
        CopyData(pDest, &ulTimeout, sizeof(ulTimeout));

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(pubMaxFrameBuffer, usLength, pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pubMaxFrameBuffer, usLength);

        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_SendNMTService(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE RESET_NODE(129);
    const BYTE RESET_COMMUNICATION(130);

    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_NODE_ID(1);
    const int PARAMETER_INDEX_CMD_SPECIFIER = 2;

    //*Constants PS*
    const BYTE OP_CODE(0x4B);
    const BYTE DATA_LENGTH(4);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    BYTE ubId(0);
    BYTE ubCmdSpecifier(0);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest(NULL);
    void* pSource(NULL);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubId, sizeof(ubId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_CMD_SPECIFIER, &ubCmdSpecifier, sizeof(ubCmdSpecifier));

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &ubId, sizeof(ubId));
        CopyData(pDest, &ubCmdSpecifier, sizeof(ubCmdSpecifier));

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //No EndAck when ResetNode or ResetCommunication
        if((ubCmdSpecifier == RESET_NODE) || (ubCmdSpecifier == RESET_COMMUNICATION))
        {
            oResult = TRUE;
            comErrorInfo.Reset();
        }

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);

        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_SendLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH(8);

    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_DATA(1);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);

    //*Constants PS*
    const BYTE OP_CODE(0x54);
    const BYTE DATA_LENGTH(10);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    BYTE pubMaxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD ulDeviceErrorCode(0);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest(NULL);
    void* pSource(NULL);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        ulDataBufferLength = p_pCommand->GetParameterLength(PARAMETER_INDEX_DATA);

        //Limit Length
        if(ulDataBufferLength > MAX_FRAME_LENGTH) ulDataBufferLength = MAX_FRAME_LENGTH;
        memset(pubMaxFrameBuffer, 0, MAX_FRAME_LENGTH);
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pubMaxFrameBuffer, ulDataBufferLength);

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, pubMaxFrameBuffer, MAX_FRAME_LENGTH);

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        if(pRetDataBuffer) free(pRetDataBuffer);
        if(pDataBuffer) free(pDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_ReadLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH(8);

    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID(0);
    const int PARAMETER_INDEX_TIMEOUT(1);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int RETURN_PARAMETER_INDEX_DATA(1);

    //*Constants PS*
    const BYTE OP_CODE(0x55);
    const BYTE DATA_LENGTH(4);

    //*Variables DCS*
    //Parameter
    WORD usNetworkId(0);
    WORD usTimeout(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    BYTE pubMaxFrameBuffer[MAX_FRAME_LENGTH];

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &usTimeout, sizeof(usTimeout));

        //Limit Length
        memset(pubMaxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNetworkId, sizeof(usNetworkId));
        CopyData(pDest, &usTimeout, sizeof(usTimeout));

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(pubMaxFrameBuffer, MAX_FRAME_LENGTH, pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pubMaxFrameBuffer, MAX_FRAME_LENGTH);

        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::Process_OpcsHistGetRange(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NB_OF_HIST_ELEMENTS(0);
    const int PARAMETER_INDEX_ELEMENT_IDS(1);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int RETURN_PARAMETER_INDEX_SIZE_OF_HIST_DATA(1);

    //*Constants PS*
    const BYTE OP_CODE(0x80);

    //*Variables DCS*
    //Parameter
    WORD usNbOfHistElements(0);
    WORD* pElementIdBuffer(NULL);
	DWORD ulElementIdBufferSize(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    DWORD ulSizeOfHistData(0);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest = NULL;
    void* pSource = NULL;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NB_OF_HIST_ELEMENTS, &usNbOfHistElements, sizeof(usNbOfHistElements));

		//Allocate Buffer
		ulElementIdBufferSize = usNbOfHistElements * sizeof(WORD);
        pElementIdBuffer = (WORD*)malloc(ulElementIdBufferSize);
        p_pCommand->GetParameterData(PARAMETER_INDEX_ELEMENT_IDS, pElementIdBuffer, ulElementIdBufferSize);

        //Prepare DataBuffer
        ulDataBufferLength = sizeof(usNbOfHistElements) + ulElementIdBufferSize;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usNbOfHistElements, sizeof(usNbOfHistElements));
        CopyData(pDest, pElementIdBuffer, ulElementIdBufferSize);

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&ulSizeOfHistData, sizeof(ulSizeOfHistData), pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_SIZE_OF_HIST_DATA, &ulSizeOfHistData, sizeof(ulSizeOfHistData));

        if(pElementIdBuffer) free(pElementIdBuffer);
		if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}
    
BOOL CGatewayEsam2ToMaxonSerialV2::Process_OpcsHistGetRangeResult(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
	//*Constants DCS*
    const int PARAMETER_INDEX_HIST_DATA_OFFSET(0);
    const int PARAMETER_INDEX_SIZE_OF_HIST_DATA(1);
	const int PARAMETER_INDEX_DONE(2);
    const int RETURN_PARAMETER_INDEX_ERROR_CODE(0);
    const int RETURN_PARAMETER_INDEX_HIST_DATA(1);

    //*Constants PS*
    const BYTE OP_CODE(0x81);

    //*Variables DCS*
    //Parameter
    DWORD ulHistDataOffset(0);
	WORD usSizeOfHistData(0);
	BYTE uDone(0);
    //Return Parameter
    DWORD ulDeviceErrorCode(0);
    BYTE* pHistData(NULL);

    //*Variables PS*
    //Parameter
    BYTE ubOpCode(OP_CODE);
    void* pDataBuffer(NULL);
    DWORD ulDataBufferLength;
    BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer(NULL);
    DWORD ulRetDataBufferLength(0);

    BOOL oResult(FALSE);
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest = NULL;
    void* pSource = NULL;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_HIST_DATA_OFFSET, &ulHistDataOffset, sizeof(ulHistDataOffset));
		p_pCommand->GetParameterData(PARAMETER_INDEX_SIZE_OF_HIST_DATA, &usSizeOfHistData, sizeof(usSizeOfHistData));
		p_pCommand->GetParameterData(PARAMETER_INDEX_DONE, &uDone, sizeof(uDone));
		
        //Prepare DataBuffer
        ulDataBufferLength = sizeof(ulHistDataOffset) + sizeof(usSizeOfHistData) + sizeof(uDone);
        pDataBuffer = malloc(ulDataBufferLength);

		//Prepare Return DataBuffer
        ulRetDataBufferLength = sizeof(ulDeviceErrorCode) + usSizeOfHistData;
        pRetDataBuffer = malloc(ulRetDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &ulHistDataOffset, sizeof(ulHistDataOffset));
        CopyData(pDest, &usSizeOfHistData, sizeof(usSizeOfHistData));
		CopyData(pDest, &uDone, sizeof(uDone));

        //Execute Command
        oResult = PS_ProcessProtocol(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
		if(oResult) pHistData = (BYTE*)pSource;
        
        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_HIST_DATA, pHistData, usSizeOfHistData);

		if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::EvaluateErrorCode(BOOL p_oResult, DWORD p_ulDeviceErrorCode, CErrorInfo* p_pComErrorInfo, CErrorInfo* p_pCmdErrorInfo)
{
    if(p_pCmdErrorInfo && p_pComErrorInfo)
    {
        if(!p_oResult)
        {
            *p_pCmdErrorInfo = *p_pComErrorInfo;
        }

        if((p_oResult)&&(p_ulDeviceErrorCode != 0))
        {
            p_pCmdErrorInfo->Init(p_ulDeviceErrorCode);
            p_oResult = FALSE;
        }
    }

    return p_oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::PS_ProcessProtocol(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, BYTE p_ubOpCode, void* p_pDataBuffer, DWORD p_ulDataBufferLength, BYTE p_ubKeepLock, void** p_ppRetDataBuffer, DWORD* p_pulRetDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    BYTE ubLen(0); //Calculated internally
    WORD usCrc(0); //Calucated internally

    BYTE ubRetLen(0);
    BYTE ubRetOpCode(0);
    WORD usRetCrc(0);

    BOOL oResult(FALSE);

    if(m_pCommand_ProcessProtocol && p_pProtocolStackManager && p_ppRetDataBuffer && p_pulRetDataBufferLength)
    {
        //Init Return Parameter Data
		m_pCommand_ProcessProtocol->SetReturnParameterData(2, *p_ppRetDataBuffer, *p_pulRetDataBufferLength);

		//Set Parameter Data
        m_pCommand_ProcessProtocol->ResetStatus();
        m_pCommand_ProcessProtocol->SetParameterData(0, &p_ubOpCode, sizeof(p_ubOpCode));
        m_pCommand_ProcessProtocol->SetParameterData(1, &ubLen, sizeof(ubLen));
        m_pCommand_ProcessProtocol->SetParameterData(2, p_pDataBuffer, p_ulDataBufferLength);
        m_pCommand_ProcessProtocol->SetParameterData(3, &usCrc, sizeof(usCrc));
        m_pCommand_ProcessProtocol->SetParameterData(4, &p_ubKeepLock, sizeof(p_ubKeepLock));

		//Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_ProcessProtocol, p_hPS_Handle, p_hTransactionHandle);

        //Prepare RetDataBuffer
		if(*p_ppRetDataBuffer == NULL)
		{
			*p_pulRetDataBufferLength = m_pCommand_ProcessProtocol->GetReturnParameterLength(2);
			if(*p_pulRetDataBufferLength > 0) *p_ppRetDataBuffer = malloc(*p_pulRetDataBufferLength);
		}

        //Get ReturnParameter Data
        m_pCommand_ProcessProtocol->GetReturnParameterData(0, &ubRetOpCode, sizeof(ubRetOpCode));
        m_pCommand_ProcessProtocol->GetReturnParameterData(1, &ubRetLen, sizeof(ubRetLen));
        m_pCommand_ProcessProtocol->GetReturnParameterData(2, *p_ppRetDataBuffer, *p_pulRetDataBufferLength);
        m_pCommand_ProcessProtocol->GetReturnParameterData(3, &usRetCrc, sizeof(usRetCrc));

        //Get ErrorCode
        m_pCommand_ProcessProtocol->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToMaxonSerialV2::PS_AbortProtocol(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_AbortProtocol && p_pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_AbortProtocol->ResetStatus();

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_AbortProtocol, p_hPS_Handle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortProtocol->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CGatewayEsam2ToMaxonSerialV2::InitCommands()
{
    DeleteCommands();

    //Process Protocol
    m_pCommand_ProcessProtocol = new CCommand_PS_MaxonSerialV2();
    m_pCommand_ProcessProtocol->InitCommand(MAXON_SERIAL_V2_PROCESS_PROTOCOL);

    //Abort Protocol
    m_pCommand_AbortProtocol = new CCommand_PS_MaxonSerialV2();
    m_pCommand_AbortProtocol->InitCommand(MAXON_SERIAL_V2_ABORT_PROTOCOL);
	m_pCommand_AbortProtocol->EnableTransactionEvent(FALSE);

}

void CGatewayEsam2ToMaxonSerialV2::DeleteCommands()
{
    if(m_pCommand_ProcessProtocol)
    {
        delete m_pCommand_ProcessProtocol;
        m_pCommand_ProcessProtocol = NULL;
    }

    if(m_pCommand_AbortProtocol)
    {
        delete m_pCommand_AbortProtocol;
        m_pCommand_AbortProtocol = NULL;
    }
}

BOOL CGatewayEsam2ToMaxonSerialV2::CopyData(void*& p_pDest, void* p_pSource, DWORD p_ulSourceSize)
{
    BYTE* p;

    if(p_pDest && p_pSource)
    {
        //Copy Data
        memcpy(p_pDest, p_pSource, p_ulSourceSize);

        //Move Destination Pointer
        p = (BYTE*)p_pDest;
        p += p_ulSourceSize;
        p_pDest = p;

        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEsam2ToMaxonSerialV2::CopyReturnData(void* p_pDest, DWORD p_ulDestSize, void*& p_pSource, DWORD& p_rulSourceSize)
{
    BYTE* p;

    if(p_pDest && p_pSource)
    {
        if(p_rulSourceSize >= p_ulDestSize)
        {
            //Copy Data
            memcpy(p_pDest, p_pSource, p_ulDestSize);

            //Move Source Pointer
            p = (BYTE*)p_pSource;
            p += p_ulDestSize;
            p_pSource = p;

            return TRUE;
        }
    }

    return FALSE;
}

BOOL CGatewayEsam2ToMaxonSerialV2::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName(_T("GatewayEsam2ToMaxonSerialV2"));

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEsam2ToMaxonSerialV2::InitLayerParameterStack(CCommandRoot* p_pCommand)
{
    CLayerParameterStack layerParameterStack;
    CLayerParameterSet layerParameterSet;
    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        if(p_pCommand->GetLayerParameterStack(layerParameterStack))
        {
            //Pop DCS Layer Parameter Set
            layerParameterStack.PopLayer(DEVICE_COMMAND_SET_LAYER, layerParameterSet);

            //Set PS Layer Commands
            if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->SetLayerParameterStack(layerParameterStack);
            if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->SetLayerParameterStack(layerParameterStack);

            oResult = TRUE;
        }
    }

    return oResult;
}


