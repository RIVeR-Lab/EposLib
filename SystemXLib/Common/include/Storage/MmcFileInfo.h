// FileVersionInformation.h: Schnittstelle f�r die Klasse CFileVersionInformation.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FileVersionInformation_H__40AC24A4_F4F6_43CB_8034_4B3FD9C57469__INCLUDED_)
#define AFX_FileVersionInformation_H__40AC24A4_F4F6_43CB_8034_4B3FD9C57469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <StdString.h>

class CMmcFileInfo
{
public:
    static CStdString GetComments(CStdString p_Filename);
    static CStdString GetCompanyName(CStdString p_Filename);
    static CStdString GetFileDescription(CStdString p_Filename);
    static CStdString GetFileVersion(CStdString p_Filename);
    static CStdString GetInternalName(CStdString p_Filename);
    static CStdString GetLanguageAndCodePage(CStdString p_Filename);
    static CStdString GetLegalCopyright(CStdString p_Filename);
    static CStdString GetLegalTrademarks(CStdString p_Filename);
    static CStdString GetOriginalFilename(CStdString p_Filename);
    static CStdString GetProductName(CStdString p_Filename);
    static CStdString GetProductVersion(CStdString p_Filename);
    static CStdString GetVersionInfo(CStdString p_Filename, CStdString p_strKey);

private:
    static void Initialize(CStdString p_Filename);
    static void Cleanup();

private:
    static char* m_pData;
	static unsigned int m_refCount;
};

#endif // !defined(AFX_FileVersionInformation_H__40AC24A4_F4F6_43CB_8034_4B3FD9C57469__INCLUDED_)
