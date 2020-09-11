#include "rf_3308_f.h"
#include "reg_def_dt3308_f_epld.h"
#include "reg_def_dt3308_f.h"
#include "algo_math.hpp"
#include "algo_chip.hpp"

using namespace std;
using namespace rd;
using namespace ns_dt3308;
using namespace ns_dt3308::f;

#ifdef RD_C_MSC
    #pragma data_seg("DT3308_ShareData")
        static rf_dt3308_f *g_rf_dt3308_f(new rf_dt3308_f);
    #pragma data_seg()
    #pragma comment(linker,"/section:DT3308_ShareData,rws")
#elif defined RD_C_GNUC
    static rf_dt3308_f *g_rf_dt3308_f __attribute__((section("DT3308_ShareData"))) = new rf_dt3308_f;
#endif

rf_dt3308_f::rf_dt3308_f()
{
    _lo_tx.push_back(common_lo_t()); _tx_hmc832 = &(_lo_tx.back());
    _lo_tx.push_back(common_lo_t()); _tx_hmc833 = &(_lo_tx.back());
    _lo_tx.resize(2);
    _lo_rx.push_back(common_lo_t()); _rx_hmc832 = &(_lo_rx.back());
    _lo_rx.push_back(common_lo_t()); _rx_hmc833 = &(_lo_rx.back());
    _lo_rx.resize(2);
}

rf_dt3308_f *rf_dt3308_f::instance()
{
    return g_rf_dt3308_f;
}

int32_t rf_dt3308_f::open_board()
{
    _tx_hmc832->freq = 1800000000;
    _tx_hmc832->is_opened = false;
    _tx_hmc833->is_opened = true;
    init_tx_hmc832();
    init_tx_hmc833();
//    init_rx_hmc832();
//    init_rx_hmc833();

    set_tx_hmc832(1800000000);
    set_tx_freq(2000000000);
//    set_rx_freq(2000000000);

    set_tx_en(true);
    set_tx_main_aux_sw(TX_MAIN);
    set_tx_port_en(false);
    set_tx_port_en(PORT_0,true);

    return 0;
}

int32_t rf_dt3308_f::close_board()
{
    return 0;
}

int32_t rf_dt3308_f::set_tx_freq(const uint64_t freq)
{
    bool hmc832_is_opened_saved = _tx_hmc832->is_opened;

    tx_freq_to_lo(freq);

    if (_tx_hmc832->is_opened != hmc832_is_opened_saved) {
        set_tx_hmc832_en(_tx_hmc832->is_opened);
    }

    set_tx_hmc833(_tx_hmc833->freq);
    set_tx_filter_sw(freq);
    return 0;
}


int32_t rf_dt3308_f::set_rx_freq(uint64_t freq)
{
    bool hmc832_is_opened_saved = _rx_hmc832->is_opened;

    rx_freq_to_lo(freq);

    if (_rx_hmc832->is_opened != hmc832_is_opened_saved) {
        set_rx_hmc832(_rx_hmc832->freq);
        set_rx_hmc832_en(_rx_hmc832->is_opened);
    }

    set_rx_hmc833(_rx_hmc833->freq);
    set_rx_filter_sw(freq);
    return 0;
}

int32_t rf_dt3308_f::set_io_mode(const port_t port,const io_mode_t mode)
{
    switch (mode) {
        case IO : {
            INT_CHECK(set_tx_port_en(port,true));
            INT_CHECK(set_rx_port_en(port,true));
            return 0;
        }
        case OUTPUT : {
            INT_CHECK(set_tx_port_en(port,true));
            INT_CHECK(set_rx_port_en(port,false));
            return 0;
        }
        case INPUT : {
            INT_CHECK(set_tx_port_en(port,false));
            INT_CHECK(set_rx_port_en(port,true));
            return 0;
        }
        case CLOSE : {
            INT_CHECK(set_tx_port_en(port,false));
            INT_CHECK(set_rx_port_en(port,false));
            return 0;
        }
        default : { return 0; }
    }
}

int32_t rf_dt3308_f::set_tx_att0(const double att)
{
    EPLD_REG_DECL(0x13)

    EPLD_REG(0x13).att = unsigned(att);
    return set_epld_reg(0x13,EPLD_REG_DATA(0x13));
}

int32_t rf_dt3308_f::set_tx_att1(const double att)
{
    EPLD_REG_DECL(0x14)

    EPLD_REG(0x14).att = unsigned(att);
    return set_epld_reg(0x14,EPLD_REG_DATA(0x14));
}

int32_t rf_dt3308_f::set_tx_att2(const double att)
{
    EPLD_REG_DECL(0x15)

    EPLD_REG(0x15).att = unsigned(att);
    return set_epld_reg(0x15,EPLD_REG_DATA(0x15));
}

int32_t rf_dt3308_f::set_tx_att3(const double att)
{
    EPLD_REG_DECL(0x16)

    EPLD_REG(0x16).att = unsigned(att);
    return set_epld_reg(0x16,EPLD_REG_DATA(0x16));
}

int32_t rf_dt3308_f::set_tx_att4(const double att)
{
    EPLD_REG_DECL(0x17)

    EPLD_REG(0x17).att = unsigned(att);
    return set_epld_reg(0x17,EPLD_REG_DATA(0x17));
}

int32_t rf_dt3308_f::set_tx_att5(const double att)
{
    EPLD_REG_DECL(0x18)

    EPLD_REG(0x18).att = unsigned(att);
    return set_epld_reg(0x18,EPLD_REG_DATA(0x18));
}

