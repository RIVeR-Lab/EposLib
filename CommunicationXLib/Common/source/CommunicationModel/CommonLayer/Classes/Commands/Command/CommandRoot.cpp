// Command.cpp: Implementierung der Klasse CCommandRoot.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <Classes/MmcDataConversion.h>
#include <CommunicationModel/Common/Journal/Time/JournalTime.h>
#include <CommunicationModel/Common/Journal/BaseClasses/JournalManagerBase.h>
#include "ParameterInfo/ParameterInfo.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>
#include <CommunicationModel/CommonLayer/Classes/Gateway/Gateway.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/CommandRoot.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CCommandRoot::CCommandRoot()
{
    m_oTransactionEventEnabled = TRUE;
    m_pErrorInfo = 0;

    m_pParameterInfo = new CParameterInfo();
    m_pLayerParameterStack = new CLayerParameterStack();
    m_pJournalTime = new CJournalTime();

    ResetCommand();
}

CCommandRoot::~CCommandRoot()
{
    if(m_pParameterInfo)
    {
        delete m_pParameterInfo;
        m_pParameterInfo = 0;
    }

    if(m_pLayerParameterStack)
    {
        delete m_pLayerParameterStack;
        m_pLayerParameterStack = 0;
    }

    if(m_pJournalTime)
    {
        delete m_pJournalTime;
        m_pJournalTime = 0;
    }

    ResetErrorInfo();
}

void CCommandRoot::ResetErrorInfo()
{
    if(m_pErrorInfo)
    {
        delete m_pErrorInfo;
        m_pErrorInfo = 0;
    }
}

BOOL CCommandRoot::SetTimeout(DWORD p_ulTimeout)
{
    BOOL oResult = TRUE;

    m_dTimeout = p_ulTimeout;

    return oResult;
}

BOOL CCommandRoot::ResetTimeout()
{
    BOOL oResult = TRUE;

    m_dTimeout = UINT_MAX;

    return oResult;
}

DWORD CCommandRoot::GetTimeout()
{
    return m_dTimeout;
}

BOOL CCommandRoot::IsTimeoutValid()
{
    return (m_dTimeout != UINT_MAX);
}

void CCommandRoot::AddParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddParameter(p_lParameterIndex, p_ParameterName, p_eParameterType);
    }
}

void CCommandRoot::AddParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType, BOOL p_oVisible, BOOL p_oEditable)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddParameter(p_lParameterIndex, p_ParameterName, p_eParameterType, p_oVisible, p_oEditable);
    }
}

void CCommandRoot::AddParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType, DWORD p_ulArraySize)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddParameter(p_lParameterIndex, p_ParameterName, p_eParameterType, p_ulArraySize);
    }
}

void CCommandRoot::AddParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType, DWORD p_ulArraySize, BOOL p_oVisible, BOOL p_oEditable)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddParameter(p_lParameterIndex, p_ParameterName, p_eParameterType, p_oVisible, p_oEditable);
    }
}

void CCommandRoot::AddReturnParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddReturnParameter(p_lParameterIndex, p_ParameterName, p_eParameterType);
    }
}

void CCommandRoot::AddReturnParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType, BOOL p_oVisible)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddReturnParameter(p_lParameterIndex, p_ParameterName, p_eParameterType, p_oVisible);
    }
}

void CCommandRoot::AddReturnParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType, DWORD p_ulArraySize)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddReturnParameter(p_lParameterIndex, p_ParameterName, p_eParameterType, p_ulArraySize);
    }
}

void CCommandRoot::AddReturnParameter(int p_lParameterIndex, CStdString p_ParameterName, EObjectDataType p_eParameterType, DWORD p_ulArraySize, BOOL p_oVisible)
{
    if(m_pParameterInfo)
    {
        m_pParameterInfo->AddReturnParameter(p_lParameterIndex, p_ParameterName, p_eParameterType, p_oVisible);
    }
}

CCommandRoot* CCommandRoot::CloneCommand()
{
    CCommandRoot* pNewCommand;

    pNewCommand = new CCommandRoot();
    *pNewCommand = *this;

    return pNewCommand;
}

BOOL CCommandRoot::DoTransactionEvent(HANDLE p_hTransactionHandle)
{
    BOOL oResult(FALSE);

    if(p_hTransactionHandle != 0 && m_oTransactionEventEnabled)
    {
        if(m_pJournalManager)
        {
            oResult = m_pJournalManager->TransactionEvent(m_eLayer, p_hTransactionHandle, this);
        }
    }

    return oResult;
}

