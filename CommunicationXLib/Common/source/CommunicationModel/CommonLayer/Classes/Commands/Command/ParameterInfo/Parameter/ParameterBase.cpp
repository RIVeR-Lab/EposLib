// ParameterBase.cpp: Implementierung der Klasse CParameterBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <malloc.h>
#include <memory.h>
#include <Classes/MmcDataConversion.h>
#include <Classes/TypeDefinitions.h>
#include <MmcTypeDefinition.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/ParameterInfo/Parameter/ParameterBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CParameterBase::CParameterBase()
{
	m_pDataBuffer = NULL;
	m_dDataBufferLength = 0;
	Reset();
}

CParameterBase::~CParameterBase()
{
	DeleteDataBuffer();
}

CParameterBase& CParameterBase::operator=(CParameterBase& other)
{
	if(this != &other)
	{
		Reset();
		m_strName = other.m_strName;
		m_iIndex = other.m_iIndex;
		m_oEditable = other.m_oEditable;
		m_oVisible = other.m_oVisible;
		m_eType = other.m_eType;
		m_dArraySize = other.m_dArraySize;

		DeleteDataBuffer();
		if(other.m_dDataBufferLength > 0)
		{
			m_dDataBufferLength = other.m_dDataBufferLength;
			m_pDataBuffer = malloc(m_dDataBufferLength);
			memcpy(m_pDataBuffer, other.m_pDataBuffer, m_dDataBufferLength);
		}
	}

	return *this;
}

#ifdef WINVER
void CParameterBase::Serialize(CArchive& ar)
{
	BYTE* p;
	int type;

	if(ar.IsStoring())
	{
		ar << m_strName;
		ar << m_iIndex;
		ar << m_oEditable;
		ar << m_oVisible;
		ar << m_eType;
		ar << m_dArraySize;

		ar << m_dDataBufferLength;
		if(m_dDataBufferLength > 0)
		{
			p = (BYTE*)m_pDataBuffer;
			for(DWORD i=0;i<m_dDataBufferLength;i++)
			{
				ar << *p;
				p++;
			}
		}
	}
	else
	{
		Reset();
		ar >> m_strName;
		ar >> m_iIndex;
		ar >> m_oEditable;
		ar >> m_oVisible;
		ar >> type; m_eType = (EObjectDataType)type;
		ar >> m_dArraySize;

		DeleteDataBuffer();
		ar >> m_dDataBufferLength;

		if(m_dDataBufferLength > 0)
		{
			m_pDataBuffer = malloc(m_dDataBufferLength);
			p = (BYTE*)m_pDataBuffer;
			for(DWORD i=0;i<m_dDataBufferLength;i++)
			{
				ar >> *p;
				p++;
			}
		}
	}
}
#endif

void CParameterBase::Reset()
{
	m_strName = "";
	m_iIndex = 0;
	m_oEditable = TRUE;
	m_oVisible = TRUE;
	m_eType = ODT_UNKNOWN;
	m_dArraySize = 1;
	DeleteDataBuffer();
}

void CParameterBase::DeleteDataBuffer()
{
	if(m_pDataBuffer)
	{
		free(m_pDataBuffer);
		m_pDataBuffer = NULL;
		m_dDataBufferLength = 0;
	}
}

void CParameterBase::InitDataBuffer()
{
	DWORD dLength;

	DeleteDataBuffer();
	dLength = GetParameterTypeLength(m_eType);
	m_dDataBufferLength = dLength * m_dArraySize;
	if(m_dDataBufferLength > 0) m_pDataBuffer = malloc(m_dDataBufferLength);
	ResetData();
}

void CParameterBase::Init(int iIndex, CStdString strName, EObjectDataType eType, DWORD dArraySize, BOOL oVisible, BOOL oEditable)
{
	m_iIndex = iIndex;
	m_strName = strName;
	m_eType = eType;
	m_dArraySize = dArraySize;
	m_oEditable = oEditable;
	m_oVisible = oVisible;
	InitDataBuffer();
}

BOOL CParameterBase::IsVisible()
{
	return m_oVisible;
}

BOOL CParameterBase::IsEditable()
{
	return m_oEditable;
}

