#include <stdafx.h>
#include <Classes/XML_PARSER.h>
#include <Xml/rapidxml_print.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <Classes/MmcTextEncoder.h>
#include <MmcTypeDefinition.h>
#include <string.h>

using namespace rapidxml;
using namespace std;

XML_PARSER::XML_PARSER()
{
	// Init our members
	//
	m_pCurrentNode = 0;
	m_pCurrentAttribute = 0;

	// -- Errors Init --
	//
	lasterror = _T("");

	tabLevel = 0;
}

XML_PARSER::~XML_PARSER()
{       // Free ressource
        //
        this->Reset_XML_Document();
}

bool XML_PARSER::Load_XML_Document(CStdString strFileName)
{       lasterror = _T("");

        // Reset Document
        this->Reset_XML_Document();
        
		std::ifstream file;

        file.open(strFileName);

        if( file.is_open() )
        {
        	std::string content;

        	file >> content;

        	CStdStringA source = content;

        	bool result = Load_XML_From_Buffer(&source);

        	file.close();

        	return result;
        }

		return false;
}

bool XML_PARSER::Load_XML_From_Buffer(char* p_pBuffer, size_t p_BufferSize)
{
	if((p_pBuffer != 0) && (0 < p_BufferSize))
	{
		try
        {
			//Parse Document
			m_Doc.parse<parse_full>(m_Doc.allocate_string(p_pBuffer));

			//Navigate to all elements and call callback function
			m_pCurrentNode = m_Doc.first_node();
			while(m_pCurrentNode)
			{
				Parse_Objects(m_pCurrentNode);
				m_pCurrentNode = m_pCurrentNode->next_sibling();
			}

			//Set CurrentNode to First Element
			m_pCurrentNode = m_Doc.first_node();
		}
		catch(rapidxml::parse_error &e)
        {
			CStdString reason = e.what();
            return false;
        }

		return true;
	}
	return false;
}

bool XML_PARSER::Load_XML_From_Buffer(CStdStringA* pSource)
{
	if( pSource != 0 )
	{
		try
        {
			//Parse Document
			m_Doc.parse<parse_full>(m_Doc.allocate_string(pSource->GetBuffer()));

			//Navigate to all elements and call callback function
			m_pCurrentNode = m_Doc.first_node();
			while(m_pCurrentNode)
			{
				Parse_Objects(m_pCurrentNode);
				m_pCurrentNode = m_pCurrentNode->next_sibling();
			}

			//Set CurrentNode to First Element
			m_pCurrentNode = m_Doc.first_node();
		}
		catch(rapidxml::parse_error &e)
        {
			CStdString reason = e.what();

			pSource->ReleaseBuffer();
            return false;
        }

		//Release
		pSource->ReleaseBuffer();

		return true;
	}

	return false;
}

void XML_PARSER::Parse_XML_Document()
{ /* Nothing to do , this method will be redefined in derived class */ }

void XML_PARSER::Parse_Objects(xml_node<char>* pObject)
{
    if(pObject)
	{
		// Update "CurrentNode"
		this->m_pCurrentNode = pObject;

		// Call User Parse Handling for let him what he want.
		this->Parse_XML_Document();

		if( pObject->type() == node_element )
		{
			// Child node
			xml_node<char>* pChild = pObject->first_node();
			while(pChild != 0)
			{
				this->Parse_Objects(pChild);

				pChild = pChild->next_sibling();
			}
		}
	}
}

CStdString XML_PARSER::GetXml()
{
	std::string xml_as_string;
	// watch for name collisions here, print() is a very common function name!
	rapidxml::print(std::back_inserter(xml_as_string), m_Doc);

	return xml_as_string;
}

bool XML_PARSER::Clone_Node(XML_PARSER* pSourceDocument, CStdString XPathNode, int p_lNodeIndex, xml_node<char>** p_pClonedNode)
{
	bool oResult = false;

	if(pSourceDocument && p_pClonedNode)
	{
		try
		{
			xml_node<char>* pChildNode = pSourceDocument->SelectSingleNode(XPathNode, p_lNodeIndex);
			if( pChildNode != 0 )
			{
				*p_pClonedNode = Clone_Node(pChildNode);
				oResult = true;
			}
		}
		catch(exception)
		{
			oResult = false;
		}
	}

	return oResult;
}

