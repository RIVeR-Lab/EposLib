// CommandRoot.h: Schnittstelle f�r die Klasse CCommandRoot.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CommandRoot_H__1FDA715C_31EA_453B_A400_47A2BC0E2173__INCLUDED_)
#define AFX_CommandRoot_H__1FDA715C_31EA_453B_A400_47A2BC0E2173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Classes/XXMLFile.h>
#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <Classes/TypeDefinitions.h>
#include <MmcTypeDefinition.h>

class CParameterInfo;
class CLayerParameterStack;
class CLayerParameterSet;
class CJournalTime;
class CJournalManagerBase;
class CLayerManagerBase;
class CGateway;
class CErrorInfo;

class CCommandRoot
{
public:
    DWORD GetParameterArraySize(int paramIndex);
    DWORD GetReturnParameterArraySize(int paramIndex);

    CStdString GetLayerStr();

    BOOL IsSuccessful();
    void ResetStatus();
    void SetStatus(BOOL oResult, CErrorInfo* pErrorInfo);

    BOOL IsEditableParameterExisting();
    BOOL IsVisibleParameterExisting();
    BOOL IsVisibleReturnParameterExisting();

    BOOL SetTimeout(DWORD timeout);
    BOOL ResetTimeout();
    DWORD GetTimeout();
    BOOL IsTimeoutValid();

	BOOL IsSubLayerLocked();

    BOOL LoadFromXMLString(CStdString *pDataString);
    BOOL LoadFromXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pCommandElement);
    CXXMLFile::CElement* StoreToXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pParentElement, BOOL oCheckVisibility = FALSE);
    BOOL StoreToXMLString(CStdString *pDataString, BOOL oCheckVisibility = FALSE);

    int GetNbOfParameter();
    int GetNbOfReturnParameter();

    BOOL FindParameterIndex(CStdString p_Name, int& p_rIndex);
    BOOL FindReturnParameterIndex(CStdString p_Name, int& p_rIndex);

    BOOL GetParameterInfo(int paramIndex, CStdString* pName, CStdString* pType, CStdString* pData, EObjectValueStringFormat format);
    BOOL GetReturnParameterInfo(int paramIndex, CStdString* pName, CStdString* pType, CStdString* pData, EObjectValueStringFormat format);

	BOOL SetParameterData(int lParameterIndex, void* pData, size_t dLength);
    BOOL SetReturnParameterData(int lParameterIndex, CStdString data);
    BOOL SetReturnParameterData(int lParameterIndex, void* pData, DWORD dLength);
    BOOL SetParameterDataString(int lParameterIndex, CStdString strValue, BOOL showMsg);
    BOOL SetReturnParameterDataString(int lParameterIndex, CStdString strValue, BOOL showMsg);

    BOOL GetReturnParameterData(int lParameterIndex, void* pData, DWORD dLength);
    BOOL GetParameterData(int lParameterIndex, void* pData, DWORD dLength);
    BOOL GetParameterDataString(int lParameterIndex, CStdString* pStrValue, EObjectValueStringFormat format);
    BOOL GetReturnParameterDataString(int lParameterIndex, CStdString* pStrValue, EObjectValueStringFormat format);

    DWORD GetParameterLength(int paramIndex);
    DWORD GetReturnParameterLength(int paramIndex);
    CStdString GetParameterName(int paramIndex);
    CStdString GetReturnParameterName(int paramIndex);
    EObjectDataType GetParameterType(int paramIndex);
    EObjectDataType GetReturnParameterType(int paramIndex);
    BOOL IsParameterEditable(int paramIndex);
    BOOL IsParameterVisible(int paramIndex);
    BOOL IsReturnParameterVisible(int paramIndex);

    BOOL GetErrorInfo(CErrorInfo* pErrorInfo);
    BOOL GetErrorCode(DWORD* pdErrorCode);

    void InitCommand(CStdString commandName, ELayer eLayer, DWORD dCommandId);
    void AddParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType);
    void AddParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType, BOOL oVisible, BOOL oEditable);
    void AddParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType, DWORD dArraySize);
    void AddParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType, DWORD dArraySize, BOOL oVisible, BOOL oEditable);
    void AddReturnParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType);
    void AddReturnParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType, int oVisible);
    void AddReturnParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType, DWORD dArraySize);
    void AddReturnParameter(int lParameterIndex, CStdString parameterName, EObjectDataType eParameterType, DWORD dArraySize, BOOL oVisible);

    CStdString GetCommandName();
    DWORD GetCommandId();
    ELayer GetLayer();
#ifdef WINVER
    void Serialize(CArchive& ar);
#endif
    CCommandRoot& operator=(CCommandRoot& other);
    CCommandRoot* CloneCommand();

    void InitJournalManager(CJournalManagerBase *pJournalManager);
    BOOL InitGateway(CGateway *pGateway);

    void ResetJournalManager();
    void ResetGateway();

    void StartCommandTime();
    void StopCommandTime();
    virtual BOOL Execute(CLayerManagerBase* pManager, HANDLE h_Handle, HANDLE hTransactionHandle);

    BOOL DoTransactionEvent(HANDLE hTransactionHandle);
	BOOL EnableTransactionEvent(BOOL p_oEnabled);

	CStdString GetJournalRunTimeString();
    CStdString GetCommandTimeString();

	//Layer Parameter Stack
	BOOL PushLayerParameterSet(CLayerParameterSet& p_rParameterSet);
	BOOL SetLayerParameterStack(CLayerParameterStack& p_rParameterStack);
	BOOL GetLayerParameterStack(CLayerParameterStack& p_rParameterStack);
	BOOL GetLayerParameter(ELayer p_eLayer, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize, CErrorInfo* pErrorInfo = NULL);
	BOOL GetLayerParameter(ELayer p_eLayer, CStdString p_Name, CStdString& p_rValue, CErrorInfo* pErrorInfo = NULL);

    CCommandRoot();
    virtual ~CCommandRoot();

protected:
    void ResetCommand();
    void ResetErrorInfo();
	
private:
    CStdString GetCommandIdStr();
    CStdString GetCommandStatusStr(ECommandStatus eCmdStatus);
    CStdString GetErrorCodeStr();

    BOOL InitCommandName(CStdString commandName);
    BOOL InitLayer(CStdString strLayer);
    BOOL InitCommandId(CStdString strCommandId);
    BOOL InitCommandStatus(CStdString strCmdStatus, CStdString strErrorCode);

    void SetJournalRunTime(CStdString* pJournalRunTime);
    void SetCommandTime(CStdString* pCommandTime);

    CXXMLFile::CElement* FindCommandElement(CXXMLFile* pFile, CXXMLFile::CElement* pParentElement);
    CXXMLFile::CElement* DeleteCommandElement(CXXMLFile* pFile, CXXMLFile::CElement* pParentElement, CXXMLFile::CElement* pCommandElement);

protected:
    CJournalManagerBase* m_pJournalManager;
    CParameterInfo* m_pParameterInfo;
	CLayerParameterStack* m_pLayerParameterStack;
    CGateway* m_pGateway;
    CJournalTime* m_pJournalTime;

    CStdString m_CommandName;
    ELayer m_eLayer;
    DWORD m_dCommandId;
    DWORD m_dTimeout;

    ECommandStatus m_CommandStatus;
    CErrorInfo* m_pErrorInfo;
	BOOL m_oSubLayerRemainsLocked;

private:
	BOOL m_oTransactionEventEnabled;

};

#endif // !defined(AFX_CommandRoot_H__1FDA715C_31EA_453B_A400_47A2BC0E2173__INCLUDED_)
