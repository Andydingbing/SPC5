#ifndef RD_COMMON_GLOBAL_H
#define RD_COMMON_GLOBAL_H

#include "rd.h"

namespace rd {

STRUCT_ALIGN_S(coef_complex_t,2)
    int16_t real : 16;
    int16_t imag : 16;
STRUCT_ALIGN_E(coef_complex_t,2)

namespace ns_sp1401 {
static const uint64_t tx_freq_star = 50000000;
static const uint64_t tx_freq_stop = 6000000000;
static const uint64_t tx_freq_step_called = 10000000;
static const uint64_t tx_freq_step_infile = 5000000;
static const uint32_t tx_freqs_called = 596;
static const uint32_t tx_freqs_infile = 1191;

static const uint64_t tx_filter_freq_star_0 = 17000000;   // (100  - 83)MHz
static const uint64_t tx_filter_freq_stop_0 = 3183000000; // (3100 + 83)MHz
static const uint64_t tx_filter_freq_star_1 = 3017000000; // (3100 - 83)MHz
static const uint64_t tx_filter_freq_stop_1 = 6083000000; // (6000 + 83)MHz
static const uint64_t tx_filter_freq_step   = 2000000;

static const uint32_t tx_filter_freqs_0 =
        SERIE_SIZE(tx_filter_freq_star_0,tx_filter_freq_stop_0,tx_filter_freq_step);

static const uint32_t tx_filter_freqs_1 =
        SERIE_SIZE(tx_filter_freq_star_1,tx_filter_freq_stop_1,tx_filter_freq_step);

static const uint32_t tx_filter_freqs = tx_filter_freqs_0 + tx_filter_freqs_1;

static const uint64_t rx_freq_star = 100000000;
static const uint64_t rx_freq_stop = 6000000000;
static const uint64_t rx_freq_step_infile = 5000000;

static const uint32_t rx_freqs_infile =
        SERIE_SIZE(rx_freq_star,rx_freq_stop,rx_freq_step_infile);

static const uint64_t rx_filter_freq_star = 17000000;   // (100  - 83) * 1000000
static const uint64_t rx_filter_freq_stop = 6083000000; // (6000 + 83) * 1000000
static const uint64_t rx_filter_freq_step = 2000000;

static const uint32_t rx_filter_freqs =
        SERIE_SIZE(rx_filter_freq_star,rx_filter_freq_stop,rx_filter_freq_step);

static const uint8_t g_sn_length = 24;

namespace r1a {
enum tx_pa_att_t { TX_PA,TX_ATT };
enum rx_lna_att_t { RX_ATT,RX_LNA };

static const uint64_t rx_freq_step_called = 5000000;
static const uint32_t rx_freqs_called = 1181;

static const double tx_pwr_star = +10.0;
static const double tx_pwr_stop = -20.0;
static const double tx_pwr_step = -10.0;
} // namespace r1a

namespace r1c {
enum rx_lna_att_t { RX_ATT,RX_LNA };
enum rx_att_019_t { RX_ATT_19,RX_ATT_0 };

static const uint64_t rx_freq_step_called = 10000000;
static const uint32_t rx_freqs_called = 591;

static const double tx_base_pwr_op = -5.0;
static const double tx_base_pwr_io = -12.0;

static const uint64_t tx_temp_comp_freq_step = 500000000;
static const uint32_t tx_temp_comp_freqs = 12; // (6000 - 50) / 500 + 1

static const uint64_t rx_temp_comp_freq_step = 500000000;
static const uint32_t rx_temp_comp_freqs = 12;
} // namespace r1c

namespace r1b = r1a;
namespace r1d = r1c;
namespace r1e = r1c;
namespace r1f = r1c;
} // namespace ns_sp1401

namespace ns_sp2401 {
static const uint16_t dl_filter_tap_2i = 41;
static const double   dl_filter_freq_step = 2000000.0;
static const double   dl_filter_160M_freq_star = -83000000.0; // (160 / 2) + 1.5 * 2
static const double   dl_filter_160M_freq_stop = +83000000.0;

static const uint32_t dl_filter_160M_freqs =
        SERIE_SIZE(dl_filter_160M_freq_star,dl_filter_160M_freq_stop,dl_filter_freq_step);

static const double   dl_filter_80M_freq_star = -43000000.0;  // (80  / 2) + 1.5 * 2
static const double   dl_filter_80M_freq_stop = +43000000.0;

static const uint32_t dl_filter_80M_freqs =
        SERIE_SIZE(dl_filter_80M_freq_star,dl_filter_80M_freq_stop,dl_filter_freq_step);

static const uint16_t ul_filter_tap = 96;
static const uint16_t ul_filter_tap_2i3d = 144;
static const double   ul_filter_freq_step = 2000000.0;
static const double   ul_filter_80M_freq_star  = -43000000.0;  // (80  / 2) + 1.5 * 2
static const double   ul_filter_80M_freq_stop  = +43000000.0;
static const double   ul_filter_160M_freq_star = -83000000.0; // (160 / 2) + 1.5 * 2
static const double   ul_filter_160M_freq_stop = +83000000.0;

static const uint32_t ul_filter_80M_freqs =
        SERIE_SIZE(ul_filter_80M_freq_star,ul_filter_80M_freq_stop,ul_filter_freq_step);

static const uint32_t ul_filter_160M_freqs =
        SERIE_SIZE(ul_filter_160M_freq_star,ul_filter_160M_freq_stop,ul_filter_freq_step);
} // namespace ns_sp2401

namespace ns_sp9500 {
static const uint8_t g_max_rfu = 5;
static const uint8_t g_max_rf  = 4; // each RFU
static const uint8_t g_max_fan = 12;

static const uint64_t g_rf_freq_space = 1000000;
static const uint64_t g_0dBFS = 80000000;
} // namespace ns_sp9500

namespace ns_dt3308 {
static const uint64_t g_0dBFS = 100000000;

enum hw_ver_t { HW_ERROR = -1, HW_VER_F = 0, HW_VER_MAX };

namespace f {
    enum rx_lna_att_0_t { RX_LNA_0,RX_ATT_0 };
    enum rx_lna_att_1_t { RX_LNA_1,RX_ATT_1 };
    enum rx_if_filter_t { RX_IF_40M = 0x2,RX_IF_100M = 0x8,RX_IF_160M = 0x7 };
} // namespace f
} // namespace dt3308

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