xml_node<char>* XML_PARSER::Clone_Node(const xml_node<char>* pSource)
{
    xml_node<char>* pResult = 0;

	if(pSource)
	{
		// Prepare result node
		pResult = m_Doc.allocate_node(pSource->type());

		if(pResult)
		{
			// Clone name and value
			pResult->name(m_Doc.allocate_string(pSource->name()), pSource->name_size());
			pResult->value(m_Doc.allocate_string(pSource->value()), pSource->value_size());

			// Clone child nodes and attributes
			for (xml_node<char> *child = pSource->first_node(); child; child = child->next_sibling())
				pResult->append_node(Clone_Node(child));
			for (xml_attribute<char> *attr = pSource->first_attribute(); attr; attr = attr->next_attribute())
				pResult->append_attribute(m_Doc.allocate_attribute(m_Doc.allocate_string(attr->name()), m_Doc.allocate_string(attr->value()), attr->name_size(), attr->value_size()));
		}
	}

    return pResult;
}

bool XML_PARSER::Copy_Node(XML_PARSER* pSourceDocument, CStdString strXPathSourceNode, int p_lSourceNodeIndex, CStdString strXPathTarget)
{
	bool oResult = false;
	try
	{
		xml_node<char>* clonedNode = NULL;

		if(pSourceDocument)
		{
			CStdString sourceDoc = pSourceDocument->GetXml();

			//Clone Node
			if(Clone_Node(pSourceDocument, strXPathSourceNode, p_lSourceNodeIndex, &clonedNode) && clonedNode)
			{
				//Add to TargetDocument
				if(Append_Child(strXPathTarget, clonedNode))
				{
					//Succeeded
					oResult = true;
				}
			}
		}
	}
	catch(exception)
	{
		oResult = false;
	}

	return oResult;
}

bool XML_PARSER::Remove_Node(CStdString XPathNode)
{
	xml_node<char>* pNodeChild = 0;
	xml_node<char>* pNodeParent = 0;
	bool oResult = false;

	pNodeChild = SelectSingleNode(XPathNode, 0);
	if(pNodeChild)
	{
		pNodeParent = pNodeChild->parent();
		if(pNodeParent)
		{
			pNodeParent->remove_node(pNodeChild);
			oResult = true;
		}
	}

	return oResult;
}

bool XML_PARSER::Remove_Node()
{
	xml_node<char>* pNodeChild = 0;
	xml_node<char>* pNodeParent = 0;
	bool oResult(false);
	
	if(m_pCurrentNode)
	{
		pNodeChild = m_pCurrentNode;
		pNodeParent = m_pCurrentNode->parent();
		if(pNodeParent)
		{
			m_pCurrentNode = m_pCurrentNode->next_sibling();
			pNodeParent->remove_node(pNodeChild);
			oResult = true;
		}
	}

	return oResult;
}

bool XML_PARSER::Remove_Childs(CStdString NodeName)
{
	bool oResult(true);
	bool oRemoved(true);

	while(oRemoved)
	{
		oRemoved = Remove_Child(NodeName);
	}

	return oResult;
}

bool XML_PARSER::Remove_Child(CStdString NodeName)
{
	xml_node<char>* pNodeChild = 0;
	bool oResult(false);
	
	if(m_pCurrentNode)
	{
		if(Go_to_Child(NodeName))
		{
			pNodeChild = m_pCurrentNode;
			if(Go_to_Parent())
			{
				m_pCurrentNode->remove_node(pNodeChild);
				oResult = true;
			}
		}
	}

	return oResult;
}

bool XML_PARSER::Go_to_Node(CStdString XPathNode)
{
	xml_node<char>* pChildNode = 0;
	bool oResult(false);

	pChildNode = SelectSingleNode(XPathNode, 0);
	if(pChildNode)
	{
		m_pCurrentNode = pChildNode;
		oResult = true;
	}

	return oResult;
}

xml_node<char>* XML_PARSER::SelectSingleNode(CStdString strXPathNode, int p_lNodeIndex)
{
	TCHAR* pch = _tcstok (strXPathNode.GetBuffer(), _T("/"));
	xml_node<char>* pChildNode = 0;
	std::string lastNodeName;
	//XPath Search
	while (pch != 0)
	{
		if(pChildNode == 0 )
		{		
			lastNodeName = Unicode2Mbcs(pch);
			pChildNode = m_Doc.first_node(lastNodeName.c_str());
		}
		else
		{
			lastNodeName = Unicode2Mbcs(pch);
			pChildNode = pChildNode->first_node(lastNodeName.c_str());
		}
		pch = _tcstok (0, _T("/"));
	}

	//NodeIndex
	for(int i = 0; i < p_lNodeIndex; i++)
	{
		if(pChildNode)	pChildNode = pChildNode->next_sibling(lastNodeName.c_str());
	}

	//Release
	strXPathNode.ReleaseBuffer();

	return pChildNode;
}