BOOL CParameterBase::InitIndex(CStdString indexStr)
{
	CMmcDataConversion dataConversion;

	return dataConversion.DecIntStr2Int(indexStr, &m_iIndex, FALSE);
}

BOOL CParameterBase::InitName(CStdString name)
{
	m_strName = name;
	return TRUE;
}

BOOL CParameterBase::InitType(CStdString typeStr)
{
	CMmcDataConversion dataConversion;
	CStdString strParameterType, strArraySize;
	int index1, index2;

	//Reset Data
	DeleteDataBuffer();

	//ArraySize
	index1 = typeStr.Find('[');
	index2 = typeStr.Find(']');
	if((index1 == -1)||(index2 == -1))
	{
		m_dArraySize = 1;
		strParameterType = typeStr;
	}
	else
	{
		strParameterType = typeStr.Left(index1);
		strArraySize = typeStr.Mid(index1+1, index2-index1-1);
		if(!dataConversion.DecDWordStr2DWord(strArraySize, &m_dArraySize, FALSE)) return FALSE;
	}

	//Convert TypeStr
	if(strParameterType.CompareNoCase(_T("UInt8")) == 0)
	{
		m_eType = ODT_UINT8;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("Int8")) == 0)
	{
		m_eType = ODT_INT8;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("UInt16")) == 0)
	{
		m_eType = ODT_UINT16;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("Int16")) == 0)
	{
		m_eType = ODT_INT16;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("UInt32")) == 0)
	{
		m_eType = ODT_UINT32;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("Int32")) == 0)
	{
		m_eType = ODT_INT32;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("UInt64")) == 0)
	{
		m_eType = ODT_UINT64;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("Int64")) == 0)
	{
		m_eType = ODT_INT64;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("Float")) == 0)
	{
		m_eType = ODT_FLOAT;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("Void")) == 0)
	{
		m_eType = ODT_MEMORY_BLOCK;
		InitDataBuffer();
		return TRUE;
	}
	else if(strParameterType.CompareNoCase(_T("Bool")) == 0)
	{
		m_eType = ODT_BOOLEAN;
		InitDataBuffer();
		return TRUE;
	}
	else
	{
		m_eType = ODT_UNKNOWN;
		return FALSE;
	}

}

int CParameterBase::GetIndex()
{
	return m_iIndex;
}

CStdString CParameterBase::GetIndexStr()
{
	CMmcDataConversion dataConversion;
	CStdString strResult;

	if(dataConversion.Int2DecIntStr(m_iIndex, &strResult))
	{
		return strResult;
	}

	return "";
}


CStdString CParameterBase::GetTypeStr()
{
	CStdString typeStr, str;
	BOOL oAddArraySize = FALSE;

	switch(m_eType)
	{
		case ODT_UNKNOWN	: typeStr = "Unknown"; oAddArraySize = FALSE; break;
		case ODT_UINT8		: typeStr =  "UInt8"; oAddArraySize = TRUE; break;
		case ODT_INT8		: typeStr =  "Int8"; oAddArraySize = TRUE; break;
		case ODT_UINT16		: typeStr =  "UInt16"; oAddArraySize = TRUE; break;
		case ODT_INT16		: typeStr =  "Int16"; oAddArraySize = TRUE; break;
		case ODT_UINT32		: typeStr =  "UInt32"; oAddArraySize = TRUE; break;
		case ODT_INT32		: typeStr =  "Int32"; oAddArraySize = TRUE; break;
		case ODT_UINT64		: typeStr =  "UInt64"; oAddArraySize = TRUE; break;
		case ODT_INT64		: typeStr =  "Int64"; oAddArraySize = TRUE; break;
		case ODT_FLOAT		: typeStr =  "Float"; oAddArraySize = TRUE; break;
		case ODT_MEMORY_BLOCK		: typeStr =  "Void"; oAddArraySize = FALSE; break;
		case ODT_BOOLEAN: typeStr =  "Bool"; oAddArraySize = TRUE; break;
		default: return "";
	}

	if(oAddArraySize && (m_dArraySize > 1))
	{
		TCHAR *pszArraySize = new TCHAR[32];
		_stprintf(pszArraySize,_T("[%i]"), (int)m_dArraySize);
		typeStr = typeStr + pszArraySize;
		delete []pszArraySize;
	}

	return typeStr;
}

