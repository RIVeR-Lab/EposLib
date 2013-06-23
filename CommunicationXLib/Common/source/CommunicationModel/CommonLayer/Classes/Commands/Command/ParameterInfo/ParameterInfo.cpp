// ParameterInfo.cpp: Implementierung der Klasse CParameterInfo.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <algorithm>

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/ParameterBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/Parameter.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/ReturnParameter.h>

#include "ParameterInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CParameterInfo::CParameterInfo()
{
}

CParameterInfo::~CParameterInfo()
{
	Reset();
}

void CParameterInfo::ResetParameterData()
{
	for(std::list<CParameter*>::iterator it = m_ParameterList.begin(); it != m_ParameterList.end(); it++)
	{
		CParameterBase* pParameter = (*it);
		pParameter->ResetData();
	}
}

void CParameterInfo::ResetReturnParameterData()
{

	for(std::list<CReturnParameter*>::iterator it = m_ReturnParameterList.begin(); it != m_ReturnParameterList.end(); it++)
	{
		CReturnParameter* pParameter = (*it);
		pParameter->ResetData();
	}
}

#ifdef WINVER
void CParameterInfo::Serialize(CArchive& ar)
{
	std::list<CParameter*>::iterator pos;
	std::list<CReturnParameter*>::iterator pos1;
    CParameter* pParameter;
	CReturnParameter* pReturnParameter;
	size_t count;

	if(ar.IsStoring())
	{
		//Parameter List
		count = m_ParameterList.size();
		ar << count;
		for(pos = m_ParameterList.begin(); pos != m_ParameterList.end(); pos++)
		{
			pParameter = *pos;
			pParameter->Serialize(ar);
		}

		//ReturnParameter List
		count = m_ReturnParameterList.size();
		ar << count;
		for(pos1 = m_ReturnParameterList.begin(); pos1 != m_ReturnParameterList.end(); pos1++)
		{
			pReturnParameter = *pos1;
			pReturnParameter->Serialize(ar);
		}
	}
	else
	{
		//Parameter List
		DeleteParameterList();
		ar >> count;
		for(int i=0;i<count;i++)
		{
			pParameter = new CParameter();
			pParameter->Serialize(ar);
			m_ParameterList.push_back(pParameter);
		}

		//ReturnParameter List
		DeleteReturnParameterList();
		ar >> count;
		for(int i=0;i<count;i++)
		{
			pReturnParameter = new CReturnParameter();
			pReturnParameter->Serialize(ar);
			m_ReturnParameterList.push_back(pReturnParameter);
		}
	}
}
#endif

int CParameterInfo::GetNbOfParameter()
{
	return (int)m_ParameterList.size();
}

CStdString CParameterInfo::GetParameterName(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetName();
	}

	return "";
}

EObjectDataType CParameterInfo::GetParameterType(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetType();
	}

	return ODT_UNKNOWN;
}

DWORD CParameterInfo::GetParameterLength(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetLength();
	}

	return 0;
}

BOOL CParameterInfo::SetParameterData(int paramIndex, void* data, size_t length)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->SetData(data, (DWORD)length);
	}

	return FALSE;
}

CParameter* CParameterInfo::FindParameter(int paramIndex)
{
	for(std::list<CParameter*>::iterator it = m_ParameterList.begin(); it != m_ParameterList.end(); it++)
	{
		CParameter* pParameter = (*it);
		if(pParameter->GetIndex() == paramIndex)
		{
			return pParameter;
		}
	}

	return 0;
}

CReturnParameter* CParameterInfo::FindReturnParameter(int paramIndex)
{
	for(std::list<CReturnParameter*>::iterator it = m_ReturnParameterList.begin(); it != m_ReturnParameterList.end(); it++)
	{
		CReturnParameter* pParameter = (*it);
		if(pParameter->GetIndex() == paramIndex)
		{
			return pParameter;
		}
	}

	return 0;
}

int CParameterInfo::GetNbOfReturnParameter()
{
	return (int)m_ReturnParameterList.size();
}

CStdString CParameterInfo::GetReturnParameterName(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetName();
	}

	return "";
}

EObjectDataType CParameterInfo::GetReturnParameterType(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetType();
	}

	return ODT_UNKNOWN;
}

DWORD CParameterInfo::GetReturnParameterLength(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetLength();
	}

	return 0;
}

