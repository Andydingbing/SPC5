#ifndef CAL_TABLE_H
#define CAL_TABLE_H

#include <time.h>
#include "liblog.h"
#include "libbd.h"
#include "global.hpp"

#define PM_IL 6.7 // Z28 Insertion Loss

/*****************************/
// Calibration : TX attenuation
#define R1A_TX_ATT_STEP_MIN 10
#define R1A_TX_ATT_DR 90
#define R1A_DGAIN_MAX 8
#define R1A_DGAIN_MIN -4

#define R1C_TX_ATT_STEP -1
#define R1C_DGAIN_MAX 2
#define R1C_DGAIN_MIN -8
#define R1F_DGAIN_MAX -3
#define R1F_DGAIN_MIN -13

#define R1C_TX_ATT_OP_POWER_STAR 10
#define R1C_TX_ATT_OP_POWER_STOP -90

#define R1C_TX_ATT_OP_POWER_PTS \
    (SERIE_SIZE(R1C_TX_ATT_OP_POWER_STAR,R1C_TX_ATT_OP_POWER_STOP,R1C_TX_ATT_STEP) - 1)

#define R1C_TX_ATT_IO_POWER_STAR 0
#define R1C_TX_ATT_IO_POWER_STOP -97

#define R1C_TX_ATT_IO_POWER_PTS \
    (SERIE_SIZE(R1C_TX_ATT_IO_POWER_STAR,R1C_TX_ATT_IO_POWER_STOP,R1C_TX_ATT_STEP) - 1)
// Calibration : TX attenuation
/*****************************/

/***************************/
// Calibration : RX reference
#define R1A_RX_REF_STEP    -1
#define R1A_RX_REF_OP_STAR 30
#define R1A_RX_REF_OP_STOP -40
#define R1A_RX_REF_IO_STAR 30
#define R1A_RX_REF_IO_STOP -30

#define R1C_RX_REF_STEP    -1
#define R1C_RX_REF_OP_STAR 30
#define R1C_RX_REF_OP_STOP -40
#define R1C_RX_REF_IO_STAR 30
#define R1C_RX_REF_IO_STOP -30

#define R1F_RX_REF_STEP    -1
#define R1F_RX_REF_OP_STAR 30
#define R1F_RX_REF_OP_STOP -40
#define R1F_RX_REF_IO_STAR 30
#define R1F_RX_REF_IO_STOP -30

#define R1A_RX_REF_OP_PTS \
    SERIE_SIZE(R1A_RX_REF_OP_STAR,R1A_RX_REF_OP_STOP,R1A_RX_REF_STEP)

#define R1A_RX_REF_IO_PTS \
    SERIE_SIZE(R1A_RX_REF_IO_STAR,R1A_RX_REF_IO_STOP,R1A_RX_REF_STEP)

#define R1C_RX_REF_OP_PTS \
    SERIE_SIZE(R1C_RX_REF_OP_STAR,R1C_RX_REF_OP_STOP,R1C_RX_REF_STEP)

#define R1C_RX_REF_IO_PTS \
    SERIE_SIZE(R1C_RX_REF_IO_STAR,R1C_RX_REF_IO_STOP,R1C_RX_REF_STEP)

#define R1F_RX_REF_OP_PTS \
    SERIE_SIZE(R1F_RX_REF_OP_STAR,R1F_RX_REF_OP_STOP,R1F_RX_REF_STEP)

#define R1F_RX_REF_IO_PTS \
    SERIE_SIZE(R1F_RX_REF_IO_STAR,R1F_RX_REF_IO_STOP,R1F_RX_REF_STEP)
// Calibration : RX reference
/***************************/

#define DECLARE_CAL_ITEM_TABLE(class_name,pt) \
    public: \
        class_name() { m_data_m = new data_m_t[pt]; } \
        virtual ~class_name() SAFE_DEL_ARRAY(m_data_m) \
        void virtual add(data_f_t *data); \
        void map2mem(); \
        void map2buf(data_f_t *buf){ m_data_f = buf; } \
        void save_as(const char *path); \
    private: \
        data_f_t *m_data_f; \
        data_m_t *m_data_m;

#define IMPLEMENT_TX_CAL_ITEM_TABLE(class_name) \
    void class_name::add(data_f_t *data) { \
        uint32_t idx = SERIE_INDEX(data->freq,tx_freq_star,tx_freq_step_infile); \
        m_data_f[idx] = *data; \
    }

#define IMPLEMENT_RX_CAL_ITEM_TABLE(class_name) \
    void class_name::add(data_f_t *data) { \
        uint32_t idx = SERIE_INDEX(data->freq,rx_freq_star,rx_freq_step_infile); \
        m_data_f[idx] = *data; \
    }

