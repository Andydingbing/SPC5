#ifndef CAL_FILE_SP9500X_H
#define CAL_FILE_SP9500X_H

#include "cal_file.hpp"
#include "cal_table_tx_lol.h"
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
        return 0;
    }

private:
    tx_lol_t _tx_lol;
    tx_sb_t  _tx_sb;
};

} // namespace ns_sp9500x
} // namespace rd

#endif // CAL_FILE_SP9500X_H
