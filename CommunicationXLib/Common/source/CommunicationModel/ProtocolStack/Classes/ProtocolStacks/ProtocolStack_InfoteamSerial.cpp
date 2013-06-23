// ProtocolStack_InfoteamSerial.cpp: Implementierung der Klasse CProtocolStack_InfoteamSerial.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ProtocolStack_InfoteamSerial.h"

#include <malloc.h>
#include <algorithm>

#include "../GatewayToInterface/BaseClasses/GatewayPStoI.h"
#include "../GatewayToInterface/InfoteamSerial/GatewayInfoteamSerialToRS232.h"
#include "../GatewayToDevice/BaseClasses/GatewayPStoDCS.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include "../../../CommonLayer/Classes/Commands/ProtocolStack/Command_PS_InfoteamSerial.h"
#include "InterfacePlugInManager/InterfacePlugInManager.h"
#include <CommunicationModel/Interface/InterfaceManager.h>
#include "../../../DeviceCommandSet/DeviceCommandSetManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CProtocolStack_InfoteamSerial::CProtocolStack_InfoteamSerial()
{
    m_strProtocolStackName = PROTOCOL_INFOTEAM_SERIAL;

    m_pCommand_ProcessProtocol = 0;
    m_pCommand_ProcessProtocolMaxon = 0;
	m_pCommand_AbortProtocolMaxon = 0;
    m_pCommand_SendChunk = 0;
    m_pCommand_ReceiveData = 0;

    m_pJournalManager = 0;
    InitCommands();
    InitErrorHandling();
    CreatePlugInManager();
}

CProtocolStack_InfoteamSerial::CProtocolStack_InfoteamSerial(const CProtocolStack_InfoteamSerial& rObject):CProtocolStackBase(rObject)
{
    m_strProtocolStackName = rObject.m_strProtocolStackName;

    m_pCommand_ProcessProtocol = 0;
    m_pCommand_ProcessProtocolMaxon = 0;
	m_pCommand_AbortProtocolMaxon = 0;
    m_pCommand_SendChunk = 0;
    m_pCommand_ReceiveData = 0;

    InitCommands();
    InitJournalManager(rObject.m_pJournalManager);
    if(m_pGatewayToInterface) InitCommands((CGateway*)m_pGatewayToInterface); else InitCommands((CGateway*)m_pGatewayToDevice);
    InitErrorHandling();
    CreatePlugInManager();
}

CProtocolStackBase* CProtocolStack_InfoteamSerial::Clone()
{
    CProtocolStack_InfoteamSerial* pProtocolStack;

    pProtocolStack = new CProtocolStack_InfoteamSerial(*this);
    return pProtocolStack;
}

CProtocolStack_InfoteamSerial::~CProtocolStack_InfoteamSerial()
{
    DeleteCommands();
}

