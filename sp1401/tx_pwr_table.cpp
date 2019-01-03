#include "tx_pwr_table.h"

using namespace sp_rd;

// r1a/b
void tx_pwr_table_r1ab::add(data_f_t *data)
{
    uint32_t idx = (uint32_t)SERIE_INDEX(data->state[0].freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
    m_data_f[idx] = *data;
}

void tx_pwr_table_r1ab::map2mem()
{
    if (NULL == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = (uint64_t)RF_TX_FREQ_STAR + idx_m * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        idx_f = (uint32_t)SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        for (uint32_t i = 0;i < sizeof(data_m_t) / sizeof(tx_state_m_t);i ++) {
            m_data_m[idx_m].state[i].d_gain = (float)(m_data_f[idx_f].state[i].d_gain);
            m_data_m[idx_m].state[i].att1   = m_data_f[idx_f].state[i].att1;
            m_data_m[idx_m].state[i].att2   = m_data_f[idx_f].state[i].att2;
            m_data_m[idx_m].state[i].pa_att = m_data_f[idx_f].state[i].pa_att;
            m_data_m[idx_m].state[i].att3   = m_data_f[idx_f].state[i].att3;
		}
	}
}

uint32_t tx_pwr_table_r1ab::get_pwr_idx(double pwr)
{
    double pwr_l = 0.0,pwr_r = 0.0;
    discretept(pwr,R1A_TX_POWER_STEP,pwr_l,pwr_r);
    return (uint32_t)SERIE_INDEX(pwr_l,R1A_TX_POWER_STAR,R1A_TX_POWER_STEP);
}

void tx_pwr_table_r1ab::guess_base_op(int32_t &att1,int32_t &att2,int32_t &att3)
{
// 		{0, 0.0,   0,   0,   sp1401::TX_PA,  0  },	//10
// 		{0, 0.0,   -10, 0,   sp1401::TX_PA,  0  },	//0
// 		{0, 0.0,   -20, 0,   sp1401::TX_PA,  0  },	//-10
// 
// 		{0, -10.0, -20, 0,   sp1401::TX_PA,  0  },	//-20
// 		{0, -10.0, -10, 0,   sp1401::TX_ATT, 0  },	//-20
// 		/**********************************************/
// 
// 		{0, -10.0, -10, 0,   sp1401::TX_ATT, -10},	//-30
// 		{0, -10.0, -10, -10, sp1401::TX_ATT, -10},	//-40
// 		{0, -10.0, -10, -10, sp1401::TX_ATT, -20},	//-50
// 		{0, -10.0, -20, -10, sp1401::TX_ATT, -20},	//-60
// 		{0, -10.0, -20, -20, sp1401::TX_ATT, -20},	//-70
// 		{0, -10.0, -20, -20, sp1401::TX_ATT, -30},	//-80
// 		{0, -10.0, -30, -20, sp1401::TX_ATT, -30},	//-90
// 		{0, -10.0, -30, -30, sp1401::TX_ATT, -30},	//-100

    att1 = -10;
    att2 = 0;
    att3 = 0;
}

void tx_pwr_table_r1ab::guess_base_io(int32_t &att1,int32_t &att2,int32_t &att3)
{
// 		{0, 0.0,   0,   0,   (int32_t)(sp1401::TX_PA),  0  },	//10
// 		{0, 0.0,   -10, 0,   (int32_t)(sp1401::TX_PA),  0  },	//0
// 		{0, 0.0,   -20, 0,   (int32_t)(sp1401::TX_PA),  0  },	//-10
// 
// 		{0, -10.0, -20, 0,   (int32_t)(sp1401::TX_PA),  0  },	//-20
// 		{0, -10.0, -10, 0,   (int32_t)(sp1401::TX_ATT), 0  },	//-20
// 		/**********************************************/
// 
// 		{0, -10.0, -10, 0,   (int32_t)(sp1401::TX_ATT), -10},	//-30
// 		{0, -10.0, -10, -10, (int32_t)(sp1401::TX_ATT), -10},	//-40
// 		{0, -10.0, -10, -10, (int32_t)(sp1401::TX_ATT), -20},	//-50
// 		{0, -10.0, -20, -10, (int32_t)(sp1401::TX_ATT), -20},	//-60
// 		{0, -10.0, -20, -20, (int32_t)(sp1401::TX_ATT), -20},	//-70
// 		{0, -10.0, -20, -20, (int32_t)(sp1401::TX_ATT), -30},	//-80
// 		{0, -10.0, -30, -20, (int32_t)(sp1401::TX_ATT), -30},	//-90
// 		{0, -10.0, -30, -30, (int32_t)(sp1401::TX_ATT), -30},	//-100

    att1 = -10;
    att2 = 0;
    att3 = 0;
}

void tx_pwr_table_r1ab::split(double att,double *rf,double *d)
{	//"pRf > 0" means RF Att;"pBb > 0" means Digital Gain & pBb < 0 means Digital Att
    *rf = ((int32_t)(att / 10.0)) * 10.0;
    *rf = (*rf >= /**(double *)*/R1A_TX_ATT_DR ? /**(double *)*/R1A_TX_ATT_DR : *rf);
    *d = *rf - att;
    if (*d < (R1A_DGAIN_MIN + 1)) {
        *rf = *rf + (*rf >= R1A_TX_ATT_DR ? 0 : R1A_TX_ATT_STEP_MIN);
        *d = *rf - att;
	}
    if (*d > R1A_DGAIN_MAX) {
        *rf = *rf - (*rf > 0 ? R1A_TX_ATT_STEP_MIN : 0);
        *d = *rf - att;
	}
}

void tx_pwr_table_r1ab::split(int32_t att,int32_t &att1,int32_t &att2,int32_t &att3)
{
    att1 = att >= 30.0 ? 30 : ((int32_t)(att / 10.0)) * 10;
    att2 = (att - att1) >= 30 ? 30 : ((int32_t)((att - att1) / 10.0)) * 10;
    att3 = (att - att1 - att2) >= 30 ? 30 : ((int32_t)((att - att1 - att2) / 10.0)) * 10;
}

void tx_pwr_table_r1ab::get_op(uint64_t freq,double pwr,double &d_gain,int32_t &att1,int32_t &att2,int32_t &pa_att,int32_t &att3)
{
    freq = freq < RF_TX_FREQ_STAR  ? RF_TX_FREQ_STAR  : freq;
    freq = freq > RF_TX_FREQ_STOP  ? RF_TX_FREQ_STOP  : freq;
    pwr = pwr > R1A_TX_POWER_STAR ? R1A_TX_POWER_STAR : pwr;
    int32_t d_gain_min = R1A_DGAIN_MIN + 1;
//    int32_t d_gain_max = R1A_DGAIN_MAX;
    uint64_t freq_l = 0,freq_r = 0;
    double pwr_l = 0.0,pwr_r = 0.0;
    double d_gain_l = 0.0,d_gain_r = 0.0;	//compare which calibration point to use
    bool use_l = true;						//decide if using the calibration point which power > Power
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);
    discretept(pwr,R1A_TX_POWER_STEP,pwr_l,pwr_r);

    uint32_t freq_idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t freq_idx_r = (uint32_t)SERIE_INDEX(freq_r,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t pwr_idx_l = get_pwr_idx(pwr_l);
    uint32_t pwr_idx_r = get_pwr_idx(pwr_r);

    data_m_t data_m_m;
    data_m_t data_m_l = m_data_m[freq_idx_l];
    data_m_t data_m_r = m_data_m[freq_idx_r];
	
	for (int32_t i = 0;i < 5;i ++) {
        if ((data_m_l.state[i].att1 == data_m_r.state[i].att1) && (data_m_l.state[i].att2 == data_m_r.state[i].att2) &&
            (data_m_l.state[i].att3 == data_m_r.state[i].att3) && (data_m_l.state[i].pa_att== data_m_r.state[i].pa_att)) {
            linear(freq_l,data_m_l.state[i].d_gain,freq_r,data_m_r.state[i].d_gain,freq,data_m_m.state[i].d_gain);
		}
		else {
            data_m_m.state[i].d_gain = data_m_l.state[i].d_gain;
		}
        data_m_m.state[i].att1  = data_m_l.state[i].att1;
        data_m_m.state[i].att2  = data_m_l.state[i].att2;
        data_m_m.state[i].att3  = data_m_l.state[i].att3;
        data_m_m.state[i].pa_att = data_m_l.state[i].pa_att;
	}

    if (pwr >= R1A_TX_POWER_STOP + 10) {
        d_gain_l = data_m_m.state[pwr_idx_l].d_gain - (pwr_l - pwr);		 //may < R1A_DGAIN_MIN
        if (d_gain_l < d_gain_min) {
            d_gain_r = data_m_m.state[pwr_idx_r].d_gain + (pwr - pwr_r);	 //may > R1A_DGAIN_MAX
            if (d_gain_r > R1A_DGAIN_MAX)
                use_l = ((d_gain_min - d_gain_l) <= (d_gain_r - R1A_DGAIN_MAX)) ? true : false;
			else
                use_l = false;
		}
		else
            use_l = true;
        d_gain = use_l ? d_gain_l : d_gain_r;
        att1  = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].att1;
        att2  = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].att2;
        att3  = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].att3;
        pa_att = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].pa_att;
	}
	else {
        pwr_idx_l = get_pwr_idx(R1A_TX_POWER_STOP + 10);                                        //digital gain all based from -20
        int32_t att = 0;																		//all rf att
        int32_t extra_att_l = (int32_t)((int32_t)(R1A_TX_POWER_STOP + 10 - pwr) / 10) * 10;     //extra rf att from -20(right version),0,10,20,30...
        int32_t extra_att_r = extra_att_l + R1A_TX_ATT_STEP_MIN;
        d_gain_l = data_m_m.state[pwr_idx_l].d_gain - (R1A_TX_POWER_STOP + 10 - extra_att_l - pwr);			//may < R1A_DGAIN_MIN
        if (d_gain_l < d_gain_min) {
            d_gain_r = data_m_m.state[pwr_idx_l].d_gain + (pwr - (R1A_TX_POWER_STOP + 10 - extra_att_r));   //may > R1A_DGAIN_MAX
            if (d_gain_r > R1A_DGAIN_MAX)
                use_l = ((d_gain_min - d_gain_l) <= (d_gain_r - R1A_DGAIN_MAX)) ? true : false;
			else
                use_l = false;
		}
		else
            use_l = true;
        d_gain = use_l ? d_gain_l : d_gain_r;
        att = data_m_m.state[pwr_idx_l].att1 + data_m_m.state[pwr_idx_l].att2 + data_m_m.state[pwr_idx_l].att3 + (use_l ? extra_att_l : extra_att_r);
        d_gain = att > R1A_TX_ATT_DR ? d_gain - (att - R1A_TX_ATT_DR) : d_gain;
        att = att > R1A_TX_ATT_DR ? R1A_TX_ATT_DR : att;
        split(att,att1,att2,att3);					//redistribute att
        pa_att = data_m_m.state[pwr_idx_l].pa_att;
	}
}

