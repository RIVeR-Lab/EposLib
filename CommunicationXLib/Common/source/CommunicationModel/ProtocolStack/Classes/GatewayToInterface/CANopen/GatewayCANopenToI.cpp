// GatewayCANopenToI.cpp: Implementierung der Klasse CGatewayCANopenToI.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <Process/MmcProcess.h>
#include <malloc.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_CANopen.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_CAN.h>

#include <CommunicationModel/Interface/InterfaceManagerBase.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>

#include "Frame/CANopenFrame.h"

#include "GatewayCANopenToI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayCANopenToI::CGatewayCANopenToI()
{
    m_pSendingFrame = new CCANopenFrame();
    m_pReceivingFrame = new CCANopenFrame();

    m_pCommand_TransmitCanFrame = NULL;
    m_pCommand_ReceiveCanFrame = NULL;
    m_pCommand_ReceiveFilteredCanFrame = NULL;

	m_ulNetworkIndicationThreadId = 0;

    InitCommands();
}

CGatewayCANopenToI::~CGatewayCANopenToI()
{
    if(m_pSendingFrame) delete m_pSendingFrame;
    if(m_pReceivingFrame) delete m_pReceivingFrame;

    DeleteCommands();
}

CGateway* CGatewayCANopenToI::Clone()
{
    CGatewayCANopenToI* pClonedGateway;

    pClonedGateway = new CGatewayCANopenToI();
    *pClonedGateway = *this;

    return pClonedGateway;
}

void CGatewayCANopenToI::InitCommands()
{
    DeleteCommands();

    //Init Command TransmitCanFrame
    m_pCommand_TransmitCanFrame = new CCommand_I_CAN();
    m_pCommand_TransmitCanFrame->InitCommand(CAN_TRANSMIT_CAN_FRAME);

    //Init Command ReceiveCanFrame
    m_pCommand_ReceiveCanFrame = new CCommand_I_CAN();
    m_pCommand_ReceiveCanFrame->InitCommand(CAN_RECEIVE_FILTERED_CAN_FRAME);

    //Init Command ReceiveFilteredCanFrame
    m_pCommand_ReceiveFilteredCanFrame = new CCommand_I_CAN();
    m_pCommand_ReceiveFilteredCanFrame->InitCommand(CAN_RECEIVE_FILTERED_CAN_FRAME);
}

void CGatewayCANopenToI::DeleteCommands()
{
    if(m_pCommand_TransmitCanFrame)
    {
        delete m_pCommand_TransmitCanFrame;
        m_pCommand_TransmitCanFrame = NULL;
    }

    if(m_pCommand_ReceiveCanFrame)
    {
        delete m_pCommand_ReceiveCanFrame;
        m_pCommand_ReceiveCanFrame = NULL;
    }

    if(m_pCommand_ReceiveFilteredCanFrame)
    {
        delete m_pCommand_ReceiveFilteredCanFrame;
        m_pCommand_ReceiveFilteredCanFrame = NULL;
    }
}

CGatewayCANopenToI& CGatewayCANopenToI::operator=(CGatewayCANopenToI& p_rOther)
{
    if(this != &p_rOther)
    {
        *((CGatewayPStoI*)this) = *((CGatewayPStoI*)&p_rOther);
    }

    return *this;
}

