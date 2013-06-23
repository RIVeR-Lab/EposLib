// ObjectFilter.cpp: Implementierung der Klasse CObjectFilter.
//
//////////////////////////////////////////////////////////////////////
#include <Classes/MmcDataConversion.h>
#include <PersistenceReaderWriter/XmlWriter.h>
#include <PersistenceReaderWriter/XmlReader.h>
#include <ObjectDictionary/ObjectFilter.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CObjectFilter::CObjectFilter()
{
    m_oInternalObjectsVisible = FALSE;
    m_oServiceModeActive = FALSE;

    Reset();
}

CObjectFilter::CObjectFilter(CObjectFilter& objectFilter)
{
    *this = objectFilter;
}

CObjectFilter::~CObjectFilter()
{
}

BOOL CObjectFilter::Reset()
{
    m_oFilterIsEmpty = TRUE;

    return DeleteAllVisibleObjects();
}

CObjectFilter& CObjectFilter::operator=(CObjectFilter &p_rObjectFilter)
{
    if(this != &p_rObjectFilter)
    {
        Reset();

        m_oFilterIsEmpty = p_rObjectFilter.m_oFilterIsEmpty;
        m_Name = p_rObjectFilter.m_Name;
        m_oServiceModeActive = p_rObjectFilter.m_oServiceModeActive;
        m_Description = p_rObjectFilter.m_Description;

        m_VisibleStructuredObject = p_rObjectFilter.m_VisibleStructuredObject;
        m_VisibleObject = p_rObjectFilter.m_VisibleObject;
    }

    return *this;
}

void CObjectFilter::SetName(CStdString p_Name)
{
    m_oFilterIsEmpty = FALSE;
    m_Name = p_Name;
}

CStdString CObjectFilter::GetName()
{
    return m_Name;
}

void CObjectFilter::SetDescription(CStdString p_Description)
{
    m_Description = p_Description;
}

CStdString CObjectFilter::GetDescription()
{
    return m_Description;
}

BOOL CObjectFilter::SetInternalObjectFilter(BOOL p_oActive)
{
    BOOL oResult(TRUE);

    m_oInternalObjectsVisible = !p_oActive;

    return oResult;
}

void CObjectFilter::SetServiceModeActive(BOOL p_oActive)
{
    m_oServiceModeActive = p_oActive;
}

BOOL CObjectFilter::AddVisibleStructuredObject(CStdString p_Index)
{
    BOOL oResult(FALSE);
    CMmcDataConversion dataConversion;
    WORD usIndex;

    if(dataConversion.HexWordStr2Word(p_Index, &usIndex, FALSE))
    {
        oResult = AddVisibleStructuredObject(usIndex);
    }

    return oResult;
}

BOOL CObjectFilter::AddVisibleStructuredObject(WORD p_usIndex)
{
    BOOL oResult(TRUE);

    m_oFilterIsEmpty = FALSE;
    m_VisibleStructuredObject.push_back(p_usIndex);

    return oResult;
}

BOOL CObjectFilter::AddVisibleObject(CStdString p_Index, CStdString p_SubIndex)
{
    BOOL oResult(FALSE);
    BOOL oShowMsg(FALSE);
    BYTE ubSubIndex;
    CMmcDataConversion dataConversion;
    WORD usIndex;

    if(dataConversion.HexWordStr2Word(p_Index, &usIndex, oShowMsg))
    {
        if(dataConversion.HexByteStr2Byte(p_SubIndex, &ubSubIndex, oShowMsg))
        {
            oResult = AddVisibleObject(usIndex, ubSubIndex);
        }
    }

    return oResult;
}

BOOL CObjectFilter::AddVisibleObject(WORD p_usIndex, BYTE p_ubSubIndex)
{
    BOOL oResult(TRUE);

    m_oFilterIsEmpty = FALSE;
    m_VisibleObject[p_usIndex].push_back(p_ubSubIndex);

    return oResult;
}

BOOL CObjectFilter::IsEmpty()
{
    return m_oFilterIsEmpty;
}

