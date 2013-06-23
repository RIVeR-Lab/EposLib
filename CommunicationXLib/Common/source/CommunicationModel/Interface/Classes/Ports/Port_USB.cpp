// Port_USB.cpp: Implementierung der Klasse CPort_USB.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Port_USB.h"


#include <string.h>
#include <algorithm>

#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/BaseClasses/Command_I.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_USB.h>
#include <CommunicationModel/Interface/Classes/Gateway/USB/GatewayUSBToFtd2xxDrv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CPort_USB::CPort_USB()
{
    InitErrorHandling();

    m_strInterfacePortName = INTERFACE_PORT_USB;

    m_pCommand_WriteData = 0;
    m_pCommand_ReadData = 0;
    InitCommands();

    SetDefault();
}

CPort_USB::CPort_USB(CPort_USB& rObject):CPortBase(rObject)
{
    InitErrorHandling();

    m_strInterfacePortName = rObject.m_strInterfacePortName;
    rObject.ResetPortMembers();

    m_pCommand_WriteData = 0;
    m_pCommand_ReadData = 0;

    InitCommands();
    InitJournalManager(rObject.m_pJournalManager);
}

CPort_USB::~CPort_USB()
{
    DeleteCommands();
}

CPortBase* CPort_USB::Clone()
{
    CPortBase* pPortBase = new CPort_USB(*this);
    return pPortBase;
}

void CPort_USB::SetDefault()
{
    m_strPortName = "";
}

BOOL CPort_USB::operator==(const CPort_USB& rPort)
{
	CStdString strPortName1 = m_strPortName;
	CStdString strPortName2 = rPort.m_strPortName;

    return (strPortName1.CompareNoCase(strPortName2) == 0);
}

CPort_USB& CPort_USB::operator=(CPort_USB& rSetting)
{
    if(this != &rSetting)
    {
        m_strPortName = rSetting.m_strPortName;
    }

    return *this;
}

void CPort_USB::ResetPortMembers()
{
    m_strPortName = "";
}

BOOL CPort_USB::I_WriteData(HANDLE hTransactionHandle, void* pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    CLayerManagerBase* pLayerManager = 0;
    HANDLE hHandle = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteData)
    {
        //Set Parameter Data
        m_pCommand_WriteData->ResetStatus();
        m_pCommand_WriteData->SetParameterData(0, pData, dNumberOfBytesToWrite);

        //Execute Command
        oResult = m_pCommand_WriteData->Execute(pLayerManager, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteData->GetReturnParameterData(0, pdNumberOfBytesWritten, sizeof(*pdNumberOfBytesWritten));

        //Get ErrorCode
        m_pCommand_WriteData->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CPort_USB::I_ReadData(HANDLE hTransactionHandle, void* pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    CLayerManagerBase* pLayerManager = 0;
    HANDLE hHandle = 0;
    BOOL oResult = FALSE;
    DWORD dataLength;

    if(m_pCommand_ReadData)
    {
        //Set Parameter Data
        m_pCommand_ReadData->ResetStatus();
        m_pCommand_ReadData->SetParameterData(0, &dNumberOfBytesToRead, sizeof(dNumberOfBytesToRead));

        //Execute Command
        oResult = m_pCommand_ReadData->Execute(pLayerManager, hHandle, hTransactionHandle);

        //Limit Parameter Length
        dataLength = m_pCommand_ReadData->GetReturnParameterLength(0);
        if(dataLength > dNumberOfBytesToRead) dataLength = dNumberOfBytesToRead;

        //Get ReturnParameter Data
        m_pCommand_ReadData->GetReturnParameterData(0, pData, dataLength);
        if(pdNumberOfBytesRead) *pdNumberOfBytesRead = dataLength;

        //Get ErrorCode
        m_pCommand_ReadData->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

void CPort_USB::InitCommands()
{
    DeleteCommands();

    //Init Command WriteData
    m_pCommand_WriteData = new CCommand_I_USB();
    m_pCommand_WriteData->InitCommand(USB_WRITE_DATA);

    //Init Command ReadData
    m_pCommand_ReadData = new CCommand_I_USB();
    m_pCommand_ReadData->InitCommand(USB_READ_DATA);
}

BOOL CPort_USB::GetCommands(CStdString* pCommandInfo)
{
    CXXMLFile xmlFile;
    CXXMLFile::CElementPart* pElementPart;
    CXXMLFile::CElement* pElement;
    BOOL oResult = TRUE;

    if(pCommandInfo)
    {
        //Root
        pElementPart = xmlFile.Root();
        if(!xmlFile.IsElement(pElementPart)) return FALSE;

        //CommandInfo Elements
        pElement = (CXXMLFile::CElement*)xmlFile.AddElement(pElementPart);
        xmlFile.SetText(pElement, "CommandInfo");

        //CommandGroup Elements
        pElement = (CXXMLFile::CElement*)xmlFile.AddElement(pElement);
        xmlFile.SetText(pElement, "CommandGroup");
        pElement->SetAt("Name", m_strPortName);

        //Command Elements
        if(m_pCommand_ReadData && !m_pCommand_ReadData->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_WriteData && !m_pCommand_WriteData->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;

        //Write to string
        xmlFile.WriteToString(pCommandInfo);
        return oResult;
    }

    return FALSE;
}

void CPort_USB::DeleteCommands()
{
    if(m_pCommand_ReadData)
    {
        delete m_pCommand_ReadData;
        m_pCommand_ReadData = 0;
    }

    if(m_pCommand_WriteData)
    {
        delete m_pCommand_WriteData;
        m_pCommand_WriteData = 0;
    }
}

void CPort_USB::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CPortBase::InitJournalManager(pJournalManager);
    if(m_pCommand_WriteData) m_pCommand_WriteData->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadData) m_pCommand_ReadData->InitJournalManager(pJournalManager);
}

void CPort_USB::ResetJournalManager()
{
    CPortBase::ResetJournalManager();
    if(m_pCommand_WriteData) m_pCommand_WriteData->ResetJournalManager();
    if(m_pCommand_ReadData) m_pCommand_ReadData->ResetJournalManager();
}

void CPort_USB::InitCommands(CGatewayIToDrv* pGateway)
{
    if(m_pCommand_WriteData) m_pCommand_WriteData->InitGateway(pGateway);
    if(m_pCommand_ReadData) m_pCommand_ReadData->InitGateway(pGateway);
}

BOOL CPort_USB::InitGateway(CGatewayIToDrv* pGateway)
{
	BOOL oResult = FALSE;

	if(CPortBase::InitGateway(pGateway))
	{
		InitParameterSet();
		oResult = TRUE;
	}

	return oResult;
}

BOOL CPort_USB::InitParameterSet()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
	}

    return oResult;
}

BOOL CPort_USB::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "Port_USB";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(INTERFACE_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

