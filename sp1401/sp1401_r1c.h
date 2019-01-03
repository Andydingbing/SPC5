#ifndef SP1401_R1C_H
#define SP1401_R1C_H

#include "sp1401.h"
#include "cal_file_r1cd.h"

namespace sp_rd {

class sp1401_r1c;

typedef int32_t (sp1401_r1c::*r1c_set_lo_reg)(uint32_t);
typedef int32_t (sp1401_r1c::*r1c_det_lo_lock)(bool &);
typedef int32_t (sp1401_r1c::*r1c_get_temp)(double *);
typedef int32_t (sp1401_r1c::*r1c_get_ad7680)(uint32_t &);

#define DECLARE_R1C_SETLOREG_FUNCTION_MAP   \
    r1c_set_lo_reg set_r1c_lo_reg[4] = {    \
        &sp1401_r1c::set_tx_lo1_reg,        \
        &sp1401_r1c::set_tx_lo2_reg,        \
        &sp1401_r1c::set_tx_lo3_reg,        \
        &sp1401_r1c::set_rx_lo1_reg };

#define DECLARE_R1C_DETLOLOCK_FUNCTION_MAP  \
    r1c_det_lo_lock det_r1c_lo_lock[4] = {  \
        &sp1401_r1c::det_tx_lo1_lock,       \
        &sp1401_r1c::det_tx_lo2_lock,       \
        &sp1401_r1c::det_tx_lo3_lock,       \
        &sp1401_r1c::det_rx_lo1_lock };

#define DECLARE_R1C_GETTEMP_FUNCTION_MAP    \
    r1c_get_temp get_r1c_temp[8] = {        \
        &sp1401_r1c::get_temp0_rx_lo2,      \
        &sp1401_r1c::get_temp1_rx_lna,      \
        &sp1401_r1c::get_temp2_rx_lo1,      \
        &sp1401_r1c::get_temp3_rx_pa1,      \
        &sp1401_r1c::get_temp4_tx_lo2,      \
        &sp1401_r1c::get_temp5_tx_lo1,      \
        &sp1401_r1c::get_temp6_tx_pa4,      \
        &sp1401_r1c::get_temp7_tx_pa3 };

#define DECLARE_R1C_GETAD7680_FUNCTION_MAP  \
    r1c_get_ad7680 get_r1c_ad7680[4] = {    \
        &sp1401_r1c::get_ad7680_rx_comb,    \
        &sp1401_r1c::get_ad7680_rx_if1,     \
        &sp1401_r1c::get_ad7680_rx_if2,     \
        &sp1401_r1c::get_ad7680_tx_if2 };

// r1c just fail
// actually this class has been modified for r1d,so some manual operation must config if work with r1c
// e.g. all lo configuration
class RD_API sp1401_r1c : public basic_sp1401
{
public:
    typedef boost::shared_ptr<sp1401_r1c> sptr;
    virtual ~sp1401_r1c();
    DECLARE_SP1401(sp1401_r1c)
public:
    enum lo_t {
        TX_LO_1 = 0,
        TX_LO_2,
        TX_LO_3,
        RX_LO_1
	};
    enum tx_filter_t {
		LPF = 0,
		BPF
	};
    enum tx_io_sw1_t {
        TX_OFF_1 = 0,
        TX_LOOP_ON,
        TX_ON_1,
        TX_IO_ON
	};
    enum tx_io_sw2_t {
        TX_OFF_2 = 0,
        TX_ON_2
	};
    enum loop_sw_t {
        LOOP_ON = 0,
        LOOP_OFF
	};
    enum rx_io_sw1_t {
        RX_IO_ON_1 = 0,
        RX_IO_OFF_1
	};
    enum rx_io_sw2_t {
        RX_ON_2 = 0,
        RX_IO_ON_2
	};
    enum rx_io_sw3_t {
        RX_LOOP_ON = 0,
        RX_OFF_3,
        RX_IO_ON_3,
        RX_ON_3
	};
    typedef struct gpio_a_t {
		unsigned pd_rxif12_txif2 : 1;
		unsigned pd_rxcomb_other : 1;
		unsigned op_green_led	 : 1;
		unsigned op_red_led		 : 1;
		unsigned tx_io_sw2		 : 1;
		unsigned tx_io_sw1		 : 2;
		unsigned io_red_led		 : 1;
		unsigned rsv			 : 24;
	public:
        gpio_a_t();
    }gpio_a_t;
    typedef struct gpio_b_t {
		unsigned io_green_led : 1;
		unsigned rx_io_sw1	  : 1;
		unsigned rx_io_sw2	  : 1;
		unsigned rx_io_sw3	  : 2;
		unsigned loop_sw	  : 1;
		unsigned rx_att019_sw : 1;
		unsigned rx_lnaatt_sw : 1;
		unsigned rsv		  : 24;
	public:
        gpio_b_t();
    }gpio_b_t;
    typedef struct tx_chain_state_all_t {
        bool        pwr_sw;
        bool        mod;
        uint64_t    lo1;
        uint64_t    lo2;
        tx_filter_t filter;
        uint64_t    lo3;
        double      att0;
        double      att1;
        double      att2;
        double      att3;
        uint32_t    det_if2;
        double      temp4;
        double      temp5;
        double      temp6;
        double      temp7;
    }tx_chain_state_all_t;
    typedef struct rx_chain_state_all_t {
        double   att1;
        double   att2;
        uint64_t lo1;
        uint64_t lo2;
        double   att3;
        uint32_t det_comb;
        uint32_t det_if1;
        uint32_t det_if2;
        double   temp0;
        double   temp1;
        double   temp2;
        double   temp3;
        rx_if_filter_t  if_filter;
        sp_rd::sp1401::io_mode_t    mode;
        sp_rd::sp1401::rx_lna_att_t lna_att;
        sp_rd::sp1401::rx_att_019_t att_019;
    }rx_chain_state_all_t;
public:
    cal_file_r1cd* get_cal_file();