BOOL CObjectFilter::IsObjectNameVisible(CStdString p_ObjectName)
{
    BOOL oResult(TRUE);
    CStdString invisibleSubString = _T("Internal");

    if(!m_oServiceModeActive && !m_oInternalObjectsVisible)
    {
        if(p_ObjectName.GetLength() >= invisibleSubString.GetLength())
        {
            if(invisibleSubString.CompareNoCase(p_ObjectName.Left(invisibleSubString.GetLength())) == 0)
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CObjectFilter::IsVisibleObject(CStdString p_ObjectName, WORD p_usIndex, BYTE p_ubSubIndex)
{
    BOOL oResult(FALSE);

    if(IsObjectNameVisible(p_ObjectName))
    {
        if(!m_oFilterIsEmpty)
        {
            for(map<WORD, vector<BYTE> >::iterator it = m_VisibleObject.begin(); it != m_VisibleObject.end(); it++)
            {
                vector<BYTE> subIndex = (*it).second;

                for(vector<BYTE>::iterator ite = subIndex.begin(); ite < subIndex.end(); ite++)
                {
                    if(p_usIndex == (*it).first && p_ubSubIndex == (*ite))
                    {
                        oResult = TRUE;
                        break;
                    }
                }
            }
        }
        else
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CObjectFilter::IsVisibleStructuredObject(CStdString p_ObjectName, WORD p_usIndex)
{
    BOOL oResult(FALSE);

    if(IsObjectNameVisible(p_ObjectName))
    {
        if(!m_oFilterIsEmpty)
        {
            for(size_t i = 0; i < m_VisibleStructuredObject.size(); i++)
            {
                if(m_VisibleStructuredObject[i] == p_usIndex)
                {
                    oResult = TRUE;
                    break;
                }
            }
        }
        else
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CObjectFilter::IsVisibleObjectExisting(WORD p_usIndex, BYTE p_ubSubIndex)
{
    BOOL oResult(FALSE);
    map<WORD, vector<BYTE> >::iterator it;

    if(!m_VisibleObject.empty())
    {
        it = m_VisibleObject.find(p_usIndex);

        if(it != m_VisibleObject.end())
        {
            vector<BYTE> subIndex = (*it).second;

            for(vector<BYTE>::iterator ite = subIndex.begin(); ite < subIndex.end(); ite++)
            {
                if((*ite) == p_ubSubIndex)
                {
                    oResult = TRUE;
                    break;
                }
            }
        }
    }

    return oResult;
}

BOOL CObjectFilter::IsVisibleStructuredObjectExisting(WORD p_usIndex)
{
    BOOL oResult(FALSE);
    map<WORD, vector<BYTE> >::iterator it;

    if(!m_VisibleObject.empty())
    {
        it = m_VisibleObject.find(p_usIndex);

        if(it != m_VisibleObject.end())
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CObjectFilter::DeleteVisibleObject(WORD p_usIndex, BYTE p_ubSubIndex)
{
    BOOL oResult(FALSE);
    map<WORD, vector<BYTE> >::iterator it;

    if(!m_VisibleObject.empty())
    {
        it = m_VisibleObject.find(p_usIndex);

        if(it != m_VisibleObject.end())
        {
            vector<BYTE> subIndex = (*it).second;

            for(vector<BYTE>::iterator ite = subIndex.begin(); ite < subIndex.end(); ite++)
            {
                if((*ite) == p_ubSubIndex)
                {
                    subIndex.erase(ite);
                    (*it).second.clear();
                    (*it).second.insert((*it).second.begin(), subIndex.begin(), subIndex.end());

                    oResult = TRUE;
                    break;
                }
            }

            if(subIndex.empty())
            {
                m_VisibleObject.erase(it);
            }
        }
    }

    return oResult;
}

BOOL CObjectFilter::DeleteVisibleStructuredObject(WORD p_usIndex)
{
    BOOL oResult(FALSE);

    for(std::vector<WORD>::iterator it = m_VisibleStructuredObject.begin(); it != m_VisibleStructuredObject.end(); it++)
    {
        if((*it) == p_usIndex)
        {
            m_VisibleStructuredObject.erase(it);

            oResult = TRUE;
            break;
        }
    }

    return oResult;
}

BOOL CObjectFilter::DeleteVisibleStructuredObjects(WORD p_usIndex)
{
    BOOL oResult(TRUE);
    map<WORD, vector<BYTE> >::iterator it;

    DeleteVisibleStructuredObject(p_usIndex);

    if(!m_VisibleObject.empty())
    {
        it = m_VisibleObject.find(p_usIndex);

        if(it != m_VisibleObject.end())
        {
            m_VisibleObject.erase(it);
        }
    }

    return oResult;
}

BOOL CObjectFilter::DeleteAllVisibleObjects()
{
    m_VisibleStructuredObject.clear();
    m_VisibleObject.clear();

    return TRUE;
}

void CObjectFilter::Serialize(CArchive &p_rArchive)
{

}

BOOL CObjectFilter::LoadObjectFilterSchema(CStdString* p_pXmlData)
{
    BOOL oResult(FALSE);
    CXmlReader xmlReader;

    if(p_pXmlData)
    {
        Reset();

        if(xmlReader.LoadFromCStdString(*p_pXmlData))
        {
            oResult = Parse_XML_Document(&xmlReader);
        }
    }

    return oResult;
}

BOOL CObjectFilter::Parse_XML_Document(CXmlReader* p_pXmlReader)
{
    BOOL oResult(FALSE);
    CStdString strIndex(_T(""));
    CStdString strSubIndex(_T(""));
    int lIndex(0);

    if(p_pXmlReader)
    {
        //Document
        if(p_pXmlReader->ReadStartDocument())
        {
            p_pXmlReader->ReadAttributeString(_T("Name"), m_Name);
            p_pXmlReader->ReadAttributeString(_T("Description"), m_Description);
            m_oFilterIsEmpty = FALSE;

            //Visible structured objects
            lIndex = 0;
            while(p_pXmlReader->ReadStartElement(_T("VisibleStructuredObject"), lIndex++))
            {
                p_pXmlReader->ReadAttributeString(_T("Index"), strIndex);
                p_pXmlReader->ReadEndElement();

                AddVisibleStructuredObject(strIndex);
            }

            //Visible objects
            lIndex = 0;
            while(p_pXmlReader->ReadStartElement(_T("VisibleObject"), lIndex++))
            {
                p_pXmlReader->ReadAttributeString(_T("Index"), strIndex);
                p_pXmlReader->ReadAttributeString(_T("SubIndex"), strSubIndex);
                p_pXmlReader->ReadEndElement();

                AddVisibleObject(strIndex, strSubIndex);
            }

            //Document end
            p_pXmlReader->ReadEndDocument();

            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CObjectFilter::StoreObjectFilterSchema(CStdString* p_pXmlData)
{
    BOOL oResult(FALSE);
    CMmcDataConversion dataConversion;
    CStdString strIndex(_T(""));
    CStdString strSubIndex(_T(""));
    CXmlWriter xmlWriter;

    if(p_pXmlData)
    {
        //Document
        if(xmlWriter.WriteStartDocument())
        {
            //Object Filter
            if(xmlWriter.WriteStartElement(_T("ObjectFilter")))
            {
                //Object Filter attributes
                xmlWriter.WriteAttributeString(_T("Name"), m_Name);
                xmlWriter.WriteAttributeString(_T("Description"), m_Description);

                //Visible structured objects
                for(size_t i = 0; i < m_VisibleStructuredObject.size(); i++)
                {
                    //Visible structured object
                    if(xmlWriter.WriteStartElement(_T("VisibleStructuredObject")))
                    {
                        //Visible object attributes
                        dataConversion.Word2HexWordStr(m_VisibleStructuredObject[i], &strIndex);
                        xmlWriter.WriteAttributeString(_T("Index"), strIndex);
                        xmlWriter.EndElement();
                    }
                }

                //Visible objects
                for(map<WORD, vector<BYTE> >::iterator it = m_VisibleObject.begin(); it != m_VisibleObject.end(); it++)
                {
                    vector<BYTE> subIndex = (*it).second;

                    for(vector<BYTE>::iterator ite = subIndex.begin(); ite < subIndex.end(); ite++)
                    {
                        //Visible object
                        if(xmlWriter.WriteStartElement(_T("VisibleObject")))
                        {
                            //Visible object attributes
                            dataConversion.Word2HexWordStr((*it).first, &strIndex);
                            dataConversion.UChar2HexUCharStr(*ite, &strSubIndex);

                            xmlWriter.WriteAttributeString(_T("Index"), strIndex);
                            xmlWriter.WriteAttributeString(_T("SubIndex"), strSubIndex);
                            xmlWriter.EndElement();
                        }
                    }
                }

                //Object Filter end
                xmlWriter.WriteFullEndElement();
            }

            //Document end
            xmlWriter.WriteEndDocument();

            oResult = xmlWriter.Flush(*p_pXmlData);
        }
    }

    return oResult;
}

BOOL CObjectFilter::ReadFrom(CXmlReader* p_pXmlReader, int p_lCount)
{
    BOOL oResult(FALSE);

    if(p_pXmlReader)
    {
        Reset();

        //Start Element
        if(p_pXmlReader->ReadStartElement(_T("ObjectFilter"), p_lCount))
        {
            oResult = p_pXmlReader->ReadElement(_T("Name"), 0, m_Name);
            if(oResult && !p_pXmlReader->ReadElement(_T("Description"), 0, m_Description)) oResult = FALSE;

            if(oResult && !ReadStructuredObjectFilterSchemaFrom(p_pXmlReader)) oResult = FALSE;
            if(oResult && !ReadVisibleObjectFilterSchemaFrom(p_pXmlReader)) oResult = FALSE;

            //End Element
            p_pXmlReader->ReadEndElement();
        }
    }

    return oResult;
}

BOOL CObjectFilter::WriteTo(CXmlWriter* p_pXmlWriter)
{
    BOOL oResult(FALSE);

    if(p_pXmlWriter)
    {
        oResult = TRUE;

        //Start Element "ObjectFilter"
        if(p_pXmlWriter->WriteStartElement(_T("ObjectFilter")))
        {
            if(oResult && !p_pXmlWriter->WriteElement(_T("Name"), m_Name)) oResult = FALSE;
            if(oResult && !p_pXmlWriter->WriteElement(_T("Description"), m_Description)) oResult = FALSE;

            if(oResult && !WriteStructuredObjectFilterSchemaTo(p_pXmlWriter)) oResult = FALSE;
            if(oResult && !WriteVisibleObjectFilterSchemaTo(p_pXmlWriter)) oResult = FALSE;

            //End Element "ObjectFilter"
            p_pXmlWriter->WriteFullEndElement();
        }
    }

    return oResult;
}

BOOL CObjectFilter::WriteStructuredObjectFilterSchemaTo(CXmlWriter* p_pXmlWriter)
{
    BOOL oResult(FALSE);
    CMmcDataConversion dataConversion;
    CStdString strIndex(_T(""));

    if(p_pXmlWriter)
    {
        oResult = TRUE;

        for(size_t i = 0; i < m_VisibleStructuredObject.size(); i++)
        {
            //Start Element "Structured object"
            if(oResult && !p_pXmlWriter->WriteStartElement(_T("StructuredObject"))) oResult = FALSE;

            dataConversion.Word2HexWordStr(m_VisibleStructuredObject[i], &strIndex);
            if(oResult && !p_pXmlWriter->WriteElement(_T("Index"), strIndex)) oResult = FALSE;

            //End Element "Structured object"
            if(oResult && !p_pXmlWriter->WriteFullEndElement()) oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CObjectFilter::ReadStructuredObjectFilterSchemaFrom(CXmlReader* p_pXmlReader)
{
    BOOL oAllObjectFiltersRead(FALSE);
    BOOL oResult(FALSE);
    CStdString strIndex(_T(""));
    int lCount(0);

    if(p_pXmlReader)
    {
        oResult = TRUE;

        while(!oAllObjectFiltersRead)
        {
            if(p_pXmlReader->ReadStartElement(_T("StructuredObject"), lCount++))
            {
                if(oResult && !p_pXmlReader->ReadElement(_T("Index"), 0, strIndex)) oResult = FALSE;
                AddVisibleStructuredObject(strIndex);

                //End Element
                p_pXmlReader->ReadEndElement();
            }
            else
            {
                oAllObjectFiltersRead = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CObjectFilter::WriteVisibleObjectFilterSchemaTo(CXmlWriter* p_pXmlWriter)
{
    BOOL oResult(FALSE);
    CMmcDataConversion dataConversion;
    CStdString strIndex(_T(""));
    CStdString strSubIndex(_T(""));

    if(p_pXmlWriter)
    {
        oResult = TRUE;

        for(map<WORD, vector<BYTE> >::iterator it = m_VisibleObject.begin(); it != m_VisibleObject.end(); it++)
        {
            vector<BYTE> subIndex = (*it).second;

            for(vector<BYTE>::iterator ite = subIndex.begin(); ite < subIndex.end(); ite++)
            {
                //Start Element "Visible object"
                if(oResult && !p_pXmlWriter->WriteStartElement(_T("VisibleObject"))) oResult = FALSE;

                dataConversion.Word2HexWordStr((*it).first, &strIndex);
                if(oResult && !p_pXmlWriter->WriteElement(_T("Index"), strIndex)) oResult = FALSE;

                dataConversion.UChar2HexUCharStr(*ite, &strSubIndex);
                if(oResult && !p_pXmlWriter->WriteElement(_T("SubIndex"), strSubIndex)) oResult = FALSE;

                //End Element "Visible object"
                if(oResult && !p_pXmlWriter->WriteFullEndElement()) oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CObjectFilter::ReadVisibleObjectFilterSchemaFrom(CXmlReader* p_pXmlReader)
{
    BOOL oAllObjectFiltersRead(FALSE);
    BOOL oResult(FALSE);
    CStdString strIndex(_T(""));
    CStdString strSubIndex(_T(""));
    int lCount(0);

    if(p_pXmlReader)
    {
        oResult = TRUE;

        while(!oAllObjectFiltersRead)
        {
            if(p_pXmlReader->ReadStartElement(_T("VisibleObject"), lCount++))
            {
                if(oResult && !p_pXmlReader->ReadElement(_T("Index"), 0, strIndex)) oResult = FALSE;
                if(oResult && !p_pXmlReader->ReadElement(_T("SubIndex"), 0, strSubIndex)) oResult = FALSE;
                AddVisibleObject(strIndex, strSubIndex);

                //End Element
                p_pXmlReader->ReadEndElement();
            }
            else
            {
                oAllObjectFiltersRead = TRUE;
            }
        }
    }

    return oResult;
}



