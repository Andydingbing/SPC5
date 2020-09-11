#ifndef SP3103_H
#define SP3103_H

#include "sp1403.h"
#include "sp1403_r1a.h"
#include "rrh.h"
#include "sp2406.h"
#include "traits.hpp"

#define SP3103_0 rd::sp3103::instance(0)

namespace rd { namespace ns_sp3103 {

static const uint8_t g_max_rf = 2;

} // namespace ns_sp3103

class RD_API sp3103 : boost::noncopyable
{
public:
    sp3103(uint32_t rfu_idx);
    static sp3103 &instance(uint32_t rfu_idx);

public:
    uint32_t rfu_idx() { return _rfu_idx; }

    vi_pci_dev *v9() { return _v9; }
    vi_pci_dev *s6() { return _s6; }

    template<typename int_t,typename data_type = traits_int<int_t>>
    sp1403 *working_sp1403(int_t rf_idx) const
    { return _sp1403.at(size_t(rf_idx)).get(); }

    template<typename int_t,typename data_type = traits_int<int_t>>
    sp1403_r1a *working_sp1403_r1a(int_t rf_idx) const
    { return _sp1403_r1a.at(size_t(rf_idx)).get(); }

    template<typename int_t,typename data_type = traits_int<int_t>>
    ns_sp9500x::rrh *working_rrh(int_t rf_idx) const
    { return _rrh.at(size_t(rf_idx)).get(); }

    template<typename int_t,typename data_type = traits_int<int_t>>
    sp2406 *working_sp2406(int_t rf_idx) const
    { return _sp2406.at(size_t(rf_idx)).get(); }

    void set_program_rfu_v9(const std::string path,const bool is_program = true)
    { _is_program_rfu_v9 = is_program; _bit_path_rfu_v9 = path; }

    void set_program_bbu_v9(const std::string path,const bool is_program = true)
    { _is_program_bbu_v9 = is_program; _bit_path_bbu_v9 = path; }

    int32_t get_ocxo(uint16_t &value);
    int32_t boot(const bool silent = false);

    int32_t program_rfu_v9() const;
    int32_t program_bbu_v9() const;

    int32_t get_rf_port(uint32_t &port) const;
    ns_sp1403::hw_ver_t get_rf_ver(uint32_t rf_idx) const;
    int32_t get_sn(char *sn);
    int32_t get_rf_sn(uint32_t rf_idx,char *sn);
    int32_t get_ver(char *ver);
    const char *get_driver_ver();

public:
    void    set_tx_en_tc(const uint32_t rf_idx,const bool en);
    int32_t set_tx_state(const uint32_t rf_idx,const bool state);
    int32_t set_tx_pwr(const uint32_t rf_idx,const double pwr);
    int32_t set_tx_freq(uint32_t rf_idx,uint64_t freq);
    int32_t get_tx_freq(uint32_t rf_idx,uint64_t &freq);
//    int32_t set_tx_bw(uint32_t rf_idx,sp1401::bw_t bw);
//    int32_t set_tx_src(uint32_t rf_idx,sp2401_r1a::da_src_t src);

    int32_t arb_load(const uint32_t rf_idx,const ns_arb::src_t &src,const std::string &arg);
    int32_t set_arb_en(const uint32_t rf_idx,const bool en) const;

    int32_t set_rx_ref(const uint32_t rf_idx,const double ref);
    int32_t set_rx_freq(uint32_t rf_idx,uint64_t freq);
    int32_t get_rx_freq(uint32_t rf_idx,uint64_t &freq);
//    int32_t rf_set_rx_bw(uint32_t rf_idx,sp1401::bw_t bw);

    int32_t set_io_mode(uint32_t rf_idx,io_mode_t mode);
    int32_t get_temp(uint32_t rf_idx,double &tx_temp,double &rx_temp);

private:
    int32_t instance_sp1403(uint32_t rf_idx);

    static void freq_to_sub8_rrh(const uint64_t freq,uint64_t &sub8,uint64_t &rrh);

private:
    uint32_t _rfu_idx;

    vi_pci_dev *_v9;
    vi_pci_dev *_s6;

    std::vector<sp1403::sptr> _sp1403;
    std::vector<sp1403_r1a::sptr> _sp1403_r1a;
    std::vector<ns_sp9500x::rrh::sptr> _rrh;
    std::vector<sp2406::sptr> _sp2406;

    double m_tx_pwr[ns_sp3103::g_max_rf];
    double m_ref[ns_sp3103::g_max_rf];
    bool _is_program_rfu_v9;
    bool _is_program_bbu_v9;
    std::string _bit_path_rfu_v9;
    std::string _bit_path_bbu_v9;
};

} //namespace rd

#endif // SP3103_H
