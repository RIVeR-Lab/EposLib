// StructuredObjectEntry.cpp: Implementierung der Klasse CStructuredObjectEntry.
//
//////////////////////////////////////////////////////////////////////
#include <Classes/XML_PARSER.h>
#include <ObjectDictionary/ObjectFilter.h>
#include <ObjectDictionary/ObjectEntry.h>
#include <ObjectDictionary/ObjectEntryIterator.h>
#include <ObjectDictionary/StructuredObjectEntry.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CStructuredObjectEntry::CStructuredObjectEntry()
{
	Init();
}

CStructuredObjectEntry::~CStructuredObjectEntry()
{
	DeleteObjectEntryList();
}

void CStructuredObjectEntry::Serialize(CArchive &ar)
{
}

CObjectEntryIterator* CStructuredObjectEntry::CreateObjectEntryIterator()
{
	CObjectEntryIterator* pIterator = NULL;

	pIterator = new CObjectEntryIterator();
	pIterator->Init(&m_ObjectEntryList);

	return pIterator;
}

CStructuredObjectEntry& CStructuredObjectEntry::operator=(CStructuredObjectEntry& other)
{
	CObjectEntry* pObjectEntry;

	if(this != &other)
	{
		*((CObjectEntryBase*)this) = *((CObjectEntryBase*)&other);

		DeleteObjectEntryList();

		for( tObjectEntryList::iterator it=other.m_ObjectEntryList.begin();
			it!=other.m_ObjectEntryList.end(); it++)
		{
			pObjectEntry = (CObjectEntry*)(*it);
			AddObject(pObjectEntry);
		}
	}

	return *this;
}

BOOL CStructuredObjectEntry::InitObjectFilter(CObjectFilter *pObjectFilter)
{
	BOOL oResult = TRUE;

	m_pObjectFilter = pObjectFilter;

    //Set object filter to all object entries of this structured object entry
	for( tObjectEntryList::iterator it=m_ObjectEntryList.begin();
			it!=m_ObjectEntryList.end(); it++)
	{
		CObjectEntryBase* pObject = (*it);
        pObject->InitObjectFilter(pObjectFilter);
	}
	return oResult;
}

CObjectEntryBase* CStructuredObjectEntry::Clone()
{
	CStructuredObjectEntry* pNewObjectEntry;

	pNewObjectEntry = new CStructuredObjectEntry();
	*pNewObjectEntry = *this;

	return (CObjectEntryBase*)pNewObjectEntry;
}

BOOL CStructuredObjectEntry::AddObject(CObjectEntry *pObject)
{
	CObjectEntry* pNewObject;
	BOOL oResult = FALSE;

	if(pObject)
	{
		pNewObject = (CObjectEntry*)pObject->Clone();
		m_ObjectEntryList.push_back(pNewObject);
		oResult = TRUE;
	}

	return oResult;
}

BOOL CStructuredObjectEntry::DeleteObjectEntryList()
{
	BOOL oResult = TRUE;

	for( tObjectEntryList::iterator it=m_ObjectEntryList.begin();
			it!=m_ObjectEntryList.end(); it++)
	{
		CObjectEntryBase* pObject = (*it);
		delete pObject;
	}

	m_ObjectEntryList.clear();

	return oResult;
}

BOOL CStructuredObjectEntry::Reset()
{
	BOOL oResult = FALSE;

	if(CObjectEntryBase::Reset())
	{
		DeleteObjectEntryList();
		oResult = TRUE;
	}

	return oResult;
}

CObjectEntryBase* CStructuredObjectEntry::SearchObject(WORD index,WORD subIndex)
{
	 for( tObjectEntryList::iterator it=m_ObjectEntryList.begin();
			it!=m_ObjectEntryList.end(); it++)
	{
		CObjectEntryBase* pObject = (*it);
		if(pObject)
		{
			CObjectEntryBase* pFoundObject = pObject->SearchObject(index,subIndex);
			if(pFoundObject) return pFoundObject;
		}
	}

	return NULL;
}

