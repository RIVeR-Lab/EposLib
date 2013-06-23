// GatewayPStoI.h: Schnittstelle für die Klasse CGatewayPStoI.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayPStoI_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
#define AFX_GatewayPStoI_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Gateway/Gateway.h>

class CCommand_PS;
class CInterfaceManagerBase;

class CGatewayPStoI: public CGateway
{
public:
	BOOL InitGateway();

	CGatewayPStoI();
	virtual ~CGatewayPStoI();
	virtual CGateway* Clone();
	CGatewayPStoI& operator=(CGatewayPStoI& other);

	virtual BOOL ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);
};

#endif // !defined(AFX_GatewayPStoI_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
