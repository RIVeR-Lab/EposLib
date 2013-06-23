// HandleRegistrationMap_I.cpp: Implementierung der Klasse CHandleRegistrationMap_I.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "../Interfaces/BaseClasses/InterfaceBase.h"
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>

#include "HandleRegistration_I.h"
#include <CommunicationModel/Interface/Classes/Handle/HandleRegistrationMap_I.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

std::map<void*,void*>* CHandleRegistrationMap_I::m_pSingletonMap = 0;

CHandleRegistrationMap_I::CHandleRegistrationMap_I():
	m_pErrorHandling(0),
	m_strSelectionListIndex(0)
{
	InitErrorHandling();
}

CHandleRegistrationMap_I::~CHandleRegistrationMap_I()
{
	DeleteErrorHandling();
	DeleteHandleRegistrationList();
}

BOOL CHandleRegistrationMap_I::InitErrorHandling()
{
	CErrorProducer errorProducer;
    CStdString strClassName = "HandleRegistrationMap_I";
	BOOL oResult = FALSE;

	DeleteErrorHandling();
	m_pErrorHandling = new CErrorHandling();
	if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(INTERFACE_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        oResult = TRUE;
    }
		
	return oResult;
}

BOOL CHandleRegistrationMap_I::DeleteErrorHandling()
{
	if(m_pErrorHandling)
	{
		delete m_pErrorHandling;
		m_pErrorHandling = NULL;
	}

	return TRUE;
}

CHandleRegistrationMap_I* CHandleRegistrationMap_I::GetInstance(int p_lInstanceValue, void* p_pParent)
{
	CHandleRegistrationMap_I::CSingleton* pSingleton = NULL;
	CHandleRegistrationMap_I* pInstance = NULL;

	//SingletonMap
	if(!m_pSingletonMap)
		m_pSingletonMap = new std::map<void*,void*>();

	if(m_pSingletonMap)
	{
		//Create Singleton
		std::map<void*,void*>::iterator it = m_pSingletonMap->find((void*)p_lInstanceValue);

		if(it==m_pSingletonMap->end())
		{
			pSingleton = new CHandleRegistrationMap_I::CSingleton();
			m_pSingletonMap->insert(std::pair<void*,void*>((void*)p_lInstanceValue,pSingleton));
		}
		else
			pSingleton = (CHandleRegistrationMap_I::CSingleton*)(*it).second;

		//Get Instance
		if(pSingleton)
			pInstance = pSingleton->GetInstance(p_pParent);
	}
	
	return pInstance;
}

BOOL CHandleRegistrationMap_I::ReleaseInstance(int p_lInstanceValue, void* p_pParent)
{
	CHandleRegistrationMap_I::CSingleton* pSingleton = NULL;
	BOOL oResult = FALSE;

	if(m_pSingletonMap)
	{
		std::map<void*,void*>::iterator it = m_pSingletonMap->find((void*)p_lInstanceValue);

		if(it!=m_pSingletonMap->end())
		{
			pSingleton = (CHandleRegistrationMap_I::CSingleton*)(*it).second;

			//Relase Instance
			oResult = pSingleton->ReleaseInstance(p_pParent);

			//Delete Singleton
			if(pSingleton->GetParentCount() == 0)
			{
				m_pSingletonMap->erase(it);

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
/************************************************************************/
void CHandleRegistrationMap_I::DeleteHandleRegistrationList()
{
	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
			it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_I* pRegistration = (*it);
		pRegistration->DeleteRegistration();
		delete pRegistration;
		pRegistration = NULL;
	}
	m_HandleRegistrationList.clear();
}

/**********************************************************************/
BOOL CHandleRegistrationMap_I::GetRegistrationList(tHandleRegistrationI_List** ppList)
{
	if(ppList)
	{
		*ppList = &m_HandleRegistrationList;
		return TRUE;
	}

	return FALSE;
}

BOOL CHandleRegistrationMap_I::IsRegistrationMapEmpty()
{
	BOOL oResult(TRUE);

	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
			it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_I* pRegistration = (*it);
        if(pRegistration->GetKeyHandle())
		{
			oResult = FALSE;
			break;
		}
    }

	return oResult;
}

/************************************************************************/
BOOL CHandleRegistrationMap_I::GetRegistrationValues(HANDLE hHandleInterface,CInterfaceBase** ppInterface,CPortBase** ppPort)
{
	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
				it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_I* pRegistration = (*it);
		if(pRegistration->GetKeyHandle() == hHandleInterface)
			return pRegistration->GetRegistrationValues(ppInterface,ppPort);
	}
	return FALSE;
}

/************************************************************************/
BOOL CHandleRegistrationMap_I::GetRegistration(HANDLE hHandleInterface,CHandleRegistration_I** ppRegistration)
{
	if((!ppRegistration) || (!hHandleInterface))
		return FALSE;

	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
				it!=m_HandleRegistrationList.end(); it++)
	{
		*ppRegistration = (*it);
		if((*ppRegistration)->GetKeyHandle() == hHandleInterface)
			return TRUE;
	}
	return FALSE;
}

