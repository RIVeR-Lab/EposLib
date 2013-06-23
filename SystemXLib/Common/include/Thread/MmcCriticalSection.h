// CriticalSectionEx.h: Schnittstelle f�r die Klasse CCriticalSectionEx.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRITICALSECTIONEX_H__C8A0C91A_4938_420B_B69D_18B5C4B00CA5__INCLUDED_)
#define AFX_CRITICALSECTIONEX_H__C8A0C91A_4938_420B_B69D_18B5C4B00CA5__INCLUDED_

#include <map>
#include <string>
#include <StdString.h>

#ifdef _WIN32

	#ifdef _MFC_VER
		#include<afxmt.h>
		typedef CSingleLock CMmcSingleLock;
		typedef CCriticalSection CMmcCriticalSection;
		typedef CSemaphore CMmcSemaphore;
	#else
		class CMmcSingleLock;
		class CMmcCriticalSection {};
		class CMmcSemaphore;
	#endif

	typedef DWORD	pthread_t;
#else
#include <pthread.h>
//////////////////////////////////////////////////////////
// CCriticalSection
//
class CMmcCriticalSection
{
public:
	CMmcCriticalSection()
	{
		pthread_mutex_init(&m_sync, NULL);
	}
	~CMmcCriticalSection()
	{
		pthread_mutex_destroy(&m_sync);
	}

	operator pthread_mutex_t* ()
	{
		return &this->m_sync;
	}

private:
	pthread_mutex_t m_sync;
};

/////////////////////////////////////////////////////////////////////////////
// CMmcSemaphore
class CMmcSemaphore : public CMmcCriticalSection
{
public:
	CMmcSemaphore( int lInitialCount = 1,
				   int lMaxCount = 1, CStdString strName="");
};

/////////////////////////////////////////////////////////////////////////////
// CMmcSingleLock
class CMmcSingleLock
{
public:
	CMmcSingleLock(CMmcCriticalSection* pObject, bool bInitialLock);
	CMmcSingleLock(CMmcSemaphore* pObject, bool bInitialLock);
	virtual ~CMmcSingleLock();
	bool Lock(unsigned int timeout=0);
	bool Unlock();
	bool IsLocked();

private:
	CMmcCriticalSection* m_pObject;
	bool m_bAcquired;
};
#endif
/////////////////////////////////////////////////////////////////////////////
// CCriticalSectionEx
class CMmcCriticalSectionEx
{
public:
	bool AbortLock(bool oActive);
	CMmcCriticalSectionEx();
	virtual ~CMmcCriticalSectionEx();
	void Init(CStdString name);

	//CriticalSection Lock
	bool Lock(unsigned int dTimeout, CStdString strTrace="");
	bool Unlock(CStdString strTrace="");
	bool IsLocked(CStdString strTrace="");

private:
	CMmcSingleLock* FindSingleLock();
	void DeleteThreadIdToSingleLockMap();

	CStdString m_Name;
	bool m_oAbortLock;

	CMmcCriticalSection m_CriticalSection;
	typedef std::map<pthread_t,CMmcSingleLock*> ThreadToSingleLockMap;
	typedef std::pair<pthread_t, CMmcSingleLock*> ThreadToSinglePair;

	ThreadToSingleLockMap m_MapThreadIdToSingleLock;
};

#endif // !defined(AFX_CRITICALSECTIONEX_H__C8A0C91A_4938_420B_B69D_18B5C4B00CA5__INCLUDED_)
