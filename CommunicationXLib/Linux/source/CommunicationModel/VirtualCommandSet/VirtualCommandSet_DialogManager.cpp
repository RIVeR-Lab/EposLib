#include <list>
#include <string>

#include <CommunicationModel/VirtualCommandSet/Common/DeviceSettings.h>
#include <CommunicationModel/VirtualCommandSet/Common/ProtocolStackSettings.h>
#include <CommunicationModel/VirtualCommandSet/Dialogs/OpenDriveDlg.h>
#include <CommunicationModel/VirtualCommandSet/Dialogs/OpenPlcDlg.h>
#include <CommunicationModel/VirtualCommandSet/Dialogs/SearchCommunicationDlg.h>
#include <CommunicationModel/VirtualCommandSet/VirtualCommandSet_Manager.h>
#include <CommunicationModel/VirtualCommandSet/VirtualCommandSet_DialogManager.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirtualCommandSet_DialogManager::CVirtualCommandSet_DialogManager()
{
    m_ulTimeout = 0;
}

CVirtualCommandSet_DialogManager::~CVirtualCommandSet_DialogManager()
{
}

HANDLE CVirtualCommandSet_DialogManager::OpenVirtualDeviceDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdStringArray *p_pstrVirtualDeviceNameArray, CErrorInfo* p_pErrorInfo)
{
    HANDLE keyHandle = 0;
    tDeviceSettingsList deviceSettingsList;
    CStdString p_strVirtualDeviceName = "";
    CStdString strDeviceName = "";
    CStdString p_strInterfaceName = "";
    CStdString p_strPortName = "";
    CStdString strProtocolStackName = "";

    DeleteDeviceSettingsSelection(&deviceSettingsList);

    return keyHandle;
}

