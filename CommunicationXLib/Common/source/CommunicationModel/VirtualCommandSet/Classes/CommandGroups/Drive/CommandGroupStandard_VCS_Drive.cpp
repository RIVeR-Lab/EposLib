// CommandGroupStandard_VCS_Drive.cpp: Implementierung der Klasse CCommandGroupStandard_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandGroupStandard_VCS_Drive.h"


#include "../../CommandSets/Drive/CommandSetConfiguration_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetConfigurationMotor_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetConfigurationSensor_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetCurrentMode_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetHomingMode_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetInputsOutputs_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetMotionInfo_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetPositionMode_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetProfilePositionMode_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetProfileVelocityMode_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetStateMachine_VCS_Drive.h"
#include "../../CommandSets/Drive/CommandSetVelocityMode_VCS_Drive.h"
#include "../../CommandSets/CanOpen/CommandSetObjectDictionary_VCS_CanOpen.h"
#include "../../CommandSets/CanOpen/CommandSetGeneralGateway_VCS_CanOpen.h"
#include "../../CommandSets/CanOpen/CommandSetLayerSettingServices_VCS_CanOpen.h"
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
CCommandGroupStandard_VCS_Drive::CCommandGroupStandard_VCS_Drive()
{
    m_strCommandGroupName = COMMAND_GROUP_STANDARD;
    FillSetList();
}

CCommandGroupStandard_VCS_Drive::~CCommandGroupStandard_VCS_Drive()
{
    DeleteSetList();
}

void CCommandGroupStandard_VCS_Drive::FillSetList()
{
    m_pCommandSetConfiguration              = new CCommandSetConfiguration_VCS_Drive();
    m_pCommandSetConfigurationMotor         = new CCommandSetConfigurationMotor_VCS_Drive();
    m_pCommandSetConfigurationSensor        = new CCommandSetConfigurationSensor_VCS_Drive();
    m_pCommandSetCurrentMode                = new CCommandSetCurrentMode_VCS_Drive();
    m_pCommandSetHomingMode                 = new CCommandSetHomingMode_VCS_Drive();
    m_pCommandSetInputsOutputs              = new CCommandSetInputsOutputs_VCS_Drive();
    m_pCommandSetMotionInfo                 = new CCommandSetMotionInfo_VCS_Drive();
    m_pCommandSetPositionMode               = new CCommandSetPositionMode_VCS_Drive();
    m_pCommandSetProfilePositionMode        = new CCommandSetProfilePositionMode_VCS_Drive();
    m_pCommandSetProfileVelocityMode        = new CCommandSetProfileVelocityMode_VCS_Drive();
    m_pCommandSetStateMachine               = new CCommandSetStateMachine_VCS_Drive();
    m_pCommandSetVelocityMode               = new CCommandSetVelocityMode_VCS_Drive();
    
	m_pCommandSetObjectDictionary			= new CCommandSetObjectDictionary_VCS_CanOpen();
	m_pCommandSetGeneralGateway             = new CCommandSetGeneralGateway_VCS_CanOpen();
    m_pCommandSetLayerSettingServices       = new CCommandSetLayerSettingServices_VCS_CanOpen();
    	
	m_pCommandSetErrorHandling				= new CCommandSetErrorHandling_VCS_Common();
	m_pCommandSetVersion					= new CCommandSetVersion_VCS_Common();

}

