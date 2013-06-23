///////////////////////////////////////////////////////////
// MmcMessageDialog.h
//
// Project:				CommonLib
// Author:				MMAGCHHE
// Date of Creation:	16-Dec-2010 09:40:00
//
// Description: Include this header in your stdafx.h
//              to use the MmcMessageBox.
//              Possible message types are:
//              -Error with OK button
//              -Warning with OK-CANCEL/YES-NO buttons
//              -Action independent warning with YES-NO-CANCEL buttons
//              -Info with OK button
//              -Question with OK-CANCEL/YES-NO buttons
//              -Action independent question with YES-NO-CANCEL buttons
//
// Copyright © 2010, maxon motor ag.
// All rights reserved.
///////////////////////////////////////////////////////////

#if !defined(AFX_MmcMessageDialog_H__04BABB13_04D6_11D6_A0E2_00A02436C4EF__INCLUDED_)
#define AFX_MmcMessageDialog_H__04BABB13_04D6_11D6_A0E2_00A02436C4EF__INCLUDED_

#include <MmcTypeDefinition.h>

enum EMessageType
{
    MT_ERROR
    , MT_WARNING
    , MT_ACTION_INDEPENDENT_WARNING
    , MT_INFO
    , MT_QUESTION
    , MT_ACTION_INDEPENDENT_QUESTION
};

int MmcMessageDialog(EMessageType p_messageType, CStdString p_message, CStdString p_reason = _T(""), CStdString p_advice  = _T(""), CStdString p_question  = _T(""),  CStdString p_title  = _T(""));

//***************
//Makros for loading Resource Strings
//***************

inline CStdString _LoadResourceCStdString(unsigned int p_resId)
{
    CStdString resourceStr;
#if defined WINVER
    resourceStr.LoadString(p_resId);
#endif
    return resourceStr;
}


#endif // !defined(AFX_MmcMessageDialog_H__04BABB13_04D6_11D6_A0E2_00A02436C4EF__INCLUDED_)

