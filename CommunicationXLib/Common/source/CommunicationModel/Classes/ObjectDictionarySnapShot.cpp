///////////////////////////////////////////////////////////
// ObjectDictionarySnapShot.cpp
//
// Project:				CommonLib
// Author:				MMAGCHHE
// Date of Creation:	24-Oct-2008 06:51:00
//
// See header file for description.
//
// Copyright � 2008, maxon motor ag.
// All rights reserved.
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include <Classes/TypeDefinitions.h>
#include <ObjectDictionary/ObjectDictionary.h>
#include <CommunicationModel/VirtualDeviceCommunicationModel.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenCommunicationDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsCanOpenDriveDef.h>
#include <CommunicationModel/CommonLayer/Classes/Commands/DeviceCommandSet/DcsEpos2Def.h>
#include <CommunicationModel/Classes/ObjectDictionarySnapShot.h>
#include <MmcTypeDefinition.h>

//Constants
const WORD FIRST_OBJECT_INDEX(0x1005); //'COB-ID SYNC Message' is first object; like this writing 'Number of Errors' is avoided!

//***************
//Constructors/Destructors
//***************

CObjectDictionarySnapShot::CObjectDictionarySnapShot():
    m_pCommunicationModel(0)
    , m_sObjectCount(0)
    , m_lDigInExecutionMask(0)
{
}

CObjectDictionarySnapShot::~CObjectDictionarySnapShot()
{
}

//***************
//Public methods
//***************

void CObjectDictionarySnapShot::SetCommunicationModel(CVirtualDeviceCommunicationModel* p_pCommunicationModel)
{
    m_pCommunicationModel = p_pCommunicationModel;
}

bool CObjectDictionarySnapShot::Get()
{
    bool oRetVal(false);

    if (m_pCommunicationModel)
    {
        __int64 value(0);
        BOOL oSuccess(FALSE);
        m_sObjectCount = 0;
        CObjectDictionary* pObjectDictionary(m_pCommunicationModel->GetObjectDictionary());
        CObjectEntryIterator* pIterator(0);
        CObjectEntry* pObjectEntry(0);
        WORD usIndex(0);
        BYTE ucSubIndex(0);
        EAccessType accessType(AT_READ_WRITE);

        //Create object dictionary iterator. This method uses new!!!
        if(pObjectDictionary)
        {
            pIterator = pObjectDictionary->CreateObjectEntryIterator();
            pIterator->First();
            while(!pIterator->IsFinished())
            {
                pObjectEntry = (CObjectEntry*)pIterator->Current();
                if(pObjectEntry)
                {
                    accessType = pObjectEntry->GetAccessType();
                    if((AT_READ_WRITE == accessType) || (AT_READ_WRITE_READ == accessType) || (AT_READ_WRITE_WRITE == accessType))
                    {
                        usIndex = pObjectEntry->GetIndex();
                        if((FIRST_OBJECT_INDEX <= usIndex) && (ODT_DOMAIN != pObjectEntry->GetDataTypeNumber()))
                        {
                            ucSubIndex = pObjectEntry->GetSubIndex();
                            m_pCommunicationModel->GetObject(usIndex, ucSubIndex, &value, 0);
                            m_aObjectDictionary[0][m_sObjectCount] = usIndex;
                            m_aObjectDictionary[1][m_sObjectCount] = ucSubIndex;
                            m_aObjectDictionary[2][m_sObjectCount] = value;
                            m_sObjectCount++;
                        }
                    }
                }
                pIterator->Next();
            }
            //Delete the iterator!!
            delete pIterator;
        }
        if(m_sObjectCount > 0)
        {
            oRetVal = true;
        }
    }
    return oRetVal;
}

bool CObjectDictionarySnapShot::Put()
{
    bool oRetVal(false);

    if (m_pCommunicationModel && (m_sObjectCount > 0))
    {
        oRetVal = true;
        //Pre put actions
        PrePut();
        //Write all RW objects back to device. Reverse order is used due to object writing dependencies
        for(long i = m_sObjectCount - 1; i >= 0 ; i--)
        {
            m_pCommunicationModel->SetObject((WORD)m_aObjectDictionary[0][i], (BYTE)m_aObjectDictionary[1][i], m_aObjectDictionary[2][i], 0);
        }
        //Post put actions
        PostPut();
    }
    return oRetVal;
}

//***************
//Protected methods
//***************

//***************
//Private methods
//***************

