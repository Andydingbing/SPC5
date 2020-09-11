#include "rrh.h"
#include "reg_def_sp9500x.h"
#include "algorithm.h"
#include "algo_math.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace rd;
using namespace rd::ns_sp9500x;
using namespace boost::gregorian;
using namespace boost::filesystem;

rrh::rrh(uint32_t rf_idx,uint32_t rfu_idx) :
    frontend(rf_idx,rfu_idx),
    _tx_freq(*(_tx_freqs.begin())),
    _rx_freq(*(_rx_freqs.begin()))
{
    _tx_freq = (*(_tx_freqs.begin()));
    _rx_freq = (*(_rx_freqs.begin()));
    _v9 = nullptr;
}

bool rrh::connect(const list<pci_dev *> &ctrller)
{
    if (ctrller.size() == 0) {
        return false;
    }

    list<pci_dev *>::const_iterator iter = ctrller.begin();
    frontend::connect(ctrller);

    _v9 = *iter;
    return true;
}

bool rrh::is_connected()
{
    uint32_t data = 0;

    return get_reg(0,data) ? false : true;
}

int32_t rrh::open_board()
{
    INT_CHECK(frontend::open_board());
    INT_CHECK(set_tx_freq(FREQ_G(26)));
    INT_CHECK(set_rx_freq(FREQ_G(26)));
    return 0;
}

int32_t rrh::close_board()
{
    INT_CHECK(set_tx_pwr(-60.0));
    INT_CHECK(set_rx_ref(30.0));
    INT_CHECK(set_pwr_en(false));
    return 0;
}

int32_t rrh::get_ctrller_ver(const std::string &des,uint32_t &ver)
{
    boost::ignore_unused(des);

    SP9500X_RFU_V9_REG_DECL(0x0000);
    SP9500X_RFU_V9_R(0x0000);
    ver = SP9500X_RFU_V9_REG_DATA(0x0000);
    return 0;
}

int32_t rrh::set_pwr_en(const bool en)
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));
    SP9500X_RRH_REG(0x07).state = en ? 0 : 1;
    return set_reg(0x07,SP9500X_RRH_REG_DATA(0x07));
}

int32_t rrh::get_pwr_en(bool &en) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));
    en = SP9500X_RRH_REG(0x07).state == 0 ? true : false;
    return 0;
}

int32_t rrh::get_sn(uint32_t &sn) const
{
    return get_reg(0x00,sn);
}

int32_t rrh::set_io_mode(const ns_rrh::port_t port,const io_mode_t mode) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));

    if (port == ns_rrh::TRX0) {
        switch (mode) {
        case IO :     { SP9500X_RRH_REG(0x07).sw = 0; break; }
        case INPUT :  { SP9500X_RRH_REG(0x07).sw = 1; break; }
        case OUTPUT : { SP9500X_RRH_REG(0x07).sw = 2; break; }
        case CLOSE :  { SP9500X_RRH_REG(0x07).sw = 3; break; }
        default:      { return -1; }
        }
    } else {
        switch (mode) {
        case IO :     { SP9500X_RRH_REG(0x07).sw = 3; break; }
        case INPUT :  { SP9500X_RRH_REG(0x07).sw = 2; break; }
        case OUTPUT : { SP9500X_RRH_REG(0x07).sw = 1; break; }
        case CLOSE :  { SP9500X_RRH_REG(0x07).sw = 0; break; }
        default:      { return -1; }
        }
    }

    INT_CHECK(set_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));
    return 0;
}

int32_t rrh::set_io_mode(const io_mode_t mode_0,const io_mode_t mode_1) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));

    if (mode_0 == IO && mode_1 == CLOSE) {
        SP9500X_RRH_REG(0x07).sw = 0;
    } else if (mode_0 == INPUT && mode_1 == OUTPUT) {
        SP9500X_RRH_REG(0x07).sw = 1;
    } else if (mode_0 == OUTPUT && mode_1 == INPUT) {
        SP9500X_RRH_REG(0x07).sw = 2;
    } else if (mode_0 == CLOSE && mode_1 == IO) {
        SP9500X_RRH_REG(0x07).sw = 3;
    } else {
        return -1;
    }

    INT_CHECK(set_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));
    return 0;
}

int32_t rrh::get_io_mode(const ns_rrh::port_t port,io_mode_t &mode) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));

    switch (SP9500X_RRH_REG(0x07).sw) {
    case 0 : { mode = (port == ns_rrh::TRX0 ? IO : CLOSE); return 0; }
    case 1 : { mode = (port == ns_rrh::TRX0 ? INPUT : OUTPUT); return 0; }
    case 2 : { mode = (port == ns_rrh::TRX0 ? OUTPUT : INPUT); return 0; }
    case 3 : { mode = (port == ns_rrh::TRX0 ? CLOSE : IO); return 0; }
    }
    return -1;
}

int32_t rrh::get_io_mode(io_mode_t &mode_0,io_mode_t &mode_1) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));

    switch (SP9500X_RRH_REG(0x07).sw) {
    case 0 : { mode_0 = IO;     mode_1 = CLOSE;  return 0; }
    case 1 : { mode_0 = INPUT;  mode_1 = OUTPUT; return 0; }
    case 2 : { mode_0 = OUTPUT; mode_1 = INPUT;  return 0; }
    case 3 : { mode_0 = CLOSE;  mode_1 = IO;     return 0; }
    }
    return -1;
}

