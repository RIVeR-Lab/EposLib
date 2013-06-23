// Gateway.cpp: Implementierung der Klasse CGateway.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <Thread/MmcCriticalSection.h>
#include <ParameterSet/ParameterSet.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>
#include <CommunicationModel/CommonLayer/LayerManagerBase.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>

#include <CommunicationModel/CommonLayer/Classes/Gateway/Gateway.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGateway::CGateway()
{
	m_eInputLayer = NO_LAYER;
	m_eOutputLayer = NO_LAYER;

	m_oAbortCommands = FALSE;

	m_pErrorHandling = 0;
	m_pParameterSet = 0;
	m_pCriticalSection = 0;

	InitCriticalSection();
	InitErrorHandling();
	InitParameterSet();
}

CGateway::~CGateway()
{
	DeleteErrorHandling();
	DeleteParameterSet();
	DeleteCriticalSection();
}

BOOL CGateway::InitErrorHandling()
{
	DeleteErrorHandling();
	m_pErrorHandling = new CErrorHandling();
	return TRUE;
}

BOOL CGateway::InitParameterSet()
{
	DeleteParameterSet();

	m_pParameterSet = new CParameterSet();

	return TRUE;
}

BOOL CGateway::InitCriticalSection()
{
	BOOL oResult = FALSE;
	if(!m_pCriticalSection)
	{
		m_pCriticalSection = new CMmcCriticalSectionEx();
		oResult = TRUE;
	}

	return oResult;
}

BOOL CGateway::DeleteErrorHandling()
{
	if(m_pErrorHandling)
	{
		delete m_pErrorHandling;
		m_pErrorHandling = 0;
	}

	return TRUE;
}

BOOL CGateway::DeleteParameterSet()
{

	if(m_pParameterSet)
	{
		delete m_pParameterSet;
		m_pParameterSet = 0;
	}

	return TRUE;
}

BOOL CGateway::DeleteCriticalSection()
{
	BOOL oResult = FALSE;

	if(m_pCriticalSection)
	{
		delete m_pCriticalSection;
		m_pCriticalSection = 0;
		oResult = TRUE;
	}

	return oResult;
}

BOOL CGateway::InitGateway()
{
	return TRUE;
}

CGateway* CGateway::Clone()
{
	return 0;
}

CGateway& CGateway::operator=(CGateway& other)
{
	if(this != &other)
	{
		m_eInputLayer = other.m_eInputLayer;
		m_eOutputLayer = other.m_eOutputLayer;

		if(m_pParameterSet && other.m_pParameterSet) *m_pParameterSet = *other.m_pParameterSet;

	}

	return *this;
}

BOOL CGateway::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
	return TRUE;
}

BOOL CGateway::InitLayerParameterStack(CCommandRoot* p_pCommand)
{
	return TRUE;
}

BOOL CGateway::AbortCommands(BOOL oActive)
{
	m_oAbortCommands = oActive;
	AbortLock(oActive);
	return TRUE;
}

BOOL CGateway::CheckLayers(CCommandRoot *pCommand, CLayerManagerBase *pManager)
{
	if(pCommand)
	{
		if(pCommand->GetLayer() != m_eInputLayer)
		{
			return FALSE;
		}
	}

	if(pManager)
	{
		if(pManager->GetLayer() != m_eOutputLayer)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CGateway::ResetParameterSet()
{
	BOOL oResult = FALSE;

	if(m_pParameterSet)
	{
		oResult = m_pParameterSet->Reset();
	}

	return oResult;
}

BOOL CGateway::AreParameterEqual(CGateway* p_pGateway)
{
	BOOL oResult = FALSE;

	if(p_pGateway && p_pGateway->m_pParameterSet && m_pParameterSet)
	{
		oResult = (*m_pParameterSet == *p_pGateway->m_pParameterSet);
	}

	return oResult;
}

BOOL CGateway::SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

	if(m_pParameterSet)
	{
		//Init
		oResult = TRUE;

		if(!m_pParameterSet->SetParameter(p_Name, p_pValue, p_ulSize))
		{
			if(pErrorInfo) pErrorInfo->Init(k_Error_BadParameter, 0);
			oResult = FALSE;
		}
	}

	return oResult;

}

BOOL CGateway::SetParameter(CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

	if(m_pParameterSet)
	{
		//Init
		oResult = TRUE;

		if(!m_pParameterSet->SetParameter(p_Name, p_Value))
		{
			if(pErrorInfo) pErrorInfo->Init(k_Error_BadParameter, 0);
			oResult = FALSE;
		}
	}

	return oResult;

}

BOOL CGateway::GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

	if(m_pParameterSet)
	{
		//Init
		oResult = TRUE;

		if(!m_pParameterSet->GetParameter(p_Name, p_pValue, p_ulSize))
		{
			if(pErrorInfo) pErrorInfo->Init(k_Error_BadParameter, 0);
			oResult = FALSE;
		}
	}

	return oResult;

}

