// GatewayEsamToInfoteamSerial.cpp: Implementierung der Klasse CGatewayEsamToInfoteamSerial.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayEsamToInfoteamSerial.h"


#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>
#include "../../../../CommonLayer/Classes/Commands/ProtocolStack/Command_PS_InfoteamSerial.h"
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
CGatewayEsamToInfoteamSerial::CGatewayEsamToInfoteamSerial()
{
    m_pCommand_ProcessProtocolMaxon = NULL;
	m_pCommand_AbortProtocolMaxon = NULL;
    m_pCommand_ProcessProtocol = NULL;
    m_pCommand_SendChunk = NULL;
    m_pCommand_ReceiveData = NULL;

    InitCommands();
    InitErrorHandling();
}

CGatewayEsamToInfoteamSerial::~CGatewayEsamToInfoteamSerial()
{
    DeleteCommands();
}

CGateway* CGatewayEsamToInfoteamSerial::Clone()
{
    CGatewayEsamToInfoteamSerial* pClonedGateway;

    pClonedGateway = new CGatewayEsamToInfoteamSerial();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayEsamToInfoteamSerial& CGatewayEsamToInfoteamSerial::operator=(CGatewayEsamToInfoteamSerial& other)
{
    if(this != &other)
    {
        *((CGatewayDCStoPS*)this) = *((CGatewayDCStoPS*)&other);
    }

    return *this;
}

BOOL CGatewayEsamToInfoteamSerial::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
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
				//ObjectDictionary
				case ESAM_READ_OBJECT: return Process_ReadObject(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_WRITE_OBJECT: return Process_WriteObject(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_INITIATE_SEGMENT_READ: return Process_InitiateSegmentedRead(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_INITIATE_SEGMENT_WRITE: return Process_InitiateSegmentedWrite(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_SEGMENT_READ: return Process_SegmentRead(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_SEGMENT_WRITE: return Process_SegmentWrite(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_ABORT_SEGMENT_TRANSFER: return Process_AbortSegmentedTransfer(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

				//Network Management
				case ESAM_SEND_NMT_SERVICE: return Process_SendNMTService(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

				//General Gateway
				case ESAM_SEND_CAN_FRAME: return Process_SendCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_READ_CAN_FRAME: return Process_ReadCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_REQUEST_CAN_FRAME: return Process_RequestCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

				//Layer Setting Services
				case ESAM_SEND_LSS_FRAME: return Process_SendLSSFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_READ_LSS_FRAME: return Process_ReadLSSFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

				//DeviceControl
				case ESAM_LOGIN: return Process_Login(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_LOGOUT: return Process_Logout(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_SET_STATE: return Process_SetState(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
			}
		}
	}

    return FALSE;
}

BOOL CGatewayEsamToInfoteamSerial::Process_ReadObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x10;
    const BYTE DATA_LENGTH = 5;
    const BYTE RETURN_DATA_LENGTH = 8;
    const BYTE RETURN_ERROR_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];
    memset(maxDataBuffer, 0, MAX_OBJECT_LENGTH);

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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(maxDataBuffer, MAX_OBJECT_LENGTH, pSource, dRetDataBufferLength);
        }
        else if(oResult && (dRetDataBufferLength == RETURN_ERROR_DATA_LENGTH))
        {
            //Only ErrorCode Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

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

BOOL CGatewayEsamToInfoteamSerial::Process_WriteObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;
    const int PARAMETER_INDEX_DATA = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x11;
    const BYTE DATA_LENGTH = 9;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];
    memset(maxDataBuffer, 0, MAX_OBJECT_LENGTH);
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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxDataBuffer, MAX_OBJECT_LENGTH);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));
        CopyData(pDest, &maxDataBuffer, MAX_OBJECT_LENGTH);

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

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

BOOL CGatewayEsamToInfoteamSerial::Process_InitiateSegmentedRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_OBJECT_LENGTH = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x12;
    const BYTE DATA_LENGTH = 5;
    const BYTE RETURN_DATA_LENGTH = 8;
    const BYTE RETURN_ERROR_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE uNodeId = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;
    DWORD dObjectLength = 0;

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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(&dObjectLength, sizeof(dObjectLength), pSource, dRetDataBufferLength);
        }
        else if(oResult && (dRetDataBufferLength == RETURN_ERROR_DATA_LENGTH))
        {
            //Only ErrorCode Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!oResult)
		{
			PS_AbortProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_InitiateSegmentedWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x13;
    const BYTE DATA_LENGTH = 9;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE uNodeId = 0;
    DWORD dObjectLength = 0;
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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wIndex, sizeof(wIndex));
        CopyData(pDest, &uSubIndex, sizeof(uSubIndex));
        CopyData(pDest, &uNodeId, sizeof(uNodeId));
        CopyData(pDest, &dObjectLength, sizeof(dObjectLength));

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

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
			PS_AbortProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_SegmentRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int RETURN_PARAMETER_INDEX_DATA = 2;

    //*Constants PS*
    const BYTE OP_CODE = 0x14;
    const BYTE DATA_LENGTH = 1;
    const BYTE MIN_RETURN_DATA_LENGTH = 5;
    const BYTE MAX_RETURN_DATA_LENGTH = 132;
    const BYTE RETURN_ERROR_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;
    UEsamControlByte retControlByte;
    retControlByte.ubValue = 0;
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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength >= MIN_RETURN_DATA_LENGTH) && (dRetDataBufferLength <= MAX_RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(&retControlByte.ubValue, sizeof(retControlByte.ubValue), pSource, dRetDataBufferLength);
            if(oResult) pData = pSource;

            //Restore controlByte
            if(retControlByte.structure.ubNbOfDataBytes > dRetDataBufferLength) retControlByte.structure.ubNbOfDataBytes = dRetDataBufferLength;
        }
        else if(oResult && (dRetDataBufferLength == RETURN_ERROR_DATA_LENGTH))
        {
            //Only ErrorCode Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pData, retControlByte.structure.ubNbOfDataBytes);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(retControlByte.structure.ubLastDataSegment || !oResult)
		{
			PS_AbortProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_AbortSegmentedTransfer(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
	BOOL oResult = FALSE;
    
	//Abort Protocol
	oResult = PS_AbortProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle);

	//Unlock CriticalSection
	Unlock();

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_SegmentWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_SEGMENT_LENGTH = 127;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int PARAMETER_INDEX_DATA = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x15;
    const BYTE DATA_LENGTH = 2;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    UEsamControlByte controlByte;
    controlByte.ubValue = 0;
    void* pData = NULL;
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
        //Check CriticalSection
		if(!IsLocked(pCommand)) return FALSE;

		//GetParameterData
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.ubValue, sizeof(controlByte.ubValue));

        //Check Max SegmentLength
        if(controlByte.structure.ubNbOfDataBytes > MAX_SEGMENT_LENGTH) controlByte.structure.ubNbOfDataBytes = MAX_SEGMENT_LENGTH;
        pData = malloc(controlByte.structure.ubNbOfDataBytes);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pData, controlByte.structure.ubNbOfDataBytes);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH + controlByte.structure.ubNbOfDataBytes;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &controlByte.ubValue, sizeof(controlByte.ubValue));
        CopyData(pDest, pData, controlByte.structure.ubNbOfDataBytes);

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Free DataBuffer
        if(pData) free(pData);
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(controlByte.structure.ubLastDataSegment || !oResult)
		{
			PS_AbortProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle);
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_SendNMTService(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_NODE_ID = 1;
    const int PARAMETER_INDEX_CMD_SPECIFIER = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x0E;
    const BYTE DATA_LENGTH = 5;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    WORD wId = 0;
    WORD wCmdSpecifier = 0;
    //ReturnParameter
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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &wId, sizeof(wId));
        pCommand->GetParameterData(PARAMETER_INDEX_CMD_SPECIFIER, &wCmdSpecifier, sizeof(wCmdSpecifier));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wId, sizeof(wId));
        CopyData(pDest, &wCmdSpecifier, sizeof(wCmdSpecifier));

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
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

