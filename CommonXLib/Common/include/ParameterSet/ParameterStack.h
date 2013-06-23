// ParameterStack.h: Schnittstelle f�r die Klasse CParameterStack.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMETERSTACK_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
#define AFX_PARAMETERSTACK_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CParameterSet;

#include <list>

typedef std::list<CParameterSet*> tParameterSetList;

class CParameterStack  
{
public:
	CParameterStack();
	virtual ~CParameterStack();
	virtual CParameterStack& operator=(CParameterStack& other);
	BOOL Reset();

	BOOL Push(CParameterSet& p_rParameterSet);
	BOOL Pop(CParameterSet& p_rParameterSet);
	BOOL Peek(CParameterSet& p_rParameterSet);
	BOOL IsEmpty();

protected:
	tParameterSetList m_ParameterSetList;
};

#endif // !defined(AFX_PARAMETERSTACK_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
