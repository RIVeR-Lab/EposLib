// CommandGroupStandard_DCS_Epos2.h: Schnittstelle f�r die Klasse CCommandGroupStandard_DCS_Epos2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandGroupStandard_DCS_Epos2_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_)
#define AFX_CommandGroupStandard_DCS_Epos2_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include "../BaseClasses/CommandGroupBase_DCS.h"

class CCommandSetObjectDictionary_DCS_Epos2;
class CCommandSetNetworkManagement_DCS_Epos2;
class CCommandSetGeneralGateway_DCS_Epos2;
class CCommandSetLayerSettingServices_DCS_Epos2;
class CGateway;

#include <Classes/XXMLFile.h>

class CCommandGroupStandard_DCS_Epos2 : public CCommandGroupBase_DCS
{
public:
    CCommandGroupStandard_DCS_Epos2();
    virtual ~CCommandGroupStandard_DCS_Epos2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
    virtual void ResetJournalManager();

    virtual BOOL InitGateway(CGateway *pGateway);

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement);

private:
    void DeleteSetList();
    void FillSetList();

    CCommandSetObjectDictionary_DCS_Epos2* m_pCommandSetObjectDictionary;
    CCommandSetNetworkManagement_DCS_Epos2* m_pCommandSetNetworkManagement;
    CCommandSetGeneralGateway_DCS_Epos2* m_pCommandSetGeneralGateway;
    CCommandSetLayerSettingServices_DCS_Epos2* m_pCommandSetLayerSettingServices;
};

#endif // !defined(AFX_CommandGroupStandard_DCS_Epos2_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_)
