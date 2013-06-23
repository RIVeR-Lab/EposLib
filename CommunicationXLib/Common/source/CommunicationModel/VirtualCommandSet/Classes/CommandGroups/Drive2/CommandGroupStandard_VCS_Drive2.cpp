// CommandGroupStandard_VCS_Drive2.cpp: Implementierung der Klasse CCommandGroupStandard_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandGroupStandard_VCS_Drive2.h"


#include "../../CommandSets/Drive2/CommandSetConfiguration_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetConfigurationMotor_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetConfigurationSensor_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetCurrentMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetHomingMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetInputsOutputs_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetMotionInfo_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetPositionMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetProfilePositionMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetInterpolatedPositionMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetMasterEncoderMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetStepDirectionMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetProfileVelocityMode_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetStateMachine_VCS_Drive2.h"
#include "../../CommandSets/Drive2/CommandSetVelocityMode_VCS_Drive2.h"
#include "../../CommandSets/CanOpen/CommandSetObjectDictionary_VCS_CanOpen.h"
#include "../../CommandSets/CanOpen/CommandSetGeneralGateway_VCS_CanOpen.h"
#include "../../CommandSets/CanOpen/CommandSetLayerSettingServices_VCS_CanOpen.h"
#include "../../CommandSets/Common/CommandSetErrorHandling_VCS_Common.h"
#include "../../CommandSets/Common/CommandSetVersion_VCS_Common.h"
#include "../../CommandSets/DataRecorder/CommandSetDataRecording_VCS_DataRecorder.h"
#include "../BaseClasses/CommandGroupBase_VCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandGroupStandard_VCS_Drive2::CCommandGroupStandard_VCS_Drive2()
{
    m_strCommandGroupName = COMMAND_GROUP_STANDARD;
    FillSetList();
}

CCommandGroupStandard_VCS_Drive2::~CCommandGroupStandard_VCS_Drive2()
{
    DeleteSetList();
}

void CCommandGroupStandard_VCS_Drive2::FillSetList()
{
    m_pCommandSetConfiguration              = new CCommandSetConfiguration_VCS_Drive2();
    m_pCommandSetConfigurationMotor         = new CCommandSetConfigurationMotor_VCS_Drive2();
    m_pCommandSetConfigurationSensor        = new CCommandSetConfigurationSensor_VCS_Drive2();
    m_pCommandSetCurrentMode                = new CCommandSetCurrentMode_VCS_Drive2();
    m_pCommandSetHomingMode                 = new CCommandSetHomingMode_VCS_Drive2();
    m_pCommandSetInputsOutputs              = new CCommandSetInputsOutputs_VCS_Drive2();
    m_pCommandSetMotionInfo                 = new CCommandSetMotionInfo_VCS_Drive2();
    m_pCommandSetPositionMode               = new CCommandSetPositionMode_VCS_Drive2();
    m_pCommandSetProfilePositionMode        = new CCommandSetProfilePositionMode_VCS_Drive2();
    m_pCommandSetInterpolatedPositionMode   = new CCommandSetInterpolatedPositionMode_VCS_Drive2();
    m_pCommandSetMasterEncoderMode          = new CCommandSetMasterEncoderMode_VCS_Drive2();
    m_pCommandSetStepDirectionMode          = new CCommandSetStepDirectionMode_VCS_Drive2();
    m_pCommandSetProfileVelocityMode        = new CCommandSetProfileVelocityMode_VCS_Drive2();
    m_pCommandSetStateMachine               = new CCommandSetStateMachine_VCS_Drive2();
    m_pCommandSetVelocityMode               = new CCommandSetVelocityMode_VCS_Drive2();
    
	m_pCommandSetObjectDictionary			= new CCommandSetObjectDictionary_VCS_CanOpen();
	m_pCommandSetGeneralGateway             = new CCommandSetGeneralGateway_VCS_CanOpen();
    m_pCommandSetLayerSettingServices       = new CCommandSetLayerSettingServices_VCS_CanOpen();
    	
	m_pCommandSetErrorHandling				= new CCommandSetErrorHandling_VCS_Common();
	m_pCommandSetVersion					= new CCommandSetVersion_VCS_Common();

	m_pCommandSetDataRecording              = new CCommandSetDataRecording_VCS_DataRecorder();
}

