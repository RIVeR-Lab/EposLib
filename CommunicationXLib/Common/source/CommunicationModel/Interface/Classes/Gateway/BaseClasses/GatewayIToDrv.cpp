// GatewayIToDrv.cpp: Implementierung der Klasse CGatewayIToDrv.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/Interface/Classes/Gateway/BaseClasses/GatewayIToDrv.h>
#include <Process/MmcProcess.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CGatewayIToDrv::CGatewayIToDrv()
{
	m_eInputLayer = INTERFACE_LAYER;
	m_eOutputLayer = NO_LAYER;

	m_strInterfaceName = "";
	m_strInterfacePortName = "";
	m_dDefaultBaudrate = 0;
	m_dDefaultTimeout = 0;

	m_dBaudrate = 0;
	m_dTimeout = 0;
	m_wPortMode = 0;

	m_TraceFileName = _T("");
	m_oTracingEnabled = FALSE;
	m_oTraceFileOpen = FALSE;
	m_ulTraceLineCount = 0;
	m_ulStartTime = 0;
	m_ulLastTime = 0;
}

CGatewayIToDrv::~CGatewayIToDrv()
{
}

BOOL CGatewayIToDrv::InitGateway()
{
	return TRUE;
}

BOOL CGatewayIToDrv::InitBaudrateSelection(CStdDWordArray& p_rBaudrateSel)
{
	m_BaudrateSelection.clear();
	m_BaudrateSelection = p_rBaudrateSel;

	return TRUE;
}

BOOL CGatewayIToDrv::InitDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout)
{
	m_dDefaultBaudrate = p_ulBaudrate;
	m_dDefaultTimeout = p_ulTimeout;

	GetDefaultPortSettings(&m_dBaudrate, &m_dTimeout);

	return TRUE;
}

BOOL CGatewayIToDrv::InitInterfacePortName(CStdString* p_pInterfacePortName, WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver)
{
	return FALSE;
}

BOOL CGatewayIToDrv::InitInterfaceName(CStdString* p_pInterfaceName, WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver)
{
	return FALSE;
}

CGateway* CGatewayIToDrv::Clone()
{
	return 0;
}

BOOL CGatewayIToDrv::InitPort(WORD p_usBoardNumber, WORD p_usNbBoardWithOldDriver)
{
	return FALSE;
}

BOOL CGatewayIToDrv::UpdatePort(tPortList& p_rOpenPortList)
{
	return FALSE;
}

