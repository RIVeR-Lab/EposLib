// GatewayMaxonSerialV1ToStandardI.cpp: Implementierung der Klasse CGatewayMaxonSerialV1ToI.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayMaxonSerialV1ToI.h"


#include <Process/MmcProcess.h>
#include <malloc.h>
#include <fstream>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_MaxonSerialV1.h>
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>
#include "Frame/MaxonSerialV1Frame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayMaxonSerialV1ToI::CGatewayMaxonSerialV1ToI()
{
	m_pSendingFrame = new CMaxonSerialV1Frame();
	m_pReceivingFrame = new CMaxonSerialV1Frame();
}

CGatewayMaxonSerialV1ToI::~CGatewayMaxonSerialV1ToI()
{
	if(m_pSendingFrame) delete m_pSendingFrame;
	if(m_pReceivingFrame) delete m_pReceivingFrame;
}

BOOL CGatewayMaxonSerialV1ToI::ProcessCommand(CCommandRoot* pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle)
{
	CCommand_PS* pCommand_PS;
	CInterfaceManagerBase* pInterfaceManager;

	if(CGateway::ProcessCommand(pCommand, pManager, h_Handle, hTransactionHandle))
	{
		if(CheckLayers(pCommand,pManager))
		{
			pCommand_PS = (CCommand_PS*)pCommand;
			pInterfaceManager = (CInterfaceManagerBase*)pManager;

			switch(pCommand->GetCommandId())
			{
				case MAXON_SERIAL_V1_PROCESS_PROTOCOL: return Process_ProcessProtocol(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
				case MAXON_SERIAL_V1_SEND_FRAME: return Process_SendFrame(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
				case MAXON_SERIAL_V1_ABORT_PROTOCOL: return Process_AbortProtocol(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
			}
		}
	}

	return FALSE;
}

BOOL CGatewayMaxonSerialV1ToI::Process_ProcessProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
{
	//*Constants PS*
	const int k_ParameterIndex_OpCode = 0;
	const int k_ParameterIndex_Len = 1;
	const int k_ParameterIndex_Data = 2;
	const int k_ParameterIndex_Crc = 3;
	const int k_ParameterIndex_KeepLock = 4;
	const int k_ReturnParameterIndex_OpCode = 0;
	const int k_ReturnParameterIndex_Len = 1;
	const int k_ReturnParameterIndex_Data = 2;
	const int k_ReturnParameterIndex_Crc = 3;

	//*Variables PS*
	//Parameter
	BYTE uOpCode;
	BYTE uLen;
	void* pDataBuffer = NULL;
	DWORD dDataBufferLength;
	WORD wCrc;
	BYTE ubKeepLock(0);

	//ReturnParameter
	BYTE uRetOpCode = 0;
	BYTE uRetLen = 0;
	void* pRetDataBuffer = NULL;
	DWORD dRetDataBufferLength = 0;
	WORD wRetCrc = 0;

	BOOL oResult = FALSE;
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
		dDataBufferLength = pCommand->GetParameterLength(k_ParameterIndex_Data);
		if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

		//Get PS Parameter Data
		pCommand->GetParameterData(k_ParameterIndex_OpCode,&uOpCode,sizeof(uOpCode));
		pCommand->GetParameterData(k_ParameterIndex_Len,&uLen,sizeof(uLen));
		pCommand->GetParameterData(k_ParameterIndex_Data,pDataBuffer,pCommand->GetParameterLength(k_ParameterIndex_Data));
		pCommand->GetParameterData(k_ParameterIndex_Crc,&wCrc,sizeof(wCrc));
		pCommand->GetParameterData(k_ParameterIndex_KeepLock,&ubKeepLock,sizeof(ubKeepLock));

		//Execute Command
		dTimeout = pCommand->GetTimeout();
		oResult = SendFrame(pInterfaceManager,hI_Handle,hTransactionHandle,uOpCode,&uLen,&pDataBuffer,&dDataBufferLength,&wCrc,&errorInfo);
		if(oResult)
		{
			oResult = ReceiveFrame(pInterfaceManager,hI_Handle,hTransactionHandle,&uRetOpCode,&uRetLen,&pRetDataBuffer,&dRetDataBufferLength,&wRetCrc,dTimeout,&errorInfo);
		}

		//Set PS ReturnParameter Data
		pCommand->SetStatus(oResult,&errorInfo);
		pCommand->SetParameterData(k_ParameterIndex_Len,&uLen,sizeof(uLen));
		pCommand->SetParameterData(k_ParameterIndex_Data,pDataBuffer,dDataBufferLength);
		pCommand->SetParameterData(k_ParameterIndex_Crc,&wCrc,sizeof(wCrc));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_OpCode,&uRetOpCode,sizeof(uRetOpCode));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Len,&uRetLen,sizeof(uRetLen));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Data,pRetDataBuffer,dRetDataBufferLength);
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Crc,&wRetCrc,sizeof(wRetCrc));

		//Free DataBuffer
		if(pDataBuffer) free(pDataBuffer);
		if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!ubKeepLock) Unlock();

	}

	return oResult;
}

