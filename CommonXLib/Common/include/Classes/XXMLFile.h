// XXMLFile.h: interface for the CXXMLFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXMLFILE_H__F5E3CD25_0B84_4191_A1A7_B3669180DFFA__INCLUDED_)
#define AFX_XXMLFILE_H__F5E3CD25_0B84_4191_A1A7_B3669180DFFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>
#include <StdString.h>
#include <fstream>

typedef std::map<CStdString,CStdString> tMmcStringToStringMap;

//************************************************************************//
/*!  \class CXXMLFile
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief A class to work with XML files as tree collections.
 */
class CXXMLFile  {
public:
	CXXMLFile();
	virtual ~CXXMLFile();
public:
	// Abstract node
	class CElementPart;
	class CElementPart{
	public:
		CElementPart* m_Parent;
		CStdString m_Text;
		enum TType { TElement, TText, TComment } m_Type;
	public:
		CElementPart(){};
		virtual ~CElementPart(){};
	};

	// Element node
	class CElement : public CElementPart, public std::list<CElementPart*>
	{
	public:
		CElement(){ m_Type=TElement; };
		virtual ~CElement();
		void SetAt(CStdString tag, CStdString value);
	public:
		tMmcStringToStringMap AttributeToValue;
	};

	// Text node
	class CText : public CElementPart{
	public:
		CText(){ m_Type=TText; };
	};

	// Comment node
	class CComment : public CElementPart{
	public:
		CComment(){ m_Type=TComment; };
	};
public:
	// The error list, after executing Write() or Read() (if any)
	std::list<CStdString> m_ErrorList;

	// Symbol table functions
	void RemoveAll();
	void AddSymbol(CStdString coded, CStdString decoded);
	void DefaultSymbols();
	void ClearSymbols();
	void AddOpenTag(CStdString tag);

	//File manipulation routines
	bool WriteToString(CStdString* pDataStr);
	bool ReadFromString(CStdString* pDataStr);

	// File manipulation routines
	bool Write();
	CStdString GetFile();
	void SetFile(CStdString filename);
	bool Read();

	// Tree manipulation routines
	CElementPart* Root();
	CElementPart* AddElement(CElementPart* Parent);
	int DeleteElement(CElementPart* parent,CElementPart* element);
	CElementPart* AddComment(CElementPart* Parent, CStdString text);
	CElementPart* AddText(CElementPart* Parent, CStdString text);
	void AddHeader(CElementPart* pElementPart, CStdString text);
	void SetText(CElementPart* node, CStdString text);
	void GetText(CElementPart* node, CStdString &text);
	bool IsElement(CElementPart* node);
	bool IsComment(CElementPart* node);
	bool IsText(CElementPart* node);
	tMmcStringToStringMap* GetElementAttrMap(CElementPart* node);
	bool BuildChildList(CElementPart* node, std::list<CElementPart*> &l);
private:
	// The XML tree
	CElementPart* m_Root;

	// Symbol codification functions
	void Decodify(CStdString &html);
	void Codify(CStdString &html);
	tMmcStringToStringMap m_Symbols; // encoded to decoded

	// Part management
	void WritePart(std::fstream *f, CElementPart * p, int Depth, bool bNoIdent=false);
	void WritePartToString(CStdString* pDataStr, CElementPart * p, int Depth, bool bNoIdent=false);

	// Filename
	CStdString m_Filename;

	// Open tags map (acts like a set, so second value is ignored)
	tMmcStringToStringMap m_OpenTags;
};

#endif // !defined(AFX_XXMLFILE_H__F5E3CD25_0B84_4191_A1A7_B3669180DFFA__INCLUDED_)
