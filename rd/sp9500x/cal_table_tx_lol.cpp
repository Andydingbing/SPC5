#include "cal_table_tx_lol.h"

using namespace rd;
using namespace rd::ns_sp9500x;

void tx_sb_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();
    for (uint32_t i = 0;i < pts;++i) {
        d_m.i = d_f[i].i;
        d_m.q = d_f[i].q;
        _data_m.push_back(d_m);
    }
}


void tx_lol_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();
    for (uint32_t i = 0;i < pts;++i) {
        d_m.i = d_f[i].i;
        d_m.q = d_f[i].q;
        _data_m.push_back(d_m);
    }
}
