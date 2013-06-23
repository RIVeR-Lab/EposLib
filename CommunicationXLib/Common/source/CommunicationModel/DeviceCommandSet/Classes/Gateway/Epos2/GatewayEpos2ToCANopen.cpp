// GatewayEpos2ToCANopen.cpp: Implementierung der Klasse CGatewayEpos2ToCANopen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayEpos2ToCANopen.h"
#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEpos2Def.h>
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
CGatewayEpos2ToCANopen::CGatewayEpos2ToCANopen()
{
    m_pCommand_InitiateSDODownload = NULL;
    m_pCommand_DownloadSDOSegment = NULL;
    m_pCommand_InitiateSDOUpload = NULL;
    m_pCommand_UploadSDOSegment = NULL;
    m_pCommand_SendNMTService = NULL;
    m_pCommand_SendCANFrame = NULL;
    m_pCommand_ReadCANFrame = NULL;
    m_pCommand_RequestCANFrame = NULL;
    m_pCommand_AbortSDOTransfer = NULL;
    m_pCommand_SendLSSFrame = NULL;
    m_pCommand_ReadLSSFrame = NULL;
    InitCommands();
    InitErrorHandling();
}

CGatewayEpos2ToCANopen::~CGatewayEpos2ToCANopen()
{
    DeleteCommands();
}

CGateway* CGatewayEpos2ToCANopen::Clone()
{
    CGatewayEpos2ToCANopen* pClonedGateway;

    pClonedGateway = new CGatewayEpos2ToCANopen();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayEpos2ToCANopen& CGatewayEpos2ToCANopen::operator=(CGatewayEpos2ToCANopen& other)
{
    if(this != &other)
    {
        *((CGatewayDCStoPS*)this) = *((CGatewayDCStoPS*)&other);
    }

    return *this;
}

BOOL CGatewayEpos2ToCANopen::ProcessCommand(CCommandRoot*pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    CCommand_DCS* pCommand_DCS;
    CProtocolStackManagerBase* pProtocolStackManager;

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
            case EPOS2_REQUEST_CAN_FRAME: return Process_RequestCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
            case EPOS2_READ_CAN_FRAME: return Process_ReadCANFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);

            //Layer Setting Services
            case EPOS2_SEND_LSS_FRAME: return Process_SendLSSFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
            case EPOS2_READ_LSS_FRAME: return Process_ReadLSSFrame(pCommand_DCS, pProtocolStackManager, h_Handle, hTransactionHandle);
        }
    }

    return FALSE;
}

