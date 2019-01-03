#include "rx_att_table.h"

using namespace sp_rd;

// r1c/d output mode
rx_att_op_table_r1cd::data_f_t::data_f_t()
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

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_att_op_table_r1cd)

void rx_att_op_table_r1cd::map2mem()
{
    if (NULL == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < RF_RX_FREQ_PTS_CALLED_R1C;idx_m ++) {
        freq = (uint64_t)RF_RX_FREQ_STAR + idx_m * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        idx_f = (uint32_t)SERIE_INDEX(freq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++) {
            m_data_m[idx_m].offset[i] = (int32_t)(m_data_f[idx_f].offset[i]);
            m_data_m[idx_m].temp = (float)(m_data_f[idx_f].temp[0]);
		}
	}
}

int32_t rx_att_op_table_r1cd::get(uint64_t freq,double ref)
{
    data_m_t data;
    get(freq,&data);
    int32_t i_ref = (ref >= 0.0 ? (int32_t)ref + 1 : (int32_t)ref);
    i_ref = i_ref < R1C_RX_REF_OP_STOP ? R1C_RX_REF_OP_STOP : i_ref;
    int32_t idx_ref = SERIE_INDEX(i_ref,R1C_RX_REF_OP_STAR,R1C_RX_REF_STEP);
    if (10 < i_ref)
        return data.offset[idx_ref];
    else if (10 == i_ref || -20 == i_ref)
		return 0;
    else if (-20 < i_ref)
        return data.offset[idx_ref - 1];
	else
        return data.offset[idx_ref - 2];
}

void rx_att_op_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_RX_FREQ_STEP_CALLED_R1C,freq_l,freq_r);

    uint32_t idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
    *data = m_data_m[idx_l];
}

void rx_att_op_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_RX_FREQ_STEP_CALLED_R1C,freq_l,freq_r);

    *data = m_data_f[(uint32_t)SERIE_INDEX(freq_l,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++)
        data->offset[i] = data_m.offset[i];
}

void rx_att_op_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (fp == NULL)
		return;

// 	float fAtt[9];
// 	memset(fAtt,0,ARRAY_SIZE(fAtt));
// 
// 	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
// 	for (uint64_t freq = CAL_RF_TX_FREQ_STAR;freq <= CAL_RF_TX_FREQ_STOP;freq += CAL_RF_TX_FREQ_STEP_INFILE) {
// 		get(freq,fAtt);
// 		fprintf(fp,"freq : %llu \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
// 					freq,fAtt[0],fAtt[1],fAtt[2],fAtt[3],fAtt[4],fAtt[5],fAtt[6],fAtt[7],fAtt[8]);
// 	}
	fclose(fp);
}

// r1c/d io mode
rx_att_io_table_r1cd::data_f_t::data_f_t()
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

IMPLEMENT_RX_CAL_ITEM_TABLE(rx_att_io_table_r1cd)

void rx_att_io_table_r1cd::map2mem()
{
    if (NULL == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < RF_RX_FREQ_PTS_CALLED_R1C;idx_m ++) {
        freq = (uint64_t)RF_RX_FREQ_STAR + idx_m * (uint64_t)RF_RX_FREQ_STEP_CALLED_R1C;
        idx_f = (uint32_t)SERIE_INDEX(freq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < ARRAY_SIZE(m_data_m[0].offset);i ++) {
            m_data_m[idx_m].offset[i] = (int32_t)(m_data_f[idx_f].offset[i]);
            m_data_m[idx_m].temp = (float)(m_data_f[idx_f].temp[0]);
		}
	}
}

int32_t rx_att_io_table_r1cd::get(uint64_t freq,double ref)
{
    data_m_t data;
    get(freq,&data);
    int32_t i_ref = ref >= 0 ? (int32_t)ref + 1 : (int32_t)ref;
    i_ref = i_ref < R1C_RX_REF_IO_STOP ? R1C_RX_REF_IO_STOP : i_ref;
    int32_t idx_ref = SERIE_INDEX(i_ref,R1C_RX_REF_IO_STAR,R1C_RX_REF_STEP);
    if (10 < i_ref)
        return data.offset[idx_ref];
    else if (10 == i_ref || -20 == i_ref)
		return 0;
    else if (-20 < i_ref)
        return data.offset[idx_ref - 1];
	else
        return data.offset[idx_ref - 2];
}

void rx_att_io_table_r1cd::get(uint64_t freq,data_m_t *data)
{
    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_RX_FREQ_STEP_CALLED_R1C,freq_l,freq_r);

    uint32_t idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1C);
    *data = m_data_m[idx_l];
}

void rx_att_io_table_r1cd::get(uint64_t freq,data_f_t *data)
{
    data_m_t data_m;
    get(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_RX_FREQ_STEP_CALLED_R1C,freq_l,freq_r);

    *data = m_data_f[(uint32_t)SERIE_INDEX(freq_l,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE)];
    data->freq = freq;
    for (uint32_t i = 0;i < ARRAY_SIZE((*data).offset);i ++)
        data->offset[i] = data_m.offset[i];
}

void rx_att_io_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (fp == NULL)
		return;

	// 	float fAtt[9];
	// 	memset(fAtt,0,ARRAY_SIZE(fAtt));
	// 
	// 	fprintf(fp,"freq \t att1_10 \t att1_20 \t att1_30 \t att2_10 \t att2_20 \t att2_30 \t att3_10 \t att3_20 \t att3_30\n");
    // 	for (uint64_t freq = CAL_RF_TX_FREQ_STAR;freq <= CAL_RF_TX_FREQ_STOP;freq += CAL_RF_TX_FREQ_STEP_INFILE) {
    // 		get(freq,fAtt);
	// 		fprintf(fp,"freq : %llu \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",
    // 					freq,fAtt[0],fAtt[1],fAtt[2],fAtt[3],fAtt[4],fAtt[5],fAtt[6],fAtt[7],fAtt[8]);
	// 	}
	fclose(fp);
}
