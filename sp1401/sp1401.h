#ifndef SP1401_H
#define SP1401_H

#define SN_NULL  0	//missing eeprom
#define SN_WRONG 1	//sn read ok but wrong
#define SN_RIGHT 2	//sn right

#include "sgpmm.h"
#include "arb_reader.h"
#include "dsp_buf.h"
#include "cal_file.h"

namespace sp_rd {

class RD_API basic_sp1401
{
public:
    typedef boost::shared_ptr<basic_sp1401> sptr;
    basic_sp1401();
    ~basic_sp1401();
public:
    enum hw_ver_t {
        HW_ERROR = -1,
        R1A = 0,
		R1B = 1,
		R1C = 2,
		R1D = 3
	};
    enum rf_ch_t {
        CH_TX = 0,
        CH_RX = 1
	};
    enum trig_src_t	{
        TS_MANUAL   = 1,
        TS_EXT_A    = 4,
        TS_EXT_B    = 5,
        TS_EXT_C    = 6,
        TS_EXT_D    = 7
	};
    enum rep_mode_t {
        RM_SINGLE_SLOT,
        RM_CONTINUOUS
	};
    enum multi_seg_trig_src_t {
        MS_TS_MANUAL
	};
    enum multi_seg_rep_mode_t {
        MS_RM_AUTO,
        MS_RM_CONTINUOUS,
        MS_RM_CONTINUOUS_SEAMLESS
	};
    enum rx_if_filter_t {
		_100MHz,
		_160MHz
	};
    enum iq_cap_src_t {
        PWR_MEAS_FREE_RUN   = 0x00,
        IQ_CAP_SRC_RST_MKR  = 0x09,
        IQ_CAP_SRC_MKR1     = 0x0a,
        IQ_CAP_SRC_MKR2     = 0x0b,
        IQ_CAP_SRC_MKR3     = 0x0c,
        IQ_CAP_SRC_MKR4     = 0x0d,
        IQ_CAP_SRC_USR_MKR  = 0x0e,
        IQ_CAP_SRC_ETRIG    = 0x11,
        IQ_CAP_SRC_ETRIG1   = 0x12,
        IQ_CAP_SRC_ETRIG2   = 0x13,
        IQ_CAP_SRC_ETRIG3   = 0x14,
        IQ_CAP_SRC_ETRIG4   = 0x15,
        PWR_MEAS_IF_PWR     = 0x21
	};

#define pwr_meas_src_t iq_cap_src_t

    enum pwr_meas_state_t {
        PMS_IDLE	= 0x0,
        PMS_WFT		= 0x1,  //waiting for trigger
        PMS_TT		= 0x2,  //trigger timeout,manual set to IDLE
        PMS_RUNNING = 0x3,  //calculating
        PMS_DONE	= 0x4   //manual set to IDLE
	};

#define DECLARE_SP1401(class_name)                                      \
public:                                                                 \
    class_name(uint32_t rf_idx,uint32_t rfu_idx);                       \
    virtual int32_t  open_board();                                      \
    virtual int32_t  close_board();                                     \
    virtual int32_t  set_led(bool tx,bool rx);                          \
    virtual bool	 is_connected();                                    \
    virtual int32_t  set_tx_freq(uint64_t freq);                        \
    virtual int32_t  set_tx_att(double att);                            \
    virtual int32_t  set_tx_modulator_en(bool en);                      \
    virtual int32_t  set_pwr_en(bool en);                               \
    virtual int32_t  set_rx_freq(uint64_t freq);                        \
    virtual int32_t  set_rx_lna_att_sw(sp1401::rx_lna_att_t lna_att);   \
    virtual int32_t  set_io_mode(sp1401::io_mode_t mode);               \
    virtual int32_t  get_io_mode(sp1401::io_mode_t &mode);              \

public:
    virtual int32_t  open_board() = 0;
    virtual int32_t  close_board() = 0;
    virtual int32_t  set_led(bool tx,bool rx) = 0;
    virtual bool	 is_connected() = 0;
    virtual int32_t  set_tx_freq(uint64_t freq) = 0;
    virtual int32_t  set_tx_att(double att) = 0;
    virtual int32_t  set_tx_modulator_en(bool en) = 0;
    virtual int32_t  set_pwr_en(bool en) = 0;
    virtual int32_t  set_rx_freq(uint64_t freq) = 0;
    virtual int32_t  set_rx_lna_att_sw(sp1401::rx_lna_att_t lna_att) = 0;
    virtual int32_t  set_io_mode(sp1401::io_mode_t mode) = 0;
    virtual int32_t  get_io_mode(sp1401::io_mode_t &mode) = 0;
public:
    static void set_expt_en(bool en);
    virtual sp_rd::sp1401::bw_t get_bw();
    virtual uint32_t get_rf_idx();
    virtual uint32_t get_rfu_idx();
    virtual cal_file* get_cal_file();
    virtual vi_pci_dev* get_k7();
    virtual int32_t connect(vi_pci_dev *k7);
    virtual int32_t get_k7_ver(uint32_t &ver);
    virtual int32_t get_sn(char *sn);
    virtual arb_reader *get_arb_reader();

