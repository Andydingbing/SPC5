#include "sp1403_r1a.h"
#include "sleep_common.h"
#include "reg_def_sp9500x.h"
#include "reg_def_lmx2594.h"
#include "algo_chip.hpp"

#define IMPL_SW(reg,sw_name,sw_enum) \
int32_t sp1403_r1a::set_##sw_name(const sw_enum &sw) const \
{ \
    SP1403_S6_REG_DECL(reg); \
    INT_CHECK(get_s6_reg(reg,SP1403_S6_REG_DATA(reg))); \
    SP1403_S6_REG(reg).sw_name = sw; \
    INT_CHECK(set_s6_reg(reg,SP1403_S6_REG_DATA(reg))); \
    return 0; \
} \
int32_t sp1403_r1a::get_##sw_name(sw_enum &sw) const \
{ \
    SP1403_S6_REG_DECL(reg); \
    INT_CHECK(get_s6_reg(reg,SP1403_S6_REG_DATA(reg))); \
    sw = sw_enum::_from_integral(SP1403_S6_REG(reg).sw_name); \
    return 0; \
}

using namespace std;
using namespace rd;
using namespace rd::ns_sp1403;
using namespace rd::ns_sp1403::r1a;

typedef int32_t (sp1403_r1a::*fp_set_lo_reg)(const uint8_t addr,const uint16_t data);
typedef int32_t (sp1403_r1a::*fp_get_lo_reg)(const uint8_t addr,uint16_t &data);

sp1403_r1a::sp1403_r1a(uint32_t rf_idx,uint32_t rfu_idx) :
    sp1403(rf_idx,rfu_idx)
{
    _lo_tx.push_back(common_lo_t()); _tx_lmx2594_0 = &(_lo_tx.back());
    _lo_tx.push_back(common_lo_t()); _tx_lmx2594_1 = &(_lo_tx.back());
    _lo_tx.resize(2);
    _lo_rx.push_back(common_lo_t()); _rx_lmx2594_0 = &(_lo_rx.back());
    _lo_rx.resize(1);

    _tx_lmx2594_0->type = common_lo_t::HIGH;
    _tx_lmx2594_0->is_opened = true;
    _tx_lmx2594_1->type = common_lo_t::HIGH;
    _tx_lmx2594_1->is_opened = true;
}

bool sp1403_r1a::is_connected()
{
    return false;
}

