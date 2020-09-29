#ifndef CAL_FILE_SP9500X_H
#define CAL_FILE_SP9500X_H

#include "cal_file.hpp"
#include "cal_table_tx_lol.h"
#include "cal_table_filter.h"
#include "cal_file.inline.hpp"

namespace rd { namespace ns_sp9500x {

class cal_file : public basic_cal_file<cal_table_t>
{
public:
    cal_file(const std::string &path) : basic_cal_file<cal_table_t>(path) {}
    virtual ~cal_file() {}

    int32_t open()
    {
        basic_cal_file<cal_table_t>::open();
        make_sure_has(cal_table_t::TX_LO_Leakage,&_tx_lol);
        make_sure_has(cal_table_t::TX_Sideband,&_tx_sb);
        make_sure_has(cal_table_t::TX_RF_FR_0,&_tx_rf_fr_0);
        make_sure_has(cal_table_t::TX_RF_FR_1,&_tx_rf_fr_1);
        make_sure_has(cal_table_t::TX_RF_FR_2,&_tx_rf_fr_2);
        make_sure_has(cal_table_t::TX_RF_FR_3,&_tx_rf_fr_3);
        make_sure_has(cal_table_t::TX_Filter,&_tx_filter);
        return 0;
    }

    tx_filter_table_t *tx_filter_table() { return &_tx_filter; }

private:
    tx_lol_t _tx_lol;
    tx_sb_t  _tx_sb;
    fr_table_t _tx_rf_fr_0;
    fr_table_t _tx_rf_fr_1;
    fr_table_t _tx_rf_fr_2;
    fr_table_t _tx_rf_fr_3;
    tx_filter_table_t _tx_filter;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // CAL_FILE_SP9500X_H
