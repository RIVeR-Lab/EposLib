// CommandSetBase_DCS.cpp: Implementierung der Klasse CCommandSetBase_DCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "CommandSetBase_DCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetBase_DCS::CCommandSetBase_DCS()
{
	m_strCommandSetName = "";
}

CCommandSetBase_DCS::~CCommandSetBase_DCS()
{
}

/*********************************************************************/
CStdString CCommandSetBase_DCS::DCS_GetCommandSetName()
{
	return m_strCommandSetName;
}

/********************************************************************/
void CCommandSetBase_DCS::InitJournalManager(CJournalManagerBase* pJournalManager)
{
}

/********************************************************************/
void CCommandSetBase_DCS::ResetJournalManager()
{
}

/********************************************************************/
BOOL CCommandSetBase_DCS::InitGateway(CGatewayDCStoPS *pGateway)
{
	return FALSE;
}