BOOL CParameterInfo::SetReturnParameterData(int paramIndex, void *data, size_t length)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->SetData(data, (DWORD)length);
	}

	return FALSE;
}

BOOL CParameterInfo::GetReturnParameterData(int paramIndex, void *data, DWORD length)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetData(data, length);
	}

	return FALSE;
}

BOOL CParameterInfo::GetParameterData(int paramIndex, void *data, DWORD length)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetData(data, length);
	}

	return FALSE;
}

void CParameterInfo::Reset()
{
	DeleteParameterList();
	DeleteReturnParameterList();
}

void CParameterInfo::AddParameter(int paramIndex, CStdString name, EObjectDataType type)
{
	const DWORD dArraySize = 1;
	const BOOL editable = TRUE;
	const BOOL visible = TRUE;

	AddParameter(paramIndex, name, type, dArraySize, editable, visible);
}

void CParameterInfo::AddParameter(int paramIndex, CStdString name, EObjectDataType type, BOOL visible, BOOL editable)
{
	const DWORD dArraySize = 1;

	AddParameter(paramIndex, name, type, dArraySize, visible, editable);
}

void CParameterInfo::AddParameter(int paramIndex, CStdString name, EObjectDataType type, DWORD dArraySize)
{
	const BOOL editable = TRUE;
	const BOOL visible = TRUE;

	AddParameter(paramIndex, name, type, dArraySize, editable, visible);
}

void CParameterInfo::AddParameter(int paramIndex, CStdString name, EObjectDataType type, DWORD dArraySize, BOOL visible, BOOL editable)
{
	CParameter* pParameter = 0;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		pParameter->Init(paramIndex, name, type, dArraySize, visible, editable);
	}
	else
	{
		pParameter = new CParameter();
		pParameter->Init(paramIndex, name, type, dArraySize, visible, editable);
		m_ParameterList.push_back(pParameter);
	}
}

void CParameterInfo::AddReturnParameter(int paramIndex, CStdString name, EObjectDataType type)
{
	const DWORD dArraySize = 1;
	const BOOL visible = TRUE;

	AddReturnParameter(paramIndex, name, type, dArraySize, visible);
}

void CParameterInfo::AddReturnParameter(int paramIndex, CStdString name, EObjectDataType type, BOOL visible)
{
	const DWORD dArraySize = 1;

	AddReturnParameter(paramIndex, name, type, dArraySize, visible);
}

void CParameterInfo::AddReturnParameter(int paramIndex, CStdString name, EObjectDataType type, DWORD dArraySize)
{
	const BOOL visible = TRUE;

	AddReturnParameter(paramIndex, name, type, dArraySize, visible);
}

void CParameterInfo::AddReturnParameter(int paramIndex, CStdString name, EObjectDataType type, DWORD dArraySize, BOOL visible)
{
	const BOOL oEditable = FALSE;

	CReturnParameter* pReturnParameter = 0;

	pReturnParameter = FindReturnParameter(paramIndex);
	if(pReturnParameter)
	{
		pReturnParameter->Init(paramIndex, name, type, dArraySize, visible, oEditable);
	}
	else
	{
		pReturnParameter = new CReturnParameter();
		pReturnParameter->Init(paramIndex, name, type, dArraySize, visible, oEditable);
		m_ReturnParameterList.push_back(pReturnParameter);
	}
}

CParameterInfo& CParameterInfo::operator=(CParameterInfo& other)
{
    CParameter* pNewParameter = 0;
    CParameter* pParameter = 0;
    CReturnParameter* pNewReturnParameter = 0;
    CReturnParameter* pReturnParameter = 0;

	if(this != &other)
	{
		Reset();

		//ParameterList
		for(std::list<CParameter*>::iterator it = other.m_ParameterList.begin(); it != other.m_ParameterList.end(); it++)
		{
			CParameter* pParameter = (*it);
			if(pParameter)
			{
				pNewParameter = new CParameter();
				*pNewParameter = *pParameter;
				m_ParameterList.push_back(pNewParameter);
			}
		}

		//ReturnParameterList
		for(std::list<CReturnParameter*>::iterator it = other.m_ReturnParameterList.begin(); it != other.m_ReturnParameterList.end(); it++)
		{
			CReturnParameter* pReturnParameter = (*it);
			if(pReturnParameter)
			{
				pNewReturnParameter = new CReturnParameter();
				*pNewReturnParameter = *pReturnParameter;
				m_ReturnParameterList.push_back(pNewReturnParameter);
			}
		}
	}
	return *this;
}

