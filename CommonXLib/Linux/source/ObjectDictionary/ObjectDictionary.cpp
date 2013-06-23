// ObjectDictionary.cpp: Implementierung der Klasse CObjectDictionary.
//
//////////////////////////////////////////////////////////////////////

#include <ObjectDictionary/ObjectEntryBase.h>
#include <ObjectDictionary/ObjectEntry.h>
#include <ObjectDictionary/StructuredObjectEntry.h>
#include <ObjectDictionary/ObjectDictionary.h>
#include <Ui/MmcMessageDialog.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CObjectDictionary::CObjectDictionary()
{
	Reset();
}

CObjectDictionary::~CObjectDictionary()
{
	DeleteObjectList();
}

void CObjectDictionary::Serialize(CArchive &ar)
{
}

CObjectEntryIterator* CObjectDictionary::CreateObjectEntryIterator()
{
	CObjectEntryIterator* pIterator = NULL;

	pIterator = new CObjectEntryIterator();
	pIterator->Init(&m_ObjectList);

	return pIterator;
}

CObjectEntryBaseIterator* CObjectDictionary::CreateObjectEntryBaseIterator()
{
	CObjectEntryBaseIterator* pIterator = NULL;

	pIterator = new CObjectEntryBaseIterator();
	pIterator->Init(&m_ObjectList);

	return pIterator;
}

