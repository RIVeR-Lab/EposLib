// HandleRegistrationMap_DCS.h: Schnittstelle f�r die Klasse CHandleRegistrationMap_DCS.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HANDLEREGISTRATIONMAP_DCS_H__4E55CCD5_B158_4A93_9D7E_8008F364DD6E__INCLUDED_)
#define AFX_HANDLEREGISTRATIONMAP_DCS_H__4E55CCD5_B158_4A93_9D7E_8008F364DD6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>

class CProtocolStackManagerBase;
class CHandleRegistration_DCS;
class CDeviceBase;
class CErrorHandling;

typedef std::list<CHandleRegistration_DCS*> tHandleRegistrationDCS_List;

class CHandleRegistrationMap_DCS
{
private:
	//Embedded Singleton Class
	class CSingleton
	{
	public:
		CSingleton();
		virtual ~CSingleton();

		CHandleRegistrationMap_DCS* GetInstance(void* p_pParent);
		BOOL ReleaseInstance(void* p_pParent);
		size_t GetParentCount();

	private:
		CHandleRegistrationMap_DCS* m_pRegistrationMap;
		std::list<void*>* m_pParentRegistrationList;
	};

public:
	CHandleRegistrationMap_DCS();
	virtual ~CHandleRegistrationMap_DCS();
	static CHandleRegistrationMap_DCS* GetInstance(size_t p_lInstanceValue, void* p_pParent);
	static BOOL ReleaseInstance(size_t p_lInstanceValue, void* p_pParent);
	
	//Registration
	HANDLE RegisterHandle(CDeviceBase* pDevice,CProtocolStackManagerBase* pManager,HANDLE hProtocolStackHandle);
	BOOL DeleteMapRegistration(HANDLE hDeviceCommandSetHandle);
	BOOL IsLastRegistration(HANDLE p_hDCS_Handle, HANDLE p_hPS_Handle);
	BOOL IsRegistrationMapEmpty();
	BOOL RemoveAll();

	BOOL GetRegistrationValues(HANDLE hDeviceCommandSetHandle,CDeviceBase** ppDevice,CProtocolStackManagerBase** ppManager,HANDLE* phProtocolStackHandle);
	BOOL GetRegistration(HANDLE hDeviceCommandSetHandle,CHandleRegistration_DCS** ppRegistration);
	BOOL GetRegistrationList(tHandleRegistrationDCS_List** ppList);

	//Parameter
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL SetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL GetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);

private:
	HANDLE AddNewRegistration(CDeviceBase* pDevice,CProtocolStackManagerBase* pManager,HANDLE hProtocolStackHandle);
	CHandleRegistration_DCS* GetNextFreeRegistration();

	void DeleteHandleRegistrationList();
	BOOL InitErrorHandling();
	BOOL DeleteErrorHandling();

private:
	static std::map<void*,void*>* m_pSingletonMap;
	CErrorHandling* m_pErrorHandling;
	std::list<CHandleRegistration_DCS*> m_HandleRegistrationList;
};

#endif // !defined(AFX_HANDLEREGISTRATIONMAP_DCS_H__4E55CCD5_B158_4A93_9D7E_8008F364DD6E__INCLUDED_)