BOOL CGatewayCANopenToI::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
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
				case CANOPEN_INITIATE_SDO_DOWNLOAD: return Process_InitiateSDODownload(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_DOWNLOAD_SDO_SEGMENT: return Process_DownloadSDOSegment(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_INITIATE_SDO_UPLOAD: return Process_InitiateSDOUpload(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_UPLOAD_SDO_SEGMENT: return Process_UploadSDOSegment(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_NETWORK_INDICATION: return Process_NetworkIndication(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_ABORT_SDO_TRANSFER: return Process_AbortSDOTransfer(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_SEND_NMT_SERVICE: return Process_SendNMTService(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_SEND_CAN_FRAME: return Process_SendCANFrame(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_READ_CAN_FRAME: return Process_ReadCANFrame(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_REQUEST_CAN_FRAME: return Process_RequestCANFrame(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_SEND_LSS_FRAME: return Process_SendLSSFrame(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
				case CANOPEN_READ_LSS_FRAME: return Process_ReadLSSFrame(pCommand_PS, pInterfaceManager, h_Handle, hTransactionHandle);
			}
		}
	}

    return FALSE;
}

BOOL CGatewayCANopenToI::Process_InitiateSDODownload(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COB_ID_CLIENT_SERVER = 0;
    const int PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 1;
    const int PARAMETER_INDEX_EXPEDITED_TRANSFER = 2;
    const int PARAMETER_INDEX_SIZE_INDICATED = 3;
    const int PARAMETER_INDEX_NON_VALID_NB_OF_BYTES = 4;
    const int PARAMETER_INDEX_INDEX = 5;
    const int PARAMETER_INDEX_SUB_INDEX = 6;
    const int PARAMETER_INDEX_DATA = 7;

    const int RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 0;
    const int RETURN_PARAMETER_INDEX_INDEX = 1;
    const int RETURN_PARAMETER_INDEX_SUB_INDEX = 2;
    const int RETURN_PARAMETER_INDEX_ABORT_CODE = 3;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE uNonValidNbOfBytes;
    WORD wIndex;
    BYTE uSubIndex;
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;

    //ReturnParameter
    DWORD dRetCobIdServerClient = 0;
    WORD wRetIndex = 0;
    BYTE uRetSubIndex = 0;
    DWORD dAbortCode = 0;

    BOOL oResult = FALSE;
    BOOL oBadSdoFrameReceived = FALSE;
    CErrorInfo errorInfo;
    DWORD dTimeout;
    DWORD time, start;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Lock CriticalSection
		if(IsNetworkIndicationThread())
		{
			//Already locked by NetworkIndication
			if(!IsLocked(pCommand)) return FALSE;
		}
		else
		{
			//No NetworkIndication received
			if(!Lock(pCommand)) return FALSE;
		}

		//Prepare DataBuffer
        dDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

        //Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_CLIENT_SERVER, &dCobIdClientServer, sizeof(dCobIdClientServer));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dCobIdServerClient, sizeof(dCobIdServerClient));
        pCommand->GetParameterData(PARAMETER_INDEX_EXPEDITED_TRANSFER, &oExpeditedTransfer, sizeof(oExpeditedTransfer));
        pCommand->GetParameterData(PARAMETER_INDEX_SIZE_INDICATED, &oSizeIndicated, sizeof(oSizeIndicated));
        pCommand->GetParameterData(PARAMETER_INDEX_NON_VALID_NB_OF_BYTES, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_InitiateSDODownload(dCobIdClientServer, oExpeditedTransfer, oSizeIndicated, uNonValidNbOfBytes, wIndex, uSubIndex, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);
        if(oResult)
        {
            //Check Timeout
            dTimeout = pCommand->GetTimeout();

            start = MmcGetTickCount();

            do
            {
                //Receive Frame
                oResult = FALSE;
                if(ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobIdServerClient, m_pReceivingFrame, dTimeout, &errorInfo))
                {
                    //Check Received Frame
                    oResult = CheckReceivedFrame(m_pSendingFrame, m_pReceivingFrame);
                    if(!oResult) oBadSdoFrameReceived = TRUE;
                }

                //Update Time
                time = MmcGetTickCount() - start;

            }while((!oResult) && (time < dTimeout));

            //ErrorInfo
            if((oBadSdoFrameReceived) && (time >= dTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_SdoReceiveFrameNotReceived, &errorInfo);
            }
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_InitiateSDODownload(&dRetCobIdServerClient, &wRetIndex, &uRetSubIndex, &dAbortCode);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dRetCobIdServerClient, sizeof(dRetCobIdServerClient));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_INDEX, &wRetIndex, sizeof(wRetIndex));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_SUB_INDEX, &uRetSubIndex, sizeof(uRetSubIndex));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ABORT_CODE, &dAbortCode, sizeof(dAbortCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		if(oExpeditedTransfer || !oResult || (dAbortCode != 0))
		{
			ResetNetworkIndication();
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_AbortSDOTransfer(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COB_ID_CLIENT_SERVER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_ABORT_CODE = 3;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    WORD wIndex;
    BYTE uSubIndex;
    DWORD dAbortCode;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_CLIENT_SERVER, &dCobIdClientServer, sizeof(dCobIdClientServer));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_ABORT_CODE, &dAbortCode, sizeof(dAbortCode));

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_AbortSDOTransfer(dCobIdClientServer, wIndex, uSubIndex, dAbortCode);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);

		//Unlock CriticalSection
		ResetNetworkIndication();
		Unlock();
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_NetworkIndication(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COB_ID_CLIENT_SERVER = 0;
    const int PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 1;
    const int PARAMETER_INDEX_TARGET_NETWORK_ID = 2;
    const int PARAMETER_INDEX_TARGET_NODE_ID = 3;

    const int RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 0;
    const int RETURN_PARAMETER_TARGET_NETWORK_ID = 1;
    const int RETURN_PARAMETER_TARGET_NODE_ID = 2;
    const int RETURN_PARAMETER_INDEX_ABORT_CODE = 3;

	//*Variables PS*
    //Parameter
    DWORD dCobIdClientServer = 0;
    DWORD dCobIdServerClient = 0;
    WORD wTargetNetworkId = 0;
    BYTE uTargetNodeId = 0;

    //ReturnParameter
    DWORD dRetCobIdServerClient = 0;
    WORD wRetTargetNetworkId = 0;
    BYTE uRetTargetNodeId = 0;
    DWORD dAbortCode = 0;

    BOOL oResult = FALSE;
    BOOL oBadSdoFrameReceived = FALSE;
    CErrorInfo errorInfo;
    DWORD dTimeout;
    DWORD time, start;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_CLIENT_SERVER, &dCobIdClientServer, sizeof(dCobIdClientServer));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dCobIdServerClient, sizeof(dCobIdServerClient));
        pCommand->GetParameterData(PARAMETER_INDEX_TARGET_NETWORK_ID, &wTargetNetworkId, sizeof(wTargetNetworkId));
        pCommand->GetParameterData(PARAMETER_INDEX_TARGET_NODE_ID, &uTargetNodeId, sizeof(uTargetNodeId));

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_NetworkIndication(dCobIdClientServer, wTargetNetworkId, uTargetNodeId);

		//NetworkIndication
		SetNetworkIndication();

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);
        if(oResult)
        {
            //Check Timeout
            dTimeout = pCommand->GetTimeout();

            start = MmcGetTickCount();

            do
            {
                //Receive Frame
                oResult = FALSE;
                if(ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobIdServerClient, m_pReceivingFrame, dTimeout, &errorInfo))
                {
                    //Check Received Frame
                    oResult = CheckReceivedFrame(m_pSendingFrame, m_pReceivingFrame);
                    if(!oResult) oBadSdoFrameReceived = TRUE;
                }

                //Update Time
                time = MmcGetTickCount() - start;

            }while((!oResult) && (time < dTimeout));

            //ErrorInfo
            if((oBadSdoFrameReceived) && (time >= dTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_SdoReceiveFrameNotReceived, &errorInfo);
            }
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_NetworkIndication(&dRetCobIdServerClient, &wRetTargetNetworkId, &uRetTargetNodeId, &dAbortCode);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dRetCobIdServerClient, sizeof(dRetCobIdServerClient));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_TARGET_NETWORK_ID, &wRetTargetNetworkId, sizeof(wRetTargetNetworkId));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_TARGET_NODE_ID, &uRetTargetNodeId, sizeof(uRetTargetNodeId));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ABORT_CODE, &dAbortCode, sizeof(dAbortCode));

		//Unlock CriticalSection
		if(!oResult || (dAbortCode != 0))
		{
			ResetNetworkIndication();
			Unlock();
		}
    }

    return oResult;
}