int32_t sp1403_r1a::open_board()
{
    INT_CHECK(sp1403::open_board());
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

int32_t sp1403_r1a::close_board()
{
    return 0;
}

int32_t sp1403_r1a::set_io_mode(const io_mode_t mode)
{
    return set_io_mode(TX0,mode);
}

int32_t sp1403_r1a::set_io_mode(const port_t port,const io_mode_t mode)
{
    if (mode == io_mode(port)) {
        return 0;
    }

    if (port == TX0) {
        SP1403_S6_REG_DECL(0x1);
        INT_CHECK(get_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        SP1403_S6_REG(0x1).tx0_sw5 = (mode == IO ? tx0_sw5_t::TO_IO : tx0_sw5_t::TO_OUT);
        SP1403_S6_REG(0x1).tx0_sw6 = (mode == OUTPUT ? tx0_sw6_t::TO_OUT : tx0_sw6_t::TO_LOOP);
        SP1403_S6_REG(0x1).tx0_sw7 = (mode == CLOSE ? tx0_sw7_t::TO_OFF : tx0_sw7_t::TO_OUT);
        SP1403_S6_REG(0x1).tx0_sw8 = (mode == LOOP ? tx0_sw8_t::TO_LOOP : tx0_sw8_t::TO_OFF);
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
        SP1403_S6_REG(0x1).tx0_sw10 = (mode == INPUT ? tx0_sw10_t::TO_RX : tx0_sw10_t::TO_TX0_LOOP);
        SP1403_S6_REG(0x1).tx0_sw11 = (mode == CLOSE ? tx0_sw11_t::TO_OFF : tx0_sw11_t::TO_RX);
        SP1403_S6_REG(0x1).tx0_sw12 = (mode == CLOSE ? tx0_sw12_t::TO_OFF : tx0_sw12_t::TO_RX);
        INT_CHECK(set_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        INT_CHECK(set_led(port,(mode == CLOSE || mode == LOOP) ? led_t::Red : led_t::Green));
        _io_mode_rx = mode;
    }

    return 0;
}

int32_t sp1403_r1a::set_led(const port_t port,const led_t &led) const
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

int32_t sp1403_r1a::get_led(const port_t port,led_t &led) const
{
    SP1403_S6_REG_DECL(0x5);

    INT_CHECK(get_s6_reg(0x5,SP1403_S6_REG_DATA(0x5)));

    if (port == port_t::TX0) {
        if (SP1403_S6_REG(0x5).tx0_red == SP1403_S6_REG(0x5).tx0_green) {
            Log.set_last_err("Error LED set of RF%d @port TX0",_rf_idx);
            return -1;
        }
        led = SP1403_S6_REG(0x5).tx0_green ? led_t::Red : led_t::Green;
    } else if (port == port_t::TX1) {
        if (SP1403_S6_REG(0x5).tx1_red == SP1403_S6_REG(0x5).tx1_green) {
            Log.set_last_err("Error LED set of RF%d @port TX1",_rf_idx);
            return -1;
        }
        led = SP1403_S6_REG(0x5).tx1_green ? led_t::Red : led_t::Green;
    } else {
        if (SP1403_S6_REG(0x5).rx_red == SP1403_S6_REG(0x5).rx_green) {
            Log.set_last_err("Error LED set of RF%d @port RX",_rf_idx);
            return -1;
        }
        led = SP1403_S6_REG(0x5).rx_green ? led_t::Red : led_t::Green;
    }
    return 0;
}

int32_t sp1403_r1a::set_tx_freq(const uint64_t freq)
{
    if (freq == _tx_freq) {
        return 0;
    }

    tx_freq_to_lo(freq);

    INT_CHECK(set_lo(TX_LMX2594_1,_tx_lmx2594_1->freq));

    uint8_t inversion = (freq <= FREQ_M(3000) ? 0 : 1);
    SP9500X_RFU_V9_REG_DECL(0x0462);
    SP9500X_RFU_V9_R(0x0462);
    SP9500X_RFU_V9_REG(0x0462).tx_0 = inversion;
    SP9500X_RFU_V9_REG(0x0462).tx_1 = inversion;
    SP9500X_RFU_V9_W(0x0462);

    if (freq <= FREQ_M(3000)) {
        INT_CHECK(set_tx0_sw1(tx_sw1_t::TX_SW1_300_3000));
        INT_CHECK(set_tx0_sw2(tx_sw2_t::TX_SW2_300_3000));
        INT_CHECK(set_tx1_sw1(tx_sw1_t::TX_SW1_300_3000));
        INT_CHECK(set_tx1_sw2(tx_sw2_t::TX_SW2_300_3000));
    } else if (freq <= FREQ_M(4800)) {
        INT_CHECK(set_tx0_sw1(tx_sw1_t::TX_SW1_3000_8000));
        INT_CHECK(set_tx0_sw2(tx_sw2_t::TX_SW2_3000_8000));
        INT_CHECK(set_tx0_sw3(tx_sw3_t::TX_SW3_3000_6000));
        INT_CHECK(set_tx0_sw4(tx_sw4_t::TX_SW4_3000_4800));
        INT_CHECK(set_tx1_sw1(tx_sw1_t::TX_SW1_3000_8000));
        INT_CHECK(set_tx1_sw2(tx_sw2_t::TX_SW2_3000_8000));
        INT_CHECK(set_tx1_sw3(tx_sw3_t::TX_SW3_3000_6000));
        INT_CHECK(set_tx1_sw4(tx_sw4_t::TX_SW4_3000_4800));
    } else if (freq <= FREQ_M(6000)) {
        INT_CHECK(set_tx0_sw1(tx_sw1_t::TX_SW1_3000_8000));
        INT_CHECK(set_tx0_sw2(tx_sw2_t::TX_SW2_3000_8000));
        INT_CHECK(set_tx0_sw3(tx_sw3_t::TX_SW3_3000_6000));
        INT_CHECK(set_tx0_sw4(tx_sw4_t::TX_SW4_4800_6000));
        INT_CHECK(set_tx1_sw1(tx_sw1_t::TX_SW1_3000_8000));
        INT_CHECK(set_tx1_sw2(tx_sw2_t::TX_SW2_3000_8000));
        INT_CHECK(set_tx1_sw3(tx_sw3_t::TX_SW3_3000_6000));
        INT_CHECK(set_tx1_sw4(tx_sw4_t::TX_SW4_4800_6000));
    } else {
        INT_CHECK(set_tx0_sw1(tx_sw1_t::TX_SW1_3000_8000));
        INT_CHECK(set_tx0_sw2(tx_sw2_t::TX_SW2_3000_8000));
        INT_CHECK(set_tx0_sw3(tx_sw3_t::TX_SW3_6000_8000));
        INT_CHECK(set_tx1_sw1(tx_sw1_t::TX_SW1_3000_8000));
        INT_CHECK(set_tx1_sw2(tx_sw2_t::TX_SW2_3000_8000));
        INT_CHECK(set_tx1_sw3(tx_sw3_t::TX_SW3_6000_8000));
    }
    _tx_freq = freq;
    return 0;
}

int32_t sp1403_r1a::set_att(const att_t att,const double value) const
{
    SP9500X_RFU_V9_REG_DECL_2(0x0103,0x0123);
    SP9500X_RFU_V9_REG_DECL_2(0x0110,0x0130);

    SP9500X_RFU_V9_REG_2(0x0110,0x0130).data = ns_pe43704::att_to_bit_0_25(value);
    SP9500X_RFU_V9_REG_2(0x0110,0x0130).addr = 0;
    SP9500X_RFU_V9_REG_2(0x0103,0x0123).ch = att;
    SP9500X_RFU_V9_W_2(0x0103,0x0123);
    SP9500X_RFU_V9_W_2(0x0110,0x0130);
    SP9500X_RFU_V9_OP_2(0x0110,0x0130);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0110,0x0130,0,INT_MAX);
    return 0;
}

int32_t sp1403_r1a::get_att(const att_t att,double &value) const
{
    SP9500X_RFU_V9_REG_DECL_2(0x0103,0x0123);
    SP9500X_RFU_V9_REG_DECL_2(0x0110,0x0130);

    SP9500X_RFU_V9_REG_2(0x0103,0x0123).ch = att;
    SP9500X_RFU_V9_W_2(0x0103,0x0123);
    SP9500X_RFU_V9_R_2(0x0110,0x0130);
    value = ns_pe43704::bit_0_25_to_att(SP9500X_RFU_V9_REG_2(0x0110,0x0130).data);
    return 0;
}

int32_t sp1403_r1a::set_rx_freq(const uint64_t freq)
{
    if (freq == _rx_freq) {
        return 0;
    }

    SP1403_S6_REG_DECL(0x3);
    SP1403_S6_REG_DECL(0x4);

    rx_freq_to_lo(freq);

    INT_CHECK(set_lo(RX_LMX2594_0,_rx_lmx2594_0->freq));

    uint8_t inversion = (freq <= FREQ_M(1000) ? 0 : 1);
    SP9500X_RFU_V9_REG_DECL(0x0462);
    SP9500X_RFU_V9_R(0x0462);
    SP9500X_RFU_V9_REG(0x0462).rx_0 = inversion;
    SP9500X_RFU_V9_REG(0x0462).rx_1 = inversion;
    SP9500X_RFU_V9_W(0x0462);

    SP1403_S6_REG(0x4).lo_sw1 = _rx_lmx2594_0->freq < FREQ_M(9750) ? 1 : 0;
    SP1403_S6_REG(0x4).lo_sw2 = !SP1403_S6_REG(0x4).lo_sw1;
    INT_CHECK(set_s6_reg(0x4,SP1403_S6_REG_DATA(0x4)));
    INT_CHECK(set_rx_sw3(freq < FREQ_G(1) ? rx_sw3_t::_300_1000 : rx_sw3_t::_1000_8000));
    _rx_freq = freq;
    return 0;
}

int32_t sp1403_r1a::set_rx_lna_att_sw(const rx_lna_att_t sw) const
{
    SP1403_S6_REG_DECL(0x3);

    INT_CHECK(get_s6_reg(0x3,SP1403_S6_REG_DATA(0x3)));
    SP1403_S6_REG(0x3).rx_sw1 = (sw._to_integral() == rx_lna_att_t::RX_LNA ? 1 : 0);
    SP1403_S6_REG(0x3).rx_sw2 = (sw._to_integral() == rx_lna_att_t::RX_ATT_0 ? 0 : 1);
    INT_CHECK(set_s6_reg(0x3,SP1403_S6_REG_DATA(0x3)));
    return 0;
}

int32_t sp1403_r1a::get_rx_lna_att_sw(rx_lna_att_t &sw) const
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

int32_t sp1403_r1a::get_temp(const temp_t &idx,double &temp) const
{
    SP9500X_RFU_V9_REG_DECL_2(0x0103,0x0123);
    SP9500X_RFU_V9_REG_DECL_2(0x0111,0x0131);
    SP9500X_RFU_V9_REG_DECL_2(0x0112,0x0132);
    SP9500X_RFU_V9_REG_DECL_2(0x0113,0x0133);

    int16_t ad = 0;

    SP9500X_RFU_V9_REG_2(0x0103,0x0123).ch = 10;
    SP9500X_RFU_V9_W_2(0x0103,0x0123);

    SP9500X_RFU_V9_REG_2(0x0111,0x0131).cfg = 1;
    SP9500X_RFU_V9_REG_2(0x0111,0x0131).incc = 7;

    SP9500X_RFU_V9_REG_2(0x0111,0x0131).in   = idx;
    SP9500X_RFU_V9_REG_2(0x0111,0x0131).bw   = 0;
    SP9500X_RFU_V9_REG_2(0x0111,0x0131).ref  = ns_ad7949::INTERNAL_2_5_V;
    SP9500X_RFU_V9_REG_2(0x0111,0x0131).seq  = 0;
    SP9500X_RFU_V9_REG_2(0x0111,0x0131).rb   = 1;
    SP9500X_RFU_V9_REG_2(0x0111,0x0131).busy = 1;

    SP9500X_RFU_V9_OP_2(0x0111,0x0131);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0113,0x0133,0,INT32_MAX);

    SP9500X_RFU_V9_R_2(0x0112,0x0132);
    ad = SP9500X_RFU_V9_REG_2(0x0112,0x0132).ad;
    ad &= 0x1FFF;

    SP9500X_RFU_V9_REG_2(0x0111,0x0131).op = 0;
    SP9500X_RFU_V9_W_2(0x0111,0x0131);

    temp = ns_tc1047::voltage_to_temp(ns_ad7949::ad_to_voltage_mv(ad,ns_ad7949::INTERNAL_2_5_V,3.0));
    return 0;
}

int32_t sp1403_r1a::init_lo(const lo_t lo)
{
    LMX2594_ALL_REG_ARRAY_DECL(reg);

    LMX2594_REG(0x2c).outa_pwr = (lo == lo_t::TX_LMX2594_0 ? 27 : 1);

    LMX2594_REG(0x00).reset = 1;
    INT_CHECK(set_lo_reg(lo,0x00,LMX2594_REG_DATA(0x00)));

    uint16_t read_data = 0;
    get_lo_reg(lo,0x00,read_data);

    LMX2594_REG(0x00).reset = 0;
    INT_CHECK(set_lo_reg(lo,0x00,LMX2594_REG_DATA(0x00)));

    for (int32_t i = ARRAY_SIZE(reg) - 1;i >= 0;--i) {
        INT_CHECK(set_lo_reg(lo,i,reg[i]));
    }

    sleep_ms(10);
    INT_CHECK(set_lo_reg(lo,0x00,LMX2594_REG_DATA(0x00)));
    return 0;
}

int32_t sp1403_r1a::set_lo(const lo_t lo,const uint64_t freq)
{
    LMX2594_REG_DECL_0x00;
    LMX2594_REG_DECL_0x14;
    LMX2594_REG_DECL_0x24;
    LMX2594_REG_DECL_0x26;
    LMX2594_REG_DECL_0x27;
    LMX2594_REG_DECL_0x2a;
    LMX2594_REG_DECL_0x2b;
    LMX2594_REG_DECL_0x2d;

    ns_lmx2594::freq_formula_in param_in;
    ns_lmx2594::freq_formula_out param_out;

    param_in.freq_rf = freq;

    ns_lmx2594::freq_formula(param_in,param_out);

    LMX2594_REG(0x00).en_fcal = 1;
    LMX2594_REG(0x14).vco_sel = (freq < FREQ_M(11900) || freq > FREQ_M(12100)) ? 7 : 4;
    LMX2594_REG(0x24).pll_n = param_out.N;
    LMX2594_REG(0x26).pll_den = param_out.DEN >> 16;
    LMX2594_REG(0x27).pll_den = param_out.DEN & 0x0000ffff;
    LMX2594_REG(0x2a).pll_num = param_out.NUM >> 16;
    LMX2594_REG(0x2b).pll_num = param_out.NUM & 0x0000ffff;
    LMX2594_REG(0x2d).outa_mux = freq < FREQ_M(7500) ? 0 : 1;

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

int32_t sp1403_r1a::set_lo_reg(const lo_t lo,const uint8_t addr,const uint16_t data)
{
    static fp_set_lo_reg funcs[LO_MAX] = {
        &sp1403_r1a::set_tx_lmx2594_0_reg,
        &sp1403_r1a::set_tx_lmx2594_1_reg,
        &sp1403_r1a::set_rx_lmx2594_0_reg
    };
    return (this->*funcs[lo - LO_BEGIN])(addr,data);
}

int32_t sp1403_r1a::get_lo_reg(const lo_t lo,const uint8_t addr,uint16_t &data)
{
    static fp_get_lo_reg funcs[LO_MAX] = {
        &sp1403_r1a::get_tx_lmx2594_0_reg,
        &sp1403_r1a::get_tx_lmx2594_1_reg,
        &sp1403_r1a::get_rx_lmx2594_0_reg
    };
    return (this->*funcs[lo - LO_BEGIN])(addr,data);
}

int32_t sp1403_r1a::set_tx_lmx2594_0_reg(const uint8_t addr,const uint16_t data)
{
    SP9500X_RFU_V9_REG_DECL_2(0x010a,0x012a);

    SP9500X_RFU_V9_REG_2(0x010a,0x012a).addr = addr;
    SP9500X_RFU_V9_REG_2(0x010a,0x012a).data = data;
    SP9500X_RFU_V9_REG_2(0x010a,0x012a).wr = 0;
    SP9500X_RFU_V9_W_2(0x010a,0x012a);
    SP9500X_RFU_V9_OP_2(0x010a,0x012a);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x010a,0x012a,0,INT_MAX);
    return 0;
}

int32_t sp1403_r1a::get_tx_lmx2594_0_reg(const uint8_t addr,uint16_t &data)
{
    SP9500X_RFU_V9_REG_DECL_2(0x010a,0x012a);
    SP9500X_RFU_V9_REG_DECL_2(0x010d,0x012d);

    SP9500X_RFU_V9_REG_2(0x010a,0x012a).addr = addr;
    SP9500X_RFU_V9_REG_2(0x010a,0x012a).wr = 1;
    SP9500X_RFU_V9_W_2(0x010a,0x012a);
    SP9500X_RFU_V9_OP_2(0x010a,0x012a);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x010a,0x012a,0,INT_MAX);
    SP9500X_RFU_V9_R_2(0x010d,0x012d);
    data = SP9500X_RFU_V9_REG_2(0x010d,0x012d).data;
    return 0;
}

