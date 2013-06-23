// CommandSetVersion_VCS_Common.cpp: Implementierung der Klasse CCommandSetVersion_VCS_Common.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/VirtualCommandSet/Command_VCS_Common.h>
#include "CommandSetVersion_VCS_Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CCommandSetVersion_VCS_Common::CCommandSetVersion_VCS_Common()
{
    m_strCommandSetName = COMMAND_SET_VERSION;

    m_pCommand_GetVersion = NULL;
    m_pCommand_GetDeviceType = NULL;
    
    InitCommands();
}

CCommandSetVersion_VCS_Common::~CCommandSetVersion_VCS_Common()
{
    DeleteCommands();
}

BOOL CCommandSetVersion_VCS_Common::VCS_GetVersion(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, WORD* p_pulHardwareVersion, WORD* p_pulSoftwareVersion, WORD* p_pulApplicationNumber, WORD* p_pulApplicationVersion, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetVersion)
    {
        //Set Parameter Data
        m_pCommand_GetVersion->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetVersion->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetVersion->GetReturnParameterData(0, p_pulHardwareVersion, sizeof(*p_pulHardwareVersion));
        m_pCommand_GetVersion->GetReturnParameterData(1, p_pulSoftwareVersion, sizeof(*p_pulSoftwareVersion));
        m_pCommand_GetVersion->GetReturnParameterData(2, p_pulApplicationNumber, sizeof(*p_pulApplicationNumber));
        m_pCommand_GetVersion->GetReturnParameterData(3, p_pulApplicationVersion, sizeof(*p_pulApplicationVersion));

        //Get ErrorCode
        m_pCommand_GetVersion->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

BOOL CCommandSetVersion_VCS_Common::VCS_GetDeviceType(CLayerManagerBase* p_pManager, HANDLE p_hHandle, HANDLE p_hTransactionHandle, DWORD* p_pulDeviceType, CErrorInfo* p_pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pCommand_GetDeviceType)
    {
        //Set Parameter Data
        m_pCommand_GetDeviceType->ResetStatus();

        //ExecuteCommand
        oResult = m_pCommand_GetDeviceType->Execute(p_pManager, p_hHandle, p_hTransactionHandle);

        //Get ReturnParameter Data
        m_pCommand_GetDeviceType->GetReturnParameterData(0, p_pulDeviceType, sizeof(*p_pulDeviceType));
        
        //Get ErrorCode
        m_pCommand_GetDeviceType->GetErrorInfo(p_pErrorInfo);
    }

    return oResult;
}

void CCommandSetVersion_VCS_Common::InitCommands()
{
    DeleteCommands();

    //Init GetVersion
    m_pCommand_GetVersion = new CCommand_VCS_Common();
    m_pCommand_GetVersion->InitCommand(COMMON_GET_VERSION);

	//Init GetDeviceType
    m_pCommand_GetDeviceType = new CCommand_VCS_Common();
    m_pCommand_GetDeviceType->InitCommand(COMMON_GET_DEVICE_TYPE);
    
}

void CCommandSetVersion_VCS_Common::DeleteCommands()
{
    if(m_pCommand_GetVersion)
    {
        delete m_pCommand_GetVersion;
        m_pCommand_GetVersion = NULL;
    }

	if(m_pCommand_GetDeviceType)
    {
        delete m_pCommand_GetDeviceType;
        m_pCommand_GetDeviceType = NULL;
    }
    
}

CXXMLFile::CElementPart* CCommandSetVersion_VCS_Common::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElementPart* p_pParentElement)
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
        if(m_pCommand_GetVersion && !m_pCommand_GetVersion->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
        if(m_pCommand_GetDeviceType && !m_pCommand_GetDeviceType->StoreToXMLFile(p_pFile, pElement, oCheckVisibility)) return FALSE;
    }

    return pElement;
}

void CCommandSetVersion_VCS_Common::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    if(m_pCommand_GetVersion) m_pCommand_GetVersion->InitJournalManager(p_pJournalManager);
    if(m_pCommand_GetDeviceType) m_pCommand_GetDeviceType->InitJournalManager(p_pJournalManager);
}

void CCommandSetVersion_VCS_Common::ResetJournalManager()
{
    if(m_pCommand_GetVersion) m_pCommand_GetVersion->ResetJournalManager();
    if(m_pCommand_GetDeviceType) m_pCommand_GetDeviceType->ResetJournalManager();
}

BOOL CCommandSetVersion_VCS_Common::InitGateway(CGateway *p_pGateway)
{
    if(m_pCommand_GetVersion && !m_pCommand_GetVersion->InitGateway(p_pGateway)) return FALSE;
    if(m_pCommand_GetDeviceType && !m_pCommand_GetDeviceType->InitGateway(p_pGateway)) return FALSE;

    return TRUE;
}
