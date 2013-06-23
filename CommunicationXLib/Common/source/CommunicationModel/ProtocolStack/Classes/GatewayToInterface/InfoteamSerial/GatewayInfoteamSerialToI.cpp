// GatewayInfoteamSerialToI.cpp: Implementierung der Klasse CGatewayInfoteamSerialToI.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayInfoteamSerialToI.h"

#include <Process/MmcProcess.h>
#include <malloc.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_InfoteamSerial.h>
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include "Frame/InfoteamSerialSendingFrame.h"
#include "Frame/InfoteamSerialReceivingFrame.h"
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
CGatewayInfoteamSerialToI::CGatewayInfoteamSerialToI()
{
	m_pSendingFrame = new CInfoteamSerialSendingFrame();
	m_pReceivingFrame = new CInfoteamSerialReceivingFrame();
}

CGatewayInfoteamSerialToI::~CGatewayInfoteamSerialToI()
{
	if(m_pSendingFrame) delete m_pSendingFrame;
	if(m_pReceivingFrame) delete m_pReceivingFrame;
}

BOOL CGatewayInfoteamSerialToI::ProcessCommand(CCommandRoot* pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle)
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
				case INFOTEAM_SERIAL_PROCESS_PROTOCOL			: return Process_ProcessProtocol(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
				case INFOTEAM_SERIAL_PROCESS_PROTOCOL_MAXON		: return Process_ProcessProtocolMaxon(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
				case INFOTEAM_SERIAL_ABORT_PROTOCOL_MAXON		: return Process_AbortProtocolMaxon(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
				case INFOTEAM_SERIAL_SEND_CHUNK					: return Process_SendChunk(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
				case INFOTEAM_SERIAL_RECEIVE_DATA				: return Process_ReceiveData(pCommand_PS,pInterfaceManager,h_Handle,hTransactionHandle);
			}
		}
	}

	return FALSE;
}

BOOL CGatewayInfoteamSerialToI::Process_ProcessProtocol(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
{
	const DWORD k_MaxPackageSize_NoLimit = 0xFFFFFFFF;

	//*Constants PS*
	const int k_ParameterIndex_PackageSize = 0;
	const int k_ParameterIndex_ChunkSize = 1;
	const int k_ParameterIndex_LastChunkFlag = 2;
	const int k_ParameterIndex_Checksum = 3;
	const int k_ParameterIndex_Data = 4;

	const int k_ReturnParameterIndex_PackageSize = 0;
	const int k_ReturnParameterIndex_Checksum = 1;
	const int k_ReturnParameterIndex_Data = 2;

	//*Variables PS*
	//Parameter
	DWORD dPackageSize;
	WORD wChunkSize;
	BYTE uLastChunkFlag;
	DWORD dChecksum;
	void* pDataBuffer = NULL;
	DWORD dDataBufferLength;

	//ReturnParameter
	DWORD dRetPackageSize;
	DWORD dRetChecksum;
	void* pRetDataBuffer = NULL;
	DWORD dRetDataBufferLength = 0;

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
		pCommand->GetParameterData(k_ParameterIndex_PackageSize,&dPackageSize,sizeof(dPackageSize));
		pCommand->GetParameterData(k_ParameterIndex_ChunkSize,&wChunkSize,sizeof(wChunkSize));
		pCommand->GetParameterData(k_ParameterIndex_LastChunkFlag,&uLastChunkFlag,sizeof(uLastChunkFlag));
		pCommand->GetParameterData(k_ParameterIndex_Checksum,&dChecksum,sizeof(dChecksum));
		pCommand->GetParameterData(k_ParameterIndex_Data,pDataBuffer,dDataBufferLength);

		//Execute Command
		dTimeout = pCommand->GetTimeout();
		oResult = SendFrameRepeated(pInterfaceManager,hI_Handle,hTransactionHandle,dPackageSize,wChunkSize,uLastChunkFlag,&dChecksum,pDataBuffer,dDataBufferLength,&errorInfo);
		if(oResult)
		{
			oResult = ReceiveFrameRepeated(pInterfaceManager,hI_Handle,hTransactionHandle, k_MaxPackageSize_NoLimit, &dRetPackageSize,&dRetChecksum,&pRetDataBuffer,&dRetDataBufferLength,dTimeout,&errorInfo);
		}

		//Set PS ReturnParameter Data
		pCommand->SetStatus(oResult,&errorInfo);
		pCommand->SetParameterData(k_ParameterIndex_Checksum,&dChecksum,sizeof(dChecksum));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_PackageSize,&dRetPackageSize,sizeof(dRetPackageSize));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Checksum,&dRetChecksum,sizeof(dRetChecksum));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Data,pRetDataBuffer,dRetDataBufferLength);

		//Free DataBuffer
		if(pDataBuffer) free(pDataBuffer);
		if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
	}

	return oResult;
}

