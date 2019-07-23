#include "tx_filter_table.h"
#include "algorithm.h"

using namespace sp_rd;

// rf frequency response
tx_rf_fr_table::data_f_t::data_f_t()
{
    freq = 0;
    pwr = 0.0;
}

void tx_rf_fr_0_table_r1cd::add(data_f_t *data)
{
    uint32_t idx = SERIE_INDEX(data->freq,RF_TX_FILTER_FREQ_STAR_0,RF_TX_FILTER_FREQ_STEP);
    m_data_f[idx] = *data;
}

void tx_rf_fr_0_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data->pwr = m_data_f[SERIE_INDEX(freq,RF_TX_FILTER_FREQ_STAR_0,RF_TX_FILTER_FREQ_STEP)].pwr;
    data->freq = freq;
}

void tx_rf_fr_0_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (fp == nullptr)
		return;
	
    data_f_t data;
    for (uint64_t freq = RF_TX_FILTER_FREQ_STAR_0;freq <= RF_TX_FILTER_FREQ_STOP_0;freq += RF_TX_FILTER_FREQ_STEP) {
        get(freq,&data);
        fprintf(fp,"freq : %I64u \t power : %.3f\n",freq,data.pwr);
	}
	fclose(fp);
}

void tx_rf_fr_1_table_r1cd::add(data_f_t *data)
{
    uint32_t idx = SERIE_INDEX(data->freq,RF_TX_FILTER_FREQ_STAR_1,RF_TX_FILTER_FREQ_STEP);
    m_data_f[idx] = *data;
}

void tx_rf_fr_1_table_r1cd::get(uint64_t freq, data_f_t *data)
{
    data->pwr = m_data_f[SERIE_INDEX(freq,RF_TX_FILTER_FREQ_STAR_1,RF_TX_FILTER_FREQ_STEP)].pwr;
    data->freq = freq;
}

void tx_rf_fr_1_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (fp == nullptr)
		return;

    data_f_t data;
    for (uint64_t freq = RF_TX_FILTER_FREQ_STAR_1;freq <= RF_TX_FILTER_FREQ_STOP_1;freq += RF_TX_FILTER_FREQ_STEP) {
        get(freq,&data);
        fprintf(fp,"freq : %I64u \t power : %.3f\n",freq,data.pwr);
	}
	fclose(fp);
}

// if frequency response
tx_if_fr_table::data_f_t::data_f_t()
{
    freq = 0.0;
    pwr = 0.0;
}

void tx_if_fr_table::add(data_f_t *data)
{
    uint32_t idx = SERIE_INDEX(data->freq,IF_TX_FILTER_160M_FREQ_STAR,IF_TX_FILTER_FREQ_STEP);
    m_data_f[idx] = *data;
}

void tx_if_fr_table::get(double freq,data_f_t *data)
{
    data->pwr = m_data_f[SERIE_INDEX(freq,IF_TX_FILTER_160M_FREQ_STAR,IF_TX_FILTER_FREQ_STEP)].pwr;
    data->freq = freq;
}

void tx_if_fr_table::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (nullptr == fp)
		return;

    data_f_t data;
    for (double freq = IF_TX_FILTER_160M_FREQ_STAR;freq <= IF_TX_FILTER_160M_FREQ_STOP;freq += IF_TX_FILTER_FREQ_STEP) {
        get(freq,&data);
        fprintf(fp, "freq : %.3f \t power : %.3f\n",freq,data.pwr);
	}
	fclose(fp);
}

// filter
tx_filter_table::data_f_t::data_f_t()
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

void tx_filter_table::data_m_t::_2double(double *real,double *imag)
{
    for (uint32_t i = 0;i < ARRAY_SIZE(coef);i ++) {
        real[i] = double(coef[i].real);
        imag[i] = double(coef[i].imag);
	}
}

// 80m
IMPLEMENT_TX_CAL_ITEM_TABLE(tx_filter_80m_table)

void tx_filter_80m_table::map2mem()
{
    if (nullptr == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < (uint32_t)(RF_TX_FREQ_PTS_CALLED);idx_m ++) {
        freq = RF_TX_FREQ_STAR + idx_m * RF_TX_FREQ_STEP_CALLED;
        idx_f = SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++) {
            m_data_m[idx_m].coef[i].real = m_data_f[idx_f].coef_real[i];
            m_data_m[idx_m].coef[i].imag = m_data_f[idx_f].coef_imag[i];
		}
	}
}

void tx_filter_80m_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    *data = m_data_m[idx_l];
}

void tx_filter_80m_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void tx_filter_80m_table::save_as(const char *path)
{

}

// 160m
IMPLEMENT_TX_CAL_ITEM_TABLE(tx_filter_160m_table)

void tx_filter_160m_table::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = RF_TX_FREQ_STAR + idx_m * RF_TX_FREQ_STEP_CALLED;
        idx_f = SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
		for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++) {
            m_data_m[idx_m].coef[i].real = m_data_f[idx_f].coef_real[i];
            m_data_m[idx_m].coef[i].imag = m_data_f[idx_f].coef_imag[i];
		}
	}
}

void tx_filter_160m_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    *data = m_data_m[idx_l];
}

void tx_filter_160m_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void tx_filter_160m_table::save_as(const char *path)
{

}