BOOL CCommandRoot::EnableTransactionEvent(BOOL p_oEnabled)
{
    m_oTransactionEventEnabled = p_oEnabled;
    return TRUE;
}

BOOL CCommandRoot::Execute(CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle)
{
    BOOL oResult = FALSE;

    if(m_pGateway)
    {
        //Start Time
        StartCommandTime();

        //Process Command
        oResult = m_pGateway->ProcessCommand(this, pManager, h_Handle, hTransactionHandle);

        //Stop Time
        StopCommandTime();

        //Transaction Event
        DoTransactionEvent(hTransactionHandle);

        //Update SubLayer Lock State
        m_oSubLayerRemainsLocked = m_pGateway->IsLocked();

    }

    return oResult;
}

CStdString CCommandRoot::GetCommandName()
{
    return m_CommandName;
}

ELayer CCommandRoot::GetLayer()
{
    return m_eLayer;
}

CStdString CCommandRoot::GetJournalRunTimeString()
{
    CStdString runTimeString(_T(""));

    //Actual Run Time
    if(m_pJournalTime)
    {
        runTimeString = m_pJournalTime->GetJournalRunTimeString();
    }

    return runTimeString;
}

CStdString CCommandRoot::GetCommandTimeString()
{
    CStdString timeString(_T(""));

    if(m_pJournalTime)
    {
        timeString = m_pJournalTime->GetCommandTimeString();
    }

    return timeString;
}

void CCommandRoot::SetCommandTime(CStdString* p_pCommandTime)
{
    if(m_pJournalTime)
    {
        m_pJournalTime->SetCommandTime(p_pCommandTime);
    }
}

void CCommandRoot::SetJournalRunTime(CStdString* p_pJournalRunTime)
{
    if(m_pJournalTime)
    {
        m_pJournalTime->SetJournalRunTime(p_pJournalRunTime);
    }
}

void CCommandRoot::InitCommand(CStdString p_CommandName, ELayer p_eLayer, DWORD p_ulCommandId)
{
    m_CommandName = p_CommandName;
    m_eLayer = p_eLayer;
    m_dCommandId = p_ulCommandId;
}

BOOL CCommandRoot::InitCommandName(CStdString p_CommandName)
{
    m_CommandName = p_CommandName;
    return TRUE;
}

BOOL CCommandRoot::InitLayer(CStdString p_Layer)
{
    BOOL oResult(FALSE);

    if(p_Layer.CompareNoCase(VIRTUAL_COMMAND_SET_LAYER_STRING) == 0)
    {
        m_eLayer = VIRTUAL_COMMAND_SET_LAYER;
        oResult = TRUE;
    }
    else if(p_Layer.CompareNoCase(COMMAND_SET_LAYER_STRING) == 0)
    {
        m_eLayer = DEVICE_COMMAND_SET_LAYER;
        oResult = TRUE;
    }
    else if(p_Layer.CompareNoCase(PROTOCOL_STACK_LAYER_STRING) == 0)
    {
        m_eLayer = PROTOCOL_STACK_LAYER;
        oResult = TRUE;
    }
    else if(p_Layer.CompareNoCase(INTERFACE_LAYER_STRING) == 0)
    {
        m_eLayer = INTERFACE_LAYER;
        oResult = TRUE;
    }

    m_eLayer = NO_LAYER;
    return oResult;
}

BOOL CCommandRoot::InitCommandId(CStdString p_CommandId)
{
    CMmcDataConversion dataConversion;

    return dataConversion.DecDWordStr2DWord(p_CommandId, &m_dCommandId, FALSE);
}

void CCommandRoot::StartCommandTime()
{
    if(m_pJournalManager)
    {
        m_pJournalManager->StartCommandTime(m_pJournalTime);
    }
}

void CCommandRoot::StopCommandTime()
{
    if(m_pJournalManager)
    {
        m_pJournalManager->StopCommandTime(m_pJournalTime);
    }
}

