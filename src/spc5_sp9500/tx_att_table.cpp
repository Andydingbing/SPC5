#include "tx_att_table.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "algorithm.h"
#include "algo_math.h"

using namespace sp_rd;

// r1a/b
IMPLEMENT_TX_CAL_ITEM_TABLE(tx_att_table_r1ab)

void tx_att_table_r1ab::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = RF_TX_FREQ_STAR + idx_m * RF_TX_FREQ_STEP_CALLED;
        idx_f = SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        m_data_m[idx_m].att[0] = m_data_f[idx_f].att_10;
        m_data_m[idx_m].att[1] = m_data_f[idx_f].att_20;
        m_data_m[idx_m].att[2] = m_data_f[idx_f].att_30;
        m_data_m[idx_m].att[3] = m_data_f[idx_f].att_40;
        m_data_m[idx_m].att[4] = m_data_f[idx_f].att_50;
        m_data_m[idx_m].att[5] = m_data_f[idx_f].att_60;
        m_data_m[idx_m].att[6] = m_data_f[idx_f].att_70;
        m_data_m[idx_m].att[7] = m_data_f[idx_f].att_80;
        m_data_m[idx_m].att[8] = m_data_f[idx_f].att_90;
	}
}

void tx_att_table_r1ab::get(uint64_t freq,float *att)
{
    uint64_t freq_l = 0;
    uint64_t freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t idx_r = SERIE_INDEX(freq_r,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    data_m_t data_l = m_data_m[idx_l];
    data_m_t data_r = m_data_m[idx_r];

    for (uint32_t i = 0;i < sizeof(data_m_t) / sizeof(float);i ++)
        linear(freq_l,data_l.att[i],freq_r,data_r.att[i],freq,att[i]);
}

void tx_att_table_r1ab::get(uint64_t freq,int32_t att,double &offset)
{
    offset = 0;
    float att_offset[9] = {0.0};
    for (uint32_t i = 0;i < ARRAY_SIZE(att_offset);i ++)
        att_offset[i] = 0.0;

    get(freq,att_offset);

    for (uint32_t i = 0;i < uint32_t(abs(att) / R1A_TX_ATT_STEP_MIN);i ++)
        offset += double(att_offset[i]);
    offset = round(offset,3);
}

void tx_att_table_r1ab::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (fp == nullptr)
		return;
    float att[9] = {0.0};
    memset(att,0,sizeof(att));

	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_INFILE) {
        get(freq,att);
        fprintf(fp,"freq : %I64u \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
                    freq,att[0],att[1],att[2],att[3],att[4],att[5],att[6],att[7],att[8]);
	}
	fclose(fp);
}

// r1c/d output mode
tx_att_op_table_r1cd::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(offset);i ++)
        offset[i] = 0.0;
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

IMPLEMENT_TX_CAL_ITEM_TABLE(tx_att_op_table_r1cd)

void tx_att_op_table_r1cd::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = RF_TX_FREQ_STAR + idx_m * RF_TX_FREQ_STEP_CALLED;
        idx_f = SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++)
            m_data_m[idx_m].offset[i] = m_data_f[idx_f].offset[i];
        m_data_m[idx_m].temp_5 = float(m_data_f[idx_f].temp[1]);
	}
}

float tx_att_op_table_r1cd::get(uint64_t freq,double pwr)
{
    data_m_t data;
    get(freq,&data);

    int32_t i_pwr = pwr >= 0 ? int32_t(pwr) + 1 : int32_t(pwr);
    i_pwr = i_pwr > R1C_TX_ATT_OP_POWER_STAR ? R1C_TX_ATT_OP_POWER_STAR : i_pwr;
    i_pwr = i_pwr < R1C_TX_ATT_OP_POWER_STOP ? R1C_TX_ATT_OP_POWER_STOP : i_pwr;

    uint32_t pwr_idx = SERIE_INDEX(i_pwr,R1C_TX_ATT_OP_POWER_STAR,R1C_TX_ATT_STEP);
    if (R1C_TX_BASE_POWER_OP < i_pwr)
        return data.offset[pwr_idx];
    else if (R1C_TX_BASE_POWER_OP == double(i_pwr))
		return 0.0;
	else
        return data.offset[pwr_idx -1];
}

void tx_att_op_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    *data = m_data_m[idx_l];
}

void tx_att_op_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++)
        data->offset[i] = data_m.offset[i];
}

void tx_att_op_table_r1cd::save_as(const char *path)
{

}

// r1c/d io mode
tx_att_io_table_r1cd::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(offset);i ++)
        offset[i] = 0.0;
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

IMPLEMENT_TX_CAL_ITEM_TABLE(tx_att_io_table_r1cd)

void tx_att_io_table_r1cd::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = RF_TX_FREQ_STAR + idx_m * RF_TX_FREQ_STEP_CALLED;
        idx_f = SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++)
            m_data_m[idx_m].offset[i] = m_data_f[idx_f].offset[i];
        m_data_m[idx_m].temp_5 = float(m_data_f[idx_f].temp[1]);
	}
}

float tx_att_io_table_r1cd::get(uint64_t freq,double pwr)
{
    data_m_t data;
    get(freq,&data);

    int32_t i_pwr = pwr >= 0 ? int32_t(pwr) + 1 : int32_t(pwr);
    i_pwr = i_pwr > R1C_TX_ATT_IO_POWER_STAR ? R1C_TX_ATT_IO_POWER_STAR : i_pwr;
    i_pwr = i_pwr < R1C_TX_ATT_IO_POWER_STOP ? R1C_TX_ATT_IO_POWER_STOP : i_pwr;

    uint32_t pwr_idx = SERIE_INDEX(i_pwr,R1C_TX_ATT_IO_POWER_STAR,R1C_TX_ATT_STEP);
    if (R1C_TX_BASE_POWER_IO < i_pwr)
        return data.offset[pwr_idx];
    else if (R1C_TX_BASE_POWER_IO == double(i_pwr))
		return 0.0;
	else
        return data.offset[pwr_idx -1];
}

void tx_att_io_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    *data = m_data_m[idx_l];
}

void tx_att_io_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    *data = m_data_f[SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++)
        data->offset[i] = data_m.offset[i];
}

void tx_att_io_table_r1cd::save_as(const char *path)
{

}