BOOL CGatewayInfoteamSerialToI::Process_ProcessProtocolMaxon(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
{
	const DWORD k_MaxPackageSize_Maxon = 256;
	
	//*Constants PS*
	const int k_ParameterIndex_PackageSize = 0;
	const int k_ParameterIndex_ChunkSize = 1;
	const int k_ParameterIndex_LastChunkFlag = 2;
	const int k_ParameterIndex_Checksum = 3;
	const int k_ParameterIndex_PackageType = 4;
	const int k_ParameterIndex_OpCode = 5;
	const int k_ParameterIndex_Data = 6;
	const int k_ParameterIndex_KeepLock = 7;

	const int k_ReturnParameterIndex_PackageSize = 0;
	const int k_ReturnParameterIndex_Checksum = 1;
	const int k_ReturnParameterIndex_PackageType = 2;
	const int k_ReturnParameterIndex_Data = 3;

	//*Variables PS*
	//Parameter
	DWORD dPackageSize;
	WORD wChunkSize;
	BYTE uLastChunkFlag;
	DWORD dChecksum;
	BYTE uPackageType = 0;
	BYTE uOpCode = 0;
	void* pDataBuffer = NULL;
	DWORD dDataBufferLength;
	BYTE ubKeepLock = 0;

	BYTE* p = NULL;
	DWORD dSize;

	//ReturnParameter
	DWORD dRetPackageSize;
	DWORD dRetChecksum;
	BYTE uRetPackageType = 0;
	void* pRetDataBuffer = NULL;
	DWORD dRetDataBufferLength = 0;

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
		dDataBufferLength = pCommand->GetParameterLength(k_ParameterIndex_Data) + sizeof(uPackageType) + sizeof(uOpCode);
		if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

		//Get PS Parameter Data
		pCommand->GetParameterData(k_ParameterIndex_PackageSize,&dPackageSize,sizeof(dPackageSize));
		pCommand->GetParameterData(k_ParameterIndex_ChunkSize,&wChunkSize,sizeof(wChunkSize));
		pCommand->GetParameterData(k_ParameterIndex_LastChunkFlag,&uLastChunkFlag,sizeof(uLastChunkFlag));
		pCommand->GetParameterData(k_ParameterIndex_Checksum,&dChecksum,sizeof(dChecksum));
		pCommand->GetParameterData(k_ParameterIndex_KeepLock,&ubKeepLock,sizeof(ubKeepLock));

		p = (BYTE*)pDataBuffer;
		pCommand->GetParameterData(k_ParameterIndex_PackageType,p,sizeof(uPackageType));p+=sizeof(uPackageType);
		pCommand->GetParameterData(k_ParameterIndex_OpCode,p,sizeof(uOpCode));p+=sizeof(uOpCode);
		pCommand->GetParameterData(k_ParameterIndex_Data,p,pCommand->GetParameterLength(k_ParameterIndex_Data));

		//Execute Command
		dTimeout = pCommand->GetTimeout();
		oResult = SendFrameRepeated(pInterfaceManager,hI_Handle,hTransactionHandle,dPackageSize,wChunkSize,uLastChunkFlag,&dChecksum,pDataBuffer,dDataBufferLength,&errorInfo);
		if(oResult)
		{
			oResult = ReceiveFrameRepeated(pInterfaceManager,hI_Handle,hTransactionHandle, k_MaxPackageSize_Maxon, &dRetPackageSize,&dRetChecksum,&pRetDataBuffer,&dRetDataBufferLength,dTimeout,&errorInfo);
		}

		//Set PS ReturnParameter Data
		pCommand->SetStatus(oResult,&errorInfo);
		pCommand->SetParameterData(k_ParameterIndex_Checksum,&dChecksum,sizeof(dChecksum));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_PackageSize,&dRetPackageSize,sizeof(dRetPackageSize));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Checksum,&dRetChecksum,sizeof(dRetChecksum));

		if(dRetDataBufferLength > 0)
		{
			p = (BYTE*)pRetDataBuffer;
			dSize = dRetDataBufferLength;
			pCommand->SetReturnParameterData(k_ReturnParameterIndex_PackageType,p,sizeof(uRetPackageType)); p+=sizeof(uPackageType); dSize -= sizeof(uPackageType);
			pCommand->SetReturnParameterData(k_ReturnParameterIndex_Data,p,dSize);
		}

		//Free DataBuffer
		if(pDataBuffer) free(pDataBuffer);
		if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(!ubKeepLock) Unlock();
	}

	return oResult;
}

