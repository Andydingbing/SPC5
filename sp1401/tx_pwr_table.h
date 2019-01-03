#ifndef TX_PWR_TABLE_H
#define TX_PWR_TABLE_H
#include "cal_table.h"

namespace sp_rd {

// tx power table r1a/b
class RD_API tx_pwr_table_r1ab
{
public:
    typedef struct tx_state_f_t {
        uint64_t freq;
        double	 d_gain;
        int32_t  att1;
        int32_t  att2;
        int32_t  pa_att;
        int32_t  att3;
    }tx_state_f_t;

    typedef struct RD_API data_f_t {
        tx_state_f_t state[5];
    }data_f_t;

    STRUCT_ALIGN_S(tx_state_m_t,4)
        float d_gain;
        short att1   : 8;
        short att2   : 8;
        short pa_att : 8;
        short att3   : 8;
    STRUCT_ALIGN_E(tx_state_m_t,4)

    STRUCT_ALIGN_S(data_m_t,8)
        tx_state_m_t state[5];
    STRUCT_ALIGN_E(data_m_t,8)

    DECLARE_CAL_ITEM_TABLE(tx_pwr_table_r1ab,RF_TX_FREQ_PTS_CALLED)
public:
    uint32_t get_pwr_idx(double pwr);
    static void guess_base_op(int32_t &att1,int32_t &att2,int32_t &att3);
    static void guess_base_io(int32_t &att1,int32_t &att2,int32_t &att3);
    static void split(double att,double *rf,double *d);
    static void split(int32_t att,int32_t &att1,int32_t &att2,int32_t &att3);
    void get_op(uint64_t freq,double pwr,double &d_gain,int32_t &att1,int32_t &att2,int32_t &pa_att,int32_t &att3);
    void get_io(uint64_t freq,double pwr,double &d_gain,int32_t &att1,int32_t &att2,int32_t &pa_att,int32_t &att3);
};

// tx power table r1c/d
class RD_API tx_pwr_table_r1cd
{
public:
    typedef struct RD_API data_f_t {
        uint64_t freq;
        double   d_gain;
        double   att0;
        double   att1;
        double   att2;
        double   att3;
        double   temp[4];
        tm       time;
        int32_t  rsv_0;
    public:
        data_f_t();
    }data_f_t;

    STRUCT_ALIGN_S(data_m_t,4)
        float d_gain;
        float temp_5;
        short att0 : 8;	//att = attx / 2.0,so this can be a short
        short att1 : 8;
        short att2 : 8;
        short att3 : 8;
    STRUCT_ALIGN_E(data_m_t,4)
public:
    virtual void get_base(uint64_t freq,data_m_t *data) = 0;
    virtual void get_base(uint64_t freq,data_f_t *data) = 0;
    virtual void get(uint64_t freq,double pwr,data_m_t *data) = 0;
    virtual void get(uint64_t freq,double pwr,data_f_t *data) = 0;
};

#define DECLARE_TX_PWR_TABLE_R1CD                                                   \
public:                                                                             \
    typedef tx_pwr_table_r1cd::data_f_t data_f_t;                                   \
    typedef tx_pwr_table_r1cd::data_m_t data_m_t;                                   \
    void get_base(uint64_t freq,data_m_t *data);                                    \
    void get_base(uint64_t freq,data_f_t *data);                                    \
    void get(uint64_t freq,double pwr,data_m_t *data);                              \
    void get(uint64_t freq,double pwr,data_f_t *data);                              \
    static void get_config_table(int32_t pwr,data_m_t data_base,data_m_t *data);    \
    static void guess_base(double &att0,double &att1,double &att2,double &att3,double &d_gain);

class RD_API tx_pwr_op_table_r1cd : public tx_pwr_table_r1cd
{
    DECLARE_TX_PWR_TABLE_R1CD
    DECLARE_CAL_ITEM_TABLE(tx_pwr_op_table_r1cd,RF_TX_FREQ_PTS_CALLED)
};

class RD_API tx_pwr_io_table_r1cd : public tx_pwr_table_r1cd
{
    DECLARE_TX_PWR_TABLE_R1CD
    DECLARE_CAL_ITEM_TABLE(tx_pwr_io_table_r1cd,RF_TX_FREQ_PTS_CALLED) 
};

} //namespace sp_rd

#endif // TX_PWR_TABLE_H
