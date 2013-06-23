#include "stdafx.h"
#include <PersistenceReaderWriter/XmlReader.h>
#include <PersistenceReaderWriter/XmlWriter.h>
#include <Classes/MmcTextEncoder.h>

using namespace std;

CXmlWriter::CXmlWriter(void):
    m_pXmlParser(0),
    m_oExternalXmlParser(false)
{
    m_pOutputFile = 0;

    CreateXmlParser();
}

CXmlWriter::~CXmlWriter(void)
{
    DeleteXmlParser();
}

bool CXmlWriter::GetXmlParser(XML_PARSER*& p_rpXmlParser)
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        p_rpXmlParser = m_pXmlParser;
        oResult = true;
    }

    return oResult;
}

bool CXmlWriter::SetXmlParser(XML_PARSER* p_pXmlParser)
{
    bool oResult(false);

    DeleteXmlParser();
    if(!m_pXmlParser && p_pXmlParser)
    {
        m_pXmlParser = p_pXmlParser;
        m_oExternalXmlParser = true;
        oResult = true;
    }

    return oResult;
}

bool CXmlWriter::Load(std::iostream* p_pFile)
{
    CStdStringA strDocument;
    bool oResult = false;

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Reset_XML_Document())
        {
            if(ReadXmlDataFromFile(p_pFile, &strDocument))
            {
                oResult = m_pXmlParser->Load_XML_From_Buffer(&strDocument);
            }
        }
    }

    return oResult;
}

bool CXmlWriter::WriteAttributeValue(CStdString p_Name, bool p_oValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(GetBoolString(p_oValue, valueString))
        {
            if(m_pXmlParser->Set_Attribute(p_Name, valueString))
            {
                oResult = true;
            }
        }
    }

    return oResult;
}

bool CXmlWriter::WriteAttributeValue(CStdString p_Name, BYTE p_ubValue, bool p_oHexFormat)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(GetUInt32String((DWORD)p_ubValue, valueString, p_oHexFormat))
        {
            if(m_pXmlParser->Set_Attribute(p_Name, valueString))
            {
                oResult = true;
            }
        }
    }

    return oResult;
}

bool CXmlWriter::WriteAttributeValue(CStdString p_Name, WORD p_usValue, bool p_oHexFormat)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(GetUInt32String((DWORD)p_usValue, valueString, p_oHexFormat))
        {
            if(m_pXmlParser->Set_Attribute(p_Name, valueString))
            {
                oResult = true;
            }
        }
    }

    return oResult;
}

bool CXmlWriter::WriteAttributeValue(CStdString p_Name, DWORD p_ulValue, bool p_oHexFormat)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(GetUInt32String((DWORD)p_ulValue, valueString, p_oHexFormat))
        {
            if(m_pXmlParser->Set_Attribute(p_Name, valueString))
            {
                oResult = true;
            }
        }
    }

    return oResult;
}

bool CXmlWriter::WriteAttributeValue(CStdString p_Name, int p_lValue, bool p_oHexFormat)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(GetInt32String(p_lValue, valueString, p_oHexFormat))
        {
            if(m_pXmlParser->Set_Attribute(p_Name, valueString))
            {
                oResult = true;
            }
        }
    }

    return oResult;
}

bool CXmlWriter::WriteElement(CXmlReader& p_rXmlReader, CStdString p_XPathSourceNode, int p_lElementIndex, CStdString p_XPathTargetNode)
{
    XML_PARSER* pSourceDocument = 0;
    bool oResult = false;

    if(m_pXmlParser)
    {
        if(p_rXmlReader.GetXmlParser(pSourceDocument))
        {
            oResult = m_pXmlParser->Copy_Node(pSourceDocument, p_XPathSourceNode, p_lElementIndex, p_XPathTargetNode);
        }
    }

    return oResult;
}

bool CXmlWriter::WriteElement(CXmlWriter& p_rXmlWriter, CStdString p_XPathSourceNode, int p_lElementIndex, CStdString p_XPathTargetNode)
{
    XML_PARSER* pSourceDocument = 0;
    bool oResult = false;

    if(m_pXmlParser)
    {
        if(p_rXmlWriter.GetXmlParser(pSourceDocument))
        {
            oResult = m_pXmlParser->Copy_Node(pSourceDocument, p_XPathSourceNode, p_lElementIndex, p_XPathTargetNode);
        }
    }

    return oResult;
}

