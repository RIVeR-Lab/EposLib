#pragma once

#include <StdString.h>
#include <MmcTypeDefinition.h>
#include "../Classes/XML_PARSER.h"
#include <iostream>

class CXmlReader;

class CXmlWriter
{
public:
    CXmlWriter(void);
    ~CXmlWriter(void);

    //Common
    bool GetXmlParser(XML_PARSER*& p_rpXmlParser);
    bool SetXmlParser(XML_PARSER* p_pXmlParser);
    
	bool Load(std::iostream* p_pFile);
	bool SetOutput(std::iostream* p_pFile);
    bool Reset();    
    bool Flush();
    bool Flush(CStdString& p_rDocument);

    //Document
    bool WriteStartDocument();
    bool WriteEndDocument();

    //Navigation
    bool StartElement(CStdString p_Name);
    bool EndElement();

    //Attributes
    bool WriteAttributeString(CStdString p_Name, CStdString p_String);
    bool WriteAttributeValue(CStdString p_Name, bool p_oValue);
    bool WriteAttributeValue(CStdString p_Name, BYTE p_ubValue, bool p_oHexFormat = false);
    bool WriteAttributeValue(CStdString p_Name, WORD p_usValue, bool p_oHexFormat = false);
    bool WriteAttributeValue(CStdString p_Name, DWORD p_ulValue, bool p_oHexFormat = false);
    bool WriteAttributeValue(CStdString p_Name, int p_lValue, bool p_oHexFormat = false);

    //Elements
    bool WriteStartElement(CStdString p_Name);
    bool WriteFullEndElement();
    bool WriteString(CStdString p_String);
    bool WriteElement(CStdString p_Name, CStdString p_String);
    bool WriteElement(CStdString p_Name, DWORD p_ulValue, bool p_oHexFormat = false);
    bool WriteElement(CStdString p_Name, int p_lValue, bool p_oHexFormat = false);
    bool WriteElement(CStdString p_Name, __int64 p_llValue, bool p_oHexFormat = false);
    bool WriteElement(CStdString p_Name, float p_fValue);
    bool WriteElement(CStdString p_Name, bool p_oValue);

    bool WriteElement(CXmlReader& p_rXmlReader, CStdString p_SourceXPathNode, int p_lElementIndex, CStdString p_TargetXPathNode);
    bool WriteElement(CXmlWriter& p_rXmlWriter, CStdString p_SourceXPathNode, int p_lElementIndex, CStdString p_TargetXPathNode);
    bool RemoveElement(CStdString p_XPathNode);

private:
    bool CreateXmlParser();
    bool DeleteXmlParser();

    bool GetUInt32String(DWORD p_ulValue, CStdString& p_rValueString, bool p_oHexFormat);
    bool GetInt32String(int p_lValue, CStdString& p_rValueString, bool p_oHexFormat);
    bool GetInt64String(__int64 p_llValue, CStdString& p_rValueString, bool p_oHexFormat);
    bool GetBoolString(bool p_oValue, CStdString& p_rValueString);
    bool GetFloatString(float p_fValue, CStdString& p_rValueString);

	bool ReadXmlDataFromFile(std::iostream* p_pFile, CStdStringA* p_pXmlData);
	bool WriteXmlDataToFile(CStdString* p_pXmlData, std::iostream* p_pFile);

private:
    XML_PARSER* m_pXmlParser;
	std::iostream* m_pOutputFile;
    bool m_oExternalXmlParser;
};