bool XML_PARSER::Append_Child(CStdString strXPathNode, xml_node<char>* pChildNode)
{
	bool oResult = false;

	try
	{
		if(strXPathNode == _T("/"))
		{
			//Add to doc as root element
			m_Doc.append_node(pChildNode);
			oResult = true;
		}
		else
		{
			//Add to element as child element
			xml_node<char>* childNode = SelectSingleNode(strXPathNode, 0);
			if( childNode )
			{
				childNode->append_node(pChildNode);
				oResult = true;
			}
		}
	}
	catch(exception)
	{
		oResult = false;
	}

	return oResult;
}

bool XML_PARSER::Get_XML_Document(CStdString* pStrXml)
{
	bool result = true;
	try
	{
		std::string s;
		rapidxml::print(std::back_inserter(s), m_Doc, 0);
		
#ifdef _UNICODE		
		int nLen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
		if( nLen )
		{
			WCHAR *pszUnicode = new WCHAR[nLen+1];
			pszUnicode[nLen] = _T('\0');
			int res = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.length()+1, pszUnicode, nLen);

			if( res )
			{	
				*pStrXml = pszUnicode;	
			}

			delete[] pszUnicode;
		}
#else
		*pStrXml = s.c_str();
#endif
		
	}
	catch(exception)
	{
		result = false;
	}

	return result;
}

CStdString XML_PARSER::Get_CurrentTag()
{
	CStdString strCurrentTag(_T(""));

	if(m_pCurrentNode != 0)
	{
		strCurrentTag = _T("<");
		strCurrentTag += m_pCurrentNode->name();
		strCurrentTag += _T(">");
	}

	return strCurrentTag;
}

CStdString XML_PARSER::Get_CurrentTagName()
{
	CStdString strCurrentTag(_T(""));

	if(m_pCurrentNode != 0)
	{
		strCurrentTag = m_pCurrentNode->name();
	}

	return strCurrentTag;
}

bool XML_PARSER::Is_Tag(CStdString aTag)
{
	bool oResult(false);

	if(this->Get_CurrentTag() == aTag)
	{
		oResult = true;
	}

	return oResult;
}

bool XML_PARSER::Is_Root()
{
	if( m_pCurrentNode == 0 )
		return false;

	xml_node<char>* childNode = m_Doc.first_node();

	while(childNode)
	{
		if( childNode->type() == node_element )
		{
			if( m_pCurrentNode == childNode )
				return true;
		}

		childNode = childNode->next_sibling();
	}

	return false;
}

CStdString XML_PARSER::Get_TextValue()
{
	CStdString strValue;
	
	if( m_pCurrentNode != 0 )
	{
		node_type type = m_pCurrentNode->type();

		if( type == node_element )
			strValue = Mbcs2Unicode(m_pCurrentNode->value());
		else
		{
			xml_node<char>* first_node = m_pCurrentNode->first_node();

			for (xml_node<char> *child = first_node; child; child = child->next_sibling())
			{
				if( type == node_element )
				{
					strValue = Mbcs2Unicode(m_pCurrentNode->value());
					break;
				}
			}
		}
	}

	return strValue;
}

int XML_PARSER::Get_Attribute_Count()
{
	xml_attribute<char>* attributes = m_pCurrentNode->first_attribute();
	xml_attribute<char>* attribute = attributes;
	int count = 0;
	while(attribute!=0)
	{
		count++;
		attribute = attributes->next_attribute();
	}

	return count;

}

CStdString XML_PARSER::Get_Attribute_Value(int index)
{  lasterror = _T("XML_PARSER::Get_Attribute_Value(int) failed");

	CStdString strRetVal;
	if(index < 0 || index > this->Get_Attribute_Count())
		return _T("");

	lasterror = _T("");

	xml_attribute<char>* attributes = m_pCurrentNode->first_attribute();
	xml_attribute<char>* attribute = attributes;
	int count = 0;
	while(attribute!=0)
	{
		if( count == index )
		{
			strRetVal = Mbcs2Unicode(attribute->value());
			break;
		}

		count++;
		attribute = attributes->next_attribute();
	}

	return strRetVal;
}

