#include "sp9500x_sp1403_r1a.h"
#include <boost/bind/bind.hpp>
#include "sleep_common.h"
#include "reg_def_sp9500x.h"
#include "reg_def_lmx2594.h"
#include "algo_chip.hpp"

using namespace std;
using namespace boost::placeholders;
using namespace rd;
using namespace rd::ns_sp1403;
using namespace rd::ns_sp1403::r1a;

typedef int32_t (ns_sp9500x::sp1403_r1a::*fp_set_lo_reg)(const uint8_t addr,const uint16_t data);
typedef int32_t (ns_sp9500x::sp1403_r1a::*fp_get_lo_reg)(const uint8_t addr,uint16_t &data);

ns_sp9500x::sp1403_r1a::sp1403_r1a(uint32_t rf_idx,uint32_t rfu_idx) :
    rd::sp1403(rf_idx,rfu_idx),
    ns_sp9500x::sp1403()
{
    _cal_file = nullptr;
    _ad908x.set_reg = boost::bind(&sp1403_r1a::set_ad998x_reg,this,_1,_2);
    _ad908x.get_reg = boost::bind(&sp1403_r1a::get_ad998x_reg,this,_1,_2);

    _lo_tx.push_back(new common_lo_t()); _tx_lmx2594_0 = _lo_tx.back();
    _lo_tx.push_back(new common_lo_t()); _tx_lmx2594_1 = _lo_tx.back();
    _lo_tx.resize(2);
    _lo_rx.push_back(new common_lo_t()); _rx_lmx2594_0 = _lo_rx.back();
    _lo_rx.resize(1);

    _tx_lmx2594_0->type = common_lo_t::HIGH;
    _tx_lmx2594_0->is_opened = true;
    _tx_lmx2594_1->type = common_lo_t::HIGH;
    _tx_lmx2594_1->is_opened = true;
}


bool ns_sp9500x::sp1403_r1a::connect(const list<pci_dev *> &ctrller)
{
    if (ctrller.size() == 0) {
        return false;
    }

    frontend::connect(ctrller);

    _v9 = ctrller.front();
    return true;
}

bool ns_sp9500x::sp1403_r1a::is_connected()
{
    return false;
}

