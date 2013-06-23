// HandleRegistrationMap_VCS.cpp: Implementierung der Klasse CHandleRegistrationMap_VCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include "../VirtualDevices/BaseClasses/VirtualDeviceBase.h"
#include "HandleRegistration_VCS.h"
#include <CommunicationModel/VirtualCommandSet/Classes/Handle/HandleRegistrationMap_VCS.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

std::map<void*,void*>* CHandleRegistrationMap_VCS::m_pSingletonMap = 0;

CHandleRegistrationMap_VCS::CHandleRegistrationMap_VCS():
	m_pErrorHandling(0)
{
	InitErrorHandling();
}

CHandleRegistrationMap_VCS::~CHandleRegistrationMap_VCS()
{
	DeleteErrorHandling();
	DeleteHandleRegistrationList();
}

BOOL CHandleRegistrationMap_VCS::InitErrorHandling()
{
	CErrorProducer errorProducer;
    CStdString strClassName = "HandleRegistrationMap_VCS";
	BOOL oResult = FALSE;

	DeleteErrorHandling();
	m_pErrorHandling = new CErrorHandling();
	if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        oResult = TRUE;
    }

	return oResult;
}

BOOL CHandleRegistrationMap_VCS::DeleteErrorHandling()
{
	if(m_pErrorHandling)
	{
		delete m_pErrorHandling;
		m_pErrorHandling = NULL;
	}

	return TRUE;
}

CHandleRegistrationMap_VCS* CHandleRegistrationMap_VCS::GetInstance(int p_lInstanceValue, void* p_pParent)
{
	CHandleRegistrationMap_VCS* pInstance = NULL;

	//SingletonMap
	if(!m_pSingletonMap)
		m_pSingletonMap = new std::map<void*,void*>();

	if(m_pSingletonMap)
	{
		std::map<void*,void*>::iterator it =
				m_pSingletonMap->find((void*)p_lInstanceValue);

		CHandleRegistrationMap_VCS::CSingleton* pSingleton = 0;

		if( it == m_pSingletonMap->end() )
		{
			pSingleton = new CHandleRegistrationMap_VCS::CSingleton();
			m_pSingletonMap->insert(std::pair<void*,void*>((void*)p_lInstanceValue, (void*)pSingleton));
		}
		else
			pSingleton = (CHandleRegistrationMap_VCS::CSingleton*)(*it).second;

		//Get Instance
		if(pSingleton != NULL)
			pInstance = pSingleton->GetInstance(p_pParent);
	}

	return pInstance;
}

BOOL CHandleRegistrationMap_VCS::ReleaseInstance(int p_lInstanceValue, void* p_pParent)
{
	BOOL oResult = FALSE;

	if(m_pSingletonMap!=NULL)
	{
		CHandleRegistrationMap_VCS::CSingleton* pSingleton =
				(CHandleRegistrationMap_VCS::CSingleton*)(*m_pSingletonMap)[(void*)p_lInstanceValue];

		if( pSingleton != NULL )
		{
			//Relase Instance
			oResult = pSingleton->ReleaseInstance(p_pParent);

			//Delete Singleton
			if(pSingleton->GetParentCount() == 0)
			{
				m_pSingletonMap->erase((void*)p_lInstanceValue);

				delete pSingleton;
				pSingleton = NULL;
			}

			//Delete Map
			if(m_pSingletonMap->size() == 0)
			{
				delete m_pSingletonMap;
				m_pSingletonMap = NULL;
			}
		}
	}

	return oResult;
}

//Gesamte Registrationen l�schen
/****************************************************************************/
void CHandleRegistrationMap_VCS::DeleteHandleRegistrationList()
{
	for( std::list<CHandleRegistration_VCS*>::iterator it=m_HandleRegistrationList.begin();
			it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_VCS* pHandleRegistration = (*it);
		if( pHandleRegistration != NULL )
		{
			delete pHandleRegistration;
			pHandleRegistration = NULL;
		}
	}

	m_HandleRegistrationList.clear();
}

