// GatewayVCStoDCS.h: Schnittstelle für die Klasse CGatewayVCStoDCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATEWAYVCSTODCS_H__1AB69440_37BB_4C95_8060_DF39FB438387__INCLUDED_)
#define AFX_GATEWAYVCSTODCS_H__1AB69440_37BB_4C95_8060_DF39FB438387__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Gateway/Gateway.h>
#include <MmcTypeDefinition.h>

class CCommandRoot;
class CLayerManagerBase;

class CGatewayVCStoDCS: public CGateway
{
public:
	CGatewayVCStoDCS();
	virtual ~CGatewayVCStoDCS();
	virtual CGateway* Clone();
	CGatewayVCStoDCS& operator=(CGatewayVCStoDCS& other);

	BOOL InitGateway();
	virtual BOOL ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

};

#endif // !defined(AFX_GATEWAYVCSTODCS_H__1AB69440_37BB_4C95_8060_DF39FB438387__INCLUDED_)
