///////////////////////////////////////////////////////////
// ObjectDictionarySnapShot.h
//
// Project:				CommonLib
// Author:				MMAGCHHE
// Date of Creation:	24-Oct-2008 06:51:00
//
// Description: Creates a snap shot of all read/write objects of any device.
//              ATTENTION: the snap shot is also get/put if there is no communication!
//              This leads to very slow progress. Ensure communication within your code
//              before using the get/put methods!
//
// Copyright © 2008, maxon motor ag.
// All rights reserved.
///////////////////////////////////////////////////////////

#if !defined(OBJECTDICTIONARYSNAPSHOT_F5978711_1BEB_4553_A245_7850786264EC__INCLUDED_)
#define OBJECTDICTIONARYSNAPSHOT_F5978711_1BEB_4553_A245_7850786264EC__INCLUDED_

class CVirtualDeviceCommunicationModel;

class CObjectDictionarySnapShot
{

public:
	CObjectDictionarySnapShot();
	virtual ~CObjectDictionarySnapShot();

    void SetCommunicationModel(CVirtualDeviceCommunicationModel* p_pCommunicationModel);
    bool Get();
    bool Put();

private:
    void PrePut();
    void PostPut();

    __int64 m_aObjectDictionary[3][0xFFFF];
    short m_sObjectCount;
    __int64 m_lDigInExecutionMask;
    CVirtualDeviceCommunicationModel* m_pCommunicationModel;
};
#endif // !defined(OBJECTDICTIONARYSNAPSHOT_F5978711_1BEB_4553_A245_7850786264EC__INCLUDED_)
