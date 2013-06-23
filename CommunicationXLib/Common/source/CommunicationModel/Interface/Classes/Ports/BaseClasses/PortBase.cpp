// PortBase.cpp: Implementierung der Klasse CPortBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/BaseClasses/Command_I.h>
#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/GatewayIToDrv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CPortBase::CPortBase()
{
    m_strInterfacePortName = _T("");
    m_strPortName = _T("");

    m_pJournalManager = 0;
    m_pGateway = 0;

    m_pErrorHandling = 0;
    InitErrorHandling();
}

CPortBase::CPortBase(const CPortBase& p_rObject)
{
    //Names
    m_strInterfacePortName = p_rObject.m_strInterfacePortName;
    m_strPortName = p_rObject.m_strPortName;

    //JournalManager
    m_pJournalManager = p_rObject.m_pJournalManager;

    //Gateway
    if(p_rObject.m_pGateway) m_pGateway = (CGatewayIToDrv*)p_rObject.m_pGateway->Clone(); else m_pGateway = 0;

    m_pErrorHandling = 0;
    InitErrorHandling();
}

CPortBase::~CPortBase()
{
    DeleteGateway();
    DeleteErrorHandling();
}

BOOL CPortBase::InitErrorHandling()
{
    BOOL oResult = TRUE;

    DeleteErrorHandling();
    m_pErrorHandling = new CErrorHandling();

    return oResult;
}

BOOL CPortBase::DeleteErrorHandling()
{
    BOOL oResult = TRUE;

    if(m_pErrorHandling)
    {
        delete m_pErrorHandling;
        m_pErrorHandling = 0;
    }

    return oResult;
}

CPortBase* CPortBase::Clone()
{
    return 0;
}

BOOL CPortBase::IsEqual(CStdString p_PortName)
{
    BOOL oResult = FALSE;

    oResult = TRUE;
    if(oResult && (m_strPortName != p_PortName)) oResult = FALSE;

    return oResult;
}

BOOL CPortBase::InitGateway(CGatewayIToDrv* p_pGateway)
{
    BOOL oResult = FALSE;

    DeleteGateway();
    if(p_pGateway)
    {
        m_pGateway = (CGatewayIToDrv*)p_pGateway->Clone();
        oResult = TRUE;
    }

    return oResult;
}

BOOL CPortBase::InitBaudrateSelection(CStdDWordArray& p_rulBaudrateSel)
{
    BOOL oResult = FALSE;

    //Init Baudrate Selection of Gateway
    if(m_pGateway)
    {
        oResult = m_pGateway->InitBaudrateSelection(p_rulBaudrateSel);
    }

    return oResult;
}

BOOL CPortBase::InitDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout)
{
    BOOL oResult = FALSE;

    //Init Baudrate Selection of Gateway
    if(m_pGateway)
    {
        oResult = m_pGateway->InitDefaultPortSettings(p_ulBaudrate, p_ulTimeout);
    }

    return oResult;
}

BOOL CPortBase::IsPortNameSupported(CStdString p_PortName)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->IsPortNameSupported(p_PortName);
    }

    return oResult;
}

void CPortBase::DeleteGateway()
{
    if(m_pGateway)
    {
        delete m_pGateway;
        m_pGateway = 0;
    }
}

//Bsp.:Der InterfacePortName kann RS232, IXXAT, Vector sein
BOOL CPortBase::GetInterfacePortName(CStdString* p_pInterfacePortName)
{
    BOOL oResult = FALSE;

    if(p_pInterfacePortName)
    {
        *p_pInterfacePortName = m_strInterfacePortName;
        oResult = TRUE;
    }

    return oResult;
}

//Bsp.:Der PortName kann COM1, COM2, CAN1... sein
BOOL CPortBase::GetPortName(CStdString* p_pPortName)
{
    BOOL oResult = FALSE;

    if(p_pPortName)
    {
        *p_pPortName = m_strPortName;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CPortBase::SetPortName(CStdString p_PortName)
{
    BOOL oResult = TRUE;

    m_strPortName = p_PortName;

    return oResult;
}

void CPortBase::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    m_pJournalManager = pJournalManager;
}

void CPortBase::ResetJournalManager()
{
    m_pJournalManager = 0;
}

BOOL CPortBase::InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* pJournalManager)
{
    m_pJournalManager = pJournalManager;

    return FALSE;
}