BOOL CGatewayEsamToInfoteamSerial::Process_SendCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_COBID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;
    const int PARAMETER_INDEX_DATA = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x20;
    const BYTE DATA_LENGTH = 13;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    WORD wCobId = 0;
    WORD wLength = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &wCobId, sizeof(wCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &wLength, sizeof(wLength));

        //Limit Length
        if(wLength > MAX_FRAME_LENGTH) wLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wCobId, sizeof(wCobId));
        CopyData(pDest, &wLength, sizeof(wLength));
        CopyData(pDest, maxFrameBuffer, MAX_FRAME_LENGTH);

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
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

BOOL CGatewayEsamToInfoteamSerial::Process_ReadCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
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
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, ulDataBufferLength, &pRetDataBuffer, &ulRetDataBufferLength, &comErrorInfo);

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

BOOL CGatewayEsamToInfoteamSerial::Process_RequestCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_COBID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x21;
    const BYTE DATA_LENGTH = 54;
    const BYTE RETURN_DATA_LENGTH = 12;
    const BYTE RETURN_ERROR_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &wCobId, sizeof(wCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &wLength, sizeof(wLength));

        //Limit Length
        if(wLength > MAX_FRAME_LENGTH) wLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wCobId, sizeof(wCobId));
        CopyData(pDest, &wLength, sizeof(wLength));

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(maxFrameBuffer, wLength, pSource, dRetDataBufferLength);
        }
        else if(oResult && (dRetDataBufferLength == RETURN_ERROR_DATA_LENGTH))
        {
            //Only ErrorCode Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_SendLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_DATA = 1;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE OP_CODE = 0x30;
    const BYTE DATA_LENGTH = 9;
    const BYTE RETURN_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];
    DWORD dLength;
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uOpCode = OP_CODE;
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;
	BYTE ubKeepLock(0);

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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));

        //Limit Length
        dLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(dLength < MAX_FRAME_LENGTH) dLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxFrameBuffer, dLength);

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, maxFrameBuffer, MAX_FRAME_LENGTH);

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
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

