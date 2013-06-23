// Device_Esam2.h: Schnittstelle f�r die Klasse CDevice_Esam2.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Device_Esam2_H__4B4C3F21_32FD_4D64_8505_A52922BC0605__INCLUDED_)
#define AFX_Device_Esam2_H__4B4C3F21_32FD_4D64_8505_A52922BC0605__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <MmcTypeDefinition.h>
#include "../CommandGroups/Esam2/CommandGroupStandard_DCS_Esam2.h"
#include "BaseClasses/DeviceBase.h"

class CProtocolStackManagerBase;

class CDevice_Esam2 : public CDeviceBase
{
public:
	//CommandInvoke
	BOOL GetCommands(CStdString* pCommandInfo);

	BOOL InitDevice(CErrorInfo* pErrorInfo = NULL);
	BOOL InitDevice(CStdString strProtocolStack, CErrorInfo* pErrorInfo = NULL);
	BOOL InitDevice(CStdString strProtocolStack, CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);
	BOOL InitGateway(CStdString strProtocolStackName);

	CDevice_Esam2();
	CDevice_Esam2(const CDevice_Esam2& rObject);
	virtual ~CDevice_Esam2();
	virtual CDeviceBase* Clone();

//JournalManager
	virtual void InitJournalManager(CJournalManagerBase* pJournalManager);
	virtual void ResetJournalManager();

private:
	BOOL VerifyGateway(CStdString strProtocolStackName);
	BOOL InitErrorHandling();

	void DeleteGroupList();
	void FillGroupList();
	BOOL InitGroupList(CGatewayDCStoPS* pGateway);

	//ParameterSet
	BOOL InitParameterSet();
	//Default Settings
	BOOL InitDefaultProtocolStackSettings(CProtocolStackManagerBase* p_pProtocolStackManager, CStdString p_ProtocolStackName, CStdString p_InterfaceName);

private:
	CCommandGroupStandard_DCS_Esam2* m_pCommandGroupStandard;
};

#endif // !defined(AFX_Device_Esam2_H__4B4C3F21_32FD_4D64_8505_A52922BC0605__INCLUDED_)
