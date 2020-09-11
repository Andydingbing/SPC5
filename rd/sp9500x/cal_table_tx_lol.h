#ifndef RD_SP9500X_CAL_TABLE_TX_LOL_H
#define RD_SP9500X_CAL_TABLE_TX_LOL_H

#include "cal_table.hpp"

namespace rd { namespace ns_sp9500x {

struct data_f_tx_sb : cal_table::basic_data_f_t
{
    int16_t i;
    int16_t q;
    uint32_t rsv0;
};

struct data_m_tx_sb { int16_t i; int16_t q; };

struct data_f_tx_lol : cal_table::basic_data_f_t
{
    int16_t i;
    int16_t q;
    uint32_t rsv0;
};

struct data_m_tx_lol { int16_t i; int16_t q; };

class tx_sb_t : public cal_table_data<data_f_tx_sb,data_m_tx_sb>
{
public:
    typedef data_f_tx_sb data_f_t;
    typedef data_m_tx_sb data_m_t;

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

class tx_lol_t : public cal_table_data<data_f_tx_lol,data_m_tx_lol>
{
public:
    typedef data_f_tx_lol data_f_t;
    typedef data_m_tx_lol data_m_t;

    void map_from(void *data,uint32_t pts);
    void save_as(const std::string &path);
};

} // namespace ns_sp9500x
} // namespace rd

#endif
