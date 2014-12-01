// GatewayVCStoEsam.cpp: Implementierung der Klasse CGatewayVCStoEsam.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayVCStoEsam.h"

#include <memory.h>
#include <Process/MmcProcess.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsamDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenCommunicationDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenDriveDef.h>

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
CGatewayVCStoEsam::CGatewayVCStoEsam()
{
    m_pCommand_WriteObject = NULL;
    m_pCommand_InitiateSegmentedWrite = NULL;
    m_pCommand_SegmentedWrite = NULL;
    m_pCommand_ReadObject = NULL;
    m_pCommand_InitiateSegmentedRead = NULL;
    m_pCommand_SegmentedRead = NULL;
    m_pCommand_AbortSegmentedTransfer = NULL;
    InitCommands_ObjectDictionary();

    m_pCommand_RequestCANFrame = NULL;
    m_pCommand_SendCANFrame = NULL;
    m_pCommand_ReadCANFrame = NULL;
    InitCommands_GeneralGateway();

    m_pCommand_SendNMTService = NULL;
    InitCommands_NetworkManagement();

    m_pCommand_SendLSSFrame = NULL;
    m_pCommand_ReadLSSFrame = NULL;
    InitCommands_LayerSettingServices();
}

CGatewayVCStoEsam::~CGatewayVCStoEsam()
{
    DeleteCommands_ObjectDictionary();
    DeleteCommands_NetworkManagement();
    DeleteCommands_GeneralGateway();
    DeleteCommands_LayerSettingServices();
}