int32_t rf_dt3308_f::set_tx_main_pa_sw(const port_t port,const bool en_0,const bool en_1)
{
    EPLD_REG_DECL(0x51)

    INT_CHECK(get_epld_reg(0x51,EPLD_REG_DATA(0x51)));

    if (port <= PORT_3) {
        EPLD_REG(0x51).main_0_3_sw0 = en_0 ? 1 : 0;
        EPLD_REG(0x51).main_0_3_sw1 = en_1 ? 1 : 0;
    } else {
        EPLD_REG(0x51).main_4_7_sw0 = en_0 ? 1 : 0;
        EPLD_REG(0x51).main_4_7_sw1 = en_1 ? 1 : 0;
    }
    return set_epld_reg(0x51,EPLD_REG_DATA(0x51));
}

int32_t rf_dt3308_f::set_rx_att0(const double att)
{
    EPLD_REG_DECL(0x23)

    EPLD_REG(0x23).att = unsigned(att);
    return set_epld_reg(0x23,EPLD_REG_DATA(0x23));
}

int32_t rf_dt3308_f::set_rx_att1(const double att)
{
    EPLD_REG_DECL(0x24)

    EPLD_REG(0x24).att = unsigned(att);
    return set_epld_reg(0x24,EPLD_REG_DATA(0x24));
}

int32_t rf_dt3308_f::set_rx_lna_att_0_sw(const rx_lna_att_0_t sw)
{
    EPLD_REG_DECL(0x25)

    EPLD_REG(0x25).lna = (sw == RX_LNA_0 ? 1 : 0);
    EPLD_REG(0x25).att = (sw == RX_ATT_0 ? 1 : 0);
    return set_epld_reg(0x25,EPLD_REG_DATA(0x25));
}

int32_t rf_dt3308_f::set_rx_lna_att_1_sw(const rx_lna_att_1_t sw)
{
    EPLD_REG_DECL(0x56)

    EPLD_REG(0x56).lna = (sw == RX_LNA_1 ? 1 : 0);
    EPLD_REG(0x56).att = (sw == RX_ATT_1 ? 1 : 0);
    return set_epld_reg(0x56,EPLD_REG_DATA(0x56));
}

int32_t rf_dt3308_f::set_rx_if_filter_sw(const rx_if_filter_t sw)
{
    EPLD_REG_DECL(0x3e)

    EPLD_REG(0x3e).sw = sw;
    return set_epld_reg(0x3e,EPLD_REG_DATA(0x3e));
}

int32_t rf_dt3308_f::set_tx_main_pa_sw_0(const port_t port,const bool en)
{
    EPLD_REG_DECL(0x51)

    INT_CHECK(get_epld_reg(0x51,EPLD_REG_DATA(0x51)));

    if (port <= PORT_3) {
        EPLD_REG(0x51).main_0_3_sw0 = en ? 1 : 0;
    } else {
        EPLD_REG(0x51).main_4_7_sw0 = en ? 1 : 0;
    }
    return set_epld_reg(0x51,EPLD_REG_DATA(0x51));
}

int32_t rf_dt3308_f::set_tx_main_pa_sw_1(const port_t port,const bool en)
{
    EPLD_REG_DECL(0x51)

    INT_CHECK(get_epld_reg(0x51,EPLD_REG_DATA(0x51)));

    if (port <= PORT_3) {
        EPLD_REG(0x51).main_0_3_sw1 = en ? 1 : 0;
    } else {
        EPLD_REG(0x51).main_4_7_sw1 = en ? 1 : 0;
    }
    return set_epld_reg(0x51,EPLD_REG_DATA(0x51));
}

#define IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(port,epld_reg) \
    int32_t rf_dt3308_f::set_tx_port_att##port(const double att) { \
        EPLD_REG_DECL(epld_reg) \
        EPLD_REG(epld_reg).att = ns_pe43704::att_to_bit_0_25(att); \
        return set_epld_reg(epld_reg,EPLD_REG_DATA(epld_reg)); \
    }

IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(0,0x2d)
IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(1,0x2c)
IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(2,0x2b)
IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(3,0x2a)
IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(4,0x29)
IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(5,0x28)
IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(6,0x27)
IMPL_FUNC_RF_DT3308_F_SET_TX_PORT_ATT(7,0x26)

int32_t rf_dt3308_f::set_tx_en(const bool en)
{
    EPLD_REG_DECL(0x53)

    INT_CHECK(get_epld_reg(0x53,EPLD_REG_DATA(0x53)));
    EPLD_REG(0x53).en_tx = en ? 1 : 0;
    return set_epld_reg(0x53,EPLD_REG_DATA(0x53));
}

int32_t rf_dt3308_f::set_tx_main_aux_sw(const tx_main_aux_sw_t sw)
{
    EPLD_REG_DECL(0x53)

    INT_CHECK(get_epld_reg(0x53,EPLD_REG_DATA(0x53)));
    EPLD_REG(0x53).sw_tx_main_aux = (sw == TX_MAIN ? 1 : 0);
    return set_epld_reg(0x53,EPLD_REG_DATA(0x53));
}

int32_t rf_dt3308_f::set_tx_port_en(const bool en)
{
    main_ports_t ports;
    ports.data = en ? 0xFF : 0x00;
    return set_tx_port_en(ports);
}

int32_t rf_dt3308_f::set_tx_port_en(const port_t port,const bool en)
{
    EPLD_REG_DECL(0x54)

    INT_CHECK(get_epld_reg(0X54,EPLD_REG_DATA(0x54)));
    EPLD_REG_DATA(0x54) |= (1 << port);
    EPLD_REG_DATA(0x54) &= ~((en ? 0 : 1) << port);
    return set_epld_reg(0x54,EPLD_REG_DATA(0x54));
}

int32_t rf_dt3308_f::set_tx_port_en(const main_ports_t ports)
{
    EPLD_REG_DECL(0x54)

    INT_CHECK(get_epld_reg(0X54,EPLD_REG_DATA(0x54)));
    EPLD_REG_DATA(0x54) = ports.data;
    return set_epld_reg(0x54,EPLD_REG_DATA(0x54));
}