EObjectDataType CParameterBase::GetType()
{
	return m_eType;
}

DWORD CParameterBase::GetArraySize()
{
	return m_dArraySize;
}

BOOL CParameterBase::SetValue(CStdString valueStr, BOOL showMsg)
{
	CMmcDataConversion dataConversion;
	bool oResult = false;
	BYTE byteValue;
	char charValue;
	WORD wordValue;
	short shortValue;
	DWORD dWordValue;
	long longValue;
	unsigned __int64 uint64Value;
	__int64 int64Value;
	float floatValue;
	DWORD offset = 0;
	EObjectValueStringFormat format;
	CStdString subValueStr;

	if(m_eType == ODT_MEMORY_BLOCK)
	{
		oResult = SetVoidValueStr(valueStr, showMsg) > 0;
	}
	else
	{
		for(DWORD i=0;i<m_dArraySize;i++)
		{
			//Extract SubString
			subValueStr = ExtractSubValueStr(&valueStr);
			if(subValueStr.size()>0)
			{
				//Detect dataFormat
				if((subValueStr.size() >= (int)strlen("0x")) && (subValueStr.Left((int)strlen("0x")).CompareNoCase(_T("0x")) == 0)) format = OVF_HEX; else format = OVF_DEC;

				if(format == OVF_DEC)
				{
					switch(m_eType)
					{
						case ODT_UNKNOWN	: break;
						case ODT_UINT8	: oResult = dataConversion.DecByteStr2Byte(subValueStr,&byteValue,showMsg == TRUE ); SetData(&byteValue,sizeof(byteValue),offset); break;
						case ODT_INT8	: oResult = dataConversion.DecCharStr2Char(subValueStr,&charValue,showMsg == TRUE ); SetData(&charValue,sizeof(charValue),offset); break;
						case ODT_UINT16	: oResult = dataConversion.DecWordStr2Word(subValueStr,&wordValue,showMsg == TRUE ); SetData(&wordValue,sizeof(wordValue),offset); break;
						case ODT_INT16	: oResult = dataConversion.DecShortStr2Short(subValueStr,&shortValue,showMsg == TRUE ); SetData(&shortValue,sizeof(shortValue),offset); break;
						case ODT_UINT32	: oResult = dataConversion.DecDWordStr2DWord(subValueStr,&dWordValue,showMsg == TRUE ); SetData(&dWordValue,sizeof(dWordValue),offset); break;
						case ODT_INT32	: oResult = dataConversion.DecLongStr2Long(subValueStr,&longValue,showMsg == TRUE ); SetData(&longValue,sizeof(longValue),offset); break;
						case ODT_UINT64	: oResult = dataConversion.DecUInt64Str2UInt64(subValueStr,&uint64Value,showMsg == TRUE ); SetData(&uint64Value,sizeof(uint64Value),offset); break;
						case ODT_INT64	: oResult = dataConversion.DecInt64Str2Int64(subValueStr,&int64Value,showMsg == TRUE ); SetData(&int64Value,sizeof(int64Value),offset); break;
						case ODT_FLOAT	: oResult = dataConversion.DecFloatStr2Float(subValueStr,&floatValue,showMsg == TRUE ); SetData(&floatValue,sizeof(floatValue),offset); break;
						case ODT_BOOLEAN: oResult = (SetBooleanValueStr(subValueStr) > 0 ); break;
						default: break;
					}
				}
				else if(format == OVF_HEX)
				{
					switch(m_eType)
					{
						case ODT_UNKNOWN	: break;
						case ODT_UINT8	: oResult = dataConversion.HexByteStr2Byte(subValueStr,&byteValue,showMsg == TRUE ); SetData(&byteValue,sizeof(byteValue),offset); break;
						case ODT_INT8	: oResult = dataConversion.HexCharStr2Char(subValueStr,&charValue,showMsg == TRUE ); SetData(&charValue,sizeof(charValue),offset); break;
						case ODT_UINT16	: oResult = dataConversion.HexWordStr2Word(subValueStr,&wordValue,showMsg == TRUE ); SetData(&wordValue,sizeof(wordValue),offset); break;
						case ODT_INT16	: oResult = dataConversion.HexShortStr2Short(subValueStr,&shortValue,showMsg == TRUE ); SetData(&shortValue,sizeof(shortValue),offset); break;
						case ODT_UINT32	: oResult = dataConversion.HexDWordStr2DWord(subValueStr,&dWordValue,showMsg == TRUE ); SetData(&dWordValue,sizeof(dWordValue),offset); break;
						case ODT_INT32	: oResult = dataConversion.HexLongStr2Long(subValueStr,&longValue,showMsg == TRUE ); SetData(&longValue,sizeof(longValue),offset); break;
						case ODT_UINT64	: oResult = dataConversion.HexUInt64Str2UInt64(subValueStr,&uint64Value,showMsg == TRUE ); SetData(&uint64Value,sizeof(uint64Value),offset); break;
						case ODT_INT64	: oResult = dataConversion.HexInt64Str2Int64(subValueStr,&int64Value,showMsg == TRUE ); SetData(&int64Value,sizeof(int64Value),offset); break;
						case ODT_FLOAT	: oResult = dataConversion.HexFloatStr2Float(subValueStr,&floatValue,showMsg == TRUE ); SetData(&floatValue,sizeof(floatValue),offset); break;
						case ODT_BOOLEAN: oResult = (SetBooleanValueStr(subValueStr) == TRUE); break;
						default: break;
					}
				}
			}
			else
			{
				oResult = false;
			}
		}
	}

	return oResult;
}

