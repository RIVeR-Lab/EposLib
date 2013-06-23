#include <Storage/MmcUserSettingsBase.h>
#include <MmcTypeDefinition.h>

CMmcUserSettingsBase::CMmcUserSettingsBase()
{
}

/*
 *
 */
CMmcUserSettingsBase::CMmcUserSettingsBase(CStdString root, bool oCreateKey)
{
}

/*
 *
 */
CMmcUserSettingsBase::CMmcUserSettingsBase(CMmcUserSettingsBase *pParent, CStdString root, bool oCreateKey)
{
	m_pParent = pParent;
}	

/*
 *
 */
CMmcUserSettingsBase::~CMmcUserSettingsBase()
{
}

/*
 *
 */
std::list<CMmcUserSettingsBase*> CMmcUserSettingsBase::GetChildren()
{
	std::list<CMmcUserSettingsBase*>	children;

	return children;
}

/*
 *
 */
unsigned long CMmcUserSettingsBase::ReadULONG(CStdString p_ValueName)
{	
	unsigned long p_rValue = 0;

	return p_rValue;
}

/*
 *
 */
CStdString CMmcUserSettingsBase::ReadString(CStdString p_ValueName)
{
	CStdString p_rValue;

	return p_rValue;
}

/*
 *
 */
bool CMmcUserSettingsBase::Write(CStdString p_ValueName, CStdString p_Value)
{
	return true;
}

/*
 *
 */
bool CMmcUserSettingsBase::Write(CStdString p_ValueName, unsigned long p_Value)
{
	return true;
}

/*
 *
 */
bool CMmcUserSettingsBase::Delete(CStdString p_SubKey, bool deep/*=true*/)
{
	return DeleteTree(m_pRootKey,p_SubKey, deep);
}

bool CMmcUserSettingsBase::DeleteTree(void *pKey,CStdString p_SubKey, bool deep)
{
	return true;
}
