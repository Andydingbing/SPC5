#include "sp9500x_sp1403_r1b.h"
#include "sleep_common.h"
#include "reg_def_sp9500x.h"
#include "reg_def_lmx2594.h"
#include "algo_chip.hpp"

using namespace std;
using namespace rd;
using namespace rd::ns_sp1403;
using namespace rd::ns_sp1403::r1b;

ns_sp9500x::sp1403_r1b::sp1403_r1b(uint32_t rf_idx,uint32_t rfu_idx) :
    rd::sp1403(rf_idx,rfu_idx),
    ns_sp9500x::sp1403_r1a(rf_idx,rfu_idx)
{

}

int32_t ns_sp9500x::sp1403_r1b::open_board()
{
    INT_CHECK(ns_sp9500x::sp1403_r1a::open_board());

    INT_CHECK(set_lo(TX_LMX2594_0,FREQ_M(9500)));
    INT_CHECK(set_tx_freq(FREQ_M(7500)));
    INT_CHECK(set_rx_freq(FREQ_M(7500)));
    INT_CHECK(set_rx_sw7(rx_sw7_t::_5100));
    INT_CHECK(set_rx_bw(rx_bw_t::_800M));

    return 0;
}

int32_t ns_sp9500x::sp1403_r1b::close_board()
{
    return 0;
}

int32_t ns_sp9500x::sp1403_r1b::set_io_mode(const port_t port,const io_mode_t mode)
{
    if (mode == io_mode(port)) {
        return 0;
    }

    if (port == TX0) {
        SP1403_S6_REG_DECL(0x1);
        INT_CHECK(get_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        SP1403_S6_REG(0x1).tx0_sw5 = (mode == OUTPUT ? tx0_sw5_t::TO_IO   : tx0_sw5_t::TO_OUT);
        SP1403_S6_REG(0x1).tx0_sw6 = (mode == IO     ? tx0_sw6_t::TO_LOOP : tx0_sw6_t::TO_OUT);
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
        SP1403_S6_REG(0x1).tx0_sw9  = (mode == INPUT ? tx0_sw9_t::TO_TX1_LOOP : tx0_sw9_t::TO_TX0_IN);
        SP1403_S6_REG(0x1).tx0_sw10 = (mode == INPUT ? tx0_sw10_t::TO_RX    : tx0_sw10_t::TO_TX0_LOOP);
        SP1403_S6_REG(0x1).tx0_sw11 = (mode == CLOSE ? tx0_sw11_t::TO_OFF   : tx0_sw11_t::TO_RX);
        SP1403_S6_REG(0x1).tx0_sw12 = (mode == CLOSE ? tx0_sw12_t::TO_OFF   : tx0_sw12_t::TO_RX);
        INT_CHECK(set_s6_reg(0x1,SP1403_S6_REG_DATA(0x1)));
        INT_CHECK(set_led(port,(mode == CLOSE || mode == LOOP) ? led_t::Red : led_t::Green));
        _io_mode_rx = mode;
    }

    return 0;
}

int32_t ns_sp9500x::sp1403_r1b::set_tx_freq(const uint64_t freq)
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
        INT_CHECK(set_tx0_sw2(tx_sw2_t::_6000_8000));
        INT_CHECK(set_tx0_sw3(tx_sw3_t::_6000_8000));
        INT_CHECK(set_tx0_sw4(tx_sw4_t::_3000_4800));

        INT_CHECK(set_tx1_sw1(tx_sw1_t::_300_3000));
        INT_CHECK(set_tx1_sw2(tx_sw2_t::_6000_8000));
        INT_CHECK(set_tx1_sw3(tx_sw3_t::_6000_8000));
        INT_CHECK(set_tx1_sw4(tx_sw4_t::_3000_4800));
    } else {
        if (_tx_freq <= FREQ_M(3000)) {
            reset_dac = true;
            freq_dac = FREQ_M(2000);
        }

        if (freq <= FREQ_M(4800)) {
            INT_CHECK(set_tx0_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx0_sw2(tx_sw2_t::_3000_6000));
            INT_CHECK(set_tx0_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx0_sw4(tx_sw4_t::_3000_4800));

            INT_CHECK(set_tx1_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx1_sw2(tx_sw2_t::_3000_6000));
            INT_CHECK(set_tx1_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx1_sw4(tx_sw4_t::_3000_4800));
        } else if (freq <= FREQ_M(6000)) {
            INT_CHECK(set_tx0_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx0_sw2(tx_sw2_t::_3000_6000));
            INT_CHECK(set_tx0_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx0_sw4(tx_sw4_t::_4800_6000));

            INT_CHECK(set_tx1_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx1_sw2(tx_sw2_t::_3000_6000));
            INT_CHECK(set_tx1_sw3(tx_sw3_t::_3000_6000));
            INT_CHECK(set_tx1_sw4(tx_sw4_t::_4800_6000));
        } else {
            INT_CHECK(set_tx0_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx0_sw2(tx_sw2_t::_6000_8000));
            INT_CHECK(set_tx0_sw3(tx_sw3_t::_6000_8000));
            INT_CHECK(set_tx0_sw4(tx_sw4_t::_4800_6000));

            INT_CHECK(set_tx1_sw1(tx_sw1_t::_3000_8000));
            INT_CHECK(set_tx1_sw2(tx_sw2_t::_6000_8000));
            INT_CHECK(set_tx1_sw3(tx_sw3_t::_6000_8000));
            INT_CHECK(set_tx1_sw4(tx_sw4_t::_4800_6000));
        }
    }

    if (reset_dac) {
        _ad908x.set_dac_duc_nco_ftw(_rf_idx * 2,freq_dac);
        _ad908x.set_dac_duc_nco_ftw(_rf_idx * 2 + 1,freq_dac);
    }

    _tx_freq = freq;
    return 0;
}

