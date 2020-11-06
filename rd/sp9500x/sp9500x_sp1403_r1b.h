#ifndef SP9500X_SP1403_R1B_H
#define SP9500X_SP1403_R1B_H

#include "sp1403_r1b.h"
#include "sp9500x_sp1403_r1a.h"

namespace rd {
namespace ns_sp9500x {

#ifdef RD_C_MSC
    #pragma warning( push )
    #pragma warning( disable : 4250 )
#endif

class RD_API sp1403_r1b : virtual public rd::sp1403_r1b, public ns_sp9500x::sp1403_r1a
{
public:
    typedef boost::shared_ptr<sp1403_r1b> sptr;

    typedef ns_sp1403::lo_t          lo_t;
    typedef ns_sp1403::r1a::att_t    att_t;
    typedef ns_sp1403::r1b::tx_sw2_t tx_sw2_t;
    typedef ns_sp1403::r1b::rx_bw_t  rx_bw_t;
    typedef ns_sp1403::r1b::rx_sw7_t rx_sw7_t;
    typedef ns_sp1403::r1b::det_sw_t det_sw_t;

    sp1403_r1b(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~sp1403_r1b() {}

public:
    int32_t open_board();
    int32_t close_board();

    int32_t hw_ver() const { return ns_sp1403::hw_ver_t::R1A; }

    int32_t set_io_mode(const ns_sp1403::port_t port,const io_mode_t mode);

    int32_t set_tx_freq(const uint64_t freq);
    int32_t set_rx_freq(const uint64_t freq);

    int32_t set_rx_bw(const rx_bw_t bw) const;
    int32_t get_rx_bw(rx_bw_t &bw) const;

    DECL_SW(tx0_sw2,tx_sw2_t)
    DECL_SW(tx1_sw2,tx_sw2_t)
    DECL_SW(rx_sw7,rx_sw7_t)

    int32_t set_det_sw(const det_sw_t &sw) const;
    int32_t get_det_sw(det_sw_t &sw) const;
    int32_t get_ad7680(uint16_t &det) const;

public:
    void tx_freq_to_lo(const uint64_t freq);
    void rx_freq_to_lo(const uint64_t freq);
};

#ifdef RD_C_MSC
    #pragma warning( pop )
#endif

} // namespace ns_sp9500x
} // namespace rd

#endif // SP9500X_SP1403_R1B_H
