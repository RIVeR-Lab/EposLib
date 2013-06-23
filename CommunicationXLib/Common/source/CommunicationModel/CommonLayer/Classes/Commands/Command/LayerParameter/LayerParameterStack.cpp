// LayerParameterStack.cpp: Implementierung der Klasse CLayerParameterStack.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterSet.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/Command/LayerParameter/LayerParameterStack.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CLayerParameterStack::CLayerParameterStack()
{
}

CLayerParameterStack::~CLayerParameterStack()
{
}

CLayerParameterStack& CLayerParameterStack::operator=(CLayerParameterStack& other)
{
    if(this != &other)
    {
        *((CParameterStack*)this) = *((CParameterStack*)&other);
    }

    return *this;
}

BOOL CLayerParameterStack::operator==(CLayerParameterStack& other)
{
    BOOL oResult = TRUE;

    std::list<CParameterSet*>::iterator it2 = other.m_ParameterSetList.begin();

    for(std::list<CParameterSet*>::iterator it = m_ParameterSetList.begin();
            		it!=m_ParameterSetList.end(); it++)
	{
		CLayerParameterSet* pParameterSet = (CLayerParameterSet*)(*it);

		if( it2!=other.m_ParameterSetList.end())
		{
			CLayerParameterSet* pParameterSet = (CLayerParameterSet*)(*it);
			CLayerParameterSet* pOtherParameterSet = (CLayerParameterSet*)(*it2++);

			if(!(*pParameterSet == *pOtherParameterSet))
			{
				oResult = FALSE;
				break;
			}
		}
	}

    return oResult;
}

BOOL CLayerParameterStack::PushLayer(CLayerParameterSet& p_rParameterSet)
{
    BOOL oResult = FALSE;

    oResult = Push((CParameterSet&)p_rParameterSet);

    return oResult;
}

BOOL CLayerParameterStack::PopLayer(ELayer p_eLayer, CLayerParameterSet& p_rParameterSet)
{
    CLayerParameterSet* pParameterSet = NULL;
    BOOL oResult = FALSE;

    if(m_ParameterSetList.size() > 0)
    {
    	std::list<CParameterSet*>::iterator it = m_ParameterSetList.begin();

        pParameterSet = (CLayerParameterSet*)(*it);
        if(pParameterSet)
        {
            //Check if Layer is correct
            if(pParameterSet->IsLayer(p_eLayer))
            {
                //Copy
                p_rParameterSet = *pParameterSet;
                m_ParameterSetList.erase(it);
                delete pParameterSet;
                oResult = TRUE;
            }
        }
    }

    return oResult;
}

BOOL CLayerParameterStack::PeekLayer(ELayer p_eLayer, CLayerParameterSet& p_rParameterSet)
{
    CLayerParameterSet* pParameterSet = NULL;
    BOOL oResult = FALSE;

    if(m_ParameterSetList.size() > 0)
    {
    	std::list<CParameterSet*>::iterator it = m_ParameterSetList.begin();
    	if( it != m_ParameterSetList.end() )
    	{
			pParameterSet = (CLayerParameterSet*)(*it);
			if(pParameterSet)
			{
				//Check if Layer is correct
				if(pParameterSet->IsLayer(p_eLayer))
				{
					//Copy
					p_rParameterSet = *pParameterSet;
					oResult = TRUE;
				}
			}
    	}
    }

    return oResult;
}

BOOL CLayerParameterStack::GetLayerParameter(ELayer p_eLayer, CStdString p_Name, BYTE* p_pValue, DWORD p_ulSize)
{
    BOOL oResult = FALSE;

    for(std::list<CParameterSet*>::iterator it = m_ParameterSetList.begin();
    		it!=m_ParameterSetList.end(); it++)
    {
    	CLayerParameterSet* pParameterSet = (CLayerParameterSet*)(*it);
        if(pParameterSet && pParameterSet->IsLayer(p_eLayer))
        {
            oResult = pParameterSet->GetParameter(p_Name, p_pValue, p_ulSize);
            break;
        }
    }

    return oResult;
}

BOOL CLayerParameterStack::GetLayerParameter(ELayer p_eLayer, CStdString p_Name, CStdString& p_rValue)
{
    BOOL oResult = FALSE;

    for(std::list<CParameterSet*>::iterator it = m_ParameterSetList.begin();
        		it!=m_ParameterSetList.end(); it++)
    {
      	CLayerParameterSet* pParameterSet = (CLayerParameterSet*)(*it);
        if(pParameterSet && pParameterSet->IsLayer(p_eLayer))
        {
            oResult = pParameterSet->GetParameter(p_Name, p_rValue);
            break;
        }
    }

    return oResult;
}