    static  int32_t is_valid_sn(const char *sn);
    static  int32_t get_hw_ver(const char *sn,hw_ver_t &ver);

    virtual int32_t  set_arb_segments(uint16_t segs);
    virtual int32_t  set_arb_param(uint32_t add_samp,uint32_t cycle,uint32_t rep_mode);
    virtual int32_t  set_arb_trigger(bool retrigger, bool auto_start, uint32_t src, uint32_t trig_delay);
    virtual int32_t  set_arb_manual_trigger();
    virtual int32_t  set_arb_bc_star_pos(uint32_t pos);
    virtual int32_t  set_arb_bc_samples(uint32_t samples);
    virtual uint32_t get_arb_current_seg();
    virtual uint32_t get_arb_current_sample();
    virtual uint32_t get_arb_current_cycle();
    virtual double   get_arb_current_timer();
    virtual int32_t  arb_start();
    virtual int32_t  arb_stop();

#define set_iq_cap_src              set_pwr_meas_src
#define set_iq_cap_timeout          set_pwr_meas_timeout
#define is_iq_cap_timeout           is_pwr_meas_timeout
#define set_iq_cap_trig_threshold   set_pwr_mesa_trig_threshold

    virtual int32_t set_pwr_meas_src(pwr_meas_src_t src,bool posedge);
    virtual int32_t set_pwr_meas_timeout(uint32_t us);
    virtual int32_t is_pwr_meas_timeout(bool &timeout);
    virtual int32_t set_pwr_mesa_trig_threshold(double if_pwr);
    virtual int32_t pwr_meas_start();
    virtual int32_t pwr_meas_abort();
    virtual int32_t set_pwr_meas_samples(uint32_t samples);
    virtual int32_t get_pwr_meas_samples(uint32_t &samples);
    virtual int32_t get_pwr_meas_state(pwr_meas_state_t &state);
    virtual int32_t get_pwr_meas_peak(double &pwr);
    virtual int32_t get_pwr_meas_pwr(double &pwr);
public:
    virtual int32_t set_sn(const char *hw_ver,int32_t ordinal);
    virtual int32_t set_sn(char *sn);
    virtual int32_t w_eeprom(uint16_t addr,uint32_t length,char *buf);
    virtual int32_t r_eeprom(uint16_t addr,uint32_t length,char *buf);

    virtual int32_t get_ads5474(int64_t &ad);
    virtual int32_t get_ads5474_manual(int64_t &ad);
    virtual int32_t set_rx_if_filter_sw(rx_if_filter_t filter);
    virtual int32_t get_ad7680(uint32_t &det);
protected:
    vi_pci_dev *m_k7;
    boost::shared_ptr<cal_file> m_cal_file;
    boost::shared_ptr<arb_reader> m_arb_reader;
    float m_arb_level_offset;
    static bool g_en_expt;
};

} //namespace sp_rd

#endif // SP1401_H
