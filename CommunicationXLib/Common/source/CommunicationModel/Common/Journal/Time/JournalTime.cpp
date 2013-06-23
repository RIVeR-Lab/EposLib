// JournalTime.cpp: Implementierung der Klasse CJournalTime.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/Common/Journal/Time/JournalTime.h>
#include <MmcTypeDefinition.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CJournalTime::CJournalTime()
{
    Reset();
}

CJournalTime::~CJournalTime()
{
}

void CJournalTime::Reset()
{
    m_ulJournalRunTime = 0;
    m_ulCommandStartTime = 0;
    m_ulCommandTime = 0;
}
#ifdef _WIN32
void CJournalTime::Serialize(CArchive& ar)
{
    if(ar.IsStoring())
    {
        ar << m_ulCommandStartTime;
        ar << m_ulCommandTime;
        ar << m_ulJournalRunTime;
    }
    else
    {
        ar >> m_ulCommandStartTime;
        ar >> m_ulCommandTime;
        ar >> m_ulJournalRunTime;
    }
}
#endif //_WIN32

CJournalTime& CJournalTime::operator=(CJournalTime& other)
{
    if(this != &other)
    {
        m_ulCommandStartTime = other.m_ulCommandStartTime;
        m_ulCommandTime = other.m_ulCommandTime;
        m_ulJournalRunTime = other.m_ulJournalRunTime;
    }

    return *this;
}

void CJournalTime::SetJournalRunTime(DWORD p_ulJournalRunTime)
{
    m_ulJournalRunTime = p_ulJournalRunTime;
}

DWORD CJournalTime::GetJournalRunTime()
{
    return m_ulJournalRunTime;
}

void CJournalTime::SetCommandStartTime(DWORD p_ulCommandStartTime)
{
    m_ulCommandStartTime = p_ulCommandStartTime;
}

DWORD CJournalTime::GetCommandStartTime()
{
    return m_ulCommandStartTime;
}

DWORD CJournalTime::GetCommandTime()
{
    return m_ulCommandTime;
}

void CJournalTime::SetCommandTime(DWORD p_ulCommandTime)
{
    m_ulCommandTime = p_ulCommandTime;
}

CStdString CJournalTime::GetCommandTimeString()
{
    return FormatCommandTimeToString(m_ulCommandTime);
}

CStdString CJournalTime::GetJournalRunTimeString()
{
    return FormatJournalRunTimeToString(m_ulJournalRunTime);
}

void CJournalTime::SetJournalRunTime(CStdString* p_pulJournalRunTime)
{
    s_Time thisTime;

    if(FormatJournalRunStringToTimeStruct(*p_pulJournalRunTime, &thisTime))
    {
        m_ulJournalRunTime = FormatStructToTime(&thisTime);
    }
}

void CJournalTime::SetCommandTime(CStdString* p_pCommandTime)
{
    s_Time thisTime;

    if(FormatCommandStringToTimeStruct(*p_pCommandTime, &thisTime))
    {
        m_ulCommandTime = FormatStructToTime(&thisTime);
    }
}

CStdString CJournalTime::FormatJournalRunTimeToString(DWORD p_ulTime)
{
    //Variablen
    CStdString buffer;
    CStdString time;

    s_Time timeStruct;

    //Time to Struct
    FormatTimeToStruct(p_ulTime, &timeStruct);

    //Init
    buffer.Empty();

    //Hours
    buffer.Format(_T("%d:"), timeStruct.s_ulHour);
    if(buffer.GetLength() == 2) {buffer.Insert(0, _T("0"));}
    time = time + buffer;

    //Min
    buffer.Empty();
    buffer.Format(_T("%d:"), timeStruct.s_ulMin);
    if(buffer.GetLength() == 2) {buffer.Insert(0, _T("0"));}
    time = time + buffer;

    //Sec
    buffer.Empty();
    buffer.Format(_T("%d."), timeStruct.s_ulSec);
    if(buffer.GetLength() == 2) {buffer.Insert(0, _T("0"));}
    time = time + buffer;

    //Auf bzw. abrunden von Mikrosekunden
    if(timeStruct.s_ulMicroSec >= 500) timeStruct.s_ulMilliSec++;
    timeStruct.s_ulMicroSec = 0;

    //Millisec
    buffer.Empty();
    buffer.Format(_T("%d"), timeStruct.s_ulMilliSec);
    if(buffer.GetLength() == 1) buffer.Insert(0, _T("00"));
    if(buffer.GetLength() == 2) buffer.Insert(0, _T("0"));
    time = time + buffer;

    return time;
}

CStdString CJournalTime::FormatCommandTimeToString(DWORD p_ulTime)
{
    //Variablen
    CStdString buffer;
    CStdString time;
    s_Time timeStruct;

    //Time to Struct
    FormatTimeToStruct(p_ulTime, &timeStruct);

    //Init
    buffer.Empty();

    //Sec
    buffer.Empty();
    buffer.Format(_T("%d."), timeStruct.s_ulSec);
    if(buffer.GetLength() == 2) {buffer.Insert(0, _T("0"));}
    time = time + buffer;

    //Millisec
    buffer.Empty();
    buffer.Format(_T("%d."), timeStruct.s_ulMilliSec);
    if(buffer.GetLength() == 1) buffer.Insert(0, _T("000"));
    if(buffer.GetLength() == 2) buffer.Insert(0, _T("00"));
    if(buffer.GetLength() == 3) buffer.Insert(0, _T("0"));
    time = time + buffer;

    //Microsec
    buffer.Empty();
    buffer.Format(_T("%d"), timeStruct.s_ulMicroSec);
    if(buffer.GetLength() == 0) buffer.Insert(0, _T("000"));
    if(buffer.GetLength() == 1) buffer.Insert(0, _T("00"));
    if(buffer.GetLength() == 2) buffer.Insert(0, _T("0"));
    time = time + buffer;

    return time;
}

