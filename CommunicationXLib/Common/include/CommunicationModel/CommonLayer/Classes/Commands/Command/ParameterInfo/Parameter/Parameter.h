// Parameter.h: Schnittstelle f�r die Klasse CParameter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Parameter_H__8CA79746_3FEC_4046_B3A5_5BE4A8990169__INCLUDED_)
#define AFX_Parameter_H__8CA79746_3FEC_4046_B3A5_5BE4A8990169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/ParameterBase.h>
#include <Classes/XXMLFile.h>

class CParameter  : public CParameterBase
{
public:
	CParameter();
	virtual ~CParameter();

	CParameter& operator=(CParameter& other);
	void Reset();

	bool StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pParentElement);
	bool LoadFromXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pParameterElement);
};

#endif // !defined(AFX_Parameter_H__8CA79746_3FEC_4046_B3A5_5BE4A8990169__INCLUDED_)
