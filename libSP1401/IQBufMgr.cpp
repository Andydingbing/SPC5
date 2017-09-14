#include "IQBufMgr.h"
#include "pthread.h"
#include <string.h>
#include "libLog.h"

#ifdef _MSC_VER
#pragma comment(lib, "pthreadVC2.lib")
#endif

pthread_mutex_t g_IQBufLock = PTHREAD_MUTEX_INITIALIZER;

CIQBufMgr::CIQBufMgr()
{
	m_pI = NULL;
	m_pQ = NULL;
	m_uiSamples = 0;
}

CIQBufMgr::~CIQBufMgr()
{
	SAFE_DELETE(m_pI);
	SAFE_DELETE(m_pQ);
}

CIQBufMgr & CIQBufMgr::operator = (CIQBufMgr &Mgr)
{
	return Mgr;
}

CIQBufMgr & CIQBufMgr::Instance()
{
	static CIQBufMgr g_IQBufMgr;
	return g_IQBufMgr;
}

void CIQBufMgr::New(uint32_t uiSamples)
{
	pthread_mutex_lock(&g_IQBufLock);

	if (uiSamples <= m_uiSamples) {
		memset(m_pI,0,uiSamples * sizeof(int16_t));
		memset(m_pQ,0,uiSamples * sizeof(int16_t));
	}
	else {
		SAFE_DELETE(m_pI);
		SAFE_DELETE(m_pQ);
		m_pI = new int16_t[uiSamples];
		m_pQ = new int16_t[uiSamples];
		m_uiSamples = uiSamples;
	}

	pthread_mutex_unlock(&g_IQBufLock);
}

int16_t *CIQBufMgr::GetI() const
{
	return static_cast<int16_t *>(m_pI);
}

int16_t *CIQBufMgr::GetQ() const
{
	return static_cast<int16_t *>(m_pQ);
}

int16_t CIQBufMgr::GetI(uint32_t uiIdx) const
{
	return m_pI[uiIdx];
}

int16_t CIQBufMgr::GetQ(uint32_t uiIdx) const
{
	return m_pQ[uiIdx];
}

void CIQBufMgr::_2Ptr(int32_t* pI,int32_t* pQ)
{
	pthread_mutex_lock(&g_IQBufLock);

	for (uint32_t i = 0;i < m_uiSamples;i ++) {
		pI[i] = (int32_t)m_pI[i];
		pQ[i] = (int32_t)m_pQ[i];
	}

	pthread_mutex_unlock(&g_IQBufLock);
}
