// CommandSetObjectDictionary_DCS_Epos.cpp: Implementierung der Klasse CCommandSetObjectDictionary_DCS_Epos.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetObjectDictionary_DCS_Epos.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEposDef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetObjectDictionary_DCS_Epos::CCommandSetObjectDictionary_DCS_Epos()
{
    m_strCommandSetName = COMMAND_SET_OBJECT_DICTIONARY;

    m_pCommand_WriteODObject = NULL;
    m_pCommand_WriteODObjectNextSegment = NULL;
    m_pCommand_WriteObject = NULL;
    m_pCommand_InitiateSegmentedWrite = NULL;
    m_pCommand_SegmentedWrite = NULL;
    m_pCommand_ReadODObject = NULL;
    m_pCommand_ReadODObjectNextSegment = NULL;
    m_pCommand_ReadObject = NULL;
    m_pCommand_InitiateSegmentedRead = NULL;
    m_pCommand_SegmentedRead = NULL;
    m_pCommand_AbortSegmentedTransfer = NULL;

    InitCommands();
}

CCommandSetObjectDictionary_DCS_Epos::~CCommandSetObjectDictionary_DCS_Epos()
{
    DeleteCommands();
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_InitiateSegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedRead)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedRead->ResetStatus();
        m_pCommand_InitiateSegmentedRead->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(2, &uNodeId, sizeof(uNodeId));

        //Execute command
        oResult = m_pCommand_InitiateSegmentedRead->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedRead->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_InitiateSegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedWrite)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedWrite->ResetStatus();
        m_pCommand_InitiateSegmentedWrite->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(2, &uNodeId, sizeof(uNodeId));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(3, &dObjectLength, sizeof(dObjectLength));

        //Execute command
        oResult = m_pCommand_InitiateSegmentedWrite->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedWrite->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_AbortSegmentedTransfer(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
{
    BYTE uSubIndex = (BYTE)wSubIndex;
    BOOL oResult = FALSE;

    if(m_pCommand_AbortSegmentedTransfer)
    {
        //Set Parameter Data
        m_pCommand_AbortSegmentedTransfer->ResetStatus();
        m_pCommand_AbortSegmentedTransfer->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(3, &dAbortCode, sizeof(dAbortCode));

        //Execute command
        oResult = m_pCommand_AbortSegmentedTransfer->Execute(pManager, hHandle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSegmentedTransfer->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_ReadObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadObject)
    {
        //Set Parameter Data
        m_pCommand_ReadObject->ResetStatus();
        m_pCommand_ReadObject->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_ReadObject->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_ReadObject->SetParameterData(2, &uNodeId, sizeof(uNodeId));
        m_pCommand_ReadObject->SetParameterData(3, &dObjectLength, sizeof(dObjectLength));

        //Execute Command
        oResult = m_pCommand_ReadObject->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadObject->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_ReadObject->GetReturnParameterData(1, pObjectBuffer, dObjectLength);

        //Get ErrorCode
        m_pCommand_ReadObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_ReadODObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, WORD wMaxNumberOfBytesToRead, WORD* pwNumberOfBytesRead, WORD* pwSegmentedRead, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadODObject)
    {
        //Set Parameter Data
        m_pCommand_ReadODObject->ResetStatus();
        m_pCommand_ReadODObject->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_ReadODObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_ReadODObject->SetParameterData(2, &wSubIndex, sizeof(wSubIndex));
        m_pCommand_ReadODObject->SetParameterData(3, &wMaxNumberOfBytesToRead, sizeof(wMaxNumberOfBytesToRead));

        //Execute Command
        oResult = m_pCommand_ReadODObject->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadODObject->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_ReadODObject->GetReturnParameterData(1, pwSegmentedRead, sizeof(*pwSegmentedRead));
        m_pCommand_ReadODObject->GetReturnParameterData(2, pwNumberOfBytesRead, sizeof(*pwNumberOfBytesRead));
        m_pCommand_ReadODObject->GetReturnParameterData(3, pDataBuffer, wMaxNumberOfBytesToRead);

        //Get ErrorCode
        m_pCommand_ReadODObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_ReadODObjectNextSegment(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, BYTE* pDataBuffer, WORD wMaxNumberOfBytesToRead, WORD* pwNumberOfBytesRead, WORD* pwSegmentedRead, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadODObjectNextSegment)
    {
        //Set Parameter Data
        m_pCommand_ReadODObjectNextSegment->ResetStatus();
        m_pCommand_ReadODObjectNextSegment->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_ReadODObjectNextSegment->SetParameterData(1, &wMaxNumberOfBytesToRead, sizeof(wMaxNumberOfBytesToRead));

        //Execute Command
        oResult = m_pCommand_ReadODObjectNextSegment->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(1, pwSegmentedRead, sizeof(*pwSegmentedRead));
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(2, pwNumberOfBytesRead, sizeof(*pwNumberOfBytesRead));
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(3, pDataBuffer, wMaxNumberOfBytesToRead);

        //Get ErrorCode
        m_pCommand_ReadODObjectNextSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_SegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, BOOL oToggle, BOOL* poMoreSegments, BYTE* pSegmentBuffer, DWORD dSegmentBufferLength, DWORD* pdSegmentLengthRead, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    UEposControlByte controlByte;
    BYTE uDummyByte = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedRead)
    {
        //Prepare ControlByte
        controlByte.bValue = 0;
        controlByte.structure.bToggle = oToggle;

        //Set Parameter Data
        m_pCommand_SegmentedRead->ResetStatus();
        m_pCommand_SegmentedRead->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_SegmentedRead->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedRead->SetParameterData(2, &uDummyByte, sizeof(uDummyByte));

        //Execute Command
        oResult = m_pCommand_SegmentedRead->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedRead->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_SegmentedRead->GetReturnParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedRead->GetReturnParameterData(2, pSegmentBuffer, dSegmentBufferLength);

        //Get ErrorCode
        m_pCommand_SegmentedRead->GetErrorInfo(pErrorInfo);

        //Restore ControlByte
        if(pdSegmentLengthRead) *pdSegmentLengthRead = controlByte.structure.bLength;
        if(poMoreSegments) *poMoreSegments = controlByte.structure.bMoreSegments;
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_SegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, BOOL oToggle, BOOL oMoreSegments, BYTE* pSegmentBuffer, DWORD dSegmentLength, DWORD* pdSegmentLengthWritten, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    UEposControlByte controlByte;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedWrite)
    {
        //Prepare ControlByte
        controlByte.bValue = 0;
        controlByte.structure.bLength = dSegmentLength;
        controlByte.structure.bToggle = oToggle;
        controlByte.structure.bMoreSegments = oMoreSegments;

        //Set Parameter Data
        m_pCommand_SegmentedWrite->ResetStatus();
        m_pCommand_SegmentedWrite->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_SegmentedWrite->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedWrite->SetParameterData(2, pSegmentBuffer, dSegmentLength);

        //Execute Command
        oResult = m_pCommand_SegmentedWrite->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedWrite->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_SegmentedWrite->GetReturnParameterData(1, &controlByte, sizeof(controlByte));

        //Restore ControlByte
        if(pdSegmentLengthWritten) *pdSegmentLengthWritten = controlByte.structure.bLength;

        //Get ErrorCode
        m_pCommand_SegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_WriteObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteObject)
    {
        //Set Parameter Data
        m_pCommand_WriteObject->ResetStatus();
        m_pCommand_WriteObject->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_WriteObject->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_WriteObject->SetParameterData(2, &uNodeId, sizeof(uNodeId));
        m_pCommand_WriteObject->SetParameterData(3, pObjectBuffer, dObjectLength);
        m_pCommand_WriteObject->SetParameterData(4, &dObjectLength, sizeof(dObjectLength));

        //Execute Command
        oResult = m_pCommand_WriteObject->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteObject->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_WriteObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_WriteODObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, WORD wNumberOfBytesToWrite, BOOL oSignedDataType, WORD* pwNumberOfBytesWritten, WORD* pwSegmentedWrite, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteODObject)
    {
        //Set Parameter Data
        m_pCommand_WriteODObject->ResetStatus();
        m_pCommand_WriteODObject->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_WriteODObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_WriteODObject->SetParameterData(2, &wSubIndex, sizeof(wSubIndex));
        m_pCommand_WriteODObject->SetParameterData(3, &wNumberOfBytesToWrite, sizeof(wNumberOfBytesToWrite));
        m_pCommand_WriteODObject->SetParameterData(4, &wNumberOfBytesToWrite, sizeof(wNumberOfBytesToWrite));
        m_pCommand_WriteODObject->SetParameterData(5, pDataBuffer, wNumberOfBytesToWrite);
        m_pCommand_WriteODObject->SetParameterData(6, &oSignedDataType, sizeof(oSignedDataType));

        //Execute Command
        oResult = m_pCommand_WriteODObject->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteODObject->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_WriteODObject->GetReturnParameterData(1, pwSegmentedWrite, sizeof(pwSegmentedWrite));
        m_pCommand_WriteODObject->GetReturnParameterData(2, pwNumberOfBytesWritten, sizeof(*pwNumberOfBytesWritten));

        //Get ErrorCode
        m_pCommand_WriteODObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Epos::DCS_WriteODObjectNextSegment(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, BYTE* pDataBuffer, WORD wNumberOfBytesToWrite, BOOL oSignedDataType, WORD* pwNumberOfBytesWritten, WORD* pwSegmentedWrite, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteODObjectNextSegment)
    {
        //Set Parameter Data
        m_pCommand_WriteODObjectNextSegment->ResetStatus();
        m_pCommand_WriteODObjectNextSegment->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_WriteODObjectNextSegment->SetParameterData(1, &wNumberOfBytesToWrite, sizeof(wNumberOfBytesToWrite));
        m_pCommand_WriteODObjectNextSegment->SetParameterData(2, pDataBuffer, wNumberOfBytesToWrite);
        m_pCommand_WriteODObjectNextSegment->SetParameterData(3, &oSignedDataType, sizeof(oSignedDataType));

        //Execute Command
        oResult = m_pCommand_WriteODObjectNextSegment->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteODObjectNextSegment->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_WriteODObjectNextSegment->GetReturnParameterData(1, pwSegmentedWrite, sizeof(pwSegmentedWrite));
        m_pCommand_WriteODObjectNextSegment->GetReturnParameterData(2, pwNumberOfBytesWritten, sizeof(*pwNumberOfBytesWritten));

        //Get ErrorCode
        m_pCommand_WriteODObjectNextSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

void CCommandSetObjectDictionary_DCS_Epos::InitCommands()
{
    DeleteCommands();

    //WriteODObject
    m_pCommand_WriteODObject = new CCommand_DCS_Epos();
    m_pCommand_WriteODObject->InitCommand(EPOS_WRITE_OD_OBJECT);

    //ReadODObject
    m_pCommand_ReadODObject = new CCommand_DCS_Epos();
    m_pCommand_ReadODObject->InitCommand(EPOS_READ_OD_OBJECT);

    //WriteODObjectNextSegment
    m_pCommand_WriteODObjectNextSegment = new CCommand_DCS_Epos();
    m_pCommand_WriteODObjectNextSegment->InitCommand(EPOS_WRITE_OD_OBJECT_NEXT_SEGMENT);

    //ReadODObjectNextSegment
    m_pCommand_ReadODObjectNextSegment = new CCommand_DCS_Epos();
    m_pCommand_ReadODObjectNextSegment->InitCommand(EPOS_READ_OD_OBJECT_NEXT_SEGMENT);

    //WriteObject
    m_pCommand_WriteObject = new CCommand_DCS_Epos();
    m_pCommand_WriteObject->InitCommand(EPOS_WRITE_OBJECT);

    //ReadObject
    m_pCommand_ReadObject = new CCommand_DCS_Epos();
    m_pCommand_ReadObject->InitCommand(EPOS_READ_OBJECT);

    //InitiateSegmentedWrite
    m_pCommand_InitiateSegmentedWrite = new CCommand_DCS_Epos();
    m_pCommand_InitiateSegmentedWrite->InitCommand(EPOS_INITIATE_SEGMENT_WRITE);

    //InitiateSegmentedRead
    m_pCommand_InitiateSegmentedRead = new CCommand_DCS_Epos();
    m_pCommand_InitiateSegmentedRead->InitCommand(EPOS_INITIATE_SEGMENT_READ);

    //SegmentedWrite
    m_pCommand_SegmentedWrite = new CCommand_DCS_Epos();
    m_pCommand_SegmentedWrite->InitCommand(EPOS_SEGMENT_WRITE);

    //SegmentedRead
    m_pCommand_SegmentedRead = new CCommand_DCS_Epos();
    m_pCommand_SegmentedRead->InitCommand(EPOS_SEGMENT_READ);

    //AbortSegmentedTransfer
    m_pCommand_AbortSegmentedTransfer = new CCommand_DCS_Epos();
    m_pCommand_AbortSegmentedTransfer->InitCommand(EPOS_ABORT_SEGMENT_TRANSFER);
}

void CCommandSetObjectDictionary_DCS_Epos::DeleteCommands()
{
    //WriteODObject
    if(m_pCommand_WriteODObject)
    {
        delete m_pCommand_WriteODObject;
        m_pCommand_WriteODObject = NULL;
    }

    //WriteODObjectNextSegment
    if(m_pCommand_WriteODObjectNextSegment)
    {
        delete m_pCommand_WriteODObjectNextSegment;
        m_pCommand_WriteODObjectNextSegment = NULL;
    }

    //WriteObject
    if(m_pCommand_WriteObject)
    {
        delete m_pCommand_WriteObject;
        m_pCommand_WriteObject = NULL;
    }

    //InitiateSegmentedWrite
    if(m_pCommand_InitiateSegmentedWrite)
    {
        delete m_pCommand_InitiateSegmentedWrite;
        m_pCommand_InitiateSegmentedWrite = NULL;
    }

    //SegmentedWrite
    if(m_pCommand_SegmentedWrite)
    {
        delete m_pCommand_SegmentedWrite;
        m_pCommand_SegmentedWrite = NULL;
    }

    //ReadODObject
    if(m_pCommand_ReadODObject)
    {
        delete m_pCommand_ReadODObject;
        m_pCommand_ReadODObject = NULL;
    }

    //ReadODObjectNextSegment
    if(m_pCommand_ReadODObjectNextSegment)
    {
        delete m_pCommand_ReadODObjectNextSegment;
        m_pCommand_ReadODObjectNextSegment = NULL;
    }

    //ReadObject
    if(m_pCommand_ReadObject)
    {
        delete m_pCommand_ReadObject;
        m_pCommand_ReadObject = NULL;
    }

    //InitateSegmentedRead
    if(m_pCommand_InitiateSegmentedRead)
    {
        delete m_pCommand_InitiateSegmentedRead;
        m_pCommand_InitiateSegmentedRead = NULL;
    }

    //SegmentedRead
    if(m_pCommand_SegmentedRead)
    {
        delete m_pCommand_SegmentedRead;
        m_pCommand_SegmentedRead = NULL;
    }

    //AbortSegmentedTransfer
    if(m_pCommand_AbortSegmentedTransfer)
    {
        delete m_pCommand_AbortSegmentedTransfer;
        m_pCommand_AbortSegmentedTransfer = NULL;
    }
}

void CCommandSetObjectDictionary_DCS_Epos::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_WriteODObject) m_pCommand_WriteODObject->InitJournalManager(pJournalManager);
    if(m_pCommand_WriteODObjectNextSegment) m_pCommand_WriteODObjectNextSegment->InitJournalManager(pJournalManager);
    if(m_pCommand_WriteObject) m_pCommand_WriteObject->InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->InitJournalManager(pJournalManager);
    if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadODObject) m_pCommand_ReadODObject->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadODObjectNextSegment) m_pCommand_ReadODObjectNextSegment->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadObject) m_pCommand_ReadObject->InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSegmentedRead) m_pCommand_InitiateSegmentedRead->InitJournalManager(pJournalManager);
    if(m_pCommand_SegmentedRead) m_pCommand_SegmentedRead->InitJournalManager(pJournalManager);
    if(m_pCommand_AbortSegmentedTransfer) m_pCommand_AbortSegmentedTransfer->InitJournalManager(pJournalManager);
}

