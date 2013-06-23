// ObjectFilter.h: Schnittstelle f�r die Klasse CObjectFilter.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTFILTER_H__491B4DA7_AB9C_11D7_A273_00A02436C4EF__INCLUDED_)
#define AFX_OBJECTFILTER_H__491B4DA7_AB9C_11D7_A273_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

#include <StdString.h>
#include <MmcTypeDefinition.h>

class CXmlReader;
class CXmlWriter;

class CObjectFilter
{
public:
    //Persistence
    BOOL ReadFrom(CXmlReader* p_pXmlReader, int p_lCount);
    BOOL WriteTo(CXmlWriter* p_pXmlWriter);

    BOOL SetInternalObjectFilter(BOOL p_oActive);
    void SetServiceModeActive(BOOL p_oActive);
    void Serialize(CArchive& p_rArchive);
    BOOL IsVisibleObject(CStdString p_ObjectName, WORD p_usIndex, BYTE p_ubSubIndex);
    BOOL IsVisibleStructuredObject(CStdString p_ObjectName, WORD p_usIndex);
    BOOL IsVisibleObjectExisting(WORD p_usIndex, BYTE p_ubSubIndex);
    BOOL IsVisibleStructuredObjectExisting(WORD p_usIndex);

    BOOL IsEmpty();
    void SetName(CStdString p_Name);
    void SetDescription(CStdString p_Description);
    CStdString GetName();
    CStdString GetDescription();
    CObjectFilter();
    CObjectFilter(CObjectFilter& p_rObjectFilter);
    virtual ~CObjectFilter();

    BOOL Reset();
    BOOL LoadObjectFilterSchema(CStdString* p_pXmlData);
    BOOL StoreObjectFilterSchema(CStdString* p_pXmlData);

    CObjectFilter& operator=(CObjectFilter &p_rObjectFilter);
    BOOL AddVisibleStructuredObject(WORD p_usIndex);
    BOOL AddVisibleObject(WORD p_usIndex, BYTE p_ubSubIndex);
    BOOL DeleteVisibleObject(WORD p_usIndex, BYTE p_ubSubIndex);
    BOOL DeleteVisibleStructuredObject(WORD p_usIndex);
    BOOL DeleteVisibleStructuredObjects(WORD p_usIndex);
    BOOL DeleteAllVisibleObjects();

private:
    BOOL Parse_XML_Document(CXmlReader* p_pXmlReader);
    BOOL IsObjectNameVisible(CStdString p_ObjectName);
    BOOL AddVisibleStructuredObject(CStdString p_Index);
    BOOL AddVisibleObject(CStdString p_Index, CStdString p_SubIndex);

    //Persistence
    BOOL ReadStructuredObjectFilterSchemaFrom(CXmlReader* p_pXmlReader);
    BOOL ReadVisibleObjectFilterSchemaFrom(CXmlReader* p_pXmlReader);
    BOOL WriteVisibleObjectFilterSchemaTo(CXmlWriter* p_pXmlWriter);
    BOOL WriteStructuredObjectFilterSchemaTo(CXmlWriter* p_pXmlWriter);

private:
    BOOL m_oFilterIsEmpty;
    BOOL m_oServiceModeActive;
    BOOL m_oInternalObjectsVisible;
    CStdString m_Name;
    CStdString m_Description;
    map<WORD, vector<BYTE> > m_VisibleObject;
    vector<WORD> m_VisibleStructuredObject;
};

#endif // !defined(AFX_OBJECTFILTER_H__491B4DA7_AB9C_11D7_A273_00A02436C4EF__INCLUDED_)
