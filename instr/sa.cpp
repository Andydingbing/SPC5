#include "sa.h"
#include "sleep_common.h"
#include "algo_math.hpp"

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
