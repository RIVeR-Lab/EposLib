// DeviceErrorHandling.h: Schnittstelle f�r die Klasse CDeviceErrorHandling.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICEERRORHANDLING_H__35065F20_2D36_4682_A442_3E76A0DD00E2__INCLUDED_)
#define AFX_DEVICEERRORHANDLING_H__35065F20_2D36_4682_A442_3E76A0DD00E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DeviceErrorCode_Definitions.h"
#include <MmcTypeDefinition.h>

class CDeviceErrorHandling
{
public:
	CDeviceErrorHandling();
	virtual ~CDeviceErrorHandling();

	static BOOL GetErrorDescription(DWORD dErrorCode, CStdString* pDescription);

private:
	static BOOL GetErrorDescription_EPOS(WORD dErrorCode, CStdString *pDescription);
	static BOOL GetErrorDescription_EPOS2(WORD dErrorCode, CStdString *pDescription);
	static BOOL GetErrorDescription_EPOS_P(WORD dErrorCode, CStdString *pDescription);
	static BOOL GetErrorDescription_EPOS2_P(WORD dErrorCode, CStdString *pDescription);
};

#endif // !defined(AFX_DEVICEERRORHANDLING_H__35065F20_2D36_4682_A442_3E76A0DD00E2__INCLUDED_)


