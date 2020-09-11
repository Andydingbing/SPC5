#include "sp1403.h"
#include "reg_def_sp9500x.h"
#include "algorithm.h"
#include "algo_math.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace rd;
using namespace rd::ns_sp1403;
using namespace boost::gregorian;
using namespace boost::filesystem;

sp1403::sp1403(uint32_t rf_idx,uint32_t rfu_idx) :
    frontend(rf_idx,rfu_idx),
    _cal_file(nullptr),
    _v9(nullptr),
    _tx_freq(*(_tx_freqs.begin())),
    _rx_freq(*(_rx_freqs.begin()))
{
    _tx_freq = (*(_tx_freqs.begin()));
    _rx_freq = (*(_rx_freqs.begin()));

    _io_mode_tx0 = CLOSE;
    _io_mode_tx1 = CLOSE;
    _io_mode_rx  = CLOSE;
}

sp1403::~sp1403()
{
    SAFE_DEL(_cal_file);
}

bool sp1403::connect(const list<pci_dev *> &ctrller)
{
    if (ctrller.size() == 0) {
        return false;
    }

    frontend::connect(ctrller);

    _v9 = ctrller.front();
    return true;
}

int32_t sp1403::open_board()
{
//    if (_cal_file == nullptr) {
//        _cal_file = new ns_sp9500x::cal_file("c:\\RD\\cal.bin");
//    }
//    _cal_file->open();
    _tx_freq = 0;
    _rx_freq = 0;
    _io_mode_tx0 = CLOSE;
    _io_mode_tx1 = CLOSE;
    _io_mode_rx  = CLOSE;
    return 0;
}

int32_t sp1403::close_board()
{
    return 0;
}

ns_sp1403::hw_ver_t sp1403::parse_hw_ver(const string &sn)
{
     return hw_ver_t::R1A;
}

int32_t sp1403::get_ctrller_ver(const std::string &des,uint32_t &ver)
{
    boost::ignore_unused(des);

    SP9500X_RFU_V9_REG_DECL(0x0000);
    SP9500X_RFU_V9_R(0x0000);
    ver = SP9500X_RFU_V9_REG_DATA(0x0000);
    return 0;
}

uint32_t sp1403::ass_ordinal(uint32_t ordinal)
{
    uint32_t assed_ordinal = 0;
//    days days_elapsed = day_clock::local_day() - date(2019,1,1);
//    days_elapsed = days_elapsed < days(0) ? days(0) : days_elapsed;
//    assed_ordinal = (uint32_t(days_elapsed.days() << 16) | (ordinal & 0xFFFF));
    return assed_ordinal;
}

int32_t sp1403::is_valid_sn(const char *sn)
{
//    int32_t i = 0;

//    if (!strcmp(sn,"")) {
//        return SN_NULL;
//    }

//    for (i = 0;i < g_sn_length;i ++) {
//        if (sn[i] != '0') {
//            break;
//        }
//    }

//    if (g_sn_length - 1 == i) {
//        return SN_NULL;
//    }

//    bool is_valid = false;
//    char sn_hw_ver[8],sn_slot[8],sn_slot_temp[8];

//    for (uint32_t i = 0;i < sizeof(sn_hw_ver);i ++) {
//        sn_hw_ver[i] = sn[i];
//    }
//    for (uint32_t i = 0;i < sizeof(sn_slot);i ++) {
//        sn_slot[i] = sn[17 + i];
//    }
//    memset(sn_slot_temp,0,sizeof(sn_slot_temp));

//    if (!strstr(sn_hw_ver,"SP1401R1")) {
//        Log.set_last_err("sn err:%24s",sn);
//        return SN_WRONG;
//    }
//    for (int i = 0;i < g_max_rfu;i ++) {
//        for (int j = 0;j < g_max_rf;j ++) {
//            sprintf(sn_slot_temp,"RFU%dRF%d",i,j);
//            if (strstr(sn_slot,sn_slot_temp)) {
//                is_valid = true;
//                break;
//            }
//        }
//        if (is_valid) {
//            break;
//        }
//    }
//    if (is_valid == false) {
//        Log.set_last_err("sn err:%24s",sn);
//        return SN_WRONG;
//    }
//    return SN_RIGHT;
    return 0;
}
