// InterfaceBase.cpp: Implementierung der Klasse CInterfaceBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <algorithm>

#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/GatewayIToDrv.h>
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>

#include "InterfaceBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEFAULT_INDEX_OF_AVAILABLE_BOARDS 0

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CInterfaceBase::CInterfaceBase()
{
	m_strClassType = _T("CInterfaceBase");

    m_lInstanceValue = 0;
	m_strInterfaceName = "";
    m_pJournalManager = 0;
    m_pGateway = 0;
    m_wModeIndex = 0;

    m_pErrorHandling = 0;
    InitErrorHandling();
}

CInterfaceBase::CInterfaceBase(const CInterfaceBase& rObject)
{
    m_strClassType = _T("CInterfaceBase");

	//Instance Value
	m_lInstanceValue = rObject.m_lInstanceValue;

	//InterfaceName
    m_strInterfaceName = rObject.m_strInterfaceName;

    //JournalManager
    m_pJournalManager = rObject.m_pJournalManager;

    //Gateway
    if(rObject.m_pGateway) m_pGateway = (CGatewayIToDrv*)rObject.m_pGateway->Clone(); else m_pGateway = 0;

    m_pErrorHandling = 0;

    InitErrorHandling();
}

CInterfaceBase::~CInterfaceBase()
{
    DeleteGateway();
    DeletePortList();
    DeleteErrorHandling();
}

bool CInterfaceBase::IsKindOf(CStdString strClassType)
{
	return (m_strClassType.CompareNoCase(strClassType) == 0);
}

CStdString CInterfaceBase::GetClassType()
{
    return m_strClassType;
}

std::list<DWORD> CInterfaceBase::GetNbOfAvailableBoards()
{
    std::list<DWORD> NbOfAvailableBoards;
    NbOfAvailableBoards.push_back(DEFAULT_INDEX_OF_AVAILABLE_BOARDS);
    return NbOfAvailableBoards;
}

BOOL CInterfaceBase::InitInstanceValue(int p_lInstanceValue)
{
	BOOL oResult = TRUE;

	m_lInstanceValue = p_lInstanceValue;

	return oResult;
}

BOOL CInterfaceBase::InitInterfaceMode(std::list<DWORD> p_rNbOfAvailableBoards, int p_Index, WORD & p_rwNbBoardWithOldDriver)
{
    BOOL oResult = TRUE;
    p_rwNbBoardWithOldDriver = 0;
    return oResult;
}

