// CommandSetConfiguration_VCS_Drive.cpp: Implementierung der Klasse CCommandSetConfiguration_VCS_Drive.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CommandSetConfiguration_VCS_Drive.h"

#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Drive.h>
#include "CommandSetConfigurationMotor_VCS_Drive.h"
#include "CommandSetConfigurationSensor_VCS_Drive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetConfiguration_VCS_Drive::CCommandSetConfiguration_VCS_Drive()
{
    m_strCommandSetName = COMMAND_SET_CONFIGURATION;

    m_pCommand_GetCurrentRegulatorGain = NULL;
    m_pCommand_GetPositionRegulatorGain = NULL;
    m_pCommand_GetVelocityRegulatorGain = NULL;
    m_pCommand_GetMaxFollowingError = NULL;
    m_pCommand_GetMaxProfileVelocity = NULL;
    m_pCommand_SetCurrentRegulatorGain = NULL;
    m_pCommand_SetPositionRegulatorGain = NULL;
    m_pCommand_SetVelocityRegulatorGain = NULL;
    m_pCommand_SetMaxFollowingError = NULL;
    m_pCommand_SetMaxProfileVelocity = NULL;

    InitCommands();
    FillSetList();
}

CCommandSetConfiguration_VCS_Drive::~CCommandSetConfiguration_VCS_Drive()
{
    DeleteCommands();
    DeleteSetList();
}

void CCommandSetConfiguration_VCS_Drive::FillSetList()
{
    m_pCommandSetConfigurationMotor         = new CCommandSetConfigurationMotor_VCS_Drive();
    m_pCommandSetConfigurationSensor        = new CCommandSetConfigurationSensor_VCS_Drive();
}

