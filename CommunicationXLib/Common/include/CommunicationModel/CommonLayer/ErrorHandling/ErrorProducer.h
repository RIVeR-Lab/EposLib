// ErrorProducer.h: Schnittstelle f�r die Klasse CErrorProducer.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORPRODUCER_H__3D25032E_223C_4B90_A0B8_5E6A6586EEA4__INCLUDED_)
#define AFX_ERRORPRODUCER_H__3D25032E_223C_4B90_A0B8_5E6A6586EEA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MmcTypeDefinition.h>
#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

class CErrorProducer
{
public:
	BOOL IsCommandNameSet();
	CErrorProducer();
	CErrorProducer(ELayer eLayer,CStdString strClassName);
	virtual ~CErrorProducer();

	void Init(ELayer eLayer,CStdString strClassName,CStdString strCommandName);
	void Init(ELayer eLayer,CStdString strClassName);
	void SetCommandName(CStdString strCommandName);

	ELayer GetLayer();

	CStdString GetClassName();
	CStdString GetCommandName();

	virtual CErrorProducer* Clone();
#ifdef _WIN32
	virtual void Serialize(CArchive& ar);
#endif //_WIN32
	CErrorProducer& operator=(CErrorProducer& other);

private:

	ELayer m_eLayer;
	CStdString m_strClassName;
	CStdString m_strCommandName;
};

#endif // !defined(AFX_ERRORPRODUCER_H__3D25032E_223C_4B90_A0B8_5E6A6586EEA4__INCLUDED_)
