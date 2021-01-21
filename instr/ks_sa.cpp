#include "ks_sa.h"
#include <string>
#include <boost/format.hpp>

using namespace std;

bool ks_n9030a::reset()
{
    return w("*RST;*WAI");
}

bool ks_n9030a::set_cf(double freq)
{
    return w((boost::format("FREQ:CENT %.0f;*WAI") % freq).str());
}

bool ks_n9030a::set_span(double freq)
{
    return w((boost::format("FREQ:SPAN %.0f;*WAI") % freq).str());
}

bool ks_n9030a::set_rbw(double freq,bool is_auto)
{
    if (is_auto) {
        return w("BAND:AUTO ON;*WAI");
    }

    return w((boost::format("BAND %.0f Hz;*WAI") % freq).str());
}

bool ks_n9030a::set_vbw(double freq,bool is_auto)
{
    if (is_auto) {
        return w("BAND:VID:AUTO ON;*WAI");
    }

    return w((boost::format("BAND:VID %.0f Hz;*WAI") % freq).str());
}

bool ks_n9030a::set_det(det_type_t type)
{
    switch (type) {
        case DET_AVERAGE  : return w("DET RMS;*WAI");
        case DET_NORMAL   : return w("DET NORM;*WAI");
        case DET_PEAK     : return w("DET POS;*WAI");
        case DET_AUTO     : return w("DET:AUTO ON;*WAI");
        case DET_SAMPLE   : return w("DET SAMP;*WAI");
        case DET_NEG_PEAK : return w("DET NEG;*WAI");
	}
	return false;
}

bool ks_n9030a::set_peak_search(peak_search_type_t type)
{
    switch (type) {
        case PEAK_HIGHEST    : return w("CALC:MARK1:MAX;*WAI");
        case PEAK_NEXT_HIGH  : return w("CALC:MARK1:MAX:NEXT;*WAI");
        case PEAK_NEXT_RIGHT : return w("CALC:MARK1:MAX:RIGH;*WAI");
        case PEAK_NEXT_LEFT  : return w("CALC:MARK1:MAX:LEFT;*WAI");
	}
	return false;
}

bool ks_n9030a::set_ref(double ref)
{
    return w((boost::format("DISP:WIND:TRAC:Y:RLEV %f dbm;*WAI") % ref).str());
}

bool ks_n9030a::get_ref(double &ref)
{
    string buf;
    BOOL_CHECK(w("DISP:WIND:TRAC:Y:RLEV?;*WAI"));
    BOOL_CHECK(r(buf,256));
    ref = atof(buf.c_str());
	return true;
}

bool ks_n9030a::set_marker(marker_type_t type)
{
    switch (type) {
        case MARKER_OFF    : return w("CALC:MARK1:STAT OFF;*WAI");
        case MARKER_NORMAL : return w("CALC:MARK1:MODE POS;*WAI");
        case MARKER_DELTA  : return w("CALC:MARK1:MODE DELT;*WAI");
	}
	return false;
}

bool ks_n9030a::set_en_preamp(bool en)
{
    if (en) {
        BOOL_CHECK(w("POW:GAIN:STAT ON;POW:GAIN:BAND FULL;*WAI"));
	} else {
        BOOL_CHECK(w("POW:GAIN:STAT OFF;*WAI"));
	}
	return true;
}

bool ks_n9030a::is_en_preamp(bool &en)
{
    string buf;

    BOOL_CHECK(w("POW:GAIN:STAT?;*WAI"));
    BOOL_CHECK(r(buf,256));

    en = atoi(buf.c_str()) == 1;
    return true;
}

bool ks_n9030a::get_marker_pwr(double &pwr)
{
    string buf;
    double freq = 0.0;

    BOOL_CHECK(w("CALC:MARK1:Y?;*WAI"));
    BOOL_CHECK(r(buf,256));
    BOOL_CHECK(get_marker_freq(freq));

    pwr= atof(buf.c_str());
	return true;
}