void CCommandSetConfiguration_VCS_Drive::DeleteSetList()
{
    if(m_pCommandSetConfigurationMotor)
    {
        delete m_pCommandSetConfigurationMotor;
        m_pCommandSetConfigurationMotor = NULL;
    }

    if(m_pCommandSetConfigurationSensor)
    {
        delete m_pCommandSetConfigurationSensor;
        m_pCommandSetConfigurationSensor = NULL;
    }
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_GetCurrentRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusP, WORD* p_pusI, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetCurrentRegulatorGain)
    {
        //Set Parameter Data
        m_pCommand_GetCurrentRegulatorGain->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetCurrentRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetCurrentRegulatorGain->GetReturnParameterData(0, p_pusP, sizeof(*p_pusP));
        m_pCommand_GetCurrentRegulatorGain->GetReturnParameterData(1, p_pusI, sizeof(*p_pusI));

        //Get ErrorCode
        m_pCommand_GetCurrentRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_GetPositionRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusP, WORD* p_pusI, WORD* p_pusD, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetPositionRegulatorGain)
    {
        //Set ParameterData
        m_pCommand_GetPositionRegulatorGain->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetPositionRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetPositionRegulatorGain->GetReturnParameterData(0, p_pusP, sizeof(*p_pusP));
        m_pCommand_GetPositionRegulatorGain->GetReturnParameterData(1, p_pusI, sizeof(*p_pusI));
        m_pCommand_GetPositionRegulatorGain->GetReturnParameterData(2, p_pusD, sizeof(*p_pusD));

        //Get ErrorCode
        m_pCommand_GetPositionRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_GetPositionRegulatorFeedForward(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD *p_pusVelocityFeedForward, WORD *p_pusAccelerationFeedForward, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetPositionRegulatorGain)
    {
        //Set ParameterData
        m_pCommand_GetPositionRegulatorGain->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetPositionRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetPositionRegulatorGain->GetReturnParameterData(0, p_pusVelocityFeedForward, sizeof(*p_pusVelocityFeedForward));
        m_pCommand_GetPositionRegulatorGain->GetReturnParameterData(1, p_pusAccelerationFeedForward, sizeof(*p_pusAccelerationFeedForward));

        //Get ErrorCode
        m_pCommand_GetPositionRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_GetVelocityRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pusP, WORD* p_pusI, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetVelocityRegulatorGain)
    {
        //Set Parameter Data
        m_pCommand_GetVelocityRegulatorGain->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetVelocityRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetVelocityRegulatorGain->GetReturnParameterData(0, p_pusP, sizeof(*p_pusP));
        m_pCommand_GetVelocityRegulatorGain->GetReturnParameterData(1, p_pusI, sizeof(*p_pusI));

        //Get ErrorCode
        m_pCommand_GetVelocityRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_GetMaxFollowingError(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulMaxFollowingError, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetMaxFollowingError)
    {
        //Set Parameter Data
        m_pCommand_GetMaxFollowingError->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetMaxFollowingError->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetMaxFollowingError->GetReturnParameterData(0, p_pulMaxFollowingError, sizeof(*p_pulMaxFollowingError));

        //Get ErrorCode
        m_pCommand_GetMaxFollowingError->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}
BOOL CCommandSetConfiguration_VCS_Drive::VCS_GetMaxProfileVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulMaxProfileVelocity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetMaxProfileVelocity)
    {
        //Set Parameter Data
        m_pCommand_GetMaxProfileVelocity->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetMaxProfileVelocity->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetMaxProfileVelocity->GetReturnParameterData(0, p_pulMaxProfileVelocity, sizeof(*p_pulMaxProfileVelocity));

        //Get ErrorCode
        m_pCommand_GetMaxProfileVelocity->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_SetCurrentRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usP, WORD p_usI, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetCurrentRegulatorGain)
    {
        //Set Parameter Data
        m_pCommand_SetCurrentRegulatorGain->ResetStatus();
        m_pCommand_SetCurrentRegulatorGain->SetParameterData(0, &p_usP, sizeof(p_usP));
        m_pCommand_SetCurrentRegulatorGain->SetParameterData(1, &p_usI, sizeof(p_usI));

        //ExecuteCommand
        oResult = m_pCommand_SetCurrentRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data

        //Get ErrorCode
        m_pCommand_SetCurrentRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_SetPositionRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usP, WORD p_usI, WORD p_usD, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetPositionRegulatorGain)
    {
        //Set Parameter Data
        m_pCommand_SetPositionRegulatorGain->ResetStatus();
        m_pCommand_SetPositionRegulatorGain->SetParameterData(0, &p_usP, sizeof(p_usP));
        m_pCommand_SetPositionRegulatorGain->SetParameterData(1, &p_usI, sizeof(p_usI));
        m_pCommand_SetPositionRegulatorGain->SetParameterData(2, &p_usD, sizeof(p_usD));

        //ExecuteCommand
        oResult = m_pCommand_SetPositionRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetPositionRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}
BOOL CCommandSetConfiguration_VCS_Drive::VCS_SetPositionRegulatorFeedForward(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usVelocityFeedForward, WORD p_usAccelerationFeedForward, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetPositionRegulatorGain)
    {
        //Set Parameter Data
        m_pCommand_SetPositionRegulatorGain->ResetStatus();
        m_pCommand_SetPositionRegulatorGain->SetParameterData(0, &p_usVelocityFeedForward, sizeof(p_usVelocityFeedForward));
        m_pCommand_SetPositionRegulatorGain->SetParameterData(1, &p_usAccelerationFeedForward, sizeof(p_usAccelerationFeedForward));

        //ExecuteCommand
        oResult = m_pCommand_SetPositionRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetPositionRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_SetVelocityRegulatorGain(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD p_usP, WORD p_usI, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetVelocityRegulatorGain)
    {
        //Set Parameter Data
        m_pCommand_SetVelocityRegulatorGain->ResetStatus();
        m_pCommand_SetVelocityRegulatorGain->SetParameterData(0, &p_usP, sizeof(p_usP));
        m_pCommand_SetVelocityRegulatorGain->SetParameterData(1, &p_usI, sizeof(p_usI));

        //ExecuteCommand
        oResult = m_pCommand_SetVelocityRegulatorGain->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetVelocityRegulatorGain->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_SetMaxFollowingError(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulMaxFollowingError, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetMaxFollowingError)
    {
        //Set Parameter Data
        m_pCommand_SetMaxFollowingError->ResetStatus();
        m_pCommand_SetMaxFollowingError->SetParameterData(0, &p_ulMaxFollowingError, sizeof(p_ulMaxFollowingError));

        //ExecuteCommand
        oResult = m_pCommand_SetMaxFollowingError->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetMaxFollowingError->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetConfiguration_VCS_Drive::VCS_SetMaxProfileVelocity(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD p_ulMaxProfileVelocity, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_SetMaxProfileVelocity)
    {
        //Set Parameter Data
        m_pCommand_SetMaxProfileVelocity->ResetStatus();
        m_pCommand_SetMaxProfileVelocity->SetParameterData(0, &p_ulMaxProfileVelocity, sizeof(p_ulMaxProfileVelocity));

        //ExecuteCommand
        oResult = m_pCommand_SetMaxProfileVelocity->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ErrorCode
        m_pCommand_SetMaxProfileVelocity->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetConfiguration_VCS_Drive::InitCommands()
{
    DeleteCommands();

    //Init GetCurrentRegualtorGain
    m_pCommand_GetCurrentRegulatorGain = new CCommand_VCS_Drive();
    m_pCommand_GetCurrentRegulatorGain->InitCommand(DRIVE_GET_CURRENT_REGULATOR_GAIN);

    //Init GetPositionRegulatorGain
    m_pCommand_GetPositionRegulatorGain = new CCommand_VCS_Drive();
    m_pCommand_GetPositionRegulatorGain->InitCommand(DRIVE_GET_POSITION_REGULATOR_GAIN);

    //Init GetVelocityRegulatorGain
    m_pCommand_GetVelocityRegulatorGain = new CCommand_VCS_Drive();
    m_pCommand_GetVelocityRegulatorGain->InitCommand(DRIVE_GET_VELOCITY_REGULATOR_GAIN);

    //Init GetMaxFollowingError
    m_pCommand_GetMaxFollowingError = new CCommand_VCS_Drive();
    m_pCommand_GetMaxFollowingError->InitCommand(DRIVE_GET_MAX_FOLLOWING_ERROR);

    //Init GetMaxProfileVelocity
    m_pCommand_GetMaxProfileVelocity = new CCommand_VCS_Drive();
    m_pCommand_GetMaxProfileVelocity->InitCommand(DRIVE_GET_MAX_PROFILE_VELOCITY);

    //Init SetCurrentRegulatorGain
    m_pCommand_SetCurrentRegulatorGain = new CCommand_VCS_Drive();
    m_pCommand_SetCurrentRegulatorGain->InitCommand(DRIVE_SET_CURRENT_REGULATOR_GAIN);

    //Init SetPositionRegulatorGain
    m_pCommand_SetPositionRegulatorGain = new CCommand_VCS_Drive();
    m_pCommand_SetPositionRegulatorGain->InitCommand(DRIVE_SET_POSITION_REGULATOR_GAIN);

    //Init SetVelocityRegualtorGain
    m_pCommand_SetVelocityRegulatorGain = new CCommand_VCS_Drive();
    m_pCommand_SetVelocityRegulatorGain->InitCommand(DRIVE_SET_VELOCITY_REGULATOR_GAIN);

    //Init SetMaxFollowingError
    m_pCommand_SetMaxFollowingError = new CCommand_VCS_Drive();
    m_pCommand_SetMaxFollowingError->InitCommand(DRIVE_SET_MAX_FOLLOWING_ERROR);

    //Init SetMaxProfileVelocity
    m_pCommand_SetMaxProfileVelocity = new CCommand_VCS_Drive();
    m_pCommand_SetMaxProfileVelocity->InitCommand(DRIVE_SET_MAX_PROFILE_VELOCITY);

}

void CCommandSetConfiguration_VCS_Drive::DeleteCommands()
{
    if(m_pCommand_GetCurrentRegulatorGain)
    {
        delete m_pCommand_GetCurrentRegulatorGain;
        m_pCommand_GetCurrentRegulatorGain = NULL;
    }
    if(m_pCommand_GetPositionRegulatorGain)
    {
        delete m_pCommand_GetPositionRegulatorGain;
        m_pCommand_GetPositionRegulatorGain = NULL;
    }
    if(m_pCommand_GetVelocityRegulatorGain)
    {
        delete m_pCommand_GetVelocityRegulatorGain;
        m_pCommand_GetVelocityRegulatorGain = NULL;
    }
    if(m_pCommand_GetMaxFollowingError)
    {
        delete m_pCommand_GetMaxFollowingError;
        m_pCommand_GetMaxFollowingError = NULL;
    }
    if(m_pCommand_GetMaxProfileVelocity)
    {
        delete m_pCommand_GetMaxProfileVelocity;
        m_pCommand_GetMaxProfileVelocity = NULL;
    }
    if(m_pCommand_SetCurrentRegulatorGain)
    {
        delete m_pCommand_SetCurrentRegulatorGain;
        m_pCommand_SetCurrentRegulatorGain = NULL;
    }
    if(m_pCommand_SetPositionRegulatorGain)
    {
        delete m_pCommand_SetPositionRegulatorGain;
        m_pCommand_SetPositionRegulatorGain = NULL;
    }
    if(m_pCommand_SetVelocityRegulatorGain)
    {
        delete m_pCommand_SetVelocityRegulatorGain;
        m_pCommand_SetVelocityRegulatorGain = NULL;
    }
    if(m_pCommand_SetMaxFollowingError)
    {
        delete m_pCommand_SetMaxFollowingError;
        m_pCommand_SetMaxFollowingError = NULL;
    }
    if(m_pCommand_SetMaxProfileVelocity)
    {
        delete m_pCommand_SetMaxProfileVelocity;
        m_pCommand_SetMaxProfileVelocity = NULL;
    }
}

CXXMLFile::CElementPart* CCommandSetConfiguration_VCS_Drive::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetCurrentRegulatorGain && !m_pCommand_GetCurrentRegulatorGain->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetPositionRegulatorGain && !m_pCommand_GetPositionRegulatorGain->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetVelocityRegulatorGain && !m_pCommand_GetVelocityRegulatorGain->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetMaxFollowingError && !m_pCommand_GetMaxFollowingError->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetMaxProfileVelocity && !m_pCommand_GetMaxProfileVelocity->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetCurrentRegulatorGain && !m_pCommand_SetCurrentRegulatorGain->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetPositionRegulatorGain && !m_pCommand_SetPositionRegulatorGain->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetVelocityRegulatorGain && !m_pCommand_SetVelocityRegulatorGain->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetMaxFollowingError && !m_pCommand_SetMaxFollowingError->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_SetMaxProfileVelocity && !m_pCommand_SetMaxProfileVelocity->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;

        //Command Group
        if(m_pCommandSetConfigurationMotor && !m_pCommandSetConfigurationMotor->StoreToXMLFile(p_pFile, pElement)) return pElement;
        if(m_pCommandSetConfigurationSensor && !m_pCommandSetConfigurationSensor->StoreToXMLFile(p_pFile, pElement)) return pElement;
    }

    return pElement;
}

void CCommandSetConfiguration_VCS_Drive::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetCurrentRegulatorGain) m_pCommand_GetCurrentRegulatorGain->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetPositionRegulatorGain) m_pCommand_GetPositionRegulatorGain->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetVelocityRegulatorGain) m_pCommand_GetVelocityRegulatorGain->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetMaxFollowingError) m_pCommand_GetMaxFollowingError->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetMaxProfileVelocity) m_pCommand_GetMaxProfileVelocity->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetCurrentRegulatorGain) m_pCommand_SetCurrentRegulatorGain->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetPositionRegulatorGain) m_pCommand_SetPositionRegulatorGain->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetVelocityRegulatorGain) m_pCommand_SetVelocityRegulatorGain->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetMaxFollowingError) m_pCommand_SetMaxFollowingError->InitJournalManager(p_pJournalManager);
    if(m_pCommand_SetMaxProfileVelocity) m_pCommand_SetMaxProfileVelocity->InitJournalManager(p_pJournalManager);

    if(m_pCommandSetConfigurationMotor) m_pCommandSetConfigurationMotor->InitJournalManager(p_pJournalManager);
    if(m_pCommandSetConfigurationSensor) m_pCommandSetConfigurationSensor->InitJournalManager(p_pJournalManager);
}

