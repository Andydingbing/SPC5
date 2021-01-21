#include "cal_table_tx_pwr.h"

using namespace rd;
using namespace rd::ns_sp9500x;

data_f_tx_pwr::data_f_tx_pwr()
{
    rsv0 = 0.0;
    rsv1 = 0.0;

    d_gain = 0.0;

    INIT_ARRAY(att,0);
    INIT_ARRAY(temp,-100.0);
}

void data_f_tx_pwr::set_att(const double att0,const double att1,const double att2,const double att3)
{
    att[0] = short(att0 * 2);
    att[1] = short(att1 * 2);
    att[2] = short(att2 * 2);
    att[3] = short(att3 * 2);
}

void data_f_tx_pwr::set_att(const double * const atts)
{
    att[0] = short(atts[0] * 2);
    att[1] = short(atts[1] * 2);
    att[2] = short(atts[2] * 2);
    att[3] = short(atts[3] * 2);
}

void data_f_tx_pwr::set_att(const uint8_t idx,const double _att)
{
    att[idx] = short(_att * 2);
}

void tx_pwr_table_t::map_from(void *data,uint32_t pts)
{
    data_f_t *d_f = static_cast<data_f_t *>(data);
    data_m_t d_m;

    _data_m.clear();
    for (uint32_t i = 0;i < pts;++i) {
        d_m.d_gain = d_f[i].d_gain;
        for (uint32_t j = 0;j < ARRAY_SIZE(data_f_t::att);++j) {
            d_m.att[j] = d_f[i].att[j];
        }
        _data_m.push_back(d_m);
    }
}
