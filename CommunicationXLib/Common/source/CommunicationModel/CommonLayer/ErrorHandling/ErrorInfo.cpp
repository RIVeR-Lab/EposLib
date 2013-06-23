// ErrorInfo.cpp: Implementierung der Klasse CErrorInfo.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorInfo.h>
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorHandling.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CErrorInfo::CErrorInfo()
{
    m_pError = NULL;
    m_pErrorProducer = NULL;
}

CErrorInfo::~CErrorInfo()
{
    DeleteError();
    DeleteErrorProducer();
}
#ifdef _WIN32
void CErrorInfo::Serialize(CArchive &ar)
{
    BOOL oErrorExisting;
    BOOL oErrorProducerExisting;

    if(ar.IsStoring())
    {
        //Error
        oErrorExisting = (BOOL)m_pError;
        ar << oErrorExisting;
        if(oErrorExisting)
        {
            m_pError->Serialize(ar);
        }

        //ErrorProducer
        oErrorProducerExisting = (BOOL)m_pErrorProducer;
        ar << oErrorProducerExisting;
        if(oErrorProducerExisting)
        {
            m_pErrorProducer->Serialize(ar);
        }
    }
    else
    {
        DeleteError();
        DeleteErrorProducer();

        //Error
        ar >> oErrorExisting;
        if(oErrorExisting)
        {
            m_pError = new CError();
            m_pError->Serialize(ar);
        }

        //ErrorProducer
        ar >> oErrorProducerExisting;
        if(oErrorProducerExisting)
        {
            m_pErrorProducer = new CErrorProducer();
            m_pErrorProducer->Serialize(ar);
        }
    }
}
#endif
CErrorInfo& CErrorInfo::operator=(CErrorInfo& other)
{
    if(this != &other)
    {
        DeleteError();
        DeleteErrorProducer();

        if(other.m_pError) m_pError = other.m_pError->Clone();
        if(other.m_pErrorProducer) m_pErrorProducer = other.m_pErrorProducer->Clone();
    }

    return *this;
}

CErrorInfo *CErrorInfo::Clone()
{
    CErrorInfo *pNewErrorInfo = new CErrorInfo();
    *pNewErrorInfo = *this;

    return pNewErrorInfo;
}

void CErrorInfo::DeleteError()
{
    if(m_pError)
    {
        delete m_pError;
        m_pError = NULL;
    }
}

void CErrorInfo::DeleteErrorProducer()
{
    if(m_pErrorProducer)
    {
        delete m_pErrorProducer;
        m_pErrorProducer = NULL;
    }
}

void CErrorInfo::Init(CError *p_pError, CErrorProducer *p_pErrorProducer)
{
    //Error
    DeleteError();
    if(p_pError) m_pError = p_pError->Clone();

    //ErrorProducer
    DeleteErrorProducer();
    if(p_pErrorProducer) m_pErrorProducer = p_pErrorProducer->Clone();
}

void CErrorInfo::Init(DWORD p_ulErrorCode, CErrorProducer *p_pErrorProducer)
{
    CStdString errorDescription;

    //Error
    DeleteError();
    m_pError = new CError();

    if(m_pError)
    {
        CErrorHandling::GetErrorDescription(p_ulErrorCode, &errorDescription);
        m_pError->Init(p_ulErrorCode, errorDescription);

        //ErrorProducer
        DeleteErrorProducer();
        if(p_pErrorProducer) m_pErrorProducer = p_pErrorProducer->Clone();
    }
}

void CErrorInfo::Init(DWORD p_ulErrorCode)
{
    CStdString errorDescription;

    //Error
    DeleteError();
    m_pError = new CError();

    if(m_pError)
    {
        CErrorHandling::GetErrorDescription(p_ulErrorCode, &errorDescription);
        m_pError->Init(p_ulErrorCode, errorDescription);

        //ErrorProducer
        DeleteErrorProducer();
    }
}

DWORD CErrorInfo::GetErrorCode()
{
    DWORD ulErrorCode = 0;

    if(m_pError)
    {
        ulErrorCode = m_pError->GetErrorCode();
    }

    return ulErrorCode;
}

BOOL CErrorInfo::IsCommandNameSet()
{
    BOOL oResult = FALSE;

    if(m_pErrorProducer)
    {
        oResult = m_pErrorProducer->IsCommandNameSet();
    }

    return oResult;
}

void CErrorInfo::SetCommandName(CStdString p_commandName)
{
    if(m_pErrorProducer)
    {
        m_pErrorProducer->SetCommandName(p_commandName);
    }
}

void CErrorInfo::Reset()
{
    DeleteError();
    DeleteErrorProducer();
}
