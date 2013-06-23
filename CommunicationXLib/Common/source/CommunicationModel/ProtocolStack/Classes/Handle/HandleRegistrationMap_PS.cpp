// HandleRegistrationMap_PS.cpp: Implementierung der Klasse CHandleRegistrationMap_PS.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include "../ProtocolStacks/BaseClasses/ProtocolStackBase.h"
#include "HandleRegistration_PS.h"
#include <CommunicationModel/ProtocolStack/Classes/Handle/HandleRegistrationMap_PS.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

std::map<void*, void*>* CHandleRegistrationMap_PS::m_pSingletonMap = 0;

CHandleRegistrationMap_PS::CHandleRegistrationMap_PS():
    m_pErrorHandling(0)
{
    InitErrorHandling();
}

CHandleRegistrationMap_PS::~CHandleRegistrationMap_PS()
{
    DeleteErrorHandling();
    DeleteHandleRegistrationList();
}

BOOL CHandleRegistrationMap_PS::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = _T("HandleRegistrationMap_PS");
    BOOL oResult = FALSE;

    DeleteErrorHandling();
    m_pErrorHandling = new CErrorHandling();
    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        oResult = m_pErrorHandling->InitErrorProducer(&errorProducer);
    }

    return oResult;
}

BOOL CHandleRegistrationMap_PS::DeleteErrorHandling()
{
    if(m_pErrorHandling)
    {
        delete m_pErrorHandling;
        m_pErrorHandling = 0;
    }

    return TRUE;
}

CHandleRegistrationMap_PS* CHandleRegistrationMap_PS::GetInstance(int p_lInstanceValue, void* p_pParent)
{
    CHandleRegistrationMap_PS* pInstance = 0;

    //SingletonMap
    if(!m_pSingletonMap)
    {
        m_pSingletonMap = new std::map<void*, void*>();
    }

    if(m_pSingletonMap)
    {
        CHandleRegistrationMap_PS::CSingleton* pSingleton = 0;
        std::map<void*, void*>::iterator it = m_pSingletonMap->find((void*)p_lInstanceValue);

        //Create Singleton
        if(it == m_pSingletonMap->end())
        {
            pSingleton = new CHandleRegistrationMap_PS::CSingleton();
            m_pSingletonMap->insert(std::pair<void*, void*>((void*)p_lInstanceValue, (void*)pSingleton));
        }
        else
            pSingleton = (CHandleRegistrationMap_PS::CSingleton*)(*it).second;

        //Get Instance
        if(pSingleton)
            pInstance = pSingleton->GetInstance(p_pParent);
    }

    return pInstance;
}

BOOL CHandleRegistrationMap_PS::ReleaseInstance(int p_lInstanceValue, void* p_pParent)
{
    BOOL oResult = FALSE;
    CHandleRegistrationMap_PS::CSingleton* pSingleton = 0;

    if(m_pSingletonMap)
    {
        std::map<void*, void*>::iterator it = m_pSingletonMap->find((void*)p_lInstanceValue);

        if(it != m_pSingletonMap->end())
        {
            pSingleton = (CHandleRegistrationMap_PS::CSingleton*)(*it).second;

            //Relase Instance
            oResult = pSingleton->ReleaseInstance(p_pParent);

            //Delete Singleton
            if(pSingleton->GetParentCount() == 0)
            {
                m_pSingletonMap->erase(it);

                delete pSingleton;
                pSingleton = 0;
            }

            //Delete Map
            if(m_pSingletonMap->size() == 0)
            {
                delete m_pSingletonMap;
                m_pSingletonMap = 0;
            }
        }
    }

    return oResult;
}

void CHandleRegistrationMap_PS::DeleteHandleRegistrationList()
{
    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);
        delete pRegistration;
        pRegistration = 0;
    }

    m_HandleRegistrationList.clear();
}

BOOL CHandleRegistrationMap_PS::GetRegistrationValues(HANDLE p_hProtocolStackHandle, CProtocolStackBase** p_ppProtocolStack, CInterfaceManagerBase** p_ppManager, HANDLE* p_phInterfaceHandle)
{
    CHandleRegistration_PS* pRegistration = 0;

    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);

        if(pRegistration->GetKeyHandle() == p_hProtocolStackHandle)
        {
            return pRegistration->GetRegistrationValues(p_ppProtocolStack, p_ppManager, p_phInterfaceHandle);
        }
    }

    return FALSE;
}

