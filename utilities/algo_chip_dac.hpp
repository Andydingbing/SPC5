#ifndef RD_UTILITIES_ALGO_CHIP_DAC_H
#define RD_UTILITIES_ALGO_CHIP_DAC_H

#include "rd.h"

namespace ns_ad9122 {

RD_INLINE void iq_phase_adj_angel_to_reg(const double &angel,uint16_t &reg)
{ reg = uint16_t(angel / 1.75 * 512.0); }

}

#endif // RD_UTILITIES_ALGO_CHIP_DAC_H