BOOL CPortBase::ResetJournalManager(HANDLE p_hHandle)
{
    m_pJournalManager = 0;

    return FALSE;
}

BOOL CPortBase::InitPort(WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        if(m_pGateway->InitPort(p_usBoardNumber, p_usNbBoardWithOldDriver))
        {
            oResult = InitInterfacePortName(p_usBoardNumber, p_usNbBoardWithOldDriver);
        }
    }

    return oResult;
}

BOOL CPortBase::UpdatePort(tPortList& p_rOpenPortList)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->UpdatePort(p_rOpenPortList);
    }

    return oResult;
}

BOOL CPortBase::OpenPort(CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        if(m_pGateway->OpenPort(p_PortName, p_pErrorInfo))
        {
            m_strPortName = p_PortName;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CPortBase::ReopenPort(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        //Reopen
        oResult = m_pGateway->ReopenPort(m_strPortName, p_pErrorInfo);
    }

    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return oResult;
}

BOOL CPortBase::ResetPort(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->ResetPort(p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::ClosePort(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        if(m_pGateway->ClosePort(p_pErrorInfo))
        {
            DeleteGateway();
            oResult = TRUE;
        }
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::GetPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetPortSettings(p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::SetPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->SetPortSettings(p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::GetDefaultPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetDefaultPortSettings(p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::SetDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->SetDefaultPortSettings(p_ulBaudrate, p_ulTimeout, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::GetPortMode(WORD* p_pusPortMode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetPortMode(p_pusPortMode, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::SetPortMode(WORD p_usPortMode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->SetPortMode(p_usPortMode, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::GetPortNameSelection(CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetPortNameSelection(p_pPortSel, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::GetBaudrateSelection(CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetBaudrateSelection(p_pulBaudrateSel, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::GetPortModeSelection(CStdStringArray* p_pPortModeSel, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetPortModeSelection(p_pPortModeSel, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::InitInterfacePortName(WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->InitInterfacePortName(&m_strInterfacePortName, p_usBoardNumber, p_usNbBoardWithOldDriver);
    }

    return oResult;
}

BOOL CPortBase::ExecuteCommand(CCommandRoot* p_pCommand, HANDLE p_hTransactionHandle)
{
    BOOL oResult = FALSE;
    CLayerManagerBase* pLayerManager = 0;
    HANDLE hHandle = 0;

    if(p_pCommand)
    {
        //Init References
        p_pCommand->InitGateway(m_pGateway);
        p_pCommand->InitJournalManager(m_pJournalManager);

        //Execute
        oResult = p_pCommand->Execute(pLayerManager, hHandle, p_hTransactionHandle);

        //Reset References
        p_pCommand->ResetGateway();
        p_pCommand->ResetJournalManager();
    }

    return oResult;
}

BOOL CPortBase::GetCommands(CStdString* p_pCommandInfo)
{
    return FALSE;
}

BOOL CPortBase::EnableTracing(CStdString p_FileName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->EnableTracing(p_FileName, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::DisableTracing(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->DisableTracing(p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return oResult;
}

BOOL CPortBase::AreParameterEqual(CPortBase* p_pPort)
{
    BOOL oResult = FALSE;

    if(p_pPort && m_pGateway)
    {
        oResult = m_pGateway->AreParameterEqual(p_pPort->m_pGateway);
    }

    return oResult;
}

BOOL CPortBase::SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->SetParameter(p_Name, p_pValue, p_ulSize);
    }

    return oResult;
}

BOOL CPortBase::SetParameter(CStdString p_Name, CStdString p_Value)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->SetParameter(p_Name, p_Value);
    }

    return oResult;
}

BOOL CPortBase::GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetParameter(p_Name, p_pValue, p_ulSize);
    }

    return oResult;
}

BOOL CPortBase::GetParameter(CStdString p_Name, CStdString& p_rValue)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->GetParameter(p_Name, p_rValue);
    }

    return oResult;
}

BOOL CPortBase::Lock(DWORD p_ulTimeout)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->Lock(p_ulTimeout);
    }

    return oResult;
}

BOOL CPortBase::Unlock()
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->Unlock();
    }

    return oResult;
}

BOOL CPortBase::IsLocked()
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        oResult = m_pGateway->IsLocked();
    }

    return oResult;
}

