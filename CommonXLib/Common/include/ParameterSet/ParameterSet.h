// ParameterSet.h: Schnittstelle f�r die Klasse CParameterSet.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMETERSET_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
#define AFX_PARAMETERSET_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CParameterSetEntry;
class CXmlWriter;
class CXmlReader;

#include <list>
#include <MmcTypeDefinition.h>

typedef std::list<CParameterSetEntry*> tParameterList;

class CParameterSet  
{
public:
	CParameterSet();
	virtual ~CParameterSet();
	virtual CParameterSet& operator=(CParameterSet& other);
	virtual BOOL operator==(CParameterSet& other);
	virtual CParameterSet* Clone();

	//Common
	BOOL Reset();
	BOOL ResetValues();
	
	//Add Parameter
	BOOL AddParameter(CStdString p_Name, BYTE* p_pDefaultValue, DWORD p_ulSize, BOOL p_oPersistent = FALSE);
	BOOL AddParameter(CStdString p_Name, CStdString p_DefaultValue, BOOL p_oPersistent = FALSE);
	BOOL DeleteParameter(CStdString p_Name);

	//Init Parameter
	BOOL InitParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, BOOL p_oPersistent = FALSE);
	BOOL InitParameter(CStdString p_Name, CStdString p_Value, BOOL p_oPersistent = FALSE);
	
	//GetParameter
	BOOL GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL GetParameter(CStdString p_Name, CStdString& p_rValue);
	
	//SetParameter
	BOOL SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL SetParameter(CStdString p_Name, CStdString p_Value);
	
	//Persistence
	BOOL WriteTo(CXmlWriter* p_pXmlWriter);
	BOOL ReadFrom(CXmlReader* p_pXmlReader);

private:
	//Common
	CParameterSetEntry* FindParameter(CStdString p_Name);

private:
	tParameterList m_ParameterEntryList;

};

#endif // !defined(AFX_PARAMETERSET_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
