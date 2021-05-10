#include "sa.h"
#include "sleep_common.h"
#include "algo_math.hpp"
#include <boost/format.hpp>

using namespace std;
using namespace boost;

bool sa::set_window_zoom(const bool is_zoomed) const
{
    return is_zoomed ? w("DISP:WIND:FORM:ZOOM") : w("DISP:WIND:FORM:TILE");
}

bool sa::set_window_next() const
{
    uint8_t n = 0;

    BOOL_CHECK(get_window_number(n));
    n += 1;
    return set_window_number(n);
}

bool sa::set_window_number(const uint8_t n) const
{
    return w((format("DISP:WIND %d;*WAI") % n).str());
}

bool sa::get_window_number(uint8_t &n) const
{
    BOOL_CHECK(w_block("DISP:WIND?"));
    return r_to_uint8(n);
}

bool sa::set_window_fullscreen(const bool is_full) const
{
    return is_full ? w_block("DISP:FSCR 1") : w_block("DISP:FSCR 0");
}

bool sa::get_window_fullscreen(bool &is_full) const
{
    BOOL_CHECK(w_block("DISP:FSCR?"));
    return r_to_bool(is_full);
}

bool sa::set_window_display(const bool is_on) const
{
    return is_on ? w_block("DISP:ENAB 1") : w_block("DISP:ENAB 0");
}

bool sa::get_window_display(bool &is_on) const
{
    BOOL_CHECK(w_block("DISP:ENAB?"));
    return r_to_bool(is_on);
}

bool sa::set_io_reset() const
{
    return w_block("SYST:DEF INP");
}

bool sa::set_io_mixer(const io_mixer_t mixer) const
{
    if (mixer == IO_MIXER_INTERNAL) {
        return w_block("INP:MIX INT");
    } else {
        return w_block("INP:MIX EXT");
    }
}

bool sa::get_io_mixer(io_mixer_t &mixer) const
{
    uint32_t n_mixer = 0;

    BOOL_CHECK(w_block("INP:MIX?"));
    BOOL_CHECK(r_to_uint32(n_mixer));
    mixer = io_mixer_t(n_mixer);
    return true;
}

bool sa::set_io_impedance(const io_impedance_t ohm) const
{
    if (ohm == IO_IMPEDANCE_50) {
        return w_block("CORR:IMP 50");
    } else {
        return w_block("CORR:IMP 75");
    }
}

bool sa::get_io_impedance(io_impedance_t &ohm) const
{
    uint32_t n_ohm = 0;

    BOOL_CHECK(w_block("CORR:IMP?"));
    BOOL_CHECK(r_to_uint32(n_ohm));
    ohm = io_impedance_t(n_ohm);
    return true;
}

bool sa::set_io_coupling(const io_coupling_t coupling) const
{
    if (coupling == IO_COUPLING_AC) {
        return w_block("INP:COUP AC");
    } else {
        return w_block("INP:COUP DC");
    }
}

bool sa::get_io_coupling(io_coupling_t &coupling) const
{
    string buf;

    BOOL_CHECK(w_block("INP:COUP?"));
    BOOL_CHECK(r(buf,32));
    coupling = (buf == "AC" ? IO_COUPLING_AC : IO_COUPLING_DC);
    return true;
}

bool sa::set_io_ref_in(const io_ref_in_t ref) const
{
    switch (ref) {
    case IO_REF_IN_SENSE    : return w_block("ROSC:SOUR:TYPE SENS");
    case IO_REF_IN_INTERNAL : return w_block("ROSC:SOUR:TYPE INT");
    case IO_REF_IN_EXTERNAL : return w_block("ROSC:SOUR:TYPE EXT");
    case IO_REF_IN_PULSE    : return w_block("ROSC:SOUR:TYPE PULS");
    }
    return false;
}

bool sa::get_io_ref_in(io_ref_in_t &ref) const
{
    uint32_t n_ref = 0;

    BOOL_CHECK(w_block("ROSC:SOUR:TYPE?"));
    BOOL_CHECK(r_to_uint32(n_ref));

    if (n_ref == 7 || n_ref == 11) {
        ref = IO_REF_IN_INTERNAL;
    } else if (n_ref == 8 || n_ref == 10) {
        ref = IO_REF_IN_EXTERNAL;
    } else {
        ref = IO_REF_IN_PULSE;
    }
    return true;
}

