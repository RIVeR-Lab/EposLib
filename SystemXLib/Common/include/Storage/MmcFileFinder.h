// MmcFileFinder.h: Schnittstelle f�r die Klasse CMmcFileFinder.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MmcFileFinder_H__40AC24A4_F4F6_43CB_8034_4B3FD9C57469__INCLUDED_)
#define AFX_MmcFileFinder_H__40AC24A4_F4F6_43CB_8034_4B3FD9C57469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../MmcTypeDefinition.h"

struct SFinderOpt
{
    CStdString baseFolder;         // The starting folder for the search
    CStdString fileMask;           // File mask (e.g.: "*.txt")
	BOOL oSubfolders;        // TRUE for recursive search
};

class CMmcFileFinder
{
public:
    static void FindFiles(SFinderOpt p_opt, CStdStringArray& p_rFiles);
};

#endif // !defined(AFX_MmcFileFinder_H__40AC24A4_F4F6_43CB_8034_4B3FD9C57469__INCLUDED_)
