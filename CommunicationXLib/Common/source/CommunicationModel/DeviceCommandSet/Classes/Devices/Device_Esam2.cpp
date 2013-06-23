// Device_Esam2.cpp: Implementierung der Klasse CDevice_Esam2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Device_Esam2.h"

#include <algorithm>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>
#include "../Gateway/Esam2/GatewayEsam2ToMaxonSerialV2.h"
#include "../Gateway/Esam2/GatewayEsam2ToCANopen.h"
#include <CommunicationModel/ProtocolStack/ProtocolStackManager.h>
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CDevice_Esam2::CDevice_Esam2()
{
    InitErrorHandling();

    FillGroupList();
    m_strDeviceName = DEVICE_ESAM2;
    m_pJournalManager = 0;
}

CDevice_Esam2::CDevice_Esam2(const CDevice_Esam2& rObject):CDeviceBase(rObject)
{
    InitErrorHandling();

    FillGroupList();
    m_strDeviceName = rObject.m_strDeviceName;

    InitJournalManager(rObject.m_pJournalManager);
    InitGroupList(m_pGateway);
}

CDeviceBase* CDevice_Esam2::Clone()
{
    CDevice_Esam2* pDevice;

    pDevice = new CDevice_Esam2(*this);
    return pDevice;
}

CDevice_Esam2::~CDevice_Esam2()
{
    DeleteGroupList();
}

BOOL CDevice_Esam2::InitDevice(CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    //Klassenstruktur für Protocolstack Epos2USB aufbauen
    if(!InitDevice(PROTOCOL_MAXON_SERIAL_V2, pErrorInfo))
    {
        oResult = FALSE;
    }

    //Klassenstruktur für Protocolstack CANopen aufbauen
    if(!InitDevice(PROTOCOL_CAN_OPEN, pErrorInfo))
    {
        oResult = FALSE;
    }

    return oResult;
}

