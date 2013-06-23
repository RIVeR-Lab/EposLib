// GatewayEsamToCANopen.cpp: Implementierung der Klasse CGatewayEsamToCANopen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayEsamToCANopen.h"

#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_CANopen.h>
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
CGatewayEsamToCANopen::CGatewayEsamToCANopen()
{
    m_uNodeId = 0;
    m_wIndex = 0;
    m_uSubIndex = 0;
    m_oToggle = FALSE;

    m_pCommand_InitiateSDODownload = NULL;
    m_pCommand_DownloadSDOSegment = NULL;
    m_pCommand_InitiateSDOUpload = NULL;
    m_pCommand_UploadSDOSegment = NULL;
    m_pCommand_SendNMTService = NULL;
    m_pCommand_SendCANFrame = NULL;
    m_pCommand_ReadCANFrame = NULL;
    m_pCommand_RequestCANFrame = NULL;
    m_pCommand_AbortSDOTransfer = NULL;

    InitCommands();
    InitErrorHandling();
}

CGatewayEsamToCANopen::~CGatewayEsamToCANopen()
{
    DeleteCommands();
}


CGateway* CGatewayEsamToCANopen::Clone()
{
    CGatewayEsamToCANopen* pClonedGateway;

    pClonedGateway = new CGatewayEsamToCANopen();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayEsamToCANopen& CGatewayEsamToCANopen::operator=(CGatewayEsamToCANopen& other)
{
    if(this != &other)
    {
        *((CGatewayDCStoPS*)this) = *((CGatewayDCStoPS*)&other);
    }

    return *this;
}

BOOL CGatewayEsamToCANopen::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
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
				case ESAM_REQUEST_CAN_FRAME: return Process_RequestCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
				case ESAM_READ_CAN_FRAME: return Process_ReadCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

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

BOOL CGatewayEsamToCANopen::Process_ReadObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    //Return Parameter
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];
    memset(maxDataBuffer, 0, MAX_OBJECT_LENGTH);

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    //ReturnParameter
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE uNonValidNbOfBytes;
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));

        //Prepare DataBuffer
        memset(maxDataBuffer, 0, sizeof(maxDataBuffer));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, &dCobIdServerClient);

        //Execute Command
        oResult = PS_InitiateSDOUpload(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, wIndex, uSubIndex, &oExpeditedTransfer, &oSizeIndicated, &uNonValidNbOfBytes, maxDataBuffer, MAX_OBJECT_LENGTH, &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Evaluate PS ReturnParameter
        if(oResult && !oExpeditedTransfer)
        {
            //Abort SDO Transfer
            PS_AbortSDOTransfer(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, wIndex, uSubIndex, dAbortCode);

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxDataBuffer, MAX_OBJECT_LENGTH);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_WriteObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;
    const int PARAMETER_INDEX_DATA = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];
    memset(maxDataBuffer, 0, MAX_OBJECT_LENGTH);

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE uNonValidNbOfBytes;

    //ReturnParameter
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxDataBuffer, MAX_OBJECT_LENGTH);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        oExpeditedTransfer = TRUE;
        oSizeIndicated = FALSE;
        uNonValidNbOfBytes = 0;

        //Execute Command
        oResult = PS_InitiateSDODownload(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, oExpeditedTransfer, oSizeIndicated, uNonValidNbOfBytes, wIndex, uSubIndex, maxDataBuffer, MAX_OBJECT_LENGTH, &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_InitiateSegmentedRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_OBJECT_LENGTH = 1;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    //Return Parameter
    DWORD dObjectLength = 0;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    //ReturnParameter
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE uNonValidNbOfBytes;
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &m_wIndex, sizeof(m_wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &m_uSubIndex, sizeof(m_uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &m_uNodeId, sizeof(m_uNodeId));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(m_uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        m_oToggle = FALSE;

        //Execute Command
        oResult = PS_InitiateSDOUpload(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, m_wIndex, m_uSubIndex, &oExpeditedTransfer, &oSizeIndicated, &uNonValidNbOfBytes, &dObjectLength, sizeof(dObjectLength), &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Evaluate PS ReturnParameter
        if(oResult && oExpeditedTransfer)
        {
            //Abort SDO Transfer
            PS_AbortSDOTransfer(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, m_wIndex, m_uSubIndex, dAbortCode);

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

		//Unlock CriticalSection
		if(oExpeditedTransfer || !oResult)
		{
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_InitiateSegmentedWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_NODE_ID = 3;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    DWORD dObjectLength = 0;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE uNonValidNbOfBytes;

    //ReturnParameter
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &m_wIndex, sizeof(m_wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &m_uSubIndex, sizeof(m_uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &m_uNodeId, sizeof(m_uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(m_uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        m_oToggle = FALSE;
        oExpeditedTransfer = FALSE;
        oSizeIndicated = TRUE;
        uNonValidNbOfBytes = 0;

        //Execute Command
        oResult = PS_InitiateSDODownload(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, oExpeditedTransfer, oSizeIndicated, uNonValidNbOfBytes, m_wIndex, m_uSubIndex, &dObjectLength, sizeof(dObjectLength), &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));

		//Unlock CriticalSection
		if(oExpeditedTransfer || !oResult)
		{
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_SegmentRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int RETURN_PARAMETER_INDEX_DATA = 2;

    //*Constants PS*
    const BYTE MAX_SEGMENT_LENGTH = 7;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    //Return Parameter
    UEsamControlByte retControlByte;
    retControlByte.ubValue = 0;
    BYTE maxDataBuffer[MAX_SEGMENT_LENGTH];

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    BOOL oToggle;

    //ReturnParameter
    BOOL oRetToggle;
    BYTE uNonValidNbOfBytes;
    BOOL oNoMoreSegments;
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Check CriticalSection
		if(!IsLocked(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(m_uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        oToggle = m_oToggle;
        m_oToggle = !m_oToggle;

        //Execute Command
        oResult = PS_UploadSDOSegment(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, oToggle, &oRetToggle, &uNonValidNbOfBytes, &oNoMoreSegments, &maxDataBuffer, MAX_SEGMENT_LENGTH, &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Restore ControlByte
        retControlByte.structure.ubLastDataSegment = oNoMoreSegments;
        retControlByte.structure.ubNbOfDataBytes = MAX_SEGMENT_LENGTH-uNonValidNbOfBytes;

        //Evaluate PS ReturnParameter
        if(oResult && (oToggle != oRetToggle))
        {
            //Abort SDO Transfer
            PS_AbortSDOTransfer(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, m_wIndex, m_uSubIndex, dAbortCode);

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxDataBuffer, retControlByte.structure.ubNbOfDataBytes);

		//Unlock CriticalSection
		if(oNoMoreSegments || !oResult)
		{
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_AbortSegmentedTransfer(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NODE_ID = 0;
    const int PARAMETER_INDEX_INDEX = 1;
    const int PARAMETER_INDEX_SUB_INDEX = 2;
    const int PARAMETER_INDEX_ABORT_CODE = 3;

    //*Variables DCS*
    //Parameter
    WORD wNodeId = 0;
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    DWORD dAbortCode = 0;

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &wNodeId, sizeof(wNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_ABORT_CODE, &dAbortCode, sizeof(dAbortCode));

		//Prepare PS Parameter
        uNodeId = (BYTE)wNodeId;
        dAbortCode = ERROR_DEVICE_EPOS_GENERAL;
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, NULL);

        //Execute Command
        oResult = PS_AbortSDOTransfer(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, wIndex, uSubIndex, dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_SegmentWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int PARAMETER_INDEX_DATA = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_SEGMENT_LENGTH = 7;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    UEsamControlByte controlByte;
    controlByte.ubValue = 0;
    BYTE maxDataBuffer[MAX_SEGMENT_LENGTH];
    //Return Parameter

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    BOOL oToggle;
    BYTE uNonValidNbOfBytes;
    BOOL oNoMoreSegments;

    //ReturnParameter
    BOOL oRetToggle;
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Check CriticalSection
		if(!IsLocked(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//GetParameterData
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.ubValue, sizeof(controlByte.ubValue));

        //Check Max SegmentLength
        if(controlByte.structure.ubNbOfDataBytes > MAX_SEGMENT_LENGTH)
        {
            controlByte.structure.ubNbOfDataBytes = MAX_SEGMENT_LENGTH;
            controlByte.structure.ubLastDataSegment = FALSE;
        }
        memset(maxDataBuffer, 0, MAX_SEGMENT_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxDataBuffer, controlByte.structure.ubNbOfDataBytes);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(m_uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        oToggle = m_oToggle;
        m_oToggle = !m_oToggle;
        uNonValidNbOfBytes = MAX_SEGMENT_LENGTH-controlByte.structure.ubNbOfDataBytes;
        oNoMoreSegments = controlByte.structure.ubLastDataSegment;

        //Execute Command
        oResult = PS_DownloadSDOSegment(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, oToggle, uNonValidNbOfBytes, oNoMoreSegments, maxDataBuffer, MAX_SEGMENT_LENGTH, &oRetToggle, &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Evaluate PS ReturnParameter
        if(oResult && (oToggle != oRetToggle))
        {
            //Abort SDO Transfer
            PS_AbortSDOTransfer(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, m_wIndex, m_uSubIndex, dAbortCode);

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));

		//Unlock CriticalSection
		if(oNoMoreSegments || !oResult)
		{
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_SendNMTService(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_NODE_ID = 1;
    const int PARAMETER_INDEX_CMD_SPECIFIER = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    WORD wId = 0;
    WORD wCmdSpecifier = 0;
    //ReturnParameter
    DWORD dDeviceErrorCode = 0;

    //*Variables PS*
    //Parameter
    BYTE uCommandSpecifier;
    BYTE uNodeId;
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &wId, sizeof(wId));
        pCommand->GetParameterData(PARAMETER_INDEX_CMD_SPECIFIER, &wCmdSpecifier, sizeof(wCmdSpecifier));

        //Prepare PS Parameter
        uCommandSpecifier = (BYTE)wCmdSpecifier;
        uNodeId = (BYTE)wId;

        //Execute Command
        oResult = PS_SendNMTService(pProtocolStackManager, hPS_Handle, hTransactionHandle, uCommandSpecifier, uNodeId, &comErrorInfo);
        dDeviceErrorCode = comErrorInfo.GetErrorCode();

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_SendCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_COB_ID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;
    const int PARAMETER_INDEX_DATA = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

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
    DWORD dCobId;
    BYTE uLength;
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID, &wCobId, sizeof(wCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &wLength, sizeof(wLength));

        //Limit Length
        if(wLength > MAX_FRAME_LENGTH) wLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

        //Prepare PS Parameter
        dCobId = wCobId;
        uLength = (BYTE)wLength;

        //Execute Command
        oResult = PS_SendCANFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobId, uLength, maxFrameBuffer, uLength, &comErrorInfo);
        dDeviceErrorCode = comErrorInfo.GetErrorCode();

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_ReadCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
//*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_COB_ID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;
    const int PARAMETER_INDEX_TIMEOUT = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

//*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

//*Variables DCS*
    //Parameter
    BYTE uPortNumber = 0;
    WORD wCobId = 0;
    WORD wLength = 0;
    DWORD ulTimeout = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD dDeviceErrorCode = 0;

//*Variables PS*
    //Parameter
    DWORD dCobId;
    BYTE uLength;
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID, &wCobId, sizeof(wCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &wLength, sizeof(wLength));
        pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &ulTimeout, sizeof(ulTimeout));

        //Limit Length
        if(wLength > MAX_FRAME_LENGTH) wLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare PS Parameter
        dCobId = wCobId;
        uLength = (BYTE)wLength;

        //Execute Command
        oResult = PS_ReadCANFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobId, uLength, maxFrameBuffer, uLength, ulTimeout, &comErrorInfo);
        dDeviceErrorCode = comErrorInfo.GetErrorCode();

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_RequestCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_PORT_NUMBER = 0;
    const int PARAMETER_INDEX_COB_ID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

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
    DWORD dCobId;
    BYTE uLength;

    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Init
		InitLayerParameterStack(pCommand);

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_PORT_NUMBER, &uPortNumber, sizeof(uPortNumber));
        pCommand->GetParameterData(PARAMETER_INDEX_COB_ID, &wCobId, sizeof(wCobId));
        pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &wLength, sizeof(wLength));

        //Limit Length
        if(wLength > MAX_FRAME_LENGTH) wLength = MAX_FRAME_LENGTH;
        memset(maxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Prepare PS Parameter
        dCobId = wCobId;
        uLength = (BYTE)wLength;

        //Execute Command
        oResult = PS_RequestCANFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobId, uLength, maxFrameBuffer, uLength, &comErrorInfo);
        dDeviceErrorCode = comErrorInfo.GetErrorCode();

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::Process_SendLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //Not Supported
    return FALSE;
}

BOOL CGatewayEsamToCANopen::Process_ReadLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //Not Supported
    return FALSE;
}

BOOL CGatewayEsamToCANopen::Process_Login(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //Not Supported
    return FALSE;
}

BOOL CGatewayEsamToCANopen::Process_Logout(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //Not Supported
    return FALSE;
}

BOOL CGatewayEsamToCANopen::Process_SetState(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //Not Supported
    return FALSE;
}

BOOL CGatewayEsamToCANopen::EvaluateErrorCode(BOOL oResult, DWORD dDeviceErrorCode, CErrorInfo* pComErrorInfo, CErrorInfo* pCmdErrorInfo)
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

void CGatewayEsamToCANopen::InitCommands()
{
    DeleteCommands();

    //Init InitiateSDODownload
    m_pCommand_InitiateSDODownload = new CCommand_PS_CANopen();
    m_pCommand_InitiateSDODownload->InitCommand(CANOPEN_INITIATE_SDO_DOWNLOAD);

    //Init DownloadSDOSegment
    m_pCommand_DownloadSDOSegment = new CCommand_PS_CANopen();
    m_pCommand_DownloadSDOSegment->InitCommand(CANOPEN_DOWNLOAD_SDO_SEGMENT);

    //Init InitiateSDOUpload
    m_pCommand_InitiateSDOUpload = new CCommand_PS_CANopen();
    m_pCommand_InitiateSDOUpload->InitCommand(CANOPEN_INITIATE_SDO_UPLOAD);

    //Init UploadSDOSegment
    m_pCommand_UploadSDOSegment = new CCommand_PS_CANopen();
    m_pCommand_UploadSDOSegment->InitCommand(CANOPEN_UPLOAD_SDO_SEGMENT);

    //Init SendNMTService
    m_pCommand_SendNMTService = new CCommand_PS_CANopen();
    m_pCommand_SendNMTService->InitCommand(CANOPEN_SEND_NMT_SERVICE);

    //Init SendCANFrame
    m_pCommand_SendCANFrame = new CCommand_PS_CANopen();
    m_pCommand_SendCANFrame->InitCommand(CANOPEN_SEND_CAN_FRAME);

    //Init ReadCANFrame
    m_pCommand_ReadCANFrame = new CCommand_PS_CANopen();
    m_pCommand_ReadCANFrame->InitCommand(CANOPEN_READ_CAN_FRAME);

    //Init RequestCANFrame
    m_pCommand_RequestCANFrame = new CCommand_PS_CANopen();
    m_pCommand_RequestCANFrame->InitCommand(CANOPEN_REQUEST_CAN_FRAME);

    //Init AbortSDOTransfer
    m_pCommand_AbortSDOTransfer = new CCommand_PS_CANopen();
    m_pCommand_AbortSDOTransfer->InitCommand(CANOPEN_ABORT_SDO_TRANSFER);
}

void CGatewayEsamToCANopen::DeleteCommands()
{
    if(m_pCommand_InitiateSDODownload)
    {
        delete m_pCommand_InitiateSDODownload;
        m_pCommand_InitiateSDODownload = NULL;
    }

    if(m_pCommand_DownloadSDOSegment)
    {
        delete m_pCommand_DownloadSDOSegment;
        m_pCommand_DownloadSDOSegment = NULL;
    }

    if(m_pCommand_InitiateSDOUpload)
    {
        delete m_pCommand_InitiateSDOUpload;
        m_pCommand_InitiateSDOUpload = NULL;
    }

    if(m_pCommand_UploadSDOSegment)
    {
        delete m_pCommand_UploadSDOSegment;
        m_pCommand_UploadSDOSegment = NULL;
    }

    if(m_pCommand_SendNMTService)
    {
        delete m_pCommand_SendNMTService;
        m_pCommand_SendNMTService = NULL;
    }

    if(m_pCommand_SendCANFrame)
    {
        delete m_pCommand_SendCANFrame;
        m_pCommand_SendCANFrame = NULL;
    }

    if(m_pCommand_ReadCANFrame)
    {
        delete m_pCommand_ReadCANFrame;
        m_pCommand_ReadCANFrame = NULL;
    }

    if(m_pCommand_RequestCANFrame)
    {
        delete m_pCommand_RequestCANFrame;
        m_pCommand_RequestCANFrame = NULL;
    }

    if(m_pCommand_AbortSDOTransfer)
    {
        delete m_pCommand_AbortSDOTransfer;
        m_pCommand_AbortSDOTransfer = NULL;
    }
}

BOOL CGatewayEsamToCANopen::PS_InitiateSDODownload(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oExpeditedTransfer, BOOL oSizeIndicated, BYTE uNonValidNbOfBytes, WORD wIndex, BYTE uSubIndex, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 4;

    DWORD dRetCobId;
    WORD wRetIndex;
    BYTE uRetSubIndex;

    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSDODownload && pProtocolStackManager)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_InitiateSDODownload->ResetStatus();
        m_pCommand_InitiateSDODownload->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_InitiateSDODownload->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_InitiateSDODownload->SetParameterData(2, &oExpeditedTransfer, sizeof(oExpeditedTransfer));
        m_pCommand_InitiateSDODownload->SetParameterData(3, &oSizeIndicated, sizeof(oSizeIndicated));
        m_pCommand_InitiateSDODownload->SetParameterData(4, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
        m_pCommand_InitiateSDODownload->SetParameterData(5, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSDODownload->SetParameterData(6, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSDODownload->SetParameterData(7, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_InitiateSDODownload, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSDODownload->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(1, &wRetIndex, sizeof(wRetIndex));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(2, &uRetSubIndex, sizeof(uRetSubIndex));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(3, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_InitiateSDODownload->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_DownloadSDOSegment(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BYTE uNonValidNbOfBytes, BOOL oNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, BOOL* poRetToggle, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 7;

    DWORD dRetCobId;

    BOOL oResult = FALSE;

    if(m_pCommand_DownloadSDOSegment && pProtocolStackManager)
    {
        //Limit DataLength
        if(dSegDataBufferLength > MAX_CAN_DATA) dSegDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_DownloadSDOSegment->ResetStatus();
        m_pCommand_DownloadSDOSegment->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_DownloadSDOSegment->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_DownloadSDOSegment->SetParameterData(2, &oToggle, sizeof(oToggle));
        m_pCommand_DownloadSDOSegment->SetParameterData(3, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
        m_pCommand_DownloadSDOSegment->SetParameterData(4, &oNoMoreSegments, sizeof(oNoMoreSegments));
        m_pCommand_DownloadSDOSegment->SetParameterData(5, pSegDataBuffer, dSegDataBufferLength);

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_DownloadSDOSegment, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(1, poRetToggle, sizeof(*poRetToggle));
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(2, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_DownloadSDOSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_AbortSDOTransfer(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, WORD wIndex, BYTE uSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_AbortSDOTransfer && pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_AbortSDOTransfer->ResetStatus();
        m_pCommand_AbortSDOTransfer->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_AbortSDOTransfer->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_AbortSDOTransfer->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_AbortSDOTransfer->SetParameterData(3, &dAbortCode, sizeof(dAbortCode));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_AbortSDOTransfer, hPS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSDOTransfer->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_InitiateSDOUpload(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, WORD wIndex, BYTE uSubIndex, BOOL* poExpeditedTransfer, BOOL* poSizeIndicated, BYTE* puNonValidNbOfBytes, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 4;

    DWORD dRetCobId;
    WORD wRetIndex;
    BYTE uRetSubIndex;

    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSDOUpload && pProtocolStackManager)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_InitiateSDOUpload->ResetStatus();
        m_pCommand_InitiateSDOUpload->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_InitiateSDOUpload->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_InitiateSDOUpload->SetParameterData(2, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSDOUpload->SetParameterData(3, &uSubIndex, sizeof(uSubIndex));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_InitiateSDOUpload, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(1, poExpeditedTransfer, sizeof(*poExpeditedTransfer));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(2, poSizeIndicated, sizeof(*poSizeIndicated));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(3, puNonValidNbOfBytes, sizeof(*puNonValidNbOfBytes));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(4, &wRetIndex, sizeof(wRetIndex));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(5, &uRetSubIndex, sizeof(uRetSubIndex));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(6, pDataBuffer, dDataBufferLength);
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(7, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_InitiateSDOUpload->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_UploadSDOSegment(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BOOL* poRetToggle, BYTE* puNonValidNbOfBytes, BOOL* poNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 7;

    DWORD dRetCobId;

    BOOL oResult = FALSE;

    if(m_pCommand_UploadSDOSegment && pProtocolStackManager)
    {
        //Limit DataLength
        if(dSegDataBufferLength > MAX_CAN_DATA) dSegDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_UploadSDOSegment->ResetStatus();
        m_pCommand_UploadSDOSegment->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_UploadSDOSegment->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_UploadSDOSegment->SetParameterData(2, &oToggle, sizeof(oToggle));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_UploadSDOSegment, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_UploadSDOSegment->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(1, poRetToggle, sizeof(*poRetToggle));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(2, puNonValidNbOfBytes, sizeof(*puNonValidNbOfBytes));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(3, poNoMoreSegments, sizeof(*poNoMoreSegments));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(4, pSegDataBuffer, dSegDataBufferLength);
        m_pCommand_UploadSDOSegment->GetReturnParameterData(5, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_UploadSDOSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_SendNMTService(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uCommandSpecifier, BYTE uNodeId, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SendNMTService && pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &uCommandSpecifier, sizeof(uCommandSpecifier));
        m_pCommand_SendNMTService->SetParameterData(1, &uNodeId, sizeof(uNodeId));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_SendNMTService, hPS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_SendCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_SendCANFrame && pProtocolStackManager)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &dCobId, sizeof(dCobId));
        m_pCommand_SendCANFrame->SetParameterData(1, &uLength, sizeof(uLength));
        m_pCommand_SendCANFrame->SetParameterData(2, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_SendCANFrame, hPS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_ReadCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, DWORD p_ulTimeout, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_ReadCANFrame && pProtocolStackManager)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_ReadCANFrame->ResetStatus();
        m_pCommand_ReadCANFrame->SetParameterData(0, &dCobId, sizeof(dCobId));
        m_pCommand_ReadCANFrame->SetParameterData(1, &uLength, sizeof(uLength));
        m_pCommand_ReadCANFrame->SetParameterData(2, &p_ulTimeout, sizeof(p_ulTimeout));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_ReadCANFrame, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::PS_RequestCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_RequestCANFrame && pProtocolStackManager)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &dCobId, sizeof(dCobId));
        m_pCommand_RequestCANFrame->SetParameterData(1, &uLength, sizeof(uLength));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_RequestCANFrame, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsamToCANopen::CalculateDefaultSdoCobIds(BYTE uNodeId, DWORD* pdCobIdClientServer, DWORD* pdCobIdServerClient)
{
    const DWORD TX_SDO_OFFSET = 0x600;
    const DWORD RX_SDO_OFFSET = 0x580;

    if(pdCobIdClientServer)
    {
        *pdCobIdClientServer = uNodeId + TX_SDO_OFFSET;
    }

    if(pdCobIdServerClient)
    {
        *pdCobIdServerClient = uNodeId + RX_SDO_OFFSET;
    }

    return TRUE;
}

BOOL CGatewayEsamToCANopen::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayEsamToCANopen";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEsamToCANopen::InitLayerParameterStack(CCommandRoot* p_pCommand)
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
			if(m_pCommand_InitiateSDODownload) m_pCommand_InitiateSDODownload->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_DownloadSDOSegment) m_pCommand_DownloadSDOSegment->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_InitiateSDOUpload) m_pCommand_InitiateSDOUpload->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_UploadSDOSegment) m_pCommand_UploadSDOSegment->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_AbortSDOTransfer) m_pCommand_AbortSDOTransfer->SetLayerParameterStack(layerParameterStack);

			if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadCANFrame) m_pCommand_ReadCANFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->SetLayerParameterStack(layerParameterStack);

			oResult = TRUE;
		}
	}

	return oResult;
}

