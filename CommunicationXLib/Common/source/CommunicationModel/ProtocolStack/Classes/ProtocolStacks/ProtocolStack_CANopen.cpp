// ProtocolStack_CANopen.cpp: Implementierung der Klasse CProtocolStack_CANopen.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <string.h>
#include <stdio.h>
#include <algorithm>

#include "../GatewayToInterface/BaseClasses/GatewayPStoI.h"
#include "ProtocolStackCANopenToIFactory.h"

#include "../GatewayToDevice/BaseClasses/GatewayPStoDCS.h"
#include "ProtocolStackCANopenToDFactory.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/BaseClasses/Command_PS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/ProtocolStack/Command_PS_CANopen.h>

#include "InterfacePlugInManager/InterfacePlugInManager.h"
#include <CommunicationModel/Interface/InterfaceManager.h>
#include "../../../DeviceCommandSet/DeviceCommandSetManager.h"

#include "ProtocolStack_CANopen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CProtocolStack_CANopen::CProtocolStack_CANopen()
{
    m_strProtocolStackName = PROTOCOL_CAN_OPEN;

    m_pCommand_InitiateSDODownload = 0;
    m_pCommand_DownloadSDOSegment = 0;
    m_pCommand_InitiateSDOUpload = 0;
    m_pCommand_UploadSDOSegment = 0;
    m_pCommand_SendNMTService = 0;
    m_pCommand_SendCANFrame = 0;
    m_pCommand_RequestCANFrame = 0;
    m_pCommand_AbortSDOTransfer = 0;
	m_pCommand_NetworkIndication = 0;
    m_pCommand_SendLSSFrame = 0;
    m_pCommand_ReadLSSFrame = 0;

    m_pJournalManager = 0;
    InitCommands();
    InitErrorHandling();
}

CProtocolStack_CANopen::CProtocolStack_CANopen(const CProtocolStack_CANopen& rObject):CProtocolStackBase(rObject)
{
    m_strProtocolStackName = rObject.m_strProtocolStackName;

    m_pCommand_InitiateSDODownload = 0;
    m_pCommand_DownloadSDOSegment = 0;
    m_pCommand_InitiateSDOUpload = 0;
    m_pCommand_UploadSDOSegment = 0;
    m_pCommand_SendNMTService = 0;
    m_pCommand_SendCANFrame = 0;
    m_pCommand_RequestCANFrame = 0;
    m_pCommand_AbortSDOTransfer = 0;
	m_pCommand_NetworkIndication = 0;
    m_pCommand_SendLSSFrame = 0;
    m_pCommand_ReadLSSFrame = 0;

    InitCommands();
    InitJournalManager(rObject.m_pJournalManager);
    if(m_pGatewayToInterface) InitCommands((CGateway*)m_pGatewayToInterface); else InitCommands((CGateway*)m_pGatewayToDevice);
    InitErrorHandling();
}

CProtocolStackBase* CProtocolStack_CANopen::Clone()
{
    CProtocolStack_CANopen* pProtocolStack;

    pProtocolStack = new CProtocolStack_CANopen(*this);
    return pProtocolStack;
}

CProtocolStack_CANopen::~CProtocolStack_CANopen()
{
    DeleteCommands();
}

