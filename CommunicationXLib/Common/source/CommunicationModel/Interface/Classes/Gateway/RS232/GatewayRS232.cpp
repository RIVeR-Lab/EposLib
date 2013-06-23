// GatewayRS232ToWin32.cpp: Implementierung der Klasse CGatewayRS232ToWin32.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayRS232.h"


#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/BaseClasses/Command_I.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Interface/Command_I_RS232.h>
#include <CommunicationModel/Interface/InterfaceManagerBase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayRS232ToWin32::CGatewayRS232ToWin32()
{
    m_oFirstInit = FALSE;

    InitErrorHandling();
    InitBaudrateSelection();
    InitDefaultPortSettings();
}

CGatewayRS232ToWin32::~CGatewayRS232ToWin32()
{
}

BOOL CGatewayRS232ToWin32::InitBaudrateSelection()
{
    m_BaudrateSelection.clear();
    m_BaudrateSelection.push_back(9600);
#ifdef WINVER    
    m_BaudrateSelection.push_back(14400);
#endif
    m_BaudrateSelection.push_back(19200);
    m_BaudrateSelection.push_back(38400);
    m_BaudrateSelection.push_back(57600);
    m_BaudrateSelection.push_back(115200);

    return TRUE;
}

BOOL CGatewayRS232ToWin32::InitDefaultPortSettings()
{
    m_dDefaultBaudrate = k_115200Baud;
    m_dDefaultTimeout = 500;

    GetDefaultPortSettings(&m_dBaudrate, &m_dTimeout);

    return TRUE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::ProcessCommand(CCommandRoot* pCommand, CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    CCommand_I* pCommand_I;

    if(CGateway::ProcessCommand(pCommand, pManager, h_Handle, hTransactionHandle))
	{
		if(CheckLayers(pCommand, pManager))
		{
			pCommand_I = (CCommand_I*)pCommand;

			switch(pCommand->GetCommandId())
			{
				case RS232_WRITE_DATA: return Process_WriteData(pCommand_I);
				case RS232_READ_DATA: return Process_ReadData(pCommand_I);
			}
		}
	}

    return FALSE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::Process_WriteData(CCommand_I* pCommand)
{
    //*Constants I*
    const int k_ParameterIndex_Data = 0;
    const int k_ReturnParameterIndex_NbOfBytesWritten = 0;

    //*Variables I*
    //Parameter
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;
    //ReturnParameter
    DWORD dNbOfBytesWritten = 0;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(pCommand )
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Prepare DataBuffer
        dDataBufferLength = pCommand->GetParameterLength(k_ParameterIndex_Data);
        if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

        //Get I Parameter Data
        pCommand->GetParameterData(k_ParameterIndex_Data, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = WriteData(pDataBuffer, dDataBufferLength, &dNbOfBytesWritten, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(k_ReturnParameterIndex_NbOfBytesWritten, &dNbOfBytesWritten, sizeof(dNbOfBytesWritten));

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();

    }

    return oResult;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::Process_ReadData(CCommand_I* pCommand)
{
    //*Constants I*
    const int k_ParameterIndex_NbOfBytesToRead = 0;
    const int k_ReturnParameterIndex_Data = 0;

    //*Variables I*
    //Parameter
    DWORD dNbOfBytesToRead;

    //ReturnParameter
    void* pDataBuffer = NULL;
    DWORD dDataBufferLength;
    DWORD dNbOfBytesRead = 0;

    BOOL oResult = FALSE;
    CErrorInfo errorInfo;

    if(pCommand)
    {
        //Lock CriticalSection
		if(!Lock(pCommand->GetTimeout())) return FALSE;

		//Get I Parameter Data
        pCommand->GetParameterData(k_ParameterIndex_NbOfBytesToRead, &dNbOfBytesToRead, sizeof(dNbOfBytesToRead));

        //Prepare DataBuffer
        dDataBufferLength = dNbOfBytesToRead;
        if(dDataBufferLength > 0) pDataBuffer = malloc(dDataBufferLength);

        //Execute Command
        oResult = ReadData(pDataBuffer, dNbOfBytesToRead, &dNbOfBytesRead, &errorInfo);

        //Set PS ReturnParameter Data
        pCommand->SetStatus(oResult, &errorInfo);
        pCommand->SetReturnParameterData(k_ReturnParameterIndex_Data, pDataBuffer, dNbOfBytesRead);

        //Free DataBuffer
        if(pDataBuffer) free(pDataBuffer);

		//Unlock CriticalSection
		Unlock();
    }

    return oResult;
}

//********************************************************************
CGateway* CGatewayRS232ToWin32::Clone()
{
    CGatewayRS232ToWin32* pClonedGateway;

    pClonedGateway = new CGatewayRS232ToWin32();
    *pClonedGateway = *this;

    return pClonedGateway;
}

//********************************************************************
CGatewayRS232ToWin32& CGatewayRS232ToWin32::operator=(CGatewayRS232ToWin32& other)
{
    if(this != &other)
    {
        *((CGatewayIToDrv*)this) = *((CGatewayIToDrv*)&other);

    }

    return *this;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::OpenPort(CStdString strPortName, CErrorInfo* pErrorInfo)
{
    const int k_MaxPortNameSize = 20;
    const BOOL k_oChangeOnly = FALSE;

	if( !m_Rs232Hndl.OpenPort(strPortName) )
	{
		if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_OpeningPort, pErrorInfo);
        return FALSE;
	}

	//Configure default settings for port
    if(!SetPortSettings(m_dBaudrate, m_dTimeout, k_oChangeOnly, pErrorInfo))
    {
        ClosePort();
        return FALSE;
    }

    return TRUE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::SetPortSettings(DWORD dBaudrate, DWORD dTimeout, BOOL oChangeOnly, CErrorInfo* pErrorInfo)
{
    BOOL oResult = TRUE;

	if(!oChangeOnly || (dTimeout != m_dTimeout) || (dBaudrate != m_dBaudrate))
		if( !m_Rs232Hndl.SetPortSettings(dBaudrate, dTimeout, oChangeOnly == TRUE) )
			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_SetPortSettings, pErrorInfo);

    Trace_Settings(dBaudrate, dTimeout);

    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_SetPortSettings, pErrorInfo);

    return oResult;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::GetPortModeSelection(CStdStringArray* pPortModeSel,CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);

	if(pPortModeSel)
	{
		//Init
		pPortModeSel->clear();
		oResult = TRUE;

		//NoParity (Index 0)
		pPortModeSel->push_back(_T("Standard"));

		//EvenParity (Index 1)
		pPortModeSel->push_back(_T("EvenParity"));
	}

	return oResult;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::SetPortMode(WORD wPortMode, CErrorInfo* pErrorInfo)
{
	BOOL oResult(FALSE);

	if(m_Rs232Hndl.SetPortMode(wPortMode))
	{
		m_wPortMode = wPortMode;
		oResult = TRUE;
	}
	else
	{
		if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_SetPortMode, pErrorInfo);
	}
	    
    return oResult;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::SetTimeout(DWORD dTimeout)
{
	m_dTimeout = dTimeout;

	return m_Rs232Hndl.SetTimeout(dTimeout)  ? TRUE : FALSE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::PurgeBuffer()
{
	return m_Rs232Hndl.Purge() ? TRUE : FALSE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::WriteData(void* pDataBuffer, DWORD dNbOfBytesToWrite, DWORD* pdNbOfBytesWritten, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten = 0;

    //ResetErrorInfo
    if(pErrorInfo) pErrorInfo->Reset();

    //Execute Command
    if(pDataBuffer && (dNbOfBytesToWrite > 0))
    {
		m_Rs232Hndl.Write((unsigned char*)pDataBuffer, dNbOfBytesToWrite, (unsigned int*)&dNumberOfBytesWritten);

		if( dNumberOfBytesWritten == dNbOfBytesToWrite )
        {
			Trace_WriteData(TRUE, pDataBuffer, dNbOfBytesToWrite, &dNumberOfBytesWritten);
			if(pdNbOfBytesWritten) *pdNbOfBytesWritten = dNumberOfBytesWritten;
			return TRUE;
        }

        Trace_WriteData(FALSE, pDataBuffer, dNbOfBytesToWrite, &dNumberOfBytesWritten);
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_RS232_WriteData, pErrorInfo);
        return FALSE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::ReadData(void* pDataBuffer, DWORD dNbOfBytesToRead, DWORD* pdNbOfBytesRead, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    //ResetErrorInfo
    if(pErrorInfo) pErrorInfo->Reset();

    //Execute Command
    if(pDataBuffer && (dNbOfBytesToRead > 0))
    {
		if( m_Rs232Hndl.Read((unsigned char*)pDataBuffer, dNbOfBytesToRead, (unsigned int*)&dNumberOfBytesRead) )
		{
			*pdNbOfBytesRead = dNumberOfBytesRead;

			Trace_ReadData(TRUE, pDataBuffer, dNbOfBytesToRead, &dNumberOfBytesRead);

			return TRUE;
		}
		
        Trace_ReadData(FALSE, pDataBuffer, dNbOfBytesToRead, &dNumberOfBytesRead);

        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_RS232_ReadData, pErrorInfo);

        return FALSE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::GetPortSettings(DWORD* pdBaudrate, DWORD* pdTimeout, CErrorInfo* pErrorInfo)
{
    if(pdBaudrate) *pdBaudrate = m_dBaudrate;
    if(pdTimeout) *pdTimeout = m_dTimeout;
    return TRUE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::GetPortMode(WORD* pwPortMode, CErrorInfo* pErrorInfo)
{
    if(pwPortMode)
    {
        *pwPortMode = m_wPortMode;
        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, pErrorInfo);
    return FALSE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::ClosePort(CErrorInfo* pErrorInfo)
{
	if( !m_Rs232Hndl.ClosePort() )
	{
		if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_ClosingPort, pErrorInfo);
		return FALSE;
	}
	return TRUE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::ResetPort(CErrorInfo* pErrorInfo)
{
    if(!PurgeBuffer())
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_I_ClosingPort, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::InitPort(WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
	std::vector<CStdString> ports = m_Rs232Hndl.InitPort();
	
	for( std::vector<CStdString>::iterator it = ports.begin(); it!= ports.end(); it++)
		AddPortList(*it);

	return ports.size() > 0;
}

BOOL CGatewayRS232ToWin32::InitInterfacePortName(CStdString* pStrInterfacePortName, WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    m_strInterfacePortName = INTERFACE_PORT_RS232;
    if(pStrInterfacePortName) *pStrInterfacePortName = m_strInterfacePortName;
    return TRUE;
}

BOOL CGatewayRS232ToWin32::InitInterfaceName(CStdString* pStrInterfaceName, WORD wBoardNumber, WORD wNbBoardWithOldDriver)
{
    m_strInterfaceName = INTERFACE_RS232;
    if(pStrInterfaceName) *pStrInterfaceName = m_strInterfaceName;
    return TRUE;
}

//********************************************************************
BOOL CGatewayRS232ToWin32::InitErrorHandling()
{
    CErrorProducer errorProducer;
    CStdString strClassName = "GatewayRS232ToWin32";

    if(m_pErrorHandling)
    {
        //Init ErrorProducer
        errorProducer.Init(INTERFACE_LAYER, strClassName);
        m_pErrorHandling->InitErrorProducer(&errorProducer);
        return TRUE;
    }

    return FALSE;
}

