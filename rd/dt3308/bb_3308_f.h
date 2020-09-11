#ifndef BB_3308_F_H
#define BB_3308_F_H

#include "libbd.h"
#include "baseband.h"
#include "../dt3308/dma_mgr.hpp"

namespace rd {

#define DT3308_BB_F bb_dt3308_f::instance()

class RD_API bb_dt3308_f : public baseband
{
public:
    typedef boost::shared_ptr<bb_dt3308_f> sptr;

    enum dl_src_t {
        ARB = 0,
        CW = 2
    };

    enum rx_filter_t {
        _2I3D,
        _3D
    };

    enum iq_cap_trig_t {
        FREE_RUN,
        _80ms,
        UL_IF_PWR,
        GPRF_IF_PWR,
        MARKER1,
        MARKER2,
        MARKER3,
        MARKER4,
        DL_GENERATOR_HEADER
    };

    enum iq_cap_src_t {
        JESD,
        FIR_FLATNESS,
        DDC,
        DEC_24576,
        DEC_12288,
        DEC_6144,
        DEC_3072,
        DEC_1536,
    };

public:
    bb_dt3308_f() {}
    virtual ~bb_dt3308_f() {}
    static bb_dt3308_f *instance();

    int32_t open_board(pci_dev *k7);
    int32_t fpga_reset();
    pci_dev* k7() { return m_k7; }
    ns_dt3308::dma_mgr* ddr() { return _ddr.get(); }

    int32_t set_dl_pwr_comp(int32_t offset);
    int32_t set_dl_dc_offset(uint16_t i,uint16_t q);
    int32_t set_dl_phase_adj(double th_i,double th_q);
    int32_t set_duc(const double freq);

    int32_t set_ul_pwr_comp(int32_t offset);
    int32_t set_ddc(const double freq);

    int32_t set_dl_en(const bool en);
    int32_t set_dl_src(const dl_src_t src);
    int32_t set_da_dc_offset_i(const uint16_t i);
    int32_t set_da_dc_offset_q(const uint16_t q);
    int32_t set_da_phase_adj_i(const double angle);
    int32_t set_da_phase_adj_q(const double angle);

    int32_t set_da_reg(uint8_t addr,uint8_t data);

    int32_t set_iq_cap_trig(const iq_cap_trig_t trig);
    int32_t set_iq_cap_src(const iq_cap_src_t src);
    static uint32_t iq_cap_iq_sr(const iq_cap_src_t src);

private:
    pci_dev *m_k7;
    ns_dt3308::dma_mgr::sptr _ddr;
};

} // namespace rd

#endif // BB_3308_F_H