BOOL CCommandRoot::LoadFromXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElement* p_pCommandElement)
{
    CXXMLFile::CElement* pParameterInfoElement;
    CXXMLFile::CElementPart* pElementPart;
    std::list<CXXMLFile::CElementPart*> childList;
    CStdString sJournalRunTime;
    CStdString sCommandTime;
    CStdString sCmdStatus;
    CStdString sCmdName;
    CStdString sCmdLayer;
    CStdString sCmdId;
    CStdString sCmdErrorCode;

    if(p_pFile && p_pCommandElement)
    {
        if(p_pCommandElement->m_Text.CompareNoCase(_T("Command")) == 0)
        {
            //Command Elements
            tMmcStringToStringMap::iterator it;

            if((it = p_pCommandElement->AttributeToValue.find("Name")) == p_pCommandElement->AttributeToValue.end() )
                return FALSE;

            sCmdName = (*it).second;

            if(!InitCommandName(sCmdName)) return FALSE;

            if((it = p_pCommandElement->AttributeToValue.find("Layer")) == p_pCommandElement->AttributeToValue.end() )
                return FALSE;

            if(!InitLayer(sCmdLayer))
                return FALSE;

            sCmdLayer = (*it).second;

            if((it = p_pCommandElement->AttributeToValue.find("CommandId")) == p_pCommandElement->AttributeToValue.end() )
                return FALSE;

            sCmdId = (*it).second;

            if(!InitCommandId(sCmdId)) return FALSE;

            //Status
            if((it = p_pCommandElement->AttributeToValue.find("Status")) != p_pCommandElement->AttributeToValue.end() )
                sCmdStatus = (*it).second;

            if((it = p_pCommandElement->AttributeToValue.find("ErrorCode")) != p_pCommandElement->AttributeToValue.end() )
                sCmdErrorCode = (*it).second;

            InitCommandStatus(sCmdStatus, sCmdErrorCode);

            //Time
            if((it = p_pCommandElement->AttributeToValue.find("JournalRunTime")) != p_pCommandElement->AttributeToValue.end() )
                sJournalRunTime = (*it).second;

            if((it = p_pCommandElement->AttributeToValue.find("CommandDuration")) != p_pCommandElement->AttributeToValue.end() )
                sCommandTime = (*it).second;

            SetJournalRunTime(&sJournalRunTime);
            SetCommandTime(&sCommandTime);

            //ParameterInfo Element
            for(std::list<CXXMLFile::CElementPart*>::iterator it = p_pCommandElement->begin(); it != p_pCommandElement->end(); it++)
            {
                pElementPart = (*it);
                if(p_pFile->IsElement(pElementPart))
                {
                    pParameterInfoElement = (CXXMLFile::CElement*)pElementPart;
                    if(pParameterInfoElement->m_Text.CompareNoCase(_T("ParameterInfo")) == 0)
                    {
                        if(m_pParameterInfo && !m_pParameterInfo->LoadFromXMLFile(p_pFile, pParameterInfoElement)) return FALSE;
                    }

                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

BOOL CCommandRoot::LoadFromXMLString(CStdString *p_pDataString)
{
    BOOL oResult(FALSE);
    CXXMLFile file;
    CXXMLFile::CElement* pRootElement;
    CXXMLFile::CElement* pCommandElement;
    CXXMLFile::CElementPart* pElementPart;

    //Read XML File
    if(file.ReadFromString(p_pDataString))
    {
        //Root
        pElementPart = file.Root();
        if(file.IsElement(pElementPart))
        {
            pRootElement = (CXXMLFile::CElement*)pElementPart;
            pCommandElement = FindCommandElement(&file, pRootElement);
            if(pCommandElement)
            {
                oResult = LoadFromXMLFile(&file, pCommandElement);
            }
        }
    }

    return oResult;
}

CXXMLFile::CElement* CCommandRoot::FindCommandElement(CXXMLFile* p_pFile, CXXMLFile::CElement* p_pParentElement)
{
    if(p_pFile && p_pParentElement)
    {
        for(std::list<CXXMLFile::CElementPart*>::iterator it = p_pParentElement->begin(); it != p_pParentElement->end(); it++)
        {
            CXXMLFile::CElementPart* pElementPart = (*it);
            if(p_pFile->IsElement(pElementPart))
            {
                CXXMLFile::CElement* pElement = (CXXMLFile::CElement*)pElementPart;
                if(pElement->m_Text.CompareNoCase(_T("Command")) == 0)
                {
                    return pElement;
                }
                else
                {
                    pElement = FindCommandElement(p_pFile, pElement);
                    if(pElement) return pElement;
                }
            }
        }
    }

    return 0;
}

CXXMLFile::CElement* CCommandRoot::DeleteCommandElement(CXXMLFile* p_pFile, CXXMLFile::CElement* p_pParentElement, CXXMLFile::CElement* p_pCommandElement)
{
    CXXMLFile::CElement* pElement;

    if(p_pFile && p_pParentElement && p_pCommandElement)
    {
        for(std::list<CXXMLFile::CElementPart*>::iterator it = p_pParentElement->begin(); it != p_pParentElement->end(); it++)
        {
            CXXMLFile::CElementPart* pElementPart = (*it);
            if(p_pFile->IsElement(pElementPart))
            {
                pElement = (CXXMLFile::CElement*)pElementPart;
                if(pElement == p_pCommandElement)
                {
                    if(p_pFile->DeleteElement(p_pParentElement, pElement))
                    {
                        return p_pParentElement;
                    }
                }
                else
                {
                    pElement = DeleteCommandElement(p_pFile, pElement, p_pCommandElement);
                    if(pElement) return pElement;
                }
            }
        }
    }

    return 0;
}

CCommandRoot& CCommandRoot::operator=(CCommandRoot& p_rOther)
{
    if(this != &p_rOther)
    {
        ResetCommand();

        m_CommandName = p_rOther.m_CommandName;
        m_dCommandId = p_rOther.m_dCommandId;
        m_eLayer = p_rOther.m_eLayer;
        m_dTimeout = p_rOther.m_dTimeout;

        m_CommandStatus = p_rOther.m_CommandStatus;
        if(p_rOther.m_pErrorInfo) m_pErrorInfo = p_rOther.m_pErrorInfo->Clone();

        m_pJournalManager = p_rOther.m_pJournalManager;
        m_pGateway = p_rOther.m_pGateway;
        m_oTransactionEventEnabled = p_rOther.m_oTransactionEventEnabled;

        if(m_pJournalTime && p_rOther.m_pJournalTime) *m_pJournalTime = *p_rOther.m_pJournalTime;
        if(m_pParameterInfo && p_rOther.m_pParameterInfo) *m_pParameterInfo = *p_rOther.m_pParameterInfo;
        if(m_pLayerParameterStack && p_rOther.m_pLayerParameterStack) *m_pLayerParameterStack = *p_rOther.m_pLayerParameterStack;
    }

    return *this;
}

void CCommandRoot::ResetCommand()
{
    m_oTransactionEventEnabled = TRUE;
    m_pJournalManager = 0;
    m_pGateway = 0;
    m_CommandName = "";
    m_eLayer = NO_LAYER;
    m_dCommandId = 0;
    m_CommandStatus = CMD_IDLE;
    m_dTimeout = UINT_MAX;
    m_oSubLayerRemainsLocked = FALSE;
    ResetErrorInfo();

    if(m_pParameterInfo) m_pParameterInfo->Reset();
    if(m_pJournalTime) m_pJournalTime->Reset();
}

#ifdef WINVER
void CCommandRoot::Serialize(CArchive &p_rArchive)
{
    const DWORD COMMAND_STRUCTURE_VERSION = 1;

    int iLayer, iCommandStatus;
    BOOL oErrorInfoExisting;
    BOOL oParameterInfoExisting;
    BOOL oJournalTimeExisting;
    DWORD ulCommandStructureVersion;

    if(p_rArchive.IsStoring())
    {
        p_rArchive << COMMAND_STRUCTURE_VERSION;

        p_rArchive << m_CommandName;
        p_rArchive << m_eLayer;
        p_rArchive << m_dCommandId;
        p_rArchive << m_dTimeout;
        p_rArchive << m_CommandStatus;
        p_rArchive << m_oSubLayerRemainsLocked;

        //ErrorInfo
        oErrorInfoExisting = BOOL(m_pErrorInfo);
        p_rArchive << oErrorInfoExisting;
        if(oErrorInfoExisting) m_pErrorInfo->Serialize(p_rArchive);

        //ParameterInfo
        oParameterInfoExisting = BOOL(m_pParameterInfo);
        p_rArchive << oParameterInfoExisting;
        if(oParameterInfoExisting) m_pParameterInfo->Serialize(p_rArchive);

        //JournalTime
        p_rArchive << m_oTransactionEventEnabled;
        oJournalTimeExisting = BOOL(m_pJournalTime);
        p_rArchive << oJournalTimeExisting;
        if(oJournalTimeExisting) m_pJournalTime->Serialize(p_rArchive);
    }
    else
    {
        ResetCommand();

        p_rArchive >> ulCommandStructureVersion;
        if(ulCommandStructureVersion == COMMAND_STRUCTURE_VERSION)
        {
            p_rArchive >> m_CommandName;
            p_rArchive >> iLayer; m_eLayer = (ELayer)iLayer;
            p_rArchive >> m_dCommandId;
            p_rArchive >> m_dTimeout;
            p_rArchive >> iCommandStatus; m_CommandStatus = (ECommandStatus)iCommandStatus;
            p_rArchive >> m_oSubLayerRemainsLocked;

            //ErrorInfo
            p_rArchive >> oErrorInfoExisting;
            if(oErrorInfoExisting)
            {
                m_pErrorInfo = new CErrorInfo();
                m_pErrorInfo->Serialize(p_rArchive);
            }

            //ParameterInfo
            p_rArchive >> oParameterInfoExisting;
            if(oParameterInfoExisting)
            {
                if(m_pParameterInfo == 0) m_pParameterInfo = new CParameterInfo();
                m_pParameterInfo->Serialize(p_rArchive);
            }

            //JournalTime
            p_rArchive >> m_oTransactionEventEnabled;
            p_rArchive >> oJournalTimeExisting;
            if(oJournalTimeExisting)
            {
                if(m_pJournalTime == 0) m_pJournalTime = new CJournalTime();
                m_pJournalTime->Serialize(p_rArchive);
            }
        }
    }
}
#endif

BOOL CCommandRoot::SetParameterData(int p_lParameterIndex, void* p_pData, size_t p_ulLength)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->SetParameterData(p_lParameterIndex, p_pData, p_ulLength);
    }

    return oResult;
}

BOOL CCommandRoot::SetReturnParameterData(int p_lParameterIndex, CStdString p_Data)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->SetReturnParameterData(p_lParameterIndex, (void*)p_Data.c_str(), p_Data.size());
    }

    return oResult;
}

BOOL CCommandRoot::SetReturnParameterData(int p_lParameterIndex, void* p_pData, DWORD p_ulLength)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->SetReturnParameterData(p_lParameterIndex, p_pData, p_ulLength);
    }

    return oResult;
}

