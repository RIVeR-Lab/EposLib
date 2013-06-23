// ErrorProducer.cpp: Implementierung der Klasse CErrorProducer.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <CommunicationModel/CommonLayer/ErrorHandling/ErrorProducer.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CErrorProducer::CErrorProducer()
{
	m_eLayer = NO_LAYER;
	m_strClassName = "";
	m_strCommandName = "";
}

CErrorProducer::CErrorProducer(ELayer eLayer,CStdString strClassName)
{
	m_eLayer = eLayer;
	m_strClassName = strClassName;
	m_strCommandName = "";
}

CErrorProducer::~CErrorProducer()
{

}

void CErrorProducer::Init(ELayer eLayer,CStdString strClassName,CStdString strCommandName)
{
	m_eLayer = eLayer;
	m_strClassName = strClassName;
	m_strCommandName = strCommandName;
}

void CErrorProducer::Init(ELayer eLayer,CStdString strClassName)
{
	m_eLayer = eLayer;
	m_strClassName = strClassName;
	m_strCommandName = "";
}

CErrorProducer* CErrorProducer::Clone()
{
	CErrorProducer* pNewErrorProducer;

	pNewErrorProducer = new CErrorProducer();
	*pNewErrorProducer = *this;

	return pNewErrorProducer;
}
#ifdef _WIN32
void CErrorProducer::Serialize(CArchive& ar)
{
	int iLayer;

	if(ar.IsStoring())
	{
		ar << m_eLayer;
		ar << m_strClassName;
		ar << m_strCommandName;
	}
	else
	{
		ar >> iLayer; m_eLayer = (ELayer)iLayer;
		ar >> m_strClassName;
		ar >> m_strCommandName;
	}
}
#endif
CErrorProducer& CErrorProducer::operator=(CErrorProducer& other)
{
	if(this != &other)
	{
		m_eLayer = other.m_eLayer;
		m_strClassName = other.m_strClassName;
		m_strCommandName = other.m_strCommandName;
	}

	return *this;
}

void CErrorProducer::SetCommandName(CStdString strCommandName)
{
	m_strCommandName = strCommandName;
}

BOOL CErrorProducer::IsCommandNameSet()
{
	return m_strCommandName.size()>0;
}

ELayer CErrorProducer::GetLayer()
{
	return m_eLayer;
}
	
CStdString CErrorProducer::GetClassName()
{
	return m_strClassName;
}
	
CStdString CErrorProducer::GetCommandName()
{
	return m_strCommandName;
}