void tx_pwr_table_r1ab::get_io(uint64_t freq,double pwr,double &d_gain,int32_t &att1,int32_t &att2,int32_t &pa_att,int32_t &att3)
{
    freq = freq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : freq;
    freq = freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : freq;
    pwr = pwr > (R1A_TX_POWER_STAR - 10) ? (R1A_TX_POWER_STAR - 10) : pwr;
    int32_t d_gain_min = R1A_DGAIN_MIN + 1;
//    int32_t d_gain_max = R1A_DGAIN_MAX;
    uint64_t freq_l = 0,freq_r = 0;
    double pwr_l = 0.0,pwr_r = 0.0;
    double d_gain_l = 0.0,d_gain_r = 0.0;	//compare which calibration point to use
    bool use_l = true;						//decide if using the calibration point which power > Power
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);
    discretept(pwr,R1A_TX_POWER_STEP,pwr_l,pwr_r);

    uint32_t freq_idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t freq_idx_r = (uint32_t)SERIE_INDEX(freq_r,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t pwr_idx_l = get_pwr_idx(pwr_l) - 1;		//io mode start from 0dBm
    uint32_t pwr_idx_r = get_pwr_idx(pwr_r) - 1;

    data_m_t data_m_m;
    data_m_t data_m_l = m_data_m[freq_idx_l];
    data_m_t data_m_r = m_data_m[freq_idx_r];

	for (int32_t i = 0;i < 5;i ++) {
        if ((data_m_l.state[i].att1 == data_m_r.state[i].att1) && (data_m_l.state[i].att2 == data_m_r.state[i].att2) &&
            (data_m_l.state[i].att3 == data_m_r.state[i].att3) && (data_m_l.state[i].pa_att== data_m_r.state[i].pa_att)) {
                linear(freq_l,data_m_l.state[i].d_gain,freq_r,data_m_r.state[i].d_gain,freq,data_m_m.state[i].d_gain);
		}
		else {
            data_m_m.state[i].d_gain = data_m_l.state[i].d_gain;
		}
        data_m_m.state[i].att1  = data_m_l.state[i].att1;
        data_m_m.state[i].att2  = data_m_l.state[i].att2;
        data_m_m.state[i].att3  = data_m_l.state[i].att3;
        data_m_m.state[i].pa_att = data_m_l.state[i].pa_att;
	}

    if (pwr >= (R1A_TX_POWER_STOP /*- 10*/)) {
        d_gain_l = data_m_m.state[pwr_idx_l].d_gain - (pwr_l - pwr);		 //may < R1A_DGAIN_MIN
        if (d_gain_l < d_gain_min) {
            d_gain_r = data_m_m.state[pwr_idx_r].d_gain + (pwr - pwr_r);	 //may > R1A_DGAIN_MAX
            if (d_gain_r > R1A_DGAIN_MAX)
                use_l = ((d_gain_min - d_gain_l) <= (d_gain_r - R1A_DGAIN_MAX)) ? true : false;
			else
                use_l = false;
		}
		else
            use_l = true;
        d_gain = use_l ? d_gain_l : d_gain_r;
        att1  = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].att1;
        att2  = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].att2;
        att3  = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].att3;
        pa_att = data_m_m.state[use_l ? pwr_idx_l : pwr_idx_r].pa_att;
	}
	else {
        pwr_idx_l = get_pwr_idx(R1A_TX_POWER_STOP /*- 10*/) - 1;                                                //digital gain all based from -20
        int32_t att = 0;                                                                                        //all rf att
        int32_t extra_att_l = (int32_t)((int32_t)(R1A_TX_POWER_STOP /*-10*/ - pwr) / 10) * 10;					//extra rf att from -20(right version),0,10,20,30...
        int32_t extra_att_r = extra_att_l + R1A_TX_ATT_STEP_MIN;
        d_gain_l = data_m_m.state[pwr_idx_l].d_gain - (R1A_TX_POWER_STOP /*-10*/ - extra_att_l - pwr);          //may < R1A_DGAIN_MIN
        if (d_gain_l < d_gain_min) {
            d_gain_r = data_m_m.state[pwr_idx_l].d_gain + (pwr - (R1A_TX_POWER_STOP /*-10*/ - extra_att_r));    //may > R1A_DGAIN_MAX
            if (d_gain_r > R1A_DGAIN_MAX)
                use_l = ((d_gain_min - d_gain_l) <= (d_gain_r - R1A_DGAIN_MAX)) ? true : false;
			else
                use_l = false;
		}
		else
            use_l = true;
        d_gain = use_l ? d_gain_l : d_gain_r;
        att = data_m_m.state[pwr_idx_l].att1 + data_m_m.state[pwr_idx_l].att2 + data_m_m.state[pwr_idx_l].att3 + (use_l ? extra_att_l : extra_att_r);
        d_gain = att > R1A_TX_ATT_DR ? d_gain - (att - R1A_TX_ATT_DR) : d_gain;
        att = att > R1A_TX_ATT_DR ? R1A_TX_ATT_DR : att;
        split(att,att1,att2,att3);
        pa_att = data_m_m.state[pwr_idx_l].pa_att;
	}
}

