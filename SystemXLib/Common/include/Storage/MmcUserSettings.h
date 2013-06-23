#ifndef _MMCUSERSETIINGS
#define _MMCUSERSETIINGS

#include <list>
#include <Storage/MmcUserSettingsBase.h>

class CMmcUserSettings : public CMmcUserSettingsBase
{
public:
	CMmcUserSettings();
	CMmcUserSettings(CStdString root, bool oCreateKey);
	CMmcUserSettings(CMmcUserSettings *pParent, CStdString root, bool oCreateKey);
	~CMmcUserSettings();

	std::list<CMmcUserSettings*>	GetChildren();

public:
	unsigned long	ReadULONG(CStdString key);
	CStdString		ReadString(CStdString key);
	bool			Write(CStdString key, CStdString value);
	bool			Write(CStdString key, unsigned long value);
	bool			Delete(CStdString key, bool deep=true);
	bool			DeleteTree(void *pKey, CStdString p_SubKey, bool deep);

#ifndef _WIN32
private:
	unsigned long m_nLocationId;
	unsigned long m_nPluggedIn;
	unsigned long m_nOpenedByProcess;
	unsigned long m_nOpenedByInstance;
	CStdString 	  m_strDefault;
	CStdString 	  m_strSerialNumber;
	CStdString 	  m_strOpenedBy;
#endif
};

#endif //_MMCUSERSETIINGS