int32_t rf_dt3308_f::set_rx_en(const bool en)
{
    EPLD_REG_DECL(0x50)

    EPLD_REG(0x50).en = en ? 1 : 0;
    return set_epld_reg(0x50,EPLD_REG_DATA(0x50));
}

int32_t rf_dt3308_f::set_rx_port_en(const port_t port,const bool en)
{
    EPLD_REG_DECL(0x52)

    INT_CHECK(get_epld_reg(0x52,EPLD_REG_DATA(0x52)));
    EPLD_REG_DATA(0x52) |= (1 << port);
    EPLD_REG_DATA(0x52) &= ~((en ? 0 : 1) << port);
    return set_epld_reg(0x52,EPLD_REG_DATA(0x52));
}

int32_t rf_dt3308_f::set_rx_port_en(const main_ports_t ports)
{
    EPLD_REG_DECL(0x52)

    INT_CHECK(get_epld_reg(0x52,EPLD_REG_DATA(0x52)));
    EPLD_REG_DATA(0x52) = ports.data;
    return set_epld_reg(0x52,EPLD_REG_DATA(0x52));
}

int32_t rf_dt3308_f::set_tx_filter_sw(const uint64_t freq)
{
    return set_tx_filter_sw(tx_filter(freq));
}

int32_t rf_dt3308_f::set_tx_filter_sw(const tx_freq_section_t section)
{
    return set_tx_filter_sw(tx_filter(section));
}

int32_t rf_dt3308_f::set_tx_filter_sw(const tx_filter_t filter)
{
    return set_epld_reg(0x11,filter);
}

int32_t rf_dt3308_f::set_lo(const lo_t lo,const uint64_t freq)
{
    fp_set_lo funcs[4] = {
        &rf_dt3308_f::set_rx_hmc833,
        &rf_dt3308_f::set_rx_hmc832,
        &rf_dt3308_f::set_tx_hmc833,
        &rf_dt3308_f::set_tx_hmc832
    };

    return (this->*funcs[lo - LO_BEGIN])(freq);
}

int32_t rf_dt3308_f::set_rx_filter_sw(const uint64_t freq)
{
    return set_rx_filter_sw(rx_filter(freq));
}

int32_t rf_dt3308_f::set_rx_filter_sw(const rx_freq_section_t section)
{
    return set_rx_filter_sw(rx_filter(section));
}

int32_t rf_dt3308_f::set_rx_filter_sw(const rx_filter_t filter)
{
    return set_epld_reg(0x21,filter);
}

int32_t rf_dt3308_f::init_tx_hmc832()
{
    HMC83X_ALL_REG_DECL

    INT_CHECK(set_tx_hmc832_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x02,HMC83X_PLL_REG_DATA(0x02)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x06,HMC83X_PLL_REG_DATA(0x06)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x07,HMC83X_PLL_REG_DATA(0x07)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x08,HMC83X_PLL_REG_DATA(0x08)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x09,HMC83X_PLL_REG_DATA(0x09)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x0a,HMC83X_PLL_REG_DATA(0x0a)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x0b,HMC83X_PLL_REG_DATA(0x0b)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x0c,HMC83X_PLL_REG_DATA(0x0c)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x0f,HMC83X_PLL_REG_DATA(0x0f)));

    INT_CHECK(set_tx_hmc832_reg_vco(0x01,HMC83X_VCO_REG_DATA(0x01)));
    INT_CHECK(set_tx_hmc832_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
    INT_CHECK(set_tx_hmc832_reg_vco(0x03,HMC83X_VCO_REG_DATA(0x03)));
    INT_CHECK(set_tx_hmc832_reg_vco(0x04,HMC83X_VCO_REG_DATA(0x04)));
    INT_CHECK(set_tx_hmc832_reg_vco(0x05,HMC83X_VCO_REG_DATA(0x05)));
    INT_CHECK(set_tx_hmc832_reg_vco(0x06,HMC83X_VCO_REG_DATA(0x06)));
    INT_CHECK(set_tx_hmc832_reg_vco(0x07,HMC83X_VCO_REG_DATA(0x07)));

    /*
     * 1, In fractional mode,set the INT and FRAC register.
     * 2, HMC832 triggers the autocalibration statemachine.
     * 3, Clear 0x05 in PLL subsystem.
     * 4, Rewrite INT/FRAC to retrigger the autocalibration.
     */
    INT_CHECK(set_tx_hmc832_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x05,0));
    INT_CHECK(set_tx_hmc832_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    return 0;
}

int32_t rf_dt3308_f::init_tx_hmc833()
{
    HMC83X_ALL_REG_DECL

    HMC833_REG_INIT

    INT_CHECK(set_tx_hmc833_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x02,HMC83X_PLL_REG_DATA(0x02)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x06,HMC83X_PLL_REG_DATA(0x06)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x07,HMC83X_PLL_REG_DATA(0x07)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x08,HMC83X_PLL_REG_DATA(0x08)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x09,HMC83X_PLL_REG_DATA(0x09)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x0a,HMC83X_PLL_REG_DATA(0x0a)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x0b,HMC83X_PLL_REG_DATA(0x0b)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x0c,HMC83X_PLL_REG_DATA(0x0c)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x0f,HMC83X_PLL_REG_DATA(0x0f)));

    INT_CHECK(set_tx_hmc833_reg_vco(0x01,HMC83X_VCO_REG_DATA(0x01)));
    INT_CHECK(set_tx_hmc833_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
    INT_CHECK(set_tx_hmc833_reg_vco(0x03,HMC83X_VCO_REG_DATA(0x03)));
    INT_CHECK(set_tx_hmc833_reg_vco(0x04,HMC83X_VCO_REG_DATA(0x04)));
    INT_CHECK(set_tx_hmc833_reg_vco(0x05,HMC83X_VCO_REG_DATA(0x05)));
    INT_CHECK(set_tx_hmc833_reg_vco(0x06,HMC83X_VCO_REG_DATA(0x06)));

    /*
     * 1, In fractional mode,set the INT and FRAC register.
     * 2, HMC832 triggers the autocalibration statemachine.
     * 3, Clear 0x05 in PLL subsystem.
     * 4, Rewrite INT/FRAC to retrigger the autocalibration.
     */
    INT_CHECK(set_tx_hmc833_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x05,0));
    INT_CHECK(set_tx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    return 0;
}

