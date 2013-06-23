#include "stdafx.h"

#include <algorithm>
#include <CommunicationModel/ProtocolStack/ProtocolStackManagerBase.h>
#include "Classes/Handle/HandleRegistration_DCS.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include "DeviceCommandSetManager.h"
#include "DeviceCommandSetFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CDeviceCommandSetManager::CDeviceCommandSetManager():
    m_pHandleRegistrationMap_DCS(0)
{
    InitErrorHandling();
    InitRegistrationMap();
}

CDeviceCommandSetManager::CDeviceCommandSetManager(int p_lInstanceValue):
    CDeviceCommandSetManagerBase(p_lInstanceValue),
    m_pHandleRegistrationMap_DCS(0)
{
    InitErrorHandling();
    InitRegistrationMap();
}

CDeviceCommandSetManager::CDeviceCommandSetManager(const CDeviceCommandSetManager& rObject):
    CDeviceCommandSetManagerBase(rObject),
    m_pHandleRegistrationMap_DCS(0)
{
    InitErrorHandling();
    InitRegistrationMap();
}

BOOL CDeviceCommandSetManager::InitRegistrationMap()
{
    BOOL oResult = FALSE;

    if(!m_pHandleRegistrationMap_DCS)
    {
        m_pHandleRegistrationMap_DCS = CHandleRegistrationMap_DCS::GetInstance(m_lInstanceValue, this);
        if(m_pHandleRegistrationMap_DCS)
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::ReleaseRegistrationMap()
{
    BOOL oResult = TRUE;

    if(m_pHandleRegistrationMap_DCS)
    {
        CHandleRegistrationMap_DCS::ReleaseInstance(m_lInstanceValue, this);
        m_pHandleRegistrationMap_DCS = 0;
    }

    return oResult;
}

CDeviceCommandSetManagerBase* CDeviceCommandSetManager::Clone()
{
    CDeviceCommandSetManager* pManager;

    pManager = new CDeviceCommandSetManager(*this);
    return pManager;
}

CDeviceCommandSetManager::~CDeviceCommandSetManager()
{
    ReleaseRegistrationMap();
    DeleteDeviceList();
}

BOOL CDeviceCommandSetManager::InitErrorHandling()
{
    BOOL oResult(FALSE);
    CErrorProducer errorProducer;
    CStdString className(_T("DeviceCommandSetManager"));

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(DEVICE_COMMAND_SET_LAYER, className);
        oResult = m_pErrorHandling->InitErrorProducer(&errorProducer);
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::DCS_InitAllDevices(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    //Reset
    DeleteDeviceList();

    std::list<CStdString> rTargets;
    
    oResult = CDeviceCommandSetFactory::GetAllIdentifiers(rTargets);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=rTargets.begin(); it!=rTargets.end() && oResult; it++)
        {
            if(!DCS_InitDevice(*it, p_pErrorInfo))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::GetDevice(CStdString p_strDeviceName, CDeviceBase*& p_rpDevice, BOOL& p_roNewCreated, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    //Init
    p_rpDevice = 0;
    p_roNewCreated = FALSE;

    //Create Device
    if(p_strDeviceName.size() == 0)
    {
        //Don't create a Device
        oResult = TRUE;
    }
    else
    {
        //Device is already existing
        if(!FindDevice(p_strDeviceName, &p_rpDevice))
        {
            //Init
            p_roNewCreated = TRUE;
            
            oResult = CDeviceCommandSetFactory::CreateInstance(p_strDeviceName, p_rpDevice);
            
            if(!oResult)
            {
                //Device not found
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
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

    //Init Device
    if(oResult && p_rpDevice)
    {
        p_rpDevice->InitInstanceValue(m_lInstanceValue);
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::DCS_InitDevice(CStdString p_strDeviceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    //Get Device
    if(GetDevice(p_strDeviceName, pDevice, oNewCreated, p_pErrorInfo) && pDevice)
    {
        //Init Device
        if(pDevice->InitDevice(p_pErrorInfo))
        {
            if(oNewCreated) m_DeviceList.push_back(pDevice);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pDevice;
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::DCS_InitDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    //Get Device
    if(GetDevice(p_strDeviceName, pDevice, oNewCreated, p_pErrorInfo) && pDevice)
    {
        //Init Device
        if(pDevice->InitDevice(p_strProtocolStackName, p_pErrorInfo))
        {
            if(oNewCreated) m_DeviceList.push_back(pDevice);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pDevice;
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::DCS_InitDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    //Get Device
    if(GetDevice(p_strDeviceName, pDevice, oNewCreated, p_pErrorInfo) && pDevice)
    {
        //Init Device
        if(pDevice->InitDevice(p_strProtocolStackName, p_strInterfaceName, p_pErrorInfo))
        {
            if(oNewCreated) m_DeviceList.push_back(pDevice);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pDevice;
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::DCS_InitBaudrateSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdDWordArray& p_ulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    BOOL oResult = TRUE;

    if(FindDevice(p_strDeviceName, &pDevice))
    {
        if(pDevice)
        {
            if(!pDevice->InitBaudrateSelection(p_strProtocolStackName, p_strInterfaceName, p_ulBaudrateSel))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::DCS_InitDefaultProtocolStackSettings(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD p_ulBaudrateSel, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    BOOL oResult = TRUE;

    if(FindDevice(p_strDeviceName, &pDevice))
    {
        if(pDevice)
        {
            if(!pDevice->InitDefaultProtocolStackSettings(p_strProtocolStackName, p_strInterfaceName, p_ulBaudrateSel, p_ulTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

void CDeviceCommandSetManager::DeleteDeviceList()
{
    CDeviceBase* pDevice = 0;

    for(std::list<CDeviceBase*>::iterator it = m_DeviceList.begin(); it != m_DeviceList.end(); it++)
    {
        CDeviceBase* pDevice = (*it);
        delete pDevice;
    }

    m_DeviceList.clear();
}

BOOL CDeviceCommandSetManager::FindCorrectManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CProtocolStackManagerBase** ppManager)
{
    CDeviceBase* pDevice = 0;

    if(FindDevice(p_strDeviceName, &pDevice) && pDevice)
    {
        return pDevice->FindCorrectManager(p_strProtocolStackName, ppManager);
    }
    return FALSE;
}

BOOL CDeviceCommandSetManager::FindCorrectManager(CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, CProtocolStackManagerBase** ppManager)
{
    CDeviceBase* pDevice = 0;

    if(FindDevice(p_strDeviceName, &pDevice) && pDevice)
    {
        return pDevice->FindCorrectManager(p_hPS_BaseLayerHandle, ppManager);
    }
    return FALSE;
}

BOOL CDeviceCommandSetManager::FindDeviceIndex(CStdString p_strDeviceName, short* piDeviceIndex)
{
    CStdString strName;

    short index = 0;

    for(std::list<CDeviceBase*>::iterator it = m_DeviceList.begin(); it != m_DeviceList.end(); it++)
    {
          CDeviceBase* pDevice = (*it);
        if(pDevice->DCS_GetDeviceName(&strName))
        {
            if(strName.CompareNoCase(p_strDeviceName) == 0)
            {
                if(piDeviceIndex) *piDeviceIndex = index;
                return TRUE;
            }
        }
        index++;
    }

    if(piDeviceIndex) *piDeviceIndex = -1;
    return FALSE;
}

BOOL CDeviceCommandSetManager::FindDevice(CStdString p_strDeviceName, CDeviceBase** ppDevice)
{
    CStdString strName;

    for(std::list<CDeviceBase*>::iterator it = m_DeviceList.begin(); it != m_DeviceList.end(); it++)
    {
        CDeviceBase* pDevice = (*it);
        if(pDevice && pDevice->DCS_GetDeviceName(&strName))
        {
            if(p_strDeviceName.CompareNoCase(strName) == 0)
            {
                if(ppDevice) *ppDevice = pDevice;
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_IsDeviceNameSupported(CStdString p_strDeviceName, CErrorInfo* p_pErrorInfo)
{
    return FindDevice(p_strDeviceName, 0);
}

HANDLE CDeviceCommandSetManager::DCS_OpenDevice(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    HANDLE hHandle_PS;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Zugehörigen Manager finden
        if(FindCorrectManager(p_strDeviceName, p_strProtocolStackName, &pManager) && pManager)
        {
            hHandle_PS = pManager->PS_OpenProtocolStack(p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_pErrorInfo);
            if(hHandle_PS)
            {
                //Zugehörige Device finden und Daten in der Mappe registrieren!
                if(FindDevice(p_strDeviceName, &pDevice) && pDevice)
                {
                    if(pDevice->InitGateway(p_strProtocolStackName))
                    {
                        return m_pHandleRegistrationMap_DCS->RegisterHandle(pDevice, pManager, hHandle_PS);
                    }
                }

                pManager->PS_CloseProtocolStack(hHandle_PS);
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
                return 0;
            }
            return 0;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
    return 0;
}

BOOL CDeviceCommandSetManager::DCS_CloseDevice(HANDLE p_hDCS_Handle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Close Only if no other Handles are registered
        if(m_pHandleRegistrationMap_DCS->IsLastRegistration(p_hDCS_Handle, p_hPS_Handle))
        {
            //Close ProtocolStack
            if(!pManager->PS_CloseProtocolStack(p_hPS_Handle, p_pErrorInfo))
            {
                if(pRegistration) pRegistration->DoUnlock();
                return FALSE;
            }
        }

        //Delete Registration
        if(!m_pHandleRegistrationMap_DCS->DeleteMapRegistration(p_hDCS_Handle))
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

BOOL CDeviceCommandSetManager::DCS_CloseAllDevices(CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationDCS_List* pRegistrationList;
    CHandleRegistration_DCS* pRegistration = 0;
    HANDLE hDCSHandle;

    if(m_pHandleRegistrationMap_DCS)
    {
        if(m_pHandleRegistrationMap_DCS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            while(pRegistrationList->size() > 0)
            {
                tHandleRegistrationDCS_List::iterator it = pRegistrationList->begin();

                pRegistration = (*it);

                if(pRegistration)
                {
                    hDCSHandle = pRegistration->GetKeyHandle();
                    if(hDCSHandle)
                    {
                        if(!DCS_CloseDevice(hDCSHandle, p_pErrorInfo)) return FALSE;
                    }

                    //Delete Registration
                    delete pRegistration;
                    pRegistrationList->erase(it);
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}

HANDLE CDeviceCommandSetManager::DCS_OpenDevice(CStdString p_strDeviceName, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    CStdString protocolStackName = _T("");
    HANDLE hHandle_PS = 0;
    HANDLE hHandle_DCS = 0;
    BOOL oResult = TRUE;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Zugehörigen Manager finden, check if ProtocolStack is supported
        if(oResult && !(FindCorrectManager(p_strDeviceName, p_hPS_BaseLayerHandle, &pManager) && pManager))
        {
            //Bad ProtocolStack Handle
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackHandle, p_pErrorInfo);
            oResult = FALSE;
        }

        //ProtocolStack Name
        if(oResult && !pManager->PS_GetProtocolStackName(p_hPS_BaseLayerHandle, &protocolStackName, p_pErrorInfo))
        {
            //Bad ProtocolStack Handle
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackHandle, p_pErrorInfo);
            oResult = FALSE;
        }

        //Zugehörige Device finden
        if(oResult && !(FindDevice(p_strDeviceName, &pDevice) && pDevice))
        {
            //Bad Device Name
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
            oResult = FALSE;
        }

        //Gateway
        if(oResult && !pDevice->InitGateway(protocolStackName))
        {
            //Bad ProtocolStack Handle
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadProtocolStackHandle, p_pErrorInfo);
            oResult = FALSE;
        }

        //Registration
        if(oResult)
        {
            hHandle_DCS = m_pHandleRegistrationMap_DCS->RegisterHandle(pDevice, pManager, p_hPS_BaseLayerHandle);
        }
    }
    else
    {
        //Internal Error
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    }

    return hHandle_DCS;
}

BOOL CDeviceCommandSetManager::DCS_CloseDevice(HANDLE p_hDCS_Handle, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Delete Registration
        if(!m_pHandleRegistrationMap_DCS->DeleteMapRegistration(p_hDCS_Handle))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Unlock
        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_CloseAllDevices(HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationDCS_List* pRegistrationList;
    CHandleRegistration_DCS* pRegistration = 0;
    HANDLE hDCSHandle = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        if(m_pHandleRegistrationMap_DCS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            while(pRegistrationList->size() > 0)
            {
                tHandleRegistrationDCS_List::iterator it = pRegistrationList->begin();

                pRegistration = (*it);

                if(pRegistration)
                {
                    hDCSHandle = pRegistration->GetKeyHandle();
                    if(hDCSHandle)
                    {
                        if(!DCS_CloseDevice(hDCSHandle, p_hPS_BaseLayerHandle, p_pErrorInfo)) return FALSE;
                    }

                    //Delete Registration
                    delete pRegistration;
                    pRegistrationList->erase(it);
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_AreAllDevicesClosed()
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_DCS)
    {
        oResult = m_pHandleRegistrationMap_DCS->IsRegistrationMapEmpty();
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::SetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pHandleRegistrationMap_DCS)
    {
        oResult = m_pHandleRegistrationMap_DCS->SetParameter(p_eParameterType, p_hDCS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::SetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pHandleRegistrationMap_DCS)
    {
        oResult = m_pHandleRegistrationMap_DCS->SetParameter(p_eParameterType, p_hDCS_Handle, p_Name, p_Value, p_pErrorInfo);
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::GetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pHandleRegistrationMap_DCS)
    {
        oResult = m_pHandleRegistrationMap_DCS->GetParameter(p_eParameterType, p_hDCS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::GetParameter(EParameterType p_eParameterType, HANDLE p_hDCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pHandleRegistrationMap_DCS)
    {
        oResult = m_pHandleRegistrationMap_DCS->GetParameter(p_eParameterType, p_hDCS_Handle, p_Name, p_rValue, p_pErrorInfo);
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::DCS_GetKeyHandle(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_pDCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationDCS_List* pRegistrationList = 0;

    CStdString strDeviceNameReg;
    CStdString strProtocolStackNameReg;
    CStdString strInterfaceNameReg;
    CStdString strPortNameReg;

    BOOL oDeviceNameFound = FALSE;
    BOOL oProtocolStackNameFound = FALSE;
    BOOL oInterfaceNameFound = FALSE;
    BOOL oPortNameFound = FALSE;

    CDeviceBase* pDevice = 0;
    CProtocolStackManagerBase* pManager = 0;
    HANDLE hProtocolStackHandle = 0;
    HANDLE hKeyHandle = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        if(m_pHandleRegistrationMap_DCS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            for(tHandleRegistrationDCS_List::iterator it = pRegistrationList->begin(); it != pRegistrationList->end(); it++)
            {
                CHandleRegistration_DCS* pRegistration = (*it);
                pRegistration->GetRegistrationValues(&pDevice, &pManager, &hProtocolStackHandle);

                if(!pDevice || !pManager)
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                    return FALSE;
                }

                //Compare DeviceName
                if(pDevice->DCS_GetDeviceName(&strDeviceNameReg))
                {
                    if(strDeviceNameReg.CompareNoCase(p_strDeviceName) == 0)
                    {
                        oDeviceNameFound = TRUE;

                        //Compare ProtocolStackName
                        if(pManager->PS_GetProtocolStackName(hProtocolStackHandle, &strProtocolStackNameReg))
                        {
                            if(strProtocolStackNameReg.CompareNoCase(p_strProtocolStackName) == 0)
                            {
                                oProtocolStackNameFound = TRUE;

                                //Compare InterfaceName
                                if(pManager->PS_GetInterfaceName(hProtocolStackHandle, &strInterfaceNameReg))
                                {
                                    if(strInterfaceNameReg.CompareNoCase(p_strInterfaceName) == 0)
                                    {
                                        oInterfaceNameFound = TRUE;

                                        //Compare PortName
                                        if(pManager->PS_GetPortName(hProtocolStackHandle, &strPortNameReg))
                                        {
                                            //0 = string1 identical to string2
                                            if(strPortNameReg.CompareNoCase(p_strPortName) == 0)
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
                }
            }
        }

        //ErrorHandling
        if(hKeyHandle == 0)
        {
            if(m_pErrorHandling)
            {
                if(!oDeviceNameFound)               m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
                else if(!oProtocolStackNameFound)   m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
                else if(!oInterfaceNameFound)       m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
                else if(!oPortNameFound)            m_pErrorHandling->GetError(k_Error_BadPortName, p_pErrorInfo);
                else if(!hKeyHandle)                m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            }
        }
    }

    if(p_pDCSKeyHandle) *p_pDCSKeyHandle = hKeyHandle;
    return (hKeyHandle != 0);
}

BOOL CDeviceCommandSetManager::DCS_GetBaseLayerKeyHandle(HANDLE p_hDCS_Handle, HANDLE* p_pPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, p_pPS_BaseLayerKeyHandle) || (pManager == 0))
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

BOOL CDeviceCommandSetManager::DCS_GetDefaultProtocolStackSettings(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(p_strDeviceName, p_strProtocolStackName, &pManager) && pManager)
    {
        return pManager->PS_GetDefaultProtocolStackSettings(p_strProtocolStackName, p_strInterfaceName, p_pulBaudrate, p_pulTimeout, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_GetProtocolStackName(HANDLE p_hDCS_Handle, CStdString* p_pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetProtocolStackName
        if(!pManager->PS_GetProtocolStackName(p_hPS_Handle, p_pProtocolStackName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_GetInterfaceName(HANDLE p_hDCS_Handle, CStdString* p_pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetInterfaceName
        if(!pManager->PS_GetInterfaceName(p_hPS_Handle, p_pInterfaceName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_GetPortName(HANDLE p_hDCS_Handle, CStdString* p_pPortName, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetPortName
        if(!pManager->PS_GetPortName(p_hPS_Handle, p_pPortName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_GetDeviceNameSelection(CStdStringArray* p_pDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CStdString strNameSel;

    if(p_pDeviceNameSel)
    {
        p_pDeviceNameSel->clear();
        for(std::list<CDeviceBase*>::iterator it = m_DeviceList.begin(); it != m_DeviceList.end();it++)
        {
              CDeviceBase* pDevice = (*it);
            if(pDevice->DCS_GetDeviceName(&strNameSel))
            {
                p_pDeviceNameSel->push_back(strNameSel);
            }
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_GetDeviceName(HANDLE p_hDCS_Handle, CStdString* p_pDeviceName, CErrorInfo* p_pErrorInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    CDeviceBase* pDevice = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pDevice == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetDeviceName
        if(!pDevice->DCS_GetDeviceName(p_pDeviceName))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_GetProtocolStackNameSelection(CStdString p_strDeviceName, CStdStringArray* p_pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceBase* pDevice = 0;

    //FindDevice
    if(!FindDevice(p_strDeviceName, &pDevice) || (pDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
        return FALSE;
    }

    //GetProtocolStackNameSelection
    return pDevice->DCS_GetProtocolStackNameSelection(p_pProtocolStackNameSel, p_pErrorInfo);
}

BOOL CDeviceCommandSetManager::DCS_GetInterfaceNameSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceBase* pDevice = 0;

    //FindDevice
    if(!FindDevice(p_strDeviceName, &pDevice) || (pDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
        return FALSE;
    }

    //GetInterfaceNameSelection
    return pDevice->DCS_GetInterfaceNameSelection(p_strProtocolStackName, p_pInterfaceNameSel, p_pErrorInfo);
}

BOOL CDeviceCommandSetManager::DCS_GetPortNameSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdStringArray* p_pPortNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceBase* pDevice = 0;

    //FindDevice
    if(!FindDevice(p_strDeviceName, &pDevice) || (pDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
        return FALSE;
    }

    //GetPortNameSelection
    return pDevice->DCS_GetPortNameSelection(p_strProtocolStackName, p_strInterfaceName, p_pPortNameSel, p_pErrorInfo);
}

BOOL CDeviceCommandSetManager::DCS_GetBaudrateSelection(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, CStdDWordArray* p_pBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceBase* pDevice = 0;

    //FindDevice
    if(!FindDevice(p_strDeviceName, &pDevice) || (pDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    return pDevice->DCS_GetBaudrateSelection(p_strProtocolStackName, p_strInterfaceName, p_strPortName, p_pBaudrateSel, p_pErrorInfo);
}

BOOL CDeviceCommandSetManager::DCS_GetProtocolStackSettings(HANDLE p_hDCS_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetProtocolStackSettings
        if(!pManager->PS_GetProtocolStackSettings(p_hPS_Handle, p_pulBaudrate, p_pulTimeout, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DCS_SetProtocolStackSettings(HANDLE p_hDCS_Handle, DWORD p_ulBaudrateSel, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetProtocolStackSettings
        if(!pManager->PS_SetProtocolStackSettings(p_hPS_Handle, p_ulBaudrateSel, p_ulTimeout, p_oChangeOnly, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

void CDeviceCommandSetManager::InitJournalManager(CJournalManagerBase* pJournalManager)
{
    CDeviceBase* pDevice = 0;

    for(std::list<CDeviceBase*>::iterator it = m_DeviceList.begin(); it != m_DeviceList.end();it++)
    {
        CDeviceBase* pDevice = (*it);
        pDevice->InitJournalManager(pJournalManager);
    }
}

void CDeviceCommandSetManager::ResetJournalManager()
{
    for(std::list<CDeviceBase*>::iterator it = m_DeviceList.begin(); it != m_DeviceList.end();it++)
    {
        CDeviceBase* pDevice = (*it);
        pDevice->ResetJournalManager();
    }
}

BOOL CDeviceCommandSetManager::InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* pJournalManager)
{
    CDeviceBase* pDevice = 0;
    CHandleRegistration_DCS* pRegistration = 0;
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(m_pHandleRegistrationMap_DCS->GetRegistration(p_hHandle, &pRegistration))
        {
            //Get registration entries
            if(m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hHandle, &pDevice, &pManager, &p_hPS_Handle))
            {
                if(pDevice) pDevice->InitJournalManager(pJournalManager);
                if(pManager) pManager->InitJournalManager(p_hPS_Handle, pJournalManager);

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::ResetJournalManager(HANDLE p_hHandle)
{
    CDeviceBase* pDevice = 0;
    CHandleRegistration_DCS* pRegistration = 0;
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(m_pHandleRegistrationMap_DCS->GetRegistration(p_hHandle, &pRegistration))
        {
            //Get registration entries
            if(m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hHandle, &pDevice, &pManager, &p_hPS_Handle))
            {
                if(pDevice) pDevice->ResetJournalManager();
                if(pManager) pManager->ResetJournalManager(p_hPS_Handle);

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE p_hHandle, HANDLE p_hTransactionHandle)
{
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CDeviceBase* pDevice = 0;
    CHandleRegistration_DCS* pRegistration = 0;
    BOOL oKeepLock = FALSE;
    BOOL oResult = FALSE;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Init
        oResult = TRUE;

        //Get registration
        if(oResult && (!m_pHandleRegistrationMap_DCS->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0)))
        {
            oResult = FALSE;
        }

        //Lock (Only if Registration is not locked from last command)
        if(oResult && !pRegistration->IsLocked())
        {
            oResult = pRegistration->DoLock();
        }

        //Get registration entries
        if(oResult && (!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hHandle, &pDevice, &pManager, &p_hPS_Handle) ||(pDevice == 0)))
        {
            oResult = FALSE;
        }

        if(oResult)
        {
            if(CheckLayer(p_pCommand))
            {
                //Execute command
                oResult = pDevice->ExecuteCommand(p_pCommand, pManager, p_hPS_Handle, p_hTransactionHandle);

                //Check lock state
                oKeepLock = pDevice->IsLocked();
            }
            else
            {
                //Forward Command To Next Layer
                oResult = pManager->ExecuteCommand(p_pCommand, p_hPS_Handle, p_hTransactionHandle);

                //Check lock state
                oKeepLock = p_pCommand->IsSubLayerLocked();
            }
        }

        //Unlock (Only if Device does not remain locked until next command)
        if(!oKeepLock)
        {
            //Unlock
            if(pRegistration) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::AbortCommands(HANDLE p_hHandle, bool oActive)
{
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CDeviceBase* pDevice = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0)) return FALSE;

        //Get registration entries
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hHandle, &pDevice, &pManager, &p_hPS_Handle) ||(pDevice == 0))
        {
            return FALSE;
        }

        //AbortCommands
        if(!pDevice->AbortCommands(oActive))
        {
            return FALSE;
        }

        //Close Only if no other Handles are registered
        if(m_pHandleRegistrationMap_DCS->IsLastRegistration(p_hHandle, p_hPS_Handle))
        {
            //Forward AbortCommands To Next Layer
            if(!pManager->AbortCommands(p_hPS_Handle, oActive))
            {
                return FALSE;
            }
        }

        //Abort Lock
        pRegistration->AbortLock(oActive);

        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::GetLayerManager(HANDLE p_hHandle, ELayer p_eLayer, CLayerManagerBase** p_ppLayerManager, HANDLE* pLayerHandle)
{
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CDeviceBase* pDevice = 0;
    CHandleRegistration_DCS* pRegistration = 0;
    BOOL oResult = FALSE;

    if(m_pHandleRegistrationMap_DCS)
    {
        if(CheckLayer(p_eLayer))
        {
            if(p_ppLayerManager) *p_ppLayerManager = this;
            if(pLayerHandle) *pLayerHandle = p_hHandle;
            return TRUE;
        }
        else
        {
            //Get registration
            if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hHandle, &pRegistration) || (pRegistration == 0)) return FALSE;

            //Lock
            if(!pRegistration->DoLock()) return FALSE;

            //Get registration entries
            if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hHandle, &pDevice, &pManager, &p_hPS_Handle) ||(pManager == 0))
            {
                //Unlock
                pRegistration->DoUnlock();
                return FALSE;
            }

            //Forward To Next Layer
            oResult = pManager->GetLayerManager(p_hPS_Handle, p_eLayer, p_ppLayerManager, pLayerHandle);

            //Unlock
            pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CDeviceCommandSetManager::SetInterfaceManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CInterfaceManagerBase* p_pInterfaceManager)
{
    CProtocolStackManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(p_strDeviceName, p_strProtocolStackName, &pManager) && pManager)
    {
        return pManager->SetInterfaceManager(p_strProtocolStackName, p_pInterfaceManager);
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::GetInterfaceManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CInterfaceManagerBase** p_ppInterfaceManager)
{
    CProtocolStackManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(p_strDeviceName, p_strProtocolStackName, &pManager) && pManager)
    {
        return pManager->GetInterfaceManager(p_strProtocolStackName, p_strInterfaceName, p_ppInterfaceManager);
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::SetProtocolStackManager(CStdString p_strDeviceName, CProtocolStackManagerBase* p_pProtocolStackManager)
{
    CDeviceBase* pDevice = 0;

    if(FindDevice(p_strDeviceName, &pDevice) && pDevice)
    {
        return pDevice->AddManager(p_pProtocolStackManager);
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::GetProtocolStackManager(CStdString p_strDeviceName, CStdString p_strProtocolStackName, CProtocolStackManagerBase** p_ppProtocolStackManager)
{
    CProtocolStackManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(p_strDeviceName, p_strProtocolStackName, &pManager) && pManager)
    {
        if(p_ppProtocolStackManager) *p_ppProtocolStackManager = pManager;
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::GetCommands(HANDLE p_hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CDeviceBase* pDevice = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration entries
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hHandle, &pDevice, &pManager, &p_hPS_Handle) ||(pDevice == 0))
        {
            return FALSE;
        }

        if(CheckLayer(p_eLayer))
        {
            //GetCommands
            return pDevice->GetCommands(p_pCommandInfo);
        }
        else
        {
            //Forward GetCommands To Next Layer
            return pManager->GetCommands(p_hPS_Handle, p_eLayer, p_pCommandInfo);
        }
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::EnableTracing(HANDLE p_hDCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //EnableTracing
        if(!pManager->EnableTracing(p_hPS_Handle, p_TracingFileName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CDeviceCommandSetManager::DisableTracing(HANDLE p_hDCS_Handle, CErrorInfo* p_pErrorInfo)
{
    CDeviceBase* pDevice = 0;
    CProtocolStackManagerBase* pManager = 0;
    HANDLE p_hPS_Handle = 0;
    CHandleRegistration_DCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_DCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_DCS->GetRegistration(p_hDCS_Handle, &pRegistration) || (pRegistration == 0))
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

        //Get registration values
        if(!m_pHandleRegistrationMap_DCS->GetRegistrationValues(p_hDCS_Handle, &pDevice, &pManager, &p_hPS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //DisableTracing
        if(!pManager->DisableTracing(p_hPS_Handle, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}
