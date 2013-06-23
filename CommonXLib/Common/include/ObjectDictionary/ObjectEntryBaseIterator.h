#pragma once

#include <ObjectDictionary/ObjectEntryBase.h>

class CObjectEntryBaseIterator
{
public:
	CObjectEntryBaseIterator(void);
	~CObjectEntryBaseIterator(void);

	//Initialisation
	BOOL Init(tObjectEntryBaseList* p_pObjectEntryBaseList);
	
	//Iteration
	BOOL First();
	BOOL Next();
	CObjectEntryBase* Current();
	BOOL IsFinished();

private:
	BOOL Reset();

protected:
	//List
	tObjectEntryBaseList* m_pObjectEntryBaseList;
		
	//Current Position
	tObjectEntryBaseList::iterator m_ListPosition;
};
