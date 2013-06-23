#include "stdafx.h"
#include "CommandSetConfigurationSensor_VCS_Drive.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetConfigurationSensor_VCS_Drive::CCommandSetConfigurationSensor_VCS_Drive()
{
    m_strCommandSetName = COMMAND_SET_SENSOR_CONFIGURATION;

    m_pCommand_GetEncoderParameter = NULL;
    m_pCommand_SetEncoderParameter = NULL;
    m_pCommand_GetSensorType = NULL;
    m_pCommand_SetSensorType = NULL;
    m_pCommand_GetIncEncoderParameter = NULL;
    m_pCommand_SetIncEncoderParameter = NULL;
    m_pCommand_GetHallSensorParameter = NULL;
    m_pCommand_SetHallSensorParameter = NULL;

    InitCommands();
}

CCommandSetConfigurationSensor_VCS_Drive::~CCommandSetConfigurationSensor_VCS_Drive()
{
    DeleteCommands();
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_GetEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusCounts, WORD *p_pusPositionSensorType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetEncoderParameter)
    {
        //Set Parameter Data
        m_pCommand_GetEncoderParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetEncoderParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetEncoderParameter->GetReturnParameterData(0, p_pusCounts, sizeof(*p_pusCounts));
        m_pCommand_GetEncoderParameter->GetReturnParameterData(1, p_pusPositionSensorType, sizeof(*p_pusPositionSensorType));

        //Get ErrorCode
        m_pCommand_GetEncoderParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_GetSensorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusSensorType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetSensorType)
    {
        //Set Parameter Data
        m_pCommand_GetSensorType->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetSensorType->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetSensorType->GetReturnParameterData(0, p_pusSensorType, sizeof(*p_pusSensorType));

        //Get ErrorCode
        m_pCommand_GetSensorType->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_GetIncEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD *p_pulEncoderResolution, BOOL *p_poInvertedPolarity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetIncEncoderParameter)
    {
        //Set Parameter Data
        m_pCommand_GetIncEncoderParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetIncEncoderParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetIncEncoderParameter->GetReturnParameterData(0, p_pulEncoderResolution, sizeof(*p_pulEncoderResolution));
        m_pCommand_GetIncEncoderParameter->GetReturnParameterData(1, p_poInvertedPolarity, sizeof(*p_poInvertedPolarity));

        //Get ErrorCode
        m_pCommand_GetIncEncoderParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_GetHallSensorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL *p_poInvertedPolarity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetHallSensorParameter)
    {
        //Set Parameter Data
        m_pCommand_GetHallSensorParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetHallSensorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetHallSensorParameter->GetReturnParameterData(0, p_poInvertedPolarity, sizeof(*p_poInvertedPolarity));

        //Get ErrorCode
        m_pCommand_GetHallSensorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_SetEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usCounts, WORD p_usPositionSensorType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetEncoderParameter)
    {
        //Set Parameter Data
        m_pCommand_SetEncoderParameter->ResetStatus();
        m_pCommand_SetEncoderParameter->SetParameterData(0, &p_usCounts, sizeof(p_usCounts));
        m_pCommand_SetEncoderParameter->SetParameterData(1, &p_usPositionSensorType, sizeof(p_usPositionSensorType));

        //ExecuteCommand
        oResult = m_pCommand_SetEncoderParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetEncoderParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_SetSensorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usSensorType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetSensorType)
    {
        //Set Parameter Data
        m_pCommand_SetSensorType->ResetStatus();
        m_pCommand_SetSensorType->SetParameterData(0, &p_usSensorType, sizeof(p_usSensorType));

        //ExecuteCommand
        oResult = m_pCommand_SetSensorType->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetSensorType->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_SetIncEncoderParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulEncoderResolution, BOOL p_oInvertedPolarity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetIncEncoderParameter)
    {
        //Set Parameter Data
        m_pCommand_SetIncEncoderParameter->ResetStatus();
        m_pCommand_SetIncEncoderParameter->SetParameterData(0, &p_ulEncoderResolution, sizeof(p_ulEncoderResolution));
        m_pCommand_SetIncEncoderParameter->SetParameterData(1, &p_oInvertedPolarity, sizeof(p_oInvertedPolarity));

        //ExecuteCommand
        oResult = m_pCommand_SetIncEncoderParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetIncEncoderParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::VCS_SetHallSensorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, BOOL p_oInvertedPolarity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetHallSensorParameter)
    {
        //Set Parameter Data
        m_pCommand_SetHallSensorParameter->ResetStatus();
        m_pCommand_SetHallSensorParameter->SetParameterData(0, &p_oInvertedPolarity, sizeof(p_oInvertedPolarity));

        //ExecuteCommand
        oResult = m_pCommand_SetHallSensorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetHallSensorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetConfigurationSensor_VCS_Drive::InitCommands()
{
    DeleteCommands();

    //Init GetEncoderParameter
    m_pCommand_GetEncoderParameter = new CCommand_VCS_Drive();
    m_pCommand_GetEncoderParameter->InitCommand(DRIVE_GET_ENCODER_PARAMETER);

    //Init SetEncoderParameter
    m_pCommand_SetEncoderParameter = new CCommand_VCS_Drive();
    m_pCommand_SetEncoderParameter->InitCommand(DRIVE_SET_ENCODER_PARAMETER);

    //Init GetSensorType
    m_pCommand_GetSensorType = new CCommand_VCS_Drive();
    m_pCommand_GetSensorType->InitCommand(DRIVE_GET_SENSOR_TYPE);

    //Init SetSensorType
    m_pCommand_SetSensorType = new CCommand_VCS_Drive();
    m_pCommand_SetSensorType->InitCommand(DRIVE_SET_SENSOR_TYPE);

    //Init GetIncEncoderParameter
    m_pCommand_GetIncEncoderParameter = new CCommand_VCS_Drive();
    m_pCommand_GetIncEncoderParameter->InitCommand(DRIVE_GET_INC_ENCODER_PARAMETER);

    //Init SetIncEncoderParameter
    m_pCommand_SetIncEncoderParameter = new CCommand_VCS_Drive();
    m_pCommand_SetIncEncoderParameter->InitCommand(DRIVE_SET_INC_ENCODER_PARAMETER);

    //Init GetHallSensorParameter
    m_pCommand_GetHallSensorParameter = new CCommand_VCS_Drive();
    m_pCommand_GetHallSensorParameter->InitCommand(DRIVE_GET_HALL_SENSOR_PARAMETER);

    //Init SetHallSensorParameter
    m_pCommand_SetHallSensorParameter = new CCommand_VCS_Drive();
    m_pCommand_SetHallSensorParameter->InitCommand(DRIVE_SET_HALL_SENSOR_PARAMETER);
}

void CCommandSetConfigurationSensor_VCS_Drive::DeleteCommands()
{
    if(m_pCommand_GetEncoderParameter)
    {
        delete m_pCommand_GetEncoderParameter;
        m_pCommand_GetEncoderParameter = NULL;
    }
    if(m_pCommand_SetEncoderParameter)
    {
        delete m_pCommand_SetEncoderParameter;
        m_pCommand_SetEncoderParameter = NULL;
    }
    if(m_pCommand_GetSensorType)
    {
        delete m_pCommand_GetSensorType;
        m_pCommand_GetSensorType = NULL;
    }
    if(m_pCommand_SetSensorType)
    {
        delete m_pCommand_SetSensorType;
        m_pCommand_SetSensorType = NULL;
    }
    if(m_pCommand_GetIncEncoderParameter)
    {
        delete m_pCommand_GetIncEncoderParameter;
        m_pCommand_GetIncEncoderParameter = NULL;
    }
    if(m_pCommand_SetIncEncoderParameter)
    {
        delete m_pCommand_SetIncEncoderParameter;
        m_pCommand_SetIncEncoderParameter = NULL;
    }
    if(m_pCommand_GetHallSensorParameter)
    {
        delete m_pCommand_GetHallSensorParameter;
        m_pCommand_GetHallSensorParameter = NULL;
    }
    if(m_pCommand_SetHallSensorParameter)
    {
        delete m_pCommand_SetHallSensorParameter;
        m_pCommand_SetHallSensorParameter = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetConfigurationSensor_VCS_Drive::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
{
    CXXMLFile::CElement* pElement = NULL;
    BOOL oCheckVisibility = FALSE;

    if(p_pFile && p_pParentElement)
    {
        //CommandSet Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "CommandSubSet");
        pElement->SetAt("Name", m_strCommandSetName);

        //Command Elements
        if(m_pCommand_GetEncoderParameter && !m_pCommand_GetEncoderParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetSensorType && !m_pCommand_GetSensorType->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetIncEncoderParameter && !m_pCommand_GetIncEncoderParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetHallSensorParameter && !m_pCommand_GetHallSensorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetEncoderParameter && !m_pCommand_SetEncoderParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetSensorType && !m_pCommand_SetSensorType->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetIncEncoderParameter && !m_pCommand_SetIncEncoderParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetHallSensorParameter && !m_pCommand_SetHallSensorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetConfigurationSensor_VCS_Drive::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetEncoderParameter) m_pCommand_GetEncoderParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetEncoderParameter) m_pCommand_SetEncoderParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetSensorType) m_pCommand_GetSensorType->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetSensorType) m_pCommand_SetSensorType->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetIncEncoderParameter) m_pCommand_GetIncEncoderParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetIncEncoderParameter) m_pCommand_SetIncEncoderParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetHallSensorParameter) m_pCommand_GetHallSensorParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetHallSensorParameter) m_pCommand_SetHallSensorParameter->InitJournalManager(p_pJournalManager);
}

