// FileVersionInformation.cpp: Implementierung der Klasse CFileVersionInformation.
//
//////////////////////////////////////////////////////////////////////
#include <Storage/MmcFileInfo.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
char* CMmcFileInfo::m_pData = 0;
unsigned int CMmcFileInfo::m_refCount = 0;

void CMmcFileInfo::Initialize(CStdString p_Filename)
{
    if( m_refCount == 0 )
	{
		//TODO
	}
	m_refCount++;
}

void CMmcFileInfo::Cleanup()
{
	m_refCount--;

    if(m_pData && m_refCount == 0)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
}

CStdString CMmcFileInfo::GetVersionInfo(CStdString p_Filename, CStdString p_strKey)
{
    Initialize(p_Filename);

    //TODO
    CStdString strInfo = p_strKey;

	Cleanup();

    return strInfo;
}

CStdString CMmcFileInfo::GetLanguageAndCodePage(CStdString p_Filename)
{
    Initialize(p_Filename);

    //TODO
    CStdString strLang = "EN";

	Cleanup();

    return strLang;
}

CStdString CMmcFileInfo::GetCompanyName(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "CompanyName");
}

CStdString CMmcFileInfo::GetFileVersion(CStdString p_Filename)
{
	Initialize(p_Filename);

	//TODO
	CStdString strFileVersion = "1.0.0.1";

	Cleanup();

    return strFileVersion;
}

CStdString CMmcFileInfo::GetInternalName(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "InternalName");
}

CStdString CMmcFileInfo::GetOriginalFilename(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "OriginalFilename");
}

CStdString CMmcFileInfo::GetProductName(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "ProductName");
}

CStdString CMmcFileInfo::GetProductVersion(CStdString p_Filename)
{
	Initialize(p_Filename);

    //TODO
	CStdString strProductVersion = "1.0.0.1";

	Cleanup();

    return strProductVersion;
}

CStdString CMmcFileInfo::GetLegalCopyright(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "LegalCopyright");
}

CStdString CMmcFileInfo::GetFileDescription(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "FileDescription");
}

CStdString CMmcFileInfo::GetComments(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "Comments");
}

CStdString CMmcFileInfo::GetLegalTrademarks(CStdString p_Filename)
{
	return GetVersionInfo(p_Filename, "LegalTrademarks");
}

