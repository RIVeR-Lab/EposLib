// GatewayPStoDCS.cpp: Implementierung der Klasse CGatewayPStoDCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayPStoDCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayPStoDCS::CGatewayPStoDCS()
{
    m_eInputLayer = PROTOCOL_STACK_LAYER;
    m_eOutputLayer = DEVICE_COMMAND_SET_LAYER;
}

CGatewayPStoDCS::~CGatewayPStoDCS()
{
}

BOOL CGatewayPStoDCS::InitGateway()
{
    return TRUE;
}

CGateway* CGatewayPStoDCS::Clone()
{
    return NULL;
}

CGatewayPStoDCS& CGatewayPStoDCS::operator=(CGatewayPStoDCS& other)
{
    if(this != &other)
    {
        *((CGateway*)this) = *((CGateway*)&other);
    }

    return *this;
}

BOOL CGatewayPStoDCS::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    return FALSE;
}

BOOL CGatewayPStoDCS::SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, DWORD p_ulTimeout)
{
    //No Device Timeout
    return TRUE;
}
