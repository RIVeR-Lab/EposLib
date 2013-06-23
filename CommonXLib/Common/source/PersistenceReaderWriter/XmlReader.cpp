#include "stdafx.h"
#include <PersistenceReaderWriter/XmlReader.h>
#include <memory.h>
#include <stdlib.h>
#include <MmcTypeDefinition.h>
#include <sstream>

using namespace std;

CXmlReader::CXmlReader(void):
    m_pXmlParser(0),
    m_oExternalXmlParser(false)
{
    CreateXmlParser();
}

CXmlReader::~CXmlReader(void)
{
    DeleteXmlParser();
}

bool CXmlReader::CreateXmlParser()
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

bool CXmlReader::DeleteXmlParser()
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

bool CXmlReader::GetXmlParser(XML_PARSER*& p_rpXmlParser)
{
    bool oResult(false);

    if(m_pXmlParser)
    {
        p_rpXmlParser = m_pXmlParser;
        oResult = true;
    }

    return oResult;
}

bool CXmlReader::SetXmlParser(XML_PARSER* p_pXmlParser)
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

bool CXmlReader::Load(CStdString p_FileName)
{
    CStdString bstrDocument;
    bool oResult(false);

    if(m_pXmlParser)
    {
        m_pXmlParser->Reset_XML_Document();

        oResult = m_pXmlParser->Load_XML_Document(p_FileName);
    }

    return oResult;
}

bool CXmlReader::Load(std::iostream* p_pFile)
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
bool CXmlReader::LoadFromBuffer(char* p_pBuffer, size_t p_BufferSize)
{
	bool oResult = false;

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Reset_XML_Document())
        {
            oResult = m_pXmlParser->Load_XML_From_Buffer(p_pBuffer, p_BufferSize);
        }
    }

    return oResult;
}

bool CXmlReader::LoadFromCStdString(CStdStringA p_FileData)
{
    bool oResult = false;

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Reset_XML_Document())
        {
            oResult = m_pXmlParser->Load_XML_From_Buffer(&p_FileData);
        }
    }

    return oResult;
}

bool CXmlReader::ReadXmlDataFromFile(std::iostream* p_pFile, CStdStringA* p_pXmlData)
{
    bool oResult = false;

    if(p_pXmlData && p_pFile && p_pFile->good())
    {
        p_pFile->seekg (0, ios::end);
        size_t length = (size_t)p_pFile->tellg();
        if(0 < length)
        {
            p_pFile->seekg (0, ios::beg);

			p_pFile->read (p_pXmlData->GetBuffer(length), length );
			
			p_pXmlData->ReleaseBuffer();

            oResult = true;
        }
    }

    return oResult;
}

bool CXmlReader::RemoveEncoding(BYTE* p_pDataBuffer, int p_lLen, TCHAR*& p_rpRemovedDataBuffer, int& p_rlRemovedLen)
{
    CStdString header = _T("<?xml version=\"1.0\"?>");
    CStdString data = _T("");
    bool oResult(false);

    if(p_pDataBuffer)
    {
#ifdef _UNICODE        
		//Init Data
		USES_CONVERSION;
        //Init Data
        data = A2CT((char*)p_pDataBuffer);
#else
		memcpy( data.GetBuffer(p_lLen), p_pDataBuffer, p_lLen);
		data.ReleaseBuffer();
#endif
        //Remove
        int lStartIndex = data.Find(_T("<?"));
        int lEndIndex = data.Find(_T("?>"));
        if((lStartIndex != -1) && (lEndIndex != -1) && (lEndIndex > lStartIndex))
        {
            data.Delete(0, lEndIndex + _tcslen(_T("?>")));
            data.Insert(0, header);
        }

        //Add to new DataBuffer
        p_rlRemovedLen = (data.GetLength()+1)*sizeof(TCHAR);
        p_rpRemovedDataBuffer = (TCHAR*)malloc(p_rlRemovedLen);

        memset(p_rpRemovedDataBuffer, 0, p_rlRemovedLen);
        memcpy(p_rpRemovedDataBuffer, data.GetBuffer(), p_rlRemovedLen-1);
        
		data.ReleaseBuffer();
        oResult = true;
    }

    return oResult;
}

bool CXmlReader::ReadStartDocument()
{
    bool oResult(true);

    if(m_pXmlParser)
    {
        m_pXmlParser->Go_to_Root();
    }
    return oResult;
}