BOOL CInterfaceBase::AppendSelectionArray(CStdStringArray* pDestArray, CStdStringArray* pSourceArray)
{
    BOOL oFound;
    CStdString strDestName;
    CStdString strSourceName;

    if(pDestArray && pSourceArray)
    {
        for(std::size_t i = 0; i < pSourceArray->size(); i++)
        {
            oFound = FALSE;
            strSourceName = pSourceArray->at(i);
            for(std::size_t j = 0; j < pDestArray->size(); j++)
            {
                strDestName = pDestArray->at(j);

                if(strSourceName.CompareNoCase(strDestName) == 0)
                {
                    oFound = TRUE;
                    break;
                }
            }

            if(!oFound)
            {
                pDestArray->push_back(strSourceName);
            }
        }

        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceBase::InitErrorHandling()
{
    DeleteErrorHandling();
    m_pErrorHandling = new CErrorHandling();
    return TRUE;
}

BOOL CInterfaceBase::DeleteErrorHandling()
{
    if(m_pErrorHandling)
    {
        delete m_pErrorHandling;
        m_pErrorHandling = 0;
    }

    return TRUE;
}

CInterfaceBase* CInterfaceBase::Clone()
{
    return 0;
}

BOOL CInterfaceBase::IsEqual(CInterfaceBase* p_pInterface)
{
	BOOL oResult = FALSE;

	if(p_pInterface)
	{
		oResult = TRUE;
		if(oResult && (m_strInterfaceName != p_pInterface->m_strInterfaceName)) oResult = FALSE;
		if(oResult && (m_lInstanceValue != p_pInterface->m_lInstanceValue)) oResult = FALSE;
	}

	return oResult;
}

BOOL CInterfaceBase::InitInterface(WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    if(InitGateway())
    {
        return InitInterfaceName(wBoardNumber, wNbBoardWithOldDriver);
    }

    return FALSE;
}

BOOL CInterfaceBase::InitInfoHandling(CDeviceInfoHandlingBase* p_pInfoHandling)
{
    return TRUE;
}

BOOL CInterfaceBase::UpdateInterface(tPortList& p_rOpenPortList)
{
    BOOL oResult = TRUE;

    for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
    {
    	CPortBase* pPort = (*it);
        if(!pPort->UpdatePort(p_rOpenPortList)) oResult = FALSE;
    }

    return oResult;
}

BOOL CInterfaceBase::InitInterfaceName(WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    if(m_pGateway)
    {
        return m_pGateway->InitInterfaceName(&m_strInterfaceName, wBoardNumber, wNbBoardWithOldDriver);
    }

    return FALSE;
}

BOOL CInterfaceBase::InitGateway()
{
    return FALSE;
}

BOOL CInterfaceBase::InitBaudrateSelection(CStdDWordArray& dBaudrateSel)
{
    BOOL oResult = TRUE;

    //Init Baudrate Selection of all Ports
    for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
    {
		CPortBase* pPort = (*it);
		if(pPort)
		{
			if(!pPort->InitBaudrateSelection(dBaudrateSel))
			{
				oResult = FALSE;
			}
		}
    }

    return oResult;
}

BOOL CInterfaceBase::InitDefaultInterfaceSettings(DWORD dBaudrate, DWORD dTimeout)
{
    BOOL oResult = TRUE;

	//Init Default Settings of Interface
	if(m_pGateway)
	{
		if(!m_pGateway->InitDefaultPortSettings(dBaudrate, dTimeout)) oResult = FALSE;
	}

    //Init Baudrate Selection of all Ports
    for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
	{
    	CPortBase* pPort = (*it);
        if(pPort)
        {
            if(!pPort->InitDefaultPortSettings(dBaudrate, dTimeout))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

void CInterfaceBase::DeleteGateway()
{
    if(m_pGateway)
    {
        delete m_pGateway;
        m_pGateway = 0;
    }
}

void CInterfaceBase::DeletePortList()
{
	for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
    {
		CPortBase* pPort = (*it);
        delete pPort;
    }
    m_PortList.clear();
}

BOOL CInterfaceBase::I_GetInterfaceName(CStdString* pInterfaceName, CErrorInfo* pErrorInfo)
{
    if(pInterfaceName)
    {
        *pInterfaceName = m_strInterfaceName;
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceBase::I_GetPortName(CPortBase* pPort, CStdString* pPortName, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        return pPort->GetPortName(pPortName);
    }

    return FALSE;
}

BOOL CInterfaceBase::I_GetPortNameSelection(CStdStringArray* pPortSel, CErrorInfo* pErrorInfo)
{
    CStdStringArray portSel;

    if(pPortSel)
    {
        pPortSel->clear();
        for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
        {
        	CPortBase* pPort = (*it);
            if(pPort->GetPortNameSelection(&portSel, pErrorInfo))
            {
                AppendSelectionArray(pPortSel, &portSel);
            }
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_GetInterfaceModeSelection(CStdStringArray* pInterfaceModeSel, CErrorInfo* pErrorInfo)
{
    if(m_pGateway)
    {
        return m_pGateway->GetPortModeSelection(pInterfaceModeSel, pErrorInfo);
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_GetBaudrateSelection(CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* pErrorInfo)
{
    CPortBase* pPort = 0;

    //FindPort
    if(!I_FindPort(m_strInterfaceName, strPortName, &pPort) || (pPort == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadPortName, pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    return pPort->GetBaudrateSelection(pdBaudrateSel, pErrorInfo);
}

BOOL CInterfaceBase::I_FindInterfacePortIndex(CStdString strPortName, short* piPortIndex)
{
    CStdString strName;
    short index = 0;

    for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
    {
        CPortBase* pPort = (*it);
        if(pPort->GetInterfacePortName(&strName))
        {
            if(strPortName.CompareNoCase(strName) == 0)
            {
                if(piPortIndex) *piPortIndex = index;
            }
        }
        index++;
    }

    if(piPortIndex) *piPortIndex = -1;
    return FALSE;
}

BOOL CInterfaceBase::I_AddPort(CStdString p_PortName)
{
    return TRUE;
}

BOOL CInterfaceBase::I_DeletePort(CStdString p_PortName)
{
    return TRUE;
}

BOOL CInterfaceBase::I_FindPort(CStdString strInterfacePortName, CStdString strPortName, CPortBase** ppPort)
{
    CStdString listInterfacePortName;

    for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
    {
        CPortBase* pPort = (*it);
        if(pPort->GetInterfacePortName(&listInterfacePortName))
        {
            if(listInterfacePortName.CompareNoCase(strInterfacePortName) == 0)
            {
                if(pPort->IsPortNameSupported(strPortName))
                {
                    if(ppPort) *ppPort = pPort;
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

BOOL CInterfaceBase::I_OpenInterface(CErrorInfo* pErrorInfo)
{
    if(m_pGateway)
    {
        if(m_pGateway->OpenInterface(m_strInterfaceName, pErrorInfo))
        {
            return TRUE;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_OpenInterfacePort(CPortBase* pPort, CStdString strPortName, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        if(pPort->InitGateway(m_pGateway))
        {
            return pPort->OpenPort(strPortName, pErrorInfo);
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_CloseInterface(CErrorInfo* pErrorInfo)
{
    if(m_pGateway)
    {
        if(m_pGateway->CloseInterface(pErrorInfo))
        {
            DeleteGateway();
            return TRUE;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_CloseInterfacePort(CPortBase* pPort, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        return pPort->ClosePort(pErrorInfo);
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_SetInterfaceSettings(CPortBase* pPort, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        return pPort->SetPortSettings(dBaudrate, dTimeout, oChangeOnly, pErrorInfo);
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_GetInterfaceSettings(CPortBase* pPort, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        return pPort->GetPortSettings(pdBaudrate, pdTimeout, pErrorInfo);
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_ResetInterface(CPortBase* pPort, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        return pPort->ResetPort(pErrorInfo);
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::I_SetInterfaceMode(WORD wModeIndex, CErrorInfo* pErrorInfo)
{
    m_wModeIndex = wModeIndex;

    return TRUE;
}

BOOL CInterfaceBase::I_GetInterfaceMode(WORD* pwModeIndex, CErrorInfo* pErrorInfo)
{
    *pwModeIndex = m_wModeIndex;

    return TRUE;
}

void CInterfaceBase::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    m_pJournalManager = pJournalManager;

    for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
	{
        CPortBase* pPort = (*it);
        pPort->InitJournalManager(pJournalManager);
    }
}

void CInterfaceBase::ResetJournalManager()
{
    m_pJournalManager = 0;

    for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
    {
        CPortBase* pPort = (*it);
        pPort->ResetJournalManager();
    }
}

BOOL CInterfaceBase::I_GetDefaultInterfaceSettings(DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

    //Get Default PortSettings
	if(m_pGateway)
	{
		oResult = m_pGateway->GetDefaultPortSettings(pdBaudrate, pdTimeout, pErrorInfo);
	}

	return oResult;
}

BOOL CInterfaceBase::I_SetDefaultInterfaceSettings(DWORD dBaudrate, DWORD dTimeout, CErrorInfo* pErrorInfo)
{
    BOOL oResult = TRUE;

	//Init Default Settings of Interface
	if(m_pGateway)
	{
		if(!m_pGateway->SetDefaultPortSettings(dBaudrate, dTimeout, pErrorInfo)) oResult = FALSE;
	}

    //Init Default Settings of all Ports
	for(std::list<CPortBase*>::iterator it = m_PortList.begin(); it != m_PortList.end(); it++)
    {
    	CPortBase* pPort = (*it);
		if(pPort!=0)
        {
            if(!pPort->SetDefaultPortSettings(dBaudrate, dTimeout, pErrorInfo))
            {
                oResult = FALSE;
            }
        }
	}

    return oResult;
}

BOOL CInterfaceBase::ExecuteCommand(CCommandRoot* pCommand, CPortBase* pPort, HANDLE hTransactionHandle)
{
    if(pPort)
    {
        return pPort->ExecuteCommand(pCommand, hTransactionHandle);
    }
    return FALSE;
}

BOOL CInterfaceBase::AbortCommands(BOOL oActive)
{
    if(m_pGateway)
    {
        return m_pGateway->AbortCommands(oActive);
    }
    return TRUE;
}

BOOL CInterfaceBase::EnableTracing(CPortBase* pPort, CStdString p_FileName, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        return pPort->EnableTracing(p_FileName, pErrorInfo);
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::DisableTracing(CPortBase* pPort, CErrorInfo* pErrorInfo)
{
    if(pPort)
    {
        return pPort->DisableTracing(pErrorInfo);
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

BOOL CInterfaceBase::AreParameterEqual(CInterfaceBase* p_pInterface)
{
	BOOL oResult = FALSE;

	if(p_pInterface && m_pGateway)
	{
		oResult = m_pGateway->AreParameterEqual(p_pInterface->m_pGateway);
	}

	return oResult;
}

BOOL CInterfaceBase::SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->SetParameter(p_Name, p_pValue, p_ulSize);
	}

	return oResult;
}

BOOL CInterfaceBase::SetParameter(CStdString p_Name, CStdString p_Value)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->SetParameter(p_Name, p_Value);
	}

	return oResult;
}

BOOL CInterfaceBase::GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->GetParameter(p_Name, p_pValue, p_ulSize);
	}

	return oResult;
}

BOOL CInterfaceBase::GetParameter(CStdString p_Name, CStdString& p_rValue)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->GetParameter(p_Name, p_rValue);
	}

	return oResult;
}

BOOL CInterfaceBase::Lock(DWORD p_ulTimeout)
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->Lock(p_ulTimeout);
	}

	return oResult;
}

BOOL CInterfaceBase::Unlock()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->Unlock();
	}

	return oResult;
}

BOOL CInterfaceBase::IsLocked()
{
	BOOL oResult = FALSE;

	if(m_pGateway)
	{
		oResult = m_pGateway->IsLocked();
	}

	return oResult;
}
