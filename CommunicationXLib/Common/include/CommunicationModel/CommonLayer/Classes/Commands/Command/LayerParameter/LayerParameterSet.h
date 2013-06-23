// LayerParameterSet.h: Schnittstelle f�r die Klasse CLayerParameterSet.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYERPARAMETERSET_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
#define AFX_LAYERPARAMETERSET_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ParameterSet/ParameterSet.h>
#include <MmcTypeDefinition.h>
#include <CommunicationModel/Common/CommunicationModelDefinitions.h>

class CLayerParameterSet: public CParameterSet  
{
public:
	CLayerParameterSet();
	CLayerParameterSet(ELayer eLayer);
	virtual ~CLayerParameterSet();
	virtual CLayerParameterSet& operator=(CLayerParameterSet& other);
	virtual CParameterSet* Clone();
	virtual BOOL operator==(CLayerParameterSet& other);
	BOOL IsLayer(ELayer p_eLayer);

private:
	ELayer m_eLayer;
};

#endif // !defined(AFX_LAYERPARAMETERSET_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