BOOL CCommandRoot::GetReturnParameterData(int p_lParameterIndex, void* p_pData, DWORD p_ulLength)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->GetReturnParameterData(p_lParameterIndex, p_pData, p_ulLength);
    }

    return oResult;
}

BOOL CCommandRoot::GetParameterData(int p_lParameterIndex, void* p_pData, DWORD p_ulLength)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->GetParameterData(p_lParameterIndex, p_pData, p_ulLength);
    }

    return oResult;
}

CXXMLFile::CElement* CCommandRoot::StoreToXMLFile(CXXMLFile* p_pFile, CXXMLFile::CElement* p_pParentElement, BOOL p_oCheckVisibility)
{
    CXXMLFile::CElement* pElement = 0;
    CStdString runTime;
    CStdString commandTime;

    if(p_pFile && p_pParentElement)
    {
        //Time
        runTime = GetJournalRunTimeString();
        commandTime = GetCommandTimeString();

        //Command Elements
        pElement = (CXXMLFile::CElement*)p_pFile->AddElement(p_pParentElement);
        p_pFile->SetText(pElement, "Command");

        pElement->SetAt("Name", m_CommandName);
        pElement->SetAt("Layer", GetLayerStr());
        pElement->SetAt("CommandId", GetCommandIdStr());

        pElement->SetAt("Status", GetCommandStatusStr(m_CommandStatus));
        pElement->SetAt("ErrorCode", GetErrorCodeStr());

        pElement->SetAt("JournalRunTime", runTime);
        pElement->SetAt("CommandDuration", commandTime);

        //Parameter Elements
        if(m_pParameterInfo && !m_pParameterInfo->StoreToXMLFile(p_pFile, pElement, p_oCheckVisibility)) return 0;
    }

    return pElement;
}

