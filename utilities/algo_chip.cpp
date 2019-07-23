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

#include "algo_chip.h"
#include "algo_math.h"
#include <boost/rational.hpp>

using namespace boost;

ns_adf5355::freq_formula_in::freq_formula_in()
{
    freq_ref = 100000000;
    freq_chsp = 6250;
    ref_doubler = false;
    ref_divider = false;
    r_counter = 1;
}

/*
 *                           FRAC2
 *                  FRAC1 + -------
 *                           MOD2
 * RF(out) = INT + ----------------- * f(PFD) / RF Divider
 *                       MOD1
 * RF(out) : the RF output frequency
 * INT     : the integer division factor
 * FRAC1   : the fractionality
 * FRAC2   : thr auxiliary fractionality
 * MOD1    : the fixed 24-bit modulus
 * MOD2    : the auxiliary modulus
 * RF Divider : the output divider that divides down the VCO frequency
 *
 * f(PFD) = REF(in) * ((1 + D) / (R * (1 + T)))
 * REF(in) : the reference frequency input
 * D       : the REF(in) doubler bit
 * R       : the REF(in) division factor
 * T       : the REF(in) divide by 2 bit(0 or 1)
 *
 */
void ns_adf5355::freq_formula(const freq_formula_in &in,freq_formula_out &out)
{
    double freq_pfd = double(in.freq_ref);
    freq_pfd *= (1.0 + in.ref_doubler ? 1.0 : 0.0);
    freq_pfd /= (1.0 + in.ref_divider ? 1.0 : 0.0);
    freq_pfd /= in.r_counter;

    out._int = uint32_t(in.freq_vco / uint32_t(freq_pfd));
    out._mod1 = 16777216;
    out._frac1 = uint32_t((in.freq_vco % uint32_t(freq_pfd)) / freq_pfd * out._mod1);

    double _mod2_bfr_reduction = freq_pfd / gcd(uint32_t(freq_pfd),in.freq_chsp);

    double _frac2_bfr_reduction = double(in.freq_vco % uint32_t(freq_pfd));
    _frac2_bfr_reduction = _frac2_bfr_reduction / freq_pfd * out._mod1 - out._frac1;
    _frac2_bfr_reduction = _frac2_bfr_reduction * _mod2_bfr_reduction;
    _frac2_bfr_reduction = round(_frac2_bfr_reduction,2);

    uint32_t gcd_mod2_frac2 = gcd(uint32_t(_mod2_bfr_reduction),uint32_t(_frac2_bfr_reduction));
    out._mod2  = uint32_t(_mod2_bfr_reduction)  / gcd_mod2_frac2;
    out._frac2 = uint32_t(_frac2_bfr_reduction) / gcd_mod2_frac2;
}

void ns_adf5355::freq_formula(const freq_formula_out &param,uint32_t freq_ref,double &freq_vco)
{
    freq_vco = double(param._frac2) / double(param._mod2);
    freq_vco += double(param._frac1);
    freq_vco /= double(param._mod1);
    freq_vco += double(param._int);
    freq_vco *= double(freq_ref);
    freq_vco = round(freq_vco,5);
}

double ns_ad7949::ad_to_voltage_mv(int16_t ad,ns_ad7949::ref_t ref,double ext_ref_v)
{
    if (ref == ns_ad7949::INTERNAL_2_5_V) {
        return ad / 16383.0 * 2500;
    } else if (ref == ns_ad7949::INTERNAL_4_096_V) {
        return ad / 16383.0 * 4096;
    }

    return ad / 16383.0 * ext_ref_v * 1000.0;
}
