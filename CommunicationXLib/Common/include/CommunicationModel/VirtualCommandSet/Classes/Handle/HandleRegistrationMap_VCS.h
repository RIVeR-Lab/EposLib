// HandleRegistrationMap_VCS.h: Schnittstelle f�r die Klasse CHandleRegistrationMap_VCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HandleRegistrationMap_VCS_H__88983CF6_B29C_4CA5_946C_B84A69C42875__INCLUDED_)
#define AFX_HandleRegistrationMap_VCS_H__88983CF6_B29C_4CA5_946C_B84A69C42875__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

class CHandleRegistration_VCS;
class CDeviceCommandSetManagerBase;
class CVirtualDeviceBase;
class CErrorHandling;

#include <list>
#include <map>

typedef std::list<CHandleRegistration_VCS*> tHandleRegistrationVCS_List;

class CHandleRegistrationMap_VCS
{
private:
	//Embedded Singleton Class
	class CSingleton
	{
	public:
		CSingleton();
		virtual ~CSingleton();

		CHandleRegistrationMap_VCS* GetInstance(void* p_pParent);
		BOOL ReleaseInstance(void* p_pParent);
		int GetParentCount();

	private:
		CHandleRegistrationMap_VCS* m_pRegistrationMap;
		std::list<void*> m_ParentRegistrationList;
	};

public:
	CHandleRegistrationMap_VCS();
	virtual ~CHandleRegistrationMap_VCS();
	static CHandleRegistrationMap_VCS* GetInstance(int p_lInstanceValue, void* p_pParent);
	static BOOL ReleaseInstance(int p_lInstanceValue, void* p_pParent);
	
	//Registration
	HANDLE RegisterHandle(CVirtualDeviceBase* pVirtualDevice,CDeviceCommandSetManagerBase* pManager,HANDLE hDeviceCommandSetHandle);
	BOOL DeleteMapRegistration(HANDLE hVCS_Handle);
	BOOL GetRegistrationValues(HANDLE hVCS_Handle,CVirtualDeviceBase** ppVirtualDevice,CDeviceCommandSetManagerBase** ppManager,HANDLE* phDeviceCommandSetHandle);
	BOOL GetRegistration(HANDLE hVCS_Handle,CHandleRegistration_VCS** ppRegistration);
	BOOL GetRegistrationList(tHandleRegistrationVCS_List** ppList);
	BOOL IsLastRegistration(HANDLE p_hVCS_Handle, HANDLE p_hDCS_Handle);
	BOOL IsRegistrationMapEmpty();

	//Parameter
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);

private:
	HANDLE AddNewRegistration(CVirtualDeviceBase* pVirtualDevice,CDeviceCommandSetManagerBase* pManager,HANDLE hDeviceCommandSetHandle);
	CHandleRegistration_VCS* GetNextFreeRegistration();

	BOOL InitErrorHandling();
	BOOL DeleteErrorHandling();
	void DeleteHandleRegistrationList();

private:
	static std::map<void*,void*>* m_pSingletonMap;
	CErrorHandling* m_pErrorHandling;
	std::list<CHandleRegistration_VCS*> m_HandleRegistrationList;
};

#endif // !defined(AFX_HandleRegistrationMap_VCS_H__88983CF6_B29C_4CA5_946C_B84A69C42875__INCLUDED_)
