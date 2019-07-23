#ifndef CAL_TABLE_H
#define CAL_TABLE_H

#include <time.h>
#include "liblog.h"
#include "libbd.h"

#define MAX_RFU	5
#define MAX_RF	4
#define MAX_CEU	2

#define NUM_BLOWER 11
#define MAX_BW	160000000LL

#define ADF5355_FREQ_SPACE 6250
#define RF_FREQ_SPACE 1000000

#define RF_SN_LENGTH 24
#define PM_IL 6.7   //Z28 Insertion Loss
#define _0dBFS  80000000LL

#define TX_FILTER_ORDER_2I   41
#define RX_FILTER_ORDER      96
#define RX_FILTER_ORDER_2I3D 144

/******************************************/
// TX frequency   R1A/B/C/D/E/F  50M:step:6G
#define RF_TX_FREQ_STAR 50000000LL
#define RF_TX_FREQ_STOP 6000000000LL
#define RF_TX_FREQ_STEP_CALLED 10000000LL // Step 10M called/memory
#define RF_TX_FREQ_STEP_INFILE 5000000LL  // Step 5M in file

#define RF_TX_FREQ_PTS_CALLED \
    (SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED))

#define RF_TX_FREQ_PTS_INFILE \
    (SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_INFILE))
// TX frequency   R1A/B/C/D/E/F  50M:step:6G
/******************************************/

/************************/
// Calibration : TX filter
#define RF_TX_FILTER_FREQ_STAR_0 17000000LL   // (100  - 83) * 1000000
#define RF_TX_FILTER_FREQ_STOP_0 3183000000LL // (3100 + 83) * 1000000
#define RF_TX_FILTER_FREQ_STAR_1 3017000000LL // (3100 - 83) * 1000000
#define RF_TX_FILTER_FREQ_STOP_1 6083000000LL // (6000 + 83) * 1000000
#define RF_TX_FILTER_FREQ_STEP   2000000LL

#define IF_TX_FILTER_160M_FREQ_STAR	-83000000.0 // (160 / 2) + 1.5 * 2
#define IF_TX_FILTER_160M_FREQ_STOP	83000000.0
#define IF_TX_FILTER_80M_FREQ_STAR  -43000000.0 // (80  / 2) + 1.5 * 2
#define IF_TX_FILTER_80M_FREQ_STOP	43000000.0
#define IF_TX_FILTER_FREQ_STEP      2000000.0

#define RF_TX_FILTER_PTS_0 \
    SERIE_SIZE(RF_TX_FILTER_FREQ_STAR_0,RF_TX_FILTER_FREQ_STOP_0,RF_TX_FILTER_FREQ_STEP)

#define RF_TX_FILTER_PTS_1 \
    SERIE_SIZE(RF_TX_FILTER_FREQ_STAR_1,RF_TX_FILTER_FREQ_STOP_1,RF_TX_FILTER_FREQ_STEP)

#define RF_TX_FILTER_PTS \
    (RF_TX_FILTER_PTS_0 + RF_TX_FILTER_PTS_1)

#define IF_TX_FILTER_160M_PTS \
    SERIE_SIZE(IF_TX_FILTER_160M_FREQ_STAR,IF_TX_FILTER_160M_FREQ_STOP,IF_TX_FILTER_FREQ_STEP)

#define IF_TX_FILTER_80M_PTS \
    SERIE_SIZE(IF_TX_FILTER_80M_FREQ_STAR,IF_TX_FILTER_80M_FREQ_STOP,IF_TX_FILTER_FREQ_STEP)
// Calibration : TX filter
/************************/

#define RF_TX_TEST_FILTER_PTS (SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FILTER_FREQ_STOP_1,RF_TX_FILTER_FREQ_STEP))

/***********************/
// Calibration : TX power
#define R1A_TX_POWER_STAR 10
#define R1A_TX_POWER_STOP -20
#define R1A_TX_POWER_STEP -10

#define R1C_TX_BASE_POWER_OP -5.0
#define R1C_TX_BASE_POWER_IO -12.0
// Calibration : TX power
/***********************/

#define RF_TX_TEST_FREQ_STEP 50000000LL		//r1a/b/c/d
#define RF_TX_TEST_FREQ_STAR 1000000000LL		//r1a/b/c/d
#define RF_TX_TEST_FREQ_STOP 6000000000LL	//r1a/b/c/d
#define RF_TX_TEST_FREQ_PTS (SERIE_SIZE(RF_TX_TEST_FREQ_STAR,RF_TX_TEST_FREQ_STOP,RF_TX_TEST_FREQ_STEP))


/*****************************/
// Calibration : TX attenuation
#define R1A_TX_ATT_STEP_MIN 10
#define R1A_TX_ATT_STEP_MAX 10
#define R1A_TX_ATT_DR 90
#define R1A_DGAIN_MAX 8
#define R1A_DGAIN_MIN -4

#define R1C_TX_ATT_STEP     -1
#define R1C_TX_ATT_STEP_MIN 0.5
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

#define RF_RX_FREQ_STAR             100000000LL		//r1a/b/c/d
#define RF_RX_FREQ_STOP             6000000000LL	//r1a/b/c/d
#define RF_RX_FREQ_STEP_CALLED_R1A  5000000LL		//r1a/b fixed
#define RF_RX_FREQ_STEP_CALLED_R1C  10000000LL		//r1c/d
#define RF_RX_FREQ_STEP_INFILE      5000000LL		//r1c/d
#define RF_RX_FREQ_PTS_CALLED_R1A   (SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A))
#define RF_RX_FREQ_PTS_CALLED_R1C   (SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1C))
#define RF_RX_FREQ_PTS_INFILE       (SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_INFILE))

