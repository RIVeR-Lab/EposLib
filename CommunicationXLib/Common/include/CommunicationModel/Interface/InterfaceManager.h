#if !defined(AFX_INTERFACEMANAGER_H__0F52D262_5C84_4F7D_816F_3DE0F0AA6833__INCLUDED_)
#define AFX_INTERFACEMANAGER_H__0F52D262_5C84_4F7D_816F_3DE0F0AA6833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include <CommunicationModel/Interface/Classes/Handle/HandleRegistrationMap_I.h>
#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/DeviceInfoHandlingBase.h>

class CInterfaceBase;

class CInterfaceManager : public CInterfaceManagerBase
{
public:
//Internal Structure Funktionen
    virtual BOOL I_InitAllInterfaces(CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_InitInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_InitBaudrateSelection(CStdString p_InterfaceName, CStdDWordArray& p_rulBaudrateSel, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_InitDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_UpdateInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_AddInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_DeleteInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = 0);

//Initialisation
    virtual HANDLE I_OpenInterface(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_CloseInterface(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_CloseAllInterfaces(CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_AreAllInterfacesClosed();

//Parameter
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo = 0);

//Hilfsfunktionen
    virtual BOOL I_GetKeyHandle(CStdString p_InterfaceName, CStdString p_PortName, HANDLE* p_pIKeyHandle, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_ResetInterface(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_GetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_SetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_IsInterfaceNameSupported(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = 0);

//Selection Funktionen
    virtual BOOL I_GetInterfaceNameSelection(CStdStringArray* p_pInterfaceNameSel, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_GetPortNameSelection(CStdString p_InterfaceName, CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_GetBaudrateSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL I_GetInterfaceModeSelection(CStdString p_InterfaceName, CStdStringArray* p_pInterfaceModeSel, CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL I_GetPortModeSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdStringArray* p_pPortModeSel, CErrorInfo* p_pErrorInfo = 0);

//Setting Funktionen
    virtual BOOL I_GetInterfaceSettings(HANDLE hI_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_SetInterfaceSettings(HANDLE hI_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = 0);

	virtual BOOL I_GetInterfaceMode(HANDLE hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_SetInterfaceMode(HANDLE hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo = 0);

	virtual BOOL I_GetPortMode(HANDLE hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_SetPortMode(HANDLE hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo = 0);

//Name Funktionen
    virtual BOOL I_GetInterfaceName(HANDLE hI_Handle, CStdString* pInterfaceName, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL I_GetPortName(HANDLE hI_Handle, CStdString* pPortName, CErrorInfo* p_pErrorInfo = 0);

//Funktionalitaet
    virtual BOOL ExecuteCommand(CCommandRoot *pCommand, HANDLE hHandle, HANDLE hTransactionHandle);
    virtual BOOL GetCommands(HANDLE p_hHandle, ELayer eLayer, CStdString* pCommandInfo);
    virtual BOOL AbortCommands(HANDLE p_hHandle, BOOL oActive);

//LayerManager
    virtual BOOL GetLayerManager(HANDLE p_hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle);

//Tracing
    virtual BOOL EnableTracing(HANDLE hI_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = 0);
    virtual BOOL DisableTracing(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo = 0);

//JournalManager
    virtual BOOL InitJournalManager(HANDLE p_hHandle, CJournalManagerBase* pJournalManager);
    virtual BOOL ResetJournalManager(HANDLE p_hHandle);
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

    CInterfaceManager();
	CInterfaceManager(int p_lInstanceValue);
    CInterfaceManager(const CInterfaceManager& rObject);
    virtual ~CInterfaceManager();
    virtual CInterfaceManagerBase* Clone();

private:
    BOOL DeleteInterface(CStdString p_InterfaceName, BOOL oCheckPrefixOnly);
    BOOL DeleteAllInterfaces(CStdString p_InterfaceName, BOOL oCheckPrefixOnly);
    BOOL FindInterface(CStdString p_InterfaceName, CInterfaceBase** ppInterface);
    BOOL FindInterfaceIndex(CStdString p_InterfaceName, short* piInterfaceIndex);
    BOOL FindInterfacePortIndex(CStdString p_InterfaceName, CStdString p_PortName, short* piPortIndex);
    BOOL FindPort(CStdString p_InterfaceName, CStdString p_PortName, CPortBase** ppPort);
    BOOL InitErrorHandling();
    BOOL GetNbOfAvailableBoards(CStdString p_Name,std::list<DWORD> & p_rNbOfAvailableBoards);
    void DeleteInterfaceList();

    //DeviceInfoHandling
    BOOL CreateDeviceInfoHandling();
    BOOL DeleteDeviceInfoHandling();
    BOOL InitDeviceInfoHandling(CInterfaceBase* pInterface);

	//Singleton RegistrationMap
	BOOL InitRegistrationMap();
	BOOL ReleaseRegistrationMap();

private:
    CHandleRegistrationMap_I* m_pHandleRegistrationMap_I;
    std::list<CInterfaceBase*> m_InterfaceList;
    std::map<CStdString,CDeviceInfoHandlingBase*> m_DeviceInfoHandlingMap;
};

#endif // !defined(AFX_INTERFACEMANAGER_H__0F52D262_5C84_4F7D_816F_3DE0F0AA6833__INCLUDED_)