void tx_pwr_table_r1ab::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (NULL == fp)
		return;

    double pwr = 10.0,d_gain = 0.0;
    int32_t att1 = 0,att2 = 0,att3 = 0;
    int32_t pa_att = 0;

	fprintf(fp,"/**********************************Output**********************************/\n");
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_INFILE) {
        for (pwr = 10;pwr >= -100;pwr --) {
            get_op(freq,pwr,d_gain,att1,att2,pa_att,att3);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t d_gain : %6.3f \t att1 : %4d \t att2 : %4d \t pa?att : %4s \t att3 : %4d\n",
                freq,pwr,d_gain,att1,att2,pa_att == 0 ? "pa" : "att",att3);
		}
	}

	fprintf(fp,"/**********************************IO**********************************/\n");
    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_INFILE) {
        for (pwr = 0;pwr >= -100;pwr --) {
            get_io(freq,pwr,d_gain,att1,att2,pa_att,att3);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t d_gain : %6.3f \t att1 : %4d \t att2 : %4d \t pa?att : %4s \t att3 : %4d\n",
                freq,pwr,d_gain,att1,att2,pa_att == 0 ? "pa" : "att",att3);
		}
	}
	fclose(fp);
}

// r1c/d
tx_pwr_table_r1cd::data_f_t::data_f_t()
{
    freq = 0;
    d_gain = -5.0;
    att0 = 5.0;
    att1 = 20.0;
    att2 = 0.0;
    att3 = 1.0;
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

// r1c/d output mode
IMPLEMENT_TX_CAL_ITEM_TABLE(tx_pwr_op_table_r1cd)

void tx_pwr_op_table_r1cd::map2mem()
{
    if (NULL == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = (uint64_t)RF_TX_FREQ_STAR + idx_m * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        idx_f = (uint32_t)SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        m_data_m[idx_m].d_gain = (float)(m_data_f[idx_f].d_gain);
        m_data_m[idx_m].temp_5 = (float)(m_data_f[idx_f].temp[1]);	//temp4,5(used),6,7
        m_data_m[idx_m].att0 = (short)(m_data_f[idx_f].att0 * 2.0);
        m_data_m[idx_m].att1 = (short)(m_data_f[idx_f].att1 * 2.0);
        m_data_m[idx_m].att2 = (short)(m_data_f[idx_f].att2 * 2.0);
        m_data_m[idx_m].att3 = (short)(m_data_f[idx_f].att3 * 2.0);
	}
}

void tx_pwr_op_table_r1cd::get_config_table(int32_t pwr,data_m_t data_base,data_m_t *data)
{
	//		DGain	Att0	Att1	Att2	Att3	//	dBm

	// 		{-5,	 -5,	 -5,	  0,	 -1}	//	10
	// 		{-5,	 -5,	-10,	  0,	 -1}	//	5
	// 		{-5,	 -5,	-15,	  0,	 -1}	//	0
	////////{-5,	 -5,	-20,	  0,	 -1}//////	-5		//

	// 		{-5,	 -5,	-20,	 -5,	 -1}	//	-10
	// 		{-5,	 -5,	-20,	-10,	 -1}	//	-15
	// 		{-5,	 -5,	-20,	-15,	 -1}	//	-20		//

	// 		{-5,	 -5,	-20,	-15,	 -6}	//	-25
	// 		{-5,	 -5,	-20,	-15,	-11}	//	-30
	// 		{-5,	 -5,	-20,	-15,	-16}	//	-35		//

	// 		{-5,	 -5,	-20,	-20,	-16}	//	-40		//

	// 		{-5,	 -5,	-20,	-20,	-21}	//	-45	
	// 		{-5,	 -5,	-20,	-20,	-26}	//	-50		//

	// 		{-5,	 -5,	-20,	-25,	-26}	//	-55		//

	// 		{-5,	 -5,	-25,	-25,	-26}	//	-60		//

	// 		{-5,	-10,	-25,	-25,	-26}	//	-65		//

	// 		{-5,	-10,	-25,	-25,	-31}	//	-70		//

	// 		{-5,	-10,	-25,	-30,	-31}	//	-75		//

	// 		{-5,	-10,	-30,	-30,	-31}	//	-80		//
	// 		{-5,	-20,	-30,	-30,	-31}	//	-90
	// 		{-15,	-20,	-30,	-30,	-31}	//	-100
	// 		{-25,	-20,	-30,	-30,	-31}	//	-110
	// 		{-35,	-20,	-30,	-30,	-31}	//	-120

    double att0 = data_base.att0 / 2.0;
    double att1 = data_base.att1 / 2.0;
    double att2 = data_base.att2 / 2.0;
    double att3 = data_base.att3 / 2.0;
    double d_gain = (double)(data_base.d_gain);

    if (pwr >= R1C_TX_BASE_POWER_OP) {
        att1 += R1C_TX_BASE_POWER_OP - pwr;
        if (att1 < 0) {
            d_gain -= att1;
            att1 = 0;
		}
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 15.0)
        att2 = -5.0 - pwr;
    else if (pwr >= R1C_TX_BASE_POWER_OP - 30.0) {
        att2 += 15.0;
        att3 = -19.0 - pwr;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 35.0) {
        att2 = -20 - pwr;
        att3 += 15.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 45.0) {
        att2 += 20.0;
        att3 = -24.0 - pwr;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 50.0) {
        att2 = -30.0 - pwr;
        att3 += 25.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 55.0) {
        att1 += -55.0 - pwr;
        att2 += 25.0;
        att3 += 25.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 60.0) {
        att0 += -60.0 - pwr;
        att1 += 5.0;
        att2 += 25.0;
        att3 += 25.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 65.0) {
        att0 += 5.0;
        att1 += 5.0;
        att2 += 25.0;
        att3 = -39.0 - pwr;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 70.0) {
        att0 += 5.0;
        att1 += 5.0;
        att2 = -45.0 - pwr;
        att3 += 30.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 75.0) {
        att0 += 5.0;
        att1 += -70.0 - pwr;
        att2 += 30.0;
        att3 += 30.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_OP - 85.0) {
        att0 += -75.0 - pwr;
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
	}
	else {
        att0 += 15.0;
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
        d_gain -= R1C_TX_BASE_POWER_OP - 85.0 - pwr;
	}
    if (att0 > 31.5) {
        d_gain -= (att0 - 31.5);
        att0 = 31.5;
    }

    data->att0 = (short)(att0 * 2);
    data->att1 = (short)(att1 * 2);
    data->att2 = (short)(att2 * 2);
    data->att3 = (short)(att3 * 2);
    data->d_gain = (float)d_gain;
    data->temp_5 = data_base.temp_5;
}

void tx_pwr_op_table_r1cd::guess_base(double &att0,double &att1,double &att2,double &att3,double &d_gain)
{
    att0 = 5.0;
    att1 = 20.0;
    att2 = 0.0;
    att3 = 1.0;
    d_gain = -5.0;
}

void tx_pwr_op_table_r1cd::get_base(uint64_t freq,data_m_t *data)
{
    freq = freq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : freq;
    freq = freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : freq;

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t freq_idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t freq_idx_r = (uint32_t)SERIE_INDEX(freq_r,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);

    data_m_t data_l = m_data_m[freq_idx_l];
    data_m_t data_r = m_data_m[freq_idx_r];

    if (data_l.att0 == data_r.att0 &&
        data_l.att1 == data_r.att1 &&
        data_l.att2 == data_r.att2 &&
        data_l.att3 == data_r.att3 &&
        data_l.temp_5 == data_r.temp_5) {
            linear(freq_l,data_l.d_gain,freq_r,data_r.d_gain,freq,data->d_gain);
	}
	else
        data->d_gain = data_l.d_gain;

    data->att0  = data_l.att0;
    data->att1  = data_l.att1;
    data->att2  = data_l.att2;
    data->att3  = data_l.att3;
    data->temp_5 = data_l.temp_5;
}

void tx_pwr_op_table_r1cd::get_base(uint64_t freq,data_f_t *data)
{
    freq = freq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : freq;
    freq = freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : freq;

    data_m_t data_m;
    get_base(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    *data = m_data_f[(uint32_t)SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
    data->freq = freq;
    data->att0 = (double)(data_m.att0 / 2.0);
    data->att1 = (double)(data_m.att1 / 2.0);
    data->att2 = (double)(data_m.att2 / 2.0);
    data->att3 = (double)(data_m.att3 / 2.0);
    data->d_gain = (double)(data_m.d_gain);
}

void tx_pwr_op_table_r1cd::get(uint64_t freq,double pwr,data_m_t *data)
{
    data_m_t data_base;
    pwr = pwr > (double)R1C_TX_ATT_OP_POWER_STAR ? (double)R1C_TX_ATT_OP_POWER_STAR : pwr;
    double pwr_l = 0.0,pwr_r = 0.0;

    discretept(pwr,R1C_TX_ATT_STEP,pwr_l,pwr_r);
    get_base(freq,&data_base);
    tx_pwr_op_table_r1cd::get_config_table((int32_t)pwr_l,data_base,data);
    data->d_gain -= (float)(pwr_l - pwr);
}

void tx_pwr_op_table_r1cd::get(uint64_t freq,double pwr,data_f_t *data)
{
    data_m_t data_m;
    get(freq,pwr,&data_m);
    get_base(freq,data);
    data->att0 = (double)(data_m.att0 / 2.0);
    data->att1 = (double)(data_m.att1 / 2.0);
    data->att2 = (double)(data_m.att2 / 2.0);
    data->att3 = (double)(data_m.att3 / 2.0);
    data->d_gain = (double)(data_m.d_gain);
}

void tx_pwr_op_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (NULL == fp)
		return;

    double pwr = 10.0;
    tx_pwr_op_table_r1cd::data_f_t data;

    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_INFILE) {
        for (pwr = R1C_TX_ATT_OP_POWER_STAR;pwr >= -120;pwr += R1C_TX_ATT_STEP) {
            get(freq,pwr,&data);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t digital gain : %6.3f \t att0 : %3.1f \t att1 : %3.1f \t att2 : %3.1f \t att3 : %3.1f \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
                    freq,
                    pwr,
                    data.d_gain,
                    data.att0,
                    data.att1,
                    data.att2,
                    data.att3,
                    data.temp[0],data.temp[1],data.temp[2],data.temp[3],
                    data.time.tm_year + 1900,data.time.tm_mon,data.time.tm_mday,data.time.tm_hour,data.time.tm_min,data.time.tm_sec);
		}
	}
	fclose(fp);
}

