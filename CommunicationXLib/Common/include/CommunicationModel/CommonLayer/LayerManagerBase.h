// LayerManagerBase.h: Schnittstelle f�r die Klasse CLayerManagerBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LayerManagerBase_H__CA2FE82D_A0B8_4525_8638_07517CCB6DF6__INCLUDED_)
#define AFX_LayerManagerBase_H__CA2FE82D_A0B8_4525_8638_07517CCB6DF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>

class CCommandRoot;
class CJournalManagerBase;
class CErrorInfo;

class CLayerManagerBase
{
public:

    ELayer GetLayer();

	CLayerManagerBase();
    CLayerManagerBase(int p_lInstanceValue);
    CLayerManagerBase(const CLayerManagerBase& rObject);
    virtual ~CLayerManagerBase();
	bool IsKindOf(CStdString strClassType);

    //JournalManager
    virtual BOOL InitJournalManager(HANDLE hHandle, CJournalManagerBase* pJournalManager);
    virtual BOOL ResetJournalManager(HANDLE hHandle);
    virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
    virtual void ResetJournalManager();

    //Funktionalit�t
    virtual BOOL ExecuteCommand(CCommandRoot* pCommand, HANDLE hHandle, HANDLE hTransactionHandle);
    virtual BOOL GetCommands(HANDLE hHandle, ELayer eLayer, CStdString* pCommandInfo);

    virtual BOOL AbortCommands(HANDLE hHandle, BOOL oActive);

    //ErrorHandling
    virtual BOOL InitErrorHandling();
    virtual BOOL DeleteErrorHandling();

	//Parameter
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);

	//LayerManager
    virtual BOOL GetLayerManager(HANDLE hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle);

protected:
    BOOL CheckLayer(CCommandRoot *pCommand);

    BOOL CheckLayer(ELayer eLayer);

protected:
    CErrorHandling* m_pErrorHandling;
    CMmcCriticalSection m_Sync;

	//Instance Value for Singletons
	int m_lInstanceValue;
	ELayer m_eLayer;
	CStdString m_strClassType;
};

#endif // !defined(AFX_LayerManagerBase_H__CA2FE82D_A0B8_4525_8638_07517CCB6DF6__INCLUDED_)