BOOL CGatewayInfoteamSerialToI::Process_AbortProtocolMaxon(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
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

BOOL CGatewayInfoteamSerialToI::Process_SendChunk(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
{
	//*Constants PS*
	const int k_ParameterIndex_PackageSize = 0;
	const int k_ParameterIndex_ChunkSize = 1;
	const int k_ParameterIndex_LastChunkFlag = 2;
	const int k_ParameterIndex_Checksum = 3;
	const int k_ParameterIndex_Data = 4;

	//*Variables PS*
	//Parameter
	DWORD dPackageSize;
	WORD wChunkSize;
	BYTE uLastChunkFlag;
	DWORD dChecksum;
	void* pDataBuffer = NULL;
	DWORD dDataBufferLength;

	BOOL oResult = FALSE;
	CErrorInfo errorInfo;

	if(pCommand && pInterfaceManager)
	{
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Prepare DataBuffer
		dDataBufferLength = pCommand->GetParameterLength(k_ParameterIndex_Data);
		if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

		//Get PS Parameter Data
		pCommand->GetParameterData(k_ParameterIndex_PackageSize,&dPackageSize,sizeof(dPackageSize));
		pCommand->GetParameterData(k_ParameterIndex_ChunkSize,&wChunkSize,sizeof(wChunkSize));
		pCommand->GetParameterData(k_ParameterIndex_LastChunkFlag,&uLastChunkFlag,sizeof(uLastChunkFlag));
		pCommand->GetParameterData(k_ParameterIndex_Checksum,&dChecksum,sizeof(dChecksum));
		pCommand->GetParameterData(k_ParameterIndex_Data,pDataBuffer,dDataBufferLength);

		//Execute Command
		oResult = SendFrameRepeated(pInterfaceManager,hI_Handle,hTransactionHandle,dPackageSize,wChunkSize,uLastChunkFlag,&dChecksum,pDataBuffer,dDataBufferLength,&errorInfo);

		//Set PS ReturnParameter Data
		pCommand->SetStatus(oResult,&errorInfo);
		pCommand->SetParameterData(k_ParameterIndex_Checksum,&dChecksum,sizeof(dChecksum));

		//Free DataBuffer
		if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
	}

	return oResult;
}

BOOL CGatewayInfoteamSerialToI::Process_ReceiveData(CCommand_PS* pCommand,CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle)
{
	const DWORD k_MaxPackageSize_NoLimit = 0xFFFFFFFF;

	//*Constants PS*
	const int k_ReturnParameterIndex_PackageSize = 0;
	const int k_ReturnParameterIndex_Checksum = 1;
	const int k_ReturnParameterIndex_Data = 2;

	//*Variables PS*
	//ReturnParameter
	DWORD dRetPackageSize;
	DWORD dRetChecksum;
	void* pRetDataBuffer = NULL;
	DWORD dRetDataBufferLength = 0;

	BOOL oResult = FALSE;
	CErrorInfo errorInfo;
	DWORD dTimeout;

	if(pCommand && pInterfaceManager)
	{
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Execute Command
		dTimeout = pCommand->GetTimeout();
		oResult = ReceiveFrameRepeated(pInterfaceManager,hI_Handle,hTransactionHandle, k_MaxPackageSize_NoLimit, &dRetPackageSize,&dRetChecksum,&pRetDataBuffer,&dRetDataBufferLength,dTimeout,&errorInfo);

		//Set PS ReturnParameter Data
		pCommand->SetStatus(oResult,&errorInfo);
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_PackageSize,&dRetPackageSize,sizeof(dRetPackageSize));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Checksum,&dRetChecksum,sizeof(dRetChecksum));
		pCommand->SetReturnParameterData(k_ReturnParameterIndex_Data,pRetDataBuffer,dRetDataBufferLength);

		//Free DataBuffer
		if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
	}

	return oResult;
}

CGateway* CGatewayInfoteamSerialToI::Clone()
{
	CGatewayInfoteamSerialToI* pClonedGateway;

	pClonedGateway = new CGatewayInfoteamSerialToI();
	*pClonedGateway = *this;

	return pClonedGateway;
}

