// DRIVE_Plc.cpp: Implementierung der Klasse CVirtualDevice_Plc.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VirtualDevice_Plc.h"

#include <algorithm>

#include "../../../DeviceCommandSet/Classes/Devices/Device_Esam.h"
#include "../../../DeviceCommandSet/DeviceCommandSetManager.h"
#include "../CommandGroups/Plc/CommandGroupStandard_VCS_Plc.h"
#include "../Gateway/Plc/GatewayPlcToEsam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CVirtualDevice_Plc::CVirtualDevice_Plc()
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = VIRTUAL_DEVICE_PLC;
}

CVirtualDevice_Plc::CVirtualDevice_Plc(const CVirtualDevice_Plc& rObject)
:	CVirtualDeviceBase(rObject)
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = rObject.m_strVirtualDeviceName;

    InitGroupList(m_pGateway);
    InitJournalManager(rObject.m_pJournalManager);
}

CVirtualDevice_Plc::~CVirtualDevice_Plc()
{
    DeleteGroupList();
}

BOOL CVirtualDevice_Plc::InitVirtualDevice(CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    //Klassenstruktur für Device Esam aufbauen
    if(!InitVirtualDevice(DEVICE_ESAM, pErrorInfo))
    {
        oResult = FALSE;
    }

    return oResult;
}

BOOL CVirtualDevice_Plc::InitVirtualDevice(CStdString strDeviceName, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strDeviceName.size() == 0)
    {
        //Don't create a device
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
        if((VerifyGateway(strDeviceName)) && (pManager->DCS_InitDevice(strDeviceName, pErrorInfo)))
        {
            if(oNewCreated) m_DeviceCommandSetManagerList.push_back(pManager);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated)delete pManager;
            oResult = FALSE;
        }
    }
    return oResult;
}

BOOL CVirtualDevice_Plc::InitVirtualDevice(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strDeviceName.size() == 0)
    {
        //Don't create a device
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
        if((VerifyGateway(strDeviceName)) && (pManager->DCS_InitDevice(strDeviceName, strProtocolStackName, strInterfaceName, pErrorInfo)))
        {
            if(oNewCreated) m_DeviceCommandSetManagerList.push_back(pManager);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated)delete pManager;
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CVirtualDevice_Plc::VerifyGateway(CStdString strDeviceName)
{
    //Überprüfen welches Gerät benutzt werden darf

    //GatewayVirtualDeviceToEsam
    if(strDeviceName.CompareNoCase(DEVICE_ESAM) == 0)
        return TRUE;

    return FALSE;
}

BOOL CVirtualDevice_Plc::InitErrorHandling()
{
    CErrorProducer errorProducer;

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, VIRTUAL_DEVICE_PLC);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualDevice_Plc::InitGateway(CStdString strDeviceName)
{
    DeleteGateway();

    //Überprüfen ob Esam Gateway
    if(strDeviceName.CompareNoCase(DEVICE_ESAM) == 0)
    {
        m_pGateway = new CGatewayPlcToEsam();
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

BOOL CVirtualDevice_Plc::InitParameterSet()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
	}

    return oResult;
}

BOOL CVirtualDevice_Plc::InitGroupList(CGateway* pGateway)
{
    if(m_pCommandGroupStandard && !m_pCommandGroupStandard->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

void CVirtualDevice_Plc::FillGroupList()
{
    m_pCommandGroupStandard = new CCommandGroupStandard_VCS_Plc();
}

void CVirtualDevice_Plc::DeleteGroupList()
{
    if(m_pCommandGroupStandard)
    {
        delete m_pCommandGroupStandard;
        m_pCommandGroupStandard = 0;
    }
}

BOOL CVirtualDevice_Plc::GetCommands(CStdString* pCommandInfo)
{
    BOOL oResult = TRUE;

    if(pCommandInfo)
    {
    	CXXMLFile xmlFile;
        //Root
    	CXXMLFile::CElementPart* pElementPart = xmlFile.Root();
        if(!xmlFile.IsElement(pElementPart)) return FALSE;

        //CommandInfo Elements
        CXXMLFile::CElement* pElement = (CXXMLFile::CElement*)xmlFile.AddElement(pElementPart);
        xmlFile.SetText(pElement, "CommandInfo");

        //CommandGroup Elements
        if(m_pCommandGroupStandard)
        {
            if(!m_pCommandGroupStandard->StoreToXMLFile(&xmlFile, pElement)) oResult = FALSE;
        }

        //Write to string
        xmlFile.WriteToString(pCommandInfo);
        //xmlFile.SetFile("C:\\Commands.xml");
        //xmlFile.Write();

        return oResult;
    }

    return FALSE;
}

void CVirtualDevice_Plc::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CVirtualDeviceBase::InitJournalManager(pJournalManager);
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->InitJournalManager(pJournalManager);
}

void CVirtualDevice_Plc::ResetJournalManager()
{
    CVirtualDeviceBase::ResetJournalManager();
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->ResetJournalManager();
}

CVirtualDeviceBase* CVirtualDevice_Plc::Clone()
{
    CVirtualDevice_Plc* pDevice;

    pDevice = new CVirtualDevice_Plc(*this);
    return pDevice;
}

