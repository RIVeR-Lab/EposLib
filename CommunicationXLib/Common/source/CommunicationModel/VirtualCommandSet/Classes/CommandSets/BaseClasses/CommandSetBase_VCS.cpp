// CommandSetBase_VCS.cpp: Implementierung der Klasse CCommandSetBase_VCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "CommandSetBase_VCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetBase_VCS::CCommandSetBase_VCS()
{
	m_strCommandSetName = "";
}

CCommandSetBase_VCS::~CCommandSetBase_VCS()
{
}

//********************************************************************
CStdString CCommandSetBase_VCS::VCS_GetCommandSetName()
{
	return m_strCommandSetName;
}

/********************************************************************/
void CCommandSetBase_VCS::InitJournalManager(CJournalManagerBase *pJournalManager)
{
}

/********************************************************************/
void CCommandSetBase_VCS::ResetJournalManager()
{
}

/********************************************************************/
BOOL CCommandSetBase_VCS::InitGateway(CGateway *pGateway)
{
	return FALSE;
}
