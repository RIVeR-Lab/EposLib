// GatewayPStoI.cpp: Implementierung der Klasse CGatewayPStoI.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayPStoI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayPStoI::CGatewayPStoI()
{
	m_eInputLayer = PROTOCOL_STACK_LAYER;
	m_eOutputLayer = INTERFACE_LAYER;
}

CGatewayPStoI::~CGatewayPStoI()
{

}

BOOL CGatewayPStoI::InitGateway()
{
	return TRUE;
}

CGateway* CGatewayPStoI::Clone()
{
	return NULL;
}

CGatewayPStoI& CGatewayPStoI::operator=(CGatewayPStoI& other)
{
	if(this != &other)
	{
		*((CGateway*)this) = *((CGateway*)&other);

	}

	return *this;
}

BOOL CGatewayPStoI::ProcessCommand(CCommandRoot* pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle)
{
	return FALSE;
}
