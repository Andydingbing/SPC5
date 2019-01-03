#ifndef DSP_BUF_H
#define DSP_BUF_H
#include "stdint_common.h"

namespace sp_rd {

class RD_API dsp_buf
{
public:
    dsp_buf();
    ~dsp_buf();
    dsp_buf & operator = (dsp_buf &);
public:
    enum dft_pts_hold_t {
        PH_NO_HOLD = 0,
        PH_MAX_HOLD,
        PH_MIN_HOLD
    };
public:
    void _new(uint32_t samples);
    void set_sr(uint32_t sr);
    uint32_t get_sr();
    int16_t *get_i() const;
    int16_t *get_q() const;
    int16_t get_i(uint32_t idx) const;
    int16_t get_q(uint32_t idx) const;
    double *get_normalize_freq() const;
    double *get_pwr() const;
    double get_normalize_freq(uint32_t idx) const;
    double get_pwr(uint32_t idx) const;
    void _2ptr(int32_t *i,int32_t *q);
    void dft();
    uint32_t td_get_samples();
    uint32_t dft_get_pts();
    uint32_t dft_get_pts(uint32_t sr,uint32_t samples);
    void dft_set_hold(dft_pts_hold_t hold);
private:
    int16_t *m_i;
    int16_t *m_q;
    double *m_normalize_freq;
    double *m_pwr;
    uint32_t m_sr;
    uint32_t m_samples;
private:
    dft_pts_hold_t m_hold;
};

} //namespace sp_rd

#endif // DSP_BUF_H
