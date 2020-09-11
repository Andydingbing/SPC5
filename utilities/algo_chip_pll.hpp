#ifndef RD_UTILITIES_ALGO_CHIP_PLL_H
#define RD_UTILITIES_ALGO_CHIP_PLL_H

#include "rd.h"

namespace ns_adf5355 {

struct freq_formula_in {
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

void freq_formula(const freq_formula_in &in,freq_formula_out &out);
void freq_formula(const freq_formula_out &param,uint32_t freq_ref,double &freq_vco);

} // namespace ns_adf5355


namespace ns_hmc83x {

struct freq_formula_in {
    uint64_t freq_vco;
    uint32_t freq_ref;
    uint32_t r_div;
    freq_formula_in();
};

struct freq_formula_out {
    uint32_t _int;
    uint32_t _frac;
};

void freq_formula(const freq_formula_in &in,freq_formula_out &out);
void freq_formula(const freq_formula_out &param,uint32_t freq_pd,double &freq_vco);

} // namespace ns_hmc83x


namespace ns_lmx2594 {

struct freq_formula_in {
    uint64_t freq_pd;
    uint64_t freq_rf;
    freq_formula_in();
};

struct freq_formula_out {
    uint16_t DIV; // CHDIV[4:0]
    uint16_t N;
    uint32_t NUM;
    uint32_t DEN;
};

void freq_formula(const freq_formula_in &in,freq_formula_out &out);

RD_INLINE uint32_t mash_seed(uint16_t reg0x29,uint16_t reg0x28)
{ return uint32_t(reg0x29) | (uint32_t(reg0x28) << 16); }

} // namespace ns_lmx2594

#endif // RD_UTILITIES_ALGO_CHIP_PLL_H
