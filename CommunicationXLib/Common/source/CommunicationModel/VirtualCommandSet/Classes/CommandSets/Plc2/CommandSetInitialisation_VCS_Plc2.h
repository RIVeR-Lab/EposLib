// CommandSetInitialisation_VCS_Plc2.h: Schnittstelle f�r die Klasse CCommandSetInitialisation_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <Classes/XXMLFile.h>
#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Plc2;
class CLayerManagerBase;

class CCommandSetInitialisation_VCS_Plc2 : public CCommandSetBase_VCS
{
public:

    CCommandSetInitialisation_VCS_Plc2();
    virtual ~CCommandSetInitialisation_VCS_Plc2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

    BOOL VCS_SetBootupBehavior(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usProgramControl, CErrorInfo* p_pErrorInfo);
    BOOL VCS_GetBootupBehavior(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusProgramControl, CErrorInfo* p_pErrorInfo);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Plc2* m_pCommand_GetBootupBehavior;
    CCommand_VCS_Plc2* m_pCommand_SetBootupBehavior;
};

