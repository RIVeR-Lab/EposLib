#include <ObjectDictionary/ObjectEntryBaseIterator.h>

CObjectEntryBaseIterator::CObjectEntryBaseIterator(void):
	m_pObjectEntryBaseList(0),	
	m_ListPosition(0)
{
}

CObjectEntryBaseIterator::~CObjectEntryBaseIterator(void)
{
	
}

BOOL CObjectEntryBaseIterator::Reset()
{
	if( m_pObjectEntryBaseList != 0 )
		m_ListPosition = m_pObjectEntryBaseList->begin();

	return TRUE;
}

BOOL CObjectEntryBaseIterator::Init(tObjectEntryBaseList* p_pObjectEntryBaseList)
{
	BOOL oResult = TRUE;

	m_pObjectEntryBaseList = p_pObjectEntryBaseList;

	return oResult;
}

BOOL CObjectEntryBaseIterator::First()
{
	BOOL oResult = FALSE;

	if(m_pObjectEntryBaseList)
	{
		m_ListPosition = m_pObjectEntryBaseList->begin();
		if(m_ListPosition!=m_pObjectEntryBaseList->end())
		{
			oResult = TRUE;
		}
	}

	return oResult;
}
	
BOOL CObjectEntryBaseIterator::Next()
{
	BOOL oResult = FALSE;

	if(m_pObjectEntryBaseList)
	{
		m_ListPosition++;
		if(m_ListPosition!=m_pObjectEntryBaseList->end())
		{
			oResult = TRUE;
		}
	}

	return oResult;
}
		
CObjectEntryBase* CObjectEntryBaseIterator::Current()
{
	CObjectEntryBase* pObjectEntryBase = NULL;
	
	if(m_pObjectEntryBaseList)
	{
		if(m_ListPosition!=m_pObjectEntryBaseList->end())
		{
			pObjectEntryBase = (*m_ListPosition);
		}
	}

	return pObjectEntryBase;
}
	
BOOL CObjectEntryBaseIterator::IsFinished()
{
	return (m_ListPosition == m_pObjectEntryBaseList->end());
}
