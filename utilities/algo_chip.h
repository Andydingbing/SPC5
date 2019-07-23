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
RD_INLINE double tc1047_voltage_to_temp(double vol_mv)
{ return (vol_mv - 500.0) / 10.0; }

// MICROCHIP MCP3208
RD_INLINE double mcp3208_ad_to_voltage_mv(int16_t ad)
{ return ad * 5000.0 / 4096.0; }

// intersil X9119
// 1024 resistor taps - 10-bit resolution
RD_INLINE uint16_t x9119_voltage_to_tap(double vol_v)
{ return uint16_t(vol_v / 5.0 * 1023.0); }

RD_INLINE double x9119_tap_to_voltage_v(uint16_t tap)
{ return 5.0 * tap / 1023.0; }

#endif // RD_UTILITIES_ALGO_CHIP_H