int32_t ns_sp9500x::sp1403_r1b::set_rx_freq(const uint64_t freq)
{
    if (freq == _rx_freq) {
        return 0;
    }

    SP1403_S6_REG_DECL(0x3);
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
        _ad908x.set_adc_ddc_coarse_nco_ftw(_rf_idx * 2,freq_dac);
        _ad908x.set_adc_ddc_coarse_nco_ftw(_rf_idx * 2 + 1,freq_dac);
    }

    _rx_freq = freq;
    return 0;
}

int32_t ns_sp9500x::sp1403_r1b::set_rx_bw(const rx_bw_t bw) const
{
    SP1403_S6_REG_DECL(0x3);
    uint32_t bw_integral = bw._to_integral();

    INT_CHECK(get_s6_reg(0x3,SP1403_S6_REG_DATA(0x3)));

    if (bw_integral == rx_bw_t::_800M) {
        SP1403_S6_REG(0x3).rx_sw4 = 0;
        SP1403_S6_REG(0x3).rx_sw5 = 1;
        SP1403_S6_REG(0x3).rx_sw6 = 0;
    } else if (bw_integral == rx_bw_t::_400M) {
        SP1403_S6_REG(0x3).rx_sw4 = 1;
        SP1403_S6_REG(0x3).rx_sw5 = 1;
        SP1403_S6_REG(0x3).rx_sw6 = 0;
    } else if (bw_integral == rx_bw_t::_200M) {
        SP1403_S6_REG(0x3).rx_sw4 = 0;
        SP1403_S6_REG(0x3).rx_sw5 = 0;
        SP1403_S6_REG(0x3).rx_sw6 = 1;
    } else {
        SP1403_S6_REG(0x3).rx_sw4 = 1;
        SP1403_S6_REG(0x3).rx_sw5 = 0;
        SP1403_S6_REG(0x3).rx_sw6 = 1;
    }

    return set_s6_reg(0x3,SP1403_S6_REG_DATA(0x3));
}

