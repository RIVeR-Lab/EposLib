// LayerParameterStack.h: Schnittstelle f�r die Klasse CLayerParameterStack.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYERPARAMETERSTACK_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
#define AFX_LAYERPARAMETERSTACK_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommunicationModel/Common/CommunicationModelDefinitions.h>
#include <ParameterSet/ParameterStack.h>

class CLayerParameterSet;

class CLayerParameterStack: public CParameterStack  
{
public:
	CLayerParameterStack();
	virtual ~CLayerParameterStack();
	virtual CLayerParameterStack& operator=(CLayerParameterStack& other);
	virtual BOOL operator==(CLayerParameterStack& other);

	BOOL PushLayer(CLayerParameterSet& p_rParameterSet);
	BOOL PopLayer(ELayer p_eLayer, CLayerParameterSet& p_rParameterSet);
	BOOL PeekLayer(ELayer p_eLayer, CLayerParameterSet& p_rParameterSet);

	BOOL GetLayerParameter(ELayer p_eLayer, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize);
	BOOL GetLayerParameter(ELayer p_eLayer, CStdString p_Name, CStdString& p_rValue);
};

#endif // !defined(AFX_LAYERPARAMETERSTACK_H__3DC7A063_B6AF_46F9_92D6_79FA3EF3E533__INCLUDED_)
