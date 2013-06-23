// ParameterSet.cpp: Implementierung der Klasse CParameterSet.
//
//////////////////////////////////////////////////////////////////////
#include <PersistenceReaderWriter/XmlWriter.h>
#include <PersistenceReaderWriter/XmlReader.h>
#include <ParameterSet/ParameterSetEntry.h>
#include <ParameterSet/ParameterSet.h>


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CParameterSet::CParameterSet()
{

}

CParameterSet::~CParameterSet()
{
	Reset();
}

CParameterSet* CParameterSet::Clone()
{
	CParameterSet* pClonedParameterSet = NULL;

	pClonedParameterSet = new CParameterSet();
	*pClonedParameterSet = *this;

	return pClonedParameterSet;
}

CParameterSet& CParameterSet::operator=(CParameterSet& other)
{
	if(this != &other)
	{
		//Reset
		Reset();

		for(tParameterList::iterator it=other.m_ParameterEntryList.begin();
					it!=other.m_ParameterEntryList.end(); it++)
		{
			CParameterSetEntry* pOtherParameterEntry = (*it);

			CParameterSetEntry* pParameterEntry = new CParameterSetEntry();
			*pParameterEntry = *pOtherParameterEntry;
			m_ParameterEntryList.push_back(pParameterEntry);
		}
	}

	return *this;
}

BOOL CParameterSet::operator==(CParameterSet& other)
{
	BOOL oResult = TRUE;

	if(this != &other)
	{

		tParameterList::iterator it2=m_ParameterEntryList.begin();
		for(tParameterList::iterator it=other.m_ParameterEntryList.begin();
							it!=other.m_ParameterEntryList.end(); it++)
		{
			CParameterSetEntry* pOtherParameterEntry = (*it);
			if( it2 != m_ParameterEntryList.end() )
			{
				CParameterSetEntry* pParameterEntry = (*it2++);
				if(!(*pParameterEntry == *pOtherParameterEntry))
				{
					oResult = FALSE;
					break;
				}
			}
		}
	}
	
	return oResult;
}

BOOL CParameterSet::Reset()
{
	BOOL oResult = TRUE;

	for(tParameterList::iterator it=m_ParameterEntryList.begin();
		it!=m_ParameterEntryList.end(); it++)
	{
		CParameterSetEntry* pParameterEntry = (*it);
		delete pParameterEntry;
	}

	m_ParameterEntryList.clear();

	return oResult;
}

BOOL CParameterSet::ResetValues()
{
	BOOL oResult = TRUE;

	for(tParameterList::iterator it=m_ParameterEntryList.begin();
			it!=m_ParameterEntryList.end(); it++)
	{
		CParameterSetEntry* pParameterEntry = (*it);
		pParameterEntry->ResetValue();
	}

	return oResult;
}

CParameterSetEntry* CParameterSet::FindParameter(CStdString p_Name)
{
	CParameterSetEntry* pFoundParameterEntry = NULL;
	CStdString name = _T("");

	for(tParameterList::iterator it=m_ParameterEntryList.begin();
			it!=m_ParameterEntryList.end(); it++)
	{
		CParameterSetEntry* pParameterEntry = (*it);
		name = pParameterEntry->GetName();
		if(p_Name.CompareNoCase(name) == 0)
		{
			pFoundParameterEntry = pParameterEntry;
			break;
		}
	}

	return pFoundParameterEntry;
}

BOOL CParameterSet::InitParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, BOOL p_oPersistent)
{
	BOOL oResult = FALSE;

	if(AddParameter(p_Name, p_pValue, p_ulSize, p_oPersistent))
	{
		oResult = SetParameter(p_Name, p_pValue, p_ulSize);
	}

	return oResult;
}
	
BOOL CParameterSet::InitParameter(CStdString p_Name, CStdString p_Value, BOOL p_oPersistent)
{
	BOOL oResult = FALSE;

	if(AddParameter(p_Name, p_Value, p_oPersistent))
	{
		oResult = SetParameter(p_Name, p_Value);
	}

	return oResult;
}

BOOL CParameterSet::AddParameter(CStdString p_Name, BYTE* p_pDefaultValue, DWORD p_ulSize, BOOL p_oPersistent)
{
	CParameterSetEntry* pParameterEntry = NULL;
	BOOL oResult = FALSE;

	if(!FindParameter(p_Name))
	{
		pParameterEntry = new CParameterSetEntry();
		pParameterEntry->Init(p_Name, p_pDefaultValue, p_ulSize, p_oPersistent);
		m_ParameterEntryList.push_back(pParameterEntry);
		oResult = TRUE;
	}

	return oResult;
}