void CCommandSetConfigurationSensor_VCS_Drive::ResetJournalManager()
{
    if(m_pCommand_GetEncoderParameter) m_pCommand_GetEncoderParameter->ResetJournalManager();
    if(m_pCommand_SetEncoderParameter) m_pCommand_SetEncoderParameter->ResetJournalManager();
    if(m_pCommand_GetSensorType) m_pCommand_GetSensorType->ResetJournalManager();
    if(m_pCommand_SetSensorType) m_pCommand_SetSensorType->ResetJournalManager();
    if(m_pCommand_GetIncEncoderParameter) m_pCommand_GetIncEncoderParameter->ResetJournalManager();
    if(m_pCommand_SetIncEncoderParameter) m_pCommand_SetIncEncoderParameter->ResetJournalManager();
    if(m_pCommand_GetHallSensorParameter) m_pCommand_GetHallSensorParameter->ResetJournalManager();
    if(m_pCommand_SetHallSensorParameter) m_pCommand_SetHallSensorParameter->ResetJournalManager();
}

BOOL CCommandSetConfigurationSensor_VCS_Drive::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetEncoderParameter && !m_pCommand_GetEncoderParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetEncoderParameter && !m_pCommand_SetEncoderParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetSensorType && !m_pCommand_GetSensorType->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetSensorType && !m_pCommand_SetSensorType->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetIncEncoderParameter && !m_pCommand_GetIncEncoderParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetIncEncoderParameter && !m_pCommand_SetIncEncoderParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetHallSensorParameter && !m_pCommand_GetHallSensorParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetHallSensorParameter && !m_pCommand_SetHallSensorParameter->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

