// CommandSetObjectDictionary_DCS_Esam.cpp: Implementierung der Klasse CCommandSetObjectDictionary_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetObjectDictionary_DCS_Esam.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetObjectDictionary_DCS_Esam::CCommandSetObjectDictionary_DCS_Esam()
{
    m_strCommandSetName = COMMAND_SET_OBJECT_DICTIONARY;

    m_pCommand_WriteObject = NULL;
    m_pCommand_InitiateSegmentedWrite = NULL;
    m_pCommand_SegmentedWrite = NULL;
    m_pCommand_ReadObject = NULL;
    m_pCommand_InitiateSegmentedRead = NULL;
    m_pCommand_SegmentedRead = NULL;
    m_pCommand_AbortSegmentedTransfer = NULL;

    InitCommands();
}

CCommandSetObjectDictionary_DCS_Esam::~CCommandSetObjectDictionary_DCS_Esam()
{
    DeleteCommands();
}

BOOL CCommandSetObjectDictionary_DCS_Esam::DCS_InitiateSegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD* pdObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedRead)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedRead->ResetStatus();
        m_pCommand_InitiateSegmentedRead->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_InitiateSegmentedRead->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(3, &uNodeId, sizeof(uNodeId));

        //Execute command
        oResult = m_pCommand_InitiateSegmentedRead->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(1, pdObjectLength, sizeof(*pdObjectLength));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedRead->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Esam::DCS_InitiateSegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedWrite)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedWrite->ResetStatus();
        m_pCommand_InitiateSegmentedWrite->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(3, &uNodeId, sizeof(uNodeId));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(4, &dObjectLength, sizeof(dObjectLength));

        //Execute command
        oResult = m_pCommand_InitiateSegmentedWrite->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedWrite->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Esam::DCS_AbortSegmentedTransfer(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
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

BOOL CCommandSetObjectDictionary_DCS_Esam::DCS_ReadObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectBufferLength, DWORD* pdObjectLengthRead, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    DWORD dObjectLength;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadObject)
    {
        //Set Parameter Data
        m_pCommand_ReadObject->ResetStatus();
        m_pCommand_ReadObject->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_ReadObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_ReadObject->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_ReadObject->SetParameterData(3, &uNodeId, sizeof(uNodeId));

        //Execute Command
        oResult = m_pCommand_ReadObject->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Length
        dObjectLength = m_pCommand_ReadObject->GetReturnParameterLength(1);
        if(dObjectBufferLength < dObjectLength) dObjectLength = dObjectBufferLength;
        if(pdObjectLengthRead) *pdObjectLengthRead = dObjectLength;

        //Get ReturnParameter Data
        m_pCommand_ReadObject->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_ReadObject->GetReturnParameterData(1, pObjectBuffer, dObjectLength);

        //Get ErrorCode
        m_pCommand_ReadObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Esam::DCS_SegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BOOL* poLastDataSegment, BYTE* pSegmentBuffer, DWORD dSegmentBufferLength, DWORD* pdSegmentLengthRead, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    UEsamControlByte controlByte;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedRead)
    {
        //Set Parameter Data
        m_pCommand_SegmentedRead->ResetStatus();
        m_pCommand_SegmentedRead->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));

        //Execute Command
        oResult = m_pCommand_SegmentedRead->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedRead->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_SegmentedRead->GetReturnParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedRead->GetReturnParameterData(2, pSegmentBuffer, dSegmentBufferLength);

        //Get ErrorCode
        m_pCommand_SegmentedRead->GetErrorInfo(pErrorInfo);

        //Restore ControlByte
        if(pdSegmentLengthRead) *pdSegmentLengthRead = controlByte.structure.ubNbOfDataBytes;
        if(poLastDataSegment) *poLastDataSegment = controlByte.structure.ubLastDataSegment;
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Esam::DCS_SegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE* pSegmentBuffer, DWORD dSegmentLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    UEsamControlByte controlByte;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedWrite)
    {
        //Prepare ControlByte
        controlByte.ubValue = 0;
        controlByte.structure.ubNbOfDataBytes = dSegmentLength;

        //Set Parameter Data
        m_pCommand_SegmentedWrite->ResetStatus();
        m_pCommand_SegmentedWrite->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_SegmentedWrite->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedWrite->SetParameterData(2, pSegmentBuffer, dSegmentLength);

        //Execute Command
        oResult = m_pCommand_SegmentedWrite->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedWrite->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Esam::DCS_WriteObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteObject)
    {
        //Set Parameter Data
        m_pCommand_WriteObject->ResetStatus();
        m_pCommand_WriteObject->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_WriteObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_WriteObject->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_WriteObject->SetParameterData(3, &uNodeId, sizeof(uNodeId));
        m_pCommand_WriteObject->SetParameterData(4, pObjectBuffer, dObjectLength);

        //Execute Command
        oResult = m_pCommand_WriteObject->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteObject->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_WriteObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

void CCommandSetObjectDictionary_DCS_Esam::InitCommands()
{
    DeleteCommands();

    //WriteObject
    m_pCommand_WriteObject = new CCommand_DCS_Esam();
    m_pCommand_WriteObject->InitCommand(ESAM_WRITE_OBJECT);

    //ReadObject
    m_pCommand_ReadObject = new CCommand_DCS_Esam();
    m_pCommand_ReadObject->InitCommand(ESAM_READ_OBJECT);

    //InitiateSegmentedWrite
    m_pCommand_InitiateSegmentedWrite = new CCommand_DCS_Esam();
    m_pCommand_InitiateSegmentedWrite->InitCommand(ESAM_INITIATE_SEGMENT_WRITE);

    //InitiateSegmentedRead
    m_pCommand_InitiateSegmentedRead = new CCommand_DCS_Esam();
    m_pCommand_InitiateSegmentedRead->InitCommand(ESAM_INITIATE_SEGMENT_READ);

    //SegmentedWrite
    m_pCommand_SegmentedWrite = new CCommand_DCS_Esam();
    m_pCommand_SegmentedWrite->InitCommand(ESAM_SEGMENT_WRITE);

    //SegmentedRead
    m_pCommand_SegmentedRead = new CCommand_DCS_Esam();
    m_pCommand_SegmentedRead->InitCommand(ESAM_SEGMENT_READ);

    //AbortSegmentedTransfer
    m_pCommand_AbortSegmentedTransfer = new CCommand_DCS_Esam();
    m_pCommand_AbortSegmentedTransfer->InitCommand(ESAM_ABORT_SEGMENT_TRANSFER);
}

void CCommandSetObjectDictionary_DCS_Esam::DeleteCommands()
{
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

void CCommandSetObjectDictionary_DCS_Esam::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_WriteObject) m_pCommand_WriteObject->InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->InitJournalManager(pJournalManager);
    if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadObject) m_pCommand_ReadObject->InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSegmentedRead) m_pCommand_InitiateSegmentedRead->InitJournalManager(pJournalManager);
    if(m_pCommand_SegmentedRead) m_pCommand_SegmentedRead->InitJournalManager(pJournalManager);
    if(m_pCommand_AbortSegmentedTransfer) m_pCommand_AbortSegmentedTransfer->InitJournalManager(pJournalManager);
}

