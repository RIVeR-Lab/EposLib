// CommandSetLayerSettingServices_VCS_CanOpen.h: Schnittstelle f�r die Klasse CCommandSetLayerSettingServices_VCS_CanOpen.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetGeneralGatewayS_VCS_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
#define AFX_CommandSetLayerSettingServices_VCS_CanOpen_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BaseClasses/CommandSetBase_VCS.h"

class CGateway;
class CCommand_VCS_CanOpen;
class CLayerManagerBase;

class CCommandSetLayerSettingServices_VCS_CanOpen : public CCommandSetBase_VCS
{
public:

    CCommandSetLayerSettingServices_VCS_CanOpen();
    virtual ~CCommandSetLayerSettingServices_VCS_CanOpen();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *p_pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *p_pGateway);

    BOOL VCS_SendLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo);
    BOOL VCS_ReadLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usTimeout, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement);

private:
    void InitCommands();
    void DeleteCommands();

    CCommand_VCS_CanOpen* m_pCommand_ReadLSSFrame;
    CCommand_VCS_CanOpen* m_pCommand_SendLSSFrame;
};

#endif // !defined(AFX_CommandSetLayerSettingServices_VCS_CanOpen_H__AB1B4AF8_98FF_4D3B_852E_D0191BB3BDDE__INCLUDED_)
