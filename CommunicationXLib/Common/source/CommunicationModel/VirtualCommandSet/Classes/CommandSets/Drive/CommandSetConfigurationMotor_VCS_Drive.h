#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_VCS_Drive;
class CLayerManagerBase;

class CCommandSetConfigurationMotor_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_SetMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usMotorType, WORD p_usNominalCurrent, WORD p_usMaxOutputCurrent, BYTE p_ubNbOfPolePairs, WORD p_usThermalTimeConstant, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusMotorType, WORD *p_pusNominalCurrent, WORD *p_pusMaxOutputCurrent, BYTE *p_pubNbOfPolePairs, WORD *p_pusThermalTimeConstant, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetMotorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usMotorType, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetMotorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusMotorType, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetDcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNominalCurrent, WORD p_usMaxOutputCurrent, WORD p_usThermalTimeConstant, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetDcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusNominalCurrent, WORD *p_pusMaxOutputCurrent, WORD *p_pusThermalTimeConstant, CErrorInfo* p_pErrorInfo = NULL);

    BOOL VCS_SetEcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNominalCurrent, WORD p_usMaxOutputCurrent, WORD p_usThermalTimeConstant, BYTE p_ubNbOfPolePair, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetEcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusNominalCurrent, WORD *p_pusMaxOutputCurrent, WORD *p_pusThermalTimeConstant, BYTE *p_pubNbOfPolePair, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetConfigurationMotor_VCS_Drive(void);
    virtual ~CCommandSetConfigurationMotor_VCS_Drive(void);

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive* m_pCommand_SetMotorParameter;
    CCommand_VCS_Drive* m_pCommand_GetMotorParameter;
    CCommand_VCS_Drive* m_pCommand_SetMotorType;
    CCommand_VCS_Drive* m_pCommand_GetMotorType;
    CCommand_VCS_Drive* m_pCommand_SetDcMotorParameter;
    CCommand_VCS_Drive* m_pCommand_GetDcMotorParameter;
    CCommand_VCS_Drive* m_pCommand_SetEcMotorParameter;
    CCommand_VCS_Drive* m_pCommand_GetEcMotorParameter;
};

