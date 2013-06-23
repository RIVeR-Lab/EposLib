#include "stdafx.h"
#include <algorithm>
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>
#include "Classes/Handle/HandleRegistration_I.h"
#include <CommunicationModel/Interface/InterfaceManager.h>
#include "InterfaceFactory.h"
#include "DeviceInfoHandlingFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CInterfaceManager::CInterfaceManager():
    m_pHandleRegistrationMap_I(0)
{
    InitErrorHandling();
    InitRegistrationMap();
    CreateDeviceInfoHandling();
}

CInterfaceManager::CInterfaceManager(int p_lInstanceValue):
    CInterfaceManagerBase(p_lInstanceValue),
    m_pHandleRegistrationMap_I(0)
{
    InitErrorHandling();
    InitRegistrationMap();
    CreateDeviceInfoHandling();
}

CInterfaceManager::CInterfaceManager(const CInterfaceManager& p_rObject):
    CInterfaceManagerBase(p_rObject),
    m_pHandleRegistrationMap_I(0)
{
    InitErrorHandling();
    InitRegistrationMap();
    CreateDeviceInfoHandling();
}

CInterfaceManagerBase* CInterfaceManager::Clone()
{
    CInterfaceManager* pManager;

    pManager = new CInterfaceManager(*this);
    return pManager;
}

CInterfaceManager::~CInterfaceManager()
{
    ReleaseRegistrationMap();
    DeleteDeviceInfoHandling();
    DeleteInterfaceList();
}

