#include "sp1403_r1b.h"
#include "sleep_common.h"
#include "reg_def_sp9500x.h"
#include "reg_def_lmx2594.h"
#include "algo_chip.hpp"

using namespace std;
using namespace rd;
using namespace rd::ns_sp1403;
using namespace rd::ns_sp1403::r1b;

typedef int32_t (sp1403_r1b::*fp_set_lo_reg)(const uint8_t addr,const uint16_t data);
typedef int32_t (sp1403_r1b::*fp_get_lo_reg)(const uint8_t addr,uint16_t &data);

sp1403_r1b::sp1403_r1b(uint32_t rf_idx,uint32_t rfu_idx) :
    sp1403_r1a(rf_idx,rfu_idx)
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

int32_t sp1403_r1b::open_board()
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

int32_t sp1403_r1b::close_board()
{
    return 0;
}

int32_t sp1403_r1b::set_tx_freq(const uint64_t freq)
{
    if (freq == _tx_freq) {
        return 0;
    }

    tx_freq_to_lo(freq);

    INT_CHECK(set_lo(TX_LMX2594_1,_tx_lmx2594_1->freq));

    bool reset_dac = false;
    double freq_dac = 0.0;
    uint8_t inversion = (freq <= FREQ_M(3000) ? 0 : 1);
    SP9500PRO_RFU_V9_REG_DECL(0x0462);
    SP9500PRO_RFU_V9_R(0x0462);
    SP9500PRO_RFU_V9_REG(0x0462).tx_0 = inversion;
    SP9500PRO_RFU_V9_REG(0x0462).tx_1 = inversion;
    SP9500PRO_RFU_V9_W(0x0462);

    if (freq <= FREQ_M(3000)) {
        reset_dac = true;
        freq_dac = freq;
        INT_CHECK(set_tx0_sw1(tx_sw1_t::TX_SW1_300_3000));
        INT_CHECK(set_tx0_sw2(tx_sw2_t::TX_SW2_300_3000));
        INT_CHECK(set_tx1_sw1(tx_sw1_t::TX_SW1_300_3000));
        INT_CHECK(set_tx1_sw2(tx_sw2_t::TX_SW2_300_3000));
    } else {
        if (_tx_freq < FREQ_M(3000)) {
            reset_dac = true;
            freq_dac = FREQ_M(1200);
        }

        if (freq <= FREQ_M(4800)) {
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
    }

    if (reset_dac) {
//        INT_CHECK(_ad908x.set_dac_duc_nco_ftw(_rf_idx,freq_dac));
//        INT_CHECK(_ad908x.set_dac_duc_nco_ftw(_rf_idx + 1,freq_dac));
    }

    _tx_freq = freq;
    return 0;
}

int32_t sp1403_r1b::set_rx_freq(const uint64_t freq)
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
    SP9500PRO_RFU_V9_REG_DECL(0x0462);
    SP9500PRO_RFU_V9_R(0x0462);
    SP9500PRO_RFU_V9_REG(0x0462).rx_0 = inversion;
    SP9500PRO_RFU_V9_REG(0x0462).rx_1 = inversion;
    SP9500PRO_RFU_V9_W(0x0462);

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
//        INT_CHECK(_ad908x.set_adc_ddc_coarse_nco_ftw(_rf_idx,freq_dac));
//        INT_CHECK(_ad908x.set_adc_ddc_coarse_nco_ftw(_rf_idx + 1,freq_dac));
    }

    _rx_freq = freq;
    return 0;
}

IMPL_SW(sp1403_r1b,0x3,rx_sw7,rx_sw7_t)

void sp1403_r1b::rx_freq_to_lo(const uint64_t freq)
{
    if (freq <= FREQ_G(1)) {
        return;
    }

    _rx_lmx2594_0->freq = FREQ_M(10800) + freq;

    if (_rx_lmx2594_0->freq > FREQ_M(14500)) {
        _rx_lmx2594_0->freq /= 2;
    }
}
