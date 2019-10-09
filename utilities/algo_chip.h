/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RD_UTILITIES_ALGO_CHIP_H
#define RD_UTILITIES_ALGO_CHIP_H

#include "rd.h"
#include "reg_def.h"

// HMC624
namespace ns_hmc624 {

RD_INLINE void att(double *x)
{ *x = (int32_t(*x * 10) / 5) * 5 / 10.0; }

} // ns_hmc624


// ADI ADF5355
namespace ns_adf5355 {

struct RD_API freq_formula_in {
    uint64_t freq_vco;
    uint32_t freq_ref;
    uint32_t freq_chsp;
    uint32_t r_counter;
    bool ref_doubler;
    bool ref_divider;
    freq_formula_in();
};

struct freq_formula_out {
    uint32_t _int;
    uint32_t _frac1;
    uint32_t _frac2;
    uint32_t _mod1;
    uint32_t _mod2;
};

REG_DEF_S(0x00)
enum prescaler_t {
    _4_5,
    _8_9
};
    unsigned ctrl      : 4;
    unsigned _int      : 16;
    unsigned prescaler : 1;
    unsigned autocal   : 1;
    unsigned rsv       : 10;
    reg_t() :
        ctrl(0x00),
        prescaler(_4_5),
        autocal(1),
        rsv(0) {}
REG_DEF_E

REG_DEF_S(0x01)
    unsigned ctrl   : 4;
    unsigned _frac1 : 24;
    unsigned rsv    : 4;
    reg_t() :
        ctrl(0x01),
        rsv(0) {}
REG_DEF_E

REG_DEF_S(0x02)
    unsigned ctrl   : 4;
    unsigned _mod2  : 14;
    unsigned _frac2 : 14;
    reg_t() :
        ctrl(0x02) {}
REG_DEF_E

REG_DEF_S(0x03)
enum sd_load_reset_t {
    ON_REG0_UPDATE,
    DISABLED
};
    unsigned ctrl          : 4;
    unsigned phase_value   : 24;
    unsigned phase_adjust  : 1;
    unsigned phase_resync  : 1;
    unsigned sd_load_reset : 1;
    unsigned rsv           : 1;
    reg_t() :
        ctrl(0x03),
        phase_value(1),
        phase_adjust(0),
        phase_resync(0),
        sd_load_reset(ON_REG0_UPDATE),
        rsv(0) {}
REG_DEF_E

REG_DEF_S(0x04)
enum pd_polarity_t {
    NEGATIVE,
    POSITIVE
};
enum mux_logic_t {
    _1_8_V,
    _3_3_V
};
enum ref_mode_t {
    SINGLE,
    DIFF
};
enum current_set_t {
    _0_31mA,
    _0_63mA,
    _0_94mA,
    _1_25mA,
    _1_56mA,
    _1_88mA,
    _2_19mA,
    _2_50mA,
    _2_81mA,
    _3_13mA,
    _3_44mA,
    _3_75mA,
    _4_06mA,
    _4_38mA,
    _4_69mA,
    _5_00mA
};
enum mux_out_t {
    THREE_STATE_OUTPUT,
    DV,
    DGND,
    R_DIVIDER_OUTPUT,
    N_DIVIDER_OUTPUT,
    ANALOG_LOCK_DETECT,
    DIGITAL_LOCK_DETECT,
    RESERVED
};
    unsigned ctrl           : 4;
    unsigned counter_reset  : 1;
    unsigned cp_three_state : 1;
    unsigned pd             : 1;
    unsigned pd_polarity    : 1;
    unsigned mux_logic      : 1;
    unsigned ref_mode       : 1;
    unsigned current_set    : 4;
    unsigned double_buff    : 1;
    unsigned r_counter      : 10;
    unsigned rdiv2          : 1;
    unsigned ref_doubler    : 1;
    unsigned mux_out        : 3;
    unsigned rsv            : 2;
    reg_t() :
        ctrl(0x04),
        counter_reset(1),
        cp_three_state(0),
        pd(0),
        pd_polarity(POSITIVE),
        mux_logic(_3_3_V),
        ref_mode(DIFF),
        current_set(_0_31mA),
        double_buff(0),
        rdiv2(0),
        ref_doubler(0),
        mux_out(DIGITAL_LOCK_DETECT),
        rsv(0) {}
REG_DEF_E

REG_DEF_S(0x05)
    unsigned rsv : 32;
    reg_t() :
        rsv(0x00800025) {}
REG_DEF_E

REG_DEF_S(0x06)
enum rf_output_pwr_t {
    __4dBm,
    __1dBm,
    _2dBm,
    _5dBm
};
enum rf_divider_sel_t {
    DIV_1,
    DIV_2,
    DIV_4,
    DIV_8,
    DIV_16,
    DIV_32,
    DIV_64
};
enum feedback_sel_t {
    DIVIDED,
    FUNDAMENTAL
};
    unsigned ctrl           : 4;
    unsigned rf_output_pwr  : 2;
    unsigned rf_out_a       : 1;
    unsigned rsv0           : 3;
    unsigned rf_out_b       : 1;
    unsigned mtld           : 1;
    unsigned rsv1           : 1;
    unsigned charge_pump_bc : 8;
    unsigned rf_divider_sel : 3;
    unsigned feedback_sel   : 1;
    unsigned rsv2           : 4;
    unsigned negative_bleed : 1;
    unsigned gated_bleed    : 1;
    unsigned rsv3           : 1;
    reg_t() :
        ctrl(0x06),
        rf_output_pwr(_5dBm),
        rf_out_a(1),
        rsv0(0),
        rf_out_b(0),
        mtld(0),
        rsv1(0),
        charge_pump_bc(5),
        rf_divider_sel(DIV_1),
        feedback_sel(FUNDAMENTAL),
        rsv2(10),
        negative_bleed(1),
        gated_bleed(0),
        rsv3(0) {}
REG_DEF_E

REG_DEF_S(0x07)
enum ld_mode_t {
    FRACTIONAL_N,
    INTEGER_N
};
enum frac_n_ld_precision_t {
    _5ns,
    _6ns,
    _8ns,
    _12ns
};
enum ld_cycle_count_t {
    _1024,
    _2048,
    _4096,
    _8192
};
enum le_sync_t {
    DISABLE,
    REF_IN,
};
    unsigned ctrl                : 4;
    unsigned ld_mode             : 1;
    unsigned frac_n_ld_precision : 2;
    unsigned lol_mode            : 1;
    unsigned ld_cycle_count      : 2;
    unsigned rsv0                : 15;
    unsigned le_sync             : 1;
    unsigned rsv1                : 6;
    reg_t() :
        ctrl(0x07),
        ld_mode(FRACTIONAL_N),
        frac_n_ld_precision(_12ns),
        lol_mode(0),
        ld_cycle_count(_1024),
        rsv0(0),
        le_sync(REF_IN),
        rsv1(4) {}
REG_DEF_E

REG_DEF_S(0x08)
    unsigned rsv : 32;
    reg_t() :
        rsv(0x102d0428) {}
REG_DEF_E

REG_DEF_S(0x09)
    unsigned ctrl          : 4;
    unsigned synth_lock_to : 5;
    unsigned auto_level_to : 5;
    unsigned timeout       : 10;
    unsigned vco_band_div  : 8;
    reg_t() :
        ctrl(0x09),
        synth_lock_to(31 - 1),
        auto_level_to(31 - 1),
        timeout(200),
        vco_band_div(40) {}
REG_DEF_E

REG_DEF_S(0x0a)
    unsigned ctrl           : 4;
    unsigned adc_enable     : 1;
    unsigned adc_conversion : 1;
    unsigned adc_clock_div  : 8;
    unsigned rsv            : 18;
    reg_t() :
        ctrl(0x0a),
        adc_enable(1),
        adc_conversion(1),
        adc_clock_div(150),
        rsv(0x300) {}
REG_DEF_E

REG_DEF_S(0x0b)
    unsigned rsv : 32;
    reg_t() :
        rsv(0x0061300b) {}
REG_DEF_E

REG_DEF_S(0x0c)
    unsigned ctrl         : 4;
    unsigned rsv          : 12;
    unsigned resync_clock : 16;
    reg_t() :
        ctrl(0x0c),
        rsv(0x41),
        resync_clock(20000) {}
REG_DEF_E

void RD_API freq_formula(const freq_formula_in &in,freq_formula_out &out);
void RD_API freq_formula(const freq_formula_out &param,uint32_t freq_ref,double &freq_vco);

} // namespace ns_adf5355


