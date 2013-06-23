#if !defined(XML_PARSER_H)
#define XML_PARSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <StdString.h>
#include <Xml/rapidxml.hpp>

class XML_PARSER
{
public:
   XML_PARSER();                                                  
   virtual ~XML_PARSER();                                         

   //Document Functions
   bool		Load_XML_Document(CStdString strFileName);						// Load an XML Document from File
   bool		Load_XML_From_Buffer(CStdStringA* pSource);
   bool		Load_XML_From_Buffer(char* p_pBuffer, size_t p_BufferSize);
   bool		Reset_XML_Document();											// Clear the XML Document
   bool		Get_XML_Document(CStdString* pBstrXml);                         // Get the XML Representation of the entire document

   //Header
   bool		Set_Header(CStdString header,CStdString name,CStdString value); // Set an header property like <? xml version="1.0"?>

   //Elements
   bool		Add_FirstChildNode(CStdString Name);							// Add a new CHILD NODE at the Begining of Current Node
   bool		Add_LastChildNode(CStdString Name);								// Add a new CHILD NODE at the End of Current Node
   CStdString Get_TextValue();												// Get the Text Value when it's a TEXT or CDATA node
   CStdString Get_CurrentTag();												// Get the Current Tag value  (with "<>")
   CStdString Get_CurrentTagName();											// Get the Current Tag value  (without "<>")
   bool		Set_TextValue(CStdString TextValue);							// Set a Text to the Current Node

   //Element Navigation
   bool		Go_to_Parent();													// Go to the parent node of the current node
   int		Get_Child_Count();												// Return the number of childs for the current node
   bool		Go_to_Child();													// Go to a Direct Child Node 
   bool		Go_Forward();													// Go to a next Node at the same Node than the Current
   bool		Go_to_Child(CStdString NodeName);								// Go to a Direct Child Node
   bool		Go_Forward(CStdString NodeName);								// Go to a next Node at the same Node than the Current
   void		Go_to_Root();													// Go to the Root node
   bool		Go_to_Node(CStdString XPathNode);							    // Got to Node defined by XPath 
   bool		Is_Root();														// Test if a Tag is the Root
   bool		Is_Tag(CStdString aTag);										// Return true if it's equal to the Current Tag

   //Attributes
   bool		Set_Attribute(CStdString AttribName,CStdString AttribValue);    // Set an attribute to the Current Node
   bool		Is_Having_Attribute(CStdString Name);							// Return true if current node have the specified attribute defined
   CStdString Get_Attribute_Value(int index);								// Return the attribute value selected by "Is_Having_Attribute()"
   CStdString Get_Attribute_Value();										// Return the attribute value selected by "Is_Having_Attribute()"
   int		Get_Attribute_Count();		                                    // Return the number of attributes for the current node

   //Helper Functions
   bool		Clone_Node(XML_PARSER* pSourceDocument, CStdString XPathNode,int p_lNodeIndex,rapidxml::xml_node<char>** p_pClonedNode);	// Clone Node
   bool		Copy_Node(XML_PARSER* pSourceDocument,CStdString strXPathSourceNode,int p_lSourceNodeIndex,CStdString strXPathTarget);
   bool		Remove_Node(CStdString XPathNode);
   bool		Remove_Node();
   bool		Remove_Child(CStdString NodeName);
   bool     Remove_Childs(CStdString NodeName);
   rapidxml::xml_node<char>* Clone_Node(const rapidxml::xml_node<char>* pSource);
   bool		FormatDocument();
   CStdString GetXml();
   
   //Tabbing
   void		IncTabLevel();
   void		DecTabLevel();
   bool		AddNewLineTab();

   //Parsing Callback
   virtual void Parse_XML_Document();										// ===> YOU MUST IMPLEMENT IT FOR PARSING PROCESS <===

private:
	void CheckError();
	bool Append_Child(CStdString strXPathNode, rapidxml::xml_node<char>* pChildNode);
	rapidxml::xml_node<char>* SelectSingleNode(CStdString strXPathNode, int p_lNodeIndex);
	void	Parse_Objects(rapidxml::xml_node<char>* pObject);
	
private:
	rapidxml::xml_document<char> m_Doc;
	rapidxml::xml_node<char>*	 m_pCurrentNode;
	rapidxml::xml_attribute<char>*	 m_pCurrentAttribute;

	// Error Status String
	CStdString lasterror;

	//Tabbing
	int tabLevel;
	
};

#endif

