// ErrorHandling.h: Interface for the CErrorHandling class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORHANDLING_H__35065F20_2D36_4682_A442_3E76A0DD00E2__INCLUDED_)
#define AFX_ERRORHANDLING_H__35065F20_2D36_4682_A442_3E76A0DD00E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorInfo.h"
#include "Error.h"
#include "ErrorProducer.h"
#include "ErrorCode_Definitions.h"

class CErrorHandling
{
public:
	CErrorHandling();
	virtual ~CErrorHandling();

	static BOOL GetErrorDescription(DWORD p_ulErrorCode, CStdString* p_pDescription);
	void GetError(DWORD p_ulErrorCode, CErrorInfo* p_pErrorInfo);
	BOOL InitErrorProducer(CErrorProducer* p_pErrorProducer);
	BOOL GetErrorProducerInfos(ELayer& p_rLayer, CStdString& p_rClassName, CStdString& p_rCommandName);

private:
	void DeleteErrorProducer();

	//General
	static BOOL GetErrorDescription_GeneralError(DWORD p_ulErrorCode, CStdString *p_pDescription);

	//InterfaceLayer
	static BOOL GetErrorDescription_I(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_I_RS232(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_I_USB(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_I_HID(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_I_CAN(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_I_Socket(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_I_TwinCAT(DWORD p_ulErrorCode, CStdString *p_pDescription);

	//ProtocolStackLayer
	static BOOL GetErrorDescription_PS(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_PS_MaxonSerialV1(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_PS_InfoteamSerial(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_PS_CANopen(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_PS_MaxonSerialV2(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_PS_EtherCAT(DWORD p_ulErrorCode, CStdString *p_pDescription);

	//DeviceCommandSetLayer
	static BOOL GetErrorDescription_DCS(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_DCS_DeviceEpos(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_DCS_OldDeviceEpos(DWORD p_ulErrorCode, CStdString *p_pDescription);

	//VirtualDeviceLayer
	static BOOL GetErrorDescription_VCS(DWORD p_ulErrorCode, CStdString *p_pDescription);
	static BOOL GetErrorDescription_VCS_VirtualDevice(DWORD p_ulErrorCode, CStdString *p_pDescription);

private:
	CErrorProducer* m_pErrorProducer;
};

#endif // !defined(AFX_ERRORHANDLING_H__35065F20_2D36_4682_A442_3E76A0DD00E2__INCLUDED_)