int32_t rf_dt3308_f::init_rx_hmc832()
{
    HMC83X_ALL_REG_DECL

    INT_CHECK(set_rx_hmc832_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x02,HMC83X_PLL_REG_DATA(0x02)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x06,HMC83X_PLL_REG_DATA(0x06)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x07,HMC83X_PLL_REG_DATA(0x07)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x08,HMC83X_PLL_REG_DATA(0x08)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x09,HMC83X_PLL_REG_DATA(0x09)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x0a,HMC83X_PLL_REG_DATA(0x0a)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x0b,HMC83X_PLL_REG_DATA(0x0b)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x0c,HMC83X_PLL_REG_DATA(0x0c)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x0f,HMC83X_PLL_REG_DATA(0x0f)));

    INT_CHECK(set_rx_hmc832_reg_vco(0x01,HMC83X_VCO_REG_DATA(0x01)));
    INT_CHECK(set_rx_hmc832_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
    INT_CHECK(set_rx_hmc832_reg_vco(0x03,HMC83X_VCO_REG_DATA(0x03)));
    INT_CHECK(set_rx_hmc832_reg_vco(0x04,HMC83X_VCO_REG_DATA(0x04)));
    INT_CHECK(set_rx_hmc832_reg_vco(0x05,HMC83X_VCO_REG_DATA(0x05)));
    INT_CHECK(set_rx_hmc832_reg_vco(0x06,HMC83X_VCO_REG_DATA(0x06)));
    INT_CHECK(set_rx_hmc832_reg_vco(0x07,HMC83X_VCO_REG_DATA(0x07)));

    /*
     * 1, In fractional mode,set the INT and FRAC register.
     * 2, HMC832 triggers the autocalibration statemachine.
     * 3, Clear 0x05 in PLL subsystem.
     * 4, Rewrite INT/FRAC to retrigger the autocalibration.
     */
    INT_CHECK(set_rx_hmc832_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x05,0));
    INT_CHECK(set_rx_hmc832_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    return 0;
}

int32_t rf_dt3308_f::init_rx_hmc833()
{
    HMC83X_ALL_REG_DECL

    HMC833_REG_INIT

    INT_CHECK(set_rx_hmc833_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x02,HMC83X_PLL_REG_DATA(0x02)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x06,HMC83X_PLL_REG_DATA(0x06)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x07,HMC83X_PLL_REG_DATA(0x07)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x08,HMC83X_PLL_REG_DATA(0x08)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x09,HMC83X_PLL_REG_DATA(0x09)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x0a,HMC83X_PLL_REG_DATA(0x0a)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x0b,HMC83X_PLL_REG_DATA(0x0b)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x0c,HMC83X_PLL_REG_DATA(0x0c)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x0f,HMC83X_PLL_REG_DATA(0x0f)));

    INT_CHECK(set_rx_hmc833_reg_vco(0x01,HMC83X_VCO_REG_DATA(0x01)));
    INT_CHECK(set_rx_hmc833_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
    INT_CHECK(set_rx_hmc833_reg_vco(0x03,HMC83X_VCO_REG_DATA(0x03)));
    INT_CHECK(set_rx_hmc833_reg_vco(0x04,HMC83X_VCO_REG_DATA(0x04)));
    INT_CHECK(set_rx_hmc833_reg_vco(0x05,HMC83X_VCO_REG_DATA(0x05)));
    INT_CHECK(set_rx_hmc833_reg_vco(0x06,HMC83X_VCO_REG_DATA(0x06)));

    /*
     * 1, In fractional mode,set the INT and FRAC register.
     * 2, HMC832 triggers the autocalibration statemachine.
     * 3, Clear 0x05 in PLL subsystem.
     * 4, Rewrite INT/FRAC to retrigger the autocalibration.
     */
    INT_CHECK(set_rx_hmc833_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x05,0));
    INT_CHECK(set_rx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    return 0;
}

