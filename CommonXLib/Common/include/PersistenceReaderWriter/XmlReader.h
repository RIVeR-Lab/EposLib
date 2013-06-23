#pragma once

#include <StdString.h>
#include <MmcTypeDefinition.h>
#include "../Classes/XML_PARSER.h"
#include <iostream>

class CXmlReader
{
public:
    CXmlReader(void);
    ~CXmlReader(void);

    bool Load(CStdString p_FileName);
	bool Load(std::iostream* p_pFile);

    bool LoadFromCStdString(CStdStringA p_FileData);
	bool LoadFromBuffer(char* p_pBuffer, size_t p_BufferSize);
    bool GetXmlParser(XML_PARSER*& p_rpXmlParser);
    bool SetXmlParser(XML_PARSER* p_pXmlParser);
    
	bool ReadString(CStdString& p_rString);
	bool ReadValue(int& p_rlValue);

    //Document
    bool ReadStartDocument();
    bool ReadEndDocument();

    //Attributes
    bool ReadAttributeString(CStdString p_Name, CStdString& p_rString);
    bool ReadAttributeValue(CStdString p_Name, bool& p_roValue);
    bool ReadAttributeValue(CStdString p_Name, BYTE& p_rubValue);
    bool ReadAttributeValue(CStdString p_Name, WORD& p_rusValue);
    bool ReadAttributeValue(CStdString p_Name, DWORD& p_rulValue);
    bool ReadAttributeValue(CStdString p_Name, __int64& p_rllValue);
    bool ReadAttributeAsHexValue(CStdString p_Name, BYTE& p_rubValue);
    bool ReadAttributeAsHexValue(CStdString p_Name, WORD& p_rusValue);
    bool ReadAttributeAsHexValue(CStdString p_Name, DWORD& p_rulValue);
    bool ReadAttributeAsHexValue(CStdString p_Name, __int64& p_rllValue);

    //Elements
    bool IsElementExisting(CStdString p_Name, int p_lElementIndex);
    bool ReadStartElement(CStdString p_Name, int p_lElementIndex);
    bool ReadNextSiblingElement(CStdString p_Name);
    bool ReadEndElement();
    bool ReadElement(CStdString p_Name, int p_lElementIndex, CStdString& p_rString);
    bool ReadElement(CStdString p_Name, int p_lElementIndex, BYTE& p_rucValue);
    bool ReadElement(CStdString p_Name, int p_lElementIndex, WORD& p_rusValue);
    bool ReadElement(CStdString p_Name, int p_lElementIndex, DWORD& p_rulValue);
    bool ReadElement(CStdString p_Name, int p_lElementIndex, int& p_rlValue);
    bool ReadElement(CStdString p_Name, int p_lElementIndex, __int64& p_rllValue);
    bool ReadElement(CStdString p_Name, int p_lElementIndex, float& p_rfValue);
	bool ReadElement(CStdString p_Name, int p_lElementIndex, double& p_rffValue);
    bool ReadElement(CStdString p_Name, int p_lElementIndex, bool& p_roValue);
    int GetChildElementCount();
	bool GetChildElementName(int p_iIndex, CStdString& p_rName);
    int GetChildElementCount(CStdString p_Name);
    int GetChildElementCount(CStdString p_Name, CStdString p_SubName);

    //Copy Elements
    bool CopyElement(CXmlReader& p_rXmlReader, CStdString p_SourceXPathNode, int p_lElementIndex, CStdString p_TargetXPathNode);

private:
    bool CreateXmlParser();
    bool DeleteXmlParser();
	bool ReadXmlDataFromFile(std::iostream* p_pFile, CStdStringA* p_pXmlData);
    bool RemoveEncoding(BYTE* p_pDataBuffer, int p_lLen, TCHAR*& p_rpRemovedDataBuffer, int& p_rlRemovedLen);

    bool GetUInt8Value(CStdString p_ValueString, BYTE& p_rubValue);
    bool GetUInt16Value(CStdString p_ValueString, WORD& p_rusValue);
    bool GetUInt32Value(CStdString p_ValueString, DWORD& p_rulValue);
    bool GetInt32Value(CStdString p_ValueString, int& p_rlValue);
    bool GetInt64Value(CStdString p_ValueString, __int64& p_rlValue);
    bool GetboolValue(CStdString p_ValueString, bool& p_roValue);
    bool GetFloatValue(CStdString p_ValueString, float& p_rfValue);
	bool GetDoubleValue(CStdString p_ValueString, double& p_rffValue);

private:
    XML_PARSER* m_pXmlParser;
    bool        m_oExternalXmlParser;
    CStdString    m_strFileName;
};
