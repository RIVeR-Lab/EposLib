// ParameterInfo.h: Schnittstelle f�r die Klasse CParameterInfo.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ParameterInfo_H__64357DA8_A4B8_4BC5_86FB_8D107B19544D__INCLUDED_)
#define AFX_ParameterInfo_H__64357DA8_A4B8_4BC5_86FB_8D107B19544D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <Classes/TypeDefinitions.h>
#include <Classes/XXMLFile.h>

class CParameter;
class CReturnParameter;

class CParameterInfo
{
public:
	void Reset();
	void AddParameter(int paramIndex,CStdString name,EObjectDataType type);
	void AddParameter(int paramIndex,CStdString name,EObjectDataType type,BOOL visible,BOOL editable);
	void AddParameter(int paramIndex,CStdString name,EObjectDataType type,DWORD dArraySize);
	void AddParameter(int paramIndex,CStdString name,EObjectDataType type,DWORD dArraySize,BOOL visible,BOOL editable);

	void AddReturnParameter(int paramIndex,CStdString name,EObjectDataType type);
	void AddReturnParameter(int paramIndex,CStdString name,EObjectDataType type,int visible);
	void AddReturnParameter(int paramIndex,CStdString name,EObjectDataType type,DWORD dArraySize);
	void AddReturnParameter(int paramIndex,CStdString name,EObjectDataType type,DWORD dArraySize,BOOL visible);

	BOOL IsEditableParameterExisting();
	BOOL IsVisibleParameterExisting();
	BOOL IsVisibleReturnParameterExisting();

	//Parameter
	int GetNbOfParameter();
    BOOL FindParameterIndex(CStdString p_Name, int& p_rIndex);
	BOOL IsParameterExisting(int paramIndex);
	CStdString GetParameterName(int paramIndex);
	EObjectDataType GetParameterType(int paramIndex);
	DWORD GetParameterLength(int paramIndex);
	CStdString GetParameterTypeStr(int paramIndex);
	CStdString GetParameterDataStr(int paramIndex,EObjectValueStringFormat format);
	BOOL IsParameterEditable(int paramIndex);
	BOOL IsParameterVisible(int paramIndex);
	DWORD GetParameterArraySize(int paramIndex);

	void ResetParameterData();
	BOOL SetParameterData(int paramIndex,void* data,size_t length);
	BOOL GetParameterData(int paramIndex,void* data,DWORD length);
	BOOL SetParameterDataString(int paramIndex,CStdString strValue, BOOL showMsg);
	BOOL GetParameterDataString(int paramIndex,CStdString* pStrValue, EObjectValueStringFormat format);

	//ReturnParameter
	int GetNbOfReturnParameter();
    BOOL FindReturnParameterIndex(CStdString p_Name, int& p_rIndex);
	BOOL IsReturnParameterExisting(int paramIndex);
	CStdString GetReturnParameterName(int paramIndex);
	EObjectDataType GetReturnParameterType(int paramIndex);
	DWORD GetReturnParameterLength(int paramIndex);
	CStdString GetReturnParameterTypeStr(int paramIndex);
	CStdString GetReturnParameterDataStr(int paramIndex,EObjectValueStringFormat format);
	BOOL IsReturnParameterVisible(int paramIndex);
	DWORD GetReturnParameterArraySize(int paramIndex);

	void ResetReturnParameterData();
	BOOL SetReturnParameterData(int paramIndex, void *data, size_t length);
	BOOL GetReturnParameterData(int paramIndex, void *data, DWORD length);
	BOOL SetReturnParameterDataString(int paramIndex, CStdString strValue, BOOL showMsg);
	BOOL GetReturnParameterDataString(int paramIndex, CStdString* pStrValue, EObjectValueStringFormat format);

	CParameterInfo();
	virtual ~CParameterInfo();
#ifdef WINVER
	void Serialize(CArchive& ar);
#endif
	CParameterInfo& operator=(CParameterInfo& other);

	BOOL LoadFromXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pParameterInfoElement);
	BOOL StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pParentElement,BOOL checkVisibility = FALSE);

private:
	CParameter* FindParameter(int paramIndex);
	CReturnParameter* FindReturnParameter(int paramIndex);
	void DeleteParameterList();
	void DeleteReturnParameterList();

	std::list<CParameter*> m_ParameterList;
	std::list<CReturnParameter*> m_ReturnParameterList;
};

#endif // !defined(AFX_ParameterInfo_H__64357DA8_A4B8_4BC5_86FB_8D107B19544D__INCLUDED_)
