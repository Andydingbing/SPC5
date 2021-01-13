#ifndef CAL_FILE_SP9500X_H
#define CAL_FILE_SP9500X_H

#include "common_cal_file.h"
#include "cal_table_tx_lol.h"
#include "cal_table_filter.h"
#include "cal_table_tx_pwr.h"

namespace rd {
namespace ns_sp9500x {

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
        make_sure_has(cal_table_t::TX_RF_FR_0000_3000,&_tx_rf_fr_0000_3000);
        make_sure_has(cal_table_t::TX_RF_FR_3000_4800,&_tx_rf_fr_3000_4800);
        make_sure_has(cal_table_t::TX_RF_FR_4800_6000,&_tx_rf_fr_4800_6000);
        make_sure_has(cal_table_t::TX_RF_FR_6000_7500,&_tx_rf_fr_6000_7500);
        make_sure_has(cal_table_t::TX_IF_FR_3000_7500,&_tx_if_fr_3000_7500);
        make_sure_has(cal_table_t::TX_Filter,&_tx_filter);
        make_sure_has(cal_table_t::TX0_Pwr_Output,&_tx0_pwr_output);
        make_sure_has(cal_table_t::TX0_Pwr_IO,&_tx0_pwr_io);
        make_sure_has(cal_table_t::TX1_Pwr,&_tx1_pwr);
        return 0;
    }

    std::string path() const;

    tx_rf_if_fr_0000_3000_table_t *tx_rf_if_fr_0000_3000_table() { return &_tx_rf_if_fr_0000_3000; }
    tx_rf_fr_0000_3000_table_t *tx_rf_fr_0000_3000() { return &_tx_rf_fr_0000_3000; }
    tx_rf_fr_3000_4800_table_t *tx_rf_fr_3000_4800() { return &_tx_rf_fr_3000_4800; }
    tx_rf_fr_4800_6000_table_t *tx_rf_fr_4800_6000() { return &_tx_rf_fr_4800_6000; }
    tx_rf_fr_6000_7500_table_t *tx_rf_fr_6000_7500() { return &_tx_rf_fr_6000_7500; }
    tx_if_fr_3000_7500_table_t *tx_if_fr_3000_7500() { return &_tx_if_fr_3000_7500; }

    tx_filter_table_t *tx_filter_table() { return &_tx_filter; }

    tx_pwr_table_t *tx0_pwr_output() { return &_tx0_pwr_output; }
    tx_pwr_table_t *tx0_pwr_io() { return &_tx0_pwr_io; }
    tx_pwr_table_t *tx1_pwr() { return &_tx1_pwr; }

private:
    tx_lol_t _tx_lol;
    tx_sb_t  _tx_sb;
    tx_rf_if_fr_0000_3000_table_t _tx_rf_if_fr_0000_3000;
    tx_rf_fr_0000_3000_table_t _tx_rf_fr_0000_3000;
    tx_rf_fr_3000_4800_table_t _tx_rf_fr_3000_4800;
    tx_rf_fr_4800_6000_table_t _tx_rf_fr_4800_6000;
    tx_rf_fr_6000_7500_table_t _tx_rf_fr_6000_7500;
    tx_if_fr_3000_7500_table_t _tx_if_fr_3000_7500;

    tx_filter_table_t _tx_filter;
    tx_pwr_table_t _tx0_pwr_output;
    tx_pwr_table_t _tx0_pwr_io;
    tx_pwr_table_t _tx1_pwr;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // CAL_FILE_SP9500X_H
