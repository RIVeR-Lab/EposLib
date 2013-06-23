// GatewayVCStoEpos.cpp: Implementierung der Klasse CGatewayVCStoEpos.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GatewayVCStoEpos.h"

#include <memory.h>
#include <Process/MmcProcess.h>

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/BaseClasses/Command_VCS.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

#include "../../../../DeviceCommandSet/BaseClasses/DeviceCommandSetManagerBase.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/Command_DCS_Epos.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEposDef.h>
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
CGatewayVCStoEpos::CGatewayVCStoEpos()
{
    m_eCommandSetVersion = EposCommandSetVer2;

    m_pCommand_WriteODObject = NULL;
    m_pCommand_WriteODObjectNextSegment = NULL;
    m_pCommand_WriteObject = NULL;
    m_pCommand_InitiateSegmentedWrite = NULL;
    m_pCommand_SegmentedWrite = NULL;
    m_pCommand_ReadODObject = NULL;
    m_pCommand_ReadODObjectNextSegment = NULL;
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

CGatewayVCStoEpos::~CGatewayVCStoEpos()
{
    DeleteCommands_ObjectDictionary();
    DeleteCommands_NetworkManagement();
    DeleteCommands_GeneralGateway();
    DeleteCommands_LayerSettingServices();
}

BOOL CGatewayVCStoEpos::ReadODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dMaxNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    BOOL oFirstTryResult = FALSE;
    CErrorInfo firstTryErrorInfo;
    BOOL oSecondTryResult = FALSE;
    CErrorInfo secondTryErrorInfo;

    switch(m_eCommandSetVersion)
    {
        case EposCommandSetVer1: oFirstTryResult = ReadODObjectVer1(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, pDataBuffer, dMaxNumberOfBytesToRead, pdNumberOfBytesRead, &firstTryErrorInfo);break;
        case EposCommandSetVer2: oFirstTryResult = ReadODObjectVer2(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, pDataBuffer, dMaxNumberOfBytesToRead, pdNumberOfBytesRead, &firstTryErrorInfo);break;
    }

    //ReDetect CommandSetVersion; No Journal Protocol
    if(!oFirstTryResult && !m_oAbortCommands)
    {
        switch(m_eCommandSetVersion)
        {
            case EposCommandSetVer2: oSecondTryResult = ReadODObjectVer1(pManager, hDCS_Handle, NULL, wNodeId, wIndex, wSubIndex, pDataBuffer, dMaxNumberOfBytesToRead, pdNumberOfBytesRead, &secondTryErrorInfo);if(oSecondTryResult) m_eCommandSetVersion = EposCommandSetVer1;break;
            case EposCommandSetVer1: oSecondTryResult = ReadODObjectVer2(pManager, hDCS_Handle, NULL, wNodeId, wIndex, wSubIndex, pDataBuffer, dMaxNumberOfBytesToRead, pdNumberOfBytesRead, &secondTryErrorInfo);if(oSecondTryResult) m_eCommandSetVersion = EposCommandSetVer2;break;
        }
    }

    if(!oFirstTryResult && oSecondTryResult)
    {
        if(pErrorInfo) *pErrorInfo = secondTryErrorInfo;
        return oSecondTryResult;
    }
    else
    {
        if(pErrorInfo) *pErrorInfo = firstTryErrorInfo;
        return oFirstTryResult;
    }
}

