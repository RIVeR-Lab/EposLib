// ErrorInfo.h: Schnittstelle f�r die Klasse CErrorInfo.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORINFO_H__7D85F2D8_6353_4EAE_BDCF_48F4B220315B__INCLUDED_)
#define AFX_ERRORINFO_H__7D85F2D8_6353_4EAE_BDCF_48F4B220315B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include "Error.h"
#include "ErrorProducer.h"

class CErrorInfo
{
public:
	void Reset();
	void SetCommandName(CStdString strCommandName);
	BOOL IsCommandNameSet();
	DWORD GetErrorCode();
	CErrorInfo();
	virtual ~CErrorInfo();

	CErrorInfo& operator=(CErrorInfo& other);
	CErrorInfo* Clone();
#ifdef _WIN32
	virtual void Serialize(CArchive &ar);
#endif //_WIN32
	void Init(CError* pError,CErrorProducer* pErrorProducer);
	void Init(DWORD dErrorCode,CErrorProducer *pErrorProducer);
	void Init(DWORD dErrorCode);

private:
	void DeleteError();
	void DeleteErrorProducer();

protected:
	CError* m_pError;
	CErrorProducer* m_pErrorProducer;
};

#endif // !defined(AFX_ERRORINFO_H__7D85F2D8_6353_4EAE_BDCF_48F4B220315B__INCLUDED_)
