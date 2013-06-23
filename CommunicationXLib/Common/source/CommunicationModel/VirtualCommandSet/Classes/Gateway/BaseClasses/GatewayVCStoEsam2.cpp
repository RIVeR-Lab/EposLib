// GatewayVCStoEsam2.cpp: Implementierung der Klasse CGatewayVCStoEsam2.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayVCStoEsam2.h"


#include <malloc.h>
#include <memory.h>
#include <Process/MmcProcess.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Plc2.h>

#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Esam2.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEsam2Def.h>
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
CGatewayVCStoEsam2::CGatewayVCStoEsam2()
{
    m_pCommand_WriteObject = NULL;
    m_pCommand_InitiateSegmentedWrite = NULL;
    m_pCommand_SegmentedWrite = NULL;
    m_pCommand_ReadObject = NULL;
    m_pCommand_InitiateSegmentedRead = NULL;
    m_pCommand_SegmentedRead = NULL;
    m_pCommand_AbortSegmentedTransfer = NULL;

    m_pCommand_SendCANFrame = NULL;
    m_pCommand_RequestCANFrame = NULL;
    m_pCommand_ReadCANFrame = NULL;
    m_pCommand_SendNMTService = NULL;

    m_pCommand_SendLSSFrame = NULL;
    m_pCommand_ReadLSSFrame = NULL;

    InitCommands_ObjectDictionary();
    InitCommands_GeneralGateway();
    InitCommands_LayerSettingServices();
}

CGatewayVCStoEsam2::~CGatewayVCStoEsam2()
{
    DeleteCommands_ObjectDictionary();
    DeleteCommands_GeneralGateway();
    DeleteCommands_LayerSettingServices();
}

BOOL CGatewayVCStoEsam2::ReadODObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE*& p_rpDataBuffer, DWORD& p_rulNumberOfBytesToRead, DWORD* p_pulNumberOfBytesRead, CErrorInfo* p_pErrorInfo)
{
    const WORD MAX_LENGTH_NON_SEGMENTED_TRANSFER(4);

    BOOL oToggle(FALSE);
    BOOL oLastDataSegment(FALSE);
    BYTE* pubSegmentBuffer(NULL);
    DWORD ulSegmentBufferLength(0);
    DWORD ulSegmentLengthRead(0);
    DWORD ulTotalNumberOfBytesRead(0);
    DWORD ulTotalNumberOfBytesToRead(0);
    DWORD ulNumberOfBytesRead(0);
    CErrorInfo errorInfo;
    DWORD ulAbortCode(0);
	DWORD ulExpeditedDataBuffer = 0;
    BOOL oResult(FALSE);

    if(p_pManager)
    {
        if((p_rulNumberOfBytesToRead > 0) && (p_rulNumberOfBytesToRead <= MAX_LENGTH_NON_SEGMENTED_TRANSFER))
        {
            //Non Segmented Transfer
            if(p_pulNumberOfBytesRead) *p_pulNumberOfBytesRead = 0;

            if(DCS_ReadObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, p_rpDataBuffer, p_rulNumberOfBytesToRead, &ulNumberOfBytesRead, &errorInfo))
            {
                if(p_pulNumberOfBytesRead) *p_pulNumberOfBytesRead = ulNumberOfBytesRead;
                if(p_pErrorInfo) *p_pErrorInfo = errorInfo;
                return TRUE;
            }
            else if(errorInfo.GetErrorCode() == ERROR_DEVICE_ESAM2_SEGMENTED_TRANSFER_REQUIRED_INIT_DONE)
            {
                //Abort in case of segmented transfer indicated by error code
                ulAbortCode = ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED;
                DCS_AbortSegmentedTransfer(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, ulAbortCode, &errorInfo);

                if(m_pErrorHandling) m_pErrorHandling->GetError(ERROR_VIRTUAL_DEVICE_BAD_OBJECT_SIZE_EXPECTED, &errorInfo);
            }

            if(p_pErrorInfo) *p_pErrorInfo = errorInfo;
            return FALSE;
        }
        else
        {
            //Don't allow new segmented Transfer
            if(m_oAbortCommands)
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, p_pErrorInfo);
                return FALSE;
            }

            //Segmented Transfer
            if(p_pulNumberOfBytesRead) *p_pulNumberOfBytesRead = 0;
			if(oResult = DCS_ReadObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&ulExpeditedDataBuffer, sizeof(ulExpeditedDataBuffer), &ulNumberOfBytesRead, &errorInfo))
            {
                //Allocate Memory in Case of unknown Size
				if(p_rpDataBuffer == NULL)
				{
					p_rpDataBuffer = (BYTE*)malloc(ulNumberOfBytesRead);
					p_rulNumberOfBytesToRead = ulNumberOfBytesRead;
				}
				
				//Return Values
				if(p_rpDataBuffer) memcpy(p_rpDataBuffer, &ulExpeditedDataBuffer, ulNumberOfBytesRead);
				if(p_pulNumberOfBytesRead) *p_pulNumberOfBytesRead = ulNumberOfBytesRead;
                if(p_pErrorInfo) *p_pErrorInfo = errorInfo;
            }
            else
            {
                if(errorInfo.GetErrorCode() == ERROR_DEVICE_ESAM2_SEGMENTED_TRANSFER_REQUIRED_INIT_DONE)
                {
                    //Segmented Transfer initialized by ReadObject
                    ulTotalNumberOfBytesToRead = ulExpeditedDataBuffer;
                    oResult = TRUE;
                }
                else
                {
                    //Segmented Transfer initialized by Initiate
                    if(DCS_InitiateSegmentedRead(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, &ulTotalNumberOfBytesToRead, p_pErrorInfo))
					{
						if(ulTotalNumberOfBytesToRead > 0)
						{
							oResult = TRUE;
						}
						else if(p_rulNumberOfBytesToRead > 0)
						{
							ulTotalNumberOfBytesToRead = p_rulNumberOfBytesToRead;
							oResult = TRUE;
						}
					}
                }

                //Allocate Memory in Case of unknown Size
				if(p_rpDataBuffer == NULL)
				{
					p_rpDataBuffer = (BYTE*)malloc(ulTotalNumberOfBytesToRead);
					p_rulNumberOfBytesToRead = ulTotalNumberOfBytesToRead;
				}

				//Limit Number of Bytes to read
                if(ulTotalNumberOfBytesToRead > p_rulNumberOfBytesToRead) ulTotalNumberOfBytesToRead = p_rulNumberOfBytesToRead;

                //Execute Segmented Transfer
                oToggle = FALSE;
                oLastDataSegment = FALSE;
                ulTotalNumberOfBytesRead = 0;
                ulSegmentBufferLength = ulTotalNumberOfBytesToRead;
                pubSegmentBuffer = (BYTE*)p_rpDataBuffer;

                while(oResult && !oLastDataSegment && (ulSegmentBufferLength > 0))
                {
                    //Abort Transfer
                    if(m_oAbortCommands)
                    {
                        ulAbortCode = (DWORD)-1;
                        if(DCS_AbortSegmentedTransfer(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, ulAbortCode))
                        {
                            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, p_pErrorInfo);
                            return FALSE;
                        }
                    }

                    //Do Transfer
                    oResult = DCS_SegmentedRead(p_pManager, p_hDCS_Handle, p_hTransactionHandle, oToggle, &oLastDataSegment, pubSegmentBuffer, ulSegmentBufferLength, &ulSegmentLengthRead, p_pErrorInfo);
                    if(oResult)
                    {
                        //Prepare next segment
                        oToggle = !oToggle;
                        ulTotalNumberOfBytesRead += ulSegmentLengthRead;
                        pubSegmentBuffer += ulSegmentLengthRead;
                        if(ulSegmentLengthRead <= ulSegmentBufferLength) ulSegmentBufferLength -= ulSegmentLengthRead; else ulSegmentBufferLength = 0;
                    }
                }

                if(p_pulNumberOfBytesRead) *p_pulNumberOfBytesRead = ulTotalNumberOfBytesRead;
            }

            return oResult;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return oResult;
}

