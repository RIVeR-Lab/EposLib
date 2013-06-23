#ifndef _MMCUSERSETIINGS_BASE
#define _MMCUSERSETIINGS_BASE

#include <list>
#include <StdString.h>

class CMmcUserSettingsBase
{
public:
	CMmcUserSettingsBase();
	CMmcUserSettingsBase(CStdString root, bool oCreateKey);
	CMmcUserSettingsBase(CMmcUserSettingsBase *pParent, CStdString root, bool oCreateKey);
	virtual ~CMmcUserSettingsBase();

public:
	CMmcUserSettingsBase*				GetParent() { return m_pParent; }
	void*								GetKey() { return m_pRootKey; }
	std::list<CMmcUserSettingsBase*>	GetChildren();

	virtual unsigned long				ReadULONG(CStdString key);
	virtual CStdString					ReadString(CStdString key);
	virtual bool						Write(CStdString key, CStdString value);
	virtual bool						Write(CStdString key, unsigned long value);
	virtual bool						Delete(CStdString key, bool deep=true);

protected:
	virtual bool						DeleteTree(void *pKey, CStdString p_SubKey, bool deep);
		
protected:
	void*			  m_pRootKey;
	CMmcUserSettingsBase* m_pParent;
};

#endif //_MMCUSERSETIINGS_BASE