    int32_t tx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2,uint64_t &lo3,tx_filter_t &filter);
    int32_t tx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t lo3,uint64_t &freq);
    int32_t rx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2);
    int32_t rx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t &freq);
    int32_t io_mode2io_sw(sp_rd::sp1401::io_mode_t mode,
                          tx_io_sw1_t &tx_sw1,tx_io_sw2_t &tx_sw2,rx_io_sw1_t &rx_sw1,rx_io_sw2_t &rx_sw2,rx_io_sw3_t &rx_sw3,loop_sw_t &loop_sw,bool &tx_led,bool &rx_led);
    int32_t io_mode2led(sp_rd::sp1401::io_mode_t mode,
                        bool &op_green,bool &op_red,bool &io_green,bool &io_red);

    int32_t init_adf5355();
    int32_t set_adf5355(lo_t lo,uint64_t freq);
    int32_t init_hmc1197();
    int32_t set_hmc1197(uint64_t freq);
    int32_t det_lo(lo_t lo,bool &lock);

    int32_t set_tx_lo1_reg(uint32_t data);
    int32_t get_tx_lo1_reg(uint32_t reg,uint32_t &data);
    int32_t set_tx_lo1_reg_open_mode(uint32_t data);
    int32_t get_tx_lo1_reg_open_mode(uint32_t reg,uint32_t &data);
    int32_t set_tx_lo1_reg_hmc_mode(uint32_t data);
    int32_t get_tx_lo1_reg_hmc_mode(uint32_t reg,uint32_t &data);

    int32_t set_tx_lo2_reg(uint32_t data);
    int32_t set_tx_lo3_reg(uint32_t data);
    int32_t det_tx_lo1_lock(bool &lock);
    int32_t det_tx_lo2_lock(bool &lock);
    int32_t det_tx_lo3_lock(bool &lock);

    int32_t set_tx_filter_sw(tx_filter_t filter1,tx_filter_t filter2);
    int32_t set_tx_io_sw1(tx_io_sw1_t sw);
    int32_t set_tx_io_sw2(tx_io_sw2_t sw);
    int32_t set_loop_sw(loop_sw_t sw);
    int32_t set_tx_att0(double att);
    int32_t set_tx_att1(double att);
    int32_t set_tx_att2(double att);
    int32_t set_tx_att3(double att);
    int32_t set_tx_att(double att0,double att1,double att2,double att3);
    int32_t sync_set_tx_gain(double att0,double att1,double att2,double att3,double d_gain);

    int32_t set_rx_io_sw1(rx_io_sw1_t sw);
    int32_t set_rx_io_sw2(rx_io_sw2_t sw);
    int32_t set_rx_io_sw3(rx_io_sw3_t sw);
    int32_t set_rx_lo1_reg(uint32_t data);
    int32_t det_rx_lo1_lock(bool &lock);
    int32_t set_rx_att_019_sw(sp_rd::sp1401::rx_att_019_t att);
    int32_t set_rx_att1(double att);
    int32_t set_rx_att2(double att);
    int32_t set_rx_att3(double att);
    int32_t set_rx_att(double att1,double att2,double att3);

    int32_t get_temp0_rx_lo2(double *temp);
    int32_t get_temp1_rx_lna(double *temp);
    int32_t get_temp2_rx_lo1(double *temp);
    int32_t get_temp3_rx_pa1(double *temp);
    int32_t get_temp4_tx_lo2(double *temp);
    int32_t get_temp5_tx_lo1(double *temp);
    int32_t get_temp6_tx_pa4(double *temp);
    int32_t get_temp7_tx_pa3(double *temp);

    int32_t get_ad7680_tx_if2(uint32_t &det);
    int32_t get_ad7680_rx_comb(uint32_t &det);
    int32_t get_ad7680_rx_if1(uint32_t &det);
    int32_t get_ad7680_rx_if2(uint32_t &det);

    int32_t set_mcp23s17_reg(uint32_t addr,uint32_t data);
    int32_t get_mcp23s17_reg(uint32_t addr,uint32_t &data);
    int32_t init_mcp23s17();

    int32_t get_mcp3208(uint32_t addr,uint32_t &data);
};

} //namespace sp_rd

#endif // SP1401_R1C_H
