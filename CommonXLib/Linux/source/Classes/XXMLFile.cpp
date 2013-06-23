// XXMLFile.cpp: implementation of the CXXMLFile class.
//
//////////////////////////////////////////////////////////////////////
#include <Classes/XXMLFile.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//************************************************************************//
/*!  \fn CXXMLFile::CXXMLFile()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Constructor
 */
CXXMLFile::CXXMLFile()
{
	// The root is null
	m_Root=NULL;

	// clean tables, and create a default root node
	RemoveAll();

	// set default symbols and open tags
	DefaultSymbols();
}


//************************************************************************//
/*!  \fn CXXMLFile::~CXXMLFile()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Destructor
 */
CXXMLFile::~CXXMLFile()
{
	// clean up root (and then, the full tree)
	if(m_Root!=NULL) delete m_Root;
}

//************************************************************************//
/*!  \fn CXXMLFile::CElement::~CElement()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Element destructor (clean up the list of elements)
 */
CXXMLFile::CElement::~CElement(){
	// You must clean up the possible list of elements
//	for(std::list::iterator it=this->begin(); it!=this->end(); it++)
//		delete (*it);
};

void CXXMLFile::CElement::SetAt(CStdString tag, CStdString value)
{
	AttributeToValue[tag] = value;
}

//************************************************************************//
/*!  \fn bool IsSeparator(char ch){
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Ask for an XML separator
 */
static bool IsSeparator(char ch){
	switch (ch){
	case ' ': return true;
	case '\t': return true;
	case '\r': return true;
	case '\n': return true;
	default: return false;
	};
};

//************************************************************************//
/*!  \fn bool HopSeparators(CStdString &html, int &pos,int FileRow)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief For internal use (Read()). It jumps over separators in a string by
 * incrementing an integer pointer.
 */
static bool HopSeparators(CStdString &html, int &pos,int FileRow){
	while( (pos<html.GetLength()) && (IsSeparator(html.GetAt(pos))) ){
		if(html.GetAt(pos)=='\n')
			FileRow++;
		pos++;
	}
	if(pos>=html.GetLength()) return false; else return true;
};

//************************************************************************//
/*!  \fn int CountChars(CStdString s,char ch)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief A brute-force character counter.
 */
static int CountChars(CStdString s,char ch){
	return s.Replace(ch,' ');
};

//************************************************************************//
/*!  \fn int FindChars(CStdString &html, int pos, LPCTSTR chars)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief For internal use (Read()). It secuentially look for any char in
 * a given set from an starting pointer in a string.
 */
static int FindChars(CStdString &html, int pos, CStdString chars){
	int tmp=pos;
	CStdString seps = chars;
	while(tmp<html.GetLength()) {
		CStdString chars;
		chars += html.GetAt(tmp);
		if(seps.FindOneOf( chars )!=-1){
			return tmp;
		}
		tmp++;
	}
	return -1;
};


//************************************************************************//
/*!  \fn bool CXXMLFile::Read()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief The read XML function.
 */
