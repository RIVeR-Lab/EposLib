// CommandSetProgramVariableAccess_VCS_Plc2.h: Schnittstelle f�r die Klasse CCommandSetProgramVariableAccess_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Plc2;
class CLayerManagerBase;

class CCommandSetProgramVariableAccess_VCS_Plc2 : public CCommandSetBase_VCS
{
public:
    CCommandSetProgramVariableAccess_VCS_Plc2();
    virtual ~CCommandSetProgramVariableAccess_VCS_Plc2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

    BOOL VCS_SetVariable(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CStdString p_VariableName, void* p_pDataBuffer, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CErrorInfo* p_pErrorInfo);
    BOOL VCS_GetVariable(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CStdString p_VariableName, void* p_pDataBuffer, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CErrorInfo* p_pErrorInfo);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Plc2* m_pCommand_GetVariable;
    CCommand_VCS_Plc2* m_pCommand_SetVariable;
};

