// CommandGroupBase_DCS.h: Schnittstelle f�r die Klasse CCommandGroupBase_DCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDGROUPBASE_DCS_H__2FBD69B9_5B69_43D1_A8B8_C42A88C0AC17__INCLUDED_)
#define AFX_COMMANDGROUPBASE_DCS_H__2FBD69B9_5B69_43D1_A8B8_C42A88C0AC17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

class CProtocolStackManagerBase;
class CJournalManagerBase;
class CGateway;

class CCommandGroupBase_DCS
{
public:
	CStdString DCS_GetCommandGroupName();

	CCommandGroupBase_DCS();
	virtual ~CCommandGroupBase_DCS();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway* pGateway);

protected:
	CStdString m_strCommandGroupName;
};

#endif // !defined(AFX_COMMANDGROUPBASE_DCS_H__2FBD69B9_5B69_43D1_A8B8_C42A88C0AC17__INCLUDED_)
