#include "ks_n9030a.h"
#include <stdlib.h>
#include <string.h>

ks_n9030a::ks_n9030a() : sa()
{
}

ks_n9030a::~ks_n9030a()
{
}

int32_t ks_n9030a::get_default_pri_addr()
{
	return -1;
}

int32_t ks_n9030a::get_default_sec_addr()
{
	return -1;
}

char* ks_n9030a::get_des()
{
    return const_cast<char *>("N9030A");
}

bool ks_n9030a::reset()
{
    return w("*RST;*WAI");
}

bool ks_n9030a::set_cf(double freq)
{
    return w("FREQ:CENT %.0f;*WAI",freq);
}

bool ks_n9030a::set_span(double freq)
{
    return w("FREQ:SPAN %.0f;*WAI",freq);
}

bool ks_n9030a::set_rbw(double freq,bool is_auto)
{
    if (is_auto)
        return w("BAND:AUTO ON;*WAI");

    return w("BAND %.0f Hz;*WAI",freq);
}

bool ks_n9030a::set_vbw(double freq,bool is_auto)
{
    if (is_auto)
        return w("BAND:VID:AUTO ON;*WAI");

    return w("BAND:VID %.0f Hz;*WAI",freq);
}

bool ks_n9030a::set_det(det_type_t type)
{
    switch (type) {
        case DET_AVERAGE    : return w("DET RMS;*WAI");
        case DET_NORMAL     : return w("DET NORM;*WAI");
        case DET_PEAK       : return w("DET POS;*WAI");
        case DET_AUTO       : return w("DET:AUTO ON;*WAI");
        case DET_SAMPLE     : return w("DET SAMP;*WAI");
        case DET_NEG_PEAK   : return w("DET NEG;*WAI");
		default:return false;
	}
	return false;
}

bool ks_n9030a::set_peak_search(peak_search_type_t type)
{
    switch (type) {
        case PEAK_HIGHEST       : return w("CALC:MARK1:MAX;*WAI");
        case PEAK_NEXT_HIGH     : return w("CALC:MARK1:MAX:NEXT;*WAI");
        case PEAK_NEXT_RIGHT    : return w("CALC:MARK1:MAX:RIGH;*WAI");
        case PEAK_NEXT_LEFT     : return w("CALC:MARK1:MAX:LEFT;*WAI");
		default:return false;
	}
	return false;
}

bool ks_n9030a::set_ref(double ref)
{
    return w("DISP:WIND:TRAC:Y:RLEV %f dbm;*WAI",ref);
}

bool ks_n9030a::get_ref(double &ref)
{
    char buf[256] = {0};
    memset(buf,0,sizeof(buf));

    BOOL_CHECK(w("DISP:WIND:TRAC:Y:RLEV?;*WAI"));
    BOOL_CHECK(r(buf,256));
    ref = atof(buf);
	return true;
}

bool ks_n9030a::set_marker(marker_type_t type)
{
    switch (type) {
        case MARKER_OFF     : return w("CALC:MARK1:STAT OFF;*WAI");
        case MARKER_NORMAL  : return w("CALC:MARK1:MODE POS;*WAI");
        case MARKER_DELTA   : return w("CALC:MARK1:MODE DELT;*WAI");
		default:return false;
	}
	return false;
}

bool ks_n9030a::set_en_preamp(bool en)
{
    if (en) {
        BOOL_CHECK(w(":POW:GAIN:STAT ON;:POW:GAIN:BAND FULL;*WAI"));
	} else {
        BOOL_CHECK(w("POW:GAIN:STAT OFF;*WAI"));
	}
	return true;
}

bool ks_n9030a::get_marker_pwr(double &pwr)
{
    char buf[256] = {0};
    double freq = 0.0;

    memset(buf,0,sizeof(buf));
    BOOL_CHECK(w("CALC:MARK1:Y?;*WAI"));
    BOOL_CHECK(r(buf,256));
    BOOL_CHECK(get_marker_freq(freq));

    pwr= atof(buf);
	return true;
}

bool ks_n9030a::get_marker_freq(double &freq)
{
    char buf[256] = {0};
    memset(buf,0,sizeof(buf));

    BOOL_CHECK(w("CALC:MARK1:X?;*WAI"));
    BOOL_CHECK(r(buf,256));
    freq = atof(buf);
	return true;
}

bool ks_n9030a::wait_for_continue()
{
    return w("*WAI");
}

bool ks_n9030a::sweep_once()
{
    char buf[256] = {0};
    memset(buf,0,sizeof(buf));

    BOOL_CHECK(w("INIT:IMM;*WAI"));
    BOOL_CHECK(w("*OPC?"));
    BOOL_CHECK(r(buf,256,60000));
	return true;
}

bool ks_n9030a::marker_move(marker_move_type_t type)
{
    switch (type) {
        case MARKER_TO_CF       : return w("CALC:MARK1:CENT;*WAI");
        case MARKER_TO_REFLVL   : return w("CALC:MARK1:RLEV;*WAI");
		default:return false;
	}
	return false;
}

bool ks_n9030a::marker_move(double freq)
{
    return w("CALC:MARK1:X %f HZ;*WAI",freq);
}

bool ks_n9030a::set_cal(cal_type_t type)
{
    switch (type) {
        case AUTO_OFF   : return w("CAL:AUTO OFF;*WAI");
        case AUTO_ON    : return w("CAL:AUTO ON;*WAI");
        case CAL_NOW    : return w("CAL:ALL;*WAI");
		default: return false;
	}
	return false;
}

bool ks_n9030a::set_avg_trace(bool en,int32_t cnt)
{
    if (en) {
        BOOL_CHECK(w("AVER:COUN %d;*WAI",cnt));
        BOOL_CHECK(w("AVER ON;*WAI"));
	} else {
        BOOL_CHECK(w("AVER OFF;*WAI"));
	}
	return true;
}

bool ks_n9030a::set_avg_trace_get_data(int32_t avg_cnt,int32_t pt_cnt,double *data)
{
    BOOL_CHECK(w("SWE:POIN %d;*WAI",pt_cnt));
    BOOL_CHECK(set_avg_trace(true,avg_cnt));
    BOOL_CHECK(sweep_once());
    BOOL_CHECK(w("FORM ASCii;*WAI"));
    BOOL_CHECK(w("TRAC? TRACE1;*WAI"));

    int buf_size = 18 * pt_cnt;
    char *buf = new char[buf_size];
    BOOL_CHECK_S(r(buf,buf_size),delete []buf);

    int data_idx = 0,last_idx = 0;
    for (int i = 0;i < buf_size;i ++) {
        if (buf[i] == ',' || i == buf_size - 1) {
            buf[i] = 0;
            data[data_idx ++] = atof(&buf[last_idx]);
            last_idx = i + 1;
			i++;
		}
	}
    delete []buf;
	return true;
}

bool ks_n9030a::set_mech_att(bool is_auto,int32_t att)
{
    if (is_auto) {
        BOOL_CHECK(w("POW:ATT:AUTO ON"));
	} else {
        BOOL_CHECK(w("POW:ATT:AUTO OFF"));
        BOOL_CHECK(w("POW:ATT %d",att));
	}
	return true;
}