void CObjectDictionarySnapShot::PrePut()
{
    //In order to put the snap shot back into the device, some object need to be set to certain values
    //due to the fact that some object values might prevent other objects from beeing written!
    const short NUMBER_OF_PDO_MAPPINGS = 4;
    const short DC_MOTOR(0x0001);
    const WORD PDO_VALUE(0x00);
    const BYTE SUB_INDEX = 0;
    const WORD SENSOR_INC_ENC = 0x02;
    const WORD SENSOR_HALL = 0x03;
    const BYTE POLE_PAIR = 0x01;
    const DWORD VELOCITY = 0x000186A0;
    WORD ulIndex(0);
    unsigned __int64 ulMotorType(0);
    WORD usSensorType(0);
    WORD usSwVersion(0);

    //Reset number of mapped application objects in order to be able to set the mapped objects
    for(ulIndex = INDEX_RX_PDO1_MAPPING; ulIndex < INDEX_RX_PDO1_MAPPING + NUMBER_OF_PDO_MAPPINGS; ulIndex++)
    {
        m_pCommunicationModel->SetObject(ulIndex, SUB_INDEX, PDO_VALUE, 0);
    }
    for(ulIndex = INDEX_TX_PDO1_MAPPING; ulIndex < INDEX_TX_PDO1_MAPPING + NUMBER_OF_PDO_MAPPINGS; ulIndex++)
    {
        m_pCommunicationModel->SetObject(ulIndex, SUB_INDEX, PDO_VALUE, 0);
    }

    //Set sensor type according to motor type (this is only necessary with FW 0x2101)
    if(m_pCommunicationModel->GetObject(INDEX_VERSION, SUBINDEX_SOFTWARE_VERSION, &usSwVersion, 0))
    {
        if(0x2101 >= usSwVersion)
        {
            for(int i = 0; i < m_sObjectCount; i++)
            {
                if(INDEX_MOTOR_TYPE == m_aObjectDictionary[0][i])
                {
                    if(SUBINDEX_MOTOR_TYPE == m_aObjectDictionary[1][i])
                    {
                        ulMotorType = m_aObjectDictionary[2][i];
                        break;
                    }
                }
            }
            if(DC_MOTOR == ulMotorType) usSensorType = SENSOR_INC_ENC;
            else usSensorType = SENSOR_HALL;
            m_pCommunicationModel->SetObject(INDEX_SENSOR_CONFIGURATION, SUBINDEX_POSITION_SENSOR_TYPE, usSensorType, 0);
        }
    }
    //Get the digital input execution mask value and store it seperately.
    //During put, zero (0) is written to ensure no input activities.
        for(int i = 0; i < m_sObjectCount; i++)
        {
            if(INDEX_DIGITAL_INPUT_FUNCTIONALITIES == m_aObjectDictionary[0][i])
            {
                if(SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK == m_aObjectDictionary[1][i])
                {
                    m_lDigInExecutionMask = m_aObjectDictionary[2][i];
                    m_aObjectDictionary[2][i] = 0;
                    break;
                }
            }
        }
    //Set Pole pair number
    m_pCommunicationModel->SetObject(INDEX_MOTOR_DATA, SUBINDEX_POLE_PAIR_NUMBER, POLE_PAIR, 0);
}

void CObjectDictionarySnapShot::PostPut()
{
    //Perform some necesarray post put actions!
    DWORD ulCount(0);

    //Write the stored digital input execution mask
    m_pCommunicationModel->SetObject(INDEX_DIGITAL_INPUT_FUNCTIONALITIES, SUBINDEX_DIGITAL_INPUT_EXECUTION_MASK, m_lDigInExecutionMask, 0);
    //Disable device! The device might be enabled because of a probably defined digital enable input.
    m_pCommunicationModel->SetObject(INDEX_CONTROL_WORD, SUBINDEX_CONTROL_WORD, (WORD)0, 0);
    //Write configuration date/time
    for(long i = 0; i < m_sObjectCount; i++)
    {
        if((INDEX_VERIFY_CONFIG == m_aObjectDictionary[0][i]) && (SUBINDEX_CONFIG_TIME >= m_aObjectDictionary[1][i]))
        {
            m_pCommunicationModel->SetObject((WORD)m_aObjectDictionary[0][i], (BYTE)m_aObjectDictionary[1][i], m_aObjectDictionary[2][i], 0);
            ulCount++;
        }
        if(ulCount >= 2) break;
    }
}
