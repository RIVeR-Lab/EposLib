#pragma once

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

class CDeviceInfoHandlingBase
{
public:
	CDeviceInfoHandlingBase(void);
	CDeviceInfoHandlingBase(int p_lInstanceValue);
    virtual ~CDeviceInfoHandlingBase();

protected:
    long m_lInstanceValue;
};