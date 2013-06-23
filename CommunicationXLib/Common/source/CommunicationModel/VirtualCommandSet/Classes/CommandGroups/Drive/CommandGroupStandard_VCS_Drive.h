// CommandGroupStandard_VCS_Drive.h: Schnittstelle f�r die Klasse CommandGroupStandard_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandGroupStandard_VCS_Drive_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_)
#define AFX_CommandGroupStandard_VCS_Drive_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandGroupBase_VCS.h"

class CCommandSetConfiguration_VCS_Drive;
class CCommandSetConfigurationMotor_VCS_Drive;
class CCommandSetConfigurationSensor_VCS_Drive;
class CCommandSetCurrentMode_VCS_Drive;
class CCommandSetHomingMode_VCS_Drive;
class CCommandSetInputsOutputs_VCS_Drive;
class CCommandSetMotionInfo_VCS_Drive;
class CCommandSetPositionMode_VCS_Drive;
class CCommandSetProfilePositionMode_VCS_Drive;
class CCommandSetProfileVelocityMode_VCS_Drive;
class CCommandSetStateMachine_VCS_Drive;
class CCommandSetVelocityMode_VCS_Drive;

class CCommandSetObjectDictionary_VCS_CanOpen;
class CCommandSetGeneralGateway_VCS_CanOpen;
class CCommandSetLayerSettingServices_VCS_CanOpen;

class CCommandSetErrorHandling_VCS_Common;
class CCommandSetVersion_VCS_Common;

class CGateway;

#include <Classes/XXMLFile.h>

class CCommandGroupStandard_VCS_Drive : public CCommandGroupBase_VCS
{
public:

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement);

    CCommandGroupStandard_VCS_Drive();
    virtual ~CCommandGroupStandard_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *pGateway);

private:
    void DeleteSetList();
    void FillSetList();

    CCommandSetConfiguration_VCS_Drive*             m_pCommandSetConfiguration;
    CCommandSetConfigurationMotor_VCS_Drive*        m_pCommandSetConfigurationMotor;
    CCommandSetConfigurationSensor_VCS_Drive*       m_pCommandSetConfigurationSensor;
    CCommandSetCurrentMode_VCS_Drive*               m_pCommandSetCurrentMode;
    CCommandSetHomingMode_VCS_Drive*                m_pCommandSetHomingMode;
    CCommandSetInputsOutputs_VCS_Drive*             m_pCommandSetInputsOutputs;
    CCommandSetMotionInfo_VCS_Drive*                m_pCommandSetMotionInfo;
    CCommandSetPositionMode_VCS_Drive*              m_pCommandSetPositionMode;
    CCommandSetProfilePositionMode_VCS_Drive*       m_pCommandSetProfilePositionMode;
    CCommandSetProfileVelocityMode_VCS_Drive*       m_pCommandSetProfileVelocityMode;
    CCommandSetStateMachine_VCS_Drive*              m_pCommandSetStateMachine;
    CCommandSetVelocityMode_VCS_Drive*              m_pCommandSetVelocityMode;

	CCommandSetObjectDictionary_VCS_CanOpen*	     m_pCommandSetObjectDictionary;
	CCommandSetGeneralGateway_VCS_CanOpen*           m_pCommandSetGeneralGateway;
	CCommandSetLayerSettingServices_VCS_CanOpen*     m_pCommandSetLayerSettingServices;
    	
	CCommandSetErrorHandling_VCS_Common*             m_pCommandSetErrorHandling;
	CCommandSetVersion_VCS_Common*				     m_pCommandSetVersion;

};



#endif // !defined(AFX_CommandGroupStandard_VCS_Drive_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_)
