// ReturnParameter.cpp: Implementierung der Klasse CReturnParameter.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/ParameterBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/ReturnParameter.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CReturnParameter::CReturnParameter()
{
	Reset();
}

CReturnParameter::~CReturnParameter()
{

}

CReturnParameter& CReturnParameter::operator=(CReturnParameter& other)
{
	if(this != &other)
	{
		*((CParameterBase*)this) = *((CParameterBase*)&other);
	}
	return *this;
}

void CReturnParameter::Reset()
{
	CParameterBase::Reset();
}

BOOL CReturnParameter::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pParentElement)
{
	CXXMLFile::CElement* pElement;

	if(pFile && pParentElement)
	{
		//ReturnParameter Elements
		pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
		pFile->SetText(pElement, "ReturnParameter");
		pElement->SetAt("Id", GetIndexStr());
		pElement->SetAt("Name", GetName());
		pElement->SetAt("Type", GetTypeStr());
		pElement->SetAt("Value", GetValueStr(OVF_HEX));

		if(!m_oVisible) pElement->SetAt("Visible", GetVisibleStr());

		return TRUE;
	}

	return FALSE;
}

BOOL CReturnParameter::LoadFromXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pParameterElement)
{
	CStdString typeStr;
	CStdString valueStr;
	CStdString indexStr;
	//CStdString editableStr;
	CStdString visibleStr;
	CStdString name;

	if(pFile && pParameterElement)
	{
		if(pParameterElement->m_Text.CompareNoCase(_T("ReturnParameter")) == 0)
		{
			//Parameter Element
			tMmcStringToStringMap::iterator it;

			if((it=pParameterElement->AttributeToValue.find("Id")) == 
				pParameterElement->AttributeToValue.end() )
				return FALSE;
			
			indexStr = (*it).second;

			if(!InitIndex(indexStr)) 
				return FALSE;

			if((it=pParameterElement->AttributeToValue.find("Name")) == 
				pParameterElement->AttributeToValue.end() )
				return FALSE;
			
			name = (*it).second;

			if(!InitName(name)) 
				return FALSE;

			if((it=pParameterElement->AttributeToValue.find("Type")) == 
				pParameterElement->AttributeToValue.end() )
				return FALSE;
			
			typeStr = (*it).second;

			if(!InitType(typeStr)) 
				return FALSE;

			if((it=pParameterElement->AttributeToValue.find("Value")) == 
				pParameterElement->AttributeToValue.end() )
				return FALSE;

			valueStr = (*it).second;

			if(!SetValue(valueStr)) return FALSE;

			m_oVisible = TRUE;
			if((it=pParameterElement->AttributeToValue.find("Visible")) != 
				pParameterElement->AttributeToValue.end() )
			{
				visibleStr = (*it).second;
				if( !InitVisible(visibleStr) )
					m_oVisible = FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