BOOL CCommandRoot::StoreToXMLString(CStdString *p_pDataString, BOOL p_oCheckVisibility)
{
    BOOL oResult(FALSE);
    CXXMLFile file;
    CXXMLFile::CElementPart* pElementPart;
    CXXMLFile::CElement* pParentElement;

    if(p_pDataString)
    {
        //Root
        file.RemoveAll();
        pElementPart = file.Root();
        if(file.IsElement(pElementPart))
        {
            pParentElement = (CXXMLFile::CElement*)pElementPart;

            //Store
            if(StoreToXMLFile(&file, pParentElement, p_oCheckVisibility))
            {
                //Write XML String
                p_pDataString->Empty();
                file.WriteToString(p_pDataString);

                oResult = TRUE;
            }
        }
    }

    return oResult;
}

void CCommandRoot::InitJournalManager(CJournalManagerBase *p_pJournalManager)
{
    m_pJournalManager = p_pJournalManager;
}

BOOL CCommandRoot::InitGateway(CGateway *p_pGateway)
{
    m_pGateway = p_pGateway;
    return TRUE;
}

void CCommandRoot::ResetGateway()
{
    m_pGateway = 0;
}

void CCommandRoot::ResetJournalManager()
{
    m_pJournalManager = 0;
}

void CCommandRoot::SetStatus(BOOL p_oResult, CErrorInfo* p_pErrorInfo)
{
    //CommandStatus
    if(p_oResult)
    {
        m_CommandStatus = CMD_SUCCESSFUL;
    }
    else
    {
        m_CommandStatus = CMD_FAILED;
    }

    //ErrorInfo
    ResetErrorInfo();
    if(p_pErrorInfo)
    {
        m_pErrorInfo = p_pErrorInfo->Clone();
        if(!m_pErrorInfo->IsCommandNameSet()) m_pErrorInfo->SetCommandName(m_CommandName);
    }
}