// r1c/d io mode
IMPLEMENT_TX_CAL_ITEM_TABLE(tx_pwr_io_table_r1cd)

void tx_pwr_io_table_r1cd::map2mem()
{
    if (NULL == m_data_f)
		return;

    uint32_t idx_f = 0,idx_m = 0;
    uint64_t freq = 0;
    for (idx_m = 0;idx_m < RF_TX_FREQ_PTS_CALLED;idx_m ++) {
        freq = (uint64_t)RF_TX_FREQ_STAR + idx_m * (uint64_t)RF_TX_FREQ_STEP_CALLED;
        idx_f = (uint32_t)SERIE_INDEX(freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE);
        m_data_m[idx_m].d_gain = (float)(m_data_f[idx_f].d_gain);
        m_data_m[idx_m].temp_5 = (float)(m_data_f[idx_f].temp[1]);	//temp4,5(used),6,7
        m_data_m[idx_m].att0 = (short)(m_data_f[idx_f].att0 * 2.0);
        m_data_m[idx_m].att1 = (short)(m_data_f[idx_f].att1 * 2.0);
        m_data_m[idx_m].att2 = (short)(m_data_f[idx_f].att2 * 2.0);
        m_data_m[idx_m].att3 = (short)(m_data_f[idx_f].att3 * 2.0);
	}
}

