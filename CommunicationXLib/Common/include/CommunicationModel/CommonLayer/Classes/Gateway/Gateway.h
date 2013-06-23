// Gateway.h: Schnittstelle f�r die Klasse CGateway.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Gateway_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
#define AFX_Gateway_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>

class CLayerManagerBase;
class CParameterSet;
class CMmcCriticalSectionEx;

class CGateway
{
public:
	BOOL InitGateway();

	CGateway();
	virtual ~CGateway();
	virtual CGateway* Clone();
	CGateway& operator=(CGateway& other);

	//Funktionalit�t
	virtual BOOL ProcessCommand(CCommandRoot* pCommand,CLayerManagerBase* pManager,HANDLE h_Handle,HANDLE hTransactionHandle);
	virtual BOOL AbortCommands(BOOL oActive);

	//ErrorHandling
	virtual BOOL InitErrorHandling();
	virtual BOOL InitParameterSet();
	virtual BOOL InitCriticalSection();
	virtual BOOL DeleteErrorHandling();
	virtual BOOL DeleteParameterSet();
	virtual BOOL DeleteCriticalSection();

	//ParameterSet
	BOOL ResetParameterSet();
	BOOL AddParameter(CStdString p_Name, BYTE* p_pDefaultValue, DWORD p_ulSize);
	BOOL AddParameter(CStdString p_Name, CStdString p_DefaultValue);
	BOOL SetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL SetParameter(CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);
	BOOL AreParameterEqual(CGateway* p_pGateway);

	//Layer Parameter Stack
	virtual BOOL InitLayerParameterStack(CCommandRoot* p_pCommand);

	//Critical Section
	BOOL Lock(CCommandRoot* p_pCommand);
    BOOL IsLocked(CCommandRoot* p_pCommand);
	BOOL Lock(DWORD p_ulTimeout = -1);
	BOOL IsLocked();
	BOOL Unlock();
	BOOL AbortLock(BOOL oActive);
    	
protected:
	BOOL CheckLayers(CCommandRoot* pCommand, CLayerManagerBase* pManager);
	BOOL GetTraceString(CStdString& p_rTraceString);

protected:
	CErrorHandling* m_pErrorHandling;
	CParameterSet* m_pParameterSet;

	ELayer m_eInputLayer;
	ELayer m_eOutputLayer;

	BOOL m_oAbortCommands;
	//CriticalSection
    CMmcCriticalSectionEx* m_pCriticalSection;
};

#endif // !defined(AFX_Gateway_H__21893C20_2412_4E7B_9899_51A57FDE37D4__INCLUDED_)