int32_t sp1403_r1a::set_tx_lmx2594_1_reg(const uint8_t addr,const uint16_t data)
{
    SP9500X_RFU_V9_REG_DECL_2(0x0108,0x0128);

    SP9500X_RFU_V9_REG_2(0x0108,0x0128).addr = addr;
    SP9500X_RFU_V9_REG_2(0x0108,0x0128).data = data;
    SP9500X_RFU_V9_REG_2(0x0108,0x0128).wr = 0;
    SP9500X_RFU_V9_W_2(0x0108,0x0128);
    SP9500X_RFU_V9_OP_2(0x0108,0x0128);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0108,0x0128,0,INT_MAX);
    return 0;
}

int32_t sp1403_r1a::get_tx_lmx2594_1_reg(const uint8_t addr,uint16_t &data)
{
    SP9500X_RFU_V9_REG_DECL_2(0x0108,0x0128);
    SP9500X_RFU_V9_REG_DECL_2(0x010b,0x012b);

    SP9500X_RFU_V9_REG_2(0x0108,0x0128).addr = addr;
    SP9500X_RFU_V9_REG_2(0x0108,0x0128).wr = 1;
    SP9500X_RFU_V9_W_2(0x0108,0x0128);
    SP9500X_RFU_V9_OP_2(0x0108,0x0128);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0108,0x0128,0,INT_MAX);
    SP9500X_RFU_V9_R_2(0x010b,0x012b);
    data = SP9500X_RFU_V9_REG_2(0x010b,0x012b).data;
    return 0;
}

