#ifndef SP2406_H
#define SP2406_H

#include "sp1403.h"
#include "traits.hpp"
#include "libbd.h"
#include "baseband.h"
#include "enum.h"
#include "complex_sequence.h"
#include "../sp9500x/dma_mgr.hpp"
#include "arb_reader.h"
#include <bitset>
#include <boost/smart_ptr.hpp>

#define DECL_CPRI_MAP(link,channel) \
    int32_t set_##link##_cpri_map_ch##channel(const uint8_t cpri) const; \
    int32_t get_##link##_cpri_map_ch##channel(uint8_t &cpri) const;

#define DECL_DL_CPRI_SR(channel) \
    int32_t set_dl_cpri_sr_ch##channel(const dl_cpri_sr_t &sr) const; \
    int32_t get_dl_cpri_sr_ch##channel(dl_cpri_sr_t &sr) const;

#define DECL_DL_SRC(channel) \
    int32_t set_dl_src_ch##channel(const dl_src_t &src) const; \
    int32_t get_dl_src_ch##channel(dl_src_t &src) const;

#define DECL_GET_LPF_COEF(link,channel) \
    int32_t get_##link##_lpf_coef_ch##channel(std::vector<int16_t> &coef) const;

#define DECL_DDS(link,dds,channel) \
    int32_t set_##link##_##dds##_ch##channel(const double freq) const; \
    int32_t get_##link##_##dds##_ch##channel(double &freq) const;

namespace rd { namespace ns_sp2406 {

BETTER_ENUM(dl_cpri_sr_t, uint8_t,
            _12288 = 1,
            _24576 = 2,
            _49152 = 4,
            _98304 = 8)

BETTER_ENUM(dl_src_t, uint8_t,
            CPRI = 0,
            Const = 1,
            DDS0 = 2,
            DDS1 = 3,
            ARB = 4,
            ARB_Bypass = 5,
            BRAM = 8,
            Disable = 15)

BETTER_ENUM(dl_jesd_src_t, uint8_t,
            DUC,
            Const,
            DDS,
            DDS_Gain)

BETTER_ENUM(dl_jesd_src_dds_mode_t, uint8_t,
            Auto,
            Coarse,
            Fine)

BETTER_ENUM(dl_jesd_src_dds_auto_period_t, uint8_t,
            _1000ms,
            _500ms,
            _200ms,
            _100ms,
            _10ms,
            _1ms)

BETTER_ENUM(dl_jesd_src_dds_auto_interval_t, uint8_t,
            _1MHz,
            _2MHz,
            _5MHz,
            _10MHz,
            _20MHz,
            _40MHz)

BETTER_ENUM(arb_sr_t, uint32_t,
            _8I_98304 = 1,
            _4I_49152 = 2,
            _2I_24576 = 4,
            _1I_12288 = 8)

BETTER_ENUM(arb_repetition_t, uint32_t,
            Single_Shot,
            Continuous)

BETTER_ENUM(arb_trig_src_t, uint32_t,
            AutoStart,
            Manual,
            _10ms,
            ExternTrigger3,
            ExternTrigger4)

BETTER_ENUM(arb_trig_edge_t, uint32_t,
            Posedge,
            Negedge)

BETTER_ENUM(arb_multi_seg_repetition_t, uint32_t,
            Auto = 1,
            Continuous,
            Continuous_Seamless)

BETTER_ENUM(iq_cap_src_t, uint8_t,
            DDC,
            DL_CPRI,
            DUC_Ch0,
            ARB,
            CW,
            Increment,
            DUC_Ch1,
            BRAM = 8)

BETTER_ENUM(iq_cap_src_ddc_t, uint8_t,
            LPF,
            HBF2,
            HBF1,
            HBF0,
            CF,
            DGC,
            DDC = 8)

BETTER_ENUM(iq_cap_src_duc_t, uint8_t,
            LPF = 1,
            HBF0,
            HBF1,
            DDS0,
            HBF2,
            DDS1,
            CF,
            DGC)

BETTER_ENUM(iq_cap_sr_t, uint32_t,
            _8I_98304 = 1,
            _4I_49152 = 2,
            _2I_24576 = 4,
            _1I_12288 = 8)

BETTER_ENUM(iq_cap_mode_t, uint32_t,
            Single,
            Continue)

BETTER_ENUM(iq_cap_trig_src_t, uint32_t,
            RestartMarker = 0x09,
            Marker1 = 0x0a,
            Marker2 = 0x0b,
            Marker3 = 0x0c,
            Marker4 = 0x0d,
            ExternTrigger = 0x11,
            ExternTrigger1 = 0x12,
            ExternTrigger2 = 0x13,
            ExternTrigger3 = 0x14,
            ExternTrigger4 = 0x15,
            IF_Power = 0x21,
            NextFrame = 0x40,
            Frame = 0x41,
            ModXY = 0x42,
            FreeRun = 0x43)

BETTER_ENUM(iq_cap_trig_edge_t, uint32_t,
            Posedge,
            Negedge)

} // namespace ns_sp2406

class RD_API sp2406 : public baseband
{
public:
    typedef boost::shared_ptr<sp2406> sptr;