void CCommandGroupStandard_VCS_Drive2::DeleteSetList()
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
    if(m_pCommandSetInterpolatedPositionMode)
    {
        delete m_pCommandSetInterpolatedPositionMode;
        m_pCommandSetInterpolatedPositionMode = NULL;
    }
    if(m_pCommandSetMasterEncoderMode)
    {
        delete m_pCommandSetMasterEncoderMode;
        m_pCommandSetMasterEncoderMode = NULL;
    }
    if(m_pCommandSetStepDirectionMode)
    {
        delete m_pCommandSetStepDirectionMode;
        m_pCommandSetStepDirectionMode = NULL;
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
    if(m_pCommandSetDataRecording)
    {
        delete m_pCommandSetDataRecording;
        m_pCommandSetDataRecording = NULL;
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

CXXMLFile::CElementPart* CCommandGroupStandard_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;

    if(p_pFile && p_pParentElement)
    {
        //CommandGroup Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandGroup");
        pElement->SetAt("Name", m_strCommandGroupName);

        //CommandSet Elements
        if(m_pCommandSetConfiguration && !m_pCommandSetConfiguration->StoreToXMLFile(p_pFile, pElement)) return pElement;
//        if(m_pCommandSetConfigurationMotor && !m_pCommandSetConfigurationMotor->StoreToXMLFile(p_pFile, pElement)) return pElement;
//        if(m_pCommandSetConfigurationSensor && !m_pCommandSetConfigurationSensor->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetProfilePositionMode && !m_pCommandSetProfilePositionMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetHomingMode && !m_pCommandSetHomingMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetInterpolatedPositionMode && !m_pCommandSetInterpolatedPositionMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetPositionMode && !m_pCommandSetPositionMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetMasterEncoderMode && !m_pCommandSetMasterEncoderMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetStepDirectionMode && !m_pCommandSetStepDirectionMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetProfileVelocityMode && !m_pCommandSetProfileVelocityMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetVelocityMode && !m_pCommandSetVelocityMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetCurrentMode && !m_pCommandSetCurrentMode->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetMotionInfo && !m_pCommandSetMotionInfo->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetInputsOutputs && !m_pCommandSetInputsOutputs->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetStateMachine && !m_pCommandSetStateMachine->StoreToXMLFile(p_pFile, pElement)) return pElement;
        
		if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->StoreToXMLFile(p_pFile, pElement)) return pElement;
		if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->StoreToXMLFile(p_pFile, pElement)) return pElement;
        //if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->StoreToXMLFile(p_pFile, pElement)) return pElement;
        		
		if(m_pCommandSetErrorHandling && !m_pCommandSetErrorHandling->StoreToXMLFile(p_pFile, pElement)) return pElement;
		if(m_pCommandSetVersion && !m_pCommandSetVersion->StoreToXMLFile(p_pFile, pElement)) return pElement;

		if(m_pCommandSetDataRecording && !m_pCommandSetDataRecording->StoreToXMLFile(p_pFile, pElement)) return pElement;
    }

    return pElement;
}