bool CXmlReader::ReadEndDocument()
{
    bool oResult(true);

    if(m_pXmlParser)
    {
        m_pXmlParser->Go_to_Root();
    }
    return oResult;
}

bool CXmlReader::IsElementExisting(CStdString p_Name, int p_lElementIndex)
{
    bool oResult(false);

    if(ReadStartElement(p_Name, p_lElementIndex))
    {
        oResult = true;
        ReadEndElement();
    }

    return oResult;
}

bool CXmlReader::ReadStartElement(CStdString p_Name, int p_lElementIndex)
{
    bool oResult(false);

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Go_to_Child(p_Name))
        {
            oResult = true;
            for(int i=0;i<p_lElementIndex;i++)
            {
                if(!m_pXmlParser->Go_Forward(p_Name))
                {
                    oResult = false;
                    m_pXmlParser->Go_to_Parent();
                    break;
                }
            }
        }
    }

    return oResult;
}

bool CXmlReader::ReadNextSiblingElement(CStdString p_Name)
{
    bool oResult(false);

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Go_Forward(p_Name))
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

bool CXmlReader::ReadString(CStdString& p_rString)
{
    bool oResult(true);

    if(m_pXmlParser)
    {
        p_rString = m_pXmlParser->Get_TextValue();
    }

    return oResult;
}

bool CXmlReader::ReadValue(int& p_rlValue)
{
	bool oResult(false);
	CStdString valueString(_T(""));

	if(ReadString(valueString))
	{
		oResult = GetInt32Value(valueString, p_rlValue);
	}

	return oResult;
}

bool CXmlReader::ReadAttributeString(CStdString p_Name, CStdString& p_rString)
{
    bool oResult(false);

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Is_Having_Attribute(p_Name))
        {
            p_rString = m_pXmlParser->Get_Attribute_Value();
            oResult = true;
        }
    }

    return oResult;
}

bool CXmlReader::ReadEndElement()
{
    bool oResult(false);

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Go_to_Parent())
        {
            oResult = true;
        }
    }

    return oResult;
}

bool CXmlReader::ReadAttributeValue(CStdString p_Name, bool& p_roValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Is_Having_Attribute(p_Name))
        {
            valueString = m_pXmlParser->Get_Attribute_Value();

            oResult = GetboolValue(valueString, p_roValue);
        }
    }

    return oResult;
}

bool CXmlReader::ReadAttributeValue(CStdString p_Name, BYTE& p_rubValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Is_Having_Attribute(p_Name))
        {
            valueString = m_pXmlParser->Get_Attribute_Value();

            oResult = GetUInt8Value(valueString, p_rubValue);
        }
    }

    return oResult;
}

bool CXmlReader::ReadAttributeValue(CStdString p_Name, WORD& p_rusValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Is_Having_Attribute(p_Name))
        {
            valueString = m_pXmlParser->Get_Attribute_Value();

            oResult = GetUInt16Value(valueString, p_rusValue);
        }
    }

    return oResult;
}

bool CXmlReader::ReadAttributeValue(CStdString p_Name, DWORD& p_rulValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Is_Having_Attribute(p_Name))
        {
            valueString = m_pXmlParser->Get_Attribute_Value();

            oResult = GetUInt32Value(valueString, p_rulValue);
        }
    }

    return oResult;
}

bool CXmlReader::ReadAttributeValue(CStdString p_Name, __int64& p_rllValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Is_Having_Attribute(p_Name))
        {
            valueString = m_pXmlParser->Get_Attribute_Value();

            oResult = GetInt64Value(valueString, p_rllValue);
        }
    }

    return oResult;
}

bool CXmlReader::ReadAttributeAsHexValue(CStdString p_Name, BYTE& p_rubValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
	{
		if(m_pXmlParser->Is_Having_Attribute(p_Name))
		{
			long lValue(0);
			valueString = m_pXmlParser->Get_Attribute_Value();

			lValue = _tcstol(valueString, 0 , 16);
			p_rubValue = (BYTE)lValue;
			oResult = TRUE;
		}
	}

    return oResult;
}

bool CXmlReader::ReadAttributeAsHexValue(CStdString p_Name, WORD& p_rusValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
	{
		if(m_pXmlParser->Is_Having_Attribute(p_Name))
		{
			long lValue(0);
			valueString = m_pXmlParser->Get_Attribute_Value();

			lValue = _tcstol(valueString, 0 , 16);
			p_rusValue = (WORD)lValue;
			oResult = TRUE;
		}
	}

    return oResult;
}

