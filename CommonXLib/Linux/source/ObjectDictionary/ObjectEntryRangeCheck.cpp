// ObjectEntryRangeCheck.cpp: Implementierung der Klasse CObjectEntryRangeCheck.
//
//////////////////////////////////////////////////////////////////////
#include <Classes/MmcDataConversion.h>
#include <memory.h>
#include <ObjectDictionary/ObjectEntry.h>
#include <ObjectDictionary/ObjectEntryRangeCheck.h>
#include <Ui/MmcMessageDialog.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CObjectEntryRangeCheck::CObjectEntryRangeCheck()
{
}

CObjectEntryRangeCheck::~CObjectEntryRangeCheck()
{
}

BOOL CObjectEntryRangeCheck::DoRangeCheck(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	EObjectDataType dataType;
	BOOL oResult = FALSE;

	if(pObjectEntry)
	{
		dataType = pObjectEntry->GetDataType();
		switch(dataType)
		{
			case ODT_BOOLEAN: oResult = DoRangeCheck_BooleanValue(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT8: oResult = DoRangeCheck_Int8Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT16: oResult = DoRangeCheck_Int16Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT32: oResult = DoRangeCheck_Int32Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT64: oResult = DoRangeCheck_Int64Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT8: oResult = DoRangeCheck_UInt8Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT16: oResult = DoRangeCheck_UInt16Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT32: oResult = DoRangeCheck_UInt32Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT64: oResult = DoRangeCheck_UInt64Value(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_FLOAT: oResult = DoRangeCheck_FloatValue(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_STRING: oResult = DoRangeCheck_StringValue(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_DOMAIN: oResult = DoRangeCheck_DomainValue(pObjectEntry, pDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			default: return TRUE;
		}
	}

	return oResult;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	EObjectDataType dataType;
	BOOL oResult = FALSE;

	if(pObjectEntry)
	{
		dataType = pObjectEntry->GetDataType();
		switch(dataType)
		{
			case ODT_BOOLEAN: oResult = DoStaticRangeCheck_BooleanValue(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT8: oResult = DoStaticRangeCheck_Int8Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT16: oResult = DoStaticRangeCheck_Int16Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT32: oResult = DoStaticRangeCheck_Int32Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_INT64: oResult = DoStaticRangeCheck_Int64Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT8: oResult = DoStaticRangeCheck_UInt8Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT16: oResult = DoStaticRangeCheck_UInt16Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT32: oResult = DoStaticRangeCheck_UInt32Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_UINT64: oResult = DoStaticRangeCheck_UInt64Value(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_FLOAT: oResult = DoStaticRangeCheck_FloatValue(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_STRING: oResult = DoStaticRangeCheck_StringValue(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			case ODT_DOMAIN: oResult = DoStaticRangeCheck_DomainValue(pObjectEntry, pDataBuffer, pLowDataBuffer, pHighDataBuffer, dDataSize, oLimitData, oShowMsg, oMsgInHexFormat);break;
			default: return TRUE;
		}
	}

	return oResult;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, void* pLowDataBuffer, DWORD dLowDataSize, void* pHighDataBuffer, DWORD dHighDataSize)
{
	EObjectDataType dataType;
	BOOL oResult = FALSE;

	if(pObjectEntry)
	{
		if((pObjectEntry->GetSize() == dLowDataSize) && (pObjectEntry->GetSize() == dHighDataSize))
		{
			dataType = pObjectEntry->GetDataType();
			switch(dataType)
			{
				case ODT_BOOLEAN: oResult = GetRangeValues(pObjectEntry, (long*)pLowDataBuffer, (long*)pHighDataBuffer); break;
				case ODT_INT8: oResult = GetRangeValues(pObjectEntry, (char*)pLowDataBuffer, (char*)pHighDataBuffer); break;
				case ODT_INT16: oResult = GetRangeValues(pObjectEntry, (short*)pLowDataBuffer, (short*)pHighDataBuffer); break;
				case ODT_INT32: oResult = GetRangeValues(pObjectEntry, (long*)pLowDataBuffer, (long*)pHighDataBuffer); break;
				case ODT_INT64: oResult = GetRangeValues(pObjectEntry, (__int64*)pLowDataBuffer, (__int64*)pHighDataBuffer); break;
				case ODT_UINT8: oResult = GetRangeValues(pObjectEntry, (BYTE*)pLowDataBuffer, (BYTE*)pHighDataBuffer); break;
				case ODT_UINT16: oResult = GetRangeValues(pObjectEntry, (WORD*)pLowDataBuffer, (WORD*)pHighDataBuffer); break;
				case ODT_UINT32: oResult = GetRangeValues(pObjectEntry, (DWORD*)pLowDataBuffer, (DWORD*)pHighDataBuffer); break;
				case ODT_UINT64: oResult = GetRangeValues(pObjectEntry, (unsigned __int64*)pLowDataBuffer, (unsigned __int64*)pHighDataBuffer); break;
				case ODT_FLOAT: oResult = GetRangeValues(pObjectEntry, (float*)pLowDataBuffer, (float*)pHighDataBuffer); break;
				default: return FALSE;
			}
		}
	}

	return oResult;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, char p_cValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT8:
                    {
                        p_pVoidContainer = new char;
                        p_rSizeOfVoidContainer = sizeof(char);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, short p_sValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT8:
                    {
                        p_pVoidContainer = new char;
                        p_rSizeOfVoidContainer = sizeof(char);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT16:
                    {
                        p_pVoidContainer = new short;
                        p_rSizeOfVoidContainer = sizeof(short);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, long p_lValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT8:
                    {
                        p_pVoidContainer = new char;
                        p_rSizeOfVoidContainer = sizeof(char);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT16:
                    {
                        p_pVoidContainer = new short;
                        p_rSizeOfVoidContainer = sizeof(short);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT32:
                    {
                        p_pVoidContainer = new long;
                        p_rSizeOfVoidContainer = sizeof(long);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_UINT16:
                    {
                        p_pVoidContainer = new unsigned short;
                        p_rSizeOfVoidContainer = sizeof(unsigned short);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, long long p_llValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT8:
                    {
                        p_pVoidContainer = new char;
                        p_rSizeOfVoidContainer = sizeof(char);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT16:
                    {
                        p_pVoidContainer = new short;
                        p_rSizeOfVoidContainer = sizeof(short);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT32:
                    {
                        p_pVoidContainer = new long;
                        p_rSizeOfVoidContainer = sizeof(long);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_INT64:
                    {
                        p_pVoidContainer = new long long;
                        p_rSizeOfVoidContainer = sizeof(long long);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_UINT16:
                    {
                        p_pVoidContainer = new unsigned short;
                        p_rSizeOfVoidContainer = sizeof(unsigned short);
                        oRetVal = TRUE;
                    }
                    break;
                case ODT_UINT32:
                    {
                        p_pVoidContainer = new unsigned long;
                        p_rSizeOfVoidContainer = sizeof(unsigned long);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned char p_ucValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned short p_usValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT16:
                    {
                        p_pVoidContainer = new unsigned short;
                        p_rSizeOfVoidContainer = sizeof(unsigned short);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned long p_ulValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT16:
                    {
                        p_pVoidContainer = new unsigned short;
                        p_rSizeOfVoidContainer = sizeof(unsigned short);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT32:
                    {
                        p_pVoidContainer = new unsigned long;
                        p_rSizeOfVoidContainer = sizeof(unsigned long);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned int p_ulValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT16:
                    {
                        p_pVoidContainer = new unsigned short;
                        p_rSizeOfVoidContainer = sizeof(unsigned short);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT32:
                    {
                        p_pVoidContainer = new unsigned int;
                        p_rSizeOfVoidContainer = sizeof(unsigned int);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned long long p_ullValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg)
{
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pVoidContainer)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        p_pVoidContainer = new BOOL;
                        p_rSizeOfVoidContainer = sizeof(BOOL);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT8:
                    {
                        p_pVoidContainer = new unsigned char;
                        p_rSizeOfVoidContainer = sizeof(unsigned char);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT16:
                    {
                        p_pVoidContainer = new unsigned short;
                        p_rSizeOfVoidContainer = sizeof(unsigned short);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT32:
                    {
                        p_pVoidContainer = new unsigned long;
                        p_rSizeOfVoidContainer = sizeof(unsigned long);
                        oRetVal = TRUE;
                    }
                    break;
			    case ODT_UINT64:
                    {
                        p_pVoidContainer = new unsigned long long;
                        p_rSizeOfVoidContainer = sizeof(unsigned long long);
                        oRetVal = TRUE;
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_CONTAINER_NOT_FIT);
        }
	    return oRetVal;
}

BOOL CObjectEntryRangeCheck::IsRangeCheckAvailable(CObjectEntry* pObjectEntry)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_BooleanValue(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	BYTE* pData = (BYTE*)pDataBuffer;
	unsigned __int64 ddValue;

	if(pData && (dDataSize >= sizeof(*pData)))
	{
		ddValue = *pData;
		if(ddValue > 1)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, 1, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memset(pDataBuffer, 0, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_BooleanValue(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	BYTE* pData = (BYTE*)pDataBuffer;
	unsigned __int64 ddValue;

	if(pData && (dDataSize >= sizeof(*pData)))
	{
		ddValue = *pData;
		if(ddValue > 1)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, 1, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memset(pDataBuffer, 0, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_Int8Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	char* pData = (char*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		llValue = *pData;
		if(GetRangeValues(pObjectEntry, &llLowValue, &llHighValue))
		{
			if(llValue < llLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

				return FALSE;
			}
			else if(llValue > llHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_Int8Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	char* pData = (char*)pDataBuffer;
	char* pLowData = (char*)pLowDataBuffer;
	char* pHighData = (char*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		llValue = *pData;
		llLowValue = *pLowData;
		llHighValue = *pHighData;

		if(llValue < llLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

			return FALSE;
		}
		else if(llValue > llHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_Int16Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	short* pData = (short*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		llValue = *pData;
		if(GetRangeValues(pObjectEntry, &llLowValue, &llHighValue))
		{
			if(llValue < llLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

				return FALSE;
			}
			else if(llValue > llHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_Int16Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	short* pData = (short*)pDataBuffer;
	short* pLowData = (short*)pLowDataBuffer;
	short* pHighData = (short*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		llValue = *pData;
		llLowValue = *pLowData;
		llHighValue = *pHighData;

		if(llValue < llLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

			return FALSE;
		}
		else if(llValue > llHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_Int32Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	long* pData = (long*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		llValue = *pData;
		if(GetRangeValues(pObjectEntry, &llLowValue, &llHighValue))
		{
			if(llValue < llLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

				return FALSE;
			}
			else if(llValue > llHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_Int32Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	long* pData = (long*)pDataBuffer;
	long* pLowData = (long*)pLowDataBuffer;
	long* pHighData = (long*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		llValue = *pData;
		llLowValue = *pLowData;
		llHighValue = *pHighData;

		if(llValue < llLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

			return FALSE;
		}
		else if(llValue > llHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_Int64Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	__int64* pData = (__int64*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		llValue = *pData;
		if(GetRangeValues(pObjectEntry, &llLowValue, &llHighValue))
		{
			if(llValue < llLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

				return FALSE;
			}
			else if(llValue > llHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_Int64Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	__int64 llLowValue, llHighValue;
	__int64 llValue;
	__int64* pData = (__int64*)pDataBuffer;
	__int64* pLowData = (__int64*)pLowDataBuffer;
	__int64* pHighData = (__int64*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		llValue = *pData;
		llLowValue = *pLowData;
		llHighValue = *pHighData;

		if(llValue < llLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, llValue, llLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llLowValue, dDataSize);

			return FALSE;
		}
		else if(llValue > llHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, llValue, llHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &llHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_UInt8Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	BYTE* pData = (BYTE*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		ddValue = *pData;
		if(GetRangeValues(pObjectEntry, &ddLowValue, &ddHighValue))
		{
			if(ddValue < ddLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

				return FALSE;
			}
			else if(ddValue > ddHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_UInt8Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	BYTE* pData = (BYTE*)pDataBuffer;
	BYTE* pLowData = (BYTE*)pLowDataBuffer;
	BYTE* pHighData = (BYTE*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		ddValue = *pData;
		ddLowValue = *pLowData;
		ddHighValue = *pHighData;

		if(ddValue < ddLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

			return FALSE;
		}
		else if(ddValue > ddHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_UInt16Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	WORD* pData = (WORD*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		ddValue = *pData;
		if(GetRangeValues(pObjectEntry, &ddLowValue, &ddHighValue))
		{
			if(ddValue < ddLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

				return FALSE;
			}
			else if(ddValue > ddHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_UInt16Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	WORD* pData = (WORD*)pDataBuffer;
	WORD* pLowData = (WORD*)pLowDataBuffer;
	WORD* pHighData = (WORD*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		ddValue = *pData;
		ddLowValue = *pLowData;
		ddHighValue = *pHighData;

		if(ddValue < ddLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

			return FALSE;
		}
		else if(ddValue > ddHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_UInt32Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	DWORD* pData = (DWORD*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		ddValue = *pData;
		if(GetRangeValues(pObjectEntry, &ddLowValue, &ddHighValue))
		{
			if(ddValue < ddLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

				return FALSE;
			}
			else if(ddValue > ddHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_UInt32Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	DWORD* pData = (DWORD*)pDataBuffer;
	DWORD* pLowData = (DWORD*)pLowDataBuffer;
	DWORD* pHighData = (DWORD*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		ddValue = *pData;
		ddLowValue = *pLowData;
		ddHighValue = *pHighData;

		if(ddValue < ddLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

			return FALSE;
		}
		else if(ddValue > ddHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_UInt64Value(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	unsigned __int64* pData = (unsigned __int64*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		ddValue = *pData;
		if(GetRangeValues(pObjectEntry, &ddLowValue, &ddHighValue))
		{
			if(ddValue < ddLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

				return FALSE;
			}
			else if(ddValue > ddHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_UInt64Value(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	unsigned __int64 ddLowValue, ddHighValue;
	unsigned __int64 ddValue;
	unsigned __int64* pData = (unsigned __int64*)pDataBuffer;
	unsigned __int64* pLowData = (unsigned __int64*)pLowDataBuffer;
	unsigned __int64* pHighData = (unsigned __int64*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		ddValue = *pData;
		ddLowValue = *pLowData;
		ddHighValue = *pHighData;

		if(ddValue < ddLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, ddValue, ddLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddLowValue, dDataSize);

			return FALSE;
		}
		else if(ddValue > ddHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, ddValue, ddHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &ddHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_FloatValue(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	float fLowValue, fHighValue;
	float fValue;
	float* pData = (float*)pDataBuffer;

	if(pData && (dDataSize == sizeof(*pData)))
	{
		fValue = *pData;
		if(GetRangeValues(pObjectEntry, &fLowValue, &fHighValue))
		{
			if(fValue < fLowValue)
			{
				//LowRange
				if(oShowMsg) HandleLowRangeError(pObjectEntry, fValue, fLowValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &fLowValue, dDataSize);

				return FALSE;
			}
			else if(fValue > fHighValue)
			{
				//HighRange
				if(oShowMsg) HandleHighRangeError(pObjectEntry, fValue, fHighValue, oMsgInHexFormat, sizeof(*pData));
				if(oLimitData) memcpy(pDataBuffer, &fHighValue, dDataSize);

				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_FloatValue(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	float fLowValue, fHighValue;
	float fValue;
	float* pData = (float*)pDataBuffer;
	float* pLowData = (float*)pLowDataBuffer;
	float* pHighData = (float*)pHighDataBuffer;

	if((pData && (dDataSize == sizeof(*pData))) && (pLowData && (dDataSize == sizeof(*pLowData))) && (pHighData && (dDataSize == sizeof(*pHighData))))
	{
		fValue = *pData;
		fLowValue = *pLowData;
		fHighValue = *pHighData;

		if(fValue < fLowValue)
		{
			//LowRange
			if(oShowMsg) HandleLowRangeError(pObjectEntry, fValue, fLowValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &fLowValue, dDataSize);

			return FALSE;
		}
		else if(fValue > fHighValue)
		{
			//HighRange
			if(oShowMsg) HandleHighRangeError(pObjectEntry, fValue, fHighValue, oMsgInHexFormat, sizeof(*pData));
			if(oLimitData) memcpy(pDataBuffer, &fHighValue, dDataSize);

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_StringValue(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	return TRUE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_StringValue(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	return TRUE;
}

BOOL CObjectEntryRangeCheck::DoStaticRangeCheck_DomainValue(CObjectEntry* pObjectEntry, void* pDataBuffer, void* pLowDataBuffer, void* pHighDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	return TRUE;
}

BOOL CObjectEntryRangeCheck::DoRangeCheck_DomainValue(CObjectEntry* pObjectEntry, void* pDataBuffer, DWORD dDataSize, BOOL oLimitData, BOOL oShowMsg, BOOL oMsgInHexFormat)
{
	return TRUE;
}

void CObjectEntryRangeCheck::HandleRangeError(CObjectEntry* pObjectEntry, CStdString strErrorText)
{
	CStdString strObjectName;
	CStdString strErrorMsg;
	CStdString str;
	WORD wIndex = 0;
	WORD wSubIndex = 0;

	if(pObjectEntry)
	{
		//Get Object Name
		 pObjectEntry->GetParameterName(strObjectName);

		//Get Object Index and SubIndex
		wIndex = pObjectEntry->GetIndex();
		wSubIndex = pObjectEntry->GetSubIndex();

		//Command name
		strErrorMsg = strErrorText + "\n";

		//Space
		strErrorMsg += "\n";

		//ObjectName
		strErrorMsg += "ObjectName: " + strObjectName + "\n";

		//Index
		str.Format("Index: 0x%.4X", wIndex);
		strErrorMsg += str + "; ";

		//SubIndex
		str.Format("SubIndex: 0x%.2X", wSubIndex);
		strErrorMsg += str;

		MmcMessageDialog(MT_ERROR, strErrorMsg);
	}
}

void CObjectEntryRangeCheck::HandleHighRangeError(CObjectEntry* pObjectEntry, unsigned __int64 ddValue, unsigned __int64 ddHighValue, BOOL oMsgInHexFormat, DWORD dValueSize)
{
	CStdString strErrorText;
	CStdString strHexValue;

	if(oMsgInHexFormat)
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is higher than %s!", GetHexValueString(ddHighValue, dValueSize));
	}
	else
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is higher than %u!", ddHighValue);
	}

	//RangeError
	HandleRangeError(pObjectEntry, strErrorText);
}

void CObjectEntryRangeCheck::HandleHighRangeError(CObjectEntry* pObjectEntry, __int64 llValue, __int64 llHighValue, BOOL oMsgInHexFormat, DWORD dValueSize)
{
	CStdString strErrorText;

	if(oMsgInHexFormat)
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is higher than %s!", GetHexValueString((unsigned __int64)llHighValue, dValueSize));
	}
	else
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is higher than %i!", llHighValue);
	}

	//RangeError
	HandleRangeError(pObjectEntry, strErrorText);
}

void CObjectEntryRangeCheck::HandleHighRangeError(CObjectEntry* pObjectEntry, float fValue, float fHighValue, BOOL oMsgInHexFormat, DWORD dValueSize)
{
	CStdString strErrorText;

	if(oMsgInHexFormat)
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is higher than %s!", GetHexValueString((unsigned __int64)fHighValue, dValueSize));
	}
	else
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is higher than %.3f!", fHighValue);
	}

	//RangeError
	HandleRangeError(pObjectEntry, strErrorText);
}

void CObjectEntryRangeCheck::HandleLowRangeError(CObjectEntry* pObjectEntry, unsigned __int64 ddValue, unsigned __int64 ddLowValue, BOOL oMsgInHexFormat, DWORD dValueSize)
{
	CStdString strErrorText;

	if(oMsgInHexFormat)
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is lower than %s!", GetHexValueString((unsigned __int64)ddLowValue, dValueSize));
	}
	else
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is lower than %u!", ddLowValue);
	}

	//RangeError
	HandleRangeError(pObjectEntry, strErrorText);
}

void CObjectEntryRangeCheck::HandleLowRangeError(CObjectEntry* pObjectEntry, __int64 llValue, __int64 llLowValue, BOOL oMsgInHexFormat, DWORD dValueSize)
{
	CStdString strErrorText;

	if(oMsgInHexFormat)
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is lower than %s!", GetHexValueString((unsigned __int64)llLowValue, dValueSize));
	}
	else
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is lower than %i!", llLowValue);
	}

	//RangeError
	HandleRangeError(pObjectEntry, strErrorText);
}

void CObjectEntryRangeCheck::HandleLowRangeError(CObjectEntry* pObjectEntry, float fValue, float fLowValue, BOOL oMsgInHexFormat, DWORD dValueSize)
{
	CStdString strErrorText;

	if(oMsgInHexFormat)
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is lower than %s!", GetHexValueString((unsigned __int64)fLowValue, dValueSize));
	}
	else
	{
		//ErrorText
		strErrorText.Format("Range Check Error!\nValue is lower than %.3f!", fLowValue);
	}

	//RangeError
	HandleRangeError(pObjectEntry, strErrorText);
}

CStdString CObjectEntryRangeCheck::GetHexValueString(unsigned __int64 ddValue, DWORD dValueSize)
{
	CStdString strValue = "";

	if(dValueSize == 1)
	{
		strValue.Format("0x%.1X", ddValue);
	}
	else if(dValueSize == 2)
	{
		strValue.Format("0x%.2X", ddValue);
	}
	else if(dValueSize == 4)
	{
		strValue.Format("0x%.4X", ddValue);
	}
	else if(dValueSize == 8)
	{
		strValue.Format("0x%.8X", ddValue);
	}

	return strValue;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, char* puLowValue, char* puHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, puLowValue)) return FALSE;
			if(!GetValue(strHighValue, puHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, short* psLowValue, short* psHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, psLowValue)) return FALSE;
			if(!GetValue(strHighValue, psHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, long* plLowValue, long* plHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, plLowValue)) return FALSE;
			if(!GetValue(strHighValue, plHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, __int64* pllLowValue, __int64* pllHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, pllLowValue)) return FALSE;
			if(!GetValue(strHighValue, pllHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, BYTE* pbLowValue, BYTE* pbHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, pbLowValue)) return FALSE;
			if(!GetValue(strHighValue, pbHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, WORD* pusLowValue, WORD* pusHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, pusLowValue)) return FALSE;
			if(!GetValue(strHighValue, pusHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, DWORD* pulLowValue, DWORD* pulHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, pulLowValue)) return FALSE;
			if(!GetValue(strHighValue, pulHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, unsigned __int64* pddLowValue, unsigned __int64* pddHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, pddLowValue)) return FALSE;
			if(!GetValue(strHighValue, pddHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetRangeValues(CObjectEntry* pObjectEntry, float* pfLowValue, float* pfHighValue)
{
	CStdString strLowValue, strHighValue;

	if(pObjectEntry)
	{
		pObjectEntry->GetLowLimit(strLowValue);
		pObjectEntry->GetHighLimit(strHighValue);

		if(!strHighValue.IsEmpty() && strLowValue.IsEmpty())
		{
			if(!GetValue(strLowValue, pfLowValue)) return FALSE;
			if(!GetValue(strHighValue, pfHighValue)) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, char* puValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexCharStr2Char(strValue, puValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecCharStr2Char(strValue, puValue, FALSE);
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, short* psValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexShortStr2Short(strValue, psValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecShortStr2Short(strValue, psValue, FALSE);
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, long* plValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexLongStr2Long(strValue, plValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecLongStr2Long(strValue, plValue, FALSE);
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, __int64* pllValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexInt64Str2Int64(strValue, pllValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecInt64Str2Int64(strValue, pllValue, FALSE);
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, BYTE* pbValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexByteStr2Byte(strValue, pbValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecByteStr2Byte(strValue, pbValue, FALSE);
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, WORD* pusValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexWordStr2Word(strValue, pusValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecWordStr2Word(strValue, pusValue, FALSE);
		}
	}

	return FALSE;

}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, DWORD* pulValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexDWordStr2DWord(strValue, pulValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecDWordStr2DWord(strValue, pulValue, FALSE);
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, unsigned __int64* pddValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexUInt64Str2UInt64(strValue, pddValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecUInt64Str2UInt64(strValue, pddValue, FALSE);
		}
	}

	return FALSE;
}

BOOL CObjectEntryRangeCheck::GetValue(CStdString strValue, float* pfValue)
{
	const CStdString HEX_FORMAT = "0x";

	CMmcDataConversion dataConversion;

	if(!strValue.IsEmpty())
	{
		if((strValue.GetLength() > HEX_FORMAT.GetLength()) && (strValue.Left(HEX_FORMAT.GetLength()) == HEX_FORMAT))
		{
			//Hex Format
			return dataConversion.HexFloatStr2Float(strValue, pfValue, FALSE);
		}
		else
		{
			//Dec Format
			return dataConversion.DecFloatStr2Float(strValue, pfValue, FALSE);
		}
	}

	return FALSE;
}

