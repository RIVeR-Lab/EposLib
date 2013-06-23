// CommandGroupStandard_VCS_Drive2.h: Schnittstelle f�r die Klasse CommandGroupStandard_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandGroupStandard_VCS_Drive2_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_)
#define AFX_CommandGroupStandard_VCS_Drive2_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <Classes/XXMLFile.h>
#include "../BaseClasses/CommandGroupBase_VCS.h"

class CCommandSetConfiguration_VCS_Drive2;
class CCommandSetConfigurationMotor_VCS_Drive2;
class CCommandSetConfigurationSensor_VCS_Drive2;
class CCommandSetCurrentMode_VCS_Drive2;
class CCommandSetHomingMode_VCS_Drive2;
class CCommandSetInputsOutputs_VCS_Drive2;
class CCommandSetMotionInfo_VCS_Drive2;
class CCommandSetPositionMode_VCS_Drive2;
class CCommandSetProfilePositionMode_VCS_Drive2;
class CCommandSetInterpolatedPositionMode_VCS_Drive2;
class CCommandSetMasterEncoderMode_VCS_Drive2;
class CCommandSetStepDirectionMode_VCS_Drive2;
class CCommandSetProfileVelocityMode_VCS_Drive2;
class CCommandSetStateMachine_VCS_Drive2;
class CCommandSetVelocityMode_VCS_Drive2;

class CCommandSetObjectDictionary_VCS_CanOpen;
class CCommandSetGeneralGateway_VCS_CanOpen;
class CCommandSetLayerSettingServices_VCS_CanOpen;

class CCommandSetErrorHandling_VCS_Common;
class CCommandSetVersion_VCS_Common;

class CCommandSetDataRecording_VCS_DataRecorder;

class CGateway;

class CCommandGroupStandard_VCS_Drive2 : public CCommandGroupBase_VCS
{
public:

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandGroupStandard_VCS_Drive2();
    virtual ~CCommandGroupStandard_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *pGateway);

private:
    void DeleteSetList();
    void FillSetList();

    CCommandSetConfiguration_VCS_Drive2*             m_pCommandSetConfiguration;
    CCommandSetConfigurationMotor_VCS_Drive2*        m_pCommandSetConfigurationMotor;
    CCommandSetConfigurationSensor_VCS_Drive2*       m_pCommandSetConfigurationSensor;
    CCommandSetCurrentMode_VCS_Drive2*               m_pCommandSetCurrentMode;
    CCommandSetHomingMode_VCS_Drive2*                m_pCommandSetHomingMode;
    CCommandSetInputsOutputs_VCS_Drive2*             m_pCommandSetInputsOutputs;
    CCommandSetMotionInfo_VCS_Drive2*                m_pCommandSetMotionInfo;
    CCommandSetPositionMode_VCS_Drive2*              m_pCommandSetPositionMode;
    CCommandSetProfilePositionMode_VCS_Drive2*       m_pCommandSetProfilePositionMode;
    CCommandSetInterpolatedPositionMode_VCS_Drive2*  m_pCommandSetInterpolatedPositionMode;
    CCommandSetMasterEncoderMode_VCS_Drive2*         m_pCommandSetMasterEncoderMode;
    CCommandSetStepDirectionMode_VCS_Drive2*         m_pCommandSetStepDirectionMode;
    CCommandSetProfileVelocityMode_VCS_Drive2*       m_pCommandSetProfileVelocityMode;
    CCommandSetStateMachine_VCS_Drive2*              m_pCommandSetStateMachine;
    CCommandSetVelocityMode_VCS_Drive2*              m_pCommandSetVelocityMode;
    
    CCommandSetObjectDictionary_VCS_CanOpen*	     m_pCommandSetObjectDictionary;
	CCommandSetGeneralGateway_VCS_CanOpen*           m_pCommandSetGeneralGateway;
	CCommandSetLayerSettingServices_VCS_CanOpen*     m_pCommandSetLayerSettingServices;
    	
	CCommandSetErrorHandling_VCS_Common*             m_pCommandSetErrorHandling;
	CCommandSetVersion_VCS_Common*				     m_pCommandSetVersion;

	CCommandSetDataRecording_VCS_DataRecorder*       m_pCommandSetDataRecording;
};


#endif // !defined(AFX_CommandGroupStandard_VCS_Drive2_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_)