BOOL CHandleRegistrationMap_PS::GetRegistrationValues(HANDLE p_hProtocolStackHandle, CProtocolStackBase** p_ppProtocolStack, CDeviceCommandSetManagerBase** p_ppManager, HANDLE* p_phDeviceCommandSetHandle)
{
    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);

        if(pRegistration->GetKeyHandle() == p_hProtocolStackHandle)
        {
            return pRegistration->GetRegistrationValues(p_ppProtocolStack, p_ppManager, p_phDeviceCommandSetHandle);
        }
    }

    return FALSE;
}

BOOL CHandleRegistrationMap_PS::GetRegistrationValues(HANDLE p_hProtocolStackHandle, CProtocolStackBase** p_ppProtocolStack, CLayerManagerBase** p_ppManager, HANDLE* p_phHandle)
{
    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);

        if(pRegistration->GetKeyHandle() == p_hProtocolStackHandle)
        {
            return pRegistration->GetRegistrationValues(p_ppProtocolStack, p_ppManager, p_phHandle);
        }
    }

    return FALSE;
}

BOOL CHandleRegistrationMap_PS::GetRegistrationList(tHandleRegistrationPS_List** p_ppList)
{
    BOOL oResult = FALSE;

    if(p_ppList)
    {
        *p_ppList = &m_HandleRegistrationList;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CHandleRegistrationMap_PS::IsRegistrationMapEmpty()
{
    BOOL oResult = TRUE;
    CHandleRegistration_PS* pRegistration = 0;

    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);

        if(pRegistration->GetKeyHandle())
        {
            oResult = FALSE;
            break;
        }
    }

    return oResult;
}

BOOL CHandleRegistrationMap_PS::GetRegistration(HANDLE p_hProtocolStackHandle, CHandleRegistration_PS** p_ppRegistration)
{
    if((!p_ppRegistration) || (!p_hProtocolStackHandle))
    {
        return FALSE;
    }

    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        *p_ppRegistration = (*it);

        if((*p_ppRegistration)->GetKeyHandle() == p_hProtocolStackHandle)
        {
            return TRUE;
        }
    }

    return FALSE;
}

//Werte aus der Liste löschen. Position der Liste bleibt bestehen
BOOL CHandleRegistrationMap_PS::DeleteMapRegistration(HANDLE p_hProtocolStackHandle)
{
    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);

        if(pRegistration->GetKeyHandle() == p_hProtocolStackHandle)
        {
            pRegistration->DeleteRegistration();
            return TRUE;
        }
    }

    return FALSE;
}

HANDLE CHandleRegistrationMap_PS::RegisterHandle(CProtocolStackBase* p_pProtocolStack, CInterfaceManagerBase* p_pManager, HANDLE p_hInterfaceHandle)
{
    HANDLE hProtocolStackHandle;

    //Add only one Registration per ProtocolStack
    hProtocolStackHandle = IsRegistrationExisting(p_pProtocolStack, p_pManager, p_hInterfaceHandle);
    if(hProtocolStackHandle == 0)
    {
        hProtocolStackHandle = AddNewRegistration(p_pProtocolStack, p_pManager, p_hInterfaceHandle);
    }

    return hProtocolStackHandle;
}

HANDLE CHandleRegistrationMap_PS::RegisterHandle(CProtocolStackBase* p_pProtocolStack, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDeviceCommandSetHandle)
{
    HANDLE hProtocolStackHandle;

    //Add only one Registration per ProtocolStack
    hProtocolStackHandle = IsRegistrationExisting(p_pProtocolStack, p_pManager, p_hDeviceCommandSetHandle);
    if(hProtocolStackHandle == 0)
    {
        hProtocolStackHandle = AddNewRegistration(p_pProtocolStack, p_pManager, p_hDeviceCommandSetHandle);
    }

    return hProtocolStackHandle;
}

HANDLE CHandleRegistrationMap_PS::IsRegistrationExisting(CProtocolStackBase* p_pProtocolStack, CInterfaceManagerBase* p_pManager, HANDLE p_hInterfaceHandle)
{
    CHandleRegistration_PS* pRegistration = 0;
    HANDLE hHandle = 0;

    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);
        if(pRegistration->IsEqual(p_pProtocolStack, p_pManager, p_hInterfaceHandle))
        {
            hHandle = pRegistration->GetKeyHandle();
            break;
        }
    }

    return hHandle;
}

