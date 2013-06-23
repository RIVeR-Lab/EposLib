// GatewayDCStoPS.h: Schnittstelle für die Klasse CGatewayDCStoPS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYDCSTOPS_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
#define AFX_GATEWAYDCSTOPS_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Gateway/Gateway.h>

class CCommand_DCS;
class CProtocolStackManagerBase;

class CGatewayDCStoPS: public CGateway
{
public:
	BOOL InitGateway();

	CGatewayDCStoPS();
	virtual ~CGatewayDCStoPS();
	virtual CGateway* Clone();
	CGatewayDCStoPS& operator=(CGatewayDCStoPS& other);

	virtual BOOL ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);
};

#endif // !defined(AFX_GATEWAYDCSTOPS_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
