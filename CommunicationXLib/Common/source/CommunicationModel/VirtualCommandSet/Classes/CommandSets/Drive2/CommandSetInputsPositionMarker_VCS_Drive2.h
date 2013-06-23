// CommandSetInputsPositionMarker_VCS_Drive2.h: Schnittstelle f�r die Klasse CCommandSetInputsPositionMarker_VCS_Drive2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDSETINPUTSPOSITIONMARKER_VCS_Drive2_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_)
#define AFX_COMMANDSETINPUTSPOSITIONMARKER_VCS_Drive2_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Drive2;
class CLayerManagerBase;

class CCommandSetInputsPositionMarker_VCS_Drive2 : public CCommandSetBase_VCS
{
public:
    BOOL VCS_SetPositionMarkerParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPositionMarkerEdgeType, BYTE p_ubPositionMarkerMode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetPositionMarkerParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubPositionMarkerEdgeType, BYTE* p_pubPositionMarkerMode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ActivatePositionMarker(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalInputNumber, BOOL p_oPolarity, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DeactivatePositionMarker(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usDigitalInputNumber, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ReadPositionMarkerCounter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pCount, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ReadPositionMarkerCapturedPosition(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCountIndex, long* p_plCapturedPosition, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ResetPositionMarkerCounter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetInputsPositionMarker_VCS_Drive2();
    virtual ~CCommandSetInputsPositionMarker_VCS_Drive2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive2* m_pCommand_SetPositionMarkerParameter;
    CCommand_VCS_Drive2* m_pCommand_GetPositionMarkerParameter;
    CCommand_VCS_Drive2* m_pCommand_ActivatePositionMarker;
    CCommand_VCS_Drive2* m_pCommand_DeactivatePositionMarker;
    CCommand_VCS_Drive2* m_pCommand_ReadPositionMarkerCounter;
    CCommand_VCS_Drive2* m_pCommand_ReadPositionMarkerCapturedPosition;
    CCommand_VCS_Drive2* m_pCommand_ResetPositionMarkerCounter;
};

#endif // !defined(AFX_COMMANDSETINPUTSPOSITIONMARKER_VCS_Drive2_H__EE1097D3_29AA_4407_BADF_75CA398DB377__INCLUDED_)