BOOL CGatewayMaxonSerialV1ToI::Process_AbortProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
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

BOOL CGatewayMaxonSerialV1ToI::Process_SendFrame(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
{
	//*Constants PS*
	const int k_ParameterIndex_OpCode = 0;
	const int k_ParameterIndex_Len = 1;
	const int k_ParameterIndex_Data = 2;
	const int k_ParameterIndex_Crc = 3;

	//*Variables PS*
	//Parameter
	BYTE uOpCode;
	BYTE uLen;
	void* pDataBuffer = NULL;
	DWORD dDataBufferLength;
	WORD wCrc;

	BOOL oResult = FALSE;
	CErrorInfo errorInfo;
	DWORD dTimeout;

	if(pCommand && pInterfaceManager)
	{
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;
		
		//Prepare DataBuffer
		dDataBufferLength = pCommand->GetParameterLength(k_ParameterIndex_Data);
		if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

		//Get PS Parameter Data
		pCommand->GetParameterData(k_ParameterIndex_OpCode,&uOpCode,sizeof(uOpCode));
		pCommand->GetParameterData(k_ParameterIndex_Len,&uLen,sizeof(uLen));
		pCommand->GetParameterData(k_ParameterIndex_Data,pDataBuffer,pCommand->GetParameterLength(k_ParameterIndex_Data));
		pCommand->GetParameterData(k_ParameterIndex_Crc,&wCrc,sizeof(wCrc));

		//Execute Command
		dTimeout = pCommand->GetTimeout();
		oResult = SendFrame(pInterfaceManager,hI_Handle,hTransactionHandle,uOpCode,&uLen,&pDataBuffer,&dDataBufferLength,&wCrc,&errorInfo);

		//Set PS ReturnParameter Data
		pCommand->SetStatus(oResult,&errorInfo);
		pCommand->SetParameterData(k_ParameterIndex_Len,&uLen,sizeof(uLen));
		pCommand->SetParameterData(k_ParameterIndex_Data,pDataBuffer,dDataBufferLength);
		pCommand->SetParameterData(k_ParameterIndex_Crc,&wCrc,sizeof(wCrc));

		//Free DataBuffer
		if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
	}

	return oResult;
}



CGateway* CGatewayMaxonSerialV1ToI::Clone()
{
	CGatewayMaxonSerialV1ToI* pClonedGateway;

	pClonedGateway = new CGatewayMaxonSerialV1ToI();
	*pClonedGateway = *this;

	return pClonedGateway;

}

CGatewayMaxonSerialV1ToI& CGatewayMaxonSerialV1ToI::operator=(CGatewayMaxonSerialV1ToI& other)
{
	if(this != &other)
	{
		*((CGatewayPStoI*)this) = *((CGatewayPStoI*)&other);
	}

	return *this;
}

BOOL CGatewayMaxonSerialV1ToI::SendFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,BYTE uOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc,CErrorInfo* pErrorInfo)
{
	BYTE	uReadyAck;				// the ready acknowledge
	BYTE	uEndAck;				// the end acknowledge
	DWORD	dWritten;				// number of bytes written
	DWORD	dRead;					// number of bytes read

	if(pInterfaceManager && hI_Handle && m_pSendingFrame)
	{
		if(pErrorInfo) pErrorInfo->Reset();

		// 0. Initialization
		pInterfaceManager->I_ResetInterface(hI_Handle);
		if(!m_pSendingFrame->PrepareSendFrame(uOpCode,puLen,ppDataBuffer,pdDataBufferLength,pwCrc))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}

		// 1. OpCode byte sending
		if(!I_WriteData(pInterfaceManager,hI_Handle,hTransactionHandle,m_pSendingFrame->GetOpCodePointer(),m_pSendingFrame->GetOpCodeSize(),&dWritten,pErrorInfo)) return FALSE;
		if(dWritten != m_pSendingFrame->GetOpCodeSize())
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}

		// 2. Wait for ready acknowledge
		if(!I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,&uReadyAck,sizeof(uReadyAck),&dRead,pErrorInfo))
		{
			return FALSE;
		}
		// 3. Check ready acknowledge
		if(dRead != 0)
		{
			switch(uReadyAck)
			{
				case OK_ACK: break;
				default:
					{
						if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_NegAckReceived,pErrorInfo);
						return FALSE;
					};
			}
		}
		else
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
		// 4. Send length,data,CRC
		if(!I_WriteData(pInterfaceManager,hI_Handle,hTransactionHandle,m_pSendingFrame->GetLengthPointer(),m_pSendingFrame->GetLengthSize()+m_pSendingFrame->GetDataSize()+m_pSendingFrame->GetCrcSize(),&dWritten,pErrorInfo)) return FALSE;
		if(dWritten != (m_pSendingFrame->GetLengthSize()+m_pSendingFrame->GetDataSize()+m_pSendingFrame->GetCrcSize()))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}

		// 5. Wait for end acknowledge
		if(!I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,&uEndAck,sizeof(uEndAck),&dRead,pErrorInfo))
		{
			return FALSE;
		}
		// 6. Check end acknowledge
		if(dRead != 0)
		{
			switch(uEndAck)
			{
				case OK_ACK: break;
				default:
					{
						if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_NegAckReceived,pErrorInfo);
						return FALSE;
					}
			}
		}
		else
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
		return TRUE;
	}

	if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
	return FALSE;
}

