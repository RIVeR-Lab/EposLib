// ObjectEntry.cpp: Implementierung der Klasse CObjectEntry.
//
//////////////////////////////////////////////////////////////////////
#include <ObjectDictionary/ObjectFilter.h>
#include <Classes/MmcDataConversion.h>
#include <Classes/XML_PARSER.h>
#include <memory.h>
#include <ObjectDictionary/ObjectEntry.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CObjectEntry::CObjectEntry()
    : m_uSubIndex(0)
    , m_DataType(ODT_UNKNOWN)
    , m_strLowLimit(_T(""))
    , m_strHighLimit(_T(""))
    , m_AccessType(AT_READ_ONLY)
    , m_strDefault(_T(""))
    , m_oPDOMapping(0)
    , m_pValue(0)
    , m_dValueSize(0)
    , m_ValueStringFormat(OVF_HEX)
{
	Init();
}

CObjectEntry::~CObjectEntry()
{
    ResetValue();
}

CObjectEntry& CObjectEntry::operator=(CObjectEntry& other)
{
    if(this != &other)
    {
        *((CObjectEntryBase*)this) = *((CObjectEntryBase*)&other);

        m_uSubIndex = other.m_uSubIndex;
        m_DataType = other.m_DataType;
        m_AccessType = other.m_AccessType;
        m_oPDOMapping = other.m_oPDOMapping;
        m_strLowLimit = other.m_strLowLimit;
        m_strHighLimit = other.m_strHighLimit;
        m_strDefault = other.m_strDefault;
        m_strObjFlags = other.m_strObjFlags;
        m_ValueStringFormat = other.m_ValueStringFormat;
        //TODO:CHHE091209Can be serialised as soon as DtmEpos and DtmEsam also use this OBD classes
        //m_Denotation = other.m_Denotation;
        //m_UniqueId = other.m_UniqueId;

		ResetValue();
        if(other.m_dValueSize > 0)
        {
            m_pValue = (BYTE*)malloc(other.m_dValueSize);
            m_dValueSize = other.m_dValueSize;
            memcpy(m_pValue, other.m_pValue, other.m_dValueSize);
        }
    }

    return *this;
}

BOOL CObjectEntry::operator==(CObjectEntry& other)
{
    if(this != &other)
    {
        if((m_wIndex == other.m_wIndex) && (m_uSubIndex == other.m_uSubIndex)) return TRUE;

        return FALSE;
    }

    return TRUE;
}

void CObjectEntry::Serialize(CArchive &ar)
{
}

CObjectEntryBase* CObjectEntry::Clone()
{
    CObjectEntry* pNewObjectEntry;

    pNewObjectEntry = new CObjectEntry();
    *pNewObjectEntry = *this;

    return (CObjectEntryBase*)pNewObjectEntry;
}

BOOL CObjectEntry::ResetValue()
{
    BOOL oResult(TRUE);

    if(m_pValue)
    {
        free(m_pValue);
        m_pValue = NULL;
        m_dValueSize = 0;
    }

    return oResult;
}

