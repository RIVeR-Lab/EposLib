#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <MmcTypeDefinition.h>
#include <PersistenceReaderWriter/XmlReader.h>

class CVariable;

#include <list>

const CStdString VARIABLE_INFO_FILE = _T("VariableInfo.xml");

class CVariableInfoFile
{
public:
    BOOL DeleteVariableList();
    BOOL Init(CStdString p_ProjectPathName);
    BOOL GetSelectionAddress(CStdString p_VariableName, unsigned __int64* p_pullVariableSelectionAddress);

    CVariableInfoFile(void);
    virtual ~CVariableInfoFile(void);

private:
    BOOL LoadFromXmlFile(CXmlReader& p_rXmlReader);
    BOOL LoadVariableAttributes(CXmlReader& p_rXmlReader, CVariable* p_pVariable);
    BOOL SearchFile(CStdString p_SearchFolder, CStdString* p_pFilePath);

    std::list<CVariable*> m_VariableList;
};

