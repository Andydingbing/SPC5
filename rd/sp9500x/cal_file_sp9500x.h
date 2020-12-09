#ifndef CAL_FILE_SP9500X_H
#define CAL_FILE_SP9500X_H

#include "common_cal_file.h"
#include "cal_table_tx_lol.h"
#include "cal_table_filter.h"

namespace rd {
namespace ns_sp9500x {

typedef fr_table_t<204> tx_rf_if_fr_0000_3000_table_t;
typedef fr_table_t<1>   tx_rf_fr_3000_4800_table_t;
typedef fr_table_t<1>   tx_rf_fr_4800_6000_table_t;
typedef fr_table_t<1>   tx_rf_fr_6000_7500_table_t;
typedef fr_table_t<403> tx_if_fr_3000_7500_table_t;

class RD_API cal_file : public basic_cal_file<cal_table_t>
{
public:
    cal_file() : basic_cal_file<cal_table_t>(path()) {}
    virtual ~cal_file() {}

    int32_t open()
    {
        basic_cal_file<cal_table_t>::open();
        make_sure_has(cal_table_t::TX_LO_Leakage,&_tx_lol);
        make_sure_has(cal_table_t::TX_Sideband,&_tx_sb);
        make_sure_has(cal_table_t::TX_RF_IF_FR_0000_3000,&_tx_rf_if_fr_0000_3000);
        make_sure_has(cal_table_t::TX_RF_FR_3000_4800,&_tx_rf_fr_3000_4800);
        make_sure_has(cal_table_t::TX_RF_FR_4800_6000,&_tx_rf_fr_4800_6000);
        make_sure_has(cal_table_t::TX_RF_FR_6000_7500,&_tx_rf_fr_6000_7500);
        make_sure_has(cal_table_t::TX_IF_FR_3000_7500,&_tx_if_fr_3000_7500);
        make_sure_has(cal_table_t::TX_Filter,&_tx_filter);
        return 0;
    }

    std::string path() const;

    tx_rf_if_fr_0000_3000_table_t *tx_rf_if_fr_0000_3000_table() { return &_tx_rf_if_fr_0000_3000; }
    tx_filter_table_t *tx_filter_table() { return &_tx_filter; }

private:
    tx_lol_t _tx_lol;
    tx_sb_t  _tx_sb;
    tx_rf_if_fr_0000_3000_table_t _tx_rf_if_fr_0000_3000;
    tx_rf_fr_3000_4800_table_t _tx_rf_fr_3000_4800;
    tx_rf_fr_4800_6000_table_t _tx_rf_fr_4800_6000;
    tx_rf_fr_6000_7500_table_t _tx_rf_fr_6000_7500;
    tx_if_fr_3000_7500_table_t _tx_if_fr_3000_7500;

    tx_filter_table_t _tx_filter;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // CAL_FILE_SP9500X_H
