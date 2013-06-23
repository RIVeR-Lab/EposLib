// LayerParameterSet.cpp: Implementierung der Klasse CLayerParameterSet.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CLayerParameterSet::CLayerParameterSet()
{
	m_eLayer = NO_LAYER;
}

CLayerParameterSet::CLayerParameterSet(ELayer eLayer):
	CParameterSet(),
	m_eLayer(eLayer)
{
	
}

CLayerParameterSet::~CLayerParameterSet()
{
	
}

CParameterSet* CLayerParameterSet::Clone()
{
	CLayerParameterSet* pParameterSet = NULL;

	pParameterSet = new CLayerParameterSet();
	*pParameterSet = *this;

	return (CParameterSet*)pParameterSet;
}

CLayerParameterSet& CLayerParameterSet::operator=(CLayerParameterSet& other)
{
	if(this != &other)
	{
		*((CParameterSet*)this) = *((CParameterSet*)&other);
		m_eLayer = other.m_eLayer;
	}

	return *this;
}

BOOL CLayerParameterSet::operator==(CLayerParameterSet& other)
{
	BOOL oResult = TRUE;

	if(this != &other)
	{
		oResult = FALSE;
		if(*((CParameterSet*)this) == *((CParameterSet*)&other))
		{
			if(m_eLayer == other.m_eLayer)
			{
				oResult = TRUE;
			}
		}
	}
	
	return oResult;
}

BOOL CLayerParameterSet::IsLayer(ELayer p_eLayer)
{
	BOOL oResult = FALSE;

	oResult = (m_eLayer == p_eLayer);

	return oResult;
}
