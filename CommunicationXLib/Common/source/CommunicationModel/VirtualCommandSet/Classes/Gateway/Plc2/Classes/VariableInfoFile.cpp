#include "stdafx.h"
#include "VariableInfoFile.h"

#include <list>
#include "Variable.h"

CVariableInfoFile::CVariableInfoFile(void)
{
}

CVariableInfoFile::~CVariableInfoFile(void)
{
    DeleteVariableList();
}

BOOL CVariableInfoFile::Init(CStdString p_ProjectPathName)
{
    BOOL oResult(FALSE);
    CStdString filePath(_T(""));
    CXmlReader xmlReader;

    //Search file
    if(SearchFile(p_ProjectPathName, &filePath))
    {
        //Load file
        if(xmlReader.Load(filePath))
        {
            if(LoadFromXmlFile(xmlReader))
            {
                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CVariableInfoFile::SearchFile(CStdString p_SearchFolder, CStdString* p_pFilePath)
{
    BOOL oFileSearch(FALSE);
    BOOL oFileFound(FALSE);
    CStdString tempFilePath(_T(""));
    CStdString filePath(_T(""));
    CStdString filerPath(_T(""));

#ifdef WINVER
    CFileFind fileFind;
    if(p_SearchFolder[p_SearchFolder.GetLength()-1] != TCHAR('\\'))
    {
        p_SearchFolder += TCHAR('\\');
    }

    oFileSearch = fileFind.FindFile(p_SearchFolder + "*.*");

    while(oFileSearch && !oFileFound)
    {
        oFileSearch = fileFind.FindNextFile();

        //exclude . and ..
        if(!fileFind.IsDots())
        {
            if(fileFind.IsDirectory())
            {
                oFileFound = SearchFile((LPCTSTR)fileFind.GetFilePath(), p_pFilePath);
            }
            else
            {
                filePath = fileFind.GetFilePath();
                if(filePath.Right(VARIABLE_INFO_FILE.GetLength()) == VARIABLE_INFO_FILE)
                {
                    *p_pFilePath = filePath;

                    oFileFound = TRUE;
                }
            }
        }
    }

    fileFind.Close();
#endif

    return oFileFound;
}

BOOL CVariableInfoFile::LoadFromXmlFile(CXmlReader& p_rXmlReader)
{
    BOOL oResult(FALSE);
    CVariable *pVariable(0);
    int lElementIndex(0);

    if(p_rXmlReader.ReadStartDocument())
    {
        oResult = TRUE;

        //Read All Variable Objects
        while(oResult && p_rXmlReader.ReadStartElement("Variable", lElementIndex))
        {
            pVariable = new CVariable();

            if(!LoadVariableAttributes(p_rXmlReader, pVariable))
            {
                delete pVariable;
                oResult = FALSE;
            }
            else
            {
                m_VariableList.push_back(pVariable);
            }

            p_rXmlReader.ReadEndElement();

            lElementIndex++;
        }

        p_rXmlReader.ReadEndElement();
    }

    return oResult;
}

BOOL CVariableInfoFile::LoadVariableAttributes(CXmlReader& p_rXmlReader, CVariable* p_pVariable)
{
    CStdString strValue(_T(""));
    __int64 ullSelectionAddress(0);

    if(p_pVariable)
    {
        //Name
        if(!p_rXmlReader.ReadAttributeString(_T("Name"), strValue)) strValue = _T("");
        p_pVariable->SetName(strValue);

        //Type
        if(!p_rXmlReader.ReadAttributeString(_T("Type"), strValue)) strValue = _T("");
        p_pVariable->SetType(strValue);

        //Selection address
        if(!p_rXmlReader.ReadAttributeValue(_T("SelectionAddress"), ullSelectionAddress)) ullSelectionAddress = 0;
        p_pVariable->SetSelectionAddress(ullSelectionAddress);

        return TRUE;
    }

    return FALSE;
}

BOOL CVariableInfoFile::GetSelectionAddress(CStdString p_VariableName, unsigned __int64* p_pullVariableSelectionAddress)
{
    for(std::list<CVariable*>::iterator it = m_VariableList.begin(); it != m_VariableList.end(); it++)
    {
        CVariable *pVariable = (*it);

        if(pVariable && (pVariable->GetName() == p_VariableName))
        {
            *p_pullVariableSelectionAddress = pVariable->GetSelectionAddress();
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CVariableInfoFile::DeleteVariableList()
{
    CVariable *pVariable(0);

    for(std::list<CVariable*>::iterator it = m_VariableList.begin(); it != m_VariableList.end(); it++)
    {
        CVariable *pVariable = (*it);
        if(pVariable != 0)
        {
            delete pVariable;
        }
    }

    m_VariableList.clear();

    return TRUE;
}