int32_t rf_dt3308_f::set_tx_hmc832(const uint64_t freq)
{
    HMC83X_PLL_REG_DECL_0x03;
    HMC83X_PLL_REG_DECL_0x04;
    HMC83X_VCO_REG_DECL_0x02;

    uint8_t div = (freq < 1500000000) ? (1500000000 / freq + (1500000000 % freq ? 1 : 0)) : 1;
    ns_hmc83x::freq_formula_in param_in;
    ns_hmc83x::freq_formula_out param_out;

    div += (div == 3) ? 1 : 0;
    div += (div == 5) ? 1 : 0;
    div += (div == 61) ? 1 : 0;

    param_in.freq_vco = freq * div;
    ns_hmc83x::freq_formula(param_in,param_out);

    HMC83X_PLL_REG(0x03)._int = param_out._int;
    HMC83X_PLL_REG(0x04)._frac = param_out._frac;
    HMC83X_VCO_REG(0x02).div = div;

    INT_CHECK(set_tx_hmc832_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_tx_hmc832_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
//    INT_CHECK(set_tx_hmc832_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_tx_hmc832_reg_pll(0x05,0));
    INT_CHECK(set_tx_hmc832_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    return 0;
}

int32_t rf_dt3308_f::set_tx_hmc832_en(const bool is_opened)
{
    HMC83X_PLL_REG_DECL_0x01;
    HMC83X_VCO_REG_DECL_0x01;

    if (is_opened == true) {
        INT_CHECK(set_tx_hmc832_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
        INT_CHECK(set_tx_hmc832_reg_vco(0x01,HMC83X_VCO_REG_DATA(0x01)));
    } else {
        HMC83X_PLL_REG(0x01).rst_from_spi = 0;
        INT_CHECK(set_tx_hmc832_reg_vco(0x01,0));
        INT_CHECK(set_tx_hmc832_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
    }
    return 0;
}

int32_t rf_dt3308_f::set_tx_hmc833(const uint64_t freq)
{
    HMC83X_PLL_REG_DECL_0x03;
    HMC83X_PLL_REG_DECL_0x04;
    HMC83X_VCO_REG_DECL_0x02;
    HMC83X_VCO_REG_DECL_0x03;

    HMC83X_VCO_REG_DATA(0x02) = 0x00c1;
    HMC83X_VCO_REG_DATA(0x03) = 0x0051;

    uint8_t div = (freq < 1500000000) ? (1500000000 / freq + (1500000000 % freq ? 1 : 0)) : 1;
    uint8_t dbr = (freq > 3000000000 ? 2 : 1);
    ns_hmc83x::freq_formula_in param_in;
    ns_hmc83x::freq_formula_out param_out;

    div += (div == 3) ? 1 : 0;
    div += (div == 5) ? 1 : 0;
    div += (div == 61) ? 1 : 0;

    param_in.freq_vco = freq * div;
    param_in.freq_vco /= dbr;
    ns_hmc83x::freq_formula(param_in,param_out);

    HMC83X_PLL_REG(0x03)._int = param_out._int;
    HMC83X_PLL_REG(0x04)._frac = param_out._frac;
    HMC83X_VCO_REG(0x02).div = div;
    HMC833_VCO_REG(0x03).out_doubler = (dbr == 2 ? 0 : 1);
    HMC833_VCO_REG(0x03).rf_buff_bias = (dbr == 2 ? 0 : 2);

    INT_CHECK(set_tx_hmc833_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_tx_hmc833_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
    INT_CHECK(set_tx_hmc833_reg_vco(0x03,HMC83X_VCO_REG_DATA(0x03)));
//    INT_CHECK(set_tx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_tx_hmc833_reg_pll(0x05,0));
    INT_CHECK(set_tx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_tx_hmc833_filter_sw(tx_hmc833_filter(freq)));
    return 0;
}

int32_t rf_dt3308_f::set_rx_hmc832(const uint64_t freq)
{
    HMC83X_PLL_REG_DECL_0x03;
    HMC83X_PLL_REG_DECL_0x04;
    HMC83X_VCO_REG_DECL_0x02;

    uint8_t div = (freq < 1500000000) ? (1500000000 / freq + (1500000000 % freq ? 1 : 0)) : 1;
    ns_hmc83x::freq_formula_in param_in;
    ns_hmc83x::freq_formula_out param_out;

    div += (div == 3) ? 1 : 0;
    div += (div == 5) ? 1 : 0;
    div += (div == 61) ? 1 : 0;

    param_in.freq_vco = freq * div;
    ns_hmc83x::freq_formula(param_in,param_out);

    HMC83X_PLL_REG(0x03)._int = param_out._int;
    HMC83X_PLL_REG(0x04)._frac = param_out._frac;
    HMC83X_VCO_REG(0x02).div = div;

    INT_CHECK(set_rx_hmc832_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_rx_hmc832_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
//    INT_CHECK(set_rx_hmc832_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_rx_hmc832_reg_pll(0x05,0));
    INT_CHECK(set_rx_hmc832_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    return 0;
}

int32_t rf_dt3308_f::set_rx_hmc832_en(const bool is_opened)
{
    HMC83X_PLL_REG_DECL_0x01;
    HMC83X_VCO_REG_DECL_0x01;

    if (is_opened == true) {
        INT_CHECK(set_rx_hmc832_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
        INT_CHECK(set_rx_hmc832_reg_vco(0x01,HMC83X_VCO_REG_DATA(0x01)));
    } else {
        HMC83X_PLL_REG(0x01).rst_from_spi = 0;
        INT_CHECK(set_rx_hmc832_reg_vco(0x01,0));
        INT_CHECK(set_rx_hmc832_reg_pll(0x01,HMC83X_PLL_REG_DATA(0x01)));
    }
    return 0;
}

int32_t rf_dt3308_f::set_rx_hmc833(const uint64_t freq)
{
    HMC83X_PLL_REG_DECL_0x03;
    HMC83X_PLL_REG_DECL_0x04;
    HMC83X_VCO_REG_DECL_0x02;
    HMC83X_VCO_REG_DECL_0x03;

    HMC83X_VCO_REG_DATA(0x02) = 0x00c1;
    HMC83X_VCO_REG_DATA(0x03) = 0x0051;

    uint8_t div = (freq < 1500000000) ? (1500000000 / freq + (1500000000 % freq ? 1 : 0)) : 1;
    uint8_t dbr = (freq > 3000000000 ? 2 : 1);
    ns_hmc83x::freq_formula_in param_in;
    ns_hmc83x::freq_formula_out param_out;

    div += (div == 3) ? 1 : 0;
    div += (div == 5) ? 1 : 0;
    div += (div == 61) ? 1 : 0;

    param_in.freq_vco = freq * div;
    param_in.freq_vco /= dbr;
    ns_hmc83x::freq_formula(param_in,param_out);

    HMC83X_PLL_REG(0x03)._int = param_out._int;
    HMC83X_PLL_REG(0x04)._frac = param_out._frac;
    HMC83X_VCO_REG(0x02).div = div;
    HMC833_VCO_REG(0x03).out_doubler = (dbr == 2 ? 0 : 1);
    HMC833_VCO_REG(0x03).rf_buff_bias = (dbr == 2 ? 0 : 2);

    INT_CHECK(set_rx_hmc833_reg_pll(0x03,HMC83X_PLL_REG_DATA(0x03)));
    INT_CHECK(set_rx_hmc833_reg_vco(0x02,HMC83X_VCO_REG_DATA(0x02)));
    INT_CHECK(set_rx_hmc833_reg_vco(0x03,HMC83X_VCO_REG_DATA(0x03)));
//    INT_CHECK(set_rx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_rx_hmc833_reg_pll(0x05,0));
    INT_CHECK(set_rx_hmc833_reg_pll(0x04,HMC83X_PLL_REG_DATA(0x04)));
    INT_CHECK(set_rx_hmc833_filter_sw(rx_hmc833_filter(freq)));
    return 0;
}

int32_t rf_dt3308_f::set_lo_reg_pll(const lo_t lo,const uint8_t addr,const uint32_t data)
{
    fp_set_lo_reg_pll funcs[4] = {
        &rf_dt3308_f::set_rx_hmc833_reg_pll,
        &rf_dt3308_f::set_rx_hmc832_reg_pll,
        &rf_dt3308_f::set_tx_hmc833_reg_pll,
        &rf_dt3308_f::set_tx_hmc832_reg_pll
    };
    return (this->*funcs[lo - LO_BEGIN])(addr,data);
}

int32_t rf_dt3308_f::get_lo_reg_pll(const lo_t lo,const uint8_t addr,uint32_t &data)
{
    fp_get_lo_reg_pll funcs[4] = {
        &rf_dt3308_f::get_rx_hmc833_reg_pll,
        &rf_dt3308_f::get_rx_hmc832_reg_pll,
        &rf_dt3308_f::get_tx_hmc833_reg_pll,
        &rf_dt3308_f::get_tx_hmc832_reg_pll
    };
    return (this->*funcs[lo - LO_BEGIN])(addr,data);
}

int32_t rf_dt3308_f::set_lo_reg_vco(const lo_t lo,const uint8_t addr,const uint16_t data)
{
    fp_set_lo_reg_vco funcs[4] = {
        &rf_dt3308_f::set_rx_hmc833_reg_vco,
        &rf_dt3308_f::set_rx_hmc832_reg_vco,
        &rf_dt3308_f::set_tx_hmc833_reg_vco,
        &rf_dt3308_f::set_tx_hmc832_reg_vco
    };
    return (this->*funcs[lo - LO_BEGIN])(addr,data);
}

int32_t rf_dt3308_f::set_tx_hmc832_reg_pll(const uint8_t addr,const uint32_t data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = TX_HMC832;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return set_lo_reg(addr,data);
}

int32_t rf_dt3308_f::get_tx_hmc832_reg_pll(const uint8_t addr,uint32_t &data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = TX_HMC832;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return get_lo_reg(addr,data);
}

int32_t rf_dt3308_f::set_tx_hmc832_reg_vco(const uint8_t addr,const uint16_t data)
{
//    KUS_REG_DECLARE(0x960000);

//    ns_reg_kusr0x960000.r_vco.addr_5 = addr;
//    ns_reg_kusr0x960000.r_vco.data = data;
//    ns_reg_kusr0x960000.r_vco.addr = 0x05;
//    KUS_W(0x960000);
//    return 0;

    HMC83X_PLL_REG_DECL_0x05;
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = TX_HMC832;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));

    HMC83X_PLL_REG(0x05).vco_id = 0;
    HMC83X_PLL_REG(0x05).vco_reg_addr = addr;
    HMC83X_PLL_REG(0x05).vco_reg_data = data;
    return set_lo_reg(0x05,HMC83X_PLL_REG_DATA(0x05));
}

int32_t rf_dt3308_f::set_tx_hmc833_reg_pll(const uint8_t addr,const uint32_t data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = TX_HMC833;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return set_lo_reg(addr,data);
}

int32_t rf_dt3308_f::get_tx_hmc833_reg_pll(const uint8_t addr,uint32_t &data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = TX_HMC833;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return get_lo_reg(addr,data);
}

int32_t rf_dt3308_f::set_tx_hmc833_reg_vco(const uint8_t addr,const uint16_t data)
{
    HMC83X_PLL_REG_DECL_0x05;
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = TX_HMC833;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));

    HMC83X_PLL_REG(0x05).vco_id = 0;
    HMC83X_PLL_REG(0x05).vco_reg_addr = addr;
    HMC83X_PLL_REG(0x05).vco_reg_data = data;
    return set_lo_reg(0x05,HMC83X_PLL_REG_DATA(0x05));
}

int32_t rf_dt3308_f::set_rx_hmc832_reg_pll(const uint8_t addr,const uint32_t data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = RX_HMC832;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return set_lo_reg(addr,data);
}

int32_t rf_dt3308_f::get_rx_hmc832_reg_pll(const uint8_t addr,uint32_t &data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = RX_HMC832;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return get_lo_reg(addr,data);
}

int32_t rf_dt3308_f::set_rx_hmc832_reg_vco(const uint8_t addr,const uint16_t data)
{
    HMC83X_PLL_REG_DECL_0x05;
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = RX_HMC832;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));

    HMC83X_PLL_REG(0x05).vco_id = 0;
    HMC83X_PLL_REG(0x05).vco_reg_addr = addr;
    HMC83X_PLL_REG(0x05).vco_reg_data = data;
    return set_lo_reg(0x05,HMC83X_PLL_REG_DATA(0x05));
}

int32_t rf_dt3308_f::set_rx_hmc833_reg_pll(const uint8_t addr,const uint32_t data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = RX_HMC833;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return set_lo_reg(addr,data);
}

int32_t rf_dt3308_f::get_rx_hmc833_reg_pll(const uint8_t addr,uint32_t &data)
{
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = RX_HMC833;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));
    return get_lo_reg(addr,data);
}

