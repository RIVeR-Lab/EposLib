// ObjectEntryBase.cpp: Implementierung der Klasse CObjectEntryBase.
//
//////////////////////////////////////////////////////////////////////
#include <Classes/MmcDataConversion.h>
#include <ObjectDictionary/ObjectFilter.h>
#include <ObjectDictionary/ObjectEntry.h>
#include <ObjectDictionary/ObjectEntryBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CObjectEntryBase::CObjectEntryBase():
    m_pObjectFilter(0)
    , m_wIndex(0)
    , m_uObjectType(0)
    , m_strParameterName(_T(""))
    , m_UserLevels(_T(""))
{
    m_strClassType = _T("CObjectEntryBase");
}

CObjectEntryBase::~CObjectEntryBase()
{
}

CObjectEntryBase& CObjectEntryBase::operator=(CObjectEntryBase& other)
{
    if(this != &other)
    {
        m_pObjectFilter = other.m_pObjectFilter;
        m_wIndex = other.m_wIndex;
        m_strParameterName = other.m_strParameterName;
        m_uObjectType = other.m_uObjectType;
        m_UserLevels = other.m_UserLevels;
    }

    return *this;
}

bool CObjectEntryBase::IsKindOf(CStdString strClassType)
{
    return ( m_strClassType.CompareNoCase(strClassType) == 0 );
}

CStdString CObjectEntryBase::GetClassType()
{
    return m_strClassType;
}

BOOL CObjectEntryBase::SetParameterName(CStdString name)
{
    BOOL oResult = TRUE;

    m_strParameterName = name;
    return oResult;
}

BOOL CObjectEntryBase::SetObjectType(BYTE objectType)
{
    BOOL oResult = TRUE;

    m_uObjectType = objectType;

    return oResult;
}

CObjectEntryBase* CObjectEntryBase::SearchObject(WORD index,WORD subIndex)
{
    return NULL;
}

BOOL CObjectEntryBase::Reset()
{
    BOOL oResult = TRUE;

    m_wIndex = 0;
    m_uObjectType = 0;
    m_strParameterName = _T("");
    m_UserLevels = _T("");

    return oResult;
}

BOOL CObjectEntryBase::ResetValue()
{
    return FALSE;
}

BOOL CObjectEntryBase::SetIndex(WORD index)
{
    BOOL oResult = TRUE;

    m_wIndex = index;

    return oResult;
}

BOOL CObjectEntryBase::SetIndex(CStdString strIndex)
{
    CMmcDataConversion dataConversion;
    BOOL oResult = TRUE;

    if(!dataConversion.HexWordStr2Word(strIndex,&m_wIndex,FALSE))
    {
        m_wIndex = 0;
        oResult = FALSE;
    }

    return oResult;
}

void CObjectEntryBase::Serialize(CArchive &ar)
{
}

CObjectEntryBase* CObjectEntryBase::Clone()
{
    return NULL;
}

WORD CObjectEntryBase::GetIndex()
{
    return m_wIndex;
}

BOOL CObjectEntryBase::InitObjectFilter(CObjectFilter *pObjectFilter)
{
    BOOL oResult = TRUE;

    m_pObjectFilter = pObjectFilter;

    return oResult;
}

BOOL CObjectEntryBase::GetParameterName(CStdString& name)
{
    BOOL oResult = TRUE;

    name = m_strParameterName;

    return oResult;
}

int CObjectEntryBase::GetNumberOfSubObjects()
{
    return 0;
}

BOOL CObjectEntryBase::IsVisible()
{
    BOOL oResult(FALSE);

    oResult = m_pObjectFilter->IsVisibleStructuredObject(m_strParameterName, m_wIndex);
    return oResult;
}

BOOL CObjectEntryBase::StoreObjectDictionarySchema(XML_PARSER* pXmlParser)
{
    return FALSE;
}

BOOL CObjectEntryBase::GetIndex(CStdString& strIndex)
{
    BOOL oResult(TRUE);

    strIndex.Format(_T("0x%.4X"),m_wIndex);
    return oResult;
}

BYTE CObjectEntryBase::GetObjectType()
{
    return m_uObjectType;
}

BOOL CObjectEntryBase::GetObjectType(CStdString& strObjectType)
{
    BOOL oResult(TRUE);

    strObjectType.Format(_T("%i"),m_uObjectType);
    return oResult;
}

BOOL CObjectEntryBase::SetObjectType(CStdString strObjectType)
{
    CMmcDataConversion dataConversion;
    BOOL oResult = TRUE;

    if(!dataConversion.DecByteStr2Byte(strObjectType,&m_uObjectType,FALSE))
    {
        m_uObjectType = 0;
        oResult = FALSE;
    }

    return oResult;
}

BOOL CObjectEntryBase::SetUserLevels(CStdString p_UserLevels)
{
    BOOL oResult(TRUE);

    m_UserLevels = p_UserLevels;

    return oResult;
}

BOOL CObjectEntryBase::GetUserLevels(CStdString& p_rUserLevels)
{
    BOOL oResult(TRUE);

    p_rUserLevels = m_UserLevels;

    return oResult;
}

BOOL CObjectEntryBase::IsUserLevelAvailable(CStdString p_UserLevel)
{
    BOOL oLevelAvailable(FALSE);

    if(m_UserLevels.find(p_UserLevel, 0) != -1)
    {
        oLevelAvailable = TRUE;
    }

    return oLevelAvailable;
}


