#ifndef RD_SP9500X_CAL_TABLE_FILTER_H
#define RD_SP9500X_CAL_TABLE_FILTER_H

#include "cal_table.hpp"
#include "global.hpp"

namespace rd {
namespace ns_sp9500x {

struct data_f_fr_tx_rf_if_0000_3000_t : public data_f_fr<200>
{
    DATA_F_KEY_MIN(ns_sp1403::tx_freq_star)
    DATA_F_KEY_MAX(ns_sp1403::tx_freq_sec0)
};

struct data_f_fr_tx_rf_0000_3000_t : public data_f_fr<1>
{
    DATA_F_KEY_MIN(ns_sp1403::tx_freq_star)
    DATA_F_KEY_MAX(ns_sp1403::tx_freq_sec0)
};

struct data_f_fr_tx_rf_3000_4800_t : public data_f_fr<1>
{
    DATA_F_KEY_MIN(ns_sp1403::tx_freq_sec0 - FREQ_M(400))
    DATA_F_KEY_MAX(ns_sp1403::tx_freq_sec1 + FREQ_M(400))
};

struct data_f_fr_tx_rf_4800_6000_t : public data_f_fr<1>
{
    DATA_F_KEY_MIN(ns_sp1403::tx_freq_sec1 - FREQ_M(400))
    DATA_F_KEY_MAX(ns_sp1403::tx_freq_sec2 + FREQ_M(400))
};

struct data_f_fr_tx_rf_6000_7500_t : public data_f_fr<1>
{
    DATA_F_KEY_MIN(ns_sp1403::tx_freq_sec2 - FREQ_M(400))
    DATA_F_KEY_MAX(ns_sp1403::tx_freq_sec3 + FREQ_M(400))
};

struct data_f_fr_tx_if_3000_7500_t : public data_f_fr<400>
{
    data_f_fr_tx_if_3000_7500_t() {
        for (uint32_t i = 0;i < size;++i) {
            pts[i].x = FREQ_M(-400) + i * FREQ_M(2);
            pts[i].y = 0;
        }
    }
};

typedef fr_table_t<data_f_fr_tx_rf_if_0000_3000_t>  tx_rf_if_fr_0000_3000_table_t;
typedef fr_table_t<data_f_fr_tx_rf_0000_3000_t> tx_rf_fr_0000_3000_table_t;
typedef fr_table_t<data_f_fr_tx_rf_3000_4800_t> tx_rf_fr_3000_4800_table_t;
typedef fr_table_t<data_f_fr_tx_rf_4800_6000_t> tx_rf_fr_4800_6000_table_t;
typedef fr_table_t<data_f_fr_tx_rf_6000_7500_t> tx_rf_fr_6000_7500_table_t;
typedef fr_table_t<data_f_fr_tx_if_3000_7500_t> tx_if_fr_3000_7500_table_t;


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
