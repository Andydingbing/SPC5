#ifndef RD_SP9500X_CAL_TABLE_FILTER_H
#define RD_SP9500X_CAL_TABLE_FILTER_H

#include "cal_table.hpp"
#include "global.hpp"

namespace rd {
namespace ns_sp9500x {

struct data_f_tx_filter : cal_table::basic_data_f_t
{
    int16_t coef_real[ns_sp2406::dl_cf_tap];
    int16_t coef_imag[ns_sp2406::dl_cf_tap];
    double  temp[4];
};

struct data_m_tx_filter
{
    coef_complex_t coef[ns_sp2406::dl_cf_tap];
};

class tx_filter_table_t : public cal_table_data<data_f_tx_filter,data_m_tx_filter>
{
public:
    typedef data_f_tx_filter data_f_t;
    typedef data_m_tx_filter data_m_t;

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

} // namespace ns_sp9500x
} // namespace rd

#endif // RD_SP9500X_CAL_TABLE_FILTER_H