void tx_pwr_io_table_r1cd::get_config_table(int32_t pwr,data_m_t data_base,data_m_t *data)
{
	//		DGain	Att0	Att1	Att2	Att3	//	dBm

	// 		{-5,	 -5,	 -3,	  0,	 -1}	//	5
	////////{-5,	 -5,	-20,	  0,	 -1}//////	-12		//
	// 		{-5,	 -5,	-20,	-15,	 -1}	//	-27		//
	// 		{-5,	 -5,	-20,	-15,	-16}	//	-42		//
	// 		{-5,	 -5,	-20,	-20,	-16}	//	-47		//
	// 		{-5,	 -5,	-20,	-20,	-26}	//	-57		//
	// 		{-5,	 -5,	-20,	-25,	-26}	//	-62		//
	//		{-5,	 -5,	-25,	-25,	-26}	//	-67		//
	// 		{-5,	-10,	-25,	-25,	-26}	//	-72		//
	// 		{-5,	-10,	-25,	-25,	-31}	//	-77		//
	// 		{-5,	-10,	-25,	-30,	-31}	//	-82		//
	//		{-5,	-10,	-30,	-30,	-31}	//	-87		//
	//		{-5,	-20,	-30,	-30,	-31}	//	-97		//
	//		{-28,	-20,	-30,	-30,	-31}	//	-120	//

    double att0 = data_base.att0 / 2.0;
    double att1 = data_base.att1 / 2.0;
    double att2 = data_base.att2 / 2.0;
    double att3 = data_base.att3 / 2.0;
    double d_gain = (double)(data_base.d_gain);

    if (pwr >= R1C_TX_BASE_POWER_IO) {
        att1 += R1C_TX_BASE_POWER_IO - pwr;
        if (att1 < 0) {
            d_gain -= att1;
            att1 = 0;
		}
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 15.0)
        att2 += R1C_TX_BASE_POWER_IO - pwr;
    else if (pwr >= R1C_TX_BASE_POWER_IO - 30.0) {
        att2 += 15.0;
        att3 += R1C_TX_BASE_POWER_IO - pwr - 15.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 35.0) {
        att2 += R1C_TX_BASE_POWER_IO - pwr - 15.0;
        att3 += 15.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 45.0) {
        att2 += 20.0;
        att3 += R1C_TX_BASE_POWER_IO - pwr - 20.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 50.0) {
        att3 += 25.0;
        att2 += R1C_TX_BASE_POWER_IO - pwr - 25.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 55.0) {
        att2 += 25.0;
        att3 += 25.0;
        att1 += R1C_TX_BASE_POWER_IO - pwr - 50.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 60.0) {
        att1 += 5.0;
        att2 += 25.0;
        att3 += 25.0;
        att0 += R1C_TX_BASE_POWER_IO - pwr - 55.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 65.0) {
        att0 += 5.0;
        att1 += 5.0;
        att2 += 25.0;
        att3 += R1C_TX_BASE_POWER_IO - pwr - 35.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 70.0) {
        att0 += 5.0;
        att1 += 5.0;
        att3 += 30.0;
        att2 += R1C_TX_BASE_POWER_IO - pwr - 40.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 75.0) {
        att0 += 5.0;
        att2 += 30.0;
        att3 += 30.0;
        att1 += R1C_TX_BASE_POWER_IO - pwr - 65.0;
	}
    else if (pwr >= R1C_TX_BASE_POWER_IO - 85.0) {
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
        att0 += R1C_TX_BASE_POWER_IO - pwr - 70.0;
	}
	else {
        att0 += 15.0;
        att1 += 10.0;
        att2 += 30.0;
        att3 += 30.0;
        d_gain -= R1C_TX_BASE_POWER_IO - pwr - 85.0;
	}
    if (att0 > 31.5) {
        d_gain -= (att0 - 31.5);
        att0 = 31.5;
    }

    data->att0 = (short)(att0 * 2);
    data->att1 = (short)(att1 * 2);
    data->att2 = (short)(att2 * 2);
    data->att3 = (short)(att3 * 2);
    data->d_gain = (float)d_gain;
    data->temp_5 = data_base.temp_5;
}