bool ks_n9030a::get_marker_freq(double &freq)
{
    string buf;

    BOOL_CHECK(w("CALC:MARK1:X?;*WAI"));
    BOOL_CHECK(r(buf,256));

    freq = atof(buf.c_str());
	return true;
}

bool ks_n9030a::wait_for_continue()
{
    return w("*WAI");
}

bool ks_n9030a::sweep_once()
{
    string buf;

    BOOL_CHECK(w("INIT:IMM;*WAI"));
    BOOL_CHECK(w("*OPC?"));
    BOOL_CHECK(r(buf,256,60000));
	return true;
}

bool ks_n9030a::marker_move(marker_move_type_t type)
{
    switch (type) {
        case MARKER_TO_CF     : return w("CALC:MARK1:CENT;*WAI");
        case MARKER_TO_REFLVL : return w("CALC:MARK1:RLEV;*WAI");
	}
	return false;
}

bool ks_n9030a::marker_move(double freq)
{
    return w((boost::format("CALC:MARK1:X %f HZ;*WAI") % freq).str());
}

bool ks_n9030a::set_cal(cal_type_t type)
{
    switch (type) {
        case AUTO_OFF : return w("CAL:AUTO OFF;*WAI");
        case AUTO_ON  : return w("CAL:AUTO ON;*WAI");
        case CAL_NOW  : return w("CAL:ALL;*WAI");
	}
	return false;
}

bool ks_n9030a::set_avg_trace(bool en, uint32_t cnt)
{
    if (en) {
        BOOL_CHECK(w((boost::format("AVER:COUN %d;*WAI") % cnt).str()));
        BOOL_CHECK(w("AVER ON;*WAI"));
	} else {
        BOOL_CHECK(w("AVER OFF;*WAI"));
	}
	return true;
}

bool ks_n9030a::get_trace_avg_cnt(uint32_t &cnt)
{
    string buf;

    BOOL_CHECK(w("AVER:COUN?;*WAI"));
    BOOL_CHECK(r(buf,256));

    cnt = uint32_t(atol(buf.c_str()));
    return true;
}

bool ks_n9030a::is_en_avg_trace(bool &en)
{
    string buf;

    BOOL_CHECK(w("AVER?;*WAI"));
    BOOL_CHECK(r(buf,256));

    en = atoi(buf.c_str()) == 1;
    return true;
}

bool ks_n9030a::set_avg_trace_get_data(uint32_t avg_cnt, uint32_t pt_cnt, double *data)
{
    uint32_t buf_size = 18 * pt_cnt;
    string buf;

    BOOL_CHECK(w((boost::format("SWE:POIN %d;*WAI") % pt_cnt).str()));
    BOOL_CHECK(set_avg_trace(true,avg_cnt));
    BOOL_CHECK(sweep_once());
    BOOL_CHECK(w("FORM ASCii;*WAI"));
    BOOL_CHECK(w("TRAC? TRACE1;*WAI"));
    BOOL_CHECK(r(buf,buf_size));

    uint32_t data_idx = 0;
    uint32_t last_idx = 0;

    for (uint32_t i = 0;i < buf_size;i ++) {
        if (buf[i] == ',' || i == buf_size - 1) {
            buf[i] = 0;
            data[data_idx ++] = atof(&buf[last_idx]);
            last_idx = i + 1;
			i++;
		}
    }

	return true;
}

bool ks_n9030a::set_mech_att(bool is_auto,int32_t att)
{
    if (is_auto) {
        BOOL_CHECK(w("POW:ATT:AUTO ON"));
	} else {
        BOOL_CHECK(w("POW:ATT:AUTO OFF"));
        BOOL_CHECK(w((boost::format("POW:ATT %d") % att).str()));
	}
	return true;
}


bool ks_e4445a::set_en_preamp(bool en)
{
    if (en) {
        BOOL_CHECK(w("POW:GAIN:STAT ON"));
    } else {
        BOOL_CHECK(w("POW:GAIN:STAT OFF"));
    }
    return true;
}
