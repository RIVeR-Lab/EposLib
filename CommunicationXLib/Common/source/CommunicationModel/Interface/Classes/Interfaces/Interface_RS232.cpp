// Interface_RS232.cpp: Implementierung der Klasse CInterface_RS232.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Interface_RS232.h"


#include "../Ports/Port_RS232.h"
#include "../Gateway/RS232/GatewayRS232.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CInterface_RS232::CInterface_RS232()
{
	Init();

    InitErrorHandling();

    m_strInterfaceName = INTERFACE_RS232;
    m_pJournalManager = NULL;
}

CInterface_RS232::CInterface_RS232(const CInterface_RS232& rObject):CInterfaceBase(rObject)
{
    InitErrorHandling();

    //JournalManager
    InitJournalManager(rObject.m_pJournalManager);
}

CInterface_RS232::~CInterface_RS232()
{
}

CInterfaceBase* CInterface_RS232::Clone()
{
    CInterfaceBase* pInterfaceBase = new CInterface_RS232(*this);
    return pInterfaceBase;
}

//********************************************************************
BOOL CInterface_RS232::InitInterface(WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    if(CInterfaceBase::InitInterface(wBoardNumber, wNbBoardWithOldDriver))
    {
        CPortBase* pPort = new CPort_RS232();
        if(pPort->InitGateway(m_pGateway) && pPort->InitPort(wBoardNumber, wNbBoardWithOldDriver))
        {
            m_PortList.push_back(pPort);
            return TRUE;
        }
        delete pPort;
    }

    return FALSE;
}

//********************************************************************
BOOL CInterface_RS232::I_OpenInterface(CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_OpenInterface(pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_OpenInterfacePort(CPortBase* pPort, CStdString strPortName, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_OpenInterfacePort(pPort, strPortName, pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_CloseInterface(CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_CloseInterface(pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_CloseInterfacePort(CPortBase* pPort, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_CloseInterfacePort(pPort, pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_SetInterfaceSettings(CPortBase* pPort, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_SetInterfaceSettings(pPort, dBaudrate, dTimeout, oChangeOnly, pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_GetInterfaceSettings(CPortBase* pPort, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_GetInterfaceSettings(pPort, pdBaudrate, pdTimeout, pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_ResetInterface(CPortBase* pPort, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_ResetInterface(pPort, pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_SetInterfaceMode(WORD wModeIndex, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_SetInterfaceMode(wModeIndex, pErrorInfo);
}

//********************************************************************
BOOL CInterface_RS232::I_GetInterfaceMode(WORD* pwModeIndex, CErrorInfo* pErrorInfo)
{
    return CInterfaceBase::I_GetInterfaceMode(pwModeIndex, pErrorInfo);
}

//********************************************************************
void CInterface_RS232::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CInterfaceBase::InitJournalManager(pJournalManager);
}

//********************************************************************
void CInterface_RS232::ResetJournalManager()
{
    CInterfaceBase::ResetJournalManager();
}

//********************************************************************
BOOL CInterface_RS232::InitGateway()
{
    DeleteGateway();

    m_pGateway = new CGatewayRS232ToWin32();
    if(m_pGateway->InitGateway())
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

BOOL CInterface_RS232::InitParameterSet()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = TRUE;
		if(oResult) oResult &= m_pGateway->ResetParameterSet();
	}
    
    return oResult;
}

//********************************************************************
BOOL CInterface_RS232::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "Interface_RS232";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(INTERFACE_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