int32_t sp1403_r1a::set_rx_lmx2594_0_reg(const uint8_t addr,const uint16_t data)
{
    SP9500X_RFU_V9_REG_DECL_2(0x0109,0x0129);

    SP9500X_RFU_V9_REG_2(0x0109,0x0129).addr = addr;
    SP9500X_RFU_V9_REG_2(0x0109,0x0129).data = data;
    SP9500X_RFU_V9_REG_2(0x0109,0x0129).wr = 0;
    SP9500X_RFU_V9_W_2(0x0109,0x0129);
    SP9500X_RFU_V9_OP_2(0x0109,0x0129);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0109,0x0129,0,INT_MAX);
    return 0;
}

int32_t sp1403_r1a::get_rx_lmx2594_0_reg(const uint8_t addr,uint16_t &data)
{
    SP9500X_RFU_V9_REG_DECL_2(0x0109,0x0129);
    SP9500X_RFU_V9_REG_DECL_2(0x010c,0x012c);

    SP9500X_RFU_V9_REG_2(0x0109,0x0129).addr = addr;
    SP9500X_RFU_V9_REG_2(0x0109,0x0129).wr = 1;
    SP9500X_RFU_V9_W_2(0x0109,0x0129);
    SP9500X_RFU_V9_OP_2(0x0109,0x0129);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0109,0x0129,0,INT_MAX);
    SP9500X_RFU_V9_R_2(0x010c,0x012c);
    data = SP9500X_RFU_V9_REG_2(0x010c,0x012c).data;
    return 0;
}

