#if !defined(AFX_ProtocolStackSettings_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_)
#define AFX_ProtocolStackSettings_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProtocolStackSettings.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CProtocolStackSettings
#include <MmcTypeDefinition.h>

class CProtocolStackSettings
{
public:
	BOOL operator==(const CProtocolStackSettings& data) const;
	CProtocolStackSettings& operator=(CProtocolStackSettings& other);
	CProtocolStackSettings(const CProtocolStackSettings& other);
	void Reset();

	CProtocolStackSettings();
	virtual ~CProtocolStackSettings();

	void Init(DWORD dBaudrate,DWORD dTimeout);

	DWORD GetBaudrate();
	DWORD GetTimeout();

	CStdString GetBaudrateString();

private:
	DWORD m_dBaudrate;
	DWORD m_dTimeout;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // AFX_ProtocolStackSettings_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_