void CCommandGroupStandard_VCS_Drive::DeleteSetList()
{
    if(m_pCommandSetConfiguration)
    {
        delete m_pCommandSetConfiguration;
        m_pCommandSetConfiguration = NULL;
    }
    if(m_pCommandSetConfigurationMotor)
    {
        delete m_pCommandSetConfigurationMotor;
        m_pCommandSetConfigurationMotor = NULL;
    }
    if(m_pCommandSetConfigurationSensor)
    {
        delete m_pCommandSetConfigurationSensor;
        m_pCommandSetConfigurationSensor = NULL;
    }
    if(m_pCommandSetCurrentMode)
    {
        delete m_pCommandSetCurrentMode;
        m_pCommandSetCurrentMode = NULL;
    }
    if(m_pCommandSetHomingMode)
    {
        delete m_pCommandSetHomingMode;
        m_pCommandSetHomingMode = NULL;
    }
    if(m_pCommandSetInputsOutputs)
    {
        delete m_pCommandSetInputsOutputs;
        m_pCommandSetInputsOutputs = NULL;
    }
    if(m_pCommandSetMotionInfo)
    {
        delete m_pCommandSetMotionInfo;
        m_pCommandSetMotionInfo = NULL;
    }
    if(m_pCommandSetPositionMode)
    {
        delete m_pCommandSetPositionMode;
        m_pCommandSetPositionMode = NULL;
    }
    if(m_pCommandSetProfilePositionMode)
    {
        delete m_pCommandSetProfilePositionMode;
        m_pCommandSetProfilePositionMode = NULL;
    }
    if(m_pCommandSetProfileVelocityMode)
    {
        delete m_pCommandSetProfileVelocityMode;
        m_pCommandSetProfileVelocityMode = NULL;
    }
    if(m_pCommandSetStateMachine)
    {
        delete m_pCommandSetStateMachine;
        m_pCommandSetStateMachine = NULL;
    }
    if(m_pCommandSetVelocityMode)
    {
        delete m_pCommandSetVelocityMode;
        m_pCommandSetVelocityMode = NULL;
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

CXXMLFile::CElementPart* CCommandGroupStandard_VCS_Drive::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;

    if(pFile && pParentElement)
    {
        //CommandGroup Elements
        pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
        pFile->SetText(pElement, "CommandGroup");
        pElement->SetAt("Name", m_strCommandGroupName);

        //CommandSet Elements
        if(m_pCommandSetConfiguration && !m_pCommandSetConfiguration->StoreToXMLFile(pFile, pElement)) return pElement;
//        if(m_pCommandSetConfigurationMotor && !m_pCommandSetConfigurationMotor->StoreToXMLFile(pFile, pElement)) return pElement;
//        if(m_pCommandSetConfigurationSensor && !m_pCommandSetConfigurationSensor->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetProfilePositionMode && !m_pCommandSetProfilePositionMode->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetHomingMode && !m_pCommandSetHomingMode->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetPositionMode && !m_pCommandSetPositionMode->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetProfileVelocityMode && !m_pCommandSetProfileVelocityMode->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetVelocityMode && !m_pCommandSetVelocityMode->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetCurrentMode && !m_pCommandSetCurrentMode->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetMotionInfo && !m_pCommandSetMotionInfo->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetInputsOutputs && !m_pCommandSetInputsOutputs->StoreToXMLFile(pFile, pElement)) return pElement;
        if(m_pCommandSetStateMachine && !m_pCommandSetStateMachine->StoreToXMLFile(pFile, pElement)) return pElement;
        
		if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->StoreToXMLFile(pFile, pElement)) return pElement;
		if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->StoreToXMLFile(pFile, pElement)) return pElement;
        //if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->StoreToXMLFile(pFile, pElement)) return pElement;
        		
		if(m_pCommandSetErrorHandling && !m_pCommandSetErrorHandling->StoreToXMLFile(pFile, pElement)) return pElement;
		if(m_pCommandSetVersion && !m_pCommandSetVersion->StoreToXMLFile(pFile, pElement)) return pElement;

    }

    return pElement;
}