BOOL CGatewayCANopenToI::Process_DownloadSDOSegment(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COB_ID_CLIENT_SERVER = 0;
    const int PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 1;
    const int PARAMETER_INDEX_TOGGLE = 2;
    const int PARAMETER_INDEX_NON_VALID_NB_OF_BYTES = 3;
    const int PARAMETER_INDEX_NO_MORE_SEGMENTS = 4;
    const int PARAMETER_INDEX_SEG_DATA = 5;

    const int RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 0;
    const int RETURN_PARAMETER_INDEX_TOGGLE = 1;
    const int RETURN_PARAMETER_INDEX_ABORT_CODE = 2;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    BOOL oToggle;
    BYTE uNonValidNbOfBytes;
    BOOL oNoMoreSegments;
    void* pSegDataBuffer = NULL;
    DWORD dSegDataBufferLength;

    //ReturnParameter
    DWORD dRetCobIdServerClient = 0;
    BOOL oRetToggle = FALSE;
    DWORD dAbortCode = 0;

    BOOL oResult = FALSE;
    BOOL oBadSdoFrameReceived = FALSE;
    CErrorInfo errorInfo;
    DWORD dTimeout = 0;
    DWORD time = 0;
    DWORD start = 0;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Check CriticalSection
		if(!IsLocked(pCommand)) return FALSE;

		//Prepare DataBuffer
        dSegDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_SEG_DATA);
        if(dSegDataBufferLength > 0) pSegDataBuffer = malloc(dSegDataBufferLength);

        //Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_CLIENT_SERVER, &dCobIdClientServer, sizeof(dCobIdClientServer));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dCobIdServerClient, sizeof(dCobIdServerClient));
        pCommand->GetParameterData(PARAMETER_INDEX_TOGGLE, &oToggle, sizeof(oToggle));
        pCommand->GetParameterData(PARAMETER_INDEX_NON_VALID_NB_OF_BYTES, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
        pCommand->GetParameterData(PARAMETER_INDEX_NO_MORE_SEGMENTS, &oNoMoreSegments, sizeof(oNoMoreSegments));
        pCommand->GetParameterData(PARAMETER_INDEX_SEG_DATA, pSegDataBuffer, dSegDataBufferLength);

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_DownloadSDOSegment(dCobIdClientServer, oToggle, uNonValidNbOfBytes, oNoMoreSegments, pSegDataBuffer, dSegDataBufferLength);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);
        if(oResult)
        {
            //Check Timeout
            dTimeout = pCommand->GetTimeout();

            start = MmcGetTickCount();

            do
            {
                //Receive Frame
                oResult = FALSE;
                if(ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobIdServerClient, m_pReceivingFrame, dTimeout, &errorInfo))
                {
                    //Check Received Frame
                    oResult = CheckReceivedFrame(m_pSendingFrame, m_pReceivingFrame);
                    if(!oResult) oBadSdoFrameReceived = TRUE;
                }

                //Update Time
                time = MmcGetTickCount() - start;

            }while((!oResult) && (time < dTimeout));
        }

        //ErrorInfo
        if((oBadSdoFrameReceived) && (time >= dTimeout))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_SdoReceiveFrameNotReceived, &errorInfo);
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_DownloadSDOSegment(&dRetCobIdServerClient, &oRetToggle, &dAbortCode);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dRetCobIdServerClient, sizeof(dRetCobIdServerClient));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_TOGGLE, &oRetToggle, sizeof(oRetToggle));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ABORT_CODE, &dAbortCode, sizeof(dAbortCode));

        //Free DataBuffer
        if(pSegDataBuffer) free(pSegDataBuffer);

		//Unlock CriticalSection
		if(oNoMoreSegments || !oResult || (dAbortCode != 0))
		{
			ResetNetworkIndication();
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_InitiateSDOUpload(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COB_ID_CLIENT_SERVER = 0;
    const int PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 1;
    const int PARAMETER_INDEX_INDEX = 2;
    const int PARAMETER_INDEX_SUB_INDEX = 3;

    const int RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 0;
    const int RETURN_PARAMETER_INDEX_EXPEDITED_TRANSFER = 1;
    const int RETURN_PARAMETER_INDEX_SIZE_INDICATED = 2;
    const int RETURN_PARAMETER_INDEX_NON_VALID_NB_OF_BYTES = 3;
    const int RETURN_PARAMETER_INDEX_INDEX = 4;
    const int RETURN_PARAMETER_INDEX_SUB_INDEX = 5;
    const int RETURN_PARAMETER_INDEX_DATA = 6;
    const int RETURN_PARAMETER_INDEX_ABORT_CODE = 7;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    WORD wIndex;
    BYTE uSubIndex;

    //ReturnParameter
    DWORD dRetCobIdServerClient = 0;
    BOOL oRetExpeditedTransfer = FALSE;
    BOOL oRetSizeIndicated = FALSE;
    BYTE uRetNonValidNbOfBytes = 0;
    WORD wRetIndex = 0;
    BYTE uRetSubIndex = 0;
    void* pRetDataBuffer = NULL;
    DWORD dRetDataBufferLength = 0;
    DWORD dAbortCode = 0;

    BOOL oResult = FALSE;
    BOOL oBadSdoFrameReceived = FALSE;
    CErrorInfo errorInfo;
    DWORD dTimeout;
    DWORD time, start;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Lock CriticalSection
		if(IsNetworkIndicationThread())
		{
			//Already locked by NetworkIndication
			if(!IsLocked(pCommand)) return FALSE;
		}
		else
		{
			//No NetworkIndication received
			if(!Lock(pCommand)) return FALSE;
		}

		//Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_CLIENT_SERVER, &dCobIdClientServer, sizeof(dCobIdClientServer));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dCobIdServerClient, sizeof(dCobIdServerClient));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_InitiateSDOUpload(dCobIdClientServer, wIndex, uSubIndex);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);
        if(oResult)
        {
            //Check Timeout
            dTimeout = pCommand->GetTimeout();

            start = MmcGetTickCount();

            do
            {
                //Receive Frame
                oResult = FALSE;
                if(ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobIdServerClient, m_pReceivingFrame, dTimeout, &errorInfo))
                {
                    //Check Received Frame
                    oResult = CheckReceivedFrame(m_pSendingFrame, m_pReceivingFrame);
                    if(!oResult) oBadSdoFrameReceived = TRUE;
                }

                //Update Time
                time = MmcGetTickCount() - start;

            }while((!oResult) && (time < dTimeout));

            //ErrorInfo
            if((oBadSdoFrameReceived) && (time >= dTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_SdoReceiveFrameNotReceived, &errorInfo);
            }
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_InitiateSDOUpload(&dRetCobIdServerClient, &oRetExpeditedTransfer, &oRetSizeIndicated, &uRetNonValidNbOfBytes, &wRetIndex, &uRetSubIndex, &pRetDataBuffer, &dRetDataBufferLength, &dAbortCode);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dRetCobIdServerClient, sizeof(dRetCobIdServerClient));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_EXPEDITED_TRANSFER, &oRetExpeditedTransfer, sizeof(oRetExpeditedTransfer));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_SIZE_INDICATED, &oRetSizeIndicated, sizeof(oRetSizeIndicated));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_NON_VALID_NB_OF_BYTES, &uRetNonValidNbOfBytes, sizeof(uRetNonValidNbOfBytes));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_INDEX, &wRetIndex, sizeof(wRetIndex));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_SUB_INDEX, &uRetSubIndex, sizeof(uRetSubIndex));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pRetDataBuffer, dRetDataBufferLength);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ABORT_CODE, &dAbortCode, sizeof(dAbortCode));

        //Free DataBuffer
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		if(oRetExpeditedTransfer || !oResult || (dAbortCode != 0))
		{
			ResetNetworkIndication();
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_UploadSDOSegment(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COB_ID_CLIENT_SERVER = 0;
    const int PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 1;
    const int PARAMETER_INDEX_TOGGLE = 2;

    const int RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT = 0;
    const int RETURN_PARAMETER_INDEX_TOGGLE = 1;
    const int RETURN_PARAMETER_INDEX_NON_VALID_NB_OF_BYTES = 2;
    const int RETURN_PARAMETER_INDEX_NO_MORE_SEGMENTS = 3;
    const int RETURN_PARAMETER_INDEX_SEG_DATA = 4;
    const int RETURN_PARAMETER_INDEX_ABORT_CODE = 5;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    BOOL oToggle;

    //ReturnParameter
    DWORD dRetCobIdServerClient = 0;
    BOOL oRetToggle = FALSE;
    BYTE uRetNonValidNbOfBytes = 0;
    BOOL oRetNoMoreSegments = FALSE;
    void* pRetSegDataBuffer = NULL;
    DWORD dRetSegDataBufferLength = 0;
    DWORD dAbortCode = 0;

    BOOL oResult = FALSE;
    BOOL oBadSdoFrameReceived = FALSE;
    CErrorInfo errorInfo;
    DWORD dTimeout;
    DWORD time, start;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Check CriticalSection
		if(!IsLocked(pCommand)) return FALSE;

		//Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_CLIENT_SERVER, &dCobIdClientServer, sizeof(dCobIdClientServer));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dCobIdServerClient, sizeof(dCobIdServerClient));
        pCommand->GetParameterData(PARAMETER_INDEX_TOGGLE, &oToggle, sizeof(oToggle));

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_UploadSDOSegment(dCobIdClientServer, oToggle);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);
        if(oResult)
        {
            //Check Timeout
            dTimeout = pCommand->GetTimeout();

            start = MmcGetTickCount();

            do
            {
                //Receive Frame
                oResult = FALSE;
                if(ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobIdServerClient, m_pReceivingFrame, dTimeout, &errorInfo))
                {
                    //Check Received Frame
                    oResult = CheckReceivedFrame(m_pSendingFrame, m_pReceivingFrame);
                    if(!oResult) oBadSdoFrameReceived = TRUE;
                }

                //Update Time
                time = MmcGetTickCount() - start;

            }while((!oResult) && (time < dTimeout));

            //ErrorInfo
            if((oBadSdoFrameReceived) && (time >= dTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_SdoReceiveFrameNotReceived, &errorInfo);
            }
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_UploadSDOSegment(&dRetCobIdServerClient, &oRetToggle, &uRetNonValidNbOfBytes, &oRetNoMoreSegments, &pRetSegDataBuffer, &dRetSegDataBufferLength, &dAbortCode);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_COB_ID_SERVER_CLIENT, &dRetCobIdServerClient, sizeof(dRetCobIdServerClient));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_TOGGLE, &oRetToggle, sizeof(oRetToggle));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_NON_VALID_NB_OF_BYTES, &uRetNonValidNbOfBytes, sizeof(uRetNonValidNbOfBytes));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_NO_MORE_SEGMENTS, &oRetNoMoreSegments, sizeof(oRetNoMoreSegments));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_SEG_DATA, pRetSegDataBuffer, dRetSegDataBufferLength);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ABORT_CODE, &dAbortCode, sizeof(dAbortCode));

        //Free DataBuffer
        if(pRetSegDataBuffer) free(pRetSegDataBuffer);

		//Unlock CriticalSection
		if(oRetNoMoreSegments || !oResult || (dAbortCode != 0))
		{
			ResetNetworkIndication();
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_SendNMTService(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int k_ParameterIndex_CommandSpecifier = 0;
    const int k_ParameterIndex_NodeId = 1;

    //*Variables PS*
    //Parameter
    BYTE uCommandSpecifier;
    BYTE uNodeId;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get PS Parameter Data
        pCommand->GetParameterData(k_ParameterIndex_CommandSpecifier, &uCommandSpecifier, sizeof(uCommandSpecifier));
        pCommand->GetParameterData(k_ParameterIndex_NodeId, &uNodeId, sizeof(uNodeId));

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_SendNMTService(uCommandSpecifier, uNodeId);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_SendCANFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COBID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int PARAMETER_INDEX_DATA = 2;

    //*Variables PS*
    //Parameter
    DWORD dCobId;
    BYTE uLength;
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Prepare DataBuffer
        dDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

        //Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &dCobId, sizeof(dCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &uLength, sizeof(uLength));
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_SendCANFrame(dCobId, uLength, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_ReadCANFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COBID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int PARAMETER_INDEX_TIMEOUT = 2;

    const int RETURN_PARAMETER_INDEX_DATA = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobId = 2020;

    //ReturnParameter
    void* pRetDataBuffer = NULL;
    DWORD ulRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    DWORD ulTimeout = 0;
    DWORD ulTime = 0;
    DWORD ulStart = 0;

    if(pCommand && pInterfaceManager && m_pReceivingFrame)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &ulCobId, sizeof(ulCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &ulRetDataBufferLength, sizeof(ulRetDataBufferLength));
        pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &ulTimeout, sizeof(ulTimeout));

        //Check Timeout

        ulStart = MmcGetTickCount();

        do
        {
            //Receive Frame
            oResult = ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, ulCobId, m_pReceivingFrame, ulTimeout, &errorInfo);

            //Update Time
            ulTime = MmcGetTickCount() - ulStart;

        }while((!oResult) && (ulTime < ulTimeout));

        //ErrorInfo
        if((!oResult) && (ulTime >= ulTimeout))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_CanFrameNotReceived, &errorInfo);
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_ReceivedCANFrame(&pRetDataBuffer, &ulRetDataBufferLength);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pRetDataBuffer, ulRetDataBufferLength);

        //Free DataBuffer
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_RequestCANFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_COBID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;

    const int RETURN_PARAMETER_INDEX_DATA = 0;

    //*Variables PS*
    //Parameter
    DWORD dCobId;
    BYTE uLength;

    //ReturnParameter
    void* pRetDataBuffer = NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    DWORD dTimeout;
    DWORD time, start;

    if(pCommand && pInterfaceManager && m_pSendingFrame && m_pReceivingFrame)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_COBID, &dCobId, sizeof(dCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &uLength, sizeof(uLength));

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_RequestCANFrame(dCobId, uLength);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);
        if(oResult)
        {
            //Check Timeout
            dTimeout = pCommand->GetTimeout();

            start = MmcGetTickCount();

            do
            {
                //Receive Frame
                oResult = ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobId, m_pReceivingFrame, dTimeout, &errorInfo);

                //Update Time
                time = MmcGetTickCount() - start;

            }while((!oResult) && (time < dTimeout));

            //ErrorInfo
            if((!oResult) && (time >= dTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_RequestedCanFrameNotReceived, &errorInfo);
            }
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_RequestCANFrame(&pRetDataBuffer, &dRetDataBufferLength);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pRetDataBuffer, dRetDataBufferLength);

        //Free DataBuffer
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_SendLSSFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_DATA = 0;

    //*Variables PS*
    //Parameter
    DWORD dCobId = 2021;
    BYTE uLength = 8;
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(pCommand && pInterfaceManager && m_pSendingFrame)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Prepare DataBuffer
        dDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

        //Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);

        //Prepare Sending Frame
        m_pSendingFrame->PrepareSendFrame_SendCANFrame(dCobId, uLength, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = SendFrame(pInterfaceManager, hI_Handle, hTransactionHandle, m_pSendingFrame, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayCANopenToI::Process_ReadLSSFrame(CCommand_PS* pCommand, CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle)
{
    //*Constants PS*
    const int PARAMETER_INDEX_TIMEOUT = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 0;

    //*Variables PS*
    //Parameter
    DWORD dCobId = 2020;
    WORD wTimeout;

    //ReturnParameter
    void* pRetDataBuffer = NULL;
    DWORD dRetDataBufferLength = 0;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;
    DWORD dTimeout = 0;
    DWORD time = 0;
    DWORD start = 0;

    if(pCommand && pInterfaceManager && m_pReceivingFrame)
    {
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get PS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &wTimeout, sizeof(wTimeout));

        //Check Timeout
        dTimeout = (DWORD)wTimeout;

        start = MmcGetTickCount();

        do
        {
            //Receive Frame
            oResult = ReceiveFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobId, m_pReceivingFrame, dTimeout, &errorInfo);

            //Update Time
            time = MmcGetTickCount() - start;

        }while((!oResult) && (time < dTimeout));

        //ErrorInfo
        if((!oResult) && (time >= dTimeout))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CANopen_CanFrameNotReceived, &errorInfo);
        }

        //CopyReceivedData Receiving Frame
        if(oResult) m_pReceivingFrame->CopyReceivedData_ReceivedCANFrame(&pRetDataBuffer, &dRetDataBufferLength);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pRetDataBuffer, dRetDataBufferLength);

        //Free DataBuffer
        if(pRetDataBuffer) free(pRetDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayCANopenToI::SendFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, CCANopenFrame* pFrame, CErrorInfo* pErrorInfo)
{
    if(pFrame)
    {
        if(pErrorInfo) pErrorInfo->Reset();

        return I_TransmitCanFrame(pInterfaceManager, hI_Handle, hTransactionHandle, pFrame->GetCobId(), pFrame->GetRtr(), pFrame->GetDlc(), pFrame->GetCanDataPointer(), pFrame->GetCanDataSize(), pErrorInfo);
    }

    return FALSE;
}