void CCommandRoot::ResetStatus()
{
    m_CommandStatus = CMD_IDLE;
    ResetErrorInfo();
}

CStdString CCommandRoot::GetLayerStr()
{
    switch(m_eLayer)
    {
        case VIRTUAL_COMMAND_SET_LAYER: return VIRTUAL_COMMAND_SET_LAYER_STRING;
        case DEVICE_COMMAND_SET_LAYER: return COMMAND_SET_LAYER_STRING;
        case PROTOCOL_STACK_LAYER: return PROTOCOL_STACK_LAYER_STRING;
        case INTERFACE_LAYER: return INTERFACE_LAYER_STRING;
        case NO_LAYER: return "NoLayer";
    }

    return "";
}

CStdString CCommandRoot::GetCommandIdStr()
{
    CMmcDataConversion dataConversion;
    CStdString strResult(_T(""));

    if(!dataConversion.DWord2DecDWordStr(m_dCommandId, &strResult))
    {
        strResult = _T("");
    }

    return strResult;
}

CStdString CCommandRoot::GetErrorCodeStr()
{
    DWORD ulErrorCode = 0;
    CMmcDataConversion dataConversion;
    CStdString strErrorCode;

    if(m_pErrorInfo) ulErrorCode = m_pErrorInfo->GetErrorCode();
    if(dataConversion.DWord2DecDWordStr(ulErrorCode, &strErrorCode))
    {
        return strErrorCode;
    }

    return "";
}

CStdString CCommandRoot::GetCommandStatusStr(ECommandStatus p_eCommandStatus)
{
    switch(p_eCommandStatus)
    {
        case CMD_IDLE:          return "Idle";
        case CMD_SUCCESSFUL:    return "Successful";
        case CMD_FAILED:        return "Failed";
        default: return "";
    }
}

BOOL CCommandRoot::InitCommandStatus(CStdString p_CmdStatus, CStdString p_CmdErrorCode)
{
    BOOL oResult(FALSE);
    CMmcDataConversion dataConversion;
    DWORD ulErrorCode;

    if(dataConversion.DecDWordStr2DWord(p_CmdErrorCode, &ulErrorCode, FALSE))
    {
        //ErrorInfo
        ResetErrorInfo();
        m_pErrorInfo = new CErrorInfo();
        m_pErrorInfo->Init(ulErrorCode);

        //CommandStatus
        if(p_CmdStatus.Compare(_T("Idle")) == 0)
        {
            m_CommandStatus = CMD_IDLE;
            oResult = TRUE;
        }
        else if(p_CmdStatus.Compare(_T("Successful")) == 0)
        {
            m_CommandStatus = CMD_SUCCESSFUL;
            oResult = TRUE;
        }
        else if(p_CmdStatus.Compare(_T("Failed")) == 0)
        {
            m_CommandStatus = CMD_FAILED;
            oResult = TRUE;
        }
        else
        {
            m_CommandStatus = CMD_IDLE;
        }
    }

    return oResult;
}

