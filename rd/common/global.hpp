#ifndef RD_COMMON_GLOBAL_H
#define RD_COMMON_GLOBAL_H

#include "rd.h"

namespace rd {

STRUCT_ALIGN_S(coef_complex_t,2)
    int16_t real : 16;
    int16_t imag : 16;
STRUCT_ALIGN_E(coef_complex_t,2)

namespace ns_sp1403 {

static const uint64_t tx_freq_star = FREQ_M(50);
static const uint64_t tx_freq_sec0 = FREQ_M(3000);
static const uint64_t tx_freq_sec1 = FREQ_M(4800);
static const uint64_t tx_freq_sec2 = FREQ_M(6000);
static const uint64_t tx_freq_sec3 = FREQ_M(7500);
static const uint64_t tx_freq_sec4 = FREQ_M(24250);
static const uint64_t tx_freq_sec5 = FREQ_M(29500);
static const uint64_t tx_freq_sec6 = FREQ_M(37000);
static const uint64_t tx_freq_stop = FREQ_M(43500);

static const uint64_t rx_freq_star = FREQ_M(7500);

} // namespace ns_sp1403

namespace ns_sp2406 {

static const uint16_t dl_cf_tap = 196;
static const uint16_t ul_cf_tap = 196;

} // namespace ns_sp2406

namespace ns_sp9500x {

static const uint32_t g_max_fan = 16;

} // namespace ns_sp9500x
} // namespace rd

#endif
