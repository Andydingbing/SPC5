#ifndef RX_REF_TABLE_H
#define RX_REF_TABLE_H

#include "cal_table.h"

#define AVERAGE_TIMES 1

namespace sp_rd {

// rx reference table r1a/b
class RD_API rx_ref_table_r1ab
{
public:
    struct rx_state_f_t {
        int64_t ad_offset;
        int32_t lna_att;
        int32_t rsv_0;
        double  att1;
        int32_t att2;
        int32_t rsv_1;
    };

    struct data_f_t {
        uint64_t freq;
        rx_state_f_t state_op[R1A_RX_REF_OP_PTS];
        rx_state_f_t state_io[R1A_RX_REF_IO_PTS];
	public:
        data_f_t();
    };

    STRUCT_ALIGN_S(rx_state_m_t,4)
        float   att1;
        int32_t ad_offset;
        short   att2    : 8;
        short   lna_att : 8;
    STRUCT_ALIGN_E(rx_state_m_t,4)

    STRUCT_ALIGN_S(data_m_t,4)
        rx_state_m_t state_op[R1A_RX_REF_OP_PTS];
        rx_state_m_t state_io[R1A_RX_REF_IO_PTS];
    STRUCT_ALIGN_E(data_m_t,4)

    DECLARE_CAL_ITEM_TABLE(rx_ref_table_r1ab,RF_RX_FREQ_PTS_CALLED_R1A)
public:
    static int32_t get_sg_pwr(int32_t ref);
    static void guess_base_op(int32_t &lna_att,double &att1,int32_t &att2);
    static void guess_base_io(int32_t &lna_att,double &att1,int32_t &att2);
    void get(uint64_t freq,double ref,sp_rd::sp1401::io_mode_t mode,
             int64_t &ad_0dbfs,sp_rd::sp1401::rx_lna_att_t &lna_att,double &att1,int32_t &att2);
    void get_op(uint64_t freq,double ref,int64_t &ad_0dbfs,int32_t &lna_att,double &att1,int32_t &att2);
    void get_io(uint64_t freq,double ref,int64_t &ad_0dbfs,int32_t &lna_att,double &att1,int32_t &att2);
};

// RX reference table R1C/D/E/F
class RD_API rx_ref_table_r1cd
{
public:
    struct RD_API rx_state_f_t {
        int16_t lna_att;
        int16_t att_019;
        float att1;
        float att2;
        float att3;
        double temp[4];
        int64_t ad_offset;
    };

    struct RD_API data_f_t {
        uint64_t freq;
        rx_state_f_t state[3];
        tm time;
        int32_t rsv_0;
    public:
        data_f_t();
    };

    STRUCT_ALIGN_S(rx_state_m_t, 4)
        float att1;
        float att2;
        float att3;
        float temp;
        int32_t ad_offset;
        short lna_att : 8;
        short att_019 : 8;
    STRUCT_ALIGN_E(rx_state_m_t, 4)

    STRUCT_ALIGN_S(data_m_t, 4)
        rx_state_m_t state[3];
    STRUCT_ALIGN_E(data_m_t, 4)

public:
    virtual void get_base(uint64_t freq,data_m_t *data) = 0;
    virtual void get_base(uint64_t freq,data_f_t *data) = 0;
    virtual void get(sp1401::hw_ver_t,uint64_t freq,double ref,rx_state_m_t *) = 0;
    virtual void get(sp1401::hw_ver_t,uint64_t freq,double ref,rx_state_f_t *,tm *time = nullptr) = 0;
};

#define DECLARE_RX_REF_TABLE_R1CD \
public: \
    typedef rx_ref_table_r1cd::rx_state_f_t rx_state_f_t; \
    typedef rx_ref_table_r1cd::rx_state_m_t rx_state_m_t; \
    typedef rx_ref_table_r1cd::data_f_t data_f_t; \
    typedef rx_ref_table_r1cd::data_m_t data_m_t; \
    static void get_config_table_r1c(int32_t ref,data_m_t data_ref,rx_state_m_t *state); \
    static void get_config_table_r1f(int32_t ref,data_m_t data_ref,rx_state_m_t *state); \
    static void guess_base_r1c(int32_t idx,int32_t &ref,int32_t &lna_att,int32_t &att_019,double &att1,double &att2,double &att3); \
    static void guess_base_r1f(int32_t idx,int32_t &ref,int32_t &lna_att,int32_t &att_019,double &att1,double &att2); \
    static int32_t get_ref_seg_idx(double ref); \
    void get_base(uint64_t freq,data_m_t *data); \
    void get_base(uint64_t freq,data_f_t *data); \
    void get(sp1401::hw_ver_t,uint64_t freq,double ref,rx_state_m_t *); \
    void get(sp1401::hw_ver_t,uint64_t freq,double ref,rx_state_f_t *,tm *time = nullptr);

class RD_API rx_ref_op_table_r1cd : public rx_ref_table_r1cd
{
    DECLARE_RX_REF_TABLE_R1CD
    DECLARE_CAL_ITEM_TABLE(rx_ref_op_table_r1cd,RF_RX_FREQ_PTS_CALLED_R1C)
};

class RD_API rx_ref_io_table_r1cd : public rx_ref_table_r1cd
{
    DECLARE_RX_REF_TABLE_R1CD
    DECLARE_CAL_ITEM_TABLE(rx_ref_io_table_r1cd,RF_RX_FREQ_PTS_CALLED_R1C)
};

} // namespace sp_rd

#endif // RX_REF_TABLE_H
