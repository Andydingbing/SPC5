#include "rd_dt3308.h"
#include "log.h"
#include "bb_3308_f.h"
#include "rf_3308_f.h"
#include "reg_def_ad9122.h"

using namespace rd;

static pci_dev_udp g_pci_dev;

int32_t DT3308_RF_Boot()
{
    g_pci_dev.init("172.27.246.3",65432);
    DT3308_BB_F->open_board(&g_pci_dev);
    DT3308_RF_F->connect(&g_pci_dev);
    DT3308_RF_F->open_board();

    return 0;
}

int32_t DT3308_RF_SetTxFrequency(const uint64_t Freq)
{
    AD9122_REG_DECL_0x1b;
    AD9122_REG_DECL_0x1c;
    AD9122_REG_DECL_0x1d;
    AD9122_REG_DECL_0x1e;

    double freq_if = 0.0;
    uint64_t freq_rf = 0;

    INT_CHECK(DT3308_RF_F->set_tx_freq(Freq));

    if (Freq <= 700000000) {
        freq_rf = DT3308_RF_F->tx_hmc833()->freq - DT3308_RF_F->tx_hmc832()->freq;
    } else {
        freq_rf = DT3308_RF_F->tx_hmc833()->freq;
    }

    if (DT3308_RF_F->tx_hmc833()->type == common_lo_t::HIGH) {
        freq_if = double(freq_rf) - double(Freq) - 184320000.0;

        AD9122_REG_DATA(0x1b) = 0x42;
        AD9122_REG_DATA(0x1c) = 0x06;
        AD9122_REG_DATA(0x1d) = 0x7e;
        AD9122_REG_DATA(0x1e) = 0x7f;
    } else {
        freq_if = double(Freq) - double(freq_rf) - 184320000.0;
        freq_if *= -1;

        AD9122_REG_DATA(0x1b) = 0x40;
        AD9122_REG_DATA(0x1c) = 0x02;
        AD9122_REG_DATA(0x1d) = 0x24;
        AD9122_REG_DATA(0x1e) = 0x7f;
    }

    INT_CHECK(DT3308_BB_F->set_duc(freq_if));
    INT_CHECK(DT3308_BB_F->set_da_reg(0x1b,AD9122_REG_DATA(0x1b)));
    INT_CHECK(DT3308_BB_F->set_da_reg(0x1c,AD9122_REG_DATA(0x1c)));
    INT_CHECK(DT3308_BB_F->set_da_reg(0x1d,AD9122_REG_DATA(0x1d)));
    INT_CHECK(DT3308_BB_F->set_da_reg(0x1e,AD9122_REG_DATA(0x1e)));
    return 0;
}

int32_t DT3308_RF_SetRxFrequency(const uint64_t Freq)
{
    return 0;
}
