// Device_Esam.h: Schnittstelle f�r die Klasse CDevice_Esam.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Device_Esam_H__4B4C3F21_32FD_4D64_8505_A52922BC0605__INCLUDED_)
#define AFX_Device_Esam_H__4B4C3F21_32FD_4D64_8505_A52922BC0605__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>


#include <MmcTypeDefinition.h>
#include "../CommandGroups/Esam/CommandGroupStandard_DCS_Esam.h"
#include "BaseClasses/DeviceBase.h"

class CProtocolStackManagerBase;

class CDevice_Esam : public CDeviceBase
{
public:
	//CommandInvoke
	BOOL GetCommands(CStdString* pCommandInfo);

	BOOL InitDevice(CErrorInfo* pErrorInfo = NULL);
	BOOL InitDevice(CStdString strProtocolStack, CErrorInfo* pErrorInfo = NULL);
	BOOL InitDevice(CStdString strProtocolStack, CStdString strInterfaceName, CErrorInfo* pErrorInfo = NULL);
	BOOL InitGateway(CStdString strProtocolStackName);

	CDevice_Esam();
	CDevice_Esam(const CDevice_Esam& rObject);
	virtual ~CDevice_Esam();
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

	CCommandGroupStandard_DCS_Esam* m_pCommandGroupStandard;
};

#endif // !defined(AFX_Device_Esam_H__4B4C3F21_32FD_4D64_8505_A52922BC0605__INCLUDED_)
