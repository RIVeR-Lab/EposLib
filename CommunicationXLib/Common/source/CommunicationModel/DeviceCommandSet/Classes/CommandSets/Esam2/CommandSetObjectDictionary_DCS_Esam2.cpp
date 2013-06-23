// CommandSetObjectDictionary_DCS_Esam2.cpp: Implementierung der Klasse CCommandSetObjectDictionary_DCS_Esam2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetObjectDictionary_DCS_Esam2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetObjectDictionary_DCS_Esam2::CCommandSetObjectDictionary_DCS_Esam2()
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

CCommandSetObjectDictionary_DCS_Esam2::~CCommandSetObjectDictionary_DCS_Esam2()
{
    DeleteCommands();
}

void CCommandSetObjectDictionary_DCS_Esam2::InitCommands()
{
    DeleteCommands();

    //WriteObject
    m_pCommand_WriteObject = new CCommand_DCS_Esam2();
    m_pCommand_WriteObject->InitCommand(ESAM2_WRITE_OBJECT);

    //ReadObject
    m_pCommand_ReadObject = new CCommand_DCS_Esam2();
    m_pCommand_ReadObject->InitCommand(ESAM2_READ_OBJECT);

    //InitiateSegmentedWrite
    m_pCommand_InitiateSegmentedWrite = new CCommand_DCS_Esam2();
    m_pCommand_InitiateSegmentedWrite->InitCommand(ESAM2_INITIATE_SEGMENT_WRITE);

    //InitiateSegmentedRead
    m_pCommand_InitiateSegmentedRead = new CCommand_DCS_Esam2();
    m_pCommand_InitiateSegmentedRead->InitCommand(ESAM2_INITIATE_SEGMENT_READ);

    //SegmentedWrite
    m_pCommand_SegmentedWrite = new CCommand_DCS_Esam2();
    m_pCommand_SegmentedWrite->InitCommand(ESAM2_SEGMENT_WRITE);

    //SegmentedRead
    m_pCommand_SegmentedRead = new CCommand_DCS_Esam2();
    m_pCommand_SegmentedRead->InitCommand(ESAM2_SEGMENT_READ);

    //AbortSegmentedTransfer
    m_pCommand_AbortSegmentedTransfer = new CCommand_DCS_Esam2();
    m_pCommand_AbortSegmentedTransfer->InitCommand(ESAM2_ABORT_SEGMENT_TRANSFER);
}

void CCommandSetObjectDictionary_DCS_Esam2::DeleteCommands()
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

void CCommandSetObjectDictionary_DCS_Esam2::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommand_WriteObject) m_pCommand_WriteObject->InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->InitJournalManager(pJournalManager);
    if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadObject) m_pCommand_ReadObject->InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSegmentedRead) m_pCommand_InitiateSegmentedRead->InitJournalManager(pJournalManager);
    if(m_pCommand_SegmentedRead) m_pCommand_SegmentedRead->InitJournalManager(pJournalManager);
    if(m_pCommand_AbortSegmentedTransfer) m_pCommand_AbortSegmentedTransfer->InitJournalManager(pJournalManager);
}

void CCommandSetObjectDictionary_DCS_Esam2::ResetJournalManager()
{
    if(m_pCommand_WriteObject) m_pCommand_WriteObject->ResetJournalManager();
    if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->ResetJournalManager();
    if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->ResetJournalManager();
    if(m_pCommand_ReadObject) m_pCommand_ReadObject->ResetJournalManager();
    if(m_pCommand_InitiateSegmentedRead) m_pCommand_InitiateSegmentedRead->ResetJournalManager();
    if(m_pCommand_SegmentedRead) m_pCommand_SegmentedRead->ResetJournalManager();
    if(m_pCommand_AbortSegmentedTransfer) m_pCommand_AbortSegmentedTransfer->ResetJournalManager();
}

BOOL CCommandSetObjectDictionary_DCS_Esam2::InitGateway(CGateway *pGateway)
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

CXXMLFile::CElementPart* CCommandSetObjectDictionary_DCS_Esam2::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
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


