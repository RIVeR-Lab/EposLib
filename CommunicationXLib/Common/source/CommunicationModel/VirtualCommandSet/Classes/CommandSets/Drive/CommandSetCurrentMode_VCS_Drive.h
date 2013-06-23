// CommandSetCurrentMode_VCS_Drive.h: Schnittstelle f�r die Klasse CCommandSetCurrentMode_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetCurrentMode_VCS_Drive_H__3D064F9C_00E0_4989_921F_7580447D6C35__INCLUDED_)
#define AFX_CommandSetCurrentMode_VCS_Drive_H__3D064F9C_00E0_4989_921F_7580447D6C35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"
#include <Classes/XXMLFile.h>

class CGateway;
class CCommand_VCS_Drive;
class CLayerManagerBase;

class CCommandSetCurrentMode_VCS_Drive : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetCurrentMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short* p_psCurrentMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_SetCurrentMust(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, short p_sCurrentMust, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ActivateAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, float fScaling, short sOffset, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DeactivateAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usAnalogInputNumber, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_EnableAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_DisableAnalogCurrentSetpoint(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);
    CCommandSetCurrentMode_VCS_Drive();
    virtual ~CCommandSetCurrentMode_VCS_Drive();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Drive* m_pCommand_GetCurrentMust;
    CCommand_VCS_Drive* m_pCommand_SetCurrentMust;
    CCommand_VCS_Drive* m_pCommand_ActivateAnalogCurrentSetpoint;
    CCommand_VCS_Drive* m_pCommand_DeactivateAnalogCurrentSetpoint;
    CCommand_VCS_Drive* m_pCommand_EnableAnalogCurrentSetpoint;
    CCommand_VCS_Drive* m_pCommand_DisableAnalogCurrentSetpoint;
};

#endif // !defined(AFX_CommandSetCurrentMode_VCS_Drive_H__3D064F9C_00E0_4989_921F_7580447D6C35__INCLUDED_)
