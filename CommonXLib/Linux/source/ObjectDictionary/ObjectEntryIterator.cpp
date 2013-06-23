#include <ObjectDictionary/StructuredObjectEntry.h>
#include <ObjectDictionary/ObjectEntryIterator.h>
#include <ObjectDictionary/ObjectEntry.h>

CObjectEntryIterator::CObjectEntryIterator(void):
	m_pObjectEntryBaseList(0),	
	m_pObjectEntryList(0),
	m_ListPosition(0),
	m_pSubObjectEntryIterator(0)
{
}

CObjectEntryIterator::~CObjectEntryIterator(void)
{
	DeleteSubObjectEntryIterator();
}

BOOL CObjectEntryIterator::Reset()
{
	if( m_pObjectEntryList != 0 )
		m_ListPosition = m_pObjectEntryList->begin();

	DeleteSubObjectEntryIterator();

	return TRUE;
}

BOOL CObjectEntryIterator::DeleteSubObjectEntryIterator()
{
	BOOL oResult = FALSE;

	if(m_pSubObjectEntryIterator)
	{
		delete m_pSubObjectEntryIterator;
		m_pSubObjectEntryIterator = NULL;
	}

	return oResult;
}

BOOL CObjectEntryIterator::Init(tObjectEntryBaseList* p_pObjectEntryBaseList)
{
	BOOL oResult = TRUE;

	m_pObjectEntryBaseList = p_pObjectEntryBaseList;
	m_pObjectEntryList = NULL;

	return oResult;
}

BOOL CObjectEntryIterator::Init(tObjectEntryList* p_pObjectEntryList)
{
	BOOL oResult = TRUE;

	m_pObjectEntryList = p_pObjectEntryList;
	m_pObjectEntryBaseList = NULL;

	return oResult;
}

BOOL CObjectEntryIterator::First()
{
	BOOL oResult = FALSE;

	Reset();
	if(m_pObjectEntryList)
	{
		oResult = FirstInObjectEntryList();
	}
	else if(m_pObjectEntryBaseList)
	{
		oResult = FirstInObjectEntryBaseList();
	}

	return oResult;
}

BOOL CObjectEntryIterator::FirstInObjectEntryList()
{	 
	BOOL oResult = FALSE;

	if(m_pObjectEntryList)
	{
		m_ListPosition=m_pObjectEntryList->begin();
		if( m_ListPosition!=m_pObjectEntryList->begin() )
		{
			oResult = TRUE;
            CObjectEntryBase* pObjectEntry = (*m_ListPosition);
            if(!pObjectEntry->IsVisible())
            {
                oResult = Next();
            }
		}		
	}

	return oResult;
}

BOOL CObjectEntryIterator::FirstInObjectEntryBaseList()
{
	CObjectEntryBase* pObjectEntryBase = NULL;
	CStructuredObjectEntry* pStructuredObjectEntry = NULL;
	BOOL oResult = FALSE;

	if(m_pObjectEntryBaseList)
	{
		m_ListBasePosition = m_pObjectEntryBaseList->begin();
		if(m_ListBasePosition!=m_pObjectEntryBaseList->end())
		{
			pObjectEntryBase = (*m_ListBasePosition);
			if(pObjectEntryBase)
			{
				if(pObjectEntryBase->IsKindOf("CStructuredObjectEntry"))
				{
					//Structured Object
					DeleteSubObjectEntryIterator();
					pStructuredObjectEntry = (CStructuredObjectEntry*)pObjectEntryBase;
					m_pSubObjectEntryIterator = pStructuredObjectEntry->CreateObjectEntryIterator();
					if(m_pSubObjectEntryIterator)
					{
						oResult = TRUE;
                        if(!m_pSubObjectEntryIterator->First())
                        {
                            oResult = Next();
                        }
					}
				}
				else if(pObjectEntryBase->IsKindOf("CObjectEntry"))
				{
					//ObjectEntry
					DeleteSubObjectEntryIterator();
					oResult = TRUE;
                    if(!((CObjectEntryBase*)pObjectEntryBase)->IsVisible())
                    {
                        oResult = Next();
                    }
				}
			}
		}
	}

	return oResult;
}
	
BOOL CObjectEntryIterator::Next()
{
	BOOL oResult = FALSE;

	if(m_pObjectEntryList)
	{
		oResult = NextInObjectEntryList();
	}
	else if(m_pObjectEntryBaseList)
	{
		oResult = NextInObjectEntryBaseList();
	}

	return oResult;
}

