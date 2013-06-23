// Device_Esam.cpp: Implementierung der Klasse CDevice_Esam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Device_Esam.h"

#include <algorithm>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>
#include "../Gateway/Esam/GatewayEsamToInfoteamSerial.h"
#include "../Gateway/Esam/GatewayEsamToCANopen.h"
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
CDevice_Esam::CDevice_Esam()
{
    InitErrorHandling();

    FillGroupList();
    m_strDeviceName = DEVICE_ESAM;
    m_pJournalManager = 0;
}

CDevice_Esam::CDevice_Esam(const CDevice_Esam& rObject):CDeviceBase(rObject)
{
    InitErrorHandling();

    FillGroupList();
    m_strDeviceName = rObject.m_strDeviceName;

    InitJournalManager(rObject.m_pJournalManager);
    InitGroupList(m_pGateway);
}

CDeviceBase* CDevice_Esam::Clone()
{
    CDevice_Esam* pDevice;

    pDevice = new CDevice_Esam(*this);
    return pDevice;
}

CDevice_Esam::~CDevice_Esam()
{
    DeleteGroupList();
}

BOOL CDevice_Esam::InitDevice(CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    //Klassenstruktur für Protocolstack EsamRS232 aufbauen
    if(!InitDevice(PROTOCOL_INFOTEAM_SERIAL, pErrorInfo))
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

BOOL CDevice_Esam::InitDevice(CStdString strProtocolStack, CErrorInfo* pErrorInfo)
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

BOOL CDevice_Esam::InitDevice(CStdString strProtocolStack, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
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

BOOL CDevice_Esam::InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName)
{
	BOOL oResult(FALSE);

	if(p_pProtocolStackManager)
	{
		if(p_InterfaceName == INTERFACE_RS232)
		{
			//RS232
			oResult = p_pProtocolStackManager->PS_InitDefaultProtocolStackSettings(p_ProtocolStackName, p_InterfaceName, DEFAULT_TRANSFER_RATE_RS232, DEFAULT_TIMEOUT);
		}
	}

	return oResult;
}

void CDevice_Esam::FillGroupList()
{
    m_pCommandGroupStandard = new CCommandGroupStandard_DCS_Esam();
}

BOOL CDevice_Esam::InitGateway(CStdString strProtocolStackName)
{
    DeleteGateway();

    //Überprüfen welcher Gateway initialisiert werden soll!
    if(strProtocolStackName.CompareNoCase(PROTOCOL_CAN_OPEN) == 0)
    {
        m_pGateway = new CGatewayEsamToCANopen();
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

    if(strProtocolStackName.CompareNoCase(PROTOCOL_INFOTEAM_SERIAL) == 0)
    {
        m_pGateway = new CGatewayEsamToInfoteamSerial();
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

BOOL CDevice_Esam::InitParameterSet()
{
	const BYTE DEFAULT_NODE_ID = 1;
	const BYTE DEFAULT_PORT_NUMBER = 1;

	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
		if(oResult) oResult &= m_pGateway->AddParameter(_T("NodeId"), (BYTE*)&DEFAULT_NODE_ID, sizeof(DEFAULT_NODE_ID));
		if(oResult) oResult &= m_pGateway->AddParameter(_T("PortNumber"), (BYTE*)&DEFAULT_PORT_NUMBER, sizeof(DEFAULT_PORT_NUMBER));
	}

    return oResult;
}

BOOL CDevice_Esam::InitGroupList(CGatewayDCStoPS* pGateway)
{
    if(m_pCommandGroupStandard && !m_pCommandGroupStandard->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

BOOL CDevice_Esam::VerifyGateway(CStdString strProtocolStackName)
{
    //Überprüfen welcher ProtocolStack benutzt werden darf

    //GatewayEposToCANopen
    if(strProtocolStackName.CompareNoCase(PROTOCOL_CAN_OPEN) == 0)
    {
        return TRUE;
    }

    //GatewayEsamToInfoteamSerial
    if(strProtocolStackName.CompareNoCase(PROTOCOL_INFOTEAM_SERIAL) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

void CDevice_Esam::DeleteGroupList()
{
    if(m_pCommandGroupStandard)
    {
        delete m_pCommandGroupStandard;
        m_pCommandGroupStandard = 0;
    }
}

void CDevice_Esam::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    CDeviceBase::InitJournalManager(pJournalManager);
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->InitJournalManager(pJournalManager);
}

void CDevice_Esam::ResetJournalManager()
{
    CDeviceBase::ResetJournalManager();
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->ResetJournalManager();
}

BOOL CDevice_Esam::GetCommands(CStdString* pCommandInfo)
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

BOOL CDevice_Esam::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "DeviceEsam";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

