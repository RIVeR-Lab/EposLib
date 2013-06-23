// CommunicationModel.h: Schnittstelle f�r die Klasse CCommunicationModel.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUNICATIONMODEL_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
#define AFX_COMMUNICATIONMODEL_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/VirtualDeviceCommunicationModel.h>

class CCommunicationModel: public CVirtualDeviceCommunicationModel  
{
public:
    CCommunicationModel();
    virtual ~CCommunicationModel();

public:
	BOOL CreateVirtualCommandSetManager();
};

#endif // !defined(AFX_COMMUNICATIONMODEL_H__FCB339D2_7313_49C0_B111_9CC664FAECFA__INCLUDED_)
