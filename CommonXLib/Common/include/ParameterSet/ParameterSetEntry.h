// ParameterSetEntry.h: Schnittstelle f�r die Klasse CParameterSetEntry.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DTMPRIVATEParameterSetEntry_H__28352344_DA65_477D_A8E0_C3B34C5D6ED9__INCLUDED_)
#define AFX_DTMPRIVATEParameterSetEntry_H__28352344_DA65_477D_A8E0_C3B34C5D6ED9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

class CParameterSetEntry  
{
public:
	CParameterSetEntry();
	virtual ~CParameterSetEntry();
	virtual CParameterSetEntry& operator=(CParameterSetEntry& other);
	virtual BOOL operator==(CParameterSetEntry& other);

	//Common
	BOOL Init(CStdString p_Name, BYTE* p_pDefaultValue, DWORD p_ulSize, BOOL p_oPersistent);
	BOOL Init(CStdString p_Name, CStdString p_DefaultValue, BOOL p_oPersistent);
	BOOL ResetValue();
	CStdString GetName();
	BOOL IsPersistent();
	
	//Get Parameter
	BOOL GetParameter(BYTE* p_pValue, DWORD p_ulSize);
	BOOL GetParameter(CStdString& p_rValue);

	//Set Parameter
	BOOL SetParameter(BYTE* p_pValue, DWORD p_ulSize);
	BOOL SetParameter(CStdString p_Value);

	//Persistence
	BOOL WriteTo(CXmlWriter* p_pXmlWriter);
	BOOL ReadFrom(CXmlReader* p_pXmlReader);

private:
	BOOL CreateDefaultValueBuffer(BYTE* p_pData, DWORD p_ulSize);
	BOOL CreateValueBuffer(BYTE* p_pData, DWORD p_ulSize);
	BOOL DeleteDefaultValueBuffer();
	BOOL DeleteValueBuffer();

	//Persistence
	BOOL FormatValueString(BYTE* p_pBuffer, DWORD p_ulBufferSize, CStdString& p_rValueString);
	BOOL ParseValueString(CStdString p_ValueString, BYTE*& p_rpBuffer, DWORD& p_rulBufferSize);

private:
	//Attributes
	CStdString			m_Name;
	BOOL			m_oPersistent;
	
	//Default Value
	BYTE*			m_pDefaultValueBuffer;
	DWORD			m_ulDefaultValueBufferSize;

	//Value
	BYTE*			m_pValueBuffer;
	DWORD			m_ulValueBufferSize;
	
};

#endif // !defined(AFX_DTMPRIVATEParameterSetEntry_H__28352344_DA65_477D_A8E0_C3B34C5D6ED9__INCLUDED_)