BOOL CGatewayIToDrv::OpenPort(CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CGatewayIToDrv::ReopenPort(CStdString p_PortName, CErrorInfo* p_pErrorInfo)
{
	BOOL oResult = FALSE;

    //Close
    if(ClosePort(p_pErrorInfo))
    {
        //Open
        oResult = OpenPort(p_PortName, p_pErrorInfo);
    }

    if(!oResult && m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);

    return oResult;
}

BOOL CGatewayIToDrv::ClosePort(CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CGatewayIToDrv::ResetPort(CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CGatewayIToDrv::SetPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, BOOL p_oChangeOnly, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CGatewayIToDrv::GetPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CGatewayIToDrv::GetDefaultPortSettings(DWORD* p_pulBaudrate, DWORD* p_pulTimeout, CErrorInfo* p_pErrorInfo)
{
	if(p_pulBaudrate) *p_pulBaudrate = m_dDefaultBaudrate;
	if(p_pulTimeout) *p_pulTimeout = m_dDefaultTimeout;

	return TRUE;
}

BOOL CGatewayIToDrv::SetDefaultPortSettings(DWORD p_ulBaudrate, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
	m_dDefaultBaudrate = p_ulBaudrate;
	m_dDefaultTimeout = p_ulTimeout;

	return TRUE;
}

BOOL CGatewayIToDrv::SetPortMode(WORD p_usPortMode, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

BOOL CGatewayIToDrv::GetPortMode(WORD* p_pusPortMode, CErrorInfo* p_pErrorInfo)
{
	return FALSE;
}

CGatewayIToDrv& CGatewayIToDrv::operator=(CGatewayIToDrv& other)
{
	if(this != &other)
	{
		*((CGateway*)this) = *((CGateway*)&other);
		m_strInterfaceName = other.m_strInterfaceName;
		m_strInterfacePortName = other.m_strInterfacePortName;

		m_BaudrateSelection.clear();
		m_BaudrateSelection=other.m_BaudrateSelection;

		m_dDefaultBaudrate = other.m_dDefaultBaudrate;
		m_dDefaultTimeout = other.m_dDefaultTimeout;

		m_dBaudrate = other.m_dBaudrate;
		m_dTimeout = other.m_dTimeout;
		m_wPortMode = other.m_wPortMode;
	}

	return *this;
}

BOOL CGatewayIToDrv::ProcessCommand(CCommandRoot* p_pCommand, CLayerManagerBase* p_pManager, HANDLE p_h_Handle, HANDLE p_hTransactionHandle)
{
	return FALSE;
}

BOOL CGatewayIToDrv::OpenInterface(CStdString p_InterfaceName, CErrorInfo* p_pErrorInfo)
{
	return TRUE;
}

BOOL CGatewayIToDrv::CloseInterface(CErrorInfo* p_pErrorInfo)
{
	return TRUE;
}

void CGatewayIToDrv::DeletePortList()
{
	m_strPortArray.clear();
}

void CGatewayIToDrv::RemoveAtPortList(CStdString p_PortName)
{
    CStdString portNameList;

	for(size_t i = 0; i < m_strPortArray.size(); i++)
	{
		portNameList = m_strPortArray.at(i);

		if(p_PortName == portNameList)
		{
            m_strPortArray.erase(m_strPortArray.begin() + i);
            break;
        }
    }
}

void CGatewayIToDrv::AddPortList(CStdString p_PortName)
{
	CStdString portNameList;

	for(size_t i = 0; i < m_strPortArray.size(); i++)
	{
		portNameList = m_strPortArray.at(i);

		if(p_PortName == portNameList)
		{
			m_strPortArray[i] = p_PortName;
			return;
		}
	}
	m_strPortArray.push_back(p_PortName);
	return;
}

BOOL CGatewayIToDrv::GetPortNameSelection(CStdStringArray* p_pPortSel, CErrorInfo* p_pErrorInfo)
{
	BOOL oResult = FALSE;

	if(p_pPortSel)
	{
		p_pPortSel->clear();
		*p_pPortSel = m_strPortArray;
		oResult = TRUE;
	}
    else
    {
	    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    }

	return oResult;
}

BOOL CGatewayIToDrv::GetBaudrateSelection(CStdDWordArray* p_pulBaudrateSel, CErrorInfo* p_pErrorInfo)
{
	BOOL oResult = FALSE;

	if(p_pulBaudrateSel)
	{
		p_pulBaudrateSel->clear();
		*p_pulBaudrateSel = m_BaudrateSelection;
		oResult = TRUE;
	}
    else
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_NullPointer, p_pErrorInfo);
    }

	return oResult;
}

BOOL CGatewayIToDrv::GetPortModeSelection(CStdStringArray* p_pPortModeSel, CErrorInfo* p_pErrorInfo)
{
	if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
	return FALSE;
}

BOOL CGatewayIToDrv::IsPortNameSupported(CStdString p_PortName)
{
	CStdString strName;

	for(size_t i = 0; i < m_strPortArray.size(); i++)
	{
		strName = m_strPortArray.at(i);

		if(strName == p_PortName)
		{
			return TRUE;
		}
	}

	return FALSE;
}

WORD CGatewayIToDrv::GetNbOfAvailableBoards()
{
	return 0;
}

BOOL CGatewayIToDrv::EnableTracing(CStdString p_FileName, CErrorInfo* p_pErrorInfo)
{
	BOOL oResult = FALSE;

	if(!m_oTracingEnabled)
	{
		//Init
		m_oTracingEnabled = TRUE;
		m_TraceFileName = p_FileName;

		//Open
		if(Trace_Open(p_FileName))
		{
			//Succeeded
			m_oTracingEnabled = TRUE;
			oResult = TRUE;
		}
		else
		{
			//Failed
			m_oTracingEnabled = FALSE;
			m_TraceFileName = _T("");

			if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
			oResult = FALSE;
		}
	}

	return oResult;
}

BOOL CGatewayIToDrv::DisableTracing(CErrorInfo* p_pErrorInfo)
{
	BOOL oResult = FALSE;

	//Close
	if(Trace_Close())
	{
		m_oTracingEnabled = FALSE;
		oResult = TRUE;
	}

	return oResult;
}

BOOL CGatewayIToDrv::Trace_Open(CStdString p_FileName)
{
	BOOL oResult = FALSE;

	if(!m_oTraceFileOpen && !m_TraceFileName.IsEmpty())
	{
		m_TraceFile.open(m_TraceFileName);

		if(!m_TraceFile.fail())
		{
			m_ulStartTime = 0;
			m_ulTraceLineCount = 0;
			m_oTraceFileOpen = TRUE;
			m_TraceFileName = p_FileName;
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CGatewayIToDrv::Trace_WriteData(BOOL p_oResult, void* p_pDataBuffer, DWORD p_ulNbOfBytesToWrite, DWORD* p_pulNbOfBytesWritten, CStdString p_Description)
{
	CStdString traceLine;
	BOOL oRes = FALSE;

	if(m_oTracingEnabled)
	{
		if(Trace_FormatData("WriteData", p_oResult, p_pDataBuffer, p_ulNbOfBytesToWrite, p_pulNbOfBytesWritten, TRUE, p_Description, traceLine))
		{
			oRes = Trace_WriteLine(traceLine);
		}
	}

	return oRes;
}

BOOL CGatewayIToDrv::Trace_ReadData(BOOL p_oResult, void* p_pDataBuffer, DWORD p_ulNbOfBytesToRead, DWORD* p_pulNbOfBytesRead, CStdString p_Description)
{
	CStdString traceLine;
	BOOL oRes = FALSE;

	if(m_oTracingEnabled)
	{
		if(Trace_FormatData("ReadData", p_oResult, p_pDataBuffer, p_ulNbOfBytesToRead, p_pulNbOfBytesRead, FALSE, p_Description, traceLine))
		{
			oRes = Trace_WriteLine(traceLine);
		}
	}

	return oRes;
}

BOOL CGatewayIToDrv::Trace_Settings(DWORD p_ulBaudrate, DWORD p_ulTimeout)
{
	CStdString traceLine;
	CStdString str;
	DWORD ulTime = 0;
	BOOL oRes = FALSE;

	if(m_oTracingEnabled)
	{
		//Line
		str.Format(_T("Line %.8X:"), m_ulTraceLineCount++);
		traceLine = str;

		//Thread
		str.Format(_T(" Thread %i"), MmcGetCurrentThreadId());
		traceLine += str;

		//Time
		if(m_ulStartTime == 0)
		{
			m_ulStartTime = MmcGetTickCount();
			m_ulLastTime = 0;
			str.Format(_T(" Time: %.8ums, %.8ums; "), 0, 0);
		}
		else
		{
			ulTime = MmcGetTickCount() - m_ulStartTime;
			str.Format(_T(" Time: %.8ums, %.8ums; "), ulTime, ulTime - m_ulLastTime);
			m_ulLastTime = ulTime;
		}
		traceLine += str;

		//Settings
		str.Format(_T("Baudrate=%i, Timeout=%i"), p_ulBaudrate, p_ulTimeout);
		traceLine += str;

		oRes = Trace_WriteLine(traceLine);
	}

	return oRes;
}

BOOL CGatewayIToDrv::Trace_Result(BOOL p_oResult, CStdString p_Description)
{
	BOOL oRes(FALSE);
	CStdString str(_T(""));
	CStdString traceLine = _T("");
	DWORD ulTime(0);

	if(m_oTracingEnabled)
	{
		//Line
		str.Format(_T("Line %.8X:"), m_ulTraceLineCount++);
		traceLine += str;

		//Thread
		str.Format(_T(" Thread %i"), MmcGetCurrentThreadId());
		traceLine += str;

		//Time
		if(m_ulStartTime == 0)
		{
			m_ulStartTime = MmcGetTickCount();
			m_ulLastTime = 0;
			str.Format(_T(" Time: %.8ums, %.8ums; "), 0, 0);
		}
		else
		{
			ulTime = MmcGetTickCount() - m_ulStartTime;
			str.Format(_T(" Time: %.8ums, %.8ums; "), ulTime, ulTime - m_ulLastTime);
			m_ulLastTime = ulTime;
		}
		traceLine += str;

		//Header
		if(p_oResult)
		{
			if(p_Description.IsEmpty())
			{
				traceLine += _T(" Succeeded: ");
			}
			else
			{
				traceLine += _T(" Succeeded (") + p_Description + _T("): ");
			}
		}
		else
		{
			if(p_Description.IsEmpty())
			{
				traceLine += _T(" Failed: ");
			}
			else
			{
				traceLine += _T(" Failed (") + p_Description + _T("): ");
			}
		}

		oRes = Trace_WriteLine(traceLine);
	}

	return oRes;
}

BOOL CGatewayIToDrv::Trace_FormatData(CStdString p_Function, BOOL p_oResult, void* p_pDataBuffer, DWORD p_ulNbOfBytesToDo, DWORD* p_pulNbOfBytesDone, BOOL p_oShowFailedData, CStdString p_Description, CStdString& p_rTraceLine)
{
	DWORD ulTime = 0;
	CStdString traceLine = _T("");
	CStdString str = _T("");
	DWORD ulDataSize = 0;
	BOOL oRes = FALSE;

	if(p_pDataBuffer && p_pulNbOfBytesDone)
	{
		//Line
		str.Format(_T("Line %.8X:"), m_ulTraceLineCount++);
		traceLine = str;

		//Thread
		str.Format(_T(" Thread %i"), MmcGetCurrentThreadId());
		traceLine += str;

		//Time
		if(m_ulStartTime == 0)
		{
			m_ulStartTime = MmcGetTickCount();
			m_ulLastTime = 0;
			str.Format(_T(" Time: %.8ums, %.8ums; "), 0, 0);
		}
		else
		{
			ulTime = MmcGetTickCount() - m_ulStartTime;
			str.Format(_T(" Time: %.8ums, %.8ums; "), ulTime, ulTime - m_ulLastTime);
			m_ulLastTime = ulTime;
		}
		traceLine += str;

		//Header
		traceLine += p_Function;
		if(p_oResult)
		{
			if(p_Description.IsEmpty())
			{
				traceLine += _T(" Succeeded: ");
			}
			else
			{
				traceLine += _T(" Succeeded (") + p_Description + _T("): ");
			}
		}
		else
		{
			if(p_Description.IsEmpty())
			{
				traceLine += _T(" Failed: ");
			}
			else
			{
				traceLine += _T(" Failed (") + p_Description + _T("): ");
			}
		}

		//Numbers
		str.Format(_T("ToDo %i, Done %i:"), p_ulNbOfBytesToDo, *p_pulNbOfBytesDone);
		traceLine += str;

		//Data
		if(p_oShowFailedData) ulDataSize = p_ulNbOfBytesToDo; else ulDataSize = *p_pulNbOfBytesDone;
		for(DWORD i = 0; i < ulDataSize; i++)
		{
			str.Format(_T("%.2X "), *((BYTE*)p_pDataBuffer + i));
			traceLine += str;
		}

		p_rTraceLine = traceLine;
		oRes = TRUE;
	}

	return oRes;
}

BOOL CGatewayIToDrv::Trace_WriteLine(CStdString p_TraceLine)
{
	char lineFeed = '\x0A';
	char carriageReturn = '\r';
	TCHAR data = 0;
	BOOL oResult = FALSE;

	if(m_oTraceFileOpen)
	{
		for(int i = 0; i < p_TraceLine.GetLength(); i++)
		{
			data = p_TraceLine.GetAt(i);

			m_TraceFile << data;
		}

		m_TraceFile << carriageReturn << lineFeed;
	}

	return oResult;
}

BOOL CGatewayIToDrv::Trace_Close()
{
	BOOL oResult = FALSE;

	if(m_oTraceFileOpen)
	{
		m_TraceFile.close();
		m_oTraceFileOpen = FALSE;
		oResult = TRUE;
	}

	return oResult;
}

BOOL CGatewayIToDrv::AddPort(CStdString p_PortName)
{
    return FALSE;
}

BOOL CGatewayIToDrv::DeletePort(CStdString p_PortName)
{
    return FALSE;
}