void CCommandSetConfiguration_VCS_Drive::ResetJournalManager()
{
    if(m_pCommand_GetCurrentRegulatorGain) m_pCommand_GetCurrentRegulatorGain->ResetJournalManager();
    if(m_pCommand_GetPositionRegulatorGain) m_pCommand_GetPositionRegulatorGain->ResetJournalManager();
    if(m_pCommand_GetVelocityRegulatorGain) m_pCommand_GetVelocityRegulatorGain->ResetJournalManager();
    if(m_pCommand_GetMaxFollowingError) m_pCommand_GetMaxFollowingError->ResetJournalManager();
    if(m_pCommand_GetMaxProfileVelocity) m_pCommand_GetMaxProfileVelocity->ResetJournalManager();
    if(m_pCommand_SetCurrentRegulatorGain) m_pCommand_SetCurrentRegulatorGain->ResetJournalManager();
    if(m_pCommand_SetPositionRegulatorGain) m_pCommand_SetPositionRegulatorGain->ResetJournalManager();
    if(m_pCommand_SetVelocityRegulatorGain) m_pCommand_SetVelocityRegulatorGain->ResetJournalManager();
    if(m_pCommand_SetMaxFollowingError) m_pCommand_SetMaxFollowingError->ResetJournalManager();
    if(m_pCommand_SetMaxProfileVelocity) m_pCommand_SetMaxProfileVelocity->ResetJournalManager();

    if(m_pCommandSetConfigurationMotor) m_pCommandSetConfigurationMotor->ResetJournalManager();
    if(m_pCommandSetConfigurationSensor) m_pCommandSetConfigurationSensor->ResetJournalManager();
}

BOOL CCommandSetConfiguration_VCS_Drive::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetCurrentRegulatorGain && !m_pCommand_GetCurrentRegulatorGain->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetPositionRegulatorGain && !m_pCommand_GetPositionRegulatorGain->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetVelocityRegulatorGain && !m_pCommand_GetVelocityRegulatorGain->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetMaxFollowingError && !m_pCommand_GetMaxFollowingError->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetMaxProfileVelocity && !m_pCommand_GetMaxProfileVelocity->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetCurrentRegulatorGain && !m_pCommand_SetCurrentRegulatorGain->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetPositionRegulatorGain && !m_pCommand_SetPositionRegulatorGain->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetVelocityRegulatorGain && !m_pCommand_SetVelocityRegulatorGain->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetMaxFollowingError && !m_pCommand_SetMaxFollowingError->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_SetMaxProfileVelocity && !m_pCommand_SetMaxProfileVelocity->InitGateway(p_pGateway)) return FALSE;

    if(m_pCommandSetConfigurationMotor && !m_pCommandSetConfigurationMotor->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommandSetConfigurationSensor && !m_pCommandSetConfigurationSensor->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}

