#ifndef RD_SP9500X_CAL_TABLE_TX_PWR_H
#define RD_SP9500X_CAL_TABLE_TX_PWR_H

#include "cal_table.hpp"
#include "global.hpp"

namespace rd {
namespace ns_sp9500x {

struct RD_API data_f_tx_pwr : cal_table::basic_data_f_t
{
    float d_gain;
    float temp[5];
    float rsv0;
    float rsv1;
    short att[4];

    RD_INLINE static double att_double(const short att) { return att / 2.0; }
    RD_INLINE static float att_float(const short att) { return att / 2.0f; }

    data_f_tx_pwr();
    void set_att(const double att0,const double att1,const double att2,const double att3);
    void set_att(const double * const atts);
    void set_att(const uint8_t idx,const double _att);
};

struct data_m_tx_pwr
{
    float d_gain;
    float temp_avg;
    short att[4];
};

class tx_pwr_table_t : public cal_table_data<data_f_tx_pwr,data_m_tx_pwr>
{
public:
    typedef data_f_tx_pwr data_f_t;
    typedef data_m_tx_pwr data_m_t;

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

} // namespace ns_sp9500x
} // namespace rd

#endif // RD_SP9500X_CAL_TABLE_TX_PWR_H
