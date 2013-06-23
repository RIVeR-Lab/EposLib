#pragma once

#include <fstream>
#include <CommunicationModel/Interface/Classes/Ports/PortBase.h>
#include <Thread/MmcCriticalSection.h>
#include <Storage/MmcUserSettings.h>
#include "../../BaseClasses/DeviceInfoHandlingBase.h"

class CFtd2xxDeviceInfo;

typedef std::list<CFtd2xxDeviceInfo*> tDeviceInfoList;

class CFtd2xxDeviceInfoHandling : public CDeviceInfoHandlingBase
{
public:
	CFtd2xxDeviceInfoHandling(void);
	CFtd2xxDeviceInfoHandling(int p_lInstanceValue);
	~CFtd2xxDeviceInfoHandling(void);
	CFtd2xxDeviceInfoHandling& operator=(CFtd2xxDeviceInfoHandling& other);
	BOOL CopyDeviceInfos(CFtd2xxDeviceInfoHandling* pOther);

	//Singleton
	static CMmcCriticalSection* GetSyncInstance();
	
	//DeviceInfos
	BOOL InitDeviceInfos(tDeviceInfoList& p_rDeviceInfoList);
	BOOL UpdateDeviceInfos(tDeviceInfoList& p_rDeviceInfoList,tPortList& p_rOpenPortList);
	BOOL UpdatePortOpened(CStdString p_PortName, BOOL p_oOpened);
	BOOL GetPortNames(CStdStringArray& p_rPortNames);
	BOOL GetLocationId(CStdString p_PortName, DWORD& p_rulLocationId);
	BOOL GetSerialNumber(CStdString p_PortName, CStdString& p_SerialNumber);
	BOOL UpdateLocationId(tDeviceInfoList& p_rDeviceInfoList,CStdString p_PortName);

	//Persistence
	BOOL DeleteFromRegistry();
	BOOL WriteToRegistry();
	BOOL ReadFromRegistry();
	
	//Logging
	BOOL WriteToLogFile(CStdString p_Action, tDeviceInfoList& p_rDeviceInfoList);
	BOOL WriteToLogFile(CStdString p_Action, CStdString portName, DWORD ulLocId);
	BOOL WriteToLogFile(CStdString p_Action, CStdString portName);
	BOOL WriteToLogFile(CStdString p_Action);

private:
	//Initialisation
	BOOL CreateRegistrySemaphore();
	BOOL DeleteRegistrySemaphore();

	//Update Devices
	BOOL UpdateRemovedDevices(tDeviceInfoList& p_rFtdiDrvDeviceInfoList, tPortList& p_rOpenPortList);
	BOOL UpdateAddedDevices(tDeviceInfoList& p_rFtdiDrvDeviceInfoList, tPortList& p_rOpenPortList);
	BOOL UpdateNewAddedDevices(tDeviceInfoList& p_rFtdiDrvDeviceInfoList, tPortList& p_rOpenPortList);
	
	//DeviceInfos
	BOOL AddDeviceInfo(CStdString p_PortName,DWORD p_ulLocId,CStdString p_SerialNumber);
	BOOL DeleteFromDeviceInfoList(CFtd2xxDeviceInfo& p_rDeviceInfo);

	//Port
	BOOL IsPortNameUsed(CStdString p_PortName,tDeviceInfoList& p_rDeviceInfoList);
	BOOL ReopenPort(CStdString p_PortName,tPortList& p_rOpenPortList);
	BOOL GetNextFreePortName(CStdString& p_rPortName);
	
	//List Handling
	BOOL IsPortExistingInList(CFtd2xxDeviceInfo& p_rDeviceInfo,tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpExistingDeviceInfo);
	BOOL AddToDeviceInfoList(CFtd2xxDeviceInfo& p_rDeviceInfo);
	BOOL DeleteDeviceInfoList(tDeviceInfoList& p_rDeviceInfoList);
	BOOL CopyDeviceInfoList(tDeviceInfoList& p_rSourceDeviceInfoList,tDeviceInfoList& p_rTargetDeviceInfoList);
	BOOL SortPortNames(CStdStringArray& p_rPortNames);
	BOOL SortDeviceInfoListByPortName(tDeviceInfoList& p_rDeviceInfoList);
	BOOL SortDeviceInfoListByLocId(tDeviceInfoList& p_rDeviceInfoList);
	BOOL FindDeviceInfo(CFtd2xxDeviceInfo& p_rDeviceInfo,tDeviceInfoList& p_rDeviceInfoList,int& p_rIndexFound);
	BOOL FindDeviceInfoByLocId(CFtd2xxDeviceInfo& p_rDeviceInfo,tDeviceInfoList& p_rDeviceInfoList,int& p_rIndexFound);
	BOOL FindDeviceInfoByLocId(CFtd2xxDeviceInfo& p_rDeviceInfo,tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpFoundDeviceInfo);
	BOOL FindDeviceInfoBySerialNumber(CFtd2xxDeviceInfo& p_rDeviceInfo,tDeviceInfoList& p_rDeviceInfoList,int& p_rIndexFound);
	BOOL FindDeviceInfoBySerialNumber(CFtd2xxDeviceInfo& p_rDeviceInfo,tDeviceInfoList& p_rDeviceInfoList,CFtd2xxDeviceInfo*& p_rpFoundDeviceInfo);
	BOOL FindDeviceInfoByPortName(CStdString p_PortName,tDeviceInfoList& p_rDeviceInfoList,CFtd2xxDeviceInfo*& p_rpDeviceInfo);
	BOOL FindNonPluggedInDeviceInfoInList(tDeviceInfoList& p_rDeviceInfoList, CFtd2xxDeviceInfo*& p_rpNonPluggedInDeviceInfo);
	BOOL ResetDeviceInfoList();
	BOOL GetModuleVersion(CStdString& p_rVersion);
	BOOL GetApplicationInfo(CStdString& p_rApplicationName, CStdString& p_rVersion);
	CStdString ExtractApplicationName(CStdString p_FileName);

	//Registry
	BOOL GetRegistryKey(CStdString& p_rRegistryKey);	
	BOOL ReadDeviceInfoFromRegistry(CMmcUserSettings *pUserSettings, CFtd2xxDeviceInfo* p_pDeviceInfo);
	BOOL WriteDeviceInfoToRegistry(CMmcUserSettings *pUserSettings, CStdString p_ValueName, CFtd2xxDeviceInfo* p_pDeviceInfo);
	BOOL WriteOpenedToRegistry(CMmcUserSettings *pUserSettings, BOOL p_oOpened);
	BOOL UpdateRegistry(CStdString p_PortName, BOOL p_oOpened);

private:
	//Singleton
	CMmcSemaphore* m_pRegistrySemaphore;

	static CMmcCriticalSection m_CriticalSection;
	int m_lInstanceValue;
		
	//DeviceInfo
	tDeviceInfoList m_DeviceInfoList;
};

