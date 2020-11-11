#include "cal_table_filter.h"

using namespace rd;
using namespace rd::ns_sp9500x;

void fr_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();
    for (uint32_t i = 0;i < pts;++i) {
        d_m.pwr = d_f[i].pwr;
        _data_m.push_back(d_m);
    }
}

void tx_filter_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();
    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::coef_real);++j) {
            d_m.coef[j].real = d_f[i].coef_real[j];
            d_m.coef[j].imag = d_f[i].coef_imag[j];
        }
        _data_m.push_back(d_m);
    }
}