DWORD CCommandRoot::GetCommandId()
{
    return m_dCommandId;
}

BOOL CCommandRoot::GetErrorInfo(CErrorInfo* p_pErrorInfo)
{
    BOOL oResult(FALSE);

    if(p_pErrorInfo)
    {
        if(m_pErrorInfo)
        {
            *p_pErrorInfo = *m_pErrorInfo;
        }
        else
        {
            p_pErrorInfo->Reset();
        }

        oResult = TRUE;
    }

    return oResult;
}

BOOL CCommandRoot::GetErrorCode(DWORD* p_pulErrorCode)
{
    BOOL oResult(FALSE);

    if(p_pulErrorCode && m_pErrorInfo)
    {
        *p_pulErrorCode = m_pErrorInfo->GetErrorCode();
        oResult = TRUE;
    }

    return oResult;
}

DWORD CCommandRoot::GetParameterLength(int p_lParameterIndex)
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetParameterLength(p_lParameterIndex);
    }

    return 0;
}

DWORD CCommandRoot::GetReturnParameterLength(int p_lParameterIndex)
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetReturnParameterLength(p_lParameterIndex);
    }

    return 0;
}

int CCommandRoot::GetNbOfParameter()
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetNbOfParameter();
    }

    return 0;
}

int CCommandRoot::GetNbOfReturnParameter()
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetNbOfReturnParameter();
    }

    return 0;
}

BOOL CCommandRoot::FindParameterIndex(CStdString p_Name, int& p_rIndex)
{
    BOOL oResult = FALSE;

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->FindParameterIndex(p_Name, p_rIndex);
    }

    return oResult;
}

BOOL CCommandRoot::FindReturnParameterIndex(CStdString p_Name, int& p_rIndex)
{
    BOOL oResult = FALSE;

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->FindReturnParameterIndex(p_Name, p_rIndex);
    }

    return oResult;
}

CStdString CCommandRoot::GetParameterName(int p_lParameterIndex)
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetParameterName(p_lParameterIndex);
    }

    return "";
}

CStdString CCommandRoot::GetReturnParameterName(int p_lParameterIndex)
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetReturnParameterName(p_lParameterIndex);
    }

    return "";
}

BOOL CCommandRoot::IsSuccessful()
{
    return (m_CommandStatus == CMD_SUCCESSFUL);
}

BOOL CCommandRoot::GetParameterInfo(int p_lParameterIndex, CStdString* p_pName, CStdString* p_pType, CStdString* p_pData, EObjectValueStringFormat p_Format)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo->IsParameterExisting(p_lParameterIndex))
    {
        if(p_pName) *p_pName = m_pParameterInfo->GetParameterName(p_lParameterIndex);
        if(p_pType) *p_pType = m_pParameterInfo->GetParameterTypeStr(p_lParameterIndex);
        if(p_pData) *p_pData = m_pParameterInfo->GetParameterDataStr(p_lParameterIndex, p_Format);

        oResult = TRUE;
    }

    return oResult;
}

BOOL CCommandRoot::GetReturnParameterInfo(int p_lParameterIndex, CStdString* p_pName, CStdString* p_pType, CStdString* p_pData, EObjectValueStringFormat p_Format)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo->IsReturnParameterExisting(p_lParameterIndex))
    {
        if(p_pName) *p_pName = m_pParameterInfo->GetReturnParameterName(p_lParameterIndex);
        if(p_pType) *p_pType = m_pParameterInfo->GetReturnParameterTypeStr(p_lParameterIndex);
        if(p_pData) *p_pData = m_pParameterInfo->GetReturnParameterDataStr(p_lParameterIndex, p_Format);

        oResult = TRUE;
    }

    return oResult;
}

EObjectDataType CCommandRoot::GetParameterType(int p_lParameterIndex)
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetParameterType(p_lParameterIndex);
    }

    return ODT_UNKNOWN;
}

EObjectDataType CCommandRoot::GetReturnParameterType(int p_lParameterIndex)
{
    if(m_pParameterInfo)
    {
        return m_pParameterInfo->GetReturnParameterType(p_lParameterIndex);
    }

    return ODT_UNKNOWN;
}

BOOL CCommandRoot::IsParameterEditable(int p_lParameterIndex)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->IsParameterEditable(p_lParameterIndex);
    }

    return oResult;
}

