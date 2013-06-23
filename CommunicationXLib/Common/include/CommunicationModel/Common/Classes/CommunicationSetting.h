// CommunicationSetting.h: Schnittstelle f�r die Klasse CCommunicationSetting.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommunicationSetting_H__1CD3F391_4000_11D3_9D9C_00A02436C4EF__INCLUDED_)
#define AFX_CommunicationSetting_H__1CD3F391_4000_11D3_9D9C_00A02436C4EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

class CCommunicationSetting
{
public:
    void SetProtocolStackSettings(DWORD dBaudrate, DWORD dTimeout);
    void GetProtocolStackSettings(DWORD* pdBaudrate, DWORD* pdTimeout);

    void SetDeviceSetting(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName);
    void GetDeviceSetting(CStdString* pStrVirtualDeviceName, CStdString *pStrDeviceName, CStdString *pStrProtocolStackName, CStdString *pStrInterfaceName, CStdString *pStrPortName);

    BOOL operator ==(const CCommunicationSetting& m);
    CCommunicationSetting& operator=(CCommunicationSetting &other);

    CCommunicationSetting();
    virtual ~CCommunicationSetting();

protected:
    CStdString m_strVirtualDeviceName;
    CStdString m_strDeviceName;
    CStdString m_strProtocolStackName;
    CStdString m_strInterfaceName;
    CStdString m_strPortName;

    DWORD m_dBaudrate;
    DWORD m_dTimeout;
};

#endif // !defined(AFX_CommunicationSetting_H__1CD3F391_4000_11D3_9D9C_00A02436C4EF__INCLUDED_)
