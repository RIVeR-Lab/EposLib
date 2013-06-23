// Error.cpp: Implementierung der Klasse CError.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/ErrorHandling/Error.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CError::CError()
{

}

CError::~CError()
{

}

CError* CError::Clone()
{
	CError* pNewError;

	pNewError = new CError();
	*pNewError = *this;

	return pNewError;
}
#ifdef _WIN32
void CError::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_dErrorCode;
		ar << m_strErrorDescription;
	}
	else
	{
		ar >> m_dErrorCode;
		ar >> m_strErrorDescription;
	}
}
#endif //_WIN32
CError& CError::operator=(CError& other)
{
	if(this != &other)
	{
		m_dErrorCode = other.m_dErrorCode;
		m_strErrorDescription = other.m_strErrorDescription;
	}

	return *this;
}

DWORD CError::GetErrorCode()
{
	return m_dErrorCode;
}

void CError::Init(DWORD dErrorCode, CStdString strErrorDescription)
{
	m_dErrorCode = dErrorCode;
	m_strErrorDescription = strErrorDescription;
}
