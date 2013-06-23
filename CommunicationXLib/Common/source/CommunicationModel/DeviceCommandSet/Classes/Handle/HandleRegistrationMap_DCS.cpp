// HandleRegistrationMap_DCS.cpp: Implementierung der Klasse CHandleRegistrationMap_DCS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>
#include "../Devices/BaseClasses/DeviceBase.h"
#include "HandleRegistration_DCS.h"

#include "HandleRegistrationMap_DCS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

std::map<void*,void*>* CHandleRegistrationMap_DCS::m_pSingletonMap = 0;

CHandleRegistrationMap_DCS::CHandleRegistrationMap_DCS():
    m_pErrorHandling(0)
{
    InitErrorHandling();
}

CHandleRegistrationMap_DCS::~CHandleRegistrationMap_DCS()
{
    DeleteErrorHandling();
    DeleteHandleRegistrationList();
}

BOOL CHandleRegistrationMap_DCS::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "HandleRegistrationMap_DCS";
    BOOL oResult = FALSE;

    DeleteErrorHandling();
    m_pErrorHandling = new CErrorHandling();
    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        oResult = TRUE;
    }

    return oResult;
}

BOOL CHandleRegistrationMap_DCS::DeleteErrorHandling()
{
    if(m_pErrorHandling)
    {
        delete m_pErrorHandling;
        m_pErrorHandling = NULL;
    }

    return TRUE;
}

CHandleRegistrationMap_DCS* CHandleRegistrationMap_DCS::GetInstance(size_t p_lInstanceValue, void* p_pParent)
{
    CHandleRegistrationMap_DCS::CSingleton* pSingleton = NULL;
    CHandleRegistrationMap_DCS* pInstance = NULL;

    //SingletonMap
    if(!m_pSingletonMap)
    {
        m_pSingletonMap = new std::map<void*,void*>();
    }

    if(m_pSingletonMap)
    {
        //Create Singleton
    	std::map<void*,void*>::iterator it = m_pSingletonMap->find((void*)p_lInstanceValue);

		if(it == m_pSingletonMap->end())
		{
			pSingleton = new CHandleRegistrationMap_DCS::CSingleton();
			m_pSingletonMap->insert(std::pair<void*,void*>((void*)p_lInstanceValue,pSingleton));
		}
		else
			pSingleton = (CHandleRegistrationMap_DCS::CSingleton*)(*it).second;

		//Get Instance
		if(pSingleton)
			pInstance = pSingleton->GetInstance(p_pParent);
    }

    return pInstance;
}

