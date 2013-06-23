// VirtualDevice_Drive2.cpp: Implementierung der Klasse CVirtualDevice_Drive2.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VirtualDevice_Drive2.h"

#include <algorithm>

#include "../../../DeviceCommandSet/DeviceCommandSetManager.h"
#include "../CommandGroups/Drive2/CommandGroupStandard_VCS_Drive2.h"
#include "VirtualDeviceDrive2Factory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CVirtualDevice_Drive2::CVirtualDevice_Drive2()
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE2;
}

CVirtualDevice_Drive2::CVirtualDevice_Drive2(const CVirtualDevice_Drive2& rObject):CVirtualDeviceBase(rObject)
{
    InitErrorHandling();

    FillGroupList();
    m_strVirtualDeviceName = rObject.m_strVirtualDeviceName;

    InitGroupList(m_pGateway);
    InitJournalManager(rObject.m_pJournalManager);
}

CVirtualDevice_Drive2::~CVirtualDevice_Drive2()
{
    DeleteGroupList();
}

BOOL CVirtualDevice_Drive2::InitVirtualDevice(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    std::list<CStdString> aIdentifiers;
    
    oResult = CVirtualDeviceDrive2Factory::GetAllIdentifiers(aIdentifiers);

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

BOOL CVirtualDevice_Drive2::InitVirtualDevice(CStdString p_DeviceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated(FALSE);
    BOOL oResult(FALSE);

    if(p_DeviceName.IsEmpty())
    {
        //Don't create a device
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(p_DeviceName, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CDeviceCommandSetManagerBase*) new CDeviceCommandSetManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

        //Klassenstruktur für Device aufbauen
        if((VerifyGateway(p_DeviceName)) && (pManager->DCS_InitDevice(p_DeviceName, p_pErrorInfo)))
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

BOOL CVirtualDevice_Drive2::InitVirtualDevice(CStdString p_DeviceName, CStdString p_ProtocolStackName, CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated(FALSE);
    BOOL oResult(FALSE);

    if(p_DeviceName.IsEmpty())
    {
        //Don't create a device
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(p_DeviceName, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CDeviceCommandSetManagerBase*) new CDeviceCommandSetManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

        //Klassenstruktur für Device aufbauen
        if((VerifyGateway(p_DeviceName)) && (pManager->DCS_InitDevice(p_DeviceName, p_ProtocolStackName, p_InterfaceName, p_pErrorInfo)))
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

BOOL CVirtualDevice_Drive2::VerifyGateway(CStdString p_DeviceName)
{
    std::list<CStdString> aIdentifiers;
    
    BOOL oResult = CVirtualDeviceDrive2Factory::GetAllIdentifiers(aIdentifiers);

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

BOOL CVirtualDevice_Drive2::InitErrorHandling()
{
    BOOL oResult(FALSE);
    CErrorProducer errorProducer;

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, VIRTUAL_DEVICE_DRIVE2);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CVirtualDevice_Drive2::InitGateway(CStdString p_DeviceName)
{
    DeleteGateway();

    BOOL oResult = CVirtualDeviceDrive2Factory::CreateInstance(p_DeviceName, m_pGateway);

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

BOOL CVirtualDevice_Drive2::InitParameterSet()
{
	BOOL oResult(FALSE);

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
	}

    return oResult;
}

BOOL CVirtualDevice_Drive2::InitGroupList(CGateway* pGateway)
{
    if(m_pCommandGroupStandard && !m_pCommandGroupStandard->InitGateway(pGateway)) return TRUE;
    return FALSE;
}

void CVirtualDevice_Drive2::FillGroupList()
{
    m_pCommandGroupStandard = new CCommandGroupStandard_VCS_Drive2();
}

void CVirtualDevice_Drive2::DeleteGroupList()
{
    if(m_pCommandGroupStandard)
    {
        delete m_pCommandGroupStandard;
        m_pCommandGroupStandard = 0;
    }
}

BOOL CVirtualDevice_Drive2::GetCommands(CStdString* pCommandInfo)
{
    BOOL oResult = FALSE;

    if(pCommandInfo)
    {
    	CXXMLFile xmlFile;
        oResult = TRUE;

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
    }

    return oResult;
}

void CVirtualDevice_Drive2::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CVirtualDeviceBase::InitJournalManager(pJournalManager);
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->InitJournalManager(pJournalManager);
}

void CVirtualDevice_Drive2::ResetJournalManager()
{
    CVirtualDeviceBase::ResetJournalManager();
    if(m_pCommandGroupStandard) m_pCommandGroupStandard->ResetJournalManager();
}

CVirtualDeviceBase* CVirtualDevice_Drive2::Clone()
{
    CVirtualDevice_Drive2* pDevice;

    pDevice = new CVirtualDevice_Drive2(*this);
    return pDevice;
}