BOOL CGatewayVCStoEsam::ReadODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pDataBuffer, DWORD dMaxNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    const WORD k_MaxLength_NonSegmentedTransfer = 4;

    BOOL oResult;
    BOOL oLastDataSegment;
    BYTE* pSegmentBuffer;
    DWORD segmentBufferLength;
    DWORD segmentLengthRead;
    DWORD dTotalNumberOfBytesRead;
    DWORD dTotalNumberOfBytesToRead;
    DWORD dNumberOfBytesRead;
    DWORD dAbortCode;

    if(pManager)
    {
        if(dMaxNumberOfBytesToRead <= k_MaxLength_NonSegmentedTransfer)
        {
            //Non Segmented Transfer
            if(pdNumberOfBytesRead) *pdNumberOfBytesRead = 0;

            if(DCS_ReadObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, pDataBuffer, dMaxNumberOfBytesToRead, &dNumberOfBytesRead, pErrorInfo))
            {
                if(pdNumberOfBytesRead) *pdNumberOfBytesRead = dNumberOfBytesRead;
                return TRUE;
            }

            return FALSE;
        }
        else
        {
            //Don't allow new segmented Transfer
            if(m_oAbortCommands)
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                return FALSE;
            }

            //Segmented Transfer
            if(pdNumberOfBytesRead) *pdNumberOfBytesRead = 0;
            if(DCS_InitiateSegmentedRead(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, &dTotalNumberOfBytesToRead, pErrorInfo))
            {
                //NumberOfBytesToRead
                if(dTotalNumberOfBytesToRead > dMaxNumberOfBytesToRead) dTotalNumberOfBytesToRead = dMaxNumberOfBytesToRead;

                //Initialize variables
                dTotalNumberOfBytesRead = 0;
                oLastDataSegment = FALSE;
                segmentBufferLength = dTotalNumberOfBytesToRead;
                pSegmentBuffer = (BYTE*)pDataBuffer;

                oResult = TRUE;
                while(oResult && !oLastDataSegment && (segmentBufferLength > 0))
                {
                    //Abort Transfer
                    if(m_oAbortCommands)
                    {
                        dAbortCode = (DWORD)-1;
                        if(DCS_AbortSegmentedTransfer(pManager, hDCS_Handle, hTransactionHandle, uNodeId, wIndex, uSubIndex, dAbortCode))
                        {
                            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                            return FALSE;
                        }
                    }

                    //Do Transfer
                    oResult = DCS_SegmentedRead(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, &oLastDataSegment, pSegmentBuffer, segmentBufferLength, &segmentLengthRead, pErrorInfo);
                    if(oResult)
                    {
                        //Prepare next segment
                        dTotalNumberOfBytesRead += segmentLengthRead;
                        pSegmentBuffer += segmentLengthRead;
                        segmentBufferLength -= segmentLengthRead;
                    }
                }

                if(pdNumberOfBytesRead) *pdNumberOfBytesRead = dTotalNumberOfBytesRead;
                return oResult;
            }

            return FALSE;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::WriteODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pDataBuffer, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    const WORD k_MaxLength_NonSegmentedTransfer = 4;
    const WORD k_MaxSegmentLength = 127;

    DWORD lTotalNumberOfBytesToWrite;
    DWORD lTotalNumberOfBytesWritten;
    BYTE* pSegmentBuffer;
    DWORD lSegmentLength;
    DWORD lSegmentLengthWritten;
    BOOL oResult;
    DWORD dAbortCode;

    if(pManager)
    {
        if(dNumberOfBytesToWrite <= k_MaxLength_NonSegmentedTransfer)
        {
            //Non Segmented Transfer
            if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = 0;
            if(DCS_WriteObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, pDataBuffer, dNumberOfBytesToWrite, pErrorInfo))
            {
                if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = dNumberOfBytesToWrite;
                return TRUE;
            }
            return FALSE;
        }
        else
        {
            //Don't allow new segmented transfer
            if(m_oAbortCommands)
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                return FALSE;
            }

            //Segmented Transfer
            if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = 0;
            if(DCS_InitiateSegmentedWrite(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, dNumberOfBytesToWrite, pErrorInfo))
            {
                //Initialize variables
                lTotalNumberOfBytesWritten = 0;
                lTotalNumberOfBytesToWrite = dNumberOfBytesToWrite;
                pSegmentBuffer = (BYTE*)pDataBuffer;
                if(dNumberOfBytesToWrite < k_MaxSegmentLength) lSegmentLength = dNumberOfBytesToWrite; else lSegmentLength = k_MaxSegmentLength;

                do
                {
                    //Abort Transfer
                    if(m_oAbortCommands)
                    {
                        dAbortCode = (DWORD)-1;
                        if(DCS_AbortSegmentedTransfer(pManager, hDCS_Handle, hTransactionHandle, uNodeId, wIndex, uSubIndex, dAbortCode))
                        {
                            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                            return FALSE;
                        }
                    }

                    //Do Transfer
                    oResult = DCS_SegmentedWrite(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, pSegmentBuffer, lSegmentLength, pErrorInfo);
                    if(oResult)
                    {
                        //No Feedback of written bytes
                        lSegmentLengthWritten = lSegmentLength;

                        //Prepare next segment
                        lTotalNumberOfBytesWritten += lSegmentLengthWritten;
                        dNumberOfBytesToWrite -= (WORD)lSegmentLengthWritten;
                        pSegmentBuffer += lSegmentLengthWritten;
                        if(dNumberOfBytesToWrite < lSegmentLengthWritten) lSegmentLength = dNumberOfBytesToWrite; else lSegmentLength = lSegmentLengthWritten;
                        if(lSegmentLength > k_MaxSegmentLength) lSegmentLength = k_MaxSegmentLength;
                    }

                } while(oResult && (lTotalNumberOfBytesWritten < lTotalNumberOfBytesToWrite));

                if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = lTotalNumberOfBytesWritten;
                return oResult;
            }

            return FALSE;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::ReadInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, __int8* pbValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(pbValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)pbValue, sizeof(*pbValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
        if(sizeof(*pbValue) != dNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::ReadInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, short* piValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(piValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)piValue, sizeof(*piValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
        if(sizeof(*piValue) != dNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::ReadInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, int* plValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(plValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)plValue, sizeof(*plValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
        if(sizeof(*plValue) != dNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::ReadUInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* puValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(puValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)puValue, sizeof(*puValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
        if(sizeof(*puValue) != dNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::ReadUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, WORD* pwValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(pwValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)pwValue, sizeof(*pwValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
        if(sizeof(*pwValue) != dNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::ReadUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD* pdValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(pdValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)pdValue, sizeof(*pdValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
        if(sizeof(*pdValue) != dNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::ReadStringObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE ubPortNumber, BYTE ubNodeId, WORD usIndex, BYTE ubSubIndex, CStdString* pValue, CErrorInfo* pErrorInfo)
{
	const DWORD EXPEDITED_DATA_BUFFER_SIZE = 4;
	const DWORD NON_EXPEDITED_DATA_BUFFER_SIZE = 256;

	char expeditedDataBuffer[EXPEDITED_DATA_BUFFER_SIZE + 1];
	char nonExpeditedDataBuffer[NON_EXPEDITED_DATA_BUFFER_SIZE + 1];
	char* pDataBuffer = NULL;
	DWORD dNumberOfBytesRead(0);
	BOOL oResult = FALSE;

	if(pValue)
	{
		//Init
		memset(expeditedDataBuffer, 0, EXPEDITED_DATA_BUFFER_SIZE + 1);
		memset(nonExpeditedDataBuffer, 0, NON_EXPEDITED_DATA_BUFFER_SIZE + 1);

		//Read Object
		if((ReadDataBufferObject(pManager, hDCS_Handle, hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, (void*)nonExpeditedDataBuffer, NON_EXPEDITED_DATA_BUFFER_SIZE, &dNumberOfBytesRead, pErrorInfo) && (NON_EXPEDITED_DATA_BUFFER_SIZE >= dNumberOfBytesRead)))
		{
			*pValue = nonExpeditedDataBuffer;
			oResult = TRUE;
		}
		else if((ReadDataBufferObject(pManager, hDCS_Handle, hTransactionHandle, ubPortNumber, ubNodeId, usIndex, ubSubIndex, (void*)expeditedDataBuffer, EXPEDITED_DATA_BUFFER_SIZE, &dNumberOfBytesRead, pErrorInfo) && (EXPEDITED_DATA_BUFFER_SIZE >= dNumberOfBytesRead)))
		{
			*pValue = expeditedDataBuffer;
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CGatewayVCStoEsam::ReadDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, void* pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;
    BOOL oResult;

    if(pData)
    {
        oResult = ReadODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)pData, dNumberOfBytesToRead, &dNumberOfBytesRead, pErrorInfo);

        if(pdNumberOfBytesRead) *pdNumberOfBytesRead = dNumberOfBytesRead;
        return oResult;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam::WriteUInt8Object(CDeviceCommandSetManagerBase *pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE uValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, &uValue, sizeof(uValue), &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(uValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam::WriteUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, WORD wValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)&wValue, sizeof(wValue), &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(wValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam::WriteUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD dValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)&dValue, sizeof(dValue), &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(dValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam::WriteInt8Object(CDeviceCommandSetManagerBase *pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, __int8 bValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)&bValue, sizeof(bValue), &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(bValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam::WriteInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, short iValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)&iValue, sizeof(iValue), &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(iValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam::WriteInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, int lValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)&lValue, sizeof(lValue), &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(lValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam::WriteDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, void* pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesWritten;
    BOOL oResult = FALSE;

    if(pData)
    {
        oResult = WriteODObject(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, uNodeId, wIndex, uSubIndex, (BYTE*)pData, dNumberOfBytesToWrite, &dNumberOfBytesWritten, pErrorInfo);

        if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = dNumberOfBytesWritten;
    }

    return oResult;
}

CGatewayVCStoEsam& CGatewayVCStoEsam::operator=(CGatewayVCStoEsam& other)
{
    if(this != &other)
    {
        *((CGatewayVCStoDCS*)this) = *((CGatewayVCStoDCS*)&other);

    }

    return *this;
}

void CGatewayVCStoEsam::InitCommands_ObjectDictionary()
{
    DeleteCommands_ObjectDictionary();

    //WriteObject
    m_pCommand_WriteObject = new CCommand_DCS_Esam();
    m_pCommand_WriteObject->InitCommand(ESAM_WRITE_OBJECT);

    //ReadObject
    m_pCommand_ReadObject = new CCommand_DCS_Esam();
    m_pCommand_ReadObject->InitCommand(ESAM_READ_OBJECT);

    //InitiateSegmentedWrite
    m_pCommand_InitiateSegmentedWrite = new CCommand_DCS_Esam();
    m_pCommand_InitiateSegmentedWrite->InitCommand(ESAM_INITIATE_SEGMENT_WRITE);

    //InitiateSegmentedRead
    m_pCommand_InitiateSegmentedRead = new CCommand_DCS_Esam();
    m_pCommand_InitiateSegmentedRead->InitCommand(ESAM_INITIATE_SEGMENT_READ);

    //SegmentedWrite
    m_pCommand_SegmentedWrite = new CCommand_DCS_Esam();
    m_pCommand_SegmentedWrite->InitCommand(ESAM_SEGMENT_WRITE);

    //SegmentedRead
    m_pCommand_SegmentedRead = new CCommand_DCS_Esam();
    m_pCommand_SegmentedRead->InitCommand(ESAM_SEGMENT_READ);

    //AbortSegmentedTransfer
    m_pCommand_AbortSegmentedTransfer = new CCommand_DCS_Esam();
    m_pCommand_AbortSegmentedTransfer->InitCommand(ESAM_ABORT_SEGMENT_TRANSFER);
}

void CGatewayVCStoEsam::DeleteCommands_ObjectDictionary()
{
    //WriteObject
    if(m_pCommand_WriteObject)
    {
        delete m_pCommand_WriteObject;
        m_pCommand_WriteObject = NULL;
    }

    //InitiateSegmentedWrite
    if(m_pCommand_InitiateSegmentedWrite)
    {
        delete m_pCommand_InitiateSegmentedWrite;
        m_pCommand_InitiateSegmentedWrite = NULL;
    }

    //SegmentedWrite
    if(m_pCommand_SegmentedWrite)
    {
        delete m_pCommand_SegmentedWrite;
        m_pCommand_SegmentedWrite = NULL;
    }

    //ReadObject
    if(m_pCommand_ReadObject)
    {
        delete m_pCommand_ReadObject;
        m_pCommand_ReadObject = NULL;
    }

    //InitateSegmentedRead
    if(m_pCommand_InitiateSegmentedRead)
    {
        delete m_pCommand_InitiateSegmentedRead;
        m_pCommand_InitiateSegmentedRead = NULL;
    }

    //SegmentedRead
    if(m_pCommand_SegmentedRead)
    {
        delete m_pCommand_SegmentedRead;
        m_pCommand_SegmentedRead = NULL;
    }

    //AbortSegmentedTransfer
    if(m_pCommand_AbortSegmentedTransfer)
    {
        delete m_pCommand_AbortSegmentedTransfer;
        m_pCommand_AbortSegmentedTransfer = NULL;
    }
}

void CGatewayVCStoEsam::InitCommands_NetworkManagement()
{
    DeleteCommands_NetworkManagement();

    //Init Command SendNMTService
    m_pCommand_SendNMTService = new CCommand_DCS_Esam();
    m_pCommand_SendNMTService->InitCommand(ESAM_SEND_NMT_SERVICE);
}

void CGatewayVCStoEsam::DeleteCommands_NetworkManagement()
{
    if(m_pCommand_SendNMTService)
    {
        delete m_pCommand_SendNMTService;
        m_pCommand_SendNMTService = NULL;
    }
}

void CGatewayVCStoEsam::InitCommands_GeneralGateway()
{
    DeleteCommands_GeneralGateway();

    //Init Command Request
    m_pCommand_RequestCANFrame = new CCommand_DCS_Esam();
    m_pCommand_RequestCANFrame->InitCommand(ESAM_REQUEST_CAN_FRAME);

    //Init Command Send
    m_pCommand_SendCANFrame = new CCommand_DCS_Esam();
    m_pCommand_SendCANFrame->InitCommand(ESAM_SEND_CAN_FRAME);

    //Init Command Read
    m_pCommand_ReadCANFrame = new CCommand_DCS_Esam();
    m_pCommand_ReadCANFrame->InitCommand(ESAM_READ_CAN_FRAME);
}

void CGatewayVCStoEsam::InitCommands_LayerSettingServices()
{
    DeleteCommands_LayerSettingServices();

    //Init Command Send
    m_pCommand_SendLSSFrame = new CCommand_DCS_Esam();
    m_pCommand_SendLSSFrame->InitCommand(ESAM_SEND_LSS_FRAME);

    //Init Command Read
    m_pCommand_ReadLSSFrame = new CCommand_DCS_Esam();
    m_pCommand_ReadLSSFrame->InitCommand(ESAM_SEND_LSS_FRAME);
}

void CGatewayVCStoEsam::DeleteCommands_GeneralGateway()
{
    if(m_pCommand_RequestCANFrame)
    {
        delete m_pCommand_RequestCANFrame;
        m_pCommand_RequestCANFrame = NULL;
    }

    if(m_pCommand_SendCANFrame)
    {
        delete m_pCommand_SendCANFrame;
        m_pCommand_SendCANFrame = NULL;
    }

    if(m_pCommand_ReadCANFrame)
    {
        delete m_pCommand_ReadCANFrame;
        m_pCommand_ReadCANFrame = NULL;
    }
}

void CGatewayVCStoEsam::DeleteCommands_LayerSettingServices()
{
    if(m_pCommand_SendLSSFrame)
    {
        delete m_pCommand_SendLSSFrame;
        m_pCommand_SendLSSFrame = NULL;
    }

    if(m_pCommand_ReadLSSFrame)
    {
        delete m_pCommand_ReadLSSFrame;
        m_pCommand_ReadLSSFrame = NULL;
    }
}

BOOL CGatewayVCStoEsam::DCS_SendCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wCobID, WORD wLength, void* pData, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SendCANFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_SendCANFrame->SetParameterData(1, &wCobID, sizeof(wCobID));
        m_pCommand_SendCANFrame->SetParameterData(2, &wLength, sizeof(wLength));
        m_pCommand_SendCANFrame->SetParameterData(3, pData, wLength);

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SendCANFrame, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendCANFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_ReadCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wCobID, WORD wLength, void* pData, DWORD p_ulTimeout, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadCANFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadCANFrame->ResetStatus();
        m_pCommand_ReadCANFrame->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_ReadCANFrame->SetParameterData(1, &wCobID, sizeof(wCobID));
        m_pCommand_ReadCANFrame->SetParameterData(2, &wLength, sizeof(wLength));
        m_pCommand_ReadCANFrame->SetParameterData(3, &p_ulTimeout, sizeof(p_ulTimeout));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadCANFrame, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_ReadCANFrame->GetReturnParameterData(1, pData, wLength);

        //Get ErrorCode
        m_pCommand_ReadCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_RequestCANFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wCobID, WORD wLength, void* pData, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_RequestCANFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_RequestCANFrame->SetParameterData(1, &wCobID, sizeof(wCobID));
        m_pCommand_RequestCANFrame->SetParameterData(2, &wLength, sizeof(wLength));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_RequestCANFrame, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_RequestCANFrame->GetReturnParameterData(1, pData, wLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_SendNMTService(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wNodeId, WORD wCommandSpecifier, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;
    DWORD dDeviceErrorCode = 0;

    if(m_pCommand_SendNMTService && pManager)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_SendNMTService->SetParameterData(1, &wNodeId, sizeof(wNodeId));
        m_pCommand_SendNMTService->SetParameterData(2, &wCommandSpecifier, sizeof(wCommandSpecifier));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SendNMTService, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendNMTService->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_SendLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SendLSSFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_SendLSSFrame->SetParameterData(1, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SendLSSFrame, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendLSSFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_ReadLSSFrame(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadLSSFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_ReadLSSFrame->SetParameterData(1, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadLSSFrame, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_ReadLSSFrame->GetReturnParameterData(1, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_InitiateSegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD* pdObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedRead && pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedRead->ResetStatus();
        m_pCommand_InitiateSegmentedRead->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_InitiateSegmentedRead->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(3, &uNodeId, sizeof(uNodeId));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_InitiateSegmentedRead, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(1, pdObjectLength, sizeof(*pdObjectLength));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedRead->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_InitiateSegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedWrite && pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedWrite->ResetStatus();
        m_pCommand_InitiateSegmentedWrite->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(3, &uNodeId, sizeof(uNodeId));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(4, &dObjectLength, sizeof(dObjectLength));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_InitiateSegmentedWrite, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedWrite->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_ReadObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectBufferLength, DWORD* pdObjectLengthRead, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    DWORD dObjectLength;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadObject->ResetStatus();
        m_pCommand_ReadObject->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_ReadObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_ReadObject->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_ReadObject->SetParameterData(3, &uNodeId, sizeof(uNodeId));
        m_pCommand_ReadObject->SetParameterData(4, &dObjectLength, sizeof(dObjectLength));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadObject, hHandle, hTransactionHandle);

        //Get ReturnParameter Length
        dObjectLength = m_pCommand_ReadObject->GetReturnParameterLength(1);
        if(dObjectBufferLength < dObjectLength) dObjectLength = dObjectBufferLength;
        if(pdObjectLengthRead) *pdObjectLengthRead = dObjectLength;

        //Get ReturnParameter Data
        m_pCommand_ReadObject->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_ReadObject->GetReturnParameterData(1, pObjectBuffer, dObjectLength);

        //Get ErrorCode
        m_pCommand_ReadObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_SegmentedRead(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BOOL* poLastDataSegment, BYTE* pSegmentBuffer, DWORD dSegmentBufferLength, DWORD* pdSegmentLengthRead, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    UEsamControlByte controlByte;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedRead && pManager)
    {
        //Set Parameter Data
        m_pCommand_SegmentedRead->ResetStatus();
        m_pCommand_SegmentedRead->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SegmentedRead, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedRead->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));
        m_pCommand_SegmentedRead->GetReturnParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedRead->GetReturnParameterData(2, pSegmentBuffer, dSegmentBufferLength);

        //Get ErrorCode
        m_pCommand_SegmentedRead->GetErrorInfo(pErrorInfo);

        //Restore ControlByte
        if(pdSegmentLengthRead) *pdSegmentLengthRead = controlByte.structure.ubNbOfDataBytes;
        if(poLastDataSegment) *poLastDataSegment = controlByte.structure.ubLastDataSegment;
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_SegmentedWrite(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE* pSegmentBuffer, DWORD dSegmentLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    UEsamControlByte controlByte;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedWrite && pManager)
    {
        //Prepare ControlByte
        controlByte.ubValue = 0;
        controlByte.structure.ubNbOfDataBytes = dSegmentLength;

        //Set Parameter Data
        m_pCommand_SegmentedWrite->ResetStatus();
        m_pCommand_SegmentedWrite->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_SegmentedWrite->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedWrite->SetParameterData(2, pSegmentBuffer, dSegmentLength);

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SegmentedWrite, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedWrite->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_WriteObject(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, BYTE uPortNumber, BYTE uNodeId, WORD wIndex, BYTE uSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD dDeviceErrorCode;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_WriteObject->ResetStatus();
        m_pCommand_WriteObject->SetParameterData(0, &uPortNumber, sizeof(uPortNumber));
        m_pCommand_WriteObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_WriteObject->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_WriteObject->SetParameterData(3, &uNodeId, sizeof(uNodeId));
        m_pCommand_WriteObject->SetParameterData(4, pObjectBuffer, dObjectLength);

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_WriteObject, hHandle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteObject->GetReturnParameterData(0, &dDeviceErrorCode, sizeof(dDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_WriteObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::DCS_AbortSegmentedTransfer(CLayerManagerBase* pManager, HANDLE hHandle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
{
    BYTE uSubIndex = (BYTE)wSubIndex;
    BOOL oResult = FALSE;

    if(m_pCommand_AbortSegmentedTransfer)
    {
        //Set Parameter Data
        m_pCommand_AbortSegmentedTransfer->ResetStatus();
        m_pCommand_AbortSegmentedTransfer->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(3, &dAbortCode, sizeof(dAbortCode));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_AbortSegmentedTransfer, hHandle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSegmentedTransfer->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}


BOOL CGatewayVCStoEsam::SetDisableState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,BYTE uPortNumber, WORD wNodeId, CErrorInfo* pErrorInfo)
{
	WORD usIndex = 0;
	BYTE ubSubIndex = 0;
	WORD usDeviceState = 0;
	BOOL oResult = FALSE;

	//Read Device State
    oResult = ReadDeviceState(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, wNodeId, &usDeviceState, pErrorInfo);
    if(oResult)
    {
        usIndex = INDEX_CONTROL_WORD;
        ubSubIndex = SUBINDEX_CONTROL_WORD;
        switch(usDeviceState)
        {
            case DS_FAULT:
            case DS_SWITCH_ON_DISABLED: oResult = TRUE; break;
            case DS_OPERATION_ENABLED:
            case DS_QUICK_STOP_ACTIV:
            case DS_SWITCHED_ON:
            case DS_READY_TO_SWITCH_ON:
                if(oResult)
                    oResult = WriteUInt16Object(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, (BYTE)wNodeId, usIndex, ubSubIndex, DISABLE_VOLTAGE, pErrorInfo);
                if(oResult)
                    oResult = CheckDeviceState(pManager, hDCS_Handle, hTransactionHandle, uPortNumber, wNodeId, DS_SWITCH_ON_DISABLED, pErrorInfo);
                break;
            default:
                oResult = FALSE;
        }
    }

	return oResult;
}

BOOL CGatewayVCStoEsam::ReadDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,BYTE uPortNumber, WORD wNodeId, WORD* pwDeviceState, CErrorInfo* pErrorInfo)
{
    const WORD k_wStateBit5Mask = 0x0020;
    const WORD k_wState5BitMask = 0x004F;
    const WORD k_wState6BitMask = 0x006F;

    WORD wIndex;
    BYTE ubSubIndex;
    WORD wDeviceState;

    //Read Device State
    wIndex = INDEX_STATUS_WORD;
    ubSubIndex = SUBINDEX_STATUS_WORD;
    if(!ReadUInt16Object(pManager, hDCS_Handle, hTransactionHandle,uPortNumber,(BYTE)wNodeId, wIndex, ubSubIndex, &wDeviceState, pErrorInfo))
    {
        return FALSE;
    }
    switch(wDeviceState & k_wState5BitMask)
    {
        case DS_NOT_READY_TO_SWITCH_ON:
        case DS_SWITCH_ON_DISABLED:
        case DS_FAULT_REACTION_ACTIV:
        case DS_FAULT:
            {
                wDeviceState &= ~k_wStateBit5Mask;
            }; break;
    }
    wDeviceState &= k_wState6BitMask;
    if(pwDeviceState) *pwDeviceState = wDeviceState;
    return TRUE;
}

BOOL CGatewayVCStoEsam::CheckDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle,BYTE uPortNumber, WORD wNodeId, WORD wState, CErrorInfo* pErrorInfo)
{
    DWORD dTime = 0;
    DWORD dTimeoutDeviceState = 1000;
    DWORD dStart = MmcGetTickCount();
    WORD wDeviceState = 0;

    while((dTime < dTimeoutDeviceState) && (wDeviceState != wState))
    {
        if(!ReadDeviceState(pManager, hDCS_Handle, hTransactionHandle,uPortNumber, wNodeId, &wDeviceState, pErrorInfo)) return FALSE;

        dTime = MmcGetTickCount()-dStart;

        if(dTime > dTimeoutDeviceState)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Timeout, pErrorInfo);
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CGatewayVCStoEsam::InitLayerParameterStack(CCommandRoot* p_pCommand)
{
	CLayerParameterStack layerParameterStack;
	CLayerParameterSet layerParameterSet;
	BOOL oResult = FALSE;
	
	if(p_pCommand)
	{
		if(p_pCommand->GetLayerParameterStack(layerParameterStack))
		{
			//Pop VCS Layer Parameter Set
			layerParameterStack.PopLayer(VIRTUAL_COMMAND_SET_LAYER, layerParameterSet);

			//Set DCS Layer Commands
			if(m_pCommand_WriteObject) m_pCommand_WriteObject->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadObject) m_pCommand_ReadObject->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_InitiateSegmentedRead) m_pCommand_InitiateSegmentedRead->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_SegmentedRead) m_pCommand_SegmentedRead->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_AbortSegmentedTransfer) m_pCommand_AbortSegmentedTransfer->SetLayerParameterStack(layerParameterStack);

			if(m_pCommand_SendCANFrame) m_pCommand_SendCANFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_RequestCANFrame) m_pCommand_RequestCANFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadCANFrame) m_pCommand_ReadCANFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_SendNMTService) m_pCommand_SendNMTService->SetLayerParameterStack(layerParameterStack);

			if(m_pCommand_SendLSSFrame) m_pCommand_SendLSSFrame->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadLSSFrame) m_pCommand_ReadLSSFrame->SetLayerParameterStack(layerParameterStack);

			oResult = TRUE;
		}
	}
    
	return oResult;
}

BOOL CGatewayVCStoEsam::StoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
	const DWORD TIMEOUT = 5000;

    DWORD ulPassword = 0;
    DWORD ulTransferRate = 0;
    DWORD ulBackupTimeout = 0;
    BOOL oResult = FALSE;

    if(p_pManager)
    {
        //Init
		oResult = TRUE;

		//Backup Timeout
        p_pManager->DCS_GetProtocolStackSettings(p_hHandle, &ulTransferRate, &ulBackupTimeout);

        //Increase Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, TIMEOUT, TRUE);

        //Write save all parameters
        ulPassword = 'e';
        ulPassword *= 256;
        ulPassword += 'v';
        ulPassword *= 256;
        ulPassword += 'a';
        ulPassword *= 256;
        ulPassword += 's';

        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubPortNumber, p_ubNodeId, INDEX_STORE_PARAMETERS, SUBINDEX_SAVE_ALL_PARAMETERS, ulPassword, p_pErrorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::ResetDevice(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, BOOL p_oWaitForReboot, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    //SendNMTService
    if(DCS_SendNMTService(p_pManager, p_hHandle, p_hTransactionHandle, p_ubPortNumber, p_ubNodeId, (WORD)NSI_RESET_NODE, p_pErrorInfo))
	{
		//Init
		oResult = TRUE;

		if(p_oWaitForReboot)
		{
			//WaitForReboot
			WaitForReboot(p_pManager, p_hHandle, 0, p_ubPortNumber, p_ubNodeId, 0);
		}
	}

    return oResult;
}

BOOL CGatewayVCStoEsam::WaitForReboot(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
    const DWORD REBOOT_TIMEOUT =  8000;

    BYTE uErrorRegister = 0;
    DWORD dStartTime = 0;
    DWORD dTime = 0;
    DWORD dSleepTime = 10;
	BOOL oResult = FALSE;

    //Start Time
	dStartTime = MmcGetTickCount();
    dTime = 0;

    while(dTime < REBOOT_TIMEOUT)
    {
		//Reset
		if(p_pErrorInfo) p_pErrorInfo->Reset();

		//Check Communication
		if(ReadUInt8Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubPortNumber, p_ubNodeId, INDEX_ERROR_REGISTER, SUBINDEX_ERROR_REGISTER, &uErrorRegister, p_pErrorInfo))
        {
            oResult =  TRUE;
			break;
        }
		else
		{
			//Update Time
			dTime = MmcGetTickCount()-dStartTime;
			MmcSleep(dSleepTime);
		}
    }

    return oResult;
}

BOOL CGatewayVCStoEsam::GetAddressParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rPortNumber, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo)
{
	BOOL oResult(TRUE);

	//NodeId
	if(oResult && !GetNodeId(p_pCommand, p_pManager, p_hDCS_Handle, p_rNodeId, p_pErrorInfo))
	{
		oResult = FALSE;
	}

	//PortNumber
	if(oResult && !GetPortNumber(p_pCommand, p_pManager, p_hDCS_Handle, p_rPortNumber, p_pErrorInfo))
	{
		if(p_rNodeId == 0) p_rPortNumber = 0; else p_rPortNumber = 1;
	}

	return oResult;
}

BOOL CGatewayVCStoEsam::GetPortNumber(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rPortNumber, CErrorInfo* p_pErrorInfo)
{
	const CStdString PORT_NUMBER = _T("PortNumber");

	BOOL oResult = FALSE;

	//Get NetworkId from Command
	if(!oResult && p_pCommand) oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, PORT_NUMBER, (BYTE*)&p_rPortNumber, sizeof(p_rPortNumber), p_pErrorInfo); 
		
	//Get NetworkId from Device
	if(!oResult && p_pManager) oResult = p_pManager->GetParameter(PT_DEVICE, p_hDCS_Handle, PORT_NUMBER, (BYTE*)&p_rPortNumber, sizeof(p_rPortNumber), p_pErrorInfo);

	return oResult;
}

BOOL CGatewayVCStoEsam::GetNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo)
{
	const CStdString NODE_ID = _T("NodeId");

	BOOL oResult = FALSE;

	//Get NetworkId from Command
	if(!oResult && p_pCommand) oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo); 
		
	//Get NetworkId from Device
	if(!oResult && p_pManager) oResult = p_pManager->GetParameter(PT_DEVICE, p_hDCS_Handle, NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

	return oResult;
}

BOOL CGatewayVCStoEsam::RestoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubPortNumber, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
    const DWORD TIMEOUT = 5000;

    //Address Parameter
	BYTE ubPortNumber = 0;
    BYTE ubNodeId = 0;

    DWORD ulPassword = 0;
    DWORD ulTransferRate = 0;
    DWORD ulBackupTimeout = 0;
    BOOL oResult = FALSE;

    if(p_pManager)
    {
        //Init
		oResult = TRUE;

		//Backup Timeout
        p_pManager->DCS_GetProtocolStackSettings(p_hHandle, &ulTransferRate, &ulBackupTimeout);

        //Increase Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, TIMEOUT, TRUE);

        //Write load all parameters
        ulPassword = 'd';
        ulPassword *= 256;
        ulPassword += 'a';
        ulPassword *= 256;
        ulPassword += 'o';
        ulPassword *= 256;
        ulPassword += 'l';

        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubPortNumber, p_ubNodeId, INDEX_RESTORE_DEFAULT_PARAMETERS, SUBINDEX_RESTORE_ALL_DEFAULT_PARAMETERS, ulPassword, p_pErrorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);
    }

    return oResult;
}

