// CommunicationModel.cpp: Implementierung der Klasse CCommunicationModel.
//
//////////////////////////////////////////////////////////////////////
#include "CommunicationModel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommunicationModel::CCommunicationModel()
{
}

CCommunicationModel::~CCommunicationModel()
{
    DeleteVirtualCommandSetDialogManager();
    DeleteVirtualCommandSetManager();
}

BOOL CCommunicationModel::CreateVirtualCommandSetManager()
{
    BOOL oResult = FALSE;

    //Init Device Manager
    DeleteVirtualCommandSetManager();
    m_pVirtualCommandSetManager = new CVirtualCommandSet_Manager();
    if(m_pVirtualCommandSetManager != 0)
    {
		//Init Epos2 Device
		if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS2, PROTOCOL_MAXON_SERIAL_V2, INTERFACE_USB)) oResult = TRUE;
		if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS2, PROTOCOL_MAXON_SERIAL_V1, INTERFACE_RS232)) oResult = TRUE;
		
		#if !defined(_WIN64)		
			if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS2, PROTOCOL_CAN_OPEN, INTERFACE_IXXAT)) oResult = TRUE;
			if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS2, PROTOCOL_CAN_OPEN, INTERFACE_NI)) oResult = TRUE;
			if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS2, PROTOCOL_CAN_OPEN, INTERFACE_VECTOR)) oResult = TRUE;
		#endif

		//Init Epos Device
		if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS, PROTOCOL_MAXON_SERIAL_V1, INTERFACE_RS232)) oResult = TRUE;
		
		#if !defined(_WIN64)		
			if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS, PROTOCOL_CAN_OPEN, INTERFACE_IXXAT)) oResult = TRUE;
			if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS, PROTOCOL_CAN_OPEN, INTERFACE_NI)) oResult = TRUE;
			if(m_pVirtualCommandSetManager->VCS_InitVirtualDevice(VIRTUAL_DEVICE_DRIVE2, DEVICE_EPOS, PROTOCOL_CAN_OPEN, INTERFACE_VECTOR)) oResult = TRUE;
		#endif
    }

    //Init Device Dialog Manager
    if(oResult)
    {
        DeleteVirtualCommandSetDialogManager();
//        m_pVirtualCommandSetDialogManager = new CVirtualCommandSet_DialogManager();
    }

    return oResult;
}