int32_t rf_dt3308_f::set_rx_hmc833_reg_vco(const uint8_t addr,const uint16_t data)
{
    HMC83X_PLL_REG_DECL_0x05;
    EPLD_REG_DECL(0x33);

    EPLD_REG(0x33).pll = RX_HMC833;
    INT_CHECK(set_epld_reg(0x33,EPLD_REG_DATA(0x33)));

    HMC83X_PLL_REG(0x05).vco_id = 0;
    HMC83X_PLL_REG(0x05).vco_reg_addr = addr;
    HMC83X_PLL_REG(0x05).vco_reg_data = data;
    return set_lo_reg(0x05,HMC83X_PLL_REG_DATA(0x05));
}

int32_t rf_dt3308_f::set_tx_hmc833_filter_sw(const tx_hmc833_freq_section_t section)
{
    return set_tx_hmc833_filter_sw(tx_hmc833_filter(section));
}

int32_t rf_dt3308_f::set_tx_hmc833_filter_sw(const tx_hmc833_filter_t filter)
{
    _tx_hmc833->filter = filter;
    return set_epld_reg(0x10,filter);
}

int32_t rf_dt3308_f::set_rx_hmc833_filter_sw(const rx_hmc833_freq_section_t section)
{
    return set_rx_hmc833_filter_sw(rx_hmc833_filter(section));
}