int32_t ns_sp9500x::sp1403_r1b::get_rx_bw(rx_bw_t &bw) const
{
    SP1403_S6_REG_DECL(0x3);

    INT_CHECK(get_s6_reg(0x3,SP1403_S6_REG_DATA(0x3)));

    if (SP1403_S6_REG(0x3).rx_sw4 == 0 &&
        SP1403_S6_REG(0x3).rx_sw5 == 1 &&
        SP1403_S6_REG(0x3).rx_sw6 == 0) {
        bw = rx_bw_t::_800M;
    } else if (
        SP1403_S6_REG(0x3).rx_sw4 == 1 &&
        SP1403_S6_REG(0x3).rx_sw5 == 1 &&
        SP1403_S6_REG(0x3).rx_sw6 == 0) {
        bw = rx_bw_t::_400M;
    } else if (
        SP1403_S6_REG(0x3).rx_sw4 == 0 &&
        SP1403_S6_REG(0x3).rx_sw5 == 0 &&
        SP1403_S6_REG(0x3).rx_sw6 == 1) {
        bw = rx_bw_t::_200M;
    } else if (
        SP1403_S6_REG(0x3).rx_sw4 == 1 &&
        SP1403_S6_REG(0x3).rx_sw5 == 0 &&
        SP1403_S6_REG(0x3).rx_sw6 == 1) {
        bw = rx_bw_t::_100M;
    } else {
        Log.set_last_err("Error RX bw switch set @RF%d",_rf_idx);
    }

    return 0;
}

IMPL_SW(ns_sp9500x::sp1403_r1b,0x1,tx0_sw2,tx_sw2_t)
IMPL_SW(ns_sp9500x::sp1403_r1b,0x2,tx1_sw2,tx_sw2_t)
IMPL_SW(ns_sp9500x::sp1403_r1b,0x3,rx_sw7,rx_sw7_t)

int32_t ns_sp9500x::sp1403_r1b::set_det_sw(const det_sw_t &sw) const
{
    SP1403_S6_REG_DECL(0x4);

    INT_CHECK(get_s6_reg(0x4,SP1403_S6_REG_DATA(0x4)));

    SP1403_S6_REG(0x4).det_sw = sw;
    INT_CHECK(set_s6_reg(0x4,SP1403_S6_REG_DATA(0x4)));
    return 0;
}

int32_t ns_sp9500x::sp1403_r1b::get_det_sw(det_sw_t &sw) const
{
    SP1403_S6_REG_DECL(0x4);

    INT_CHECK(get_s6_reg(0x4,SP1403_S6_REG_DATA(0x4)));
    sw = det_sw_t::_from_integral(SP1403_S6_REG(0x4).det_sw);
    return 0;
}

int32_t ns_sp9500x::sp1403_r1b::get_ad7680(uint16_t &det) const
{
    RFU_V9_REG_DECL_2(0x0114,0x0134);
    RFU_V9_REG_DECL_2(0x0115,0x0135);
    RFU_V9_REG_DECL_2(0x0117,0x0137);

    RFU_V9_OP_2(0x0114,0x0134);
    RFU_V9_WAIT_IDLE_2(0x0117,0x0137,0,INT_MAX);
    RFU_V9_R_2(0x0115,0x0135);
    det = RFU_V9_REG_2(0x0115,0x0135).det;
    return 0;
}

void ns_sp9500x::sp1403_r1b::tx_freq_to_lo(const uint64_t freq)
{
    if (freq <= FREQ_G(3)) {
        return;
    }

    _tx_lmx2594_0->freq = FREQ_M(9500);
    _tx_lmx2594_1->freq = (freq + FREQ_M(11500)) / 2;
}

void ns_sp9500x::sp1403_r1b::rx_freq_to_lo(const uint64_t freq)
{
    if (freq <= FREQ_G(1)) {
        return;
    }

    _rx_lmx2594_0->freq = FREQ_M(10000) + freq;

    if (_rx_lmx2594_0->freq > FREQ_M(14500)) {
        _rx_lmx2594_0->freq /= 2;
    }
}