BOOL CGatewayMaxonSerialV1ToI::ReceiveFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,BYTE* puOpCode,BYTE* puLen,void** ppDataBuffer,DWORD* pdDataBufferLength,WORD* pwCrc,DWORD dTimeout,CErrorInfo* pErrorInfo)
{
	DWORD	startTime,time;
	BOOL	oFirstCharReceived;
	BYTE	uOpCode = 0;
	BYTE	uLen;

	BYTE	uReadyAck = OK_ACK;
	BYTE	uEndAck;

	DWORD	dWritten;				// number of bytes written
	DWORD	dRead;					// number of bytes read

	if(pInterfaceManager && hI_Handle && m_pReceivingFrame)
	{
		if(pErrorInfo) pErrorInfo->Reset();

		//1. Read Command Number (FirstByte / Including Timeout Handling)
		oFirstCharReceived = FALSE;
		time = 0;

		startTime = MmcGetTickCount();

		while((!oFirstCharReceived && (time < dTimeout)) || (!oFirstCharReceived && (dTimeout == 0)))
		{
			if(I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,&uOpCode,sizeof(uOpCode),&dRead,pErrorInfo))
			{
				if(dRead == sizeof(uOpCode))
				{
					oFirstCharReceived = TRUE;
				}
			}

			if(dTimeout != 0) time = MmcGetTickCount()-startTime;

		}
		if(!oFirstCharReceived)
		{
			return FALSE;
		}
		//2. Send ready acknowledge
		if(!I_WriteData(pInterfaceManager,hI_Handle,hTransactionHandle,&uReadyAck,sizeof(uReadyAck),&dWritten,pErrorInfo))
		{
			return FALSE;
		}
		if(dWritten != sizeof(uReadyAck))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
		//3. Read number of data
		if(!I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,&uLen,sizeof(uLen),&dRead,pErrorInfo))
		{
			return FALSE;
		}
		if(dRead != sizeof(uLen))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
		//4. Prepare Receive Frame
		if(!m_pReceivingFrame->PrepareReceiveFrame(uOpCode,uLen))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
		//5. Read Data,CRC
		if(!I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,m_pReceivingFrame->GetDataPointer(),m_pReceivingFrame->GetDataSize()+m_pReceivingFrame->GetCrcSize(),&dRead,pErrorInfo))
		{
			return FALSE;
		}
		if(dRead != (m_pReceivingFrame->GetDataSize()+m_pReceivingFrame->GetCrcSize()))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
		//6. Check CRC
		if(m_pReceivingFrame->CheckCRC())
		{
			//Send Okay End Acknowledge
			uEndAck = OK_ACK;
		}
		else
		{
			//Send Negative End Acknowledge
			uEndAck = NEG_ACK;
		}
		//7. Send End Acknowledge
		if(!I_WriteData(pInterfaceManager,hI_Handle,hTransactionHandle,&uEndAck,sizeof(uEndAck),&dWritten,pErrorInfo))
		{
			return FALSE;
		}
		if(dWritten != sizeof(uEndAck))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
		//8. Copy received data to destination
		if(uEndAck == OK_ACK)
		{
			if(!m_pReceivingFrame->CopyReceivedData(puOpCode,puLen,ppDataBuffer,pdDataBufferLength,pwCrc))
			{
				if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
				return FALSE;
			}
			return TRUE;
		}
		else //NEG_ACK
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_MaxonSerialV1_BadCrcReceived,pErrorInfo);
			return FALSE;
		}
	}

	if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
	return FALSE;
}

BOOL CGatewayMaxonSerialV1ToI::I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);

	//Has to be implementeded by child class
    return oResult;
}

BOOL CGatewayMaxonSerialV1ToI::I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);

	//Has to be implementeded by child class
    return oResult;
}

