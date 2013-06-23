// GatewayMaxonSerialV2ToUSB.cpp: Implementierung der Klasse CGatewayMaxonSerialV2ToUSB.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayMaxonSerialV2ToUSB.h"


#include <malloc.h>
#include <Process/MmcProcess.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_USB.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
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
CGatewayMaxonSerialV2ToUSB::CGatewayMaxonSerialV2ToUSB()
	: m_pCommand_WriteData(NULL)
    , m_pCommand_ReadData(NULL)
{
    InitCommands();
    InitErrorHandling();
}

CGatewayMaxonSerialV2ToUSB::~CGatewayMaxonSerialV2ToUSB()
{
    DeleteCommands();
}

void CGatewayMaxonSerialV2ToUSB::DeleteCommands()
{
    if(m_pCommand_ReadData)
    {
        delete m_pCommand_ReadData;
        m_pCommand_ReadData = NULL;
    }

    if(m_pCommand_WriteData)
    {
        delete m_pCommand_WriteData;
        m_pCommand_WriteData = NULL;
    }
}

void CGatewayMaxonSerialV2ToUSB::InitCommands()
{
    DeleteCommands();

    //Init Command WriteData
    m_pCommand_WriteData = new CCommand_I_USB();
    m_pCommand_WriteData->InitCommand(USB_WRITE_DATA);

    //Init Command ReadData
    m_pCommand_ReadData = new CCommand_I_USB();
    m_pCommand_ReadData->InitCommand(USB_READ_DATA);
}

BOOL CGatewayMaxonSerialV2ToUSB::Process_ProcessProtocol(CCommand_PS* pCommand, CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle)
{
    const DWORD MAX_RETRY(2);

    //*Constants PS*
    const int PARAMETER_INDEX_OP_CODE(0);
    const int PARAMETER_INDEX_LEN(1);
    const int PARAMETER_INDEX_DATA(2);
    const int PARAMETER_INDEX_CRC(3);
	const int PARAMETER_INDEX_KEEP_LOCK(4);
    const int RETURN_PARAMETER_INDEX_OP_CODE(0);
    const int RETURN_PARAMETER_INDEX_LEN(1);
    const int RETURN_PARAMETER_INDEX_DATA(2);
    const int RETURN_PARAMETER_INDEX_CRC(3);

    //*Variables PS*
    //Parameter
    BYTE uOpCode(0);
    BYTE uLen(0);
    void* pDataBuffer(NULL);
    DWORD dDataBufferLength(0);
    WORD wCrc(0);
	BYTE ubKeepLock(0);

    //ReturnParameter
    BYTE uRetOpCode(0);
    BYTE uRetLen(0);
    void* pRetDataBuffer(NULL);
    DWORD dRetDataBufferLength(0);
    WORD wRetCrc(0);

    //Retry
    BOOL oExecuteRetry(FALSE);
    DWORD dRetryCount(0);

    //SendFrame
    BOOL oSendResult(FALSE);
    CErrorInfo sendErrorInfo;

    //ReceiveFrame
    BOOL oReceiveResult(FALSE);
    CErrorInfo receiveErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    DWORD dTimeout;

    if(pCommand && p_pInterfaceManager)
    {
        //Lock CriticalSection
		if(!IsLocked(pCommand))
		{
			if(!Lock(pCommand)) return FALSE;
		}

		//Prepare DataBuffer
        dDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
		if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

		//Prepare Return DataBuffer
		dRetDataBufferLength = pCommand->GetReturnParameterLength(RETURN_PARAMETER_INDEX_DATA);
		if(dRetDataBufferLength > 0) pRetDataBuffer = malloc(dRetDataBufferLength);

        //Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_OP_CODE, &uOpCode, sizeof(uOpCode));
        pCommand->GetParameterData(PARAMETER_INDEX_LEN, &uLen, sizeof(uLen));
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);
        pCommand->GetParameterData(PARAMETER_INDEX_CRC, &wCrc, sizeof(wCrc));
		pCommand->GetParameterData(PARAMETER_INDEX_KEEP_LOCK, &ubKeepLock, sizeof(ubKeepLock));

        //Timeout
        dTimeout = pCommand->GetTimeout();

        //Execute Command
        while(!oResult && !m_oAbortCommands)
        {
            //Reset
            sendErrorInfo.Reset();
            receiveErrorInfo.Reset();

            //Send and Receive
            oSendResult = SendFrame(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, uOpCode, &uLen, &pDataBuffer, &dDataBufferLength, &wCrc, &sendErrorInfo);
            oReceiveResult = ReceiveFrame(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, &uRetOpCode, &uRetLen, &pRetDataBuffer, &dRetDataBufferLength, &wRetCrc, dTimeout, &receiveErrorInfo);

            //Evaluate Retry
            oResult = EvaluateWriteRetry(oSendResult, sendErrorInfo, oReceiveResult, receiveErrorInfo, oExecuteRetry, errorInfo);
            if(oExecuteRetry)
            {
                dRetryCount++;
                if(dRetryCount > MAX_RETRY) break;
            }
            else
            {
                //Don't retry
                break;
            }
        }

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetParameterData(PARAMETER_INDEX_LEN, &uLen, sizeof(uLen));
        pCommand->SetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);
        pCommand->SetParameterData(PARAMETER_INDEX_CRC, &wCrc, sizeof(wCrc));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_OP_CODE, &uRetOpCode, sizeof(uRetOpCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_LEN, &uRetLen, sizeof(uRetLen));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pRetDataBuffer, dRetDataBufferLength);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CRC, &wRetCrc, sizeof(wRetCrc));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!ubKeepLock) Unlock();
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToUSB::EvaluateWriteRetry(BOOL p_oSendResult, CErrorInfo& p_rSendErrorInfo, BOOL p_oReceiveResult, CErrorInfo& p_rReceiveErrorInfo, BOOL& p_roExecuteRetry, CErrorInfo& p_rErrorInfo)
{
    static BOOL oSendFrameSizeErrorDetected(FALSE);
    BOOL oResult(FALSE);

    if(p_oSendResult)
    {
        //SendFrame succeeded
        if(p_oReceiveResult)
        {
            //ReceiveFrame succeeded -> Don't Retry
            oSendFrameSizeErrorDetected = FALSE;
            p_roExecuteRetry = FALSE;
            p_rErrorInfo = p_rReceiveErrorInfo;
            oResult = TRUE;
        }
        else
        {
            //ReceiveFrame failed
            if(oSendFrameSizeErrorDetected)
            {
                //Retry
                MmcSleep(5);

                p_roExecuteRetry = TRUE;
                p_rErrorInfo = p_rReceiveErrorInfo;
                oResult = FALSE;
            }
            else
            {
                //Don't Retry
                p_roExecuteRetry = FALSE;
                p_rErrorInfo = p_rReceiveErrorInfo;
                oResult = FALSE;
            }
        }
    }
    else
    {
        //SendFrame failed
        if(p_oReceiveResult)
        {
            //ReceiveFrame succeeded -> Don't Retry
            oSendFrameSizeErrorDetected = TRUE;
            p_roExecuteRetry = FALSE;
            p_rErrorInfo = p_rReceiveErrorInfo;
            oResult = TRUE;
        }
        else
        {
            //ReceiveFrame failed -> Retry
            MmcSleep(5);

            p_roExecuteRetry = TRUE;
            p_rErrorInfo = p_rSendErrorInfo;
            oResult = FALSE;
        }
    }

    return oResult;
}

