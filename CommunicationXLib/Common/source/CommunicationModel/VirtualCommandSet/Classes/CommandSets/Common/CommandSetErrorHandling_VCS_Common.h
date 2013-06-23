// CommandSetErrorHandling_VCS_Common.h: Schnittstelle f�r die Klasse CCommandSetErrorHandling_VCS_Common.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDSETUTILITIESS_VCS_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
#define AFX_CommandSetErrorHandling_VCS_Common_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Common;
class CLayerManagerBase;

class CCommandSetErrorHandling_VCS_Common : public CCommandSetBase_VCS
{
public:
    BOOL VCS_ResetDevice(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_ClearDeviceErrors(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetNbOfDeviceErrors(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE* p_pubNbOfDeviceErrors, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GetDeviceErrorCode(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubErrorNumber, DWORD* p_pulDeviceErrorCode, CErrorInfo* p_pErrorInfo = NULL);
    BOOL VCS_GotoSafeState(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetErrorHandling_VCS_Common();
    virtual ~CCommandSetErrorHandling_VCS_Common();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Common* m_pCommand_ResetDevice;
    CCommand_VCS_Common* m_pCommand_ClearDeviceErrors;
    CCommand_VCS_Common* m_pCommand_GetNbOfDeviceErrors;
    CCommand_VCS_Common* m_pCommand_GetDeviceErrorCode;
    CCommand_VCS_Common* m_pCommand_GotoSafeState;
};

#endif // !defined(AFX_CommandSetErrorHandling_VCS_Common_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