int32_t rf_dt3308_f::set_rx_hmc833_filter_sw(const rx_hmc833_filter_t filter)
{
    _rx_hmc833->filter = filter;
    return set_epld_reg(0x20,filter);
}

int32_t rf_dt3308_f::set_lo_reg(const uint8_t addr,const uint32_t data)
{
    KUS_REG_DECLARE(0x960000);
    KUS_REG_DECLARE(0x960001);

    KUS_REG(0x960001).sw = 0;
    KUS_W(0x960001);

    ns_reg_kusr0x960000.r_pll.wr = 0;
    ns_reg_kusr0x960000.r_pll.addr = addr;
    ns_reg_kusr0x960000.r_pll.data = data;
    KUS_W(0x960000);
    return 0;
}

int32_t rf_dt3308_f::get_lo_reg(const uint8_t addr,uint32_t &data)
{
    KUS_REG_DECLARE(0x960000);
    KUS_REG_DECLARE(0x960001);
    KUS_REG_DECLARE(0x960002);

    KUS_REG(0x960001).sw = 0;
    KUS_W(0x960001);

    ns_reg_kusr0x960000.r_pll.wr = 1;
    ns_reg_kusr0x960000.r_pll.addr = addr;
    KUS_W(0x960000);

//    KUS_WAIT_IDLE(0x960002,1,1000);
    KUS_R(0x960002);
    data = KUS_REG(0x960002).data;
    return 0;
}

int32_t rf_dt3308_f::set_epld_reg(const uint8_t addr,const uint8_t data)
{
    KUS_REG_DECLARE(0x960000);
    KUS_REG_DECLARE(0x960001);

    KUS_REG(0x960001).sw = 1;
    KUS_W(0x960001);

    ns_reg_kusr0x960000.r_epld.rsv0 = 0;
    ns_reg_kusr0x960000.r_epld.wr = 0;
    ns_reg_kusr0x960000.r_epld.addr = addr;
    ns_reg_kusr0x960000.r_epld.data = data;
    KUS_W(0x960000);
    return 0;
}

int32_t rf_dt3308_f::get_epld_reg(const uint8_t addr,uint8_t &data)
{
    KUS_REG_DECLARE(0x960000);
    KUS_REG_DECLARE(0x960001);
    KUS_REG_DECLARE(0x960002);

    KUS_REG(0x960001).sw = 1;
    KUS_W(0x960001);

    ns_reg_kusr0x960000.r_epld.wr = 1;
    ns_reg_kusr0x960000.r_epld.addr = addr;
    KUS_W(0x960000);

    KUS_WAIT_IDLE(0x960002,1,1000);
    KUS_R(0x960002);
    data = KUS_REG(0x960002).data;
    return 0;
}

void rf_dt3308_f::tx_freq_to_lo(const uint64_t freq)
{
    uint64_t freq_l = 0;
    uint64_t freq_r = 0;
    const uint64_t freq_step = 25000000;
    const uint64_t freq_if = 184320000;
    uint64_t freq_star = freq_step;
    common_lo_t::lo_t type = tx_lo_type(freq);

    freq_l = linear_quantify<uint64_t>(freq_star,freq_step,freq);
    freq_r = freq_l + freq_step;

    _tx_hmc833->type = type;
    _tx_hmc833->freq = (freq_l + freq_r) / 2;
    _tx_hmc833->freq += freq_if * (type == common_lo_t::HIGH ? 1 : -1);
    _tx_hmc833->freq /= 1000000;
    _tx_hmc833->freq *= 1000000;

    if (freq <= 700000000) {
        _tx_hmc833->freq += 1800000000;
        _tx_hmc832->is_opened = true;
    } else {
        _tx_hmc832->is_opened = false;
    }
}

void rf_dt3308_f::rx_freq_to_lo(const uint64_t freq)
{
    uint64_t freq_lo = 0;
    uint64_t freq_l = 0;
    uint64_t freq_r = 0;
    const uint64_t freq_step = 25000000;
    const uint64_t freq_if = 184320000;
    uint64_t freq_star = freq_step;
    common_lo_t::lo_t type = rx_lo_type(freq);

    freq_l = linear_quantify<uint64_t>(freq_star,freq_step,freq);
    freq_r = freq_l + freq_step;

    _rx_hmc833->type = type;
    freq_lo = (freq_l + freq_r) / 2;
    freq_lo += freq_if * (type == common_lo_t::HIGH ? 1 : -1);
    freq_lo -= (freq > 1700000000 && freq <= 1800000000) ? 6000000 : 0;
    freq_lo /= 1000000;
    freq_lo *= 1000000;

    if (freq <= 675000000) {
        _rx_hmc833->freq = 1800000000 - freq_if;
        _rx_hmc832->freq = _rx_hmc833->freq + freq_lo;
        _rx_hmc832->is_opened = true;
    } else {
        _rx_hmc833->freq = freq_lo;
        _rx_hmc832->is_opened = false;
    }
}

