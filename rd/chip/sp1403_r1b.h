#ifndef SP1403_R1B_H
#define SP1403_R1B_H

#include "sp1403_r1a.h"

namespace rd {
namespace ns_sp1403 {
namespace r1b {
BETTER_ENUM(tx_sw2_t, uint32_t, _3000_6000, _6000_8000)
BETTER_ENUM(rx_bw_t,  uint32_t, _100M, _200M, _400M, _800M)
BETTER_ENUM(rx_sw7_t, uint32_t, _300_1000, _5100)
BETTER_ENUM(det_sw_t, uint32_t, TX, RX)

} // namespace r1b
} // namespace ns_sp1403
} // namespace rd

#endif // SP1403_R1B_H
