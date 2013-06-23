// ProtocolStack_MaxonSerialV2.cpp: Implementierung der Klasse CProtocolStack_MaxonSerialV2.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProtocolStack_MaxonSerialV2.h"
#include <algorithm>
#include <malloc.h>
#include "../GatewayToInterface/BaseClasses/GatewayPStoI.h"
#include "ProtocolStackMaxonSerialV2Factory.h"

#include "../GatewayToDevice/BaseClasses/GatewayPStoDCS.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include "../../../CommonLayer/Classes/Commands/ProtocolStack/Command_PS_MaxonSerialV2.h"

#include <CommunicationModel/Interface/InterfaceManager.h>
#include "InterfacePlugInManager/InterfacePlugInManager.h"
#include "../../../DeviceCommandSet/DeviceCommandSetManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CProtocolStack_MaxonSerialV2::CProtocolStack_MaxonSerialV2()
{
    m_strProtocolStackName = PROTOCOL_MAXON_SERIAL_V2;

    m_pCommand_ProcessProtocol = 0;
	m_pCommand_AbortProtocol = 0;
    m_pJournalManager = 0;

    InitCommands();
    InitErrorHandling();
    CreatePlugInManager();
}

CProtocolStack_MaxonSerialV2::CProtocolStack_MaxonSerialV2(const CProtocolStack_MaxonSerialV2& rObject):CProtocolStackBase(rObject)
{
    m_strProtocolStackName = rObject.m_strProtocolStackName;

    m_pCommand_ProcessProtocol = 0;
	m_pCommand_AbortProtocol = 0;

    InitCommands();
    InitJournalManager(rObject.m_pJournalManager);
    if(m_pGatewayToInterface) InitCommands((CGateway*)m_pGatewayToInterface); else InitCommands((CGateway*)m_pGatewayToDevice);
    InitErrorHandling();
}

CProtocolStackBase* CProtocolStack_MaxonSerialV2::Clone()
{
    CProtocolStack_MaxonSerialV2* pProtocolStack;

    pProtocolStack = new CProtocolStack_MaxonSerialV2(*this);
    return pProtocolStack;
}

CProtocolStack_MaxonSerialV2::~CProtocolStack_MaxonSerialV2()
{
    DeleteCommands();
}

