// TransactionEventData.h: Schnittstelle f�r die Klasse CTransactionEventData.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSACTIONEVENTDATA_H__8716821E_4625_496E_B789_A2B2622981D1__INCLUDED_)
#define AFX_TRANSACTIONEVENTDATA_H__8716821E_4625_496E_B789_A2B2622981D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <Classes/XXMLFile.h>
#include <list>

class CCommandRoot;

class CTransactionEventData
{
public:
    BOOL LoadFromXMLFile(CXXMLFile* pFile, CXXMLFile::CElement* pCommandElement);
    CXXMLFile::CElement* StoreToXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pParentElement);

    void MoveChildList(std::list<CTransactionEventData*>* pList);

    CTransactionEventData* Clone();
    CTransactionEventData& operator=(CTransactionEventData& other);

    void StoreToASCIIFile(std::ofstream* pFile);

    CTransactionEventData();
    virtual ~CTransactionEventData();

    virtual void SetString(CStdString* pData);
    virtual void SetCommand(CCommandRoot* pCommand);
    virtual CCommandRoot *GetCommand();

private:
    virtual CXXMLFile::CElement* StoreEventDataToXMLFile(CXXMLFile* pFile,CXXMLFile::CElement* pElement);

    void StoreString(std::ofstream* pFile,CStdString* pData);
    void DeleteChildTransactionEventDataList();

    CCommandRoot* m_pCommand;
    CStdString m_Data;
    std::list<CTransactionEventData*> m_ChildTransactionEventDataList;
};

#endif // !defined(AFX_TRANSACTIONEVENTDATA_H__8716821E_4625_496E_B789_A2B2622981D1__INCLUDED_)