BOOL CGatewayVCStoEsam2::WriteODObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pDataBuffer, DWORD p_ulNumberOfBytesToWrite, DWORD* p_pulNumberOfBytesWritten, CErrorInfo* p_pErrorInfo)
{
    const WORD MAX_LENGTH_NON_SEGMENTED_TRANSFER(4);
    const WORD MAX_SEGMENT_LENGTH(255);

    BOOL oToggle(FALSE);
    BOOL oLastDataSegment(FALSE);
    DWORD ulTotalNumberOfBytesToWrite;
    DWORD ulTotalNumberOfBytesWritten;
    BYTE* pubSegmentBuffer;
    DWORD ulSegmentLength;
    DWORD ulSegmentLengthWritten;
    BOOL oResult;
    DWORD ulAbortCode;

    if(p_pManager)
    {
        if(p_ulNumberOfBytesToWrite <= MAX_LENGTH_NON_SEGMENTED_TRANSFER)
        {
            //Non Segmented Transfer
            if(p_pulNumberOfBytesWritten) *p_pulNumberOfBytesWritten = 0;
            if(DCS_WriteObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, p_pDataBuffer, p_ulNumberOfBytesToWrite, p_pErrorInfo))
            {
                if(p_pulNumberOfBytesWritten) *p_pulNumberOfBytesWritten = p_ulNumberOfBytesToWrite;
                return TRUE;
            }
            return FALSE;
        }
        else
        {
            //Don't allow new segmented transfer
            if(m_oAbortCommands)
            {
                if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, p_pErrorInfo);
                return FALSE;
            }

            //Segmented Transfer
            if(p_pulNumberOfBytesWritten) *p_pulNumberOfBytesWritten = 0;
            if(DCS_InitiateSegmentedWrite(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, p_ulNumberOfBytesToWrite, p_pErrorInfo))
            {
                //Initialize variables
                oToggle = FALSE;
                oLastDataSegment = FALSE;
                ulTotalNumberOfBytesWritten = 0;
                ulTotalNumberOfBytesToWrite = p_ulNumberOfBytesToWrite;
                pubSegmentBuffer = (BYTE*)p_pDataBuffer;
                if(p_ulNumberOfBytesToWrite < MAX_SEGMENT_LENGTH) ulSegmentLength = p_ulNumberOfBytesToWrite; else ulSegmentLength = MAX_SEGMENT_LENGTH;
                oLastDataSegment = ulSegmentLength == p_ulNumberOfBytesToWrite ? TRUE : FALSE;

                do
                {
                    //Abort Transfer
                    if(m_oAbortCommands)
                    {
                        ulAbortCode = (DWORD)-1;
                        if(DCS_AbortSegmentedTransfer(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, ulAbortCode))
                        {
                            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, p_pErrorInfo);
                            return FALSE;
                        }
                    }

                    //Do Transfer
                    oResult = DCS_SegmentedWrite(p_pManager, p_hDCS_Handle, p_hTransactionHandle, oToggle, oLastDataSegment, pubSegmentBuffer, ulSegmentLength, &ulSegmentLengthWritten, p_pErrorInfo);
                    if(oResult)
                    {
                        //Prepare next segment
                        oToggle = !oToggle;
                        ulTotalNumberOfBytesWritten += ulSegmentLengthWritten;
                        p_ulNumberOfBytesToWrite -= (WORD)ulSegmentLengthWritten;
                        pubSegmentBuffer += ulSegmentLengthWritten;
                        if(p_ulNumberOfBytesToWrite <= ulSegmentLengthWritten)
                        {
                            ulSegmentLength = p_ulNumberOfBytesToWrite;
                            oLastDataSegment = TRUE;
                        }
                        else
                        {
                            ulSegmentLength = ulSegmentLengthWritten;
                            oLastDataSegment = FALSE;
                        }
                        if(ulSegmentLength > MAX_SEGMENT_LENGTH) ulSegmentLength = MAX_SEGMENT_LENGTH;
                    }

                } while(oResult && (ulTotalNumberOfBytesWritten < ulTotalNumberOfBytesToWrite));

                if(p_pulNumberOfBytesWritten) *p_pulNumberOfBytesWritten = ulTotalNumberOfBytesWritten;
                return oResult;
            }

            return FALSE;
        }
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadInt8Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int8* p_pbValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_pbValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_pbValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_pbValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, short* p_psValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_psValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_psValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_psValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, long* p_plValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_plValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_plValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_plValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int64* p_pullValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_pullValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_pullValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_pullValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadUInt8Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pubValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_pubValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_pubValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_pubValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadUInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, WORD* p_pusValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_pusValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_pusValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_pusValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadUInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD* p_pulValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_pulValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_pulValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_pulValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadUInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, unsigned __int64* p_pullValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesToRead(0);
	DWORD ulNumberOfBytesRead(0);

    if(p_pullValue)
    {
        //Init
		ulNumberOfBytesToRead = sizeof(*p_pullValue);

		if(!ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_pullValue, ulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo)) return FALSE;
        if(ulNumberOfBytesToRead != ulNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEsam2::ReadStringObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, CStdString* p_pValue, CErrorInfo* p_pErrorInfo)
{
	const int MAX_STRING_SIZE = 100;

	char* pDataBuffer(0);
	DWORD dNumberOfBytesToRead(0);
	DWORD dNumberOfBytesRead(0);
	BOOL oResult = FALSE;

	if(p_pValue)
	{
		//Read Object
		if(ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (void*&)pDataBuffer, dNumberOfBytesToRead, &dNumberOfBytesRead, p_pErrorInfo))
		{
			*p_pValue = pDataBuffer;
			oResult = TRUE;
		}
		else
		{
			//Allocate Buffer
			pDataBuffer = (char*)malloc(MAX_STRING_SIZE);
			memset(pDataBuffer, 0, MAX_STRING_SIZE);
			dNumberOfBytesToRead = MAX_STRING_SIZE - 1;
			
			//Retry with allocated Buffer
			if(ReadDataBufferObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (void*&)pDataBuffer, dNumberOfBytesToRead, &dNumberOfBytesRead, p_pErrorInfo))
			{
				*p_pValue = pDataBuffer;
				oResult = TRUE;
			}
		}

		//Free Buffer
		if(pDataBuffer) free(pDataBuffer);
	}

	return oResult;
}

