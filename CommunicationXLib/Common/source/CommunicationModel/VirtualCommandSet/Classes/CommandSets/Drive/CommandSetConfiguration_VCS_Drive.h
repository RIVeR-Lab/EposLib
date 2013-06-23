// CommandSetConfiguration_VCS_Drive.h: Schnittstelle f�r die Klasse CCommandSetConfiguration_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetConfiguration_VCS_Drive_H__968303EA_5BE3_4673_9EEA_2CE9A864E1BD__INCLUDED_)
#define AFX_CommandSetConfiguration_VCS_Drive_H__968303EA_5BE3_4673_9EEA_2CE9A864E1BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CCommand_VCS_Drive;
class CCommandSetConfigurationMotor_VCS_Drive;
class CCommandSetConfigurationSensor_VCS_Drive;
class CGateway;
class CLayerManagerBase;

class CCommandSetConfiguration_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_SetCurrentRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usP, WORD p_usI, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetCurrentRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusP, WORD* p_pusI, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetPositionRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usP, WORD p_usI, WORD p_usD, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetPositionRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusP, WORD* p_pusI, WORD* p_pusD, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetPositionRegulatorFeedForward(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usVelocityFeedForward, WORD p_usAccelerationFeedForward, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetPositionRegulatorFeedForward(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusVelocityFeedForward, WORD *p_pusAccelerationFeedForward, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetVelocityRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usP, WORD p_usI, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetVelocityRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusP, WORD* p_pusI, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetMaxFollowingError(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulMaxFollowingError, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetMaxFollowingError(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulMaxFollowingError, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetMaxProfileVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulMaxProfileVelocity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetMaxProfileVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulMaxProfileVelocity, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetConfiguration_VCS_Drive();
    virtual ~CCommandSetConfiguration_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    void FillSetList();
    void DeleteSetList();

    CCommand_VCS_Drive* m_pCommand_GetCurrentRegulatorGain;
    CCommand_VCS_Drive* m_pCommand_GetPositionRegulatorGain;
    CCommand_VCS_Drive* m_pCommand_GetPositionRegulatorFeedForward;
    CCommand_VCS_Drive* m_pCommand_GetVelocityRegulatorGain;
    CCommand_VCS_Drive* m_pCommand_GetMaxFollowingError;
    CCommand_VCS_Drive* m_pCommand_GetVelocityUnits;
    CCommand_VCS_Drive* m_pCommand_GetMaxProfileVelocity;
    CCommand_VCS_Drive* m_pCommand_SetCurrentRegulatorGain;
    CCommand_VCS_Drive* m_pCommand_SetPositionRegulatorGain;
    CCommand_VCS_Drive* m_pCommand_SetPositionRegulatorFeedForward;
    CCommand_VCS_Drive* m_pCommand_SetVelocityRegulatorGain;
    CCommand_VCS_Drive* m_pCommand_SetMaxFollowingError;
    CCommand_VCS_Drive* m_pCommand_SetVelocityUnits;
    CCommand_VCS_Drive* m_pCommand_SetMaxProfileVelocity;

    CCommandSetConfigurationMotor_VCS_Drive*     m_pCommandSetConfigurationMotor;
    CCommandSetConfigurationSensor_VCS_Drive*    m_pCommandSetConfigurationSensor;
};

#endif // !defined(AFX_CommandSetConfiguration_VCS_Drive_H__968303EA_5BE3_4673_9EEA_2CE9A864E1BD__INCLUDED_)
