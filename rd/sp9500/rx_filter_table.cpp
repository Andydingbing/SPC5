#include "rx_filter_table.h"
#include "algo_math.hpp"

using namespace rd;
using namespace rd::ns_sp1401;
using namespace rd::ns_sp2401;

// rf frequency response
rx_rf_fr_table::data_f_t::data_f_t()
{
    freq = 0;
    pwr = 0.0;
}

void rx_rf_fr_table::add(data_f_t *data)
{
    uint32_t idx = SERIE_INDEX(data->freq,rx_filter_freq_star,rx_filter_freq_step);
    m_data_f[idx] = *data;
}

void rx_rf_fr_table::get(uint64_t freq,data_f_t *data)
{
    data->pwr = m_data_f[SERIE_INDEX(freq,rx_filter_freq_star,rx_filter_freq_step)].pwr;
    data->freq = freq;
}

void rx_rf_fr_table::save_as(const char *path)
{

}

// if frequency response
rx_if_fr_table::data_f_t::data_f_t()
{
    freq = 0.0;
    pwr = 0.0;
}

void rx_if_fr_table::add(data_f_t *data)
{
    uint32_t idx = SERIE_INDEX(data->freq,ul_filter_160M_freq_star,ul_filter_freq_step);
    m_data_f[idx] = *data;
}

void rx_if_fr_table::get(double freq,data_f_t *data)
{
    data->pwr = m_data_f[SERIE_INDEX(freq,ul_filter_160M_freq_star,ul_filter_freq_step)].pwr;
    data->freq = freq;
}

void rx_if_fr_table::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (nullptr == fp)
		return;

    data_f_t data;
    for (double freq = ul_filter_160M_freq_star;freq <= ul_filter_160M_freq_stop;freq += ul_filter_freq_step) {
        get(freq,&data);
        fprintf(fp, "freq : %.3f \t power : %.3f\n",freq,data.pwr);
	}
	fclose(fp);
}

// 80m
rx_filter_80m_table::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(coef_real);i ++)
        coef_real[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(coef_imag);i ++)
        coef_imag[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(temp);i ++)
        temp[i] = 0.0;
    time.tm_sec = 0;
    time.tm_min = 0;
    time.tm_hour = 0;
    time.tm_mday = 0;
    time.tm_mon = 0;
    time.tm_year = 0;
    time.tm_wday = 0;
    time.tm_yday = 0;
    time.tm_isdst = 0;
}

void rx_filter_80m_table::data_m_t::_2double(double *real,double *imag)
{
    for (uint32_t i = 0;i < ARRAY_SIZE(coef);i ++) {
        real[i] = double(coef[i].real);
        imag[i] = double(coef[i].imag);
	}
}

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_filter_80m_table)

void rx_filter_80m_table::map2mem()
{
    if (nullptr == m_data_f)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1c::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1c::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < ul_filter_tap;i ++) {
            m_data_m[idx_m].coef[i].real = m_data_f[idx_f].coef_real[i];
            m_data_m[idx_m].coef[i].imag = m_data_f[idx_f].coef_imag[i];
		}
	}
}

void rx_filter_80m_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1c::rx_freq_step_called);
    *data = m_data_m[idx_l];
}

void rx_filter_80m_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,rx_freq_step_infile);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void rx_filter_80m_table::save_as(const char *path)
{

}

//_160M
rx_filter_160m_table::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(coef_real);i ++)
        coef_real[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(coef_imag);i ++)
        coef_imag[i] = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(temp);i ++)
        temp[i] = 0.0;
    time.tm_sec = 0;
    time.tm_min = 0;
    time.tm_hour = 0;
    time.tm_mday = 0;
    time.tm_mon = 0;
    time.tm_year = 0;
    time.tm_wday = 0;
    time.tm_yday = 0;
    time.tm_isdst = 0;
}

void rx_filter_160m_table::data_m_t::_2double(double *real,double *imag)
{
    for (uint32_t i = 0;i < ARRAY_SIZE(coef);i ++) {
        real[i] = double(coef[i].real);
        imag[i] = double(coef[i].imag);
	}
}

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_filter_160m_table)

void rx_filter_160m_table::map2mem()
{
    if (nullptr == m_data_f)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < r1c::rx_freqs_called;idx_m ++) {
        freq = rx_freq_star + idx_m * r1c::rx_freq_step_called;
        idx_f = SERIE_INDEX(freq,rx_freq_star,rx_freq_step_infile);
        for (uint32_t i = 0;i < ul_filter_tap;i ++) {
            m_data_m[idx_m].coef[i].real = m_data_f[idx_f].coef_real[i];
            m_data_m[idx_m].coef[i].imag = m_data_f[idx_f].coef_imag[i];
		}
	}
}

void rx_filter_160m_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,r1c::rx_freq_step_called);
    *data = m_data_m[idx_l];
}

void rx_filter_160m_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,r1c::rx_freq_step_called,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,rx_freq_star,rx_freq_step_infile);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void rx_filter_160m_table::save_as(const char *path)
{

}
