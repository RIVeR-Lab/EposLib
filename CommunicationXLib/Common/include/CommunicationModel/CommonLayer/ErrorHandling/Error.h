// Error.h: Schnittstelle f�r die Klasse CError.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERROR_H__5CE81141_1051_4F7C_BF64_E586F5B64A25__INCLUDED_)
#define AFX_ERROR_H__5CE81141_1051_4F7C_BF64_E586F5B64A25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

class CError
{
public:
	void Init(DWORD dErrorCode, CStdString strErrorDescription);
	DWORD GetErrorCode();
	CError();
	virtual ~CError();

	virtual CError* Clone();
#ifdef _WIN32
	virtual void Serialize(CArchive& ar);
#endif //_WIN32
	CError& operator=(CError& other);

private:
	DWORD		m_dErrorCode;
	CStdString		m_strErrorDescription;
};

#endif // !defined(AFX_ERROR_H__5CE81141_1051_4F7C_BF64_E586F5B64A25__INCLUDED_)
