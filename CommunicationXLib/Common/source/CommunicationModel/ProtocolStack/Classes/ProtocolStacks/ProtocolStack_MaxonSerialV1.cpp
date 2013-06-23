// ProtocolStack_MaxonSerialV1.cpp: Implementierung der Klasse CProtocolStack_MaxonSerialV1.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ProtocolStack_MaxonSerialV1.h"

#include <malloc.h>
#include <algorithm>

#include "../GatewayToInterface/BaseClasses/GatewayPStoI.h"
#include "../GatewayToInterface/MaxonSerialV1/GatewayMaxonSerialV1ToRS232.h"

#include "../GatewayToDevice/BaseClasses/GatewayPStoDCS.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include "../../../CommonLayer/Classes/Commands/ProtocolStack/Command_PS_MaxonSerialV1.h"

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
CProtocolStack_MaxonSerialV1::CProtocolStack_MaxonSerialV1()
{
    m_strProtocolStackName = PROTOCOL_MAXON_SERIAL_V1;

    m_pCommand_ProcessProtocol = 0;
    m_pCommand_SendFrame = 0;
	m_pCommand_AbortProtocol = 0;

    m_pJournalManager = 0;
    InitCommands();
    InitErrorHandling();
    CreatePlugInManager();
}

CProtocolStack_MaxonSerialV1::CProtocolStack_MaxonSerialV1(const CProtocolStack_MaxonSerialV1& rObject):CProtocolStackBase(rObject)
{
    m_strProtocolStackName = rObject.m_strProtocolStackName;

    m_pCommand_ProcessProtocol = 0;
    m_pCommand_SendFrame = 0;
	m_pCommand_AbortProtocol = 0;

    InitCommands();
    InitJournalManager(rObject.m_pJournalManager);
    if(m_pGatewayToInterface) InitCommands((CGateway*)m_pGatewayToInterface); else InitCommands((CGateway*)m_pGatewayToDevice);
    InitErrorHandling();
}

CProtocolStackBase* CProtocolStack_MaxonSerialV1::Clone()
{
    CProtocolStack_MaxonSerialV1* pProtocolStack;

    pProtocolStack = new CProtocolStack_MaxonSerialV1(*this);
    return pProtocolStack;
}

CProtocolStack_MaxonSerialV1::~CProtocolStack_MaxonSerialV1()
{
    DeleteCommands();
}

