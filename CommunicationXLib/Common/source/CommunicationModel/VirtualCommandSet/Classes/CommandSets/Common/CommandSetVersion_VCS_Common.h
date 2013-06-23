// CommandSetVersion_VCS_Common.h: Schnittstelle f�r die Klasse CCommandSetVersion_VCS_Common.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDSETUTILITIESS_VCS_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
#define AFX_CommandSetVersion_VCS_Common_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_Common;
class CLayerManagerBase;

class CCommandSetVersion_VCS_Common : public CCommandSetBase_VCS
{
public:
    BOOL VCS_GetVersion(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pulHardwareVersion, WORD* p_pulSoftwareVersion, WORD* p_pulApplicationNumber, WORD* p_pulApplicationVersion, CErrorInfo* p_pErrorInfo = NULL);
	BOOL VCS_GetDeviceType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulDeviceType, CErrorInfo* p_pErrorInfo = NULL);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

    CCommandSetVersion_VCS_Common();
    virtual ~CCommandSetVersion_VCS_Common();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_Common* m_pCommand_GetVersion;
	CCommand_VCS_Common* m_pCommand_GetDeviceType;

};

#endif // !defined(AFX_CommandSetVersion_VCS_Common_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
