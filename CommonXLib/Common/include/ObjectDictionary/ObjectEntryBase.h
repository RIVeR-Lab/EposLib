// ObjectEntryBase.h: Schnittstelle f�r die Klasse CObjectEntryBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ObjectEntryBase_H__0253E5E7_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
#define AFX_ObjectEntryBase_H__0253E5E7_6B67_11D7_A23D_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <StdString.h>
#include <MmcTypeDefinition.h>

class CObjectFilter;
class CObjectEntryBase;
class CObjectEntry;
class XML_PARSER;

typedef std::list<CObjectEntryBase*> tObjectEntryBaseList;
typedef std::list<CObjectEntry*> tObjectEntryList;

class CObjectEntryBase
{
public:
    //Class initialisation
    virtual void Init() = 0;
	bool IsKindOf(CStdString strClassType);
	CStdString GetClassType();

    //User levels
    BOOL SetUserLevels(CStdString p_UserLevels);
    BOOL GetUserLevels(CStdString& p_rUserLevels);
    virtual BOOL IsUserLevelAvailable(CStdString p_UserLevel);

	//ObjectFilter
	virtual BOOL InitObjectFilter(CObjectFilter* pObjectFilter);
	
	//Set Object Information
	BOOL SetIndex(WORD index);
	BOOL SetIndex(CStdString strIndex);
	BOOL SetParameterName(CStdString name);
	BOOL SetObjectType(BYTE objectType);
	BOOL SetObjectType(CStdString strObjectType);
	
	//Get Object Information
	virtual WORD GetIndex();
	virtual BOOL GetIndex(CStdString& strIndex);
	BOOL GetParameterName(CStdString& name);
	BYTE GetObjectType();
	BOOL GetObjectType(CStdString& strObjectType);
	
	//Information
	virtual CObjectEntryBase* SearchObject(WORD index, WORD subIndex);
	virtual int GetNumberOfSubObjects();
    virtual BOOL IsVisible();
	
	//Common
	virtual CObjectEntryBase* Clone();
	virtual CObjectEntryBase& operator=(CObjectEntryBase& other);
	virtual BOOL Reset();
    virtual BOOL ResetValue();

	//Xml
	virtual BOOL StoreObjectDictionarySchema(XML_PARSER* pXmlParser);

	CObjectEntryBase();
	virtual ~CObjectEntryBase();
	virtual void Serialize(CArchive &ar);

protected:
	WORD			m_wIndex;
	CStdString		m_strParameterName;
	BYTE			m_uObjectType;
	CStdString		m_strClassType;
	CObjectFilter*	m_pObjectFilter;
    CStdString m_UserLevels;
};

#endif // !defined(AFX_ObjectEntryBase_H__0253E5E7_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