BOOL CGatewayEpos2ToCANopen::Process_ReadObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 3;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    DWORD dObjectLength = 0;
    //Return Parameter
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    //ReturnParameter
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE uNonValidNbOfBytes;
    DWORD dReturnDataLength;
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

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

        //Prepare DataBuffer
        memset(maxDataBuffer, 0, sizeof(maxDataBuffer));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, &dCobIdServerClient);

        //Execute Command
        oResult = PS_InitiateSDOUpload(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, wIndex, uSubIndex, &oExpeditedTransfer, &oSizeIndicated, &uNonValidNbOfBytes, maxDataBuffer, MAX_OBJECT_LENGTH, &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Restore Length
        if(oSizeIndicated) dReturnDataLength = MAX_OBJECT_LENGTH-uNonValidNbOfBytes; else dReturnDataLength = MAX_OBJECT_LENGTH;

        //Evaluate PS ReturnParameter
        if(oResult && !oExpeditedTransfer)
        {
            //Abort Segmented Transfer
            dAbortCode = ERROR_DEVICE_EPOS_GENERAL;
            PS_AbortSDOTransfer(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, wIndex, uSubIndex, dAbortCode);

            //ErrorHandling
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxDataBuffer, dReturnDataLength);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_WriteObject(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int PARAMETER_INDEX_DATA = 3;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE maxDataBuffer[MAX_OBJECT_LENGTH];
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

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Check max objectLength
        if(dObjectLength > MAX_OBJECT_LENGTH) dObjectLength = MAX_OBJECT_LENGTH;
        memset(maxDataBuffer, 0, MAX_OBJECT_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxDataBuffer, dObjectLength);

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

BOOL CGatewayEpos2ToCANopen::Process_InitiateSegmentedRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Variables DCS*
    //Parameter
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE uNodeId = 0;
    //Return Parameter

    //*Variables PS*
    //Parameter
    DWORD dCobIdClientServer;
    DWORD dCobIdServerClient;
    //ReturnParameter
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE uNonValidNbOfBytes;
    DWORD dObjectLength;
    DWORD dAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;
    CErrorInfo cmdErrorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, &dCobIdServerClient);

        //Execute Command
        oResult = PS_InitiateSDOUpload(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, wIndex, uSubIndex, &oExpeditedTransfer, &oSizeIndicated, &uNonValidNbOfBytes, &dObjectLength, sizeof(dObjectLength), &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Evaluate PS ReturnParameter
        if(oResult && oExpeditedTransfer)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
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

BOOL CGatewayEpos2ToCANopen::Process_AbortSegmentedTransfer(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
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

BOOL CGatewayEpos2ToCANopen::Process_InitiateSegmentedWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_INDEX = 0;
    const int PARAMETER_INDEX_SUB_INDEX = 1;
    const int PARAMETER_INDEX_NODE_ID = 2;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Variables DCS*
    //Parameter
    WORD wIndex = 0;
    BYTE uSubIndex = 0;
    BYTE uNodeId = 0;
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

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &wIndex, sizeof(wIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &uSubIndex, sizeof(uSubIndex));
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &dObjectLength, sizeof(dObjectLength));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        oExpeditedTransfer = FALSE;
        oSizeIndicated = TRUE;
        uNonValidNbOfBytes = 0;

        //Execute Command
        oResult = PS_InitiateSDODownload(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, oExpeditedTransfer, oSizeIndicated, uNonValidNbOfBytes, wIndex, uSubIndex, &dObjectLength, sizeof(dObjectLength), &dAbortCode, &comErrorInfo);
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

BOOL CGatewayEpos2ToCANopen::Process_SegmentRead(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NODE_ID = 0;
    const int PARAMETER_INDEX_CONTROL_BYTE = 1;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int RETURN_PARAMETER_INDEX_DATA = 2;

    //*Constants PS*
    const BYTE MAX_SEGMENT_LENGTH = 7;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    UEpos2ControlByte controlByte;
    controlByte.bValue = 0;
    //Return Parameter
    UEpos2ControlByte retControlByte;
    retControlByte.bValue = 0;
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

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.bValue, sizeof(controlByte.bValue));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        oToggle = controlByte.structure.bToggle;

        //Execute Command
        oResult = PS_UploadSDOSegment(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, oToggle, &oRetToggle, &uNonValidNbOfBytes, &oNoMoreSegments, &maxDataBuffer, MAX_SEGMENT_LENGTH, &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Restore ControlByte
        retControlByte.structure.bToggle = oRetToggle;
        retControlByte.structure.bLength = MAX_SEGMENT_LENGTH-uNonValidNbOfBytes;
        retControlByte.structure.bMoreSegments = !oNoMoreSegments;

        //Evaluate PS ReturnParameter
        if(oResult && (controlByte.structure.bToggle != retControlByte.structure.bToggle))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxDataBuffer, retControlByte.structure.bLength);

		//Unlock CriticalSection
		if(oNoMoreSegments || !oResult)
		{
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_SegmentWrite(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NODE_ID = 0;
    const int PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int PARAMETER_INDEX_DATA = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 1;

    //*Constants PS*
    const BYTE MAX_SEGMENT_LENGTH = 7;

    //*Variables DCS*
    //Parameter
    BYTE uNodeId = 0;
    UEpos2ControlByte controlByte;
    controlByte.bValue = 0;
    BYTE maxDataBuffer[MAX_SEGMENT_LENGTH];
    //Return Parameter
    UEpos2ControlByte retControlByte;
    retControlByte.bValue = 0;

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

		//GetParameterData
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &uNodeId, sizeof(uNodeId));
        pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.bValue, sizeof(controlByte.bValue));

        //Check Max SegmentLength
        if(controlByte.structure.bLength > MAX_SEGMENT_LENGTH)
        {
            controlByte.structure.bMoreSegments = TRUE;
            controlByte.structure.bLength = MAX_SEGMENT_LENGTH;
        }
        memset(maxDataBuffer, 0, MAX_SEGMENT_LENGTH);
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, maxDataBuffer, controlByte.structure.bLength);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(uNodeId, &dCobIdClientServer, &dCobIdServerClient);
        oToggle = controlByte.structure.bToggle;
        uNonValidNbOfBytes = MAX_SEGMENT_LENGTH-controlByte.structure.bLength;
        oNoMoreSegments = !controlByte.structure.bMoreSegments;

        //Execute Command
        oResult = PS_DownloadSDOSegment(pProtocolStackManager, hPS_Handle, hTransactionHandle, dCobIdClientServer, dCobIdServerClient, oToggle, uNonValidNbOfBytes, oNoMoreSegments, maxDataBuffer, MAX_SEGMENT_LENGTH, &oRetToggle, &dAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, dAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Restore ControlByte
        retControlByte.structure.bLength = controlByte.structure.bLength;
        retControlByte.structure.bToggle = oRetToggle;

        //Evaluate PS ReturnParameter
        if(oResult && (controlByte.structure.bToggle != retControlByte.structure.bToggle))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //SetReturnParameterData
        pCommand->SetStatus(oResult, &cmdErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));

		//Unlock CriticalSection
		if(oNoMoreSegments || !oResult)
		{
			Unlock();
		}
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_SendNMTService(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NODE_ID = 0;
    const int PARAMETER_INDEX_CMD_SPECIFIER = 1;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Variables DCS*
    //Parameter
    WORD wId = 0;
    WORD wCmdSpecifier = 0;

    //*Variables PS*
    //Parameter
    BYTE uCommandSpecifier;
    BYTE uNodeId;
    //ReturnParameter
    DWORD dAbortCode = 0;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &wId, sizeof(wId));
        pCommand->GetParameterData(PARAMETER_INDEX_CMD_SPECIFIER, &wCmdSpecifier, sizeof(wCmdSpecifier));

        //Prepare PS Parameter
        uCommandSpecifier = (BYTE)wCmdSpecifier;
        uNodeId = (BYTE)wId;

        //Execute Command
        oResult = PS_SendNMTService(pProtocolStackManager, hPS_Handle, hTransactionHandle, uCommandSpecifier, uNodeId, &comErrorInfo);

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_SendCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_COB_ID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int PARAMETER_INDEX_DATA = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    WORD wCobId = 0;
    WORD wLength = 0;
    BYTE maxFrameBuffer[MAX_FRAME_LENGTH];

    //*Variables PS*
    //Parameter
    DWORD dCobId;
    BYTE uLength;
    //ReturnParameter
    DWORD dAbortCode = 0;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;

    if(pCommand && pProtocolStackManager)
    {
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
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

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dAbortCode, sizeof(dAbortCode));

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_ReadCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
//*Constants DCS*
    const int PARAMETER_INDEX_COB_ID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int PARAMETER_INDEX_TIMEOUT = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

//*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

//*Variables DCS*
    //Parameter
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

    if(pCommand && pProtocolStackManager)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
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

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, wLength);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_RequestCANFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_COB_ID = 0;
    const int PARAMETER_INDEX_LENGTH = 1;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
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

    if(pCommand && pProtocolStackManager)
    {
		//Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Get DCS Parameter Data
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
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, maxFrameBuffer, uLength);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_SendLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_DATA = 0;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    DWORD dDataBufferLength = 0;
    void* pDataBuffer = NULL;

    //*Variables PS*
    //Parameter
    //ReturnParameter
    DWORD dErrorCode = 0;
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;

    if(pCommand && pProtocolStackManager)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Prepare DataBuffer
        dDataBufferLength = pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(dDataBufferLength > MAX_FRAME_LENGTH) dDataBufferLength = MAX_FRAME_LENGTH;
        if(dDataBufferLength > 0)
        {
            pDataBuffer = malloc(dDataBufferLength);
            memset(pDataBuffer, 0, dDataBufferLength);
        }

        //Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = PS_SendLSSFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, pDataBuffer, dDataBufferLength, &comErrorInfo);
        if(!oResult) dErrorCode = comErrorInfo.GetErrorCode();

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dErrorCode, sizeof(dErrorCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::Process_ReadLSSFrame(CCommand_DCS* pCommand, CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_TIMEOUT = 0;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    WORD wTimeout;
    //ReturnParameter
    DWORD dErrorCode = 0;
    DWORD dDataBufferLength = 0;
    void* pDataBuffer = NULL;

    //*Variables PS*
    //Parameter
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult = FALSE;

    if(pCommand && pProtocolStackManager)
    {
        //Lock CriticalSection
		if(!Lock(pCommand)) return FALSE;

		//Prepare DataBuffer
        dDataBufferLength = MAX_FRAME_LENGTH;
        pDataBuffer = malloc(dDataBufferLength);
        memset(pDataBuffer, 0, dDataBufferLength);

        //Get DCS Parameter Data
        pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = PS_ReadLSSFrame(pProtocolStackManager, hPS_Handle, hTransactionHandle, wTimeout, pDataBuffer, dDataBufferLength, &comErrorInfo);
        if(!oResult) dErrorCode = comErrorInfo.GetErrorCode();

        //Set DCS ReturnParameter Data
        pCommand->SetStatus(oResult, &comErrorInfo);
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &dErrorCode, sizeof(dErrorCode));
        pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pDataBuffer, dDataBufferLength);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::EvaluateErrorCode(BOOL oResult, DWORD dDeviceErrorCode, CErrorInfo* pComErrorInfo, CErrorInfo* pCmdErrorInfo)
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

void CGatewayEpos2ToCANopen::InitCommands()
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

    //Init SendLSSFrame
    m_pCommand_SendLSSFrame = new CCommand_PS_CANopen();
    m_pCommand_SendLSSFrame->InitCommand(CANOPEN_SEND_LSS_FRAME);

    //Init ReadLSSFrame
    m_pCommand_ReadLSSFrame = new CCommand_PS_CANopen();
    m_pCommand_ReadLSSFrame->InitCommand(CANOPEN_READ_LSS_FRAME);
}