BOOL CVirtualCommandSet_DialogManager::InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings(NULL);
    CStdString strDeviceNameSel;
    CStdStringArray strDeviceNameArray;
    BOOL oResult(FALSE);

    if(p_pVirtualCommandSet_Manager && p_pList)
    {
        //DeviceName Selection
        p_pVirtualCommandSet_Manager->VCS_GetDeviceNameSelection(p_strVirtualDeviceName, &strDeviceNameArray);

        if(strDeviceNameArray.size() > 0)
        {
            //DeviceSettings with Devices
            oResult = TRUE;

            for( CStdStringArray::iterator it = strDeviceNameArray.begin(); it != strDeviceNameArray.end(); it++)
            {
            	strDeviceNameSel = (*it);
            	if(!InitDeviceSettingsSelection(p_pVirtualCommandSet_Manager, p_strVirtualDeviceName, strDeviceNameSel, p_pList))
				{
					oResult = FALSE;
					break;
				}
            }
        }
        else
        {
            //DeviceSettings without Devices
            pDeviceSettings = new CDeviceSettings();
            pDeviceSettings->Init(p_strVirtualDeviceName, _T(""), _T(""), _T(""), _T(""));
            p_pList->push_back(pDeviceSettings);
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_DialogManager::InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings = NULL;
    CStdString strProtocolStackNameSel;
    CStdStringArray strProtocolStackNameArray;
    BOOL oResult(FALSE);

    if(p_pVirtualCommandSet_Manager && p_pList)
    {
        //ProtocolStackName Selection
        p_pVirtualCommandSet_Manager->VCS_GetProtocolStackNameSelection(p_strVirtualDeviceName, p_strDeviceName, &strProtocolStackNameArray);

        if(strProtocolStackNameArray.size() > 0)
        {
            //DeviceSettings with ProtocolStacks
            oResult = TRUE;
            for(CStdStringArray::iterator it = strProtocolStackNameArray.begin(); it != strProtocolStackNameArray.end(); it++)
            {
                strProtocolStackNameSel = (*it);
                if(!InitDeviceSettingsSelection(p_pVirtualCommandSet_Manager, p_strVirtualDeviceName, p_strDeviceName, strProtocolStackNameSel, p_pList))
                {
                    oResult = FALSE;
                    break;
                }
            }
        }
        else
        {
            //DeviceSettings without ProtocolStacks
            pDeviceSettings = new CDeviceSettings();
            pDeviceSettings->Init(p_strVirtualDeviceName, p_strDeviceName, _T(""), _T(""), _T(""));
            p_pList->push_back(pDeviceSettings);
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_DialogManager::InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings = NULL;
    CStdString strInterfaceNameSel;
    CStdStringArray strInterfaceNameArray;
    BOOL oResult(FALSE);

    if(p_pVirtualCommandSet_Manager && p_pList)
    {
        //InterfaceName Selection
        p_pVirtualCommandSet_Manager->VCS_GetInterfaceNameSelection(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, &strInterfaceNameArray);

        if(strInterfaceNameArray.size() > 0)
        {
            //DeviceSettings with Interfaces
            oResult = TRUE;
            for(CStdStringArray::iterator it = strInterfaceNameArray.begin(); it != strInterfaceNameArray.end(); it++)
            {
                strInterfaceNameSel = (*it);
                if(!InitDeviceSettingsSelection(p_pVirtualCommandSet_Manager, p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, strInterfaceNameSel, p_pList))
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
            pDeviceSettings->Init(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, _T(""), _T(""));
            p_pList->push_back(pDeviceSettings);
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_DialogManager::InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings = NULL;
    CStdString strPortNameSel;
    CStdStringArray strPortNameArray;
    BOOL oResult(FALSE);

    if(p_pVirtualCommandSet_Manager && p_pList)
    {
        //PortName Selection
        p_pVirtualCommandSet_Manager->VCS_GetPortNameSelection(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, &strPortNameArray);

        if(strPortNameArray.size() > 0)
        {
            //DeviceSettings with Ports
            oResult = TRUE;
            for(CStdStringArray::iterator it = strPortNameArray.begin(); it != strPortNameArray.end(); it++)
            {
                strPortNameSel = (*it);
                if(!InitDeviceSettingsSelection(p_pVirtualCommandSet_Manager, p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, strPortNameSel, p_pList))
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
            pDeviceSettings->Init(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, _T(""));
            p_pList->push_back(pDeviceSettings);
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CVirtualCommandSet_DialogManager::InitDeviceSettingsSelection(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdString p_strDeviceName, CStdString p_strProtocolStackName, CStdString p_strInterfaceName, CStdString p_strPortName, tDeviceSettingsList* p_pList)
{
    CDeviceSettings* pDeviceSettings = NULL;
    CProtocolStackSettings* pProtocolStackSettings = NULL;
    CProtocolStackSettings defaultProtocolStackSettings;

    CStdDWordArray ulBaudrateArray;
    DWORD dDefaultBaudrate, dDefaultTimeout;
    DWORD ulBaudrateSel;
    BOOL oResult(FALSE);

    if(p_pVirtualCommandSet_Manager && p_pList)
    {
        //Default Values
        p_pVirtualCommandSet_Manager->VCS_GetDefaultProtocolStackSettings(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, &dDefaultBaudrate, &dDefaultTimeout);
        defaultProtocolStackSettings.Init(dDefaultBaudrate, dDefaultTimeout);

        //DeviceSettings
        pDeviceSettings = new CDeviceSettings();
        pDeviceSettings->Init(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, "");
        pDeviceSettings->SetDefaultProtocolStackSettings(&defaultProtocolStackSettings);
        p_pList->push_back(pDeviceSettings);

        //Baudrate Selection
        p_pVirtualCommandSet_Manager->VCS_GetBaudrateSelection(p_strVirtualDeviceName, p_strDeviceName, p_strProtocolStackName, p_strInterfaceName, p_strPortName, &ulBaudrateArray);

        if(ulBaudrateArray.size() > 0)
        {
            //Baudrates available
            oResult = TRUE;
            for(std::vector<DWORD>::iterator it = ulBaudrateArray.begin(); it != ulBaudrateArray.end(); it++)
            {
                //ProtocolStackSettings
                ulBaudrateSel = (*it);
                pProtocolStackSettings = new CProtocolStackSettings();
                pProtocolStackSettings->Init(ulBaudrateSel, m_ulTimeout);
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

BOOL CVirtualCommandSet_DialogManager::DeleteDeviceSettingsSelection(tDeviceSettingsList* p_pList)
{
    for( tDeviceSettingsList::iterator it = p_pList->begin(); it!=p_pList->end(); it++)
    {
    	CDeviceSettings* pDeviceSettings = (*it);
    	delete pDeviceSettings;
    }
    p_pList->clear();

    return TRUE;
}

BOOL CVirtualCommandSet_DialogManager::SearchCommunicationDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, HANDLE *p_pKeyHandle, CStdString p_strVirtualDeviceName, CCommandRoot* p_pSearchCommand, CStdString p_strSearchingInfo, CStdString p_strRootDeviceName, CStdString p_strRootProtocolStackName, CStdString p_strRootInterfaceName, CStdString p_strRootPortName, EDialogMode p_eDialogMode)
{
    return FALSE;
}

HANDLE CVirtualCommandSet_DialogManager::OpenDriveDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrDriveNameArray, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
	HANDLE keyHandle(0);

    return keyHandle;
}

HANDLE CVirtualCommandSet_DialogManager::OpenPlcDlg(CVirtualCommandSet_Manager* p_pVirtualCommandSet_Manager, CStdString p_strVirtualDeviceName, CStdStringArray *p_pstrPlcNameArray, HANDLE p_hPS_BaseLayerHandle, CErrorInfo* p_pErrorInfo)
{
	HANDLE keyHandle(0);

    return keyHandle;
}


