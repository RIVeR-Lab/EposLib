// GatewayPStoDCS.h: Schnittstelle für die Klasse CGatewayPStoDCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GatewayPStoDCS_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
#define AFX_GatewayPStoDCS_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/Classes/Gateway/Gateway.h>

class CCommand_PS;
class CLayerManagerBase;
class CDeviceCommandSetManagerBase;

class CGatewayPStoDCS: public CGateway
{
public:
    BOOL InitGateway();

    CGatewayPStoDCS();
    virtual ~CGatewayPStoDCS();
    virtual CGateway* Clone();
    CGatewayPStoDCS& operator=(CGatewayPStoDCS& other);

    virtual BOOL ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

    virtual BOOL SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, DWORD p_ulTimeout);
};

#endif // !defined(AFX_GatewayPStoDCS_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