BOOL CGatewayCANopenToI::ReceiveFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, CCANopenFrame* pFrame, DWORD dTimeout, CErrorInfo* pErrorInfo)
{
    const BOOL RTR_FRAME = FALSE;

    if(pFrame)
    {
        if(pErrorInfo) pErrorInfo->Reset();

        return I_ReceiveFilteredCanFrame(pInterfaceManager, hI_Handle, hTransactionHandle, dCobId, RTR_FRAME, (DWORD*)pFrame->GetCobIdPointer(), (BOOL*)pFrame->GetRtrPointer(), (BYTE*)pFrame->GetDlcPointer(), pFrame->GetCanDataPointer(), pFrame->GetCanDataSize(), dTimeout, pErrorInfo);
    }

    return FALSE;
}

BOOL CGatewayCANopenToI::CheckReceivedFrame(CCANopenFrame* pSendingFrame, CCANopenFrame* pReceivingFrame)
{
    if(pSendingFrame && pReceivingFrame)
    {
        //Check Command Specifier
        if(!CCANopenFrame::CheckReceivedCommandSpecifier(pSendingFrame, pReceivingFrame)) return FALSE;

        //Check Index & SubIndex
        if(!CCANopenFrame::CheckReceivedMultiplexor(pSendingFrame, pReceivingFrame)) return FALSE;

		//Check TargetNetwork & TargetNodeId
        if(!CCANopenFrame::CheckReceivedTargetNetwork(pSendingFrame, pReceivingFrame)) return FALSE;

        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayCANopenToI::I_TransmitCanFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, BOOL oRtr, BYTE uDlc, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_DATA_LENGTH = 8;

    DWORD dDataLength;
    BOOL oResult = FALSE;

    if(m_pCommand_TransmitCanFrame && pInterfaceManager)
    {
        //Reset
        pInterfaceManager->I_ResetInterface(hI_Handle);

        //Limit DataLength
        dDataLength = uDlc;
        if(dDataLength > MAX_DATA_LENGTH) dDataLength = MAX_DATA_LENGTH;
        if(dDataLength > dDataBufferLength) dDataLength = dDataBufferLength;

        //Set Parameter Data
        m_pCommand_TransmitCanFrame->ResetStatus();
        m_pCommand_TransmitCanFrame->SetParameterData(0, &dCobId, sizeof(dCobId));
        m_pCommand_TransmitCanFrame->SetParameterData(1, &oRtr, sizeof(oRtr));
        m_pCommand_TransmitCanFrame->SetParameterData(2, &uDlc, sizeof(uDlc));
        m_pCommand_TransmitCanFrame->SetParameterData(3, pDataBuffer, dDataLength);

        //Execute Command
        oResult = pInterfaceManager->ExecuteCommand(m_pCommand_TransmitCanFrame, hI_Handle, hTransactionHandle);

        //Get Error
        m_pCommand_TransmitCanFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayCANopenToI::I_ReceiveCanFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD* pdCobId, BOOL* poRtr, BYTE* puDlc, void* pDataBuffer, DWORD dDataBufferLength, DWORD dTimeout, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_DATA_LENGTH = 8;

    DWORD dDataLength;
    BOOL oResult = FALSE;

    if(m_pCommand_ReceiveCanFrame && pInterfaceManager)
    {
        //Limit dataLength
        dDataLength = MAX_DATA_LENGTH;
        if(dDataLength > dDataBufferLength) dDataLength = dDataBufferLength;

        //Set Parameter Data
        m_pCommand_ReceiveCanFrame->ResetStatus();
        m_pCommand_ReceiveCanFrame->SetTimeout(dTimeout);

        //Execute Command
        oResult = pInterfaceManager->ExecuteCommand(m_pCommand_ReceiveCanFrame, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReceiveCanFrame->GetReturnParameterData(0, pdCobId, sizeof(*pdCobId));
        m_pCommand_ReceiveCanFrame->GetReturnParameterData(1, poRtr, sizeof(*poRtr));
        m_pCommand_ReceiveCanFrame->GetReturnParameterData(2, puDlc, sizeof(*puDlc));
        m_pCommand_ReceiveCanFrame->GetReturnParameterData(3, pDataBuffer, dDataLength);

        //Get ErrorCode
        m_pCommand_ReceiveCanFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayCANopenToI::I_ReceiveFilteredCanFrame(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdFilter, BOOL oRtrFilter, DWORD* pdCobId, BOOL* poRtr, BYTE* puDlc, void* pDataBuffer, DWORD dDataBufferLength, DWORD dTimeout, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_DATA_LENGTH = 8;

    DWORD dDataLength;
    BOOL oResult = FALSE;

    if(m_pCommand_ReceiveFilteredCanFrame && pInterfaceManager)
    {
        //Limit dataLength
        dDataLength = MAX_DATA_LENGTH;
        if(dDataLength > dDataBufferLength) dDataLength = dDataBufferLength;

        //Set Parameter Data
        m_pCommand_ReceiveFilteredCanFrame->ResetStatus();
        m_pCommand_ReceiveFilteredCanFrame->SetTimeout(dTimeout);
        m_pCommand_ReceiveFilteredCanFrame->SetParameterData(0, &dCobIdFilter, sizeof(dCobIdFilter));
        m_pCommand_ReceiveFilteredCanFrame->SetParameterData(1, &oRtrFilter, sizeof(oRtrFilter));

        //Execute Command
        oResult = pInterfaceManager->ExecuteCommand(m_pCommand_ReceiveFilteredCanFrame, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReceiveFilteredCanFrame->GetReturnParameterData(0, pdCobId, sizeof(*pdCobId));
        m_pCommand_ReceiveFilteredCanFrame->GetReturnParameterData(1, poRtr, sizeof(*poRtr));
        m_pCommand_ReceiveFilteredCanFrame->GetReturnParameterData(2, puDlc, sizeof(*puDlc));
        m_pCommand_ReceiveFilteredCanFrame->GetReturnParameterData(3, pDataBuffer, dDataLength);

        //Get ErrorCode
        m_pCommand_ReceiveFilteredCanFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayCANopenToI::SetNetworkIndication()
{
	BOOL oResult = FALSE;

	m_ulNetworkIndicationThreadId = MmcGetCurrentThreadId();

	oResult = TRUE;

	return oResult;
}

BOOL CGatewayCANopenToI::ResetNetworkIndication()
{
	BOOL oResult = FALSE;

	m_ulNetworkIndicationThreadId = 0;
	oResult = TRUE;

	return oResult;
}

BOOL CGatewayCANopenToI::IsNetworkIndicationActive()
{
	return (m_ulNetworkIndicationThreadId != 0);
}

BOOL CGatewayCANopenToI::IsNetworkIndicationThread()
{
	BOOL oResult = FALSE;

	if(IsNetworkIndicationActive())
	{
		oResult = (MmcGetCurrentThreadId() == m_ulNetworkIndicationThreadId);
	}

	return oResult;
}

BOOL CGatewayCANopenToI::InitLayerParameterStack(CCommandRoot* p_pCommand)
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
			if(m_pCommand_TransmitCanFrame) m_pCommand_TransmitCanFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReceiveCanFrame) m_pCommand_ReceiveCanFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReceiveFilteredCanFrame) m_pCommand_ReceiveFilteredCanFrame->SetLayerParameterStack(layerParameterStack);
			oResult = TRUE;
		}
	}

	return oResult;
}
