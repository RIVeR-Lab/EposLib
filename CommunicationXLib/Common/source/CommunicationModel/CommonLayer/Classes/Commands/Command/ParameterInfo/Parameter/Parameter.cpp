// Parameter.cpp: Implementierung der Klasse CParameter.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/ParameterBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/Parameter.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CParameter::CParameter()
{
	Reset();
}

CParameter::~CParameter()
{
}

//*********************************************************************
CParameter& CParameter::operator=(CParameter& other)
{
	if(this != &other)
	{
		*((CParameterBase*)this) = *((CParameterBase*)&other);
	}
	return *this;
}
//*********************************************************************
void CParameter::Reset()
{
	CParameterBase::Reset();
}

//*********************************************************************
bool CParameter::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pParentElement)
{
	CXXMLFile::CElement* pElement;

	if(pFile && pParentElement)
	{
		//Parameter Elements
		pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
		pFile->SetText(pElement, "Parameter");
		pElement->SetAt("Id", GetIndexStr());
		pElement->SetAt("Name", GetName());
		pElement->SetAt("Type", GetTypeStr());
		pElement->SetAt("Value", GetValueStr(OVF_HEX));

		if(!m_oEditable) pElement->SetAt("Editable", GetEditableStr());
		if(!m_oVisible) pElement->SetAt("Visible", GetVisibleStr());

		return true;
	}

	return false;
}

//*********************************************************************
bool CParameter::LoadFromXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pParameterElement)
{
	CStdString typeStr;
	CStdString valueStr;
	CStdString indexStr;
	CStdString editableStr;
	CStdString visibleStr;
	CStdString name;

	if(pFile && pParameterElement)
	{
		if(pParameterElement->m_Text.CompareNoCase(_T("Parameter")) == 0)
		{
			//Parameter Element
			tMmcStringToStringMap::iterator it;

			if((it=pParameterElement->AttributeToValue.find("Id")) == 
				pParameterElement->AttributeToValue.end() )
				return false;
			
			indexStr = (*it).second;

			if(!InitIndex(indexStr)) 
				return false;

			if((it=pParameterElement->AttributeToValue.find("Name")) == 
				pParameterElement->AttributeToValue.end() )
				return false;
			
			name = (*it).second;

			if(!InitName(name)) 
				return false;

			if((it=pParameterElement->AttributeToValue.find("Type")) == 
				pParameterElement->AttributeToValue.end() )
				return false;
			
			typeStr = (*it).second;

			if(!InitType(typeStr)) 
				return false;

			if((it=pParameterElement->AttributeToValue.find("Value")) == 
				pParameterElement->AttributeToValue.end() )
				return false;

			valueStr = (*it).second;

			if(!SetValue(valueStr)) return false;

			m_oVisible = true;
			if((it=pParameterElement->AttributeToValue.find("Visible")) != 
				pParameterElement->AttributeToValue.end() )
			{
				visibleStr = (*it).second;
				if( !InitVisible(visibleStr) )
					m_oVisible = false;
			}

			//Optional Elements
			m_oVisible = true;
			if((it=pParameterElement->AttributeToValue.find("Editable")) != 
				pParameterElement->AttributeToValue.end() )
			{
				editableStr = (*it).second;
				if( InitVisible(editableStr) )
					m_oEditable = false;
			}

			return true;
		}
	}

	return false;
}
