#ifndef RD_SP9500X_CAL_TABLE_FILTER_H
#define RD_SP9500X_CAL_TABLE_FILTER_H

#include "cal_table.hpp"
#include "global.hpp"

namespace rd {
namespace ns_sp9500x {

template<uint32_t size = 1>
struct data_f_fr : cal_table::basic_data_f_t
{ double pwr[size]; };

template<uint32_t size = 1>
struct data_m_fr
{ double pwr[size]; };

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


template<uint32_t size = 1>
class fr_table_t : public cal_table_data<data_f_fr<size>,data_m_fr<size>>
{
public:
    typedef data_f_fr<size> data_f_t;
    typedef data_m_fr<size> data_m_t;

    void map_from(void *data,uint32_t pts)
    {
        data_f_t *d_f = static_cast<data_f_t *>(data);
        data_m_t d_m;

        this->_data_m.clear();
        for (uint32_t i = 0;i < pts;++i) {
            for (uint32_t j = 0;j < size;++j) {
                d_m.pwr[j] = d_f[i].pwr[j];
            }
            this->_data_m.push_back(d_m);
        }
    }
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