#define RF_RX_TEST_FREQ_STAR 1000000000LL
#define RF_RX_TEST_FREQ_STOP 6000000000LL
#define RF_RX_TEST_FREQ_STEP 50000000LL
#define RF_RX_TEST_PTS       (SERIE_SIZE(RF_RX_TEST_FREQ_STAR,RF_RX_TEST_FREQ_STOP,RF_RX_TEST_FREQ_STEP))

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

/************************/
// Calibration : RX filter
#define RF_RX_FILTER_FREQ_STAR 17000000LL   // (100  - 83) * 1000000
#define RF_RX_FILTER_FREQ_STOP 6083000000LL	// (6000 + 83) * 1000000
#define RF_RX_FILTER_FREQ_STEP 2000000LL

#define IF_RX_FILTER_160M_FREQ_STAR -83000000.0 // (160 / 2) + 1.5 * 2
#define IF_RX_FILTER_160M_FREQ_STOP	83000000.0
#define IF_RX_FILTER_80M_FREQ_STAR	-43000000.0 // (80  / 2) + 1.5 * 2
#define IF_RX_FILTER_80M_FREQ_STOP	43000000.0
#define IF_RX_FILTER_FREQ_STEP      2000000.0

#define RF_RX_FILTER_PTS \
    SERIE_SIZE(RF_RX_FILTER_FREQ_STAR,RF_RX_FILTER_FREQ_STOP,RF_RX_FILTER_FREQ_STEP)

#define IF_RX_FILTER_160M_PTS \
    SERIE_SIZE(IF_RX_FILTER_160M_FREQ_STAR,IF_RX_FILTER_160M_FREQ_STOP,IF_RX_FILTER_FREQ_STEP)

#define IF_RX_FILTER_80M_PTS \
    SERIE_SIZE(IF_RX_FILTER_80M_FREQ_STAR,IF_RX_FILTER_80M_FREQ_STOP,IF_RX_FILTER_FREQ_STEP)
// Calibration : RX filter
/************************/


#define DECLARE_CAL_ITEM_TABLE(class_name,pt) \
    public: \
        class_name() { \
            m_data_m = new data_m_t[pt]; \
        } \
        virtual ~class_name() SAFE_DEL_ARRAY(m_data_m) \
    public: \
        void virtual add(data_f_t *data); \
        void map2mem(); \
        void map2buf(data_f_t *buf){ m_data_f = buf; } \
        void save_as(const char *path); \
    private: \
        data_f_t *m_data_f; \
        data_m_t *m_data_m;

#define IMPLEMENT_TX_CAL_ITEM_TABLE(class_name) \
    void class_name::add(data_f_t *data) { \
        uint32_t idx = SERIE_INDEX(data->freq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE); \
        m_data_f[idx] = *data; \
    }

#define IMPLEMENT_RX_CAL_ITEM_TABLE(class_name) \
    void class_name::add(data_f_t *data) { \
        uint32_t idx = SERIE_INDEX(data->freq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE); \
        m_data_f[idx] = *data; \
    }

namespace sp_rd  {
namespace sp1401 {

    enum hw_ver_t {
        HW_ERROR = -1,
        R1A = 0,
        R1B = 1,
        R1C = 2,
        R1D = 3,
        R1E = 4,
        R1F = 5,
        HW_MAX
    };

    RD_INLINE bool is_rf_ver_between(hw_ver_t ver,hw_ver_t first,hw_ver_t second)
    {
        return (ver >= first && ver <= second);
    }

    RD_INLINE bool is_rf_ver_before(hw_ver_t ver,hw_ver_t max)
    {
        return (ver <= max);
    }

    RD_INLINE bool is_rf_ver_after(hw_ver_t ver,hw_ver_t min)
    {
        return (ver >= min);
    }

    enum tx_pa_att_t {  //r1a/b
        TX_PA = 0,
        TX_ATT
    };

    enum rx_lna_att_t { //r1a/b/c/d/e
        RX_ATT = 0,
        RX_LNA
    };

    enum rx_att_019_t {	//r1c/d/e
        RX_ATT_19 = 0,
        RX_ATT_0
    };

    enum io_mode_t {
		IO = 0,
        OUTPUT,
        LOOP
	};

    enum bw_t {
		_80M,
		_160M
	};

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
        char mac_0[32];     //! CXU MAC address 0
        char mac_1[32];     //! CXU MAC address 1(reserved)
        char sn_rfu[32];    //! RFU SN(reserved)
        uint32_t tx_ts_num; //! TX trouble-shoot number
        uint32_t rx_ts_num; //! RX trouble-shoot number
        uint32_t remark_num;//! User-definded remark
        sn_redundant_t() : tx_ts_num(0),rx_ts_num(0),remark_num(0) {}
    };

    STRUCT_ALIGN_S(coef_complex_t,2)
        int16_t real : 16;
        int16_t imag : 16;
    STRUCT_ALIGN_E(coef_complex_t,2)

} //namespace sp1401
} //namespace sp_rd

#endif // CAL_TABLE_H
