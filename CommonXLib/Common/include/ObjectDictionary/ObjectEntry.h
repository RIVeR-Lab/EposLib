// ObjectEntry.h: Schnittstelle f�r die Klasse CObjectEntry.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ObjectEntry_H__0253E5E8_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
#define AFX_ObjectEntry_H__0253E5E8_6B67_11D7_A23D_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <StdString.h>
#include <MmcTypeDefinition.h>
#include <Ui/MmcDragDrop.h>
#include <ObjectDictionary/ObjectEntryBase.h>
#include <Classes/TypeDefinitions.h>

class XML_PARSER;

#include <StdString.h>

class CObjectEntry : public CObjectEntryBase, public CMmcDragDrop
{
public:
    //Initialisation
    void Init() {m_strClassType="CObjectEntry";}

    CObjectEntry();
    virtual ~CObjectEntry();

    //Get Information
    BYTE GetSubIndex();
    BOOL GetSubIndex(CStdString& strSubIndex);
    EObjectDataType GetDataType();
    WORD GetDataTypeNumber();
    BOOL GetDataTypeNumber(CStdString& p_rDataType);
    DWORD GetSize();
    WORD GetWordSize();
    BOOL GetDataType(CStdString& p_rDataType, BOOL p_oLongString);
    __int64 GetDataTypeMaxValue();
    __int64 GetDataTypeMinValue();
    BOOL GetLowLimit(CStdString& strLowLimit);
    BOOL GetHighLimit(CStdString& strHighLimit);
    EAccessType GetAccessType();
    BOOL GetAccessType(CStdString& strAccessType, bool p_oLowerCase = false);
    BOOL GetAccessTypeAttribute(CStdString& strAccessType);
    BOOL GetDefaultValue(CStdString& strDefaultValue);
    BOOL GetPDOMapping(CStdString& strPdoMapping);
    BOOL GetPDOMappingToken(CStdString& strPdoMapping);
    DWORD GetObjFlags();
    BOOL GetObjFlags(CStdString& strObjFlags);
    CStdString GetDataTypeLongStr();
    CStdString GetParameterNameStr();

    //Get Value
    BOOL GetValue(BYTE* pValue, DWORD dValueSize);
    BOOL GetValue(CStdString& strValue, EObjectValueStringFormat eFormat = OVF_UNKNOWN);
    BOOL IsValueAvailable();
    DWORD GetValueSize();

    //Set Information
    BOOL SetSubIndex(BYTE subIndex);
    BOOL SetSubIndex(CStdString strSubIndex);
    BOOL SetDataType(WORD dataType);
    BOOL SetDataTypeLongStr(CStdString strDataType);
    BOOL SetDataType(CStdString strDataType);
    BOOL SetLowLimit(CStdString lowLimit);
    BOOL SetHighLimit(CStdString highLimit);
    BOOL SetAccessType(CStdString accessType);
    BOOL SetDefaultValue(CStdString defaultValue);
    BOOL SetPDOMapping(BYTE pdoMapping);
    BOOL SetPDOMapping(CStdString strPDOMapping);
    BOOL SetPDOMappingToken(CStdString strPDOMapping);
    BOOL SetObjFlags(DWORD objFlags);
    BOOL SetObjFlags(CStdString objFlags);

    //Unit
    BOOL SetUnit(float p_fMultiplier, CStdString p_Unit);
    BOOL GetUnit(float &p_rfMultiplier, CStdString &p_rUnit);

    //Set Value
    BOOL SetValue(BYTE* pValue, DWORD dValueSize);
    BOOL SetValue(CStdString strValue);
    BOOL SetValueStringFormat(CStdString strFormattedValue);

    //Information
    CObjectEntry* SearchObject(WORD index, WORD subIndex);
    int GetNumberOfSubObjects();
    virtual BOOL IsVisible();

    //Common
    virtual BOOL Reset();
    virtual BOOL operator==(CObjectEntry& other);
    virtual CObjectEntry& operator=(CObjectEntry& other);
    virtual CObjectEntryBase* Clone();
    CObjectEntry* CloneObjectEntry();
    BOOL IsEqual(void* pValue, DWORD dValueSize);
    BOOL IsEqual(CObjectEntry& other);
    virtual BOOL ResetValue();

    //PDO Mapping
    BOOL IsPdoMappable();
    BOOL IsRxPdoMappable();
    BOOL IsTxPdoMappable();

    //Access Information
    BOOL IsWritable();
    BOOL IsReadable();
    BOOL IsWriteOnDownloadAllowed();
    BOOL IsReadOnUploadAllowed();

    //Xml
    virtual void Serialize(CArchive &ar);

    BOOL StoreObjectDictionarySchema(XML_PARSER* pXmlParser);
    void Parse_XML_Document(XML_PARSER* pXmlParser);

    //Drag & drop
//    BOOL BeginDrag();

private:
    //Common
    BOOL AddClonedObjectEntry(tObjectEntryList* pObjectEntryList, CObjectEntry* pObjectEntry);
    BOOL InitValue(DWORD dSize);
    CStdString GetDataTypeStr();

private:
    BYTE m_uSubIndex;
    EObjectDataType m_DataType;
    CStdString m_strLowLimit;
    CStdString m_strHighLimit;
    EAccessType m_AccessType;
    CStdString m_strDefault;
    BOOL m_oPDOMapping;
    CStdString m_strObjFlags;
    CStdString m_Unit;
    float m_fMultiplier;

    BYTE* m_pValue;
    DWORD m_dValueSize;
    EObjectValueStringFormat m_ValueStringFormat;
};

#endif // !defined(AFX_ObjectEntry_H__0253E5E8_6B67_11D7_A23D_00A02436C4EF__INCLUDED_)
