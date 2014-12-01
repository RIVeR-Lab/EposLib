// GatewayMaxonSerialV2ToI.cpp: Implementierung der Klasse CGatewayMaxonSerialV2ToI.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayMaxonSerialV2ToI.h"
#include <Process/MmcProcess.h>
#include <malloc.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_MaxonSerialV2.h>
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include "Frame/MaxonSerialV2Frame.h"

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
CGatewayMaxonSerialV2ToI::CGatewayMaxonSerialV2ToI()
    : m_pBlockBuffer(NULL)
    , m_ulBlockBufferSize(0)
     ,m_ulGetBlockBufferIndex(0)
{
    m_pSendingFrame = new CMaxonSerialV2Frame();
    m_pReceivingFrame = new CMaxonSerialV2Frame();
}

CGatewayMaxonSerialV2ToI::~CGatewayMaxonSerialV2ToI()
{
    DeleteBlockBuffer();
    if(m_pSendingFrame) delete m_pSendingFrame;
    if(m_pReceivingFrame) delete m_pReceivingFrame;
}

BOOL CGatewayMaxonSerialV2ToI::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    CCommand_PS* pCommand_PS;
    CInterfaceManagerBase* pInterfaceManager;

    if(CGateway::ProcessCommand(pCommand, pManager, h_Handle, hTransactionHandle))
	{
		if(CheckLayers(pCommand, pManager))
		{
			pCommand_PS = (CCommand_PS*)pCommand;
			pInterfaceManager = (CInterfaceManagerBase*)pManager;

			switch(pCommand->GetCommandId())
			{
				case MAXON_SERIAL_V2_PROCESS_PROTOCOL: return Process_ProcessProtocol(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case MAXON_SERIAL_V2_ABORT_PROTOCOL: return Process_AbortProtocol(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
			}
		}
	}

    return FALSE;
}

BOOL CGatewayMaxonSerialV2ToI::Process_ProcessProtocol(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_OPCODE(0);
    const int PARAMETER_INDEX_LEN(1);
    const int PARAMETER_INDEX_DATA(2);
    const int PARAMETER_INDEX_CRC(3);
	const int PARAMETER_INDEX_KEEP_LOCK(4);
    const int RETURN_PARAMETER_INDEX_OPCODE(0);
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

    BOOL oResult(FALSE);
    CErrorInfo errorInfo;
    DWORD dTimeout;

    if(pCommand && pInterfaceManager)
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
        pCommand->GetParameterData(PARAMETER_INDEX_OPCODE, &uOpCode, sizeof(uOpCode));
        pCommand->GetParameterData(PARAMETER_INDEX_LEN, &uLen, sizeof(uLen));
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);
        pCommand->GetParameterData(PARAMETER_INDEX_CRC, &wCrc, sizeof(wCrc));
		pCommand->GetParameterData(PARAMETER_INDEX_KEEP_LOCK, &ubKeepLock, sizeof(ubKeepLock));

        //Execute Command
        dTimeout = pCommand->GetTimeout();
        if(SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, uOpCode, &uLen, &pDataBuffer, &dDataBufferLength, &wCrc, &errorInfo))
        {
            oResult = ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, &uRetOpCode, &uRetLen, &pRetDataBuffer, &dRetDataBufferLength, &wRetCrc, dTimeout, &errorInfo);
        }

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetParameterData(PARAMETER_INDEX_LEN, &uLen, sizeof(uLen));
        pCommand->SetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);
        pCommand->SetParameterData(PARAMETER_INDEX_CRC, &wCrc, sizeof(wCrc));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_OPCODE, &uRetOpCode, sizeof(uRetOpCode));
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

BOOL CGatewayMaxonSerialV2ToI::Process_AbortProtocol(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    CErrorInfo errorInfo;
	BOOL oResult = FALSE;
	
	if(pCommand)
	{
		//Unlock CriticalSection
		Unlock();
		oResult = TRUE;

		//Set PS ReturnParameter Data
		pCommand->SetStatus(oResult,&errorInfo);
	}

	return oResult;
}

