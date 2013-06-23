// GatewayEsam2ToCANopen.cpp: Implementierung der Klasse CGatewayEsam2ToCANopen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayEsam2ToCANopen.h"

#include <malloc.h>
#include <memory.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/BaseClasses/Command_DCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenCommunicationDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>
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
CGatewayEsam2ToCANopen::CGatewayEsam2ToCANopen()
{
    m_usIndex = 0;
    m_ubSubIndex = 0;

    m_pCommand_InitiateSDODownload = NULL;
    m_pCommand_DownloadSDOSegment = NULL;
    m_pCommand_InitiateSDOUpload = NULL;
    m_pCommand_UploadSDOSegment = NULL;
    m_pCommand_NetworkIndication = NULL;
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

CGatewayEsam2ToCANopen::~CGatewayEsam2ToCANopen()
{
    DeleteCommands();
}


CGateway* CGatewayEsam2ToCANopen::Clone()
{
    CGatewayEsam2ToCANopen* pClonedGateway;

    pClonedGateway = new CGatewayEsam2ToCANopen();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayEsam2ToCANopen& CGatewayEsam2ToCANopen::operator=(CGatewayEsam2ToCANopen& other)
{
    if(this != &other)
    {
        *((CGatewayDCStoPS*)this) = *((CGatewayDCStoPS*)&other);
    }

    return *this;
}

BOOL CGatewayEsam2ToCANopen::ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
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
                //ObjectDictionary
                case ESAM2_READ_OBJECT: return Process_ReadObject(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_WRITE_OBJECT: return Process_WriteObject(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_INITIATE_SEGMENT_READ: return Process_InitiateSegmentedRead(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_INITIATE_SEGMENT_WRITE: return Process_InitiateSegmentedWrite(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_SEGMENT_READ: return Process_SegmentRead(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_SEGMENT_WRITE: return Process_SegmentWrite(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_ABORT_SEGMENT_TRANSFER: return Process_AbortSegmentedTransfer(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);

                //Network Management
                case ESAM2_SEND_NMT_SERVICE: return Process_SendNMTService(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);

                //General Gateway
                case ESAM2_SEND_CAN_FRAME: return Process_SendCANFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_REQUEST_CAN_FRAME: return Process_RequestCANFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_READ_CAN_FRAME: return Process_ReadCANFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);

                //Layer Setting Services
                case ESAM2_SEND_LSS_FRAME: return Process_SendLSSFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
                case ESAM2_READ_LSS_FRAME: return Process_ReadLSSFrame(pCommand_DCS, pProtocolStackManager, p_hHandle, p_hTransactionHandle);
            }
        }
    }

    return FALSE;
}

BOOL CGatewayEsam2ToCANopen::Process_ReadObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_NODE_ID = 1;
    const int PARAMETER_INDEX_INDEX = 2;
    const int PARAMETER_INDEX_SUB_INDEX = 3;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    BYTE ubNodeId = 0;
    WORD usIndex = 0;
    BYTE ubSubIndex = 0;
    //Return Parameter
    BYTE pubMaxDataBuffer[MAX_OBJECT_LENGTH];
    memset(pubMaxDataBuffer, 0, MAX_OBJECT_LENGTH);

    //Adress Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobIdClientServer;
    DWORD ulCobIdServerClient;
    //ReturnParameter
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE ubNonValidNbOfBytes;
    DWORD ulAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;
    BOOL oDoUnlock(TRUE);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &usIndex, sizeof(usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &ubSubIndex, sizeof(ubSubIndex));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        //Prepare DataBuffer
        memset(pubMaxDataBuffer, 0, sizeof(pubMaxDataBuffer));

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, &ulCobIdServerClient);

        //Execute Command NetworkIndication
        if(oResult && usNetworkId != 0)
        {
            oResult = PS_NetworkIndication(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, usNetworkId, ubNodeId, &ulAbortCode, &comErrorInfo);
        }

        //Execute Command SDO Upload
        if(oResult)
        {
            oResult = PS_InitiateSDOUpload(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, usIndex, ubSubIndex, &oExpeditedTransfer, &oSizeIndicated, &ubNonValidNbOfBytes, pubMaxDataBuffer, MAX_OBJECT_LENGTH, &ulAbortCode, &comErrorInfo);
        }

		//Non Expedited Transfer required -> Initialisation already done
        if(oResult && (ulAbortCode == 0) && !oExpeditedTransfer)
        {
            if(oSizeIndicated)
			{
				ulAbortCode = ERROR_DEVICE_ESAM2_SEGMENTED_TRANSFER_REQUIRED_INIT_DONE;
				oDoUnlock = FALSE;
			}
			else
			{
				ulAbortCode = k_Error_ExecutingCommand;
			}
        }

        //Evaluate PS ReturnParameter
        oResult = EvaluateErrorCode(oResult, ulAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulAbortCode, sizeof(ulAbortCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pubMaxDataBuffer, MAX_OBJECT_LENGTH);

        //Unlock CriticalSection
        if(oDoUnlock) Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_WriteObject(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_NODE_ID = 1;
    const int PARAMETER_INDEX_INDEX = 2;
    const int PARAMETER_INDEX_SUB_INDEX = 3;
    const int PARAMETER_INDEX_DATA = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_OBJECT_LENGTH = 4;

    //*Variables DCS*
    //Parameter
    WORD usTargetNetworkId = 0;
    BYTE ubTargetNodeId = 0;
    WORD usIndex = 0;
    BYTE ubSubIndex = 0;
    BYTE pubMaxDataBuffer[MAX_OBJECT_LENGTH];
    memset(pubMaxDataBuffer, 0, MAX_OBJECT_LENGTH);

    //Adress Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobIdClientServer;
    DWORD ulCobIdServerClient;
    BOOL oExpeditedTransfer;
    BOOL oSizeIndicated;
    BYTE ubNonValidNbOfBytes;

    //ReturnParameter
    DWORD ulAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usTargetNetworkId, sizeof(usTargetNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubTargetNodeId, sizeof(ubTargetNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &usIndex, sizeof(usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &ubSubIndex, sizeof(ubSubIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pubMaxDataBuffer, MAX_OBJECT_LENGTH);

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, &ulCobIdServerClient);
        oExpeditedTransfer = TRUE;
        oSizeIndicated = FALSE;
        ubNonValidNbOfBytes = 0;

        //Execute Command Network Indication
        if(oResult && usTargetNetworkId != 0)
        {
            oResult = PS_NetworkIndication(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, usTargetNetworkId, ubTargetNodeId, &ulAbortCode, &comErrorInfo);
        }

        //Execute Command SDO Download
        if(oResult)
        {
            oResult = PS_InitiateSDODownload(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, oExpeditedTransfer, oSizeIndicated, ubNonValidNbOfBytes, usIndex, ubSubIndex, pubMaxDataBuffer, MAX_OBJECT_LENGTH, &ulAbortCode, &comErrorInfo);
        }

        //Evaluate Return Parameter
        oResult = EvaluateErrorCode(oResult, ulAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulAbortCode, sizeof(ulAbortCode));

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_InitiateSegmentedRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_NODE_ID = 1;
    const int PARAMETER_INDEX_INDEX = 2;
    const int PARAMETER_INDEX_SUB_INDEX = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_OBJECT_LENGTH = 1;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    BYTE ubNodeId = 0;
    //Return Parameter
    DWORD ulObjectLength = 0;

    //Adress Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobIdClientServer = 0;
    DWORD ulCobIdServerClient = 0;
    //ReturnParameter
    BOOL oExpeditedTransfer = 0;
    BOOL oSizeIndicated = 0;
    BYTE ubNonValidNbOfBytes = 0;
    DWORD ulAbortCode = 0;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &m_usIndex, sizeof(m_usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &m_ubSubIndex, sizeof(m_ubSubIndex));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, &ulCobIdServerClient);

        //Execute Command Network Indication
        if(oResult && usNetworkId != 0)
        {
            oResult = PS_NetworkIndication(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, usNetworkId, ubNodeId, &ulAbortCode, &comErrorInfo);
        }

        //Execute Command Initiate SDO Upload
        if(oResult)
        {
            oResult = PS_InitiateSDOUpload(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, m_usIndex, m_ubSubIndex, &oExpeditedTransfer, &oSizeIndicated, &ubNonValidNbOfBytes, &ulObjectLength, sizeof(ulObjectLength), &ulAbortCode, &comErrorInfo);
        }

        //Evaluate PS ReturnParameter
        oResult = EvaluateErrorCode(oResult, ulAbortCode, &comErrorInfo, &cmdErrorInfo);
        if(oResult && oExpeditedTransfer)
        {
            //Abort SDO Transfer
            PS_AbortSDOTransfer(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, m_usIndex, m_ubSubIndex, ulAbortCode);

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulAbortCode, sizeof(ulAbortCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_OBJECT_LENGTH, &ulObjectLength, sizeof(ulObjectLength));

        //Unlock CriticalSection
        if(oExpeditedTransfer || !oResult)
        {
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_InitiateSegmentedWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_NODE_ID = 1;
    const int PARAMETER_INDEX_INDEX = 2;
    const int PARAMETER_INDEX_SUB_INDEX = 3;
    const int PARAMETER_INDEX_OBJECT_LENGTH = 4;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    BYTE ubNodeId = 0;
    DWORD ulObjectLength = 0;

    //Adress Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobIdClientServer = 0;
    DWORD ulCobIdServerClient = 0;
    BOOL oExpeditedTransfer = 0;
    BOOL oSizeIndicated = 0;
    BYTE ubNonValidNbOfBytes = 0;

    //ReturnParameter
    DWORD ulAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &m_usIndex, sizeof(m_usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &m_ubSubIndex, sizeof(m_ubSubIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_OBJECT_LENGTH, &ulObjectLength, sizeof(ulObjectLength));

         //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, &ulCobIdServerClient);
        oExpeditedTransfer = FALSE;
        oSizeIndicated = TRUE;
        ubNonValidNbOfBytes = 0;

        //Execute Command Network Indication
        if(oResult && usNetworkId != 0)
        {
            oResult = PS_NetworkIndication(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, usNetworkId, ubNodeId, &ulAbortCode, &comErrorInfo);
        }

        //Execute Command
        if(oResult)
        {
            oResult = PS_InitiateSDODownload(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, oExpeditedTransfer, oSizeIndicated, ubNonValidNbOfBytes, m_usIndex, m_ubSubIndex, &ulObjectLength, sizeof(ulObjectLength), &ulAbortCode, &comErrorInfo);
        }

        //Evaluate Return Parameter
        oResult = EvaluateErrorCode(oResult, ulAbortCode, &comErrorInfo, &cmdErrorInfo);

        //SetReturnParameterData
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulAbortCode, sizeof(ulAbortCode));

        //Unlock CriticalSection
        if(oExpeditedTransfer || !oResult)
        {
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_SegmentRead(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_CONTROL_BYTE = 0;
    const int PARAMETER_INDEX_DUMMY_BYTE = 1;

    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_LENGTH = 1;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 2;
    const int RETURN_PARAMETER_INDEX_DATA = 3;

    //*Constants PS*
    const BYTE MAX_SEGMENT_LENGTH = 7;

    //*Variables DCS*
    //Parameter
    UEsam2ControlByte controlByte;
    controlByte.ubValue = 0;
    BYTE ubDummyByte = 0;
    //Return Parameter
    BYTE ubRetLength = 0;
    UEsam2ControlByte retControlByte;
    retControlByte.ubValue = 0;
    BYTE pubMaxDataBuffer[MAX_SEGMENT_LENGTH];

    //Adress Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobIdClientServer;
    DWORD ulCobIdServerClient;
    BOOL oToggle;

    //ReturnParameter
    BOOL oRetToggle;
    BYTE ubNonValidNbOfBytes;
    BOOL oNoMoreSegments;
    DWORD ulAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;

    if(p_pCommand)
    {
        //Check CriticalSection
        if(!IsLocked(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.ubValue, sizeof(controlByte.ubValue));
        p_pCommand->GetParameterData(PARAMETER_INDEX_DUMMY_BYTE, &ubDummyByte, sizeof(ubDummyByte));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, &ulCobIdServerClient);
        oToggle = controlByte.structure.ubToggle;

        //Execute Command
        if(oResult) oResult = PS_UploadSDOSegment(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, oToggle, &oRetToggle, &ubNonValidNbOfBytes, &oNoMoreSegments, &pubMaxDataBuffer, MAX_SEGMENT_LENGTH, &ulAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, ulAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Restore Return Parameter
        ubRetLength = MAX_SEGMENT_LENGTH-ubNonValidNbOfBytes;
        retControlByte.structure.ubLastDataSegment = oNoMoreSegments;
        retControlByte.structure.ubToggle = oRetToggle;

        //Evaluate PS ReturnParameter
        if(oResult && (oToggle != oRetToggle))
        {
            //Abort SDO Transfer
            PS_AbortSDOTransfer(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, m_usIndex, m_ubSubIndex, ulAbortCode);

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulAbortCode, sizeof(ulAbortCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_LENGTH, &ubRetLength, sizeof(ubRetLength));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte, sizeof(retControlByte));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pubMaxDataBuffer, ubRetLength);

        //Unlock CriticalSection
        if(oNoMoreSegments || !oResult)
        {
            Unlock();
        }
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_AbortSegmentedTransfer(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_NODE_ID = 1;
    const int PARAMETER_INDEX_INDEX = 2;
    const int PARAMETER_INDEX_SUB_INDEX = 3;
    const int PARAMETER_INDEX_ABORT_CODE = 4;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    BYTE ubNodeId = 0;
    WORD usIndex = 0;
    BYTE ubSubIndex = 0;
    DWORD ulAbortCode = 0;

    //Adress Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobIdClientServer;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;

    if(p_pCommand)
    {
        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_ID, &ubNodeId, sizeof(ubNodeId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_INDEX, &usIndex, sizeof(usIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_SUB_INDEX, &ubSubIndex, sizeof(ubSubIndex));
        p_pCommand->GetParameterData(PARAMETER_INDEX_ABORT_CODE, &ulAbortCode, sizeof(ulAbortCode));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        //Prepare PS Parameter
        ulAbortCode = ERROR_DEVICE_EPOS_GENERAL;
        CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, NULL);

        //Execute Command
        if(oResult) oResult = PS_AbortSDOTransfer(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, usIndex, ubSubIndex, ulAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, ulAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_SegmentWrite(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_LENGTH = 0;
    const int PARAMETER_INDEX_CONTROL_BYTE = 1;
    const int PARAMETER_INDEX_DATA = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_LENGTH_WRITTEN = 1;
    const int RETURN_PARAMETER_INDEX_CONTROL_BYTE = 2;

    //*Constants PS*
    const BYTE MAX_SEGMENT_LENGTH = 7;

    //*Variables DCS*
    //Parameter
    BYTE ubLength = 0;
    UEsam2ControlByte controlByte;
    controlByte.ubValue = 0;
    BYTE pubMaxDataBuffer[MAX_SEGMENT_LENGTH];
    //Return Parameter
    BYTE ubRetLengthWritten = 0;
    UEsam2ControlByte retControlByte;
    retControlByte.ubValue = 0;

    //Adress Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobIdClientServer;
    DWORD ulCobIdServerClient;
    BOOL oToggle;
    BYTE ubNonValidNbOfBytes;
    BOOL oNoMoreSegments;

    //ReturnParameter
    BOOL oRetToggle;
    DWORD ulAbortCode;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;

    if(p_pCommand)
    {
        //Check CriticalSection
        if(!IsLocked(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //GetParameterData
        p_pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &ubLength, sizeof(ubLength));
        p_pCommand->GetParameterData(PARAMETER_INDEX_CONTROL_BYTE, &controlByte.ubValue, sizeof(controlByte.ubValue));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        //Check Max SegmentLength
        if(ubLength > MAX_SEGMENT_LENGTH)
        {
            ubLength = MAX_SEGMENT_LENGTH;
            controlByte.structure.ubLastDataSegment = FALSE;
        }
        memset(pubMaxDataBuffer, 0, MAX_SEGMENT_LENGTH);
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pubMaxDataBuffer, ubLength);

        //Prepare PS Parameter
        CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, &ulCobIdServerClient);
        oToggle = controlByte.structure.ubToggle;
        ubNonValidNbOfBytes = MAX_SEGMENT_LENGTH - ubLength;
        oNoMoreSegments = controlByte.structure.ubLastDataSegment;

        //Execute Command
        if(oResult) oResult = PS_DownloadSDOSegment(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, oToggle, ubNonValidNbOfBytes, oNoMoreSegments, pubMaxDataBuffer, MAX_SEGMENT_LENGTH, &oRetToggle, &ulAbortCode, &comErrorInfo);
        oResult = EvaluateErrorCode(oResult, ulAbortCode, &comErrorInfo, &cmdErrorInfo);

        //Restore Return Parameter
        ubRetLengthWritten = MAX_SEGMENT_LENGTH - ubNonValidNbOfBytes;
        retControlByte.structure.ubToggle = oRetToggle;

        //Evaluate PS ReturnParameter
        if(oResult && (oToggle != oRetToggle))
        {
            //Abort SDO Transfer
            PS_AbortSDOTransfer(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, m_usIndex, m_ubSubIndex, ulAbortCode);

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, &cmdErrorInfo);
            oResult = FALSE;
        }

        //SetReturnParameterData
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulAbortCode, sizeof(ulAbortCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_LENGTH_WRITTEN, &ubRetLengthWritten, sizeof(ubRetLengthWritten));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_CONTROL_BYTE, &retControlByte.ubValue, sizeof(retControlByte.ubValue));

        //Unlock CriticalSection
        if(oNoMoreSegments || !oResult)
        {
            Unlock();
        }

    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_SendNMTService(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
	//*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_NODE_IDENTIFIER = 1;
    const int PARAMETER_INDEX_CMD_SPECIFIER = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Variables DCS*
    //Parameter
    WORD usRemoteNetworkId = 0;
	BYTE ubRemoteNodeId = 0;
    WORD usNodeIdentifier = 0;
    WORD usCmdSpecifier = 0;
	BYTE ubSubIndex = (BYTE)-1;
	DWORD ulWriteAccessValue = 0;
    //ReturnParameter
    DWORD ulDeviceErrorCode = 0;

    //Address Parameter
	BYTE ubRouterNodeId = 0;
	DWORD ulBackupBaudrate = 0;
	DWORD ulBackupTimeout = 0;
	DWORD ulTimeout = 0;

    //*Variables PS*
    //Parameter
    BYTE ubCommandSpecifier;
    BYTE ubNodeIdentifier;
	DWORD ulCobIdClientServer;
    DWORD ulCobIdServerClient;
	BOOL oExpeditedTransfer = FALSE;
    BOOL oSizeIndicated = FALSE;
    BYTE ubNonValidNbOfBytes = 0;
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usRemoteNetworkId, sizeof(usRemoteNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_NODE_IDENTIFIER, &usNodeIdentifier, sizeof(usNodeIdentifier));
        p_pCommand->GetParameterData(PARAMETER_INDEX_CMD_SPECIFIER, &usCmdSpecifier, sizeof(usCmdSpecifier));

        if(oResult)
        {
            if(usRemoteNetworkId != 0)
            {
                //Prepare Parameter
				if(GetRequestNmtParameter(usNodeIdentifier, usCmdSpecifier, ubSubIndex, ulWriteAccessValue, ulTimeout))
				{
					//Get Address Parameter
					if(GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo) && GetRemoteNodeId(p_pCommand, ubRemoteNodeId, &comErrorInfo))
					{
						//Prepare PS Parameter
						CalculateDefaultSdoCobIds(ubRouterNodeId, &ulCobIdClientServer, &ulCobIdServerClient);
						oExpeditedTransfer = TRUE;
						oSizeIndicated = FALSE;
						ubNonValidNbOfBytes = 0;

						//Backup Timeout
						if(p_pProtocolStackManager && (ulTimeout > 0)) p_pProtocolStackManager->PS_GetProtocolStackSettings(p_hPS_Handle, &ulBackupBaudrate, &ulBackupTimeout);

						//Set Timeout
						if(p_pProtocolStackManager && (ulBackupBaudrate > 0) && (ulTimeout > 0)) p_pProtocolStackManager->PS_SetProtocolStackSettings(p_hPS_Handle, ulBackupBaudrate, ulTimeout, TRUE);

						//Network Indication
						if(oResult && !PS_NetworkIndication(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, usRemoteNetworkId, ubRemoteNodeId, &ulDeviceErrorCode, &comErrorInfo)) oResult = FALSE;

						//Write Object of Router
						if(oResult && !PS_InitiateSDODownload(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobIdClientServer, ulCobIdServerClient, oExpeditedTransfer, oSizeIndicated, ubNonValidNbOfBytes, INDEX_REQUEST_NMT, ubSubIndex, &ulWriteAccessValue, sizeof(ulWriteAccessValue), &ulDeviceErrorCode, &comErrorInfo)) oResult = FALSE;

						//Restore Timeout
						if(p_pProtocolStackManager && (ulBackupBaudrate > 0) && (ulTimeout > 0)) p_pProtocolStackManager->PS_SetProtocolStackSettings(p_hPS_Handle, ulBackupBaudrate, ulBackupTimeout, TRUE);
					}
					else
					{
						if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadAddressParameter, &comErrorInfo);
						oResult = FALSE;
					}
				}
				else
				{
	                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_FunctionNotSupported, &comErrorInfo);
		            oResult = FALSE;
				}
            }
            else
            {
                //Prepare PS Parameter
                ubCommandSpecifier = (BYTE)usCmdSpecifier;
                ubNodeIdentifier = (BYTE)usNodeIdentifier;

                //Execute Command
                if(oResult) oResult = PS_SendNMTService(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ubCommandSpecifier, ubNodeIdentifier, &comErrorInfo);
                ulDeviceErrorCode = comErrorInfo.GetErrorCode();
            }
        }

        //SetReturnParameterData
        p_pCommand->SetStatus(oResult, &comErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::GetRequestNmtParameter(WORD p_usNodeIdentifier, WORD p_usCmdSpecifier, BYTE& p_rubSubIndex, DWORD& p_rulWriteAccessValue, DWORD& p_rulTimeout)
{
	const DWORD RESET_TIMEOUT = 600;

	BOOL oResult(TRUE);

	//SubIndex
	if((p_usNodeIdentifier >= SUBINDEX_REQUEST_NMT_NODE1) && (p_usNodeIdentifier < SUBINDEX_REQUEST_NMT_ALL_SLAVES))
	{
		//Specific Node
		p_rubSubIndex = (BYTE)p_usNodeIdentifier;
	}
	else if(p_usNodeIdentifier == 0)
	{
		//All Nodes
		p_rubSubIndex = SUBINDEX_REQUEST_NMT_ALL_SLAVES;
	}
	else
	{
		oResult = FALSE;
	}

	//WriteAccess Value
	switch((ENmtServiceIdentifier)p_usCmdSpecifier)
	{
		case NSI_START_REMOTE_NODE		: p_rulWriteAccessValue = RNW_REQUEST_NMT_START; break;
		case NSI_STOP_REMOTE_NODE		: p_rulWriteAccessValue = RNW_REQUEST_NMT_STOP; break;
		case NSI_ENTER_PRE_OPERATIONAL	: p_rulWriteAccessValue = RNW_REQUEST_NMT_ENTER_PRE_OPERATIONAL; break;
		case NSI_RESET_NODE				: p_rulWriteAccessValue = RNW_REQUEST_NMT_RESET; break;
		case NSI_RESET_COMMUNICATION	: p_rulWriteAccessValue = RNW_REQUEST_NMT_RESET_COMMUNICATION; break;
		default							: oResult = FALSE;
	}

	//Timeout (Fix Timeout of 500ms in FW in case of Reset Node)
	switch((ENmtServiceIdentifier)p_usCmdSpecifier)
	{
		case NSI_RESET_NODE				:
		case NSI_RESET_COMMUNICATION	: p_rulTimeout = RESET_TIMEOUT; break;
		default							: p_rulTimeout = 0;
	}

	return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_SendCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_COB_ID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;
    const int PARAMETER_INDEX_DATA = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    WORD usCobId = 0;
    WORD usLength = 0;
    BYTE pubMaxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD ulDeviceErrorCode = 0;

    //Address Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobId = 0;
    BYTE ubLength = 0;
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_COB_ID, &usCobId, sizeof(usCobId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &usLength, sizeof(usLength));

        //Limit Length
        if(usLength > MAX_FRAME_LENGTH) usLength = MAX_FRAME_LENGTH;
        memset(pubMaxFrameBuffer, 0, MAX_FRAME_LENGTH);
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pubMaxFrameBuffer, usLength);

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        if(oResult)
        {
            if(usNetworkId != 0)
            {
                //Remote Command not supported
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_FunctionNotSupported, &comErrorInfo);
                oResult = FALSE;
            }
            else
            {
                //Prepare PS Parameter
                ulCobId = usCobId;
                ubLength = (BYTE)usLength;

                //Execute Command
                if(oResult) oResult = PS_SendCANFrame(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobId, ubLength, pubMaxFrameBuffer, ubLength, &comErrorInfo);
                ulDeviceErrorCode = comErrorInfo.GetErrorCode();
            }
        }

        //SetReturnParameterData
        p_pCommand->SetStatus(oResult, &comErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_ReadCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_COB_ID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;
    const int PARAMETER_INDEX_TIMEOUT = 3;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    WORD usCobId = 0;
    WORD usLength = 0;
    DWORD ulTimeout = 0;
    BYTE pubMaxFrameBuffer[MAX_FRAME_LENGTH];
    //Return Parameter
    DWORD ulDeviceErrorCode = 0;

    //Address Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobId = 0;
    BYTE ubLength = 0;
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_COB_ID, &usCobId, sizeof(usCobId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &usLength, sizeof(usLength));
        p_pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &ulTimeout, sizeof(ulTimeout));

        //Limit Length
        if(usLength > MAX_FRAME_LENGTH) usLength = MAX_FRAME_LENGTH;
        memset(pubMaxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        if(oResult)
        {
            if(usNetworkId != 0)
            {
                //Remote Command not supported
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_FunctionNotSupported, &comErrorInfo);
                oResult = FALSE;
            }
            else
            {
                //Prepare PS Parameter
                ulCobId = usCobId;
                ubLength = (BYTE)usLength;

                //Execute Command
                if(oResult) oResult = PS_ReadCANFrame(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobId, ubLength, pubMaxFrameBuffer, ubLength, ulTimeout, &comErrorInfo);
                ulDeviceErrorCode = comErrorInfo.GetErrorCode();
            }
        }

        //SetReturnParameterData
        p_pCommand->SetStatus(oResult, &comErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pubMaxFrameBuffer, usLength);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_RequestCANFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_COB_ID = 1;
    const int PARAMETER_INDEX_LENGTH = 2;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    WORD usCobId = 0;
    WORD usLength = 0;
    //Return Parameter
    DWORD ulDeviceErrorCode = 0;
    BYTE pubMaxFrameBuffer[MAX_FRAME_LENGTH];

    //Address Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    DWORD ulCobId;
    BYTE ubLength;

    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);
    CErrorInfo cmdErrorInfo;

    if(p_pCommand)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_COB_ID, &usCobId, sizeof(usCobId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_LENGTH, &usLength, sizeof(usLength));

        //Limit Length
        if(usLength > MAX_FRAME_LENGTH) usLength = MAX_FRAME_LENGTH;
        memset(pubMaxFrameBuffer, 0, MAX_FRAME_LENGTH);

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        if(oResult)
        {
            if(usNetworkId != 0)
            {
                //Remote Command not supported
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_FunctionNotSupported, &comErrorInfo);
                oResult = FALSE;
            }
            else
            {
                //Prepare PS Parameter
                ulCobId = usCobId;
                ubLength = (BYTE)usLength;

                //Execute Command
                if(oResult) oResult = PS_RequestCANFrame(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, ulCobId, ubLength, pubMaxFrameBuffer, ubLength, &comErrorInfo);
                ulDeviceErrorCode = comErrorInfo.GetErrorCode();
            }
        }

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &cmdErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pubMaxFrameBuffer, usLength);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::Process_SendLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_DATA = 1;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    DWORD ulDataBufferLength = 0;
    void* pDataBuffer = NULL;

    //Address Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    //ReturnParameter
    DWORD ulErrorCode = 0;
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);

    if(p_pCommand && p_pProtocolStackManager)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Prepare DataBuffer
        ulDataBufferLength = p_pCommand->GetParameterLength(PARAMETER_INDEX_DATA);
        if(ulDataBufferLength > MAX_FRAME_LENGTH) ulDataBufferLength = MAX_FRAME_LENGTH;
        if(ulDataBufferLength > 0)
        {
            pDataBuffer = malloc(ulDataBufferLength);
            memset(pDataBuffer, 0, ulDataBufferLength);
        }

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_DATA, pDataBuffer, ulDataBufferLength);

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        if(oResult)
        {
            if(usNetworkId != 0)
            {
                //Remote Command not supported
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_FunctionNotSupported, &comErrorInfo);
                oResult = FALSE;
            }
            else
            {
                //Execute Command
                if(oResult) oResult = PS_SendLSSFrame(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, pDataBuffer, ulDataBufferLength, &comErrorInfo);
                if(!oResult) ulErrorCode = comErrorInfo.GetErrorCode();
            }
        }

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &comErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulErrorCode, sizeof(ulErrorCode));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;

}

BOOL CGatewayEsam2ToCANopen::Process_ReadLSSFrame(CCommand_DCS* p_pCommand, CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle)
{
    //*Constants DCS*
    const int PARAMETER_INDEX_NETWORK_ID = 0;
    const int PARAMETER_INDEX_TIMEOUT = 1;
    const int RETURN_PARAMETER_INDEX_ERROR_CODE = 0;
    const int RETURN_PARAMETER_INDEX_DATA = 1;

    //*Constants PS*
    const BYTE MAX_FRAME_LENGTH = 8;

    //*Variables DCS*
    //Parameter
    WORD usNetworkId = 0;
    WORD usTimeout = 0;
    //ReturnParameter
    DWORD ulErrorCode = 0;
    DWORD ulDataBufferLength = 0;
    void* pDataBuffer = NULL;

    //Address Parameter
    BYTE ubRouterNodeId = 0;

    //*Variables PS*
    //Parameter
    //ReturnParameter
    CErrorInfo comErrorInfo;

    BOOL oResult(FALSE);

    if(p_pCommand && p_pProtocolStackManager)
    {
        //Lock CriticalSection
        if(!Lock(p_pCommand)) return FALSE;

        //Init
        oResult = TRUE;

        //Prepare DataBuffer
        ulDataBufferLength = MAX_FRAME_LENGTH;
        pDataBuffer = malloc(ulDataBufferLength);
        memset(pDataBuffer, 0, ulDataBufferLength);

        //Get DCS Parameter Data
        p_pCommand->GetParameterData(PARAMETER_INDEX_NETWORK_ID, &usNetworkId, sizeof(usNetworkId));
        p_pCommand->GetParameterData(PARAMETER_INDEX_TIMEOUT, &usTimeout, sizeof(usTimeout));

        //Get Address Parameter
        if(oResult) oResult = GetNodeId(p_pCommand, ubRouterNodeId, &comErrorInfo);

        if(oResult)
        {
            if(usNetworkId != 0)
            {
                //Remote Command not supported
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_FunctionNotSupported, &comErrorInfo);
                oResult = FALSE;
            }
            else
            {
                //Execute Command
                if(oResult) oResult = PS_ReadLSSFrame(p_pProtocolStackManager, p_hPS_Handle, p_hTransactionHandle, usTimeout, pDataBuffer, ulDataBufferLength, &comErrorInfo);
                if(!oResult) ulErrorCode = comErrorInfo.GetErrorCode();
            }
        }

        //Set DCS ReturnParameter Data
        p_pCommand->SetStatus(oResult, &comErrorInfo);
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_ERROR_CODE, &ulErrorCode, sizeof(ulErrorCode));
        p_pCommand->SetReturnParameterData(RETURN_PARAMETER_INDEX_DATA, pDataBuffer, ulDataBufferLength);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

        //Unlock CriticalSection
        Unlock();
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::GetNodeId(CCommand_DCS* p_pCommand, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo)
{
    const CStdString NODE_ID(_T("NodeId"));

    BOOL oResult(FALSE);

    //Get NodeId from Command
    if(!oResult && p_pCommand) oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, NODE_ID, &p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

    //Get NodeId from Device
    if(!oResult) oResult = GetParameter(NODE_ID, &p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::GetRemoteNetworkId(CCommand_DCS* p_pCommand, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo)
{
    const CStdString NODE_ID(_T("RemoteNetworkId"));

    BOOL oResult(FALSE);

    //Get RemoteNetworkId from Command
    if(!oResult && p_pCommand) oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, NODE_ID, &p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

    //Get RemoteNetworkId from Device
    if(!oResult) oResult = GetParameter(NODE_ID, &p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::GetRemoteNodeId(CCommand_DCS* p_pCommand, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo)
{
    const CStdString NODE_ID(_T("RemoteNodeId"));

    BOOL oResult(FALSE);

    //Get RemoteNodeId from Command
    if(!oResult && p_pCommand) oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, NODE_ID, &p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

    //Get RemoteNodeId from Device
    if(!oResult) oResult = GetParameter(NODE_ID, &p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::EvaluateErrorCode(BOOL p_oResult, DWORD p_ulDeviceErrorCode, CErrorInfo* p_pComErrorInfo, CErrorInfo* p_pCmdErrorInfo)
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

void CGatewayEsam2ToCANopen::InitCommands()
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

    //Init NetworkIndication
    m_pCommand_NetworkIndication = new CCommand_PS_CANopen();
    m_pCommand_NetworkIndication->InitCommand(CANOPEN_NETWORK_INDICATION);

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

void CGatewayEsam2ToCANopen::DeleteCommands()
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

    if(m_pCommand_NetworkIndication)
    {
        delete m_pCommand_NetworkIndication;
        m_pCommand_NetworkIndication = NULL;
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

BOOL CGatewayEsam2ToCANopen::PS_InitiateSDODownload(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, BOOL p_oExpeditedTransfer, BOOL p_oSizeIndicated, BYTE p_ubNonValidNbOfBytes, WORD p_usIndex, BYTE p_ubSubIndex, void* p_pDataBuffer, DWORD p_ulDataBufferLength, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 4;

    DWORD ulRetCobId;
    WORD usRetIndex;
    BYTE ubRetSubIndex;

    BOOL oResult(FALSE);

    if(m_pCommand_InitiateSDODownload && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulDataBufferLength > MAX_CAN_DATA) p_ulDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_InitiateSDODownload->ResetStatus();
        m_pCommand_InitiateSDODownload->SetParameterData(0, &p_ulCobIdClientServer, sizeof(p_ulCobIdClientServer));
        m_pCommand_InitiateSDODownload->SetParameterData(1, &p_ulCobIdServerClient, sizeof(p_ulCobIdServerClient));
        m_pCommand_InitiateSDODownload->SetParameterData(2, &p_oExpeditedTransfer, sizeof(p_oExpeditedTransfer));
        m_pCommand_InitiateSDODownload->SetParameterData(3, &p_oSizeIndicated, sizeof(p_oSizeIndicated));
        m_pCommand_InitiateSDODownload->SetParameterData(4, &p_ubNonValidNbOfBytes, sizeof(p_ubNonValidNbOfBytes));
        m_pCommand_InitiateSDODownload->SetParameterData(5, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_InitiateSDODownload->SetParameterData(6, &p_ubSubIndex, sizeof(p_ubSubIndex));
        m_pCommand_InitiateSDODownload->SetParameterData(7, p_pDataBuffer, p_ulDataBufferLength);

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_InitiateSDODownload, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSDODownload->GetReturnParameterData(0, &ulRetCobId, sizeof(ulRetCobId));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(1, &usRetIndex, sizeof(usRetIndex));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(2, &ubRetSubIndex, sizeof(ubRetSubIndex));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(3, p_pulAbortCode, sizeof(*p_pulAbortCode));

        //Get ErrorCode
        m_pCommand_InitiateSDODownload->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_DownloadSDOSegment(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, BOOL p_oToggle, BYTE p_ubNonValidNbOfBytes, BOOL p_oNoMoreSegments, void* p_pSegDataBuffer, DWORD p_ulSegDataBufferLength, BOOL* p_poRetToggle, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 7;

    DWORD ulRetCobId;

    BOOL oResult(FALSE);

    if(m_pCommand_DownloadSDOSegment && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulSegDataBufferLength > MAX_CAN_DATA) p_ulSegDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_DownloadSDOSegment->ResetStatus();
        m_pCommand_DownloadSDOSegment->SetParameterData(0, &p_ulCobIdClientServer, sizeof(p_ulCobIdClientServer));
        m_pCommand_DownloadSDOSegment->SetParameterData(1, &p_ulCobIdServerClient, sizeof(p_ulCobIdServerClient));
        m_pCommand_DownloadSDOSegment->SetParameterData(2, &p_oToggle, sizeof(p_oToggle));
        m_pCommand_DownloadSDOSegment->SetParameterData(3, &p_ubNonValidNbOfBytes, sizeof(p_ubNonValidNbOfBytes));
        m_pCommand_DownloadSDOSegment->SetParameterData(4, &p_oNoMoreSegments, sizeof(p_oNoMoreSegments));
        m_pCommand_DownloadSDOSegment->SetParameterData(5, p_pSegDataBuffer, p_ulSegDataBufferLength);

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_DownloadSDOSegment, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(0, &ulRetCobId, sizeof(ulRetCobId));
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(1, p_poRetToggle, sizeof(*p_poRetToggle));
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(2, p_pulAbortCode, sizeof(*p_pulAbortCode));

        //Get ErrorCode
        m_pCommand_DownloadSDOSegment->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_NetworkIndication(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, WORD p_usTargetNetworkId, BYTE p_ubTargetNodeId, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo)
{
    DWORD ulRetCobId = 0;
    WORD usRetTargetNetworkId = 0;
    BYTE ubRetTargetNodeId = 0;

    BOOL oResult(FALSE);

    if(m_pCommand_NetworkIndication && p_pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_NetworkIndication->ResetStatus();
        m_pCommand_NetworkIndication->SetParameterData(0, &p_ulCobIdClientServer, sizeof(p_ulCobIdClientServer));
        m_pCommand_NetworkIndication->SetParameterData(1, &p_ulCobIdServerClient, sizeof(p_ulCobIdServerClient));
        m_pCommand_NetworkIndication->SetParameterData(2, &p_usTargetNetworkId, sizeof(p_usTargetNetworkId));
        m_pCommand_NetworkIndication->SetParameterData(3, &p_ubTargetNodeId, sizeof(p_ubTargetNodeId));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_NetworkIndication, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_NetworkIndication->GetReturnParameterData(0, &ulRetCobId, sizeof(ulRetCobId));
        m_pCommand_NetworkIndication->GetReturnParameterData(1, &usRetTargetNetworkId, sizeof(usRetTargetNetworkId));
        m_pCommand_NetworkIndication->GetReturnParameterData(2, &ubRetTargetNodeId, sizeof(ubRetTargetNodeId));
        m_pCommand_NetworkIndication->GetReturnParameterData(3, p_pulAbortCode, sizeof(*p_pulAbortCode));

        //Get ErrorCode
        m_pCommand_NetworkIndication->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_AbortSDOTransfer(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulAbortCode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_AbortSDOTransfer && p_pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_AbortSDOTransfer->ResetStatus();
        m_pCommand_AbortSDOTransfer->SetParameterData(0, &p_ulCobIdClientServer, sizeof(p_ulCobIdClientServer));
        m_pCommand_AbortSDOTransfer->SetParameterData(1, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_AbortSDOTransfer->SetParameterData(2, &p_ubSubIndex, sizeof(p_ubSubIndex));
        m_pCommand_AbortSDOTransfer->SetParameterData(3, &p_ulAbortCode, sizeof(p_ulAbortCode));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_AbortSDOTransfer, p_hPS_Handle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSDOTransfer->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_InitiateSDOUpload(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, WORD p_usIndex, BYTE p_ubSubIndex, BOOL* p_poExpeditedTransfer, BOOL* p_poSizeIndicated, BYTE* p_pubNonValidNbOfBytes, void* p_pDataBuffer, DWORD p_ulDataBufferLength, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 4;

    DWORD ulRetCobId;
    WORD usRetIndex;
    BYTE ubRetSubIndex;

    BOOL oResult(FALSE);

    if(m_pCommand_InitiateSDOUpload && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulDataBufferLength > MAX_CAN_DATA) p_ulDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_InitiateSDOUpload->ResetStatus();
        m_pCommand_InitiateSDOUpload->SetParameterData(0, &p_ulCobIdClientServer, sizeof(p_ulCobIdClientServer));
        m_pCommand_InitiateSDOUpload->SetParameterData(1, &p_ulCobIdServerClient, sizeof(p_ulCobIdServerClient));
        m_pCommand_InitiateSDOUpload->SetParameterData(2, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_InitiateSDOUpload->SetParameterData(3, &p_ubSubIndex, sizeof(p_ubSubIndex));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_InitiateSDOUpload, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(0, &ulRetCobId, sizeof(ulRetCobId));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(1, p_poExpeditedTransfer, sizeof(*p_poExpeditedTransfer));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(2, p_poSizeIndicated, sizeof(*p_poSizeIndicated));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(3, p_pubNonValidNbOfBytes, sizeof(*p_pubNonValidNbOfBytes));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(4, &usRetIndex, sizeof(usRetIndex));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(5, &ubRetSubIndex, sizeof(ubRetSubIndex));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(6, p_pDataBuffer, p_ulDataBufferLength);
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(7, p_pulAbortCode, sizeof(*p_pulAbortCode));

        //Get ErrorCode
        m_pCommand_InitiateSDOUpload->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_UploadSDOSegment(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobIdClientServer, DWORD p_ulCobIdServerClient, BOOL p_oToggle, BOOL* p_poRetToggle, BYTE* p_pubNonValidNbOfBytes, BOOL* p_poNoMoreSegments, void* p_pSegDataBuffer, DWORD p_ulSegDataBufferLength, DWORD* p_pulAbortCode, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 7;

    DWORD ulRetCobId;

    BOOL oResult(FALSE);

    if(m_pCommand_UploadSDOSegment && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulSegDataBufferLength > MAX_CAN_DATA) p_ulSegDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_UploadSDOSegment->ResetStatus();
        m_pCommand_UploadSDOSegment->SetParameterData(0, &p_ulCobIdClientServer, sizeof(p_ulCobIdClientServer));
        m_pCommand_UploadSDOSegment->SetParameterData(1, &p_ulCobIdServerClient, sizeof(p_ulCobIdServerClient));
        m_pCommand_UploadSDOSegment->SetParameterData(2, &p_oToggle, sizeof(p_oToggle));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_UploadSDOSegment, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_UploadSDOSegment->GetReturnParameterData(0, &ulRetCobId, sizeof(ulRetCobId));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(1, p_poRetToggle, sizeof(*p_poRetToggle));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(2, p_pubNonValidNbOfBytes, sizeof(*p_pubNonValidNbOfBytes));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(3, p_poNoMoreSegments, sizeof(*p_poNoMoreSegments));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(4, p_pSegDataBuffer, p_ulSegDataBufferLength);
        m_pCommand_UploadSDOSegment->GetReturnParameterData(5, p_pulAbortCode, sizeof(*p_pulAbortCode));

        //Get ErrorCode
        m_pCommand_UploadSDOSegment->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_SendNMTService(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, BYTE p_ubCommandSpecifier, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_SendNMTService && p_pProtocolStackManager)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &p_ubCommandSpecifier, sizeof(p_ubCommandSpecifier));
        m_pCommand_SendNMTService->SetParameterData(1, &p_ubNodeId, sizeof(p_ubNodeId));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_SendNMTService, p_hPS_Handle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_SendCANFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobId, BYTE p_ubLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult(FALSE);

    if(m_pCommand_SendCANFrame && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulDataBufferLength > MAX_CAN_DATA) p_ulDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &p_ulCobId, sizeof(p_ulCobId));
        m_pCommand_SendCANFrame->SetParameterData(1, &p_ubLength, sizeof(p_ubLength));
        m_pCommand_SendCANFrame->SetParameterData(2, p_pDataBuffer, p_ulDataBufferLength);

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_SendCANFrame, p_hPS_Handle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_ReadCANFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobId, BYTE p_ubLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult(FALSE);

    if(m_pCommand_ReadCANFrame && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulDataBufferLength > MAX_CAN_DATA) p_ulDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_ReadCANFrame->ResetStatus();
        m_pCommand_ReadCANFrame->SetParameterData(0, &p_ulCobId, sizeof(p_ulCobId));
        m_pCommand_ReadCANFrame->SetParameterData(1, &p_ubLength, sizeof(p_ubLength));
        m_pCommand_ReadCANFrame->SetParameterData(2, &p_ulTimeout, sizeof(p_ulTimeout));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_ReadCANFrame, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, p_pDataBuffer, p_ulDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_RequestCANFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, DWORD p_ulCobId, BYTE p_ubLength, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult(FALSE);

    if(m_pCommand_RequestCANFrame && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulDataBufferLength > MAX_CAN_DATA) p_ulDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &p_ulCobId, sizeof(p_ulCobId));
        m_pCommand_RequestCANFrame->SetParameterData(1, &p_ubLength, sizeof(p_ubLength));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_RequestCANFrame, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, p_pDataBuffer, p_ulDataBufferLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_SendLSSFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult(FALSE);

    if(m_pCommand_SendLSSFrame && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulDataBufferLength > MAX_CAN_DATA) p_ulDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, p_pDataBuffer, p_ulDataBufferLength);

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_SendLSSFrame, p_hPS_Handle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::PS_ReadLSSFrame(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hPS_Handle, HANDLE p_hTransactionHandle, WORD p_usTimeout, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult(FALSE);

    if(m_pCommand_ReadLSSFrame && p_pProtocolStackManager)
    {
        //Limit DataLength
        if(p_ulDataBufferLength > MAX_CAN_DATA) p_ulDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &p_usTimeout, sizeof(p_usTimeout));

        //Execute Command
        oResult = p_pProtocolStackManager->ExecuteCommand(m_pCommand_ReadLSSFrame, p_hPS_Handle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, p_pDataBuffer, p_ulDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayEsam2ToCANopen::CalculateDefaultSdoCobIds(BYTE p_ubNodeId, DWORD* p_pulCobIdClientServer, DWORD* p_pulCobIdServerClient)
{
    const DWORD TX_SDO_OFFSET(0x600);
    const DWORD RX_SDO_OFFSET(0x580);

    if(p_pulCobIdClientServer)
    {
        *p_pulCobIdClientServer = p_ubNodeId + TX_SDO_OFFSET;
    }

    if(p_pulCobIdServerClient)
    {
        *p_pulCobIdServerClient = p_ubNodeId + RX_SDO_OFFSET;
    }

    return TRUE;
}

BOOL CGatewayEsam2ToCANopen::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName(_T("GatewayEsam2ToCANopen"));

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);

        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayEsam2ToCANopen::InitLayerParameterStack(CCommandRoot* p_pCommand)
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
            if(m_pCommand_InitiateSDODownload) m_pCommand_InitiateSDODownload->SetLayerParameterStack(layerParameterStack);
            if(m_pCommand_DownloadSDOSegment) m_pCommand_DownloadSDOSegment->SetLayerParameterStack(layerParameterStack);
            if(m_pCommand_InitiateSDOUpload) m_pCommand_InitiateSDOUpload->SetLayerParameterStack(layerParameterStack);
            if(m_pCommand_UploadSDOSegment) m_pCommand_UploadSDOSegment->SetLayerParameterStack(layerParameterStack);
            if(m_pCommand_NetworkIndication) m_pCommand_NetworkIndication->SetLayerParameterStack(layerParameterStack);
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

