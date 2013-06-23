#include "stdafx.h"
#include <algorithm>
#include "Classes/Handle/HandleRegistration_VCS.h"
#include "../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>

#include <CommunicationModel/VirtualCommandSet/VirtualCommandSet_Manager.h>
#include <CommunicationModel/VirtualCommandSet/VirtualCommandSetFactory.h>
#include "Classes/VirtualDevices/BaseClasses/VirtualDeviceBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CVirtualCommandSet_Manager::CVirtualCommandSet_Manager():
    m_pHandleRegistrationMap_VCS(0)
{
    InitRegistrationMap();
    InitErrorHandling();
}

CVirtualCommandSet_Manager::CVirtualCommandSet_Manager(int p_lInstanceValue):
    CVirtualCommandSet_ManagerBase(p_lInstanceValue),
    m_pHandleRegistrationMap_VCS(0)
{
    InitRegistrationMap();
    InitErrorHandling();
}

CVirtualCommandSet_Manager::CVirtualCommandSet_Manager(const CVirtualCommandSet_Manager& rObject):
    CVirtualCommandSet_ManagerBase(rObject),
    m_pHandleRegistrationMap_VCS(0)
{
    InitRegistrationMap();
    InitErrorHandling();
}

CVirtualCommandSet_ManagerBase* CVirtualCommandSet_Manager::Clone()
{
    CVirtualCommandSet_Manager* pManager;

    pManager = new CVirtualCommandSet_Manager(*this);
    return pManager;
}

CVirtualCommandSet_Manager::~CVirtualCommandSet_Manager()
{
    ReleaseRegistrationMap();
    DeleteVirtualDeviceList();
}

