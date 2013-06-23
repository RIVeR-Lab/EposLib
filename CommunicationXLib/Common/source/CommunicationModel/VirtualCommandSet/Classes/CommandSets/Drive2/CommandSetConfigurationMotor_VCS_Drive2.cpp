#include "stdafx.h"
#include "CommandSetConfigurationMotor_VCS_Drive2.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive2.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetConfigurationMotor_VCS_Drive2::CCommandSetConfigurationMotor_VCS_Drive2()
{
    m_strCommandSetName = COMMAND_SET_MOTOR_CONFIGURATION;

    m_pCommand_GetMotorParameter = NULL;
    m_pCommand_SetMotorParameter = NULL;
    m_pCommand_SetMotorType = NULL;
    m_pCommand_GetMotorType = NULL;
    m_pCommand_SetDcMotorParameter = NULL;
    m_pCommand_GetDcMotorParameter = NULL;
    m_pCommand_SetEcMotorParameter = NULL;
    m_pCommand_GetEcMotorParameter = NULL;

    InitCommands();
}

CCommandSetConfigurationMotor_VCS_Drive2::~CCommandSetConfigurationMotor_VCS_Drive2()
{
    DeleteCommands();
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_SetMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usMotorType, WORD p_usNominalCurrent, WORD p_usMaxOutputCurrent, BYTE p_ubNbOfPolePairs, WORD p_usThermalTimeConstant, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetMotorParameter)
    {
        //Set Parameter Data
        m_pCommand_SetMotorParameter->ResetStatus();
        m_pCommand_SetMotorParameter->SetParameterData(0, &p_usMotorType, sizeof(p_usMotorType));
        m_pCommand_SetMotorParameter->SetParameterData(1, &p_usNominalCurrent, sizeof(p_usNominalCurrent));
        m_pCommand_SetMotorParameter->SetParameterData(2, &p_usMaxOutputCurrent, sizeof(p_usMaxOutputCurrent));
        m_pCommand_SetMotorParameter->SetParameterData(3, &p_ubNbOfPolePairs, sizeof(p_ubNbOfPolePairs));
        m_pCommand_SetMotorParameter->SetParameterData(4, &p_usThermalTimeConstant, sizeof(p_usThermalTimeConstant));

        //ExecuteCommand
        oResult = m_pCommand_SetMotorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetMotorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_GetMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusMotorType, WORD *p_pusNominalCurrent, WORD *p_pusMaxOutputCurrent, BYTE *p_pubNbOfPolePairs, WORD *p_pusThermalTimeConstant, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetMotorParameter)
    {
        //Set Parameter Data
        m_pCommand_GetMotorParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetMotorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetMotorParameter->GetReturnParameterData(0, p_pusMotorType, sizeof(*p_pusMotorType));
        m_pCommand_GetMotorParameter->GetReturnParameterData(1, p_pusNominalCurrent, sizeof(*p_pusNominalCurrent));
        m_pCommand_GetMotorParameter->GetReturnParameterData(2, p_pusMaxOutputCurrent, sizeof(*p_pusMaxOutputCurrent));
        m_pCommand_GetMotorParameter->GetReturnParameterData(3, p_pubNbOfPolePairs, sizeof(*p_pubNbOfPolePairs));
        m_pCommand_GetMotorParameter->GetReturnParameterData(4, p_pusThermalTimeConstant, sizeof(*p_pusThermalTimeConstant));

        //Get ErrorCode
        m_pCommand_GetMotorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_SetMotorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usMotorType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetMotorType)
    {
        //Set Parameter Data
        m_pCommand_SetMotorType->ResetStatus();
        m_pCommand_SetMotorType->SetParameterData(0, &p_usMotorType, sizeof(p_usMotorType));

        //ExecuteCommand
        oResult = m_pCommand_SetMotorType->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetMotorType->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_GetMotorType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusMotorType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetMotorType)
    {
        //Set Parameter Data
        m_pCommand_GetMotorType->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetMotorType->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetMotorType->GetReturnParameterData(0, p_pusMotorType, sizeof(*p_pusMotorType));

        //Get ErrorCode
        m_pCommand_GetMotorType->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_SetDcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNominalCurrent, WORD p_usMaxOutputCurrent, WORD p_usThermalTimeConstant, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetDcMotorParameter)
    {
        //Set Parameter Data
        m_pCommand_SetDcMotorParameter->ResetStatus();
        m_pCommand_SetDcMotorParameter->SetParameterData(0, &p_usNominalCurrent, sizeof(p_usNominalCurrent));
        m_pCommand_SetDcMotorParameter->SetParameterData(1, &p_usMaxOutputCurrent, sizeof(p_usMaxOutputCurrent));
        m_pCommand_SetDcMotorParameter->SetParameterData(2, &p_usThermalTimeConstant, sizeof(p_usThermalTimeConstant));

        //ExecuteCommand
        oResult = m_pCommand_SetDcMotorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetDcMotorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_GetDcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusNominalCurrent, WORD *p_pusMaxOutputCurrent, WORD *p_pusThermalTimeConstant, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetDcMotorParameter)
    {
        //Set Parameter Data
        m_pCommand_GetDcMotorParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetDcMotorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetDcMotorParameter->GetReturnParameterData(0, p_pusNominalCurrent, sizeof(*p_pusNominalCurrent));
        m_pCommand_GetDcMotorParameter->GetReturnParameterData(1, p_pusMaxOutputCurrent, sizeof(*p_pusMaxOutputCurrent));
        m_pCommand_GetDcMotorParameter->GetReturnParameterData(2, p_pusThermalTimeConstant, sizeof(*p_pusThermalTimeConstant));

        //Get ErrorCode
        m_pCommand_GetDcMotorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_SetEcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usNominalCurrent, WORD p_usMaxOutputCurrent, WORD p_usThermalTimeConstant, BYTE p_ubNbOfPolePair, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetEcMotorParameter)
    {
        //Set Parameter Data
        m_pCommand_SetEcMotorParameter->ResetStatus();
        m_pCommand_SetEcMotorParameter->SetParameterData(0, &p_usNominalCurrent, sizeof(p_usNominalCurrent));
        m_pCommand_SetEcMotorParameter->SetParameterData(1, &p_usMaxOutputCurrent, sizeof(p_usMaxOutputCurrent));
        m_pCommand_SetEcMotorParameter->SetParameterData(2, &p_usThermalTimeConstant, sizeof(p_usThermalTimeConstant));
        m_pCommand_SetEcMotorParameter->SetParameterData(3, &p_ubNbOfPolePair, sizeof(p_ubNbOfPolePair));

        //ExecuteCommand
        oResult = m_pCommand_SetEcMotorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetEcMotorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::VCS_GetEcMotorParameter(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusNominalCurrent, WORD *p_pusMaxOutputCurrent, WORD *p_pusThermalTimeConstant, BYTE *p_pubNbOfPolePair, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetEcMotorParameter)
    {
        //Set Parameter Data
        m_pCommand_GetEcMotorParameter->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetEcMotorParameter->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetEcMotorParameter->GetReturnParameterData(0, p_pusNominalCurrent, sizeof(*p_pusNominalCurrent));
        m_pCommand_GetEcMotorParameter->GetReturnParameterData(1, p_pusMaxOutputCurrent, sizeof(*p_pusMaxOutputCurrent));
        m_pCommand_GetEcMotorParameter->GetReturnParameterData(2, p_pusThermalTimeConstant, sizeof(*p_pusThermalTimeConstant));
        m_pCommand_GetEcMotorParameter->GetReturnParameterData(3, p_pubNbOfPolePair, sizeof(*p_pubNbOfPolePair));

        //Get ErrorCode
        m_pCommand_GetEcMotorParameter->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetConfigurationMotor_VCS_Drive2::InitCommands()
{
    DeleteCommands();

    //Init SetMotorParameter
    m_pCommand_SetMotorParameter = new CCommand_VCS_Drive2();
    m_pCommand_SetMotorParameter->InitCommand(DRIVE2_SET_MOTOR_PARAMETER);

    //Init GetMotorParameter
    m_pCommand_GetMotorParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetMotorParameter->InitCommand(DRIVE2_GET_MOTOR_PARAMETER);

    //Init SetMotorType
    m_pCommand_SetMotorType = new CCommand_VCS_Drive2();
    m_pCommand_SetMotorType->InitCommand(DRIVE2_SET_MOTOR_TYPE);

    //Init GetMotorType
    m_pCommand_GetMotorType = new CCommand_VCS_Drive2();
    m_pCommand_GetMotorType->InitCommand(DRIVE2_GET_MOTOR_TYPE);

    //Init SetDcMotorParameter
    m_pCommand_SetDcMotorParameter = new CCommand_VCS_Drive2();
    m_pCommand_SetDcMotorParameter->InitCommand(DRIVE2_SET_DC_MOTOR_PARAMETER);

    //Init GetDcMotorParameter
    m_pCommand_GetDcMotorParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetDcMotorParameter->InitCommand(DRIVE2_GET_DC_MOTOR_PARAMETER);

    //Init SetEcMotorParameter
    m_pCommand_SetEcMotorParameter = new CCommand_VCS_Drive2();
    m_pCommand_SetEcMotorParameter->InitCommand(DRIVE2_SET_EC_MOTOR_PARAMETER);

    //Init GetEcMotorParameter
    m_pCommand_GetEcMotorParameter = new CCommand_VCS_Drive2();
    m_pCommand_GetEcMotorParameter->InitCommand(DRIVE2_GET_EC_MOTOR_PARAMETER);
}

void CCommandSetConfigurationMotor_VCS_Drive2::DeleteCommands()
{
    if(m_pCommand_SetMotorParameter)
    {
        delete m_pCommand_SetMotorParameter;
        m_pCommand_SetMotorParameter = NULL;
    }
    if(m_pCommand_GetMotorParameter)
    {
        delete m_pCommand_GetMotorParameter;
        m_pCommand_GetMotorParameter = NULL;
    }
    if(m_pCommand_SetMotorType)
    {
        delete m_pCommand_SetMotorType;
        m_pCommand_SetMotorType = NULL;
    }
    if(m_pCommand_GetMotorType)
    {
        delete m_pCommand_GetMotorType;
        m_pCommand_GetMotorType = NULL;
    }
    if(m_pCommand_SetDcMotorParameter)
    {
        delete m_pCommand_SetDcMotorParameter;
        m_pCommand_SetDcMotorParameter = NULL;
    }
    if(m_pCommand_GetDcMotorParameter)
    {
        delete m_pCommand_GetDcMotorParameter;
        m_pCommand_GetDcMotorParameter = NULL;
    }
    if(m_pCommand_SetEcMotorParameter)
    {
        delete m_pCommand_SetEcMotorParameter;
        m_pCommand_SetEcMotorParameter = NULL;
    }
    if(m_pCommand_GetEcMotorParameter)
    {
        delete m_pCommand_GetEcMotorParameter;
        m_pCommand_GetEcMotorParameter = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetConfigurationMotor_VCS_Drive2::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetMotorParameter && !m_pCommand_GetMotorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetMotorType && !m_pCommand_GetMotorType->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetDcMotorParameter && !m_pCommand_GetDcMotorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetEcMotorParameter && !m_pCommand_GetEcMotorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetMotorParameter && !m_pCommand_SetMotorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetMotorType && !m_pCommand_SetMotorType->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetDcMotorParameter && !m_pCommand_SetDcMotorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetEcMotorParameter && !m_pCommand_SetEcMotorParameter->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetConfigurationMotor_VCS_Drive2::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_SetMotorParameter) m_pCommand_SetMotorParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetMotorParameter) m_pCommand_GetMotorParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetMotorType) m_pCommand_SetMotorType->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetMotorType) m_pCommand_GetMotorType->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetDcMotorParameter) m_pCommand_SetDcMotorParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetDcMotorParameter) m_pCommand_GetDcMotorParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetEcMotorParameter) m_pCommand_SetEcMotorParameter->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetEcMotorParameter) m_pCommand_GetEcMotorParameter->InitJournalManager(p_pJournalManager);
}