bool CXmlWriter::WriteElement(CStdString p_Name, CStdString p_String)
{
    bool oResult = true;

    if(oResult && !WriteStartElement(p_Name)) oResult = false;
    if(oResult && !WriteString(p_String)) oResult = false;
    if(oResult && !WriteFullEndElement()) oResult = false;

    return oResult;
}

bool CXmlWriter::WriteElement(CStdString p_Name, DWORD p_ulValue, bool p_oHexFormat)
{
    CStdString valueString = _T("");
    bool oResult = false;

    if(GetUInt32String(p_ulValue, valueString, p_oHexFormat))
    {
        oResult = WriteElement(p_Name, valueString);
    }

    return oResult;
}

bool CXmlWriter::WriteElement(CStdString p_Name, int p_lValue, bool p_oHexFormat)
{
    CStdString valueString = _T("");
    bool oResult = false;

    if(GetInt32String(p_lValue, valueString, p_oHexFormat))
    {
        oResult = WriteElement(p_Name, valueString);
    }

    return oResult;
}

bool CXmlWriter::WriteElement(CStdString p_Name, __int64 p_llValue, bool p_oHexFormat)
{
    CStdString valueString = _T("");
    bool oResult = false;

    if(GetInt64String(p_llValue, valueString, p_oHexFormat))
    {
        oResult = WriteElement(p_Name, valueString);
    }

    return oResult;
}

bool CXmlWriter::WriteElement(CStdString p_Name, float p_fValue)
{
    CStdString valueString = _T("");
    bool oResult = false;

    if(GetFloatString(p_fValue, valueString))
    {
        oResult = WriteElement(p_Name, valueString);
    }

    return oResult;
}

bool CXmlWriter::WriteElement(CStdString p_Name, bool p_oValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(p_oValue)
    {
        valueString = _T("true");
    }
    else
    {
        valueString = _T("false");
    }
    oResult = WriteElement(p_Name, valueString);

    return oResult;
}

bool CXmlWriter::ReadXmlDataFromFile(std::iostream* p_pFile, CStdStringA* p_pXmlData)
{
    bool oResult = false;

    if(p_pXmlData && p_pFile && p_pFile->good() )
    {
        p_pFile->seekg (0, ios::end);
        
		size_t length = (size_t)p_pFile->tellg();

        p_pFile->seekg (0, ios::beg);

		p_pFile->read (p_pXmlData->GetBuffer(length + 1), length );

		p_pXmlData->ReleaseBuffer();
    }

    return oResult;
}

bool CXmlWriter::Reset()
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        m_pXmlParser->Reset_XML_Document();

        oResult = true;
    }

    return oResult;
}

bool CXmlWriter::SetOutput(std::iostream* p_pFile)
{
    bool oResult = true;

    m_pOutputFile = p_pFile;

    return oResult;
}

bool CXmlWriter::Flush()
{
    CStdString strDocument;
    CStdString strTest;
    bool oResult = false;

    if(m_pOutputFile && m_pXmlParser)
    {
        //Init
        oResult = true;

        //Format XmlDocument
        m_pXmlParser->FormatDocument();

        //Get XmlDocument from Parser
        if(oResult && !m_pXmlParser->Get_XML_Document(&strDocument)) oResult = false;

        //Write XmlDocument to File
        if(oResult && !WriteXmlDataToFile(&strDocument, m_pOutputFile)) oResult = false;
    }

    return oResult;
}


bool CXmlWriter::WriteXmlDataToFile(CStdString* p_pXmlData, std::iostream* p_pFile)
{
    bool oResult = false;

    if(p_pXmlData && p_pFile)
    {
		CStdStringA utf8data = Unicode2Mbcs(*p_pXmlData);
		//write to outfile
        p_pFile->write (utf8data, utf8data.size());

        oResult = true;
    }

    return oResult;
}

bool CXmlWriter::Flush(CStdString& p_rDocument)
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        //Init
        oResult = true;

        //Format XmlDocument
        m_pXmlParser->FormatDocument();

        //Get XmlDocument from Parser
        if(oResult && !m_pXmlParser->Get_XML_Document(&p_rDocument)) oResult = false;
    }

    return oResult;
}

bool CXmlWriter::WriteStartDocument()
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Reset_XML_Document())
        {
            m_pXmlParser->Set_Header("xml", "version", "1.0");
            m_pXmlParser->Set_Header("xml", "encoding", "utf-8");

            oResult = true;
        }
    }

    return oResult;
}

bool CXmlWriter::WriteEndDocument()
{
    bool oResult = true;

    return oResult;
}