BOOL CProtocolStack_CANopen::CreatePlugInManager()
{
    const DWORD NOTIFICATION_DELAY = 100;

    BOOL oResult = FALSE;

    if(CProtocolStackBase::CreatePlugInManager() && m_pPlugInManager)
    {
        oResult = m_pPlugInManager->SetNotificationDelay(NOTIFICATION_DELAY);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::InitGatewayToInterface(CStdString strInterfaceName)
{
    CStdString strSubInterfaceName = _T("");
	BOOL oResult = FALSE;

    //Reset
	DeleteGatewayToInterface();

	//Create Gateway
	if(!m_pGatewayToInterface)
	{
     	oResult = CProtocolStackCANopenToIFactory::CreateInstance(strInterfaceName, m_pGatewayToInterface);
	}
    
	//InitGateway
	if(m_pGatewayToInterface)
	{
		if(m_pGatewayToInterface->InitGateway())
        {
            InitParameterSet();
			InitCommands(m_pGatewayToInterface);
            oResult = TRUE;
        }
        else
        {
            DeleteGatewayToInterface();
        }
	}

	return oResult;
}

BOOL CProtocolStack_CANopen::InitGatewayToDevice(CStdString strDeviceName)
{
	BOOL oResult = FALSE;

    //Reset
	DeleteGatewayToDevice();

    if(oResult) oResult = CProtocolStackCANopenToDFactory::CreateInstance(strDeviceName, m_pGatewayToDevice);

	//InitGateway
	if(m_pGatewayToDevice)
	{
		if(m_pGatewayToDevice->InitGateway())
        {
            InitParameterSet();
			InitCommands((CGateway*)m_pGatewayToDevice);
            oResult = TRUE;
        }
        else
        {
            DeleteGatewayToDevice();
        }
	}

	return oResult;
}

BOOL CProtocolStack_CANopen::InitParameterSet()
{
	CGateway* pGateway = 0;
	BOOL oResult = TRUE;

	if(m_pGatewayToInterface)
	{
		pGateway = (CGateway*)m_pGatewayToInterface;
	}
	else if(m_pGatewayToDevice)
	{
		pGateway = (CGateway*)m_pGatewayToDevice;
	}

	if(pGateway)
	{
		if(oResult) oResult &= pGateway->ResetParameterSet();
	}

    return oResult;
}

BOOL CProtocolStack_CANopen::InitProtocolStack(CErrorInfo* pErrorInfo)
{
    BOOL oResult = TRUE;

    std::list<CStdString> rTargets;
    
    oResult = CProtocolStackCANopenToIFactory::GetAllIdentifiers(rTargets);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=rTargets.begin(); it!=rTargets.end() && oResult; it++)
        {
            if(!InitProtocolStack(*it))
            {
                oResult = FALSE;
            }
        }
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::InitProtocolStack(CStdString strInterfaceName, CErrorInfo* pErrorInfo)
{
    CInterfaceManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strInterfaceName.size() == 0)
    {
        //Don't create an interface manager
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(strInterfaceName, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CInterfaceManagerBase*) new CInterfaceManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

        //Klassenstruktur für Interface aufbauen
        if(VerifyGatewayToInterface(strInterfaceName))
        {
            if(pManager->I_InitInterface(strInterfaceName, pErrorInfo))
            {
                //Interface already existing
                oResult = TRUE;
            }
            else
            {
                //Interface not existing
                oResult = TRUE;
            }

            //Init PlugIn Manager
            if(m_pPlugInManager) m_pPlugInManager->RegisterInterface(strInterfaceName);

            if(oNewCreated) m_InterfaceManagerList.push_back(pManager);
        }
        else
        {
            if(oNewCreated) delete pManager;
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::InitProtocolStack(CStdString strDeviceName, CStdString strProtocolStackName, CErrorInfo* pErrorInfo)
{
	CDeviceCommandSetManagerBase* pManager = 0;
    BOOL oNewCreated = FALSE;
    BOOL oResult = FALSE;

    if(strDeviceName.size() == 0)
    {
        //Don't create an device manager
        oResult = TRUE;
    }
    else
    {
        //Is Manager already existing
        if(!FindCorrectManager(strDeviceName, &pManager) && !GetFirstManager(&pManager))
        {
            //Create new Manager
            pManager = (CDeviceCommandSetManagerBase*) new CDeviceCommandSetManager(m_lInstanceValue);
            oNewCreated = TRUE;
        }

		//Klassenstruktur für Device aufbauen
        if((VerifyGatewayToDevice(strDeviceName)) && (pManager->DCS_InitDevice(strDeviceName, strProtocolStackName, pErrorInfo)))
        {
            if(oNewCreated) m_DeviceCommandSetManagerList.push_back(pManager);
            oResult = TRUE;
        }
        else
        {
            if(oNewCreated) delete pManager;
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "ProtocolStack_CANopen";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CProtocolStack_CANopen::VerifyGatewayToInterface(CStdString strInterfaceName)
{
    std::list<CStdString> rTargets;    
    BOOL oResult = CProtocolStackCANopenToIFactory::GetAllIdentifiers(rTargets);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=rTargets.begin(); it!=rTargets.end() && oResult; it++)
        {
            if(strInterfaceName.CompareNoCase((*it)) == 0)
            {
                oResult = TRUE;
            }
        }
    }

    return FALSE;
}

BOOL CProtocolStack_CANopen::VerifyGatewayToDevice(CStdString strDeviceName)
{
    std::list<CStdString> rTargets;    
    BOOL oResult = CProtocolStackCANopenToDFactory::GetAllIdentifiers(rTargets);

    if(oResult)
    {
        for(std::list<CStdString>::iterator it=rTargets.begin(); it!=rTargets.end() && oResult; it++)
        {
            if(strDeviceName.CompareNoCase((*it)) == 0)
            {
                oResult = TRUE;
            }
        }
    }

    return FALSE;
}

void CProtocolStack_CANopen::InitCommands()
{
    DeleteCommands();

    //Init InitiateSDODownload
    m_pCommand_InitiateSDODownload = new CCommand_PS_CANopen();
    m_pCommand_InitiateSDODownload->InitCommand(CANOPEN_INITIATE_SDO_DOWNLOAD);

    //Init DownloadSDOSegment
    m_pCommand_DownloadSDOSegment = new CCommand_PS_CANopen();
    m_pCommand_DownloadSDOSegment->InitCommand(CANOPEN_DOWNLOAD_SDO_SEGMENT);

    //Init InitiateSDOUpload
    m_pCommand_InitiateSDOUpload = new CCommand_PS_CANopen();
    m_pCommand_InitiateSDOUpload->InitCommand(CANOPEN_INITIATE_SDO_UPLOAD);

    //Init UploadSDOSegment
    m_pCommand_UploadSDOSegment = new CCommand_PS_CANopen();
    m_pCommand_UploadSDOSegment->InitCommand(CANOPEN_UPLOAD_SDO_SEGMENT);

    //Init AbortSDOTransfer
    m_pCommand_AbortSDOTransfer = new CCommand_PS_CANopen();
    m_pCommand_AbortSDOTransfer->InitCommand(CANOPEN_ABORT_SDO_TRANSFER);

	//Init NetworkIndication
    m_pCommand_NetworkIndication = new CCommand_PS_CANopen();
    m_pCommand_NetworkIndication->InitCommand(CANOPEN_NETWORK_INDICATION);

    //Init SendNMTService
    m_pCommand_SendNMTService = new CCommand_PS_CANopen();
    m_pCommand_SendNMTService->InitCommand(CANOPEN_SEND_NMT_SERVICE);

    //Init SendCANFrame
    m_pCommand_SendCANFrame = new CCommand_PS_CANopen();
    m_pCommand_SendCANFrame->InitCommand(CANOPEN_SEND_CAN_FRAME);

    //Init RequestCANFrame
    m_pCommand_RequestCANFrame = new CCommand_PS_CANopen();
    m_pCommand_RequestCANFrame->InitCommand(CANOPEN_REQUEST_CAN_FRAME);

    //Init SendLSSFrame
    m_pCommand_SendLSSFrame = new CCommand_PS_CANopen();
    m_pCommand_SendLSSFrame->InitCommand(CANOPEN_SEND_LSS_FRAME);

    //Init ReadLSSFrame
    m_pCommand_ReadLSSFrame = new CCommand_PS_CANopen();
    m_pCommand_ReadLSSFrame->InitCommand(CANOPEN_READ_LSS_FRAME);
}

void CProtocolStack_CANopen::DeleteCommands()
{
    if(m_pCommand_InitiateSDODownload)
    {
        delete m_pCommand_InitiateSDODownload;
        m_pCommand_InitiateSDODownload = 0;
    }

    if(m_pCommand_DownloadSDOSegment)
    {
        delete m_pCommand_DownloadSDOSegment;
        m_pCommand_DownloadSDOSegment = 0;
    }

    if(m_pCommand_InitiateSDOUpload)
    {
        delete m_pCommand_InitiateSDOUpload;
        m_pCommand_InitiateSDOUpload = 0;
    }

    if(m_pCommand_UploadSDOSegment)
    {
        delete m_pCommand_UploadSDOSegment;
        m_pCommand_UploadSDOSegment = 0;
    }

    if(m_pCommand_AbortSDOTransfer)
    {
        delete m_pCommand_AbortSDOTransfer;
        m_pCommand_AbortSDOTransfer = 0;
    }

	if(m_pCommand_NetworkIndication)
    {
        delete m_pCommand_NetworkIndication;
        m_pCommand_NetworkIndication = 0;
    }

    if(m_pCommand_SendNMTService)
    {
        delete m_pCommand_SendNMTService;
        m_pCommand_SendNMTService = 0;
    }

    if(m_pCommand_SendCANFrame)
    {
        delete m_pCommand_SendCANFrame;
        m_pCommand_SendCANFrame = 0;
    }

    if(m_pCommand_RequestCANFrame)
    {
        delete m_pCommand_RequestCANFrame;
        m_pCommand_RequestCANFrame = 0;
    }

    if(m_pCommand_SendLSSFrame)
    {
        delete m_pCommand_SendLSSFrame;
        m_pCommand_SendLSSFrame = 0;
    }

    if(m_pCommand_ReadLSSFrame)
    {
        delete m_pCommand_ReadLSSFrame;
        m_pCommand_ReadLSSFrame = 0;
    }
}

void CProtocolStack_CANopen::InitJournalManager(CJournalManagerBase *pJournalManager)
{
    CProtocolStackBase::InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSDODownload) m_pCommand_InitiateSDODownload->InitJournalManager(pJournalManager);
    if(m_pCommand_DownloadSDOSegment) m_pCommand_DownloadSDOSegment->InitJournalManager(pJournalManager);
    if(m_pCommand_InitiateSDOUpload) m_pCommand_InitiateSDOUpload->InitJournalManager(pJournalManager);
    if(m_pCommand_UploadSDOSegment) m_pCommand_UploadSDOSegment->InitJournalManager(pJournalManager);
    if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->InitJournalManager(pJournalManager);
    if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->InitJournalManager(pJournalManager);
    if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->InitJournalManager(pJournalManager);
    if(m_pCommand_AbortSDOTransfer) m_pCommand_AbortSDOTransfer->InitJournalManager(pJournalManager);
	if(m_pCommand_NetworkIndication) m_pCommand_NetworkIndication->InitJournalManager(pJournalManager);
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->InitJournalManager(pJournalManager);
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->InitJournalManager(pJournalManager);
}

void CProtocolStack_CANopen::ResetJournalManager()
{
    CProtocolStackBase::ResetJournalManager();
    if(m_pCommand_InitiateSDODownload) m_pCommand_InitiateSDODownload->ResetJournalManager();
    if(m_pCommand_DownloadSDOSegment) m_pCommand_DownloadSDOSegment->ResetJournalManager();
    if(m_pCommand_InitiateSDOUpload) m_pCommand_InitiateSDOUpload->ResetJournalManager();
    if(m_pCommand_UploadSDOSegment) m_pCommand_UploadSDOSegment->ResetJournalManager();
    if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->ResetJournalManager();
    if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->ResetJournalManager();
    if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->ResetJournalManager();
    if(m_pCommand_AbortSDOTransfer) m_pCommand_AbortSDOTransfer->ResetJournalManager();
	if(m_pCommand_NetworkIndication) m_pCommand_NetworkIndication->ResetJournalManager();
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->ResetJournalManager();
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->ResetJournalManager();
}

void CProtocolStack_CANopen::InitCommands(CGateway* pGateway)
{
    if(m_pCommand_InitiateSDODownload) m_pCommand_InitiateSDODownload->InitGateway(pGateway);
    if(m_pCommand_DownloadSDOSegment) m_pCommand_DownloadSDOSegment->InitGateway(pGateway);
    if(m_pCommand_InitiateSDOUpload) m_pCommand_InitiateSDOUpload->InitGateway(pGateway);
    if(m_pCommand_UploadSDOSegment) m_pCommand_UploadSDOSegment->InitGateway(pGateway);
    if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->InitGateway(pGateway);
    if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->InitGateway(pGateway);
    if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->InitGateway(pGateway);
    if(m_pCommand_AbortSDOTransfer) m_pCommand_AbortSDOTransfer->InitGateway(pGateway);
	if(m_pCommand_NetworkIndication) m_pCommand_NetworkIndication->InitGateway(pGateway);
    if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->InitGateway(pGateway);
    if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->InitGateway(pGateway);
}

BOOL CProtocolStack_CANopen::GetCommands(CStdString* pCommandInfo)
{
    CXXMLFile xmlFile;
    CXXMLFile::CElementPart* pElementPart;
    CXXMLFile::CElement* pElement;
    BOOL oResult = TRUE;

    if(pCommandInfo)
    {
        //Root
        pElementPart = xmlFile.Root();
        if(!xmlFile.IsElement(pElementPart)) return FALSE;

        //CommandInfo Elements
        pElement = (CXXMLFile::CElement*)xmlFile.AddElement(pElementPart);
        xmlFile.SetText(pElement, "CommandInfo");

        //CommandGroup Elements
        pElement = (CXXMLFile::CElement*)xmlFile.AddElement(pElement);
        xmlFile.SetText(pElement, "CommandGroup");
        pElement->SetAt("Name", m_strProtocolStackName);

        //Command Elements
        if(m_pCommand_InitiateSDOUpload && !m_pCommand_InitiateSDOUpload->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_UploadSDOSegment && !m_pCommand_UploadSDOSegment->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_InitiateSDODownload && !m_pCommand_InitiateSDODownload->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_DownloadSDOSegment && !m_pCommand_DownloadSDOSegment->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_AbortSDOTransfer && !m_pCommand_AbortSDOTransfer->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
		if(m_pCommand_NetworkIndication && !m_pCommand_NetworkIndication->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_SendNMTService && !m_pCommand_SendNMTService->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_SendCANFrame && !m_pCommand_SendCANFrame->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_RequestCANFrame && !m_pCommand_RequestCANFrame->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_SendLSSFrame && !m_pCommand_SendLSSFrame->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;
        if(m_pCommand_ReadLSSFrame && !m_pCommand_ReadLSSFrame->StoreToXMLFile(&xmlFile, pElement, TRUE)) return FALSE;

        //Write to string
        xmlFile.WriteToString(pCommandInfo);
        return oResult;
    }

    return FALSE;
}

BOOL CProtocolStack_CANopen::PS_InitiateSDODownload(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oExpeditedTransfer, BOOL oSizeIndicated, BYTE uNonValidNbOfBytes, WORD wIndex, BYTE uSubIndex, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 4;

    BOOL oResult = FALSE;
    BYTE uRetSubIndex;
    DWORD dRetCobId;
    WORD wRetIndex;

    if(m_pCommand_InitiateSDODownload)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_InitiateSDODownload->ResetStatus();
        m_pCommand_InitiateSDODownload->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_InitiateSDODownload->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_InitiateSDODownload->SetParameterData(2, &oExpeditedTransfer, sizeof(oExpeditedTransfer));
        m_pCommand_InitiateSDODownload->SetParameterData(3, &oSizeIndicated, sizeof(oSizeIndicated));
        m_pCommand_InitiateSDODownload->SetParameterData(4, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
        m_pCommand_InitiateSDODownload->SetParameterData(5, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSDODownload->SetParameterData(6, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSDODownload->SetParameterData(7, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = m_pCommand_InitiateSDODownload->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSDODownload->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(1, &wRetIndex, sizeof(wRetIndex));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(2, &uRetSubIndex, sizeof(uRetSubIndex));
        m_pCommand_InitiateSDODownload->GetReturnParameterData(3, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_InitiateSDODownload->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_DownloadSDOSegment(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BYTE uNonValidNbOfBytes, BOOL oNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, BOOL* poRetToggle, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 7;

    BOOL oResult = FALSE;
    DWORD dRetCobId;

    if(m_pCommand_DownloadSDOSegment)
    {
        //Limit DataLength
        if(dSegDataBufferLength > MAX_CAN_DATA) dSegDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_DownloadSDOSegment->ResetStatus();
        m_pCommand_DownloadSDOSegment->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_DownloadSDOSegment->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_DownloadSDOSegment->SetParameterData(2, &oToggle, sizeof(oToggle));
        m_pCommand_DownloadSDOSegment->SetParameterData(3, &uNonValidNbOfBytes, sizeof(uNonValidNbOfBytes));
        m_pCommand_DownloadSDOSegment->SetParameterData(4, &oNoMoreSegments, sizeof(oNoMoreSegments));
        m_pCommand_DownloadSDOSegment->SetParameterData(5, pSegDataBuffer, dSegDataBufferLength);

        //Execute Command
        oResult = m_pCommand_DownloadSDOSegment->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(1, poRetToggle, sizeof(*poRetToggle));
        m_pCommand_DownloadSDOSegment->GetReturnParameterData(2, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_DownloadSDOSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_InitiateSDOUpload(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, WORD wIndex, BYTE uSubIndex, BOOL* poExpeditedTransfer, BOOL* poSizeIndicated, BYTE* puNonValidNbOfBytes, void* pDataBuffer, DWORD dDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 4;

    DWORD dRetCobId;
    WORD wRetIndex;
    BYTE uRetSubIndex;

    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSDOUpload)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_InitiateSDOUpload->ResetStatus();
        m_pCommand_InitiateSDOUpload->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_InitiateSDOUpload->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_InitiateSDOUpload->SetParameterData(2, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSDOUpload->SetParameterData(3, &uSubIndex, sizeof(uSubIndex));

        //Execute Command
        oResult = m_pCommand_InitiateSDOUpload->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(1, poExpeditedTransfer, sizeof(*poExpeditedTransfer));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(2, poSizeIndicated, sizeof(*poSizeIndicated));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(3, puNonValidNbOfBytes, sizeof(*puNonValidNbOfBytes));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(4, &wRetIndex, sizeof(wRetIndex));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(5, &uRetSubIndex, sizeof(uRetSubIndex));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(6, pDataBuffer, dDataBufferLength);
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(7, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_InitiateSDOUpload->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_UploadSDOSegment(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, BOOL oToggle, BOOL* poRetToggle, BYTE* puNonValidNbOfBytes, BOOL* poNoMoreSegments, void* pSegDataBuffer, DWORD dSegDataBufferLength, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 7;

    DWORD dRetCobId;

    BOOL oResult = FALSE;

    if(m_pCommand_UploadSDOSegment)
    {
        //Limit DataLength
        if(dSegDataBufferLength > MAX_CAN_DATA) dSegDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_UploadSDOSegment->ResetStatus();
        m_pCommand_UploadSDOSegment->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_UploadSDOSegment->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_UploadSDOSegment->SetParameterData(2, &oToggle, sizeof(oToggle));

        //Execute Command
        oResult = m_pCommand_UploadSDOSegment->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_UploadSDOSegment->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(1, poRetToggle, sizeof(*poRetToggle));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(2, puNonValidNbOfBytes, sizeof(*puNonValidNbOfBytes));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(3, poNoMoreSegments, sizeof(*poNoMoreSegments));
        m_pCommand_UploadSDOSegment->GetReturnParameterData(4, pSegDataBuffer, dSegDataBufferLength);
        m_pCommand_UploadSDOSegment->GetReturnParameterData(5, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_UploadSDOSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_AbortSDOTransfer(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, WORD wIndex, BYTE uSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_AbortSDOTransfer)
    {
        //Set Parameter Data
        m_pCommand_AbortSDOTransfer->ResetStatus();
        m_pCommand_AbortSDOTransfer->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_AbortSDOTransfer->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_AbortSDOTransfer->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_AbortSDOTransfer->SetParameterData(3, &dAbortCode, sizeof(dAbortCode));

        //Execute Command
        oResult = m_pCommand_AbortSDOTransfer->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSDOTransfer->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_NetworkIndication(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobIdClientServer, DWORD dCobIdServerClient, WORD wTargetNetworkId, BYTE uTargetNodeId, DWORD* pdAbortCode, CErrorInfo* pErrorInfo)
{
    DWORD dRetCobId = 0;
    WORD wRetTargetNetworkId = 0;
    BYTE uRetTargetNodeId = 0;

    BOOL oResult = FALSE;

    if(m_pCommand_NetworkIndication)
    {
        //Set Parameter Data
        m_pCommand_NetworkIndication->ResetStatus();
        m_pCommand_NetworkIndication->SetParameterData(0, &dCobIdClientServer, sizeof(dCobIdClientServer));
        m_pCommand_NetworkIndication->SetParameterData(1, &dCobIdServerClient, sizeof(dCobIdServerClient));
        m_pCommand_NetworkIndication->SetParameterData(2, &wTargetNetworkId, sizeof(wTargetNetworkId));
        m_pCommand_NetworkIndication->SetParameterData(3, &uTargetNodeId, sizeof(uTargetNodeId));

        //Execute Command
        oResult = m_pCommand_NetworkIndication->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(0, &dRetCobId, sizeof(dRetCobId));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(1, &wRetTargetNetworkId, sizeof(wRetTargetNetworkId));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(2, &uRetTargetNodeId, sizeof(uRetTargetNodeId));
        m_pCommand_InitiateSDOUpload->GetReturnParameterData(3, pdAbortCode, sizeof(*pdAbortCode));

        //Get ErrorCode
        m_pCommand_NetworkIndication->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_SendNMTService(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, BYTE uCommandSpecifier, BYTE uNodeId, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SendNMTService)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &uCommandSpecifier, sizeof(uCommandSpecifier));
        m_pCommand_SendNMTService->SetParameterData(1, &uNodeId, sizeof(uNodeId));

        //Execute Command
        oResult = m_pCommand_SendNMTService->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_SendCANFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_SendCANFrame)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &dCobId, sizeof(dCobId));
        m_pCommand_SendCANFrame->SetParameterData(1, &uLength, sizeof(uLength));
        m_pCommand_SendCANFrame->SetParameterData(2, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = m_pCommand_SendCANFrame->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_RequestCANFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, DWORD dCobId, BYTE uLength, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_RequestCANFrame)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &dCobId, sizeof(dCobId));
        m_pCommand_RequestCANFrame->SetParameterData(1, &uLength, sizeof(uLength));

        //Execute Command
        oResult = m_pCommand_RequestCANFrame->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_SendLSSFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_SendLSSFrame)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = m_pCommand_SendLSSFrame->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CProtocolStack_CANopen::PS_ReadLSSFrame(CLayerManagerBase* pManager, HANDLE hI_Handle, HANDLE hTransactionHandle, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    const DWORD MAX_CAN_DATA = 8;

    BOOL oResult = FALSE;

    if(m_pCommand_ReadLSSFrame)
    {
        //Limit DataLength
        if(dDataBufferLength > MAX_CAN_DATA) dDataBufferLength = MAX_CAN_DATA;

        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = m_pCommand_ReadLSSFrame->Execute(pManager, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}