void CGatewayEpos2ToCANopen::DeleteCommands()
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

    if(m_pCommand_SendLSSFrame)
    {
        delete m_pCommand_SendLSSFrame;
        m_pCommand_SendLSSFrame = NULL;
    }

    if(m_pCommand_ReadLSSFrame)
    {
        delete m_pCommand_ReadLSSFrame;
        m_pCommand_ReadLSSFrame = NULL;
    }
}

BOOL CGatewayEpos2ToCANopen::PS_InitiateSDODownload(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oExpeditedTransfer, BOOL oSizeIndicated, BYTE uNonValidNbOfBytes, WORD wIndex, BYTE uSubIndex, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_DownloadSDOSegment(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BYTE uNonValidNbOfBytes, BOOL oNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, BOOL* poRetToggle, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_AbortSDOTransfer(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, WORD wIndex, BYTE uSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_InitiateSDOUpload(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, WORD wIndex, BYTE uSubIndex, BOOL* poExpeditedTransfer, BOOL* poSizeIndicated, BYTE* puNonValidNbOfBytes, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_UploadSDOSegment(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BOOL* poRetToggle, BYTE* puNonValidNbOfBytes, BOOL* poNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_SendNMTService(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, BYTE uCommandSpecifier, BYTE uNodeId, CErrorInfo* pErrorInfo)
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

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::PS_SendCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_ReadCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, DWORD p_ulTimeout, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_RequestCANFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
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

BOOL CGatewayEpos2ToCANopen::PS_SendLSSFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_SendLSSFrame && pProtocolStackManager)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_SendLSSFrame, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::PS_ReadLSSFrame(CProtocolStackManagerBase* pProtocolStackManager, HANDLE hPS_Handle, HANDLE hTransactionHandle, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_ReadLSSFrame && pProtocolStackManager)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = pProtocolStackManager->ExecuteCommand(m_pCommand_ReadLSSFrame, hPS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEpos2ToCANopen::CalculateDefaultSdoCobIds(BYTE uNodeId, DWORD* pdCobIdClientServer, DWORD* pdCobIdServerClient)
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

BOOL CGatewayEpos2ToCANopen::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayEpos2ToCANopen";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEpos2ToCANopen::InitLayerParameterStack(CCommandRoot* p_pCommand)
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

			if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->SetLayerParameterStack(layerParameterStack);

			oResult = TRUE;
		}
	}

	return oResult;
}