HANDLE CHandleRegistrationMap_PS::IsRegistrationExisting(CProtocolStackBase* p_pProtocolStack, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDeviceCommandSetHandle)
{
    HANDLE hHandle = 0;

    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);
        if(pRegistration->IsEqual(p_pProtocolStack, p_pManager, p_hDeviceCommandSetHandle))
        {
            hHandle = pRegistration->GetKeyHandle();
            break;
        }
    }

    return hHandle;
}

HANDLE CHandleRegistrationMap_PS::AddNewRegistration(CProtocolStackBase* p_pProtocolStack, CInterfaceManagerBase* p_pManager, HANDLE p_hInterfaceHandle)
{
    CHandleRegistration_PS* pRegistration = 0;
    HANDLE hHandle = 0;

    pRegistration = GetNextFreeRegistration();
    if(pRegistration)
    {
        pRegistration->Init(p_pProtocolStack, p_pManager, p_hInterfaceHandle);
        hHandle = pRegistration->GetKeyHandle();
    }

    return hHandle;
}

HANDLE CHandleRegistrationMap_PS::AddNewRegistration(CProtocolStackBase* p_pProtocolStack, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDeviceCommandSetHandle)
{
    CHandleRegistration_PS* pRegistration = 0;
    HANDLE hHandle = 0;

    pRegistration = GetNextFreeRegistration();
    if(pRegistration)
    {
        pRegistration->Init(p_pProtocolStack, p_pManager, p_hDeviceCommandSetHandle);
        hHandle = pRegistration->GetKeyHandle();
    }

    return hHandle;
}

//In der Liste leere Positionen aus vorherige gelöschte Objekte suchen
//Falls keine Vorhanden neue position einfügen und handle zurückgeben
CHandleRegistration_PS* CHandleRegistrationMap_PS::GetNextFreeRegistration()
{
    DWORD hHandle_PS = ((int)PROTOCOL_STACK_LAYER << 16) + 1;

    for(std::list<CHandleRegistration_PS*>::iterator it = m_HandleRegistrationList.begin(); it != m_HandleRegistrationList.end(); it++)
    {
        CHandleRegistration_PS* pRegistration = (*it);
        if(pRegistration->IsEmpty())
        {
            pRegistration->SetKeyHandle((HANDLE)hHandle_PS);
            return pRegistration;
        }

        hHandle_PS++;
    }

    CHandleRegistration_PS* pRegistration = new CHandleRegistration_PS();
    pRegistration->SetKeyHandle((HANDLE)hHandle_PS);

    m_HandleRegistrationList.push_back(pRegistration);

    return pRegistration;
}

/************************************************************************/
//Embedded Singleton Class
/************************************************************************/
CHandleRegistrationMap_PS::CSingleton::CSingleton():
    m_pRegistrationMap(0)
{
    m_pRegistrationMap = new CHandleRegistrationMap_PS();
}

CHandleRegistrationMap_PS::CSingleton::~CSingleton()
{
    if(m_pRegistrationMap) delete m_pRegistrationMap;
}

CHandleRegistrationMap_PS* CHandleRegistrationMap_PS::CSingleton::GetInstance(void* p_pParent)
{
    bool uFound = false;

    for(std::list<void*>::iterator it = m_ParentRegistrationList.begin(); it != m_ParentRegistrationList.end(); it++)
    {
        if((*it) == p_pParent)
            uFound = true;
    }

    //Register Parent
    if(!uFound)
        m_ParentRegistrationList.push_back(p_pParent);

    //Instance
    return m_pRegistrationMap;
}

BOOL CHandleRegistrationMap_PS::CSingleton::ReleaseInstance(void* p_pParent)
{
    BOOL oResult = FALSE;

    //Unregister parent
    for(std::list<void*>::iterator it = m_ParentRegistrationList.begin(); it != m_ParentRegistrationList.end(); it++)
    {
        if((*it) == p_pParent)
        {
            m_ParentRegistrationList.remove(p_pParent);
            oResult = TRUE;
            break;
        }
    }

    return oResult;
}

int CHandleRegistrationMap_PS::CSingleton::GetParentCount()
{
    return (int)m_ParentRegistrationList.size();
}

