#include "stdafx.h"
#include <CommunicationModel/Interface/Classes/Gateway/USB/Classes/RxBufferThread.h>
#include <unistd.h>
#include <CommunicationModel/Interface/Classes/Gateway/USB/GatewayUSBToFtd2xxDrv.h>

CRxBufferThread::CRxBufferThread(void)
{
	m_pGateway = 0;
}

CRxBufferThread::~CRxBufferThread(void)
{
}

void CRxBufferThread::Init(CGatewayUSBToFtd2xxDrv* pGateway)
{
	m_pGateway = pGateway;
}

unsigned int CRxBufferThread::ThreadFunc()
{
	DWORD dTimeout = 1;
#ifdef WINVER
	//Initialize COM
	if(SUCCEEDED(CoInitializeEx(0, COINIT_MULTITHREADED)))
	{
#endif
		while(m_Running)
		{
			if(m_pGateway)
			{
				if(m_pGateway->WaitForRxData(dTimeout))
				{
					m_pGateway->UpdateRxBuffer();
				}
			}

#ifdef WINVER
			Sleep(1);
#else
			usleep(10);
#endif

		}
#ifdef WINVER
		//Unitialize COM
		CoUninitialize();

	}
#endif
	m_Running = false;

	return (DWORD)-1;
}

