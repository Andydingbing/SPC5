#include "rx_filter_offset_table.h"

using namespace sp_rd;

rx_filter_offset_table::data_f_t::data_f_t()
{
    freq = 0;
    for (uint32_t i = 0;i < ARRAY_SIZE(offset);i ++)
        offset[i] = 0;
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

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_filter_offset_table)

void rx_filter_offset_table::map2mem()
{
    if (NULL == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < RF_RX_FREQ_PTS_CALLED_R1C;idx_m ++) {
        freq = (uint64_t)RF_RX_FREQ_STAR + idx_m * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        idx_f = (uint32_t)SERIE_INDEX(freq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[idx_m].offset);i ++)
            m_data_m[idx_m].offset[i] = (int32_t)(m_data_f[idx_f].offset[i]);
        m_data_m[idx_m].temp_5 = (float)(m_data_f[idx_f].temp[1]);	//temp4,5(used),6,7
	}
}

void rx_filter_offset_table::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_RX_FREQ_STEP_CALLED_R1C,freq_l,freq_r);

    uint32_t idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
    *data = m_data_m[idx_l];
}

void rx_filter_offset_table::get(uint64_t freq,data_f_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_RX_FREQ_STEP_CALLED_R1C,freq_l,freq_r);

    uint32_t idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
    *data = m_data_f[idx_l];
    data->freq = freq;
}

void rx_filter_offset_table::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (NULL == fp)
		return;

    data_f_t data;

    for (uint64_t freq = RF_RX_FREQ_STAR;freq <= RF_RX_FREQ_STOP;freq += RF_RX_FREQ_STEP_INFILE) {
        get(freq,&data);
        fprintf(fp,"freq : %10I64u \t offset : %8I64i %8I64i %8I64i \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
                freq,
                data.offset[0],
                data.offset[1],
                data.offset[2],
                data.temp[0],data.temp[1],data.temp[2],data.temp[3],
                data.time.tm_year + 1900,data.time.tm_mon,data.time.tm_mday,data.time.tm_hour,data.time.tm_min,data.time.tm_sec);
	}
	fclose(fp);
}
