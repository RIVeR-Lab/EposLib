// ParameterStack.cpp: Implementierung der Klasse CParameterStack.
//
//////////////////////////////////////////////////////////////////////
#include <ParameterSet/ParameterSet.h>
#include <ParameterSet/ParameterStack.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CParameterStack::CParameterStack()
{

}

CParameterStack::~CParameterStack()
{
	Reset();
}

CParameterStack& CParameterStack::operator=(CParameterStack& other)
{
	if(this != &other)
	{
		//Reset
		Reset();

		for(tParameterSetList::iterator it=other.m_ParameterSetList.begin();
							it!=other.m_ParameterSetList.end(); it++)
		{
			CParameterSet* pOtherParameterSet = (*it);

			CParameterSet* pParameterSet = pOtherParameterSet->Clone();

			m_ParameterSetList.push_back(pParameterSet);
		}
	}

	return *this;
}

BOOL CParameterStack::Reset()
{
	BOOL oResult = TRUE;

	for(tParameterSetList::iterator it=m_ParameterSetList.begin();
									it!=m_ParameterSetList.end(); it++)
	{
		CParameterSet* pParameterSet = (*it);
		delete pParameterSet;
	}

	m_ParameterSetList.clear();

	return oResult;
}

BOOL CParameterStack::Push(CParameterSet& p_rParameterSet)
{
	CParameterSet* pClonedParameterSet = NULL;
	BOOL oResult = FALSE;

	pClonedParameterSet = p_rParameterSet.Clone();
	if(pClonedParameterSet)
	{
		m_ParameterSetList.push_back(pClonedParameterSet);
		oResult = TRUE;
	}

	return oResult;
}
	
BOOL CParameterStack::Pop(CParameterSet& p_rParameterSet)
{
	CParameterSet* pParameterSet = NULL;
	BOOL oResult = FALSE;

	if(m_ParameterSetList.size() > 0)
	{
		tParameterSetList::iterator it=m_ParameterSetList.begin();
		if( it!=m_ParameterSetList.end() )
		{
			pParameterSet = (*it);
			if(pParameterSet)
			{
				//Copy
				p_rParameterSet = *pParameterSet;

				//Delete
				delete pParameterSet;
				m_ParameterSetList.erase(it);
				oResult = TRUE;
			}
		}
	}

	return oResult;
}

BOOL CParameterStack::Peek(CParameterSet& p_rParameterSet)
{
	CParameterSet* pParameterSet = NULL;
	BOOL oResult = FALSE;

	if(m_ParameterSetList.size() > 0)
	{
		tParameterSetList::iterator it=m_ParameterSetList.begin();
		if( it!=m_ParameterSetList.end() )
		{
			pParameterSet = (*it);
			if(pParameterSet)
			{
				//Copy
				p_rParameterSet = *pParameterSet;
				oResult = TRUE;
			}
		}
	}

	return oResult;
}

BOOL CParameterStack::IsEmpty()
{
	BOOL oResult = FALSE;
	
	oResult = (m_ParameterSetList.size() == 0);

	return oResult;
}
