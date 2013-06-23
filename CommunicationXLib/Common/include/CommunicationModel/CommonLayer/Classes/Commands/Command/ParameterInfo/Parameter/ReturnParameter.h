// ReturnParameter.h: Schnittstelle f�r die Klasse CReturnParameter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ReturnParameter_H__3F34E3A2_6333_4C03_B65D_3E956430CF3E__INCLUDED_)
#define AFX_ReturnParameter_H__3F34E3A2_6333_4C03_B65D_3E956430CF3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParameterBase.h"
#include <Classes/XXMLFile.h>

class CReturnParameter  :	public CParameterBase
{
public:
	virtual CReturnParameter& operator=(CReturnParameter& other);
	virtual void Reset();

	CReturnParameter();
	virtual ~CReturnParameter();

	BOOL StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pParentElement);
	BOOL LoadFromXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pParameterElement);

};

#endif // !defined(AFX_ReturnParameter_H__3F34E3A2_6333_4C03_B65D_3E956430CF3E__INCLUDED_)
