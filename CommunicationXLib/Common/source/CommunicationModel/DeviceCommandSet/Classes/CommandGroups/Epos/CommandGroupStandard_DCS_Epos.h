// CommandGroupStandard_DCS_Epos.h: Schnittstelle f�r die Klasse CCommandGroupStandard_DCS_Epos.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandGroupStandard_DCS_Epos_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_)
#define AFX_CommandGroupStandard_DCS_Epos_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

#include "../BaseClasses/CommandGroupBase_DCS.h"
#include <Classes/XXMLFile.h>

class CCommandSetObjectDictionary_DCS_Epos;
class CCommandSetNetworkManagement_DCS_Epos;
class CCommandSetGeneralGateway_DCS_Epos;
class CCommandSetLayerSettingServices_DCS_Epos;
class CGateway;

class CCommandGroupStandard_DCS_Epos : public CCommandGroupBase_DCS
{
public:
	CCommandGroupStandard_DCS_Epos();
	virtual ~CCommandGroupStandard_DCS_Epos();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);
	CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElementPart* pParentElement);

private:
	void DeleteSetList();
	void FillSetList();

	CCommandSetObjectDictionary_DCS_Epos* m_pCommandSetObjectDictionary;
	CCommandSetNetworkManagement_DCS_Epos* m_pCommandSetNetworkManagement;
	CCommandSetGeneralGateway_DCS_Epos* m_pCommandSetGeneralGateway;
	CCommandSetLayerSettingServices_DCS_Epos* m_pCommandSetLayerSettingServices;
};

#endif // !defined(AFX_CommandGroupStandard_DCS_Epos_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_)
