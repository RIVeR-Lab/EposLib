// DRIVE_Plc2.cpp: Implementierung der Klasse CVirtualDevice_Plc2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VirtualDevice_Plc2.h"

#include <algorithm>
#include "../../../DeviceCommandSet/Classes/Devices/Device_Esam2.h"
#include "../../../DeviceCommandSet/DeviceCommandSetManager.h"
#include "../CommandGroups/Plc2/CommandGroupStandard_VCS_Plc2.h"
#include "../Gateway/Plc2/GatewayPlc2ToEsam2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CVirtualDevice_Plc2::CVirtualDevice_Plc2()
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = VIRTUAL_DEVICE_PLC2;
}

CVirtualDevice_Plc2::CVirtualDevice_Plc2(const CVirtualDevice_Plc2& rObject)
:	CVirtualDeviceBase(rObject)
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = rObject.m_strVirtualDeviceName;

    InitGroupList(m_pGateway);
    InitJournalManager(rObject.m_pJournalManager);
}

CVirtualDevice_Plc2::~CVirtualDevice_Plc2()
{
    DeleteGroupList();
}

BOOL CVirtualDevice_Plc2::InitVirtualDevice(CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

    //Klassenstruktur für Device Esam2 aufbauen
    if(!InitVirtualDevice(DEVICE_ESAM2, pErrorInfo))
    {
        oResult = FALSE;
    }

    return oResult;
}

BOOL CVirtualDevice_Plc2::InitVirtualDevice(CStdString strDeviceName, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult(FALSE);

    if(strDeviceName.IsEmpty())
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

BOOL CVirtualDevice_Plc2::InitVirtualDevice(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult(FALSE);

    if(strDeviceName.IsEmpty())
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

BOOL CVirtualDevice_Plc2::VerifyGateway(CStdString strDeviceName)
{
    //Überprüfen welches Gerät benutzt werden darf

    //GatewayPlc2ToEsam2
    if(strDeviceName.CompareNoCase(DEVICE_ESAM2) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualDevice_Plc2::InitErrorHandling()
{
    CErrorProducer errorProducer;

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, VIRTUAL_DEVICE_PLC2);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualDevice_Plc2::InitGateway(CStdString strDeviceName)
{
    DeleteGateway();

    //Überprüfen ob Esam2 Gateway
    if(strDeviceName.CompareNoCase(DEVICE_ESAM2) == 0)
    {
        m_pGateway = new CGatewayPlc2ToEsam2();
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

BOOL CVirtualDevice_Plc2::InitParameterSet()
{
	BOOL oResult(FALSE);

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
	}

    return oResult;
}

BOOL CVirtualDevice_Plc2::InitGroupList(CGateway* pGateway)
{
    if(m_pCommandGroupStandard && !m_pCommandGroupStandard->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

void CVirtualDevice_Plc2::FillGroupList()
{
    m_pCommandGroupStandard = new CCommandGroupStandard_VCS_Plc2();
}

void CVirtualDevice_Plc2::DeleteGroupList()
{
    if(m_pCommandGroupStandard)
    {
        delete m_pCommandGroupStandard;
        m_pCommandGroupStandard = 0;
    }
}

BOOL CVirtualDevice_Plc2::GetCommands(CStdString* pCommandInfo)
{
    CXXMLFile xmlFile;
    CXXMLFile::CElementPart* pElementPart;
    CXXMLFile::CElement* pElement;
    BOOL oResult(TRUE);

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
        //xmlFile.SetFile("C:\\Commands.xml");
        //xmlFile.Write();
        return oResult;
    }

    return FALSE;
}

void CVirtualDevice_Plc2::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CVirtualDeviceBase::InitJournalManager(pJournalManager);
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->InitJournalManager(pJournalManager);
}

void CVirtualDevice_Plc2::ResetJournalManager()
{
    CVirtualDeviceBase::ResetJournalManager();
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->ResetJournalManager();
}

CVirtualDeviceBase* CVirtualDevice_Plc2::Clone()
{
    CVirtualDevice_Plc2* pDevice;

    pDevice = new CVirtualDevice_Plc2(*this);
    return pDevice;
}