BOOL CVirtualCommandSet_Manager::InitRegistrationMap()
{
    BOOL oResult(FALSE);

    if(!m_pHandleRegistrationMap_VCS)
    {
        m_pHandleRegistrationMap_VCS = CHandleRegistrationMap_VCS::GetInstance(m_lInstanceValue, this);
        if(m_pHandleRegistrationMap_VCS)
        {
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::ReleaseRegistrationMap()
{
    BOOL oResult(TRUE);

    if(m_pHandleRegistrationMap_VCS)
    {
        CHandleRegistrationMap_VCS::ReleaseInstance(m_lInstanceValue, this);
        m_pHandleRegistrationMap_VCS = 0;
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::GetVirtualDevice(CStdString p_VirtualDeviceName, CVirtualDeviceBase*& p_rpVirtualDevice, BOOL& p_roNewCreated, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    //Init
    p_rpVirtualDevice = 0;
    p_roNewCreated = FALSE;

    //Create Virtual Device
    if(p_VirtualDeviceName.size() == 0)
    {
        //Don't create a VirtualDevice
        oResult = TRUE;
    }
    else
    {
        //VirtualDevice is already existing
        if(!FindVirtualDevice(p_VirtualDeviceName, &p_rpVirtualDevice))
        {
            //Init
            p_roNewCreated = TRUE;
            oResult = TRUE;
                        
            oResult = CVirtualCommandSetFactory::CreateInstance(p_VirtualDeviceName, p_rpVirtualDevice);
            
            if(!oResult)
            {
                //VirtualDevice not found
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadVirtualDeviceName, p_pErrorInfo);
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

    //Init Virtual Device
    if(oResult && p_rpVirtualDevice)
    {
        p_rpVirtualDevice->InitInstanceValue(m_lInstanceValue);
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::VCS_InitVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CVirtualDeviceBase* pVirtualDevice = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult(FALSE);

    //Get VirtualDevice
    if(GetVirtualDevice(strVirtualDeviceName, pVirtualDevice, oNewCreated, p_pErrorInfo) && pVirtualDevice)
    {
        //Init VirtualDevice
        if(pVirtualDevice->InitVirtualDevice(strDeviceName, strProtocolStackName, strInterfaceName, p_pErrorInfo))
        {
            if(oNewCreated) m_VirtualDeviceList.push_back(pVirtualDevice);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pVirtualDevice;
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::VCS_InitBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdDWordArray& dBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;
    BOOL oResult(TRUE);

    if(FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice))
    {
        if(pVirtualDevice)
        {
            if(!pVirtualDevice->InitBaudrateSelection(strDeviceName, strProtocolStackName, strInterfaceName, dBaudrateSel))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::VCS_InitDefaultProtocolStackSettings(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, DWORD dBaudrate, DWORD dTimeout, CErrorInfo* p_pErrorInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;
    BOOL oResult(TRUE);

    if(FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice))
    {
        if(pVirtualDevice)
        {
            if(!pVirtualDevice->InitDefaultProtocolStackSettings(strDeviceName, strProtocolStackName, strInterfaceName, dBaudrate, dTimeout))
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

void CVirtualCommandSet_Manager::DeleteVirtualDeviceList()
{
    for(std::list<CVirtualDeviceBase*>::iterator it = m_VirtualDeviceList.begin(); it != m_VirtualDeviceList.end(); it++)
    {
        CVirtualDeviceBase* pVirtualDevice = (*it);
        delete pVirtualDevice;
    }
    m_VirtualDeviceList.clear();
}

BOOL CVirtualCommandSet_Manager::FindCorrectManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CDeviceCommandSetManagerBase** ppManager)
{
    CVirtualDeviceBase* pVirtualDevice = 0;

    if(FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) && pVirtualDevice)
    {
        return pVirtualDevice->FindCorrectManager(strDeviceName, ppManager);
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::FindVirtualDevice(CStdString strVirtualDeviceName, CVirtualDeviceBase** ppVirtualDevice)
{
    for(std::list<CVirtualDeviceBase*>::iterator it = m_VirtualDeviceList.begin(); it != m_VirtualDeviceList.end(); it++)
    {
        CVirtualDeviceBase* pVirtualDevice = (*it);
        CStdString strName;
        if(pVirtualDevice && pVirtualDevice->VCS_GetVirtualDeviceName(&strName))
        {
            if(strVirtualDeviceName.CompareNoCase(strName) == 0)
            {
                if(ppVirtualDevice) *ppVirtualDevice = pVirtualDevice;
                return TRUE;
            }
        }
    }

    return FALSE;
}

HANDLE CVirtualCommandSet_Manager::VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Zugehörigen Manager finden
        if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
        {
            //Device command set handle
            hDCS_Handle = pManager->DCS_OpenDevice(strDeviceName, strProtocolStackName, strInterfaceName, strPortName, p_pErrorInfo);
            if(hDCS_Handle)
            {
                if(FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) && pVirtualDevice)
                {
                    if(pVirtualDevice->InitGateway(strDeviceName))
                    {
                        //Device command set handle
                        return m_pHandleRegistrationMap_VCS->RegisterHandle(pVirtualDevice, pManager, hDCS_Handle);
                    }
                }

                pManager->DCS_CloseDevice(hDCS_Handle);
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            }

            return 0;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
    return 0;
}

BOOL CVirtualCommandSet_Manager::VCS_CloseVirtualDevice(HANDLE hVCS_Handle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceCommandSetManagerBase* pManager = 0;
    CHandleRegistration_VCS* pRegistration = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Close Only if no other Handles are registered
        if(m_pHandleRegistrationMap_VCS->IsLastRegistration(hVCS_Handle, hDCS_Handle))
        {
            //Close Device
            if(!pManager->DCS_CloseDevice(hDCS_Handle, p_pErrorInfo))
            {
                if(pRegistration) pRegistration->DoUnlock();
                return FALSE;
            }
        }

        //Delete Registration
        if(!m_pHandleRegistrationMap_VCS->DeleteMapRegistration(hVCS_Handle))
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

BOOL CVirtualCommandSet_Manager::VCS_CloseAllVirtualDevices(CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationVCS_List* pRegistrationList;

    HANDLE hVCSHandle;

    if(m_pHandleRegistrationMap_VCS)
    {
        if(m_pHandleRegistrationMap_VCS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            std::list<CHandleRegistration_VCS*>::iterator it = pRegistrationList->begin();
            while(it != pRegistrationList->end())
            {
                CHandleRegistration_VCS* pRegistration = (*it);
                if(pRegistration != 0)
                {
                    hVCSHandle = pRegistration->GetKeyHandle();
                    if(hVCSHandle)
                    {
                        if(!VCS_CloseVirtualDevice(hVCSHandle, p_pErrorInfo))
                            return FALSE;
                    }
                    pRegistrationList->remove(pRegistration);
                    delete pRegistration;
                }
                it = pRegistrationList->begin();
            }

            pRegistrationList->clear();

            return TRUE;
        }
    }

    return FALSE;
}

HANDLE CVirtualCommandSet_Manager::VCS_OpenVirtualDevice(CStdString strVirtualDeviceName, CStdString strDeviceName, HANDLE hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Zugehörigen Manager finden
        if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
        {
            //Device command set handle
            hDCS_Handle = pManager->DCS_OpenDevice(strDeviceName, hPS_BaseLayerHandle, p_pErrorInfo);
            if(hDCS_Handle)
            {
                if(FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) && pVirtualDevice)
                {
                    if(pVirtualDevice->InitGateway(strDeviceName))
                    {
                        //Device command set handle
                        return m_pHandleRegistrationMap_VCS->RegisterHandle(pVirtualDevice, pManager, hDCS_Handle);
                    }
                }

                pManager->DCS_CloseDevice(hDCS_Handle, hPS_BaseLayerHandle);
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            }

            return FALSE;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_CloseVirtualDevice(HANDLE hVCS_Handle, HANDLE hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CDeviceCommandSetManagerBase* pManager = 0;
    CHandleRegistration_VCS* pRegistration = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Close Device
        if(!pManager->DCS_CloseDevice(hDCS_Handle, hPS_BaseLayerHandle, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Delete Registration
        if(!m_pHandleRegistrationMap_VCS->DeleteMapRegistration(hVCS_Handle))
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

BOOL CVirtualCommandSet_Manager::VCS_CloseAllVirtualDevices(HANDLE hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationVCS_List* pRegistrationList;
    HANDLE hVCSHandle;

    if(m_pHandleRegistrationMap_VCS)
    {
        if(m_pHandleRegistrationMap_VCS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            tHandleRegistrationVCS_List::iterator it = pRegistrationList->begin();
            while(it != pRegistrationList->end())
            {
                CHandleRegistration_VCS* pRegistration = (*it);
                if(pRegistration)
                {
                    hVCSHandle = pRegistration->GetKeyHandle();
                    if(hVCSHandle)
                    {
                        if(!VCS_CloseVirtualDevice(hVCSHandle, hPS_BaseLayerHandle, p_pErrorInfo))
                            return FALSE;
                    }

                    pRegistrationList->remove(pRegistration);

                    //Delete Registration
                    delete pRegistration;

                    it = pRegistrationList->begin();
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_AreAllVirtualDevicesClosed()
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_VCS)
    {
        oResult = m_pHandleRegistrationMap_VCS->IsRegistrationMapEmpty();
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_VCS)
    {
        oResult = m_pHandleRegistrationMap_VCS->SetParameter(p_eParameterType, hVCS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::SetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_VCS)
    {
        oResult = m_pHandleRegistrationMap_VCS->SetParameter(p_eParameterType, hVCS_Handle, p_Name, p_Value, p_pErrorInfo);
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_VCS)
    {
        oResult = m_pHandleRegistrationMap_VCS->GetParameter(p_eParameterType, hVCS_Handle, p_Name, p_pValue, p_ulSize, p_pErrorInfo);
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::GetParameter(EParameterType p_eParameterType, HANDLE hVCS_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_VCS)
    {
        oResult = m_pHandleRegistrationMap_VCS->GetParameter(p_eParameterType, hVCS_Handle, p_Name, p_rValue, p_pErrorInfo);
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, HANDLE* p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationVCS_List* pRegistrationList = 0;

    CStdString strVirtualDeviceNameReg;
    CStdString strDeviceNameReg;
    CStdString strProtocolStackNameReg;
    CStdString strInterfaceNameReg;
    CStdString strPortNameReg;

    BOOL oVirtualDeviceNameFound = FALSE;
    BOOL oDeviceNameFound = FALSE;
    BOOL oProtocolStackNameFound = FALSE;
    BOOL oInterfaceNameFound = FALSE;
    BOOL oPortNameFound = FALSE;

    CVirtualDeviceBase* pVirtualDevice = 0;
    CDeviceCommandSetManagerBase* pManager = 0;
    HANDLE hDeviceCommandSetHandle = 0;
    HANDLE hKeyHandle = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        if(m_pHandleRegistrationMap_VCS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            for(tHandleRegistrationVCS_List::iterator it = pRegistrationList->begin(); it != pRegistrationList->end(); it++)
            {
                CHandleRegistration_VCS* pRegistration = (*it);

                pRegistration->GetRegistrationValues(&pVirtualDevice, &pManager, &hDeviceCommandSetHandle);

                if(!pVirtualDevice || !pManager)
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                    return FALSE;
                }

                //Compare VirtualDeviceName
                if(pVirtualDevice->VCS_GetVirtualDeviceName(&strVirtualDeviceNameReg))
                {
                    if(p_strVirtualDeviceName.CompareNoCase(strVirtualDeviceNameReg) == 0)
                    {
                        oVirtualDeviceNameFound = TRUE;

                        //Compare DeviceName
                        if(pManager->DCS_GetDeviceName(hDeviceCommandSetHandle, &strDeviceNameReg))
                        {
                            if(p_strDeviceName.CompareNoCase(strDeviceNameReg) == 0)
                            {
                                oDeviceNameFound = TRUE;

                                //Compare ProtocolStackName
                                if(pManager->DCS_GetProtocolStackName(hDeviceCommandSetHandle, &strProtocolStackNameReg))
                                {
                                    if(p_strProtocolStackName.CompareNoCase(strProtocolStackNameReg) == 0)
                                    {
                                        oProtocolStackNameFound = TRUE;

                                        //Compare InterfaceName
                                        if(pManager->DCS_GetInterfaceName(hDeviceCommandSetHandle, &strInterfaceNameReg))
                                        {
                                            if(p_strInterfaceName.CompareNoCase(strInterfaceNameReg) == 0)
                                            {
                                                oInterfaceNameFound = TRUE;

                                                //Compare PortName
                                                if(pManager->DCS_GetPortName(hDeviceCommandSetHandle, &strPortNameReg))
                                                {
                                                    if(p_strPortName.CompareNoCase(strPortNameReg) == 0)
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
                    }
                }
            }
        }

        //ErrorHandling
        if(hKeyHandle == 0)
        {
            if(m_pErrorHandling)
            {
                if(!oVirtualDeviceNameFound)        m_pErrorHandling->GetError(k_Error_BadVirtualDeviceName, p_pErrorInfo);
                else if(!oDeviceNameFound)          m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
                else if(!oProtocolStackNameFound)   m_pErrorHandling->GetError(k_Error_BadProtocolStackName, p_pErrorInfo);
                else if(!oInterfaceNameFound)       m_pErrorHandling->GetError(k_Error_BadInterfaceName, p_pErrorInfo);
                else if(!oPortNameFound)            m_pErrorHandling->GetError(k_Error_BadPortName, p_pErrorInfo);
                else if(!hKeyHandle)                m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            }
        }
    }

    if(p_pVCSKeyHandle) *p_pVCSKeyHandle = hKeyHandle;
    return (hKeyHandle != 0);
}

BOOL CVirtualCommandSet_Manager::VCS_GetKeyHandle(CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CLayerParameterStack& p_rAddressParameter, HANDLE* p_pVCSKeyHandle, CErrorInfo* p_pErrorInfo)
{
    tHandleRegistrationVCS_List* pRegistrationList = 0;

    CStdString virtualDeviceNameReg;
    CStdString deviceNameReg;

    BOOL oVirtualDeviceNameFound(FALSE);
    BOOL oDeviceNameFound(FALSE);
    BOOL oAddressSettingsFound(FALSE);

    CVirtualDeviceBase* pVirtualDevice(0);
    CDeviceCommandSetManagerBase* pManager(0);
    HANDLE hDeviceCommandSetHandle(0);
    HANDLE hKeyHandle(0);
    HANDLE handle(0);
    BYTE ubNodeIdLayer(0);
    BYTE ubNodeId(0);

    if(m_pHandleRegistrationMap_VCS)
    {
        if(m_pHandleRegistrationMap_VCS->GetRegistrationList(&pRegistrationList) && pRegistrationList)
        {
            for(tHandleRegistrationVCS_List::iterator it = pRegistrationList->begin(); it != pRegistrationList->end(); it++)
            {
                CHandleRegistration_VCS* pRegistration = (*it);
                pRegistration->GetRegistrationValues(&pVirtualDevice, &pManager, &hDeviceCommandSetHandle);

                if(!pVirtualDevice || !pManager)
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                    return FALSE;
                }

                //Compare VirtualDeviceName
                if(pVirtualDevice->VCS_GetVirtualDeviceName(&virtualDeviceNameReg))
                {
                    if(p_strVirtualDeviceName.CompareNoCase(virtualDeviceNameReg) == 0)
                    {
                        oVirtualDeviceNameFound = TRUE;

                        //Compare DeviceName
                        if(pManager->DCS_GetDeviceName(hDeviceCommandSetHandle, &deviceNameReg))
                        {
                            if(p_strDeviceName.CompareNoCase(deviceNameReg) == 0)
                            {
                                oDeviceNameFound = TRUE;

                                if(p_rAddressParameter.GetLayerParameter(DEVICE_COMMAND_SET_LAYER, _T("NodeId"), &ubNodeId, sizeof(ubNodeId)))
                                {
                                    if(pManager->GetParameter(PT_DEVICE, hDeviceCommandSetHandle, _T("NodeId"), &ubNodeIdLayer, sizeof(ubNodeIdLayer), p_pErrorInfo))
                                    {
                                        if(ubNodeId == ubNodeIdLayer)
                                        {
                                            oAddressSettingsFound = TRUE;
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
        }

        //ErrorHandling
        if(hKeyHandle == 0)
        {
            if(m_pErrorHandling)
            {
                if(!oVirtualDeviceNameFound)        m_pErrorHandling->GetError(k_Error_BadVirtualDeviceName, p_pErrorInfo);
                else if(!oDeviceNameFound)          m_pErrorHandling->GetError(k_Error_BadDeviceName, p_pErrorInfo);
                else if(!oAddressSettingsFound)     m_pErrorHandling->GetError(k_Error_BadAddressParameter, p_pErrorInfo);
                else if(!hKeyHandle)                m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
                else;
            }
        }
    }

    if(p_pVCSKeyHandle) *p_pVCSKeyHandle = hKeyHandle;
    return (hKeyHandle != 0);
}

BOOL CVirtualCommandSet_Manager::VCS_GetBaseLayerKeyHandle(HANDLE p_hVCS_Handle, HANDLE* p_phPS_BaseLayerKeyHandle, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager(0);
    CHandleRegistration_VCS* pRegistration(0);
    CVirtualDeviceBase* pVirtualDevice(0);
    HANDLE hHandle(0);

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(p_hVCS_Handle, &pRegistration) || (pRegistration == 0))
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
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(p_hVCS_Handle, &pVirtualDevice, &pManager, &hHandle) || (pManager == 0) || (pVirtualDevice == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get BaseLayer ProtocolStack KeyHandle
        if(!pManager->DCS_GetBaseLayerKeyHandle(hHandle, p_phPS_BaseLayerKeyHandle))
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

BOOL CVirtualCommandSet_Manager::VCS_GetVirtualDeviceName(HANDLE hVCS_Handle, CStdString* pVirtualDeviceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pVirtualDevice == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get VirtualDeviceName
        if(!pVirtualDevice->VCS_GetVirtualDeviceName(pVirtualDeviceName))
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

BOOL CVirtualCommandSet_Manager::VCS_GetDeviceName(HANDLE hVCS_Handle, CStdString* pDeviceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get DeviceName
        if(!pManager->DCS_GetDeviceName(hDCS_Handle, pDeviceName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_GetProtocolStackName(HANDLE hVCS_Handle, CStdString* pProtocolStackName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get DeviceName
        if(!pManager->DCS_GetProtocolStackName(hDCS_Handle, pProtocolStackName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_GetInterfaceName(HANDLE hVCS_Handle, CStdString* pInterfaceName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get DeviceName
        if(!pManager->DCS_GetInterfaceName(hDCS_Handle, pInterfaceName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_GetPortName(HANDLE hVCS_Handle, CStdString* pPortName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //Get DeviceName
        if(!pManager->DCS_GetPortName(hDCS_Handle, pPortName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_GetVirtualDeviceNameSelection(CStdStringArray* pVirtualDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    CStdString strNameSel;

    if(pVirtualDeviceNameSel)
    {
        pVirtualDeviceNameSel->clear();

        for(std::list<CVirtualDeviceBase*>::iterator it = m_VirtualDeviceList.begin(); it != m_VirtualDeviceList.end(); it++)
        {
            CVirtualDeviceBase* pVirtualDevice = (*it);
            if(pVirtualDevice->VCS_GetVirtualDeviceName(&strNameSel))
                pVirtualDeviceNameSel->push_back(strNameSel);
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_GetDeviceNameSelection(CStdString strVirtualDeviceName, CStdStringArray* pDeviceNameSel, CErrorInfo* p_pErrorInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;

    //FindVirtualDevice
    if(!FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) || (pVirtualDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    //GetDeviceNameSelection
    return pVirtualDevice->VCS_GetDeviceNameSelection(pDeviceNameSel, p_pErrorInfo);
}

BOOL CVirtualCommandSet_Manager::VCS_GetProtocolStackNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdStringArray* pProtocolStackNameSel, CErrorInfo* p_pErrorInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;

    //FindVirtualDevice
    if(!FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) || (pVirtualDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    //GetProtocolStackNameSelection
    return pVirtualDevice->VCS_GetProtocolStackNameSelection(strDeviceName, pProtocolStackNameSel, p_pErrorInfo);
}

BOOL CVirtualCommandSet_Manager::VCS_GetInterfaceNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdStringArray* pInterfaceNameSel, CErrorInfo* p_pErrorInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;

    //FindVirtualDevice
    if(!FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) || (pVirtualDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    //GetInterfaceNameSelection
    return pVirtualDevice->VCS_GetInterfaceNameSelection(strDeviceName, strProtocolStackName, pInterfaceNameSel, p_pErrorInfo);
}

BOOL CVirtualCommandSet_Manager::VCS_GetPortNameSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdStringArray* pPortSel, CErrorInfo* p_pErrorInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;

    //FindVirtualDevice
    if(!FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) || (pVirtualDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    //GetPortNameSelection
    return pVirtualDevice->VCS_GetPortNameSelection(strDeviceName, strProtocolStackName, strInterfaceName, pPortSel, p_pErrorInfo);
}

BOOL CVirtualCommandSet_Manager::VCS_GetBaudrateSelection(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CStdString strPortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* p_pErrorInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;

    //FindVirtualDevice
    if(!FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) || (pVirtualDevice == 0))
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    //GetBaudrateSelection
    return pVirtualDevice->VCS_GetBaudrateSelection(strDeviceName, strProtocolStackName, strInterfaceName, strPortName, pdBaudrateSel, p_pErrorInfo);
}

BOOL CVirtualCommandSet_Manager::VCS_GetProtocolStackSettings(HANDLE hVCS_Handle, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //GetProtocolStackSettings
        if(!pManager->DCS_GetProtocolStackSettings(hDCS_Handle, pdBaudrate, pdTimeout, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_SetProtocolStackSettings(HANDLE hVCS_Handle, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //SetProtocolStackSettings
        if(!pManager->DCS_SetProtocolStackSettings(hDCS_Handle, dBaudrate, dTimeout, oChangeOnly, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::GetCommands(HANDLE hHandle, ELayer p_eLayer, CStdString* p_pCommandInfo)
{
    CVirtualDeviceBase* pVirtualDevice = 0;
    CDeviceCommandSetManagerBase* pManager = 0;
    HANDLE hDCS_Handle = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration entries
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hHandle, &pVirtualDevice, &pManager, &hDCS_Handle) ||(pVirtualDevice == 0))
        {
            return FALSE;
        }

        if(CheckLayer(p_eLayer))
        {
            //GetCommands
            return pVirtualDevice->GetCommands(p_pCommandInfo);
        }
        else
        {
            //Forward GetCommands To Next Layer
            return pManager->GetCommands(hDCS_Handle, p_eLayer, p_pCommandInfo);
        }
    }

    return FALSE;
}

void CVirtualCommandSet_Manager::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    for(std::list<CVirtualDeviceBase*>::iterator it = m_VirtualDeviceList.begin();it != m_VirtualDeviceList.begin();it++)
    {
        CVirtualDeviceBase* pVirtualDevice = (*it);
        if(pVirtualDevice!=0) pVirtualDevice->InitJournalManager(pJournalManager);
    }
}

void CVirtualCommandSet_Manager::ResetJournalManager()
{
    for(std::list<CVirtualDeviceBase*>::iterator it = m_VirtualDeviceList.begin();it != m_VirtualDeviceList.begin();it++)
    {
        CVirtualDeviceBase* pVirtualDevice = (*it);
        if(pVirtualDevice!=0) pVirtualDevice->ResetJournalManager();
    }
}

BOOL CVirtualCommandSet_Manager::InitJournalManager(HANDLE hHandle, CJournalManagerBase *pJournalManager)
{
    CVirtualDeviceBase* pVirtualDevice = 0;
    CDeviceCommandSetManagerBase* pManager = 0;
    CHandleRegistration_VCS* pRegistration = 0;
    HANDLE hDCS_Handle = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(m_pHandleRegistrationMap_VCS->GetRegistration(hHandle, &pRegistration))
        {
            //Get registration entries
            if(m_pHandleRegistrationMap_VCS->GetRegistrationValues(hHandle, &pVirtualDevice, &pManager, &hDCS_Handle))
            {
                if(pVirtualDevice) pVirtualDevice->InitJournalManager(pJournalManager);
                if(pManager) pManager->InitJournalManager(hDCS_Handle, pJournalManager);

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::ResetJournalManager(HANDLE hHandle)
{
    CVirtualDeviceBase* pVirtualDevice = 0;
    CDeviceCommandSetManagerBase* pManager = 0;
    CHandleRegistration_VCS* pRegistration = 0;
    HANDLE hDCS_Handle = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(m_pHandleRegistrationMap_VCS->GetRegistration(hHandle, &pRegistration))
        {
            //Get registration entries
            if(m_pHandleRegistrationMap_VCS->GetRegistrationValues(hHandle, &pVirtualDevice, &pManager, &hDCS_Handle))
            {
                if(pVirtualDevice) pVirtualDevice->ResetJournalManager();
                if(pManager) pManager->ResetJournalManager(hDCS_Handle);

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::ExecuteCommand(CCommandRoot *p_pCommand, HANDLE hHandle, HANDLE hTransactionHandle)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    HANDLE hDCS_Handle = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    CHandleRegistration_VCS* pRegistration = 0;
    BOOL oKeepLock = 0;
    CErrorInfo errorInfo;
    BOOL oResult(FALSE);

    if(m_pHandleRegistrationMap_VCS)
    {
        //Init
        oResult = TRUE;

        //Get registration
        if(oResult && !m_pHandleRegistrationMap_VCS->GetRegistration(hHandle, &pRegistration) || (pRegistration == 0))
        {
            if(p_pCommand)
            {
                errorInfo.Init(k_Error_HandleNotValid);
                p_pCommand->SetStatus(oResult, &errorInfo);
            }
            oResult = FALSE;
        }

        //Lock (Only if Registration is not locked from last command)
        if(oResult && !pRegistration->IsLocked())
        {
            oResult = pRegistration->DoLock();
        }

        //Get registration entries
        if(oResult && !m_pHandleRegistrationMap_VCS->GetRegistrationValues(hHandle, &pVirtualDevice, &pManager, &hDCS_Handle) ||(pVirtualDevice == 0))
        {
            oResult = FALSE;
        }

        if(oResult)
        {
            if(CheckLayer(p_pCommand))
            {
                //Execute command
                oResult = pVirtualDevice->ExecuteCommand(p_pCommand, pManager, hDCS_Handle, hTransactionHandle);

                //Check lock state
                oKeepLock = pVirtualDevice->IsLocked();
            }
            else
            {
                //Forward Command To Next Layer
                oResult = pManager->ExecuteCommand(p_pCommand, hDCS_Handle, hTransactionHandle);

                //Check lock state
                oKeepLock = p_pCommand->IsSubLayerLocked();
            }
        }

        //Unlock (Only if VirtualDevice does not remain locked until next command)
        if(!oKeepLock)
        {
            //Unlock
            if(pRegistration) pRegistration->DoUnlock();
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_Manager::AbortCommands(HANDLE hHandle, BOOL p_oActive)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    HANDLE hDCS_Handle = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hHandle, &pRegistration) || (pRegistration == 0)) return FALSE;

        //Get registration entries
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hHandle, &pVirtualDevice, &pManager, &hDCS_Handle) ||(pVirtualDevice == 0))
        {
            return FALSE;
        }

        //AbortCommands
        if(!pVirtualDevice->AbortCommands(p_oActive))
        {
            return FALSE;
        }

        //Close Only if no other Handles are registered
        if(m_pHandleRegistrationMap_VCS->IsLastRegistration(hHandle, hDCS_Handle))
        {
            //Forward AbortCommands To Next Layer
            if(!pManager->AbortCommands(hDCS_Handle, p_oActive))
            {
                return FALSE;
            }
        }

        //Abort Lock
        pRegistration->AbortLock(p_oActive);

        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::GetLayerManager(HANDLE hHandle, ELayer p_eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    HANDLE hDCS_Handle = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    CHandleRegistration_VCS* pRegistration = 0;
    BOOL oResult;

    if(m_pHandleRegistrationMap_VCS)
    {
        if(CheckLayer(p_eLayer))
        {
            if(ppLayerManager) *ppLayerManager = this;
            if(pLayerHandle) *pLayerHandle = hHandle;
            return TRUE;
        }
        else
        {
            //Get registration
            if(!m_pHandleRegistrationMap_VCS->GetRegistration(hHandle, &pRegistration) || (pRegistration == 0)) return FALSE;

            //Lock
            if(!pRegistration->DoLock()) return FALSE;

            //Get registration entries
            if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hHandle, &pVirtualDevice, &pManager, &hDCS_Handle) ||(pManager == 0))
            {
                //Unlock
                pRegistration->DoUnlock();
                return FALSE;
            }

            //Forward To Next Layer
            oResult = pManager->GetLayerManager(hDCS_Handle, p_eLayer, ppLayerManager, pLayerHandle);

            //Unlock
            pRegistration->DoUnlock();
            return oResult;
        }
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::SetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CInterfaceManagerBase* pInterfaceManager)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
    {
        return pManager->SetInterfaceManager(strDeviceName, strProtocolStackName, pInterfaceManager);
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::GetInterfaceManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, CInterfaceManagerBase** ppInterfaceManager)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
    {
        return pManager->GetInterfaceManager(strDeviceName, strProtocolStackName, strInterfaceName, ppInterfaceManager);
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::SetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CProtocolStackManagerBase* pProtocolStackManager)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
    {
        return pManager->SetProtocolStackManager(strDeviceName, pProtocolStackManager);
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::GetProtocolStackManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CProtocolStackManagerBase** ppProtocolStackManager)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
    {
        return pManager->GetProtocolStackManager(strDeviceName, strProtocolStackName, ppProtocolStackManager);
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::SetDeviceCommandSetManager(CStdString strVirtualDeviceName, CDeviceCommandSetManagerBase* pDeviceCommandSetManager)
{
    CVirtualDeviceBase* pVirtualDevice = 0;

    if(FindVirtualDevice(strVirtualDeviceName, &pVirtualDevice) && pVirtualDevice)
    {
        return pVirtualDevice->AddManager(pDeviceCommandSetManager);
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::GetDeviceCommandSetManager(CStdString strVirtualDeviceName, CStdString strDeviceName, CDeviceCommandSetManagerBase** ppDeviceCommandSetManager)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
    {
        if(ppDeviceCommandSetManager) *ppDeviceCommandSetManager = pManager;
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::VCS_GetDefaultProtocolStackSettings(CStdString strVirtualDeviceName, CStdString strDeviceName, CStdString strProtocolStackName, CStdString strInterfaceName, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;

    //Zugehörigen Manager finden
    if(FindCorrectManager(strVirtualDeviceName, strDeviceName, &pManager) && pManager)
    {
        return pManager->DCS_GetDefaultProtocolStackSettings(strDeviceName, strProtocolStackName, strInterfaceName, pdBaudrate, pdTimeout, p_pErrorInfo);
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "VirtualCommandSetManager";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(VIRTUAL_COMMAND_SET_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::EnableTracing(HANDLE hVCS_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //EnableTracing
        if(!pManager->EnableTracing(hDCS_Handle, p_TracingFileName, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

BOOL CVirtualCommandSet_Manager::DisableTracing(HANDLE hVCS_Handle, CErrorInfo* p_pErrorInfo)
{
    CDeviceCommandSetManagerBase* pManager = 0;
    CVirtualDeviceBase* pVirtualDevice = 0;
    HANDLE hDCS_Handle = 0;
    CHandleRegistration_VCS* pRegistration = 0;

    if(m_pHandleRegistrationMap_VCS)
    {
        //Get registration
        if(!m_pHandleRegistrationMap_VCS->GetRegistration(hVCS_Handle, &pRegistration) || (pRegistration == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Lock
        if(!pRegistration->DoLock())
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_HandleNotValid, p_pErrorInfo);
            return FALSE;
        }

        //Get registration values
        if(!m_pHandleRegistrationMap_VCS->GetRegistrationValues(hVCS_Handle, &pVirtualDevice, &pManager, &hDCS_Handle) || (pManager == 0))
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        //DisableTracing
        if(!pManager->DisableTracing(hDCS_Handle, p_pErrorInfo))
        {
            if(pRegistration) pRegistration->DoUnlock();
            return FALSE;
        }

        if(pRegistration) pRegistration->DoUnlock();
        return TRUE;
    }

    return FALSE;
}