int32_t ns_sp9500x::sp1403_r1a::open_board()
{
    INT_CHECK(ns_sp9500x::sp1403::open_board());
    INT_CHECK(init_lo(TX_LMX2594_0));
    INT_CHECK(init_lo(TX_LMX2594_1));
    INT_CHECK(init_lo(RX_LMX2594_0));
    INT_CHECK(set_lo(TX_LMX2594_0,FREQ_M(10300)));

    INT_CHECK(set_tx_freq(FREQ_M(7500)));
    INT_CHECK(set_rx_freq(FREQ_M(7500)));

    INT_CHECK(set_att(TX0_ATT0,5.0));
    INT_CHECK(set_att(TX0_ATT1,5.0));
    INT_CHECK(set_att(TX0_ATT2,10.0));
    INT_CHECK(set_att(TX0_ATT3,8.0));

    INT_CHECK(set_att(TX1_ATT0,5.0));
    INT_CHECK(set_att(TX1_ATT1,5.0));
    INT_CHECK(set_att(TX1_ATT2,10.0));
    INT_CHECK(set_att(TX1_ATT3,8.0));

    INT_CHECK(set_rx_lna_att_sw(rx_lna_att_t::RX_LNA));
    INT_CHECK(set_att(RX_ATT0,0));
    INT_CHECK(set_att(RX_ATT1,0));
    INT_CHECK(set_rx_bw(rx_bw_t::_800M));

    INT_CHECK(set_io_mode(TX0,OUTPUT));
    INT_CHECK(set_io_mode(TX1,OUTPUT));
    INT_CHECK(set_io_mode(RX,INPUT));
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::close_board()
{
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_ctrller_ver(const std::string &des,uint32_t &ver)
{
    boost::ignore_unused(des);

    RFU_V9_REG_DECL(ver);
    RFU_V9_R(ver);
    ver = RFU_V9_REG_DATA(ver);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_io_mode(const io_mode_t mode)
{
    return set_io_mode(TX0,mode);
}

int32_t ns_sp9500x::sp1403_r1a::set_io_mode(const port_t port,const io_mode_t mode)
{
    if (mode == io_mode(port)) {
        return 0;
    }

    if (port == TX0) {
        SP1403_S6_REG_DECL(0x1);
        INT_CHECK(get_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        SP1403_S6_REG(0x1).tx0_sw5 = (mode == IO     ? tx0_sw5_t::TO_IO   : tx0_sw5_t::TO_OUT);
        SP1403_S6_REG(0x1).tx0_sw6 = (mode == OUTPUT ? tx0_sw6_t::TO_OUT  : tx0_sw6_t::TO_LOOP);
        SP1403_S6_REG(0x1).tx0_sw7 = (mode == CLOSE  ? tx0_sw7_t::TO_OFF  : tx0_sw7_t::TO_OUT);
        SP1403_S6_REG(0x1).tx0_sw8 = (mode == LOOP   ? tx0_sw8_t::TO_LOOP : tx0_sw8_t::TO_OFF);
        INT_CHECK(set_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        INT_CHECK(set_led(port,(mode == OUTPUT) ? led_t::Green : led_t::Red));
        _io_mode_tx0 = mode;
    } else if (port == TX1) {
        SP1403_S6_REG_DECL(0x2);
        INT_CHECK(get_s6_reg(0x2,SP1403_S6_REG_DATA(0x2)));
        SP1403_S6_REG(0x2).tx1_sw5 = (mode == OUTPUT ? tx1_sw5_t::TO_OUT : tx1_sw5_t::TO_LOOP);
        SP1403_S6_REG(0x2).tx1_sw6 = (mode == OUTPUT ? tx1_sw6_t::TO_OUT : tx1_sw6_t::TO_OFF);
        INT_CHECK(set_s6_reg(0x2,SP1403_S6_REG_DATA(0x2)));
        INT_CHECK(set_led(port,(mode == OUTPUT) ? led_t::Green : led_t::Red));
        _io_mode_tx1 = mode;
    } else {
        SP1403_S6_REG_DECL(0x1);
        INT_CHECK(get_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        SP1403_S6_REG(0x1).tx0_sw9  = (mode == INPUT ? tx0_sw9_t::TO_TX0_IN : tx0_sw9_t::TO_TX1_LOOP);
        SP1403_S6_REG(0x1).tx0_sw10 = (mode == INPUT ? tx0_sw10_t::TO_RX    : tx0_sw10_t::TO_TX0_LOOP);
        SP1403_S6_REG(0x1).tx0_sw11 = (mode == CLOSE ? tx0_sw11_t::TO_OFF   : tx0_sw11_t::TO_RX);
        SP1403_S6_REG(0x1).tx0_sw12 = (mode == CLOSE ? tx0_sw12_t::TO_OFF   : tx0_sw12_t::TO_RX);
        INT_CHECK(set_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        INT_CHECK(set_led(port,(mode == CLOSE || mode == LOOP) ? led_t::Red : led_t::Green));
        _io_mode_rx = mode;
    }

    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_led(const port_t port,const led_t &led) const
{
    SP1403_S6_REG_DECL(0x5);

    INT_CHECK(get_s6_reg(0x5,SP1403_S6_REG_DATA(0x5)));

    if (port == port_t::TX0) {
        SP1403_S6_REG(0x5).tx0_red   = led._to_integral() == led_t::Red ? 0 : 1;
        SP1403_S6_REG(0x5).tx0_green = SP1403_S6_REG(0x5).tx0_red == 0 ? 1 : 0;
    } else if (port == port_t::TX1) {
        SP1403_S6_REG(0x5).tx1_red   = led._to_integral() == led_t::Red ? 0 : 1;
        SP1403_S6_REG(0x5).tx1_green = SP1403_S6_REG(0x5).tx1_red == 0 ? 1 : 0;
    } else {
        SP1403_S6_REG(0x5).rx_red    = led._to_integral() == led_t::Red ? 0 : 1;
        SP1403_S6_REG(0x5).rx_green  = SP1403_S6_REG(0x5).rx_red  == 0 ? 1 : 0;
    }

    INT_CHECK(set_s6_reg(0x5,SP1403_S6_REG_DATA(0x5)));
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_led(const port_t port,led_t &led) const
{
    SP1403_S6_REG_DECL(0x5);

    INT_CHECK(get_s6_reg(0x5,SP1403_S6_REG_DATA(0x5)));

    if (port == port_t::TX0) {
        if (SP1403_S6_REG(0x5).tx0_red == SP1403_S6_REG(0x5).tx0_green) {
            Log.set_last_err("Error LED set @RF%d @port TX0",_rf_idx);
            return -1;
        }
        led = SP1403_S6_REG(0x5).tx0_green ? led_t::Red : led_t::Green;
    } else if (port == port_t::TX1) {
        if (SP1403_S6_REG(0x5).tx1_red == SP1403_S6_REG(0x5).tx1_green) {
            Log.set_last_err("Error LED set @RF%d @port TX1",_rf_idx);
            return -1;
        }
        led = SP1403_S6_REG(0x5).tx1_green ? led_t::Red : led_t::Green;
    } else {
        if (SP1403_S6_REG(0x5).rx_red == SP1403_S6_REG(0x5).rx_green) {
            Log.set_last_err("Error LED set @RF%d @port RX",_rf_idx);
            return -1;
        }
        led = SP1403_S6_REG(0x5).rx_green ? led_t::Red : led_t::Green;
    }
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_tx_freq(const uint64_t freq)
{
    if (freq == _tx_freq) {
        return 0;
    }

    tx_freq_to_lo(freq);

    INT_CHECK(set_lo(TX_LMX2594_1,_tx_lmx2594_1->freq));

    bool reset_dac = false;
    double freq_dac = 0.0;
    uint8_t inversion = (freq <= FREQ_M(3000) ? 0 : 1);
    RFU_V9_REG_DECL(0x0462);
    RFU_V9_R(0x0462);
    RFU_V9_REG(0x0462).tx_0 = inversion;
    RFU_V9_REG(0x0462).tx_1 = inversion;
    RFU_V9_W(0x0462);

    if (freq <= FREQ_M(3000)) {
        reset_dac = true;
        freq_dac = freq;
        INT_CHECK(set_tx0_sw1(tx_sw1_t::_300_3000));
        INT_CHECK(set_tx0_sw2(tx_sw2_t::_300_3000));
        INT_CHECK(set_tx1_sw1(tx_sw1_t::_300_3000));
        INT_CHECK(set_tx1_sw2(tx_sw2_t::_300_3000));
    } else {
        if (_tx_freq <= FREQ_M(3000)) {
            reset_dac = true;
            freq_dac = FREQ_M(1200);
        }

        if (freq <= FREQ_M(4800)) {
            INT_CHECK(set_tx0_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx0_sw2(tx_sw2_t::_3000_8000));
            INT_CHECK(set_tx0_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx0_sw4(tx_sw4_t::_3000_4800));
            INT_CHECK(set_tx1_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx1_sw2(tx_sw2_t::_3000_8000));
            INT_CHECK(set_tx1_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx1_sw4(tx_sw4_t::_3000_4800));
        } else if (freq <= FREQ_M(6000)) {
            INT_CHECK(set_tx0_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx0_sw2(tx_sw2_t::_3000_8000));
            INT_CHECK(set_tx0_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx0_sw4(tx_sw4_t::_4800_6000));
            INT_CHECK(set_tx1_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx1_sw2(tx_sw2_t::_3000_8000));
            INT_CHECK(set_tx1_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx1_sw4(tx_sw4_t::_4800_6000));
        } else {
            INT_CHECK(set_tx0_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx0_sw2(tx_sw2_t::_3000_8000));
            INT_CHECK(set_tx0_sw3(tx_sw3_t::_6000_8000));
            INT_CHECK(set_tx1_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx1_sw2(tx_sw2_t::_3000_8000));
            INT_CHECK(set_tx1_sw3(tx_sw3_t::_6000_8000));
        }
    }

    if (reset_dac) {
        _ad908x.set_dac_duc_nco_ftw(_rf_idx,freq_dac);
        _ad908x.set_dac_duc_nco_ftw(_rf_idx + 1,freq_dac);
    }

    _tx_freq = freq;
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_tx_att(const double att,const int32_t port)
{
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_att(const att_t att,const double value) const
{
    RFU_V9_REG_DECL_2(0x0103,0x0123);
    RFU_V9_REG_DECL_2(0x0110,0x0130);

    RFU_V9_REG_2(0x0110,0x0130).data = ns_pe43704::att_to_bit_0_25(value);
    RFU_V9_REG_2(0x0110,0x0130).addr = 0;
    RFU_V9_REG_2(0x0103,0x0123).ch = att;
    RFU_V9_W_2(0x0103,0x0123);
    RFU_V9_W_2(0x0110,0x0130);
    RFU_V9_OP_2(0x0110,0x0130);
    RFU_V9_WAIT_IDLE_2(0x0110,0x0130,0,INT_MAX);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_att(const att_t att,double &value) const
{
    RFU_V9_REG_DECL_2(0x0103,0x0123);
    RFU_V9_REG_DECL_2(0x0110,0x0130);

    RFU_V9_REG_2(0x0103,0x0123).ch = att;
    RFU_V9_W_2(0x0103,0x0123);
    RFU_V9_R_2(0x0110,0x0130);
    value = ns_pe43704::bit_0_25_to_att(RFU_V9_REG_2(0x0110,0x0130).data);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_rx_freq(const uint64_t freq)
{
    if (freq == _rx_freq) {
        return 0;
    }

    SP1403_S6_REG_DECL(0x4);

    rx_freq_to_lo(freq);

    INT_CHECK(set_lo(RX_LMX2594_0,_rx_lmx2594_0->freq));

    bool reset_dac = false;
    double freq_dac = 0.0;
    uint8_t inversion = (freq <= FREQ_M(1000) ? 0 : 1);
    RFU_V9_REG_DECL(0x0462);
    RFU_V9_R(0x0462);
    RFU_V9_REG(0x0462).rx_0 = inversion;
    RFU_V9_REG(0x0462).rx_1 = inversion;
    RFU_V9_W(0x0462);

    SP1403_S6_REG(0x4).lo_sw1 = _rx_lmx2594_0->freq < FREQ_M(9750) ? 1 : 0;
    SP1403_S6_REG(0x4).lo_sw2 = !SP1403_S6_REG(0x4).lo_sw1;
    INT_CHECK(set_s6_reg(0x4,SP1403_S6_REG_DATA(0x4)));

    if (freq < FREQ_G(1)) {
        reset_dac = true;
        freq_dac = freq;
        set_rx_sw3(rx_sw3_t::_300_1000);
    } else {
        if (_rx_freq < FREQ_G(1)) {
            reset_dac = true;
            freq_dac = FREQ_M(500);
        }
        set_rx_sw3(rx_sw3_t::_1000_8000);
    }

    if (reset_dac) {
        _ad908x.set_adc_ddc_coarse_nco_ftw(_rf_idx,freq_dac);
        _ad908x.set_adc_ddc_coarse_nco_ftw(_rf_idx + 1,freq_dac);
    }

    _rx_freq = freq;
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_rx_lna_att_sw(const rx_lna_att_t sw) const
{
    SP1403_S6_REG_DECL(0x3);

    INT_CHECK(get_s6_reg(0x3,SP1403_S6_REG_DATA(0x3)));
    SP1403_S6_REG(0x3).rx_sw1 = (sw._to_integral() == rx_lna_att_t::RX_LNA ? 1 : 0);
    SP1403_S6_REG(0x3).rx_sw2 = (sw._to_integral() == rx_lna_att_t::RX_ATT_0 ? 0 : 1);
    INT_CHECK(set_s6_reg(0x3,SP1403_S6_REG_DATA(0x3)));
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_rx_lna_att_sw(rx_lna_att_t &sw) const
{
    SP1403_S6_REG_DECL(0x3);

    INT_CHECK(get_s6_reg(0x3,SP1403_S6_REG_DATA(0x3)));

    if (SP1403_S6_REG(0x3).rx_sw1 == 1 && SP1403_S6_REG(0x3).rx_sw2 == 1) {
        sw = rx_lna_att_t::RX_LNA;
    } else if (SP1403_S6_REG(0x3).rx_sw2 == 1) {
        sw = rx_lna_att_t::RX_ATT_10;
    } else {
        sw = rx_lna_att_t::RX_ATT_0;
    }
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_temp(const temp_t &idx,double &temp) const
{
    RFU_V9_REG_DECL_2(0x0103,0x0123);
    RFU_V9_REG_DECL_2(0x0111,0x0131);
    RFU_V9_REG_DECL_2(0x0112,0x0132);
    RFU_V9_REG_DECL_2(0x0113,0x0133);

    int16_t ad = 0;

    RFU_V9_REG_2(0x0103,0x0123).ch = 10;
    RFU_V9_W_2(0x0103,0x0123);

    RFU_V9_REG_2(0x0111,0x0131).cfg = 1;
    RFU_V9_REG_2(0x0111,0x0131).incc = 7;

    RFU_V9_REG_2(0x0111,0x0131).in   = idx;
    RFU_V9_REG_2(0x0111,0x0131).bw   = 0;
    RFU_V9_REG_2(0x0111,0x0131).ref  = ns_ad7949::INTERNAL_2_5_V;
    RFU_V9_REG_2(0x0111,0x0131).seq  = 0;
    RFU_V9_REG_2(0x0111,0x0131).rb   = 1;
    RFU_V9_REG_2(0x0111,0x0131).busy = 1;

    RFU_V9_OP_2(0x0111,0x0131);
    RFU_V9_WAIT_IDLE_2(0x0113,0x0133,0,INT32_MAX);

    RFU_V9_R_2(0x0112,0x0132);
    ad = RFU_V9_REG_2(0x0112,0x0132).ad;
    ad &= 0x1FFF;

    RFU_V9_REG_2(0x0111,0x0131).op = 0;
    RFU_V9_W_2(0x0111,0x0131);

    temp = ns_tc1047::voltage_to_temp(ns_ad7949::ad_to_voltage_mv(ad,ns_ad7949::INTERNAL_2_5_V,3.0));
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::init_lo(const lo_t lo)
{
    uint16_t read_data = 0;
    LMX2594_ALL_REG_ARRAY_DECL(reg);

    LMX2594_REG(0x0e).cpg = (lo == lo_t::TX_LMX2594_0 ? 5 : 1);
    LMX2594_REG(0x2c).outa_pwr = (lo == lo_t::TX_LMX2594_0 ? 27 : 1);

    LMX2594_REG(0x00).reset = 1;
    INT_CHECK(set_lo_reg(lo,0x00,LMX2594_REG_DATA(0x00)));
    INT_CHECK(get_lo_reg(lo,0x00,read_data));

    LMX2594_REG(0x00).reset = 0;
    INT_CHECK(set_lo_reg(lo,0x00,LMX2594_REG_DATA(0x00)));

    for (int32_t i = ARRAY_SIZE(reg) - 1;i >= 0;--i) {
        INT_CHECK(set_lo_reg(lo,i,reg[i]));
    }

    sleep_ms(10);
    INT_CHECK(set_lo_reg(lo,0x00,LMX2594_REG_DATA(0x00)));
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_lo(const lo_t lo,const uint64_t freq)
{
    LMX2594_REG_DECL_0x00;
    LMX2594_REG_DECL_0x14;
    LMX2594_REG_DECL_0x24;
    LMX2594_REG_DECL_0x25;
    LMX2594_REG_DECL_0x26;
    LMX2594_REG_DECL_0x27;
    LMX2594_REG_DECL_0x2a;
    LMX2594_REG_DECL_0x2b;
    LMX2594_REG_DECL_0x2c;
    LMX2594_REG_DECL_0x2d;

    uint32_t pfd_dly_sel = 0;
    ns_lmx2594::freq_formula_in param_in;
    ns_lmx2594::freq_formula_out param_out;

    param_in.freq_rf = freq;

    ns_lmx2594::freq_formula(param_in,param_out);

    if (freq < FREQ_M(7500)) {
        pfd_dly_sel = 3;
    } else if (freq <= FREQ_M(7900)) {
        pfd_dly_sel = 2;
    } else if (freq < FREQ_M(10000)) {
        pfd_dly_sel = 3;
    } else {
        pfd_dly_sel = 4;
    }

    LMX2594_REG(0x00).en_fcal = 1;
    LMX2594_REG(0x14).vco_sel = (freq < FREQ_M(11900) || freq > FREQ_M(12100)) ? 7 : 4;
    LMX2594_REG(0x24).pll_n = param_out.N;
    LMX2594_REG(0x25).pfd_dly_sel = pfd_dly_sel;
    LMX2594_REG(0x26).pll_den = param_out.DEN >> 16;
    LMX2594_REG(0x27).pll_den = param_out.DEN & 0x0000ffff;
    LMX2594_REG(0x2a).pll_num = param_out.NUM >> 16;
    LMX2594_REG(0x2b).pll_num = param_out.NUM & 0x0000ffff;
    LMX2594_REG(0x2c).mash_order = freq > FREQ_M(7900) ? 3 : 2;
    LMX2594_REG(0x2d).outa_mux = freq < FREQ_M(7500) ? 0 : 1;

    INT_CHECK(set_lo_reg(lo,0x25,LMX2594_REG_DATA(0x25)));
    INT_CHECK(set_lo_reg(lo,0x2c,LMX2594_REG_DATA(0x2c)));

    INT_CHECK(set_lo_reg(lo,0x14,LMX2594_REG_DATA(0x14)));
    INT_CHECK(set_lo_reg(lo,0x2d,LMX2594_REG_DATA(0x2d)));
    INT_CHECK(set_lo_reg(lo,0x24,LMX2594_REG_DATA(0x24)));
    INT_CHECK(set_lo_reg(lo,0x2a,LMX2594_REG_DATA(0x2a)));
    INT_CHECK(set_lo_reg(lo,0x2b,LMX2594_REG_DATA(0x2b)));
    INT_CHECK(set_lo_reg(lo,0x26,LMX2594_REG_DATA(0x26)));
    INT_CHECK(set_lo_reg(lo,0x27,LMX2594_REG_DATA(0x27)));
    INT_CHECK(set_lo_reg(lo,0x00,LMX2594_REG_DATA(0x00)));
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_lo_reg(const lo_t lo,const uint8_t addr,const uint16_t data)
{
    static fp_set_lo_reg funcs[LO_MAX] = {
        &ns_sp9500x::sp1403_r1a::set_tx_lmx2594_0_reg,
        &ns_sp9500x::sp1403_r1a::set_tx_lmx2594_1_reg,
        &ns_sp9500x::sp1403_r1a::set_rx_lmx2594_0_reg
    };
    return (this->*funcs[lo - LO_BEGIN])(addr,data);
}

int32_t ns_sp9500x::sp1403_r1a::get_lo_reg(const lo_t lo,const uint8_t addr,uint16_t &data)
{
    static fp_get_lo_reg funcs[LO_MAX] = {
        &ns_sp9500x::sp1403_r1a::get_tx_lmx2594_0_reg,
        &ns_sp9500x::sp1403_r1a::get_tx_lmx2594_1_reg,
        &ns_sp9500x::sp1403_r1a::get_rx_lmx2594_0_reg
    };
    return (this->*funcs[lo - LO_BEGIN])(addr,data);
}

int32_t ns_sp9500x::sp1403_r1a::set_tx_lmx2594_0_reg(const uint8_t addr,const uint16_t data)
{
    RFU_V9_REG_DECL_2(0x010a,0x012a);

    RFU_V9_REG_2(0x010a,0x012a).addr = addr;
    RFU_V9_REG_2(0x010a,0x012a).data = data;
    RFU_V9_REG_2(0x010a,0x012a).wr = 0;
    RFU_V9_W_2(0x010a,0x012a);
    RFU_V9_OP_2(0x010a,0x012a);
    RFU_V9_WAIT_IDLE_2(0x010a,0x012a,0,INT_MAX);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_tx_lmx2594_0_reg(const uint8_t addr,uint16_t &data)
{
    RFU_V9_REG_DECL_2(0x010a,0x012a);
    RFU_V9_REG_DECL_2(0x010d,0x012d);

    RFU_V9_REG_2(0x010a,0x012a).addr = addr;
    RFU_V9_REG_2(0x010a,0x012a).wr = 1;
    RFU_V9_W_2(0x010a,0x012a);
    RFU_V9_OP_2(0x010a,0x012a);
    RFU_V9_WAIT_IDLE_2(0x010a,0x012a,0,INT_MAX);
    RFU_V9_R_2(0x010d,0x012d);
    data = RFU_V9_REG_2(0x010d,0x012d).data;
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_tx_lmx2594_1_reg(const uint8_t addr,const uint16_t data)
{
    RFU_V9_REG_DECL_2(0x0108,0x0128);

    RFU_V9_REG_2(0x0108,0x0128).addr = addr;
    RFU_V9_REG_2(0x0108,0x0128).data = data;
    RFU_V9_REG_2(0x0108,0x0128).wr = 0;
    RFU_V9_W_2(0x0108,0x0128);
    RFU_V9_OP_2(0x0108,0x0128);
    RFU_V9_WAIT_IDLE_2(0x0108,0x0128,0,INT_MAX);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_tx_lmx2594_1_reg(const uint8_t addr,uint16_t &data)
{
    RFU_V9_REG_DECL_2(0x0108,0x0128);
    RFU_V9_REG_DECL_2(0x010b,0x012b);

    RFU_V9_REG_2(0x0108,0x0128).addr = addr;
    RFU_V9_REG_2(0x0108,0x0128).wr = 1;
    RFU_V9_W_2(0x0108,0x0128);
    RFU_V9_OP_2(0x0108,0x0128);
    RFU_V9_WAIT_IDLE_2(0x0108,0x0128,0,INT_MAX);
    RFU_V9_R_2(0x010b,0x012b);
    data = RFU_V9_REG_2(0x010b,0x012b).data;
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_rx_lmx2594_0_reg(const uint8_t addr,const uint16_t data)
{
    RFU_V9_REG_DECL_2(0x0109,0x0129);

    RFU_V9_REG_2(0x0109,0x0129).addr = addr;
    RFU_V9_REG_2(0x0109,0x0129).data = data;
    RFU_V9_REG_2(0x0109,0x0129).wr = 0;
    RFU_V9_W_2(0x0109,0x0129);
    RFU_V9_OP_2(0x0109,0x0129);
    RFU_V9_WAIT_IDLE_2(0x0109,0x0129,0,INT_MAX);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_rx_lmx2594_0_reg(const uint8_t addr,uint16_t &data)
{
    RFU_V9_REG_DECL_2(0x0109,0x0129);
    RFU_V9_REG_DECL_2(0x010c,0x012c);

    RFU_V9_REG_2(0x0109,0x0129).addr = addr;
    RFU_V9_REG_2(0x0109,0x0129).wr = 1;
    RFU_V9_W_2(0x0109,0x0129);
    RFU_V9_OP_2(0x0109,0x0129);
    RFU_V9_WAIT_IDLE_2(0x0109,0x0129,0,INT_MAX);
    RFU_V9_R_2(0x010c,0x012c);
    data = RFU_V9_REG_2(0x010c,0x012c).data;
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::set_s6_reg(const uint8_t addr,const uint16_t data) const
{
    RFU_V9_REG_DECL_2(0x010e,0x012e);

    RFU_V9_REG_2(0x010e,0x012e).addr = addr;
    RFU_V9_REG_2(0x010e,0x012e).data = data;
    RFU_V9_REG_2(0x010e,0x012e).wr = 0;
    RFU_V9_W_2(0x010e,0x012e);
    RFU_V9_OP_2(0x010e,0x012e);
    RFU_V9_WAIT_IDLE_2(0x010e,0x012e,0,INT_MAX);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1a::get_s6_reg(const uint8_t addr,uint16_t &data) const
{
    RFU_V9_REG_DECL_2(0x010e,0x012e);
    RFU_V9_REG_DECL_2(0x010f,0x012f);

    data = 0;
    RFU_V9_REG_2(0x010e,0x012e).addr = addr;
    RFU_V9_REG_2(0x010e,0x012e).wr = 1;
    RFU_V9_W_2(0x010e,0x012e);
    RFU_V9_OP_2(0x010e,0x012e);
    RFU_V9_WAIT_IDLE_2(0x010e,0x012e,0,INT_MAX);
    RFU_V9_R_2(0x010f,0x012f);
    data = RFU_V9_REG_2(0x010f,0x012f).data;
    return 0;
}

IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw1,tx_sw1_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw2,tx_sw2_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw3,tx_sw3_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw4,tx_sw4_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw5,tx0_sw5_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw6,tx0_sw6_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw7,tx0_sw7_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw8,tx0_sw8_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw9,tx0_sw9_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw10,tx0_sw10_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw11,tx0_sw11_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x1,tx0_sw12,tx0_sw12_t)

IMPL_SW(ns_sp9500x::sp1403_r1a,0x2,tx1_sw1,tx_sw1_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x2,tx1_sw2,tx_sw2_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x2,tx1_sw3,tx_sw3_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x2,tx1_sw4,tx_sw4_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x2,tx1_sw5,tx1_sw5_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x2,tx1_sw6,tx1_sw6_t)

IMPL_SW(ns_sp9500x::sp1403_r1a,0x3,rx_sw1,rx_sw1_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x3,rx_sw2,rx_sw2_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x3,rx_sw3,rx_sw3_t)
IMPL_SW(ns_sp9500x::sp1403_r1a,0x3,rx_sw4,rx_sw4_t)

int32_t ns_sp9500x::sp1403_r1a::set_tx_state(const port_t port,const data_f_tx_pwr &data) const
{
    if (port == TX0) {
        INT_CHECK(set_att(TX0_ATT0,data_f_tx_pwr::att_double(data.att[0])));
        INT_CHECK(set_att(TX0_ATT1,data_f_tx_pwr::att_double(data.att[1])));
        INT_CHECK(set_att(TX0_ATT2,data_f_tx_pwr::att_double(data.att[2])));
        INT_CHECK(set_att(TX0_ATT3,data_f_tx_pwr::att_double(data.att[3])));
    } else if (port == TX1) {
        INT_CHECK(set_att(TX1_ATT0,data_f_tx_pwr::att_double(data.att[0])));
        INT_CHECK(set_att(TX1_ATT1,data_f_tx_pwr::att_double(data.att[1])));
        INT_CHECK(set_att(TX1_ATT2,data_f_tx_pwr::att_double(data.att[2])));
        INT_CHECK(set_att(TX1_ATT3,data_f_tx_pwr::att_double(data.att[3])));
    }

    return 0;
}

double ns_sp9500x::sp1403_r1a::tx_base_pwr(const uint64_t freq,const io_mode_t mode) const
{
    if (is_between(freq,uint64_t(FREQ_M(0)),uint64_t(FREQ_M(6500)))) {
        return mode == OUTPUT ? 0.0 : -6.0;
    } else if (is_before(freq,uint64_t(FREQ_M(7900)))) {
        return mode == OUTPUT ? -8.0 : -14.0;
    } else {
        return mode == OUTPUT ? -15.0 : -21.0;
    }
}

void ns_sp9500x::sp1403_r1a::tx_state(const uint64_t freq,const io_mode_t mode,data_f_tx_pwr &state) const
{
    boost::ignore_unused(freq,mode);

    const double att[4] = { 0.0,0.0,0.0,5.0 };
    const float d_gain = 0.0;

    state.set_att(att);
    state.d_gain = d_gain;
}

void ns_sp9500x::sp1403_r1a::tx_state(const double att,const data_f_tx_pwr &base_state,data_f_tx_pwr &state) const
{
    double att0 = data_f_tx_pwr::att_double(base_state.att[0]);
    double att1 = data_f_tx_pwr::att_double(base_state.att[1]);
    double att2 = data_f_tx_pwr::att_double(base_state.att[2]);
    double att3 = data_f_tx_pwr::att_double(base_state.att[3]);
    float  d_gain = base_state.d_gain;

    if (att < 0.0) {
        att0 -= att;
    } else if (att <= 30.0) {
        att3 = att;
    } else if (att <= 60.0) {
        att3 = 30.0;
        att2 = att - att3;
    } else if (att <= 90.0) {
        att3 = 30.0;
        att2 = 30.0;
        att1 = att - att3 - att2;
    } else {
        att3 = 30.0;
        att2 = 30.0;
        att1 = 30.0;
        att0 = att - att3 - att2 - att1;
    }

    if (att0 < 0.0) {
        d_gain -= float(att0);
        att0 = 0.0;
    }

    if (att0 > 30.0) {
        d_gain -= float(att0 - 30.0);
        att0 = 30.0;
    }

    state.set_att(att0,att1,att2,att3);
    state.d_gain = d_gain;
}