CGatewayInfoteamSerialToI& CGatewayInfoteamSerialToI::operator=(CGatewayInfoteamSerialToI& other)
{
	if(this != &other)
	{
		*((CGatewayPStoI*)this) = *((CGatewayPStoI*)&other);
	}

	return *this;
}

BOOL CGatewayInfoteamSerialToI::SendFrameRepeated(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,DWORD dPackageSize,WORD wChunkSize,BYTE uLastChunkFlag,DWORD* pdChecksum,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo)
{
	const DWORD k_MaxSendingCount = 3;

	CErrorInfo errorInfo;
	DWORD dSendingCount = 0;

	while(dSendingCount < k_MaxSendingCount)
	{
		dSendingCount++;
		if(SendFrame(pInterfaceManager,hI_Handle,hTransactionHandle,dPackageSize,wChunkSize,uLastChunkFlag,pdChecksum,pDataBuffer,dDataBufferLength,&errorInfo))
		{
			return TRUE;
		}
		else
		{
			if(errorInfo.GetErrorCode() != k_Error_InfoteamSerial_RepAckReceived)
			{
				if(pErrorInfo) *pErrorInfo = errorInfo;
				return FALSE;
			}
		}
	}

	if(pErrorInfo) *pErrorInfo = errorInfo;
	return FALSE;
}

BOOL CGatewayInfoteamSerialToI::SendFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,DWORD dPackageSize,WORD wChunkSize,BYTE uLastChunkFlag,DWORD* pdChecksum,void* pDataBuffer,DWORD dDataBufferLength,CErrorInfo* pErrorInfo)
{
	BYTE	uAck;
	DWORD	dWritten;
	DWORD	dRead;

	if(pInterfaceManager && hI_Handle && m_pSendingFrame)
	{
		if(pErrorInfo) pErrorInfo->Reset();

		// Check CunkSize
		if(wChunkSize > k_MaxChunkSize)
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_ChunkSizeTooHigh,pErrorInfo);
			return FALSE;
		}

		// Initialization
		pInterfaceManager->I_ResetInterface(hI_Handle);
		if(!m_pSendingFrame->PrepareSendFrame(dPackageSize,wChunkSize,uLastChunkFlag,pdChecksum,pDataBuffer,dDataBufferLength))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}

		// Send Header & Data
		if(!I_WriteData(pInterfaceManager,hI_Handle,hTransactionHandle,m_pSendingFrame->GetFramePointer(),m_pSendingFrame->GetFrameSize(),&dWritten,pErrorInfo)) return FALSE;
		if(dWritten != m_pSendingFrame->GetFrameSize())
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}

		// Wait for acknowledge
		if(!I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,&uAck,sizeof(uAck),&dRead,pErrorInfo))
		{
			return FALSE;
		}

		// Check acknowledge
		if(dRead != 0)
		{
			switch(uAck)
			{
				case OK_ACK:
					{
						return TRUE;
					} break;

				case REP_ACK:
					{
						if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_RepAckReceived,pErrorInfo);
						return FALSE;
					} break;

				default:
					{
						if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadAckReceived,pErrorInfo);
						return FALSE;
					};
			}
		}
		else
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}
	}

	if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
	return FALSE;
}

BOOL CGatewayInfoteamSerialToI::ReceiveFrameRepeated(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle, DWORD p_ulMaxPackageSize, DWORD* pdRetPackageSize,DWORD* pdRetChecksum,void** ppRetDataBuffer,DWORD* pdRetDataBufferLength,DWORD dTimeout,CErrorInfo* pErrorInfo)
{
	const DWORD k_MaxReceivingCount = 3;

	CErrorInfo errorInfo;
	DWORD dReceivingCount = 0;
	BOOL oSendRepAck = TRUE;

	while(dReceivingCount < k_MaxReceivingCount)
	{
		dReceivingCount++;
		if(dReceivingCount == k_MaxReceivingCount) oSendRepAck = FALSE;
		if(ReceiveFrame(pInterfaceManager,hI_Handle,hTransactionHandle, p_ulMaxPackageSize, pdRetPackageSize,pdRetChecksum,ppRetDataBuffer,pdRetDataBufferLength,dTimeout,oSendRepAck,&errorInfo))
		{
			return TRUE;
		}
		else
		{
			if(!oSendRepAck || (errorInfo.GetErrorCode() != k_Error_InfoteamSerial_BadDataSizeReceived) || (errorInfo.GetErrorCode() != k_Error_InfoteamSerial_BadCrcReceived))
			{
				if(pErrorInfo) *pErrorInfo = errorInfo;
				return FALSE;
			}
		}
	}

	if(pErrorInfo) *pErrorInfo = errorInfo;
	return FALSE;
}

