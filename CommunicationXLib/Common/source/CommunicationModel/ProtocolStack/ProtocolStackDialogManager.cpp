#include "stdafx.h"

#include <CommunicationModel/VirtualCommandSet/Common/DeviceSettings.h>
#include <CommunicationModel/VirtualCommandSet/Common/ProtocolStackSettings.h>
#include <CommunicationModel/ProtocolStack/Common/GatewaySettings.h>
#include <CommunicationModel/ProtocolStack/Dialogs/OpenCommunicationDlg.h>
#include <CommunicationModel/ProtocolStack/Dialogs/OpenGatewayDlg.h>
#include <CommunicationModel/ProtocolStack/ProtocolStackManager.h>

#include "ProtocolStackDialogManager.h"

CProtocolStackDialogManager::CProtocolStackDialogManager(void)
{
    m_ulTimeout = 0;
}

CProtocolStackDialogManager::~CProtocolStackDialogManager(void)
{
}

HANDLE CProtocolStackDialogManager::OpenProtocolStackDlg(CProtocolStackManagerBase* p_pProtocolStackManager, CStdStringArray *p_paProtocolStackName, CErrorInfo* p_pErrorInfo)
{
	HANDLE keyHandle(0);

    BOOL oResult(TRUE);
    CStdString strInterfaceName(_T(""));
    CStdString strPortName(_T(""));
    CStdString strProtocolStackName(_T(""));
    DWORD_PTR ulBaudrate(0);
    tDeviceSettingsList deviceSettingsList;

    //Init Selection
	for(CStdStringArray::iterator it = p_paProtocolStackName->begin(); it != p_paProtocolStackName->end(); it++)
    {
        if(!InitDeviceSettingsSelection(p_pProtocolStackManager, (*it), &deviceSettingsList))
        {
            oResult = FALSE;
			break;
        }
    }
#ifdef WINVER
	COpenCommunicationDlg dlg;
	if(oResult)
    {
        dlg.SetParameter(&deviceSettingsList, p_paProtocolStackName, strProtocolStackName, strInterfaceName, strPortName, ulBaudrate, m_ulTimeout);
        if(dlg.DoModal() == IDOK)
        {
            dlg.GetParameter(&strProtocolStackName, &strInterfaceName, &strPortName, &ulBaudrate, &m_ulTimeout);

            //Open communication
            keyHandle = p_pProtocolStackManager->PS_OpenProtocolStack(strProtocolStackName, strInterfaceName, strPortName, p_pErrorInfo);
            if(keyHandle)
            {
                //SetProtocolStackSettings
                if(!p_pProtocolStackManager->PS_SetProtocolStackSettings(keyHandle, (DWORD)ulBaudrate, (DWORD)m_ulTimeout, TRUE))
                {
                    p_pProtocolStackManager->PS_CloseProtocolStack(keyHandle);
                    keyHandle = 0;
                }
            }
        }
    }
#endif
    DeleteDeviceSettingsSelection(&deviceSettingsList);

    return keyHandle;
}

