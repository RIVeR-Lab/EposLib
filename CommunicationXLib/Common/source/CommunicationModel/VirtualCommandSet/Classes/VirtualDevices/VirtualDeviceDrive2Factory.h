#pragma once

/*DONT EDIT THIS FILE! autogenerated code by XlibFactoryBuilder 1.0.2.3*/

#include <list>
#include <StdString.h>

#include "../Gateway/Drive2/GatewayDrive2ToEpos.h"
#include "../Gateway/Drive2/GatewayDrive2ToEpos2.h"

class CVirtualDeviceDrive2Factory
{
public:
	static BOOL GetAllIdentifiers(std::list<CStdString>& p_rTargets);
	static BOOL CreateInstance(CStdString p_Name, CGatewayVCStoDCS*& p_rpInstance);
};