BOOL CProtocolStack_MaxonSerialV1::CreatePlugInManager()
{
    const DWORD NOTIFICATION_DELAY = 500;

    BOOL oResult = FALSE;

    if(CProtocolStackBase::CreatePlugInManager() && m_pPlugInManager)
    {
        oResult = m_pPlugInManager->SetNotificationDelay(NOTIFICATION_DELAY);
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV1::InitGatewayToInterface(CStdString strInterfaceName)
{
    BOOL oResult = FALSE;

    //Reset
	DeleteGatewayToInterface();

	//Create Gateway
    if(strInterfaceName.CompareNoCase(INTERFACE_RS232) == 0)
    {
        m_pGatewayToInterface = new CGatewayMaxonSerialV1ToRS232();
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

BOOL CProtocolStack_MaxonSerialV1::InitGatewayToDevice(CStdString strDeviceName)
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

BOOL CProtocolStack_MaxonSerialV1::InitParameterSet()
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

BOOL CProtocolStack_MaxonSerialV1::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "ProtocolStack_MaxonSerialV1";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStack_MaxonSerialV1::InitProtocolStack(CErrorInfo* pErrorInfo)
{
    BOOL oResult = TRUE;

    //Klassenstruktur für Interfaces RS232 aufbauen
    if(!InitProtocolStack(INTERFACE_RS232))
    {
        oResult = FALSE;
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV1::InitProtocolStack(CStdString strInterfaceName, CErrorInfo* pErrorInfo)
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
            if(pManager->I_InitInterface(strInterfaceName, pErrorInfo))
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

BOOL CProtocolStack_MaxonSerialV1::InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStackName, CErrorInfo* pErrorInfo)
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

BOOL CProtocolStack_MaxonSerialV1::InitBaudrateSelection(CInterfaceManagerBase* pInterfaceManager, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CStdDWordArray dBaudrateSelection;
    BOOL oResult = FALSE;

    if(pInterfaceManager)
    {
        //Baudrate Selection
        dBaudrateSelection.clear();
        dBaudrateSelection.push_back(9600);
        dBaudrateSelection.push_back(14400);
        dBaudrateSelection.push_back(19200);
        dBaudrateSelection.push_back(38400);
        dBaudrateSelection.push_back(57600);
        dBaudrateSelection.push_back(115200);

        //Init
        oResult = pInterfaceManager->I_InitBaudrateSelection(strInterfaceName, dBaudrateSelection, pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV1::InitDefaultProtocolStackSettings(CInterfaceManagerBase* pInterfaceManager, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
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

BOOL CProtocolStack_MaxonSerialV1::VerifyGatewayToInterface(CStdString strInterfaceName)
{
    //GatewayMaxonSerialV1ToRS232
	if(strInterfaceName.CompareNoCase(INTERFACE_RS232) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStack_MaxonSerialV1::VerifyGatewayToDevice(CStdString strDeviceName)
{
    //No Gateway supported
    return FALSE;
}

BOOL CProtocolStack_MaxonSerialV1::PS_ProcessProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    BYTE uLen = 0; //Calculated internally
    WORD wCrc = 0; //Calucated internally

    BYTE uRetLen;
    BYTE uRetOpCode;
    WORD wRetCrc;

    BOOL oResult = FALSE;
    DWORD dRetDataLength;

    if(m_pCommand_ProcessProtocol && ppRetDataBuffer && pdRetDataBufferLength)
    {
        //Set Parameter Data
        m_pCommand_ProcessProtocol->ResetStatus();
        m_pCommand_ProcessProtocol->SetParameterData(0, &uOpCode, sizeof(uOpCode));
        m_pCommand_ProcessProtocol->SetParameterData(1, &uLen, sizeof(uLen));
        m_pCommand_ProcessProtocol->SetParameterData(2, pDataBuffer, sizeof(dDataBufferLength));
        m_pCommand_ProcessProtocol->SetParameterData(3, &wCrc, sizeof(wCrc));
		m_pCommand_ProcessProtocol->SetParameterData(4, &ubKeepLock, sizeof(ubKeepLock));

        //Execute Command
        oResult = m_pCommand_ProcessProtocol->Execute(pManager, hI_Handle, hTransactionHandle);

        //Prepare RetDataBuffer
        dRetDataLength = m_pCommand_ProcessProtocol->GetReturnParameterLength(2);
        if(dRetDataLength > 0) *ppRetDataBuffer = malloc(dRetDataLength);

        //Get ReturnParameter Data
        m_pCommand_ProcessProtocol->GetReturnParameterData(0, &uRetOpCode, sizeof(uRetOpCode));
        m_pCommand_ProcessProtocol->GetReturnParameterData(1, &uRetLen, sizeof(uRetLen));
        m_pCommand_ProcessProtocol->GetReturnParameterData(2, *ppRetDataBuffer, dRetDataLength);
        m_pCommand_ProcessProtocol->GetReturnParameterData(3, &wRetCrc, sizeof(wRetCrc));

        //Get ErrorCode
        m_pCommand_ProcessProtocol->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV1::PS_SendFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    BYTE uLen = 0; //Calculated internally
    WORD wCrc = 0; //Calucated internally

    BOOL oResult = FALSE;

    if(m_pCommand_SendFrame)
    {
        //Set Parameter Data
        m_pCommand_SendFrame->ResetStatus();
        m_pCommand_SendFrame->SetParameterData(0, &uOpCode, sizeof(uOpCode));
        m_pCommand_SendFrame->SetParameterData(1, &uLen, sizeof(uLen));
        m_pCommand_SendFrame->SetParameterData(2, pDataBuffer, dDataBufferLength);
        m_pCommand_SendFrame->SetParameterData(3, &wCrc, sizeof(wCrc));

        //Execute Command
        oResult = m_pCommand_SendFrame->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV1::PS_AbortProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

    if(m_pCommand_AbortProtocol)
    {
        //Set Parameter Data
        m_pCommand_AbortProtocol->ResetStatus();

        //Execute Command
        oResult = m_pCommand_AbortProtocol->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortProtocol->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

void CProtocolStack_MaxonSerialV1::InitCommands(CGateway* pGateway)
{
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->InitGateway(pGateway);
	if(m_pCommand_SendFrame) m_pCommand_SendFrame->InitGateway(pGateway);
	if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->InitGateway(pGateway);
}

void CProtocolStack_MaxonSerialV1::InitCommands()
{
    DeleteCommands();

    //Process Protocol
    m_pCommand_ProcessProtocol = new CCommand_PS_MaxonSerialV1();
    m_pCommand_ProcessProtocol->InitCommand(MAXON_SERIAL_V1_PROCESS_PROTOCOL);

    //SendFrame
    m_pCommand_SendFrame = new CCommand_PS_MaxonSerialV1();
    m_pCommand_SendFrame->InitCommand(MAXON_SERIAL_V1_SEND_FRAME);

	//Abort Protocol
    m_pCommand_AbortProtocol = new CCommand_PS_MaxonSerialV1();
    m_pCommand_AbortProtocol->InitCommand(MAXON_SERIAL_V1_ABORT_PROTOCOL);

}

BOOL CProtocolStack_MaxonSerialV1::GetCommands(CStdString* pCommandInfo)
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
        if(m_pCommand_SendFrame && !m_pCommand_SendFrame->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
		if(m_pCommand_AbortProtocol && !m_pCommand_AbortProtocol->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;

        //Write to string
        xmlFile.WriteToString(pCommandInfo);
    }

   return oResult;
}

void CProtocolStack_MaxonSerialV1::DeleteCommands()
{
    if(m_pCommand_ProcessProtocol)
    {
        delete m_pCommand_ProcessProtocol;
        m_pCommand_ProcessProtocol = 0;
    }

    if(m_pCommand_SendFrame)
    {
        delete m_pCommand_SendFrame;
        m_pCommand_SendFrame = 0;
    }

	if(m_pCommand_AbortProtocol)
    {
        delete m_pCommand_AbortProtocol;
        m_pCommand_AbortProtocol = 0;
    }
}

void CProtocolStack_MaxonSerialV1::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CProtocolStackBase::InitJournalManager(pJournalManager);
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->InitJournalManager(pJournalManager);
    if(m_pCommand_SendFrame) m_pCommand_SendFrame->InitJournalManager(pJournalManager);
	if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->InitJournalManager(pJournalManager);
}

void CProtocolStack_MaxonSerialV1::ResetJournalManager()
{
    CProtocolStackBase::ResetJournalManager();
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->ResetJournalManager();
    if(m_pCommand_SendFrame) m_pCommand_SendFrame->ResetJournalManager();
	if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->ResetJournalManager();
}

