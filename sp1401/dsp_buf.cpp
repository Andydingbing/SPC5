#include "dsp_buf.h"
#include "pthread.h"
#include <string.h>
#include <complex>
#include <math.h>
#include "fftw3.h"
#include "liblog.h"

#ifdef RD_C_MSC
#pragma comment(lib, "pthreadVC2.lib")
#pragma comment(lib, "libfftw3-3.lib")
#endif

pthread_mutex_t g_dsp_buf_lock = PTHREAD_MUTEX_INITIALIZER;

using namespace sp_rd;

dsp_buf::dsp_buf()
{
    m_i = NULL;
    m_q = NULL;
    m_normalize_freq = NULL;
    m_pwr = NULL;
    m_sr = 245760000;
    m_samples = 1;
    m_hold = PH_NO_HOLD;
}

dsp_buf::~dsp_buf()
{
    SAFE_DELETE(m_i);
    SAFE_DELETE(m_q);
}

dsp_buf & dsp_buf::operator = (dsp_buf &buf)
{
    return buf;
}

void dsp_buf::_new(uint32_t samples)
{
    pthread_mutex_lock(&g_dsp_buf_lock);

    uint32_t dft_pts = dft_get_pts(m_sr,samples);

    if (samples > m_samples) {
        SAFE_DELETE(m_i);
        SAFE_DELETE(m_q);
        m_i = new int16_t[samples];
        m_q = new int16_t[samples];

        SAFE_DELETE(m_normalize_freq);
        SAFE_DELETE(m_pwr);
        m_normalize_freq = new double[dft_pts];
        m_pwr = new double[dft_pts];

        memset(m_i,0,samples * sizeof(int16_t));
        memset(m_q,0,samples * sizeof(int16_t));
        memset(m_normalize_freq,0,dft_pts * sizeof(double));
        memset(m_pwr,0,dft_pts * sizeof(double));
	}

    m_samples = samples;
    pthread_mutex_unlock(&g_dsp_buf_lock);
}

void dsp_buf::set_sr(uint32_t sr)
{
    m_sr = sr;
}

uint32_t dsp_buf::get_sr()
{
    return m_sr;
}

int16_t *dsp_buf::get_i() const
{
    return m_i;
}

int16_t *dsp_buf::get_q() const
{
    return m_q;
}

int16_t dsp_buf::get_i(uint32_t idx) const
{
    return m_i[idx];
}

int16_t dsp_buf::get_q(uint32_t idx) const
{
    return m_q[idx];
}

double *dsp_buf::get_normalize_freq() const
{
    return m_normalize_freq;
}

double *dsp_buf::get_pwr() const
{
    return m_pwr;
}

double dsp_buf::get_normalize_freq(uint32_t idx) const
{
    return m_normalize_freq[idx];
}

double dsp_buf::get_pwr(uint32_t idx) const
{
    return m_pwr[idx];
}

void dsp_buf::_2ptr(int32_t *i,int32_t *q)
{
    pthread_mutex_lock(&g_dsp_buf_lock);

    for (uint32_t idx = 0;idx < m_samples;idx ++) {
        i[idx] = (int32_t)m_i[idx];
        q[idx] = (int32_t)m_q[idx];
	}

    pthread_mutex_unlock(&g_dsp_buf_lock);
}

void dsp_buf::dft()
{
    uint32_t dft_pts = dft_get_pts();
    double pwr_new = 0.0;
    double pwr_old = 0.0;
    fftw_complex *in,*out;
    fftw_plan plan;
    in  = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_samples);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_samples);
    plan = fftw_plan_dft_1d(m_samples,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
    for (uint32_t i = 0;i < m_samples;i ++) {
        in[i][0] = (double)m_i[i];
        in[i][1] = (double)m_q[i];
    }
    fftw_execute(plan);

    for (uint32_t i = 0;i < dft_pts / 2;i ++) {
        m_normalize_freq[i + dft_pts / 2] = m_sr / 2.0 / (double)(dft_pts / 2.0) * i / 1e6;             //positive frequencys
        m_normalize_freq[dft_pts / 2 - 1 - i] = m_sr / 2.0 / (double)(dft_pts / 2.0) * i / 1e6 * -1;	//negative frequencys
    }

    if (PH_NO_HOLD == m_hold) {
        for (uint32_t i = 0;i < dft_pts / 2;i ++) {
            m_pwr[i + dft_pts / 2] = 10.0 * log10((out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 134217728.0 / m_samples / m_samples);
            m_pwr[i] = 10.0 * log10((out[i + dft_pts / 2][0] * out[i + dft_pts / 2][0] + out[i + dft_pts / 2][1] * out[i + dft_pts / 2][1]) / 134217728.0 / m_samples / m_samples);
        }
    }
    else if (PH_MAX_HOLD == m_hold) {
        for (uint32_t i = 0;i < dft_pts / 2;i ++) {
            pwr_new = 10.0 * log10((out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 134217728.0 / m_samples / m_samples);
            pwr_old = m_pwr[i + dft_pts / 2];
            if (pwr_new > pwr_old)
                m_pwr[i + dft_pts / 2] = pwr_new;
        }
        for (uint32_t i = 0;i < dft_pts / 2;i ++) {
            pwr_new = 10.0 * log10((out[i + dft_pts / 2][0] * out[i + dft_pts / 2][0] + out[i + dft_pts / 2][1] * out[i + dft_pts / 2][1]) / 134217728.0 / m_samples / m_samples);
            pwr_old = m_pwr[i];
            if (pwr_new > pwr_old)
                m_pwr[i] = pwr_new;
        }
    }
    else {
        for (uint32_t i = 0;i < dft_pts / 2;i ++) {
            pwr_new = 10.0 * log10((out[i][0] * out[i][0] + out[i][1] * out[i][1]) / 134217728.0 / m_samples / m_samples);
            pwr_old = m_pwr[i + dft_pts / 2];
            if (pwr_new < pwr_old)
                m_pwr[i + dft_pts / 2] = pwr_new;
        }
        for (uint32_t i = 0;i < dft_pts / 2;i ++) {
            pwr_new = 10.0 * log10((out[i + dft_pts / 2][0] * out[i + dft_pts / 2][0] + out[i + dft_pts / 2][1] * out[i + dft_pts / 2][1]) / 134217728.0 / m_samples / m_samples);
            pwr_old = m_pwr[i];
            if (pwr_new < pwr_old)
                m_pwr[i] = pwr_new;
        }
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
}

uint32_t dsp_buf::td_get_samples()
{
    return m_samples;
}

uint32_t dsp_buf::dft_get_pts(uint32_t sr, uint32_t samples)
{
    return (sr / 2) / (sr / samples) * 2;
}

uint32_t dsp_buf::dft_get_pts()
{
    return dft_get_pts(m_sr,m_samples);
}

void dsp_buf::dft_set_hold(dft_pts_hold_t hold)
{
    m_hold = hold;
}