int32_t sp1403_r1a::set_s6_reg(const uint8_t addr,const uint16_t data) const
{
    SP9500X_RFU_V9_REG_DECL_2(0x010e,0x012e);

    SP9500X_RFU_V9_REG_2(0x010e,0x012e).addr = addr;
    SP9500X_RFU_V9_REG_2(0x010e,0x012e).data = data;
    SP9500X_RFU_V9_REG_2(0x010e,0x012e).wr = 0;
    SP9500X_RFU_V9_W_2(0x010e,0x012e);
    SP9500X_RFU_V9_OP_2(0x010e,0x012e);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x010e,0x012e,0,INT_MAX);
    return 0;
}

int32_t sp1403_r1a::get_s6_reg(const uint8_t addr,uint16_t &data) const
{
    SP9500X_RFU_V9_REG_DECL_2(0x010e,0x012e);
    SP9500X_RFU_V9_REG_DECL_2(0x010f,0x012f);

    data = 0;
    SP9500X_RFU_V9_REG_2(0x010e,0x012e).addr = addr;
    SP9500X_RFU_V9_REG_2(0x010e,0x012e).wr = 1;
    SP9500X_RFU_V9_W_2(0x010e,0x012e);
    SP9500X_RFU_V9_OP_2(0x010e,0x012e);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x010e,0x012e,0,INT_MAX);
    SP9500X_RFU_V9_R_2(0x010f,0x012f);
    data = SP9500X_RFU_V9_REG_2(0x010f,0x012f).data;
    return 0;
}