BOOL CGatewayInfoteamSerialToI::ReceiveFrame(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle, DWORD p_ulMaxPackageSize, DWORD* pdRetPackageSize,DWORD* pdRetChecksum,void** ppRetDataBuffer,DWORD* pdRetDataBufferLength,DWORD dTimeout,BOOL oSendRepAck,CErrorInfo* pErrorInfo)
{
	DWORD startTime,time;
	BOOL oFirstDataReceived;
	DWORD dPackageSize = 0;
	BYTE uAck;

	DWORD	dWritten;				// number of bytes written
	DWORD	dRead;					// number of bytes read
	BOOL	oCrcOkay = FALSE;
	BOOL	oDataOkay = TRUE;

	if(pInterfaceManager && hI_Handle && m_pReceivingFrame)
	{
		if(pErrorInfo) pErrorInfo->Reset();

		//1.Read PackageSize (FirstData / Including Timeout Handling)
		oFirstDataReceived = FALSE;
		time = 0;

		startTime = MmcGetTickCount();

		while((!oFirstDataReceived && (time < dTimeout)) || (!oFirstDataReceived && (dTimeout == 0)))
		{
			if(I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,&dPackageSize,sizeof(dPackageSize),&dRead,pErrorInfo))
			{
				if(dRead == sizeof(dPackageSize))
				{
					oFirstDataReceived = TRUE;
				}
			}

			if(dTimeout != 0) time = MmcGetTickCount()-startTime;

		}
		if(!oFirstDataReceived)
		{
			return FALSE;
		}

		//2. Prepare Receive Frame
		if(!m_pReceivingFrame->PrepareReceiveFrame(dPackageSize, p_ulMaxPackageSize))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}

		//3. Read Checksum,Data
		if(!I_ReadData(pInterfaceManager,hI_Handle,hTransactionHandle,m_pReceivingFrame->GetChecksumPointer(),m_pReceivingFrame->GetChecksumSize()+m_pReceivingFrame->GetDataSize(),&dRead,pErrorInfo))
		{
			oDataOkay = FALSE;
		}
		if(dRead != (m_pReceivingFrame->GetChecksumSize()+m_pReceivingFrame->GetDataSize()))
		{
			oDataOkay = FALSE;
		}

		//4. Check CRC
        if(oDataOkay) oCrcOkay = m_pReceivingFrame->CheckCRC();
		if(oDataOkay && oCrcOkay)
		{
			//Send Okay Acknowledge
			uAck = OK_ACK;
		}
		else
		{
			if(oSendRepAck)
			{
				//Send Repeat Acknowledge
				pInterfaceManager->I_ResetInterface(hI_Handle);
				uAck = REP_ACK;
			}
			else
			{
				//Send Okay Acknowledge and handle frame as failed
				uAck = OK_ACK;
			}
		}

		//5. Send Acknowledge
		if(!I_WriteData(pInterfaceManager,hI_Handle,hTransactionHandle,&uAck,sizeof(uAck),&dWritten,pErrorInfo))
		{
			return FALSE;
		}
		if(dWritten != sizeof(uAck))
		{
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
			return FALSE;
		}

		//6. Copy received data to destination
		if(oDataOkay && oCrcOkay)
		{
			if(!m_pReceivingFrame->CopyReceivedData(pdRetPackageSize,pdRetChecksum,ppRetDataBuffer,pdRetDataBufferLength))
			{
				if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
				return FALSE;
			}
			return TRUE;
		}
		else //REP_ACK
		{
			if(!oDataOkay)
			{
				if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadDataSizeReceived,pErrorInfo);
			}
			else if(!oCrcOkay)
			{
				if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_InfoteamSerial_BadCrcReceived,pErrorInfo);
			}
			return FALSE;
		}
	}

	if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal,pErrorInfo);
	return FALSE;
}

BOOL CGatewayInfoteamSerialToI::I_ReadData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToRead,DWORD* pdNumberOfBytesRead,CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);

	//Has to be implementeded by child class
    return oResult;
}

BOOL CGatewayInfoteamSerialToI::I_WriteData(CInterfaceManagerBase* pInterfaceManager,HANDLE hI_Handle,HANDLE hTransactionHandle,void* pData,DWORD dNumberOfBytesToWrite,DWORD* pdNumberOfBytesWritten,CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);

	//Has to be implementeded by child class
    return oResult;
}

