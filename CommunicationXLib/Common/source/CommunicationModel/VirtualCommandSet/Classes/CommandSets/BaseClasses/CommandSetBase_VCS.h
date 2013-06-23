// CommandSetBase_VCS.h: Schnittstelle f�r die Klasse CCommandSetBase_VCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandSetBase_VCS_H__6735C302_8AAA_44C1_9CA6_1C6580C064DE__INCLUDED_)
#define AFX_CommandSetBase_VCS_H__6735C302_8AAA_44C1_9CA6_1C6580C064DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJournalManagerBase;
class CGatewayVCStoDCS;
class CGateway;

#include <MmcTypeDefinition.h>

#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <Classes/XXMLFile.h>

class CCommandSetBase_VCS
{
public:
	CStdString VCS_GetCommandSetName();

	CCommandSetBase_VCS();
	virtual ~CCommandSetBase_VCS();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGateway *pGateway);

protected:
	CStdString m_strCommandSetName;
};

#endif // !defined(AFX_CommandSetBase_VCS_H__6735C302_8AAA_44C1_9CA6_1C6580C064DE__INCLUDED_)