CStdString CParameterBase::ExtractSubValueStr(CStdString* pValueStr)
{
	CStdString subStr = "";
	int index;

	if(pValueStr && pValueStr->size()>0)
	{
		pValueStr->TrimRight();
		*pValueStr += ' ';
		index = pValueStr->Find(' ', 0);
		if(index != -1)
		{
			subStr = pValueStr->Left(index);
			*pValueStr = pValueStr->Right((int)pValueStr->size()-index);
			pValueStr->TrimLeft();
		}
	}

	return subStr;
}

CStdString CParameterBase::GetValueStr(EObjectValueStringFormat format)
{
	CStdString strResult = "";
	CStdString strSubValue;
	CMmcDataConversion dataConversion;
	DWORD dParameterTypeLength;
	BYTE* pData;

	if(m_pDataBuffer)
	{
		if(m_eType == ODT_MEMORY_BLOCK)
		{
			strResult = GetVoidValueStr();
		}
		else
		{
			dParameterTypeLength = GetParameterTypeLength(m_eType);
			pData = (BYTE*)m_pDataBuffer;
			for(DWORD i=0;i<m_dArraySize;i++)
			{
				if(format == OVF_DEC)
				{
					switch(m_eType)
					{
						case ODT_UNKNOWN	: break;
						case ODT_UINT8	: dataConversion.UChar2DecUCharStr(*(BYTE*)pData, &strSubValue); break;
						case ODT_INT8	: dataConversion.Char2DecCharStr(*(char*)pData, &strSubValue); break;
						case ODT_UINT16	: dataConversion.Word2DecWordStr(*(WORD*)pData, &strSubValue); break;
						case ODT_INT16	: dataConversion.Short2DecShortStr(*(short*)pData, &strSubValue); break;
						case ODT_UINT32	: dataConversion.DWord2DecDWordStr(*(DWORD*)pData, &strSubValue); break;
						case ODT_INT32	: dataConversion.Long2DecLongStr(*(long*)pData, &strSubValue); break;
						case ODT_UINT64	: dataConversion.UInt64_2DecUInt64Str(*(unsigned __int64*)pData, &strSubValue); break;
						case ODT_INT64	: dataConversion.Int64_2DecInt64Str(*(__int64*)pData, &strSubValue); break;
						case ODT_FLOAT	: dataConversion.Float2DecFloatStr(*(float*)pData, &strSubValue); break;
						case ODT_BOOLEAN: strSubValue = GetBooleanValueStr(pData);break;
						default: break;
					}
				}
				else if(format == OVF_HEX)
				{
					switch(m_eType)
					{
						case ODT_UNKNOWN	: break;
						case ODT_UINT8	: dataConversion.UChar2HexUCharStr(*(BYTE*)pData, &strSubValue); break;
						case ODT_INT8	: dataConversion.Char2HexCharStr(*(char*)pData, &strSubValue); break;
						case ODT_UINT16	: dataConversion.Word2HexWordStr(*(WORD*)pData, &strSubValue); break;
						case ODT_INT16	: dataConversion.Short2HexShortStr(*(short*)pData, &strSubValue); break;
						case ODT_UINT32	: dataConversion.DWord2HexDWordStr(*(DWORD*)pData, &strSubValue); break;
						case ODT_INT32	: dataConversion.Long2HexLongStr(*(long*)pData, &strSubValue); break;
						case ODT_UINT64	: dataConversion.UInt64_2HexUInt64Str(*(unsigned __int64*)pData, &strSubValue); break;
						case ODT_INT64	: dataConversion.Int64_2HexInt64Str(*(__int64*)pData, &strSubValue); break;
						case ODT_FLOAT	: dataConversion.Float2HexFloatStr(*(float*)pData, &strSubValue); break;
						case ODT_BOOLEAN: strSubValue = GetBooleanValueStr(pData);break;
						default: break;
					}
				}
				strResult = strResult + strSubValue + _T(" ");
				pData = pData + dParameterTypeLength;
			}
			strResult.TrimRight();
		}
	}

	return strResult;
}