BOOL CCommandSetObjectDictionary_DCS_Esam2::DCS_InitiateSegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD* pdObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedRead && pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedRead->ResetStatus();
        m_pCommand_InitiateSegmentedRead->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
		m_pCommand_InitiateSegmentedRead->SetParameterData(1, &uNodeId, sizeof(uNodeId));
        m_pCommand_InitiateSegmentedRead->SetParameterData(2, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(3, &uSubIndex, sizeof(uSubIndex));
        
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

BOOL CCommandSetObjectDictionary_DCS_Esam2::DCS_InitiateSegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedWrite && pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedWrite->ResetStatus();
        m_pCommand_InitiateSegmentedWrite->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
		m_pCommand_InitiateSegmentedWrite->SetParameterData(1, &uNodeId, sizeof(uNodeId));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(2, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(3, &uSubIndex, sizeof(uSubIndex));
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

BOOL CCommandSetObjectDictionary_DCS_Esam2::DCS_ReadObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectBufferLength, DWORD* pdObjectLengthRead, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
	DWORD dObjectLength = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadObject->ResetStatus();
        m_pCommand_ReadObject->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
		m_pCommand_ReadObject->SetParameterData(1, &uNodeId, sizeof(uNodeId));
        m_pCommand_ReadObject->SetParameterData(2, &wIndex, sizeof(wIndex));
        m_pCommand_ReadObject->SetParameterData(3, &uSubIndex, sizeof(uSubIndex));

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

BOOL CCommandSetObjectDictionary_DCS_Esam2::DCS_SegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BOOL oToggle, BOOL* poLastDataSegment, BYTE* pSegmentBuffer, DWORD dSegmentBufferLength, DWORD* pdSegmentLengthRead, CErrorInfo* pErrorInfo)
{
    UEsam2ControlByte controlByte;
	BYTE uDummyByte = 0;

	DWORD dDeviceErrorCode;
	BYTE uLength = 0;
	UEsam2ControlByte retControlByte;
    
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedRead && pManager)
    {
        //Prepare Parameter
		controlByte.ubValue = 0;
        controlByte.structure.ubToggle = oToggle;

		//Set Parameter Data
        m_pCommand_SegmentedRead->ResetStatus();
        m_pCommand_SegmentedRead->SetParameterData(0, &controlByte, sizeof(controlByte));
		m_pCommand_SegmentedRead->SetParameterData(1, &uDummyByte, sizeof(uDummyByte));

        //Execute Command
        oResult = m_pCommand_SegmentedRead->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedRead->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
		m_pCommand_SegmentedRead->GetReturnParameterData(1, &uLength, sizeof(uLength));
        m_pCommand_SegmentedRead->GetReturnParameterData(2, &retControlByte, sizeof(retControlByte));
        m_pCommand_SegmentedRead->GetReturnParameterData(3, pSegmentBuffer, dSegmentBufferLength);

        //Get ErrorCode
        m_pCommand_SegmentedRead->GetErrorInfo(pErrorInfo);

        //Restore Parameters
        if(pdSegmentLengthRead) *pdSegmentLengthRead = uLength;
        if(poLastDataSegment) *poLastDataSegment = controlByte.structure.ubLastDataSegment;
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Esam2::DCS_SegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BOOL oToggle, BOOL oLastDataSegment, BYTE* pSegmentBuffer, DWORD dSegmentLength, DWORD* pdSegmentLengthWritten, CErrorInfo* pErrorInfo)
{
    BYTE uLength = 0;
    UEsam2ControlByte controlByte;
    
	DWORD dDeviceErrorCode;
	BYTE uLengthWritten = 0;
	UEsam2ControlByte retControlByte;
	
	BOOL oResult = FALSE;

    if(m_pCommand_SegmentedWrite && pManager)
    {
        //Prepare Parameters
		uLength = (BYTE)dSegmentLength;
        controlByte.ubValue = 0;
        controlByte.structure.ubToggle = oToggle;
		controlByte.structure.ubLastDataSegment = oLastDataSegment;

        //Set Parameter Data
        m_pCommand_SegmentedWrite->ResetStatus();
        m_pCommand_SegmentedWrite->SetParameterData(0, &uLength, sizeof(uLength));
        m_pCommand_SegmentedWrite->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedWrite->SetParameterData(2, pSegmentBuffer, dSegmentLength);

        //Execute Command
        oResult = m_pCommand_SegmentedWrite->Execute(pManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedWrite->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
		m_pCommand_SegmentedWrite->GetReturnParameterData(1, &uLengthWritten, sizeof(uLengthWritten));
		m_pCommand_SegmentedWrite->GetReturnParameterData(2, &retControlByte, sizeof(retControlByte));

		//Restore Parameters
        if(pdSegmentLengthWritten) *pdSegmentLengthWritten = uLengthWritten;

        //Get ErrorCode
        m_pCommand_SegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetObjectDictionary_DCS_Esam2::DCS_WriteObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_WriteObject->ResetStatus();
        m_pCommand_WriteObject->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
		m_pCommand_WriteObject->SetParameterData(1, &uNodeId, sizeof(uNodeId));
        m_pCommand_WriteObject->SetParameterData(2, &wIndex, sizeof(wIndex));
        m_pCommand_WriteObject->SetParameterData(3, &uSubIndex, sizeof(uSubIndex));
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

BOOL CCommandSetObjectDictionary_DCS_Esam2::DCS_AbortSegmentedTransfer(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNetworkId, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_AbortSegmentedTransfer)
    {
        //Set Parameter Data
        m_pCommand_AbortSegmentedTransfer->ResetStatus();
		m_pCommand_AbortSegmentedTransfer->SetParameterData(0, &wNetworkId, sizeof(wNetworkId));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(1, &uNodeId, sizeof(uNodeId));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(2, &wIndex, sizeof(wIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(3, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(4, &dAbortCode, sizeof(dAbortCode));

        //Execute Command
        oResult = m_pCommand_AbortSegmentedTransfer->Execute(pManager, hHandle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSegmentedTransfer->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

