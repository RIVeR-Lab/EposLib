// CommandSetProgramStatus_VCS_Plc2.h: Schnittstelle f�r die Klasse CCommandSetProgramStatus_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Plc2;
class CLayerManagerBase;

class CCommandSetProgramStatus_VCS_Plc2 : public CCommandSetBase_VCS
{
public:

    CCommandSetProgramStatus_VCS_Plc2();
    virtual ~CCommandSetProgramStatus_VCS_Plc2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

    BOOL VCS_GetPlcStatus(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poIsProgramRunning, BOOL* p_poIsProgramAvailable, CErrorInfo* p_pErrorInfo);
    BOOL VCS_ColdstartPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo);
    BOOL VCS_WarmstartPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo);
    BOOL VCS_HotstartPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo);
    BOOL VCS_StopPlc(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Plc2* m_pCommand_GetPlcStatus;
    CCommand_VCS_Plc2* m_pCommand_ColdstartPlc;
    CCommand_VCS_Plc2* m_pCommand_WarmstartPlc;
    CCommand_VCS_Plc2* m_pCommand_HotstartPlc;
    CCommand_VCS_Plc2* m_pCommand_StopPlc;
};

