// InterfaceManagerBase.h: Schnittstelle f�r die Klasse CInterfaceManagerBase.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACEMANAGERBASE_H__CC6EFF22_52C9_4A59_AB67_6B2B44E101FF__INCLUDED_)
#define AFX_INTERFACEMANAGERBASE_H__CC6EFF22_52C9_4A59_AB67_6B2B44E101FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/CommonLayer/LayerManagerBase.h>

class CJournalManagerBase;

class CInterfaceManagerBase  : public CLayerManagerBase
{
public:
//Internal Structure Funktionen
	virtual BOOL I_InitAllInterfaces(CErrorInfo* p_pErrorInfo = 0);
	virtual BOOL I_InitInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_InitBaudrateSelection(CStdString p_InterfaceName, CStdDWordArray& p_rulBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_InitDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_UpdateInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL I_AddInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL I_DeleteInterfacePort(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);

//Initialisation
	virtual HANDLE I_OpenInterface(CStdString p_InterfaceName, CStdString p_PortName, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_CloseInterface(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_CloseAllInterfaces(CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_AreAllInterfacesClosed();

//Parameter
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL SetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString p_Value, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL GetParameter(EParameterType p_eParameterType, HANDLE hI_Handle, CStdString p_Name, CStdString& p_rValue, CErrorInfo* p_pErrorInfo = NULL);

//Hilfsfunktionen
	virtual BOOL I_GetKeyHandle(CStdString p_InterfaceName, CStdString p_PortName, HANDLE* pIKeyHandle, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_ResetInterface(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_GetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL I_SetDefaultInterfaceSettings(CStdString p_InterfaceName, DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_IsInterfaceNameSupported(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo = NULL);

//Selection Funktionen
	virtual BOOL I_GetInterfaceNameSelection(CStdStringArray* pInterfaceNameSel, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_GetPortNameSelection(CStdString p_InterfaceName, CStdStringArray* pPortSel, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_GetBaudrateSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdDWordArray* pdBaudrateSel, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_GetInterfaceModeSelection(CStdString p_InterfaceName, CStdStringArray* pInterfaceModeSel, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_GetPortModeSelection(CStdString p_InterfaceName, CStdString p_PortName, CStdStringArray* pInterfaceModeSel, CErrorInfo* p_pErrorInfo = NULL);

//Setting Funktionen
	virtual BOOL I_GetInterfaceSettings(HANDLE hI_Handle, DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_SetInterfaceSettings(HANDLE hI_Handle, DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo = NULL);

	virtual BOOL I_GetInterfaceMode(HANDLE hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL I_SetInterfaceMode(HANDLE hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo = NULL);

	virtual BOOL I_GetPortMode(HANDLE hI_Handle, WORD* p_pusModeIndex, CErrorInfo* p_pErrorInfo = NULL);
    virtual BOOL I_SetPortMode(HANDLE hI_Handle, WORD p_usModeIndex, CErrorInfo* p_pErrorInfo = NULL);

//Name Funktionen
	virtual BOOL I_GetInterfaceName(HANDLE hI_Handle, CStdString* pInterfaceName, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL I_GetPortName(HANDLE hI_Handle, CStdString* pPortName, CErrorInfo* p_pErrorInfo = NULL);

//Funktionalit�t
	virtual BOOL ExecuteCommand(CCommandRoot *pCommand, HANDLE hHandle, HANDLE hTransactionHandle);
	virtual BOOL GetCommands(HANDLE p_hHandle, ELayer eLayer, CStdString* pCommandInfo);

//Tracing
	virtual BOOL EnableTracing(HANDLE hI_Handle, CStdString p_TracingFileName, CErrorInfo* p_pErrorInfo = NULL);
	virtual BOOL DisableTracing(HANDLE hI_Handle, CErrorInfo* p_pErrorInfo = NULL);

//LayerManager
	virtual BOOL GetLayerManager(HANDLE p_hHandle, ELayer eLayer, CLayerManagerBase** ppLayerManager, HANDLE* pLayerHandle);
	virtual BOOL GetLayerManager(CStdString strDeviceName, CStdString strProtocolStackName, ELayer eLayer, CLayerManagerBase** ppLayerManager);

	CInterfaceManagerBase();
	CInterfaceManagerBase(int p_lInstanceValue);
	CInterfaceManagerBase(const CInterfaceManagerBase& rObject);
	virtual ~CInterfaceManagerBase();
	virtual CInterfaceManagerBase* Clone();
};

#endif // !defined(AFX_INTERFACEMANAGERBASE_H__CC6EFF22_52C9_4A59_AB67_6B2B44E101FF__INCLUDED_)