BOOL CProtocolStackDialogManager::InitDeviceSettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings(0);
    CStdString strInterfaceNameSel;
    CStdStringArray strInterfaceNameArray;
    BOOL oResult(FALSE);

    if(p_pProtocolStackManager && p_pList)
    {
        //InterfaceName Selection
        p_pProtocolStackManager->PS_GetInterfaceNameSelection(p_ProtocolStackName, &strInterfaceNameArray);

        if(strInterfaceNameArray.size() > 0)
        {
            //DeviceSettings with Interfaces
            oResult = TRUE;
            for(std::size_t iPos = 0; iPos < strInterfaceNameArray.size(); iPos++)
            {
                strInterfaceNameSel = strInterfaceNameArray.at(iPos);
                if(!InitDeviceSettingsSelection(p_pProtocolStackManager, p_ProtocolStackName, strInterfaceNameSel, p_pList))
                {
                    oResult = FALSE;
                    break;
                }
            }
        }
        else
        {
            //DeviceSettings without Interfaces
            pDeviceSettings = new CDeviceSettings();
            pDeviceSettings->Init( _T(""), _T(""), p_ProtocolStackName, _T(""), _T(""));
            p_pList->push_back(pDeviceSettings);
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackDialogManager::InitDeviceSettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings(0);
    CStdString strPortNameSel;
    CStdStringArray strPortNameArray;
    BOOL oResult(FALSE);

    if(p_pProtocolStackManager && p_pList)
    {
        //PortName Selection
        p_pProtocolStackManager->PS_GetPortNameSelection(p_ProtocolStackName, p_InterfaceName, &strPortNameArray);

        if(strPortNameArray.size() > 0)
        {
            //DeviceSettings with Ports
            oResult = TRUE;
            for(std::size_t iPos = 0; iPos < strPortNameArray.size(); iPos++)
            {
                strPortNameSel = strPortNameArray.at(iPos);
                if(!InitDeviceSettingsSelection(p_pProtocolStackManager, p_ProtocolStackName, p_InterfaceName, strPortNameSel, p_pList))
                {
                    oResult = FALSE;
                    break;
                }
            }
        }
        else
        {
            //DeviceSettings without Ports
            pDeviceSettings = new CDeviceSettings();
            pDeviceSettings->Init(_T(""), _T(""), p_ProtocolStackName, p_InterfaceName, _T(""));
            p_pList->push_back(pDeviceSettings);
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackDialogManager::InitDeviceSettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName, CStdString p_PortName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings(0);
    CProtocolStackSettings* pProtocolStackSettings(0);
    CProtocolStackSettings defaultProtocolStackSettings;

    CStdDWordArray ulBaudrateArray;
    DWORD dDefaultBaudrate, dDefaultTimeout;
    DWORD ulBaudrateSel;
    BOOL oResult(FALSE);

    if(p_pProtocolStackManager && p_pList)
    {
        //Default Values
        p_pProtocolStackManager->PS_GetDefaultProtocolStackSettings(p_ProtocolStackName, p_InterfaceName, &dDefaultBaudrate, &dDefaultTimeout);
        defaultProtocolStackSettings.Init(dDefaultBaudrate, dDefaultTimeout);

        //DeviceSettings
        pDeviceSettings = new CDeviceSettings();
        pDeviceSettings->Init(_T(""), _T(""), p_ProtocolStackName, p_InterfaceName, p_PortName);
        pDeviceSettings->SetDefaultProtocolStackSettings(&defaultProtocolStackSettings);
        p_pList->push_back(pDeviceSettings);

        //Baudrate Selection
        p_pProtocolStackManager->PS_GetBaudrateSelection(p_ProtocolStackName, p_InterfaceName, p_PortName, &ulBaudrateArray);

        if(ulBaudrateArray.size() > 0)
        {
            //Baudrates available
            oResult = TRUE;
            for(std::size_t iPos = 0; iPos < ulBaudrateArray.size(); iPos++)
            {
                //ProtocolStackSettings
                ulBaudrateSel = ulBaudrateArray.at(iPos);

                pProtocolStackSettings = new CProtocolStackSettings();
                pProtocolStackSettings->Init(ulBaudrateSel, (DWORD)m_ulTimeout);
                pDeviceSettings->AddProtocolStackSettings(pProtocolStackSettings);
            }
        }
        else
        {
            //No Baudrates available
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackDialogManager::DeleteDeviceSettingsSelection(tDeviceSettingsList* p_pList)
{
    for( std::list<CDeviceSettings*>::iterator it = p_pList->begin();
    		it != p_pList->end(); it++ )
    {
    	CDeviceSettings* pDeviceSettings = (*it);
    	if( pDeviceSettings != 0 )
    		delete pDeviceSettings;
    }
    p_pList->clear();

    return TRUE;
}

HANDLE CProtocolStackDialogManager::OpenLayerProtocolStackDlg(CProtocolStackManagerBase* p_pProtocolStackManager, HANDLE p_hCommunication, CErrorInfo* p_pErrorInfo)
{
	HANDLE keyHandle = 0;
    CStdString strGatewayName(_T(""));
    CStdString strProtocolStackName(_T(""));
    tGatewaySettingsList gatewaySettingsList;

    if(InitGatewaySettingsSelection(p_pProtocolStackManager, &gatewaySettingsList))
    {
#ifdef WINVER
    	COpenGatewayDlg dlg;

        dlg.SetParameter(&gatewaySettingsList);
        if(dlg.DoModal() == IDOK)
        {
        	WORD usRemoteNetworkId(0);
        	BYTE ubNodeId(0);

            dlg.GetParameter(&strGatewayName, &strProtocolStackName, &ubNodeId, &usRemoteNetworkId);

            //Open gateway
            keyHandle = p_pProtocolStackManager->PS_OpenProtocolStack(strProtocolStackName, strGatewayName, p_hCommunication, p_pErrorInfo);
            if(keyHandle)
            {
            	EParameterType eParameterType(PT_DEVICE);

                p_pProtocolStackManager->SetParameter(eParameterType, keyHandle, _T("NodeId"), &ubNodeId, sizeof(ubNodeId), p_pErrorInfo);
                p_pProtocolStackManager->SetParameter(eParameterType, keyHandle, _T("RemoteNetworkId"), (BYTE*)&usRemoteNetworkId, sizeof(usRemoteNetworkId), p_pErrorInfo);
            }
        }
#endif
    }

    DeleteGatewaySettingsSelection(&gatewaySettingsList);

    return keyHandle;
}

BOOL CProtocolStackDialogManager::InitGatewaySettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, tGatewaySettingsList* p_pList)
{
    CGatewaySettings* pGatewaySettings(0);
    CStdStringArray aGatewayProtocolStackName;
    CStdString GatewayProtocolStackName;
    BOOL oResult(FALSE);

    if(p_pProtocolStackManager && p_pList)
    {
        //Protocol stack name
        aGatewayProtocolStackName.push_back("CANopen");
 //       p_pProtocolStackManager->PS_GetProtocolStackNameSelection(&aGatewayProtocolStackName, );

        if(aGatewayProtocolStackName.size() > 0)
        {
            oResult = TRUE;
            for(std::size_t iPos = 0; iPos < aGatewayProtocolStackName.size(); iPos++)
            {
                GatewayProtocolStackName = aGatewayProtocolStackName.at(iPos);
                if(!InitGatewaySettingsSelection(p_pProtocolStackManager, GatewayProtocolStackName, p_pList))
                {
                    oResult = FALSE;
                    break;
                }
            }
        }
        else
        {
            //No gateway protocol stacks available
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackDialogManager::InitGatewaySettingsSelection(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_GatewayProtocolStackName, tGatewaySettingsList* p_pList)
{
    CGatewaySettings* pGatewaySettings(0);
    CStdStringArray aGatewayName;
    CStdString GatewayName;
    BOOL oResult(FALSE);

    if(p_pProtocolStackManager && p_pList)
    {
        //Gateway names
        p_pProtocolStackManager->PS_GetDeviceNameSelection(p_GatewayProtocolStackName, &aGatewayName);

        if(aGatewayName.size() > 0)
        {
            oResult = TRUE;
            for(std::size_t iPos = 0; iPos < aGatewayName.size(); iPos++)
            {
                GatewayName = aGatewayName.at(iPos);

                pGatewaySettings = new CGatewaySettings();
                pGatewaySettings->Init(GatewayName, p_GatewayProtocolStackName);
                p_pList->push_back(pGatewaySettings);
            }
        }
        else
        {
            //No gateways available
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CProtocolStackDialogManager::DeleteGatewaySettingsSelection(tGatewaySettingsList* p_pList)
{
	for( tGatewaySettingsList::iterator it = p_pList->begin();
			it != p_pList->end(); it++ )
	{
		CGatewaySettings* pGatewaySettings = (*it);
		if(pGatewaySettings!=0) delete pGatewaySettings;
	}
	p_pList->clear();

	return TRUE;
}