void CCommandGroupStandard_VCS_Drive::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    if(m_pCommandSetConfiguration) m_pCommandSetConfiguration->InitJournalManager(pJournalManager);
    if(m_pCommandSetConfigurationMotor) m_pCommandSetConfigurationMotor->InitJournalManager(pJournalManager);
    if(m_pCommandSetConfigurationSensor) m_pCommandSetConfigurationSensor->InitJournalManager(pJournalManager);
    if(m_pCommandSetCurrentMode) m_pCommandSetCurrentMode->InitJournalManager(pJournalManager);
    if(m_pCommandSetHomingMode) m_pCommandSetHomingMode->InitJournalManager(pJournalManager);
    if(m_pCommandSetInputsOutputs) m_pCommandSetInputsOutputs->InitJournalManager(pJournalManager);
    if(m_pCommandSetMotionInfo) m_pCommandSetMotionInfo->InitJournalManager(pJournalManager);
    if(m_pCommandSetPositionMode) m_pCommandSetPositionMode->InitJournalManager(pJournalManager);
    if(m_pCommandSetProfilePositionMode) m_pCommandSetProfilePositionMode->InitJournalManager(pJournalManager);
    if(m_pCommandSetProfileVelocityMode) m_pCommandSetProfileVelocityMode->InitJournalManager(pJournalManager);
    if(m_pCommandSetStateMachine) m_pCommandSetStateMachine->InitJournalManager(pJournalManager);
    if(m_pCommandSetVelocityMode) m_pCommandSetVelocityMode->InitJournalManager(pJournalManager);
    
	if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->InitJournalManager(pJournalManager);
	if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->InitJournalManager(pJournalManager);
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->InitJournalManager(pJournalManager);
    
	if(m_pCommandSetErrorHandling) m_pCommandSetErrorHandling->InitJournalManager(pJournalManager);
	if(m_pCommandSetVersion) m_pCommandSetVersion->InitJournalManager(pJournalManager);

}

void CCommandGroupStandard_VCS_Drive::ResetJournalManager()
{
    if(m_pCommandSetConfiguration) m_pCommandSetConfiguration->ResetJournalManager();
    if(m_pCommandSetConfigurationMotor) m_pCommandSetConfigurationMotor->ResetJournalManager();
    if(m_pCommandSetConfigurationSensor) m_pCommandSetConfigurationSensor->ResetJournalManager();
    if(m_pCommandSetCurrentMode) m_pCommandSetCurrentMode->ResetJournalManager();
    if(m_pCommandSetHomingMode) m_pCommandSetHomingMode->ResetJournalManager();
    if(m_pCommandSetInputsOutputs) m_pCommandSetInputsOutputs->ResetJournalManager();
    if(m_pCommandSetMotionInfo) m_pCommandSetMotionInfo->ResetJournalManager();
    if(m_pCommandSetPositionMode) m_pCommandSetPositionMode->ResetJournalManager();
    if(m_pCommandSetProfilePositionMode) m_pCommandSetProfilePositionMode->ResetJournalManager();
    if(m_pCommandSetProfileVelocityMode) m_pCommandSetProfileVelocityMode->ResetJournalManager();
    if(m_pCommandSetStateMachine) m_pCommandSetStateMachine->ResetJournalManager();
    if(m_pCommandSetVelocityMode) m_pCommandSetVelocityMode->ResetJournalManager();
    
	if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->ResetJournalManager();
	if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->ResetJournalManager();
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->ResetJournalManager();
    	
	if(m_pCommandSetErrorHandling) m_pCommandSetErrorHandling->ResetJournalManager();
	if(m_pCommandSetVersion) m_pCommandSetVersion->ResetJournalManager();

}

BOOL CCommandGroupStandard_VCS_Drive::InitGateway(CGateway *pGateway)
{
    if(m_pCommandSetConfiguration && !m_pCommandSetConfiguration->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetConfigurationMotor && !m_pCommandSetConfigurationMotor->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetConfigurationSensor && !m_pCommandSetConfigurationSensor->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetCurrentMode && !m_pCommandSetCurrentMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetHomingMode && !m_pCommandSetHomingMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetInputsOutputs && !m_pCommandSetInputsOutputs->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetMotionInfo && !m_pCommandSetMotionInfo->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetPositionMode && !m_pCommandSetPositionMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetProfilePositionMode && !m_pCommandSetProfilePositionMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetProfileVelocityMode && !m_pCommandSetProfileVelocityMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetStateMachine && !m_pCommandSetStateMachine->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetVelocityMode && !m_pCommandSetVelocityMode->InitGateway(pGateway)) return FALSE;
    
	if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->InitGateway(pGateway)) return FALSE;
	if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->InitGateway(pGateway)) return FALSE;
        
    if(m_pCommandSetErrorHandling && !m_pCommandSetErrorHandling->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetVersion && !m_pCommandSetVersion->InitGateway(pGateway)) return FALSE;

    return TRUE;
}

