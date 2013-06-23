// DRIVE_Drive.cpp: Implementierung der Klasse CVirtualDevice_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VirtualDevice_Drive.h"

#include <algorithm>

#include "../../../DeviceCommandSet/Classes/Devices/Device_Epos.h"
#include "../../../DeviceCommandSet/DeviceCommandSetManager.h"

#include "../CommandGroups/Drive/CommandGroupStandard_VCS_Drive.h"
#include "VirtualDeviceDriveFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CVirtualDevice_Drive::CVirtualDevice_Drive()
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE;
}

CVirtualDevice_Drive::CVirtualDevice_Drive(const CVirtualDevice_Drive& rObject):CVirtualDeviceBase(rObject)
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = rObject.m_strVirtualDeviceName;

    InitGroupList(m_pGateway);
    InitJournalManager(rObject.m_pJournalManager);
}

CVirtualDevice_Drive::~CVirtualDevice_Drive()
{
    DeleteGroupList();
}

BOOL CVirtualDevice_Drive::InitVirtualDevice(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    std::list<CStdString> aIdentifiers;    
    oResult = CVirtualDeviceDriveFactory::GetAllIdentifiers(aIdentifiers);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=aIdentifiers.begin(); it!=aIdentifiers.end() && oResult; it++)
        {
            if(!InitVirtualDevice(*it, p_pErrorInfo))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CVirtualDevice_Drive::InitVirtualDevice(CStdString strDeviceName, CErrorInfo* pErrorInfo)
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

BOOL CVirtualDevice_Drive::InitVirtualDevice(CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* pErrorInfo)
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

BOOL CVirtualDevice_Drive::VerifyGateway(CStdString p_DeviceName)
{
    std::list<CStdString> aIdentifiers;
    
    BOOL oResult = CVirtualDeviceDriveFactory::GetAllIdentifiers(aIdentifiers);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=aIdentifiers.begin(); it!=aIdentifiers.end() && oResult; it++)
        {
            if(p_DeviceName.CompareNoCase((*it)) == 0)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CVirtualDevice_Drive::InitErrorHandling()
{
    CErrorProducer errorProducer;

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, VIRTUAL_DEVICE_DRIVE);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualDevice_Drive::InitGateway(CStdString p_DeviceName)
{
    DeleteGateway();

    BOOL oResult = CVirtualDeviceDriveFactory::CreateInstance(p_DeviceName, m_pGateway);

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

BOOL CVirtualDevice_Drive::InitParameterSet()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
	}

    return oResult;
}

BOOL CVirtualDevice_Drive::InitGroupList(CGateway* pGateway)
{
    if(m_pCommandGroupStandard && !m_pCommandGroupStandard->InitGateway(pGateway)) return FALSE;
    return TRUE;
}

void CVirtualDevice_Drive::FillGroupList()
{
    m_pCommandGroupStandard = new CCommandGroupStandard_VCS_Drive();
}

void CVirtualDevice_Drive::DeleteGroupList()
{
    if(m_pCommandGroupStandard)
    {
        delete m_pCommandGroupStandard;
        m_pCommandGroupStandard = 0;
    }
}

BOOL CVirtualDevice_Drive::GetCommands(CStdString* pCommandInfo)
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

void CVirtualDevice_Drive::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CVirtualDeviceBase::InitJournalManager(pJournalManager);
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->InitJournalManager(pJournalManager);
}

void CVirtualDevice_Drive::ResetJournalManager()
{
    CVirtualDeviceBase::ResetJournalManager();
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->ResetJournalManager();
}

CVirtualDeviceBase* CVirtualDevice_Drive::Clone()
{
    CVirtualDevice_Drive* pDevice;

    pDevice = new CVirtualDevice_Drive(*this);
    return pDevice;
}