bool CXmlReader::ReadAttributeAsHexValue(CStdString p_Name, DWORD& p_rulValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
	{
		if(m_pXmlParser->Is_Having_Attribute(p_Name))
		{
			long lValue(0);
			valueString = m_pXmlParser->Get_Attribute_Value();

			lValue = _tcstol(valueString, 0 , 16);
			p_rulValue = (DWORD)lValue;
			oResult = TRUE;
		}
	}

    return oResult;
}

bool CXmlReader::ReadAttributeAsHexValue(CStdString p_Name, __int64& p_rllValue)
{
    bool oResult(false);
    CStdString valueString(_T(""));

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Is_Having_Attribute(p_Name))
        {
            __int64 lValue(0);
            valueString = m_pXmlParser->Get_Attribute_Value();

            lValue = _tcstol(valueString, 0 , 16);
            p_rllValue = (__int64)lValue;
            oResult = TRUE;
        }
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, CStdString& p_rString)
{
    bool oResult(false);

    if(ReadStartElement(p_Name, p_lElementIndex))
    {
        oResult = ReadString(p_rString);
        ReadEndElement();
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, BYTE& p_rucValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = GetUInt8Value(valueString, p_rucValue);
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, WORD& p_rusValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = GetUInt16Value(valueString, p_rusValue);
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, DWORD& p_rulValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = GetUInt32Value(valueString, p_rulValue);
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, int& p_rlValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = GetInt32Value(valueString, p_rlValue);
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, __int64& p_rllValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = GetInt64Value(valueString, p_rllValue);
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, float& p_rfValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = GetFloatValue(valueString, p_rfValue);
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, double& p_rffValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = GetDoubleValue(valueString, p_rffValue);
    }

    return oResult;
}

bool CXmlReader::ReadElement(CStdString p_Name, int p_lElementIndex, bool& p_roValue)
{
    CStdString valueString(_T(""));
    bool oResult(false);

    if(ReadElement(p_Name, p_lElementIndex, valueString))
    {
        oResult = true;
        valueString.MakeLower();
        if(_T("true") == (valueString))
        {
            p_roValue = true;
        }
        else if(_T("false") == valueString)
        {
            p_roValue = false;
        }
        else
        {
            //No boolean
            oResult = false;
        }
    }

    return oResult;
}

bool CXmlReader::GetUInt8Value(CStdString p_ValueString, BYTE& p_rubValue)
{
    __int64 llValue(0);
    bool oResult(true);
    int base(10);

    p_ValueString = p_ValueString.ToLower();
    if(0 == p_ValueString.find(_T("#x"))) p_ValueString.Replace('#', '0');
    if(0 == p_ValueString.find(_T("0x"))) base = 16;

    llValue = _tcstol(p_ValueString, 0, base);
    p_rubValue = (BYTE)llValue;

    return oResult;
}

bool CXmlReader::GetUInt16Value(CStdString p_ValueString, WORD& p_rusValue)
{
    __int64 llValue(0);
    int base(10);
    bool oResult(true);

    p_ValueString = p_ValueString.ToLower();
    if(0 == p_ValueString.find(_T("#x"))) p_ValueString.Replace('#', '0');
    if(0 == p_ValueString.find(_T("0x"))) base = 16;

    llValue = _tcstol(p_ValueString, 0, base);
    p_rusValue = (WORD)llValue;

    return oResult;
}

bool CXmlReader::GetUInt32Value(CStdString p_ValueString, DWORD& p_rulValue)
{
    __int64 llValue(0);
    int base(10);
    bool oResult(true);

    p_ValueString = p_ValueString.ToLower();
    if(0 == p_ValueString.find(_T("#x"))) p_ValueString.Replace('#', '0');
    if(0 == p_ValueString.find(_T("0x"))) base = 16;

    llValue = _tcstol(p_ValueString, 0, base);
    p_rulValue = (DWORD)llValue;

    return oResult;
}

bool CXmlReader::GetInt32Value(CStdString p_ValueString, int& p_rlValue)
{
    __int64 llValue(0);
    int base(10);
    bool oResult(true);

    p_ValueString = p_ValueString.ToLower();
    if(0 == p_ValueString.find(_T("#x"))) p_ValueString.Replace('#', '0');
    if(0 == p_ValueString.find(_T("0x"))) base = 16;

    llValue = _tcstol(p_ValueString, 0, base);
    p_rlValue = (int)llValue;

    return oResult;
}

