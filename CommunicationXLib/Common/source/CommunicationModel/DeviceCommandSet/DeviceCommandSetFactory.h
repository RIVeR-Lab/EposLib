#pragma once

/*DONT EDIT THIS FILE! autogenerated code by XlibFactoryBuilder 1.0.2.3*/

#include <list>
#include <StdString.h>

#include "Classes/Devices/Device_Epos.h"
#include "Classes/Devices/Device_Esam.h"
#include "Classes/Devices/Device_Epos2.h"
#include "Classes/Devices/Device_Esam2.h"

class CDeviceCommandSetFactory
{
public:
	static BOOL GetAllIdentifiers(std::list<CStdString>& p_rTargets);
	static BOOL CreateInstance(CStdString p_Name, CDeviceBase*& p_rpInstance);
};