bool XML_PARSER::Is_Having_Attribute(CStdString Name)
{
	bool oRetVal(false);

	if(m_pCurrentNode)
	{
		m_pCurrentAttribute = m_pCurrentNode->first_attribute();

   		while(m_pCurrentAttribute != 0)
   		{
			if( Name == Mbcs2Unicode(m_pCurrentAttribute->name()) )
   				oRetVal = true;

			if(oRetVal) break;
   			m_pCurrentAttribute = m_pCurrentAttribute->next_attribute();
   		}
	}
	return oRetVal;
}

CStdString XML_PARSER::Get_Attribute_Value()
{  // Assume Success
   //
	lasterror = _T("");
	CStdString strRetVal;
   
	if( m_pCurrentAttribute != 0 && m_pCurrentAttribute->value() == NULL )
	{
		lasterror = _T("XML_PARSER::Get_Attribute_Value()  : Can't Retrieve an Attribute");
		return lasterror;
	}

	if(m_pCurrentAttribute != 0)
         strRetVal = Mbcs2Unicode(m_pCurrentAttribute->value());

	return strRetVal;
}

bool XML_PARSER::Add_LastChildNode(CStdString Name)
{
	xml_node<char>* pLastChild = m_Doc.allocate_node(node_element, m_Doc.allocate_string(Unicode2Mbcs(Name)));

	if( m_pCurrentNode == 0 )
		Go_to_Root();

	if( m_pCurrentNode != 0 )
		m_pCurrentNode->append_node(pLastChild);
	else
		m_Doc.append_node(pLastChild);

	m_pCurrentNode = pLastChild;

	return (m_pCurrentNode != 0);
}

int XML_PARSER::Get_Child_Count()
{
	int lCount = 0;

	if( m_pCurrentNode == 0 )
		Go_to_Root();

	xml_node<char>* first_node = m_pCurrentNode->first_node();

	for (xml_node<char> *child = first_node; child; child = child->next_sibling())
	{
		lCount++;
	}

	return lCount;
}

bool XML_PARSER::Add_FirstChildNode(CStdString Name)
{
    // If no child then use Add_LastChildNode or CurrentElement node not set yet
    //
    if(m_pCurrentNode != 0)
    {
		if( Get_Child_Count()==0 )
			return this->Add_LastChildNode(Name);
    }
    else
      return (this->Add_LastChildNode(Name));

	xml_node<char>* pFirstChild = m_Doc.allocate_node(node_element, m_Doc.allocate_string(Unicode2Mbcs(Name)));

	if( m_pCurrentNode == 0 )
		Go_to_Root();

	m_pCurrentNode->insert_node(m_pCurrentNode->first_node(), pFirstChild);
    m_pCurrentNode = pFirstChild;

	return true;
}

bool XML_PARSER::Set_Attribute(CStdString AttribName, CStdString AttribValue)
{
	if( m_pCurrentNode == 0 )
		Go_to_Root();

	for (xml_attribute<char>* attr = m_pCurrentNode->first_attribute(); attr; attr = attr->next_attribute())
	{
		if(Mbcs2Unicode(attr->name()) == AttribName)
		{
			attr->value(m_Doc.allocate_string(Unicode2Mbcs(AttribValue)));
			return true;
		}
	}

	m_pCurrentNode->append_attribute(
		m_Doc.allocate_attribute(
			m_Doc.allocate_string(Unicode2Mbcs(AttribName)), m_Doc.allocate_string(Unicode2Mbcs(AttribValue))));

    return true;
}

bool XML_PARSER::Set_TextValue(CStdString TextValue)
{
	if( m_pCurrentNode != 0 )
	{
		node_type type = m_pCurrentNode->type();

		if( type == node_element )
		{
#ifdef _UNICODE
			int	nByteRet = WideCharToMultiByte( CP_UTF8, 0, TextValue.GetBuf(), -1, NULL, 0, NULL, NULL);
			char *buffer = new char[ nByteRet + 1];
			memset(buffer,0,nByteRet + 1);
			nByteRet = WideCharToMultiByte ( CP_UTF8, 0, TextValue.GetBuf(), -1, buffer, nByteRet + 1, NULL, NULL);
			if ( nByteRet > 1)
				m_pCurrentNode->value(m_Doc.allocate_string(buffer));
			delete [] buffer;
#else
			m_pCurrentNode->value(m_Doc.allocate_string(TextValue));
#endif
		}
		else
		{
			xml_node<char>* first_node = m_pCurrentNode->first_node();

			for (xml_node<char> *child = first_node; child; child = child->next_sibling())
			{
				type = child->type();

				if( type == node_element )
				{
#ifdef _UNICODE
			int	nByteRet = WideCharToMultiByte( CP_UTF8, 0, TextValue.GetBuf(), -1, NULL, 0, NULL, NULL);
			char *buffer = new char[ nByteRet + 1];
			memset(buffer,0,nByteRet + 1);
			nByteRet = WideCharToMultiByte ( CP_UTF8, 0, TextValue.GetBuf(), -1, buffer, nByteRet + 1, NULL, NULL);
			if ( nByteRet > 1)
				m_pCurrentNode->value(m_Doc.allocate_string(buffer));
			delete [] buffer;
#else
					m_pCurrentNode->value(m_Doc.allocate_string(TextValue));
#endif
					break;
				}
			}
		}
	}

	return true;
}