BOOL CGatewayEsamToInfoteamSerial::Process_ReadLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_TIMEOUT = 1;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE OP_CODE = 0x31;
    const BYTE DATA_LENGTH = 3;
    const BYTE RETURN_DATA_LENGTH = 12;
    const BYTE RETURN_ERROR_DATA_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
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
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &wTimeout, sizeof(wTimeout));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPortNumber, sizeof(uPortNumber));
        CopyData(pDest, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = PS_ProcessProtocolMaxon(pProtocolStackManager, hPS_Handle, hTransactionHandle, uOpCode, pDataBuffer, dDataBufferLength, ubKeepLock, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength == RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(maxFrameBuffer, MAX_FRAME_LENGTH, pSource, dRetDataBufferLength);
        }
        else if(oResult && (dRetDataBufferLength == RETURN_ERROR_DATA_LENGTH))
        {
            //Only ErrorCode Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&dDeviceErrorCode, sizeof(dDeviceErrorCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, dDeviceErrorCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, MAX_FRAME_LENGTH);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_Login(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_MAX_RECEIVE_SIZE = 0;
    const int RETURN_PARAMETER_INDEX_RETURN_CODE = 0;

    //*Constants PS*
    const BYTE PACKAGE_TYPE = 0x01;
    const BYTE CMD_ID = 0x01;
    const BYTE DATA_LENGTH = 5;
    const BYTE MIN_RETURN_DATA_LENGTH = 2;

    //*Variables DCS*
    //Parameter
    WORD wMaxReceiveSize = 0;
    //ReturnParameter
    BYTE uReturnCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uPackageType = PACKAGE_TYPE;
    BYTE uCmdId = CMD_ID;
    BYTE uParameterLen = sizeof(WORD);
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;

    //ReturnParameter
    BYTE uRetPackageType;
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
        pCommand->GetParameterData(PARAMETER_INDEX_MAX_RECEIVE_SIZE, &wMaxReceiveSize, sizeof(wMaxReceiveSize));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPackageType, sizeof(uPackageType));
        CopyData(pDest, &uCmdId, sizeof(uCmdId));
        CopyData(pDest, &uParameterLen, sizeof(uParameterLen));
        CopyData(pDest, &wMaxReceiveSize, sizeof(wMaxReceiveSize));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, pCommand->GetTimeout(), pDataBuffer, dDataBufferLength, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength >= MIN_RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&uRetPackageType, sizeof(uRetPackageType), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(&uReturnCode, sizeof(uReturnCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, (DWORD)uReturnCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_RETURN_CODE, &uReturnCode, sizeof(uReturnCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_Logout(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int RETURN_PARAMETER_INDEX_RETURN_CODE = 0;

    //*Constants PS*
    const BYTE PACKAGE_TYPE = 0x01;
    const BYTE CMD_ID = 0x01;
    const BYTE DATA_LENGTH = 3;
    const BYTE MIN_RETURN_DATA_LENGTH = 2;

    //*Variables DCS*
    //ReturnParameter
    BYTE uReturnCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uPackageType = PACKAGE_TYPE;
    BYTE uCmdId = CMD_ID;
    BYTE uParameterLen = 0;
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;

    //ReturnParameter
    BYTE uRetPackageType;
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

		//Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPackageType, sizeof(uPackageType));
        CopyData(pDest, &uCmdId, sizeof(uCmdId));
        CopyData(pDest, &uParameterLen, sizeof(uParameterLen));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, pCommand->GetTimeout(), pDataBuffer, dDataBufferLength, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength >= MIN_RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&uRetPackageType, sizeof(uRetPackageType), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(&uReturnCode, sizeof(uReturnCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, (DWORD)uReturnCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_RETURN_CODE, &uReturnCode, sizeof(uReturnCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::Process_SetState(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_RUN_COMMAND = 0;
    const int RETURN_PARAMETER_INDEX_RETURN_CODE = 0;

    //*Constants PS*
    const BYTE PACKAGE_TYPE = 0x01;
    const BYTE CMD_ID = 0x07;
    const BYTE DATA_LENGTH = 4;
    const BYTE MIN_RETURN_DATA_LENGTH = 2;

    //*Variables DCS*
    //Parameter
    BYTE uRunCommand = 0;
    //ReturnParameter
    BYTE uReturnCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uPackageType = PACKAGE_TYPE;
    BYTE uCmdId = CMD_ID;
    BYTE uParameterLen = sizeof(BYTE);
    void* pDataBuffer=NULL;
    DWORD dDataBufferLength;

    //ReturnParameter
    BYTE uRetPackageType;
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
        pCommand->GetParameterData(PARAMETER_INDEX_RUN_COMMAND, &uRunCommand, sizeof(uRunCommand));

        //Prepare DataBuffer
        dDataBufferLength = DATA_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Data
        pDest = pDataBuffer;
        CopyData(pDest, &uPackageType, sizeof(uPackageType));
        CopyData(pDest, &uCmdId, sizeof(uCmdId));
        CopyData(pDest, &uParameterLen, sizeof(uParameterLen));
        CopyData(pDest, &uRunCommand, sizeof(uRunCommand));

        //Execute Command
        oResult = PS_ProcessProtocol(pProtocolStackManager, hPS_Handle, hTransactionHandle, pCommand->GetTimeout(), pDataBuffer, dDataBufferLength, &pRetDataBuffer, &dRetDataBufferLength, &comErrorInfo);

        //ReturnData
        if(oResult && (dRetDataBufferLength >= MIN_RETURN_DATA_LENGTH))
        {
            //All Data Received
            pSource = pRetDataBuffer;
            if(oResult) oResult = CopyReturnData(&uRetPackageType, sizeof(uRetPackageType), pSource, dRetDataBufferLength);
            if(oResult) oResult = CopyReturnData(&uReturnCode, sizeof(uReturnCode), pSource, dRetDataBufferLength);
        }
        else if(oResult)
        {
            //Bad Size Received
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived, &comErrorInfo);
            oResult = FALSE;
        }

        //Evaluate ErrorCode
        oResult = EvaluateErrorCode(oResult, (DWORD)uReturnCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_RETURN_CODE, &uReturnCode, sizeof(uReturnCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

//*************************************************************************************
BOOL CGatewayEsamToInfoteamSerial::PS_ProcessProtocolMaxon(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dPackageSize;
    WORD wChunkSize;
    BYTE uLastChunkFlag;
    DWORD dChecksum = 0;        //Calculated internally
    BYTE uPackageType;

    DWORD dRetPackageSize;
    DWORD dRetChecksum;
    BYTE uRetPackageType;

    BOOL oResult = FALSE;
    DWORD dRetDataLength;

    if(m_pCommand_ProcessProtocolMaxon && pProtocolStackManager && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Prepare Parameter
        if(dDataBufferLength > k_MaxChunkSize) dDataBufferLength = k_MaxChunkSize;
        wChunkSize = (WORD)dDataBufferLength + sizeof(uPackageType) + sizeof(uOpCode);
        dPackageSize = wChunkSize;
        uLastChunkFlag = k_FirstAndLastChunk;
        uPackageType = k_PT_MAXON_CMD;

        //Set Parameter Data
        m_pCommand_ProcessProtocolMaxon->ResetStatus();
        m_pCommand_ProcessProtocolMaxon->SetParameterData(0, &dPackageSize, sizeof(dPackageSize));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(1, &wChunkSize, sizeof(wChunkSize));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(2, &uLastChunkFlag, sizeof(uLastChunkFlag));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(3, &dChecksum, sizeof(dChecksum));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(4, &uPackageType, sizeof(uPackageType));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(5, &uOpCode, sizeof(uOpCode));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(6, pDataBuffer, dDataBufferLength);
		m_pCommand_ProcessProtocolMaxon->SetParameterData(7, &ubKeepLock, sizeof(ubKeepLock));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_ProcessProtocolMaxon, hPS_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        dRetDataLength = m_pCommand_ProcessProtocolMaxon->GetReturnParameterLength(3);
        if(pdRetDataBufferLength) *pdRetDataBufferLength = dRetDataLength;
        if(dRetDataLength > 0) *ppRetDataBuffer = malloc(dRetDataLength);

        //Get ReturnParameter Data
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(0, &dRetPackageSize, sizeof(dRetPackageSize));
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(1, &dRetChecksum, sizeof(dRetChecksum));
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(2, &uRetPackageType, sizeof(uRetPackageType));
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(3, *ppRetDataBuffer, dRetDataLength);

        //Get ErrorInfo
        m_pCommand_ProcessProtocolMaxon->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::PS_AbortProtocolMaxon(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

    if(m_pCommand_AbortProtocolMaxon && pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_AbortProtocolMaxon->ResetStatus();
        
        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_AbortProtocolMaxon, hPS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortProtocolMaxon->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::PS_ProcessProtocol(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dTimeout, void* pDataBuffer, DWORD dDataBufferLength, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dPackageSize;
    WORD wChunkSize;
    BYTE uLastChunkFlag;
    DWORD dChecksum = 0;        //Calculated internally

    DWORD dRetPackageSize;
    DWORD dRetChecksum;

    BOOL oResult = FALSE;
    DWORD dRetDataLength;

    if(m_pCommand_ProcessProtocol && pProtocolStackManager && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Prepare Parameter
        if(dDataBufferLength > k_MaxChunkSize) dDataBufferLength = k_MaxChunkSize;
        wChunkSize = (WORD)dDataBufferLength;
        dPackageSize = wChunkSize;
        uLastChunkFlag = k_FirstAndLastChunk;

        //Set Parameter Data
        m_pCommand_ProcessProtocol->ResetStatus();
        m_pCommand_ProcessProtocol->SetTimeout(dTimeout);
        m_pCommand_ProcessProtocol->SetParameterData(0, &dPackageSize, sizeof(dPackageSize));
        m_pCommand_ProcessProtocol->SetParameterData(1, &wChunkSize, sizeof(wChunkSize));
        m_pCommand_ProcessProtocol->SetParameterData(2, &uLastChunkFlag, sizeof(uLastChunkFlag));
        m_pCommand_ProcessProtocol->SetParameterData(3, &dChecksum, sizeof(dChecksum));
        m_pCommand_ProcessProtocol->SetParameterData(4, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_ProcessProtocol, hPS_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        dRetDataLength = m_pCommand_ProcessProtocol->GetReturnParameterLength(2);
        if(pdRetDataBufferLength) *pdRetDataBufferLength = dRetDataLength;
        if(dRetDataLength > 0) *ppRetDataBuffer = malloc(dRetDataLength);

        //Get ReturnParameter Data
        m_pCommand_ProcessProtocol->GetReturnParameterData(0, &dRetPackageSize, sizeof(dRetPackageSize));
        m_pCommand_ProcessProtocol->GetReturnParameterData(1, &dRetChecksum, sizeof(dRetChecksum));
        m_pCommand_ProcessProtocol->GetReturnParameterData(2, *ppRetDataBuffer, dRetDataLength);

        //Get ErrorInfo
        m_pCommand_ProcessProtocol->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToInfoteamSerial::PS_SendData(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dPackageSize;
    WORD wChunkSize;
    BYTE uLastChunkFlag;
    DWORD dChecksum = 0;        //Calculated internally

    BYTE* pData;
    DWORD dTotalSizeToSend = 0;
    DWORD dTotalSizeSended = 0;

    if(m_pCommand_SendChunk && pProtocolStackManager)
    {
        dTotalSizeToSend = dDataBufferLength;
        while(dTotalSizeSended < dTotalSizeToSend)
        {
            //Prepare Parameter
            dPackageSize = dDataBufferLength;
            if((dTotalSizeToSend-dTotalSizeSended) > k_MaxChunkSize) wChunkSize = k_MaxChunkSize; else wChunkSize = (WORD)(dTotalSizeToSend-dTotalSizeSended);
            if((DWORD)wChunkSize == dPackageSize)
            {
                //FirstAndLastChunk
                uLastChunkFlag = k_FirstAndLastChunk;
            }
            else if(dTotalSizeToSend == dPackageSize)
            {
                //FirstChunk
                uLastChunkFlag = k_FirstChunk;
            }
            else if(wChunkSize == dTotalSizeToSend)
            {
                //LastChunk
                uLastChunkFlag = k_LastChunk;
            }
            else
            {
                //NormalChunk
                uLastChunkFlag = k_NormalChunk;
            }
            pData = (BYTE*)pDataBuffer + dTotalSizeSended;

            //Set Parameter Data
            m_pCommand_SendChunk->ResetStatus();
            m_pCommand_SendChunk->SetParameterData(0, &dPackageSize, sizeof(dPackageSize));
            m_pCommand_SendChunk->SetParameterData(1, &wChunkSize, sizeof(wChunkSize));
            m_pCommand_SendChunk->SetParameterData(2, &uLastChunkFlag, sizeof(uLastChunkFlag));
            m_pCommand_SendChunk->SetParameterData(3, &dChecksum, sizeof(dChecksum));
            m_pCommand_SendChunk->SetParameterData(4, pData, wChunkSize);

            //Execute Command
            if(pProtocolStackManager->ExecuteCommand(m_pCommand_SendChunk, hPS_Handle, hTransactionHandle))
            {
                dTotalSizeSended += wChunkSize;
            }
            else
            {
                //Get ErrorInfo
                m_pCommand_SendChunk->GetErrorInfo(pErrorInfo);
                return FALSE;
            }
        }

        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEsamToInfoteamSerial::PS_ReceiveData(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dRetPackageSize;
    DWORD dRetChecksum;

    BOOL oResult = FALSE;
    DWORD dRetDataLength;

    if(m_pCommand_ReceiveData && pProtocolStackManager && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Set Parameter Data
        m_pCommand_ReceiveData->ResetStatus();

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_ReceiveData, hPS_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        dRetDataLength = m_pCommand_ReceiveData->GetReturnParameterLength(2);
        if(pdRetDataBufferLength) *pdRetDataBufferLength = dRetDataLength;
        if(dRetDataLength > 0) *ppRetDataBuffer = malloc(dRetDataLength);

        //Get ReturnParameter Data
        m_pCommand_ReceiveData->GetReturnParameterData(0, &dRetPackageSize, sizeof(dRetPackageSize));
        m_pCommand_ReceiveData->GetReturnParameterData(1, &dRetChecksum, sizeof(dRetChecksum));
        m_pCommand_ReceiveData->GetReturnParameterData(2, *ppRetDataBuffer, dRetDataLength);

        //Get ErrorInfo
        m_pCommand_ReceiveData->GetErrorInfo(pErrorInfo);
    }

    return oResult;

}

BOOL CGatewayEsamToInfoteamSerial::EvaluateErrorCode(BOOL oResult, DWORD dDeviceErrorCode, CErrorInfo* pComErrorInfo, CErrorInfo* pCmdErrorInfo)
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

void CGatewayEsamToInfoteamSerial::InitCommands()
{
    DeleteCommands();

    //Process Protocol Maxon
    m_pCommand_ProcessProtocolMaxon = new CCommand_PS_InfoteamSerial();
    m_pCommand_ProcessProtocolMaxon->InitCommand(INFOTEAM_SERIAL_PROCESS_PROTOCOL_MAXON);

	//Abort Protocol Maxon
    m_pCommand_AbortProtocolMaxon = new CCommand_PS_InfoteamSerial();
    m_pCommand_AbortProtocolMaxon->InitCommand(INFOTEAM_SERIAL_ABORT_PROTOCOL_MAXON);
	m_pCommand_AbortProtocolMaxon->EnableTransactionEvent(FALSE);

	//Process Protocol
    m_pCommand_ProcessProtocol = new CCommand_PS_InfoteamSerial();
    m_pCommand_ProcessProtocol->InitCommand(INFOTEAM_SERIAL_PROCESS_PROTOCOL);

    //SendChunk
    m_pCommand_SendChunk = new CCommand_PS_InfoteamSerial();
    m_pCommand_SendChunk->InitCommand(INFOTEAM_SERIAL_SEND_CHUNK);

    //ReceiveData
    m_pCommand_ReceiveData = new CCommand_PS_InfoteamSerial();
    m_pCommand_ReceiveData->InitCommand(INFOTEAM_SERIAL_RECEIVE_DATA);
}

void CGatewayEsamToInfoteamSerial::DeleteCommands()
{
    if(m_pCommand_ProcessProtocolMaxon)
    {
        delete m_pCommand_ProcessProtocolMaxon;
        m_pCommand_ProcessProtocolMaxon = NULL;
    }

	if(m_pCommand_AbortProtocolMaxon)
    {
        delete m_pCommand_AbortProtocolMaxon;
        m_pCommand_AbortProtocolMaxon = NULL;
    }

    if(m_pCommand_ProcessProtocol)
    {
        delete m_pCommand_ProcessProtocol;
        m_pCommand_ProcessProtocol = NULL;
    }

    if(m_pCommand_SendChunk)
    {
        delete m_pCommand_SendChunk;
        m_pCommand_SendChunk = NULL;
    }

    if(m_pCommand_ReceiveData)
    {
        delete m_pCommand_ReceiveData;
        m_pCommand_ReceiveData = NULL;
    }
}

BOOL CGatewayEsamToInfoteamSerial::CopyData(void*& pDest, void* pSource, DWORD sourceSize)
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

BOOL CGatewayEsamToInfoteamSerial::CopyReturnData(void* pDest, DWORD destSize, void*& pSource, DWORD& sourceSize)
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
            sourceSize -= destSize;

            return TRUE;
        }
    }

    return FALSE;
}

/************************************************************************/
BOOL CGatewayEsamToInfoteamSerial::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayEsamToInfoteamSerial";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEsamToInfoteamSerial::InitLayerParameterStack(CCommandRoot* p_pCommand)
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
			if(m_pCommand_ProcessProtocolMaxon) m_pCommand_ProcessProtocolMaxon->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_AbortProtocolMaxon) m_pCommand_AbortProtocolMaxon->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_SendChunk) m_pCommand_SendChunk->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReceiveData) m_pCommand_ReceiveData->SetLayerParameterStack(layerParameterStack);
			oResult = TRUE;
		}
	}
    
	return oResult;
}