void tx_pwr_io_table_r1cd::guess_base(double &att0,double &att1,double &att2,double &att3,double &d_gain)
{
    att0 = 5.0;
    att1 = 20.0;
    att2 = 0.0;
    att3 = 1.0;
    d_gain = -5.0;
}

void tx_pwr_io_table_r1cd::get_base(uint64_t freq,data_m_t *data)
{
    freq = freq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : freq;
    freq = freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : freq;

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    uint32_t freq_idx_l = (uint32_t)SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);
    uint32_t freq_idx_r = (uint32_t)SERIE_INDEX(freq_r,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_CALLED);

    data_m_t data_l = m_data_m[freq_idx_l];
    data_m_t data_r = m_data_m[freq_idx_r];

    if (data_l.att0 == data_r.att0 &&
        data_l.att1 == data_r.att1 &&
        data_l.att2 == data_r.att2 &&
        data_l.att3 == data_r.att3 &&
        data_l.temp_5 == data_r.temp_5) {
            linear(freq_l,data_l.d_gain,freq_r,data_r.d_gain,freq,data->d_gain);
	}
	else
        data->d_gain = data_l.d_gain;

    data->att0  = data_l.att0;
    data->att1  = data_l.att1;
    data->att2  = data_l.att2;
    data->att3  = data_l.att3;
    data->temp_5 = data_l.temp_5;
}

