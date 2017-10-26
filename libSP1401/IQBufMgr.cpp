#include "IQBufMgr.h"
#include "pthread.h"
#include <string.h>
#include <complex>
#include <math.h>
#include "fftw3.h"
#include "libLog.h"

#ifdef _MSC_VER
#pragma comment(lib, "pthreadVC2.lib")
#pragma comment(lib, "libfftw3-3.lib")
#endif

pthread_mutex_t g_IQBufLock = PTHREAD_MUTEX_INITIALIZER;

CIQBufMgr::CIQBufMgr()
{
	m_pI = NULL;
	m_pQ = NULL;
    m_pNormFreq = NULL;
    m_pPower = NULL;
    m_uiSr = 245760000;
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

CIQBufMgr &CIQBufMgr::Instance()
{
    static CIQBufMgr g_IQBufMgr = CIQBufMgr();
    return g_IQBufMgr;
}

void CIQBufMgr::New(uint32_t uiSamples)
{
	pthread_mutex_lock(&g_IQBufLock);

    uint32_t uiDFTPts = DFTGetPts(m_uiSr,uiSamples);

    if (uiSamples > m_uiSamples) {
		SAFE_DELETE(m_pI);
		SAFE_DELETE(m_pQ);
		m_pI = new int16_t[uiSamples];
		m_pQ = new int16_t[uiSamples];

        SAFE_DELETE(m_pNormFreq);
        SAFE_DELETE(m_pPower);
        m_pNormFreq = new double[uiDFTPts];
        m_pPower = new double[uiDFTPts];

	}

    memset(m_pI,0,uiSamples * sizeof(int16_t));
    memset(m_pQ,0,uiSamples * sizeof(int16_t));
    memset(m_pNormFreq,0,uiDFTPts * sizeof(double));
    memset(m_pPower,0,uiDFTPts * sizeof(double));
    m_uiSamples = uiSamples;
	pthread_mutex_unlock(&g_IQBufLock);
}

void CIQBufMgr::SetSr(uint32_t uiSr)
{
    m_uiSr = uiSr;
}

uint32_t CIQBufMgr::GetSr()
{
    return m_uiSr;
}

int16_t *CIQBufMgr::GetI() const
{
    return m_pI;
}

int16_t *CIQBufMgr::GetQ() const
{
    return m_pQ;
}

int16_t CIQBufMgr::GetI(uint32_t uiIdx) const
{
	return m_pI[uiIdx];
}

int16_t CIQBufMgr::GetQ(uint32_t uiIdx) const
{
	return m_pQ[uiIdx];
}

double *CIQBufMgr::GetNormFreq() const
{
    return m_pNormFreq;
}

double *CIQBufMgr::GetPower() const
{
    return m_pPower;
}

double CIQBufMgr::GetNormFreq(uint32_t uiIdx) const
{
    return m_pNormFreq[uiIdx];
}

double CIQBufMgr::GetPower(uint32_t uiIdx) const
{
    return m_pPower[uiIdx];
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

void CIQBufMgr::DFT()
{
    uint32_t uiDFTPts = DFTGetPts();
    fftw_complex *in,*out;
    fftw_plan plan;
    in  = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_uiSamples);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_uiSamples);
    plan = fftw_plan_dft_1d(m_uiSamples,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
    for (uint32_t i = 0;i < m_uiSamples;i ++) {
        in[i][0] = (double)m_pI[i];
        in[i][1] = (double)m_pQ[i];
    }
    fftw_execute(plan);
    for (uint32_t i = 0;i < uiDFTPts / 2;i ++) {
        m_pNormFreq[i + uiDFTPts / 2] = m_uiSr / 2.0 / (double)(uiDFTPts / 2.0) * i / 1e6;
        m_pPower[i + uiDFTPts / 2] = 10.0 * log10((out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 134217728.0 / m_uiSamples / m_uiSamples);
    }
    for (uint32_t i = 0;i < uiDFTPts / 2;i ++) {
        m_pNormFreq[uiDFTPts / 2 - 1 - i] = m_uiSr / 2.0 / (double)(uiDFTPts / 2.0) * i / 1e6 * -1;	//negative frequencys
        m_pPower[i] = 10.0 * log10((out[i + uiDFTPts / 2][0] * out[i + uiDFTPts / 2][0] + out[i + uiDFTPts / 2][1] * out[i + uiDFTPts / 2][1]) / 134217728.0 / m_uiSamples / m_uiSamples);
    }
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
}

uint32_t CIQBufMgr::TDGetSamples()
{
    return m_uiSamples;
}

uint32_t CIQBufMgr::DFTGetPts(uint32_t uiSr, uint32_t uiSamples)
{
    return (uiSr / 2) / (uiSr / uiSamples) * 2;
}

uint32_t CIQBufMgr::DFTGetPts()
{
    return DFTGetPts(m_uiSr,m_uiSamples);
}
