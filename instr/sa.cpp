#include "sa.h"
#include "sleep_common.h"

bool sa::set_ref_auto()
{
    double ref = 0.0;
    double peak_pwr = 0.0;
	do {
        BOOL_CHECK(sweep_once());
        BOOL_CHECK(set_peak_search(PEAK_HIGHEST));
        BOOL_CHECK(get_marker_pwr(peak_pwr));
        BOOL_CHECK(get_ref(ref));
        BOOL_CHECK(set_ref(int(peak_pwr) + 5));
        BOOL_CHECK(sweep_once());
        BOOL_CHECK(set_peak_search(PEAK_HIGHEST));
        BOOL_CHECK(get_marker_pwr(peak_pwr));
    } while(peak_pwr > ref);
    sleep_ms(200);
	return true;
}