    typedef std::bitset<8> bitset_dl_cpri;
    typedef std::bitset<8> bitset_dl_lpf;
    typedef std::bitset<8> bitset_dl_hbf0;
    typedef std::bitset<8> bitset_dl_hbf1;

    typedef std::bitset<8> bitset_ul_hbf1;
    typedef std::bitset<8> bitset_ul_hbf2;
    typedef std::bitset<8> bitset_ul_lpf;

    typedef ns_sp1403::path_t path_t;
    typedef ns_sp2406::dl_cpri_sr_t dl_cpri_sr_t;
    typedef ns_sp2406::dl_src_t dl_src_t;
    typedef ns_sp2406::dl_jesd_src_t dl_jesd_src_t;
    typedef ns_sp2406::dl_jesd_src_dds_mode_t dl_jesd_src_dds_mode_t;
    typedef ns_sp2406::dl_jesd_src_dds_auto_period_t dl_jesd_src_dds_auto_period_t;
    typedef ns_sp2406::dl_jesd_src_dds_auto_interval_t dl_jesd_src_dds_auto_interval_t;

    typedef ns_sp2406::arb_repetition_t arb_repetition_t;
    typedef ns_sp2406::arb_trig_src_t   arb_trig_src_t;
    typedef ns_sp2406::arb_trig_edge_t  arb_trig_edge_t;
    typedef ns_sp2406::arb_multi_seg_repetition_t arb_multi_seg_repetition_t;

    typedef ns_sp2406::iq_cap_src_t       iq_cap_src_t;
    typedef ns_sp2406::iq_cap_src_ddc_t   iq_cap_src_ddc_t;
    typedef ns_sp2406::iq_cap_src_duc_t   iq_cap_src_duc_t;
    typedef ns_sp2406::iq_cap_sr_t        iq_cap_sr_t;
    typedef ns_sp2406::iq_cap_mode_t      iq_cap_mode_t;
    typedef ns_sp2406::iq_cap_trig_src_t  iq_cap_trig_src_t;
    typedef ns_sp2406::iq_cap_trig_edge_t iq_cap_trig_edge_t;

    enum v9_t { BBU = 1, RFU };

    sp2406(uint32_t rf_idx);
    virtual ~sp2406() {}

public:
    bool connect(const std::list<pci_dev *> &ctrller);

    int32_t open_board();
    int32_t close_board();

    int32_t get_v9_ver(uint32_t &ver);
    int32_t get_s6_ver(uint32_t &ver);

    pci_dev* v9() { return _v9; }
    pci_dev* s6() { return _s6; }

    uint32_t rf_idx() const { return _rf_idx; }

    ns_sp9500x::dma_mgr *ddr() { return &_dma_mgr; }
    arb_mgr *arb() { return &_arb; }

    const complex_sequence *ul_sequence() const { return &_ul_sequence; }
    complex_sequence *ul_sequence() { return &_ul_sequence; }

public:
    int32_t jesd_reset() const;
    int32_t jesd_status(bool &status) const;
    int32_t set_da_freq(double freq);
    int32_t set_ad_freq(double freq);

public:
    int32_t set_dl_pwr_comp(int32_t offset) { return 0; }
    int32_t set_dl_pwr_comp(double offset);
    int32_t get_dl_pwr_comp(double &offset);
    int32_t set_dl_dc_offset(uint16_t i,uint16_t q);
    int32_t set_dl_phase_adj(double,double) { return 0; }
    int32_t set_duc(const double);

