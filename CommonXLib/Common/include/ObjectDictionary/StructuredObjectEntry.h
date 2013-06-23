// StructuredObjectEntry.h: Schnittstelle f�r die Klasse CStructuredObjectEntry.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_StructuredObjectEntry_H__0253E5E9_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
#define AFX_StructuredObjectEntry_H__0253E5E9_6B67_11D7_A23D_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ObjectDictionary/ObjectEntryBase.h>

class CObjectEntryIterator;
class XML_PARSER;

class CStructuredObjectEntry : public CObjectEntryBase
{
public:
    CStructuredObjectEntry();
    virtual ~CStructuredObjectEntry();

    void Init() {m_strClassType=_T("CStructuredObjectEntry");}

    //ObjectFilter
    virtual BOOL InitObjectFilter(CObjectFilter* p_pObjectFilter);
    CObjectFilter* GetFilter();

    //Common
    virtual CObjectEntryBase* Clone();
    virtual CStructuredObjectEntry& operator=(CStructuredObjectEntry& p_rStructuredObjectEntry);
    virtual BOOL Reset();
    virtual BOOL ResetValue();

    //Object
    BOOL AddObject(CObjectEntry* p_pObject);
    BOOL SetFirstSubObjectEntryValues(CObjectEntryBase* p_pObjectEntry);

    //Information
    CObjectEntryBase* SearchObject(WORD p_usIndex, WORD p_usSubIndex);
    int GetNumberOfSubObjects();
    CStdString GetParameterNameStr();

    //Xml
    virtual void Serialize(CArchive &p_rArchive);

    BOOL StoreObjectDictionarySchema(XML_PARSER* p_pXmlParser);
    void Parse_XML_Document(XML_PARSER* p_pXmlParser);

    //Iteration
    CObjectEntryIterator* CreateObjectEntryIterator();

private:
    BOOL DeleteObjectEntryList();

private:
    tObjectEntryList m_ObjectEntryList;
};

#endif // !defined(AFX_StructuredObjectEntry_H__0253E5E9_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
