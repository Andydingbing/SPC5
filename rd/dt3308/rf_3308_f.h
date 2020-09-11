#ifndef RF_3308_F_H
#define RF_3308_F_H

#include "libbd.h"
#include "frontend.h"
#include "cal_table.h"
#include "reg_def_hmc832.h"
#include "reg_def_sp9500.h"

namespace rd {

#define DT3308_RF_F rf_dt3308_f::instance()

class rf_dt3308_f;

typedef int32_t (rf_dt3308_f::*fp_set_lo)(const uint64_t);
typedef int32_t (rf_dt3308_f::*fp_set_lo_reg_pll)(const uint8_t,const uint32_t);
typedef int32_t (rf_dt3308_f::*fp_get_lo_reg_pll)(const uint8_t,uint32_t &);
typedef int32_t (rf_dt3308_f::*fp_set_lo_reg_vco)(const uint8_t,const uint16_t);

class RD_API rf_dt3308_f : public frontend
{
public:
    typedef boost::shared_ptr<rf_dt3308_f> sptr;

    enum lo_t {
        LO_BEGIN = 2,
        RX_HMC833 = 2,
        RX_HMC832 = 3,
        TX_HMC833 = 4,
        TX_HMC832 = 5,
        LO_END
    };

    enum tx_hmc833_freq_section_t {
        TX_HMC833_FREQ_10_700,
        TX_HMC833_FREQ_700_1000,
        TX_HMC833_FREQ_1000_2000,
        TX_HMC833_FREQ_2000_2700,
        TX_HMC833_FREQ_2700_3800,
        TX_HMC833_FREQ_3800_6000
    };

    enum tx_hmc833_filter_t {
        TX_HMC833_FILTER_10_700 = 7,
        TX_HMC833_FILTER_700_1000 = 2,
        TX_HMC833_FILTER_1000_2000 = 13,
        TX_HMC833_FILTER_2000_2700 = 7,
        TX_HMC833_FILTER_2700_3800 = 8,
        TX_HMC833_FILTER_3800_6000 = 16
    };

    enum tx_freq_section_t {
        TX_FREQ_10_700,
        TX_FREQ_700_1000,
        TX_FREQ_1000_2000,
        TX_FREQ_2000_2700,
        TX_FREQ_2700_3800,
        TX_FREQ_3800_6000
    };

    enum tx_filter_t {
        TX_FILTER_10_700 = 0x2f,
        TX_FILTER_700_1000 = 0x24,
        TX_FILTER_1000_2000 = 0x30,
        TX_FILTER_2000_2700 = 0x2e,
        TX_FILTER_2700_3800 = 0x3a,
        TX_FILTER_3800_6000 = 0x00
    };

    enum tx_main_aux_sw_t {
        TX_MAIN, // port 1~8
        TX_AUX   // port 9
    };

    enum rx_hmc833_freq_section_t {
        RX_HMC833_FREQ_10_700,
        RX_HMC833_FREQ_700_1000,
        RX_HMC833_FREQ_1000_2000,
        RX_HMC833_FREQ_2000_2700,
        RX_HMC833_FREQ_2700_4000,
        RX_HMC833_FREQ_4000_6000
    };

    enum rx_hmc833_filter_t {
        RX_HMC833_FILTER_10_700 = 0x17,
        RX_HMC833_FILTER_700_1000 = 0x12,
        RX_HMC833_FILTER_1000_2000 = 0x1d,
        RX_HMC833_FILTER_2000_2700 = 0x17,
        RX_HMC833_FILTER_2700_4000 = 0x18,
        RX_HMC833_FILTER_4000_6000 = 0x00
    };

    enum rx_freq_section_t {
        RX_FREQ_10_675,
        RX_FREQ_675_3800,
        RX_FREQ_3800_6000
    };