//Werte aus der Liste l�schen. Position der Liste bleibt bestehen
/************************************************************************/
BOOL CHandleRegistrationMap_I::DeleteMapRegistration(HANDLE hInterfaceHandle)
{
	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
					it!=m_HandleRegistrationList.end(); it++)
	{
		CHandleRegistration_I* pRegistration = (*it);
		if(pRegistration->GetKeyHandle() == hInterfaceHandle)
		{
			pRegistration->DeleteRegistration();
			return TRUE;
		}
	}
	return FALSE;
}

//Suche im Register das Handle f�r bestimmte Parameter
/************************************************************************/
HANDLE CHandleRegistrationMap_I::RegisterHandle(CInterfaceBase* pInterface,CPortBase* pPort)
{
	CStdString portName = _T("");
	HANDLE hInterface = 0;

	if(pPort) pPort->GetPortName(&portName);

	//Add only one Registration per Interface
	hInterface = IsRegistrationExisting(pInterface, portName);
	if(hInterface == 0)
	{
		//Add New Registration Entry
		hInterface = AddNewRegistration(pInterface,pPort);
	}
	
	return hInterface;
}

//Sind Parameter in der Liste bereits vorhanden?
/************************************************************************/
HANDLE CHandleRegistrationMap_I::IsRegistrationExisting(CInterfaceBase* pInterface, CStdString p_PortName)
{
	HANDLE hHandle = 0;

	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
					it!=m_HandleRegistrationList.end(); it++)
		{
			CHandleRegistration_I* pRegistration = (*it);
		if(pRegistration->IsEqual(pInterface,p_PortName))
		{
			hHandle = pRegistration->GetKeyHandle();
			break;
		}
	}

	return hHandle;
}

//Neue registrierung, erhalte neues handle
/************************************************************************/
HANDLE CHandleRegistrationMap_I::AddNewRegistration(CInterfaceBase* pInterface,CPortBase* pPort)
{
	CHandleRegistration_I* pRegistration = NULL;

	pRegistration = GetNextFreeRegistration();

	if(pRegistration)
	{
		pRegistration->Init(pInterface,pPort);
		return pRegistration->GetKeyHandle();
	}
	return 0;
}

//In der Liste leere Positionen aus vorheriger gel�schter Objekte suchen
//Falls keine Vorhanden neue Position einf�gen und handle zur�ckgeben
/************************************************************************/
CHandleRegistration_I* CHandleRegistrationMap_I::GetNextFreeRegistration()
{
	CHandleRegistration_I* pRegistration = NULL;
	DWORD dInterfaceHandle = ((int)INTERFACE_LAYER << 16) + 1;

	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
					it!=m_HandleRegistrationList.end(); it++)
		{
			CHandleRegistration_I* pRegistration = (*it);
		if(pRegistration->IsEmpty())
		{
			pRegistration->SetKeyHandle((HANDLE)dInterfaceHandle);
			return pRegistration;
		}
		dInterfaceHandle++;
	}

	pRegistration = new CHandleRegistration_I();
	pRegistration->SetKeyHandle((HANDLE)dInterfaceHandle);
	m_HandleRegistrationList.push_back(pRegistration);
	return pRegistration;
}

