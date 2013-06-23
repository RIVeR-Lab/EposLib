// GatewayVCStoDCS.cpp: Implementierung der Klasse CGatewayVCStoDCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayVCStoDCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayVCStoDCS::CGatewayVCStoDCS()
{
	m_eInputLayer = VIRTUAL_COMMAND_SET_LAYER;
	m_eOutputLayer = DEVICE_COMMAND_SET_LAYER;
}

CGatewayVCStoDCS::~CGatewayVCStoDCS()
{
}

BOOL CGatewayVCStoDCS::InitGateway()
{
	return TRUE;
}

CGateway* CGatewayVCStoDCS::Clone()
{
	return NULL;
}

CGatewayVCStoDCS& CGatewayVCStoDCS::operator=(CGatewayVCStoDCS& other)
{
	if(this != &other)
	{
		*((CGateway*)this) = *((CGateway*)&other);

	}

	return *this;
}

BOOL CGatewayVCStoDCS::ProcessCommand(CCommandRoot* pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle)
{
	return FALSE;
}