BOOL CDevice_Esam2::InitDevice(CStdString strProtocolStack, CErrorInfo* pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strProtocolStack.size() == 0)
    {
        //Don't create a protocol stack
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(strProtocolStack, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CProtocolStackManagerBase*) new CProtocolStackManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

        //Klassenstruktur für Device aufbauen
        if((VerifyGateway(strProtocolStack)) && (pManager->PS_InitProtocolStack(strProtocolStack, pErrorInfo)))
        {
		    //Default Settings
			CDeviceBase::InitDefaultProtocolStackSettings(pManager, strProtocolStack);

            if(oNewCreated) m_ProtocolStackManagerList.push_back(pManager);
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

BOOL CDevice_Esam2::InitDevice(CStdString strProtocolStack, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strProtocolStack.size() == 0)
    {
        //Don't create a protocol stack
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(strProtocolStack, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CProtocolStackManagerBase*) new CProtocolStackManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

        //Klassenstruktur für Device aufbauen
        if((VerifyGateway(strProtocolStack)) && (pManager->PS_InitProtocolStack(strProtocolStack, strInterfaceName, pErrorInfo)))
        {
		    //Default Settings
			InitDefaultProtocolStackSettings(pManager, strProtocolStack, strInterfaceName);

            if(oNewCreated) m_ProtocolStackManagerList.push_back(pManager);
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

BOOL CDevice_Esam2::InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName)
{
	BOOL oResult(FALSE);

	if(p_pProtocolStackManager)
	{
		if(p_ProtocolStackName == PROTOCOL_CAN_OPEN)
		{
			//CANopen
			oResult = p_pProtocolStackManager->PS_InitDefaultProtocolStackSettings(p_ProtocolStackName, p_InterfaceName, DEFAULT_TRANSFER_RATE_CAN_OPEN, DEFAULT_TIMEOUT);
		}
		else if(p_InterfaceName == INTERFACE_RS232)
		{
			//RS232
			oResult = p_pProtocolStackManager->PS_InitDefaultProtocolStackSettings(p_ProtocolStackName, p_InterfaceName, DEFAULT_TRANSFER_RATE_RS232, DEFAULT_TIMEOUT);
		}
		else if(p_InterfaceName == INTERFACE_USB)
		{
			//USB
			oResult = p_pProtocolStackManager->PS_InitDefaultProtocolStackSettings(p_ProtocolStackName, p_InterfaceName, DEFAULT_TRANSFER_RATE_USB, DEFAULT_TIMEOUT);
		}
	}

	return oResult;
}

void CDevice_Esam2::FillGroupList()
{
    m_pCommandGroupStandard = new CCommandGroupStandard_DCS_Esam2();
}

BOOL CDevice_Esam2::InitGateway(CStdString strProtocolStackName)
{
    DeleteGateway();

    //Überprüfen welcher Gateway initialisiert werden soll!
    if(strProtocolStackName.CompareNoCase(PROTOCOL_CAN_OPEN) == 0)
    {
        m_pGateway = new CGatewayEsam2ToCANopen();
        if(m_pGateway->InitGateway())
        {
            InitParameterSet();
			InitGroupList(m_pGateway);
            return TRUE;
        }
        else
        {
            DeleteGateway();
            return FALSE;
        }
    }

    if(strProtocolStackName.CompareNoCase(PROTOCOL_MAXON_SERIAL_V2) == 0)
    {
        m_pGateway = new CGatewayEsam2ToMaxonSerialV2();
        if(m_pGateway->InitGateway())
        {
            InitParameterSet();
			InitGroupList(m_pGateway);
            return TRUE;
        }
        else
        {
            DeleteGateway();
            return FALSE;
        }
    }

    return FALSE;
}

BOOL CDevice_Esam2::InitParameterSet()
{
	const BYTE DEFAULT_NODE_ID = 1;
	const BYTE DEFAULT_REMOTE_NODE_ID = 0;
	const WORD DEFAULT_REMOTE_NETWORK_ID = 0;

	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
		if(oResult) oResult &= m_pGateway->AddParameter(_T("NodeId"), (BYTE*)&DEFAULT_NODE_ID, sizeof(DEFAULT_NODE_ID));
		if(oResult) oResult &= m_pGateway->AddParameter(_T("RemoteNetworkId"), (BYTE*)&DEFAULT_REMOTE_NETWORK_ID, sizeof(DEFAULT_REMOTE_NETWORK_ID));
		if(oResult) oResult &= m_pGateway->AddParameter(_T("RemoteNodeId"), (BYTE*)&DEFAULT_REMOTE_NODE_ID, sizeof(DEFAULT_REMOTE_NODE_ID));

	}

    return oResult;
}

BOOL CDevice_Esam2::InitGroupList(CGatewayDCStoPS* pGateway)
{
    if(m_pCommandGroupStandard && !m_pCommandGroupStandard->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

BOOL CDevice_Esam2::VerifyGateway(CStdString strProtocolStackName)
{
    //Überprüfen welcher ProtocolStack benutzt werden darf

    //GatewayEsam2ToCANopen
    if(strProtocolStackName.CompareNoCase(PROTOCOL_CAN_OPEN) == 0)
    {
        return TRUE;
    }

    //GatewayEsam2ToMaxonSerialV2
    if(strProtocolStackName.CompareNoCase(PROTOCOL_MAXON_SERIAL_V2) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

void CDevice_Esam2::DeleteGroupList()
{
    if(m_pCommandGroupStandard)
    {
        delete m_pCommandGroupStandard;
        m_pCommandGroupStandard = 0;
    }
}

void CDevice_Esam2::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    CDeviceBase::InitJournalManager(pJournalManager);
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->InitJournalManager(pJournalManager);
}

void CDevice_Esam2::ResetJournalManager()
{
    CDeviceBase::ResetJournalManager();
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->ResetJournalManager();
}

BOOL CDevice_Esam2::GetCommands(CStdString* pCommandInfo)
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
        if(m_pCommandGroupStandard)
        {
            if(!m_pCommandGroupStandard->StoreToXMLFile(&xmlFile, pElement)) oResult = FALSE;
        }

        //Write to string
        xmlFile.WriteToString(pCommandInfo);
        return oResult;
    }

    return FALSE;
}

BOOL CDevice_Esam2::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "DeviceEsam2";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