/************************************************************************/
BOOL CHandleRegistrationMap_I::GetInterfaceHandleSelection(BOOL oStartOfSelection,HANDLE* hInterfaceHandleSel,BOOL* poEndOfSelection)
{
	CHandleRegistration_I* pRegistration = NULL;

	if((!hInterfaceHandleSel) || (!poEndOfSelection))
	{
        if(poEndOfSelection)
        {
		    *poEndOfSelection = TRUE;
        }

		return FALSE;
	}

	if(m_HandleRegistrationList.size() <= 0)
	{
        if(poEndOfSelection)
        {
		    *poEndOfSelection = TRUE;
        }

		return FALSE;
	}

	if(oStartOfSelection)
    {
		m_strSelectionListIndex = 0;
    }

	std::size_t index = 0;
	for( std::list<CHandleRegistration_I*>::iterator it =
			m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
	{
		if( index == m_strSelectionListIndex )
		{
			pRegistration = (*it);
			*hInterfaceHandleSel = pRegistration->GetKeyHandle();
			m_strSelectionListIndex++;
			break;
		}
		index++;
	}

	if(m_strSelectionListIndex++ >= m_HandleRegistrationList.size())
	{
		m_strSelectionListIndex = 0;
		*poEndOfSelection = TRUE;
	}

	return TRUE;
}

HANDLE CHandleRegistrationMap_I::IsRegistrationExisting(CInterfaceBase* pInterface)
{
	HANDLE hHandle = 0;

	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
					it!=m_HandleRegistrationList.end(); it++)
		{
			CHandleRegistration_I* pRegistration = (*it);
		if(pRegistration->IsEqual(pInterface))
		{
			hHandle = pRegistration->GetKeyHandle();
			break;
		}
	}

	return hHandle;
}

BOOL CHandleRegistrationMap_I::GetRegisteredPorts(CInterfaceBase* pInterface,tPortList& p_rPortList)
{
	CInterfaceBase* pInterfaceBase = NULL;
	CPortBase* pPortBase = NULL;
	BOOL oResult = TRUE;

	//Reset
	p_rPortList.clear();

	//Add Ports
	for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
					it!=m_HandleRegistrationList.end(); it++)
		{
			CHandleRegistration_I* pRegistration = (*it);
		if(pRegistration)
		{
			if(pRegistration->IsEqual(pInterface))
			{
				if(pRegistration->GetRegistrationValues(&pInterfaceBase,&pPortBase) && pPortBase)
				{
					p_rPortList.push_back(pPortBase);
				}
			}
		}
	}
	
	return oResult;
}

int CHandleRegistrationMap_I::GetRegistrationCount(CInterfaceBase* pInterface)
{
	int count = 0;

	if(pInterface)
	{
		for( std::list<CHandleRegistration_I*>::iterator it=m_HandleRegistrationList.begin();
						it!=m_HandleRegistrationList.end(); it++)
			{
				CHandleRegistration_I* pRegistration = (*it);
			if(pRegistration)
			{
				if(pRegistration->IsEqual(pInterface)) count++;
			}
		}
	}

	return count;
}

/************************************************************************/
//Embedded Singleton Class
/************************************************************************/
CHandleRegistrationMap_I::CSingleton::CSingleton():
	m_pRegistrationMap(0)
{
	m_pRegistrationMap = new CHandleRegistrationMap_I();
}

CHandleRegistrationMap_I::CSingleton::~CSingleton()
{
	if(m_pRegistrationMap) delete m_pRegistrationMap;
}

CHandleRegistrationMap_I* CHandleRegistrationMap_I::CSingleton::GetInstance(void* p_pParent)
{
	bool uFound = false;

	for( std::list<void*>::iterator it=m_ParentRegistrationList.begin();
					it!=m_ParentRegistrationList.end(); it++)
	{
		CHandleRegistration_I* pRegistration = (CHandleRegistration_I*)(*it);

		if( pRegistration==p_pParent)
			uFound = true;
	}

	if(!uFound)
		m_ParentRegistrationList.push_back(p_pParent);

	//Instance
	return m_pRegistrationMap;
}

