// Interface_USB.cpp: Implementierung der Klasse CInterface_USB.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Interface_USB.h"

#include "../Ports/Port_USB.h"
#include <CommunicationModel/Interface/Classes/Gateway/USB/GatewayUSBToFtd2xxDrv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CInterface_USB::CInterface_USB()
{
	Init();

    InitErrorHandling();

    m_strInterfaceName = INTERFACE_USB;
    m_pJournalManager = 0;
    m_pUsbDeviceInfoHandling = 0;
}

CInterface_USB::CInterface_USB(const CInterface_USB& rObject):CInterfaceBase(rObject)
{
    InitErrorHandling();

    //JournalManager
    InitJournalManager(rObject.m_pJournalManager);

    //UsbDeviceInfoHandling
    m_pUsbDeviceInfoHandling = rObject.m_pUsbDeviceInfoHandling;
}

CInterface_USB::~CInterface_USB()
{
}

CInterfaceBase* CInterface_USB::Clone()
{
    CInterfaceBase* pInterfaceBase = new CInterface_USB(*this);
    return pInterfaceBase;
}

BOOL CInterface_USB::InitInterface(WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    if(CInterfaceBase::InitInterface(wBoardNumber, wNbBoardWithOldDriver))
    {
        CPortBase* pPort = new CPort_USB();
        if(pPort->InitGateway(m_pGateway) && pPort->InitPort(wBoardNumber, wNbBoardWithOldDriver))
        {
            m_PortList.push_back(pPort);
            return TRUE;
        }
        delete pPort;
    }

    return FALSE;
}

BOOL CInterface_USB::I_OpenInterface(CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_OpenInterface(pErrorInfo);
}

BOOL CInterface_USB::I_OpenInterfacePort(CPortBase* pPort, CStdString strPortName, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_OpenInterfacePort(pPort, strPortName, pErrorInfo);
}

BOOL CInterface_USB::I_CloseInterface(CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_CloseInterface(pErrorInfo);
}

BOOL CInterface_USB::I_CloseInterfacePort(CPortBase* pPort, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_CloseInterfacePort(pPort, pErrorInfo);
}

BOOL CInterface_USB::I_SetInterfaceSettings(CPortBase* pPort, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_SetInterfaceSettings(pPort, dBaudrate, dTimeout, oChangeOnly, pErrorInfo);
}

BOOL CInterface_USB::I_GetInterfaceSettings(CPortBase* pPort, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_GetInterfaceSettings(pPort, pdBaudrate, pdTimeout, pErrorInfo);
}

BOOL CInterface_USB::I_ResetInterface(CPortBase* pPort, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_ResetInterface(pPort, pErrorInfo);
}

BOOL CInterface_USB::I_SetInterfaceMode(WORD wModeIndex, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_SetInterfaceMode(wModeIndex, pErrorInfo);
}

BOOL CInterface_USB::I_GetInterfaceMode(WORD* pwModeIndex, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_GetInterfaceMode(pwModeIndex, pErrorInfo);
}

void CInterface_USB::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CInterfaceBase::InitJournalManager(pJournalManager);
}

void CInterface_USB::ResetJournalManager()
{
    CInterfaceBase::ResetJournalManager();
}

BOOL CInterface_USB::InitGateway()
{
    CGatewayUSBToFtd2xxDrv* pGateway = 0;

    //Reset
    DeleteGateway();

    //Creaate Gateway
    pGateway = new CGatewayUSBToFtd2xxDrv();
    m_pGateway = pGateway;

    //Init Gateway
    if(m_pGateway->InitGateway())
    {
        if(pGateway->InitUsbDeviceInfoHandling(m_pUsbDeviceInfoHandling))
        {
            InitParameterSet();
			return TRUE;
        }
        else
        {
            DeleteGateway();
            return FALSE;
        }
    }
    else
    {
        DeleteGateway();
        return FALSE;
    }
}

BOOL CInterface_USB::InitParameterSet()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
	}

    return oResult;
}

BOOL CInterface_USB::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "Interface_USB";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(INTERFACE_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CInterface_USB::InitInfoHandling(CDeviceInfoHandlingBase* pUsbDeviceInfoHandling)
{
    BOOL oResult = TRUE;

    m_pUsbDeviceInfoHandling = dynamic_cast<CFtd2xxDeviceInfoHandling*>(pUsbDeviceInfoHandling);

    return oResult;
}