BOOL CCommandRoot::IsParameterVisible(int p_lParameterIndex)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->IsParameterVisible(p_lParameterIndex);
    }

    return oResult;
}

BOOL CCommandRoot::IsReturnParameterVisible(int p_lParameterIndex)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->IsReturnParameterVisible(p_lParameterIndex);
    }

    return oResult;
}

BOOL CCommandRoot::SetParameterDataString(int p_lParameterIndex, CStdString p_Value, BOOL p_oShowMsg)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->SetParameterDataString(p_lParameterIndex, p_Value, p_oShowMsg);
    }

    return oResult;
}

BOOL CCommandRoot::SetReturnParameterDataString(int p_lParameterIndex, CStdString p_Value, BOOL p_oShowMsg)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->SetReturnParameterDataString(p_lParameterIndex, p_Value, p_oShowMsg);
    }

    return oResult;
}

BOOL CCommandRoot::GetParameterDataString(int p_lParameterIndex, CStdString* pStrValue, EObjectValueStringFormat p_Format)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->GetParameterDataString(p_lParameterIndex, pStrValue, p_Format);
    }

    return oResult;
}

BOOL CCommandRoot::GetReturnParameterDataString(int p_lParameterIndex, CStdString* p_pValue, EObjectValueStringFormat p_Format)
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->GetReturnParameterDataString(p_lParameterIndex, p_pValue, p_Format);
    }

    return oResult;
}

BOOL CCommandRoot::IsEditableParameterExisting()
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->IsEditableParameterExisting();
    }

    return oResult;
}

BOOL CCommandRoot::IsVisibleParameterExisting()
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->IsVisibleParameterExisting();
    }

    return oResult;
}

BOOL CCommandRoot::IsVisibleReturnParameterExisting()
{
    BOOL oResult(FALSE);

    if(m_pParameterInfo)
    {
        oResult = m_pParameterInfo->IsVisibleReturnParameterExisting();
    }

    return oResult;
}

DWORD CCommandRoot::GetParameterArraySize(int p_lParameterIndex)
{
    DWORD ulResult(0);

    if(m_pParameterInfo)
    {
        ulResult = m_pParameterInfo->GetParameterArraySize(p_lParameterIndex);
    }

    return ulResult;
}

DWORD CCommandRoot::GetReturnParameterArraySize(int p_lParameterIndex)
{
    DWORD ulResult(0);

    if(m_pParameterInfo)
    {
        ulResult = m_pParameterInfo->GetReturnParameterArraySize(p_lParameterIndex);
    }

    return ulResult;
}

BOOL CCommandRoot::PushLayerParameterSet(CLayerParameterSet& p_rParameterSet)
{
    BOOL oResult = FALSE;

    if(m_pLayerParameterStack)
    {
        oResult = m_pLayerParameterStack->PushLayer(p_rParameterSet);
    }

    return oResult;
}

BOOL CCommandRoot::SetLayerParameterStack(CLayerParameterStack& p_rParameterStack)
{
    BOOL oResult = FALSE;

    if(m_pLayerParameterStack)
    {
        *m_pLayerParameterStack = p_rParameterStack;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CCommandRoot::GetLayerParameterStack(CLayerParameterStack& p_rParameterStack)
{
    BOOL oResult = FALSE;

    if(m_pLayerParameterStack)
    {
        p_rParameterStack = *m_pLayerParameterStack;
        oResult = TRUE;
    }

    return oResult;
}

BOOL CCommandRoot::GetLayerParameter(ELayer p_eLayer, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pLayerParameterStack)
    {
        //Init
        oResult = TRUE;

        if(!m_pLayerParameterStack->GetLayerParameter(p_eLayer, p_Name, p_pValue, p_ulSize))
        {
            if(pErrorInfo) pErrorInfo->Init(k_Error_BadParameter, 0);
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CCommandRoot::GetLayerParameter(ELayer p_eLayer, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo)
{
    BOOL oResult = FALSE;

    if(m_pLayerParameterStack)
    {
        //Init
        oResult = TRUE;

        if(!m_pLayerParameterStack->GetLayerParameter(p_eLayer, p_Name, p_rValue))
        {
            if(pErrorInfo) pErrorInfo->Init(k_Error_BadParameter, 0);
            oResult = FALSE;
        }
    }

    return oResult;
}

BOOL CCommandRoot::IsSubLayerLocked()
{
    return m_oSubLayerRemainsLocked;
}
