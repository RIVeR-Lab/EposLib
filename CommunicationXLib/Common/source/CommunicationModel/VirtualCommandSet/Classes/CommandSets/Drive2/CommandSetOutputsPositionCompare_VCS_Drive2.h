// CommandSetOutputsPositionCompare_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetOutputsPositionCompare_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetOutputsPositionCompare_VCS_Drive2_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_)
#define AFX_CommandSetOutputsPositionCompare_VCS_Drive2_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetOutputsPositionCompare_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_SetPositionCompareParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubOperationalMode, BYTE p_ubIntervalMode, BYTE p_ubDirectionDependency, WORD p_usIntervalWidth, WORD p_usIntervalRepetitions, WORD p_usPulseWidth, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetPositionCompareParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubOperationalMode, BYTE* p_pubIntervalMode, BYTE* p_pubDirectionDependency, WORD* p_pusIntervalWidth, WORD* p_pusIntervalRepetitions, WORD* p_pusPulseWidth, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ActivatePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalOutputNumber, BOOL p_oPolarity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DeactivatePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalOutputNumber, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_EnablePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DisablePositionCompare(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetPositionCompareReferencePosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long p_lReferencePosition, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetOutputsPositionCompare_VCS_Drive2();
    virtual ~CCommandSetOutputsPositionCompare_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_SetPositionCompareParameter;
    CCommand_VCS_Drive2* m_pCommand_GetPositionCompareParameter;
    CCommand_VCS_Drive2* m_pCommand_ActivatePositionCompare;
    CCommand_VCS_Drive2* m_pCommand_DeactivatePositionCompare;
    CCommand_VCS_Drive2* m_pCommand_EnablePositionCompare;
    CCommand_VCS_Drive2* m_pCommand_DisablePositionCompare;
    CCommand_VCS_Drive2* m_pCommand_SetPositionCompareReferencePosition;
};

#endif // !defined(AFX_CommandSetOutputsPositionCompare_VCS_Drive2_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_)
