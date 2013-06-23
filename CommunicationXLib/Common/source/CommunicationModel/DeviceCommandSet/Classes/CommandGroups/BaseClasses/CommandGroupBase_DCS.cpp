// CommandGroupBase_DCS.cpp: Implementierung der Klasse CCommandGroupBase_DCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandGroupBase_DCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandGroupBase_DCS::CCommandGroupBase_DCS()
{
	m_strCommandGroupName = "";
}

CCommandGroupBase_DCS::~CCommandGroupBase_DCS()
{
}

/********************************************************************/
CStdString CCommandGroupBase_DCS::DCS_GetCommandGroupName()
{
	return m_strCommandGroupName;
}

/********************************************************************/
void CCommandGroupBase_DCS::InitJournalManager(CJournalManagerBase* pJournalManager)
{
}

/********************************************************************/
void CCommandGroupBase_DCS::ResetJournalManager()
{
}

/********************************************************************/
BOOL CCommandGroupBase_DCS::InitGateway(CGateway *pGateway)
{
	return TRUE;
}
