#pragma once

/*DONT EDIT THIS FILE! autogenerated code by XlibFactoryBuilder 1.0.2.3*/

#include <list>
#include <StdString.h>

#include "../GatewayToInterface/MaxonSerialV2/GatewayMaxonSerialV2ToRS232.h"
#include "../GatewayToInterface/MaxonSerialV2/GatewayMaxonSerialV2ToUSB.h"

class CProtocolStackMaxonSerialV2Factory
{
public:
	static BOOL GetAllIdentifiers(std::list<CStdString>& p_rTargets);
	static BOOL CreateInstance(CStdString p_Name, CGatewayPStoI*& p_rpInstance);
};