CStdString CParameterBase::GetName()
{
	return m_strName;
}

BOOL CParameterBase::SetData(void* pData, DWORD length)
{
	DWORD minLength;

	if((m_eType == ODT_MEMORY_BLOCK) || (m_eType == ODT_STRING))
	{
		DeleteDataBuffer();
		if(pData && (length > 0))
		{
			if( m_eType == ODT_STRING )
			{
				m_dDataBufferLength = length*sizeof(TCHAR);
				m_pDataBuffer = malloc(m_dDataBufferLength);
				memcpy(m_pDataBuffer, pData, length*sizeof(TCHAR));
			}
			else
			{
				m_dDataBufferLength = length;
				m_pDataBuffer = malloc(m_dDataBufferLength);
				memcpy(m_pDataBuffer, pData, length);
			}
		}
		return TRUE;
	}
	else
	{
		if(pData && (length > 0))
		{
			minLength = m_dDataBufferLength;
			if(length < minLength) minLength = length;

			if(m_pDataBuffer && (minLength > 0)) memcpy(m_pDataBuffer, pData, minLength);
			return (m_dDataBufferLength == length);
		}
	}

	return FALSE;
}

BOOL CParameterBase::SetData(void* p_pData, DWORD p_ulLength, DWORD& p_rulOffset)
{
    BOOL oRetVal(FALSE);
	DWORD ulMinLength;
	DWORD ulOffset(0);
	BYTE* pByte;

	ulOffset = p_rulOffset;
	if(p_pData && (p_ulLength > 0))
	{
		if(m_eType != ODT_MEMORY_BLOCK)
		{
			if(ulOffset < m_dDataBufferLength)
			{
				ulMinLength = m_dDataBufferLength - ulOffset;
				if(p_ulLength < ulMinLength) ulMinLength = p_ulLength;

				if(m_pDataBuffer && (ulMinLength > 0))
				{
					pByte = (BYTE*)m_pDataBuffer + ulOffset;
					memcpy(p_pData, pByte, ulMinLength);
				}
				p_rulOffset += p_ulLength;
				oRetVal = TRUE;
			}
		}
	}
	return oRetVal;
}

BOOL CParameterBase::GetData(void* pData, DWORD length)
{
	DWORD minLength;

	if(pData && (length > 0))
	{
		minLength = m_dDataBufferLength;
		if(length < minLength) minLength = length;

		if(m_pDataBuffer && (minLength > 0)) memcpy(pData, m_pDataBuffer, minLength);
		return (length <= m_dDataBufferLength);
	}

	return FALSE;
}

DWORD CParameterBase::GetLength()
{
	return m_dDataBufferLength;
}

void CParameterBase::ResetData()
{
	if(m_pDataBuffer) memset(m_pDataBuffer, 0, m_dDataBufferLength);
}