BOOL CParameterSet::AddParameter(CStdString p_Name, CStdString p_DefaultValue, BOOL p_oPersistent)
{
	CParameterSetEntry* pParameterEntry = NULL;
	BOOL oResult = FALSE;

	if(!FindParameter(p_Name))
	{
		pParameterEntry = new CParameterSetEntry();
		pParameterEntry->Init(p_Name, p_DefaultValue, p_oPersistent);
		m_ParameterEntryList.push_back(pParameterEntry);
		oResult = TRUE;
	}

	return oResult;
}


BOOL CParameterSet::DeleteParameter(CStdString p_Name)
{
	CStdString name = _T("");
	BOOL oResult = FALSE;

	for(tParameterList::iterator it=m_ParameterEntryList.begin();
			it!=m_ParameterEntryList.end(); it++)
	{
		CParameterSetEntry* pParameterEntry = (*it);
		name = pParameterEntry->GetName();
		if(p_Name == name)
		{
			m_ParameterEntryList.erase(it);
			delete pParameterEntry;
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}


BOOL CParameterSet::GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	CParameterSetEntry* pParameterEntry = NULL;
	BOOL oResult = FALSE;

	pParameterEntry = FindParameter(p_Name);
	if(pParameterEntry)
	{
		oResult = pParameterEntry->GetParameter(p_pValue,p_ulSize);
	}

	return oResult;
}

BOOL CParameterSet::GetParameter(CStdString p_Name, CStdString& p_rValue)
{
	CParameterSetEntry* pParameterEntry = NULL;
	BOOL oResult = FALSE;

	pParameterEntry = FindParameter(p_Name);
	if(pParameterEntry)
	{
		oResult = pParameterEntry->GetParameter(p_rValue);
	}

	return oResult;
}

BOOL CParameterSet::SetParameter(CStdString p_Name, CStdString p_Value)
{
	CParameterSetEntry* pParameterEntry = NULL;
	BOOL oResult = FALSE;

	pParameterEntry = FindParameter(p_Name);
	if(pParameterEntry)
	{
		oResult = pParameterEntry->SetParameter(p_Value);
	}

	return oResult;
}

BOOL CParameterSet::SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	CParameterSetEntry* pParameterEntry = NULL;
	BOOL oResult = FALSE;

	pParameterEntry = FindParameter(p_Name);
	if(pParameterEntry)
	{
		oResult = pParameterEntry->SetParameter(p_pValue,p_ulSize);
	}

	return oResult;
}

BOOL CParameterSet::WriteTo(CXmlWriter* p_pXmlWriter)
{
	BOOL oResult = FALSE;
	
	if(p_pXmlWriter)
	{
		//Init
		oResult = TRUE;

		//Start Element
		if(oResult && !p_pXmlWriter->WriteStartElement(_T("ParameterSet"))) oResult = FALSE;

		//Parametes		
		for(tParameterList::iterator it=m_ParameterEntryList.begin(); it!=m_ParameterEntryList.end(); it++)
		{
			CParameterSetEntry* pParameterEntry = (*it);
			if(pParameterEntry && pParameterEntry->IsPersistent())
			{
				if(!pParameterEntry->WriteTo(p_pXmlWriter)) oResult = FALSE;
			}
		}

		//End Element
		if(oResult && !p_pXmlWriter->WriteFullEndElement()) oResult = FALSE;
	}

	return oResult;
}

BOOL CParameterSet::ReadFrom(CXmlReader* p_pXmlReader)
{
	BOOL oResult = FALSE;
	
	if(p_pXmlReader)
	{
		//Start Element
		if(p_pXmlReader->ReadStartElement(_T("ParameterSet"),0))
		{
			//Init
			oResult = TRUE;

			//Parametes			
			for(tParameterList::iterator it=m_ParameterEntryList.begin(); it!=m_ParameterEntryList.end(); it++)
			{
				CParameterSetEntry* pParameterEntry = (*it);
				if(pParameterEntry && pParameterEntry->IsPersistent())
				{
					if(!pParameterEntry->ReadFrom(p_pXmlReader)) oResult = FALSE;
				}
			}

			//End Element
			p_pXmlReader->ReadEndElement();
		}
	}

	return oResult;
}
