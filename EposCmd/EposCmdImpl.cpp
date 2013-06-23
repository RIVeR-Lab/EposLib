/*************************************************************************************************************************************
**                    maxon motor ag, CH-6072 Sachseln
**************************************************************************************************************************************
**
** HEADER FILE:     Win32_EposCmd.h
**
** Summary:         Exported Functions for Windows 32-Bit DLL
**
** Date:            16.10.2003
** Dev. Platform:   Microsoft Visual Studio 2005
** Target:          Win9x, WinNT, Win2000 and WinXP
** Written by:      maxon motor ag, CH-6072 Sachseln
**
*************************************************************************************************************************************/

#include "CommunicationModel/CommunicationModel.h"
#include <string.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_CanOpen.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_InputsOutputs.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEpos2Def.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/DeviceErrorHandling.h>

#include "EposCmd.h"
#include "Definitions.h"
#include <Thread/MmcCriticalSection.h>
#include <Process/MmcProcess.h>
#include <MmcTypeDefinition.h>

void __attribute__ ((constructor)) EposCmdLoad(void);
void __attribute__ ((destructor)) EposCmdUnload(void);

//Parameters
CCommunicationModel *m_pCommunicationModel(0);
CMmcCriticalSection m_Sync;

/*************************************************************************************************************************************
* INITIALISATION FUNCTIONS
*************************************************************************************************************************************/

// Called when the library is loaded and before dlopen() returns
void EposCmdLoad(void)
{
    // Add initialization code…
	m_pDllInstance = new CEPOSLinuxSharedLibApp();
}

// Called when the library is unloaded and before dlclose()
// returns
void EposCmdUnload(void)
{
    // Add clean-up code…
	DWORD ulErrorCode;

	VCS_CloseAllDevices(&ulErrorCode);

	DeleteCommunication();

	delete m_pDllInstance;
}

BOOL CreateCommunication(void)
{
    int oResult(FALSE);

    m_pCommunicationModel = m_pDllInstance->CreateCommunicationModel();

    if(m_pCommunicationModel)
        oResult = m_pCommunicationModel->CreateVirtualCommandSetManager();
    else
        DeleteCommunication();

    return oResult;
}

BOOL DeleteCommunication(void)
{
    int oResult(TRUE);

    //Communication Model
    if(m_pDllInstance->DeleteCommunicationModel())
    {
        m_pCommunicationModel = NULL;
    }
    else
    {
        oResult = FALSE;
    }

    return oResult;
}

Initialisation_DllExport HANDLE __stdcall VCS_OpenDevice(char* DeviceName, char* ProtocolStackName, char* InterfaceName, char* PortName, DWORD* pErrorCode)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    HANDLE KeyHandle(NULL);
    CErrorInfo errorInfo;
    CStdString strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE2;

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    if(m_pCommunicationModel)
    {
        if(!m_pCommunicationModel->VCS_OpenVirtualDevice(strVirtualDeviceName, DeviceName, ProtocolStackName, InterfaceName, PortName, &KeyHandle, &errorInfo))
        {
            KeyHandle = NULL;
        }

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return KeyHandle;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return 0;
}

