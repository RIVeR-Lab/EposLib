#include "stdafx.h"
#include "Variable.h"


CVariable::CVariable(void)
: m_Name(_T(""))
, m_Type(_T(""))
, m_ullSelectionAddress(0)
{
}

CVariable::~CVariable(void)
{
}

void CVariable::SetName(CStdString p_Name)
{
    m_Name = p_Name;
}

CStdString CVariable::GetName()
{
    return m_Name;
}

void CVariable::SetType(CStdString p_Type)
{
    m_Type = p_Type;
}

CStdString CVariable::GetType()
{
    return m_Type;
}

void CVariable::SetSelectionAddress(unsigned __int64 p_ullSelectionAddress)
{
    m_ullSelectionAddress = p_ullSelectionAddress;
}

unsigned __int64 CVariable::GetSelectionAddress()
{
    return m_ullSelectionAddress;
}

