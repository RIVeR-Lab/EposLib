// HandleRegistrationMap_PS.h: Schnittstelle f�r die Klasse CHandleRegistrationMap_PS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLEREGISTRATIONMAP_PS_H__D5236CAD_ACEA_43B8_9A2D_18C4DB32B310__INCLUDED_)
#define AFX_HANDLEREGISTRATIONMAP_PS_H__D5236CAD_ACEA_43B8_9A2D_18C4DB32B310__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <list>
#include <map>

class CHandleRegistration_PS;
class CInterfaceManagerBase;
class CDeviceCommandSetManagerBase;
class CErrorHandling;

typedef std::list<CHandleRegistration_PS*> tHandleRegistrationPS_List;

class CHandleRegistrationMap_PS
{
private:
	//Embedded Singleton Class
	class CSingleton
	{
	public:
		CSingleton();
		virtual ~CSingleton();

		CHandleRegistrationMap_PS* GetInstance(void* p_pParent);
		BOOL ReleaseInstance(void* p_pParent);
		int GetParentCount();

	private:
		CHandleRegistrationMap_PS* m_pRegistrationMap;
		std::list<void*> m_ParentRegistrationList;
	};

public:
	CHandleRegistrationMap_PS();
	virtual ~CHandleRegistrationMap_PS();
	static CHandleRegistrationMap_PS* GetInstance(int p_lInstanceValue, void* p_pParent);
	static BOOL ReleaseInstance(int p_lInstanceValue, void* p_pParent);

	//Registration
	HANDLE RegisterHandle(CProtocolStackBase* pProtocolStack, CInterfaceManagerBase* pManager, HANDLE hInterfaceHandle);
	HANDLE RegisterHandle(CProtocolStackBase* pProtocolStack, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle);
	BOOL DeleteMapRegistration(HANDLE hProtocolStackHandle);
	BOOL GetRegistrationValues(HANDLE hProtocolStackHandle, CProtocolStackBase** ppProtocolStack, CInterfaceManagerBase** ppManager, HANDLE* phInterfaceHandle);
	BOOL GetRegistrationValues(HANDLE hProtocolStackHandle, CProtocolStackBase** ppProtocolStack, CDeviceCommandSetManagerBase** ppManager, HANDLE* phDeviceCommandSetHandle);
	BOOL GetRegistrationValues(HANDLE hProtocolStackHandle, CProtocolStackBase** ppProtocolStack, CLayerManagerBase** ppManager, HANDLE* phHandle);
	BOOL GetRegistration(HANDLE hProtocolStackHandle, CHandleRegistration_PS** ppRegistration);
	BOOL GetRegistrationList(tHandleRegistrationPS_List** ppList);
	BOOL IsRegistrationMapEmpty();

	//Parameter
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hPS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hPS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);
	
private:
	HANDLE IsRegistrationExisting(CProtocolStackBase* pProtocolStack, CInterfaceManagerBase* pManager,HANDLE hInterfaceHandle);
	HANDLE IsRegistrationExisting(CProtocolStackBase* pProtocolStack, CDeviceCommandSetManagerBase* pManager,HANDLE hDeviceCommandSetHandle);
	HANDLE AddNewRegistration(CProtocolStackBase* pProtocolStack, CInterfaceManagerBase* pManager,HANDLE hInterfaceHandle);
	HANDLE AddNewRegistration(CProtocolStackBase* pProtocolStack, CDeviceCommandSetManagerBase* pManager, HANDLE hDeviceCommandSetHandle);
	CHandleRegistration_PS* GetNextFreeRegistration();

	BOOL InitErrorHandling();
	BOOL DeleteErrorHandling();
	void DeleteHandleRegistrationList();

private:
	static std::map<void*,void*>* m_pSingletonMap;
	CErrorHandling* m_pErrorHandling;
	std::list<CHandleRegistration_PS*> m_HandleRegistrationList;
};

#endif // !defined(AFX_HANDLEREGISTRATIONMAP_PS_H__D5236CAD_ACEA_43B8_9A2D_18C4DB32B310__INCLUDED_)