BOOL CHandleRegistrationMap_PS::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_ErrorInfo)
{
    CProtocolStackBase* pProtocolStack = 0;
    CLayerManagerBase* pManager = 0;
    HANDLE h_Handle = 0;
    CHandleRegistration_PS* pRegistration = 0;
    BOOL oResult = FALSE;

    //GetRegistration
    if(GetRegistration(p_hPS_Handle, &pRegistration) && (pRegistration != 0))
    {
        //Lock
        if(pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pProtocolStack, &pManager, &h_Handle) && pProtocolStack && pManager && h_Handle)
            {
                if(PT_PROTOCOL_STACK == p_eParameterType)
                {
                    //Set ProtocolStack Parameter
                    oResult = pProtocolStack->SetParameter(p_Name, p_pValue, p_ulSize);
                    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, p_ErrorInfo);
                }
                else
                {
                    //Forward Parameter to next layer
                    oResult = pManager->SetParameter(p_eParameterType, h_Handle, p_Name, p_pValue, p_ulSize, p_ErrorInfo);
                }
            }

            //Unlock
            if(pRegistration) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CHandleRegistrationMap_PS::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackBase* pProtocolStack = 0;
    CLayerManagerBase* pManager = 0;
    HANDLE h_Handle = 0;
    CHandleRegistration_PS* pRegistration = 0;
    BOOL oResult = FALSE;

    //GetRegistration
    if(GetRegistration(p_hPS_Handle, &pRegistration) && (pRegistration != 0))
    {
        //Lock
        if(pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pProtocolStack, &pManager, &h_Handle) && pProtocolStack && pManager && h_Handle)
            {
                if(PT_PROTOCOL_STACK == p_eParameterType)
                {
                    //Set ProtocolStack Parameter
                    oResult = pProtocolStack->SetParameter(p_Name, p_Value);
                    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, p_pErrorInfo);
                }
                else
                {
                    //Forward Parameter to next layer
                    oResult = pManager->SetParameter(p_eParameterType, h_Handle, p_Name, p_Value, p_pErrorInfo);
                }
            }

            //Unlock
            if(pRegistration) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CHandleRegistrationMap_PS::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_ErrorInfo)
{
    CProtocolStackBase* pProtocolStack = 0;
    CLayerManagerBase* pManager = 0;
    HANDLE h_Handle = 0;
    CHandleRegistration_PS* pRegistration = 0;
    BOOL oIsLocked = FALSE;
    BOOL oResult = FALSE;

    //GetRegistration
    if(GetRegistration(p_hPS_Handle, &pRegistration) && (pRegistration != 0))
    {
        //Lock
        oIsLocked = pRegistration->IsLocked();
        if(oIsLocked || pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pProtocolStack, &pManager, &h_Handle) && pProtocolStack && pManager && h_Handle)
            {
                if(PT_PROTOCOL_STACK == p_eParameterType)
                {
                    //Set ProtocolStack Parameter
                    oResult = pProtocolStack->GetParameter(p_Name, p_pValue, p_ulSize);
                    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadParameter, p_ErrorInfo);
                }
                else
                {
                    //Forward Parameter to next layer
                    oResult = pManager->GetParameter(p_eParameterType, h_Handle, p_Name, p_pValue, p_ulSize, p_ErrorInfo);
                }
            }

            //Unlock
            if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CHandleRegistrationMap_PS::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_ErrorInfo)
{
    BOOL oIsLocked = FALSE;
    BOOL oResult = FALSE;
    CHandleRegistration_PS* pRegistration = 0;
    CLayerManagerBase* pManager = 0;
    CProtocolStackBase* pProtocolStack = 0;
    HANDLE h_Handle = 0;

    //GetRegistration
    if(GetRegistration(p_hPS_Handle, &pRegistration) && (pRegistration != 0))
    {
        //Lock
        oIsLocked = pRegistration->IsLocked();
        if(oIsLocked || pRegistration->DoLock())
        {
            if(pRegistration->GetRegistrationValues(&pProtocolStack, &pManager, &h_Handle) && pProtocolStack && pManager && h_Handle)
            {
                if(PT_PROTOCOL_STACK == p_eParameterType)
                {
                    //Set ProtocolStack Parameter
                    oResult = pProtocolStack->GetParameter(p_Name, p_rValue);
                    if(!oResult && m_pErrorHandling)
                        m_pErrorHandling->GetError(k_Error_BadParameter, p_ErrorInfo);
                }
                else if(PT_PROTOCOL_STACK > p_eParameterType)
                {
                    //Forward Parameter to next layer
                    oResult = pManager->GetParameter(p_eParameterType, h_Handle, p_Name, p_rValue, p_ErrorInfo);
                }
            }

            //Unlock
            if(pRegistration && !oIsLocked) pRegistration->DoUnlock();
        }
    }

    return oResult;
}
