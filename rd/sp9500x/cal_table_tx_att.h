#ifndef RD_SP9500X_CAL_TABLE_TX_ATT_H
#define RD_SP9500X_CAL_TABLE_TX_ATT_H

#include "cal_table.hpp"
#include "global.hpp"

namespace rd {
namespace ns_sp9500x {

struct RD_API data_f_tx_att : cal_table::basic_data_f_t
{
    float offset[120];
    float temp[5];
    float rsv0;

    data_f_tx_att();
};

struct data_m_tx_att
{
    float offset[120];
    float temp_avg;
};

class tx_att_table_t : public cal_table_data<data_f_tx_att,data_m_tx_att>
{
public:
    typedef data_f_tx_att data_f_t;
    typedef data_m_tx_att data_m_t;

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

} // namespace ns_sp9500x
} // namespace rd

#endif // RD_SP9500X_CAL_TABLE_TX_ATT_H