    int32_t set_dl_cpri_map(const uint8_t ch,const uint8_t cpri) const;
    int32_t get_dl_cpri_map(const uint8_t ch,uint8_t &cpri) const;

    int32_t set_dl_cpri_sr(const uint8_t ch,const dl_cpri_sr_t &sr) const;
    int32_t get_dl_cpri_sr(const uint8_t ch,dl_cpri_sr_t &sr) const;

    int32_t set_dl_src(const uint8_t ch,const dl_src_t &src) const;
    int32_t get_dl_src(const uint8_t ch,dl_src_t &src) const;

    int32_t set_dl_dds0(const uint8_t ch,const double freq) const;
    int32_t get_dl_dds0(const uint8_t ch,double &freq) const;

    int32_t set_dl_dds1(const uint8_t ch,const double freq) const
    { return ch < 4 ? set_dl_dds1_ch0_3(freq) : set_dl_dds1_ch4_7(freq); }

    int32_t get_dl_dds1(const uint8_t ch,double &freq) const
    { return ch < 4 ? get_dl_dds1_ch0_3(freq) : get_dl_dds1_ch4_7(freq); }

    int32_t set_dl_dds1_ch0_3(const double freq) const;
    int32_t get_dl_dds1_ch0_3(double &freq) const;
    int32_t set_dl_dds1_ch4_7(const double freq) const;
    int32_t get_dl_dds1_ch4_7(double &freq) const;

    DECL_CPRI_MAP(dl,0)
    DECL_CPRI_MAP(dl,1)
    DECL_CPRI_MAP(dl,2)
    DECL_CPRI_MAP(dl,3)
    DECL_CPRI_MAP(dl,4)
    DECL_CPRI_MAP(dl,5)
    DECL_CPRI_MAP(dl,6)
    DECL_CPRI_MAP(dl,7)

    DECL_DL_CPRI_SR(0)
    DECL_DL_CPRI_SR(1)
    DECL_DL_CPRI_SR(2)
    DECL_DL_CPRI_SR(3)
    DECL_DL_CPRI_SR(4)
    DECL_DL_CPRI_SR(5)
    DECL_DL_CPRI_SR(6)
    DECL_DL_CPRI_SR(7)

    DECL_DL_SRC(0)
    DECL_DL_SRC(1)
    DECL_DL_SRC(2)
    DECL_DL_SRC(3)
    DECL_DL_SRC(4)
    DECL_DL_SRC(5)
    DECL_DL_SRC(6)
    DECL_DL_SRC(7)

    DECL_DDS(dl,dds0,0)
    DECL_DDS(dl,dds0,1)
    DECL_DDS(dl,dds0,2)
    DECL_DDS(dl,dds0,3)
    DECL_DDS(dl,dds0,4)
    DECL_DDS(dl,dds0,5)
    DECL_DDS(dl,dds0,6)
    DECL_DDS(dl,dds0,7)

    int32_t set_dl_src_const(const int16_t i,const int16_t q) const;
    int32_t get_dl_src_const(int16_t &i,int16_t &q) const;

    int32_t set_dl_src_dds0_freq(const double freq) const;
    int32_t get_dl_src_dds0_freq(double &freq) const;
    int32_t set_dl_src_dds0_gain(const double gain) const;
    int32_t get_dl_src_dds0_gain(double &gain) const;

    int32_t set_dl_lpf_en(const uint8_t ch,const bool en) const;
    int32_t set_dl_lpf_en(const bitset_dl_lpf &en) const;
    int32_t set_dl_lpf_en(const bool en) const;
    int32_t get_dl_lpf_en(const uint8_t ch,bool &en) const;
    int32_t get_dl_lpf_en(bitset_dl_lpf &en) const;

    int32_t set_dl_lpf_coef(const uint8_t ch,const std::vector<int16_t> &coef) const;
    int32_t get_dl_lpf_coef(const uint8_t ch,std::vector<int16_t> &coef) const;