void CCommandSetObjectDictionary_DCS_Esam::ResetJournalManager()
{
    if(m_pCommand_WriteObject) m_pCommand_WriteObject->ResetJournalManager();
    if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->ResetJournalManager();
    if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->ResetJournalManager();
    if(m_pCommand_ReadObject) m_pCommand_ReadObject->ResetJournalManager();
    if(m_pCommand_InitiateSegmentedRead) m_pCommand_InitiateSegmentedRead->ResetJournalManager();
    if(m_pCommand_SegmentedRead) m_pCommand_SegmentedRead->ResetJournalManager();
    if(m_pCommand_AbortSegmentedTransfer) m_pCommand_AbortSegmentedTransfer->ResetJournalManager();
}

BOOL CCommandSetObjectDictionary_DCS_Esam::InitGateway(CGateway *pGateway)
{
    if(m_pCommand_WriteObject && !m_pCommand_WriteObject->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_InitiateSegmentedWrite && !m_pCommand_InitiateSegmentedWrite->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_SegmentedWrite && !m_pCommand_SegmentedWrite->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_ReadObject && !m_pCommand_ReadObject->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_InitiateSegmentedRead && !m_pCommand_InitiateSegmentedRead->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_SegmentedRead && !m_pCommand_SegmentedRead->InitGateway(pGateway)) return FALSE;
    if(m_pCommand_AbortSegmentedTransfer && !m_pCommand_AbortSegmentedTransfer->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

CXXMLFile::CElementPart* CCommandSetObjectDictionary_DCS_Esam::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
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

