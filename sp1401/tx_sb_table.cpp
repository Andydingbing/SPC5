#include "tx_sb_table.h"

using namespace sp_rd;

// r1a/b
tx_sb_table_r1ab::data_f_t::data_f_t()
{
    freq = 0;
    th = 0.0;
    am_i = 0;
    am_q = 0;
}

IMPLEMENT_TX_CAL_ITEM_TABLE(tx_sb_table_r1ab)

void tx_sb_table_r1ab::map2mem()
{
    if (NULL == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = (uint64_t)RF_TX_FREQ_STAR + idx_m * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        idx_f = (uint32_t)SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        m_data_m[idx_m].th   = (float)(m_data_f[idx_f].th);
        m_data_m[idx_m].am_i = m_data_f[idx_f].am_i;
        m_data_m[idx_m].am_q = m_data_f[idx_f].am_q;
	}
}

void tx_sb_table_r1ab::get(uint64_t freq,double &th,uint16_t &am_i,uint16_t &am_q)
{
    uint64_t freq_l = 0,freq_r = 0;
    float f_th = 0.0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t idx_r = (uint32_t)SERIE_INDEX(freq_r,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    data_m_t data_l = m_data_m[idx_l];
    data_m_t data_r = m_data_m[idx_r];

    linear(freq_l,data_l.th,freq_r,data_r.th,freq,f_th);
    th = (double)f_th;
    linear(freq_l,data_l.am_i,freq_r,data_r.am_i,freq,am_i);
    linear(freq_l,data_l.am_q,freq_r,data_r.am_q,freq,am_q);
}

void tx_sb_table_r1ab::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (fp == NULL)
		return;
    double th = 0.0;
    uint16_t am_i = 0,am_q = 0;
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_INFILE) {
        get(freq,th,am_i,am_q);
        fprintf(fp,"freq : %I64u \t th : %6.3f \t am_i : %u \t am_q : %u\n",freq,th,am_i,am_q);
	}
	fclose(fp);
}

// r1c/d
tx_sb_table_r1cd::data_f_t::data_f_t()
{
    use_sa = false;
    pwr = 0.0;
    freq = 0;
    th = 0.0;
    am_i = 0;
    am_q = 0;
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

void tx_sb_table_r1cd::add(data_f_t *data)
{
    m_data_f[0] = *data;
}

void tx_sb_table_r1cd::map2mem()
{
    if (NULL == m_data_f)
		return;

    m_data_m[0].th   = (float)(m_data_f[0].th);
    m_data_m[0].am_i = m_data_f[0].am_i;
    m_data_m[0].am_q = m_data_f[0].am_q;
    m_data_m[0].temp_5 = (float)(m_data_f[0].temp[1]);	//temp4,5(used),6,7
}

void tx_sb_table_r1cd::get(uint64_t freq,double &th,uint16_t &am_i,uint16_t &am_q,float &temp_5)
{
    th = (double)(m_data_m[0].th);
    am_i = m_data_m[0].am_i;
    am_q = m_data_m[0].am_q;
    temp_5 = m_data_m[0].temp_5;
}

void tx_sb_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    data->th = m_data_m[0].th;
    data->am_i = m_data_m[0].am_i;
    data->am_q = m_data_m[0].am_q;
    data->temp_5 = m_data_m[0].temp_5;
}

void tx_sb_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    *data = m_data_f[0];
    data->freq = freq;
}

void tx_sb_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (fp == NULL)
		return;
    data_f_t data;
    uint64_t freq = RF_TX_FREQ_STAR;
    get(freq,&data);
	fprintf(fp,"th : %-6.3f \t am_i : %-4u \t am_q : %-4u \t method : %-s \t power : %-.3f \t temp4 : %-.3f \t temp5 : %-.3f \t temp6 : %-.3f \t temp7 : %-.3f \t time : %4d/%02d/%02d %02d:%02d:%02d\n",
            data.th,
            data.am_i,
            data.am_q,
            data.use_sa ? "spectrum" : "loopback",
            data.pwr,
            data.temp[0],data.temp[1],data.temp[2],data.temp[3],
            data.time.tm_year + 1900,data.time.tm_mon,data.time.tm_mday,data.time.tm_hour,data.time.tm_min,data.time.tm_sec);
	fclose(fp);
}
