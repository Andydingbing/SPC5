#ifndef SP1403_R1B_H
#define SP1403_R1B_H

#include "sp1403_r1a.h"

namespace rd {
namespace ns_sp1403 {
namespace r1b {
BETTER_ENUM(rx_bw_t, uint32_t,
            _100M,
            _200M,
            _400M,
            _800M)

BETTER_ENUM(rx_sw7_t, uint32_t,
            _300_1000,
            _5100)
} // namespace r1b
} // namespace ns_sp1403

namespace ns_sp9500x {

class RD_API sp1403_r1b : public sp1403_r1a
{
public:
    typedef boost::shared_ptr<sp1403_r1b> sptr;

    typedef ns_sp1403::r1b::rx_bw_t rx_bw_t;
    typedef ns_sp1403::r1b::rx_sw7_t rx_sw7_t;

    sp1403_r1b(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~sp1403_r1b() {}

public:
    int32_t open_board();
    int32_t close_board();

    int32_t hw_ver() const { return ns_sp1403::hw_ver_t::R1A; }

    int32_t set_rx_freq(const uint64_t freq);

    int32_t set_rx_bw(const rx_bw_t bw) const;
    int32_t get_rx_bw(rx_bw_t &bw) const;

    DECL_SW(rx_sw7,rx_sw7_t)

public:
    void rx_freq_to_lo(const uint64_t freq);

private:
    common_lo_t *_tx_lmx2594_0;
    common_lo_t *_tx_lmx2594_1;
    common_lo_t *_rx_lmx2594_0;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // SP1403_R1A_H