CGateway* CGatewayMaxonSerialV2ToI::Clone()
{
    CGatewayMaxonSerialV2ToI* pClonedGateway;

    pClonedGateway = new CGatewayMaxonSerialV2ToI();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayMaxonSerialV2ToI& CGatewayMaxonSerialV2ToI::operator=(CGatewayMaxonSerialV2ToI& other)
{
    if(this != &other)
    {
        *((CGatewayPStoI*)this) = *((CGatewayPStoI*)&other);
    }

    return *this;
}

BOOL CGatewayMaxonSerialV2ToI::SendFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, BYTE* puLen, void** ppDataBuffer, DWORD* pdDataBufferLength, WORD* pwCrc, CErrorInfo* pErrorInfo)
{
    DWORD ulToWrite(0);
    DWORD ulWritten(0);    // number of bytes written
    BOOL oResult(FALSE);

    if(pInterfaceManager && hI_Handle && m_pSendingFrame)
    {
        //Reset
        oResult = TRUE;
        if(pErrorInfo) pErrorInfo->Reset();

        // 0. Initialization
        pInterfaceManager->I_ResetInterface(hI_Handle);
        if(oResult && !m_pSendingFrame->PrepareStuffedSendFrame(uOpCode, puLen, ppDataBuffer, pdDataBufferLength, pwCrc))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            oResult = FALSE;
        }

        // 1. Send DLE, STX, OpCode, Len, Data, Crc
        ulToWrite = m_pSendingFrame->GetFrameSize();
        if(oResult && !I_WriteData(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame->GetFramePointer(), ulToWrite, &ulWritten, pErrorInfo))
        {
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_SendFrame, pErrorInfo);
            oResult = FALSE;
        }
        if(oResult && (ulWritten != ulToWrite))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadDataSizeWritten, pErrorInfo);
            oResult = FALSE;
        }
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        oResult = FALSE;
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::ReceiveFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE* puOpCode, BYTE* puLen, void** ppDataBuffer, DWORD* pdDataBufferLength, WORD* pwCrc, DWORD dTimeout, CErrorInfo* pErrorInfo)
{
    EReceiveFrameState receiveFrameState(RFS_WAITING_FOR_DLE);
    BYTE uDle(0);
    BYTE uStx(0);
    BYTE uOpCode(0);
    BYTE uLen(0);
    BYTE* pData(NULL);
    DWORD dNbOfDataBytesToRead(0);
    DWORD dReadBlockSize(0);
    BOOL oEscapeState(FALSE);
    unsigned long dStartTime(0);
    DWORD dTime(0);
    BOOL oResult(FALSE);

    if(pInterfaceManager && hI_Handle && ppDataBuffer && pdDataBufferLength && m_pReceivingFrame)
    {
        //1. Reset
        oResult = TRUE;
        DeleteBlockBuffer();
        if(pErrorInfo) pErrorInfo->Reset();

        //Init

        dStartTime = MmcGetTickCount();

        dTime = 0;
        InitWaitingForDleState(receiveFrameState, uDle, dReadBlockSize);

        //2. Read Frame
        while(oResult && (receiveFrameState != RFS_FRAME_RECEIVED))
        {
            if(receiveFrameState == RFS_WAITING_FOR_DLE)
            {
                //Waiting for DLE
                if(dTime < dTimeout)
                {
                    //Timeout not elapsed
                    if(ProcessWaitingForDle(pInterfaceManager, hI_Handle, hTransactionHandle, receiveFrameState, uDle, dReadBlockSize, pErrorInfo))
                    {
                        //Init Next States
                        if(receiveFrameState == RFS_WAITING_FOR_STX) InitWaitingForStxState(receiveFrameState, uStx, dReadBlockSize);
                    }

                    //Update Time

                    dTime = MmcGetTickCount()-dStartTime;
                }
                else
                {
                    //Timeout elapsed
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_ReceiveFrame, pErrorInfo);
                    oResult = FALSE;
                }
            }
            else if(receiveFrameState == RFS_WAITING_FOR_STX)
            {
                //Waiting for STX
                if(oResult = ProcessWaitingForStx(pInterfaceManager, hI_Handle, hTransactionHandle, receiveFrameState, uStx, dReadBlockSize, pErrorInfo))
                {
                    //Init Next States
                    if(receiveFrameState == RFS_WAITING_FOR_DLE) InitWaitingForDleState(receiveFrameState, uDle, dReadBlockSize);
                    if(receiveFrameState == RFS_WAITING_FOR_OPCODE) InitWaitingForOpCodeState(receiveFrameState, uOpCode, dReadBlockSize, oEscapeState);
                }
            }
            else if(receiveFrameState == RFS_WAITING_FOR_OPCODE)
            {
                //Waiting for OpCode
                if(oResult = ProcessWaitingForOpCode(pInterfaceManager, hI_Handle, hTransactionHandle, receiveFrameState, uOpCode, dReadBlockSize, oEscapeState, pErrorInfo))
                {
                    //Init Next States
                    if(receiveFrameState == RFS_WAITING_FOR_LEN) InitWaitingForLenState(receiveFrameState, uLen, dReadBlockSize, oEscapeState);
                }
            }
            else if(receiveFrameState == RFS_WAITING_FOR_LEN)
            {
                //Waiting for Len
                if(oResult = ProcessWaitingForLen(pInterfaceManager, hI_Handle, hTransactionHandle, receiveFrameState, uLen, dReadBlockSize, oEscapeState, pErrorInfo))
                {
                    //Init Next States
                    if(receiveFrameState == RFS_WAITING_FOR_OPCODE) InitWaitingForOpCodeState(receiveFrameState, uOpCode, dReadBlockSize, oEscapeState);
                    if(receiveFrameState == RFS_WAITING_FOR_DATA_AND_CRC) InitWaitingForDataAndCrcState(receiveFrameState, m_pReceivingFrame, uOpCode, uLen, *pdDataBufferLength, pData, dNbOfDataBytesToRead, dReadBlockSize, oEscapeState);
                }
            }
            else if(receiveFrameState == RFS_WAITING_FOR_DATA_AND_CRC)
            {
                //Waiting for Data and Crc
                if(oResult = ProcessWaitingForDataAndCrc(pInterfaceManager, hI_Handle, hTransactionHandle, receiveFrameState, pData, dNbOfDataBytesToRead, dReadBlockSize, oEscapeState, pErrorInfo))
                {
                    //Init Next States
                    if(receiveFrameState == RFS_WAITING_FOR_OPCODE) InitWaitingForOpCodeState(receiveFrameState, uOpCode, dReadBlockSize, oEscapeState);
                }
            }
        }

        //3. Check CRC
        if(oResult)
        {
            if(m_pReceivingFrame->CheckCRC())
            {
                //CRC okay
                if(!m_pReceivingFrame->CopyReceivedData(puOpCode, puLen, ppDataBuffer, pdDataBufferLength, pwCrc))
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_ReceiveFrame, pErrorInfo);
                    oResult = FALSE;
                }
            }
            else
            {
                //CRC bad
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_BadCrcReceived, pErrorInfo);
                oResult = FALSE;
            }
        }
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        oResult = FALSE;
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::InitWaitingForDleState(EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruDle, DWORD& p_rulReadBlockSize)
{
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        oResult = TRUE;
        p_rReceiveFrameState = RFS_WAITING_FOR_DLE;
        p_ruDle = 0;
        p_rulReadBlockSize = m_pReceivingFrame->GetDleSize() + m_pReceivingFrame->GetStxSize() + m_pReceivingFrame->GetOpCodeSize() + m_pReceivingFrame->GetLenSize();
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::InitWaitingForStxState(EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruStx, DWORD& p_rulReadBlockSize)
{
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        oResult = TRUE;
        p_rReceiveFrameState = RFS_WAITING_FOR_STX;
        p_ruStx = 0;
        p_rulReadBlockSize = m_pReceivingFrame->GetStxSize() + m_pReceivingFrame->GetOpCodeSize() + m_pReceivingFrame->GetLenSize();
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::InitWaitingForOpCodeState(EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruOpCode, DWORD& p_rulReadBlockSize, BOOL& p_roEscapeState)
{
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        oResult = TRUE;
        p_rReceiveFrameState = RFS_WAITING_FOR_OPCODE;
        p_ruOpCode = 0;
        p_rulReadBlockSize = m_pReceivingFrame->GetOpCodeSize() + m_pReceivingFrame->GetLenSize();
        p_roEscapeState = FALSE;
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::InitWaitingForLenState(EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruLen, DWORD& p_rulReadBlockSize, BOOL p_roEscapeState)
{
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        oResult = TRUE;
        p_rReceiveFrameState = RFS_WAITING_FOR_LEN;
        p_ruLen = 0;
        p_rulReadBlockSize = m_pReceivingFrame->GetLenSize();
        p_roEscapeState = FALSE;
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::InitWaitingForDataAndCrcState(EReceiveFrameState& p_rReceiveFrameState, CMaxonSerialV2Frame* p_pReceivingFrame, BYTE p_uOpCode, BYTE p_uLen, DWORD p_ulRetDataBufferSize, BYTE*& p_rpData, DWORD& p_rulNbOfDataBytesToRead, DWORD& p_rulReadBlockSize, BOOL& p_roEscapeState)
{
    BOOL oResult(FALSE);

    if(p_pReceivingFrame && p_pReceivingFrame->PrepareReceiveFrame(p_uOpCode, p_uLen, p_ulRetDataBufferSize))
    {
        oResult = TRUE;
        p_rpData = (BYTE*)p_pReceivingFrame->GetDataPointer();
        p_rulNbOfDataBytesToRead = p_pReceivingFrame->GetDataSize() + p_pReceivingFrame->GetCrcSize();
        p_rulReadBlockSize = p_pReceivingFrame->GetDataSize() + p_pReceivingFrame->GetCrcSize();
        p_roEscapeState = FALSE;
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::ProcessWaitingForDle(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruDle, DWORD& p_rulReadBlockSize, CErrorInfo* p_pErrorInfo)
{
    BYTE ubData(0);
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        if(GetReceivedData(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, ubData, p_rulReadBlockSize))
        {
            if(ubData == m_pReceivingFrame->GetDleCharacter())
            {
                //Goto Waiting for STX
                p_rReceiveFrameState = RFS_WAITING_FOR_STX;
                p_ruDle = ubData;
                oResult = TRUE;
            }
            else
            {
                //Goto Waiting for DLE
                p_rReceiveFrameState = RFS_WAITING_FOR_DLE;
                p_ruDle = 0;
                oResult = TRUE;
            }
        }
        else
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_ReceiveFrame, p_pErrorInfo);
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::ProcessWaitingForStx(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruStx, DWORD& p_rulReadBlockSize, CErrorInfo* p_pErrorInfo)
{
    BYTE ubData(0);
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        if(GetReceivedData(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, ubData, p_rulReadBlockSize))
        {
            if(ubData == m_pReceivingFrame->GetStxCharacter())
            {
                //Goto Waiting for OpCode
                p_rReceiveFrameState = RFS_WAITING_FOR_OPCODE;
                p_ruStx = ubData;
                oResult = TRUE;
            }
            else if(ubData == m_pReceivingFrame->GetDleCharacter())
            {
                //Goto Waiting for STX
                p_rReceiveFrameState = RFS_WAITING_FOR_STX;
                p_ruStx = 0;
                oResult = TRUE;
            }
        }
        else
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_ReceiveFrame, p_pErrorInfo);
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::ProcessWaitingForOpCode(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruOpCode, DWORD& p_rulReadBlockSize, BOOL& p_roEscapeState, CErrorInfo* p_pErrorInfo)
{
    BYTE ubData(0);
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        if(GetReceivedData(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, ubData, p_rulReadBlockSize))
        {
            if(p_roEscapeState)
            {
                //Already in Escape State
                if(ubData == m_pReceivingFrame->GetDleCharacter())
                {
                    //Goto Waiting for Len
                    p_rReceiveFrameState = RFS_WAITING_FOR_LEN;
                    p_roEscapeState = FALSE;
                    p_ruOpCode = ubData;
                    oResult = TRUE;
                }
                else if(ubData == m_pReceivingFrame->GetStxCharacter())
                {
                    //Goto Waiting for OpCode
                    p_rReceiveFrameState = RFS_WAITING_FOR_OPCODE;
                    p_roEscapeState = FALSE;
                    p_ruOpCode = 0;
                    oResult = TRUE;
                }
                else
                {
                    //Stuffing Error
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_Destuffing, p_pErrorInfo);
                    oResult = FALSE;
                }
            }
            else
            {
                //Not in Escape State
                if(ubData == m_pReceivingFrame->GetDleCharacter())
                {
                    //Goto Escape OpCode
                    p_rReceiveFrameState = RFS_WAITING_FOR_OPCODE;
                    p_roEscapeState = TRUE;
                    p_ruOpCode = 0;
                    oResult = TRUE;
                }
                else
                {
                    //Goto Waiting for Len
                    p_rReceiveFrameState = RFS_WAITING_FOR_LEN;
                    p_roEscapeState = FALSE;
                    p_ruOpCode = ubData;
                    oResult = TRUE;
                }
            }
        }
        else
        {
            //Failed reading Data
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_ReceiveFrame, p_pErrorInfo);
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::ProcessWaitingForLen(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, EReceiveFrameState& p_rReceiveFrameState, BYTE& p_ruLen, DWORD& p_rulReadBlockSize, BOOL& p_roEscapeState, CErrorInfo* p_pErrorInfo)
{
    BYTE ubData(0);
    BOOL oResult(FALSE);

    if(m_pReceivingFrame)
    {
        if(GetReceivedData(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, ubData, p_rulReadBlockSize))
        {
            if(p_roEscapeState)
            {
                //Already in Escape State
                if(ubData == m_pReceivingFrame->GetDleCharacter())
                {
                    //Goto Waiting for Data and Crc
                    p_rReceiveFrameState = RFS_WAITING_FOR_DATA_AND_CRC;
                    p_roEscapeState = FALSE;
                    p_ruLen = ubData;
                    oResult = TRUE;
                }
                else if(ubData == m_pReceivingFrame->GetStxCharacter())
                {
                    //Goto Waiting for OpCode
                    p_rReceiveFrameState = RFS_WAITING_FOR_OPCODE;
                    p_roEscapeState = FALSE;
                    p_ruLen = 0;
                    oResult = TRUE;
                }
                else
                {
                    //Stuffing Error
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_Destuffing, p_pErrorInfo);
                    oResult = FALSE;
                }
            }
            else
            {
                //Not in Escape State
                if(ubData == m_pReceivingFrame->GetDleCharacter())
                {
                    //Goto Escape Len
                    p_rReceiveFrameState = RFS_WAITING_FOR_LEN;
                    p_roEscapeState = TRUE;
                    p_ruLen = 0;
                    oResult = TRUE;
                }
                else
                {
                    //Goto Waiting for Data and Crc
                    p_rReceiveFrameState = RFS_WAITING_FOR_DATA_AND_CRC;
                    p_roEscapeState = FALSE;
                    p_ruLen = ubData;
                    oResult = TRUE;
                }
            }
        }
        else
        {
            //Failed reading Data
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_ReceiveFrame, p_pErrorInfo);
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::ProcessWaitingForDataAndCrc(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, EReceiveFrameState& p_rReceiveFrameState, BYTE*& p_rpData, DWORD& p_rulNbOfDataBytesToRead, DWORD& p_rulReadBlockSize, BOOL& p_roEscapeState, CErrorInfo* p_pErrorInfo)
{
    BYTE ubData(0);
    BOOL oResult(FALSE);

    if(m_pReceivingFrame && p_rpData)
    {
        if(GetReceivedData(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, ubData, p_rulReadBlockSize))
        {
            if(p_roEscapeState)
            {
                //Already in Escape State
                if(ubData == m_pReceivingFrame->GetDleCharacter())
                {
                    //Copy Data
                    *p_rpData = ubData;
                    p_rpData++;
                    p_rulNbOfDataBytesToRead--;
                    p_rulReadBlockSize--;
                    p_roEscapeState = FALSE;
                    oResult = TRUE;

                    if(p_rulNbOfDataBytesToRead == 0)
                    {
                        //Goto Frame Received
                        p_rReceiveFrameState = RFS_FRAME_RECEIVED;
                    }
                }
                else if(ubData == m_pReceivingFrame->GetStxCharacter())
                {
                    //Goto Waiting for OpCode
                    p_rReceiveFrameState = RFS_WAITING_FOR_OPCODE;
                    p_roEscapeState = FALSE;
                    oResult = TRUE;
                }
                else
                {
                    //Stuffing Error
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_Destuffing, p_pErrorInfo);
                    oResult = FALSE;
                }
            }
            else
            {
                //Not in Escape State
                if(ubData == m_pReceivingFrame->GetDleCharacter())
                {
                    //Goto Escape Len
                    p_rReceiveFrameState = RFS_WAITING_FOR_DATA_AND_CRC;
                    p_roEscapeState = TRUE;
                    oResult = TRUE;
                }
                else
                {
                    //Copy Data
                    *p_rpData = ubData;
                    p_rpData++;
                    p_rulNbOfDataBytesToRead--;
                    p_rulReadBlockSize--;
                    p_roEscapeState = FALSE;
                    oResult = TRUE;

                    if(p_rulNbOfDataBytesToRead == 0)
                    {
                        //Goto Frame Received
                        p_rReceiveFrameState = RFS_FRAME_RECEIVED;
                    }
                }
            }
        }
        else
        {
            //Failed reading Data
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_ReceiveFrame, p_pErrorInfo);
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::GetReceivedData(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, BYTE& p_ruData, DWORD p_ulReadBlockSize)
{
    BOOL oResult(FALSE);

    if(GetDataFromBlockBuffer(p_ruData))
    {
        //Buffered Data from BlockBuffer
        oResult = TRUE;
    }
    else
    {
        //Update BlockBuffer
        if(UpdateBlockBuffer(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, p_ulReadBlockSize))
        {
            oResult = GetDataFromBlockBuffer(p_ruData);
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::InitBlockBuffer(DWORD p_ulReadBlockSize)
{
    BOOL oResult(TRUE);

    DeleteBlockBuffer();
    if(!m_pBlockBuffer)
    {
        m_pBlockBuffer = (BYTE*)malloc(p_ulReadBlockSize);
        m_ulBlockBufferSize = p_ulReadBlockSize;
        m_ulGetBlockBufferIndex = 0;
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::UpdateBlockBuffer(CInterfaceManagerBase* p_pInterfaceManager, HANDLE p_hI_Handle, HANDLE p_hTransactionHandle, DWORD p_ulReadBlockSize)
{
    DWORD ulNumberOfBytesRead(0);
    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(InitBlockBuffer(p_ulReadBlockSize))
    {
        if(I_ReadData(p_pInterfaceManager, p_hI_Handle, p_hTransactionHandle, m_pBlockBuffer, m_ulBlockBufferSize, &ulNumberOfBytesRead, &errorInfo))
        {
            if(m_ulBlockBufferSize == ulNumberOfBytesRead)
            {
                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::DeleteBlockBuffer()
{
    BOOL oResult(TRUE);

    if(m_pBlockBuffer)
    {
        free(m_pBlockBuffer);
        m_pBlockBuffer = NULL;
        m_ulBlockBufferSize = 0;
        m_ulGetBlockBufferIndex = 0;
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::GetDataFromBlockBuffer(BYTE& p_ruData)
{
    BOOL oResult(FALSE);

    if(m_pBlockBuffer)
    {
        if(m_ulGetBlockBufferIndex < m_ulBlockBufferSize)
        {
            p_ruData = *(m_pBlockBuffer+m_ulGetBlockBufferIndex);
            m_ulGetBlockBufferIndex++;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::I_ReadData(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

	//Has to be implementeded by child class
    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::I_WriteData(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

	//Has to be implementeded by child class
    return oResult;
}

BOOL CGatewayMaxonSerialV2ToI::I_WriteStuffedData(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    void* pStuffedData(NULL);
    DWORD dNumberOfStuffedBytesToWrite(0);
    DWORD dNumberOfStuffedBytesWritten(0);
    DWORD dNumberOfStuffedBytes(0);
    BOOL oResult(FALSE);

    //Stuffing Data
    if(CMaxonSerialV2Frame::StuffingData((BYTE*)pData, dNumberOfBytesToWrite, (BYTE*&)pStuffedData, dNumberOfStuffedBytesToWrite, dNumberOfStuffedBytes))
    {
        //Write Data
        if(I_WriteData(pInterfaceManager, hI_Handle, hTransactionHandle, pStuffedData, dNumberOfStuffedBytesToWrite, &dNumberOfStuffedBytesWritten, pErrorInfo))
        {
            if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = dNumberOfStuffedBytesWritten - dNumberOfStuffedBytes;
            oResult = TRUE;
        }

        if(pStuffedData) free(pStuffedData);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV2_Stuffing, pErrorInfo);
        oResult = FALSE;
    }

    return oResult;
}

bool CGatewayMaxonSerialV2ToI::StartTime(double* pStartCount)
{
    //Start Timer
	*pStartCount = m_timer.start();

	return true;
}

DWORD CGatewayMaxonSerialV2ToI::StopTime(double startCount)
{
    //Stop Timer
	m_timer.stop();

    return (DWORD)m_timer.getElapsedTimeInMilliSec();
}