BOOL CGatewayVCStoEpos::ReadODObjectVer1(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dMaxNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    const WORD k_EndOfSegmentedRead = 0x00;

    BYTE* pSegmentDataBuffer;
    WORD wSegmentMaxNumberOfBytesToRead;
    WORD wSegmentNumberOfBytesRead;
    WORD wTotalNumberOfBytesRead;
    WORD wSegmentedRead;
    BOOL oResult;
    WORD wMaxNumberOfBytesToRead;
    DWORD dAbortCode;

    if(pManager)
    {
        //Limit dMaxNumberOfBytesToRead
        if(dMaxNumberOfBytesToRead > USHRT_MAX) wMaxNumberOfBytesToRead = USHRT_MAX; else wMaxNumberOfBytesToRead = (WORD)dMaxNumberOfBytesToRead;

        //Prepare First Segment
        pSegmentDataBuffer = pDataBuffer;
        wSegmentMaxNumberOfBytesToRead = wMaxNumberOfBytesToRead;

        //Read First Segment
        oResult = DCS_ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, pSegmentDataBuffer, wSegmentMaxNumberOfBytesToRead, &wSegmentNumberOfBytesRead, &wSegmentedRead, pErrorInfo);

        //Update Total Number of Words Read
        wTotalNumberOfBytesRead = wSegmentNumberOfBytesRead;

        while(oResult && (wSegmentedRead != k_EndOfSegmentedRead))
        {
            //Abort Transfer
            if(m_oAbortCommands)
            {
                dAbortCode = (DWORD)-1;
                if(DCS_AbortSegmentedTransfer(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, dAbortCode))
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                    return FALSE;
                }
            }

            //Prepare Next Segment Buffer
            if(wSegmentMaxNumberOfBytesToRead >= wSegmentNumberOfBytesRead)
            {
                wSegmentMaxNumberOfBytesToRead = wSegmentMaxNumberOfBytesToRead - wSegmentNumberOfBytesRead;
                pSegmentDataBuffer += wSegmentNumberOfBytesRead;
            }
            else
            {
                //Too Small Data Buffer
                if(pdNumberOfBytesRead) *pdNumberOfBytesRead = wTotalNumberOfBytesRead;
                return TRUE;
            }

            //Read Next Segment Buffer
            oResult = DCS_ReadODObjectNextSegment(pManager, hDCS_Handle, hTransactionHandle, wNodeId, pSegmentDataBuffer, wSegmentMaxNumberOfBytesToRead, &wSegmentNumberOfBytesRead, &wSegmentedRead, pErrorInfo);

            //Update Total Number of Words Read
            wTotalNumberOfBytesRead = wTotalNumberOfBytesRead + wSegmentNumberOfBytesRead;
        }

        if(pdNumberOfBytesRead) *pdNumberOfBytesRead = (DWORD)wTotalNumberOfBytesRead;
        return oResult;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEpos::ReadODObjectVer2(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dMaxNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    const WORD k_MaxLength_NonSegmentedTransfer = 4;

    BYTE subIndex = (BYTE)wSubIndex;

    BOOL oResult;
    BOOL bToggle;
    BOOL bMoreSegments;
    BYTE* pSegmentBuffer;
    DWORD segmentBufferLength;
    DWORD segmentLengthRead;
    DWORD dTotalNumberOfBytesRead;
    DWORD dAbortCode;

    if(pManager)
    {
        if(dMaxNumberOfBytesToRead <= k_MaxLength_NonSegmentedTransfer)
        {
            //Non Segmented Transfer
            if(pdNumberOfBytesRead) *pdNumberOfBytesRead = 0;

            if(DCS_ReadObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, subIndex, pDataBuffer, dMaxNumberOfBytesToRead, pErrorInfo))
            {
                if(pdNumberOfBytesRead) *pdNumberOfBytesRead = dMaxNumberOfBytesToRead;
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
            if(DCS_InitiateSegmentedRead(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, subIndex, pErrorInfo))
            {
                //Initialize variables
                dTotalNumberOfBytesRead = 0;
                bToggle = FALSE;
                bMoreSegments = TRUE;
                segmentBufferLength = dMaxNumberOfBytesToRead;
                pSegmentBuffer = (BYTE*)pDataBuffer;

                oResult = TRUE;
                while(oResult && bMoreSegments && (segmentBufferLength > 0))
                {
                    //Abort Transfer
                    if(m_oAbortCommands)
                    {
                        dAbortCode = (DWORD)-1;
                        if(DCS_AbortSegmentedTransfer(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, subIndex, dAbortCode))
                        {
                            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                            return FALSE;
                        }
                    }

                    //Do Transfer
                    oResult = DCS_SegmentedRead(pManager, hDCS_Handle, hTransactionHandle, wNodeId, bToggle, &bMoreSegments, pSegmentBuffer, segmentBufferLength, &segmentLengthRead, pErrorInfo);
                    if(oResult)
                    {
                        //Prepare next segment
                        dTotalNumberOfBytesRead += segmentLengthRead;
                        bToggle = !bToggle;
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

BOOL CGatewayVCStoEpos::WriteODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dNumberOfBytesToWrite, BOOL oSignedDataType, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    BOOL oFirstTryResult = FALSE;
    CErrorInfo firstTryErrorInfo;
    BOOL oSecondTryResult = FALSE;
    CErrorInfo secondTryErrorInfo;

    switch(m_eCommandSetVersion)
    {
        case EposCommandSetVer1: oFirstTryResult = WriteODObjectVer1(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, pDataBuffer, dNumberOfBytesToWrite, oSignedDataType, pdNumberOfBytesWritten, &firstTryErrorInfo);break;
        case EposCommandSetVer2: oFirstTryResult = WriteODObjectVer2(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, pDataBuffer, dNumberOfBytesToWrite, pdNumberOfBytesWritten, &firstTryErrorInfo);break;
    }

    if(!oFirstTryResult && !m_oAbortCommands)
    {
        //ReDetect CommandSetVersion; No Journal Protocol
        switch(m_eCommandSetVersion)
        {
            case EposCommandSetVer2: oSecondTryResult = WriteODObjectVer1(pManager, hDCS_Handle, NULL, wNodeId, wIndex, wSubIndex, pDataBuffer, dNumberOfBytesToWrite, oSignedDataType, pdNumberOfBytesWritten, &secondTryErrorInfo);if(oSecondTryResult) m_eCommandSetVersion = EposCommandSetVer1;break;
            case EposCommandSetVer1: oSecondTryResult = WriteODObjectVer2(pManager, hDCS_Handle, NULL, wNodeId, wIndex, wSubIndex, pDataBuffer, dNumberOfBytesToWrite, pdNumberOfBytesWritten, &secondTryErrorInfo);if(oSecondTryResult) m_eCommandSetVersion = EposCommandSetVer2;break;
        }
    }

    if(!oFirstTryResult && oSecondTryResult)
    {
        if(pErrorInfo) *pErrorInfo = secondTryErrorInfo;
        return oSecondTryResult;
    }
    else
    {
        if(pErrorInfo) *pErrorInfo = firstTryErrorInfo;
        return oFirstTryResult;
    }
}

BOOL CGatewayVCStoEpos::WriteODObjectVer1(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dNumberOfBytesToWrite, BOOL oSignedDataType, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    const WORD k_EndOfSegmentedWrite = 0x00;

    BYTE* pSegmentDataBuffer;
    WORD wTotalNumberOfBytesToWrite;
    WORD wTotalNumberOfBytesWritten;
    WORD wSegmentNumberOfBytesToWrite;
    WORD wSegmentNumberOfBytesWritten;
    WORD wSegmentedWrite;
    BOOL oResult;
    WORD wNumberOfBytesToWrite;
    DWORD dAbortCode;

    if(pManager)
    {
        //Limit dMaxNumberOfBytesToWrite
        if(dNumberOfBytesToWrite > USHRT_MAX) wNumberOfBytesToWrite = USHRT_MAX; else wNumberOfBytesToWrite = (WORD)dNumberOfBytesToWrite;

        //Prepare First Segment
        pSegmentDataBuffer = pDataBuffer;
        wTotalNumberOfBytesToWrite = wNumberOfBytesToWrite;
        wSegmentNumberOfBytesToWrite = wNumberOfBytesToWrite;

        //Write First Segment
        oResult = DCS_WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, pSegmentDataBuffer, wSegmentNumberOfBytesToWrite, oSignedDataType, &wSegmentNumberOfBytesWritten, &wSegmentedWrite, pErrorInfo);

        //Update Total Number of Words Written
        wTotalNumberOfBytesWritten = wSegmentNumberOfBytesWritten;

        while(oResult && (wSegmentedWrite != k_EndOfSegmentedWrite))
        {
            //Abort Transfer
            if(m_oAbortCommands)
            {
                dAbortCode = (DWORD)-1;
                if(DCS_AbortSegmentedTransfer(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, dAbortCode))
                {
                    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                    return FALSE;
                }
            }

            //Prepare Next Segment Write
            if(wTotalNumberOfBytesToWrite >= wSegmentNumberOfBytesWritten)
            {
                wTotalNumberOfBytesToWrite = wTotalNumberOfBytesToWrite - wSegmentNumberOfBytesWritten;
                pSegmentDataBuffer += wSegmentNumberOfBytesWritten;
            }
            else
            {
                //No more data in dataBuffer to write
                if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = wTotalNumberOfBytesWritten;
                return TRUE;
            }

            //Limit Number of Words to last segment size
            if(wTotalNumberOfBytesToWrite > wSegmentNumberOfBytesWritten)
            {
                wSegmentNumberOfBytesToWrite = wSegmentNumberOfBytesWritten;
            }
            else
            {
                wSegmentNumberOfBytesToWrite = wTotalNumberOfBytesToWrite;
            }

            //Write Next Segment Buffer
            oResult = DCS_WriteODObjectNextSegment(pManager, hDCS_Handle, hTransactionHandle, wNodeId, pSegmentDataBuffer, wSegmentNumberOfBytesToWrite, oSignedDataType, &wSegmentNumberOfBytesWritten, &wSegmentedWrite, pErrorInfo);

            //Add number of Words Written
            wTotalNumberOfBytesWritten = wTotalNumberOfBytesWritten + wSegmentNumberOfBytesWritten;
        }

        if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = wTotalNumberOfBytesWritten;
        return oResult;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEpos::WriteODObjectVer2(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    const WORD k_MaxLength_NonSegmentedTransfer = 4;
    const WORD k_MaxSegmentLength = 59;  //63

    BYTE subIndex = (BYTE)wSubIndex;

    DWORD lTotalNumberOfBytesToWrite;
    DWORD lTotalNumberOfBytesWritten;
    BOOL bToggle;
    BOOL bMoreSegments;
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
            if(DCS_WriteObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, subIndex, pDataBuffer, dNumberOfBytesToWrite, pErrorInfo))
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
            if(DCS_InitiateSegmentedWrite(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, subIndex, dNumberOfBytesToWrite, pErrorInfo))
            {
                //Initialize variables
                lTotalNumberOfBytesWritten = 0;
                lTotalNumberOfBytesToWrite = dNumberOfBytesToWrite;
                bToggle = FALSE;
                bMoreSegments = TRUE;
                pSegmentBuffer = (BYTE*)pDataBuffer;
                if(dNumberOfBytesToWrite < k_MaxSegmentLength) lSegmentLength = dNumberOfBytesToWrite; else lSegmentLength = k_MaxSegmentLength;
                if(lSegmentLength > 0) bMoreSegments = TRUE;

                do
                {
                    //Abort Transfer
                    if(m_oAbortCommands)
                    {
                        dAbortCode = (DWORD)-1;
                        if(DCS_AbortSegmentedTransfer(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, dAbortCode))
                        {
                            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_CommandAbortedByUser, pErrorInfo);
                            return FALSE;
                        }
                    }

                    //Do Transfer
                    oResult = DCS_SegmentedWrite(pManager, hDCS_Handle, hTransactionHandle, wNodeId, bToggle, bMoreSegments, pSegmentBuffer, lSegmentLength, &lSegmentLengthWritten, pErrorInfo);
                    if(oResult)
                    {
                        //Prepare next segment
                        lTotalNumberOfBytesWritten += lSegmentLengthWritten;
                        dNumberOfBytesToWrite -= (WORD)lSegmentLengthWritten;
                        bToggle = !bToggle;
                        pSegmentBuffer += lSegmentLengthWritten;
                        if(dNumberOfBytesToWrite < lSegmentLengthWritten) lSegmentLength = dNumberOfBytesToWrite; else lSegmentLength = lSegmentLengthWritten;
                        if(lSegmentLength > k_MaxSegmentLength) lSegmentLength = k_MaxSegmentLength;
                        bMoreSegments = (dNumberOfBytesToWrite > lSegmentLength);
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

BOOL CGatewayVCStoEpos::ReadInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, __int8* pbValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(pbValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)pbValue, sizeof(*pbValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
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

BOOL CGatewayVCStoEpos::ReadInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, short* piValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(piValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)piValue, sizeof(*piValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
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

BOOL CGatewayVCStoEpos::ReadInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, long* plValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(plValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)plValue, sizeof(*plValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
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

BOOL CGatewayVCStoEpos::ReadUInt8Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* puValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(puValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)puValue, sizeof(*puValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
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

BOOL CGatewayVCStoEpos::ReadUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, WORD* pwValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(pwValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)pwValue, sizeof(*pwValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
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

BOOL CGatewayVCStoEpos::ReadUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD* pdValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(pdValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)pdValue, sizeof(*pdValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
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

BOOL CGatewayVCStoEpos::ReadUInt64Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, __int64* pullValue, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;

    if(pullValue)
    {
        if(!ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)pullValue, sizeof(*pullValue), &dNumberOfBytesRead, pErrorInfo)) return FALSE;
        if(sizeof(*pullValue) != dNumberOfBytesRead)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
            return FALSE;
        }

        return TRUE;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEpos::ReadStringObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, CStdString* pValue, CErrorInfo* pErrorInfo)
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
		if((ReadDataBufferObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (void*)nonExpeditedDataBuffer, NON_EXPEDITED_DATA_BUFFER_SIZE, &dNumberOfBytesRead, pErrorInfo) && (NON_EXPEDITED_DATA_BUFFER_SIZE >= dNumberOfBytesRead)))
		{
			*pValue = nonExpeditedDataBuffer;
			oResult = TRUE;
		}
		else if((ReadDataBufferObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (void*)expeditedDataBuffer, EXPEDITED_DATA_BUFFER_SIZE, &dNumberOfBytesRead, pErrorInfo) && (EXPEDITED_DATA_BUFFER_SIZE >= dNumberOfBytesRead)))
		{
			*pValue = expeditedDataBuffer;
			oResult = TRUE;
		}
	}

	return oResult;
}

BOOL CGatewayVCStoEpos::ReadDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, void* p_pData, DWORD dNumberOfBytesToRead, DWORD* pdNumberOfBytesRead, CErrorInfo* pErrorInfo)
{
    DWORD dNumberOfBytesRead;
    BOOL oResult;

    if(p_pData)
    {
        oResult = ReadODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)p_pData, dNumberOfBytesToRead, &dNumberOfBytesRead, pErrorInfo);

        if(pdNumberOfBytesRead) *pdNumberOfBytesRead = dNumberOfBytesRead;
        return oResult;
    }

    if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
    return FALSE;
}

BOOL CGatewayVCStoEpos::WriteUInt8Object(CDeviceCommandSetManagerBase *pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE uValue, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = FALSE;
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, &uValue, sizeof(uValue), oSignedDataType, &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(uValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEpos::WriteUInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, WORD wValue, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = FALSE;
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)&wValue, sizeof(wValue), oSignedDataType, &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(wValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEpos::WriteUInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dValue, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = FALSE;
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)&dValue, sizeof(dValue), oSignedDataType, &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(dValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEpos::WriteUInt64Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, __int64 ullValue, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = FALSE;
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, ( BYTE*)&ullValue, sizeof(ullValue), oSignedDataType, &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(ullValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEpos::WriteInt8Object(CDeviceCommandSetManagerBase *pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, __int8 bValue, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = TRUE;
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)&bValue, sizeof(bValue), oSignedDataType, &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(bValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEpos::WriteInt16Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, short iValue, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = TRUE;
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)&iValue, sizeof(iValue), oSignedDataType, &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(iValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEpos::WriteInt32Object(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, long lValue, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = TRUE;
    DWORD dNumberOfBytesWritten;

    if(!WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)&lValue, sizeof(lValue), oSignedDataType, &dNumberOfBytesWritten, pErrorInfo)) return FALSE;
    if(sizeof(lValue) != dNumberOfBytesWritten)
    {
        if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Internal, pErrorInfo);
        return FALSE;
    }

    return TRUE;
}

BOOL CGatewayVCStoEpos::WriteDataBufferObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, void* p_pData, DWORD dNumberOfBytesToWrite, DWORD* pdNumberOfBytesWritten, CErrorInfo* pErrorInfo)
{
    const BOOL oSignedDataType = FALSE;
    DWORD dNumberOfBytesWritten;
    BOOL oResult = FALSE;

    if(p_pData)
    {
        oResult = WriteODObject(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, wSubIndex, (BYTE*)p_pData, dNumberOfBytesToWrite, oSignedDataType, &dNumberOfBytesWritten, pErrorInfo);

        if(pdNumberOfBytesWritten) *pdNumberOfBytesWritten = dNumberOfBytesWritten;
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::SetDisableState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, CErrorInfo* pErrorInfo)
{
	WORD usIndex = 0;
	WORD usSubIndex = 0;
	WORD usDeviceState = 0;
	BOOL oResult = FALSE;

	//Read Device State
    oResult = ReadDeviceState(pManager, hDCS_Handle, hTransactionHandle, wNodeId, &usDeviceState, pErrorInfo);
    if(oResult)
    {
        usIndex = INDEX_CONTROL_WORD;
        usSubIndex = SUBINDEX_CONTROL_WORD;
        switch(usDeviceState)
        {
            case DS_FAULT:
            case DS_SWITCH_ON_DISABLED: oResult = TRUE; break;
            case DS_OPERATION_ENABLED:
            case DS_QUICK_STOP_ACTIV:
            case DS_SWITCHED_ON:
            case DS_READY_TO_SWITCH_ON:
                if(oResult)
                    oResult = WriteUInt16Object(pManager, hDCS_Handle, hTransactionHandle, wNodeId, usIndex, usSubIndex, DISABLE_VOLTAGE, pErrorInfo);
                if(oResult)
                    oResult = CheckDeviceState(pManager, hDCS_Handle, hTransactionHandle, wNodeId, DS_SWITCH_ON_DISABLED, pErrorInfo);
                break;
            default:
                oResult = FALSE;
        }
    }

	return oResult;
}

BOOL CGatewayVCStoEpos::ReadDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD* pwDeviceState, CErrorInfo* pErrorInfo)
{
    const WORD k_wStateBit5Mask = 0x0020;
    const WORD k_wState5BitMask = 0x004F;
    const WORD k_wState6BitMask = 0x006F;

    WORD wIndex;
    WORD ubSubIndex;
    WORD wDeviceState;

    //Read Device State
    wIndex = INDEX_STATUS_WORD;
    ubSubIndex = SUBINDEX_STATUS_WORD;
    if(!ReadUInt16Object(pManager, hDCS_Handle, hTransactionHandle, wNodeId, wIndex, ubSubIndex, &wDeviceState, pErrorInfo))
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

BOOL CGatewayVCStoEpos::CheckDeviceState(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wState, CErrorInfo* pErrorInfo)
{
    DWORD dTime = 0;
    DWORD dTimeoutDeviceState = 1000;
    DWORD dStart = MmcGetTickCount();
    WORD wDeviceState = 0;

    while((dTime < dTimeoutDeviceState) && (wDeviceState != wState))
    {
        if(!ReadDeviceState(pManager, hDCS_Handle, hTransactionHandle, wNodeId, &wDeviceState, pErrorInfo)) return FALSE;

        dTime = MmcGetTickCount()-dStart;

        if(dTime > dTimeoutDeviceState)
        {
            if(m_pErrorHandling) m_pErrorHandling->GetError(k_Error_Timeout, pErrorInfo);
            return FALSE;
        }
    }
    return TRUE;
}

CGatewayVCStoEpos& CGatewayVCStoEpos::operator=(CGatewayVCStoEpos& other)
{
    if(this != &other)
    {
        *((CGatewayVCStoDCS*)this) = *((CGatewayVCStoDCS*)&other);
    }

    return *this;
}

void CGatewayVCStoEpos::InitCommands_ObjectDictionary()
{
    DeleteCommands_ObjectDictionary();

    //WriteODObject
    m_pCommand_WriteODObject = new CCommand_DCS_Epos();
    m_pCommand_WriteODObject->InitCommand(EPOS_WRITE_OD_OBJECT);

    //ReadODObject
    m_pCommand_ReadODObject = new CCommand_DCS_Epos();
    m_pCommand_ReadODObject->InitCommand(EPOS_READ_OD_OBJECT);

    //WriteODObjectNextSegment
    m_pCommand_WriteODObjectNextSegment = new CCommand_DCS_Epos();
    m_pCommand_WriteODObjectNextSegment->InitCommand(EPOS_WRITE_OD_OBJECT_NEXT_SEGMENT);

    //ReadODObjectNextSegment
    m_pCommand_ReadODObjectNextSegment = new CCommand_DCS_Epos();
    m_pCommand_ReadODObjectNextSegment->InitCommand(EPOS_READ_OD_OBJECT_NEXT_SEGMENT);

    //WriteObject
    m_pCommand_WriteObject = new CCommand_DCS_Epos();
    m_pCommand_WriteObject->InitCommand(EPOS_WRITE_OBJECT);

    //ReadObject
    m_pCommand_ReadObject = new CCommand_DCS_Epos();
    m_pCommand_ReadObject->InitCommand(EPOS_READ_OBJECT);

    //InitiateSegmentedWrite
    m_pCommand_InitiateSegmentedWrite = new CCommand_DCS_Epos();
    m_pCommand_InitiateSegmentedWrite->InitCommand(EPOS_INITIATE_SEGMENT_WRITE);

    //InitiateSegmentedRead
    m_pCommand_InitiateSegmentedRead = new CCommand_DCS_Epos();
    m_pCommand_InitiateSegmentedRead->InitCommand(EPOS_INITIATE_SEGMENT_READ);

    //SegmentedWrite
    m_pCommand_SegmentedWrite = new CCommand_DCS_Epos();
    m_pCommand_SegmentedWrite->InitCommand(EPOS_SEGMENT_WRITE);

    //SegmentedRead
    m_pCommand_SegmentedRead = new CCommand_DCS_Epos();
    m_pCommand_SegmentedRead->InitCommand(EPOS_SEGMENT_READ);

    //AbortSegmentedTransfer
    m_pCommand_AbortSegmentedTransfer = new CCommand_DCS_Epos();
    m_pCommand_AbortSegmentedTransfer->InitCommand(EPOS_ABORT_SEGMENT_TRANSFER);
}

void CGatewayVCStoEpos::DeleteCommands_ObjectDictionary()
{
    //WriteODObject
    if(m_pCommand_WriteODObject)
    {
        delete m_pCommand_WriteODObject;
        m_pCommand_WriteODObject = NULL;
    }

    //WriteODObjectNextSegment
    if(m_pCommand_WriteODObjectNextSegment)
    {
        delete m_pCommand_WriteODObjectNextSegment;
        m_pCommand_WriteODObjectNextSegment = NULL;
    }

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

    //ReadODObject
    if(m_pCommand_ReadODObject)
    {
        delete m_pCommand_ReadODObject;
        m_pCommand_ReadODObject = NULL;
    }

    //ReadODObjectNextSegment
    if(m_pCommand_ReadODObjectNextSegment)
    {
        delete m_pCommand_ReadODObjectNextSegment;
        m_pCommand_ReadODObjectNextSegment = NULL;
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

void CGatewayVCStoEpos::InitCommands_NetworkManagement()
{
    DeleteCommands_NetworkManagement();

    //Init Command SendNMTService
    m_pCommand_SendNMTService = new CCommand_DCS_Epos();
    m_pCommand_SendNMTService->InitCommand(EPOS_SEND_NMT_SERVICE);
}

void CGatewayVCStoEpos::DeleteCommands_NetworkManagement()
{
    if(m_pCommand_SendNMTService)
    {
        delete m_pCommand_SendNMTService;
        m_pCommand_SendNMTService = NULL;
    }
}

void CGatewayVCStoEpos::InitCommands_GeneralGateway()
{
    DeleteCommands_GeneralGateway();

    //Init Command Request
    m_pCommand_RequestCANFrame = new CCommand_DCS_Epos();
    m_pCommand_RequestCANFrame->InitCommand(EPOS_REQUEST_CAN_FRAME);

    //Init Command Send
    m_pCommand_SendCANFrame = new CCommand_DCS_Epos();
    m_pCommand_SendCANFrame->InitCommand(EPOS_SEND_CAN_FRAME);

    //Init Command Read
    m_pCommand_ReadCANFrame = new CCommand_DCS_Epos();
    m_pCommand_ReadCANFrame->InitCommand(EPOS_READ_CAN_FRAME);
}

void CGatewayVCStoEpos::InitCommands_LayerSettingServices()
{
    DeleteCommands_LayerSettingServices();

    //Init Command Send
    m_pCommand_SendLSSFrame = new CCommand_DCS_Epos();
    m_pCommand_SendLSSFrame->InitCommand(EPOS_SEND_LSS_FRAME);

    //Init Command Read
    m_pCommand_ReadLSSFrame = new CCommand_DCS_Epos();
    m_pCommand_ReadLSSFrame->InitCommand(EPOS_READ_LSS_FRAME);
}

void CGatewayVCStoEpos::DeleteCommands_GeneralGateway()
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

void CGatewayVCStoEpos::DeleteCommands_LayerSettingServices()
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

BOOL CGatewayVCStoEpos::DCS_SendCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SendCANFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_SendCANFrame->ResetStatus();
        m_pCommand_SendCANFrame->SetParameterData(0, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_SendCANFrame->SetParameterData(1, &p_usLength, sizeof(p_usLength));
        m_pCommand_SendCANFrame->SetParameterData(2, p_pData, p_usLength);

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SendCANFrame, hDCS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_ReadCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, DWORD p_ulTimeout, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadCANFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadCANFrame->ResetStatus();
        m_pCommand_ReadCANFrame->SetParameterData(0, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_ReadCANFrame->SetParameterData(1, &p_usLength, sizeof(p_usLength));
        m_pCommand_ReadCANFrame->SetParameterData(2, &p_ulTimeout, sizeof(p_ulTimeout));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadCANFrame, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_ReadCANFrame->GetReturnParameterData(0, p_pData, p_usLength);

        //Get ErrorCode
        m_pCommand_ReadCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_RequestCANFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD p_usCobID, WORD p_usLength, void* p_pData, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_RequestCANFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_RequestCANFrame->ResetStatus();
        m_pCommand_RequestCANFrame->SetParameterData(0, &p_usCobID, sizeof(p_usCobID));
        m_pCommand_RequestCANFrame->SetParameterData(1, &p_usLength, sizeof(p_usLength));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_RequestCANFrame, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_RequestCANFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_RequestCANFrame->GetReturnParameterData(1, p_pData, p_usLength);

        //Get ErrorCode
        m_pCommand_RequestCANFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_SendNMTService(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wCommandSpecifier, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SendNMTService && pManager)
    {
        //Set Parameter Data
        m_pCommand_SendNMTService->ResetStatus();
        m_pCommand_SendNMTService->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_SendNMTService->SetParameterData(1, &wCommandSpecifier, sizeof(wCommandSpecifier));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SendNMTService, hDCS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SendNMTService->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_SendLSSFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SendLSSFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_SendLSSFrame->ResetStatus();
        m_pCommand_SendLSSFrame->SetParameterData(0, pDataBuffer, dDataBufferLength);

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_SendLSSFrame, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SendLSSFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_SendLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_ReadLSSFrame(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wTimeout, void* pDataBuffer, DWORD dDataBufferLength, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadLSSFrame && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadLSSFrame->ResetStatus();
        m_pCommand_ReadLSSFrame->SetParameterData(0, &wTimeout, sizeof(wTimeout));

        //Execute Command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadLSSFrame, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadLSSFrame->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_ReadLSSFrame->GetReturnParameterData(1, pDataBuffer, dDataBufferLength);

        //Get ErrorCode
        m_pCommand_ReadLSSFrame->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_InitiateSegmentedRead(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedRead && pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedRead->ResetStatus();
        m_pCommand_InitiateSegmentedRead->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedRead->SetParameterData(2, &uNodeId, sizeof(uNodeId));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_InitiateSegmentedRead, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedRead->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedRead->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_InitiateSegmentedWrite(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_InitiateSegmentedWrite && pManager)
    {
        //Set Parameter Data
        m_pCommand_InitiateSegmentedWrite->ResetStatus();
        m_pCommand_InitiateSegmentedWrite->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(2, &uNodeId, sizeof(uNodeId));
        m_pCommand_InitiateSegmentedWrite->SetParameterData(3, &dObjectLength, sizeof(dObjectLength));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_InitiateSegmentedWrite, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_InitiateSegmentedWrite->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_InitiateSegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_ReadObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadObject->ResetStatus();
        m_pCommand_ReadObject->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_ReadObject->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_ReadObject->SetParameterData(2, &uNodeId, sizeof(uNodeId));
        m_pCommand_ReadObject->SetParameterData(3, &dObjectLength, sizeof(dObjectLength));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadObject, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadObject->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_ReadObject->GetReturnParameterData(1, pObjectBuffer, dObjectLength);

        //Get ErrorCode
        m_pCommand_ReadObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_ReadODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, WORD wMaxNumberOfBytesToRead, WORD* pwNumberOfBytesRead, WORD* pwSegmentedRead, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadODObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadODObject->ResetStatus();
        m_pCommand_ReadODObject->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_ReadODObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_ReadODObject->SetParameterData(2, &wSubIndex, sizeof(wSubIndex));
        m_pCommand_ReadODObject->SetParameterData(3, &wMaxNumberOfBytesToRead, sizeof(wMaxNumberOfBytesToRead));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadODObject, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadODObject->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_ReadODObject->GetReturnParameterData(1, pwSegmentedRead, sizeof(*pwSegmentedRead));
        m_pCommand_ReadODObject->GetReturnParameterData(2, pwNumberOfBytesRead, sizeof(*pwNumberOfBytesRead));
        m_pCommand_ReadODObject->GetReturnParameterData(3, pDataBuffer, wMaxNumberOfBytesToRead);

        //Get ErrorCode
        m_pCommand_ReadODObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_ReadODObjectNextSegment(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, BYTE* pDataBuffer, WORD wMaxNumberOfBytesToRead, WORD* pwNumberOfBytesRead, WORD* pwSegmentedRead, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_ReadODObjectNextSegment && pManager)
    {
        //Set Parameter Data
        m_pCommand_ReadODObjectNextSegment->ResetStatus();
        m_pCommand_ReadODObjectNextSegment->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_ReadODObjectNextSegment->SetParameterData(1, &wMaxNumberOfBytesToRead, sizeof(wMaxNumberOfBytesToRead));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_ReadODObjectNextSegment, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(1, pwSegmentedRead, sizeof(*pwSegmentedRead));
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(2, pwNumberOfBytesRead, sizeof(*pwNumberOfBytesRead));
        m_pCommand_ReadODObjectNextSegment->GetReturnParameterData(3, pDataBuffer, wMaxNumberOfBytesToRead);

        //Get ErrorCode
        m_pCommand_ReadODObjectNextSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_SegmentedRead(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, BOOL oToggle, BOOL* poMoreSegments, BYTE* pSegmentBuffer, DWORD dSegmentBufferLength, DWORD* pdSegmentLengthRead, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    UEposControlByte controlByte;
    BYTE uDummyByte = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedRead && pManager)
    {
        //Prepare ControlByte
        controlByte.bValue = 0;
        controlByte.structure.bToggle = oToggle;

        //Set Parameter Data
        m_pCommand_SegmentedRead->ResetStatus();
        m_pCommand_SegmentedRead->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_SegmentedRead->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedRead->SetParameterData(2, &uDummyByte, sizeof(uDummyByte));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_SegmentedRead, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedRead->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_SegmentedRead->GetReturnParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedRead->GetReturnParameterData(2, pSegmentBuffer, dSegmentBufferLength);

        //Get ErrorCode
        m_pCommand_SegmentedRead->GetErrorInfo(pErrorInfo);

        //Restore ControlByte
        if(pdSegmentLengthRead) *pdSegmentLengthRead = controlByte.structure.bLength;
        if(poMoreSegments) *poMoreSegments = controlByte.structure.bMoreSegments;
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_SegmentedWrite(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, BOOL oToggle, BOOL oMoreSegments, BYTE* pSegmentBuffer, DWORD dSegmentLength, DWORD* pdSegmentLengthWritten, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    UEposControlByte controlByte;
    BOOL oResult = FALSE;

    if(m_pCommand_SegmentedWrite && pManager)
    {
        //Prepare ControlByte
        controlByte.bValue = 0;
        controlByte.structure.bLength = dSegmentLength;
        controlByte.structure.bToggle = oToggle;
        controlByte.structure.bMoreSegments = oMoreSegments;

        //Set Parameter Data
        m_pCommand_SegmentedWrite->ResetStatus();
        m_pCommand_SegmentedWrite->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_SegmentedWrite->SetParameterData(1, &controlByte, sizeof(controlByte));
        m_pCommand_SegmentedWrite->SetParameterData(2, pSegmentBuffer, dSegmentLength);

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_SegmentedWrite, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_SegmentedWrite->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));
        m_pCommand_SegmentedWrite->GetReturnParameterData(1, &controlByte, sizeof(controlByte));

        //Restore ControlByte
        if(pdSegmentLengthWritten) *pdSegmentLengthWritten = controlByte.structure.bLength;

        //Get ErrorCode
        m_pCommand_SegmentedWrite->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_WriteObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pObjectBuffer, DWORD dObjectLength, CErrorInfo* pErrorInfo)
{
    DWORD ulDeviceErrorCode = 0;
    BYTE uSubIndex = (BYTE)wSubIndex;
    BYTE uNodeId = (BYTE)wNodeId;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_WriteObject->ResetStatus();
        m_pCommand_WriteObject->SetParameterData(0, &wIndex, sizeof(wIndex));
        m_pCommand_WriteObject->SetParameterData(1, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_WriteObject->SetParameterData(2, &uNodeId, sizeof(uNodeId));
        m_pCommand_WriteObject->SetParameterData(3, pObjectBuffer, dObjectLength);
        m_pCommand_WriteObject->SetParameterData(4, &dObjectLength, sizeof(dObjectLength));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_WriteObject, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteObject->GetReturnParameterData(0, &ulDeviceErrorCode, sizeof(ulDeviceErrorCode));

        //Get ErrorCode
        m_pCommand_WriteObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_WriteODObject(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, BYTE* pDataBuffer, WORD wNumberOfBytesToWrite, BOOL oSignedDataType, WORD* pwNumberOfBytesWritten, WORD* pwSegmentedWrite, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteODObject && pManager)
    {
        //Set Parameter Data
        m_pCommand_WriteODObject->ResetStatus();
        m_pCommand_WriteODObject->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_WriteODObject->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_WriteODObject->SetParameterData(2, &wSubIndex, sizeof(wSubIndex));
        m_pCommand_WriteODObject->SetParameterData(3, &wNumberOfBytesToWrite, sizeof(wNumberOfBytesToWrite));
        m_pCommand_WriteODObject->SetParameterData(4, &wNumberOfBytesToWrite, sizeof(wNumberOfBytesToWrite));
        m_pCommand_WriteODObject->SetParameterData(5, pDataBuffer, wNumberOfBytesToWrite);
        m_pCommand_WriteODObject->SetParameterData(6, &oSignedDataType, sizeof(oSignedDataType));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_WriteODObject, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteODObject->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_WriteODObject->GetReturnParameterData(1, pwSegmentedWrite, sizeof(pwSegmentedWrite));
        m_pCommand_WriteODObject->GetReturnParameterData(2, pwNumberOfBytesWritten, sizeof(*pwNumberOfBytesWritten));

        //Get ErrorCode
        m_pCommand_WriteODObject->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_WriteODObjectNextSegment(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, BYTE* pDataBuffer, WORD wNumberOfBytesToWrite, BOOL oSignedDataType, WORD* pwNumberOfBytesWritten, WORD* pwSegmentedWrite, CErrorInfo* pErrorInfo)
{
    WORD wDeviceErrorCode = 0;
    BOOL oResult = FALSE;

    if(m_pCommand_WriteODObjectNextSegment && pManager)
    {
        //Set Parameter Data
        m_pCommand_WriteODObjectNextSegment->ResetStatus();
        m_pCommand_WriteODObjectNextSegment->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_WriteODObjectNextSegment->SetParameterData(1, &wNumberOfBytesToWrite, sizeof(wNumberOfBytesToWrite));
        m_pCommand_WriteODObjectNextSegment->SetParameterData(2, pDataBuffer, wNumberOfBytesToWrite);
        m_pCommand_WriteODObjectNextSegment->SetParameterData(3, &oSignedDataType, sizeof(oSignedDataType));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_WriteODObjectNextSegment, hDCS_Handle, hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_WriteODObjectNextSegment->GetReturnParameterData(0, &wDeviceErrorCode, sizeof(wDeviceErrorCode));
        m_pCommand_WriteODObjectNextSegment->GetReturnParameterData(1, pwSegmentedWrite, sizeof(pwSegmentedWrite));
        m_pCommand_WriteODObjectNextSegment->GetReturnParameterData(2, pwNumberOfBytesWritten, sizeof(*pwNumberOfBytesWritten));

        //Get ErrorCode
        m_pCommand_WriteODObjectNextSegment->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::DCS_AbortSegmentedTransfer(CDeviceCommandSetManagerBase* pManager, HANDLE hDCS_Handle, HANDLE hTransactionHandle, WORD wNodeId, WORD wIndex, WORD wSubIndex, DWORD dAbortCode, CErrorInfo* pErrorInfo)
{
    BYTE uSubIndex = (BYTE)wSubIndex;
    BOOL oResult = FALSE;

    if(m_pCommand_AbortSegmentedTransfer && pManager)
    {
        //Set Parameter Data
        m_pCommand_AbortSegmentedTransfer->ResetStatus();
        m_pCommand_AbortSegmentedTransfer->SetParameterData(0, &wNodeId, sizeof(wNodeId));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(1, &wIndex, sizeof(wIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(2, &uSubIndex, sizeof(uSubIndex));
        m_pCommand_AbortSegmentedTransfer->SetParameterData(3, &dAbortCode, sizeof(dAbortCode));

        //Execute command
        oResult = pManager->ExecuteCommand(m_pCommand_AbortSegmentedTransfer, hDCS_Handle, hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AbortSegmentedTransfer->GetErrorInfo(pErrorInfo);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::InitLayerParameterStack(CCommandRoot* p_pCommand)
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
			if(m_pCommand_WriteODObject) m_pCommand_WriteODObject->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_WriteODObjectNextSegment) m_pCommand_WriteODObjectNextSegment->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_WriteObject) m_pCommand_WriteObject->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_InitiateSegmentedWrite) m_pCommand_InitiateSegmentedWrite->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_SegmentedWrite) m_pCommand_SegmentedWrite->SetLayerParameterStack(layerParameterStack);
			
			if(m_pCommand_ReadODObject) m_pCommand_ReadODObject->SetLayerParameterStack(layerParameterStack);
			if(m_pCommand_ReadODObjectNextSegment) m_pCommand_ReadODObjectNextSegment->SetLayerParameterStack(layerParameterStack);
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

BOOL CGatewayVCStoEpos::GetNodeId(CCommand_VCS* p_pCommand, CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hDCS_Handle, BYTE& p_rNodeId, CErrorInfo* p_pErrorInfo)
{
	const CStdString NODE_ID = _T("NodeId");

	BOOL oResult = FALSE;

	//Get NetworkId from Command
	if(!oResult && p_pCommand) oResult = p_pCommand->GetLayerParameter(DEVICE_COMMAND_SET_LAYER, NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo); 
		
	//Get NetworkId from Device
	if(!oResult && p_pManager) oResult = p_pManager->GetParameter(PT_DEVICE, p_hDCS_Handle, NODE_ID, (BYTE*)&p_rNodeId, sizeof(p_rNodeId), p_pErrorInfo);

	return oResult;
}

BOOL CGatewayVCStoEpos::GetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, DWORD& p_rulTransferRate, CErrorInfo* p_pErrorInfo)
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
                if(ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, INDEX_CAN_BITRATE, SUBINDEX_CAN_BITRATE, &usTransferRateIndex, p_pErrorInfo))
				{
					if(CheckBitrateIndexCiaStandard(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, usTransferRateIndex))
					{
						//Use CiA Standard for Conversion
						p_rulTransferRate = ConvertBitrateIndexToValue_CiaStandard(usTransferRateIndex);
						oResult = TRUE;
					}
					else
					{
						//Use Old Maxon Standard for Conversion
						p_rulTransferRate = ConvertBitrateIndexToValue_Maxon(usTransferRateIndex);
						oResult = TRUE;
					}
				}
            }
            else
            {
                if(interfaceName == INTERFACE_RS232)
                {
                    //RS232 Baudrate
                    if(ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, INDEX_BAUDRATE, SUBINDEX_BAUDRATE, &usTransferRateIndex, p_pErrorInfo))
					{
						p_rulTransferRate = ConvertBaudrateIndexToValue(usTransferRateIndex);
						oResult = TRUE;
					}
                }
            }
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::SetDeviceTransferRate(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, DWORD p_ulTransferRate, CErrorInfo* p_pErrorInfo)
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
				if(CheckBitrateValueCiaStandard(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, p_ulTransferRate))
				{
					//Use CiA Standard for Conversion
					usTransferRateIndex = ConvertBitrateValueToIndex_CiaStandard(p_ulTransferRate);
				}
				else
				{
					//Use Old Maxon Standard for Conversion
					usTransferRateIndex = ConvertBitrateValueToIndex_Maxon(p_ulTransferRate);
				}

				//Write Object
                oResult = WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, INDEX_CAN_BITRATE, SUBINDEX_CAN_BITRATE, usTransferRateIndex, p_pErrorInfo);
            }
            else
            {
                if(interfaceName == INTERFACE_RS232)
                {
                    //RS232 Frame Timeout
					usTransferRateIndex = ConvertBaudrateValueToIndex(p_ulTransferRate);
                    oResult = WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, INDEX_BAUDRATE, SUBINDEX_BAUDRATE, usTransferRateIndex, p_pErrorInfo);
                }
            }
        }
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::RestoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
{
    const DWORD TIMEOUT = 1000;

    WORD usIndex;
    WORD usSubIndex;
    DWORD ulPassword = 0;
    DWORD ulTransferRate = 0;
    DWORD ulBackupTimeout = 0;
    BOOL oResult = TRUE;

    CErrorInfo errorInfo;

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

        usIndex = INDEX_RESTORE_DEFAULT_PARAMETERS;
        usSubIndex = SUBINDEX_RESTORE_ALL_DEFAULT_PARAMETERS;
        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hHandle, p_hTransactionHandle, (WORD)p_ubNodeId, usIndex, usSubIndex, ulPassword, p_pErrorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);

        return oResult;
    }

    return FALSE;
}

BOOL CGatewayVCStoEpos::StoreParameter(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
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

        if(oResult) oResult = WriteUInt32Object(p_pManager, p_hHandle, p_hTransactionHandle, (WORD)p_ubNodeId, INDEX_STORE_PARAMETERS, SUBINDEX_SAVE_ALL_PARAMETERS, ulPassword, p_pErrorInfo);

        //Restore Timeout
        p_pManager->DCS_SetProtocolStackSettings(p_hHandle, ulTransferRate, ulBackupTimeout, TRUE);
    }

    return oResult;
}
	
BOOL CGatewayVCStoEpos::ResetDevice(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, BOOL p_oWaitForReboot, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    //SendNMTService
    if(DCS_SendNMTService(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, (WORD)NSI_RESET_NODE, p_pErrorInfo))
	{
		//Init
		oResult = TRUE;

		if(p_oWaitForReboot)
		{
			//WaitForReboot
			WaitForReboot(p_pManager, p_hHandle, 0, p_ubNodeId, 0);
		}
	}

    return oResult;
}

BOOL CGatewayVCStoEpos::WaitForReboot(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE p_ubNodeId, CErrorInfo* p_pErrorInfo)
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
		if(ReadUInt8Object(p_pManager, p_hHandle, p_hTransactionHandle, p_ubNodeId, INDEX_ERROR_REGISTER, SUBINDEX_ERROR_REGISTER, &uErrorRegister, p_pErrorInfo))
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

DWORD CGatewayVCStoEpos::ConvertBaudrateIndexToValue(WORD wBaudrateIndex)
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

WORD CGatewayVCStoEpos::ConvertBaudrateValueToIndex(DWORD dBaudrate)
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

BOOL CGatewayVCStoEpos::CheckBitrateIndexCiaStandard(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE uNodeId, WORD wBaudrateIndex)
{
    const WORD COMPATIBILITY_LIMIT = 4;
    
    BOOL oResult = FALSE;

    if(wBaudrateIndex <= COMPATIBILITY_LIMIT)
    {
        //Compatible to Cia Standard
        oResult = TRUE;
    }
    else
    {
        //Check by trying to write Bitrate Index
        oResult = CheckBitrateCiaStandard(p_pManager, p_hHandle, p_hTransactionHandle, uNodeId);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::CheckBitrateValueCiaStandard(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE uNodeId, DWORD ulBaudrate)
{
    const DWORD COMPATIBILITY_LIMIT = 125000;
    const WORD BITRATE_INDEX_CHECK = 7;
    
    WORD usBitrateIndex = 0;
    BOOL oResult = FALSE;

    if(ulBaudrate >= COMPATIBILITY_LIMIT)
    {
        //Compatible to Cia Standard
        oResult = TRUE;
    }
    else
    {
        //Check by trying to write Bitrate Index
        oResult = CheckBitrateCiaStandard(p_pManager, p_hHandle, p_hTransactionHandle, uNodeId);
    }

    return oResult;
}

BOOL CGatewayVCStoEpos::CheckBitrateCiaStandard(CDeviceCommandSetManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BYTE uNodeId)
{
    const WORD BITRATE_INDEX_CHECK = 7;
    
    WORD usBitrateIndex = 0;
    BOOL oResult = FALSE;

    //Backup Bitrate Index
    if(ReadUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, uNodeId, INDEX_CAN_BITRATE, SUBINDEX_CAN_BITRATE, &usBitrateIndex))
    {
        //Check Bitrate Index
		if(WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, uNodeId, INDEX_CAN_BITRATE, SUBINDEX_CAN_BITRATE, BITRATE_INDEX_CHECK))
        {
            //Compatible to Cia Standard
            oResult = TRUE;
        }

        //Restore Bitrate Index 
		WriteUInt16Object(p_pManager, p_hHandle, p_hTransactionHandle, uNodeId, INDEX_CAN_BITRATE, SUBINDEX_CAN_BITRATE, usBitrateIndex);
    }

    return oResult;
}

DWORD CGatewayVCStoEpos::ConvertBitrateIndexToValue_CiaStandard(WORD wBaudrateIndex)
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

WORD CGatewayVCStoEpos::ConvertBitrateValueToIndex_CiaStandard(DWORD dBaudrate)
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

DWORD CGatewayVCStoEpos::ConvertBitrateIndexToValue_Maxon(WORD wBaudrateIndex)
{
	switch(wBaudrateIndex)
	{
		case 0: return 1000000;
		case 1: return 800000;
		case 2: return 500000;
		case 3: return 250000;
		case 4: return 125000;
		case 5: return 50000;
		case 6: return 20000;
		default: return 1000000;
		case 9: return TRANSFER_RATE_AUTO;
	}
}

WORD CGatewayVCStoEpos::ConvertBitrateValueToIndex_Maxon(DWORD dBaudrate)
{
	switch(dBaudrate)
	{
		case 1000000: return 0 ;
		case 800000: return 1 ;
		case 500000: return 2;
		case 250000: return 3;
		case 125000: return 4;
		case 50000: return 5;
		case 20000: return 6;
		case TRANSFER_RATE_AUTO: return 9;
		default: return 0;
	}
}

