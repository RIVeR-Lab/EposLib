// Device_Epos2.cpp: Implementierung der Klasse CDevice_Epos2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Device_Epos2.h"
#include <algorithm>

#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEpos2Def.h>
#include "DeviceEpos2Factory.h"
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
CDevice_Epos2::CDevice_Epos2()
{
    InitErrorHandling();

    FillGroupList();
    m_strDeviceName = DEVICE_EPOS2;
    m_pJournalManager = 0;
}

CDevice_Epos2::CDevice_Epos2(const CDevice_Epos2& rObject):CDeviceBase(rObject)
{
    InitErrorHandling();

    FillGroupList();
    m_strDeviceName = rObject.m_strDeviceName;

    InitJournalManager(rObject.m_pJournalManager);
    InitGroupList(m_pGateway);
}

CDeviceBase* CDevice_Epos2::Clone()
{
    CDevice_Epos2* pDevice;

    pDevice = new CDevice_Epos2(*this);
    return pDevice;
}

CDevice_Epos2::~CDevice_Epos2()
{
    DeleteGroupList();
}

BOOL CDevice_Epos2::InitDevice(CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    std::list<CStdString> aIdentifiers;    
    oResult = CDeviceEpos2Factory::GetAllIdentifiers(aIdentifiers);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=aIdentifiers.begin(); it!=aIdentifiers.end() && oResult; it++)
        {
             if(!InitDevice((*it), pErrorInfo))
             {
                oResult = FALSE;
             }
        }
    }

    return oResult;
}

BOOL CDevice_Epos2::InitDevice(CStdString strProtocolStack, CErrorInfo* pErrorInfo)
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

BOOL CDevice_Epos2::InitDevice(CStdString strProtocolStack, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
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

BOOL CDevice_Epos2::InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName)
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

void CDevice_Epos2::FillGroupList()
{
    m_pCommandGroupStandard = new CCommandGroupStandard_DCS_Epos2();
}

BOOL CDevice_Epos2::InitGateway(CStdString strProtocolStackName)
{
    DeleteGateway();

    BOOL oResult = CDeviceEpos2Factory::CreateInstance(strProtocolStackName, m_pGateway);

    if(oResult)
    {
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

BOOL CDevice_Epos2::InitParameterSet()
{
	const BYTE DEFAULT_NODE_ID = 1;

	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
		if(oResult) oResult &= m_pGateway->AddParameter(_T("NodeId"), (BYTE*)&DEFAULT_NODE_ID, sizeof(DEFAULT_NODE_ID));
	}

    return oResult;
}

BOOL CDevice_Epos2::InitGroupList(CGatewayDCStoPS* pGateway)
{
    if(m_pCommandGroupStandard && !m_pCommandGroupStandard->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

BOOL CDevice_Epos2::VerifyGateway(CStdString strProtocolStackName)
{
    std::list<CStdString> aIdentifiers;
    
    BOOL oResult = CDeviceEpos2Factory::GetAllIdentifiers(aIdentifiers);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=aIdentifiers.begin(); it!=aIdentifiers.end() && oResult; it++)
        {
            if(strProtocolStackName.CompareNoCase((*it)) == 0)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

void CDevice_Epos2::DeleteGroupList()
{
    if(m_pCommandGroupStandard)
    {
        delete m_pCommandGroupStandard;
        m_pCommandGroupStandard = 0;
    }
}

void CDevice_Epos2::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    CDeviceBase::InitJournalManager(pJournalManager);
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->InitJournalManager(pJournalManager);
}

void CDevice_Epos2::ResetJournalManager()
{
    CDeviceBase::ResetJournalManager();
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->ResetJournalManager();
}

BOOL CDevice_Epos2::GetCommands(CStdString* pCommandInfo)
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

BOOL CDevice_Epos2::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "Device_Epos2";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

