#ifndef SP1401_R1F_H
#define SP1401_R1F_H

#include "sp1401_r1e.h"
#include "sleep_common.h"

namespace sp_rd {

class RD_API sp1401_r1f : public sp1401_r1e
{
public:
    typedef boost::shared_ptr<sp1401_r1f> sptr;
    sp1401_r1f(uint32_t rf_idx,uint32_t rfu_idx) :
        sp1401_r1e(rf_idx,rfu_idx) {}

    int32_t open_board();
    int32_t set_adf5355(lo_t lo,uint64_t freq);

    int32_t tx_freq2lo(
            uint64_t freq,
            uint64_t &lo1,
            uint64_t &lo2,
            uint64_t &lo3,
            tx_filter_t &filter1,
            tx_filter_t &filter2);

    int32_t set_rx_att2(double att) { return set_rx_att3(att); }

    int32_t set_rx_att(double att1,double att2,double att3)
    {
        boost::ignore_unused(att3);
        INT_CHECK(set_rx_att1(att1));
        INT_CHECK(set_rx_att2(att2));
        return 0;
    }

    int32_t get_temp(uint32_t idx,double &temp);

    void tx_att_states(std::list<sp1401::common_chain_pwr_state_t> &states);
    void rx_att_states(std::list<sp1401::common_chain_pwr_state_t> &states);
};

} // namespace sp_rd

#endif // SP1401_R1F_H
