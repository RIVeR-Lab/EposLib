#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <MmcTypeDefinition.h>

class CVariable
{
public:
    void SetName(CStdString p_Name);
    CStdString GetName();

    void SetType(CStdString p_Type);
    CStdString GetType();

    void SetSelectionAddress(unsigned __int64 p_ullSelectionAddress);
    unsigned __int64 GetSelectionAddress();

    CVariable(void);
    virtual ~CVariable(void);

private:
    CStdString m_Name;
    CStdString m_Type;
    unsigned __int64 m_ullSelectionAddress;
};

