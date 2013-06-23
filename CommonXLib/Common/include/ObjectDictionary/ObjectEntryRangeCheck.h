// ObjectEntryRangeCheck.h: Schnittstelle f�r die Klasse CObjectEntryRangeCheck.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTENTRYRANGECHECK_H__2352CF9A_1794_4AD7_BDCA_E7C66C5D0D74__INCLUDED_)
#define AFX_OBJECTENTRYRANGECHECK_H__2352CF9A_1794_4AD7_BDCA_E7C66C5D0D74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <StdString.h>
#include "ObjectEntry.h"

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////
const CStdString MSG_CONTAINER_NOT_FIT     = _T("Provided data container is smaller then data container of object to be retrieved!");
const CStdString MSG_DATA_NOT_FIT     = _T("Provided data does not fit into device object!");

class CObjectEntryRangeCheck  
{
public:
	CObjectEntryRangeCheck();
	virtual ~CObjectEntryRangeCheck();

	BOOL IsRangeCheckAvailable(CObjectEntry* pObjectEntry);
	BOOL DoRangeCheck(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,void* pLowDataBuffer,DWORD dLowDataSize,void* pHighDataBuffer,DWORD dHighDataSize);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, char p_cValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, short p_sValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, long p_lValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, long long p_llValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned char p_ucValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned short p_usValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned long p_ulValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned int p_ulValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    BOOL CreateSafeGetVoidDataContainer(CObjectEntry* p_pObjectEntry, unsigned long long p_ullValue, void*& p_pVoidContainer, DWORD& p_rSizeOfVoidContainer, BOOL p_oShowMsg = TRUE);
    template <class T>
    BOOL CreateSafeSetVoidDataContainer(CObjectEntry* p_pObjectEntry, T p_value, void*& p_pReturnValue, DWORD& p_rSizeOfReturnValue, BOOL p_oShowMsg = TRUE)
    {
        BOOL oRetVal(FALSE);

	    if(p_pObjectEntry && !p_pReturnValue)
	    {
		    switch(p_pObjectEntry->GetDataType())
		    {
			    case ODT_BOOLEAN:
                    {
                        if((p_value >= FALSE) && (p_value <= TRUE))
                        {
                            p_pReturnValue = new BOOL;
                            *(BOOL*)p_pReturnValue = (BOOL)p_value;
                            p_rSizeOfReturnValue = sizeof(BOOL);
                            oRetVal = TRUE;
                        }
                    }
                    break;
                case ODT_INT8:
                    {
                        if((p_value >= _I8_MIN) && (p_value <= _I8_MAX))
                        {
                            p_pReturnValue = new char;
                            *(char*)p_pReturnValue = (char)p_value;
                            p_rSizeOfReturnValue = sizeof(char);
                            oRetVal = TRUE;
                        }
                    }
                    break;
                case ODT_INT16:
                    {
                        if((p_value >= SHRT_MIN) && (p_value <= SHRT_MAX))
                        {
                            p_pReturnValue = new short;
                            *(short*)p_pReturnValue = (short)p_value;
                            p_rSizeOfReturnValue = sizeof(short);
                            oRetVal = TRUE;
                        }
                    }
                    break;
                case ODT_INT32:
                    {
                        if((p_value >= LONG_MIN) && (p_value <= LONG_MAX))
                        {
                            p_pReturnValue = new long;
                            *(long*)p_pReturnValue = (long)p_value;
                            p_rSizeOfReturnValue = sizeof(long);
                            oRetVal = TRUE;
                        }
                    }
                    break;
                case ODT_INT64:
                    {
                        if((p_value >= _I64_MIN) && (p_value <= _I64_MAX))
                        {
                            p_pReturnValue = new long long;
                            *(long long*)p_pReturnValue = (long long)p_value;
                            p_rSizeOfReturnValue = sizeof(long long);
                            oRetVal = TRUE;
                        }
                    }
                    break;
			    case ODT_UINT8:
                    {
                        if((p_value >= 0) && (p_value <= UCHAR_MAX))
                        {
                            p_pReturnValue = new unsigned char;
                            *(unsigned char*)p_pReturnValue = (unsigned char)p_value;
                            p_rSizeOfReturnValue = sizeof(unsigned char);
                            oRetVal = TRUE;
                        }
                    }
                    break;
			    case ODT_UINT16:
                    {
                        if((p_value >= 0) && (p_value <= USHRT_MAX))
                        {
                            p_pReturnValue = new unsigned short;
                            *(unsigned short*)p_pReturnValue = (unsigned short)p_value;
                            p_rSizeOfReturnValue = sizeof(unsigned short);
                            oRetVal = TRUE;
                        }
                    }
                    break;
			    case ODT_UINT32:
                    {
                        if((p_value >= 0) && (p_value <= ULONG_MAX))
                        {
                            p_pReturnValue = new unsigned long;
                            *(unsigned long*)p_pReturnValue = (unsigned long)p_value;
                            p_rSizeOfReturnValue = sizeof(unsigned long);
                            oRetVal = TRUE;
                        }
                    }
                    break;
			    case ODT_UINT64:
                    {
                        if((p_value >= 0) && (p_value <= _UI64_MAX))
                        {
                            p_pReturnValue = new unsigned long long;
                            *(unsigned long long*)p_pReturnValue = (unsigned long long)p_value;
                            p_rSizeOfReturnValue = sizeof(unsigned long long);
                            oRetVal = TRUE;
                        }
                    }
                    break;
			    default: break;
		    }
	    }
        if(!oRetVal && p_oShowMsg)
        {
            HandleRangeError(p_pObjectEntry, MSG_DATA_NOT_FIT);
        }
	    return oRetVal;
    };
    template <class T>
    void GetSafeVoidDataContainerData(CObjectEntry* p_pObjectEntry, void*& p_pVoidData, T* p_pValue)
    {
        if(p_pObjectEntry && p_pVoidData && p_pValue)
        {
	        switch(p_pObjectEntry->GetDataType())
	        {
		        case ODT_BOOLEAN:
                    {
                        *p_pValue = (T)(*(BOOL*)p_pVoidData);
                    }
                    break;
                case ODT_INT8:
                    {
                        *p_pValue = (T)(*(char*)p_pVoidData);
                    }
                    break;
                case ODT_INT16:
                    {
                        *p_pValue = (T)(*(short*)p_pVoidData);
                    }
                    break;
                case ODT_INT32:
                    {
                        *p_pValue = (T)(*(long*)p_pVoidData);
                    }
                    break;
                case ODT_INT64:
                    {
                        *p_pValue = (T)(*(long long*)p_pVoidData);
                    }
                    break;
		        case ODT_UINT8:
                    {
                        *p_pValue = (T)(*(unsigned char*)p_pVoidData);
                    }
                    break;
		        case ODT_UINT16:
                    {
                        *p_pValue = (T)(*(unsigned short*)p_pVoidData);
                    }
                    break;
		        case ODT_UINT32:
                    {
                        *p_pValue = (T)(*(unsigned long*)p_pVoidData);
                    }
                    break;
		        case ODT_UINT64:
                    {
                        *p_pValue = (T)(*(unsigned long long*)p_pVoidData);
                    }
                    break;
		        default: break;
	        }
        }
    };

private:
	BOOL DoRangeCheck_BooleanValue(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_Int8Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_Int16Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_Int32Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_Int64Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_UInt8Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_UInt16Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_UInt32Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_UInt64Value(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_FloatValue(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_StringValue(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoRangeCheck_DomainValue(CObjectEntry* pObjectEntry,void* pDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);

	BOOL DoStaticRangeCheck_BooleanValue(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_Int8Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_Int16Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_Int32Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_Int64Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_UInt8Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_UInt16Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_UInt32Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_UInt64Value(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_FloatValue(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_StringValue(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);
	BOOL DoStaticRangeCheck_DomainValue(CObjectEntry* pObjectEntry,void* pDataBuffer,void* pLowDataBuffer,void* pHighDataBuffer,DWORD dDataSize,BOOL oLimitData,BOOL oShowMsg,BOOL oMsgInHexFormat);

	void HandleHighRangeError(CObjectEntry* pObjectEntry,unsigned __int64 ddValue,unsigned __int64 ddHighValue,BOOL oMsgInHexFormat,DWORD dValueSize);
	void HandleHighRangeError(CObjectEntry* pObjectEntry,__int64 llValue,__int64 llHighValue,BOOL oMsgInHexFormat,DWORD dValueSize);
	void HandleHighRangeError(CObjectEntry* pObjectEntry,float fValue,float fHighValue,BOOL oMsgInHexFormat,DWORD dValueSize);
	void HandleLowRangeError(CObjectEntry* pObjectEntry,unsigned __int64 ddValue,unsigned __int64 ddLowValue,BOOL oMsgInHexFormat,DWORD dValueSize);
	void HandleLowRangeError(CObjectEntry* pObjectEntry,__int64 lliValue,__int64 llLowValue,BOOL oMsgInHexFormat,DWORD dValueSize);
	void HandleLowRangeError(CObjectEntry* pObjectEntry,float fValue,float fLowValue,BOOL oMsgInHexFormat,DWORD dValueSize);

	BOOL GetRangeValues(CObjectEntry* pObjectEntry,char* puLowValue,char* puHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,short* psLowValue,short* psHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,long* plLowValue,long* plHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,__int64* pllLowValue,__int64* pllHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,BYTE* pbLowValue,BYTE* pbHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,WORD* pusLowValue,WORD* pusHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,DWORD* pulLowValue,DWORD* pulHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,unsigned __int64* pddLowValue,unsigned __int64* pddHighValue);
	BOOL GetRangeValues(CObjectEntry* pObjectEntry,float* pfLowValue,float* pfHighValue);

	BOOL GetValue(CStdString strValue, char* puValue);
	BOOL GetValue(CStdString strValue, short* psValue);
	BOOL GetValue(CStdString strValue, long* plValue);
	BOOL GetValue(CStdString strValue, __int64* pllValue);
	BOOL GetValue(CStdString strValue, BYTE* pbValue);
	BOOL GetValue(CStdString strValue, WORD* pusValue);
	BOOL GetValue(CStdString strValue, DWORD* pulValue);
	BOOL GetValue(CStdString strValue, unsigned __int64* pddValue);
	BOOL GetValue(CStdString strValue, float* pfValue);

	void HandleRangeError(CObjectEntry* pObjectEntry,CStdString strErrorText);
	CStdString GetHexValueString(unsigned __int64 ddValue,DWORD dValueSize);	
};

#endif // !defined(AFX_OBJECTENTRYRANGECHECK_H__2352CF9A_1794_4AD7_BDCA_E7C66C5D0D74__INCLUDED_)