bool CXXMLFile::Read()
{
	CStdString strErrorText = "";
	CStdString filename = m_Filename;
    CStdString text = "";
	int FileRow = 1;

	// Init Tree
	if(m_Root!=NULL) delete m_Root;
	CElementPart** crut = &m_Root;
	CElement* Element = new CElement();
	Element->m_Parent=(*crut);
	(*crut)=Element;
	((CElement*)(*crut))->m_Text = "?root?";

	// Clean up errors
	m_ErrorList.clear();

	// A brute-force reading of a text file (the most faster)
	CStdString html;
	try
	{
		std::ifstream f;

		f.open(filename);

		if( !f.is_open() )
			throw;

		html.Empty();

		char * buf = html.GetBufferSetLength((int)f.gcount());

		f >> buf;

		html.ReleaseBuffer();
		html.FreeExtra();
		html.Replace("\r\n","\n");
	}
	catch(...)
	{
		m_ErrorList.push_back("Error: File not found.");
		return false;
	}

	int p1,p2,p3,p4;
	p1=p2=p3=p4=0;
	while(true){
		if(p1>=html.GetLength()) return true;
		p2 = html.Find('<',p1);
		if(p2==-1){
			strErrorText.Format("Warning at line %i: There's some text before EOF, ignoring.",FileRow);
			m_ErrorList.push_back(strErrorText);
			return true;
		}
		text = html.Mid(p1,p2-p1);
		Decodify(text);
		if(!text.IsEmpty()){
			if( (*crut)==NULL ){
				strErrorText.Format("Warning at line %i: No tag active but text found, must be at the start, ignoring.",FileRow);
				m_ErrorList.push_back(strErrorText);
			} else {
				CText * t = new CText();
				t->m_Text = text;
				((CElement*)(*crut))->push_back( t );
				FileRow+=CountChars(text,'\n');
			}
		}

		// tag part
		p1=p2+1;
		if(p1>=html.GetLength()){
			strErrorText.Format("Error at line %i: Tag started buf EOF found.",FileRow);
			m_ErrorList.push_back(strErrorText);
			return false;
		} else
		if(html.Mid(p1,3)=="!--"){ // comment
			p1=p1+3;
			p2 = html.Find("-->",p1);
			text = html.Mid(p1,p2-p1);
			if(p2==-1){
				strErrorText.Format("Error at line %i: Comment tag unclosed.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			if( (*crut)==NULL ){
				strErrorText.Format("Warning at line %i: No tag active but text found, ignoring.",FileRow);
				m_ErrorList.push_back(strErrorText);
			} else {
				CComment * t = new CComment();
				t->m_Text = text;
				((CElement*)(*crut))->push_back( t );
				FileRow+=CountChars(text,'\n');
			}
			p1=p2+=3;
		} else
		if(html.GetAt(p1)=='?'){ // ?xml or something to avoid
			p2 = html.Find("?>",p1+1);
			if(p2==-1){
				strErrorText.Format("Error at line %i: Tag <? unclosed.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			} else {
				p1=p2+2;
				continue;
			}
		} else
		if(html.GetAt(p1)=='/'){ // an end tag
			p2 = html.Find('>',p1);
			if(p2==-1){
				strErrorText.Format("Error at line %i: Tag unclosed.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			p1++;
			CStdString tagname = html.Mid(p1,p2-p1);
			tagname.TrimLeft();
			tagname.TrimRight();
			if( (*crut)==NULL ){
				strErrorText.Format("Error at line %i: Closing tag when no tag???.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			if( (*crut)->m_Text!=tagname ){
				strErrorText.Format("Error at line %i: Closing tag differs from open tag.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			(*crut)=(*crut)->m_Parent;
			p1=p2+1;
		} else { // a start tag
			// < _ tag ...
			if(!HopSeparators(html,p1,FileRow)){
				strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			// < tag _ ...
			p2=FindChars(html,p1," \t\r\n>");
			if(p2==-1){
				strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			CStdString tag = html.Mid(p1,p2-p1);
			Element = new CElement();
			Element->m_Parent=(*crut);
			((CElement*)(*crut))->push_back( ((CElementPart*)Element) );
			(*crut)=Element;
			((CElement*)(*crut))->m_Text = tag;
			p1=p2;
			while(true){
				// _ >...
				// _ value="...
				if(!HopSeparators(html,p1,FileRow)){
					strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
					m_ErrorList.push_back(strErrorText);
					return false;
				}
				if(html.GetAt(p1)=='>'){
					// _ >...
					p1+=1;
					tag.MakeLower(); // tag string will be no longer used
					CStdString value;

					tMmcStringToStringMap::iterator it = m_OpenTags.find(tag);
					if(it!=m_OpenTags.end())
					{
						value = (*it).second;
						// It's an open tag, that means that it will never be closed
						// Eg: <br> in HTML
						(*crut)=(*crut)->m_Parent; // closing node
					}
					break;
				};
				if(html.Mid(p1,2)=="/>"){
					// _ />...
					// This kind of tag means that no close tag is expected.
					p1+=2;
					(*crut)=(*crut)->m_Parent; // collapse node
					break;
				};
				// _ value="...
				p2=html.Find("=\"",p1);
				if(p2==-1){
					strErrorText.Format("Error at line %i: Unspected value form.",FileRow);
					m_ErrorList.push_back(strErrorText);
					return false;
				}
				CStdString valname = html.Mid(p1,p2-p1);
				valname.TrimLeft();
				valname.TrimRight();
				p1=p2+2;
				CStdString value;
				while(true){
					if(p1>=html.GetLength()){
						strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
						m_ErrorList.push_back(strErrorText);
						return false;
					} else
					if(html.GetAt(p1)=='\"'){
						p1++;
						break;
					} else
					if(html.Mid(p1,2)=="\\\""){
						value+="\"";
						p1+=2;
					} else {
						value+=html.GetAt(p1);
						p1++;
					}
				}
				Element->AttributeToValue[valname]=value;
			}
		}
	}
}

//************************************************************************//
/*!  \fn bool CXXMLFile::ReadFromString(CString* pDataStr)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief The read XML function.
 */
bool CXXMLFile::ReadFromString(CStdString* pDataStr)
{
	CStdString strErrorText = "";
	CStdString filename = m_Filename;
    CStdString text = "";
	int FileRow = 1;

	// Init Tree
	if(m_Root!=NULL) delete m_Root;
	CElementPart** crut = &m_Root;
	CElement* Element = new CElement();
	Element->m_Parent=(*crut);
	(*crut)=Element;
	((CElement*)(*crut))->m_Text = "?root?";

	// Clean up errors
	m_ErrorList.clear();

	// A brute-force reading of a text file (the most faster)
	CStdString html;
	try
	{
		html.Empty();
		if(pDataStr) html = *pDataStr;
		html.Replace("\r\n","\n");
	}
	catch(...)
	{
		m_ErrorList.push_back("Error: File not found.");
		return false;
	}

	int p1,p2,p3,p4;
	p1=p2=p3=p4=0;
	while(true){
		if(p1>=html.GetLength()) return true;
		p2 = html.Find('<',p1);
		if(p2==-1){
			strErrorText.Format("Warning at line %i: There's some text before EOF, ignoring.",FileRow);
			m_ErrorList.push_back(strErrorText);
			return true;
		}
		text = html.Mid(p1,p2-p1);
		Decodify(text);
		if(!text.IsEmpty()){
			if( (*crut)==NULL ){
				strErrorText.Format("Warning at line %i: No tag active but text found, must be at the start, ignoring.",FileRow);
				m_ErrorList.push_back(strErrorText);
			} else {
				CText * t = new CText();
				t->m_Text = text;
				((CElement*)(*crut))->push_back( t );
				FileRow+=CountChars(text,'\n');
			}
		}

		// tag part
		p1=p2+1;
		if(p1>=html.GetLength()){
			strErrorText.Format("Error at line %i: Tag started buf EOF found.",FileRow);
			m_ErrorList.push_back(strErrorText);
			return false;
		} else
		if(html.Mid(p1,3)=="!--"){ // comment
			p1=p1+3;
			p2 = html.Find("-->",p1);
			text = html.Mid(p1,p2-p1);
			if(p2==-1){
				strErrorText.Format("Error at line %i: Comment tag unclosed.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			if( (*crut)==NULL ){
				strErrorText.Format("Warning at line %i: No tag active but text found, ignoring.",FileRow);
				m_ErrorList.push_back(strErrorText);
			} else {
				CComment * t = new CComment();
				t->m_Text = text;
				((CElement*)(*crut))->push_back( t );
				FileRow+=CountChars(text,'\n');
			}
			p1=p2+=3;
		} else
		if(html.GetAt(p1)=='?'){ // ?xml or something to avoid
			p2 = html.Find("?>",p1+1);
			if(p2==-1){
				strErrorText.Format("Error at line %i: Tag <? unclosed.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			} else {
				p1=p2+2;
				continue;
			}
		} else
		if(html.GetAt(p1)=='/'){ // an end tag
			p2 = html.Find('>',p1);
			if(p2==-1){
				strErrorText.Format("Error at line %i: Tag unclosed.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			p1++;
			CStdString tagname = html.Mid(p1,p2-p1);
			tagname.TrimLeft();
			tagname.TrimRight();
			if( (*crut)==NULL ){
				strErrorText.Format("Error at line %i: Closing tag when no tag???.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			if( (*crut)->m_Text!=tagname ){
				strErrorText.Format("Error at line %i: Closing tag differs from open tag.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			(*crut)=(*crut)->m_Parent;
			p1=p2+1;
		} else { // a start tag
			// < _ tag ...
			if(!HopSeparators(html,p1,FileRow)){
				strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			// < tag _ ...
			p2=FindChars(html,p1," \t\r\n>");
			if(p2==-1){
				strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
				m_ErrorList.push_back(strErrorText);
				return false;
			}
			CStdString tag = html.Mid(p1,p2-p1);
			Element = new CElement();
			Element->m_Parent=(*crut);
			((CElement*)(*crut))->push_back( ((CElementPart*)Element) );
			(*crut)=Element;
			((CElement*)(*crut))->m_Text = tag;
			p1=p2;
			while(true){
				// _ >...
				// _ value="...
				if(!HopSeparators(html,p1,FileRow)){
					strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
					m_ErrorList.push_back(strErrorText);
					return false;
				}
				if(html.GetAt(p1)=='>'){
					// _ >...
					p1+=1;
					tag.MakeLower(); // tag string will be no longer used
					CStdString value;
					tMmcStringToStringMap::iterator it = m_OpenTags.find(tag);
					if(it!=m_OpenTags.end())
					{
						value = (*it).second;
						// It's an open tag, that means that it will never be closed
						// Eg: <br> in HTML
						(*crut)=(*crut)->m_Parent; // closing node
					}
					break;
				};
				if(html.Mid(p1,2)=="/>"){
					// _ />...
					// This kind of tag means that no close tag is expected.
					p1+=2;
					(*crut)=(*crut)->m_Parent; // collapse node
					break;
				};
				// _ value="...
				p2=html.Find("=\"",p1);
				if(p2==-1){
					strErrorText.Format("Error at line %i: Unspected value form.",FileRow);
					m_ErrorList.push_back(strErrorText);
					return false;
				}
				CStdString valname = html.Mid(p1,p2-p1);
				valname.TrimLeft();
				valname.TrimRight();
				p1=p2+2;
				CStdString value;
				while(true){
					if(p1>=html.GetLength()){
						strErrorText.Format("Error at line %i: Unspected EOF.",FileRow);
						m_ErrorList.push_back(strErrorText);
						return false;
					} else
					if(html.GetAt(p1)=='\"'){
						p1++;
						break;
					} else
					if(html.Mid(p1,2)=="\\\""){
						value+="\"";
						p1+=2;
					} else {
						value+=html.GetAt(p1);
						p1++;
					}
				}
				Element->AttributeToValue[valname]=value;
			}
		}
	}
}


//************************************************************************//
/*!  \fn void CXXMLFile::SetFile(CStdString filename)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Set XML filename.
 */
void CXXMLFile::SetFile(CStdString filename)
{
	m_Filename=filename;
}


//************************************************************************//
/*!  \fn CStdString CXXMLFile::GetFile()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Get XML filename.
 */
CStdString CXXMLFile::GetFile()
{
	return m_Filename;
}


//************************************************************************//
/*!  \fn void CXXMLFile::WritePart(fstream *f, CXXMLFile::CElementPart * p, int Depth, bool bNoIdent){
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Writes an XML node to a file (used by Write only).
 *
 *   \param f The target file.
 *   \param p The tree node.
 *   \param Depth The node depth (for identation).
 *   \param bNoIdent Boolean value used to cancel identation if the node is alone,
 * that is, without children and siblings.
 */
void CXXMLFile::WritePart(fstream *f,  CXXMLFile::CElementPart * p, int Depth, bool bNoIdent)
{
	int j;
	if(p->m_Type==CElementPart::TElement)
	{
		{// Write identation
			long pos = f->tellp();
			// Avoid line-feed at the begining of the file
			if(pos!=0)
				*f << "\n";
			// the Identation with spaces
			for(j=0;j<Depth;j++)
				*f << " ";
		}
		// Write tag header
		*f << "<" << p->m_Text;

		// Write values
		for(tMmcStringToStringMap::iterator it=((CElement*)p)->AttributeToValue.begin();
			it!=((CElement*)p)->AttributeToValue.end(); it++)
		{
			CStdString AtName = (*it).first;
			CStdString AtValue = (*it).second;

			*f << " " + AtName + "=\"" + AtValue + "\"";
		}

		// If the Element does not have subelements, sub-texts, or sub-comments
		// write it closed
		if(((CElement*)p)->size()==0)
		{
			// Is an open tag (like HTML <br>)?
			CStdString tag;
			tag = p->m_Text;
			tag.MakeLower();
			CStdString value;
			tMmcStringToStringMap::iterator it = m_OpenTags.find(tag);
			if(it!=m_OpenTags.end())
			{
				value = (*it).second;
				// It's an open tag, that means that it will never be closed
				// Eg: <br> in HTML
				*f << ">";
			} else {
				// Not an open tag
				*f << "/>";
			}
		} else {
			*f << ">";

			// Optimization to block ident on single subtext in element.
			bool NoIdent = false;
			if(((CElement*)p)->size()==1)
				NoIdent=true;

			// For each element...
			for(std::list<CElementPart*>::iterator it=((CElement*)p)->begin();
				it!=((CElement*)p)->end(); it++)
			{
				CElementPart* e = (*it);
				WritePart(f,e,Depth+1,NoIdent);
			}

			// Optimization to block ident on single subtext in element.
			if(!NoIdent){
				{// Write identation
					// Avoid line-feed at the begining of the file
					long pos = f->tellp();
					if(pos!=0)
						*f << "\n";
					// the Identation with spaces
					for(j=0;j<Depth;j++)
						*f << " ";
				}
			}

			// Close tag
			*f << "</" << p->m_Text << ">";
		}
	} else
	if(p->m_Type==CElementPart::TComment){
		// Write comment AS-IS
		{// Write identation
			// Avoid line-feed at the begining of the file
			long pos=f->tellp();
			if(pos!=0)
				*f << "\n";
			// the Identation with spaces
			for(j=0;j<Depth;j++)
				*f << " ";
		}
		*f << "<!--" << p->m_Text << "-->";
	} else
	if(p->m_Type==CElementPart::TText){
		// Write text if it's not empty (an empty text can have sparators)
		CStdString empty_string = p->m_Text;
		empty_string.Replace('\n',' ');
		empty_string.Replace('\r',' ');
		empty_string.Replace('\t',' ');
		while(0!=empty_string.Replace("  "," "));
		if((!empty_string.IsEmpty())&&(empty_string!=" "))
		{
			if(!bNoIdent){
				{// Write identation
					// Avoid line-feed at the begining of the file
					long pos=f->tellp();
					if(pos!=0)
						*f << "\n";
					// the Identation with spaces
					for(j=0;j<Depth;j++)
						*f << " ";
				}
			}

			// The string must be written with symbol codification
			CStdString text = p->m_Text;
			Codify(text);
			*f << text;
		}
	}
};


//************************************************************************//
/*!  \fn void CXXMLFile::WritePartToString(CString* pDataStr, CXXMLFile::CElementPart * p, int Depth, bool bNoIdent){
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Writes an XML node to a string (used by Write only).
 *
 *   \param pDataStr string
 *   \param p The tree node.
 *   \param Depth The node depth (for identation).
 *   \param bNoIdent Boolean value used to cancel identation if the node is alone,
 * that is, without children and siblings.
 */
void CXXMLFile::WritePartToString(CStdString* pDataStr, CElementPart * p, int Depth, bool bNoIdent){
	int j;

	if(pDataStr)
	{
		if(p->m_Type==CElementPart::TElement){
			{// Write identation
				// Avoid line-feed at the begining of the file
				if(!pDataStr->IsEmpty())
					*pDataStr += "\n";
				// the Identation with spaces
				for(j=0;j<Depth;j++)
					*pDataStr += " ";
			}
			// Write tag header
			*pDataStr += "<" + p->m_Text;

			// Write values
			for(tMmcStringToStringMap::iterator it=((CElement*)p)->AttributeToValue.begin();
			it!=((CElement*)p)->AttributeToValue.end(); it++)
			{
				CStdString AtName = (*it).first;
				CStdString AtValue = (*it).second;

				*pDataStr += " " + AtName + "=\"" + AtValue + "\"";
			}

			// If the Element does not have subelements, sub-texts, or sub-comments
			// write it closed
			if(((CElement*)p)->size()==0) {
				// Is an open tag (like HTML <br>)?
				CStdString tag;
				tag = p->m_Text;
				tag.MakeLower();
				CStdString value;
				tMmcStringToStringMap::iterator it=m_OpenTags.find(tag);
				if(it!=m_OpenTags.end())
				{
					value = (*it).second;
					// It's an open tag, that means that it will never be closed
					// Eg: <br> in HTML
					*pDataStr += ">";
				} else {
					// Not an open tag
					*pDataStr += "/>";
				}
			} else {
				*pDataStr += ">";

				// Optimization to block ident on single subtext in element.
				bool NoIdent = false;
				if(((CElement*)p)->size()==1)
					NoIdent=true;

				// For each element...
				for(std::list<CElementPart*>::iterator it=((CElement*)p)->begin();
					it!=((CElement*)p)->end(); it++)
				{
					CElementPart*e = (*it);
					WritePartToString(pDataStr,e,Depth+1,NoIdent);
				}

				// Optimization to block ident on single subtext in element.
				if(!NoIdent){
					{// Write identation
						// Avoid line-feed at the begining of the file
						if(!pDataStr->IsEmpty())
							*pDataStr += "\n";
						// the Identation with spaces
						for(j=0;j<Depth;j++)
							*pDataStr += " ";
					}
				}

				// Close tag
				*pDataStr += "</" + p->m_Text + ">";
			}
		} else
		if(p->m_Type==CElementPart::TComment){
			// Write comment AS-IS
			{// Write identation
				// Avoid line-feed at the begining of the file
				if(!pDataStr->IsEmpty())
					*pDataStr += "\n";
				// the Identation with spaces
				for(j=0;j<Depth;j++)
					*pDataStr += " ";
			}
			*pDataStr += "<!--" + p->m_Text + "-->";
		} else
		if(p->m_Type==CElementPart::TText){
			// Write text if it's not empty (an empty text can have sparators)
			CStdString empty_string = p->m_Text;
			empty_string.Replace('\n',' ');
			empty_string.Replace('\r',' ');
			empty_string.Replace('\t',' ');
			while(0!=empty_string.Replace("  "," "));
			if((!empty_string.IsEmpty())&&(empty_string!=" ")) {
				if(!bNoIdent){
					{// Write identation
						// Avoid line-feed at the begining of the file
						if(!pDataStr->IsEmpty())
							*pDataStr += "\n";
						// the Identation with spaces
						for(j=0;j<Depth;j++)
							*pDataStr += " ";
					}
				}

				// The string must be written with symbol codification
				CStdString text = p->m_Text;
				Codify(text);
				*pDataStr += text;
			}
		}
	}
};


//************************************************************************//
/*!  \fn bool CXXMLFile::Write()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Writes XML file.
 */
bool CXXMLFile::Write()
{
	m_ErrorList.clear();

	// check tree
	if(m_Root==NULL) {
		m_ErrorList.push_back("Error: NULL tree.");
		return false;
	}
	if(m_Root->m_Type!=CElementPart::TElement) {
		m_ErrorList.push_back("Error: tree root is not an Element.");
		return false; // root must be TElement
	}
	if(((CElement*)m_Root)->m_Text!="?root?") {
		m_ErrorList.push_back("Error: tree root is not named ?root?");
		return false; // bad-written root
	}

	// Ok write file;
	fstream f;

	f.open(m_Filename,ios::in|ios::out);

	if(!f.is_open()){
		m_ErrorList.push_back("Error: cannot open '"+m_Filename+"' for writing.");
		return false;
	}

	CElement * root = ((CElement*)m_Root);
	for(std::list<CElementPart*>::iterator it=root->begin(); it!=root->end(); it++)
	{
		CElementPart * p = (*it);
		WritePart(&f,p,0);
	}

	f.close();

	return true;
}

//************************************************************************//
/*!  \fn bool CXXMLFile::WriteToString(CString* pDataStr)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Writes XML file.
 */
bool CXXMLFile::WriteToString(CStdString* pDataStr)
{
	m_ErrorList.clear();

	if(pDataStr)
	{
		// check tree
		if(m_Root==NULL) {
			m_ErrorList.push_back("Error: NULL tree.");
			return false;
		}
		if(m_Root->m_Type!=CElementPart::TElement) {
			m_ErrorList.push_back("Error: tree root is not an Element.");
			return false; // root must be TElement
		}
		if(((CElement*)m_Root)->m_Text!="?root?") {
			m_ErrorList.push_back("Error: tree root is not named ?root?");
			return false; // bad-written root
		}

		// Ok write string;
		pDataStr->Empty();
		CElement * root = ((CElement*)m_Root);

		for(std::list<CElementPart*>::iterator it=root->begin(); it!=root->end(); it++)
		{
			CElementPart* p = (*it);
			WritePartToString(pDataStr,p,0);
		}

		return true;
	}

	return false;
}


//************************************************************************//
/*!  \fn void CXXMLFile::DefaultSymbols()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Set default symbols.
 */
void CXXMLFile::DefaultSymbols()
{
	AddSymbol("&lt;","<");
	AddSymbol("&gt;",">");
	AddSymbol("&quot;","\"");
	AddSymbol("&nbsp;"," ");
	AddSymbol("&apos;","'");
	AddSymbol("&amp;","&");
	m_OpenTags["br"];
}


//************************************************************************//
/*!  \fn void CXXMLFile::ClearSymbols()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Clear symbols.
 */
void CXXMLFile::ClearSymbols()
{
	// Clear symbol table
	m_Symbols.clear();

	// Remove open tags
	m_OpenTags.clear();
}


//************************************************************************//
/*!  \fn void CXXMLFile::AddSymbol(CStdString coded, CStdString decoded)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Adds a symbol.
 */
void CXXMLFile::AddSymbol(CStdString coded, CStdString decoded)
{
	m_Symbols[coded]=decoded;
}


//************************************************************************//
/*!  \fn void CXXMLFile::RemoveAll()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Delete all entries and make a default root node.
 */
void CXXMLFile::RemoveAll()
{
	// Clear symbol table
	ClearSymbols();

	// Remove open tags
	m_OpenTags.clear();

	// Clear tree and create the default main node
	if(m_Root!=NULL) delete m_Root;
	CElementPart** crut = &m_Root;
	CElement* Element = new CElement();
	Element->m_Parent=(*crut);
	(*crut)=Element;
	((CElement*)(*crut))->m_Text = "?root?";
}


//************************************************************************//
/*!  \fn void CXXMLFile::Codify(CStdString &html)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Codify using symbol table.
 */
void CXXMLFile::Codify(CStdString &html)
{
	int pos = 0;
	while(pos<html.GetLength())
	{
		for(tMmcStringToStringMap::iterator it=m_Symbols.begin();it!=m_Symbols.end();it++)
		{
			CStdString coded = (*it).first;
			CStdString decoded = (*it).second;

			if((pos+decoded.GetLength())<=html.GetLength()){
				if(html.Mid(pos,decoded.GetLength())==decoded){
					html = html.Left(pos) + coded + html.Mid(pos+decoded.GetLength());
					pos = pos + coded.GetLength()-1;
					break;
				}
			}
		}
		pos++;
	}
}


//************************************************************************//
/*!  \fn void CXXMLFile::Decodify(CStdString &html)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Decodify using symbol table.
 */
void CXXMLFile::Decodify(CStdString &html)
{
	int pos = 0;
	while(pos<html.GetLength())
	{
		for(tMmcStringToStringMap::iterator it=m_Symbols.begin();it!=m_Symbols.end();it++)
		{
			CStdString coded = (*it).first;
			CStdString decoded = (*it).second;
			if((pos+coded.GetLength())<=html.GetLength()){
				if(html.Mid(pos,coded.GetLength())==coded){
					html = html.Left(pos) + decoded + html.Mid(pos+coded.GetLength());
					pos = pos + decoded.GetLength()-1;
					break;
				}
			}
		}
		pos++;
	}
}

//************************************************************************//
/*!  \fn void CXXMLFile::AddOpenTag(CStdString tag)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Adds an open tag. (HTML <br>)
 */
void CXXMLFile::AddOpenTag(CStdString tag){
	tag.MakeLower();
	m_OpenTags[tag];
}

//************************************************************************//
/*!  \fn CXXMLFile::CElementPart* CXXMLFile::Root()
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Gets the root. (Create one if it's empty).
 */
CXXMLFile::CElementPart* CXXMLFile::Root(){
	if(m_Root!=NULL)
		return (CXXMLFile::CElementPart*)m_Root;
	CElementPart** crut = &m_Root;
	CElement* Element = new CElement();
	Element->m_Parent=(*crut);
	(*crut)=Element;
	((CElement*)(*crut))->m_Text = "?root?";
	return Element;
}

//************************************************************************//
/*!  \fn CXXMLFile::CElementPart* CXXMLFile::AddElement(CElementPart* Parent)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Adds a node type element
 */
CXXMLFile::CElementPart* CXXMLFile::AddElement(CElementPart* Parent){
	// Only TElement nodes support childs
	if(Parent->m_Type!=CXXMLFile::CElementPart::TElement)
		return NULL;
	CXXMLFile::CElement * elem = (CXXMLFile::CElement*)Parent;
	CXXMLFile::CElement * new_elem = new CElement();
	elem->push_back( ((CXXMLFile::CElementPart*)new_elem) );
	return new_elem;
}

//************************************************************************//
/*!  \fn BOOL CXXMLFile::DeleteElement(CElementPart* parent,CElementPart* element)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Adds a node type element
 */
int CXXMLFile::DeleteElement(CElementPart* parent,CElementPart* element)
{
	CXXMLFile::CElementPart* pElementPart;
	CXXMLFile::CElement* pElementParent;

	if(parent && element)
	{
		if(parent->m_Type!=CXXMLFile::CElementPart::TElement) return 0;
		pElementParent = (CXXMLFile::CElement*)parent;

		for(std::list<CElementPart*>::iterator it=pElementParent->begin(); it!=pElementParent->end(); it++)
		{
			pElementPart = (*it);
			if(pElementPart)
			{
				if(pElementPart == element)
				{
					delete pElementPart;
					pElementParent->erase(it);
					return 1;
				}
			}
		}
	}

	return 0;
}

//************************************************************************//
/*!  \fn void CXXMLFile::SetText(CElementPart* node, CStdString text)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Sets text property in a node.
 */
void CXXMLFile::SetText(CElementPart* node, CStdString text){
	node->m_Text = text;
};

//************************************************************************//
/*!  \fn void CXXMLFile::GetText(CElementPart* node, CStdString &text)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Gets text property in a node.
 */
void CXXMLFile::GetText(CElementPart* node, CStdString &text){
	text = node->m_Text;
};

//************************************************************************//
/*!  \fn CXXMLFile::CElementPart* CXXMLFile::AddComment(CElementPart* Parent, CStdString text)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Adds a comment node
 */
CXXMLFile::CElementPart* CXXMLFile::AddComment(CElementPart* Parent, CStdString text){
	// Only TElement nodes support childs
	if(Parent->m_Type!=CXXMLFile::CElementPart::TElement)
		return NULL;
	CXXMLFile::CElement * elem = (CXXMLFile::CElement*)Parent;
	CXXMLFile::CComment * new_elem = new CComment();
	elem->push_back( ((CXXMLFile::CElementPart*)new_elem) );
	return new_elem;
};

//************************************************************************//
/*!  \fn CXXMLFile::AddHeader(CElementPart* pElementPart, CXXMLFile* file, CStdString text)
 *   \author Daniel Lussi
 *   \date 28/11/2005
 *   \brief Adds a XML Header
 */
void CXXMLFile::AddHeader(CElementPart* pElementPart, CStdString text){
	CStdString str;
	str.Format("%s%s%s","?",text,"?");
	pElementPart->m_Text = str;
};

//************************************************************************//
/*!  \fn CXXMLFile::CElementPart* CXXMLFile::AddText(CElementPart* Parent, CStdString text)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Adds a text node
 */
CXXMLFile::CElementPart* CXXMLFile::AddText(CElementPart* Parent, CStdString text){
	// Only TElement nodes support childs
	if(Parent->m_Type!=CXXMLFile::CElementPart::TElement)
		return NULL;
	CXXMLFile::CElement * elem = (CXXMLFile::CElement*)Parent;
	CXXMLFile::CText * new_elem = new CText();
	elem->push_back( ((CXXMLFile::CElementPart*)new_elem) );
	return new_elem;
}

//************************************************************************//
/*!  \fn bool CXXMLFile::IsElement(CElementPart* node)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief  Determines wheter node is element.
 */
bool CXXMLFile::IsElement(CElementPart* node)
{
	return (node->m_Type==CXXMLFile::CElementPart::TElement);
}

//************************************************************************//
/*!  \fn bool CXXMLFile::IsComment(CElementPart* node)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Determines wheter node is comment.
 */
bool CXXMLFile::IsComment(CElementPart* node)
{
	return (node->m_Type==CXXMLFile::CElementPart::TComment);
}

//************************************************************************//
/*!  \fn bool CXXMLFile::IsText(CElementPart* node)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Determines wheter node is text.
 */
bool CXXMLFile::IsText(CElementPart* node)
{
	return (node->m_Type==CXXMLFile::CElementPart::TText);
}

//************************************************************************//
/*!  \fn CMapStringToString* CXXMLFile::GetElementAttrMap(CElementPart* node)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Returns a pointer to the attribute map of the element.
 */
tMmcStringToStringMap* CXXMLFile::GetElementAttrMap(CElementPart* node)
{
	// Only TElement nodes support childs
	if(node->m_Type!=CXXMLFile::CElementPart::TElement)
		return NULL;
	CXXMLFile::CElement * elem = (CXXMLFile::CElement*)node;
	return &elem->AttributeToValue;
}

//************************************************************************//
/*!  \fn bool CXXMLFile::BuildChildList(CElementPart* node, CList<CElementPart*,CElementPart*> &l)
 *   \author Manuel Lucas Vi�as Livschitz (MLVL)
 *   \date 04/05/2002
 *   \brief Builds a list of child nodes.
 */
bool CXXMLFile::BuildChildList(CElementPart* node, std::list<CElementPart*> &l)
{
	// Only TElement nodes support childs
	if(node->m_Type!=CXXMLFile::CElementPart::TElement)
		return false;
	l.clear();
	l.push_back( ((CElementPart*)node) );
	return true;
}