// ADI AD7949
namespace ns_ad7949 {

enum ref_t {
    INTERNAL_2_5_V = 0,
    INTERNAL_4_096_V = 1,
    EXTERNAL_TEMP = 2,
    EXTERNAL_TEMP_INTERNAL = 3,
    EXTERNAL = 6,
    EXTERNAL_INTERNAL = 7
};

double ad_to_voltage_mv(int16_t ad,ref_t ref = INTERNAL_4_096_V,double ext_ref_v = 0.0);

} // namespace ns_ad7949


// MICROCHIP TC1047
namespace ns_tc1047 {

RD_INLINE double voltage_to_temp(double vol_mv)
{ return (vol_mv - 500.0) / 10.0; }

} // namespace ns_tc1047


// MICROCHIP MCP3208
namespace ns_mcp3208 {

RD_INLINE double ad_to_voltage_mv(int16_t ad)
{ return ad * 5000.0 / 4096.0; }

} // namespace ns_mcp3208


// intersil X9119
// 1024 resistor taps - 10-bit resolution
namespace ns_x9119 {

RD_INLINE uint16_t voltage_to_tap(double vol_v)
{ return uint16_t(vol_v / 5.0 * 1023.0); }

RD_INLINE double tap_to_voltage_v(uint16_t tap)
{ return 5.0 * tap / 1023.0; }

} // namespace ns_x9119

#endif // RD_UTILITIES_ALGO_CHIP_H
