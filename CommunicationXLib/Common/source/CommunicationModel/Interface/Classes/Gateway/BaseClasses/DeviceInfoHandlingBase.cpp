#include "stdafx.h"
#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/DeviceInfoHandlingBase.h>

CDeviceInfoHandlingBase::CDeviceInfoHandlingBase()
    : m_lInstanceValue(0)
{

}

CDeviceInfoHandlingBase::CDeviceInfoHandlingBase(int p_lInstanceValue) :
	m_lInstanceValue(p_lInstanceValue)

{
	
}

CDeviceInfoHandlingBase::~CDeviceInfoHandlingBase()
{

}