IMPL_SW(0x1,tx0_sw1,tx_sw1_t)
IMPL_SW(0x1,tx0_sw2,tx_sw2_t)
IMPL_SW(0x1,tx0_sw3,tx_sw3_t)
IMPL_SW(0x1,tx0_sw4,tx_sw4_t)
IMPL_SW(0x1,tx0_sw5,tx0_sw5_t)
IMPL_SW(0x1,tx0_sw6,tx0_sw6_t)
IMPL_SW(0x1,tx0_sw7,tx0_sw7_t)
IMPL_SW(0x1,tx0_sw8,tx0_sw8_t)
IMPL_SW(0x1,tx0_sw9,tx0_sw9_t)
IMPL_SW(0x1,tx0_sw10,tx0_sw10_t)
IMPL_SW(0x1,tx0_sw11,tx0_sw11_t)
IMPL_SW(0x1,tx0_sw12,tx0_sw12_t)

IMPL_SW(0x2,tx1_sw1,tx_sw1_t)
IMPL_SW(0x2,tx1_sw2,tx_sw2_t)
IMPL_SW(0x2,tx1_sw3,tx_sw3_t)
IMPL_SW(0x2,tx1_sw4,tx_sw4_t)
IMPL_SW(0x2,tx1_sw5,tx1_sw5_t)
IMPL_SW(0x2,tx1_sw6,tx1_sw6_t)

IMPL_SW(0x3,rx_sw1,rx_sw1_t)
IMPL_SW(0x3,rx_sw2,rx_sw2_t)
IMPL_SW(0x3,rx_sw3,rx_sw3_t)
IMPL_SW(0x3,rx_sw4,rx_sw4_t)

void sp1403_r1a::tx_freq_to_lo(const uint64_t freq)
{
    if (freq <= FREQ_G(3)) {
        return;
    }

    _tx_lmx2594_0->freq = FREQ_M(10300);
    _tx_lmx2594_1->freq = (freq + FREQ_M(11500)) / 2;
}

void sp1403_r1a::rx_freq_to_lo(const uint64_t freq)
{
    if (freq <= FREQ_G(1)) {
        return;
    }

    _rx_lmx2594_0->freq = FREQ_M(10800) + freq;

    if (_rx_lmx2594_0->freq > FREQ_M(14500)) {
        _rx_lmx2594_0->freq /= 2;
    }
}