BOOL CProtocolStack_MaxonSerialV2::CreatePlugInManager()
{
    const DWORD NOTIFICATION_DELAY = 500;
    BOOL oResult = FALSE;

    if(CProtocolStackBase::CreatePlugInManager() && m_pPlugInManager)
    {
        oResult = m_pPlugInManager->SetNotificationDelay(NOTIFICATION_DELAY);
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV2::InitGatewayToInterface(CStdString strInterfaceName)
{
    BOOL oResult = TRUE;

    //Reset
	DeleteGatewayToInterface();

    if(oResult) oResult = CProtocolStackMaxonSerialV2Factory::CreateInstance(strInterfaceName, m_pGatewayToInterface);

	//InitGateway
	if(m_pGatewayToInterface)
	{
		if(m_pGatewayToInterface->InitGateway())
        {
            InitParameterSet();
			InitCommands((CGateway*)m_pGatewayToInterface);
            oResult = TRUE;
        }
        else
        {
            DeleteGatewayToInterface();
        }
	}

	return oResult;
}

BOOL CProtocolStack_MaxonSerialV2::InitGatewayToDevice(CStdString strDeviceName)
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

BOOL CProtocolStack_MaxonSerialV2::InitParameterSet()
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

BOOL CProtocolStack_MaxonSerialV2::InitErrorHandling()
{
    BOOL oResult = FALSE;
    CErrorProducer errorProducer;
    CStdString strClassName = "ProtocolStack_MaxonSerialV2";

    if(m_pErrorHandling)
    {
        //Init error producer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        oResult = m_pErrorHandling->InitErrorProducer(&errorProducer);
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV2::InitProtocolStack(CErrorInfo* pErrorInfo)
{
    BOOL oResult = TRUE;
    std::list<CStdString> rTargets;
    oResult = CProtocolStackMaxonSerialV2Factory::GetAllIdentifiers(rTargets);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=rTargets.begin(); it!=rTargets.end() && oResult; it++)
        {
            if(!InitProtocolStack(*it))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV2::InitProtocolStack(CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CInterfaceManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strInterfaceName.IsEmpty())
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
                //Interface already existing
                oResult = TRUE;
            }
            else
            {
                //Interface not existing
                oResult = TRUE;
            }

            //Init PlugIn Manager
            if(m_pPlugInManager) m_pPlugInManager->RegisterInterface(strInterfaceName);

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

BOOL CProtocolStack_MaxonSerialV2::InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStackName, CErrorInfo* pErrorInfo)
{
	CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strDeviceName.IsEmpty())
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

BOOL CProtocolStack_MaxonSerialV2::VerifyGatewayToInterface(CStdString strInterfaceName)
{
    BOOL oResult = FALSE;

    //GatewayMaxonSerialV2ToUSB
    if(!oResult && strInterfaceName.CompareNoCase(INTERFACE_USB) == 0)
    {
        oResult = TRUE;
    }
    
    //GatewayMaxonSerialV2ToHID
    if(!oResult && strInterfaceName.CompareNoCase(INTERFACE_HID) == 0)
    {
        oResult = TRUE;
    }

    //GatewayMaxonSerialV2ToMCP
    if(!oResult && strInterfaceName.CompareNoCase(INTERFACE_MCP) == 0)
    {
        oResult = TRUE;
    }

    //GatewayMaxonSerialV2ToRS232
    if(!oResult && strInterfaceName.CompareNoCase(INTERFACE_RS232) == 0)
    {
        oResult = TRUE;
    }

    //GatewayMaxonSerialV2ToIPC
    if(!oResult && strInterfaceName.CompareNoCase(INTERFACE_IPC) == 0)
    {
        oResult = TRUE;
    }

    return oResult;
}

BOOL CProtocolStack_MaxonSerialV2::VerifyGatewayToDevice(CStdString strDeviceName)
{
    //No Gateway supported
    return FALSE;
}

BOOL CProtocolStack_MaxonSerialV2::PS_ProcessProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uOpCode, void* pDataBuffer, DWORD dDataBufferLength, BYTE ubKeepLock, void** ppRetDataBuffer, DWORD* pdRetDataBufferLength, CErrorInfo* pErrorInfo)
{
    BYTE uLen = 0; //Calculated internally
    WORD wCrc = 0; //Calculated internally

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

BOOL CProtocolStack_MaxonSerialV2::PS_AbortProtocol(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, CErrorInfo* pErrorInfo)
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

void CProtocolStack_MaxonSerialV2::InitCommands(CGateway* pGateway)
{
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->InitGateway(pGateway);
	if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->InitGateway(pGateway);
}

void CProtocolStack_MaxonSerialV2::InitCommands()
{
    DeleteCommands();

    //Process Protocol
    m_pCommand_ProcessProtocol = new CCommand_PS_MaxonSerialV2();
    m_pCommand_ProcessProtocol->InitCommand(MAXON_SERIAL_V2_PROCESS_PROTOCOL);

	//Abort Protocol
    m_pCommand_AbortProtocol = new CCommand_PS_MaxonSerialV2();
    m_pCommand_AbortProtocol->InitCommand(MAXON_SERIAL_V2_ABORT_PROTOCOL);
}

BOOL CProtocolStack_MaxonSerialV2::GetCommands(CStdString* pCommandInfo)
{
    BOOL oResult = FALSE;
    CXXMLFile xmlFile;
    CXXMLFile::CElementPart* pElementPart;
    CXXMLFile::CElement* pElement;

    if(pCommandInfo)
    {
        //Root
        pElementPart = xmlFile.Root();
        if(xmlFile.IsElement(pElementPart))
        {
            //Init
            oResult = TRUE;

            //CommandInfo elements
            pElement = (CXXMLFile::CElement*)xmlFile.AddElement(pElementPart);
            xmlFile.SetText(pElement, "CommandInfo");

            //CommandGroup elements
            pElement = (CXXMLFile::CElement*)xmlFile.AddElement(pElement);
            xmlFile.SetText(pElement, "CommandGroup");
            pElement->AttributeToValue["Name"] = m_strProtocolStackName;

            //Command elements
            if(oResult &&m_pCommand_ProcessProtocol && !m_pCommand_ProcessProtocol->StoreToXMLFile(&xmlFile, pElement, TRUE)) oResult = FALSE;
		    if(oResult && m_pCommand_AbortProtocol && !m_pCommand_AbortProtocol->StoreToXMLFile(&xmlFile, pElement, TRUE)) oResult = FALSE;

            //Write to string
            if(oResult) xmlFile.WriteToString(pCommandInfo);
        }
    }

    return oResult;
}

void CProtocolStack_MaxonSerialV2::DeleteCommands()
{
    if(m_pCommand_ProcessProtocol)
    {
        delete m_pCommand_ProcessProtocol;
        m_pCommand_ProcessProtocol = 0;
    }

	if(m_pCommand_AbortProtocol)
    {
        delete m_pCommand_AbortProtocol;
        m_pCommand_AbortProtocol = 0;
    }
}

void CProtocolStack_MaxonSerialV2::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CProtocolStackBase::InitJournalManager(pJournalManager);
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->InitJournalManager(pJournalManager);
	if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->InitJournalManager(pJournalManager);
}

void CProtocolStack_MaxonSerialV2::ResetJournalManager()
{
    CProtocolStackBase::ResetJournalManager();
    if(m_pCommand_ProcessProtocol) m_pCommand_ProcessProtocol->ResetJournalManager();
	if(m_pCommand_AbortProtocol) m_pCommand_AbortProtocol->ResetJournalManager();
}
