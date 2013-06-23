#include "stdafx.h"
#include <CommunicationModel/Interface/Classes/Gateway/USB/Classes/Ftd2xxDeviceInfo.h>

#include <MmcTypeDefinition.h>

CFtd2xxDeviceInfo::CFtd2xxDeviceInfo(void):
	m_ulLocationId(0),
	m_SerialNumber(_T("")),
	m_ulDrvFlags(0),
	m_PortName(_T("")),
	m_ulPluggedIn(0),
	m_ulOpenedByProcess(0),
	m_ulOpenedByInstance(0),
	m_OpenedBy(_T(""))
{
}

CFtd2xxDeviceInfo::~CFtd2xxDeviceInfo(void)
{
}

CFtd2xxDeviceInfo* CFtd2xxDeviceInfo::Clone()
{
	CFtd2xxDeviceInfo* pDeviceInfo = 0;

	pDeviceInfo = new CFtd2xxDeviceInfo();
	*pDeviceInfo = *this;

	return pDeviceInfo;
}

CFtd2xxDeviceInfo& CFtd2xxDeviceInfo::operator=(CFtd2xxDeviceInfo& other)
{
	if(this != &other)
	{
		m_ulLocationId = other.m_ulLocationId;
		m_SerialNumber = other.m_SerialNumber;
		m_ulDrvFlags = other.m_ulDrvFlags;
		m_PortName = other.m_PortName;
		m_ulPluggedIn = other.m_ulPluggedIn;
		m_ulOpenedByProcess = other.m_ulOpenedByProcess;
		m_ulOpenedByInstance = other.m_ulOpenedByInstance;
		m_OpenedBy = other.m_OpenedBy;
	}

	return *this;
}

BOOL CFtd2xxDeviceInfo::Init(DWORD p_ulLocId, CStdString p_SerialNumber, DWORD p_ulFlags)
{
	BOOL oResult = TRUE;

	m_ulLocationId = p_ulLocId;
	m_SerialNumber = p_SerialNumber;
	m_ulDrvFlags = p_ulFlags;

	return oResult;
}

BOOL CFtd2xxDeviceInfo::Init(CFtd2xxDeviceInfo& p_rDeviceInfo)
{
	BOOL oResult = TRUE;

	m_ulLocationId = p_rDeviceInfo.m_ulLocationId;
	m_SerialNumber = p_rDeviceInfo.m_SerialNumber;

	return oResult;
}

BOOL CFtd2xxDeviceInfo::Update(CFtd2xxDeviceInfo& p_rDeviceInfo)
{
	BOOL oResult = TRUE;

	if(p_rDeviceInfo.m_ulLocationId != 0) m_ulLocationId = p_rDeviceInfo.m_ulLocationId;
	if(!p_rDeviceInfo.m_SerialNumber.size()==0) m_SerialNumber = p_rDeviceInfo.m_SerialNumber;

	return oResult;
}

BOOL CFtd2xxDeviceInfo::IsEqual(CFtd2xxDeviceInfo& p_rDeviceInfo)
{
	BOOL oResult = FALSE;

	if((m_ulLocationId == 0) || (p_rDeviceInfo.m_ulLocationId == 0))
	{
		//Check SerialNumber, if Location ID is not available
		if(m_SerialNumber == p_rDeviceInfo.m_SerialNumber)
		{
			oResult = TRUE;
		}
	}
	else
	{
		//Check Location ID
		if(m_ulLocationId == p_rDeviceInfo.m_ulLocationId)
		{
		 oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfo::IsLocIdEqual(CFtd2xxDeviceInfo& p_rDeviceInfo)
{
	BOOL oResult = FALSE;

	if(m_ulLocationId == p_rDeviceInfo.m_ulLocationId)
	{
		oResult = TRUE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfo::IsSerialNumberEqual(CFtd2xxDeviceInfo& p_rDeviceInfo)
{
	BOOL oResult = FALSE;

	if(m_SerialNumber == p_rDeviceInfo.m_SerialNumber)
	{
		oResult = TRUE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfo::IsPortNameEqual(CStdString p_PortName)
{
	BOOL oResult = FALSE;

	if(m_PortName == p_PortName)
	{
		oResult = TRUE;
	}

	return oResult;
}

BOOL CFtd2xxDeviceInfo::IsPluggedIn()
{
	return (m_ulPluggedIn > 0);
}

BOOL CFtd2xxDeviceInfo::IsOpened()
{
	return (m_ulOpenedByProcess > 0) && (m_ulOpenedByInstance > 0);
}

BOOL CFtd2xxDeviceInfo::IsOpenedBy(DWORD p_ulProcessId, DWORD p_ulInstanceValue)
{
	return ((m_ulOpenedByProcess == p_ulProcessId) && (m_ulOpenedByInstance == p_ulInstanceValue));
}

BOOL CFtd2xxDeviceInfo::SetPortName(CStdString p_PortName)
{
	BOOL oResult = TRUE;

	m_PortName = p_PortName;

	return oResult;
}

BOOL CFtd2xxDeviceInfo::SetOpenedByProcess(DWORD p_ulOpenedByProcess)
{
	m_ulOpenedByProcess = p_ulOpenedByProcess;
	return TRUE;
}

BOOL CFtd2xxDeviceInfo::SetOpenedByInstance(DWORD p_ulOpenedByInstance)
{
	m_ulOpenedByInstance = p_ulOpenedByInstance;
	return TRUE;
}

BOOL CFtd2xxDeviceInfo::SetOpenedBy(CStdString p_OpenedBy)
{
	m_OpenedBy = p_OpenedBy;
	return TRUE;
}

BOOL CFtd2xxDeviceInfo::SetPluggedIn(DWORD p_ulPluggedIn)
{
	m_ulPluggedIn = p_ulPluggedIn;
	return TRUE;
}

DWORD CFtd2xxDeviceInfo::GetOpenedByProcess()
{
	return m_ulOpenedByProcess;
}

DWORD CFtd2xxDeviceInfo::GetOpenedByInstance()
{
	return m_ulOpenedByInstance;
}

CStdString CFtd2xxDeviceInfo::GetOpenedBy()
{
	return m_OpenedBy;
}

DWORD CFtd2xxDeviceInfo::GetPluggedIn()
{
	return m_ulPluggedIn;
}

BOOL CFtd2xxDeviceInfo::GetPortName(CStdString& p_rPortName)
{
	BOOL oResult = TRUE;

	p_rPortName = m_PortName;

	return oResult;
}

DWORD CFtd2xxDeviceInfo::GetLocId()
{
	return m_ulLocationId;
}

DWORD CFtd2xxDeviceInfo::GetDrvFlags()
{
	return m_ulDrvFlags;
}

CStdString CFtd2xxDeviceInfo::GetSerialNumber()
{
	return m_SerialNumber;
}

CStdString CFtd2xxDeviceInfo::GetPortName()
{
	return m_PortName;
}