void tx_pwr_io_table_r1cd::get_base(uint64_t freq,data_f_t *data)
{
    freq = freq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : freq;
    freq = freq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : freq;

    data_m_t data_m;
    get_base(freq,&data_m);

    uint64_t freq_l = 0,freq_r = 0;
    discretept(freq,RF_TX_FREQ_STEP_CALLED,freq_l,freq_r);

    *data = m_data_f[(uint32_t)SERIE_INDEX(freq_l,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE)];
    data->freq = freq;
    data->att0 = (double)(data_m.att0 / 2.0);
    data->att1 = (double)(data_m.att1 / 2.0);
    data->att2 = (double)(data_m.att2 / 2.0);
    data->att3 = (double)(data_m.att3 / 2.0);
    data->d_gain = (double)(data_m.d_gain);
}

void tx_pwr_io_table_r1cd::get(uint64_t freq,double pwr,data_m_t *data)
{
    data_m_t data_base;
    pwr = pwr > (double)R1C_TX_ATT_IO_POWER_STAR ? (double)R1C_TX_ATT_IO_POWER_STAR : pwr;
    double pwr_l = 0.0,pwr_r = 0.0;

    discretept(pwr,R1C_TX_ATT_STEP,pwr_l,pwr_r);
    get_base(freq,&data_base);
    tx_pwr_io_table_r1cd::get_config_table((int32_t)pwr_l,data_base,data);
    data->d_gain -= (float)(pwr_l - pwr);
}

