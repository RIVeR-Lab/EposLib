// CommandGroupStandard_VCS_Plc.cpp: Implementierung der Klasse CCommandGroupStandard_VCS_Plc.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandGroupStandard_VCS_Plc.h"


#include "../../CommandSets/CanOpen/CommandSetGeneralGateway_VCS_CanOpen.h"
#include "../../CommandSets/CanOpen/CommandSetLayerSettingServices_VCS_CanOpen.h"
#include "../../CommandSets/CanOpen/CommandSetObjectDictionary_VCS_CanOpen.h"
#include "../../CommandSets/Common/CommandSetErrorHandling_VCS_Common.h"
#include "../../CommandSets/Common/CommandSetVersion_VCS_Common.h"
#include "../BaseClasses/CommandGroupBase_VCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandGroupStandard_VCS_Plc::CCommandGroupStandard_VCS_Plc()
{
    m_strCommandGroupName = COMMAND_GROUP_STANDARD;
    FillSetList();
}

CCommandGroupStandard_VCS_Plc::~CCommandGroupStandard_VCS_Plc()
{
    DeleteSetList();
}

void CCommandGroupStandard_VCS_Plc::FillSetList()
{
    m_pCommandSetGeneralGateway             = new CCommandSetGeneralGateway_VCS_CanOpen();
    m_pCommandSetLayerSettingServices       = new CCommandSetLayerSettingServices_VCS_CanOpen();
	m_pCommandSetObjectDictionary			= new CCommandSetObjectDictionary_VCS_CanOpen();

	m_pCommandSetErrorHandling				= new CCommandSetErrorHandling_VCS_Common();
	m_pCommandSetVersion					= new CCommandSetVersion_VCS_Common();
}

void CCommandGroupStandard_VCS_Plc::DeleteSetList()
{
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

	if(m_pCommandSetObjectDictionary)
    {
        delete m_pCommandSetObjectDictionary;
        m_pCommandSetObjectDictionary = NULL;
    }

	if(m_pCommandSetErrorHandling)
    {
        delete m_pCommandSetErrorHandling;
        m_pCommandSetErrorHandling = NULL;
    }

	if(m_pCommandSetVersion)
    {
        delete m_pCommandSetVersion;
        m_pCommandSetVersion = NULL;
    }
}

CXXMLFile::CElementPart* CCommandGroupStandard_VCS_Plc::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;

    if(pFile && pParentElement)
    {
        //CommandGroup Elements
        pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
        pFile->SetText(pElement, "CommandGroup");
        pElement->SetAt("Name", m_strCommandGroupName);

        //CommandSet Elements
        if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->StoreToXMLFile(pFile, pElement)) return pElement;
        ////if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->StoreToXMLFile(pFile, pElement)) return pElement;
		if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->StoreToXMLFile(pFile, pElement)) return pElement;

		if(m_pCommandSetErrorHandling && !m_pCommandSetErrorHandling->StoreToXMLFile(pFile, pElement)) return pElement;
		if(m_pCommandSetVersion && !m_pCommandSetVersion->StoreToXMLFile(pFile, pElement)) return pElement;

    }

    return pElement;
}

void CCommandGroupStandard_VCS_Plc::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->InitJournalManager(pJournalManager);
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->InitJournalManager(pJournalManager);
	if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->InitJournalManager(pJournalManager);

	if(m_pCommandSetErrorHandling) m_pCommandSetErrorHandling->InitJournalManager(pJournalManager);
	if(m_pCommandSetVersion) m_pCommandSetVersion->InitJournalManager(pJournalManager);
}

void CCommandGroupStandard_VCS_Plc::ResetJournalManager()
{
    if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->ResetJournalManager();
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->ResetJournalManager();
	if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->ResetJournalManager();

	if(m_pCommandSetErrorHandling) m_pCommandSetErrorHandling->ResetJournalManager();
	if(m_pCommandSetVersion) m_pCommandSetVersion->ResetJournalManager();
}

BOOL CCommandGroupStandard_VCS_Plc::InitGateway(CGateway *pGateway)
{
    if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->InitGateway(pGateway)) return FALSE;
	if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->InitGateway(pGateway)) return FALSE;

	if(m_pCommandSetErrorHandling && !m_pCommandSetErrorHandling->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetVersion && !m_pCommandSetVersion->InitGateway(pGateway)) return FALSE;

    return TRUE;
}

