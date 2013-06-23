// JournalTime.h: Schnittstelle f�r die Klasse CJournalTime.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOURNALTIME_H__F0946010_64A8_464B_B191_681B3311F1B2__INCLUDED_)
#define AFX_JOURNALTIME_H__F0946010_64A8_464B_B191_681B3311F1B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>

const DWORD HOUR_IN_MICROSEC        = 0xD693A400;   //Hour in microsecond (60*60*1000*1000)
const DWORD MINUTE_IN_MICROSEC      = 0x03938700;   //Min in microsecond (60*1000*1000)
const DWORD SECOND_IN_MICROSEC      = 0x000F4240;   //Sek in microsecond (1000 * 1000)
const DWORD MILLISEC_IN_MICROSEC    = 0x000003E8;   //Ms in microsecond (1000)

class CJournalTime
{
private:
    struct s_Time
    {
        DWORD s_ulHour;
        DWORD s_ulMin;
        DWORD s_ulSec;
        DWORD s_ulMilliSec;
        DWORD s_ulMicroSec;
    };

public:
    CJournalTime();
    virtual ~CJournalTime();

    void Reset();
#ifdef _WIN32
    void Serialize(CArchive& ar);
#endif
    CJournalTime& operator=(CJournalTime& other);

    void SetJournalRunTime(DWORD p_ulJournalRunTime);
    void SetJournalRunTime(CStdString* p_pulJournalRunTime);
    DWORD GetJournalRunTime();
    CStdString GetJournalRunTimeString();

    void SetCommandStartTime(DWORD p_ulCommandStartTime);
    DWORD GetCommandStartTime();

    void SetCommandTime(DWORD p_ulCommandTime);
    void SetCommandTime(CStdString* p_pCommandTime);
    DWORD GetCommandTime();
    CStdString GetCommandTimeString();

private:
    //Common
    DWORD FormatStructToTime(s_Time* p_pThisTime);
    BOOL FormatTimeToStruct(DWORD p_ulTime, s_Time* p_pThisTime);

    //CommandTime
    BOOL FormatCommandStringToTimeStruct(CStdString p_Time, s_Time* p_pThisTime);
    CStdString FormatCommandTimeToString(DWORD p_ulTime);

    //JournalRunTime
    BOOL FormatJournalRunStringToTimeStruct(CStdString p_Time, s_Time* p_pThisTime);
    CStdString FormatJournalRunTimeToString(DWORD p_ulTime);

    DWORD m_ulCommandStartTime;
    DWORD m_ulCommandTime;
    DWORD m_ulJournalRunTime;
};

#endif // !defined(AFX_JOURNALTIME_H__F0946010_64A8_464B_B191_681B3311F1B2__INCLUDED_)