void tx_pwr_io_table_r1cd::get(uint64_t freq,double pwr,data_f_t *data)
{
    data_m_t data_m;
    get(freq,pwr,&data_m);
    get_base(freq,data);
    data->att0 = (double)(data_m.att0 / 2.0);
    data->att1 = (double)(data_m.att1 / 2.0);
    data->att2 = (double)(data_m.att2 / 2.0);
    data->att3 = (double)(data_m.att3 / 2.0);
    data->d_gain = (double)(data_m.d_gain);
}

void tx_pwr_io_table_r1cd::save_as(const char *path)
{
    FILE *fp = fopen(path,"w");
	if (NULL == fp)
		return;

    double pwr = 0.0;
    tx_pwr_io_table_r1cd::data_f_t data;

    for (uint64_t freq = RF_TX_FREQ_STAR;freq <= RF_TX_FREQ_STOP;freq += RF_TX_FREQ_STEP_INFILE) {
        for (pwr = R1C_TX_ATT_IO_POWER_STAR;pwr >= -120;pwr --) {
            get(freq,pwr,&data);
            fprintf(fp,"freq : %10I64u \t power : %4.1f \t digital gain : %6.3f \t att0 : %3.1f \t att1 : %3.1f \t att2 : %3.1f \t att3 : %3.1f \t temp4 : %-.3f temp5 : %-.3f temp6 : %-.3f temp7 : %-.3f time : %4d/%02d/%02d %02d:%02d:%02d\n",
                    freq,
                    pwr,
                    data.d_gain,
                    data.att0,
                    data.att1,
                    data.att2,
                    data.att3,
                    data.temp[0],data.temp[1],data.temp[2],data.temp[3],
                    data.time.tm_year + 1900,data.time.tm_mon,data.time.tm_mday,data.time.tm_hour,data.time.tm_min,data.time.tm_sec);
		}
	}
	fclose(fp);
}