BOOL CHandleRegistrationMap_VCS::GetRegistrationValues(HANDLE hVCS_Handle,CVirtualDeviceBase** ppVirtualDevice,CDeviceCommandSetManagerBase** ppManager,HANDLE* phDeviceCommandSetHandle)
{
	for( std::list<CHandleRegistration_VCS*>::iterator it=m_HandleRegistrationList.begin();
				it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_VCS* pHandleRegistration = (*it);
		if( pHandleRegistration != NULL )
		{
			if(pHandleRegistration->GetKeyHandle() == hVCS_Handle)
				return pHandleRegistration->GetRegistrationValues(ppVirtualDevice,ppManager,phDeviceCommandSetHandle);
		}
	}
	return FALSE;
}

BOOL CHandleRegistrationMap_VCS::GetRegistration(HANDLE hVCS_Handle,CHandleRegistration_VCS** ppRegistration)
{
	if((!ppRegistration) || (!hVCS_Handle))
		return FALSE;

	for( std::list<CHandleRegistration_VCS*>::iterator it=m_HandleRegistrationList.begin();
				it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_VCS* pHandleRegistration = (*it);
		if( pHandleRegistration != NULL )
		{
			if(pHandleRegistration->GetKeyHandle() == hVCS_Handle)
			{
				*ppRegistration = pHandleRegistration;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CHandleRegistrationMap_VCS::GetRegistrationList(tHandleRegistrationVCS_List** ppList)
{
	if(ppList)
	{
		*ppList = &m_HandleRegistrationList;
		return TRUE;
	}

	return FALSE;
}

BOOL CHandleRegistrationMap_VCS::IsLastRegistration(HANDLE p_hVCS_Handle, HANDLE p_hDCS_Handle)
{
	int iCount = 0;

	for( std::list<CHandleRegistration_VCS*>::iterator it=m_HandleRegistrationList.begin();
				it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_VCS* pHandleRegistration = (*it);
		if( pHandleRegistration != NULL )
		{
			HANDLE hDCS_Handle = NULL;
			HANDLE hVCS_Handle = pHandleRegistration->GetKeyHandle();
			pHandleRegistration->GetRegistrationValues(NULL, NULL, &hDCS_Handle);

			//Check
			if((p_hVCS_Handle != hVCS_Handle) && (p_hDCS_Handle == hDCS_Handle))
			{
				iCount++;
			}
		}
	}

	return (BOOL)(iCount == 0);
}

BOOL CHandleRegistrationMap_VCS::IsRegistrationMapEmpty()
{
    BOOL oResult(TRUE);

    for( std::list<CHandleRegistration_VCS*>::iterator it=m_HandleRegistrationList.begin();
    				it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_VCS* pHandleRegistration = (*it);
		if( pHandleRegistration != NULL && pHandleRegistration->GetKeyHandle() )
		{
			oResult = FALSE;
			break;
		}
	}

	return oResult;
}

//Werte aus der Liste l�schen. Position der Liste bleibt bestehen
/****************************************************************************/
BOOL CHandleRegistrationMap_VCS::DeleteMapRegistration(HANDLE hVCS_Handle)
{
    for( std::list<CHandleRegistration_VCS*>::iterator it=m_HandleRegistrationList.begin();
    				it!=m_HandleRegistrationList.end(); it++)
	{
    	CHandleRegistration_VCS* pHandleRegistration = (*it);
		if( pHandleRegistration != NULL )
		{
			if(pHandleRegistration->GetKeyHandle() == hVCS_Handle)
			{
				pHandleRegistration->DeleteRegistration();
				return TRUE;
			}
		}
	}

	return FALSE;
}

HANDLE CHandleRegistrationMap_VCS::RegisterHandle(CVirtualDeviceBase* pVirtualDevice,CDeviceCommandSetManagerBase* pManager,HANDLE hDeviceCommandSetHandle)
{
	HANDLE hVCS_Handle;

	//Always add new Registration for Virtual Device
	hVCS_Handle = AddNewRegistration(pVirtualDevice,pManager,hDeviceCommandSetHandle);

	return hVCS_Handle;
}

HANDLE CHandleRegistrationMap_VCS::AddNewRegistration(CVirtualDeviceBase* pVirtualDevice,CDeviceCommandSetManagerBase* pManager,HANDLE hDeviceCommandSetHandle)
{
	CHandleRegistration_VCS* pHandleRegistration = NULL;

	pHandleRegistration = GetNextFreeRegistration();

	if(pHandleRegistration)
	{
		pHandleRegistration->Init(pVirtualDevice,pManager,hDeviceCommandSetHandle);
		return pHandleRegistration->GetKeyHandle();
	}
	return 0;
}

//In der Liste leere Positionen aus vorherige gel�schte Objekte suchen
// Falls keine Vorhanden neue position einf�gen und handle zur�ckgeben
/****************************************************************************/
CHandleRegistration_VCS* CHandleRegistrationMap_VCS::GetNextFreeRegistration()
{
	CHandleRegistration_VCS* pHandleRegistration = NULL;
	DWORD wVCS = ((int)VIRTUAL_COMMAND_SET_LAYER << 16) + 1;

	for( std::list<CHandleRegistration_VCS*>::iterator it=m_HandleRegistrationList.begin();
				it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_VCS* pHandleRegistration = (*it);

		if( pHandleRegistration != NULL && pHandleRegistration->IsEmpty())
		{
			pHandleRegistration->SetKeyHandle((HANDLE)wVCS);
			return pHandleRegistration;
		}
		wVCS++;
	}

	pHandleRegistration = new CHandleRegistration_VCS();
	pHandleRegistration->SetKeyHandle((HANDLE)wVCS);
	m_HandleRegistrationList.push_back(pHandleRegistration);

	return pHandleRegistration;
}

/************************************************************************/
//Embedded Singleton Class
/************************************************************************/
CHandleRegistrationMap_VCS::CSingleton::CSingleton():
	m_pRegistrationMap(0)
{
	m_pRegistrationMap = new CHandleRegistrationMap_VCS();
}

CHandleRegistrationMap_VCS::CSingleton::~CSingleton()
{
	if(m_pRegistrationMap) delete m_pRegistrationMap;
}

CHandleRegistrationMap_VCS* CHandleRegistrationMap_VCS::CSingleton::GetInstance(void* p_pParent)
{
	bool found = false;

	for( std::list<void*>::iterator it = m_ParentRegistrationList.begin(); it != m_ParentRegistrationList.end(); it++)
	{
		void* pRegistration = (*it);
		if( pRegistration == p_pParent )
			found = true;
	}

	if(!found)
		m_ParentRegistrationList.push_back(p_pParent);

	//Instance
	return m_pRegistrationMap;
}

BOOL CHandleRegistrationMap_VCS::CSingleton::ReleaseInstance(void* p_pParent)
{
	BOOL oResult = FALSE;
	bool oFound = false;

	for( std::list<void*>::iterator it = m_ParentRegistrationList.begin();it != m_ParentRegistrationList.end(); it++)
	{
		void* pRegistration = (*it);
		if( pRegistration == p_pParent )
			oFound = true;
	}

	//Unregister Parent
	if(oFound)
	{
		m_ParentRegistrationList.remove(p_pParent);
		oResult = TRUE;
	}

	return oResult;
}

int CHandleRegistrationMap_VCS::CSingleton::GetParentCount()
{
	return (int)m_ParentRegistrationList.size();
}

BOOL CHandleRegistrationMap_VCS::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	CMmcSingleLock* pRegLock = NULL;
    CVirtualDeviceBase* pVirtualDevice = NULL;
	CDeviceCommandSetManagerBase* pManager = NULL;
	HANDLE hDCS_Handle = 0;
	CHandleRegistration_VCS* pRegistration = NULL;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hVCS_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		if(pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pVirtualDevice,&pManager,&hDCS_Handle) && pVirtualDevice && pManager && hDCS_Handle)
			{
				if(PT_VIRTUAL_DEVICE == p_eParameterType)
				{
					//Set Virtual Device Parameter
					oResult = pVirtualDevice->SetParameter(p_Name,p_pValue,p_ulSize);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_VIRTUAL_DEVICE > p_eParameterType)
				{
					//Forward Parameter to next layer
					oResult = pManager->SetParameter(p_eParameterType, hDCS_Handle, p_Name, p_pValue, p_ulSize, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration) pRegistration->DoUnlock();
		}
	}

	return oResult;
}

BOOL CHandleRegistrationMap_VCS::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo)
{
	CMmcSingleLock* pRegLock = NULL;
    CVirtualDeviceBase* pVirtualDevice = NULL;
	CDeviceCommandSetManagerBase* pManager = NULL;
	HANDLE hDCS_Handle = 0;
	CHandleRegistration_VCS* pRegistration = NULL;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hVCS_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		if(pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pVirtualDevice,&pManager,&hDCS_Handle) && pVirtualDevice && pManager && hDCS_Handle)
			{
				if(PT_VIRTUAL_DEVICE == p_eParameterType)
				{
					//Set Virtual Device Parameter
					oResult = pVirtualDevice->SetParameter(p_Name,p_Value);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_VIRTUAL_DEVICE > p_eParameterType)
				{
					//Forward Parameter to next layer
					oResult = pManager->SetParameter(p_eParameterType, hDCS_Handle, p_Name, p_Value, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration) pRegistration->DoUnlock();
		}
	}

	return oResult;
}

