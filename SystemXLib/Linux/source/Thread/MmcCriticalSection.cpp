// CriticalSectionEx.cpp: Implementierung der Klasse CCriticalSectionEx.
//
//////////////////////////////////////////////////////////////////////
#include <unistd.h>
#include <errno.h>
#include <Thread/MmcCriticalSection.h>

CMmcSemaphore::CMmcSemaphore(int lInitialCount,
							 int lMaxCount,
							 CStdString strName)
: CMmcCriticalSection()
{
	//TODO CMmcSemaphore
}

/////////////////////////////////////////////////////////////////////////////
// CSingleLock
CMmcSingleLock::CMmcSingleLock(CMmcCriticalSection* pObject, bool bInitialLock)
{
	m_pObject = pObject;
	m_bAcquired = false;

	if (bInitialLock)
		Lock();
}

CMmcSingleLock::CMmcSingleLock(CMmcSemaphore* pObject, bool bInitialLock)
{
	m_pObject = pObject;
	m_bAcquired = false;

	if (bInitialLock)
		Lock();
}

CMmcSingleLock::~CMmcSingleLock()
{
	if( IsLocked() )
		Unlock();
}

bool CMmcSingleLock::Lock(unsigned int timeout /*=0*/)
{
	m_bAcquired = true;
	if( m_pObject == 0 )
	{
		m_bAcquired = true;
		return true;
	}

	if( timeout != 0 )
	{
		int rc = pthread_mutex_trylock(reinterpret_cast<pthread_mutex_t*>(m_pObject));
		unsigned int tm = timeout;
		__useconds_t k_shortTimeout = 100;
		while(rc!=0 && tm > 0)
		{
			m_bAcquired = false;

			switch(rc)
			{
				case EBUSY: break;
				case EINVAL: return false;
				case EFAULT: return false;
			}

			rc = pthread_mutex_trylock(reinterpret_cast<pthread_mutex_t*>(m_pObject));

			usleep(k_shortTimeout);
			tm-=k_shortTimeout;
		}

		m_bAcquired = (rc == 0);
	}
	else
	{
		m_bAcquired = (pthread_mutex_trylock (reinterpret_cast<pthread_mutex_t*>(m_pObject))==0);
	}

	return m_bAcquired;
}

bool CMmcSingleLock::Unlock()
{
	if( m_pObject  == 0 )
	{
		m_bAcquired = false;
	}

	if (m_bAcquired)
		m_bAcquired = !(pthread_mutex_unlock (reinterpret_cast<pthread_mutex_t*>(m_pObject))==0);

	// successfully unlocking means it isn't acquired
	return !m_bAcquired;
}

bool CMmcSingleLock::IsLocked()
{
	return m_bAcquired;
}

CMmcCriticalSectionEx::CMmcCriticalSectionEx()
{
	m_Name = "";
	m_oAbortLock = false;
}

CMmcCriticalSectionEx::~CMmcCriticalSectionEx()
{
	DeleteThreadIdToSingleLockMap();
}

void CMmcCriticalSectionEx::Init(CStdString name)
{
	m_Name = name;
}

bool CMmcCriticalSectionEx::Lock(unsigned int dTimeout, CStdString strTrace)
{
	const unsigned int k_SmallTimeout = 100;
	const unsigned int k_ShortTimeout = 100;

	unsigned int dStartTime;
	unsigned int dTime;
	bool oLockResult = false;
	bool oAbortLock = false;

	CMmcSingleLock* pSingleLock;
	pSingleLock = FindSingleLock();
	if(pSingleLock)
	{
		if(!pSingleLock->IsLocked())
		{
			//Try to Lock
			dTime = dTimeout;
			while(!oLockResult && !oAbortLock && (dTime > 0))
			{
				oLockResult = pSingleLock->Lock(k_SmallTimeout);

				dTime -= k_ShortTimeout;

				oAbortLock = m_oAbortLock;
                usleep(k_ShortTimeout);
			}

			//Update Lock State
			if(oLockResult)
			{
				return true;
			}
		}
	}

	return false;
}

bool CMmcCriticalSectionEx::Unlock(CStdString strTrace)
{
	CMmcSingleLock* pSingleLock;

	pSingleLock = FindSingleLock();
	if(pSingleLock)
	{
		if(pSingleLock->IsLocked())
		{
			if(pSingleLock->Unlock())
			{
				return true;
			}
		}
	}

	return false;
}

bool CMmcCriticalSectionEx::IsLocked(CStdString strTrace)
{
	CMmcSingleLock* pSingleLock;

	pSingleLock = FindSingleLock();
	if(pSingleLock)
	{
		return pSingleLock->IsLocked();
	}

	return false;
}

bool CMmcCriticalSectionEx::AbortLock(bool oActive)
{
	m_oAbortLock = oActive;
	return true;
}

void CMmcCriticalSectionEx::DeleteThreadIdToSingleLockMap()
{
	for( ThreadToSingleLockMap::iterator it=m_MapThreadIdToSingleLock.begin();
			it != m_MapThreadIdToSingleLock.end(); it++ )
	{
		CMmcSingleLock* pSingleLock = (*it).second;
		if(pSingleLock)
			delete pSingleLock;
	}
}

CMmcSingleLock* CMmcCriticalSectionEx::FindSingleLock()
{
	pthread_t threadId = pthread_self();
	CMmcSingleLock* pSingleLock = NULL;

	ThreadToSingleLockMap::iterator it=m_MapThreadIdToSingleLock.find(threadId);

	if( it == m_MapThreadIdToSingleLock.end() )
	{
		pSingleLock = new CMmcSingleLock(&m_CriticalSection,false);

		m_MapThreadIdToSingleLock.insert(ThreadToSinglePair(threadId,pSingleLock));
	}
	else
		pSingleLock = (*it).second;

	return pSingleLock;
}