BOOL CStructuredObjectEntry::ResetValue()
{
	BOOL oResult = TRUE;

	for( tObjectEntryList::iterator it=m_ObjectEntryList.begin();
			it!=m_ObjectEntryList.end(); it++)
	{
		CObjectEntryBase* pObject = (*it);
		pObject->ResetValue();
	}

	return oResult;
}

BOOL CStructuredObjectEntry::SetFirstSubObjectEntryValues(CObjectEntryBase *pObjectEntry)
{
	tObjectEntryList::iterator it=m_ObjectEntryList.begin();

	if(it!=m_ObjectEntryList.end())
	{
		CObjectEntryBase* pFirstObjectEntry = (*it);

		*pFirstObjectEntry = *pObjectEntry;

		return TRUE;
	}

	return FALSE;
}

int CStructuredObjectEntry::GetNumberOfSubObjects()
{
	return m_ObjectEntryList.size();
}

BOOL CStructuredObjectEntry::StoreObjectDictionarySchema(XML_PARSER* pXmlParser)
{
	CObjectEntryBase* pObjectEntry;
	CStdString strIndex = "";
	CStdString strParameterName = "";
	CStdString strObjectType = "";

	if(pXmlParser)
	{
		//StructuredObjectEntry
		pXmlParser->AddNewLineTab();
		pXmlParser->Add_LastChildNode(_T("StructuredObjectEntry"));
		pXmlParser->IncTabLevel();

		//Prepare Attributes
		GetIndex(strIndex);
		GetParameterName(strParameterName);
		GetObjectType(strObjectType);

		//StructuredObjectEntry Attributes
		pXmlParser->Set_Attribute(_T("Index"),strIndex);
		pXmlParser->Set_Attribute(_T("ParameterName"),strParameterName);
		pXmlParser->Set_Attribute(_T("ObjectType"),strObjectType);

		//StructuredObjectEntry Elements
		for( tObjectEntryList::iterator it=m_ObjectEntryList.begin();
			it != m_ObjectEntryList.end(); it++)
		{
			pObjectEntry = (*it);

			if(!pObjectEntry->StoreObjectDictionarySchema(pXmlParser)) return FALSE;
		}

		//End ObjectEntry
		pXmlParser->DecTabLevel();
		pXmlParser->AddNewLineTab();
		pXmlParser->Go_to_Parent();

		return TRUE;
	}

	return FALSE;
}

void CStructuredObjectEntry::Parse_XML_Document(XML_PARSER* pXmlParser)
{
	BOOL moreObjectEntries;
	CObjectEntry* pObjectEntry;

	if(pXmlParser)
	{
		//*Attributes*
		//Index
		if(pXmlParser->Is_Having_Attribute(_T("Index")))
		{
			SetIndex(pXmlParser->Get_Attribute_Value());
		}

		//ParameterName
		if(pXmlParser->Is_Having_Attribute(_T("ParameterName")))
		{
			m_strParameterName = pXmlParser->Get_Attribute_Value();
		}

		//ObjectType
		if(pXmlParser->Is_Having_Attribute(_T("ObjectType")))
		{
			SetObjectType(pXmlParser->Get_Attribute_Value());
		}

		//ObjectEntry
		DeleteObjectEntryList();
		if(pXmlParser->Go_to_Child(_T("ObjectEntry")))
		{
			moreObjectEntries = TRUE;
			while(moreObjectEntries)
			{
				pObjectEntry = new CObjectEntry();
				pObjectEntry->Parse_XML_Document(pXmlParser);
				m_ObjectEntryList.push_back(pObjectEntry);

				if(!pXmlParser->Go_Forward(_T("ObjectEntry")))
				{
					moreObjectEntries = FALSE;
				}
			}
			pXmlParser->Go_to_Parent();
		}
	}
}

