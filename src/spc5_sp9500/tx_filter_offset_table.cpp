#include "tx_filter_offset_table.h"
#include "algorithm.h"

using namespace sp_rd;

tx_filter_offset_table::data_f_t::data_f_t()
{
    freq = 0;
    offset = 0.0;
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

IMPLEMENT_TX_CAL_ITEM_TABLE(tx_filter_offset_table)

void tx_filter_offset_table::map2mem()
{
    if (m_data_f == nullptr)
		return;

    uint32_t idx_f = 0;
    uint32_t idx_m = 0;
    uint64_t freq = 0;

    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = RF_TX_FREQ_STAR + idx_m * RF_TX_FREQ_STEP_CALLED;
        idx_f = SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        m_data_m[idx_m].offset = float(m_data_f[idx_f].offset);
        m_data_m[idx_m].temp_5 = float(m_data_f[idx_f].temp[1]);	//temp4,5(used),6,7
	}
}

void tx_filter_offset_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    *data = m_data_m[idx_l];
}

void tx_filter_offset_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t idx_l = SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void tx_filter_offset_table::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
    if (nullptr == fp)
		return;

    data_f_t data;

    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_INFILE) {
        get(freq,&data);
        fprintf(fp,"freq : %10I64u \t offset : %6.3f \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
                freq,
                data.offset,
                data.temp[0],data.temp[1],data.temp[2],data.temp[3],
                data.time.tm_year + 1900,data.time.tm_mon,data.time.tm_mday,data.time.tm_hour,data.time.tm_min,data.time.tm_sec);
	}
	fclose(fp);
}