BOOL CHandleRegistrationMap_I::CSingleton::ReleaseInstance(void* p_pParent)
{
	BOOL oResult = FALSE;

	for( std::list<void*>::iterator it=m_ParentRegistrationList.begin();
						it!=m_ParentRegistrationList.end(); it++)
		{
			CHandleRegistration_I* pRegistration = (CHandleRegistration_I*)(*it);

		if( pRegistration==p_pParent)
		{
			m_ParentRegistrationList.remove(pRegistration);
			oResult = TRUE;
			break;
		}
	}

	return oResult;
}

int CHandleRegistrationMap_I::CSingleton::GetParentCount()
{
	return (int)m_ParentRegistrationList.size();
}

BOOL CHandleRegistrationMap_I::SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	CInterfaceBase* pInterface = NULL;
	CPortBase* pPort = NULL;
	CHandleRegistration_I* pRegistration = NULL;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hI_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		if(pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pInterface,&pPort) && pInterface && pPort)
			{
				if(PT_INTERFACE == p_eParameterType)
				{
					//Set Interface Parameter
					oResult = pInterface->SetParameter(p_Name,p_pValue,p_ulSize);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_PORT == p_eParameterType)
				{
					//Set Port Parameter
					oResult = pPort->SetParameter(p_Name,p_pValue,p_ulSize);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration) pRegistration->DoUnlock();
		}
	}
	
	return oResult;
}

BOOL CHandleRegistrationMap_I::SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo)
{
	CInterfaceBase* pInterface = NULL;
	CPortBase* pPort = NULL;
	CHandleRegistration_I* pRegistration = NULL;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hI_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		if(pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pInterface,&pPort) && pInterface && pPort)
			{
				if(PT_INTERFACE == p_eParameterType)
				{
					//Set Interface Parameter
					oResult = pInterface->SetParameter(p_Name,p_Value);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_PORT == p_eParameterType)
				{
					//Set Port Parameter
					oResult = pPort->SetParameter(p_Name,p_Value);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration) pRegistration->DoUnlock();
		}
	}
	
	return oResult;
}

BOOL CHandleRegistrationMap_I::GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
	CInterfaceBase* pInterface = NULL;
	CPortBase* pPort = NULL;
	CHandleRegistration_I* pRegistration = NULL;
	BOOL oIsLocked = FALSE;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hI_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		oIsLocked = pRegistration->IsLocked();
		if(oIsLocked || pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pInterface,&pPort) && pInterface && pPort)
			{
				if(PT_INTERFACE == p_eParameterType)
				{
					//Get Interface Parameter
					oResult = pInterface->GetParameter(p_Name,p_pValue,p_ulSize);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_PORT == p_eParameterType)
				{
					//Get Port Parameter
					oResult = pPort->GetParameter(p_Name,p_pValue,p_ulSize);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
		}
	}
	
	return oResult;
}

BOOL CHandleRegistrationMap_I::GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo)
{
	CInterfaceBase* pInterface = NULL;
	CPortBase* pPort = NULL;
	CHandleRegistration_I* pRegistration = NULL;
	BOOL oIsLocked = FALSE;
	BOOL oResult = FALSE;

	//GetRegistration
	if(GetRegistration(hI_Handle, &pRegistration) && (pRegistration != NULL))
	{
		//Lock
		oIsLocked = pRegistration->IsLocked();
		if(oIsLocked || pRegistration->DoLock())
		{
			if(pRegistration->GetRegistrationValues(&pInterface,&pPort) && pInterface && pPort)
			{
				if(PT_INTERFACE == p_eParameterType)
				{
					//Set Interface Parameter
					oResult = pInterface->GetParameter(p_Name,p_rValue);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
				else if(PT_PORT == p_eParameterType)
				{
					//Set Port Parameter
					oResult = pPort->GetParameter(p_Name,p_rValue);
					if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
				}
			}

			//Unlock
			if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
		}
	}
	
	return oResult;
}