BOOL CHandleRegistrationMap_DCS::ReleaseInstance(size_t p_lInstanceValue, void* p_pParent)
{
    CHandleRegistrationMap_DCS::CSingleton* pSingleton = NULL;
    BOOL oResult = FALSE;

    if(m_pSingletonMap)
    {
    	std::map<void*,void*>::iterator it = m_pSingletonMap->find((void*)p_lInstanceValue);

		if(it!=m_pSingletonMap->end())
		{
			pSingleton = (CHandleRegistrationMap_DCS::CSingleton*)(*it).second;

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

//Gesamte Registrationen löschen
/**********************************************************************/
void CHandleRegistrationMap_DCS::DeleteHandleRegistrationList()
{
    for(std::list<CHandleRegistration_DCS*>::iterator it=m_HandleRegistrationList.begin();
    		it!=m_HandleRegistrationList.end(); it++)
    {
    	CHandleRegistration_DCS* pHandleRegistration = (*it);
        if(pHandleRegistration)
            delete pHandleRegistration;
        pHandleRegistration = NULL;
    }
    m_HandleRegistrationList.clear();
}

BOOL CHandleRegistrationMap_DCS::GetRegistrationValues(HANDLE hDeviceCommandSetHandle, CDeviceBase** ppDevice, CProtocolStackManagerBase** ppManager, HANDLE* phProtocolStackHandle)
{
	for(std::list<CHandleRegistration_DCS*>::iterator it=m_HandleRegistrationList.begin();
	    		it!=m_HandleRegistrationList.end(); it++)
	{
	  	CHandleRegistration_DCS* pHandleRegistration = (*it);
        if(pHandleRegistration->GetKeyHandle() == hDeviceCommandSetHandle)
            return pHandleRegistration->GetRegistrationValues(ppDevice, ppManager, phProtocolStackHandle);
    }
    return FALSE;
}

BOOL CHandleRegistrationMap_DCS::GetRegistrationList(tHandleRegistrationDCS_List** ppList)
{
    if(ppList)
    {
        *ppList = &m_HandleRegistrationList;
        return TRUE;
    }

    return FALSE;
}

BOOL CHandleRegistrationMap_DCS::GetRegistration(HANDLE hDeviceCommandSetHandle, CHandleRegistration_DCS** ppRegistration)
{
    if((!ppRegistration) || (!hDeviceCommandSetHandle))
        return FALSE;

    for(std::list<CHandleRegistration_DCS*>::iterator it=m_HandleRegistrationList.begin();
        		it!=m_HandleRegistrationList.end(); it++)
	{
       	*ppRegistration = (*it);
        if((*ppRegistration)->GetKeyHandle() == hDeviceCommandSetHandle)
            return TRUE;
    }
    return FALSE;
}

//Werte aus der Liste löschen. Position der Liste bleibt bestehen
/**********************************************************************/
BOOL CHandleRegistrationMap_DCS::DeleteMapRegistration(HANDLE hDeviceCommandSetHandle)
{
	for(std::list<CHandleRegistration_DCS*>::iterator it=m_HandleRegistrationList.begin();
	    		it!=m_HandleRegistrationList.end(); it++)
	{
	  	CHandleRegistration_DCS* pHandleRegistration = (*it);
        if(pHandleRegistration->GetKeyHandle() == hDeviceCommandSetHandle)
        {
        	pHandleRegistration->DeleteRegistration();
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CHandleRegistrationMap_DCS::IsLastRegistration(HANDLE p_hDCS_Handle, HANDLE p_hPS_Handle)
{
    int iCount = 0;
	HANDLE hPS_Handle = 0;
	HANDLE hDCS_Handle = 0;

	for(std::list<CHandleRegistration_DCS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
	{
	    CHandleRegistration_DCS* pRegistration = (*it);
		if(pRegistration)
		{
			//Init
			hDCS_Handle = pRegistration->GetKeyHandle();
			pRegistration->GetRegistrationValues(NULL, NULL, &hPS_Handle);

			//Check
			if((p_hDCS_Handle != hDCS_Handle) && (p_hPS_Handle == hPS_Handle))
			{
				iCount++;
			}
		}
    }

	return (BOOL)(iCount == 0);
}

BOOL CHandleRegistrationMap_DCS::IsRegistrationMapEmpty()
{
	CHandleRegistration_DCS* pRegistration = NULL;
    BOOL oResult(TRUE);

    for(std::list<CHandleRegistration_DCS*>::iterator it=m_HandleRegistrationList.begin();
        		it!=m_HandleRegistrationList.end(); it++)
        {
        	CHandleRegistration_DCS* pHandleRegistration = (*it);
        if(pHandleRegistration->GetKeyHandle())
		{
			oResult = FALSE;
			break;
		}
    }

	return oResult;
}

//Werte aus der Liste l�schen. Position der Liste bleibt bestehen
/**********************************************************************/
BOOL CHandleRegistrationMap_DCS::RemoveAll()
{
    DeleteHandleRegistrationList();
    return TRUE;
}

//Suche im Register der Handle f�r die bestimmte Parameter
/**********************************************************************/
HANDLE CHandleRegistrationMap_DCS::RegisterHandle(CDeviceBase* pDevice, CProtocolStackManagerBase* pManager, HANDLE hProtocolStackHandle)
{
    HANDLE hDCS;

    //Always add new Registration for Device
    hDCS = AddNewRegistration(pDevice, pManager, hProtocolStackHandle);

    return hDCS;
}

//Neue registrierung, erhalte neue handle
/**********************************************************************/
HANDLE CHandleRegistrationMap_DCS::AddNewRegistration(CDeviceBase* pDevice, CProtocolStackManagerBase* pManager, HANDLE hProtocolStackHandle)
{
    CHandleRegistration_DCS* pHandleRegistration = NULL;

    pHandleRegistration = GetNextFreeRegistration();
    if(pHandleRegistration)
    {
        pHandleRegistration->Init(pDevice, pManager, hProtocolStackHandle);
        return pHandleRegistration->GetKeyHandle();
    }
    return 0;
}

//In der Liste leere Positionen aus vorherige gel�schte Objekte suchen
//Falls keine Vorhanden neue position einf�gen und handle zur�ckgeben
/**********************************************************************/
CHandleRegistration_DCS* CHandleRegistrationMap_DCS::GetNextFreeRegistration()
{
    CHandleRegistration_DCS* pHandleRegistration = NULL;
    size_t hDCS = ((int)DEVICE_COMMAND_SET_LAYER << 16) + 1;

    for(std::list<CHandleRegistration_DCS*>::iterator it=m_HandleRegistrationList.begin();
            		it!=m_HandleRegistrationList.end(); it++)
    {
       	CHandleRegistration_DCS* pHandleRegistration = (*it);
        if(pHandleRegistration->IsEmpty())
        {
            pHandleRegistration->SetKeyHandle((HANDLE)hDCS);
            return pHandleRegistration;
        }

        hDCS++;
    }

    pHandleRegistration = new CHandleRegistration_DCS();
    pHandleRegistration->SetKeyHandle((HANDLE)hDCS);

    m_HandleRegistrationList.push_back(pHandleRegistration);

    return pHandleRegistration;
}

/************************************************************************/
//Embedded Singleton Class
/************************************************************************/
CHandleRegistrationMap_DCS::CSingleton::CSingleton()
{
    m_pRegistrationMap = new CHandleRegistrationMap_DCS();
    m_pParentRegistrationList = new std::list<void*>();
}

CHandleRegistrationMap_DCS::CSingleton::~CSingleton()
{
    if(m_pRegistrationMap != 0)
    {
    	delete m_pRegistrationMap;
    	m_pRegistrationMap = 0;
    }

    if(m_pParentRegistrationList != 0)
    {
    	delete m_pParentRegistrationList;
    	m_pParentRegistrationList = 0;
    }
}

CHandleRegistrationMap_DCS* CHandleRegistrationMap_DCS::CSingleton::GetInstance(void* p_pParent)
{
	bool bFound = false;
	for(std::list<void*>::iterator it=m_pParentRegistrationList->begin();
			it!=m_pParentRegistrationList->end(); it++)
	{
		if((*it)==p_pParent)
			bFound = true;
	}

    //Register Parent
    if(!bFound)
        m_pParentRegistrationList->push_back(p_pParent);

    //Instance
    return m_pRegistrationMap;
}

BOOL CHandleRegistrationMap_DCS::CSingleton::ReleaseInstance(void* p_pParent)
{
    BOOL oResult = FALSE;

    if( m_pParentRegistrationList != 0 )
    {
		for(std::list<void*>::iterator it=m_pParentRegistrationList->begin();
					it!=m_pParentRegistrationList->end(); it++)
		{
			if((*it)==p_pParent)
			{
				m_pParentRegistrationList->erase(it);
				oResult = TRUE;
				break;
			}
		}
    }

    return oResult;
}

size_t CHandleRegistrationMap_DCS::CSingleton::GetParentCount()
{
    return m_pParentRegistrationList->size();
}

BOOL CHandleRegistrationMap_DCS::SetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
    CDeviceBase* pDevice = NULL;
    CProtocolStackManagerBase* pManager = NULL;
    HANDLE hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = NULL;
    BOOL oResult = FALSE;

    //Get Registration
    if(GetRegistration(hDCS_Handle, &pRegistration) && (pRegistration != NULL))
    {
        //Lock
        if(pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pDevice, &pManager, &hPS_Handle) && pDevice && pManager && hPS_Handle)
            {
                if(PT_DEVICE == p_eParameterType)
                {
                    //Set Device Parameter
                    oResult = pDevice->SetParameter(p_Name, p_pValue, p_ulSize);
                    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
                }
                else if(PT_DEVICE > p_eParameterType)
                {
                    //Forward Parameter to next layer
                    oResult = pManager->SetParameter(p_eParameterType, hPS_Handle, p_Name, p_pValue, p_ulSize, pErrorInfo);
                }
            }

            //Unlock
            if(pRegistration) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CHandleRegistrationMap_DCS::SetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* pErrorInfo)
{
    CDeviceBase* pDevice = NULL;
    CProtocolStackManagerBase* pManager = NULL;
    HANDLE hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = NULL;
    BOOL oResult = FALSE;

    //GetRegistration
    if(GetRegistration(hDCS_Handle, &pRegistration) && (pRegistration != NULL))
    {
        //Lock
        if(pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pDevice, &pManager, &hPS_Handle) && pDevice && pManager && hPS_Handle)
            {
                if(PT_DEVICE == p_eParameterType)
                {
                    //Set Device Parameter
                    oResult = pDevice->SetParameter(p_Name, p_Value);
                    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
                }
                else if(PT_DEVICE > p_eParameterType)
                {
                    //Forward Parameter to next layer
                    oResult = pManager->SetParameter(p_eParameterType, hPS_Handle, p_Name, p_Value, pErrorInfo);
                }
            }

            //Unlock
            if(pRegistration) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CHandleRegistrationMap_DCS::GetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
    CDeviceBase* pDevice = NULL;
    CProtocolStackManagerBase* pManager = NULL;
    HANDLE hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = NULL;
    BOOL oIsLocked = FALSE;
    BOOL oResult = FALSE;

    //GetRegistration
    if(GetRegistration(hDCS_Handle, &pRegistration) && (pRegistration != NULL))
    {
        //Lock
        oIsLocked = pRegistration->IsLocked();
        if(oIsLocked || pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pDevice, &pManager, &hPS_Handle) && pDevice && pManager && hPS_Handle)
            {
                if(PT_DEVICE == p_eParameterType)
                {
                    //Get Device Parameter
                    oResult = pDevice->GetParameter(p_Name, p_pValue, p_ulSize);
                    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
                }
                else if(PT_DEVICE > p_eParameterType)
                {
                    //Forward Parameter to next layer
                    oResult = pManager->GetParameter(p_eParameterType, hPS_Handle, p_Name, p_pValue, p_ulSize, pErrorInfo);
                }
            }

            //Unlock
            if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CHandleRegistrationMap_DCS::GetParameter(EParameterType p_eParameterType, HANDLE hDCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo)
{
    CDeviceBase* pDevice = NULL;
    CProtocolStackManagerBase* pManager = NULL;
    HANDLE hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = NULL;
    BOOL oIsLocked = FALSE;
	BOOL oResult = FALSE;

    //GetRegistration
    if(GetRegistration(hDCS_Handle, &pRegistration) && (pRegistration != NULL))
    {
        //Lock
        oIsLocked = pRegistration->IsLocked();
		if(oIsLocked || pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pDevice, &pManager, &hPS_Handle) && pDevice && pManager && hPS_Handle)
            {
                if(PT_DEVICE == p_eParameterType)
                {
                    //Set Device Parameter
                    oResult = pDevice->SetParameter(p_Name, p_rValue);
                    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, pErrorInfo);
                }
                else if(PT_DEVICE > p_eParameterType)
                {
                    //Forward Parameter to next layer
                    oResult = pManager->SetParameter(p_eParameterType, hPS_Handle, p_Name, p_rValue, pErrorInfo);
                }
            }

            //Unlock
            if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