void CCommandSetConfigurationMotor_VCS_Drive2::ResetJournalManager()
{
    if(m_pCommand_GetMotorParameter) m_pCommand_GetMotorParameter->ResetJournalManager();
    if(m_pCommand_SetMotorParameter) m_pCommand_SetMotorParameter->ResetJournalManager();
    if(m_pCommand_SetMotorType) m_pCommand_SetMotorType->ResetJournalManager();
    if(m_pCommand_GetMotorType) m_pCommand_GetMotorType->ResetJournalManager();
    if(m_pCommand_SetDcMotorParameter) m_pCommand_SetDcMotorParameter->ResetJournalManager();
    if(m_pCommand_GetDcMotorParameter) m_pCommand_GetDcMotorParameter->ResetJournalManager();
    if(m_pCommand_SetEcMotorParameter) m_pCommand_SetEcMotorParameter->ResetJournalManager();
    if(m_pCommand_GetEcMotorParameter) m_pCommand_GetEcMotorParameter->ResetJournalManager();
}

BOOL CCommandSetConfigurationMotor_VCS_Drive2::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetMotorParameter && !m_pCommand_GetMotorParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetMotorParameter && !m_pCommand_SetMotorParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetMotorType && !m_pCommand_SetMotorType->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetMotorType && !m_pCommand_GetMotorType->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetDcMotorParameter && !m_pCommand_SetDcMotorParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetDcMotorParameter && !m_pCommand_GetDcMotorParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetEcMotorParameter && !m_pCommand_SetEcMotorParameter->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetEcMotorParameter && !m_pCommand_GetEcMotorParameter->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

