// CommandGroupStandard_DCS_Esam.h: Schnittstelle f�r die Klasse CCommandGroupStandard_DCS_Esam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandGroupStandard_DCS_Esam_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_)
#define AFX_CommandGroupStandard_DCS_Esam_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include "../BaseClasses/CommandGroupBase_DCS.h"
#include <Classes/XXMLFile.h>

class CCommandSetObjectDictionary_DCS_Esam;
class CCommandSetNetworkManagement_DCS_Esam;
class CCommandSetGeneralGateway_DCS_Esam;
class CCommandSetLayerSettingServices_DCS_Esam;
class CCommandSetDeviceControl_DCS_Esam;
class CGateway;

class CCommandGroupStandard_DCS_Esam : public CCommandGroupBase_DCS
{
public:
	CCommandGroupStandard_DCS_Esam();
	virtual ~CCommandGroupStandard_DCS_Esam();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);
	CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElementPart* pParentElement);

private:
	void DeleteSetList();
	void FillSetList();

	CCommandSetObjectDictionary_DCS_Esam* m_pCommandSetObjectDictionary;
	CCommandSetNetworkManagement_DCS_Esam* m_pCommandSetNetworkManagement;
	CCommandSetGeneralGateway_DCS_Esam* m_pCommandSetGeneralGateway;
	CCommandSetLayerSettingServices_DCS_Esam* m_pCommandSetLayerSettingServices;
	CCommandSetDeviceControl_DCS_Esam* m_pCommandSetDeviceControl;
};
#endif // !defined(AFX_CommandGroupStandard_DCS_Esam_H__4FBA0447_A413_4AB9_A680_DAD22FFD9CFD__INCLUDED_)
