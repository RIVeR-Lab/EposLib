// GatewayDCStoPS.cpp: Implementierung der Klasse CGatewayDCStoPS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayDCStoPS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayDCStoPS::CGatewayDCStoPS()
{
	m_eInputLayer = DEVICE_COMMAND_SET_LAYER;
	m_eOutputLayer = PROTOCOL_STACK_LAYER;
}

CGatewayDCStoPS::~CGatewayDCStoPS()
{

}

BOOL CGatewayDCStoPS::InitGateway()
{
	return TRUE;
}

CGateway* CGatewayDCStoPS::Clone()
{
	return NULL;
}

CGatewayDCStoPS& CGatewayDCStoPS::operator=(CGatewayDCStoPS& other)
{
	if(this != &other)
	{
		*((CGateway*)this) = *((CGateway*)&other);

	}

	return *this;
}

BOOL CGatewayDCStoPS::ProcessCommand(CCommandRoot* pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle)
{
	return FALSE;
}
