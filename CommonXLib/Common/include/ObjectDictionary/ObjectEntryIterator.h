#pragma once

#include <ObjectDictionary/ObjectEntryBase.h>

class CObjectEntryIterator
{
public:
	CObjectEntryIterator(void);
	~CObjectEntryIterator(void);

	//Initialisation
	BOOL Init(tObjectEntryBaseList* p_pObjectEntryBaseList);
	BOOL Init(tObjectEntryList* p_pObjectEntryList);
	
	//Iteration
	BOOL First();
	BOOL Next();
	CObjectEntry* Current();
	BOOL IsFinished();

private:
	BOOL FirstInObjectEntryList();
	BOOL FirstInObjectEntryBaseList();
	BOOL NextInObjectEntryList();
	BOOL NextInObjectEntryBaseList();
	CObjectEntry* CurrentInObjectEntryList();
	CObjectEntry* CurrentInObjectEntryBaseList();
	
	BOOL Reset();
	BOOL DeleteSubObjectEntryIterator();

private:
	//List
	tObjectEntryBaseList* m_pObjectEntryBaseList;
	tObjectEntryList* m_pObjectEntryList;
	
	//Current Position
	tObjectEntryList::iterator m_ListPosition;
	tObjectEntryBaseList::iterator m_ListBasePosition;
	CObjectEntryIterator* m_pSubObjectEntryIterator;
};
