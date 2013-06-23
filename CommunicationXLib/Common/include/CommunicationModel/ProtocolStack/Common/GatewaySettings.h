#if !defined(AFX_GatewaySETTINGS_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_)
#define AFX_GatewaySETTINGS_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GatewaySettings.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CGatewaySettings
#include <MmcTypeDefinition.h>

class CGatewaySettings
{
public:
    CGatewaySettings();
    virtual ~CGatewaySettings();

    void Init(CStdString p_GatewayName, CStdString p_GatewayProtocolStackName);
    CStdString GetGatewayName();
    CStdString GetGatewayProtocolStackName();

    BOOL operator==(const CGatewaySettings& data) const;
    CGatewaySettings& operator=(CGatewaySettings& other);
    CGatewaySettings(const CGatewaySettings& other);
    void Reset();

private:
    CStdString m_GatewayName;
    CStdString m_GatewayProtocolStackName;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_GatewaySETTINGS_H__00073834_9C0D_4CF7_94AC_E1217FFAA377__INCLUDED_