void CCommandSetObjectDictionary_DCS_Epos::ResetJournalManager()
{
    if(m_pCommand_WriteODObject) m_pCommand_WriteODObject->ResetJournalManager();
    if(m_pCommand_WriteODObjectNextSegment) m_pCommand_WriteODObjectNextSegment->ResetJournalManager();
    if(m_pCommand_WriteObject) m_pCommand_WriteObject->ResetJournalManager();
    if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->ResetJournalManager();
    if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->ResetJournalManager();
    if(m_pCommand_ReadODObject) m_pCommand_ReadODObject->ResetJournalManager();
    if(m_pCommand_ReadODObjectNextSegment) m_pCommand_ReadODObjectNextSegment->ResetJournalManager();
    if(m_pCommand_ReadObject) m_pCommand_ReadObject->ResetJournalManager();
    if(m_pCommand_InitiateSegmentedRead) m_pCommand_InitiateSegmentedRead->ResetJournalManager();
    if(m_pCommand_SegmentedRead) m_pCommand_SegmentedRead->ResetJournalManager();
    if(m_pCommand_AbortSegmentedTransfer) m_pCommand_AbortSegmentedTransfer->ResetJournalManager();
}

BOOL CCommandSetObjectDictionary_DCS_Epos::InitGateway(CGateway *pGateway)
{
    if(m_pCommand_WriteODObject && !m_pCommand_WriteODObject->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_WriteODObjectNextSegment && !m_pCommand_WriteODObjectNextSegment->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_WriteObject && !m_pCommand_WriteObject->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_InitiateSegmentedWrite && !m_pCommand_InitiateSegmentedWrite->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_SegmentedWrite && !m_pCommand_SegmentedWrite->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_ReadODObject && !m_pCommand_ReadODObject->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_ReadODObjectNextSegment && !m_pCommand_ReadODObjectNextSegment->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_ReadObject && !m_pCommand_ReadObject->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_InitiateSegmentedRead && !m_pCommand_InitiateSegmentedRead->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_SegmentedRead && !m_pCommand_SegmentedRead->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_AbortSegmentedTransfer && !m_pCommand_AbortSegmentedTransfer->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

CXXMLFile::CElementPart* CCommandSetObjectDictionary_DCS_Epos::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
{
    CXXMLFile::CElement* pElement;
    BOOL oCheckVisibility = FALSE;

    if(pFile && pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
        pFile->SetText(pElement, "CommandSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_ReadObject && !m_pCommand_ReadObject->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_WriteObject && !m_pCommand_WriteObject->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_InitiateSegmentedRead && !m_pCommand_InitiateSegmentedRead->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SegmentedRead && !m_pCommand_SegmentedRead->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_InitiateSegmentedWrite && !m_pCommand_InitiateSegmentedWrite->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SegmentedWrite && !m_pCommand_SegmentedWrite->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_AbortSegmentedTransfer && !m_pCommand_AbortSegmentedTransfer->StoreToXMLFile(pFile, pElement, oCheckVisibility)) return FALSE;

        return pElement;
    }

    return NULL;
}