bool XML_PARSER::FormatDocument()
{
	return true;
}

bool XML_PARSER::Set_Header(CStdString header, CStdString name, CStdString value)
{
	bool result = true;
	try
	{
		//<?xml version="1.0" encoding="UTF-8"?>
		xml_node<char>* xml = m_Doc.first_node();

		if( xml == 0 )
		{
			// xml declaration
			xml_node<char>* decl = m_Doc.allocate_node(node_declaration);
			decl->append_attribute(m_Doc.allocate_attribute(m_Doc.allocate_string(Unicode2Mbcs(name)), m_Doc.allocate_string(Unicode2Mbcs(value))));
			m_Doc.append_node(decl);
		}
		else
		{
			xml_attribute<char>* attr = xml->first_attribute(Unicode2Mbcs(name));
			if( attr == 0 )
				xml->append_attribute(m_Doc.allocate_attribute(m_Doc.allocate_string(Unicode2Mbcs(name)), m_Doc.allocate_string(Unicode2Mbcs(value))));
			else
				attr->value(m_Doc.allocate_string(Unicode2Mbcs(value)));
		}
	}
	catch(exception)
	{
		result = false;
	}

	return result;
}

bool XML_PARSER::Reset_XML_Document()
{
	m_Doc.remove_all_nodes();
	m_Doc.remove_all_attributes();

	return true;
}

void XML_PARSER::Go_to_Root()
{
	xml_node<char>* childNode = m_Doc.first_node();

	m_pCurrentNode = 0;

	while(childNode)
	{
		if( childNode->type() == node_element )
		{
			m_pCurrentNode = childNode;
			break;
		}

		childNode = childNode->next_sibling();
	}
}

bool XML_PARSER::Go_to_Parent()
{       // Parent node

	if( m_pCurrentNode != 0 )
		m_pCurrentNode = m_pCurrentNode->parent();
	else
		Go_to_Root();

    return true;
}

bool XML_PARSER::Go_to_Child()
{
	if( m_pCurrentNode == 0 )
		Go_to_Root();

	xml_node<char>* first_node = m_pCurrentNode->first_node();
	if(first_node)
	{
		m_pCurrentNode = first_node;
		return true;
	}

	return false;
}

bool XML_PARSER::Go_to_Child(CStdString NodeName)
{
	if( m_pCurrentNode == 0 )
		Go_to_Root();

	xml_node<char>* first_node = m_pCurrentNode->first_node();

	for (xml_node<char> *child = first_node; child; child = child->next_sibling())
	{		
		if( NodeName == Mbcs2Unicode(child->name()) )
		{
			m_pCurrentNode = child;

			return true;
		}
	}

	return false;
}

bool XML_PARSER::Go_Forward()
{
	xml_node<char>* sibling = 0;
	bool result(false);
	
	if( m_pCurrentNode == 0 )
		Go_to_Root();

	sibling = m_pCurrentNode->next_sibling();
	if(sibling)
	{
		m_pCurrentNode = sibling;
		result = true;
	}
	
    return result;
}

// Go to a Node attached at the same Node than the Current Node (Forward sens)
//
bool XML_PARSER::Go_Forward(CStdString NodeName)
{
	xml_node<char>* sibling = 0;
	bool result(false);
	
	if( m_pCurrentNode == 0 )
		Go_to_Root();

	for(sibling = m_pCurrentNode->next_sibling(); true; sibling = sibling->next_sibling())
	{
		if(sibling == 0) break;
		if(NodeName == Mbcs2Unicode(sibling->name()))
		{
			m_pCurrentNode = sibling;
			result = true;
			break;

		}
	}
	
    return result;
}

void XML_PARSER::IncTabLevel()
{
	tabLevel++;
}

void XML_PARSER::DecTabLevel()
{
	tabLevel--;
}

bool XML_PARSER::AddNewLineTab()
{	
	return true;
}