BOOL CHandleRegistrationMap_VCS::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	CMmcSingleLock* pRegLock = NULL;
    CVirtualDeviceBase* pVirtualDevice = NULL;
	CDeviceCommandSetManagerBase* pManager = NULL;
	HANDLE hDCS_Handle = 0;
	CHandleRegistration_VCS* pRegistration = NULL;
	BOOL oIsLocked = FALSE;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hVCS_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		oIsLocked = pRegistration->IsLocked();
		if(oIsLocked || pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pVirtualDevice,&pManager,&hDCS_Handle) && pVirtualDevice && pManager && hDCS_Handle)
			{
				if(PT_VIRTUAL_DEVICE == p_eParameterType)
				{
					//Get Virtual Device Parameter
					oResult = pVirtualDevice->GetParameter(p_Name,p_pValue,p_ulSize);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_VIRTUAL_DEVICE > p_eParameterType)
				{
					//Forward Parameter to next layer
					oResult = pManager->GetParameter(p_eParameterType, hDCS_Handle, p_Name, p_pValue, p_ulSize, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
		}
	}

	return oResult;
}

BOOL CHandleRegistrationMap_VCS::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo)
{
	CMmcSingleLock* pRegLock = NULL;
    CVirtualDeviceBase* pVirtualDevice = NULL;
	CDeviceCommandSetManagerBase* pManager = NULL;
	HANDLE hDCS_Handle = 0;
	CHandleRegistration_VCS* pRegistration = NULL;
	BOOL oIsLocked = FALSE;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hVCS_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		oIsLocked = pRegistration->IsLocked();
		if(oIsLocked || pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pVirtualDevice,&pManager,&hDCS_Handle) && pVirtualDevice && pManager && hDCS_Handle)
			{
				if(PT_VIRTUAL_DEVICE == p_eParameterType)
				{
					//Set Virtual Device Parameter
					oResult = pVirtualDevice->SetParameter(p_Name,p_rValue);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_VIRTUAL_DEVICE > p_eParameterType)
				{
					//Forward Parameter to next layer
					oResult = pManager->SetParameter(p_eParameterType, hDCS_Handle, p_Name, p_rValue, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
		}
	}

	return oResult;
}


