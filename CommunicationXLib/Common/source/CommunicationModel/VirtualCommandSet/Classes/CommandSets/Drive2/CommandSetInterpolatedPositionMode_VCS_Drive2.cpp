#include "stdafx.h"
#include "CommandSetInterpolatedPositionMode_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetInterpolatedPositionMode_VCS_Drive2::CCommandSetInterpolatedPositionMode_VCS_Drive2(void)
{
    m_strCommandSetName = COMMAND_SET_INTERPOLATED_POSITION_MODE;

    m_pCommand_SetIpmBufferParameter = NULL;
    m_pCommand_GetIpmBufferParameter = NULL;
    m_pCommand_ClearIpmBuffer = NULL;
    m_pCommand_GetFreeIpmBufferSize = NULL;
    m_pCommand_AddPvtValueToIpmBuffer = NULL;
    m_pCommand_StartIpmTrajectory = NULL;
    m_pCommand_StopIpmTrajectory = NULL;
    m_pCommand_GetIpmStatus = NULL;

    InitCommands();
}

CCommandSetInterpolatedPositionMode_VCS_Drive2::~CCommandSetInterpolatedPositionMode_VCS_Drive2(void)
{
    DeleteCommands();
}
BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_SetIpmBufferParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usUnderflowWarningLimit, WORD p_usOverflowWarningLimit, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetIpmBufferParameter)
    {
        //Set Parameter Data
        m_pCommand_SetIpmBufferParameter->ResetStatus();
        m_pCommand_SetIpmBufferParameter->SetParameterData(0, &p_usUnderflowWarningLimit, sizeof(p_usUnderflowWarningLimit));
        m_pCommand_SetIpmBufferParameter->SetParameterData(1, &p_usOverflowWarningLimit, sizeof(p_usOverflowWarningLimit));

        //ExecuteCommand
        oResult = m_pCommand_SetIpmBufferParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetIpmBufferParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_GetIpmBufferParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusUnderflowWarningLimit, WORD *p_pusOverflowWarningLimit, DWORD *p_pulMaxBufferSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetIpmBufferParameter)
    {
        //Set Parameter Data
        m_pCommand_GetIpmBufferParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetIpmBufferParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetIpmBufferParameter->GetReturnParameterData(0, p_pusUnderflowWarningLimit, sizeof(*p_pusUnderflowWarningLimit));
        m_pCommand_GetIpmBufferParameter->GetReturnParameterData(1, p_pusOverflowWarningLimit, sizeof(*p_pusOverflowWarningLimit));
        m_pCommand_GetIpmBufferParameter->GetReturnParameterData(2, p_pulMaxBufferSize, sizeof(*p_pulMaxBufferSize));

        //Get ErrorCode
        m_pCommand_GetIpmBufferParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_ClearIpmBuffer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_ClearIpmBuffer)
    {
        //Set Parameter Data
        m_pCommand_ClearIpmBuffer->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_ClearIpmBuffer->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_ClearIpmBuffer->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_GetFreeIpmBufferSize(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD *p_pulBufferSize, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetFreeIpmBufferSize)
    {
        //Set Parameter Data
        m_pCommand_GetFreeIpmBufferSize->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetFreeIpmBufferSize->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetFreeIpmBufferSize->GetReturnParameterData(0, p_pulBufferSize, sizeof(*p_pulBufferSize));

        //Get ErrorCode
        m_pCommand_GetFreeIpmBufferSize->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_AddPvtValueToIpmBuffer(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, long p_lPosition, long p_lVelocity, BYTE p_ubTime, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_AddPvtValueToIpmBuffer)
    {
        //Set Parameter Data
        m_pCommand_AddPvtValueToIpmBuffer->ResetStatus();
        m_pCommand_AddPvtValueToIpmBuffer->SetParameterData(0, &p_lPosition, sizeof(p_lPosition));
        m_pCommand_AddPvtValueToIpmBuffer->SetParameterData(1, &p_lVelocity, sizeof(p_lVelocity));
        m_pCommand_AddPvtValueToIpmBuffer->SetParameterData(2, &p_ubTime, sizeof(p_ubTime));

        //ExecuteCommand
        oResult = m_pCommand_AddPvtValueToIpmBuffer->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_AddPvtValueToIpmBuffer->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_StartIpmTrajectory(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_StartIpmTrajectory)
    {
        //Set Parameter Data
        m_pCommand_StartIpmTrajectory->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_StartIpmTrajectory->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_StartIpmTrajectory->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_StopIpmTrajectory(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_StopIpmTrajectory)
    {
        //Set Parameter Data
        m_pCommand_StopIpmTrajectory->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_StopIpmTrajectory->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_StopIpmTrajectory->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::VCS_GetIpmStatus(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL* p_poTrajectoryRunning, BOOL* p_poIsUnderflowWarning, BOOL* p_poIsOverflowWarning, BOOL* p_poIsVelocityWarning, BOOL* p_poIsAccelerationWarning, BOOL *p_poIsUnderflowError, BOOL* p_poIsOverflowError, BOOL* p_poIsVelocityError, BOOL* p_poIsAccelerationError, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetIpmStatus)
    {
        //Set Parameter Data
        m_pCommand_GetIpmStatus->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetIpmStatus->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetIpmStatus->GetReturnParameterData(0, p_poTrajectoryRunning, sizeof(*p_poTrajectoryRunning));
        m_pCommand_GetIpmStatus->GetReturnParameterData(1, p_poIsUnderflowWarning, sizeof(*p_poIsUnderflowWarning));
        m_pCommand_GetIpmStatus->GetReturnParameterData(2, p_poIsOverflowWarning, sizeof(*p_poIsOverflowWarning));
        m_pCommand_GetIpmStatus->GetReturnParameterData(3, p_poIsVelocityWarning, sizeof(*p_poIsVelocityWarning));
        m_pCommand_GetIpmStatus->GetReturnParameterData(4, p_poIsAccelerationWarning, sizeof(*p_poIsAccelerationWarning));
        m_pCommand_GetIpmStatus->GetReturnParameterData(5, p_poIsUnderflowError, sizeof(*p_poIsUnderflowError));
        m_pCommand_GetIpmStatus->GetReturnParameterData(6, p_poIsOverflowError, sizeof(*p_poIsOverflowError));
        m_pCommand_GetIpmStatus->GetReturnParameterData(7, p_poIsVelocityError, sizeof(*p_poIsVelocityError));
        m_pCommand_GetIpmStatus->GetReturnParameterData(8, p_poIsAccelerationError, sizeof(*p_poIsAccelerationError));

        //Get ErrorCode
        m_pCommand_GetIpmStatus->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetInterpolatedPositionMode_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init SetIpmBufferParameter
    m_pCommand_SetIpmBufferParameter = new CCommand_VCS_Drive2();
    m_pCommand_SetIpmBufferParameter->InitCommand(DRIVE2_SET_IPM_BUFFER_PARAMETER);

    //Init GetIpmBufferParameter
    m_pCommand_GetIpmBufferParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetIpmBufferParameter->InitCommand(DRIVE2_GET_IPM_BUFFER_PARAMETER);

    //Init ClearIpmBuffer
    m_pCommand_ClearIpmBuffer = new CCommand_VCS_Drive2();
    m_pCommand_ClearIpmBuffer->InitCommand(DRIVE2_CLEAR_IPM_BUFFER);

    //Init GetFreeIpmBuffer
    m_pCommand_GetFreeIpmBufferSize = new CCommand_VCS_Drive2();
    m_pCommand_GetFreeIpmBufferSize->InitCommand(DRIVE2_GET_FREE_IPM_BUFFER_SIZE);

    //Init AddPvtValueToIpmBuffer
    m_pCommand_AddPvtValueToIpmBuffer = new CCommand_VCS_Drive2();
    m_pCommand_AddPvtValueToIpmBuffer->InitCommand(DRIVE2_ADD_PVT_VALUE_TO_IPM_BUFFER);

    //Init StartIpmTrajectory
    m_pCommand_StartIpmTrajectory = new CCommand_VCS_Drive2();
    m_pCommand_StartIpmTrajectory->InitCommand(DRIVE2_START_IPM_TRAJECTORY);

    //Init StopIpmTrajectory
    m_pCommand_StopIpmTrajectory = new CCommand_VCS_Drive2();
    m_pCommand_StopIpmTrajectory->InitCommand(DRIVE2_STOP_IPM_TRAJECTORY);

    //Init GetIpmStatus
    m_pCommand_GetIpmStatus = new CCommand_VCS_Drive2();
    m_pCommand_GetIpmStatus->InitCommand(DRIVE2_GET_IPM_STATUS);
}

void CCommandSetInterpolatedPositionMode_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_SetIpmBufferParameter)
    {
        delete m_pCommand_SetIpmBufferParameter;
        m_pCommand_SetIpmBufferParameter = NULL;
    }
    if(m_pCommand_GetIpmBufferParameter)
    {
        delete m_pCommand_GetIpmBufferParameter;
        m_pCommand_GetIpmBufferParameter = NULL;
    }
    if(m_pCommand_ClearIpmBuffer)
    {
        delete m_pCommand_ClearIpmBuffer;
        m_pCommand_ClearIpmBuffer = NULL;
    }
    if(m_pCommand_GetFreeIpmBufferSize)
    {
        delete m_pCommand_GetFreeIpmBufferSize;
        m_pCommand_GetFreeIpmBufferSize = NULL;
    }
    if(m_pCommand_AddPvtValueToIpmBuffer)
    {
        delete m_pCommand_AddPvtValueToIpmBuffer;
        m_pCommand_AddPvtValueToIpmBuffer = NULL;
    }
    if(m_pCommand_StartIpmTrajectory)
    {
        delete m_pCommand_StartIpmTrajectory;
        m_pCommand_StartIpmTrajectory = NULL;
    }
    if(m_pCommand_StopIpmTrajectory)
    {
        delete m_pCommand_StopIpmTrajectory;
        m_pCommand_StopIpmTrajectory = NULL;
    }
    if(m_pCommand_GetIpmStatus)
    {
        delete m_pCommand_GetIpmStatus;
        m_pCommand_GetIpmStatus = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetInterpolatedPositionMode_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;
    BOOL oCheckVisibility = FALSE;

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_GetIpmBufferParameter && !m_pCommand_GetIpmBufferParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetFreeIpmBufferSize && !m_pCommand_GetFreeIpmBufferSize->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetIpmStatus && !m_pCommand_GetIpmStatus->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetIpmBufferParameter && !m_pCommand_SetIpmBufferParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_ClearIpmBuffer && !m_pCommand_ClearIpmBuffer->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_AddPvtValueToIpmBuffer && !m_pCommand_AddPvtValueToIpmBuffer->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_StartIpmTrajectory && !m_pCommand_StartIpmTrajectory->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_StopIpmTrajectory && !m_pCommand_StopIpmTrajectory->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetInterpolatedPositionMode_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetIpmBufferParameter) m_pCommand_SetIpmBufferParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetIpmBufferParameter) m_pCommand_GetIpmBufferParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_ClearIpmBuffer) m_pCommand_ClearIpmBuffer->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetFreeIpmBufferSize) m_pCommand_GetFreeIpmBufferSize->InitJournalManager(p_pJournalManager);
    if(m_pCommand_AddPvtValueToIpmBuffer) m_pCommand_AddPvtValueToIpmBuffer->InitJournalManager(p_pJournalManager);
    if(m_pCommand_StartIpmTrajectory) m_pCommand_StartIpmTrajectory->InitJournalManager(p_pJournalManager);
    if(m_pCommand_StopIpmTrajectory) m_pCommand_StopIpmTrajectory->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetIpmStatus) m_pCommand_GetIpmStatus->InitJournalManager(p_pJournalManager);
}