namespace rd  {

enum io_mode_t { IO,OUTPUT,LOOP,INPUT,CLOSE };

RD_INLINE std::string string_of(const io_mode_t mode)
{ return mode == IO ? std::string("IO") : (mode == OUTPUT ? std::string("Output") : std::string("Loop")); }

namespace ns_sp1401 {

enum hw_ver_t { HW_ERROR = -1, R1A = 0, R1B, R1C, R1D, R1E, R1F, HW_VER_SP9500_MAX };
enum bw_t { _80M, _160M };

RD_INLINE bool is_rf_ver_between(hw_ver_t ver,hw_ver_t first,hw_ver_t second)
{ return (ver >= first && ver <= second); }

RD_INLINE bool is_rf_ver_before(hw_ver_t ver,hw_ver_t max)
{ return (ver <= max); }

RD_INLINE bool is_rf_ver_after(hw_ver_t ver,hw_ver_t min)
{ return (ver >= min); }

#define TX_TROUBLES                 \
        TX_TS_BEGIN = 0x0000,       \
        TX_TS_LO_Leakage_High,      \
        TX_TS_Sideband_High,        \
        TX_TS_Passband_Ripple_High, \
        TX_TS_Transband,            \
        TX_TS_Stopband,             \
        TX_TS_Power_High,           \
        TX_TS_Power_Low,            \
        TX_TS_Att_Offset_High,      \
        TX_TS_Att_Offset_Low,       \
        TX_TS_Output_P1_Low,        \
        TX_TS_IMD3,                 \
        TX_TS_Supurious,            \
        TX_TS_MAX

#define RX_TROUBLES                 \
        RX_TS_BEGIN = 0x7fff,       \
        RX_TS_Passband_Ripple_High, \
        RX_TS_Transband,            \
        RX_TS_Stopband,             \
        RX_TS_Power_High,           \
        RX_TS_Power_Low,            \
        RX_TS_Att_Offset_High,      \
        RX_TS_Att_Offset_Low,       \
        RX_TS_Input_P1_Low,         \
        RX_TS_Output_P1_Low,        \
        RX_TS_Supurious,            \
        RX_TS_MAX

#define DECLARE_TX_TS_ENUM(name) enum name { TX_TROUBLES };
#define DECLARE_RX_TS_ENUM(name) enum name { RX_TROUBLES };

DECLARE_TX_TS_ENUM(tx_trouble_shoot_t)
DECLARE_RX_TS_ENUM(rx_trouble_shoot_t)

struct tx_trouble_shoot_item {
    tx_trouble_shoot_t t;
    bool is_attached;
    tx_trouble_shoot_item() : is_attached(false) {}
    tx_trouble_shoot_item(tx_trouble_shoot_t ts) : t(ts),is_attached(false) {}
    tx_trouble_shoot_item(uint32_t code) : t(tx_trouble_shoot_t(code)),is_attached(false) {}
};

struct rx_trouble_shoot_item {
    rx_trouble_shoot_t t;
    bool is_attached;
    rx_trouble_shoot_item() : is_attached(false) {}
    rx_trouble_shoot_item(rx_trouble_shoot_t ts) : t(ts),is_attached(false) {}
    rx_trouble_shoot_item(uint32_t code) : t(rx_trouble_shoot_t(code)),is_attached(false) {}
};

struct sn_redundant_t {
    char mac_0[32];      // CXU MAC address 0
    char mac_1[32];      // CXU MAC address 1(reserved)
    char sn_rfu[32];     // RFU SN(reserved)
    uint32_t tx_ts_num;  // Tx trouble-shoot number
    uint32_t rx_ts_num;  // Rx trouble-shoot number
    uint32_t remark_num; // User-definded remark
    sn_redundant_t() : tx_ts_num(0),rx_ts_num(0),remark_num(0) {}
};

} // namespace ns_sp1401

// common T/Rx states.
struct common_lo_t {
    enum lo_t{ HIGH,LOW };
    uint64_t freq;
    lo_t type : 8;
    bool is_opened : 8;
    uint8_t filter;
    uint8_t rsv0;
    uint32_t rsv1;
};

struct common_atts_t {
    float att[7];
    float d_gain;

    union {
        int32_t rsv[8];
        struct {
            ns_sp1401::r1a::rx_lna_att_t lna_att;
        }
        rx_sp1401_r1a,
        rx_sp1401_r1b;
        struct {
            ns_sp1401::r1c::rx_lna_att_t lna_att;
            ns_sp1401::r1c::rx_att_019_t att_019;
        }
        rx_sp1401_r1c,
        rx_sp1401_r1d,
        rx_sp1401_r1e,
        rx_sp1401_r1f;
    };
};

struct common_tmps_t {
    float temp[4];
};

} // namespace rd

#endif // CAL_TABLE_H