void CCommandGroupStandard_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommandSetConfiguration) m_pCommandSetConfiguration->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetConfigurationMotor) m_pCommandSetConfigurationMotor->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetConfigurationSensor) m_pCommandSetConfigurationSensor->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetCurrentMode) m_pCommandSetCurrentMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetHomingMode) m_pCommandSetHomingMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetInputsOutputs) m_pCommandSetInputsOutputs->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetMotionInfo) m_pCommandSetMotionInfo->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetPositionMode) m_pCommandSetPositionMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetProfilePositionMode) m_pCommandSetProfilePositionMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetInterpolatedPositionMode) m_pCommandSetInterpolatedPositionMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetMasterEncoderMode) m_pCommandSetMasterEncoderMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetStepDirectionMode) m_pCommandSetStepDirectionMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetProfileVelocityMode) m_pCommandSetProfileVelocityMode->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetStateMachine) m_pCommandSetStateMachine->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetVelocityMode) m_pCommandSetVelocityMode->InitJournalManager(p_pJournalManager);
    
	if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->InitJournalManager(p_pJournalManager);
	if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->InitJournalManager(p_pJournalManager);
    	
	if(m_pCommandSetErrorHandling) m_pCommandSetErrorHandling->InitJournalManager(p_pJournalManager);
	if(m_pCommandSetVersion) m_pCommandSetVersion->InitJournalManager(p_pJournalManager);

	if(m_pCommandSetDataRecording) m_pCommandSetDataRecording->InitJournalManager(p_pJournalManager);
}

void CCommandGroupStandard_VCS_Drive2::ResetJournalManager()
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
    if(m_pCommandSetInterpolatedPositionMode) m_pCommandSetInterpolatedPositionMode->ResetJournalManager();
    if(m_pCommandSetMasterEncoderMode) m_pCommandSetMasterEncoderMode->ResetJournalManager();
    if(m_pCommandSetStepDirectionMode) m_pCommandSetStepDirectionMode->ResetJournalManager();
    if(m_pCommandSetProfileVelocityMode) m_pCommandSetProfileVelocityMode->ResetJournalManager();
    if(m_pCommandSetStateMachine) m_pCommandSetStateMachine->ResetJournalManager();
    if(m_pCommandSetVelocityMode) m_pCommandSetVelocityMode->ResetJournalManager();
    
	if(m_pCommandSetObjectDictionary) m_pCommandSetObjectDictionary->ResetJournalManager();
	if(m_pCommandSetGeneralGateway) m_pCommandSetGeneralGateway->ResetJournalManager();
    if(m_pCommandSetLayerSettingServices) m_pCommandSetLayerSettingServices->ResetJournalManager();
    
	if(m_pCommandSetErrorHandling) m_pCommandSetErrorHandling->ResetJournalManager();
	if(m_pCommandSetVersion) m_pCommandSetVersion->ResetJournalManager();

	if(m_pCommandSetDataRecording) m_pCommandSetDataRecording->ResetJournalManager();
}

BOOL CCommandGroupStandard_VCS_Drive2::InitGateway(CGateway *pGateway)
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
    if(m_pCommandSetInterpolatedPositionMode && !m_pCommandSetInterpolatedPositionMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetMasterEncoderMode && !m_pCommandSetMasterEncoderMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetStepDirectionMode && !m_pCommandSetStepDirectionMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetProfileVelocityMode && !m_pCommandSetProfileVelocityMode->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetStateMachine && !m_pCommandSetStateMachine->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetVelocityMode && !m_pCommandSetVelocityMode->InitGateway(pGateway)) return FALSE;
    
	if(m_pCommandSetObjectDictionary && !m_pCommandSetObjectDictionary->InitGateway(pGateway)) return FALSE;
	if(m_pCommandSetGeneralGateway && !m_pCommandSetGeneralGateway->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetLayerSettingServices && !m_pCommandSetLayerSettingServices->InitGateway(pGateway)) return FALSE;
    	
    if(m_pCommandSetErrorHandling && !m_pCommandSetErrorHandling->InitGateway(pGateway)) return FALSE;
    if(m_pCommandSetVersion && !m_pCommandSetVersion->InitGateway(pGateway)) return FALSE;

	if(m_pCommandSetDataRecording && !m_pCommandSetDataRecording->InitGateway(pGateway)) return FALSE;

    return TRUE;
}