BOOL CGateway::GetParameter(CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo)
{
	BOOL oResult = FALSE;

	if(m_pParameterSet)
	{
		//Init
		oResult = TRUE;

		if(!m_pParameterSet->GetParameter(p_Name, p_rValue))
		{
			if(pErrorInfo) pErrorInfo->Init(k_Error_BadParameter, 0);
			oResult = FALSE;
		}
	}

	return oResult;

}

BOOL CGateway::AddParameter(CStdString p_Name, BYTE* p_pDefaultValue, DWORD p_ulSize)
{
	BOOL oResult = FALSE;

	if(m_pParameterSet)
	{
		oResult = m_pParameterSet->AddParameter(p_Name, p_pDefaultValue, p_ulSize, FALSE);
	}

	return oResult;
}

BOOL CGateway::AddParameter(CStdString p_Name, CStdString p_DefaultValue)
{
	BOOL oResult = FALSE;

	if(m_pParameterSet)
	{
		oResult = m_pParameterSet->AddParameter(p_Name, p_DefaultValue, FALSE);
	}

	return oResult;
}

BOOL CGateway::Lock(CCommandRoot* p_pCommand)
{
    BOOL oResult = FALSE;

    if(p_pCommand)
    {
    	//Lock
		if(Lock(p_pCommand->GetTimeout()))
		{
			//Init
			oResult = TRUE;

			//Layer Parameter Stack
			InitLayerParameterStack(p_pCommand);
		}
    }

    return oResult;
}

BOOL CGateway::IsLocked(CCommandRoot* p_pCommand)
{
    BOOL oResult = FALSE;

    //Check if is locked
	if(IsLocked())
	{
		//Init
		oResult = TRUE;

		//Layer Parameter Stack
		InitLayerParameterStack(p_pCommand);
	}

    return oResult;
}

BOOL CGateway::Lock(DWORD p_ulTimeout)
{
	CStdString strTrace = _T("Gateway");
	BOOL oResult = FALSE;

    if(m_pCriticalSection)
    {
        //Trace
		GetTraceString(strTrace);

		//Lock
		if(m_pCriticalSection->Lock(p_ulTimeout, strTrace))
		{
			//Init
			oResult = TRUE;
		}
    }

    return oResult;
}

BOOL CGateway::IsLocked()
{
	CStdString strTrace = _T("Gateway");
	BOOL oResult = FALSE;

    if(m_pCriticalSection)
    {
        //Trace
		GetTraceString(strTrace);

		//Check if is locked
		if(m_pCriticalSection->IsLocked(strTrace))
		{
			//Init
			oResult = TRUE;
		}
    }

    return oResult;
}

BOOL CGateway::Unlock()
{
    CStdString strTrace = _T("Gateway");
	BOOL oResult = FALSE;

    if(m_pCriticalSection)
    {
        //Trace
		GetTraceString(strTrace);

		//Unlock
		oResult = m_pCriticalSection->Unlock(strTrace);
    }

    return oResult;
}

BOOL CGateway::AbortLock(BOOL oActive)
{
	BOOL oResult = FALSE;
	if(m_pCriticalSection)
	{
		oResult = m_pCriticalSection->AbortLock(oActive == TRUE) ? TRUE : FALSE;
	}

	return oResult;
}

BOOL CGateway::GetTraceString(CStdString& p_rTraceString)
{
	ELayer eLayer = NO_LAYER;
	CStdString className = _T("");
	CStdString commandName = _T("");
	BOOL oResult = FALSE;

    if(m_pErrorHandling)
    {
		if(m_pErrorHandling->GetErrorProducerInfos(eLayer, className, commandName))
		{
			p_rTraceString = className;
			oResult = TRUE;
		}
    }

    return oResult;
}
