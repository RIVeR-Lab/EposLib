// Interface_USB.h: Schnittstelle f�r die Klasse CInterface_USB.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Interface_USB_H__27D9CCA8_43A0_4813_8EF9_7B2298F7567B__INCLUDED_)
#define AFX_Interface_USB_H__27D9CCA8_43A0_4813_8EF9_7B2298F7567B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseClasses/InterfaceBase.h"

class CFtd2xxDeviceInfoHandling;

class CInterface_USB : public CInterfaceBase
{
public:
	void Init() {m_strClassType="CInterface_USB";}
//Initialisation
    BOOL InitInterface(WORD wBoardNumber, WORD wNbBoardWithOldDriver);

    BOOL I_OpenInterface(CErrorInfo* pErrorInfo = NULL);
    BOOL I_OpenInterfacePort(CPortBase* pPort, CStdString strPortName, CErrorInfo* pErrorInfo = NULL);

    BOOL I_CloseInterface(CErrorInfo* pErrorInfo = NULL);
    BOOL I_CloseInterfacePort(CPortBase* pPort, CErrorInfo* pErrorInfo = NULL);

    BOOL I_GetInterfaceSettings(CPortBase* pPort, DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo = NULL);
    BOOL I_SetInterfaceSettings(CPortBase* pPort, DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo = NULL);

    BOOL I_GetInterfaceMode(WORD* pwModeIndex, CErrorInfo* pErrorInfo = NULL);
    BOOL I_SetInterfaceMode(WORD wModeIndex, CErrorInfo* pErrorInfo = NULL);

    BOOL I_ResetInterface(CPortBase* pPort, CErrorInfo* pErrorInfo = NULL);

    CInterface_USB();
    CInterface_USB(const CInterface_USB& rObject);
    virtual ~CInterface_USB();
    CInterfaceBase* Clone();

//JournalManager
    virtual void InitJournalManager(CJournalManagerBase *pJournalManager);
    virtual void ResetJournalManager();

//UsbDeviceInfoHandling
    BOOL InitInfoHandling(CDeviceInfoHandlingBase* pUsbDeviceInfoHandling);

private:
    BOOL InitErrorHandling();
    BOOL InitGateway();

	//ParameterSet
	BOOL InitParameterSet();

private:
    //UsbDeviceInfoHandling
    CFtd2xxDeviceInfoHandling* m_pUsbDeviceInfoHandling;
};

#endif // !defined(AFX_Interface_USB_H__27D9CCA8_43A0_4813_8EF9_7B2298F7567B__INCLUDED_)
