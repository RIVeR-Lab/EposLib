// CommandGroupStandard_VCS_Plc2.h: Schnittstelle f�r die Klasse CommandGroupStandard_VCS_Plc2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandGroupStandard_VCS_Plc2_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_)
#define AFX_CommandGroupStandard_VCS_Plc2_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../BaseClasses/CommandGroupBase_VCS.h"
#include <Classes/XXMLFile.h>


class CCommandSetGeneralGateway_VCS_CanOpen;
class CCommandSetLayerSettingServices_VCS_CanOpen;
class CCommandSetObjectDictionary_VCS_CanOpen;
class CCommandSetErrorHandling_VCS_Common;
class CCommandSetVersion_VCS_Common;
class CGateway;

class CCommandGroupStandard_VCS_Plc2 : public CCommandGroupBase_VCS
{
public:

    CXXMLFile::CElementPart* StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElementPart* pParentElement);

    CCommandGroupStandard_VCS_Plc2();
    virtual ~CCommandGroupStandard_VCS_Plc2();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

//Gateway
    virtual BOOL InitGateway(CGateway *pGateway);

private:
    void DeleteSetList();
    void FillSetList();

    CCommandSetGeneralGateway_VCS_CanOpen*            m_pCommandSetGeneralGateway;
    CCommandSetLayerSettingServices_VCS_CanOpen*      m_pCommandSetLayerSettingServices;
	CCommandSetObjectDictionary_VCS_CanOpen*		   m_pCommandSetObjectDictionary;

	CCommandSetErrorHandling_VCS_Common*           m_pCommandSetErrorHandling;
	CCommandSetVersion_VCS_Common*				   m_pCommandSetVersion;
};


#endif // !defined(AFX_CommandGroupStandard_VCS_Plc2_H__2002A120_26D9_46B6_A3CD_8153D5DF13B6__INCLUDED_)