BOOL CObjectDictionary::AddObject(CObjectEntryBase *pNewObjectEntry)
{
	CObjectEntryBase* pClonedObjectEntry = NULL;
	BOOL oResult = FALSE;

	if(pNewObjectEntry)
	{
		pClonedObjectEntry = pNewObjectEntry->Clone();
		if(pClonedObjectEntry)
		{
			pClonedObjectEntry->InitObjectFilter(&m_ObjectFilter);
			m_ObjectList.push_back(pClonedObjectEntry);
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CObjectDictionary::DeleteObjectList()
{
	BOOL oResult = TRUE;

	for(tObjectEntryBaseList::iterator it=m_ObjectList.begin();
		it!=m_ObjectList.end(); it++)
	{
		CObjectEntryBase* pObject = (*it);
		delete pObject;
	}
	m_ObjectList.clear();

	return oResult;
}

BOOL CObjectDictionary::Reset()
{
	BOOL oResult = FALSE;

	oResult = DeleteObjectList();

	return oResult;
}

CObjectDictionary& CObjectDictionary::operator=(CObjectDictionary& other)
{
	if(this != &other)
	{
		//ObjectList
		DeleteObjectList();
		for(tObjectEntryBaseList::iterator it=other.m_ObjectList.begin();
		it!=other.m_ObjectList.end(); it++)
		{
			CObjectEntryBase* pObjectEntryBase = (*it);
			AddObject(pObjectEntryBase);
		}
	}

	return *this;
}

BOOL CObjectDictionary::DeleteAllObjects()
{
	BOOL oResult = TRUE;

	for(tObjectEntryBaseList::iterator it=m_ObjectList.begin(); it!=m_ObjectList.end(); it++)
	{
		CObjectEntryBase* pObjectEntryBase = (*it);
		delete pObjectEntryBase;
	}

	m_ObjectList.clear();
	return oResult;
}

BOOL CObjectDictionary::SetInternalObjectFilter(BOOL p_oActive)
{
	BOOL oResult = TRUE;

	oResult = m_ObjectFilter.SetInternalObjectFilter(p_oActive);

	return oResult;
}

BOOL CObjectDictionary::SetObjectFilter(CObjectFilter objectFilter)
{
	BOOL oResult = TRUE;

	m_ObjectFilter = objectFilter;

	return oResult;
}

BOOL CObjectDictionary::ResetObjectFilter()
{
	BOOL oResult = TRUE;

	m_ObjectFilter.Reset();

	return oResult;
}

BOOL CObjectDictionary::ResetValues()
{
	CObjectEntryIterator* pIterator = NULL;
	CObjectEntryBase* pObjectEntry = NULL;
	BOOL oResult = TRUE;

	pIterator = CreateObjectEntryIterator();
	if(pIterator)
	{
		for(pIterator->First(); !pIterator->IsFinished(); pIterator->Next())
		{
			pObjectEntry = pIterator->Current();
			if(pObjectEntry)
			{
				pObjectEntry->ResetValue();
			}
		}
		delete pIterator;
	}

	return oResult;
}

CStdString CObjectDictionary::GetObjectName(WORD index,WORD subIndex)
{
	CObjectEntry* pObjectEntry = NULL;
	CStdString strParameterName = "";

	pObjectEntry = SearchObject(index,subIndex);
	if(pObjectEntry)
	{
		pObjectEntry->GetParameterName(strParameterName);
	}

	return strParameterName;
}

CObjectEntry* CObjectDictionary::SearchObject(WORD index,WORD subIndex)
{
	CObjectEntry* pObjectEntry;

	for(tObjectEntryBaseList::iterator it=m_ObjectList.begin(); it!=m_ObjectList.end(); it++)
	{
		CObjectEntryBase* pObjectEntryBase = (*it);
		if(pObjectEntryBase)
		{
			pObjectEntry = (CObjectEntry*)pObjectEntryBase->SearchObject(index,subIndex);
			if(pObjectEntry) return pObjectEntry;
		}
	}

	return NULL;
}

CObjectEntryBase* CObjectDictionary::SearchObject(WORD index)
{
	for(tObjectEntryBaseList::iterator it=m_ObjectList.begin(); it!=m_ObjectList.end(); it++)
	{
		CObjectEntryBase* pObjectEntryBase = (*it);
		if(pObjectEntryBase)
		{
			if(pObjectEntryBase->GetIndex() == index)
			{
				return pObjectEntryBase;
			}
		}
	}

	return NULL;
}

BOOL CObjectDictionary::GetLowLimit(WORD index,WORD subIndex,CStdString *strLowLimit)
{
	CObjectEntry* pObjectEntry;

	if(strLowLimit)
	{
		pObjectEntry = SearchObject(index,subIndex);
		if(pObjectEntry)
		{
			return pObjectEntry->GetLowLimit(*strLowLimit);
		}
	}

	return FALSE;
}
int CObjectDictionary::GetNumberOfSubObjects(WORD index)
{
	int iNbOfSubObjects = 0;
	CObjectEntryBase* pObjectEntryBase = NULL;

	pObjectEntryBase = SearchObject(index);
	if(pObjectEntryBase)
	{
		iNbOfSubObjects = pObjectEntryBase->GetNumberOfSubObjects();

	}

	return iNbOfSubObjects;
}

int CObjectDictionary::GetNumberOfObjects()
{
	int iNbOfObjects = 0;
	CObjectEntryIterator* pObjectEntryIterator = NULL;

	pObjectEntryIterator = CreateObjectEntryIterator();
	if(pObjectEntryIterator)
	{
		for(pObjectEntryIterator->First(); !pObjectEntryIterator->IsFinished(); pObjectEntryIterator->Next())
		{
			iNbOfObjects++;
		}
		delete pObjectEntryIterator;
	}

	return iNbOfObjects;
}

BOOL CObjectDictionary::StoreObjectDictionarySchema(CStdString* pXmlData)
{
	XML_PARSER xmlParser;

	if(pXmlData)
	{
		xmlParser.Reset_XML_Document();

		//Root
		xmlParser.Add_FirstChildNode(_T("ObjectDictionary"));
		//xmlParser.Set_Attribute(_T("xmlns"),_T("x-schema:DTMInformationSchema.xml"));
		//xmlParser.Set_Attribute(_T("xmlns:fdt"),_T("x-schema:FDTDataTypesSchema.xml"));
		//xmlParser.Set_Attribute(_T("xmlns:fdtappid"),_T("x-schema:FDTApplicationIdSchema.xml"));
		xmlParser.IncTabLevel();

		//ObjectEntryBase
		for(tObjectEntryBaseList::iterator it=m_ObjectList.begin(); it!=m_ObjectList.end(); it++)
		{
			CObjectEntryBase* pObjectEntryBase = (*it);
			if(!pObjectEntryBase->StoreObjectDictionarySchema(&xmlParser)) return FALSE;
		}

		//Root End
		xmlParser.DecTabLevel();
		xmlParser.AddNewLineTab();

//		xmlParser.Save_XML_Document("C:\\ObjectDictionary.xml");
		return xmlParser.Get_XML_Document(pXmlData);
	}

	return FALSE;
}

BOOL CObjectDictionary::LoadObjectDictionarySchema(CStdString* pXmlData)
{
	if(pXmlData)
	{
		DeleteObjectList();
		if(Load_XML_From_Buffer(pXmlData))
		{
//			Save_XML_Document("C:\\Test.xml");
			return TRUE;
		}
	}

	return FALSE;
}

void CObjectDictionary::Parse_XML_Document()
{
	CObjectEntry* pObjectEntry;
	CStructuredObjectEntry* pStructuredObjectEntry;

	//ObjectEntry
	if(Is_Tag(_T("<ObjectEntry>")))
	{
		pObjectEntry = new CObjectEntry();
		pObjectEntry->Parse_XML_Document(this);
		m_ObjectList.push_back(pObjectEntry);
	}

	//StructuredObjectEntry
	if(Is_Tag(_T("<StructuredObjectEntry>")))
	{
		pStructuredObjectEntry = new CStructuredObjectEntry();
		pStructuredObjectEntry->Parse_XML_Document(this);
		m_ObjectList.push_back(pStructuredObjectEntry);
	}
}