void CParameterInfo::DeleteParameterList()
{
	for(std::list<CParameter*>::iterator it = m_ParameterList.begin(); it != m_ParameterList.end(); it++)
	{
		CParameterBase* pParameter = (*it);
		delete pParameter;
	}
	m_ParameterList.clear();
}

void CParameterInfo::DeleteReturnParameterList()
{
	for(std::list<CReturnParameter*>::iterator it = m_ReturnParameterList.begin(); it != m_ReturnParameterList.end(); it++)
	{
	    CReturnParameter* pReturnParameter = (*it);
		delete pReturnParameter;
	}
	m_ReturnParameterList.clear();
}

BOOL CParameterInfo::StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pParentElement, BOOL checkVisibility)
{
	CXXMLFile::CElement* pElement;
	int parameterCount = 0;

	if(pFile && pParentElement)
	{
		//ParameterInfo Elements
		pElement = (CXXMLFile::CElement*)pFile->AddElement(pParentElement);
		pFile->SetText(pElement, "ParameterInfo");

		//Parameter Elements
		for(std::list<CParameter*>::iterator it = m_ParameterList.begin(); it != m_ParameterList.end(); it++)
		{
			CParameter* pParameter = (*it);
			if(pParameter && (!checkVisibility || pParameter->IsVisible()))
			{
				parameterCount++;
				if(!pParameter->StoreToXMLFile(pFile, pElement)) return FALSE;
			}
		}

		//Return Parameter
		for(std::list<CReturnParameter*>::iterator it = m_ReturnParameterList.begin(); it != m_ReturnParameterList.end(); it++)
		{
			CReturnParameter* pReturnParameter = (*it);
			if(pReturnParameter && (!checkVisibility || pReturnParameter->IsVisible()))
			{
				parameterCount++;
				if(!pReturnParameter->StoreToXMLFile(pFile, pElement)) return FALSE;
			}
		}

		//Delete Element if no parameters
		if(parameterCount == 0)
		{
			pFile->DeleteElement(pParentElement, pElement);
			pElement = pParentElement;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CParameterInfo::LoadFromXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pParameterInfoElement)
{
	CParameter* pParameter;
	CReturnParameter* pReturnParameter;
	CXXMLFile::CElementPart* pElementPart;
	CXXMLFile::CElement* pParameterElement;

	if(pFile && pParameterInfoElement)
	{
		if(pParameterInfoElement->m_Text.CompareNoCase(_T("ParameterInfo")) == 0)
		{
			//ParameterInfo Elements
			//No Attributes

			//Parameter Elements
			for(std::list<CXXMLFile::CElementPart*>::iterator it = pParameterInfoElement->begin(); it != pParameterInfoElement->end(); it++)
			{
				pElementPart = (*it);
				if(pFile->IsElement(pElementPart))
				{
					pParameterElement = (CXXMLFile::CElement*) pElementPart;
					if(pParameterElement->m_Text.CompareNoCase(_T("Parameter")) == 0)
					{
						//Parameter
						pParameter = new CParameter();
						if(!pParameter->LoadFromXMLFile(pFile, pParameterElement))
						{
							delete pParameter;
							return FALSE;
						}
						m_ParameterList.push_back(pParameter);
					}
					else if(pParameterElement->m_Text.CompareNoCase(_T("ReturnParameter")) == 0)
					{
						//ReturnParameter
						pReturnParameter = new CReturnParameter();
						if(!pReturnParameter->LoadFromXMLFile(pFile, pParameterElement))
						{
							delete pReturnParameter;
							return FALSE;
						}
						m_ReturnParameterList.push_back(pReturnParameter);
					}
				}
			}

			return TRUE;
		}
	}
	return FALSE;
}

CStdString CParameterInfo::GetParameterTypeStr(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetTypeStr();
	}

	return "";
}

CStdString CParameterInfo::GetParameterDataStr(int paramIndex, EObjectValueStringFormat format)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetValueStr(format);
	}

	return "";
}

CStdString CParameterInfo::GetReturnParameterTypeStr(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetTypeStr();
	}

	return "";
}

