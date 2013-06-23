// CommandGroupStandard_DCS_Esam.cpp: Implementierung der Klasse CCommandGroupStandard_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandGroupStandard_DCS_Esam.h"
#include "../../CommandSets/Esam/CommandSetObjectDictionary_DCS_Esam.h"
#include "../../CommandSets/Esam/CommandSetNetworkManagement_DCS_Esam.h"
#include "../../CommandSets/Esam/CommandSetGeneralGateway_DCS_Esam.h"
#include "../../CommandSets/Esam/CommandSetLayerSettingServices_DCS_Esam.h"
#include "../../CommandSets/Esam/CommandSetDeviceControl_DCS_Esam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandGroupStandard_DCS_Esam::CCommandGroupStandard_DCS_Esam()
{
    m_strCommandGroupName = COMMAND_GROUP_STANDARD;
    FillSetList();
}

CCommandGroupStandard_DCS_Esam::~CCommandGroupStandard_DCS_Esam()
{
    DeleteSetList();
}

void CCommandGroupStandard_DCS_Esam::FillSetList()
{
    m_pCommandSetObjectDictionary = new CCommandSetObjectDictionary_DCS_Esam();
    m_pCommandSetNetworkManagement = new CCommandSetNetworkManagement_DCS_Esam();
    m_pCommandSetGeneralGateway = new CCommandSetGeneralGateway_DCS_Esam();
    m_pCommandSetLayerSettingServices = new CCommandSetLayerSettingServices_DCS_Esam();
    m_pCommandSetDeviceControl = new CCommandSetDeviceControl_DCS_Esam();
}

void CCommandGroupStandard_DCS_Esam::DeleteSetList()
{
    if(m_pCommandSetObjectDictionary)
    {
        delete m_pCommandSetObjectDictionary;
        m_pCommandSetObjectDictionary = NULL;
    }

    if(m_pCommandSetNetworkManagement)
    {
        delete m_pCommandSetNetworkManagement;
        m_pCommandSetNetworkManagement = NULL;
    }

    if(m_pCommandSetGeneralGateway)
    {
        delete m_pCommandSetGeneralGateway;
        m_pCommandSetGeneralGateway = NULL;
    }

    if(m_pCommandSetLayerSettingServices)
    {
        delete m_pCommandSetLayerSettingServices;
        m_pCommandSetLayerSettingServices = NULL;
    }

    if(m_pCommandSetDeviceControl)
    {
        delete m_pCommandSetDeviceControl;
        m_pCommandSetDeviceControl = NULL;
    }
}

void CCommandGroupStandard_DCS_Esam::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->InitJournalManager(pJournalManager);
    if(m_pCommandSetNetworkManagement) m_pCommandSetNetworkManagement->InitJournalManager(pJournalManager);
    if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->InitJournalManager(pJournalManager);
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->InitJournalManager(pJournalManager);
    if(m_pCommandSetDeviceControl) m_pCommandSetDeviceControl->InitJournalManager(pJournalManager);
}

void CCommandGroupStandard_DCS_Esam::ResetJournalManager()
{
    if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->ResetJournalManager();
    if(m_pCommandSetNetworkManagement) m_pCommandSetNetworkManagement->ResetJournalManager();
    if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->ResetJournalManager();
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->ResetJournalManager();
    if(m_pCommandSetDeviceControl) m_pCommandSetDeviceControl->ResetJournalManager();
}

BOOL CCommandGroupStandard_DCS_Esam::InitGateway(CGateway *pGateway)
{
    if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetNetworkManagement && !m_pCommandSetNetworkManagement->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetDeviceControl && !m_pCommandSetDeviceControl->InitGateway(pGateway)) return FALSE;

    return TRUE;
}

CXXMLFile::CElementPart* CCommandGroupStandard_DCS_Esam::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;

    if(pFile && pParentElement)
    {
        //CommandGroup Elements
        pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
        pFile->SetText(pElement, "CommandGroup");
        pElement->SetAt("Name", m_strCommandGroupName);

        //CommandSet Elements
        if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetNetworkManagement && !m_pCommandSetNetworkManagement->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetDeviceControl && !m_pCommandSetDeviceControl->StoreToXMLFile(pFile, pElement)) return pElement;
    }

    return pElement;
}