BOOL CGatewayVCStoEsam2::ReadDataBufferObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, void*& p_rpData, DWORD& p_rulNumberOfBytesToRead, DWORD* p_pulNumberOfBytesRead, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesRead(0);
    BOOL oResult;

    oResult = ReadODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*&)p_rpData, p_rulNumberOfBytesToRead, &ulNumberOfBytesRead, p_pErrorInfo);

    if(p_pulNumberOfBytesRead) *p_pulNumberOfBytesRead = ulNumberOfBytesRead;
    return oResult;
}

BOOL CGatewayVCStoEsam2::WriteUInt8Object(CDeviceCommandSetManagerBase *p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE p_ubValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, &p_ubValue, sizeof(p_ubValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_ubValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteUInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, WORD p_usValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&p_usValue, sizeof(p_usValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_usValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteUInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&p_ulValue, sizeof(p_ulValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_ulValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteUInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, unsigned __int64 p_ullValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&p_ullValue, sizeof(p_ullValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_ullValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteInt8Object(CDeviceCommandSetManagerBase *p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int8 p_bValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&p_bValue, sizeof(p_bValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_bValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteInt16Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, short p_sValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&p_sValue, sizeof(p_sValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_sValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteInt32Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, long p_lValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&p_lValue, sizeof(p_lValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_lValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteInt64Object(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, __int64 p_ullValue, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);

    if(!WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)&p_ullValue, sizeof(p_ullValue), &ulNumberOfBytesWritten, p_pErrorInfo)) return FALSE;
    if(sizeof(p_ullValue) != ulNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, p_pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEsam2::WriteDataBufferObject(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, void* p_pData, DWORD p_ulNumberOfBytesToWrite, DWORD* p_pulNumberOfBytesWritten, CErrorInfo* p_pErrorInfo)
{
    DWORD ulNumberOfBytesWritten(0);
    BOOL oResult(FALSE);

    if(p_pData)
    {
        oResult = WriteODObject(p_pManager, p_hDCS_Handle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, p_usIndex, p_ubSubIndex, (BYTE*)p_pData, p_ulNumberOfBytesToWrite, &ulNumberOfBytesWritten, p_pErrorInfo);

        if(p_pulNumberOfBytesWritten) *p_pulNumberOfBytesWritten = ulNumberOfBytesWritten;
    }

    return oResult;
}

CGatewayVCStoEsam2& CGatewayVCStoEsam2::operator=(CGatewayVCStoEsam2& other)
{
    if(this != &other)
    {
        *((CGatewayVCStoDCS*)this) = *((CGatewayVCStoDCS*)&other);
    }

    return *this;
}

void CGatewayVCStoEsam2::InitCommands_ObjectDictionary()
{
    DeleteCommands_ObjectDictionary();

    //WriteObject
    m_pCommand_WriteObject = new CCommand_DCS_Esam2();
    m_pCommand_WriteObject->InitCommand(ESAM2_WRITE_OBJECT);

    //ReadObject
    m_pCommand_ReadObject = new CCommand_DCS_Esam2();
    m_pCommand_ReadObject->InitCommand(ESAM2_READ_OBJECT);

    //InitiateSegmentedWrite
    m_pCommand_InitiateSegmentedWrite = new CCommand_DCS_Esam2();
    m_pCommand_InitiateSegmentedWrite->InitCommand(ESAM2_INITIATE_SEGMENT_WRITE);

    //InitiateSegmentedRead
    m_pCommand_InitiateSegmentedRead = new CCommand_DCS_Esam2();
    m_pCommand_InitiateSegmentedRead->InitCommand(ESAM2_INITIATE_SEGMENT_READ);

    //SegmentedWrite
    m_pCommand_SegmentedWrite = new CCommand_DCS_Esam2();
    m_pCommand_SegmentedWrite->InitCommand(ESAM2_SEGMENT_WRITE);

    //SegmentedRead
    m_pCommand_SegmentedRead = new CCommand_DCS_Esam2();
    m_pCommand_SegmentedRead->InitCommand(ESAM2_SEGMENT_READ);

    //AbortSegmentedTransfer
    m_pCommand_AbortSegmentedTransfer = new CCommand_DCS_Esam2();
    m_pCommand_AbortSegmentedTransfer->InitCommand(ESAM2_ABORT_SEGMENT_TRANSFER);
}

void CGatewayVCStoEsam2::InitCommands_GeneralGateway()
{
    DeleteCommands_GeneralGateway();

    //SendCANFrame
    m_pCommand_SendCANFrame = new CCommand_DCS_Esam2();
    m_pCommand_SendCANFrame->InitCommand(ESAM2_SEND_CAN_FRAME);

    //RequestCANFrame
    m_pCommand_RequestCANFrame = new CCommand_DCS_Esam2();
    m_pCommand_RequestCANFrame->InitCommand(ESAM2_REQUEST_CAN_FRAME);

    //ReadCANFrame
    m_pCommand_ReadCANFrame = new CCommand_DCS_Esam2();
    m_pCommand_ReadCANFrame->InitCommand(ESAM2_READ_CAN_FRAME);

    //SendNMTService
    m_pCommand_SendNMTService = new CCommand_DCS_Esam2();
    m_pCommand_SendNMTService->InitCommand(ESAM2_SEND_NMT_SERVICE);
}

void CGatewayVCStoEsam2::InitCommands_LayerSettingServices()
{
    DeleteCommands_LayerSettingServices();

    //SendLSSFrame
    m_pCommand_SendLSSFrame = new CCommand_DCS_Esam2();
    m_pCommand_SendLSSFrame->InitCommand(ESAM2_SEND_LSS_FRAME);

    //ReadLSSFrame
    m_pCommand_ReadLSSFrame = new CCommand_DCS_Esam2();
    m_pCommand_ReadLSSFrame->InitCommand(ESAM2_READ_LSS_FRAME);
}

void CGatewayVCStoEsam2::DeleteCommands_ObjectDictionary()
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

void CGatewayVCStoEsam2::DeleteCommands_GeneralGateway()
{
    //SendCANFrame
    if(m_pCommand_SendCANFrame)
    {
        delete m_pCommand_SendCANFrame;
        m_pCommand_SendCANFrame = NULL;
    }

    //RequestCANFrame
    if(m_pCommand_RequestCANFrame)
    {
        delete m_pCommand_RequestCANFrame;
        m_pCommand_RequestCANFrame = NULL;
    }

    //ReadCANFrame
    if(m_pCommand_ReadCANFrame)
    {
        delete m_pCommand_ReadCANFrame;
        m_pCommand_ReadCANFrame = NULL;
    }

    //SendNMTService
    if(m_pCommand_SendNMTService)
    {
        delete m_pCommand_SendNMTService;
        m_pCommand_SendNMTService = NULL;
    }
}

void CGatewayVCStoEsam2::DeleteCommands_LayerSettingServices()
{
    //SendLSSFrame
    if(m_pCommand_SendLSSFrame)
    {
        delete m_pCommand_SendLSSFrame;
        m_pCommand_SendLSSFrame = NULL;
    }

    //ReadLSSFrame
    if(m_pCommand_ReadLSSFrame)
    {
        delete m_pCommand_ReadLSSFrame;
        m_pCommand_ReadLSSFrame = NULL;
    }
}

BOOL CGatewayVCStoEsam2::GetAddressParameter(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, WORD& p_rNetworkId, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo)
{
    const CStdString NODE_ID           = _T("NodeId");
    const CStdString REMOTE_NETWORK_ID = _T("RemoteNetworkId");
    const CStdString REMOTE_NODE_ID    = _T("RemoteNodeId");

    BOOL oResult(FALSE);

    if(p_pCommand && p_pManager)
    {
        //Get Address Parameter from Command
        if(!oResult && p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, REMOTE_NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo))
        {
            //Remote NetworkId, NodeId
            oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, REMOTE_NETWORK_ID, (BYTE*)&p_rNetworkId, sizeof(p_rNetworkId), p_pErrorInfo);
        }
        else if(!oResult && p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo))
        {
            //Local NetworkId, NodeId
            p_rNetworkId = 0;
            oResult = TRUE;
        }

        //Get Address Parameter from Device
        if(!oResult && p_pManager->GetParameter(PT_DEVICE, p_hDCS_Handle, REMOTE_NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo))
        {
            //Remote NetworkId, NodeId
            oResult = p_pManager->GetParameter(PT_DEVICE, p_hDCS_Handle, REMOTE_NETWORK_ID, (BYTE*)&p_rNetworkId, sizeof(p_rNetworkId), p_pErrorInfo);
        }
        else if(!oResult && p_pManager->GetParameter(PT_DEVICE, p_hDCS_Handle, NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo))
        {
            //Local NetworkId, NodeId
            p_rNetworkId = 0;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::GetNetworkId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, WORD& p_rNetworkId, CErrorInfo* p_pErrorInfo)
{
    const CStdString REMOTE_NETWORK_ID = _T("RemoteNetworkId");

    BOOL oResult(FALSE);

    if(p_pCommand && p_pManager)
    {
        //Get NetworkId from Command
        if(!oResult) oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, REMOTE_NETWORK_ID, (BYTE*)&p_rNetworkId, sizeof(p_rNetworkId), p_pErrorInfo);

        //Get NetworkId from Device
        if(!oResult) oResult = p_pManager->GetParameter(PT_DEVICE, p_hDCS_Handle, REMOTE_NETWORK_ID, (BYTE*)&p_rNetworkId, sizeof(p_rNetworkId), p_pErrorInfo);

        //Get Local NetworkId
        if(!oResult)
        {
            p_rNetworkId = 0;
            oResult = TRUE;
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_InitiateSegmentedRead(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD* p_pulObjectLength, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_InitiateSegmentedRead && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedRead->ResetStatus();
        m_pCommand_InitiateSegmentedRead->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_InitiateSegmentedRead->SetParameterData(1, &p_ubNodeId, sizeof(p_ubNodeId));
        m_pCommand_InitiateSegmentedRead->SetParameterData(2, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(3, &p_ubSubIndex, sizeof(p_ubSubIndex));

        //Execute command
        oResult = p_pManager->ExecuteCommand(m_pCommand_InitiateSegmentedRead, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(1, p_pulObjectLength, sizeof(*p_pulObjectLength));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedRead->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_InitiateSegmentedWrite(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulObjectLength, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_InitiateSegmentedWrite && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedWrite->ResetStatus();
        m_pCommand_InitiateSegmentedWrite->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(1, &p_ubNodeId, sizeof(p_ubNodeId));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(2, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(3, &p_ubSubIndex, sizeof(p_ubSubIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(4, &p_ulObjectLength, sizeof(p_ulObjectLength));

        //Execute command
        oResult = p_pManager->ExecuteCommand(m_pCommand_InitiateSegmentedWrite, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedWrite->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedWrite->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_ReadObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pObjectBuffer, DWORD p_ulObjectBufferLength, DWORD* p_pulObjectLengthRead, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    DWORD ulObjectLength(0);
    BOOL oResult(FALSE);

    if(m_pCommand_ReadObject && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadObject->ResetStatus();
        m_pCommand_ReadObject->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_ReadObject->SetParameterData(1, &p_ubNodeId, sizeof(p_ubNodeId));
        m_pCommand_ReadObject->SetParameterData(2, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_ReadObject->SetParameterData(3, &p_ubSubIndex, sizeof(p_ubSubIndex));

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_ReadObject, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Length
        ulObjectLength = m_pCommand_ReadObject->GetReturnParameterLength(1);
        if(p_ulObjectBufferLength < ulObjectLength) ulObjectLength = p_ulObjectBufferLength;
        if(p_pulObjectLengthRead) *p_pulObjectLengthRead = ulObjectLength;

        //Get ReturnParameter Data
        m_pCommand_ReadObject->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_ReadObject->GetReturnParameterData(1, p_pObjectBuffer, ulObjectLength);

        //Get ErrorCode
        m_pCommand_ReadObject->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_SegmentedRead(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL p_oToggle, BOOL* p_poLastDataSegment, BYTE* p_pSegmentBuffer, DWORD p_ulSegmentBufferLength, DWORD* p_pulSegmentLengthRead, CErrorInfo* p_pErrorInfo)
{
    UEsam2ControlByte controlByte;
    BYTE ubDummyByte(0);

    DWORD ulDeviceErrorCode(0);
    BYTE ubLength(0);
    UEsam2ControlByte retControlByte;

    BOOL oResult(FALSE);

    if(m_pCommand_SegmentedRead && p_pManager)
    {
        //Prepare Parameter
        controlByte.ubValue = 0;
        controlByte.structure.ubToggle = p_oToggle;

        //Set Parameter Data
        m_pCommand_SegmentedRead->ResetStatus();
        m_pCommand_SegmentedRead->SetParameterData(0, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedRead->SetParameterData(1, &ubDummyByte, sizeof(ubDummyByte));

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_SegmentedRead, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedRead->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_SegmentedRead->GetReturnParameterData(1, &ubLength, sizeof(ubLength));
        m_pCommand_SegmentedRead->GetReturnParameterData(2, &retControlByte, sizeof(retControlByte));
        m_pCommand_SegmentedRead->GetReturnParameterData(3, p_pSegmentBuffer, p_ulSegmentBufferLength);

		//Limit Length
		if(ubLength > p_ulSegmentBufferLength) ubLength = (BYTE)p_ulSegmentBufferLength;

        //Get ErrorCode
        m_pCommand_SegmentedRead->GetErrorInfo(p_pErrorInfo);

        //Restore Parameters
        if(p_pulSegmentLengthRead) *p_pulSegmentLengthRead = ubLength;
        if(p_poLastDataSegment) *p_poLastDataSegment = retControlByte.structure.ubLastDataSegment;
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_SegmentedWrite(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL p_oToggle, BOOL p_oMoreSegments, BYTE* p_pSegmentBuffer, DWORD p_ulSegmentLength, DWORD* p_pulSegmentLengthWritten, CErrorInfo* p_pErrorInfo)
{
    BYTE ubLength(0);
    UEsam2ControlByte controlByte;

    DWORD ulDeviceErrorCode(0);
    BYTE ulLengthWritten(0);
    UEsam2ControlByte retControlByte;

    BOOL oResult(FALSE);

    if(m_pCommand_SegmentedWrite && p_pManager)
    {
        //Prepare Parameters
        ubLength = (BYTE)p_ulSegmentLength;
        controlByte.ubValue = 0;
        controlByte.structure.ubToggle = p_oToggle;
        controlByte.structure.ubLastDataSegment = p_oMoreSegments;

        //Set Parameter Data
        m_pCommand_SegmentedWrite->ResetStatus();
        m_pCommand_SegmentedWrite->SetParameterData(0, &ubLength, sizeof(ubLength));
        m_pCommand_SegmentedWrite->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedWrite->SetParameterData(2, p_pSegmentBuffer, p_ulSegmentLength);

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_SegmentedWrite, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedWrite->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_SegmentedWrite->GetReturnParameterData(1, &ulLengthWritten, sizeof(ulLengthWritten));
        m_pCommand_SegmentedWrite->GetReturnParameterData(2, &retControlByte, sizeof(retControlByte));

        //Restore Parameters
        if(p_pulSegmentLengthWritten) *p_pulSegmentLengthWritten = ulLengthWritten;

        //Get ErrorCode
        m_pCommand_SegmentedWrite->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_WriteObject(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, BYTE* p_pObjectBuffer, DWORD p_ulObjectLength, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_WriteObject && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_WriteObject->ResetStatus();
        m_pCommand_WriteObject->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_WriteObject->SetParameterData(1, &p_ubNodeId, sizeof(p_ubNodeId));
        m_pCommand_WriteObject->SetParameterData(2, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_WriteObject->SetParameterData(3, &p_ubSubIndex, sizeof(p_ubSubIndex));
        m_pCommand_WriteObject->SetParameterData(4, p_pObjectBuffer, p_ulObjectLength);

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_WriteObject, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteObject->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_WriteObject->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_AbortSegmentedTransfer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usIndex, BYTE p_ubSubIndex, DWORD p_ulAbortCode, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(m_pCommand_AbortSegmentedTransfer)
    {
        //Set Parameter Data
        m_pCommand_AbortSegmentedTransfer->ResetStatus();
        m_pCommand_AbortSegmentedTransfer->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(1, &p_ubNodeId, sizeof(p_ubNodeId));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(2, &p_usIndex, sizeof(p_usIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(3, &p_ubSubIndex, sizeof(p_ubSubIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(4, &p_ulAbortCode, sizeof(p_ulAbortCode));

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_AbortSegmentedTransfer, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSegmentedTransfer->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_SendCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_SendCANFrame && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_SendCANFrame->SetParameterData(1, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_SendCANFrame->SetParameterData(2, &p_usLength, sizeof(p_usLength));
        m_pCommand_SendCANFrame->SetParameterData(3, p_pData, p_usLength);

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_SendCANFrame, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendCANFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_ReadCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usCobID, WORD p_usLength, void* p_pData, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_ReadCANFrame && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadCANFrame->ResetStatus();
        m_pCommand_ReadCANFrame->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_ReadCANFrame->SetParameterData(1, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_ReadCANFrame->SetParameterData(2, &p_usLength, sizeof(p_usLength));
        m_pCommand_ReadCANFrame->SetParameterData(3, &p_ulTimeout, sizeof(p_ulTimeout));

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_ReadCANFrame, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_ReadCANFrame->GetReturnParameterData(1, p_pData, p_usLength);

        //Get ErrorCode
        m_pCommand_ReadCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_RequestCANFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_RequestCANFrame && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_RequestCANFrame->SetParameterData(1, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_RequestCANFrame->SetParameterData(2, &p_usLength, sizeof(p_usLength));

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_RequestCANFrame, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_RequestCANFrame->GetReturnParameterData(1, p_pData, p_usLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_SendNMTService(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, WORD p_usCommandSpecifier, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);
    DWORD ulDeviceErrorCode(0);

    if(m_pCommand_SendNMTService && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_SendNMTService->SetParameterData(1, &p_ubNodeId, sizeof(p_ubNodeId));
        m_pCommand_SendNMTService->SetParameterData(2, &p_usCommandSpecifier, sizeof(p_usCommandSpecifier));

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_SendNMTService, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendNMTService->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_SendLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_SendLSSFrame && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_SendLSSFrame->SetParameterData(1, p_pDataBuffer, p_ulDataBufferLength);

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_SendLSSFrame, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendLSSFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::DCS_ReadLSSFrame(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, WORD p_usTimeout, void* p_pDataBuffer, DWORD p_ulDataBufferLength, CErrorInfo* p_pErrorInfo)
{
    DWORD ulDeviceErrorCode(0);
    BOOL oResult(FALSE);

    if(m_pCommand_ReadLSSFrame && p_pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &p_usNetworkId, sizeof(p_usNetworkId));
        m_pCommand_ReadLSSFrame->SetParameterData(1, &p_usTimeout, sizeof(p_usTimeout));

        //Execute Command
        oResult = p_pManager->ExecuteCommand(m_pCommand_ReadLSSFrame, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_ReadLSSFrame->GetReturnParameterData(1, p_pDataBuffer, p_ulDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::InitLayerParameterStack(CCommandRoot* p_pCommand)
{
    CLayerParameterStack layerParameterStack;
    CLayerParameterSet layerParameterSet;
    BOOL oResult(FALSE);

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

BOOL CGatewayVCStoEsam2::GetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD& p_rulTimeout)
{
    CStdString protocolStackName(_T(""));
    CStdString interfaceName(_T(""));
    WORD usTimeout(0);
    BOOL oResult(FALSE);

    if(p_pManager)
    {
        if(p_pManager->DCS_GetProtocolStackName(p_hHandle, &protocolStackName) && p_pManager->DCS_GetInterfaceName(p_hHandle, &interfaceName))
        {
            if(protocolStackName == PROTOCOL_CAN_OPEN)
            {
                //CAN Frame Timeout
                oResult = ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_CAN_FRAME_TIMEOUT, SUBINDEX_CAN_FRAME_TIMEOUT, &usTimeout);
            }
            else if(protocolStackName == PROTOCOL_MAXON_SERIAL_V2)
            {
                if(interfaceName == INTERFACE_RS232)
                {
                    //RS232 Frame Timeout
                       oResult = ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_RS232_FRAME_TIMEOUT, SUBINDEX_RS232_FRAME_TIMEOUT, &usTimeout);
                }
                else if(interfaceName == INTERFACE_USB)
                {
                    //USB Frame Timeout
                    oResult = ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_USB_FRAME_TIMEOUT, SUBINDEX_USB_FRAME_TIMEOUT, &usTimeout);
                }
            }

            //Result
            if(oResult) p_rulTimeout = usTimeout;
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::SetDeviceTimeout(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD p_ulTimeout)
{
    CStdString protocolStackName(_T(""));
    CStdString interfaceName(_T(""));
    WORD usTimeout(0);
    BOOL oResult(FALSE);

    if(p_pManager)
    {
        if(p_pManager->DCS_GetProtocolStackName(p_hHandle, &protocolStackName) && p_pManager->DCS_GetInterfaceName(p_hHandle, &interfaceName))
        {
            //Init
            usTimeout = (WORD)p_ulTimeout;

            if(protocolStackName == PROTOCOL_CAN_OPEN)
            {
                //CAN Frame Timeout
                oResult = WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_CAN_FRAME_TIMEOUT, SUBINDEX_CAN_FRAME_TIMEOUT, usTimeout);
            }
            else if(protocolStackName == PROTOCOL_MAXON_SERIAL_V2)
            {
                if(interfaceName == INTERFACE_RS232)
                {
                    //RS232 Frame Timeout
                       oResult = WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_RS232_FRAME_TIMEOUT, SUBINDEX_RS232_FRAME_TIMEOUT, usTimeout);
                }
                else if(interfaceName == INTERFACE_USB)
                {
                    //USB Frame Timeout
                    oResult = WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_USB_FRAME_TIMEOUT, SUBINDEX_USB_FRAME_TIMEOUT, usTimeout);
                }
            }
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::GetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD& p_rulTransferRate, CErrorInfo* p_pErrorInfo)
{
    CStdString protocolStackName(_T(""));
    CStdString interfaceName(_T(""));
    WORD usTransferRateIndex(0);
    BOOL oResult(FALSE);

    if(p_pManager)
    {
        if(p_pManager->DCS_GetProtocolStackName(p_hHandle, &protocolStackName) && p_pManager->DCS_GetInterfaceName(p_hHandle, &interfaceName))
        {
            if(protocolStackName == PROTOCOL_CAN_OPEN)
            {
                //CAN Bitrate
                if(ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_CAN_BITRATE, SUBINDEX_CAN_BITRATE, &usTransferRateIndex, p_pErrorInfo))
				{
					p_rulTransferRate = ConvertBitrateIndexToValue_CiaStandard(usTransferRateIndex);
					oResult = TRUE;
				}
            }
            else if(protocolStackName == PROTOCOL_MAXON_SERIAL_V2)
            {
                if(interfaceName == INTERFACE_RS232)
                {
                    //RS232 Baudrate
                    if(ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_BAUDRATE, SUBINDEX_BAUDRATE, &usTransferRateIndex, p_pErrorInfo))
					{
						p_rulTransferRate = ConvertBaudrateIndexToValue(usTransferRateIndex);
						oResult = TRUE;
					}
                }
                else if(interfaceName == INTERFACE_USB)
                {
                    //No USB TransferRate
                    oResult = FALSE;
                }
            }
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::SetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD p_ulTransferRate, CErrorInfo* p_pErrorInfo)
{
    CStdString protocolStackName(_T(""));
    CStdString interfaceName(_T(""));
    WORD usTransferRateIndex(0);
    BOOL oResult(FALSE);

    if(p_pManager)
    {
        if(p_pManager->DCS_GetProtocolStackName(p_hHandle, &protocolStackName) && p_pManager->DCS_GetInterfaceName(p_hHandle, &interfaceName))
        {
            if(protocolStackName == PROTOCOL_CAN_OPEN)
            {
                //CAN Frame Timeout
				usTransferRateIndex = ConvertBitrateValueToIndex_CiaStandard(p_ulTransferRate);
                oResult = WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_CAN_BITRATE, SUBINDEX_CAN_BITRATE, usTransferRateIndex, p_pErrorInfo);
            }
            else if(protocolStackName == PROTOCOL_MAXON_SERIAL_V2)
            {
                if(interfaceName == INTERFACE_RS232)
                {
                    //RS232 Frame Timeout
					usTransferRateIndex = ConvertBaudrateValueToIndex(p_ulTransferRate);
                    oResult = WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_BAUDRATE, SUBINDEX_BAUDRATE, usTransferRateIndex, p_pErrorInfo);
                }
                else if(interfaceName == INTERFACE_USB)
                {
                    //No USB TransferRate
                    oResult = FALSE;
                }
            }
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::StoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
    const DWORD TIMEOUT(5000);

    DWORD ulPassword(0);
    DWORD ulTransferRate(0);
    DWORD ulBackupTimeout(0);
    DWORD ulBackupDeviceTimeout(0);

    BOOL oResult(FALSE);

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

        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_STORE_PARAMETERS, SUBINDEX_SAVE_ALL_PARAMETERS, ulPassword, p_pErrorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::ResetDevice(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, BOOL p_oWaitForReboot, CErrorInfo* p_pErrorInfo)
{
    const DWORD TIMEOUT(100);
	const WORD COMMAND_SPECIFIER_RESET_NODE(0x81);

	DWORD ulTransferRate(0);
    DWORD ulBackupTimeout(0);
    BOOL oResult(FALSE);

	if(p_pManager)
	{
		//Backup Timeout
		p_pManager->DCS_GetProtocolStackSettings(p_hHandle, &ulTransferRate, &ulBackupTimeout);

		//Decrease Timeout
		p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, TIMEOUT, TRUE);

		//SendNMTService
		oResult = DCS_SendNMTService(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, (WORD)NSI_RESET_NODE, p_pErrorInfo);

		//Wait before changing Transfer Rate
		MmcSleep(TIMEOUT);

		//Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);

		if(oResult && p_oWaitForReboot)
		{
			//WaitForReboot
			WaitForReboot(p_pManager, p_hHandle, 0, p_usNetworkId, p_ubNodeId, 0);
		}
	}

    return oResult;
}


BOOL CGatewayVCStoEsam2::WaitForTermination(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, DWORD p_ulTimeout, CErrorInfo* p_pErrorInfo)
{
    WORD wIndex = 0x1001; //ErrorRegister
    BYTE uSubIndex = 0x00;
    BYTE uErrorRegister;
    BOOL oResult(FALSE);

    DWORD dStartTime = 0;
    DWORD dTime = 0;
    DWORD dSleepTime = 500;


    dStartTime = MmcGetTickCount();

    dTime = 0;

    while(dTime < p_ulTimeout)
    {

        MmcSleep(dSleepTime);

		if(ReadUInt8Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, wIndex, uSubIndex, &uErrorRegister, p_pErrorInfo))
        {

			MmcSleep(dSleepTime);

            return TRUE;
        }

        dTime = MmcGetTickCount()-dStartTime;

    }

    return FALSE;
}

DWORD CGatewayVCStoEsam2::ConvertBaudrateIndexToValue(WORD wBaudrateIndex)
{
	switch(wBaudrateIndex)
	{
		case 0: return 9600;
		case 1: return 14400;
		case 2: return 19200;
		case 3: return 38400;
		case 4: return 57600;
		case 5: return 115200;
		default: return 115200;
	}
}

WORD CGatewayVCStoEsam2::ConvertBaudrateValueToIndex(DWORD dBaudrate)
{
	switch(dBaudrate)
	{
		case 9600: return 0;
		case 14400: return 1;
		case 19200: return 2;
		case 38400: return 3;
		case 57600: return 4;
		case 115200: return 5;
		default: return 0;
	}
}

DWORD CGatewayVCStoEsam2::ConvertBitrateIndexToValue_CiaStandard(WORD wBaudrateIndex)
{
	switch(wBaudrateIndex)
	{
		case 0: return 1000000;
		case 1: return 800000;
		case 2: return 500000;
		case 3: return 250000;
		case 4: return 125000;
		case 5: return 0;
        case 6: return 50000;
		case 7: return 20000;
		case 9: return TRANSFER_RATE_AUTO;
		default: return 1000000;
	}
}

WORD CGatewayVCStoEsam2::ConvertBitrateValueToIndex_CiaStandard(DWORD dBaudrate)
{
	switch(dBaudrate)
	{
		case 1000000: return 0 ;
		case 800000: return 1 ;
		case 500000: return 2;
		case 250000: return 3;
		case 125000: return 4;
		//case Reserved: return 5;
        case 50000: return 6;
		case 20000: return 7;
		case TRANSFER_RATE_AUTO: return 9;
		default: return 0;
	}
}

BOOL CGatewayVCStoEsam2::RestoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
    const DWORD TIMEOUT(5000);

    DWORD ulPassword(0);
    DWORD ulTransferRate(0);
    DWORD ulBackupTimeout(0);
    DWORD ulBackupDeviceTimeout(0);

    BOOL oResult(FALSE);

    if(p_pManager)
    {
        //Init
        oResult = TRUE;

        //Backup Timeout
        p_pManager->DCS_GetProtocolStackSettings(p_hHandle, &ulTransferRate, &ulBackupTimeout);
        GetDeviceTimeout(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, ulBackupDeviceTimeout);

        //Increase Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, TIMEOUT, TRUE);
        SetDeviceTimeout(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, TIMEOUT);

        //Write load all parameters
        ulPassword = 'd';
        ulPassword *= 256;
        ulPassword += 'a';
        ulPassword *= 256;
        ulPassword += 'o';
        ulPassword *= 256;
        ulPassword += 'l';

        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_RESTORE_DEFAULT_PARAMETERS, SUBINDEX_RESTORE_ALL_DEFAULT_PARAMETERS, ulPassword, p_pErrorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);
        SetDeviceTimeout(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, ulBackupDeviceTimeout);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::RestoreFactoryParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
    const DWORD TIMEOUT(5000);

    DWORD ulPassword(0);
    DWORD ulTransferRate(0);
    DWORD ulBackupTimeout(0);
    DWORD ulBackupDeviceTimeout(0);

    BOOL oResult(FALSE);

    if(p_pManager)
    {
        //Init
        oResult = TRUE;

        //Backup Timeout
        p_pManager->DCS_GetProtocolStackSettings(p_hHandle, &ulTransferRate, &ulBackupTimeout);
        GetDeviceTimeout(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, ulBackupDeviceTimeout);

        //Increase Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, TIMEOUT, TRUE);
        SetDeviceTimeout(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, TIMEOUT);

        //Write load all parameters
        ulPassword = 'd';
        ulPassword *= 256;
        ulPassword += 'a';
        ulPassword *= 256;
        ulPassword += 'o';
        ulPassword *= 256;
        ulPassword += 'l';

        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_RESTORE_DEFAULT_PARAMETERS, SUBINDEX_RESTORE_FACTORY_DEFAULT_PARAMETERS, ulPassword, p_pErrorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);
    }

    return oResult;
}

BOOL CGatewayVCStoEsam2::WaitForReboot(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNetworkId, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
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
		if(ReadUInt8Object(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, INDEX_ERROR_REGISTER, SUBINDEX_ERROR_REGISTER, &uErrorRegister, p_pErrorInfo))
        {
            oResult =  TRUE;
			break;
        }
		else
		{
			//Force Autobitrate Detection
			DCS_SendNMTService(p_pManager, p_hHandle, p_hTransactionHandle, p_usNetworkId, p_ubNodeId, (WORD)NSI_ENTER_PRE_OPERATIONAL);

			//Update Time
			dTime = MmcGetTickCount()-dStartTime;
			MmcSleep(dSleepTime);
		}
    }

    return oResult;
}