    DECL_GET_LPF_COEF(dl,0)
    DECL_GET_LPF_COEF(dl,1)
    DECL_GET_LPF_COEF(dl,2)
    DECL_GET_LPF_COEF(dl,3)
    DECL_GET_LPF_COEF(dl,4)
    DECL_GET_LPF_COEF(dl,5)
    DECL_GET_LPF_COEF(dl,6)
    DECL_GET_LPF_COEF(dl,7)

    int32_t set_dl_hbf0_en(const uint8_t ch,const bool en) const;
    int32_t set_dl_hbf0_en(const bitset_dl_hbf0 &en) const;
    int32_t set_dl_hbf0_en(const bool en) const;
    int32_t get_dl_hbf0_en(const uint8_t ch,bool &en) const;
    int32_t get_dl_hbf0_en(bitset_dl_hbf0 &en) const;

    int32_t set_dl_hbf1_en(const uint8_t ch,const bool en) const;
    int32_t set_dl_hbf1_en(const bitset_dl_hbf1 &en) const;
    int32_t set_dl_hbf1_en(const bool en) const;
    int32_t get_dl_hbf1_en(const uint8_t ch,bool &en) const;
    int32_t get_dl_hbf1_en(bitset_dl_hbf1 &en) const;

    int32_t set_dl_rc_en(const bool en) const;
    int32_t get_dl_rc_en(bool &en) const;

    int32_t set_dl_cf_en(const bool en) const;
    int32_t get_dl_cf_en(bool &en) const;

    int32_t set_dl_cf_coef(const std::vector<iq_data_t> &coef) const;
    int32_t set_dl_cf_coef(const std::vector<int16_t> &real,const std::vector<int16_t> &imag) const;
    int32_t get_dl_cf_coef(std::vector<iq_data_t> &coef) const;
    int32_t get_dl_cf_coef(std::vector<int16_t> &i,std::vector<int16_t> &q) const;

    int32_t set_dl_jesd_src(const path_t path,const dl_jesd_src_t &src) const;
    int32_t get_dl_jesd_src(const path_t path,dl_jesd_src_t &src) const;

    int32_t set_dl_jesd_src_const(const int16_t i,const int16_t q) const;
    int32_t get_dl_jesd_src_const(int16_t &i,int16_t &q) const;

    int32_t set_dl_jesd_src_dds(const dl_jesd_src_dds_mode_t &mode) const;
    int32_t set_dl_jesd_src_dds(const dl_jesd_src_dds_auto_period_t &period) const;
    int32_t set_dl_jesd_src_dds(const dl_jesd_src_dds_auto_interval_t &interval) const;
    int32_t set_dl_jesd_src_dds_coarse(const int16_t freq_mhz) const;
    int32_t set_dl_jesd_src_dds_fine(const double freq) const;
    int32_t set_dl_jesd_src_dds_gain(const double gain) const;
    int32_t get_dl_jesd_src_dds(dl_jesd_src_dds_mode_t &mode) const;
    int32_t get_dl_jesd_src_dds(dl_jesd_src_dds_auto_period_t &period) const;
    int32_t get_dl_jesd_src_dds(dl_jesd_src_dds_auto_interval_t &interval) const;
    int32_t get_dl_jesd_src_dds_coarse(int16_t &freq_mhz) const;
    int32_t get_dl_jesd_src_dds_fine(double &freq) const;
    int32_t get_dl_jesd_src_dds_gain(double &gain) const;

    int32_t set_ul_pwr_comp(int32_t);
    int32_t set_ul_pwr_comp(const double offset);
    int32_t get_ul_pwr_comp(double &offset);

    int32_t set_ddc(const double);

public:
    int32_t set_arb_start() const;
    int32_t set_arb_abort() const;

    int32_t set_arb_segs(const uint8_t segs) const;

    int32_t set_arb_repetition(const arb_repetition_t rep) const;
    int32_t get_arb_repetition(arb_repetition_t &rep) const;

    int32_t set_arb_cycles(const uint16_t cycle) const;
    int32_t get_arb_cycles(uint16_t &cycle) const;

    int32_t set_arb_additional_samples(const uint32_t samples) const;
    int32_t get_arb_additional_samples(uint32_t &samples) const;