CStdString CParameterInfo::GetReturnParameterDataStr(int paramIndex, EObjectValueStringFormat format)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->GetValueStr(format);
	}

	return "";
}

BOOL CParameterInfo::IsParameterExisting(int paramIndex)
{
	if(FindParameter(paramIndex))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CParameterInfo::IsReturnParameterExisting(int paramIndex)
{
	if(FindReturnParameter(paramIndex))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CParameterInfo::IsParameterEditable(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->IsEditable();
	}

	return FALSE;
}

BOOL CParameterInfo::IsParameterVisible(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->IsVisible();
	}

	return FALSE;
}

BOOL CParameterInfo::IsReturnParameterVisible(int paramIndex)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->IsVisible();
	}

	return FALSE;
}

BOOL CParameterInfo::SetParameterDataString(int paramIndex, CStdString strValue, BOOL showMsg)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->SetValue(strValue, showMsg);
	}

	return FALSE;
}

BOOL CParameterInfo::GetParameterDataString(int paramIndex, CStdString* pStrValue, EObjectValueStringFormat format)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(paramIndex);
	if(pParameter && pStrValue)
	{
		*pStrValue = pParameter->GetValueStr(format);
		return TRUE;
	}

	return FALSE;
}

BOOL CParameterInfo::SetReturnParameterDataString(int paramIndex, CStdString strValue, BOOL showMsg)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter)
	{
		return pParameter->SetValue(strValue, showMsg);
	}

	return FALSE;
}

BOOL CParameterInfo::GetReturnParameterDataString(int paramIndex, CStdString* pStrValue, EObjectValueStringFormat format)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(paramIndex);
	if(pParameter && pStrValue)
	{
		*pStrValue = pParameter->GetValueStr(format);
		return TRUE;
	}

	return FALSE;
}

BOOL CParameterInfo::IsEditableParameterExisting()
{
	for(std::list<CParameter*>::iterator it = m_ParameterList.begin(); it != m_ParameterList.end(); it++)
	{
		CParameterBase* pParameter = (*it);
		if(pParameter->IsEditable())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CParameterInfo::IsVisibleParameterExisting()
{
	for(std::list<CParameter*>::iterator it = m_ParameterList.begin(); it != m_ParameterList.end(); it++)
	{
		CParameterBase* pParameter = (*it);
		if(pParameter->IsVisible())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CParameterInfo::IsVisibleReturnParameterExisting()
{
	for(std::list<CReturnParameter*>::iterator it = m_ReturnParameterList.begin(); it != m_ReturnParameterList.end(); it++)
	{
		CReturnParameter* pReturnParameter = (*it);
		if(pReturnParameter->IsVisible())
		{
			return TRUE;
		}
	}

	return FALSE;
}

DWORD CParameterInfo::GetParameterArraySize(int p_lParamIndex)
{
	CParameterBase* pParameter;

	pParameter = FindParameter(p_lParamIndex);
	if(pParameter)
	{
		return pParameter->GetArraySize();
	}

	return 0;
}

DWORD CParameterInfo::GetReturnParameterArraySize(int p_lParamIndex)
{
	CParameterBase* pParameter;

	pParameter = FindReturnParameter(p_lParamIndex);
	if(pParameter)
	{
		return pParameter->GetArraySize();
	}

	return 0;
}

BOOL CParameterInfo::FindReturnParameterIndex(CStdString p_Name, int& p_rIndex)
{
    int lIndex = 0;

    for(std::list<CReturnParameter*>::iterator it = m_ReturnParameterList.begin(); it != m_ReturnParameterList.end(); it++)
    {
        CReturnParameter* pReturnParameter = (*it);
        CStdString name = pReturnParameter->GetName();

        if(p_Name.CompareNoCase(name) == 0)
		{
			p_rIndex = lIndex;
			return TRUE;
		}

        lIndex++;
	}

	return FALSE;
}

BOOL CParameterInfo::FindParameterIndex(CStdString p_Name, int& p_rIndex)
{
    int lIndex = 0;

    for(std::list<CParameter*>::iterator it = m_ParameterList.begin(); it != m_ParameterList.end(); it++)
    {
    	CParameterBase* pParameter = (*it);
    	CStdString name = pParameter->GetName();

		if(p_Name.CompareNoCase(name) == 0)
		{
			p_rIndex = lIndex;
            return TRUE;
		}

        lIndex++;
	}

	return FALSE;
}
