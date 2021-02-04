#ifndef SP3103_H
#define SP3103_H

#include "sp1403.h"
#include "pci_dev_vi.h"
#include "arb_reader.h"

namespace rd { namespace ns_sp3103 {

static const uint8_t g_max_rf = 2;

} // namespace ns_sp3103

class RD_API sp3103 : boost::noncopyable
{
public:
    sp3103(uint32_t rfu_idx);
    virtual ~sp3103();

public:
    uint32_t rfu_idx() { return _rfu_idx; }

    vi_pci_dev *v9() { return _v9; }
    vi_pci_dev *s6() { return _s6; }

    void set_program_rfu_v9(const std::string path,const bool is_program = true)
    { _is_program_rfu_v9 = is_program; _bit_path_rfu_v9 = path; }

    void set_program_bbu_v9(const std::string path,const bool is_program = true)
    { _is_program_bbu_v9 = is_program; _bit_path_bbu_v9 = path; }

    virtual int32_t boot(const bool silent = false) = 0;

    virtual int32_t program_rfu_v9() const = 0;
    virtual int32_t program_bbu_v9() const = 0;

    virtual ns_sp1403::hw_ver_t get_rf_ver(uint32_t rf_idx) const = 0;

    virtual int32_t set_tx_en_tc(const uint32_t rf_idx,const bool en) = 0;
    virtual int32_t set_tx_state(const uint32_t rf_idx,const bool state) = 0;
    virtual int32_t set_tx_pwr(const uint32_t rf_idx,const double pwr) = 0;
    virtual int32_t set_tx_freq(uint32_t rf_idx,uint64_t freq) = 0;
    virtual int32_t get_tx_freq(uint32_t rf_idx,uint64_t &freq) = 0;

    virtual int32_t arb_load(const uint32_t rf_idx,const ns_arb::src_t &src,const std::string &arg) = 0;
    virtual int32_t set_arb_en(const uint32_t rf_idx,const bool en) const = 0;

    virtual int32_t set_rx_ref(const uint32_t rf_idx,const double ref) = 0;
    virtual int32_t set_rx_freq(uint32_t rf_idx,uint64_t freq) = 0;
    virtual int32_t get_rx_freq(uint32_t rf_idx,uint64_t &freq) = 0;

    virtual int32_t set_io_mode(uint32_t rf_idx,io_mode_t mode) = 0;

    static void freq_to_sub8_rrh(const uint64_t freq,uint64_t &sub8,uint64_t &rrh);

protected:
    virtual int32_t instance_sp1403(uint32_t rf_idx) = 0;

protected:
    uint32_t _rfu_idx;

    vi_pci_dev *_v9;
    vi_pci_dev *_s6;

    double _tx_pwr[ns_sp3103::g_max_rf];
    double _rx_ref[ns_sp3103::g_max_rf];
    bool _is_program_rfu_v9;
    bool _is_program_bbu_v9;
    std::string _bit_path_rfu_v9;
    std::string _bit_path_bbu_v9;
};

} // namespace rd

#endif // SP3103_H
