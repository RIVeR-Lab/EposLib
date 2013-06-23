// CommandGroupBase_VCS.h: Schnittstelle f�r die Klasse CCommandGroupBase_VCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandGroupBase_VCS_H__E5611500_BAE1_47B0_B64F_C0116C62EB87__INCLUDED_)
#define AFX_CommandGroupBase_VCS_H__E5611500_BAE1_47B0_B64F_C0116C62EB87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

class CVirtualCommandSetDD_ManagerBase;
class CJournalManagerBase;
class CGateway;

class CCommandGroupBase_VCS
{
public:
	virtual CStdString VCS_GetCommandGroupName();

	CCommandGroupBase_VCS();
	virtual ~CCommandGroupBase_VCS();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);

protected:
	CStdString m_strCommandGroupName;
};

#endif // !defined(AFX_CommandGroupBase_VCS_H__E5611500_BAE1_47B0_B64F_C0116C62EB87__INCLUDED_)
