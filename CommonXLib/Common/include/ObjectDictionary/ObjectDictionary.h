// ObjectDictionary.h: Schnittstelle f�r die Klasse CObjectDictionary.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTDICTIONARY_H__0253E5E6_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
#define AFX_OBJECTDICTIONARY_H__0253E5E6_6B67_11D7_A23D_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <StdString.h>
#include <MmcTypeDefinition.h>
#include <fstream>

#include <ObjectDictionary/ObjectEntryIterator.h>
#include <ObjectDictionary/ObjectEntryBaseIterator.h>
#include <ObjectDictionary/ObjectEntry.h>
#include <ObjectDictionary/ObjectFilter.h>
#include <Classes/XML_PARSER.h>

class CObjectDictionary: public XML_PARSER
{
public:
    //Object
    BOOL AddObject(CObjectEntryBase* p_pNewObjectEntry);
    BOOL DeleteObject(CObjectEntryBase* p_pObjectEntry);
    BOOL DeleteAllObjects();
    CObjectEntry* SearchObject(WORD p_usIndex, WORD p_usSubIndex);
    CObjectEntryBase* SearchObject(WORD p_usIndex);
    CObjectEntry* SearchObjectReverse(WORD p_usIndex, WORD p_usSubIndex);
    CObjectEntryBase* SearchObjectReverse(WORD p_usIndex);

    //Iteration
    CObjectEntryIterator* CreateObjectEntryIterator();
    CObjectEntryBaseIterator* CreateObjectEntryBaseIterator();

    //Information
    int GetNumberOfObjects();
    int GetNumberOfSubObjects(WORD p_usIndex);
    CStdString GetObjectName(WORD p_usIndex, WORD p_usSubIndex);
    BOOL GetHighLimit(WORD p_usIndex, WORD p_usSubIndex, CStdString *p_pHighLimit);
    BOOL GetLowLimit(WORD p_usIndex, WORD p_usSubIndex, CStdString *p_pLowLimit);

    //Object Filter
    BOOL SetInternalObjectFilter(BOOL p_oActive);
    BOOL SetObjectFilter(CObjectFilter p_ObjectFilter);
    BOOL ResetObjectFilter();
    BOOL IsObjectFilterEmpty();

    BOOL StoreObjectDictionarySchema(CStdString* p_pXmlData);
    BOOL LoadObjectDictionarySchema(CStdStringA* p_pXmlData);

    CObjectDictionary();
    virtual ~CObjectDictionary();

    //Common
    CObjectDictionary& operator=(CObjectDictionary& p_rObjectDictionary);
    void Serialize(CArchive& p_rArchive);
    BOOL Reset();
    BOOL ResetValues();

private:
    BOOL DeleteObjectList();
    void Parse_XML_Document();

private:
    tObjectEntryBaseList m_ObjectList;
    CObjectFilter m_ObjectFilter;
};

#endif // !defined(AFX_OBJECTDICTIONARY_H__0253E5E6_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
