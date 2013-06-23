#include "stdafx.h"
#include <algorithm>

#include <CommunicationModel/Interface/InterfaceManager.h>
#include "../DeviceCommandSet/DeviceCommandSetManager.h"
#include "Classes/Handle/HandleRegistration_PS.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>

#include <CommunicationModel/ProtocolStack/ProtocolStackManager.h>
#include <CommunicationModel/ProtocolStack/ProtocolStackFactory.h>
#include "Classes/ProtocolStacks/BaseClasses/ProtocolStackBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CProtocolStackManager::CProtocolStackManager():
    m_pHandleRegistrationMap_PS(0)
{
    Init();
    InitErrorHandling();
    InitRegistrationMap();
}

CProtocolStackManager::CProtocolStackManager(int p_lInstanceValue):
    CProtocolStackManagerBase(p_lInstanceValue),
    m_pHandleRegistrationMap_PS(0)
{
    Init();
    InitErrorHandling();
    InitRegistrationMap();
}

CProtocolStackManager::CProtocolStackManager(const CProtocolStackManager& p_rObject):
    CProtocolStackManagerBase(p_rObject),
    m_pHandleRegistrationMap_PS(0)
{
    InitErrorHandling();
    InitRegistrationMap();
}

CProtocolStackManagerBase* CProtocolStackManager::Clone()
{
    CProtocolStackManager* pManager;

    pManager = new CProtocolStackManager(*this);
    return pManager;
}

CProtocolStackManager::~CProtocolStackManager()
{
    ReleaseRegistrationMap();
    DeleteProtocolStackList();
    DeleteRemoteProtocolStackList();
}