int32_t rrh::set_tx_freq(const uint64_t freq)
{
    if (freq == _tx_freq) {
        return 0;
    }

    if (freq == 0) {
        _tx_freq = freq;
        return 0;
    }

    uint8_t inversion = (freq <= FREQ_M(29500) ? 0 : 1);
    SP9500X_RFU_V9_REG_DECL(0x0462);
    SP9500X_RFU_V9_R(0x0462);
    SP9500X_RFU_V9_REG(0x0462).tx_0 = inversion;
    SP9500X_RFU_V9_REG(0x0462).tx_1 = inversion;
    SP9500X_RFU_V9_W(0x0462);

    INT_CHECK(set_reg(0x01,uint32_t(freq / 1000)));
    _tx_freq = freq;
    return 0;
}

int32_t rrh::set_tx_pwr(const double pwr) const
{
    SP9500X_RRH_REG_DECL(0x03);

    INT_CHECK(get_reg(0x03,SP9500X_RRH_REG_DATA(0x03)));
    SP9500X_RRH_REG(0x03).tx_pwr_sign = pwr > 0 ? 0 : 1;
    SP9500X_RRH_REG(0x03).tx_pwr = unsigned(abs(int16_t(pwr * 10.0)));
    INT_CHECK(set_reg(0x03,SP9500X_RRH_REG_DATA(0x03)));
    return 0;
}

int32_t rrh::get_tx_pwr(double &pwr) const
{
    SP9500X_RRH_REG_DECL(0x03);

    INT_CHECK(get_reg(0x03,SP9500X_RRH_REG_DATA(0x03)));
    pwr  = SP9500X_RRH_REG(0x03).tx_pwr / 10.0;
    pwr *= SP9500X_RRH_REG(0x03).tx_pwr_sign ? -1.0 : 0.0;
    return 0;
}

int32_t rrh::set_rx_freq(const uint64_t freq)
{
    if (freq == _rx_freq) {
        return 0;
    }

    if (freq == 0) {
        _rx_freq = freq;
        return 0;
    }

    uint8_t inversion = (freq <= FREQ_M(29500) ? 0 : 1);
    SP9500X_RFU_V9_REG_DECL(0x0462);
    SP9500X_RFU_V9_R(0x0462);
    SP9500X_RFU_V9_REG(0x0462).rx_0 = inversion;
    SP9500X_RFU_V9_REG(0x0462).rx_1 = inversion;
    SP9500X_RFU_V9_W(0x0462);

    INT_CHECK(set_reg(0x02,uint32_t(freq / 1000)));
    _rx_freq = freq;
    return 0;
}

int32_t rrh::set_rx_ref(const double pwr) const
{
    SP9500X_RRH_REG_DECL(0x03);

    INT_CHECK(get_reg(0x03,SP9500X_RRH_REG_DATA(0x03)));
    SP9500X_RRH_REG(0x03).rx_ref_sign = pwr > 0 ? 0 : 1;
    SP9500X_RRH_REG(0x03).rx_ref = unsigned(abs(int16_t(pwr * 10.0)));
    INT_CHECK(set_reg(0x03,SP9500X_RRH_REG_DATA(0x03)));
    return 0;
}

int32_t rrh::get_rx_ref(double &pwr) const
{
    SP9500X_RRH_REG_DECL(0x03);

    INT_CHECK(get_reg(0x03,SP9500X_RRH_REG_DATA(0x03)));
    pwr  = SP9500X_RRH_REG(0x03).rx_ref / 10.0;
    pwr *= SP9500X_RRH_REG(0x03).rx_ref_sign ? -1.0 : 0.0;
    return 0;
}

int32_t rrh::set_if_loop_sw(const bool en) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));
    SP9500X_RRH_REG(0x07).if_loop = en ? 1 : 0;
    return set_reg(0x07,SP9500X_RRH_REG_DATA(0x07));
}

int32_t rrh::get_if_loop_sw(bool &en) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));
    en = SP9500X_RRH_REG(0x07).if_loop ? true : false;
    return 0;
}

int32_t rrh::get_det(uint16_t &det) const
{
    SP9500X_RRH_REG_DECL(0x07);

    INT_CHECK(get_reg(0x07,SP9500X_RRH_REG_DATA(0x07)));
    det = SP9500X_RRH_REG(0x07).det;
    return 0;
}

int32_t rrh::set_reg(const uint8_t addr,const uint32_t data) const
{
    SP9500X_RFU_V9_REG_DECL_2(0x0105,0x0125);
    SP9500X_RFU_V9_REG_DECL_2(0x0106,0x0126);

    SP9500X_RFU_V9_REG_2(0x0105,0x0125).addr = addr;
    SP9500X_RFU_V9_REG_2(0x0105,0x0125).wr = 0;
    SP9500X_RFU_V9_REG_2(0x0106,0x0126).data = data;
    SP9500X_RFU_V9_W_2(0x0106,0x0126);
    SP9500X_RFU_V9_OP_2(0x0105,0x0125);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0105,0x0125,0,INT_MAX);
    return 0;
}

int32_t rrh::get_reg(const uint8_t addr,uint32_t &data) const
{
    SP9500X_RFU_V9_REG_DECL_2(0x0105,0x0125);
    SP9500X_RFU_V9_REG_DECL_2(0x0107,0x0127);

    SP9500X_RFU_V9_REG_2(0x0105,0x0125).addr = addr;
    SP9500X_RFU_V9_REG_2(0x0105,0x0125).wr = 1;
    SP9500X_RFU_V9_OP_2(0x0105,0x0125);
    SP9500X_RFU_V9_WAIT_IDLE_2(0x0105,0x0125,0,10000);
    SP9500X_RFU_V9_R_2(0x0107,0x0127);
    data = SP9500X_RFU_V9_REG_2(0x0107,0x0127).data;
    return 0;
}
