// CommandSetBase_DCS.h: Schnittstelle f�r die Klasse CCommandSetBase_DCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDSETBASE_DCS_H__D115C6FE_C543_46C4_81F8_E42F06F10364__INCLUDED_)
#define AFX_COMMANDSETBASE_DCS_H__D115C6FE_C543_46C4_81F8_E42F06F10364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

class CJournalManagerBase;
class CGatewayDCStoPS;

class CCommandSetBase_DCS
{
public:
	CStdString DCS_GetCommandSetName();

	CCommandSetBase_DCS();
	virtual ~CCommandSetBase_DCS();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
	virtual void ResetJournalManager();

	virtual BOOL InitGateway(CGatewayDCStoPS *pGateway);

protected:
	CStdString m_strCommandSetName;
};

#endif // !defined(AFX_COMMANDSETBASE_DCS_H__D115C6FE_C543_46C4_81F8_E42F06F10364__INCLUDED_)