    int32_t set_arb_trig_src(const arb_trig_src_t src) const;
    int32_t get_arb_trig_src(arb_trig_src_t &src) const;

    int32_t set_arb_trig_edge(const arb_trig_edge_t edge) const;
    int32_t get_arb_trig_edge(arb_trig_edge_t &edge) const;

    int32_t set_arb_manual_trig() const;

    int32_t set_arb_retrig(const bool en) const;
    int32_t get_arb_retrig(bool &en) const;

    int32_t set_arb_multi_seg_repetition(const arb_multi_seg_repetition_t rep) const;
    int32_t get_arb_multi_seg_repetition(arb_multi_seg_repetition_t &rep) const;

    int32_t set_arb_multi_seg_next_seg(const uint8_t n) const;
    int32_t get_arb_multi_seg_next_seg(uint8_t &n) const;

    int32_t set_arb_multi_seg_manual_trig() const;

public:
    int32_t get_ul_pwr_manual(double &pwr,const uint32_t samples = 2048);
    int32_t get_ul_pwr_fpga(double &pwr,const uint32_t samples = 2048) const;

    int32_t set_ul_cf_en(const bool en) const;
    int32_t get_ul_cf_en(bool &en) const;

    int32_t set_ul_cf_coef(const std::vector<iq_data_t> &coef) const;
    int32_t set_ul_cf_coef(const std::vector<int16_t> &real,const std::vector<int16_t> &imag) const;
    int32_t get_ul_cf_coef(std::vector<iq_data_t> &coef) const;
    int32_t get_ul_cf_coef(std::vector<int16_t> &i,std::vector<int16_t> &q) const;

    int32_t set_ul_hbf1_en(const uint8_t ch,const bool en) const;
    int32_t set_ul_hbf1_en(const bitset_ul_hbf1 &en) const;
    int32_t set_ul_hbf1_en(const bool en) const;
    int32_t get_ul_hbf1_en(const uint8_t ch,bool &en) const;
    int32_t get_ul_hbf1_en(bitset_ul_hbf1 &en) const;

    int32_t set_ul_hbf2_en(const uint8_t ch,const bool en) const;
    int32_t set_ul_hbf2_en(const bitset_ul_hbf2 &en) const;
    int32_t set_ul_hbf2_en(const bool en) const;
    int32_t get_ul_hbf2_en(const uint8_t ch,bool &en) const;
    int32_t get_ul_hbf2_en(bitset_ul_hbf2 &en) const;

    int32_t set_ul_lpf_en(const uint8_t ch,const bool en) const;
    int32_t set_ul_lpf_en(const bitset_ul_lpf &en) const;
    int32_t set_ul_lpf_en(const bool en) const;
    int32_t get_ul_lpf_en(const uint8_t ch,bool &en) const;
    int32_t get_ul_lpf_en(bitset_ul_lpf &en) const;

    int32_t set_ul_lpf_coef(const uint8_t ch,const std::vector<int16_t> &coef) const;
    int32_t get_ul_lpf_coef(const uint8_t ch,std::vector<int16_t> &coef) const;

    DECL_GET_LPF_COEF(ul,0)
    DECL_GET_LPF_COEF(ul,1)
    DECL_GET_LPF_COEF(ul,2)
    DECL_GET_LPF_COEF(ul,3)
    DECL_GET_LPF_COEF(ul,4)
    DECL_GET_LPF_COEF(ul,5)
    DECL_GET_LPF_COEF(ul,6)
    DECL_GET_LPF_COEF(ul,7)

    int32_t set_ul_ddc0(const uint8_t ch,const double freq) const;
    int32_t get_ul_ddc0(const uint8_t ch,double &freq) const;
    int32_t set_ul_ddc0_ch0_3(const double freq) const;
    int32_t get_ul_ddc0_ch0_3(double &freq) const;
    int32_t set_ul_ddc0_ch4_7(const double freq) const;
    int32_t get_ul_ddc0_ch4_7(double &freq) const;

    int32_t set_ul_ddc1(const uint8_t ch,const double freq) const;
    int32_t get_ul_ddc1(const uint8_t ch,double &freq) const;

