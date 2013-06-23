#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_VCS_Drive;
class CLayerManagerBase;

class CCommandSetConfigurationSensor_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_SetEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCounts, WORD p_usPositionSensorType, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusCounts, WORD *p_pusPositionSensorType, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetSensorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usSensorType, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetSensorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusSensorType, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetIncEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulEncoderResolution, BOOL p_oInvertedPolarity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetIncEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD *p_pulEncoderResolution, BOOL *p_poInvertedPolarity, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetHallSensorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL p_oInvertedPolarity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetHallSensorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL *p_poInvertedPolarity, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetConfigurationSensor_VCS_Drive(void);
    virtual ~CCommandSetConfigurationSensor_VCS_Drive(void);

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive* m_pCommand_GetEncoderParameter;
    CCommand_VCS_Drive* m_pCommand_SetEncoderParameter;
    CCommand_VCS_Drive* m_pCommand_GetSensorType;
    CCommand_VCS_Drive* m_pCommand_SetSensorType;
    CCommand_VCS_Drive* m_pCommand_GetIncEncoderParameter;
    CCommand_VCS_Drive* m_pCommand_SetIncEncoderParameter;
    CCommand_VCS_Drive* m_pCommand_GetHallSensorParameter;
    CCommand_VCS_Drive* m_pCommand_SetHallSensorParameter;
};