bool CXmlWriter::CreateXmlParser()
{
    bool oResult = false;

    DeleteXmlParser();
    if(!m_pXmlParser)
    {
        m_pXmlParser = new XML_PARSER();
        m_oExternalXmlParser = false;
        oResult = true;
    }

    return oResult;
}

bool CXmlWriter::DeleteXmlParser()
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        if(!m_oExternalXmlParser)
        {
            delete m_pXmlParser;
        }
        m_pXmlParser = 0;
        oResult = true;
    }

    return oResult;
}

bool CXmlWriter::StartElement(CStdString p_Name)
{
    bool oResult = true;

    if(m_pXmlParser)
    {
        //Start of Element
        if(oResult && !m_pXmlParser->Go_to_Child(p_Name)) oResult = false;
    }

    return oResult;
}

bool CXmlWriter::EndElement()
{
    bool oResult = true;

    if(m_pXmlParser)
    {
        //End of Element
        if(!m_pXmlParser->Is_Root())
        {
            //Go to Parent
            if(oResult && !m_pXmlParser->Go_to_Parent()) oResult = false;
        }
    }

    return oResult;
}

bool CXmlWriter::WriteStartElement(CStdString p_Name)
{
    bool oResult = true;

    if(m_pXmlParser)
    {
        //Start of Element
        if(oResult && !m_pXmlParser->Add_LastChildNode(p_Name)) oResult = false;
    }

    return oResult;
}

bool CXmlWriter::WriteString(CStdString p_String)
{
    bool oResult = true;

    if(m_pXmlParser)
    {
        //Element Value
        if(oResult && !m_pXmlParser->Set_TextValue(p_String)) oResult = false;
    }

    return oResult;
}

bool CXmlWriter::WriteFullEndElement()
{
    bool oResult = true;

    if(m_pXmlParser)
    {
        //End of Element
        if(!m_pXmlParser->Is_Root())
        {
            //29.03.2011: Not supported by XmlParser ODR
            /*//Add Empty Text
            if(m_pXmlParser->Get_Child_Count() == 0)
            {
                if(oResult && !m_pXmlParser->Add_LastChildText(_T(""))) oResult = false;
                if(oResult && !m_pXmlParser->Go_to_Parent()) oResult = false;
            }*/

            //Go to Parent
            if(oResult && !m_pXmlParser->Go_to_Parent()) oResult = false;
        }
    }

    return oResult;
}

bool CXmlWriter::WriteAttributeString(CStdString p_Name, CStdString p_String)
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        //Attribute Value
        oResult = m_pXmlParser->Set_Attribute(p_Name, p_String);
    }

    return oResult;
}

bool CXmlWriter::RemoveElement(CStdString p_XPathNode)
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        oResult = m_pXmlParser->Remove_Node(p_XPathNode);
    }

    return oResult;
}

bool CXmlWriter::GetUInt32String(DWORD p_ulValue, CStdString& p_rValueString, bool p_oHexFormat)
{
    bool oResult = TRUE;

    if(p_oHexFormat)
    {
        p_rValueString.Format(_T("0x%.8X"), p_ulValue);
    }
    else
    {
        p_rValueString.Format(_T("%u"), p_ulValue);
    }

    return oResult;
}

bool CXmlWriter::GetInt32String(int p_lValue, CStdString& p_rValueString, bool p_oHexFormat)
{
    bool oResult = TRUE;

    if(p_oHexFormat)
    {
        p_rValueString.Format(_T("0x%.8X"), p_lValue);
    }
    else
    {
        p_rValueString.Format(_T("%i"), p_lValue);
    }

    return oResult;
}

bool CXmlWriter::GetInt64String(__int64 p_llValue, CStdString& p_rValueString, bool p_oHexFormat)
{
    bool oResult = TRUE;

    if(p_oHexFormat)
    {
        p_rValueString.Format(_T("0x%.16X"), p_llValue);
    }
    else
    {
        p_rValueString.Format(_T("%i"), p_llValue);
    }

    return oResult;
}

bool CXmlWriter::GetBoolString(bool p_oValue, CStdString& p_rValueString)
{
    bool oResult = TRUE;

    if(p_oValue)
    {
        p_rValueString = _T("TRUE");
    }
    else
    {
        p_rValueString = _T("FALSE");
    }

    return oResult;
}

bool CXmlWriter::GetFloatString(float p_fValue, CStdString& p_rValueString)
{
    bool oResult = TRUE;

    p_rValueString.Format(_T("%.6f"), p_fValue);

    return oResult;
}