void CCommandSetInterpolatedPositionMode_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_SetIpmBufferParameter) m_pCommand_SetIpmBufferParameter->ResetJournalManager();
    if(m_pCommand_GetIpmBufferParameter) m_pCommand_GetIpmBufferParameter->ResetJournalManager();
    if(m_pCommand_ClearIpmBuffer) m_pCommand_ClearIpmBuffer->ResetJournalManager();
    if(m_pCommand_GetFreeIpmBufferSize) m_pCommand_GetFreeIpmBufferSize->ResetJournalManager();
    if(m_pCommand_AddPvtValueToIpmBuffer) m_pCommand_AddPvtValueToIpmBuffer->ResetJournalManager();
    if(m_pCommand_StartIpmTrajectory) m_pCommand_StartIpmTrajectory->ResetJournalManager();
    if(m_pCommand_StopIpmTrajectory) m_pCommand_StopIpmTrajectory->ResetJournalManager();
    if(m_pCommand_GetIpmStatus) m_pCommand_GetIpmStatus->ResetJournalManager();
}

BOOL CCommandSetInterpolatedPositionMode_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_SetIpmBufferParameter && !m_pCommand_SetIpmBufferParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetIpmBufferParameter && !m_pCommand_GetIpmBufferParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_ClearIpmBuffer && !m_pCommand_ClearIpmBuffer->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetFreeIpmBufferSize && !m_pCommand_GetFreeIpmBufferSize->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_AddPvtValueToIpmBuffer && !m_pCommand_AddPvtValueToIpmBuffer->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_StartIpmTrajectory && !m_pCommand_StartIpmTrajectory->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_StopIpmTrajectory && !m_pCommand_StopIpmTrajectory->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetIpmStatus && !m_pCommand_GetIpmStatus->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