    int32_t set_ul_cpri_map(const uint8_t ch,const uint8_t cpri) const;
    int32_t get_ul_cpri_map(const uint8_t ch,uint8_t &cpri) const;

    DECL_DDS(ul,ddc1,0)
    DECL_DDS(ul,ddc1,1)
    DECL_DDS(ul,ddc1,2)
    DECL_DDS(ul,ddc1,3)
    DECL_DDS(ul,ddc1,4)
    DECL_DDS(ul,ddc1,5)
    DECL_DDS(ul,ddc1,6)
    DECL_DDS(ul,ddc1,7)

    DECL_CPRI_MAP(ul,0)
    DECL_CPRI_MAP(ul,1)
    DECL_CPRI_MAP(ul,2)
    DECL_CPRI_MAP(ul,3)
    DECL_CPRI_MAP(ul,4)
    DECL_CPRI_MAP(ul,5)
    DECL_CPRI_MAP(ul,6)
    DECL_CPRI_MAP(ul,7)

public:
    int32_t set_iq_cap_start();
    int32_t set_iq_cap_abort() const;

    int32_t set_iq_cap_src(const iq_cap_src_t &src);
    int32_t get_iq_cap_src(iq_cap_src_t &src) const;

    int32_t set_iq_cap_src_ddc(const iq_cap_src_ddc_t &src) const;
    int32_t get_iq_cap_src_ddc(iq_cap_src_ddc_t &src) const;

    int32_t set_iq_cap_src_ddc_ch(const uint8_t ch) const;
    int32_t get_iq_cap_src_ddc_ch(uint8_t &ch) const;

    int32_t set_iq_cap_src_dl_cpri_ch(const uint8_t ch) const;
    int32_t get_iq_cap_src_dl_cpri_ch(uint8_t &ch) const;

    int32_t set_iq_cap_src_duc(const iq_cap_src_duc_t &src) const;
    int32_t get_iq_cap_src_duc(iq_cap_src_duc_t &src) const;

    int32_t set_iq_cap_src_duc_ch(const uint8_t ch) const;
    int32_t get_iq_cap_src_duc_ch(uint8_t &ch) const;

    int32_t set_iq_cap_sr(const iq_cap_sr_t &sr);
    int32_t get_iq_cap_sr(iq_cap_sr_t &sr) const;

    int32_t set_iq_cap_mode(const iq_cap_mode_t mode) const;
    int32_t get_iq_cap_mode(iq_cap_mode_t &mode) const;

    int32_t set_iq_cap_samples(const uint32_t samples);
    int32_t get_iq_cap_samples(uint32_t &samples) const;

    int32_t set_iq_cap_trig_src(const iq_cap_trig_src_t src) const;
    int32_t get_iq_cap_trig_src(iq_cap_trig_src_t &src) const;

    int32_t set_iq_cap_trig_edge(const iq_cap_trig_edge_t edge) const;
    int32_t get_iq_cap_trig_edge(iq_cap_trig_edge_t &edge) const;

    int32_t set_iq_cap_trig_threshold(const uint32_t threshold) const;
    int32_t get_iq_cap_trig_threshold(uint32_t &threshold) const;

    int32_t set_iq_cap_trig_offset(const int32_t offset) const;
    int32_t get_iq_cap_trig_offset(int32_t &offset) const;

    int32_t set_iq_cap_trig_gap(const uint32_t gap) const;
    int32_t get_iq_cap_trig_gap(uint32_t &gap) const;

    int32_t set_iq_cap_trig_frame(const uint16_t frame) const;
    int32_t get_iq_cap_trig_frame(uint16_t &frame) const;

    int32_t set_iq_cap_trig_frame_x(const uint16_t x) const;
    int32_t get_iq_cap_trig_frame_x(uint16_t &x) const;

    int32_t set_iq_cap_trig_frame_y(const uint16_t y) const;
    int32_t get_iq_cap_trig_frame_y(uint16_t &y) const;

    int32_t program(const v9_t fpga,const char *path);

private:
    uint32_t _rf_idx;
    pci_dev *_v9;
    pci_dev *_s6;

    static ns_sp9500x::dma_mgr _dma_mgr;
    arb_mgr _arb;
    complex_sequence _ul_sequence;
};

} // namespace rd

#endif // SP2406_H