BOOL CInterfaceManager::InitRegistrationMap()
{
    BOOL oResult(FALSE);

    if(!m_pHandleRegistrationMap_I)
    {
        m_pHandleRegistrationMap_I = CHandleRegistrationMap_I::GetInstance(m_lInstanceValue, this);
        if(m_pHandleRegistrationMap_I)
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CInterfaceManager::ReleaseRegistrationMap()
{
    BOOL oResult(TRUE);

    if(m_pHandleRegistrationMap_I)
    {
        CHandleRegistrationMap_I::ReleaseInstance(m_lInstanceValue, this);
        m_pHandleRegistrationMap_I = 0;
    }

    return oResult;
}

BOOL CInterfaceManager::CreateDeviceInfoHandling()
{
    BOOL oResult(FALSE);

    std::list<CStdString> aIdentifiers;
    oResult = CInterfaceFactory::GetAllIdentifiers(aIdentifiers);
    if(oResult)
    {
        for(std::list<CStdString>::iterator it=aIdentifiers.begin(); it!=aIdentifiers.end() && oResult; it++)
        {
            CDeviceInfoHandlingBase* pInstance = NULL;
            if(CDeviceInfoHandlingFactory::CreateInstance((*it), pInstance))
            {
                m_DeviceInfoHandlingMap[*it] = pInstance;
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::DeleteDeviceInfoHandling()
{
    BOOL oResult(FALSE);

    for(std::map<CStdString,CDeviceInfoHandlingBase*>::iterator it=m_DeviceInfoHandlingMap.begin(); it!=m_DeviceInfoHandlingMap.end(); it++)
    {
        CDeviceInfoHandlingBase* pDeviceInfoHandlingBase = (*it).second;
        
        if(pDeviceInfoHandlingBase)
            delete pDeviceInfoHandlingBase;

        oResult = TRUE;
    }

    return oResult;
}

BOOL CInterfaceManager::InitDeviceInfoHandling(CInterfaceBase* p_pInterface)
{
    BOOL oResult(FALSE);
    CStdString InterfaceName;
    CDeviceInfoHandlingBase* pInfoHandlingInstance = NULL;
    CInterfaceBase* pInterfaceInstance = NULL;

    oResult = p_pInterface->I_GetInterfaceName(&InterfaceName);

    map<CStdString,CDeviceInfoHandlingBase*>::iterator it = m_DeviceInfoHandlingMap.find(InterfaceName);        
    if(it!=m_DeviceInfoHandlingMap.end())
    {
        pInfoHandlingInstance = (*it).second;
        oResult = TRUE;
    }
    
    if(pInfoHandlingInstance == NULL)
    {
        //try to create
        oResult = CDeviceInfoHandlingFactory::CreateInstance(InterfaceName, pInfoHandlingInstance);
        if(oResult) m_DeviceInfoHandlingMap[InterfaceName] = pInfoHandlingInstance;
    }

    if(oResult)
    {
        oResult = p_pInterface->InitInfoHandling(pInfoHandlingInstance);
    }

    return oResult;
}

BOOL CInterfaceManager::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_I)
    {
        if(CheckLayer(p_pCommand))
        {
            //Init
            oResult = TRUE;

            //GetRegistration
            if(oResult && !m_pHandleRegistrationMap_I->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0))
            {
                oResult = FALSE;
            }

            //Lock (Only if Registration is not locked from last command)
            if(oResult && !pRegistration->IsLocked())
            {
                oResult = pRegistration->DoLock();
            }

            //GetRegistrationEntries
            if(oResult && !m_pHandleRegistrationMap_I->GetRegistrationValues(p_hHandle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
            {
                oResult = FALSE;
            }

            //ExecuteCommand
            if(oResult && !pInterface->ExecuteCommand(p_pCommand, pPort, p_hTransactionHandle))
            {
                oResult = FALSE;
            }

            //Unlock (Only if Interface does not remain locked until next command)
            if(pInterface && !pInterface->IsLocked())
            {
                //Unlock
                if(pRegistration) pRegistration->DoUnlock();
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::AbortCommands(HANDLE p_hHandle, BOOL p_oActive)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //GetRegistration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0)) return FALSE;

        //GetRegistrationEntries
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hHandle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            return FALSE;
        }

        //AbortCommands
        if(!pInterface->AbortCommands(p_oActive))
        {
            return FALSE;
        }

        //Abort Lock
        pRegistration->AbortLock(p_oActive);

        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::GetLayerManager(HANDLE p_hHandle, ELayer p_eLayer, CLayerManagerBase** p_ppLayerManager, HANDLE* p_pLayerHandle)
{
    BOOL oResult(FALSE);

    if(CheckLayer(p_eLayer))
    {
        if(p_ppLayerManager) *p_ppLayerManager = this;
        if(p_pLayerHandle) *p_pLayerHandle = p_hHandle;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CInterfaceManager::GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    BOOL oResult = FALSE;
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Init
        oResult = TRUE;

        //Get registration entries
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hHandle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            oResult = FALSE;
        }

        if(oResult && CheckLayer(p_eLayer))
        {
            //Get commands
            oResult = pPort->GetCommands(p_pCommandInfo);
        }
    }

    return oResult;
}

BOOL CInterfaceManager::I_InitAllInterfaces(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(TRUE);

    //Rest
    DeleteInterfaceList();

    std::list<CStdString> aIdentifiers;
    oResult = CInterfaceFactory::GetAllIdentifiers(aIdentifiers);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=aIdentifiers.begin(); it!=aIdentifiers.end() && oResult; it++)
        {
            if(!I_InitInterface(*it, p_pErrorInfo))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::I_InitInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface = 0;
    BOOL oResult = FALSE;

    //Interface is already existing
    if(!FindInterface(p_InterfaceName, 0))
    {
        std::list<DWORD> rNbOfAvailableBoards;
        oResult = GetNbOfAvailableBoards(p_InterfaceName, rNbOfAvailableBoards);
        int index = 0;
        for(std::list<DWORD>::iterator it=rNbOfAvailableBoards.begin(); it!=rNbOfAvailableBoards.end(); it++)
        {
            WORD wBoardNumber = (WORD)(*it);

            oResult = CInterfaceFactory::CreateInstance(p_InterfaceName, pInterface);

            if(oResult)
            {
                WORD wNbBoardWithOldDriver = 0;

                pInterface->InitInstanceValue(m_lInstanceValue);
                pInterface->InitInterfaceMode(rNbOfAvailableBoards, index, wNbBoardWithOldDriver);
                if(InitDeviceInfoHandling(pInterface))
                {
                    if(pInterface->InitInterface(wBoardNumber,wNbBoardWithOldDriver))
                    {
                        m_InterfaceList.push_back(pInterface);
                        oResult = TRUE;
                    }
                    else
                    {
                        delete pInterface;
                        oResult = FALSE;
                    }
                }
            }
            index++;
        }
    }
    else
    {
        oResult = TRUE;
    }

    if(m_InterfaceList.empty()) oResult = FALSE;

    return oResult;
}

BOOL CInterfaceManager::I_UpdateInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_I)
    {
        std::list<CStdString> rTargets;

        oResult = CInterfaceFactory::GetAllIdentifiers(rTargets);

        if(oResult)
        {
            for(std::list<CStdString>::iterator it=rTargets.begin(); it!=rTargets.end() && oResult; it++)
            {
                CInterfaceBase* pInterface = 0;
                //Interface is existing
                if(FindInterface(p_InterfaceName, &pInterface) && pInterface)
                {
                    tPortList openPortList;
                    if(m_pHandleRegistrationMap_I->GetRegisteredPorts(pInterface, openPortList))
                    {
                        oResult = pInterface->UpdateInterface(openPortList);
                    }
                }
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::I_AddInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(TRUE);
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface(0);

    oResult = CInterfaceFactory::CreateInstance(p_InterfaceName, pInterface);

    if(oResult && pInterface)
    {
        if(!pInterface->I_AddPort(p_PortName))
        {
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CInterfaceManager::I_DeleteInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(TRUE);
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface(0);

    oResult = CInterfaceFactory::CreateInstance(p_InterfaceName, pInterface);

    if(oResult && pInterface)
    {
        if(!pInterface->I_DeletePort(p_PortName))
        {
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CInterfaceManager::I_InitBaudrateSelection(CStdString p_InterfaceName, CStdDWordArray& p_rBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface(0);
    BOOL oResult(TRUE);

    if(FindInterface(p_InterfaceName, &pInterface))
    {
        if(pInterface)
        {
            if(!pInterface->InitBaudrateSelection(p_rBaudrateSel))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::I_InitDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface = 0;
    BOOL oResult(TRUE);

    if(FindInterface(p_InterfaceName, &pInterface))
    {
        if(pInterface)
        {
            if(!pInterface->InitDefaultInterfaceSettings(p_ulBaudrate, p_ulTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

void CInterfaceManager::DeleteInterfaceList()
{
    for(std::list<CInterfaceBase*>::iterator it = m_InterfaceList.begin(); it != m_InterfaceList.end(); it++)
    {
        CInterfaceBase* pInterface = (*it);
        delete pInterface;
    }

    m_InterfaceList.clear();
}

BOOL CInterfaceManager::FindInterfaceIndex(CStdString p_InterfaceName, short* p_puInterfaceIndex)
{
    CStdString strName;
    short index = 0;

    for(std::list<CInterfaceBase*>::iterator it = m_InterfaceList.begin(); it != m_InterfaceList.end(); it++)
    {
        CInterfaceBase* pInterface = (*it);
        if(pInterface->I_GetInterfaceName(&strName))
        {
            if(p_InterfaceName.CompareNoCase(strName) == 0)
            {
                if(p_puInterfaceIndex) *p_puInterfaceIndex = index;
                return TRUE;
            }
        }
        index++;
    }

    if(p_puInterfaceIndex) *p_puInterfaceIndex = -1;
    return FALSE;
}

BOOL CInterfaceManager::FindInterface(CStdString p_InterfaceName, CInterfaceBase** p_ppInterface)
{
    BOOL oResult(FALSE);
    CStdString name;

    for(std::list<CInterfaceBase*>::iterator it = m_InterfaceList.begin(); it != m_InterfaceList.end(); it++)
    {
        CInterfaceBase* pInterface = (*it);
        if(pInterface && pInterface->I_GetInterfaceName(&name))
        {
            if(p_InterfaceName.CompareNoCase(name) == 0)
            {
                if(p_ppInterface) *p_ppInterface = pInterface;
                oResult = TRUE;
                break;
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::DeleteInterface(CStdString p_InterfaceName, BOOL p_oCheckPrefixOnly)
{
    CStdString name(_T(""));
    BOOL oResult(FALSE);

    for(std::list<CInterfaceBase*>::iterator it = m_InterfaceList.begin(); it != m_InterfaceList.end(); it++)
    {
        CInterfaceBase* pInterface = (*it);
        if(pInterface && pInterface->I_GetInterfaceName(&name))
        {
            if((p_oCheckPrefixOnly && name.find(p_InterfaceName) == 0) || (!p_oCheckPrefixOnly && name.CompareNoCase(p_InterfaceName) == 0))
            {
                m_InterfaceList.remove(pInterface);
                delete pInterface;
                oResult = TRUE;
                break;
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::DeleteAllInterfaces(CStdString p_InterfaceName, BOOL p_oCheckPrefixOnly)
{
    BOOL oInterfaceDeleted(TRUE);
    BOOL oResult(TRUE);

    //Delete All Interfaces
    while(oInterfaceDeleted)
    {
        oInterfaceDeleted = DeleteInterface(p_InterfaceName, p_oCheckPrefixOnly);
    }

    return oResult;
}

BOOL CInterfaceManager::I_IsInterfaceNameSupported(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    return FindInterface(p_InterfaceName, 0);
}

BOOL CInterfaceManager::FindInterfacePortIndex(CStdString p_InterfaceName, CStdString p_PortName, short* p_puPortIndex)
{
    BOOL oResult(FALSE);
    CInterfaceBase* pInterface(0);

    if(FindInterface(p_InterfaceName, &pInterface) && pInterface)
    {
        oResult = pInterface->I_FindInterfacePortIndex(p_PortName, p_puPortIndex);
    }

    return oResult;
}

BOOL CInterfaceManager::FindPort(CStdString p_InterfaceName, CStdString p_PortName, CPortBase** p_ppPort)
{
    BOOL oResult(FALSE);
    CInterfaceBase* pInterface(0);

    if(FindInterface(p_InterfaceName, &pInterface) && pInterface)
    {
        oResult = pInterface->I_FindPort(p_InterfaceName, p_PortName, p_ppPort);
    }

    return oResult;
}

HANDLE CInterfaceManager::I_OpenInterface(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface = 0;
    CInterfaceBase* pExistingInterface = 0;
    CPortBase* pPort = 0;
    HANDLE hI_Handle = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Suchen Position der Schnittstellen in der Liste
        if(!FindInterface(p_InterfaceName, &pInterface))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
            return (HANDLE)0;
        }
        //Suche Position des Ports in der Liste
        if(!FindPort(p_InterfaceName, p_PortName, &pPort))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadPortName, p_pErrorInfo);
            return (HANDLE)0;
        }

        //Überprüfen ob Interface & Port schon offen
        hI_Handle = m_pHandleRegistrationMap_I->IsRegistrationExisting(pInterface, p_PortName);
        if(!hI_Handle)
        {
            //Überprüfen ob Interface schon offen
            hI_Handle = m_pHandleRegistrationMap_I->IsRegistrationExisting(pInterface);
            if(hI_Handle)
            {
                //Interface schon offen -> nur Port öffnen
                if(m_pHandleRegistrationMap_I->GetRegistrationValues(hI_Handle, &pExistingInterface, 0) && pExistingInterface)
                {
                    pInterface = pExistingInterface;

                    //Schnittstelle in der Mappe registrieren (-> Clone)
                    hI_Handle = m_pHandleRegistrationMap_I->RegisterHandle(pInterface, pPort);
                    if(hI_Handle)
                    {
                        if(m_pHandleRegistrationMap_I->GetRegistrationValues(hI_Handle, &pInterface, &pPort) && pInterface && pPort)
                        {
                            //Port öffnen
                            if(pInterface->I_OpenInterfacePort(pPort, p_PortName, p_pErrorInfo))
                            {
                                return hI_Handle;
                            }
                            else
                            {
                                m_pHandleRegistrationMap_I->DeleteMapRegistration(hI_Handle);
                                return (HANDLE)0;
                            }
                        }

                        m_pHandleRegistrationMap_I->DeleteMapRegistration(hI_Handle);
                    }

                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                    return (HANDLE)0;
                }
            }
            else
            {
                //Schnittstelle in der Mappe registrieren (-> Clone)
                hI_Handle = m_pHandleRegistrationMap_I->RegisterHandle(pInterface, pPort);
                if(hI_Handle)
                {
                    if(m_pHandleRegistrationMap_I->GetRegistrationValues(hI_Handle, &pInterface, &pPort) && pInterface && pPort)
                    {
                        //Interface & Port öffnen
                        if(pInterface->I_OpenInterface(p_pErrorInfo))
                        {
                            if(pInterface->I_OpenInterfacePort(pPort, p_PortName, p_pErrorInfo))
                            {
                                return hI_Handle;
                            }
                            else
                            {
                                m_pHandleRegistrationMap_I->DeleteMapRegistration(hI_Handle);
                                return (HANDLE)0;
                            }
                        }
                        else
                        {
                            m_pHandleRegistrationMap_I->DeleteMapRegistration(hI_Handle);
                            return (HANDLE)0;
                        }
                    }

                    m_pHandleRegistrationMap_I->DeleteMapRegistration(hI_Handle);
                }

                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                return (HANDLE)0;
            }
        }
    }

    return hI_Handle;
}

BOOL CInterfaceManager::I_CloseInterface(HANDLE p_hI_Handle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;
    int portCount = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Anzahl geöffnete Ports pro Interface ermitteln
        portCount = m_pHandleRegistrationMap_I->GetRegistrationCount(pInterface);

        //InterfacePort schliessen
        if(pInterface->I_CloseInterfacePort(pPort, p_pErrorInfo))
        {
            //Interface schliessen
            if(portCount == 1)
            {
                if(pInterface->I_CloseInterface(p_pErrorInfo))
                {
                    m_pHandleRegistrationMap_I->DeleteMapRegistration(p_hI_Handle);
                    if(pRegistration) pRegistration->DoUnlock();
                    return TRUE;
                }
                else
                {
                    m_pHandleRegistrationMap_I->DeleteMapRegistration(p_hI_Handle);
                    if(pRegistration) pRegistration->DoUnlock();
                    return FALSE;
                }
            }
            else
            {
                //Registrierte Werte aus der Mappe löschen
                if(m_pHandleRegistrationMap_I->DeleteMapRegistration(p_hI_Handle))
                {
                    if(pRegistration) pRegistration->DoUnlock();
                    return TRUE;
                }
                else
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                    if(pRegistration) pRegistration->DoUnlock();
                    return FALSE;
                }
            }
        }
        else
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }
    }

    return FALSE;
}

BOOL CInterfaceManager::I_CloseAllInterfaces(CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationI_List* pRegistrationList;

    if(m_pHandleRegistrationMap_I)
    {
        if(m_pHandleRegistrationMap_I->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            while(pRegistrationList->size() > 0)
            {
                tHandleRegistrationI_List::iterator it = pRegistrationList->begin();
                if(it != pRegistrationList->end())
                {
                    CHandleRegistration_I* pRegistration = (*it);
                    if(pRegistration)
                    {
                        HANDLE hIHandle = pRegistration->GetKeyHandle();
                        if(hIHandle)
                            if(!I_CloseInterface(hIHandle, p_pErrorInfo)) return FALSE;

                        pRegistrationList->remove(pRegistration);

                        //Delete Registration
                        delete pRegistration;
                    }
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}

BOOL CInterfaceManager::I_AreAllInterfacesClosed()
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_I)
    {
        oResult = m_pHandleRegistrationMap_I->IsRegistrationMapEmpty();
    }

    return oResult;
}

BOOL CInterfaceManager::I_SetInterfaceSettings(HANDLE p_hI_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //SetInterfaceSettings
        if(!pInterface->I_SetInterfaceSettings(pPort, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_GetInterfaceSettings(HANDLE p_hI_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetInterfaceSettings
        if(!pInterface->I_GetInterfaceSettings(pPort, p_pulBaudrate, p_pulTimeout, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_GetInterfaceMode(HANDLE p_hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetInterfaceMode
        if(!pInterface->I_GetInterfaceMode(p_pusModeIndex, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_SetInterfaceMode(HANDLE p_hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //SetInterfaceMode
        if(!pInterface->I_SetInterfaceMode(p_usModeIndex, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_GetPortMode(HANDLE p_hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetPortMode
        if(!pPort->GetPortMode(p_pusModeIndex, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_SetPortMode(HANDLE p_hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //SetPortMode
        if(!pPort->SetPortMode(p_usModeIndex, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_ResetInterface(HANDLE p_hI_Handle, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //ResetInterface
        if(!pInterface->I_ResetInterface(pPort, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::SetParameter(EParameterType p_eParameterType, HANDLE p_hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_I)
    {
        oResult = m_pHandleRegistrationMap_I->SetParameter(p_eParameterType, p_hI_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CInterfaceManager::SetParameter(EParameterType p_eParameterType, HANDLE p_hI_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_I)
    {
        oResult = m_pHandleRegistrationMap_I->SetParameter(p_eParameterType, p_hI_Handle, p_Name, p_Value, p_pErrorInfo);
    }

    return oResult;
}

BOOL CInterfaceManager::GetParameter(EParameterType p_eParameterType, HANDLE p_hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_I)
    {
        oResult = m_pHandleRegistrationMap_I->GetParameter(p_eParameterType, p_hI_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CInterfaceManager::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_I)
    {
        oResult = m_pHandleRegistrationMap_I->GetParameter(p_eParameterType, hVCS_Handle, p_Name, p_rValue, p_pErrorInfo);
    }

    return oResult;
}

//Schnittstellen-Handle der offenen Schnittstelle holen
BOOL CInterfaceManager::I_GetKeyHandle(CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_pIKeyHandle, CErrorInfo* p_pErrorInfo)
{
    HANDLE hKeyHandle = 0;
    BOOL oInterfaceNameFound = FALSE;
    BOOL oPortNameFound = FALSE;
    CHandleRegistration_I* pRegistration = 0;
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CStdString strInterfaceNameReg;
    CStdString strPortNameReg;
    tHandleRegistrationI_List* pRegistrationList = 0;

    if(m_pHandleRegistrationMap_I)
    {
        if(m_pHandleRegistrationMap_I->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            for(tHandleRegistrationI_List::iterator it = pRegistrationList->begin(); it != pRegistrationList->end(); it++)
            {
                pRegistration = (*it);

                pRegistration->GetRegistrationValues(&pInterface, &pPort);

                if(!pInterface || !pPort)
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                    return FALSE;
                }

                //Compare InterfaceName
                if(pInterface->I_GetInterfaceName(&strInterfaceNameReg))
                {
                    if(strInterfaceNameReg.CompareNoCase(p_InterfaceName) == 0)
                    {
                        oInterfaceNameFound = TRUE;

                        //Compare PortName
                        if(pPort->GetPortName(&strPortNameReg))
                        {
                            //0 = string1 identical to string2
                            if(strPortNameReg.CompareNoCase(p_PortName) == 0)
                            {
                                oPortNameFound = TRUE;
                                hKeyHandle = pRegistration->GetKeyHandle();
                            }
                        }
                    }
                }
            }
        }
    }

    //ErrorHandling
    if(hKeyHandle == 0)
    {
        if(m_pErrorHandling)
        {
            if(!oInterfaceNameFound)    m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
            else if(!oPortNameFound)    m_pErrorHandling->GetError(k_Error_BadPortName, p_pErrorInfo);
            else if(!hKeyHandle)        m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        }
    }

    if(p_pIKeyHandle) *p_pIKeyHandle = hKeyHandle;
    return (hKeyHandle != 0);
}

BOOL CInterfaceManager::I_GetInterfaceName(HANDLE p_hI_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetInterfaceName
        if(!pInterface->I_GetInterfaceName(p_pInterfaceName))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_GetInterfaceNameSelection(CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CStdString strNameSel;

    if(p_pInterfaceNameSel)
    {
        p_pInterfaceNameSel->clear();

        for(std::list<CInterfaceBase*>::iterator it = m_InterfaceList.begin(); it != m_InterfaceList.end(); it++)
        {
            CInterfaceBase* pInterface = (*it);
            if(pInterface->I_GetInterfaceName(&strNameSel))
                p_pInterfaceNameSel->push_back(strNameSel);
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}

BOOL CInterfaceManager::I_GetPortName(HANDLE p_hI_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetPortName
        if(!pPort->GetPortName(p_pPortName))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::I_GetPortNameSelection(CStdString p_InterfaceName, CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface = 0;

    //FindInterface
    if(!FindInterface(p_InterfaceName, &pInterface) || (pInterface == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
        return FALSE;
    }

    //GetPortNameSelection
    return pInterface->I_GetPortNameSelection(p_pPortSel, p_pErrorInfo);
}

BOOL CInterfaceManager::I_GetBaudrateSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface = 0;

    //FindInterface
    if(!FindInterface(p_InterfaceName, &pInterface) || (pInterface == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    return pInterface->I_GetBaudrateSelection(p_PortName, p_pulBaudrateSel, p_pErrorInfo);
}

BOOL CInterfaceManager::I_GetInterfaceModeSelection(CStdString p_InterfaceName, CStdStringArray* p_pInterfaceModeSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface = 0;

    //FindInterface
    if(!FindInterface(p_InterfaceName, &pInterface) || (pInterface == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
        return FALSE;
    }

    //GetInterfaceModeSelection
    return pInterface->I_GetInterfaceModeSelection(p_pInterfaceModeSel, p_pErrorInfo);
}

BOOL CInterfaceManager::I_GetPortModeSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdStringArray* p_pPortModeSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CPortBase* pPort(0);

    //FindPort
    if(!FindPort(p_InterfaceName, p_PortName, &pPort) || (pPort == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadPortName, p_pErrorInfo);
        return FALSE;
    }

    //GetPortModeSelection
    return pPort->GetPortModeSelection(p_pPortModeSel, p_pErrorInfo);
}

BOOL CInterfaceManager::GetNbOfAvailableBoards(CStdString p_Name, std::list<DWORD> & p_rNbOfAvailableBoards)
{
    BOOL oResult = FALSE;
    CInterfaceBase* pInterface = NULL;

    oResult = CInterfaceFactory::CreateInstance(p_Name, pInterface);

    if(oResult)
    {
        if(pInterface)
        {
            p_rNbOfAvailableBoards = pInterface->GetNbOfAvailableBoards();
            delete pInterface;
            oResult = TRUE;
        }
        else
        {
            oResult = FALSE;
        }
    }

    return oResult;
}

void CInterfaceManager::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    for(std::list<CInterfaceBase*>::iterator it = m_InterfaceList.begin(); it != m_InterfaceList.end(); it++)
        {
            CInterfaceBase* pInterface = (*it);
        pInterface->InitJournalManager(p_pJournalManager);
    }
}

void CInterfaceManager::ResetJournalManager()
{
    for(std::list<CInterfaceBase*>::iterator it = m_InterfaceList.begin(); it != m_InterfaceList.end(); it++)
        {
            CInterfaceBase* pInterface = (*it);
        pInterface->ResetJournalManager();
    }
}

BOOL CInterfaceManager::InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* p_pJournalManager)
{
    BOOL oResult(FALSE);
    CHandleRegistration_I* pRegistration(0);
    CInterfaceBase* pInterface(0);
    CPortBase* pPort(0);

    if(m_pHandleRegistrationMap_I)
    {
        //GetRegistration
        if(m_pHandleRegistrationMap_I->GetRegistration(p_hHandle, &pRegistration))
        {
            //GetRegistrationEntries
            if(m_pHandleRegistrationMap_I->GetRegistrationValues(p_hHandle, &pInterface, &pPort))
            {
                if(pInterface) pInterface->InitJournalManager(p_pJournalManager);
                if(pPort) pPort->InitJournalManager(p_hHandle, p_pJournalManager);

                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::ResetJournalManager(HANDLE p_hHandle)
{
    BOOL oResult(FALSE);
    CHandleRegistration_I* pRegistration = 0;
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //GetRegistration
        if(m_pHandleRegistrationMap_I->GetRegistration(p_hHandle, &pRegistration))
        {
            //GetRegistrationEntries
            if(m_pHandleRegistrationMap_I->GetRegistrationValues(p_hHandle, &pInterface, &pPort))
            {
                if(pInterface) pInterface->ResetJournalManager();
                if(pPort) pPort->ResetJournalManager(p_hHandle);

                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CInterfaceManager::I_GetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface(0);

    //FindInterface
    if(!FindInterface(p_InterfaceName, &pInterface) || (pInterface == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
    }
    else
    {
        //GetDefaultInterfaceSettings
        oResult = pInterface->I_GetDefaultInterfaceSettings(p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }

    return oResult;
}

BOOL CInterfaceManager::I_SetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceBase* pInterface(0);

    //FindInterface
    if(!FindInterface(p_InterfaceName, &pInterface) || (pInterface == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
    }
    else
    {
        //SetDefaultInterfaceSettings
        oResult = pInterface->I_SetDefaultInterfaceSettings(p_ulBaudrate, p_ulTimeout, p_pErrorInfo);
    }

    return oResult;
}

BOOL CInterfaceManager::InitErrorHandling()
{
    BOOL oResult(FALSE);
    CErrorProducer errorProducer;
    CStdString strClassName(_T("InterfaceManager"));

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(INTERFACE_LAYER, strClassName);
        oResult = m_pErrorHandling->InitErrorProducer(&errorProducer);
    }

    return oResult;
}

BOOL CInterfaceManager::EnableTracing(HANDLE p_hI_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Enable Tracing
        if(!pInterface->EnableTracing(pPort, p_TracingFileName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Unlock
        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CInterfaceManager::DisableTracing(HANDLE p_hI_Handle, CErrorInfo* p_pErrorInfo)
{
    CInterfaceBase* pInterface = 0;
    CPortBase* pPort = 0;
    CHandleRegistration_I* pRegistration = 0;

    if(m_pHandleRegistrationMap_I)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_I->GetRegistration(p_hI_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        //Get Registration Values
        if(!m_pHandleRegistrationMap_I->GetRegistrationValues(p_hI_Handle, &pInterface, &pPort) || (pInterface == 0) || (pPort == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Disable Tracing
        if(!pInterface->DisableTracing(pPort, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Unlock
        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}