BOOL CProtocolStack_InfoteamSerial::CreatePlugInManager()
{
    const DWORD NOTIFICATION_DELAY = 5000;

    BOOL oResult = FALSE;

    if(CProtocolStackBase::CreatePlugInManager() && m_pPlugInManager)
    {
        oResult = m_pPlugInManager->SetNotificationDelay(NOTIFICATION_DELAY);
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitGatewayToInterface(CStdString strInterfaceName)
{
    BOOL oResult = FALSE;

    //Reset
	DeleteGatewayToInterface();

	//Create Gateway
    if(strInterfaceName.CompareNoCase(INTERFACE_RS232) == 0)
    {
        m_pGatewayToInterface = new CGatewayInfoteamSerialToRS232();
    }

	//InitGateway
	if(m_pGatewayToInterface)
	{
		if(m_pGatewayToInterface->InitGateway())
        {
            InitParameterSet();
			InitCommands(m_pGatewayToInterface);
            oResult = TRUE;
        }
        else
        {
            DeleteGatewayToInterface();
        }
	}

	return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitGatewayToDevice(CStdString strDeviceName)
{
    BOOL oResult = FALSE;

    //Reset
	DeleteGatewayToDevice();

	//Create Gateway
    //No Gateway supported

	//InitGateway
	if(m_pGatewayToDevice)
	{
		if(m_pGatewayToDevice->InitGateway())
        {
            InitParameterSet();
			InitCommands((CGateway*)m_pGatewayToDevice);
            oResult = TRUE;
        }
        else
        {
            DeleteGatewayToDevice();
        }
	}

	return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitParameterSet()
{
	CGateway* pGateway = 0;
	BOOL oResult = TRUE;

	if(m_pGatewayToInterface)
	{
		pGateway = (CGateway*)m_pGatewayToInterface;
	}
	else if(m_pGatewayToDevice)
	{
		pGateway = (CGateway*)m_pGatewayToDevice;
	}

	if(pGateway)
	{
		if(!pGateway->ResetParameterSet()) oResult = FALSE;
	}

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "ProtocolStack_InfoteamSerial";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStack_InfoteamSerial::InitProtocolStack(CErrorInfo* pErrorInfo)
{
    BOOL oResult = TRUE;

    //Klassenstruktur für Interfaces RS232 aufbauen
    if(!InitProtocolStack(INTERFACE_RS232))
    {
        oResult = FALSE;
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitProtocolStack(CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CInterfaceManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strInterfaceName.size() == 0)
    {
        //Don't create an interface manager
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(strInterfaceName, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CInterfaceManagerBase*) new CInterfaceManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

        //Klassenstruktur für Interface aufbauen
        if(VerifyGatewayToInterface(strInterfaceName))
        {
            if((pManager->I_InitInterface(strInterfaceName, pErrorInfo)))
            {
                //BaudrateSelection
                InitBaudrateSelection(pManager, strInterfaceName);

                //DefaultProtocolStackSettings
                InitDefaultProtocolStackSettings(pManager, strInterfaceName);

                oResult = TRUE;
            }
            else
            {
                //Interface not available
                oResult = TRUE;
            }

            //Init PlugIn Manager
            if(m_pPlugInManager)m_pPlugInManager->RegisterInterface(strInterfaceName);

            if(oNewCreated) m_InterfaceManagerList.push_back(pManager);
        }
        else
        {
            if(oNewCreated) delete pManager;
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStackName, CErrorInfo* pErrorInfo)
{
	CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strDeviceName.size() == 0)
    {
        //Don't create an device manager
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(strDeviceName, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CDeviceCommandSetManagerBase*) new CDeviceCommandSetManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

		//Klassenstruktur für Device aufbauen
        if((VerifyGatewayToDevice(strDeviceName)) && (pManager->DCS_InitDevice(strDeviceName, strProtocolStackName, pErrorInfo)))
        {
            if(oNewCreated) m_DeviceCommandSetManagerList.push_back(pManager);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pManager;
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitBaudrateSelection(CInterfaceManagerBase* pInterfaceManager, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CStdDWordArray dBaudrateSelection;
    BOOL oResult = FALSE;

    if(pInterfaceManager)
    {
        //Baudrate Selection
        dBaudrateSelection.clear();
        dBaudrateSelection.push_back(115200);

        //Init
        oResult = pInterfaceManager->I_InitBaudrateSelection(strInterfaceName, dBaudrateSelection, pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::InitDefaultProtocolStackSettings(CInterfaceManagerBase* pInterfaceManager, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    DWORD dDefaultBaudrate = 0;
    DWORD dDefaultTimeout = 0;
    BOOL oResult = FALSE;

    if(pInterfaceManager)
    {
        //Default
        dDefaultBaudrate = 115200;
        dDefaultTimeout = 500;

        //Init
        oResult = pInterfaceManager->I_InitDefaultInterfaceSettings(strInterfaceName, dDefaultBaudrate, dDefaultTimeout, pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::VerifyGatewayToInterface(CStdString strInterfaceName)
{
    //GatewayInfoteamSerialToRS232
    if(strInterfaceName.CompareNoCase(INTERFACE_RS232) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStack_InfoteamSerial::VerifyGatewayToDevice(CStdString strDeviceName)
{
    //No Gateway supported
    return FALSE;
}

BOOL CProtocolStack_InfoteamSerial::PS_ProcessProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dPackageSize;
    WORD wChunkSize;
    BYTE uLastChunkFlag;
    DWORD dChecksum = 0;        //Calculated internally

    DWORD dRetPackageSize;
    DWORD dRetChecksum;

    BOOL oResult = FALSE;
    DWORD dRetDataLength;

    if(m_pCommand_ProcessProtocol && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Prepare Parameter
        if(dDataBufferLength > USHRT_MAX) wChunkSize = USHRT_MAX; else wChunkSize = (WORD)dDataBufferLength;
        dPackageSize = wChunkSize;
        uLastChunkFlag = k_FirstAndLastChunk;

        //Set Parameter Data
        m_pCommand_ProcessProtocol->ResetStatus();
        m_pCommand_ProcessProtocol->SetParameterData(0, &dPackageSize, sizeof(dPackageSize));
        m_pCommand_ProcessProtocol->SetParameterData(1, &wChunkSize, sizeof(wChunkSize));
        m_pCommand_ProcessProtocol->SetParameterData(2, &uLastChunkFlag, sizeof(uLastChunkFlag));
        m_pCommand_ProcessProtocol->SetParameterData(3, &dChecksum, sizeof(dChecksum));
        m_pCommand_ProcessProtocol->SetParameterData(4, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = m_pCommand_ProcessProtocol->Execute(pManager, hI_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        dRetDataLength = m_pCommand_ProcessProtocol->GetReturnParameterLength(2);
        if(dRetDataLength > 0) *ppRetDataBuffer = malloc(dRetDataLength);

        //Get ReturnParameter Data
        m_pCommand_ProcessProtocol->GetReturnParameterData(0, &dRetPackageSize, sizeof(dRetPackageSize));
        m_pCommand_ProcessProtocol->GetReturnParameterData(1, &dRetChecksum, sizeof(dRetChecksum));
        m_pCommand_ProcessProtocol->GetReturnParameterData(2, *ppRetDataBuffer, dRetDataLength);

        //Get ErrorInfo
        m_pCommand_ProcessProtocol->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::PS_ProcessProtocolMaxon(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dPackageSize;
    WORD wChunkSize;
    BYTE uLastChunkFlag;
    DWORD dChecksum = 0;        //Calculated internally
    BYTE uPackageType;

    DWORD dRetPackageSize;
    DWORD dRetChecksum;
    BYTE uRetPackageType;

    BOOL oResult = FALSE;
    DWORD dRetDataLength;

    if(m_pCommand_ProcessProtocolMaxon && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Prepare Parameter
        if(dDataBufferLength > USHRT_MAX) wChunkSize = USHRT_MAX; else wChunkSize = (WORD)dDataBufferLength;
        dPackageSize = wChunkSize;
        uLastChunkFlag = k_FirstAndLastChunk;
        uPackageType = k_PT_MAXON_CMD;

        //Set Parameter Data
        m_pCommand_ProcessProtocolMaxon->ResetStatus();
        m_pCommand_ProcessProtocolMaxon->SetParameterData(0, &dPackageSize, sizeof(dPackageSize));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(1, &wChunkSize, sizeof(wChunkSize));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(2, &uLastChunkFlag, sizeof(uLastChunkFlag));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(3, &dChecksum, sizeof(dChecksum));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(4, &uPackageType, sizeof(uPackageType));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(5, &uOpCode, sizeof(uOpCode));
        m_pCommand_ProcessProtocolMaxon->SetParameterData(6, pDataBuffer, dDataBufferLength);
		m_pCommand_ProcessProtocolMaxon->SetParameterData(7, &ubKeepLock, sizeof(ubKeepLock));

        //Execute Command
        oResult = m_pCommand_ProcessProtocolMaxon->Execute(pManager, hI_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        dRetDataLength = m_pCommand_ProcessProtocolMaxon->GetReturnParameterLength(2);
        if(dRetDataLength > 0) *ppRetDataBuffer = malloc(dRetDataLength);

        //Get ReturnParameter Data
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(0, &dRetPackageSize, sizeof(dRetPackageSize));
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(1, &dRetChecksum, sizeof(dRetChecksum));
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(2, &uRetPackageType, sizeof(uRetPackageType));
        m_pCommand_ProcessProtocolMaxon->GetReturnParameterData(3, *ppRetDataBuffer, dRetDataLength);

        //Get ErrorInfo
        m_pCommand_ProcessProtocolMaxon->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::PS_AbortProtocolMaxon(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

    if(m_pCommand_AbortProtocolMaxon)
    {
        //Set Parameter Data
        m_pCommand_AbortProtocolMaxon->ResetStatus();

        //Execute Command
        oResult = m_pCommand_AbortProtocolMaxon->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortProtocolMaxon->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::PS_SendChunk(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dPackageSize, BOOL oFirstChunk, BOOL oLastChunk, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    WORD wChunkSize;
    BYTE uLastChunkFlag;
    DWORD dChecksum = 0;        //Calculated internally

    BOOL oResult = FALSE;

    if(m_pCommand_SendChunk)
    {
        //Prepare Parameter
        if(dDataBufferLength > USHRT_MAX) wChunkSize = USHRT_MAX; else wChunkSize = (WORD)dDataBufferLength;
        dPackageSize = wChunkSize;
        uLastChunkFlag = k_FirstAndLastChunk;

        //Set Parameter Data
        m_pCommand_SendChunk->ResetStatus();
        m_pCommand_SendChunk->SetParameterData(0, &dPackageSize, sizeof(dPackageSize));
        m_pCommand_SendChunk->SetParameterData(1, &wChunkSize, sizeof(wChunkSize));
        m_pCommand_SendChunk->SetParameterData(2, &uLastChunkFlag, sizeof(uLastChunkFlag));
        m_pCommand_SendChunk->SetParameterData(3, &dChecksum, sizeof(dChecksum));
        m_pCommand_SendChunk->SetParameterData(4, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = m_pCommand_SendChunk->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ErrorInfo
        m_pCommand_SendChunk->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_InfoteamSerial::PS_ReceiveData(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dRetPackageSize;
    DWORD dRetChecksum;

    BOOL oResult = FALSE;
    DWORD dRetDataLength;

    if(m_pCommand_ReceiveData && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Set Parameter Data
        m_pCommand_ReceiveData->ResetStatus();

        //Execute Command
        oResult = m_pCommand_ReceiveData->Execute(pManager, hI_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        dRetDataLength = m_pCommand_ReceiveData->GetReturnParameterLength(2);
        if(dRetDataLength > 0) *ppRetDataBuffer = malloc(dRetDataLength);

        //Get ReturnParameter Data
        m_pCommand_ReceiveData->GetReturnParameterData(0, &dRetPackageSize, sizeof(dRetPackageSize));
        m_pCommand_ReceiveData->GetReturnParameterData(1, &dRetChecksum, sizeof(dRetChecksum));
        m_pCommand_ReceiveData->GetReturnParameterData(2, *ppRetDataBuffer, dRetDataLength);

        //Get ErrorInfo
        m_pCommand_ReceiveData->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

void CProtocolStack_InfoteamSerial::InitCommands(CGateway* pGateway)
{
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->InitGateway(pGateway);
	if(m_pCommand_ProcessProtocolMaxon) m_pCommand_ProcessProtocolMaxon->InitGateway(pGateway);
	if(m_pCommand_AbortProtocolMaxon) m_pCommand_AbortProtocolMaxon->InitGateway(pGateway);
	if(m_pCommand_SendChunk) m_pCommand_SendChunk->InitGateway(pGateway);
	if(m_pCommand_ReceiveData) m_pCommand_ReceiveData->InitGateway(pGateway);
}

void CProtocolStack_InfoteamSerial::InitCommands()
{
    DeleteCommands();

    //Process Protocol
    m_pCommand_ProcessProtocol = new CCommand_PS_InfoteamSerial();
    m_pCommand_ProcessProtocol->InitCommand(INFOTEAM_SERIAL_PROCESS_PROTOCOL);

    //Process ProtocolMaxon
    m_pCommand_ProcessProtocolMaxon = new CCommand_PS_InfoteamSerial();
    m_pCommand_ProcessProtocolMaxon->InitCommand(INFOTEAM_SERIAL_PROCESS_PROTOCOL_MAXON);

	//Abort ProtocolMaxon
    m_pCommand_AbortProtocolMaxon = new CCommand_PS_InfoteamSerial();
    m_pCommand_AbortProtocolMaxon->InitCommand(INFOTEAM_SERIAL_ABORT_PROTOCOL_MAXON);

    //SendChunk
    m_pCommand_SendChunk = new CCommand_PS_InfoteamSerial();
    m_pCommand_SendChunk->InitCommand(INFOTEAM_SERIAL_SEND_CHUNK);

    //ReceiveData
    m_pCommand_ReceiveData = new CCommand_PS_InfoteamSerial();
    m_pCommand_ReceiveData->InitCommand(INFOTEAM_SERIAL_RECEIVE_DATA);
}

BOOL CProtocolStack_InfoteamSerial::GetCommands(CStdString* pCommandInfo)
{
	BOOL oResult = TRUE;
    CXXMLFile xmlFile;
    CXXMLFile::CElementPart* pElementPart;
    CXXMLFile::CElement* pElement;

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
        pElement->SetAt("Name", m_strProtocolStackName);

        //Command Elements
        if(m_pCommand_ProcessProtocol && !m_pCommand_ProcessProtocol->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_ProcessProtocolMaxon && !m_pCommand_ProcessProtocolMaxon->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
		if(m_pCommand_AbortProtocolMaxon && !m_pCommand_AbortProtocolMaxon->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_SendChunk && !m_pCommand_SendChunk->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_ReceiveData && !m_pCommand_ReceiveData->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;

        //Write to string
        xmlFile.WriteToString(pCommandInfo);
    }

    return oResult;
}

void CProtocolStack_InfoteamSerial::DeleteCommands()
{
    if(m_pCommand_ProcessProtocol)
    {
        delete m_pCommand_ProcessProtocol;
        m_pCommand_ProcessProtocol = 0;
    }

    if(m_pCommand_ProcessProtocolMaxon)
    {
        delete m_pCommand_ProcessProtocolMaxon;
        m_pCommand_ProcessProtocolMaxon = 0;
    }

	if(m_pCommand_AbortProtocolMaxon)
    {
        delete m_pCommand_AbortProtocolMaxon;
        m_pCommand_AbortProtocolMaxon = 0;
    }

    if(m_pCommand_SendChunk)
    {
        delete m_pCommand_SendChunk;
        m_pCommand_SendChunk = 0;
    }

    if(m_pCommand_ReceiveData)
    {
        delete m_pCommand_ReceiveData;
        m_pCommand_ReceiveData = 0;
    }
}

void CProtocolStack_InfoteamSerial::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CProtocolStackBase::InitJournalManager(pJournalManager);
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->InitJournalManager(pJournalManager);
    if(m_pCommand_ProcessProtocolMaxon) m_pCommand_ProcessProtocolMaxon->InitJournalManager(pJournalManager);
	if(m_pCommand_AbortProtocolMaxon) m_pCommand_AbortProtocolMaxon->InitJournalManager(pJournalManager);
    if(m_pCommand_SendChunk) m_pCommand_SendChunk->InitJournalManager(pJournalManager);
    if(m_pCommand_ReceiveData) m_pCommand_ReceiveData->InitJournalManager(pJournalManager);
}

void CProtocolStack_InfoteamSerial::ResetJournalManager()
{
    CProtocolStackBase::ResetJournalManager();
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->ResetJournalManager();
    if(m_pCommand_ProcessProtocolMaxon) m_pCommand_ProcessProtocolMaxon->ResetJournalManager();
	if(m_pCommand_AbortProtocolMaxon) m_pCommand_AbortProtocolMaxon->ResetJournalManager();
    if(m_pCommand_SendChunk) m_pCommand_SendChunk->ResetJournalManager();
    if(m_pCommand_ReceiveData) m_pCommand_ReceiveData->ResetJournalManager();
}