hmc83x_t rf_dt3308_f::lo_type(const lo_t lo)
{
    return (lo == TX_HMC832 || lo == RX_HMC832) ? HMC832 : HMC833;
}

common_lo_t::lo_t rf_dt3308_f::tx_lo_type(const uint64_t freq)
{
    if ((freq > 700000000   && freq <= 1000000000) ||
        (freq > 1925000000  && freq <= 2000000000) ||
        (freq >= 2110000000 && freq <= 2200000000)) {
        return common_lo_t::HIGH;
    }
    return common_lo_t::LOW;
}

common_lo_t::lo_t rf_dt3308_f::rx_lo_type(const uint64_t freq)
{
    if((freq > 900000000 && freq <= 925000000) ||
       (freq > 1625000000 && freq <= 1675000000) ||
       (freq > 1900000000 && freq <= 2025000000) ||
       (freq > 4975000000 && freq <= 6000000000)) {
        return common_lo_t::LOW;
    }
    return common_lo_t::HIGH;
}

rf_dt3308_f::tx_hmc833_filter_t rf_dt3308_f::tx_hmc833_filter(const uint64_t freq)
{
    if (freq >= 10000000 && freq <= 700000000) {
        return TX_HMC833_FILTER_10_700;
    } else if (freq > 700000000 && freq <= 1000000000) {
        return TX_HMC833_FILTER_700_1000;
    } else if (freq > 1000000000 && freq <= 2000000000) {
        return TX_HMC833_FILTER_1000_2000;
    } else if (freq > 2000000000 && freq <= 2700000000) {
        return TX_HMC833_FILTER_2000_2700;
    } else if (freq > 2700000000 && freq <= 3800000000) {
        return TX_HMC833_FILTER_2700_3800;
    } else {
        return TX_HMC833_FILTER_3800_6000;
    }
}

rf_dt3308_f::tx_hmc833_filter_t rf_dt3308_f::tx_hmc833_filter(const tx_hmc833_freq_section_t idx)
{
    tx_hmc833_filter_t tx_hmc833_filters[6] = {
        TX_HMC833_FILTER_10_700,
        TX_HMC833_FILTER_700_1000,
        TX_HMC833_FILTER_1000_2000,
        TX_HMC833_FILTER_2000_2700,
        TX_HMC833_FILTER_2700_3800,
        TX_HMC833_FILTER_3800_6000
    };
    return tx_hmc833_filters[idx];
}

rf_dt3308_f::rx_hmc833_filter_t rf_dt3308_f::rx_hmc833_filter(const uint64_t freq)
{
    if (freq >= 10000000 && freq <= 700000000) {
        return RX_HMC833_FILTER_10_700;
    } else if (freq > 700000000 && freq <= 1000000000) {
        return RX_HMC833_FILTER_700_1000;
    } else if (freq > 1000000000 && freq <= 2000000000) {
        return RX_HMC833_FILTER_1000_2000;
    } else if (freq > 2000000000 && freq <= 2700000000) {
        return RX_HMC833_FILTER_2000_2700;
    } else if (freq > 2700000000 && freq <= 4000000000) {
        return RX_HMC833_FILTER_2700_4000;
    } else {
        return RX_HMC833_FILTER_4000_6000;
    }
}

rf_dt3308_f::rx_hmc833_filter_t rf_dt3308_f::rx_hmc833_filter(const rx_hmc833_freq_section_t idx)
{
    rx_hmc833_filter_t rx_hmc833_filters[6] = {
        RX_HMC833_FILTER_10_700,
        RX_HMC833_FILTER_700_1000,
        RX_HMC833_FILTER_1000_2000,
        RX_HMC833_FILTER_2000_2700,
        RX_HMC833_FILTER_2700_4000,
        RX_HMC833_FILTER_4000_6000
    };
    return rx_hmc833_filters[idx];
}

rf_dt3308_f::tx_filter_t rf_dt3308_f::tx_filter(const uint64_t freq)
{
    if (freq >= 10000000 && freq <= 700000000) {
        return TX_FILTER_10_700;
    } else if (freq > 700000000 && freq <= 1000000000) {
        return TX_FILTER_700_1000;
    } else if (freq > 1000000000 && freq <= 2000000000) {
        return TX_FILTER_1000_2000;
    } else if (freq > 2000000000 && freq <= 2700000000) {
        return TX_FILTER_2000_2700;
    } else if (freq > 2700000000 && freq <= 3800000000) {
        return TX_FILTER_2700_3800;
    } else {
        return TX_FILTER_3800_6000;
    }
}

rf_dt3308_f::tx_filter_t rf_dt3308_f::tx_filter(const tx_freq_section_t idx)
{
    tx_filter_t tx_filters[6] = {
        TX_FILTER_10_700,
        TX_FILTER_700_1000,
        TX_FILTER_1000_2000,
        TX_FILTER_2000_2700,
        TX_FILTER_2700_3800,
        TX_FILTER_3800_6000
    };
    return tx_filters[idx];
}

rf_dt3308_f::rx_filter_t rf_dt3308_f::rx_filter(const uint64_t freq)
{
    if (freq >= 10000000 && freq <= 675000000) {
        return RX_FILTER_10_675;
    } else if (freq > 675000000 && freq <= 3800000000) {
        return RX_FILTER_675_3800;
    } else {
        return RX_FILTER_3800_6000;
    }
}

rf_dt3308_f::rx_filter_t rf_dt3308_f::rx_filter(const rx_freq_section_t idx)
{
    rx_filter_t rx_filters[3] = {
        RX_FILTER_10_675,
        RX_FILTER_675_3800,
        RX_FILTER_3800_6000
    };
    return rx_filters[idx];
}