DWORD CParameterBase::GetParameterTypeLength(EObjectDataType type)
{
	switch(type)
	{
		case ODT_UNKNOWN: return 0;
		case ODT_UINT8:
		case ODT_INT8: return 1;
		case ODT_UINT16:
		case ODT_INT16: return 2;
		case ODT_BOOLEAN:
		case ODT_UINT32:
		case ODT_INT32: return 4;
		case ODT_UINT64:
		case ODT_INT64: return 8;
		case ODT_FLOAT: return 4;
		case ODT_MEMORY_BLOCK: return 0;
		default: return 0;
	}
}

CStdString CParameterBase::GetVoidValueStr()
{
	CStdString str = _T("");
	BYTE* p;

	if(m_pDataBuffer && (m_dDataBufferLength > 0))
	{
		p = (BYTE*)m_pDataBuffer;
		for(DWORD i=0;i<m_dDataBufferLength;i++)
		{
			if(p)
			{
				TCHAR* pszAddStr = new TCHAR[32];
				_stprintf(pszAddStr,_T("0x%.2X"), *p++);
				str += pszAddStr;
				delete []pszAddStr;
				if(i < m_dDataBufferLength-1) str += _T(" ");
			}
		}
	}

	return str;
}

CStdString CParameterBase::GetBooleanValueStr(void* pData)
{
	if(pData)
	{
		if(*((BOOL*)pData))
		{
			return _T("True");
		}
		else
		{
			return _T("False");
		}
	}
	return "";
}

BOOL CParameterBase::SetVoidValueStr(CStdString valueStr, BOOL showMsg)
{
	CMmcDataConversion dataConversion;
	CStdString byteStr;
	BYTE byteValue;
	int index;
	CStdByteArray byteArray;
	BYTE* pDataBuffer = NULL;
	BYTE* p = NULL;

	if(valueStr.size()>0)
	{
		//Convert String
		valueStr.TrimRight();
		valueStr += ' ';
		while(!valueStr.IsEmpty())
		{
			index = valueStr.Find(' ', 0);
			if(index != -1)
			{
				byteStr = valueStr.Left(index);
				valueStr = valueStr.Right(valueStr.GetLength()-index);
				valueStr.TrimLeft();				
				if(dataConversion.HexByteStr2Byte(byteStr, &byteValue, showMsg > 0 ? true : false))
				{
					byteArray.push_back(byteValue);
				}
			}
			else break;
		}
		//Write Data
		pDataBuffer = (BYTE*)malloc(byteArray.size());
		if(pDataBuffer == NULL)
		{
			return FALSE;
		}
		else
		{
			p = pDataBuffer;
			for(std::size_t i=0;i<byteArray.size();i++)
				*p++ = byteArray.at(i);

			SetData(pDataBuffer, (DWORD)byteArray.size());
			free(pDataBuffer);
		}
	}
	else
	{
		//Reset DataBuffer
		SetData(NULL, 0);
	}

	return TRUE;
}

BOOL CParameterBase::SetBooleanValueStr(CStdString valueStr)
{
	BOOL oValue = FALSE;

	if(valueStr.CompareNoCase(_T("True")) == 0)
	{
		oValue = TRUE;
	}
	else
	{
		oValue = FALSE;
	}

	SetData(&oValue, sizeof(oValue));
	return TRUE;
}

CStdString CParameterBase::GetEditableStr()
{
	if(m_oEditable) return _T("True"); else return _T("False");
}

CStdString CParameterBase::GetVisibleStr()
{
	if(m_oVisible) return _T("True"); else return _T("False");
}

BOOL CParameterBase::InitEditable(CStdString editableStr)
{

	if(editableStr.CompareNoCase(_T("True")) == 0)
	{
		m_oEditable = TRUE;
		return TRUE;
	}

	if(editableStr.CompareNoCase(_T("False")) == 0)
	{
		m_oEditable = FALSE;
		return TRUE;
	}

	return FALSE;
}

BOOL CParameterBase::InitVisible(CStdString visibleStr)
{
	if(visibleStr.CompareNoCase(_T("True")) == 0)
	{
		m_oVisible = TRUE;
		return TRUE;
	}

	if(visibleStr.CompareNoCase(_T("False")) == 0)
	{
		m_oVisible = FALSE;
		return TRUE;
	}

	return FALSE;
}