CGateway* CGatewayMaxonSerialV2ToUSB::Clone()
{
    CGatewayMaxonSerialV2ToUSB* pClonedGateway;

    pClonedGateway = new CGatewayMaxonSerialV2ToUSB();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayMaxonSerialV2ToUSB& CGatewayMaxonSerialV2ToUSB::operator=(CGatewayMaxonSerialV2ToUSB& other)
{
    if(this != &other)
    {
        *((CGatewayMaxonSerialV2ToI*)this) = *((CGatewayMaxonSerialV2ToI*)&other);
    }

    return *this;
}

BOOL CGatewayMaxonSerialV2ToUSB::I_ReadData(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, void* pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);
    DWORD ulDataLength(0);

    if(m_pCommand_ReadData && p_pInterfaceManager)
    {
        //Set Parameter Data
        m_pCommand_ReadData->ResetStatus();
        m_pCommand_ReadData->SetParameterData(0, &dNumberOfBytesToRead, sizeof(dNumberOfBytesToRead));

        //Execute Command
        oResult = p_pInterfaceManager->ExecuteCommand(m_pCommand_ReadData, p_hI_Handle, p_hTransactionHandle);

        //Limit Parameter Length
        ulDataLength = m_pCommand_ReadData->GetReturnParameterLength(0);
        if(ulDataLength > dNumberOfBytesToRead) ulDataLength = dNumberOfBytesToRead;

        //Get ReturnParameter Data
        m_pCommand_ReadData->GetReturnParameterData(0, pData, ulDataLength);
        if(pdNumberOfBytesRead) *pdNumberOfBytesRead = ulDataLength;

        //Get ErrorCode
        m_pCommand_ReadData->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToUSB::I_WriteData(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, void* pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_WriteData && p_pInterfaceManager)
    {
        //Set Parameter Data
        m_pCommand_WriteData->ResetStatus();
        m_pCommand_WriteData->SetParameterData(0, pData, dNumberOfBytesToWrite);

        //Execute Command
        oResult = p_pInterfaceManager->ExecuteCommand(m_pCommand_WriteData, p_hI_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteData->GetReturnParameterData(0, pdNumberOfBytesWritten, sizeof(*pdNumberOfBytesWritten));

        //Get ErrorCode
        m_pCommand_WriteData->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToUSB::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName(_T("GatewayMaxonSerialV2ToUSB"));

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayMaxonSerialV2ToUSB::InitLayerParameterStack(CCommandRoot* p_pCommand)
{
	CLayerParameterStack layerParameterStack;
	CLayerParameterSet layerParameterSet;
	BOOL oResult = FALSE;

	if(p_pCommand)
	{
		if(p_pCommand->GetLayerParameterStack(layerParameterStack))
		{
			//Pop PS Layer Parameter Set
			layerParameterStack.PopLayer(PROTOCOL_STACK_LAYER, layerParameterSet);

			//Set I Layer Commands
			if(m_pCommand_WriteData) m_pCommand_WriteData->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadData) m_pCommand_ReadData->SetLayerParameterStack(layerParameterStack);
			oResult = TRUE;
		}
	}

	return oResult;
}