Initialisation_DllExport HANDLE __stdcall VCS_OpenDeviceDlg(DWORD* pErrorCode)
{
    CErrorInfo errorInfo;
    CMmcSingleLock lock(&m_Sync, TRUE);
    CStdStringArray strVirtualDeviceNameArray;
    HANDLE KeyHandle(NULL);

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    if(m_pCommunicationModel)
    {
        strVirtualDeviceNameArray.push_back(VIRTUAL_DEVICE_DRIVE2);

        if(!m_pCommunicationModel->VCS_OpenVirtualDeviceDlg(&strVirtualDeviceNameArray, &KeyHandle, &errorInfo))
        {
            KeyHandle = NULL;
        }

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return KeyHandle;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return 0;
}

Initialisation_DllExport int __stdcall VCS_SetProtocolStackSettings(void* KeyHandle, unsigned int Baudrate, unsigned int Timeout, unsigned int* pErrorCode)
{
    int oResult(FALSE);
    int oChangeOnly(FALSE);
    CErrorInfo errorInfo;

    if(m_pCommunicationModel)
    {
        oResult = m_pCommunicationModel->VCS_SetProtocolStackSettings(KeyHandle, Baudrate, Timeout, oChangeOnly, &errorInfo);

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return oResult;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

Initialisation_DllExport int __stdcall VCS_GetProtocolStackSettings(HANDLE KeyHandle, DWORD* pBaudrate, DWORD* pTimeout, DWORD* pErrorCode)
{
    int oResult(FALSE);
    CErrorInfo errorInfo;

    if(m_pCommunicationModel)
    {
        oResult = m_pCommunicationModel->VCS_GetProtocolStackSettings(KeyHandle, pBaudrate, pTimeout, &errorInfo);

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return oResult;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

Initialisation_DllExport int __stdcall VCS_FindDeviceCommunicationSettings(HANDLE *pKeyHandle, char *pDeviceName, char *pProtocolStackName, char *pInterfaceName, char *pPortName, WORD SizeName, DWORD *pBaudrate, DWORD *pTimeout, WORD *pNodeId, int DialogMode, DWORD *pErrorCode)
{
    int oResult(FALSE);
    CCommand_VCS_Common command(COMMON_GET_DEVICE_TYPE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);
    DWORD ulNbOfBytesRead(0);
    WORD usNodeIdSearch(0);
    BYTE ubNodeId(0);
    CStdString strVirtualDeviceName = _T("");;
    int oCommunicationFound(TRUE);

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    if(m_pCommunicationModel && pKeyHandle)
    {
        //Init
        usNodeIdSearch = 0;
        ubNodeId = (BYTE)usNodeIdSearch;

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

		//Search Communication
/*		if(!m_pCommunicationModel->SearchCommunicationWholeCanIdRangeDlg(*pKeyHandle, &command, "Searching communication", strVirtualDeviceName, pDeviceName, pProtocolStackName, pInterfaceName, pPortName, pNodeId, pKeyHandle, (EDialogMode) DialogMode))
        {
            if(pErrorCode) *pErrorCode = k_Error_NoCommunicationFound;
            oCommunicationFound = FALSE;
        }
*/
        //Return Parameter
		if(oCommunicationFound == TRUE)
        {
            //Get Settings
			oResult = VCS_GetDeviceName(*pKeyHandle, pDeviceName, SizeName, pErrorCode);
            if(oResult) oResult = VCS_GetProtocolStackName(*pKeyHandle, pProtocolStackName, SizeName, pErrorCode);
            if(oResult) oResult = VCS_GetInterfaceName(*pKeyHandle, pInterfaceName, SizeName, pErrorCode);
            if(oResult) oResult = VCS_GetPortName(*pKeyHandle, pPortName, SizeName, pErrorCode);
            if(oResult) oResult = VCS_GetProtocolStackSettings(*pKeyHandle, pBaudrate, pTimeout, pErrorCode);

            //Read NodeId from Object if Scanning with Node Id 0
			if(0 == (*pNodeId))
            {
                //Object node id (EPOS and EPOS2)
                if(oResult) oResult = VCS_GetObject(*pKeyHandle, usNodeIdSearch, INDEX_NODE_ID, SUBINDEX_NODE_ID, pNodeId, sizeof(*pNodeId), &ulNbOfBytesRead, pErrorCode);
            }
        }
    }
    else
    {
        if(pErrorCode) *pErrorCode = k_Error_Internal;
    }

    return oResult;
}

Initialisation_DllExport int __stdcall VCS_CloseDevice(HANDLE KeyHandle, DWORD* pErrorCode)
{
    CMmcSingleLock lock(&m_Sync, TRUE);
    int oResult(TRUE);
    CErrorInfo errorInfo;

    if(m_pCommunicationModel)
    {
        if(m_pCommunicationModel->VCS_CloseVirtualDevice(KeyHandle, &errorInfo))
		{
			oResult = TRUE;
			if(m_pCommunicationModel->VCS_AreAllVirtualDevicesClosed())
			{
				oResult = DeleteCommunication();
				return oResult;
			}
		}
		else
		{
	        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
		    return oResult;
		}
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

Initialisation_DllExport int __stdcall VCS_CloseAllDevices(DWORD* pErrorCode)
{
    int oResult(FALSE);
    CMmcSingleLock lock(&m_Sync, TRUE);
    CErrorInfo errorInfo;

    if(m_pCommunicationModel)
    {
        if(m_pCommunicationModel->VCS_CloseAllVirtualDevices(&errorInfo))
		{
			oResult = TRUE;
			if(m_pCommunicationModel->VCS_AreAllVirtualDevicesClosed())
			{
				DeleteCommunication();
			}
		}
		else
		{
			if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
			return oResult;
		}
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

HelpFunctions_DllExport int __stdcall VCS_GetVersion(HANDLE KeyHandle, WORD NodeId, WORD* pHardwareVersion, WORD* pSoftwareVersion, WORD* pApplicationNumber, WORD* pApplicationVersion, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Common command(COMMON_GET_VERSION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pHardwareVersion, sizeof(*pHardwareVersion));
        command.GetReturnParameterData(1, pSoftwareVersion, sizeof(*pSoftwareVersion));
        command.GetReturnParameterData(2, pApplicationNumber, sizeof(*pApplicationNumber));
        command.GetReturnParameterData(3, pApplicationVersion, sizeof(*pApplicationVersion));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

HelpFunctions_DllExport int __stdcall VCS_GetErrorInfo(DWORD ErrorCodeValue, char* pErrorInfo, WORD MaxStrSize)
{
    int oResult(FALSE);
    CStdString strDescription;

    if(!oResult) oResult = CErrorHandling::GetErrorDescription(ErrorCodeValue, &strDescription);
	if(!oResult) oResult = CDeviceErrorHandling::GetErrorDescription(ErrorCodeValue, &strDescription);

    //Check if enough memory allocated for error information string
    if(strDescription.GetLength() < MaxStrSize)
        strcpy(pErrorInfo, strDescription.c_str());
    else
        oResult = FALSE;

    return oResult;
}

//Advanced Functions
HelpFunctions_DllExport int __stdcall VCS_GetDeviceNameSelection(int StartOfSelection, char* pDeviceNameSel, WORD MaxStrSize, int* pEndOfSelection, DWORD* pErrorCode)
{
    static CStdStringArray strNameArray;
    static CStdStringArray strNameArrayTemp;
    static size_t lIndex(0);
    CStdString strName;
    CErrorInfo errorInfo;
    size_t lCount(0);

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    //GetSelection
    if(StartOfSelection)
    {
        strNameArray.clear();
        if(!m_pCommunicationModel->VCS_GetDeviceNameSelection(VIRTUAL_DEVICE_DRIVE2, &strNameArrayTemp, &errorInfo))
        {
                if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
                return FALSE;
        }
        else
        {
            for(lCount = 0; lCount < strNameArrayTemp.size(); lCount++)
            {
                strNameArray.push_back(strNameArrayTemp[lCount]);
            }
        }

        lIndex = 0;
    }

    //Check Index
    if(lIndex >= strNameArray.size())
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Get SelectionValue
    strName = strNameArray[lIndex++];
    if(pDeviceNameSel && (strName.GetLength() < MaxStrSize))
    {
        strcpy(pDeviceNameSel, strName);
    }
    else
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Check End
    if(pEndOfSelection)
    {
        *pEndOfSelection = (lIndex >= strNameArray.size());
    }

    return TRUE;
}

HelpFunctions_DllExport int __stdcall VCS_GetProtocolStackNameSelection(char* DeviceName, int StartOfSelection, char* pProtocolStackNameSel, WORD MaxStrSize,  int* pEndOfSelection, DWORD* pErrorCode)
{
    static CStdStringArray strNameArray;
    static size_t lIndex(0);
    CStdString strName;
    CErrorInfo errorInfo;
    CStdString strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE2;

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    //GetSelection
    if(StartOfSelection)
    {
        if(!m_pCommunicationModel->VCS_GetProtocolStackNameSelection(strVirtualDeviceName, DeviceName, &strNameArray, &errorInfo))
        {
            if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
            return FALSE;
        }
        lIndex = 0;
    }

    //Check Index
    if(lIndex >= strNameArray.size())
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Get SelectionValue
    strName = strNameArray[lIndex++];
    if(pProtocolStackNameSel && (strName.GetLength() < MaxStrSize))
    {
        strcpy(pProtocolStackNameSel, strName);
    }
    else
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Check End
    if(pEndOfSelection)
    {
        *pEndOfSelection = (lIndex >= strNameArray.size());
    }

    return TRUE;
}

HelpFunctions_DllExport int __stdcall VCS_GetInterfaceNameSelection(char* DeviceName, char* ProtocolStackName, int StartOfSelection, char* pInterfaceNameSel, WORD MaxStrSize,  int* pEndOfSelection, DWORD* pErrorCode)
{
    static CStdStringArray strNameArray;
    static size_t lIndex(0);
    CStdString strName;
    CErrorInfo errorInfo;
    CStdString strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE2;

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    //GetSelection
    if(StartOfSelection)
    {
        if(!m_pCommunicationModel->VCS_GetInterfaceNameSelection(strVirtualDeviceName, DeviceName, ProtocolStackName, &strNameArray, &errorInfo))
        {
            if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
            return FALSE;
        }
        lIndex = 0;
    }

    //Check Index
    if(lIndex >= strNameArray.size())
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Get SelectionValue
    strName = strNameArray[lIndex++];
    if(pInterfaceNameSel && (strName.GetLength() < MaxStrSize))
    {
        strcpy(pInterfaceNameSel,  strName);
    }
    else
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Check End
    if(pEndOfSelection)
    {
        *pEndOfSelection = (lIndex >= strNameArray.size());
    }

    return TRUE;
}

HelpFunctions_DllExport int __stdcall VCS_GetPortNameSelection(char* DeviceName, char* ProtocolStackName, char* InterfaceName, int StartOfSelection, char* pPortSel, WORD MaxStrSize, int* pEndOfSelection, DWORD* pErrorCode)
{
    static CStdStringArray strNameArray;
    static size_t lIndex(0);
    CStdString strName;
    CErrorInfo errorInfo;
    CStdString strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE2;

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    //GetSelection
    if(StartOfSelection)
    {
        if(!m_pCommunicationModel->VCS_GetPortNameSelection(strVirtualDeviceName, DeviceName, ProtocolStackName, InterfaceName, &strNameArray, &errorInfo))
        {
            if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
            return FALSE;
        }
        lIndex = 0;
    }

    //Check Index
    if(lIndex >= strNameArray.size())
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Get SelectionValue
    strName = strNameArray[lIndex++];
    if(pPortSel && (strName.GetLength() < MaxStrSize))
    {
        strcpy(pPortSel, strName);
    }
    else
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Check End
    if(pEndOfSelection)
    {
        *pEndOfSelection = (lIndex >= strNameArray.size());
    }

    return TRUE;
}

HelpFunctions_DllExport int __stdcall VCS_GetBaudrateSelection(char* DeviceName, char* ProtocolStackName, char* InterfaceName, char* PortName, int StartOfSelection, DWORD* pBaudrateSel, int* pEndOfSelection, DWORD* pErrorCode)
{
    static CStdDWordArray dBaudrateArray;
    static size_t lIndex(0);
    CErrorInfo errorInfo;
    CStdString strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE2;

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    //GetSelection
    if(StartOfSelection)
    {
        if(!m_pCommunicationModel->VCS_GetBaudrateSelection(strVirtualDeviceName, DeviceName, ProtocolStackName, InterfaceName, PortName, &dBaudrateArray, &errorInfo))
        {
            if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
            return FALSE;
        }
        lIndex = 0;
    }

    //Check Index
    if(lIndex >= dBaudrateArray.size())
    {
        if(pErrorCode) *pErrorCode = k_Error_BadParameter;
        return FALSE;
    }

    //Get SelectionValue
    if(pBaudrateSel)
    {
        *pBaudrateSel = dBaudrateArray[lIndex++];
    }

    //Check End
    if(pEndOfSelection)
    {
        *pEndOfSelection = (lIndex >= dBaudrateArray.size());
    }

    return TRUE;
}

HelpFunctions_DllExport int __stdcall VCS_GetKeyHandle(char* DeviceName, char* ProtocolStackName, char* InterfaceName, char* PortName, HANDLE* pKeyHandle, DWORD* pErrorCode)
{
    int oResult(FALSE);
    CErrorInfo errorInfo;
    CStdString strVirtualDeviceName = VIRTUAL_DEVICE_DRIVE2;

    //Create Communication
    if(!m_pCommunicationModel) CreateCommunication();

    if(m_pCommunicationModel)
    {
        oResult = m_pCommunicationModel->VCS_GetKeyHandle(strVirtualDeviceName, DeviceName, ProtocolStackName, InterfaceName, PortName, pKeyHandle, &errorInfo);

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return oResult;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

HelpFunctions_DllExport int __stdcall VCS_GetDeviceName(HANDLE KeyHandle, char* pDeviceName, WORD MaxStrSize, DWORD* pErrorCode)
{
    int oResult(FALSE);
    CErrorInfo errorInfo;
    CStdString strName;

    if(m_pCommunicationModel)
    {
        oResult = m_pCommunicationModel->VCS_GetDeviceName(KeyHandle, &strName, &errorInfo);
        if(oResult)
        {
            if(strName.GetLength() >= MaxStrSize)
            {
                if(pErrorCode) *pErrorCode = k_Error_BadParameter;
                return FALSE;
            }

            strcpy(pDeviceName,  strName);
        }

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return oResult;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

HelpFunctions_DllExport int __stdcall VCS_GetProtocolStackName(HANDLE KeyHandle, char* pProtocolStackName, WORD MaxStrSize, DWORD* pErrorCode)
{
    int oResult(FALSE);
    CErrorInfo errorInfo;
    CStdString strName;

    if(m_pCommunicationModel)
    {
        oResult = m_pCommunicationModel->VCS_GetProtocolStackName(KeyHandle, &strName, &errorInfo);
        if(oResult)
        {
            if(strName.GetLength() >= MaxStrSize)
            {
                if(pErrorCode) *pErrorCode = k_Error_BadParameter;
                return FALSE;
            }

            strcpy(pProtocolStackName,  strName);
        }

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return oResult;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

HelpFunctions_DllExport int __stdcall VCS_GetInterfaceName(HANDLE KeyHandle, char* pInterfaceName, WORD MaxStrSize, DWORD* pErrorCode)
{
    int oResult(FALSE);
    CStdString strName;
    CErrorInfo errorInfo;

    if(m_pCommunicationModel)
    {
        oResult = m_pCommunicationModel->VCS_GetInterfaceName(KeyHandle, &strName, &errorInfo);
        if(oResult)
        {
            if(strName.GetLength() >= MaxStrSize)
            {
                if(pErrorCode) *pErrorCode = k_Error_BadParameter;
                return FALSE;
            }

            strcpy(pInterfaceName,  strName);
        }

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return oResult;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

HelpFunctions_DllExport int __stdcall VCS_GetPortName(HANDLE KeyHandle, char* pPortName, WORD MaxStrSize, DWORD* pErrorCode)
{
    int oResult(FALSE);
    CErrorInfo errorInfo;
    CStdString strName;

    if(m_pCommunicationModel)
    {
        oResult = m_pCommunicationModel->VCS_GetPortName(KeyHandle, &strName, &errorInfo);
        if(oResult)
        {
            if(strName.GetLength() >= MaxStrSize)
            {
                if(pErrorCode) *pErrorCode = k_Error_BadParameter;
                return FALSE;
            }

            strcpy(pPortName,  strName);
        }

        if(pErrorCode) *pErrorCode = errorInfo.GetErrorCode();
        return oResult;
    }

    if(pErrorCode) *pErrorCode = k_Error_Internal;
    return FALSE;
}

/*************************************************************************************************************************************
* CONFIGURATION FUNCTIONS
*************************************************************************************************************************************/

Configuration_DllExport int __stdcall VCS_SetObject(HANDLE KeyHandle, WORD NodeId, WORD ObjectIndex, BYTE ObjectSubIndex, void* pData, DWORD NbOfBytesToWrite, DWORD* pNbOfBytesWritten, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_SET_OBJECT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &ObjectIndex, sizeof(ObjectIndex));
        command.SetParameterData(1, &ObjectSubIndex, sizeof(ObjectSubIndex));
        command.SetParameterData(2, pData, NbOfBytesToWrite);
        command.SetParameterData(3, &NbOfBytesToWrite, sizeof(NbOfBytesToWrite));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pNbOfBytesWritten, sizeof(*pNbOfBytesWritten));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetObject(HANDLE KeyHandle, WORD NodeId, WORD ObjectIndex, BYTE ObjectSubIndex, void* pData, DWORD NbOfBytesToRead, DWORD* pNbOfBytesRead, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_GET_OBJECT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &ObjectIndex, sizeof(ObjectIndex));
        command.SetParameterData(1, &ObjectSubIndex, sizeof(ObjectSubIndex));
        command.SetParameterData(2, &NbOfBytesToRead, sizeof(NbOfBytesToRead));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pData, NbOfBytesToRead);
        command.GetReturnParameterData(1, pNbOfBytesRead, sizeof(*pNbOfBytesRead));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_Restore(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_RESTORE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_Store(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_STORE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Motor
Configuration_DllExport int __stdcall VCS_SetMotorType(HANDLE KeyHandle, WORD NodeId, WORD MotorType, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_MOTOR_TYPE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &MotorType, sizeof(MotorType));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetDcMotorParameter(HANDLE KeyHandle, WORD NodeId, WORD NominalCurrent, WORD MaxOutputCurrent, WORD ThermalTimeConstant, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_DC_MOTOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &NominalCurrent, sizeof(NominalCurrent));
        command.SetParameterData(1, &MaxOutputCurrent, sizeof(MaxOutputCurrent));
        command.SetParameterData(2, &ThermalTimeConstant, sizeof(ThermalTimeConstant));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetEcMotorParameter(HANDLE KeyHandle, WORD NodeId, WORD NominalCurrent, WORD MaxOutputCurrent, WORD ThermalTimeConstant, BYTE NbOfPolePairs, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_EC_MOTOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &NominalCurrent, sizeof(NominalCurrent));
        command.SetParameterData(1, &MaxOutputCurrent, sizeof(MaxOutputCurrent));
        command.SetParameterData(2, &ThermalTimeConstant, sizeof(ThermalTimeConstant));
        command.SetParameterData(3, &NbOfPolePairs, sizeof(NbOfPolePairs));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetMotorType(HANDLE KeyHandle, WORD NodeId, WORD* pMotorType, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_MOTOR_TYPE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pMotorType, sizeof(*pMotorType));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetDcMotorParameter(HANDLE KeyHandle, WORD NodeId, WORD* pNominalCurrent, WORD* pMaxOutputCurrent, WORD* pThermalTimeConstant, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_DC_MOTOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pNominalCurrent, sizeof(*pNominalCurrent));
        command.GetReturnParameterData(1, pMaxOutputCurrent, sizeof(*pMaxOutputCurrent));
        command.GetReturnParameterData(2, pThermalTimeConstant, sizeof(*pThermalTimeConstant));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetEcMotorParameter(HANDLE KeyHandle, WORD NodeId, WORD* pNominalCurrent, WORD* pMaxOutputCurrent, WORD* pThermalTimeConstant, BYTE* pNbOfPolePairs, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_EC_MOTOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pNominalCurrent, sizeof(*pNominalCurrent));
        command.GetReturnParameterData(1, pMaxOutputCurrent, sizeof(*pMaxOutputCurrent));
        command.GetReturnParameterData(2, pThermalTimeConstant, sizeof(*pThermalTimeConstant));
        command.GetReturnParameterData(3, pNbOfPolePairs, sizeof(*pNbOfPolePairs));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Sensor
Configuration_DllExport int __stdcall VCS_SetSensorType(HANDLE KeyHandle, WORD NodeId, WORD SensorType, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_SENSOR_TYPE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &SensorType, sizeof(SensorType));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetIncEncoderParameter(HANDLE KeyHandle, WORD NodeId, DWORD EncoderResolution, int InvertedPolarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_INC_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &EncoderResolution, sizeof(EncoderResolution));
        command.SetParameterData(1, &InvertedPolarity, sizeof(InvertedPolarity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetHallSensorParameter(HANDLE KeyHandle, WORD NodeId, int InvertedPolarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_HALL_SENSOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &InvertedPolarity, sizeof(InvertedPolarity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetSsiAbsEncoderParameter(HANDLE KeyHandle, WORD NodeId, WORD DataRate, WORD NbOfMultiTurnDataBits, WORD NbOfSingleTurnDataBits, int InvertedPolarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_SSI_ABS_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &DataRate, sizeof(DataRate));
        command.SetParameterData(1, &NbOfMultiTurnDataBits, sizeof(NbOfMultiTurnDataBits));
        command.SetParameterData(2, &NbOfSingleTurnDataBits, sizeof(NbOfSingleTurnDataBits));
        command.SetParameterData(3, &InvertedPolarity, sizeof(InvertedPolarity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetSensorType(HANDLE KeyHandle, WORD NodeId, WORD* pSensorType, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_SENSOR_TYPE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pSensorType, sizeof(*pSensorType));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetIncEncoderParameter(HANDLE KeyHandle, WORD NodeId, DWORD* pEncoderResolution, int* pInvertedPolarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_INC_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pEncoderResolution, sizeof(*pEncoderResolution));
        command.GetReturnParameterData(1, pInvertedPolarity, sizeof(*pInvertedPolarity));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetHallSensorParameter(HANDLE KeyHandle, WORD NodeId, int* pInvertedPolarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_HALL_SENSOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pInvertedPolarity, sizeof(*pInvertedPolarity));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetSsiAbsEncoderParameter(HANDLE KeyHandle, WORD NodeId, WORD* pDataRate, WORD* pNbOfMultiTurnDataBits, WORD* pNbOfSingleTurnDataBits, int* pInvertedPolarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_SSI_ABS_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pDataRate, sizeof(*pDataRate));
        command.GetReturnParameterData(1, pNbOfMultiTurnDataBits, sizeof(*pNbOfMultiTurnDataBits));
        command.GetReturnParameterData(2, pNbOfSingleTurnDataBits, sizeof(*pNbOfSingleTurnDataBits));
        command.GetReturnParameterData(3, pInvertedPolarity, sizeof(*pInvertedPolarity));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Safety
Configuration_DllExport int __stdcall VCS_SetMaxFollowingError(HANDLE KeyHandle, WORD NodeId, DWORD MaxFollowingError, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_MAX_FOLLOWING_ERROR);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &MaxFollowingError, sizeof(MaxFollowingError));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetMaxFollowingError(HANDLE KeyHandle, WORD NodeId, DWORD* pMaxFollowingError, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_MAX_FOLLOWING_ERROR);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pMaxFollowingError, sizeof(*pMaxFollowingError));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetMaxProfileVelocity(HANDLE KeyHandle, WORD NodeId, DWORD MaxProfileVelocity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_MAX_PROFILE_VELOCITY);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &MaxProfileVelocity, sizeof(MaxProfileVelocity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetMaxProfileVelocity(HANDLE KeyHandle, WORD NodeId, DWORD* pMaxProfileVelocity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_MAX_PROFILE_VELOCITY);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pMaxProfileVelocity, sizeof(*pMaxProfileVelocity));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetMaxAcceleration(HANDLE KeyHandle, WORD NodeId, DWORD MaxAcceleration, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_MAX_ACCELERATION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &MaxAcceleration, sizeof(MaxAcceleration));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetMaxAcceleration(HANDLE KeyHandle, WORD NodeId, DWORD* pMaxAcceleration, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_MAX_ACCELERATION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pMaxAcceleration, sizeof(*pMaxAcceleration));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Position Regulator
Configuration_DllExport int __stdcall VCS_SetPositionRegulatorGain(HANDLE KeyHandle, WORD NodeId, WORD P, WORD I, WORD D, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_POSITION_REGULATOR_GAIN);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &P, sizeof(P));
        command.SetParameterData(1, &I, sizeof(I));
        command.SetParameterData(2, &D, sizeof(D));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetPositionRegulatorFeedForward(HANDLE KeyHandle, WORD NodeId, WORD VelocityFeedForward, WORD AccelerationFeedForward, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_POSITION_REGULATOR_FEED_FORWARD);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &VelocityFeedForward, sizeof(VelocityFeedForward));
        command.SetParameterData(1, &AccelerationFeedForward, sizeof(AccelerationFeedForward));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetPositionRegulatorGain(HANDLE KeyHandle, WORD NodeId, WORD* pP, WORD* pI, WORD* pD, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_POSITION_REGULATOR_GAIN);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pP, sizeof(*pP));
        command.GetReturnParameterData(1, pI, sizeof(*pI));
        command.GetReturnParameterData(2, pD, sizeof(*pD));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetPositionRegulatorFeedForward(HANDLE KeyHandle, WORD NodeId, WORD* pVelocityFeedForward, WORD* pAccelerationFeedForward, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_POSITION_REGULATOR_FEED_FORWARD);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pVelocityFeedForward, sizeof(*pVelocityFeedForward));
        command.GetReturnParameterData(1, pAccelerationFeedForward, sizeof(*pAccelerationFeedForward));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Velocity Regulator
Configuration_DllExport int __stdcall VCS_SetVelocityRegulatorGain(HANDLE KeyHandle, WORD NodeId, WORD P, WORD I, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_VELOCITY_REGULATOR_GAIN);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &P, sizeof(P));
        command.SetParameterData(1, &I, sizeof(I));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetVelocityRegulatorGain(HANDLE KeyHandle, WORD NodeId, WORD* pP, WORD* pI, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_VELOCITY_REGULATOR_GAIN);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pP, sizeof(*pP));
        command.GetReturnParameterData(1, pI, sizeof(*pI));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport BOOL __stdcall VCS_SetVelocityRegulatorFeedForward(HANDLE KeyHandle, WORD NodeId, WORD VelocityFeedForward, WORD AccelerationFeedForward, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_VELOCITY_REGULATOR_FEED_FORWARD);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &VelocityFeedForward, sizeof(VelocityFeedForward));
        command.SetParameterData(1, &AccelerationFeedForward, sizeof(AccelerationFeedForward));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport BOOL __stdcall VCS_GetVelocityRegulatorFeedForward(HANDLE KeyHandle, WORD NodeId, WORD* pVelocityFeedForward, WORD* pAccelerationFeedForward, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_VELOCITY_REGULATOR_FEED_FORWARD);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pVelocityFeedForward, sizeof(*pVelocityFeedForward));
        command.GetReturnParameterData(1, pAccelerationFeedForward, sizeof(*pAccelerationFeedForward));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Current Regulator
Configuration_DllExport int __stdcall VCS_SetCurrentRegulatorGain(HANDLE KeyHandle, WORD NodeId, WORD P, WORD I, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_CURRENT_REGULATOR_GAIN);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &P, sizeof(P));
        command.SetParameterData(1, &I, sizeof(I));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetCurrentRegulatorGain(HANDLE KeyHandle, WORD NodeId, WORD* pP, WORD* pI, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_CURRENT_REGULATOR_GAIN);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pP, sizeof(*pP));
        command.GetReturnParameterData(1, pI, sizeof(*pI));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Inputs/Outputs
Configuration_DllExport int __stdcall VCS_DigitalInputConfiguration(HANDLE KeyHandle, WORD NodeId, WORD DigitalInputNb, WORD Configuration, int Mask, int Polarity, int ExecutionMask, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_DIGITAL_INPUT_CONFIGURATION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &DigitalInputNb, sizeof(DigitalInputNb));
        command.SetParameterData(1, &Configuration, sizeof(Configuration));
        command.SetParameterData(2, &Mask, sizeof(Mask));
        command.SetParameterData(3, &Polarity, sizeof(Polarity));
        command.SetParameterData(4, &ExecutionMask, sizeof(ExecutionMask));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_DigitalOutputConfiguration(HANDLE KeyHandle, WORD NodeId, WORD DigitalOutputNb, WORD Configuration, int State, int Mask, int Polarity, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_DIGITAL_OUTPUT_CONFIGURATION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &DigitalOutputNb, sizeof(DigitalOutputNb));
        command.SetParameterData(1, &Configuration, sizeof(Configuration));
        command.SetParameterData(2, &State, sizeof(State));
        command.SetParameterData(3, &Mask, sizeof(Mask));
        command.SetParameterData(4, &Polarity, sizeof(Polarity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_AnalogInputConfiguration(HANDLE KeyHandle, WORD NodeId, WORD AnalogInputNb, WORD Configuration, int ExecutionMask, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_ANALOG_INPUT_CONFIGURATION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &AnalogInputNb, sizeof(AnalogInputNb));
        command.SetParameterData(1, &Configuration, sizeof(Configuration));
        command.SetParameterData(2, &ExecutionMask, sizeof(ExecutionMask));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Units
Configuration_DllExport int __stdcall VCS_SetVelocityUnits(HANDLE KeyHandle, WORD NodeId, BYTE VelDimension, char VelNotation, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_VELOCITY_UNITS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &VelDimension, sizeof(VelDimension));
        command.SetParameterData(1, &VelNotation, sizeof(VelNotation));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetVelocityUnits(HANDLE KeyHandle, WORD NodeId, BYTE* pVelDimension, char* pVelNotation, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_VELOCITY_UNITS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pVelDimension, sizeof(*pVelDimension));
        command.GetReturnParameterData(1, pVelNotation, sizeof(*pVelNotation));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Compatibility Functions (do not use)
Configuration_DllExport int __stdcall VCS_SetMotorParameter(HANDLE KeyHandle, WORD NodeId, WORD MotorType, WORD ContinuousCurrent, WORD PeakCurrent, BYTE PolePair, WORD ThermalTimeConstant, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_MOTOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &MotorType, sizeof(MotorType));
        command.SetParameterData(1, &ContinuousCurrent, sizeof(ContinuousCurrent));
        command.SetParameterData(2, &PeakCurrent, sizeof(PeakCurrent));
        command.SetParameterData(3, &PolePair, sizeof(PolePair));
        command.SetParameterData(4, &ThermalTimeConstant, sizeof(ThermalTimeConstant));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_SetEncoderParameter(HANDLE KeyHandle, WORD NodeId, WORD Counts, WORD PositionSensorType, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &Counts, sizeof(Counts));
        command.SetParameterData(1, &PositionSensorType, sizeof(PositionSensorType));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetMotorParameter(HANDLE KeyHandle, WORD NodeId, WORD* pMotorType, WORD* pContinuousCurrent, WORD* pPeakCurrent, BYTE* pPolePair, WORD* pThermalTimeConstant, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_MOTOR_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pMotorType, sizeof(*pMotorType));
        command.GetReturnParameterData(1, pContinuousCurrent, sizeof(*pContinuousCurrent));
        command.GetReturnParameterData(2, pPeakCurrent, sizeof(*pPeakCurrent));
        command.GetReturnParameterData(3, pPolePair, sizeof(*pPolePair));
        command.GetReturnParameterData(4, pThermalTimeConstant, sizeof(*pThermalTimeConstant));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Configuration_DllExport int __stdcall VCS_GetEncoderParameter(HANDLE KeyHandle, WORD NodeId, WORD* pCounts, WORD* pPositionSensorType, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pCounts, sizeof(*pCounts));
        command.GetReturnParameterData(1, pPositionSensorType, sizeof(*pPositionSensorType));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

/*************************************************************************************************************************************
* OPERATION FUNCTIONS
*************************************************************************************************************************************/

//OperationMode
Status_DllExport int __stdcall VCS_SetOperationMode(HANDLE KeyHandle, WORD NodeId, __int8 OperationMode, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_OPERATION_MODE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &OperationMode, sizeof(OperationMode));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

Status_DllExport int __stdcall VCS_GetOperationMode(HANDLE KeyHandle, WORD NodeId, __int8* pOperationMode, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_OPERATION_MODE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pOperationMode, sizeof(*pOperationMode));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//StateMachine
StateMachine_DllExport int __stdcall VCS_ResetDevice(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
	BOOL oWaitForReboot(TRUE);

    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Common command(COMMON_RESET_DEVICE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

		//Parameter
		command.SetParameterData(0, &oWaitForReboot, sizeof(oWaitForReboot));

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_SetEnableState(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_ENABLE_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_SetState(HANDLE KeyHandle, WORD NodeId, WORD State, DWORD* pErrorCode)
{
    int oResult(FALSE);

    if(m_pCommunicationModel)
    {
        if(ST_DISABLED == State)
        {
            oResult = VCS_SetDisableState(KeyHandle, NodeId, pErrorCode);
        }
        else if(ST_ENABLED == State)
        {
            oResult = VCS_SetEnableState(KeyHandle, NodeId, pErrorCode);
        }
        else if(ST_QUICKSTOP == State)
        {
            oResult = VCS_SetQuickStopState(KeyHandle, NodeId, pErrorCode);
        }
        else if(ST_FAULT == State)
        {
        }
        else
        {
        }
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_SetDisableState(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_DISABLE_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_SetQuickStopState(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_QUICK_STOP_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_ClearFault(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_CLEAR_FAULT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter("NodeId", &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_GetState(HANDLE KeyHandle, WORD NodeId, WORD* pState, DWORD* pErrorCode)
{
    int oResult(FALSE);

    int oIsDisabled(FALSE);
    int oIsEnabled(FALSE);
    int oIsQuickStopped(FALSE);
    int oIsInFault(FALSE);
    int oReadNextState(TRUE);

    if(m_pCommunicationModel)
    {
        oResult = VCS_GetDisableState(KeyHandle, NodeId, &oIsDisabled, pErrorCode);
        if(oReadNextState) oReadNextState = !oIsDisabled;

        if(oResult && oReadNextState && !VCS_GetEnableState(KeyHandle, NodeId, &oIsEnabled, pErrorCode)) oResult = FALSE;
        if(oReadNextState) oReadNextState = !oIsEnabled;

        if(oResult && oReadNextState && !VCS_GetQuickStopState(KeyHandle, NodeId, &oIsQuickStopped, pErrorCode)) oResult = FALSE;
        if(oReadNextState) oReadNextState = !oIsQuickStopped;

        if(oResult && oReadNextState && !VCS_GetFaultState(KeyHandle, NodeId, &oIsInFault, pErrorCode)) oResult = FALSE;
        if(oReadNextState) oReadNextState = !oIsInFault;

        if(oResult)
        {
            if(oIsDisabled) *pState = ST_DISABLED;
            else if(oIsEnabled) *pState = ST_ENABLED;
            else if(oIsQuickStopped) *pState = ST_QUICKSTOP;
            else if(oIsInFault) *pState = ST_FAULT;
            else *pState = 0;
        }
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_GetEnableState(HANDLE KeyHandle, WORD NodeId, int* pIsEnabled, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_ENABLE_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pIsEnabled, sizeof(*pIsEnabled));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_GetDisableState(HANDLE KeyHandle, WORD NodeId, int* pIsDisabled, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_DISABLE_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pIsDisabled, sizeof(*pIsDisabled));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_GetQuickStopState(HANDLE KeyHandle, WORD NodeId, int* pIsQuickStopped, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_QUICK_STOP_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pIsQuickStopped, sizeof(*pIsQuickStopped));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StateMachine_DllExport int __stdcall VCS_GetFaultState(HANDLE KeyHandle, WORD NodeId, int* pIsInFault, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_FAULT_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pIsInFault, sizeof(*pIsInFault));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//ErrorHandling
ErrorHandling_DllExport int __stdcall VCS_GetNbOfDeviceError(HANDLE KeyHandle, WORD NodeId, BYTE *pNbDeviceError, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Common command(COMMON_GET_NB_OF_DEVICE_ERRORS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pNbDeviceError, sizeof(*pNbDeviceError));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ErrorHandling_DllExport int __stdcall VCS_GetDeviceErrorCode(HANDLE KeyHandle, WORD NodeId, BYTE SubIndex, DWORD *pDeviceErrorCode, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Common command(COMMON_GET_DEVICE_ERROR_CODE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &SubIndex, sizeof(SubIndex));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pDeviceErrorCode, sizeof(*pDeviceErrorCode));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Motion Info
MotionInfo_DllExport int __stdcall VCS_GetMovementState(HANDLE KeyHandle, WORD NodeId, int* pTargetReached, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_MOVEMENT_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pTargetReached, sizeof(*pTargetReached));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

MotionInfo_DllExport int __stdcall VCS_GetPositionIs(void* KeyHandle, unsigned short NodeId, int* pPositionIs, unsigned int* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_POSITION_IS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pPositionIs, sizeof(*pPositionIs));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

MotionInfo_DllExport int __stdcall VCS_GetVelocityIs(void* KeyHandle, unsigned short NodeId, int* pVelocityIs, unsigned int* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_VELOCITY_IS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pVelocityIs, sizeof(*pVelocityIs));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

MotionInfo_DllExport int VCS_GetVelocityIsAveraged(void* KeyHandle, unsigned short NodeId, int* pVelocityIsAveraged, unsigned int* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_VELOCITY_IS_AVERAGED);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pVelocityIsAveraged, sizeof(*pVelocityIsAveraged));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}


MotionInfo_DllExport int __stdcall VCS_GetCurrentIs(HANDLE KeyHandle, WORD NodeId, short* pCurrentIs, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_CURRENT_IS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pCurrentIs, sizeof(*pCurrentIs));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

MotionInfo_DllExport int VCS_GetCurrentIsAveraged(void* KeyHandle, unsigned short NodeId, short* pCurrentIsAveraged, unsigned int* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_CURRENT_IS_AVERAGED);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pCurrentIsAveraged, sizeof(*pCurrentIsAveraged));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

MotionInfo_DllExport int __stdcall VCS_WaitForTargetReached(HANDLE KeyHandle, WORD NodeId, DWORD Timeout, DWORD* pErrorCode)
{
    int oResult(FALSE);
    int oTargetReached(FALSE);

    DWORD ulTimeStart;
    DWORD ulTimeRunOff;

    //Start Time
    ulTimeStart = MmcGetTickCount();
    ulTimeRunOff = 0;

    while(!oTargetReached && (Timeout > ulTimeRunOff))
    {
        //Execute Command
        oResult = VCS_GetMovementState(KeyHandle, NodeId, &oTargetReached, pErrorCode);

        //Time run off
        ulTimeRunOff = MmcGetTickCount() - ulTimeStart;
    }

    if(!oTargetReached && pErrorCode)
    {
        if(*pErrorCode == 0)
        {
            *pErrorCode = k_Error_Timeout;
            oResult = FALSE;
        }
    }

    return oResult;
}

//Profile Position Mode
ProfilePositionMode_DllExport int __stdcall VCS_ActivateProfilePositionMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_PROFILE_POSITION_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

ProfilePositionMode_DllExport int __stdcall VCS_SetPositionProfile(HANDLE KeyHandle, WORD NodeId, DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_POSITION_PROFILE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &ProfileVelocity, sizeof(ProfileVelocity));
        command.SetParameterData(1, &ProfileAcceleration, sizeof(ProfileAcceleration));
        command.SetParameterData(2, &ProfileDeceleration, sizeof(ProfileDeceleration));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfilePositionMode_DllExport int __stdcall VCS_GetPositionProfile(HANDLE KeyHandle, WORD NodeId, DWORD* pProfileVelocity, DWORD* pProfileAcceleration, DWORD* pProfileDeceleration, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_POSITION_PROFILE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pProfileVelocity, sizeof(*pProfileVelocity));
        command.GetReturnParameterData(1, pProfileAcceleration, sizeof(*pProfileAcceleration));
        command.GetReturnParameterData(2, pProfileDeceleration, sizeof(*pProfileDeceleration));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfilePositionMode_DllExport int __stdcall VCS_MoveToPosition(HANDLE KeyHandle, WORD NodeId, long TargetPosition, int Absolute, int Immediately, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_MOVE_TO_POSITION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &TargetPosition, sizeof(TargetPosition));
        command.SetParameterData(1, &Absolute, sizeof(Absolute));
        command.SetParameterData(2, &Immediately, sizeof(Immediately));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfilePositionMode_DllExport int VCS_GetTargetPosition(HANDLE KeyHandle, WORD NodeId, long* pTargetPosition, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_TARGET_POSITION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pTargetPosition, sizeof(*pTargetPosition));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfilePositionMode_DllExport int VCS_HaltPositionMovement(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_HALT_POSITION_MOVEMENT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Advanced Functions PPM
ProfilePositionMode_DllExport int VCS_EnablePositionWindow(HANDLE KeyHandle, WORD NodeId, DWORD PositionWindow, WORD PositionWindowTime, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ENABLE_POSITION_WINDOW);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &PositionWindow, sizeof(PositionWindow));
        command.SetParameterData(1, &PositionWindowTime, sizeof(PositionWindowTime));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfilePositionMode_DllExport int VCS_DisablePositionWindow(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DISABLE_POSITION_WINDOW);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Profile Velocity Mode
ProfileVelocityMode_DllExport int VCS_ActivateProfileVelocityMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_PROFILE_VELOCITY_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

ProfileVelocityMode_DllExport int VCS_SetVelocityProfile(HANDLE KeyHandle, WORD NodeId, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_VELOCITY_PROFILE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &ProfileAcceleration, sizeof(ProfileAcceleration));
        command.SetParameterData(1, &ProfileDeceleration, sizeof(ProfileDeceleration));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfileVelocityMode_DllExport int VCS_GetVelocityProfile(HANDLE KeyHandle, WORD NodeId, DWORD* pProfileAcceleration, DWORD* pProfileDeceleration, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_VELOCITY_PROFILE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pProfileAcceleration, sizeof(*pProfileAcceleration));
        command.GetReturnParameterData(1, pProfileDeceleration, sizeof(*pProfileDeceleration));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfileVelocityMode_DllExport int VCS_MoveWithVelocity(HANDLE KeyHandle, WORD NodeId, long TargetVelocity, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_MOVE_WITH_VELOCITY);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &TargetVelocity, sizeof(TargetVelocity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfileVelocityMode_DllExport int VCS_GetTargetVelocity(HANDLE KeyHandle, WORD NodeId, long* pTargetVelocity, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_TARGET_VELOCITY);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pTargetVelocity, sizeof(*pTargetVelocity));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfileVelocityMode_DllExport int VCS_HaltVelocityMovement(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_HALT_VELOCITY_MOVEMENT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Advanced Functions PVM
ProfileVelocityMode_DllExport int VCS_EnableVelocityWindow(HANDLE KeyHandle, WORD NodeId, DWORD VelocityWindow, WORD VelocityWindowTime, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ENABLE_VELOCITY_WINDOW);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &VelocityWindow, sizeof(VelocityWindow));
        command.SetParameterData(1, &VelocityWindowTime, sizeof(VelocityWindowTime));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

ProfileVelocityMode_DllExport int VCS_DisableVelocityWindow(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DISABLE_VELOCITY_WINDOW);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Homing Mode
HomingMode_DllExport int VCS_ActivateHomingMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_HOMING_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

HomingMode_DllExport int VCS_SetHomingParameter(HANDLE KeyHandle, WORD NodeId, DWORD HomingAcceleration, DWORD SpeedSwitch, DWORD SpeedIndex, long HomeOffset, WORD CurrentTreshold, long HomePosition, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_HOMING_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &HomingAcceleration, sizeof(HomingAcceleration));
        command.SetParameterData(1, &SpeedSwitch, sizeof(SpeedSwitch));
        command.SetParameterData(2, &SpeedIndex, sizeof(SpeedIndex));
        command.SetParameterData(3, &HomeOffset, sizeof(HomeOffset));
        command.SetParameterData(4, &CurrentTreshold, sizeof(CurrentTreshold));
        command.SetParameterData(5, &HomePosition, sizeof(HomePosition));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

HomingMode_DllExport int VCS_GetHomingParameter(HANDLE KeyHandle, WORD NodeId, DWORD* pHomingAcceleration, DWORD* pSpeedSwitch, DWORD* pSpeedIndex, long* pHomeOffset, WORD* pCurrentTreshold, long* pHomePosition, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_HOMING_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pHomingAcceleration, sizeof(*pHomingAcceleration));
        command.GetReturnParameterData(1, pSpeedSwitch, sizeof(*pSpeedSwitch));
        command.GetReturnParameterData(2, pSpeedIndex, sizeof(*pSpeedIndex));
        command.GetReturnParameterData(3, pHomeOffset, sizeof(*pHomeOffset));
        command.GetReturnParameterData(4, pCurrentTreshold, sizeof(*pCurrentTreshold));
        command.GetReturnParameterData(5, pHomePosition, sizeof(*pHomePosition));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

HomingMode_DllExport int VCS_FindHome(HANDLE KeyHandle, WORD NodeId, __int8 HomingMethod, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_FIND_HOME);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &HomingMethod, sizeof(HomingMethod));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

HomingMode_DllExport int VCS_StopHoming(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_STOP_HOMING);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

HomingMode_DllExport int VCS_DefinePosition(void* KeyHandle, unsigned short NodeId, int HomePosition, unsigned int *pErrorCode)
{
    const DWORD MAX_TIMEOUT(100);

    BOOL oResult(FALSE);
    BOOL oIsEnabled(FALSE);
    char bOperationalModeOld(0);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    //Is device enabled?
    if(VCS_GetEnableState(KeyHandle, NodeId, &oIsEnabled, pErrorCode) && oIsEnabled)
    {
        //Set homing mode
        oResult = VCS_GetOperationMode(KeyHandle, NodeId, &bOperationalModeOld, pErrorCode);
        if(oResult && (OM_HOMING_MODE != bOperationalModeOld))
        {
            oResult = VCS_ActivateHomingMode(KeyHandle, NodeId, pErrorCode);
        }

        //Set home position
        if(oResult)
        {
            CCommand_VCS_Drive2 command(DRIVE2_DEFINE_POSITION);
            CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

            if(m_pCommunicationModel)
            {
                //Start Transaction
                //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

                //Set Parameter
                command.SetParameterData(0, &HomePosition, sizeof(HomePosition));

                //Address Parameter
                deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
                command.PushLayerParameterSet(deviceParameterSet);

                //Execute Command
                oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

                //Get ReturnParameter
                command.GetErrorCode(pErrorCode);

                //Stop Transaction
                //StopTransaction(hTransactionHandle);
            }
        }

        //Start homing
        if(oResult)
        {
            oResult = VCS_FindHome(KeyHandle, NodeId, HM_ACTUAL_POSITION, pErrorCode);
        }

        //Wait for homing attained
        if(oResult)
        {
            oResult = VCS_WaitForHomingAttained(KeyHandle, NodeId, MAX_TIMEOUT, pErrorCode);
        }

        //Activate old operational mode
        if(oResult)
        {
            oResult = VCS_SetOperationMode(KeyHandle, NodeId, bOperationalModeOld, pErrorCode);
        }
    }

    return oResult;
}

HomingMode_DllExport int VCS_WaitForHomingAttained(void* KeyHandle, unsigned short NodeId, int Timeout, unsigned int* pErrorCode)
{
    int oResult(FALSE);
    int oHomingAttained(FALSE);
    int oHomingError(FALSE);
    int oTimeoutReached(FALSE);
    unsigned long ulTimeStart(0);

    //Start Time
    ulTimeStart = MmcGetTickCount();

    do
    {
        //Execute Command
        oResult = VCS_GetHomingState(KeyHandle, NodeId, &oHomingAttained, &oHomingError, pErrorCode);

        //Check timeout
        oTimeoutReached = (MmcGetTickCount() - ulTimeStart) > Timeout;
    }
    while(oResult && !oHomingAttained && !oHomingError && !oTimeoutReached);

    if(oResult && !oHomingAttained)
    {
        if(pErrorCode)
        {
            if(oHomingError)
            {
                *pErrorCode = ERROR_VIRTUAL_DEVICE_HOMING_ERROR;
            }
            else if(oTimeoutReached)
            {
                *pErrorCode = k_Error_Timeout;
            }
        }

        oResult = FALSE;
    }

    return oResult;
}

HomingMode_DllExport int VCS_GetHomingState(void* KeyHandle, unsigned short NodeId, int* pHomingAttained, int* pHomingError, unsigned int* pErrorCode)
{
    int oResult(FALSE);

    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_HOMING_STATE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pHomingAttained, sizeof(*pHomingAttained));
        command.GetReturnParameterData(1, pHomingError, sizeof(*pHomingError));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Interpolated Position Mode
InterpolatedPositionMode_DllExport int VCS_ActivateInterpolatedPositionMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_INTERPOLATED_POSITION_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

InterpolatedPositionMode_DllExport int VCS_SetIpmBufferParameter(HANDLE KeyHandle, WORD NodeId, WORD UnderflowWarningLimit, WORD OverflowWarningLimit, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_IPM_BUFFER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &UnderflowWarningLimit, sizeof(UnderflowWarningLimit));
        command.SetParameterData(1, &OverflowWarningLimit, sizeof(OverflowWarningLimit));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InterpolatedPositionMode_DllExport int __stdcall VCS_GetIpmBufferParameter(HANDLE KeyHandle, WORD NodeId, WORD* pUnderflowWarningLimit, WORD* pOverflowWarningLimit, DWORD* pMaxBufferSize, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_IPM_BUFFER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pUnderflowWarningLimit, sizeof(*pUnderflowWarningLimit));
        command.GetReturnParameterData(1, pOverflowWarningLimit, sizeof(*pOverflowWarningLimit));
        command.GetReturnParameterData(2, pMaxBufferSize, sizeof(*pMaxBufferSize));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InterpolatedPositionMode_DllExport int __stdcall VCS_ClearIpmBuffer(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_CLEAR_IPM_BUFFER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InterpolatedPositionMode_DllExport int __stdcall VCS_GetFreeIpmBufferSize(HANDLE KeyHandle, WORD NodeId, DWORD* pBufferSize, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_FREE_IPM_BUFFER_SIZE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pBufferSize, sizeof(*pBufferSize));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InterpolatedPositionMode_DllExport int __stdcall VCS_AddPvtValueToIpmBuffer(HANDLE KeyHandle, WORD NodeId, long Position, long Velocity, BYTE Time, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ADD_PVT_VALUE_TO_IPM_BUFFER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &Position, sizeof(Position));
        command.SetParameterData(1, &Velocity, sizeof(Velocity));
        command.SetParameterData(2, &Time, sizeof(Time));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InterpolatedPositionMode_DllExport int __stdcall VCS_StartIpmTrajectory(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_START_IPM_TRAJECTORY);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InterpolatedPositionMode_DllExport int __stdcall VCS_StopIpmTrajectory(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_STOP_IPM_TRAJECTORY);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InterpolatedPositionMode_DllExport int __stdcall VCS_GetIpmStatus(HANDLE KeyHandle, WORD NodeId, int* pTrajectoryRunning, int* pIsUnderflowWarning, int* pIsOverflowWarning, int* pIsVelocityWarning, int* pIsAccelerationWarning, int* pIsUnderflowError, int* pIsOverflowError, int* pIsVelocityError, int* pIsAccelerationError, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_IPM_STATUS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pTrajectoryRunning, sizeof(*pTrajectoryRunning));
        command.GetReturnParameterData(1, pIsUnderflowWarning, sizeof(*pIsUnderflowWarning));
        command.GetReturnParameterData(2, pIsOverflowWarning, sizeof(*pIsOverflowWarning));
        command.GetReturnParameterData(3, pIsVelocityWarning, sizeof(*pIsVelocityWarning));
        command.GetReturnParameterData(4, pIsAccelerationWarning, sizeof(*pIsAccelerationWarning));
        command.GetReturnParameterData(5, pIsUnderflowError, sizeof(*pIsUnderflowError));
        command.GetReturnParameterData(6, pIsOverflowError, sizeof(*pIsOverflowError));
        command.GetReturnParameterData(7, pIsVelocityError, sizeof(*pIsVelocityError));
        command.GetReturnParameterData(8, pIsAccelerationError, sizeof(*pIsAccelerationError));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Position Mode
PositionMode_DllExport int __stdcall VCS_ActivatePositionMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_POSITION_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

PositionMode_DllExport int __stdcall VCS_SetPositionMust(HANDLE KeyHandle, WORD NodeId, long PositionMust, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_POSITION_MUST);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &PositionMust, sizeof(PositionMust));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

PositionMode_DllExport int __stdcall VCS_GetPositionMust(HANDLE KeyHandle, WORD NodeId, long* pPositionMust, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_POSITION_MUST);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pPositionMust, sizeof(*pPositionMust));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Advanced Functions Position Mode
PositionMode_DllExport int __stdcall VCS_ActivateAnalogPositionSetpoint(HANDLE KeyHandle, WORD NodeId, WORD AnalogInputNumber, float Scaling, long Offset, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ACTIVATE_ANALOG_POSITION_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &AnalogInputNumber, sizeof(AnalogInputNumber));
        command.SetParameterData(1, &Scaling, sizeof(Scaling));
        command.SetParameterData(2, &Offset, sizeof(Offset));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

PositionMode_DllExport int __stdcall VCS_DeactivateAnalogPositionSetpoint(HANDLE KeyHandle, WORD NodeId, WORD AnalogInputNumber, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DEACTIVATE_ANALOG_POSITION_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &AnalogInputNumber, sizeof(AnalogInputNumber));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

PositionMode_DllExport int __stdcall VCS_EnableAnalogPositionSetpoint(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ENABLE_ANALOG_POSITION_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

PositionMode_DllExport int __stdcall VCS_DisableAnalogPositionSetpoint(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DISABLE_ANALOG_POSITION_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Velocity Mode
VelocityMode_DllExport int __stdcall VCS_ActivateVelocityMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_VELOCITY_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

VelocityMode_DllExport int __stdcall VCS_SetVelocityMust(HANDLE KeyHandle, WORD NodeId, long VelocityMust, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_VELOCITY_MUST);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &VelocityMust, sizeof(VelocityMust));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

VelocityMode_DllExport int __stdcall VCS_GetVelocityMust(HANDLE KeyHandle, WORD NodeId, long* pVelocityMust, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_VELOCITY_MUST);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pVelocityMust, sizeof(*pVelocityMust));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Advanced Functions
VelocityMode_DllExport int __stdcall VCS_ActivateAnalogVelocitySetpoint(HANDLE KeyHandle, WORD NodeId, WORD AnalogInputNumber, float Scaling, long Offset, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ACTIVATE_ANALOG_VELOCITY_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &AnalogInputNumber, sizeof(AnalogInputNumber));
        command.SetParameterData(1, &Scaling, sizeof(Scaling));
        command.SetParameterData(2, &Offset, sizeof(Offset));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

VelocityMode_DllExport int __stdcall VCS_DeactivateAnalogVelocitySetpoint(HANDLE KeyHandle, WORD NodeId, WORD AnalogInputNumber, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DEACTIVATE_ANALOG_POSITION_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &AnalogInputNumber, sizeof(AnalogInputNumber));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

VelocityMode_DllExport int __stdcall VCS_EnableAnalogVelocitySetpoint(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ENABLE_ANALOG_VELOCITY_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

VelocityMode_DllExport int __stdcall VCS_DisableAnalogVelocitySetpoint(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DISABLE_ANALOG_VELOCITY_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Current Mode
CurrentMode_DllExport int __stdcall VCS_ActivateCurrentMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_CURRENT_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

CurrentMode_DllExport int __stdcall VCS_SetCurrentMust(HANDLE KeyHandle, WORD NodeId, short CurrentMust, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_CURRENT_MUST);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &CurrentMust, sizeof(CurrentMust));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

CurrentMode_DllExport int __stdcall VCS_GetCurrentMust(HANDLE KeyHandle, WORD NodeId, short* pCurrentMust, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_CURRENT_MUST);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pCurrentMust, sizeof(*pCurrentMust));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Advanced Functions Current Mode
CurrentMode_DllExport int __stdcall VCS_ActivateAnalogCurrentSetpoint(HANDLE KeyHandle, WORD NodeId, WORD AnalogInputNumber, float Scaling, short Offset, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ACTIVATE_ANALOG_CURRENT_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &AnalogInputNumber, sizeof(AnalogInputNumber));
        command.SetParameterData(1, &Scaling, sizeof(Scaling));
        command.SetParameterData(2, &Offset, sizeof(Offset));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

CurrentMode_DllExport int __stdcall VCS_DeactivateAnalogCurrentSetpoint(HANDLE KeyHandle, WORD NodeId, WORD AnalogInputNumber, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DEACTIVATE_ANALOG_CURRENT_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &AnalogInputNumber, sizeof(AnalogInputNumber));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

CurrentMode_DllExport int __stdcall VCS_EnableAnalogCurrentSetpoint(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ENABLE_ANALOG_CURRENT_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

CurrentMode_DllExport int __stdcall VCS_DisableAnalogCurrentSetpoint(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DISABLE_ANALOG_CURRENT_SETPOINT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//MasterEncoder Mode
MasterEncoderMode_DllExport int __stdcall VCS_ActivateMasterEncoderMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_MASTER_ENCODER_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

MasterEncoderMode_DllExport int __stdcall VCS_SetMasterEncoderParameter(HANDLE KeyHandle, WORD NodeId, WORD ScalingNumerator, WORD ScalingDenominator, BYTE Polarity, DWORD MaxVelocity, DWORD MaxAcceleration, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_MASTER_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &ScalingNumerator, sizeof(ScalingNumerator));
        command.SetParameterData(1, &ScalingDenominator, sizeof(ScalingDenominator));
        command.SetParameterData(2, &Polarity, sizeof(Polarity));
        command.SetParameterData(3, &MaxVelocity, sizeof(MaxVelocity));
        command.SetParameterData(4, &MaxAcceleration, sizeof(MaxAcceleration));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

MasterEncoderMode_DllExport int __stdcall VCS_GetMasterEncoderParameter(HANDLE KeyHandle, WORD NodeId, WORD* pScalingNumerator, WORD* pScalingDenominator, BYTE* pPolarity, DWORD* pMaxVelocity, DWORD* pMaxAcceleration, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_MASTER_ENCODER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pScalingNumerator, sizeof(*pScalingNumerator));
        command.GetReturnParameterData(1, pScalingDenominator, sizeof(*pScalingDenominator));
        command.GetReturnParameterData(2, pPolarity, sizeof(*pPolarity));
        command.GetReturnParameterData(3, pMaxVelocity, sizeof(*pMaxVelocity));
        command.GetReturnParameterData(4, pMaxAcceleration, sizeof(*pMaxAcceleration));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//StepDirection Mode
StepDirectionMode_DllExport int __stdcall VCS_ActivateStepDirectionMode(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    __int8 OperationMode(OM_STEP_DIRECTION_MODE);

    return VCS_SetOperationMode(KeyHandle, NodeId, OperationMode, pErrorCode);
}

StepDirectionMode_DllExport int __stdcall VCS_SetStepDirectionParameter(HANDLE KeyHandle, WORD NodeId, WORD ScalingNumerator, WORD ScalingDenominator, BYTE Polarity, DWORD MaxVelocity, DWORD MaxAcceleration, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_STEP_DIRECTION_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &ScalingNumerator, sizeof(ScalingNumerator));
        command.SetParameterData(1, &ScalingDenominator, sizeof(ScalingDenominator));
        command.SetParameterData(2, &Polarity, sizeof(Polarity));
        command.SetParameterData(3, &MaxVelocity, sizeof(MaxVelocity));
        command.SetParameterData(4, &MaxAcceleration, sizeof(MaxAcceleration));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

StepDirectionMode_DllExport int __stdcall VCS_GetStepDirectionParameter(HANDLE KeyHandle, WORD NodeId, WORD* pScalingNumerator, WORD* pScalingDenominator, BYTE* pPolarity, DWORD* pMaxVelocity, DWORD* pMaxAcceleration, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_STEP_DIRECTION_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pScalingNumerator, sizeof(*pScalingNumerator));
        command.GetReturnParameterData(1, pScalingDenominator, sizeof(*pScalingDenominator));
        command.GetReturnParameterData(2, pPolarity, sizeof(*pPolarity));
        command.GetReturnParameterData(3, pMaxVelocity, sizeof(*pMaxVelocity));
        command.GetReturnParameterData(4, pMaxAcceleration, sizeof(*pMaxAcceleration));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Inputs Outputs
//General
InputsOutputs_DllExport int __stdcall VCS_GetAllDigitalInputs(HANDLE KeyHandle, WORD NodeId, WORD* pInputs, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_GET_ALL_DIGITAL_INPUTS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pInputs, sizeof(*pInputs));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_GetAllDigitalOutputs(HANDLE KeyHandle, WORD NodeId, WORD* pOutputs, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_GET_ALL_DIGITAL_OUTPUTS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pOutputs, sizeof(*pOutputs));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_SetAllDigitalOutputs(HANDLE KeyHandle, WORD NodeId, WORD Outputs, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_SET_ALL_DIGITAL_OUTPUTS);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &Outputs, sizeof(Outputs));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_GetAnalogInput(HANDLE KeyHandle, WORD NodeId, WORD InputNumber, WORD* pAnalogValue, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_GET_ANALOG_INPUT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &InputNumber, sizeof(InputNumber));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pAnalogValue, sizeof(*pAnalogValue));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_SetAnalogOutput(HANDLE KeyHandle, WORD NodeId, WORD OutputNumber, WORD AnalogValue, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_InputsOutputs command(IO_SET_ANALOG_OUTPUT);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &OutputNumber, sizeof(OutputNumber));
        command.SetParameterData(1, &AnalogValue, sizeof(AnalogValue));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Position Compare
InputsOutputs_DllExport int __stdcall VCS_SetPositionCompareParameter(HANDLE KeyHandle, WORD NodeId, BYTE OperationalMode, BYTE IntervalMode, BYTE DirectionDependency, WORD IntervalWidth, WORD IntervalRepetitions, WORD PulseWidth, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_POSITION_COMPARE_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &OperationalMode, sizeof(OperationalMode));
        command.SetParameterData(1, &IntervalMode, sizeof(IntervalMode));
        command.SetParameterData(2, &DirectionDependency, sizeof(DirectionDependency));
        command.SetParameterData(3, &IntervalWidth, sizeof(IntervalWidth));
        command.SetParameterData(4, &IntervalRepetitions, sizeof(IntervalRepetitions));
        command.SetParameterData(5, &PulseWidth, sizeof(PulseWidth));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_GetPositionCompareParameter(HANDLE KeyHandle, WORD NodeId, BYTE* pOperationalMode, BYTE* pIntervalMode, BYTE* pDirectionDependency, WORD* pIntervalWidth, WORD* pIntervalRepetitions, WORD* pPulseWidth, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_POSITION_COMPARE_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pOperationalMode, sizeof(*pOperationalMode));
        command.GetReturnParameterData(1, pIntervalMode, sizeof(*pIntervalMode));
        command.GetReturnParameterData(2, pDirectionDependency, sizeof(*pDirectionDependency));
        command.GetReturnParameterData(3, pIntervalWidth, sizeof(*pIntervalWidth));
        command.GetReturnParameterData(4, pIntervalRepetitions, sizeof(*pIntervalRepetitions));
        command.GetReturnParameterData(5, pPulseWidth, sizeof(*pPulseWidth));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_ActivatePositionCompare(HANDLE KeyHandle, WORD NodeId, WORD DigitalOutputNumber, int Polarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ACTIVATE_POSITION_COMPARE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &DigitalOutputNumber, sizeof(DigitalOutputNumber));
        command.SetParameterData(1, &Polarity, sizeof(Polarity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_DeactivatePositionCompare(HANDLE KeyHandle, WORD NodeId, WORD DigitalOutputNumber, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DEACTIVATE_POSITION_COMPARE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &DigitalOutputNumber, sizeof(DigitalOutputNumber));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_EnablePositionCompare(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ENABLE_POSITION_COMPARE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_DisablePositionCompare(HANDLE KeyHandle, WORD NodeId, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DISABLE_POSITION_COMPARE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_SetPositionCompareReferencePosition(HANDLE KeyHandle, WORD NodeId, long ReferencePosition, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_POSITION_COMPARE_REFERENCE_POSITION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &ReferencePosition, sizeof(ReferencePosition));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

//Position Marker
InputsOutputs_DllExport int __stdcall VCS_SetPositionMarkerParameter(HANDLE KeyHandle, WORD NodeId, BYTE PositionMarkerEdgeType, BYTE PositionMarkerMode, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_SET_POSITION_MARKER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &PositionMarkerEdgeType, sizeof(PositionMarkerEdgeType));
        command.SetParameterData(1, &PositionMarkerMode, sizeof(PositionMarkerMode));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_GetPositionMarkerParameter(HANDLE KeyHandle, WORD NodeId, BYTE* pPositionMarkerEdgeType, BYTE* pPositionMarkerMode, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_GET_POSITION_MARKER_PARAMETER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pPositionMarkerEdgeType, sizeof(*pPositionMarkerEdgeType));
        command.GetReturnParameterData(1, pPositionMarkerMode, sizeof(*pPositionMarkerMode));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_ActivatePositionMarker(HANDLE KeyHandle, WORD NodeId, WORD DigitalInputNumber, int Polarity, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_ACTIVATE_POSITION_MARKER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &DigitalInputNumber, sizeof(DigitalInputNumber));
        command.SetParameterData(1, &Polarity, sizeof(Polarity));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_DeactivatePositionMarker(HANDLE KeyHandle, WORD NodeId, WORD DigitalInputNumber, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_DEACTIVATE_POSITION_MARKER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &DigitalInputNumber, sizeof(DigitalInputNumber));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_ReadPositionMarkerCounter(HANDLE KeyHandle, WORD NodeId, WORD* pCount, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_READ_POSITION_MARKER_COUNTER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pCount, sizeof(*pCount));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_ReadPositionMarkerCapturedPosition(HANDLE KeyHandle, WORD NodeId, WORD CounterIndex, long* pCapturedPosition, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_READ_POSITION_MARKER_CAPTURED_POSITION);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &CounterIndex, sizeof(CounterIndex));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pCapturedPosition, sizeof(*pCapturedPosition));
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

InputsOutputs_DllExport int __stdcall VCS_ResetPositionMarkerCounter(HANDLE KeyHandle, WORD NodeId, DWORD *pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_Drive2 command(DRIVE2_RESET_POSITION_MARKER_COUNTER);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

/*************************************************************************************************************************************
* LOW LAYER FUNCTIONS
*************************************************************************************************************************************/

//CanLayer Functions
CanLayer_DllExport int __stdcall VCS_SendCANFrame(HANDLE KeyHandle, WORD CobID, WORD Length, void* pData, DWORD* pErrorCode)
{
    int oResult(FALSE);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_SEND_CAN_FRAME);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &CobID, sizeof(CobID));
        command.SetParameterData(1, &Length, sizeof(Length));
        command.SetParameterData(2, pData, Length);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

CanLayer_DllExport int __stdcall VCS_ReadCANFrame(HANDLE KeyHandle, WORD CobID, WORD Length, void* pData, DWORD Timeout, DWORD *pErrorCode)
{
    int oResult(FALSE);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_READ_CAN_FRAME);

    if(m_pCommunicationModel)
            {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &CobID, sizeof(CobID));
        command.SetParameterData(1, &Length, sizeof(Length));
        command.SetParameterData(2, &Timeout, sizeof(Timeout));

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pData, Length);
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
            }

    return oResult;
        }

CanLayer_DllExport int __stdcall VCS_RequestCANFrame(HANDLE KeyHandle, WORD CobID, WORD Length, void* pData, DWORD* pErrorCode)
{
    int oResult(FALSE);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_REQUEST_CAN_FRAME);

    if(m_pCommunicationModel)
            {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &CobID, sizeof(CobID));
        command.SetParameterData(1, &Length, sizeof(Length));

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetReturnParameterData(0, pData, Length);
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}

CanLayer_DllExport int __stdcall VCS_SendNMTService(HANDLE KeyHandle, WORD NodeId, WORD CommandSpecifier, DWORD* pErrorCode)
{
    int oResult(FALSE);
    BYTE ubNodeId((BYTE)NodeId);
    //HANDLE hJournalHandle(NULL);
    HANDLE hTransactionHandle(NULL);

    CCommand_VCS_CanOpen command(VCS_CANOPEN_SEND_NMT_SERVICE);
    CLayerParameterSet deviceParameterSet(DEVICE_COMMAND_SET_LAYER);

    if(m_pCommunicationModel)
    {
        //Start Transaction
        //hTransactionHandle = StartTransaction(KeyHandle, hJournalHandle);

        //Set Parameter
        command.SetParameterData(0, &CommandSpecifier, sizeof(CommandSpecifier));

        //Address Parameter
        deviceParameterSet.InitParameter(_T("NodeId"), &ubNodeId, sizeof(ubNodeId));
        command.PushLayerParameterSet(deviceParameterSet);

        //Execute Command
        oResult = m_pCommunicationModel->ExecuteCommand(&command, KeyHandle, hTransactionHandle);

        //Get ReturnParameter
        command.GetErrorCode(pErrorCode);

        //Stop Transaction
        //StopTransaction(hTransactionHandle);
    }

    return oResult;
}