bool CXmlReader::GetInt64Value(CStdString p_ValueString, __int64& p_rlValue)
{
    __int64 llValue(0);
    int base(10);
    bool oResult(true);

    p_ValueString = p_ValueString.ToLower();
    if(0 == p_ValueString.find(_T("#x"))) p_ValueString.Replace('#', '0');
    if(0 == p_ValueString.find(_T("0x"))) base = 16;

    llValue = _tcstol(p_ValueString, 0, base);
    p_rlValue = (int)llValue;

    return oResult;
}

bool CXmlReader::GetboolValue(CStdString p_ValueString, bool& p_roValue)
{
    p_roValue = (p_ValueString.CompareNoCase(_T("true")) == 0);

    return !p_ValueString.IsEmpty();
}

bool CXmlReader::GetFloatValue(CStdString p_ValueString, float& p_rfValue)
{
    double ffValue(0);
    bool oResult(true);

    ffValue = _tstof(p_ValueString);
    p_rfValue = (float)ffValue;

    return oResult;
}

bool CXmlReader::GetDoubleValue(CStdString p_ValueString, double& p_rffValue)
{
    bool oResult(true);

    p_rffValue = _tstof(p_ValueString);

    return oResult;
}

bool CXmlReader::CopyElement(CXmlReader& p_rXmlReader, CStdString p_SourceXPathNode, int p_lElementIndex, CStdString p_TargetXPathNode)
{
    XML_PARSER* pSourceDocument(0);
    bool oResult(false);

    if(m_pXmlParser)
    {
        if(p_rXmlReader.GetXmlParser(pSourceDocument))
        {
            oResult = m_pXmlParser->Copy_Node(pSourceDocument, p_SourceXPathNode, p_lElementIndex, p_TargetXPathNode);
        }
    }

    return oResult;
}

int CXmlReader::GetChildElementCount()
{
    int lCount = 0;

    if(m_pXmlParser)
    {
        lCount = m_pXmlParser->Get_Child_Count();
    }

    return lCount;
}

bool CXmlReader::GetChildElementName(int p_iIndex, CStdString& p_rName)
{
	bool oResult(false);
	int lCount = 0;

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Go_to_Child())
		{
			//Init
			oResult = true;

			//Goto Sibling defined by index
			for(int i = 0; i < p_iIndex; i++)
			{
				if(!m_pXmlParser->Go_Forward())
				{
					oResult = false;
				}
			}

			//Get Name
			if(oResult)
			{
				p_rName = m_pXmlParser->Get_CurrentTagName();
			}

			//Goto Parent
			if(!m_pXmlParser->Go_to_Parent()) oResult = false;
		}
    }

    return oResult;
}

int CXmlReader::GetChildElementCount(CStdString p_Name)
{
    int lCount = 0;

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Go_to_Child(p_Name))
        {
            lCount++;
            while(m_pXmlParser->Go_Forward(p_Name))
            {
                lCount++;
            }

            m_pXmlParser->Go_to_Parent();
        }
    }

    return lCount;
}

int CXmlReader::GetChildElementCount(CStdString p_Name, CStdString p_SubName)
{
    int lCount = 0;

    if(m_pXmlParser)
    {
        if(m_pXmlParser->Go_to_Child(p_Name))
        {
            //First Element
            if(m_pXmlParser->Go_to_Child(p_SubName))
            {
                //SubElements available
                lCount++;
                while(m_pXmlParser->Go_Forward(p_SubName))
                {
                    lCount++;
                }
                m_pXmlParser->Go_to_Parent();
            }
            else
            {
                //No SubElements available
                lCount++;
            }

            //Following Elements
            while(m_pXmlParser->Go_Forward(p_Name))
            {
                if(m_pXmlParser->Go_to_Child(p_SubName))
                {
                    //SubElements available
                    lCount++;
                    while(m_pXmlParser->Go_Forward(p_SubName))
                    {
                        lCount++;
                    }
                    m_pXmlParser->Go_to_Parent();
                }
                else
                {
                    //No SubElements available
                    lCount++;
                }
            }
            m_pXmlParser->Go_to_Parent();
        }
    }

    return lCount;
}