    enum rx_filter_t {
        RX_FILTER_10_675 = 0x3,
        RX_FILTER_675_3800 = 0x5,
        RX_FILTER_3800_6000 = 0xc
    };

    enum port_t {
        PORT_0,
        PORT_1,
        PORT_2,
        PORT_3,
        PORT_4,
        PORT_5,
        PORT_6,
        PORT_7,
        PORT_8,
        PORT_MAX = PORT_8
    };

    union main_ports_t {
        uint8_t data;
        struct bitmap_t {
            unsigned _0 : 1;
            unsigned _1 : 1;
            unsigned _2 : 1;
            unsigned _3 : 1;
            unsigned _4 : 1;
            unsigned _5 : 1;
            unsigned _6 : 1;
            unsigned _7 : 1;
        }port;
    };

public:
    rf_dt3308_f();
    static rf_dt3308_f *instance();

    int32_t open_board();
    int32_t close_board();
    void connect(pci_dev *k7) { m_k7 = k7; }

    int32_t set_tx_freq(const uint64_t freq);
    int32_t set_tx_att0(const double att);
    int32_t set_tx_att1(const double att);
    int32_t set_tx_att2(const double att);
    int32_t set_tx_att3(const double att);
    int32_t set_tx_att4(const double att);
    int32_t set_tx_att5(const double att);
    int32_t set_tx_main_pa_sw_0(const port_t port,const bool en);
    int32_t set_tx_main_pa_sw_1(const port_t port,const bool en);
    int32_t set_tx_main_pa_sw(const port_t port,const bool en_0,const bool en_1);

    common_lo_t *tx_hmc832() { return _tx_hmc832; }
    common_lo_t *tx_hmc833() { return _tx_hmc833; }

    int32_t set_rx_freq(const uint64_t freq);
    int32_t set_rx_att0(const double att);
    int32_t set_rx_att1(const double att);
    int32_t set_rx_lna_att_0_sw(const ns_dt3308::f::rx_lna_att_0_t sw);
    int32_t set_rx_lna_att_1_sw(const ns_dt3308::f::rx_lna_att_1_t sw);
    int32_t set_rx_if_filter_sw(const ns_dt3308::f::rx_if_filter_t sw);

    common_lo_t *rx_hmc832() { return _rx_hmc832; }
    common_lo_t *rx_hmc833() { return _rx_hmc833; }

    int32_t set_tx_port_att0(const double att);
    int32_t set_tx_port_att1(const double att);
    int32_t set_tx_port_att2(const double att);
    int32_t set_tx_port_att3(const double att);
    int32_t set_tx_port_att4(const double att);
    int32_t set_tx_port_att5(const double att);
    int32_t set_tx_port_att6(const double att);
    int32_t set_tx_port_att7(const double att);

    int32_t set_io_mode(const port_t port,const io_mode_t mode);

    int32_t set_tx_en(const bool en);
    int32_t set_tx_main_aux_sw(const tx_main_aux_sw_t sw);
    int32_t set_tx_port_en(const bool en);
    int32_t set_tx_port_en(const port_t port,const bool en);
    int32_t set_tx_port_en(const main_ports_t ports);

    int32_t set_rx_en(const bool en);
    int32_t set_rx_port_en(const port_t port,const bool en);
    int32_t set_rx_port_en(const main_ports_t ports);

    int32_t set_tx_filter_sw(const uint64_t freq);
    int32_t set_tx_filter_sw(const tx_freq_section_t section);
    int32_t set_tx_filter_sw(const tx_filter_t filter);
    int32_t set_rx_filter_sw(const uint64_t freq);
    int32_t set_rx_filter_sw(const rx_freq_section_t section);
    int32_t set_rx_filter_sw(const rx_filter_t filter);