BOOL CProtocolStackManager::InitRegistrationMap()
{
    BOOL oResult(FALSE);

    if(!m_pHandleRegistrationMap_PS)
    {
        m_pHandleRegistrationMap_PS = CHandleRegistrationMap_PS::GetInstance(m_lInstanceValue, this);
        if(m_pHandleRegistrationMap_PS)
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackManager::ReleaseRegistrationMap()
{
    BOOL oResult(TRUE);

    if(m_pHandleRegistrationMap_PS)
    {
        CHandleRegistrationMap_PS::ReleaseInstance(m_lInstanceValue, this);
        m_pHandleRegistrationMap_PS = 0;
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_InitAllProtocolStacks(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(TRUE);

    //Reset
    DeleteProtocolStackList();

    std::list<CStdString> rTargets;
    
    oResult = CProtocolStackFactory::GetAllIdentifiers(rTargets);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=rTargets.begin(); it!=rTargets.end() && oResult; it++)
        {
            if(!PS_InitProtocolStack(*it, p_pErrorInfo))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackManager::GetProtocolStack(CStdString p_ProtocolStackName, CProtocolStackBase*& p_rpProtocolStack, BOOL& p_roNewCreated, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    //Init
    p_rpProtocolStack = 0;
    p_roNewCreated = FALSE;

    //Get ProtocolStack
    if(p_ProtocolStackName.size() == 0)
    {
        //Don't create ProtocolStack
        oResult = TRUE;
    }
    else
    {
        //ProtocolStack is already existing
        if(!FindProtocolStack(p_ProtocolStackName, &p_rpProtocolStack))
        {
            //Init
            p_roNewCreated = TRUE;
            
            oResult = CProtocolStackFactory::CreateInstance(p_ProtocolStackName, p_rpProtocolStack);

            if(!oResult)
            {
                //ProtocolStack not found
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
                p_roNewCreated = FALSE;
                oResult = FALSE;
            }
        }
        else
        {
            //Init
            p_roNewCreated = FALSE;
            oResult = TRUE;
        }
    }

    //Init ProtocolStack
    if(oResult && p_rpProtocolStack)
    {
        p_rpProtocolStack->InitInstanceValue(m_lInstanceValue);
    }

    return oResult;
}

BOOL CProtocolStackManager::GetRemoteProtocolStack(CStdString p_ProtocolStackName, CProtocolStackBase*& p_rpProtocolStack, BOOL& p_roNewCreated, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    //Init
    p_rpProtocolStack = 0;
    p_roNewCreated = FALSE;

    //Get ProtocolStack
    if(p_ProtocolStackName.size() == 0)
    {
        //Don't create ProtocolStack
        oResult = TRUE;
    }
    else
    {
        //ProtocolStack is already existing
        if(!FindRemoteProtocolStack(p_ProtocolStackName, &p_rpProtocolStack))
        {
            //Init
            p_roNewCreated = TRUE;
            
            oResult = CProtocolStackFactory::CreateInstance(p_ProtocolStackName, p_rpProtocolStack);

            if(!oResult)
            {
                //ProtocolStack not found
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
                p_roNewCreated = FALSE;
                oResult = FALSE;
            }
        }
        else
        {
            //Init
            p_roNewCreated = FALSE;
            oResult = TRUE;
        }
    }

    //Init ProtocolStack
    if(oResult && p_rpProtocolStack)
    {
        p_rpProtocolStack->InitInstanceValue(m_lInstanceValue);
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_InitProtocolStack(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackBase* pProtocolStack(0);
    BOOL oNewCreated(FALSE);
    BOOL oResult(FALSE);

    //Get ProtocolStack
    if(GetProtocolStack(p_ProtocolStackName, pProtocolStack, oNewCreated, p_pErrorInfo) && pProtocolStack)
    {
        //Init ProtocolStack
        if(pProtocolStack->InitProtocolStack(p_pErrorInfo))
        {
            if(oNewCreated) m_ProtocolStackList.push_back(pProtocolStack);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pProtocolStack;
        }
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_InitProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackBase* pProtocolStack(0);
    BOOL oNewCreated(FALSE);
    BOOL oResult(FALSE);

    //Get ProtocolStack
    if(GetProtocolStack(p_ProtocolStackName, pProtocolStack, oNewCreated, p_pErrorInfo) && pProtocolStack)
    {
        //Init ProtocolStack
        if(pProtocolStack->InitProtocolStack(p_InterfaceName, p_pErrorInfo))
        {
            if(oNewCreated) m_ProtocolStackList.push_back(pProtocolStack);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pProtocolStack;
        }
    }
    return oResult;
}

BOOL CProtocolStackManager::PS_InitProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdString p_BaseLayerProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackBase* pProtocolStack(0);
    BOOL oNewCreated(FALSE);
    BOOL oResult(FALSE);

    //Get ProtocolStack
    if(GetRemoteProtocolStack(p_ProtocolStackName, pProtocolStack, oNewCreated, p_pErrorInfo) && pProtocolStack)
    {
        //Init ProtocolStack
        if(pProtocolStack->InitProtocolStack(p_DeviceName, p_BaseLayerProtocolStackName, p_pErrorInfo))
        {
            if(oNewCreated) m_RemoteProtocolStackList.push_back(pProtocolStack);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pProtocolStack;
        }
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_InitBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdDWordArray& p_ulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackBase* pProtocolStack(0);
    BOOL oResult(TRUE);

    if(FindProtocolStack(p_ProtocolStackName, &pProtocolStack))
    {
        if(pProtocolStack)
        {
            if(!pProtocolStack->InitBaudrateSelection(p_InterfaceName, p_ulBaudrateSel))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_InitDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackBase* pProtocolStack(0);
    BOOL oResult(TRUE);

    if(FindProtocolStack(p_ProtocolStackName, &pProtocolStack))
    {
        if(pProtocolStack)
        {
            if(!pProtocolStack->InitDefaultProtocolStackSettings(p_InterfaceName, p_ulBaudrate, p_ulTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_AddPort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);
    CStdString name;

    //Find protocol stack
    for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
    {
        pProtocolStack = (*it);
        if(pProtocolStack && pProtocolStack->PS_GetProtocolStackName(&name))
        {
            if(p_ProtocolStackName.CompareNoCase(name) == 0)
            {
                oResult = TRUE;
                break;
            }
        }
    }

    if(oResult)
    {
        oResult = pProtocolStack->AddPort(p_InterfaceName, p_PortName, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_DeletePort(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);
    CStdString name;

    //Find protocol stack
    for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
    {
        pProtocolStack = (*it);
        if(pProtocolStack && pProtocolStack->PS_GetProtocolStackName(&name))
        {
            if(p_ProtocolStackName.CompareNoCase(name) == 0)
            {
                oResult = TRUE;
                break;
            }
        }
    }

    if(oResult)
    {
        oResult = pProtocolStack->DeletePort(p_InterfaceName, p_PortName, p_pErrorInfo);
    }
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
    }

    return oResult;
}

void CProtocolStackManager::DeleteProtocolStackList()
{
	for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
	{
		CProtocolStackBase* pProtocolStack = (*it);
		delete pProtocolStack;
		pProtocolStack = 0;
	}

	m_ProtocolStackList.clear();
}

void CProtocolStackManager::DeleteRemoteProtocolStackList()
{
    for(std::list<CProtocolStackBase*>::iterator it = m_RemoteProtocolStackList.begin(); it != m_RemoteProtocolStackList.end(); it++)
    {
        CProtocolStackBase* pProtocolStack = (*it);
        delete pProtocolStack;
        pProtocolStack = 0;
    }

    m_ProtocolStackList.clear();
}

BOOL CProtocolStackManager::FindCorrectManager(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CInterfaceManagerBase** ppManager)
{
    CProtocolStackBase* pProtocol(0);

    if(FindProtocolStack(p_ProtocolStackName, &pProtocol) && pProtocol)
    {
        return pProtocol->FindCorrectManager(p_InterfaceName, ppManager);
    }

    return FALSE;
}

BOOL CProtocolStackManager::FindCorrectManager(CStdString p_ProtocolStackName, CStdString p_DeviceName, CDeviceCommandSetManagerBase** ppManager)
{
    CProtocolStackBase* pProtocolStack(0);

    if(FindRemoteProtocolStack(p_ProtocolStackName, &pProtocolStack) && pProtocolStack)
    {
        return pProtocolStack->FindCorrectManager(p_DeviceName, ppManager);
    }

    return FALSE;
}

BOOL CProtocolStackManager::FindProtocolStackIndex(CStdString p_ProtocolStackName, short* p_psProtocolStackIndex)
{
    CStdString name;
    short sIndex(0);

    for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
	{
		CProtocolStackBase* pProtocolStack = (*it);
		if(pProtocolStack->PS_GetProtocolStackName(&name))
		{
            if(p_ProtocolStackName.CompareNoCase(name))
			{
				if(p_psProtocolStackIndex) *p_psProtocolStackIndex = sIndex;
				return TRUE;
			}
		}

		sIndex++;
	}

    if(p_psProtocolStackIndex) *p_psProtocolStackIndex = -1;
    return FALSE;
}

BOOL CProtocolStackManager::FindRemoteProtocolStackIndex(CStdString p_ProtocolStackName, short* p_psProtocolStackIndex)
{
    CStdString name;
    short sIndex(0);

    for(std::list<CProtocolStackBase*>::iterator it = m_RemoteProtocolStackList.begin(); it != m_RemoteProtocolStackList.end(); it++)
    {
    	CProtocolStackBase* pProtocolStack = (*it);
    	if(pProtocolStack->PS_GetProtocolStackName(&name))
		{
			if(p_ProtocolStackName.CompareNoCase(name))
			{
				if(p_psProtocolStackIndex) *p_psProtocolStackIndex = sIndex;
				return TRUE;
			}
		}

		sIndex++;
    }

    if(p_psProtocolStackIndex) *p_psProtocolStackIndex = -1;
    return FALSE;
}

BOOL CProtocolStackManager::FindProtocolStack(CStdString p_ProtocolStackName, CProtocolStackBase** p_ppProtocolStack)
{
    CStdString name;

    for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
    {
    	CProtocolStackBase* pProtocolStack = (*it);
        if(pProtocolStack && pProtocolStack->PS_GetProtocolStackName(&name))
        {
            if(p_ProtocolStackName.CompareNoCase(name) == 0)
            {
                if(p_ppProtocolStack) *p_ppProtocolStack = pProtocolStack;
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CProtocolStackManager::FindRemoteProtocolStack(CStdString p_ProtocolStackName, CProtocolStackBase** p_ppProtocolStack)
{
    CProtocolStackBase* pProtocolStack(0);
    CStdString name;

    for(std::list<CProtocolStackBase*>::iterator it = m_RemoteProtocolStackList.begin();it != m_RemoteProtocolStackList.end(); it++)
    {
        CProtocolStackBase* pProtocolStack = (*it);
        if(pProtocolStack && pProtocolStack->PS_GetProtocolStackName(&name))
		{
			if(p_ProtocolStackName.CompareNoCase(name) == 0)
			{
				if(p_ppProtocolStack) *p_ppProtocolStack = pProtocolStack;
				return TRUE;
			}
		}
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_IsProtocolStackNameSupported(CStdString p_ProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    if(!FindProtocolStack(p_ProtocolStackName, 0))
    {
        return FindRemoteProtocolStack(p_ProtocolStackName, 0);
    }

    return TRUE;
}

HANDLE CProtocolStackManager::PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceManagerBase* pManager(0);
    CProtocolStackBase* pProtocolStack(0);
    HANDLE hHandle_I;

    if(m_pHandleRegistrationMap_PS)
    {
        //Zugehörigen Manager finden
        if(FindCorrectManager(p_ProtocolStackName, p_InterfaceName, &pManager) && pManager)
        {
            //Suchen des gewünschten Protokolls aus der Liste
            if(FindProtocolStack(p_ProtocolStackName, &pProtocolStack) && pProtocolStack)
            {
				//Logging
				#ifdef _Logging_UsbEnumeration
					pProtocolStack->WriteToLogFile(_T("Event OpenProtocolStack"));
				#endif

				//Force PlugIn Manager Update
				pProtocolStack->UpdateProtocolStack(p_InterfaceName);

				//Init Gateway
				if(pProtocolStack->InitGatewayToInterface(p_InterfaceName))
                {
					hHandle_I = pManager->I_OpenInterface(p_InterfaceName, p_PortName, p_pErrorInfo);
					if(hHandle_I)
					{
                    	pProtocolStack->UpdateTimeout(pManager, hHandle_I);
						return m_pHandleRegistrationMap_PS->RegisterHandle(pProtocolStack, pManager, hHandle_I);
					}

					return 0;
                }
            }

            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
            return 0;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
    return 0;
}

BOOL CProtocolStackManager::PS_CloseProtocolStack(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CInterfaceManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hInterfaceHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hInterfaceHandle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Close Interface
        if(!pManager->I_CloseInterface(hInterfaceHandle, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Delete Registration
        if(!m_pHandleRegistrationMap_PS->DeleteMapRegistration(p_hPS_Handle))
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

BOOL CProtocolStackManager::PS_CloseAllProtocolStacks(CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationPS_List* pRegistrationList(0);

    if(m_pHandleRegistrationMap_PS)
    {
        if(m_pHandleRegistrationMap_PS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
        	tHandleRegistrationPS_List::iterator it = pRegistrationList->begin();

        	while(it != pRegistrationList->end())
        	{
        		CHandleRegistration_PS* pRegistration = (*it);
        		if(pRegistration != 0)
        		{
        			HANDLE hPSHandle = pRegistration->GetKeyHandle();
        			if(hPSHandle)
						if(!PS_CloseProtocolStack(hPSHandle, p_pErrorInfo))
							return FALSE;

        			//Delete Registration
        			pRegistrationList->remove(pRegistration);
                    delete pRegistration;
                    pRegistration = 0;
        		}

        		it = pRegistrationList->begin();
        	}

            return TRUE;
        }
    }

    return FALSE;
}

HANDLE CProtocolStackManager::PS_OpenProtocolStack(CStdString p_ProtocolStackName, CStdString p_DeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceCommandSetManagerBase* pManager(0);
    CProtocolStackBase* pProtocolStack(0);
    HANDLE hHandle_DCS;

    if(m_pHandleRegistrationMap_PS)
    {
        //Zugehörigen Manager finden
        if(FindCorrectManager(p_ProtocolStackName, p_DeviceName, &pManager) && pManager)
        {
            hHandle_DCS = pManager->DCS_OpenDevice(p_DeviceName, p_hPS_BaseLayerHandle, p_pErrorInfo);
            if(hHandle_DCS)
            {
                //Suchen des gewünschten Protokolls aus der Liste
                if(FindRemoteProtocolStack(p_ProtocolStackName, &pProtocolStack) && pProtocolStack)
                {
                    if(pProtocolStack->InitGatewayToDevice(p_DeviceName))
                    {
					    pProtocolStack->UpdateTimeout(pManager, hHandle_DCS);
                        return m_pHandleRegistrationMap_PS->RegisterHandle(pProtocolStack, pManager, hHandle_DCS);
                    }
                }

                pManager->DCS_CloseDevice(hHandle_DCS);
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
                return 0;
            }

            return 0;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
    return 0;
}

BOOL CProtocolStackManager::PS_CloseProtocolStack(HANDLE p_hPS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceCommandSetManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hDeviceCommandSetHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hDeviceCommandSetHandle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Close Device
        if(!pManager->DCS_CloseDevice(hDeviceCommandSetHandle, p_hPS_BaseLayerHandle, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Delete Registration
        if(!m_pHandleRegistrationMap_PS->DeleteMapRegistration(p_hPS_Handle))
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

BOOL CProtocolStackManager::PS_CloseAllProtocolStacks(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    if(m_pHandleRegistrationMap_PS)
    {
    	tHandleRegistrationPS_List* pRegistrationList(0);
        if(m_pHandleRegistrationMap_PS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
        	tHandleRegistrationPS_List::iterator it = pRegistrationList->begin();
            while(it != pRegistrationList->end())
            {
            	CHandleRegistration_PS* pRegistration = (*it);
                if(pRegistration && pRegistration->IsBasedOnDevice())
                {
                	HANDLE hPSHandle = pRegistration->GetKeyHandle();
                    if(hPSHandle)
                        if(!PS_CloseProtocolStack(hPSHandle, p_hPS_BaseLayerHandle, p_pErrorInfo)) return FALSE;

                    //Delete Registration
                    delete pRegistration;
                    pRegistrationList->remove(pRegistration);
                }
                it = pRegistrationList->begin();
            }

            return TRUE;
        }
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_AreAllProtocolStacksClosed()
{
	BOOL oResult(FALSE);

	if(m_pHandleRegistrationMap_PS)
    {
		oResult = m_pHandleRegistrationMap_PS->IsRegistrationMapEmpty();
	}

	return oResult;
}

BOOL CProtocolStackManager::PS_SetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pDeviceCommandSetManager(0);
    CHandleRegistration_PS* pRegistration(0);
    CInterfaceManagerBase* pInterfaceManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hDeviceCommandSetHandle(0);
    HANDLE hInterfaceHandle(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pInterfaceManager, &hInterfaceHandle) && (pInterfaceManager != 0) && (pProtocol != 0))
        {
            //ProtocolStack Timeout
            pProtocol->SetTimeout(p_ulTimeout);

            //Set InterfaceSettings
            if(!pInterfaceManager->I_SetInterfaceSettings(hInterfaceHandle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo))
            {
                if(pRegistration) pRegistration->DoUnlock();
                return FALSE;
            }

            if(pRegistration) pRegistration->DoUnlock();
            return TRUE;
        }
        else if(m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pDeviceCommandSetManager, &hDeviceCommandSetHandle) && (pDeviceCommandSetManager != 0) && (pProtocol != 0))
        {
            //ProtocolStack Timeout
            pProtocol->SetTimeout(p_ulTimeout);

            //Try to write Device Timeout
            pProtocol->SetDeviceTimeout(pDeviceCommandSetManager, hDeviceCommandSetHandle, p_ulTimeout);

            //Forward Timeout
            if(p_ulBaudrate == 0) pDeviceCommandSetManager->DCS_GetProtocolStackSettings(hDeviceCommandSetHandle, &p_ulBaudrate, 0, p_pErrorInfo);
            if(!pDeviceCommandSetManager->DCS_SetProtocolStackSettings(hDeviceCommandSetHandle, p_ulBaudrate, p_ulTimeout, p_oChangeOnly, p_pErrorInfo))
            {
                if(pRegistration) pRegistration->DoUnlock();
                return FALSE;
            }

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

    return FALSE;
}

BOOL CProtocolStackManager::PS_GetProtocolStackSettings(HANDLE p_hPS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pDeviceCommandSetManager(0);
    CHandleRegistration_PS* pRegistration(0);
    CInterfaceManagerBase* pInterfaceManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hDeviceCommandSetHandle(0);
    HANDLE hInterfaceHandle(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pInterfaceManager, &hInterfaceHandle) && (pInterfaceManager != 0) && (pProtocol != 0))
        {
            //Get InterfaceSettings
            if(!pInterfaceManager->I_GetInterfaceSettings(hInterfaceHandle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo))
            {
                if(pRegistration) pRegistration->DoUnlock();
                return FALSE;
            }

            //ProtocolStack Timeout
            if(p_pulTimeout) pProtocol->SetTimeout(*p_pulTimeout);

            if(pRegistration) pRegistration->DoUnlock();
            return TRUE;
        }
        else if(m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pDeviceCommandSetManager, &hDeviceCommandSetHandle) && (pDeviceCommandSetManager != 0) && (pProtocol != 0))
        {
            //ProtocolStack Timeout
            if(p_pulTimeout) *p_pulTimeout = pProtocol->GetTimeout();

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

    return FALSE;
}

BOOL CProtocolStackManager::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);
    CHandleRegistration_PS* pRegistration(0);
    CLayerManagerBase* pManager(0);
    CProtocolStackBase* pProtocolStack(0);
	BOOL oKeepLock = FALSE;
    HANDLE hIorDCS_Handle(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Init
        oResult = TRUE;

        //GetRegistration
        if(oResult && !m_pHandleRegistrationMap_PS->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0))
        {
            oResult = FALSE;
        }

        //Lock (Only if Registration is not locked from last command)
        if(oResult && !pRegistration->IsLocked())
        {
            oResult = pRegistration->DoLock();
        }

        //GetRegistrationEntries
        if(oResult && !m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hHandle, &pProtocolStack, &pManager, &hIorDCS_Handle) ||(pProtocolStack == 0))
        {
            oResult = FALSE;
        }

        //Execute Command
        if(oResult)
        {
            if(CheckLayer(p_pCommand))
            {
                //Execute Command on Protocol Stack Layer
                oResult = pProtocolStack->ExecuteCommand(p_pCommand, pManager, hIorDCS_Handle, p_hTransactionHandle);

				//Check Lock State
				oKeepLock = pProtocolStack->IsLocked();
            }
            else
            {
                //Forward Command To Next Layer
                oResult = pManager->ExecuteCommand(p_pCommand, hIorDCS_Handle, p_hTransactionHandle);

				//Check Lock State
				oKeepLock = p_pCommand->IsSubLayerLocked();
            }
        }

        //Unlock (Only if ProtocolStack does not remain locked until next command)
        if(!oKeepLock)
        {
            //Unlock
            if(pRegistration) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CProtocolStackManager::AbortCommands(HANDLE p_hHandle, BOOL p_oActive)
{
    CLayerManagerBase* pManager(0);
    HANDLE hIorDCS_Handle(0);
    CProtocolStackBase* pProtocolStack(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //GetRegistration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0)) return FALSE;

        //GetRegistrationEntries
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hHandle, &pProtocolStack, &pManager, &hIorDCS_Handle) ||(pProtocolStack == 0))
        {
            return FALSE;
        }

        //AbortCommands
        if(!pProtocolStack->AbortCommands(p_oActive))
        {
            return FALSE;
        }

        //Forward AbortCommands To Next Layer
        if(!pManager->AbortCommands(hIorDCS_Handle, p_oActive))
        {
            return FALSE;
        }

        //Abort Lock
        pRegistration->AbortLock(p_oActive);

        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::GetLayerManager(HANDLE p_hHandle, ELayer p_eLayer, CLayerManagerBase** p_ppLayerManager, HANDLE* pLayerHandle)
{
    CLayerManagerBase* pManager(0);
    HANDLE hIorDCS_Handle(0);
    CProtocolStackBase* pProtocolStack(0);
    CHandleRegistration_PS* pRegistration(0);
    BOOL oResult;

    if(m_pHandleRegistrationMap_PS)
    {
        if(CheckLayer(p_eLayer))
        {
            if(p_ppLayerManager) *p_ppLayerManager = this;
			if(pLayerHandle) *pLayerHandle = p_hHandle;
            return TRUE;
        }
        else
        {
            //GetRegistration
            if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0)) return FALSE;

            //Lock
            if(!pRegistration->DoLock()) return FALSE;

            //GetRegistrationEntries
            if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hHandle, &pProtocolStack, &pManager, &hIorDCS_Handle) ||(pManager == 0))
            {
                //Unlock
                pRegistration->DoUnlock();
                return FALSE;
            }

            //Forward To Next Layer
            oResult = pManager->GetLayerManager(hIorDCS_Handle, p_eLayer, p_ppLayerManager, pLayerHandle);

            //Unlock
            pRegistration->DoUnlock();
            return oResult;
        }
    }

    return FALSE;
}

BOOL CProtocolStackManager::SetInterfaceManager(CStdString p_ProtocolStackName, CInterfaceManagerBase* p_pInterfaceManager)
{
    CProtocolStackBase* pProtocolStack(0);

    if(FindProtocolStack(p_ProtocolStackName, &pProtocolStack) && pProtocolStack)
    {
        return pProtocolStack->AddManager(p_pInterfaceManager);
    }

    return FALSE;
}

BOOL CProtocolStackManager::GetInterfaceManager(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CInterfaceManagerBase** p_ppInterfaceManager)
{
    CInterfaceManagerBase* pManager(0);

    //Zugehörigen Manager finden
    if(FindCorrectManager(p_ProtocolStackName, p_InterfaceName, &pManager) && pManager)
    {
        if(p_ppInterfaceManager) *p_ppInterfaceManager = pManager;
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::SetProtocolStackManager(CStdString p_ProtocolStackName, CStdString p_DeviceName, CProtocolStackManagerBase* p_pProtocolStackManager)
{
    CDeviceCommandSetManagerBase* pManager(0);

    //Zugehörigen Manager finden
    if(FindCorrectManager(p_ProtocolStackName, p_DeviceName, &pManager) && pManager)
    {
        return pManager->SetProtocolStackManager(p_DeviceName, p_pProtocolStackManager);
    }

    return FALSE;
}

BOOL CProtocolStackManager::GetProtocolStackManager(CStdString p_ProtocolStackName, CStdString p_DeviceName, CStdString p_BaseLayerProtocolStackName, CProtocolStackManagerBase** p_ppProtocolStackManager)
{
    CDeviceCommandSetManagerBase* pManager(0);

    //Zugehörigen Manager finden
    if(FindCorrectManager(p_ProtocolStackName, p_DeviceName, &pManager) && pManager)
    {
        return pManager->GetProtocolStackManager(p_DeviceName, p_BaseLayerProtocolStackName, p_ppProtocolStackManager);
    }

    return FALSE;
}

BOOL CProtocolStackManager::GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    CLayerManagerBase* pManager(0);
    HANDLE hIorDCS_Handle(0);
    CProtocolStackBase* pProtocolStack(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //GetRegistrationEntries
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hHandle, &pProtocolStack, &pManager, &hIorDCS_Handle) ||(pProtocolStack == 0))
        {
            return FALSE;
        }

        if(CheckLayer(p_eLayer))
        {
            //GetCommands
            return pProtocolStack->GetCommands(p_pCommandInfo);
        }
        else
        {
            //Forward GetCommands To Next Layer
            return pManager->GetCommands(hIorDCS_Handle, p_eLayer, p_pCommandInfo);
        }
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_SetProtocolStackMode(HANDLE p_hPS_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo)
{
    CLayerManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Set ProtocolStackMode
        if(!pProtocol->PS_SetProtocolStackMode(p_usModeIndex, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_GetProtocolStackMode(HANDLE p_hPS_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo)
{
    CLayerManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get ProtocolStackMode
        if(!pProtocol->PS_GetProtocolStackMode(p_pusModeIndex, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_PS)
    {
        oResult = m_pHandleRegistrationMap_PS->SetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackManager::SetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_PS)
    {
        oResult = m_pHandleRegistrationMap_PS->SetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_Value, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackManager::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_PS)
    {
        oResult = m_pHandleRegistrationMap_PS->GetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackManager::GetParameter(EParameterType p_eParameterType, HANDLE p_hPS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_PS)
    {
        oResult = m_pHandleRegistrationMap_PS->GetParameter(p_eParameterType, p_hPS_Handle, p_Name, p_rValue, p_pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStackManager::PS_GetKeyHandle(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationPS_List* pRegistrationList(0);
    CHandleRegistration_PS* pRegistration(0);

    CStdString strProtocolStackNameReg;
    CStdString strInterfaceNameReg;
    CStdString strPortNameReg;

    BOOL oProtocolStackNameFound(FALSE);
    BOOL oInterfaceNameFound(FALSE);
    BOOL oPortNameFound(FALSE);

    CProtocolStackBase* pProtocolStack(0);
    CInterfaceManagerBase* pManager(0);
    HANDLE hInterfaceHandle(0);
    HANDLE hKeyHandle(0);

    if(m_pHandleRegistrationMap_PS)
    {
        if(m_pHandleRegistrationMap_PS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            for(tHandleRegistrationPS_List::iterator it = pRegistrationList->begin(); it != pRegistrationList->end(); it++)
            {
                pRegistration = (*it);

                pRegistration->GetRegistrationValues(&pProtocolStack, &pManager, &hInterfaceHandle);

                if(!pProtocolStack || !pManager)
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                    return FALSE;
                }

                //Compare ProtocolStackName
                if(pProtocolStack->PS_GetProtocolStackName(&strProtocolStackNameReg))
                {
                    if(p_ProtocolStackName.CompareNoCase(strProtocolStackNameReg) == 0)
                    {
                        oProtocolStackNameFound = TRUE;

                        //Compare InterfaceName
                        if(pManager->I_GetInterfaceName(hInterfaceHandle, &strInterfaceNameReg))
                        {
                        	if(p_InterfaceName.CompareNoCase(strInterfaceNameReg) == 0)
                            {
                                oInterfaceNameFound = TRUE;

                                //Compare PortName
                                if(pManager->I_GetPortName(hInterfaceHandle, &strPortNameReg))
                                {
                                    if(p_PortName.CompareNoCase(strPortNameReg) == 0)
                                    {
                                        oPortNameFound = TRUE;
                                        hKeyHandle = pRegistration->GetKeyHandle();

                                        break;
                                    }
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
                if(!oProtocolStackNameFound)    m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
                else if(!oInterfaceNameFound)   m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
                else if(!oPortNameFound)        m_pErrorHandling->GetError(k_Error_BadPortName, p_pErrorInfo);
                else if(!hKeyHandle)            m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            }
        }
    }

    if(p_pPSKeyHandle) *p_pPSKeyHandle = hKeyHandle;
    return (hKeyHandle != 0);
}

BOOL CProtocolStackManager::PS_GetLayerKeyHandle(CStdString p_ProtocolStackName, CStdString p_DeviceName, CLayerParameterStack &p_rAddressParameter, HANDLE* p_pPSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationPS_List* pRegistrationList(0);
    CHandleRegistration_PS* pRegistration(0);

    CStdString strProtocolStackNameReg;
    CStdString strDeviceNameReg;
    BYTE ubNodeIdReg(0);

    BOOL oProtocolStackNameFound(FALSE);
    BOOL oAddressParameterFound(FALSE);
    BOOL oDeviceNameFound(FALSE);

    CProtocolStackBase* pProtocolStack(0);
    CDeviceCommandSetManagerBase* pManager(0);
    HANDLE hDeviceHandle(0);
    HANDLE hKeyHandle(0);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);
    BYTE ubNodeId(0);

    if(m_pHandleRegistrationMap_PS)
    {
        if(m_pHandleRegistrationMap_PS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            p_rAddressParameter.PeekLayer(DEVICE_COMMAND_SET_LAYER, deviceParameterSet);

            deviceParameterSet.GetParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));

            for(tHandleRegistrationPS_List::iterator it = pRegistrationList->begin(); it != pRegistrationList->end(); it++)
            {
                pRegistration = (*it);
                pRegistration->GetRegistrationValues(&pProtocolStack, &pManager, &hDeviceHandle);

                //Compare ProtocolStackName
                if(pProtocolStack && pProtocolStack->PS_GetProtocolStackName(&strProtocolStackNameReg))
                {
                    if(p_ProtocolStackName.CompareNoCase(strProtocolStackNameReg) == 0)
                    {
                        oProtocolStackNameFound = TRUE;

                        //Compare DeviceName
                        if(pManager && pManager->DCS_GetDeviceName(hDeviceHandle, &strDeviceNameReg))
                        {
							if(p_DeviceName.CompareNoCase(strDeviceNameReg) == 0)
                            {
                                oDeviceNameFound = TRUE;

                                //Compare Address Parameter
                                pManager->GetParameter(PT_DEVICE, hDeviceHandle, _T("NodeId"), &ubNodeIdReg, sizeof(ubNodeIdReg));
                                if(ubNodeId == ubNodeIdReg)
                                {
                                    oAddressParameterFound = TRUE;
                                    hKeyHandle = pRegistration->GetKeyHandle();

                                    break;
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
                if(!oProtocolStackNameFound)        m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
                else if(!oDeviceNameFound)          m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
                else if(!oAddressParameterFound)    m_pErrorHandling->GetError(k_Error_BadAddressParameter, p_pErrorInfo);
                else if(!hKeyHandle)                m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            }
        }
    }

    if(p_pPSKeyHandle) *p_pPSKeyHandle = hKeyHandle;
    return (hKeyHandle != 0);
}

BOOL CProtocolStackManager::PS_GetBaseLayerKeyHandle(HANDLE p_hPS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get BaseLayer ProtocolStack KeyHandle
        if(!pManager->DCS_GetBaseLayerKeyHandle(hHandle, p_pPS_BaseLayerKeyHandle))
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

BOOL CProtocolStackManager::PS_GetProtocolStackName(HANDLE p_hPS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    CLayerManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get ProtocolStackName
        if(!pProtocol->PS_GetProtocolStackName(p_pProtocolStackName))
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

BOOL CProtocolStackManager::PS_GetInterfaceName(HANDLE p_hPS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hInterfaceHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hInterfaceHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetInterfaceName
        if(!pManager->I_GetInterfaceName(hInterfaceHandle, p_pInterfaceName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_GetDeviceName(HANDLE p_hPS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hDeviceCommandSetHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hDeviceCommandSetHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetDeviceName
        if(!pManager->DCS_GetDeviceName(hDeviceCommandSetHandle, p_pDeviceName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_GetPortName(HANDLE p_hPS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hInterfaceHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hInterfaceHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetPortName
        if(!pManager->I_GetPortName(hInterfaceHandle, p_pPortName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_GetProtocolStackNameSelection(CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CStdString strNameSel;

    if(p_pProtocolStackNameSel)
    {
        p_pProtocolStackNameSel->clear();

        for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
        {
        	CProtocolStackBase* pProtocolStack = (*it);
            if(pProtocolStack->PS_GetProtocolStackName(&strNameSel))
                p_pProtocolStackNameSel->push_back(strNameSel);
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}

BOOL CProtocolStackManager::PS_GetInterfaceNameSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindProtocolStack(p_ProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //GetInterfaceNameSelection
    return pProtocolStack->PS_GetInterfaceNameSelection(p_pInterfaceNameSel, p_pErrorInfo);
}

BOOL CProtocolStackManager::PS_GetPortNameSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindProtocolStack(p_ProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //GetPortNameSelection
    return pProtocolStack->PS_GetPortNameSelection(p_InterfaceName, p_pPortSel, p_pErrorInfo);
}

BOOL CProtocolStackManager::PS_GetRemoteProtocolStackNameSelection(CStdStringArray* p_pRemoteProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);
    CStdString strNameSel;

    if(p_pRemoteProtocolStackNameSel)
    {
        p_pRemoteProtocolStackNameSel->clear();

        for(std::list<CProtocolStackBase*>::iterator it = m_RemoteProtocolStackList.begin(); it != m_RemoteProtocolStackList.end(); it++)
		{
			CProtocolStackBase* pProtocolStack = (*it);
			if(pProtocolStack->PS_GetProtocolStackName(&strNameSel))
				p_pRemoteProtocolStackNameSel->push_back(strNameSel);
		}

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}

BOOL CProtocolStackManager::PS_GetDeviceNameSelection(CStdString p_strRemoteProtocolStackName, CStdStringArray* p_pDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindRemoteProtocolStack(p_strRemoteProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //GetDeviceNameSelection
    return pProtocolStack->PS_GetDeviceNameSelection(p_pDeviceNameSel, p_pErrorInfo);
}

BOOL CProtocolStackManager::PS_GetProtocolStackNameSelection(CStdString p_strRemoteProtocolStackName, CStdString p_DeviceName, CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindRemoteProtocolStack(p_strRemoteProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //GetDeviceNameSelection
    return pProtocolStack->PS_GetProtocolStackNameSelection(p_DeviceName, p_pProtocolStackNameSel, p_pErrorInfo);
}

BOOL CProtocolStackManager::PS_GetBaudrateSelection(CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindProtocolStack(p_ProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    return pProtocolStack->PS_GetBaudrateSelection(p_InterfaceName, p_PortName, p_pulBaudrateSel, p_pErrorInfo);
}

BOOL CProtocolStackManager::PS_GetProtocolStackModeSelection(CStdString p_ProtocolStackName, CStdStringArray* p_pProtocolStackModeSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindProtocolStack(p_ProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    return pProtocolStack->PS_GetProtocolStackModeSelection(p_pProtocolStackModeSel, p_pErrorInfo);
}

void CProtocolStackManager::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    //ProtocolStack
    for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
    {
    	CProtocolStackBase* pProtocolStack = (*it);
    	if(pProtocolStack!=0) pProtocolStack->InitJournalManager(p_pJournalManager);
    }

    //Remote ProtocolStack
    for(std::list<CProtocolStackBase*>::iterator it = m_RemoteProtocolStackList.begin(); it != m_RemoteProtocolStackList.end(); it++)
    {
    	CProtocolStackBase* pProtocolStack = (*it);
        if(pProtocolStack!=0) pProtocolStack->InitJournalManager(p_pJournalManager);
    }
}

void CProtocolStackManager::ResetJournalManager()
{
	for(std::list<CProtocolStackBase*>::iterator it = m_ProtocolStackList.begin(); it != m_ProtocolStackList.end(); it++)
	{
		CProtocolStackBase* pProtocolStack = (*it);
		if(pProtocolStack!=0) pProtocolStack->ResetJournalManager();
	}

	//Remote ProtocolStack
	for(std::list<CProtocolStackBase*>::iterator it = m_RemoteProtocolStackList.begin(); it != m_RemoteProtocolStackList.end(); it++)
	{
		CProtocolStackBase* pProtocolStack = (*it);
		if(pProtocolStack!=0) pProtocolStack->ResetJournalManager();
	}
}

BOOL CProtocolStackManager::InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* p_pJournalManager)
{
    CHandleRegistration_PS* pRegistration(0);
    //CInterfaceManagerBase* pManager(0);
	CLayerManagerBase* pManager(0);
    CProtocolStackBase* pProtocolStack(0);
    HANDLE hHandle(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //GetRegistration
        if(m_pHandleRegistrationMap_PS->GetRegistration(p_hHandle, &pRegistration))
        {
            //GetRegistrationEntries
			if(m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hHandle, &pProtocolStack, &pManager, &hHandle))
            {
                if(pProtocolStack) pProtocolStack->InitJournalManager(p_pJournalManager);
                if(pManager) pManager->InitJournalManager(hHandle, p_pJournalManager);

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CProtocolStackManager::ResetJournalManager(HANDLE p_hHandle)
{
    CHandleRegistration_PS* pRegistration(0);
    //CInterfaceManagerBase* pManager(0);
	CLayerManagerBase* pManager(0);
    CProtocolStackBase* pProtocolStack(0);
    HANDLE hHandle(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //GetRegistration
        if(m_pHandleRegistrationMap_PS->GetRegistration(p_hHandle, &pRegistration))
        {
            //GetRegistrationEntries
            if(m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hHandle, &pProtocolStack, &pManager, &hHandle))
            {
                if(pProtocolStack) pProtocolStack->ResetJournalManager();
                if(pManager) pManager->ResetJournalManager(hHandle);

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CProtocolStackManager::PS_GetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindProtocolStack(p_ProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //GetDefaultProtocolStackSettings
    return pProtocolStack->PS_GetDefaultProtocolStackSettings(p_InterfaceName, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
}

BOOL CProtocolStackManager::PS_SetDefaultProtocolStackSettings(CStdString p_ProtocolStackName, CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackBase* pProtocolStack(0);

    //FindProtocolStack
    if(!FindProtocolStack(p_ProtocolStackName, &pProtocolStack) || (pProtocolStack == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
        return FALSE;
    }

    //SetDefaultProtocolStackSettings
    return pProtocolStack->PS_SetDefaultProtocolStackSettings(p_InterfaceName, p_ulBaudrate, p_ulTimeout, p_pErrorInfo);
}

BOOL CProtocolStackManager::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "ProtocolStackManager";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStackManager::EnableTracing(HANDLE p_hPS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hInterfaceHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hInterfaceHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Enable Tracing
        if(!pManager->EnableTracing(hInterfaceHandle, p_TracingFileName, p_pErrorInfo))
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

BOOL CProtocolStackManager::DisableTracing(HANDLE p_hPS_Handle, CErrorInfo* p_pErrorInfo)
{
    CInterfaceManagerBase* pManager(0);
    CProtocolStackBase* pProtocol(0);
    HANDLE hInterfaceHandle(0);
    CHandleRegistration_PS* pRegistration(0);

    if(m_pHandleRegistrationMap_PS)
    {
        //Get Registration
        if(!m_pHandleRegistrationMap_PS->GetRegistration(p_hPS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_PS->GetRegistrationValues(p_hPS_Handle, &pProtocol, &pManager, &hInterfaceHandle) || (pManager == 0) || (pProtocol == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Disable Tracing
        if(!pManager->DisableTracing(hInterfaceHandle, p_pErrorInfo))
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

HANDLE CProtocolStackManager::StartJournal(HANDLE p_hPS_Handle, IJournalEvents* p_pIJournalEvents)
{
    return 0;
}

BOOL CProtocolStackManager::StopJournal(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle)
{
    return FALSE;
}

HANDLE CProtocolStackManager::StartTransaction(HANDLE p_hPS_Handle, HANDLE p_hJournalHandle)
{
    return 0;
}

BOOL CProtocolStackManager::StopTransaction(HANDLE p_hTransactionHandle)
{
    return FALSE;
}

BOOL CProtocolStackManager::StartCommandTime(CJournalTime* p_pThisTime)
{
    return FALSE;
}

BOOL CProtocolStackManager::StopCommandTime(CJournalTime* p_pThisTime)
{
    return FALSE;
}

BOOL CProtocolStackManager::TransactionEvent_VCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}

BOOL CProtocolStackManager::TransactionEvent_DCSLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}

BOOL CProtocolStackManager::TransactionEvent_ProtocolStackLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}

BOOL CProtocolStackManager::TransactionEvent_InterfaceLayer(HANDLE p_hTransactionHandle, CCommandRoot *p_pCommand)
{
    return FALSE;
}

