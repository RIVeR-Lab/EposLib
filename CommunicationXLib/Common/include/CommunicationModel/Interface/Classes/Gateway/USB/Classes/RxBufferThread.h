#pragma once

#include <MmcTypeDefinition.h>
#include <Thread/MmcThread.h>

class CGatewayUSBToFtd2xxDrv;

class CRxBufferThread :	public CMmcThread
{
public:
	CRxBufferThread(void);
	~CRxBufferThread(void);

	void Init(CGatewayUSBToFtd2xxDrv* pGateway);

	unsigned int WINAPI ThreadFunc();

private:
	CGatewayUSBToFtd2xxDrv* m_pGateway;
};

