// GatewayMaxonSerialV2ToRS232.cpp: Implementierung der Klasse CGatewayMaxonSerialV2ToRS232.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayMaxonSerialV2ToRS232.h"


#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_RS232.h>
#include <CommunicationModel/Interface/InterfaceManagerBase.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayMaxonSerialV2ToRS232::CGatewayMaxonSerialV2ToRS232()
	: m_pCommand_WriteData(NULL)
    , m_pCommand_ReadData(NULL)
{
    InitCommands();
    InitErrorHandling();
}

CGatewayMaxonSerialV2ToRS232::~CGatewayMaxonSerialV2ToRS232()
{
    DeleteCommands();
}

void CGatewayMaxonSerialV2ToRS232::DeleteCommands()
{
    if(m_pCommand_ReadData)
    {
        delete m_pCommand_ReadData;
        m_pCommand_ReadData = NULL;
    }

    if(m_pCommand_WriteData)
    {
        delete m_pCommand_WriteData;
        m_pCommand_WriteData = NULL;
    }
}

void CGatewayMaxonSerialV2ToRS232::InitCommands()
{
    DeleteCommands();

    //Init Command WriteData
    m_pCommand_WriteData = new CCommand_I_RS232();
    m_pCommand_WriteData->InitCommand(RS232_WRITE_DATA);

    //Init Command ReadData
    m_pCommand_ReadData = new CCommand_I_RS232();
    m_pCommand_ReadData->InitCommand(RS232_READ_DATA);
}

CGateway* CGatewayMaxonSerialV2ToRS232::Clone()
{
    CGatewayMaxonSerialV2ToRS232* pClonedGateway;

    pClonedGateway = new CGatewayMaxonSerialV2ToRS232();
    *pClonedGateway = *this;

    return pClonedGateway;
}

CGatewayMaxonSerialV2ToRS232& CGatewayMaxonSerialV2ToRS232::operator=(CGatewayMaxonSerialV2ToRS232& other)
{
    if(this != &other)
    {
        *((CGatewayMaxonSerialV2ToI*)this) = *((CGatewayMaxonSerialV2ToI*)&other);
    }

    return *this;
}

BOOL CGatewayMaxonSerialV2ToRS232::I_ReadData(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);
    DWORD ulDataLength;

    if(m_pCommand_ReadData && pInterfaceManager)
    {
        //Set Parameter Data
        m_pCommand_ReadData->ResetStatus();
        m_pCommand_ReadData->SetParameterData(0, &dNumberOfBytesToRead, sizeof(dNumberOfBytesToRead));

        //Execute Command
        oResult = pInterfaceManager->ExecuteCommand(m_pCommand_ReadData, hI_Handle, hTransactionHandle);

        //Limit Parameter Length
        ulDataLength = m_pCommand_ReadData->GetReturnParameterLength(0);
        if(ulDataLength > dNumberOfBytesToRead) ulDataLength = dNumberOfBytesToRead;

        //Get ReturnParameter Data
        m_pCommand_ReadData->GetReturnParameterData(0, pData, ulDataLength);
        if(pdNumberOfBytesRead) *pdNumberOfBytesRead = ulDataLength;

        //Get ErrorCode
        m_pCommand_ReadData->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayMaxonSerialV2ToRS232::I_WriteData(CInterfaceManagerBase* pInterfaceManager, HANDLE hI_Handle, HANDLE hTransactionHandle, void* pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_WriteData && pInterfaceManager)
    {
        //Set Parameter Data
        m_pCommand_WriteData->ResetStatus();
        m_pCommand_WriteData->SetParameterData(0, pData, dNumberOfBytesToWrite);

        //Execute Command
        oResult = pInterfaceManager->ExecuteCommand(m_pCommand_WriteData, hI_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteData->GetReturnParameterData(0, pdNumberOfBytesWritten, sizeof(*pdNumberOfBytesWritten));

        //Get ErrorCode
        m_pCommand_WriteData->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}
BOOL CGatewayMaxonSerialV2ToRS232::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName(_T("GatewayMaxonSerialV2ToRS232"));

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(PROTOCOL_STACK_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

BOOL CGatewayMaxonSerialV2ToRS232::InitLayerParameterStack(CCommandRoot* p_pCommand)
{
	CLayerParameterStack layerParameterStack;
	CLayerParameterSet layerParameterSet;
	BOOL oResult = FALSE;

	if(p_pCommand)
	{
		if(p_pCommand->GetLayerParameterStack(layerParameterStack))
		{
			//Pop PS Layer Parameter Set
			layerParameterStack.PopLayer(PROTOCOL_STACK_LAYER, layerParameterSet);

			//Set I Layer Commands
			if(m_pCommand_WriteData) m_pCommand_WriteData->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadData) m_pCommand_ReadData->SetLayerParameterStack(layerParameterStack);
			oResult = TRUE;
		}
	}

	return oResult;
}