bool sa::set_io_ref_ext_freq(const double freq) const
{
    return w_block((format("ROSC:EXT:FREQ %f") % freq).str());
}

bool sa::get_io_ref_ext_freq(double &freq) const
{
    BOOL_CHECK(w_block("ROSC:EXT:FREQ?"));
    return r_to_double(freq);
}

bool sa::set_io_ref_ext_freq_default() const
{
    return w_block("ROSC:EXT:FREQ:DEF");
}

bool sa::set_io_ref_ext_bw(const io_ref_ext_bw_t bw) const
{
    return w_block((format("ROSC:BAND %s") % (bw == IO_REF_EXT_BW_WIDE ? "WIDE" : "NARR")).str());
}

bool sa::get_io_ref_ext_bw(io_ref_ext_bw_t &bw) const
{
    string buf;

    BOOL_CHECK(w_block("ROSC:BAND?"));
    BOOL_CHECK(r(buf,32));
    bw = (buf == "WIDE" ? IO_REF_EXT_BW_WIDE : IO_REF_EXT_BW_NARROW);
    return true;
}

bool sa::set_ref_auto()
{
    double ref = 0.0;
    double peak_pwr = 0.0;
    double delta = 0.0;

    BOOL_CHECK(sweep_once());
    BOOL_CHECK(set_peak_search(PEAK_HIGHEST));
    BOOL_CHECK(get_marker_pwr(peak_pwr));
    BOOL_CHECK(get_ref(ref));

    delta = ref - peak_pwr;

    if (delta > 0.0 && delta <= 40.0) {
        return true;
    }

    do {
        BOOL_CHECK(set_ref(int(peak_pwr) + 5));
        BOOL_CHECK(sweep_once());
        BOOL_CHECK(get_marker_pwr(peak_pwr));
        BOOL_CHECK(get_ref(ref));
    } while(peak_pwr > ref);
    sleep_ms(200);
    return true;
}

bool sa::set_ref_auto(double pwr,double &ref)
{
    pwr += 10.0;
    double ref_cur = 0.0;

    BOOL_CHECK(get_ref(ref_cur));

    ref = linear_quantify(20.0,-20.0,pwr);

    if (ref_cur - ref != 0.0) {
        BOOL_CHECK(set_ref(ref));
    } else {
        return true;
    }

    if (pwr < -40.0) {
        BOOL_CHECK(set_en_preamp(true));
        BOOL_CHECK(set_mech_att(false,0));

        if (pwr > -60.0) {
            BOOL_CHECK(set_avg_trace(true,3));
        } else {
            BOOL_CHECK(set_avg_trace(true,6));
        }
        sleep_ms(100);
    }
    return true;

}

bool sa::set_ref_auto(double pwr,double &ref,double &offset)
{
    pwr += 10.0;

    double pwr_before = 0.0;
    double pwr_after  = 0.0;
    double ref_cur = 0.0;
    bool change_setting = false;

    BOOL_CHECK(get_ref(ref_cur));

    ref = linear_quantify(20.0,-20.0,pwr);

    if (ref_cur - ref != 0.0) {
        change_setting = true;

        BOOL_CHECK(sweep_once());
        BOOL_CHECK(set_peak_search(PEAK_HIGHEST));
        BOOL_CHECK(get_marker_pwr(pwr_before));
        BOOL_CHECK(set_ref(ref));
    } else {
        return true;
    }

    if (pwr < -40.0) {
        BOOL_CHECK(set_en_preamp(true));
        BOOL_CHECK(set_mech_att(false,0));

        if (pwr > -60.0) {
            BOOL_CHECK(set_avg_trace(true,3));
        } else {
            BOOL_CHECK(set_avg_trace(true,6));
        }
        sleep_ms(100);
    }

    BOOL_CHECK(sweep_once());
    BOOL_CHECK(set_peak_search(PEAK_HIGHEST));
    BOOL_CHECK(get_marker_pwr(pwr_after));

    offset += pwr_after - pwr_before;
    return true;

}
