#include <Storage/MmcUserSettings.h>
#include <MmcTypeDefinition.h>

CMmcUserSettings::CMmcUserSettings()
{
	m_nLocationId = 0;
	m_nPluggedIn = 0;
	m_nOpenedByProcess = 0;
	m_nOpenedByInstance = 0;
}

CMmcUserSettings::CMmcUserSettings(CStdString root, bool oCreateKey)
: CMmcUserSettingsBase(root, oCreateKey)
{
}

CMmcUserSettings::CMmcUserSettings(CMmcUserSettings *pParent, CStdString root, bool oCreateKey)
: CMmcUserSettingsBase(pParent, root, oCreateKey)
{
}

/*
 *
 */
CMmcUserSettings::~CMmcUserSettings()
{
}

/*
 *
 */
std::list<CMmcUserSettings*> CMmcUserSettings::GetChildren()
{
	std::list<CMmcUserSettings*>	children;

	for( unsigned long nUsbPortId=0; nUsbPortId<1; nUsbPortId++)
	{
		CMmcUserSettings* pUsb0 = new CMmcUserSettings();

		pUsb0->m_nLocationId = 0;
		pUsb0->m_nPluggedIn = 0;
		pUsb0->m_nOpenedByProcess = 0;
		pUsb0->m_nOpenedByInstance = 0;

		pUsb0->m_strDefault.Format("USB%ld", nUsbPortId);
		pUsb0->m_strSerialNumber = "";
		pUsb0->m_strOpenedBy="DAWS";

		children.push_back(pUsb0);
	}

	return children;
}

/*
 *
 */
unsigned long CMmcUserSettings::ReadULONG(CStdString p_ValueName)
{	
	unsigned long p_rValue = 0;
	
	if(p_ValueName.CompareNoCase("LocationId")==0)
		return m_nLocationId;
	else if(p_ValueName.CompareNoCase("PluggedIn")==0)
		return m_nPluggedIn;
	else if(p_ValueName.CompareNoCase("OpenedByProcess")==0)
		return m_nOpenedByProcess;
	else if(p_ValueName.CompareNoCase("OpenedByInstance")==0)
		return m_nOpenedByInstance;

	return p_rValue;
}

/*
 *
 */
CStdString CMmcUserSettings::ReadString(CStdString p_ValueName)
{
	CStdString p_rValue;

	if( p_ValueName.IsEmpty() )
		return m_strDefault;
	else if(p_ValueName.CompareNoCase("SerialNumber")==0)
		return m_strSerialNumber;
	else if(p_ValueName.CompareNoCase("OpenedBy")==0)
		return m_strOpenedBy;

	return p_rValue;
}

bool  CMmcUserSettings::Write(CStdString key, CStdString value) {return true;}
bool  CMmcUserSettings::Write(CStdString key, unsigned long value) {return true;}
bool  CMmcUserSettings::Delete(CStdString key, bool deep) {return true;}
bool  CMmcUserSettings::DeleteTree(void *pKey, CStdString p_SubKey, bool deep) {return true;}
