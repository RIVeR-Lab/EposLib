// GatewayEpos2ToMaxonSerialV1.cpp: Implementierung der Klasse CGatewayEpos2ToMaxonSerialV1.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayEpos2ToMaxonSerialV1.h"


#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEpos2Def.h>
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>
#include "../../../../CommonLayer/Classes/Commands/ProtocolStack/Command_PS_MaxonSerialV1.h"
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
CGatewayEpos2ToMaxonSerialV1::CGatewayEpos2ToMaxonSerialV1()
{
    m_pCommand_ProcessProtocol = NULL;
    m_pCommand_SendFrame = NULL;
	m_pCommand_AbortProtocol = NULL;

    InitCommands();
    InitErrorHandling();
}

CGatewayEpos2ToMaxonSerialV1::~CGatewayEpos2ToMaxonSerialV1()
{
    DeleteCommands();
}

CGateway* CGatewayEpos2ToMaxonSerialV1::Clone()
{
    CGatewayEpos2ToMaxonSerialV1* pClonedGateway;

    pClonedGateway = new CGatewayEpos2ToMaxonSerialV1();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayEpos2ToMaxonSerialV1& CGatewayEpos2ToMaxonSerialV1::operator=(CGatewayEpos2ToMaxonSerialV1& other)
{
    if(this != &other)
    {
        *((CGatewayDCStoPS*)this) = *((CGatewayDCStoPS*)&other);
    }

    return *this;
}

BOOL CGatewayEpos2ToMaxonSerialV1::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    CCommand_DCS* pCommand_DCS;
    CProtocolStackManagerBase* pProtocolStackManager;

    if(CGateway::ProcessCommand(pCommand, pManager, h_Handle, hTransactionHandle))
	{
		if(CheckLayers(pCommand, pManager))
		{
			pCommand_DCS = (CCommand_DCS*)pCommand;
			pProtocolStackManager = (CProtocolStackManagerBase*)pManager;

			switch(pCommand->GetCommandId())
			{
				//Object Dictionary Access Commands
				case EPOS2_READ_OBJECT: return Process_ReadObject(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_WRITE_OBJECT: return Process_WriteObject(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_INITIATE_SEGMENT_READ: return Process_InitiateSegmentedRead(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_INITIATE_SEGMENT_WRITE: return Process_InitiateSegmentedWrite(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_SEGMENT_READ: return Process_SegmentRead(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_SEGMENT_WRITE: return Process_SegmentWrite(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_ABORT_SEGMENT_TRANSFER: return Process_AbortSegmentedTransfer(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

				//Network Management Commands
				case EPOS2_SEND_NMT_SERVICE: return Process_SendNMTService(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

				//General Gateway
				case EPOS2_SEND_CAN_FRAME: return Process_SendCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_READ_CAN_FRAME: return Process_ReadCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_REQUEST_CAN_FRAME: return Process_RequestCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

				//Layer Setting Services
				case EPOS2_SEND_LSS_FRAME: return Process_SendLSSFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case EPOS2_READ_LSS_FRAME: return Process_ReadLSSFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
			}
		}
	}

    return FALSE;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_ReadObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 3;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x10;
    const BYTE DATA_LENGTH = 4;
    const BYTE RETURN_DATA_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    DWORD dObjectLength = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;
        
		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (dRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(maxDataBuffer, MAX_OBJECT_LENGTH, pSource, dRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxDataBuffer, MAX_OBJECT_LENGTH);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_WriteObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int PARAMETER_INDEX_DATA = 3;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x11;
    const BYTE DATA_LENGTH = 8;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];
    DWORD dObjectLength = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Check max objectLength
        if(dObjectLength > MAX_OBJECT_LENGTH) dObjectLength = MAX_OBJECT_LENGTH;
        memset(maxDataBuffer, 0, MAX_OBJECT_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxDataBuffer, dObjectLength);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));
        CopyData(pDest, &maxDataBuffer, MAX_OBJECT_LENGTH);

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (dRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_InitiateSegmentedRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x12;
    const BYTE DATA_LENGTH = 4;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE uNodeId = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (dRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!oResult)
		{
			PS_AbortProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_InitiateSegmentedWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x13;
    const BYTE DATA_LENGTH = 8;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE uNodeId = 0;
    DWORD dObjectLength = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer = NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));
        CopyData(pDest, &dObjectLength, sizeof(dObjectLength));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (dRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!oResult)
		{
			PS_AbortProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_SegmentRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NODE_ID = 0;
    const int PARAMETER_INDEX_CONTROL_BYTE = 1;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int RETURN_PARAMETER_INDEX_DATA = 2;

    //*Constants PS*
    const BYTE OP_CODE = 0x14;
    const BYTE DATA_LENGTH = 2;
    const BYTE MIN_RETURN_DATA_LENGTH = 5;
    const BYTE MAX_RETURN_DATA_LENGTH = 68;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    UEpos2ControlByte controlByte;
    controlByte.bValue = 0;
    BYTE uDummyByte = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;
    UEpos2ControlByte retControlByte;
    retControlByte.bValue = 0;
    void* pData = NULL;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
        //Check CriticalSection
		if(!IsLocked(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.bValue, sizeof(controlByte.bValue));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &controlByte.bValue, sizeof(controlByte.bValue));
        CopyData(pDest, &uDummyByte, sizeof(uDummyByte));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && ((dRetDataBufferLength < MIN_RETURN_DATA_LENGTH)||(dRetDataBufferLength > MAX_RETURN_DATA_LENGTH)))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&retControlByte.bValue, sizeof(retControlByte.bValue), pSource, dRetDataBufferLength);
        if(oResult) pData = pSource;

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Restore controlByte
        if(controlByte.structure.bToggle != retControlByte.structure.bToggle)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(ERROR_DEVICE_EPOS_TOGGLE, &cmdErrorInfo);
            oResult = FALSE;
        }
        if(retControlByte.structure.bLength > dRetDataBufferLength) retControlByte.structure.bLength = dRetDataBufferLength;

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pData, retControlByte.structure.bLength);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!retControlByte.structure.bMoreSegments || !oResult)
		{
			PS_AbortProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_SegmentWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_SEGMENT_LENGTH = 63;

    //*Constants DCS*
    const int PARAMETER_INDEX_NODE_ID = 0;
    const int PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int PARAMETER_INDEX_DATA = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x15;
    const BYTE DATA_LENGTH = 1;
    const BYTE RETURN_DATA_LENGTH = 6;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    UEpos2ControlByte controlByte;
    controlByte.bValue = 0;
    void* pData = NULL;
    //Return Parameter
    UEpos2ControlByte retControlByte;
    retControlByte.bValue = 0;
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(1);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
        //Check CriticalSection
		if(!IsLocked(pCommand)) return FALSE;

		//GetParameterData
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.bValue, sizeof(controlByte.bValue));

        //Check Max SegmentLength
        if(controlByte.structure.bLength > MAX_SEGMENT_LENGTH) controlByte.structure.bLength = MAX_SEGMENT_LENGTH;
        pData = malloc(controlByte.structure.bLength);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pData, controlByte.structure.bLength);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH + controlByte.structure.bLength;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &controlByte.bValue, sizeof(controlByte.bValue));
        CopyData(pDest, pData, controlByte.structure.bLength);

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //Check ReturnData Size
        if(oResult && (dRetDataBufferLength != RETURN_DATA_LENGTH))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(&retControlByte.bValue, sizeof(retControlByte.bValue), pSource, dRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Restore controlByte
        if(oResult && (controlByte.structure.bToggle != retControlByte.structure.bToggle))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(ERROR_DEVICE_EPOS_TOGGLE, &cmdErrorInfo);
            oResult = FALSE;
        }

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));

        //Free DataBuffer
        if(pData) free(pData);
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!controlByte.structure.bMoreSegments || !oResult)
		{
			PS_AbortProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_AbortSegmentedTransfer(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
	BOOL oResult = FALSE;
    
	//Abort Protocol
    oResult = PS_AbortProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle);

	//Unlock CriticalSection
	Unlock();

	return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_SendNMTService(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const WORD RESET_NODE = 129;
    const WORD RESET_COMMUNICATION = 130;

    //*Constants DCS*
    const int PARAMETER_INDEX_NODE_ID = 0;
    const int PARAMETER_INDEX_CMD_SPECIFIER = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x0E;
    const BYTE DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    WORD wId = 0;
    WORD wCmdSpecifier = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength = 0;
	BYTE ubKeepLock(0);

    //ReturnParameter

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest = NULL;
    void* pSource = NULL;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &wId, sizeof(wId));
        pCommand->GetParameterData(PARAMETER_INDEX_CMD_SPECIFIER, &wCmdSpecifier, sizeof(wCmdSpecifier));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wId, sizeof(wId));
        CopyData(pDest, &wCmdSpecifier, sizeof(wCmdSpecifier));

        //Execute Command
        oResult = PS_SendFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, &comErrorInfo);

        //No EndAck when ResetNode or ResetCommunication
        if((wCmdSpecifier == RESET_NODE) || (wCmdSpecifier == RESET_COMMUNICATION))
        {
            oResult = TRUE;
            comErrorInfo.Reset();
        }

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);

        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_SendCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_COBID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int PARAMETER_INDEX_DATA = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x20;
    const BYTE DATA_LENGTH = 12;

    //*Variables DCS*
    //Parameter
    WORD wCobId = 0;
    WORD wLength = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

    //ReturnParameter

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest = NULL;
    void* pSource = NULL;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &wCobId, sizeof(wCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &wLength, sizeof(wLength));

        //Limit Length
        if(wLength > MAX_FRAME_LENGTH) wLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wCobId, sizeof(wCobId));
        CopyData(pDest, &wLength, sizeof(wLength));
        CopyData(pDest, maxFrameBuffer, MAX_FRAME_LENGTH);

        //Execute Command
        oResult = PS_SendFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, &comErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &comErrorInfo);

        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_ReadCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_COBID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int PARAMETER_INDEX_TIMEOUT = 2;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x21;
    const BYTE DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    WORD usCobId = 0;
    WORD usLength = 0;
    DWORD ulTimeout = 0;
    //Return Parameter
    DWORD ulDeviceErrorCode = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer = NULL;
    DWORD ulDataBufferLength = 0;
	BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD ulRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest = NULL;
    void* pSource = NULL;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &usCobId, sizeof(usCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &usLength, sizeof(usLength));
        pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &ulTimeout, sizeof(ulTimeout));

        //Limit Length
        if(usLength > MAX_FRAME_LENGTH) usLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare DataBuffer
        ulDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &usCobId, sizeof(usCobId));
        CopyData(pDest, &usLength, sizeof(usLength));
        CopyData(pDest, &ulTimeout, sizeof(ulTimeout));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, ulDataBufferLength, ubKeepLock, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&ulDeviceErrorCode, sizeof(ulDeviceErrorCode), pSource, ulRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(maxFrameBuffer, usLength, pSource, ulRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, ulDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, usLength);

        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_RequestCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_COBID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x21;
    const BYTE DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    WORD wCobId = 0;
    WORD wLength = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &wCobId, sizeof(wCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &wLength, sizeof(wLength));

        //Limit Length
        if(wLength > MAX_FRAME_LENGTH) wLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wCobId, sizeof(wCobId));
        CopyData(pDest, &wLength, sizeof(wLength));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(maxFrameBuffer, wLength, pSource, dRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_SendLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_DATA = 0;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x30;
    const BYTE DATA_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

    //ReturnParameter

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest = NULL;
    void* pSource = NULL;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        dDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);

        //Limit Length
        if(dDataBufferLength > MAX_FRAME_LENGTH) dDataBufferLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxFrameBuffer, dDataBufferLength);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, maxFrameBuffer, MAX_FRAME_LENGTH);

        //Execute Command
        oResult = PS_SendFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, &comErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);

        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::Process_ReadLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_TIMEOUT = 0;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x31;
    const BYTE DATA_LENGTH = 2;

    //*Variables DCS*
    //Parameter
    WORD wTimeout = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

    //ReturnParameter
    void* pRetDataBuffer=NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo comErrorInfo;
    CErrorInfo cmdErrorInfo;

    void* pDest;
    void* pSource;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &wTimeout, sizeof(wTimeout));

        //Limit Length
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        pSource = pRetDataBuffer;
        if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        if(oResult) oResult = CopyReturnData(maxFrameBuffer, MAX_FRAME_LENGTH, pSource, dRetDataBufferLength);

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, MAX_FRAME_LENGTH);

        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::EvaluateErrorCode(BOOL oResult, DWORD dDeviceErrorCode, CErrorInfo* pComErrorInfo, CErrorInfo* pCmdErrorInfo)
{
    if(pCmdErrorInfo && pComErrorInfo)
    {
        if(!oResult)
        {
            *pCmdErrorInfo = *pComErrorInfo;
        }

        if((oResult)&&(dDeviceErrorCode != 0))
        {
            pCmdErrorInfo->Init(dDeviceErrorCode);
            oResult = FALSE;
        }
    }

    return oResult;
}