    int32_t set_lo(const lo_t lo,const uint64_t freq);
    int32_t init_tx_hmc832();
    int32_t init_tx_hmc833();
    int32_t init_rx_hmc832();
    int32_t init_rx_hmc833();
    int32_t set_tx_hmc832(const uint64_t freq);
    int32_t set_tx_hmc832_en(const bool is_opened);
    int32_t set_tx_hmc833(const uint64_t freq);
    int32_t set_rx_hmc832(const uint64_t freq);
    int32_t set_rx_hmc832_en(const bool is_opened);
    int32_t set_rx_hmc833(const uint64_t freq);

    int32_t set_lo_reg_pll(const lo_t lo,const uint8_t addr,const uint32_t data);
    int32_t get_lo_reg_pll(const lo_t lo,const uint8_t addr,uint32_t &data);
    int32_t set_lo_reg_vco(const lo_t lo,const uint8_t addr,const uint16_t data);
    int32_t set_tx_hmc832_reg_pll(const uint8_t addr,const uint32_t data);
    int32_t get_tx_hmc832_reg_pll(const uint8_t addr,uint32_t &data);
    int32_t set_tx_hmc832_reg_vco(const uint8_t addr,const uint16_t data);
    int32_t set_tx_hmc833_reg_pll(const uint8_t addr,const uint32_t data);
    int32_t get_tx_hmc833_reg_pll(const uint8_t addr,uint32_t &data);
    int32_t set_tx_hmc833_reg_vco(const uint8_t addr,const uint16_t data);
    int32_t set_rx_hmc832_reg_pll(const uint8_t addr,const uint32_t data);
    int32_t get_rx_hmc832_reg_pll(const uint8_t addr,uint32_t &data);
    int32_t set_rx_hmc832_reg_vco(const uint8_t addr,const uint16_t data);
    int32_t set_rx_hmc833_reg_pll(const uint8_t addr,const uint32_t data);
    int32_t get_rx_hmc833_reg_pll(const uint8_t addr,uint32_t &data);
    int32_t set_rx_hmc833_reg_vco(const uint8_t addr,const uint16_t data);

    int32_t set_tx_hmc833_filter_sw(const tx_hmc833_freq_section_t section);
    int32_t set_tx_hmc833_filter_sw(const tx_hmc833_filter_t filter);
    int32_t set_rx_hmc833_filter_sw(const rx_hmc833_freq_section_t section);
    int32_t set_rx_hmc833_filter_sw(const rx_hmc833_filter_t filter);

    int32_t set_lo_reg(const uint8_t addr,const uint32_t data);
    int32_t get_lo_reg(const uint8_t addr,uint32_t &data);

    int32_t set_epld_reg(const uint8_t addr,const uint8_t data);
    int32_t get_epld_reg(const uint8_t addr,uint8_t &data);

    void tx_freq_to_lo(const uint64_t freq);
    void rx_freq_to_lo(const uint64_t freq);

    static hmc83x_t lo_type(const lo_t lo);
    static common_lo_t::lo_t tx_lo_type(const uint64_t freq);
    static common_lo_t::lo_t rx_lo_type(const uint64_t freq);
    static tx_hmc833_filter_t tx_hmc833_filter(const uint64_t freq);
    static tx_hmc833_filter_t tx_hmc833_filter(const tx_hmc833_freq_section_t idx);
    static rx_hmc833_filter_t rx_hmc833_filter(const uint64_t freq);
    static rx_hmc833_filter_t rx_hmc833_filter(const rx_hmc833_freq_section_t idx);
    static tx_filter_t tx_filter(const uint64_t freq);
    static tx_filter_t tx_filter(const tx_freq_section_t idx);
    static rx_filter_t rx_filter(const uint64_t freq);
    static rx_filter_t rx_filter(const rx_freq_section_t idx);

private:
    pci_dev *m_k7;
    common_lo_t *_tx_hmc832;
    common_lo_t *_tx_hmc833;
    common_lo_t *_rx_hmc832;
    common_lo_t *_rx_hmc833;
};

} // namespace rd

#endif // RF_3308_F_H
