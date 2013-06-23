// LayerManagerBase.cpp: Implementierung der Klasse CLayerManagerBase.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>
#include <CommunicationModel/CommonLayer/LayerManagerBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CLayerManagerBase::CLayerManagerBase()
{
    m_strClassType = _T("CLayerManagerBase");
    m_lInstanceValue = 0;
	m_eLayer = NO_LAYER;
    m_pErrorHandling = NULL;
    InitErrorHandling();
}

CLayerManagerBase::CLayerManagerBase(int p_lInstanceValue)
{
    m_strClassType = _T("CLayerManagerBase");
    m_lInstanceValue = p_lInstanceValue;
	m_eLayer = NO_LAYER;
    m_pErrorHandling = NULL;
    InitErrorHandling();
}

CLayerManagerBase::CLayerManagerBase(const CLayerManagerBase& rObject)
{
    m_lInstanceValue = rObject.m_lInstanceValue;
	m_eLayer = NO_LAYER;
    m_pErrorHandling = NULL;
    InitErrorHandling();
}

CLayerManagerBase::~CLayerManagerBase()
{
    DeleteErrorHandling();
}

bool CLayerManagerBase::IsKindOf(CStdString strClassType)
{
	return ( m_strClassType.CompareNoCase(strClassType) == 0 );
}

BOOL CLayerManagerBase::InitErrorHandling()
{
    DeleteErrorHandling();
    m_pErrorHandling = new CErrorHandling();
    return TRUE;
}


BOOL CLayerManagerBase::DeleteErrorHandling()
{
    if(m_pErrorHandling)
    {
        delete m_pErrorHandling;
        m_pErrorHandling = NULL;
    }

    return TRUE;
}


void CLayerManagerBase::InitJournalManager(CJournalManagerBase* pJournalManager)
{
}


void CLayerManagerBase::ResetJournalManager()
{
}


BOOL CLayerManagerBase::InitJournalManager(HANDLE hHandle, CJournalManagerBase* pJournalManager)
{
    return FALSE;
}

BOOL CLayerManagerBase::ResetJournalManager(HANDLE hHandle)
{
    return FALSE;
}

ELayer CLayerManagerBase::GetLayer()
{
    return m_eLayer;
}

BOOL CLayerManagerBase::ExecuteCommand(CCommandRoot *pCommand, HANDLE hHandle, HANDLE hTransactionHandle)
{
    return FALSE;
}

BOOL CLayerManagerBase::CheckLayer(CCommandRoot *pCommand)
{
    if(pCommand)
    {
        return (pCommand->GetLayer() == m_eLayer);
    }

    return FALSE;
}

BOOL CLayerManagerBase::CheckLayer(ELayer eLayer)
{
    return (eLayer == m_eLayer);
}

BOOL CLayerManagerBase::GetCommands(HANDLE hHandle, ELayer eLayer, CStdString* pCommandInfo)
{
    return FALSE;
}

BOOL CLayerManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
    return FALSE;
}
	
BOOL CLayerManagerBase::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo)
{
    return FALSE;
}
	
BOOL CLayerManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
    return FALSE;
}
	
BOOL CLayerManagerBase::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo)
{
    return FALSE;
}

BOOL CLayerManagerBase::GetLayerManager(HANDLE hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle)
{
    return FALSE;
}

BOOL CLayerManagerBase::AbortCommands(HANDLE hHandle, BOOL oActive)
{
    return TRUE;
}

