#ifndef SP9500X_SP3103_H
#define SP9500X_SP3103_H

#include "sp3103.h"
#include "sp9500x_sp1403_r1a.h"
#include "sp9500x_sp1403_r1b.h"
#include "rrh.h"
#include "sp2406.h"
#include "traits.hpp"

#define SP3103_0 rd::ns_sp9500x::sp3103::instance(0)

namespace rd {
namespace ns_sp9500x {

class RD_API sp3103 : public rd::sp3103
{
public:
    sp3103(uint32_t rfu_idx);
    static sp3103 &instance(uint32_t rfu_idx);

public:
    template<typename int_t,typename data_type = traits_int<int_t>>
    sp1403 *working_sp1403(int_t rf_idx) const
    { return _sp1403.at(size_t(rf_idx)).get(); }

    template<typename int_t,typename data_type = traits_int<int_t>>
    sp1403_r1a *working_sp1403_r1a(int_t rf_idx) const
    { return _sp1403_r1a.at(size_t(rf_idx)).get(); }

    template<typename int_t,typename traits_t = traits_int<int_t>>
    sp1403_r1b *working_sp1403_r1b(int_t rf_idx) const
    { return _sp1403_r1b.at(size_t(rf_idx)).get(); }

    template<typename int_t,typename data_type = traits_int<int_t>>
    ns_sp9500x::rrh *working_rrh(int_t rf_idx) const
    { return _rrh.at(size_t(rf_idx)).get(); }

    template<typename int_t,typename data_type = traits_int<int_t>>
    sp2406 *working_sp2406(int_t rf_idx) const
    { return _sp2406.at(size_t(rf_idx)).get(); }

    int32_t boot(const bool silent = false) OVERRIDE;

    int32_t program_rfu_v9() const OVERRIDE;
    int32_t program_bbu_v9() const OVERRIDE;

    ns_sp1403::hw_ver_t get_rf_ver(uint32_t rf_idx) const OVERRIDE;

public:
    int32_t set_tx_en_tc(const uint32_t rf_idx,const bool en) OVERRIDE;
    int32_t set_tx_state(const uint32_t rf_idx,const bool state) OVERRIDE;
    int32_t set_tx_pwr(const uint32_t rf_idx,const double pwr) OVERRIDE;
    int32_t set_tx_freq(uint32_t rf_idx,uint64_t freq) OVERRIDE;
    int32_t get_tx_freq(uint32_t rf_idx,uint64_t &freq) OVERRIDE;

    int32_t arb_load(const uint32_t rf_idx,const ns_arb::src_t &src,const std::string &arg) OVERRIDE;
    int32_t set_arb_en(const uint32_t rf_idx,const bool en) const OVERRIDE;

    int32_t set_rx_ref(const uint32_t rf_idx,const double ref) OVERRIDE;
    int32_t set_rx_freq(uint32_t rf_idx,uint64_t freq) OVERRIDE;
    int32_t get_rx_freq(uint32_t rf_idx,uint64_t &freq) OVERRIDE;

    int32_t set_io_mode(uint32_t rf_idx,io_mode_t mode) OVERRIDE;

private:
    int32_t instance_sp1403(uint32_t rf_idx) OVERRIDE;

private:
    std::vector<ns_sp9500x::sp1403::sptr> _sp1403;
    std::vector<ns_sp9500x::sp1403_r1a::sptr> _sp1403_r1a;
    std::vector<ns_sp9500x::sp1403_r1b::sptr> _sp1403_r1b;
    std::vector<ns_sp9500x::rrh::sptr> _rrh;
    std::vector<ns_sp9500x::sp2406::sptr> _sp2406;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // SP9500X_SP3103_H
