// CommandGroupBase_VCS.cpp: Implementierung der Klasse CCommandGroupBase_VCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandGroupBase_VCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandGroupBase_VCS::CCommandGroupBase_VCS()
{
	m_strCommandGroupName = "";
}

CCommandGroupBase_VCS::~CCommandGroupBase_VCS()
{
}

/********************************************************************/
CStdString CCommandGroupBase_VCS::VCS_GetCommandGroupName()
{
	return m_strCommandGroupName;
}

/********************************************************************/
void CCommandGroupBase_VCS::InitJournalManager(CJournalManagerBase *pJournalManager)
{
}

/********************************************************************/
void CCommandGroupBase_VCS::ResetJournalManager()
{
}

/********************************************************************/
BOOL CCommandGroupBase_VCS::InitGateway(CGateway *pGateway)
{
	return TRUE;
}