//*************************************************************************************
BOOL CGatewayEpos2ToMaxonSerialV1::PS_ProcessProtocol(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    BYTE uLen = 0; //Calculated internally
    WORD wCrc = 0; //Calucated internally

    BYTE uRetLen;
    BYTE uRetOpCode;
    WORD wRetCrc;

    BOOL oResult = FALSE;

    if(m_pCommand_ProcessProtocol && pProtocolStackManager && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Set Parameter Data
        m_pCommand_ProcessProtocol->ResetStatus();
        m_pCommand_ProcessProtocol->SetParameterData(0, &uOpCode, sizeof(uOpCode));
        m_pCommand_ProcessProtocol->SetParameterData(1, &uLen, sizeof(uLen));
        m_pCommand_ProcessProtocol->SetParameterData(2, pDataBuffer, dDataBufferLength);
        m_pCommand_ProcessProtocol->SetParameterData(3, &wCrc, sizeof(wCrc));
		m_pCommand_ProcessProtocol->SetParameterData(4, &ubKeepLock, sizeof(ubKeepLock));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_ProcessProtocol, hPS_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        *pdRetDataBufferLength = m_pCommand_ProcessProtocol->GetReturnParameterLength(2);
        if(*pdRetDataBufferLength > 0) *ppRetDataBuffer = malloc(*pdRetDataBufferLength);

        //Get ReturnParameter Data
        m_pCommand_ProcessProtocol->GetReturnParameterData(0, &uRetOpCode, sizeof(uRetOpCode));
        m_pCommand_ProcessProtocol->GetReturnParameterData(1, &uRetLen, sizeof(uRetLen));
        m_pCommand_ProcessProtocol->GetReturnParameterData(2, *ppRetDataBuffer, *pdRetDataBufferLength);
        m_pCommand_ProcessProtocol->GetReturnParameterData(3, &wRetCrc, sizeof(wRetCrc));

        //Get ErrorCode
        m_pCommand_ProcessProtocol->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::PS_SendFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    BYTE uLen = 0; //Calculated internally
    WORD wCrc = 0; //Calucated internally

    BOOL oResult = FALSE;

    if(m_pCommand_SendFrame && pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_SendFrame->ResetStatus();
        m_pCommand_SendFrame->SetParameterData(0, &uOpCode, sizeof(uOpCode));
        m_pCommand_SendFrame->SetParameterData(1, &uLen, sizeof(uLen));
        m_pCommand_SendFrame->SetParameterData(2, pDataBuffer, dDataBufferLength);
        m_pCommand_SendFrame->SetParameterData(3, &wCrc, sizeof(wCrc));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_SendFrame, hPS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEpos2ToMaxonSerialV1::PS_AbortProtocol(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

    if(m_pCommand_AbortProtocol && pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_AbortProtocol->ResetStatus();
        
        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_AbortProtocol, hPS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortProtocol->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

void CGatewayEpos2ToMaxonSerialV1::InitCommands()
{
    DeleteCommands();

    //Process Protocol
    m_pCommand_ProcessProtocol = new CCommand_PS_MaxonSerialV1();
    m_pCommand_ProcessProtocol->InitCommand(MAXON_SERIAL_V1_PROCESS_PROTOCOL);

    //SendFrame
    m_pCommand_SendFrame = new CCommand_PS_MaxonSerialV1();
    m_pCommand_SendFrame->InitCommand(MAXON_SERIAL_V1_SEND_FRAME);

	//Abort Protocol
    m_pCommand_AbortProtocol = new CCommand_PS_MaxonSerialV1();
    m_pCommand_AbortProtocol->InitCommand(MAXON_SERIAL_V1_ABORT_PROTOCOL);
	m_pCommand_AbortProtocol->EnableTransactionEvent(FALSE);

}

void CGatewayEpos2ToMaxonSerialV1::DeleteCommands()
{
    if(m_pCommand_ProcessProtocol)
    {
        delete m_pCommand_ProcessProtocol;
        m_pCommand_ProcessProtocol = NULL;
    }

    if(m_pCommand_SendFrame)
    {
        delete m_pCommand_SendFrame;
        m_pCommand_SendFrame = NULL;
    }

	if(m_pCommand_AbortProtocol)
    {
        delete m_pCommand_AbortProtocol;
        m_pCommand_AbortProtocol = NULL;
    }
}

BOOL CGatewayEpos2ToMaxonSerialV1::CopyData(void*& pDest, void* pSource, DWORD sourceSize)
{
    BYTE* p;

    if(pDest && pSource)
    {
        //Copy Data
        memcpy(pDest, pSource, sourceSize);

        //Move Destination Pointer
        p = (BYTE*)pDest;
        p += sourceSize;
        pDest = p;
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEpos2ToMaxonSerialV1::CopyReturnData(void* pDest, DWORD destSize, void*& pSource, DWORD& sourceSize)
{
    BYTE* p;

    if(pDest && pSource)
    {
        if(sourceSize >= destSize)
        {
            //Copy Data
            memcpy(pDest, pSource, destSize);

            //Move Source Pointer
            p = (BYTE*)pSource;
            p += destSize;
            pSource = p;

            return TRUE;
        }
    }

    return FALSE;
}

/************************************************************************/
BOOL CGatewayEpos2ToMaxonSerialV1::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayEpos2ToMaxonSerialV1";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEpos2ToMaxonSerialV1::InitLayerParameterStack(CCommandRoot* p_pCommand)
{
	CLayerParameterStack layerParameterStack;
	CLayerParameterSet layerParameterSet;
	BOOL oResult = FALSE;
	
	if(p_pCommand)
	{
		if(p_pCommand->GetLayerParameterStack(layerParameterStack))
		{
			//Pop DCS Layer Parameter Set
			layerParameterStack.PopLayer(DEVICE_COMMAND_SET_LAYER, layerParameterSet);

			//Set PS Layer Commands
			if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_SendFrame) m_pCommand_SendFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->SetLayerParameterStack(layerParameterStack);
			oResult = TRUE;
		}
	}
    
	return oResult;
}


