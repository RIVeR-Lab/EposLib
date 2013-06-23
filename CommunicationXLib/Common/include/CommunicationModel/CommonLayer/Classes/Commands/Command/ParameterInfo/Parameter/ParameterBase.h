// ParameterBase.h: Schnittstelle f�r die Klasse CParameterBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ParameterBase_H__B509D8EB_DD7D_4903_9097_C542734202FC__INCLUDED_)
#define AFX_ParameterBase_H__B509D8EB_DD7D_4903_9097_C542734202FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Classes/TypeDefinitions.h>
#include <MmcTypeDefinition.h>
#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

class CParameterBase
{
public:
	CParameterBase();
	virtual ~CParameterBase();
#ifdef WINVER
   	virtual void Serialize(CArchive& ar);
#endif

	virtual CParameterBase& operator=(CParameterBase& other);

	void Init(int iIndex, CStdString strName, EObjectDataType eType, DWORD dArraySize, BOOL oEditable, BOOL oVisible);
	BOOL InitIndex(CStdString indexStr);
	BOOL InitName(CStdString name);
	BOOL InitType(CStdString typeStr);
	BOOL InitEditable(CStdString editableStr);
	BOOL InitVisible(CStdString visibleStr);

	BOOL IsVisible();
	BOOL IsEditable();

	int GetIndex();
	CStdString GetIndexStr();

	EObjectDataType GetType();
	CStdString GetTypeStr();

	BOOL SetValue(CStdString valueStr, BOOL showMsg = FALSE);
	CStdString GetValueStr(EObjectValueStringFormat format);

	CStdString GetName();
	CStdString GetEditableStr();
	CStdString GetVisibleStr();
	DWORD GetArraySize();

	BOOL SetData(void* pData, DWORD length);
	BOOL GetData(void* pData, DWORD length);
	DWORD GetLength();
	void ResetData();

protected:
	virtual void Reset();

private:
	void DeleteDataBuffer();
	void InitDataBuffer();

	DWORD GetParameterTypeLength(EObjectDataType type);

	CStdString GetVoidValueStr();
	CStdString GetBooleanValueStr(void* pData);
	BOOL SetData(void* p_pData, DWORD p_ulLength, DWORD& p_rulOffset);
	BOOL SetVoidValueStr(CStdString valueStr, BOOL showMsg = FALSE);
	BOOL SetBooleanValueStr(CStdString valueStr);
	CStdString ExtractSubValueStr(CStdString* pValueStr);

protected:
	CStdString m_strName;
	int m_iIndex;
	BOOL m_oEditable;
	BOOL m_oVisible;
	EObjectDataType m_eType;
	DWORD m_dArraySize;

	void* m_pDataBuffer;
	DWORD m_dDataBufferLength;
};

#endif // !defined(AFX_ParameterBase_H__B509D8EB_DD7D_4903_9097_C542734202FC__INCLUDED_)
