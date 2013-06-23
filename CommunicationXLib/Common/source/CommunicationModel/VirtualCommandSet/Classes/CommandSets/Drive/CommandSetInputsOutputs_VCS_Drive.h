// CommandSetInputsOutputs_VCS_Drive.h: Schnittstelle f�r die Klasse CCommandSetInputsOutputs_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetInputsOutputs_VCS_Drive_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_)
#define AFX_CommandSetInputsOutputs_VCS_Drive_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CCommand_VCS_Drive;
class CCommandSetInputsPositionMarker_VCS_Drive;
class CGateway;
class CLayerManagerBase;

class CCommandSetInputsOutputs_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_DigitalInputConfiguration(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalInputNb, WORD p_usConfiguration, BOOL p_oMask, BOOL p_oPolarity, BOOL p_oExecutionMask, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DigitalOutputConfiguration(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalOutputNb, WORD p_usConfiguration, BOOL p_oState, BOOL p_oMask, BOOL p_oPolarity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetAllDigitalInputs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusInputs, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetAllDigitalOutputs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusOutputs, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetAllDigitalOutputs(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usOutputs, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetAnalogInput(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNumber, WORD *p_pusAnalog, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetInputsOutputs_VCS_Drive();
    virtual ~CCommandSetInputsOutputs_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    WORD GetNewBitMask(WORD p_usConfiguration, WORD p_usOldMask, BOOL p_oState);

    void InitCommands();
    void DeleteCommands();

    void FillSetList();
    void DeleteSetList();

    CCommand_VCS_Drive* m_pCommand_DigitalInputConfiguration;
    CCommand_VCS_Drive* m_pCommand_DigitalOutputConfiguration;
    CCommand_VCS_Drive* m_pCommand_GetAllDigitalInputs;
    CCommand_VCS_Drive* m_pCommand_GetAllDigitalOutputs;
    CCommand_VCS_Drive* m_pCommand_GetAnalogInput;
    CCommand_VCS_Drive* m_pCommand_SetAllDigitalOutputs;

    CCommandSetInputsPositionMarker_VCS_Drive* m_pCommand_CommandSetInputsPositionMarker;
};

#endif // !defined(AFX_CommandSetInputsOutputs_VCS_Drive_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_)
