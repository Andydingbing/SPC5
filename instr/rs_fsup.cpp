#include "rs_fsup.h"
#include <stdlib.h>
#include <string>
#include <boost/format.hpp>
#include <boost/core/ignore_unused.hpp>

using namespace std;
using namespace boost;

bool rs_fsup::reset()
{
    return w("SYSTem:PRESet;*WAI");
}

bool rs_fsup::set_cf(double freq)
{
    return w((boost::format("FREQ:CENT %.0f;*WAI") % freq).str());
}

bool rs_fsup::set_span(double freq)
{
    return w((boost::format("FREQ:SPAN %.0f;*WAI") % freq).str());
}

bool rs_fsup::set_rbw(double freq, bool is_auto)
{
    if (is_auto) {
        return w("BAND:AUTO ON;*WAI");
    }

    return w((boost::format("BAND %.0f Hz;*WAI") % freq).str());
}

bool rs_fsup::set_vbw(double freq, bool is_auto)
{
    if (is_auto) {
        return w("BAND:VID:AUTO ON;*WAI");
    }

    return w((boost::format("BAND:VID %.0f Hz;*WAI") % freq).str());
}

bool rs_fsup::set_det(det_type_t type)
{
    switch (type) {
        case DET_AVERAGE  : return w("DET RMS;*WAI");
        case DET_NORMAL   : return w("DET APE;*WAI");
        case DET_PEAK     : return w("DET POS;*WAI");
        case DET_AUTO     : return w("DET:AUTO ON;*WAI");
        case DET_SAMPLE   : return w("DET SAMP;*WAI");
        case DET_NEG_PEAK : return w("DET NEG;*WAI");
	}
	return false;
}

bool rs_fsup::set_peak_search(peak_search_type_t type)
{
    switch (type) {
        case PEAK_HIGHEST    : return w("CALC:MARK1:MAX;*WAI");
        case PEAK_NEXT_HIGH  : return w("CALC:MARK1:MAX:NEXT;*WAI");
        case PEAK_NEXT_RIGHT : return w("CALC:MARK1:MAX:RIGH;*WAI");
        case PEAK_NEXT_LEFT  : return w("CALC:MARK1:MAX:LEFT;*WAI");
	}
	return false;
}

bool rs_fsup::set_ref(double ref)
{
    return w((boost::format("DISP:WIND:TRAC:Y:RLEV %f dbm;*WAI") % ref).str());
}

bool rs_fsup::get_ref(double &ref)
{
    string buf;

    BOOL_CHECK(w("DISP:WIND:TRAC:Y:RLEV?;*WAI"));
    BOOL_CHECK(r(buf,256));
    ref = atof(buf.c_str());
	return true;
}

bool rs_fsup::set_marker(marker_type_t type)
{
    switch (type) {
        case MARKER_OFF    : return w("CALC:MARK1:STAT OFF;*WAI");
        case MARKER_NORMAL : return w("CALC:MARK1:MODE POS;*WAI");
        case MARKER_DELTA  : return w("CALC:MARK1:MODE DELT;*WAI");
	}
	return false;
}

bool rs_fsup::set_en_preamp(bool en)
{
    if (en)
        return w("INP:GAIN:STAT ON;*WAI");
	else
        return w("INP:GAIN:STAT OFF;*WAI");
}

bool rs_fsup::get_marker_pwr(double &pwr)
{
    string buf;
    double freq = 0.0;

    BOOL_CHECK(w("CALC:MARK1:Y?;*WAI"));
    BOOL_CHECK(r(buf,256));
    BOOL_CHECK(get_marker_freq(freq));

    pwr = atof(buf.c_str());
	return true;
}

bool rs_fsup::get_marker_freq(double &freq)
{
    string buf;

    BOOL_CHECK(w("CALC:MARK1:X?;*WAI"));
    BOOL_CHECK(r(buf, 200));
    freq = atof(buf.c_str());
	return true;
}

bool rs_fsup::wait_for_continue()
{
    return w("*WAI");
}

bool rs_fsup::sweep_once()
{
    BOOL_CHECK(w("INIT1:CONT OFF;*WAI;"));
    BOOL_CHECK(w("INIT1:IMM;*WAI;"));
	return true;
}

bool rs_fsup::marker_move(marker_move_type_t type)
{
    switch (type) {
        case MARKER_TO_CF     : return w("CALC:MARK1:FUNC:CENT;*WAI;");
        case MARKER_TO_REFLVL : return w("CALC:MARK1:FUNC:REF;*WAI;");
	}
	return false;
}

bool rs_fsup::marker_move(double freq)
{
    return w((boost::format("CALC:MARK1:X %f HZ;*WAI") % freq).str());
}

bool rs_fsup::set_cal(cal_type_t type)
{
    ignore_unused(type);
    return w("CAL:ALL;*WAI");
}

bool rs_fsup::set_avg_trace(bool en,uint32_t cnt)
{
    if (en) {
        BOOL_CHECK(w((boost::format("AVER:COUN %d;*WAI") % cnt).str()));
        BOOL_CHECK(w("AVER:STAT ON;*WAI"));
	} else {
        BOOL_CHECK(w("AVER:STAT OFF;*WAI"));
	}
	return true;
}

bool rs_fsup::set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data)
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

bool rs_fsup::set_mech_att(bool is_auto,int32_t att)
{
    ignore_unused(is_auto,att);
	return false;
}
