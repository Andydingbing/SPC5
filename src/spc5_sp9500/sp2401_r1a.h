#ifndef SP2401_R1A_H
#define SP2401_R1A_H

#include "libbd.h"
#include "rx_filter_table.h"
#include <boost/smart_ptr.hpp>

#define PROGRAM_BIT_ONEBLOCK_SIZE 500   // * 32bit

namespace sp_rd {

class RD_API sp2401_r1a : boost::noncopyable
{
public:
    typedef boost::shared_ptr<sp2401_r1a> sptr;
    sp2401_r1a(uint32_t rf_idx);

public:
    enum da_src_t {
        ALL_0,
        INTER_FILTER,
        All_1,
        SINGLE_TONE,
        DUAL_TONE,
        DRIVER_ARB,
        DDR3
	};

    enum ad_da_port_t {
        FROM_TO_RF,
        FROM_TO_BB
	};

    enum rx_filter_t {
        _2I3D,
        _3D
	};

    typedef enum frame_trig_mode_t {
        TRIGGER_IN ,
        TRIGGER_OUT
    } hyper_frame_trig_mode_t;

    enum frame_trig_src_t {
        NEXT_FRAME,
        SPEC_FRAME,
        MOD_X_Y
    };

    enum link_t {
        UL,
        DL
    };

public:
    int32_t open_board(vi_pci_dev *k7,vi_pci_dev *s6);
    int32_t	open_board(vi_pci_dev *s6);
    int32_t get_s6_ver(uint32_t &ver);
    uint32_t get_rf_idx() const { return m_rf_idx; }
    int32_t fpga_reset();
    pci_dev* get_k7();
    pci_dev* get_s6();

public:
    int32_t clock_of_da_ld(bool &lock);
    int32_t clock_of_ad_ld(bool &lock);
    int32_t set_da_freq_MHz(double freq);
    int32_t set_da_sw(ad_da_port_t port);
    int32_t da_reset();
    int32_t set_ad_freq_MHz(double freq);
    int32_t set_ad_sw(ad_da_port_t port);
    int32_t set_tx_pwr_comp(double offset);
    int32_t set_tx_pwr_comp(float offset);
    int32_t set_dds_src(da_src_t src);
    int32_t set_dds1(double freq);
    int32_t set_dds2(double freq);
    int32_t set_dds_pwr_comp(double offset = 0.0);
    int32_t set_duc_dds(double freq);
    int32_t set_tx_filter_truncation(uint32_t digit);
    int32_t set_tx_filter_sw(bool en);
    int32_t set_tx_filter(double *real,double *imag);
    int32_t set_ddc(double freq);
    int32_t set_rx_filter_truncation(uint32_t digit);
    int32_t set_rx_filter_sw(rx_filter_t filter);
    int32_t set_rx_filter_2I3D(double *real,double *imag);
    int32_t set_rx_filter_2I3D_default();
    int32_t set_rx_filter(double *real,double *imag);

    template <typename data_t>
    int32_t set_rx_filter(const data_t &data) const;
    int32_t set_rx_filter(const rx_filter_80m_table::data_m_t &data) const;
    int32_t set_rx_filter(const rx_filter_160m_table::data_m_t &data) const;

    int32_t set_rx_filter_default();
    int32_t set_rx_filter_nb();
    int32_t set_rx_pwr_comp(int32_t offset);
    int32_t set_tx_dc_offset(uint16_t I,uint16_t Q);
    int32_t set_tx_phase_rotate_I(double th);
    int32_t set_tx_amplitude_balance(uint16_t I,uint16_t Q);

public:
    int32_t set_frame_trig_mode(frame_trig_mode_t mode);
    int32_t set_frame_trig_in_offset(bool en, double time_ms = 0.0);
    int32_t set_frame_trig_out_offset(bool en, double time_ms = 0.0);
    int32_t set_frame_time(double time_ms);
    double get_frame_time_ms();
    int32_t set_hyper_frame_trig_mode(hyper_frame_trig_mode_t mode);
    int32_t set_hyper_frame_trig_in_offset(bool en, double time_ms = 0.0);
    int32_t set_hyper_frame_trig_out_offset(bool en, double time_ms = 0.0);
    int32_t set_hyper_frame_period(uint32_t period);
    double get_hyper_frame_time_ms();

    int32_t set_slot_time(double time_ms);
    int32_t set_trig_frame(uint32_t frame);
    int32_t set_trig_slot(uint32_t slot);
    int32_t set_frame_trig_src(frame_trig_src_t src);
    int32_t set_frame_trig_mod_x_y(uint16_t x,uint16_t y);
    uint32_t get_cur_frame();
    uint32_t get_cur_slot();

    int32_t set_frame_trig_offset(int32_t offset);
public:
    static int32_t rx_filter_coef_default(double *real,double *imag,int32_t order);
public:
    int32_t set_fpga_bit(const char *path,int32_t block_size = PROGRAM_BIT_ONEBLOCK_SIZE);
private:
    uint32_t m_rf_idx;
    vi_pci_dev *m_k7;
    vi_pci_dev *m_s6;
};

int32_t set_fpga_bit(int32_t fpga_idx,char *path,vi_pci_dev *s6,int32_t block_size);

} // namespace sp_rd

#endif // SP2401_R1A_H
