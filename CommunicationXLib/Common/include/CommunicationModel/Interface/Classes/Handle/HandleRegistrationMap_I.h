// HandleRegistrationMap_I.h: Schnittstelle f�r die Klasse CHandleRegistrationMap_I.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLEREGISTRATIONMAP_I_H__DBB200B9_BE8A_4E00_A907_3A8E225A2384__INCLUDED_)
#define AFX_HANDLEREGISTRATIONMAP_I_H__DBB200B9_BE8A_4E00_A907_3A8E225A2384__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>
#include <MmcTypeDefinition.h>
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>

class CHandleRegistration_I;
class CInterfaceBase;
class CPortBase;
class CErrorHandling;

typedef std::list<CHandleRegistration_I*> tHandleRegistrationI_List;

class CHandleRegistrationMap_I
{
private:
	//Embedded Singleton Class
	class CSingleton
	{
	public:
		CSingleton();
		virtual ~CSingleton();

		CHandleRegistrationMap_I* GetInstance(void* p_pParent);
		BOOL ReleaseInstance(void* p_pParent);
		int GetParentCount();

	private:
		CHandleRegistrationMap_I* m_pRegistrationMap;
		std::list<void*> m_ParentRegistrationList;
	};

public:
	CHandleRegistrationMap_I();
	virtual ~CHandleRegistrationMap_I();
	static CHandleRegistrationMap_I* GetInstance(int p_lInstanceValue, void* p_pParent);
	static BOOL ReleaseInstance(int p_lInstanceValue, void* p_pParent);

	//Registration
	HANDLE RegisterHandle(CInterfaceBase* pInterface,CPortBase* pPort);
	BOOL DeleteMapRegistration(HANDLE hInterfaceHandle);
	BOOL IsRegistrationMapEmpty();

	BOOL GetRegistrationValues(HANDLE hInterfaceHandle,CInterfaceBase** ppInterface,CPortBase** ppPort);
	BOOL GetRegistration(HANDLE hInterfaceHandle,CHandleRegistration_I** ppRegistration);
	BOOL GetRegistrationList(tHandleRegistrationI_List** ppList);
	BOOL GetInterfaceHandleSelection(BOOL oStartOfSelection,HANDLE* hInterfaceHandleSel,BOOL* poEndOfSelection);
	BOOL GetRegisteredPorts(CInterfaceBase* pInterface,tPortList& p_rPortList);

	HANDLE IsRegistrationExisting(CInterfaceBase* pInterface, CStdString p_PortName);
	HANDLE IsRegistrationExisting(CInterfaceBase* pInterface);
	int GetRegistrationCount(CInterfaceBase* pInterface);

	//Parameter
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);

private:
	HANDLE AddNewRegistration(CInterfaceBase* pInterface,CPortBase* pPort);
	CHandleRegistration_I* GetNextFreeRegistration();
	
	BOOL InitErrorHandling();
	BOOL DeleteErrorHandling();
	void DeleteHandleRegistrationList();

private:
	static std::map<void*,void*>* m_pSingletonMap;
	CErrorHandling* m_pErrorHandling;
	
	std::size_t m_strSelectionListIndex;
	std::list<CHandleRegistration_I*> m_HandleRegistrationList;
};

#endif // !defined(AFX_HANDLEREGISTRATIONMAP_I_H__DBB200B9_BE8A_4E00_A907_3A8E225A2384__INCLUDED_)