BOOL CObjectEntry::InitValue(DWORD dSize)
{
    BOOL oResult(FALSE);

    ResetValue();
    if(dSize > 0)
    {
        m_dValueSize = dSize;
        m_pValue = (BYTE*)malloc(m_dValueSize);
        memset(m_pValue, 0, m_dValueSize);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CObjectEntry::Reset()
{
    BOOL oResult(TRUE);

    CObjectEntryBase::Reset();

    m_AccessType = AT_READ_ONLY;
    m_DataType = ODT_UNKNOWN;
    m_ValueStringFormat = OVF_HEX;
    m_oPDOMapping = 0;
    m_strDefault = _T("");
    m_strHighLimit = _T("");
    m_strLowLimit = _T("");
    m_strObjFlags = _T("0");
    m_uSubIndex = 0;

    if(!ResetValue()) oResult = FALSE;

    return oResult;
}

BOOL CObjectEntry::SetDataType(WORD dataType)
{
    BOOL oResult(TRUE);

    switch(dataType)
    {
        case 0: m_DataType = ODT_UNKNOWN; break;
        case 1: m_DataType = ODT_BOOLEAN; break;
        case 2: m_DataType = ODT_INT8; break;
        case 3: m_DataType = ODT_INT16; break;
        case 4: m_DataType = ODT_INT32; break;
        case 5: m_DataType = ODT_UINT8; break;
        case 6: m_DataType = ODT_UINT16; break;
        case 7: m_DataType = ODT_UINT32; break;
        case 8: m_DataType = ODT_FLOAT; break;
        case 9: m_DataType = ODT_STRING; break;
        case 15: m_DataType = ODT_DOMAIN; break;
        case 21: m_DataType = ODT_INT64; break;
        case 27: m_DataType = ODT_UINT64; break;
        default: m_DataType = ODT_UNKNOWN;
    }

    return oResult;
}

BOOL CObjectEntry::SetDataType(CStdString strDataType)
{
    BOOL oResult(TRUE);

    if(strDataType.Compare("Unknown") == 0) m_DataType = ODT_UNKNOWN;
    else if(strDataType.Compare("Boolean") == 0) m_DataType = ODT_BOOLEAN;
    else if(strDataType.Compare("Int8") == 0) m_DataType = ODT_INT8;
    else if(strDataType.Compare("Int16") == 0) m_DataType = ODT_INT16;
    else if(strDataType.Compare("Int32") == 0) m_DataType = ODT_INT32;
    else if(strDataType.Compare("Int64") == 0) m_DataType = ODT_INT64;
    else if(strDataType.Compare("UInt8") == 0) m_DataType = ODT_UINT8;
    else if(strDataType.Compare("UInt16") == 0) m_DataType = ODT_UINT16;
    else if(strDataType.Compare("UInt32") == 0) m_DataType = ODT_UINT32;
    else if(strDataType.Compare("UInt64") == 0) m_DataType = ODT_UINT64;
    else if(strDataType.Compare("Float") == 0) m_DataType = ODT_FLOAT;
    else if(strDataType.Compare("String") == 0) m_DataType = ODT_STRING;
    else if(strDataType.Compare("Domain") == 0) m_DataType = ODT_DOMAIN;
    else m_DataType = ODT_UNKNOWN;

    return oResult;
}

BOOL CObjectEntry::SetDataTypeLongStr(CStdString strDataType)
{
    BOOL oResult(TRUE);

    if(strDataType.Compare("Unknown") == 0) m_DataType = ODT_UNKNOWN;
    else if(strDataType.Compare("Boolean") == 0) m_DataType = ODT_BOOLEAN;
    else if(strDataType.Compare("Signed 8-Bit Integer") == 0) m_DataType = ODT_INT8;
    else if(strDataType.Compare("Signed 16-Bit Integer") == 0) m_DataType = ODT_INT16;
    else if(strDataType.Compare("Signed 32-Bit Integer") == 0) m_DataType = ODT_INT32;
    else if(strDataType.Compare("Signed 64-Bit Integer") == 0) m_DataType = ODT_INT64;
    else if(strDataType.Compare("Unsigned 8-Bit Integer") == 0) m_DataType = ODT_UINT8;
    else if(strDataType.Compare("Unsigned 16-Bit Integer") == 0) m_DataType = ODT_UINT16;
    else if(strDataType.Compare("Unsigned 32-Bit Integer") == 0) m_DataType = ODT_UINT32;
    else if(strDataType.Compare("Unsigned 64-Bit Integer") == 0) m_DataType = ODT_UINT64;
    else if(strDataType.Compare("32-Bit Floating Point") == 0) m_DataType = ODT_FLOAT;
    else if(strDataType.Compare("String") == 0) m_DataType = ODT_STRING;
    else if(strDataType.Compare("Domain") == 0) m_DataType = ODT_DOMAIN;
    else m_DataType = ODT_UNKNOWN;

    return oResult;
}

BOOL CObjectEntry::SetAccessType(CStdString accessType)
{
    BOOL oResult(FALSE);

    if(accessType.CompareNoCase("RW") == 0)
    {
        m_AccessType = AT_READ_WRITE;
        oResult = TRUE;
    }
    else if(accessType.CompareNoCase("RWW") == 0)
    {
        m_AccessType = AT_READ_WRITE_WRITE;
        oResult = TRUE;
    }
    else if(accessType.CompareNoCase("RWR") == 0)
    {
        m_AccessType = AT_READ_WRITE_READ;
        oResult = TRUE;
    }
    else if(accessType.CompareNoCase("WO") == 0)
    {
        m_AccessType = AT_WRITE_ONLY;
        oResult = TRUE;
    }
    else if(accessType.CompareNoCase("RO") == 0)
    {
        m_AccessType = AT_READ_ONLY;
        oResult = TRUE;
    }
    else if(accessType.CompareNoCase("Const") == 0)
    {
        m_AccessType = AT_READ_ONLY_CONST;
        oResult = TRUE;
    }
    else
    {
        m_AccessType = AT_READ_ONLY;
        oResult = FALSE;
    }

    return oResult;
}

BOOL CObjectEntry::SetSubIndex(BYTE subIndex)
{
    BOOL oResult(TRUE);

    m_uSubIndex = subIndex;

    return oResult;
}

BOOL CObjectEntry::SetObjFlags(DWORD objFlags)
{
    BOOL oResult(TRUE);

    m_strObjFlags.Format("%i", objFlags);

    return oResult;
}

BOOL CObjectEntry::SetObjFlags(CStdString objFlags)
{
    BOOL oResult(TRUE);

    m_strObjFlags = objFlags;

    return oResult;
}

BOOL CObjectEntry::SetPDOMapping(BYTE pdoMapping)
{
    BOOL oResult(TRUE);

    m_oPDOMapping = pdoMapping;

    return oResult;
}

BOOL CObjectEntry::SetLowLimit(CStdString lowLimit)
{
    BOOL oResult(TRUE);

    m_strLowLimit = lowLimit;

    return oResult;
}

BOOL CObjectEntry::SetHighLimit(CStdString highLimit)
{
    BOOL oResult(TRUE);

    m_strHighLimit = highLimit;

    return oResult;
}

BOOL CObjectEntry::SetDefaultValue(CStdString defaultValue)
{
    BOOL oResult(TRUE);

	m_strDefault = defaultValue;

    return oResult;
}

BOOL CObjectEntry::GetSubIndex(CStdString& strSubIndex)
{
    BOOL oResult(TRUE);

    strSubIndex.Format("0x%.2X", m_uSubIndex);

    return oResult;
}

BOOL CObjectEntry::SetSubIndex(CStdString strSubIndex)
{
    CMmcDataConversion dataConversion;
    BOOL oResult(TRUE);

    if(!dataConversion.HexByteStr2Byte(strSubIndex, &m_uSubIndex, FALSE))
    {
        oResult = FALSE;
        m_uSubIndex = 0;
    }

    return oResult;
}

BOOL CObjectEntry::GetDataType(CStdString& strDataType, BOOL oLongString)
{
    BOOL oResult(FALSE);

    if(oLongString)
    {
        strDataType = GetDataTypeLongStr();
        oResult = TRUE;
    }
    else
    {
        strDataType = GetDataTypeStr();
        oResult = TRUE;
    }

    return oResult;
}

CStdString CObjectEntry::GetDataTypeStr()
{
    switch(m_DataType)
    {
        case ODT_UNKNOWN: return "Unknown";
        case ODT_BOOLEAN: return "Boolean";
        case ODT_INT8: return "Int8";
        case ODT_INT16: return "Int16";
        case ODT_INT32: return "Int32";
        case ODT_INT64: return "Int64";
        case ODT_UINT8: return "UInt8";
        case ODT_UINT16: return "UInt16";
        case ODT_UINT32: return "UInt32";
        case ODT_UINT64: return "UInt64";
        case ODT_FLOAT: return "Float";
        case ODT_STRING: return "String";
        case ODT_DOMAIN: return "Domain";
        default: return "";
    }
}

__int64 CObjectEntry::GetDataTypeMaxValue()
{
    switch(m_DataType)
    {
        case ODT_UNKNOWN: return 0;
        case ODT_BOOLEAN: return 1;
        case ODT_INT8: return SCHAR_MAX;
        case ODT_INT16: return SHRT_MAX;
        case ODT_INT32: return LONG_MAX;
        case ODT_INT64: return LLONG_MAX;
        case ODT_UINT8: return UCHAR_MAX;
        case ODT_UINT16: return USHRT_MAX;
        case ODT_UINT32: return ULONG_MAX;
        case ODT_UINT64: return ULLONG_MAX;
        case ODT_FLOAT: return LLONG_MAX;
        case ODT_STRING: return 0;
        case ODT_DOMAIN: return 0;
        default: return 0;
    }
}

__int64 CObjectEntry::GetDataTypeMinValue()
{
    switch(m_DataType)
    {
        case ODT_UNKNOWN: return 0;
        case ODT_BOOLEAN: return 1;
        case ODT_INT8: return SCHAR_MIN;
        case ODT_INT16: return SHRT_MIN;
        case ODT_INT32: return LONG_MIN;
        case ODT_INT64: return LLONG_MIN;
        case ODT_UINT8: return 0;
        case ODT_UINT16: return 0;
        case ODT_UINT32: return 0;
        case ODT_UINT64: return 0;
        case ODT_FLOAT: return LLONG_MIN;
        case ODT_STRING: return 0;
        case ODT_DOMAIN: return 0;
        default: return 0;
    }
}

CStdString CObjectEntry::GetDataTypeLongStr()
{
    switch(m_DataType)
    {
        case ODT_UNKNOWN: return "Unknown";
        case ODT_BOOLEAN: return "Boolean";
        case ODT_INT8: return "Signed 8-Bit";
        case ODT_INT16: return "Signed 16-Bit";
        case ODT_INT32: return "Signed 32-Bit";
        case ODT_INT64: return "Signed 64-Bit";
        case ODT_UINT8: return "Unsigned 8-Bit";
        case ODT_UINT16: return "Unsigned 16-Bit";
        case ODT_UINT32: return "Unsigned 32-Bit";
        case ODT_UINT64: return "Unsigned 64-Bit";
        case ODT_FLOAT: return "32-Bit Floating Point";
        case ODT_STRING: return "String";
        case ODT_DOMAIN: return "Domain";
        default: return "";
    }
}

BOOL CObjectEntry::GetAccessType(CStdString& strAccessType, bool p_oLowerCase)
{
    BOOL oResult(FALSE);

    switch(m_AccessType)
    {
        case AT_READ_WRITE: strAccessType = "RW"; oResult = TRUE; break;
        case AT_READ_WRITE_READ: strAccessType =  "RW"; oResult = TRUE; break;
        case AT_READ_WRITE_WRITE: strAccessType =  "RW"; oResult = TRUE; break;
        case AT_WRITE_ONLY: strAccessType =  "WO"; oResult = TRUE; break;
        case AT_READ_ONLY: strAccessType =  "RO"; oResult = TRUE; break;
        case AT_READ_ONLY_CONST: strAccessType =  "Const"; oResult = TRUE; break;
        default: strAccessType =  ""; oResult = FALSE; break;
    }
    if(p_oLowerCase)
	{
		strAccessType.MakeLower();
	}
    return oResult;
}

BOOL CObjectEntry::GetAccessTypeAttribute(CStdString& strAccessType)
{
    BOOL oResult(FALSE);

    switch(m_AccessType)
    {
        case AT_READ_WRITE: strAccessType = "rw"; oResult = TRUE; break;
        case AT_READ_WRITE_READ: strAccessType =  "rwr"; oResult = TRUE; break;
        case AT_READ_WRITE_WRITE: strAccessType =  "rww"; oResult = TRUE; break;
        case AT_WRITE_ONLY: strAccessType =  "wo"; oResult = TRUE; break;
        case AT_READ_ONLY: strAccessType =  "ro"; oResult = TRUE; break;
        case AT_READ_ONLY_CONST: strAccessType =  "const"; oResult = TRUE; break;
        default: strAccessType =  ""; oResult = FALSE; break;
    }

    return oResult;
}

BOOL CObjectEntry::GetDefaultValue(CStdString& strDefaultValue)
{
    BOOL oResult(TRUE);

    strDefaultValue = m_strDefault;

    return oResult;
}

BOOL CObjectEntry::GetPDOMapping(CStdString& strPdoMapping)
{
    BOOL oResult(TRUE);

    if(m_oPDOMapping) strPdoMapping = "1"; else strPdoMapping = "0";

    return oResult;
}

BOOL CObjectEntry::SetPDOMapping(CStdString strPDOMapping)
{
    BOOL oResult(FALSE);

    if(strPDOMapping.Compare("1") == 0)
    {
        m_oPDOMapping = TRUE;
        oResult = TRUE;
    }
    else
    {
        m_oPDOMapping = FALSE;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CObjectEntry::SetPDOMappingToken(CStdString strPDOMapping)
{
	BOOL oResult = FALSE;

	if((strPDOMapping.Find("TPDO") != -1) || (strPDOMapping.Find("RPDO") != -1) || (strPDOMapping.Find("default") != -1) || (strPDOMapping.Find("optional") != -1))
	{
		m_oPDOMapping = TRUE;
		oResult = TRUE;
	}
	else if(strPDOMapping.Find("no") != -1)
	{
		m_oPDOMapping = FALSE;
		oResult = TRUE;
	}
	else
	{
		m_oPDOMapping = FALSE;
		oResult = FALSE;
	}

	return oResult;
}

BOOL CObjectEntry::GetPDOMappingToken(CStdString& strPdoMapping)
{
    BOOL oResult(TRUE);

    if(m_oPDOMapping) strPdoMapping = "optional"; else strPdoMapping = "no";

    return oResult;
}

BOOL CObjectEntry::GetObjFlags(CStdString& strObjFlags)
{
    BOOL oResult(TRUE);

    strObjFlags = m_strObjFlags;

    return oResult;
}

DWORD CObjectEntry::GetObjFlags()
{
    CMmcDataConversion conversion;
    DWORD ulObjFlags(0);

    if(conversion.DecDWordStr2DWord(m_strObjFlags, &ulObjFlags, FALSE))
    {
        return ulObjFlags;
    }

    return 0;
}

BOOL CObjectEntry::GetLowLimit(CStdString& strLowLimit)
{
    BOOL oResult(TRUE);

    strLowLimit = m_strLowLimit;

    return oResult;
}

BOOL CObjectEntry::GetHighLimit(CStdString& strHighLimit)
{
    BOOL oResult(TRUE);

    strHighLimit = m_strHighLimit;

    return oResult;
}

EObjectDataType CObjectEntry::GetDataType()
{
    return m_DataType;
}

EAccessType CObjectEntry::GetAccessType()
{
    return m_AccessType;
}

BYTE CObjectEntry::GetSubIndex()
{
    return m_uSubIndex;
}

BOOL CObjectEntry::SetValueStringFormat(CStdString strFormattedValue)
{
    CMmcDataConversion conversion;
    unsigned __int64 uInt64Value(0);
    CStdString strValue = _T("");
    int iIndex;
    BOOL oResult(TRUE);

    //Remove leading text
    iIndex = strFormattedValue.FindOneOf("0123456789");
    if(iIndex != -1) strValue = strFormattedValue.Right(strFormattedValue.GetLength()-iIndex); else strValue = strFormattedValue;

    //Check Types
    if(conversion.DecUInt64Str2UInt64(strValue, &uInt64Value, FALSE))
    {
        m_ValueStringFormat = OVF_DEC;
        oResult = TRUE;
    }
    else if(conversion.HexUInt64Str2UInt64(strValue, &uInt64Value, FALSE))
    {
        m_ValueStringFormat = OVF_HEX;
        oResult = TRUE;
    }
    else
    {
        m_ValueStringFormat = OVF_DEC;
        oResult = FALSE;
    }

    return oResult;
}

BOOL CObjectEntry::SetValue(CStdString strValue)
{
    CMmcDataConversion conversion;
    BOOL oResult(FALSE);

    //Reset
    ResetValue();

    //SetValue
    switch(m_DataType)
    {
        case ODT_BOOLEAN:
            {
                if(InitValue(sizeof(WORD)))
                {
                    if(strValue.CompareNoCase("TRUE") == 0)
                    {
                        *((WORD*)m_pValue) = TRUE;
                        oResult = TRUE;
                    }
                    else
                    {
                        *((WORD*)m_pValue) = FALSE;
                        oResult = TRUE;
                    }
                }
            };break;

        case ODT_INT8:
            {
                if(InitValue(sizeof(char)))
                {
                    oResult = conversion.CharStr2Char(strValue, ((char*)m_pValue), FALSE);
                }
            };break;
        case ODT_INT16:
            {
                if(InitValue(sizeof(short)))
                {
                    oResult = conversion.ShortStr2Short(strValue, ((short*)m_pValue), FALSE);
                }
            };break;
        case ODT_INT32:
            {
                if(InitValue(sizeof(long)))
                {
                    oResult = conversion.LongStr2Long(strValue, ((long*)m_pValue), FALSE);
                }
            };break;
        case ODT_INT64:
            {
                if(InitValue(sizeof(__int64)))
                {
                    oResult = conversion.Int64Str2Int64(strValue, ((__int64*)m_pValue), FALSE);
                }
            };break;
        case ODT_UINT8:
            {
                if(InitValue(sizeof(BYTE)))
                {
                    oResult = conversion.ByteStr2Byte(strValue, ((BYTE*)m_pValue), FALSE);
                }
            };break;
        case ODT_UINT16:
            {
                if(InitValue(sizeof(WORD)))
                {
                    oResult = conversion.WordStr2Word(strValue, ((WORD*)m_pValue));
                }
            };break;
        case ODT_UINT32:
            {
                if(InitValue(sizeof(DWORD)))
                {
                    oResult = conversion.DWordStr2DWord(strValue, ((DWORD*)m_pValue), FALSE);
                }
            };break;
        case ODT_UINT64:
            {
                if(InitValue(sizeof(unsigned __int64)))
                {
                    oResult = conversion.UInt64Str2UInt64(strValue, ((unsigned __int64*)m_pValue), FALSE);
                }
            };break;
        case ODT_FLOAT:
            {
                if(InitValue(sizeof(float)))
                {
                    oResult = conversion.FloatStr2Float(strValue, ((float*)m_pValue), FALSE);
                }
            };break;
        case ODT_STRING:
            {
                if(InitValue(strlen(strValue))*sizeof(TCHAR))
                {
                    for(DWORD d=0;d<m_dValueSize;d++)
                    {
                        *((TCHAR*)m_pValue+d) = strValue.GetAt(d);
                    }
                    oResult = TRUE;
                }
            };break;
    }

    return oResult;
}

BOOL CObjectEntry::IsValueAvailable()
{
    BOOL oResult(FALSE);

    if(m_pValue)
    {
        oResult = TRUE;
    }

    return oResult;
}

DWORD CObjectEntry::GetValueSize()
{
	return m_dValueSize;
}

BOOL CObjectEntry::GetValue(CStdString& strValue, EObjectValueStringFormat eFormat)
{
    EObjectValueStringFormat eStringFormat(OVF_UNKNOWN);
    CMmcDataConversion conversion;
    BOOL oResult(FALSE);

    //ValueFormat
    if(eFormat == OVF_UNKNOWN) eStringFormat = m_ValueStringFormat; else eStringFormat = eFormat;

    //GetValues
    if(m_pValue && ((m_dValueSize == GetSize()) || (m_DataType == ODT_STRING)))
    {
        if(m_AccessType == AT_WRITE_ONLY)
        {
            strValue = "Can't be read!";
            oResult = TRUE;
        }
        else
        {
            switch(m_DataType)
            {
                case ODT_BOOLEAN:
                    {
                        if(*((WORD*)m_pValue) == 0) strValue = "FALSE"; else strValue = "TRUE";
                        oResult = TRUE;

                    };break;
                case ODT_INT8:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.Char2HexCharStr(*((BYTE*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.Char2DecCharStr(*((BYTE*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_INT16:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.Short2HexShortStr(*((short*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.Short2DecShortStr(*((short*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_INT32:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.Long2HexLongStr(*((long*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.Long2DecLongStr(*((long*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_INT64:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.Int64_2HexInt64Str(*((__int64*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.Int64_2DecInt64Str(*((__int64*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_UINT8:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.UChar2HexUCharStr(*((char*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.UChar2DecUCharStr(*((char*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_UINT16:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.Word2HexWordStr(*((WORD*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.Word2DecWordStr(*((WORD*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_UINT32:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.DWord2HexDWordStr(*((DWORD*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.DWord2DecDWordStr(*((DWORD*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_UINT64:
                    {
                        if(OVF_HEX == eStringFormat)
                        {
                            oResult = conversion.UInt64_2HexUInt64Str(*((unsigned __int64*)m_pValue), &strValue);
                        }
                        else
                        {
                            oResult = conversion.UInt64_2DecUInt64Str(*((unsigned __int64*)m_pValue), &strValue);
                        }

                    };break;
                case ODT_FLOAT:
                    {
                        oResult = conversion.Float2DecFloatStr(*((float*)m_pValue), &strValue);

                    };break;
                case ODT_STRING:
                    {
                        strValue = "";
                        for(DWORD d=0;d<m_dValueSize;d++)
                        {
                            strValue += *((char*)m_pValue+d);
                        }

                        oResult = TRUE;

                    };break;
                case ODT_DOMAIN:
                    {
                        strValue = "Can't be displayed";
                        oResult = TRUE;

                    };break;
            }
        }
    }

    return oResult;
}

CObjectEntry* CObjectEntry::SearchObject(WORD index, WORD subIndex)
{
    if((index == m_wIndex) && (subIndex == m_uSubIndex))
    {
        return this;
    }
    else
    {
        return NULL;
    }
}

WORD CObjectEntry::GetWordSize()
{
    switch(m_DataType)
    {
        case ODT_UNKNOWN: return 0;
        case ODT_BOOLEAN: return 0;
        case ODT_INT8: return 1;
        case ODT_INT16: return 1;
        case ODT_INT32: return 2;
        case ODT_INT64: return 4;
        case ODT_UINT8: return 1;
        case ODT_UINT16: return 1;
        case ODT_UINT32: return 2;
        case ODT_UINT64: return 4;
        case ODT_FLOAT: return 2;
        case ODT_STRING: return 0;
        case ODT_DOMAIN: return 0;
        default: return 0;
    }
}

DWORD CObjectEntry::GetSize()
{
    switch(m_DataType)
    {
        case ODT_UNKNOWN: return 0;
        //BOOLEAN id OBD is actually only 1Bit! This is not handled yet (and also not used so far).
        case ODT_BOOLEAN: return 1;
        case ODT_INT8: return sizeof(__int8);
        case ODT_INT16: return sizeof(__int16);
        case ODT_INT32: return sizeof(__int32);
        case ODT_INT64: return sizeof(__int64);
        case ODT_UINT8: return sizeof(BYTE);
        case ODT_UINT16: return sizeof(WORD);
        case ODT_UINT32: return sizeof(DWORD);
        case ODT_UINT64: return (sizeof(DWORD)*2);
        case ODT_FLOAT: return sizeof(float);
        case ODT_STRING: return 0;
        case ODT_DOMAIN: return 0;
        default: return 0;
    }
}

WORD CObjectEntry::GetDataTypeNumber()
{
    return m_DataType;
}

BOOL CObjectEntry::GetDataTypeNumber(CStdString& p_rDataType)
{
	BOOL oResult = FALSE;

	if(m_ValueStringFormat == OVF_DEC)
	{
		p_rDataType.Format("%i", m_DataType);
		oResult = TRUE;

	}
	else
	{
		p_rDataType.Format("0x%.2X", (BYTE)m_DataType);
		oResult = TRUE;
	}

	return oResult;
}

BOOL CObjectEntry::IsPdoMappable()
{
    BOOL oResult(FALSE);

    if(IsRxPdoMappable() || IsTxPdoMappable())
    {
        oResult = TRUE;
    }

    return oResult;
}

BOOL CObjectEntry::IsRxPdoMappable()
{
    BOOL oResult(FALSE);

	if(m_oPDOMapping && ((m_AccessType == AT_READ_WRITE) || (m_AccessType == AT_READ_WRITE_READ) || (m_AccessType == AT_READ_WRITE_WRITE) || (m_AccessType == AT_WRITE_ONLY)))
    {
        oResult = TRUE;
    }

    return oResult;
}

BOOL CObjectEntry::IsTxPdoMappable()
{
    BOOL oResult(FALSE);

    if(m_oPDOMapping && ((m_AccessType == AT_READ_WRITE) || (m_AccessType == AT_READ_WRITE_WRITE) || (m_AccessType == AT_READ_WRITE_READ) || (m_AccessType == AT_READ_ONLY)))
    {
        oResult = TRUE;
    }

    return oResult;
}

CObjectEntry* CObjectEntry::CloneObjectEntry()
{
    CObjectEntry* pNewObjectEntry;

    pNewObjectEntry = new CObjectEntry();
    *pNewObjectEntry = *this;

    return (CObjectEntry*)pNewObjectEntry;
}

int CObjectEntry::GetNumberOfSubObjects()
{
    return 1;
}

BOOL CObjectEntry::IsVisible()
{
    return m_pObjectFilter->IsVisibleObject(m_strParameterName, m_wIndex, m_uSubIndex);
}

BOOL CObjectEntry::StoreObjectDictionarySchema(XML_PARSER* pXmlParser)
{
    CStdString strIndex = "";
    CStdString strSubIndex = "";
    CStdString strParameterName = "";
    CStdString strObjectType = "";
    CStdString strDataType = "";
    CStdString strAccessType = "";
    CStdString strLowLimit = "";
    CStdString strHighLimit = "";
    CStdString strDefaultValue = "";
    CStdString strPDOMapping = "";
    CStdString strObjFlags = "";

    if(pXmlParser)
    {
        //ObjectEntry
        pXmlParser->AddNewLineTab();
        pXmlParser->Add_LastChildNode(_T("ObjectEntry"));
        pXmlParser->IncTabLevel();

        //Prepare Attributes
        GetIndex(strIndex);
        GetSubIndex(strSubIndex);
        GetParameterName(strParameterName);
        GetObjectType(strObjectType);
        GetDataType(strDataType, FALSE);
        GetAccessType(strAccessType);
        GetLowLimit(strLowLimit);
        GetHighLimit(strHighLimit);
        GetDefaultValue(strDefaultValue);
        GetPDOMapping(strPDOMapping);
        GetObjFlags(strObjFlags);

        //ObjectEntry Attributes
        pXmlParser->Set_Attribute(_T("Index"), strIndex);
        pXmlParser->Set_Attribute(_T("SubIndex"), strSubIndex);
        pXmlParser->Set_Attribute(_T("ParameterName"), strParameterName);
        pXmlParser->Set_Attribute(_T("ObjectType"), strObjectType);
        pXmlParser->Set_Attribute(_T("DataType"), strDataType);
        pXmlParser->Set_Attribute(_T("AccessType"), strAccessType);

        if(!strLowLimit.IsEmpty()) pXmlParser->Set_Attribute(_T("LowLimit"), strLowLimit);
        if(!strHighLimit.IsEmpty()) pXmlParser->Set_Attribute(_T("HighLimit"), strHighLimit);
        if(!strDefaultValue.IsEmpty()) pXmlParser->Set_Attribute(_T("DefaultValue"), strDefaultValue);
        if(!strPDOMapping.IsEmpty()) pXmlParser->Set_Attribute(_T("PDOMapping"), strPDOMapping);
        if(!strObjFlags.IsEmpty()) pXmlParser->Set_Attribute(_T("ObjFlags"), strObjFlags);

        //End ObjectEntry
        pXmlParser->DecTabLevel();
        pXmlParser->AddNewLineTab();
        pXmlParser->Go_to_Parent();

        return TRUE;
    }

    return FALSE;
}

void CObjectEntry::Parse_XML_Document(XML_PARSER* pXmlParser)
{
    if(pXmlParser)
    {
        //*Attributes*
        //Index
        if(pXmlParser->Is_Having_Attribute(_T("Index")))
        {
            SetIndex(pXmlParser->Get_Attribute_Value());
        }

        //SubIndex
        if(pXmlParser->Is_Having_Attribute(_T("SubIndex")))
        {
            SetSubIndex(pXmlParser->Get_Attribute_Value());
        }

        //ParameterName
        if(pXmlParser->Is_Having_Attribute(_T("ParameterName")))
        {
            SetParameterName(pXmlParser->Get_Attribute_Value());
        }

        //ObjectType
        if(pXmlParser->Is_Having_Attribute(_T("ObjectType")))
        {
            SetObjectType(pXmlParser->Get_Attribute_Value());
        }

        //DataType
        if(pXmlParser->Is_Having_Attribute(_T("DataType")))
        {
            SetDataType(pXmlParser->Get_Attribute_Value());
        }

        //AccessType
        if(pXmlParser->Is_Having_Attribute(_T("AccessType")))
        {
            SetAccessType(pXmlParser->Get_Attribute_Value());
        }

        //Default
        if(pXmlParser->Is_Having_Attribute(_T("DefaultValue")))
        {
            SetDefaultValue(pXmlParser->Get_Attribute_Value());
        }

        //LowLimit
        if(pXmlParser->Is_Having_Attribute(_T("LowLimit")))
        {
            SetLowLimit(pXmlParser->Get_Attribute_Value());
        }

        //HighLimit
        if(pXmlParser->Is_Having_Attribute(_T("HighLimit")))
        {
            SetHighLimit(pXmlParser->Get_Attribute_Value());
        }

        //PDOMapping
        if(pXmlParser->Is_Having_Attribute(_T("PDOMapping")))
        {
            SetPDOMapping(pXmlParser->Get_Attribute_Value());
        }

        //ObjFlags
        if(pXmlParser->Is_Having_Attribute(_T("ObjFlags")))
        {
            SetObjFlags(pXmlParser->Get_Attribute_Value());
        }
    }
}

BOOL CObjectEntry::GetValue(BYTE* pValue, DWORD dValueSize)
{
    BOOL oResult(FALSE);

    if(pValue && m_pValue && (dValueSize > 0) && (m_dValueSize > 0))
    {
        if(dValueSize == m_dValueSize)
        {
            if(m_AccessType != AT_WRITE_ONLY)
            {
                memcpy(pValue, m_pValue, dValueSize);
                oResult = TRUE;
            }
            else
            {
                memcpy(pValue, 0, dValueSize);
                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CObjectEntry::SetValue(BYTE* pValue, DWORD dValueSize)
{
    BOOL oResult(FALSE);

    ResetValue();
    if(InitValue(dValueSize))
    {
        memcpy(m_pValue, pValue, dValueSize);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CObjectEntry::IsEqual(void* pValue, DWORD dValueSize)
{
    if(m_dValueSize == dValueSize)
    {
        if(memcmp(m_pValue, pValue, dValueSize) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CObjectEntry::IsEqual(CObjectEntry& other)
{
    if(m_dValueSize == other.m_dValueSize)
    {
        if(memcmp(m_pValue, other.m_pValue, m_dValueSize) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CObjectEntry::AddClonedObjectEntry(tObjectEntryList* pObjectEntryList, CObjectEntry* pObjectEntry)
{
    CObjectEntry* pClonedObjectEntry;

    if(pObjectEntryList && pObjectEntry)
    {
        pClonedObjectEntry = pObjectEntry->CloneObjectEntry();
        pObjectEntryList->push_back(pClonedObjectEntry);

        return TRUE;
    }

    return FALSE;
}

CStdString CObjectEntry::GetParameterNameStr()
{
    return m_strParameterName;
}

BOOL CObjectEntry::IsWritable()
{
	BOOL oResult = FALSE;

	//Check AccessType
	if((GetAccessType() == AT_READ_WRITE) || (GetAccessType() == AT_WRITE_ONLY) || (GetAccessType() == AT_READ_WRITE_WRITE) || (GetAccessType() == AT_READ_WRITE_READ))
	{
		oResult = TRUE;
	}

	return oResult;
}

BOOL CObjectEntry::IsReadable()
{
	BOOL oResult = FALSE;

	//Check AccessType
	if((GetAccessType() == AT_READ_WRITE) || (GetAccessType() == AT_READ_ONLY) || (GetAccessType() == AT_READ_ONLY_CONST) || (GetAccessType() == AT_READ_WRITE_WRITE) || (GetAccessType() == AT_READ_WRITE_READ))
	{
		oResult = TRUE;
	}

	return oResult;
}

BOOL CObjectEntry::IsWriteOnDownloadAllowed()
{
	const DWORD REFUSE_WRITE_BIT = 0x01;

	BOOL oResult = FALSE;

	//Check ObjFlags
	if((GetObjFlags() & REFUSE_WRITE_BIT) == 0)
	{
		//Check AccessType
		if((GetAccessType() == AT_READ_WRITE) || (GetAccessType() == AT_READ_WRITE_WRITE) || (GetAccessType() == AT_READ_WRITE_READ))
		{
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CObjectEntry::IsReadOnUploadAllowed()
{
	const DWORD REFUSE_READ_BIT = 0x02;

	BOOL oResult = FALSE;

	//Check ObjFlags
	if((GetObjFlags() & REFUSE_READ_BIT) == 0)
	{
		//Check AccessType
		if(GetAccessType() != AT_WRITE_ONLY)
		{
			oResult = TRUE;
		}
	}

	return oResult;
}
