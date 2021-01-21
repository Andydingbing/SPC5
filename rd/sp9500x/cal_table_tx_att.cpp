#include "cal_table_tx_att.h"

using namespace rd;
using namespace rd::ns_sp9500x;

data_f_tx_att::data_f_tx_att()
{
    rsv0 = 0.0;

    ZERO_ARRAY(offset);
    INIT_ARRAY(temp,-100.0);
}

void tx_att_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();
    for (uint32_t i = 0;i < pts;++i) {
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::offset);++j) {
            d_m.offset[j] = d_f[i].offset[j];
        }
        _data_m.push_back(d_m);
    }
}