BOOL CJournalTime::FormatTimeToStruct(DWORD p_ulTime, s_Time* p_pThisTime)
{
    if(!p_pThisTime) return FALSE;

    if(p_ulTime > 0)
    {
        //Extraction
        p_pThisTime->s_ulHour = p_ulTime  / HOUR_IN_MICROSEC;

        p_pThisTime->s_ulMin  = (p_ulTime - p_pThisTime->s_ulHour * HOUR_IN_MICROSEC) / MINUTE_IN_MICROSEC;

        p_pThisTime->s_ulSec  = (p_ulTime - p_pThisTime->s_ulHour * HOUR_IN_MICROSEC
                                          - p_pThisTime->s_ulMin * MINUTE_IN_MICROSEC) / SECOND_IN_MICROSEC;

        p_pThisTime->s_ulMilliSec = (p_ulTime - p_pThisTime->s_ulHour * HOUR_IN_MICROSEC
                                              - p_pThisTime->s_ulMin * MINUTE_IN_MICROSEC
                                              - p_pThisTime->s_ulSec * SECOND_IN_MICROSEC) / MILLISEC_IN_MICROSEC;

        p_pThisTime->s_ulMicroSec = (p_ulTime - p_pThisTime->s_ulHour * HOUR_IN_MICROSEC
                                              - p_pThisTime->s_ulMin * MINUTE_IN_MICROSEC
                                              - p_pThisTime->s_ulSec * SECOND_IN_MICROSEC
                                              - p_pThisTime->s_ulMilliSec * MILLISEC_IN_MICROSEC);
    }
    else
    {
        p_pThisTime->s_ulHour = 0;
        p_pThisTime->s_ulMin = 0;
        p_pThisTime->s_ulSec = 0;
        p_pThisTime->s_ulMilliSec = 0;
        p_pThisTime->s_ulMicroSec = 0;
    }

    return TRUE;
}

BOOL CJournalTime::FormatJournalRunStringToTimeStruct(CStdString p_Time, s_Time* p_pThisTime)
{
    //Variablen
    CStdString buffer;
    CStdString value;
    int iZeiger = 0;
    TCHAR* str;

    //Check parameter
    if((!p_pThisTime) || (p_Time.IsEmpty())) return FALSE;

    //Init
    buffer.Empty();

    //Daten �bernehmen
    buffer.Insert(0, p_Time);

    //No microseconds
    p_pThisTime->s_ulMicroSec = 0;

    //Milliseconds
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind('.');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulMilliSec = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulMilliSec = 0;

    //Seconds
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind(':');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulSec = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulSec = 0;

    //Minutes
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind(':');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulMin = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulMin = 0;

    //Hour
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind(':');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulHour = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulHour = 0;

    return TRUE;
}

BOOL CJournalTime::FormatCommandStringToTimeStruct(CStdString p_Time, s_Time* p_pThisTime)
{
    //Variablen
    CStdString buffer;
    CStdString value;
    int iZeiger = 0;
    TCHAR* str;

    //Check parameter
    if((!p_pThisTime) || (p_Time.IsEmpty())) return FALSE;

    //Init
    buffer.Empty();

    //Daten �bernehmen
    buffer.Insert(0, p_Time);

    //Mikroseconds
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind('.');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulMicroSec = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulMicroSec = 0;

    //Milliseconds
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind('.');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulMilliSec = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulMilliSec = 0;

    //Seconds
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind(':');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulSec = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulSec = 0;

    //Minutes
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind(':');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulMin = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulMin = 0;

    //Hour
    if(!buffer.IsEmpty())
    {
        iZeiger = buffer.ReverseFind(':');
        value = buffer.Right(buffer.GetLength() - iZeiger -1);
        p_pThisTime->s_ulHour = _tcstoul(value, &str, 10);
        if(iZeiger == -1) iZeiger = 0;
        buffer.Delete(iZeiger, buffer.GetLength()-iZeiger);
    }
    else p_pThisTime->s_ulHour = 0;

    return TRUE;
}

DWORD CJournalTime::FormatStructToTime(s_Time* p_pThisTime)
{
    DWORD ulTime = 0;

    if(!p_pThisTime) return 0;

    //Contract
    ulTime = p_pThisTime->s_ulMicroSec +
             p_pThisTime->s_ulMilliSec * MILLISEC_IN_MICROSEC +
             p_pThisTime->s_ulSec * SECOND_IN_MICROSEC +
             p_pThisTime->s_ulMin * MINUTE_IN_MICROSEC +
             p_pThisTime->s_ulHour * HOUR_IN_MICROSEC;

    return ulTime;
}