BOOL CObjectEntryIterator::NextInObjectEntryList()
{	 
	BOOL oResult = FALSE;

	if(m_pObjectEntryList)
	{
        //Get next list position if position is not NULL
        if(m_ListPosition!=m_pObjectEntryList->end()) m_pObjectEntryList++;
		if(m_ListPosition!=m_pObjectEntryList->end())
		{
			oResult = TRUE;
            CObjectEntryBase* pObjectEntry = (*m_ListPosition);
            if(!pObjectEntry->IsVisible())
            {
                oResult = Next();
            }
		}
	}

	return oResult;
}

BOOL CObjectEntryIterator::NextInObjectEntryBaseList()
{
	CObjectEntryBase* pObjectEntryBase = NULL;
	CStructuredObjectEntry* pStructuredObjectEntry = NULL;
	BOOL oResult = FALSE;

	if(m_pObjectEntryBaseList)
	{
		//Next SubObject
		if(m_pSubObjectEntryIterator)
		{
			if(m_pSubObjectEntryIterator->Next())
			{
				//Next SubObject available
				oResult = TRUE;
			}
		}

		//Next Object
		if(!oResult)
		{
            //Get next list position if position is not NULL
			if(m_ListBasePosition!=m_pObjectEntryBaseList->end()) m_ListBasePosition++;
			if(m_ListBasePosition!=m_pObjectEntryBaseList->end())
			{
				pObjectEntryBase = (*m_ListPosition);
				if(pObjectEntryBase)
				{
					if(pObjectEntryBase->IsKindOf("CStructuredObjectEntry"))
					{
						//Structured Object
						DeleteSubObjectEntryIterator();
						pStructuredObjectEntry = (CStructuredObjectEntry*)pObjectEntryBase;
						m_pSubObjectEntryIterator = pStructuredObjectEntry->CreateObjectEntryIterator();
						if(m_pSubObjectEntryIterator)
						{
							oResult = TRUE;
                            if(!m_pSubObjectEntryIterator->First())
                            {
                                oResult = Next();
                            }
						}
					}
					else if(pObjectEntryBase->IsKindOf("CObjectEntry"))
					{
						//ObjectEntry
						DeleteSubObjectEntryIterator();
						oResult = TRUE;
                        if(!((CObjectEntryBase*)pObjectEntryBase)->IsVisible())
                        {
                            oResult = Next();
                        }
					}
				}
			}
		}
	}

	return oResult;
}
		
CObjectEntry* CObjectEntryIterator::Current()
{
	CObjectEntry* pObjectEntry = NULL;

	if(m_pObjectEntryList)
	{
		pObjectEntry = CurrentInObjectEntryList();
	}
	else if(m_pObjectEntryBaseList)
	{
		pObjectEntry = CurrentInObjectEntryBaseList();
	}

	return pObjectEntry;
}

CObjectEntry* CObjectEntryIterator::CurrentInObjectEntryList()
{
	CObjectEntry* pObjectEntry = NULL;

	if(m_pObjectEntryList && m_ListPosition != m_pObjectEntryList->end())
	{
		pObjectEntry = (*m_ListPosition);
	}
	
	return pObjectEntry;
}
	
CObjectEntry* CObjectEntryIterator::CurrentInObjectEntryBaseList()
{
	CObjectEntryBase* pObjectEntryBase = NULL;
	CObjectEntry* pObjectEntry = NULL;

	if(m_pObjectEntryBaseList)
	{
		if(m_pSubObjectEntryIterator)
		{
			//Current SubObject
			pObjectEntry = m_pSubObjectEntryIterator->Current();
		}
		else
		{
			//Current Object
			pObjectEntryBase = (*m_ListPosition);
			if(pObjectEntryBase)
			{
				if(pObjectEntryBase->IsKindOf("CObjectEntry"))
				{
					//ObjectEntry
					pObjectEntry = (CObjectEntry*)pObjectEntryBase;
				}
			}
		}
	}

	return pObjectEntry;
}
	
BOOL CObjectEntryIterator::IsFinished()
{
	if( m_pObjectEntryList != 0 )
		return (m_ListPosition==m_pObjectEntryList->end());

	return TRUE;
}
