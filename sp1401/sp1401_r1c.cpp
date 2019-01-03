#include "sp1401_r1c.h"
#include "reg_def.h"
#include <math.h>
#include "sleep_common.h"

using namespace sp_rd;
using namespace sp_rd::sp1401;

sp1401_r1c::gpio_a_t::gpio_a_t()
{
	pd_rxif12_txif2 = 0;
	pd_rxcomb_other = 0;
	op_green_led	= 0;
	op_red_led		= 0;
	tx_io_sw1		= 0;
	io_red_led		= 0;
	tx_io_sw2		= 0;
	rsv			    = 0;
}

sp1401_r1c::gpio_b_t::gpio_b_t()
{
	io_green_led = 0;
	rx_io_sw1	 = 0;
	rx_io_sw2	 = 0;
	rx_io_sw3	 = 0;
	loop_sw	     = 0;
	rx_att019_sw = 0;
	rx_lnaatt_sw = 0;
	rsv		     = 0;
}

sp1401_r1c::sp1401_r1c(uint32_t rf_idx,uint32_t rfu_idx)
{
    m_cal_file = boost::make_shared<cal_file_r1cd>(rf_idx,rfu_idx);
}

sp1401_r1c::~sp1401_r1c()
{
}

int32_t sp1401_r1c::open_board()
{
    INT_CHECK(m_cal_file->open());
    INT_CHECK(set_pwr_en(true));
    INT_CHECK(init_adf5355());
    INT_CHECK(init_hmc1197());
    INT_CHECK(init_mcp23s17());
    INT_CHECK(set_io_mode(OUTPUT));
    INT_CHECK(set_tx_modulator_en(true));
	return 0;
}

int32_t sp1401_r1c::init_adf5355()
{
    uint32_t reg[13];
	for (int i = 0;i < 13;i ++)
        reg[i] = 0;

    uint64_t freq_vco = 4000000000;
    uint64_t freq_lo[4] = {0,0,0,0};
    tx_filter_t filter = LPF;
    uint32_t _int = 0;
    uint32_t _frac1 = 0;
    uint32_t _frac2 = 0;
    uint32_t _mod1 = 0;
    uint32_t _mod2 = 0;

    reg[3] = 0x00000013;
    reg[4] = 0x30008394;
    reg[5] = 0x00800025;
    reg[7] = 0x12000067;
    reg[8] = 0x102d0428;
    reg[9] = 0x28323de9;
    reg[10] = 0x00c025ba;
    reg[11] = 0x0061300b;
    reg[12] = 0x4e20041c;

    DECLARE_R1C_SETLOREG_FUNCTION_MAP

    tx_freq2lo(2400000000,freq_lo[0],freq_lo[1],freq_lo[2],filter);
    rx_freq2lo(2400000000,freq_lo[3],freq_lo[1]);

    for (int32_t lo_idx = (int32_t)TX_LO_2;lo_idx <= (int32_t)RX_LO_1;lo_idx ++) {
        freq_vco = freq_lo[lo_idx];
        if (freq_lo[lo_idx] > 6800000000) freq_vco = freq_lo[lo_idx] / 2;
        if (freq_lo[lo_idx] < 3400000000) freq_vco = freq_lo[lo_idx] * 2;
        adf5355_freq_formula(freq_vco,100000000,false,false,1,ADF5355_FREQ_SPACE,_int,_frac1,_frac2,_mod1,_mod2);
        reg[0] = adf5355para2reg0(_int,0,1);
        reg[1] = adf5355para2reg1(_frac1);
        reg[2] = adf5355para2reg2(_mod2,_frac2);
        reg[6] = freq_lo[lo_idx] < 3400000000 ? 0x3520a076 : 0x3500a076;
		for (int i = 0;i < 13;i ++)
            (this->*set_r1c_lo_reg[lo_idx])(reg[12 - i]);
	}
    set_adf5355(TX_LO_2,freq_lo[(int32_t)TX_LO_2]);
	return 0;
}

int32_t sp1401_r1c::init_hmc1197()
{
    uint32_t reg[32];
	for (int32_t i = 0;i < 32;i ++)
        reg[i] = 0;

    DECLARE_R1C_SETLOREG_FUNCTION_MAP

    reg[0x00] = 0x00000000;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x00]);
    reg[0x01] = 0x00000208;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x01]);
    reg[0x02] = 0x00000110;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x02]);
//	reg[0x03] = 0x00002718;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x03]);	//r1c	975MHz * 4
    reg[0x03] = 0x00002518;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x03]);	//r1d	925MHz * 4
    reg[0x04] = 0x00000020;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x04]);
    reg[0x05] = 0x00000028;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x05]);
    reg[0x06] = 0x03078830;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x06]);
    reg[0x07] = 0x20094438;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x07]);
    reg[0x08] = 0x41bfff40;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x08]);
    reg[0x09] = 0x00326448;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x09]);
    reg[0x0a] = 0x00204650;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0a]);
    reg[0x0b] = 0x07806158;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0b]);
    reg[0x0c] = 0x00000060;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0c]);
    reg[0x0f] = 0x00008178;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x0f]);
    reg[0x14] = 0x000220a0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x14]);
    reg[0x15] = 0x0f48a0a8;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x15]);
    reg[0x16] = 0x0006c4b0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x16]);
    reg[0x17] = 0x00012bb8;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x17]);
    reg[0x18] = 0x0054c3c0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x18]);
    reg[0x19] = 0x000ab2c8;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x19]);
    reg[0x1a] = 0xb29d0bd0;	(this->*set_r1c_lo_reg[TX_LO_1])(reg[0x1a]);

	return 0;
}

int32_t sp1401_r1c::close_board()
{
	return 0;
}

int32_t sp1401_r1c::set_led(bool tx,bool rx)
{
    uint32_t reg_gpa = 0,reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpa.op_green_led = tx ? 0 : 1;
    gpa.op_red_led   = tx ? 1 : 0;
    gpb.io_green_led = rx ? 0 : 1;
    gpa.io_red_led   = rx ? 1 : 0;
    reg_gpa = *(uint32_t *)&gpa;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

bool sp1401_r1c::is_connected()
{
    DECLARE_R1C_GETTEMP_FUNCTION_MAP

    double temp = 0.0;
	for (int32_t i = 0;i < 8;i ++) {
        INT_CHECKB((this->*get_r1c_temp[i])(&temp));
        if (temp > 0.0 && temp < 100.0)
			return true;
    }
    Log.set_last_err(-2,"missing rfu%u--->rf%u",m_cal_file->get_rfu_idx(),m_cal_file->get_rf_idx());
	return false;
}

cal_file_r1cd* sp1401_r1c::get_cal_file()
{
    return (cal_file_r1cd *)(m_cal_file.get());
}

int32_t sp1401_r1c::tx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2,uint64_t &lo3,tx_filter_t &filter)
{
// 	lo1 = 975000000;	//r1c
// 	lo2 = 6524000000;	//r1c
    lo1 = 925000000;	//r1d
    lo2 = 6574000000;	//r1d
    lo3 = freq + lo1 + lo2;
    filter = (freq >= 3100000000 ? BPF : LPF);
	return 0;
}

int32_t sp1401_r1c::tx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t lo3,uint64_t &freq)
{
    freq = 0;
//	if (975000000 == lo1 && 6524000000 == lo2) {	//r1c
    if (925000000 == lo1 && 6574000000 == lo2) {	//r1d
        if (lo3 > 7499000000 && lo3 <= 13499000000)
            freq = lo3 - 7499000000;
		else
			return -1;
	}
	else
		return -1;
	return 0;
}

int32_t sp1401_r1c::rx_freq2lo(uint64_t freq,uint64_t &lo1,uint64_t &lo2)
{
//	lo2 = 6524000000;	//r1c
    lo2 = 6574000000;	//r1d
    lo1 = freq + lo2 + 276000000;
	return 0;
}

int32_t sp1401_r1c::rx_lo2freq(uint64_t lo1,uint64_t lo2,uint64_t &freq)
{
    freq = 0;
//	if (lo1 > 6800000000)	//r1c
    if (lo1 > 6850000000)	//r1d
        freq = lo1 - lo2 - 276000000;
	else
		return -1;
	return 0;
}

int32_t sp1401_r1c::io_mode2io_sw(io_mode_t mode,tx_io_sw1_t &tx_sw1,tx_io_sw2_t &tx_sw2,rx_io_sw1_t &rx_sw1,rx_io_sw2_t &rx_sw2,rx_io_sw3_t &rx_sw3,loop_sw_t &loop_sw,bool &tx_led,bool &rx_led)
{
    switch (mode) {
		case IO : {
            tx_sw1 = TX_IO_ON;
            tx_sw2 = TX_OFF_2;
            rx_sw1 = RX_IO_ON_1;
            rx_sw2 = RX_IO_ON_2;
            rx_sw3 = RX_IO_ON_3;
            loop_sw  = LOOP_OFF;
            tx_led = false;
            rx_led = true;
			break;
				  }
        case OUTPUT : {
            tx_sw1 = TX_ON_1;
            tx_sw2 = TX_ON_2;
            rx_sw1 = RX_IO_ON_1;
            rx_sw2 = RX_ON_2;
            rx_sw3 = RX_ON_3;
            loop_sw  = LOOP_OFF;
            tx_led = true;
            rx_led = true;
			break;
					  }
        case LOOP : {
            tx_sw1 = TX_LOOP_ON;
            tx_sw2 = TX_OFF_2;
            rx_sw1 = RX_IO_OFF_1;
            rx_sw2 = RX_IO_ON_2;
            rx_sw3 = RX_LOOP_ON;
            loop_sw  = LOOP_ON;
            tx_led = false;
            rx_led = false;
			break;
					}
        default:break;
	}
	return 0;
}

int32_t sp1401_r1c::io_mode2led(io_mode_t mode,bool &op_green,bool &op_red,bool &io_green,bool &io_red)
{
    switch (mode) {
		case IO : {
            op_green = true;
            op_red   = false;
            io_green = false;
            io_red   = true;
			break;
				  }
        case OUTPUT : {
            op_green = false;
            op_red   = true;
            io_green = false;
            io_red   = true;
			break;
					  }
        case LOOP : {
            op_green = true;
            op_red   = false;
            io_green = true;
            io_red   = false;
			break;
					}
        default:break;
	}
	return 0;
}

int32_t sp1401_r1c::set_pwr_en(bool en)
{
    RFU_K7_REG_DECLARE_2(0x1829,0x2829);
    RFU_K7_REG_2(0x1829,0x2829).pow_en = (en ? 1 : 0);
    RFU_K7_W_2(0x1829,0x2829);
	return 0;
}

int32_t sp1401_r1c::set_tx_modulator_en(bool en)
{
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).mod = en ? 1 : 0;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_io_mode(io_mode_t mode)
{
    tx_io_sw1_t tx_sw1 = TX_IO_ON;
    tx_io_sw2_t tx_sw2 = TX_OFF_2;
    rx_io_sw1_t rx_sw1 = RX_IO_ON_1;
    rx_io_sw2_t rx_sw2 = RX_IO_ON_2;
    rx_io_sw3_t rx_sw3 = RX_IO_ON_3;
    loop_sw_t  loop_sw = LOOP_OFF;
    bool tx_led = false,rx_led = true;
    bool op_green = false,op_red = true,io_green = false,io_red = true;
    uint32_t reg_gpa = 0,reg_gpb = 0;

    io_mode2io_sw(mode,tx_sw1,tx_sw2,rx_sw1,rx_sw2,rx_sw3,loop_sw,tx_led,rx_led);
    io_mode2led(mode,op_green,op_red,io_green,io_red);
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpa.tx_io_sw1 = (unsigned)tx_sw1;
    gpa.tx_io_sw2 = (unsigned)tx_sw2;
    gpb.rx_io_sw1 = (unsigned)rx_sw1;
    gpb.rx_io_sw2 = (unsigned)rx_sw2;
    gpb.rx_io_sw3 = (unsigned)rx_sw3;
    gpb.loop_sw   = (unsigned)loop_sw;
    gpa.op_green_led = op_green ? 1 : 0;
    gpa.op_red_led	 = op_red   ? 1 : 0;
    gpb.io_green_led = io_green ? 1 : 0;
    gpa.io_red_led   = io_red   ? 1 : 0;
    reg_gpa = *(uint32_t *)&gpa;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::get_io_mode(io_mode_t &mode)
{
    uint32_t reg_gpa = 0,reg_gpb = 0;

    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));

    if (0xd8 == reg_gpa && 0x38 == reg_gpb)
        mode = OUTPUT;
    if (0xe4 == reg_gpa && 0x34 == reg_gpb)
        mode = IO;
    if (0x24 == reg_gpa && 0x07 == reg_gpb)
        mode = LOOP;
	return 0;
}

int32_t sp1401_r1c::set_adf5355(lo_t lo,uint64_t freq)
{
	uint32_t reg[13];
	for (int i = 0;i < 13;i ++)
		reg[i] = 0;

    uint64_t freg_vco = freq;
    if (freq > 6800000000) freg_vco = freq / 2;
    if (freq < 3400000000) freg_vco = freq * 2;

    uint32_t _int = 0;
    uint32_t _frac1 = 0;
    uint32_t _frac2 = 0;
    uint32_t _mod1 = 0;
    uint32_t _mod2 = 0;

    adf5355_freq_formula(freg_vco,100000000,false,false,1,ADF5355_FREQ_SPACE,_int,_frac1,_frac2,_mod1,_mod2);
    reg[0] = adf5355para2reg0(_int,0,1);
    reg[1] = adf5355para2reg1(_frac1);
    reg[2] = adf5355para2reg2(_mod2,_frac2);
	reg[4] = 0x30008394;
    reg[6] = freq < 3400000000 ? 0x3520a076 : 0x3500a076;

    DECLARE_R1C_SETLOREG_FUNCTION_MAP

    (this->*set_r1c_lo_reg[lo])(reg[6]);
	reg[4] = reg[4] | (1 << 4);
    (this->*set_r1c_lo_reg[lo])(reg[4]);
    (this->*set_r1c_lo_reg[lo])(reg[2]);
    (this->*set_r1c_lo_reg[lo])(reg[1]);
	reg[0] = reg[0] & (~(1 << 21));
    (this->*set_r1c_lo_reg[lo])(reg[0]);
	reg[4] = reg[4] & (~(1 << 4));
    (this->*set_r1c_lo_reg[lo])(reg[4]);
    sleep_ms(100);
	reg[0] = reg[0] | (1 << 21);
    (this->*set_r1c_lo_reg[lo])(reg[0]);
	return 0;
}

int32_t sp1401_r1c::set_hmc1197(uint64_t freq)
{
	return 0;
}

int32_t sp1401_r1c::det_lo(lo_t lo,bool &lock)
{
    sleep_ms(100);

    DECLARE_R1C_DETLOLOCK_FUNCTION_MAP

    return (this->*det_r1c_lo_lock[lo])(lock);
}

int32_t sp1401_r1c::set_tx_lo1_reg(uint32_t data)
{
    return set_tx_lo1_reg_open_mode(data);
}

int32_t sp1401_r1c::get_tx_lo1_reg(uint32_t reg,uint32_t &data)
{
    return get_tx_lo1_reg_open_mode(reg,data);
}

int32_t sp1401_r1c::set_tx_lo1_reg_open_mode(uint32_t data)
{	
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);
    RFU_K7_REG_DECLARE_2(0x1805,0x2805);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).wr = 0;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1805,0x2805).data = data;
    RFU_K7_W_2(0x1805,0x2805);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_OP_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
	return 0;
}

int32_t sp1401_r1c::get_tx_lo1_reg_open_mode(uint32_t reg,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x1804,0x2804);
    RFU_K7_REG_DECLARE_2(0x1805,0x2805);
    RFU_K7_REG_DECLARE_2(0x1807,0x2807);

    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1804,0x2804).wr = 1;
    RFU_K7_W_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_REG_2(0x1805,0x2805).data = (0 | (reg << 8));
    RFU_K7_W_2(0x1805,0x2805);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_OP_2(0x1804,0x2804);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    RFU_K7_R_2(0x1807,0x2807);
    RFU_K7_WAIT_IDLE_2(0x1804,0x2804,0,1000);
    data = (uint32_t)(RFU_K7_REG_2(0x1807,0x2807).data) >> 8;
	return 0;
}

int32_t sp1401_r1c::set_tx_lo1_reg_hmc_mode(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x1836,0x2836);
    RFU_K7_REG_DECLARE_2(0x1837,0x2837);

    RFU_K7_REG_2(0x1836,0x2836).wr = 0;
    RFU_K7_REG_2(0x1836,0x2836).addr = (data & 0x000000f8) >> 3;
    RFU_K7_W_2(0x1836,0x2836);
    RFU_K7_REG_2(0x1837,0x2837).data = (data & 0xffffff00) >> 8;
    RFU_K7_W_2(0x1837,0x2837);

    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
    RFU_K7_OP_2(0x1836,0x2836);
    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
	return 0;
}

int32_t sp1401_r1c::get_tx_lo1_reg_hmc_mode(uint32_t reg,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x1836,0x2836);
    RFU_K7_REG_DECLARE_2(0x1838,0x2838);

    RFU_K7_REG_2(0x1836,0x2836).wr = 1;
    RFU_K7_REG_2(0x1836,0x2836).addr = reg;
    RFU_K7_W_2(0x1836,0x2836);

    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);
    RFU_K7_OP_2(0x1836,0x2836);
    RFU_K7_WAIT_IDLE_2(0x1836,0x2836,0,1000);

    RFU_K7_R_2(0x1838,0x2838);
    data = (uint32_t)(RFU_K7_REG_2(0x1838,0x2838).data);
	return 0;
}

int32_t sp1401_r1c::det_tx_lo1_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    lock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_tx_lo2_reg(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x1609,0x1689);
    RFU_K7_REG_DECLARE_2(0x160a,0x168a);
    RFU_K7_REG_2(0x160a,0x168a).data = data;
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
    RFU_K7_W_2(0x160a,0x168a);
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
    RFU_K7_OP_2(0x1609,0x1689);
    RFU_K7_WAIT_IDLE_2(0x1609,0x1689,0,1000);
	return 0;
}

int32_t sp1401_r1c::det_tx_lo2_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160b,0x168b);
    RFU_K7_R_2(0x160b,0x168b);
    lock = (RFU_K7_REG_2(0x160b,0x168b).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_tx_lo3_reg(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x160d,0x168d);
    RFU_K7_REG_DECLARE_2(0x160e,0x168e);
    RFU_K7_REG_2(0x160e,0x168e).data = data;
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
    RFU_K7_W_2(0x160e,0x168e);
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
    RFU_K7_OP_2(0x160d,0x168d);
    RFU_K7_WAIT_IDLE_2(0x160d,0x168d,0,1000);
	return 0;
}

int32_t sp1401_r1c::det_tx_lo3_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x160f,0x168f);
    RFU_K7_R_2(0x160f,0x168f);
    lock = (RFU_K7_REG_2(0x160f,0x168f).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_tx_freq(uint64_t freq)
{
    uint64_t lo1 = 0,lo2 = 0,lo3 = 0;
    tx_filter_t filter = LPF;
    tx_freq2lo(freq,lo1,lo2,lo3,filter);
    INT_CHECK(set_tx_filter_sw(filter,filter));
    INT_CHECK(set_adf5355(TX_LO_3,lo3));
	return 0;
}

int32_t sp1401_r1c::set_tx_filter_sw(tx_filter_t filter1,tx_filter_t filter2)
{
    RFU_K7_REG_DECLARE_2(0x1828,0x2828);
    RFU_K7_REG_DECLARE_2(0x1835,0x2835);
    RFU_K7_REG_2(0x1828,0x2828).filter = (LPF == filter1 ? 0 : 1);
    RFU_K7_REG_2(0x1835,0x2835).filter = (LPF == filter2 ? 0 : 1);
    RFU_K7_W_2(0x1828,0x2828);
    RFU_K7_W_2(0x1835,0x2835);
	return 0;
}

int32_t sp1401_r1c::set_tx_io_sw1(tx_io_sw1_t sw)
{
    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpa.tx_io_sw1 = (unsigned)sw;
    reg_gpa = *(uint32_t *)&gpa;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
	return 0;
}

int32_t sp1401_r1c::set_tx_io_sw2(tx_io_sw2_t sw)
{
    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    gpa.tx_io_sw2 = (unsigned)sw;
    reg_gpa = *(uint32_t *)&gpa;
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
	return 0;
}

int32_t sp1401_r1c::set_loop_sw(loop_sw_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.loop_sw = (unsigned)sw;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_tx_att0(double att)
{
    int32_t d0_5 = 63 - (int32_t)(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1810,0x2810);
    RFU_K7_REG_DECLARE_2(0x1811,0x2811);
    RFU_K7_REG_2(0x1811,0x2811).data_word = (unsigned)d0_5;
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
    RFU_K7_W_2(0x1811,0x2811);
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
    RFU_K7_OP_2(0x1810,0x2810);
    RFU_K7_WAIT_IDLE_2(0x1810,0x2810,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_tx_att1(double att)
{
    int32_t d0_5 = 63 - (int32_t)(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1814,0x2814);
    RFU_K7_REG_DECLARE_2(0x1815,0x2815);
    RFU_K7_REG_2(0x1815,0x2815).data_word = (unsigned)d0_5;
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
    RFU_K7_W_2(0x1815,0x2815);
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
    RFU_K7_OP_2(0x1814,0x2814);
    RFU_K7_WAIT_IDLE_2(0x1814,0x2814,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_tx_att2(double att)
{
    int32_t d0_5 = 63 - (int32_t)(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1818,0x2818);
    RFU_K7_REG_DECLARE_2(0x1819,0x2819);
    RFU_K7_REG_2(0x1819,0x2819).data_word = (unsigned)d0_5;
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
    RFU_K7_W_2(0x1819,0x2819);
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
    RFU_K7_OP_2(0x1818,0x2818);
    RFU_K7_WAIT_IDLE_2(0x1818,0x2818,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_tx_att3(double att)
{
    int32_t d0_5 = 63 - (int32_t)(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x181c,0x281c);
    RFU_K7_REG_DECLARE_2(0x181d,0x281d);
    RFU_K7_REG_2(0x181d,0x281d).data_word = (unsigned)d0_5;
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
    RFU_K7_W_2(0x181d,0x281d);
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
    RFU_K7_OP_2(0x181c,0x281c);
    RFU_K7_WAIT_IDLE_2(0x181c,0x281c,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_tx_att(double att0,double att1,double att2,double att3)
{
    INT_CHECK(set_tx_att0(att0));
    INT_CHECK(set_tx_att1(att1));
    INT_CHECK(set_tx_att2(att2));
    INT_CHECK(set_tx_att3(att3));
	return 0;
}

int32_t sp1401_r1c::set_tx_att(double att)
{
	return 0;
}

int32_t sp1401_r1c::sync_set_tx_gain(double att0,double att1,double att2,double att3,double d_gain)
{
    RFU_K7_REG_DECLARE_2(0x0094,0x2094);
    RFU_K7_REG_DECLARE_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    INT_CHECK(set_tx_att(att0,att1,att2,att3));
    RFU_K7_REG_2(0x0094,0x2094).i = (uint16_t)(d_gain * 512.0);
    RFU_K7_REG_2(0x0094,0x2094).q = (uint16_t)(d_gain * 512.0);
    RFU_K7_W_2(0x0094,0x2094);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 1;
    RFU_K7_W_2(0x00e6,0x20e6);

    RFU_K7_REG_2(0x00e6,0x20e6).op_0 = 0;
    RFU_K7_REG_2(0x00e6,0x20e6).op_1 = 0;
    RFU_K7_W_2(0x00e6,0x20e6);
	return 0;
}

int32_t sp1401_r1c::set_rx_freq(uint64_t freq)
{
    uint64_t lo1 = 0,lo2 = 0;
    rx_freq2lo(freq,lo1,lo2);
    INT_CHECK(set_adf5355(RX_LO_1,lo1));
	return 0;
}

int32_t sp1401_r1c::set_rx_io_sw1(rx_io_sw1_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_io_sw1 = (unsigned)sw;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_io_sw2(rx_io_sw2_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_io_sw2 = (unsigned)sw;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_io_sw3(rx_io_sw3_t sw)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_io_sw3 = (unsigned)sw;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_lo1_reg(uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x1601,0x1681);
    RFU_K7_REG_DECLARE_2(0x1602,0x1682);
    RFU_K7_REG_2(0x1602,0x1682).data = data;
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
    RFU_K7_W_2(0x1602,0x1682);
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
    RFU_K7_OP_2(0x1601,0x1681);
    RFU_K7_WAIT_IDLE_2(0x1601,0x1681,0,1000);
	return 0;
}

int32_t sp1401_r1c::det_rx_lo1_lock(bool &lock)
{
    RFU_K7_REG_DECLARE_2(0x1603,0x1683);
    RFU_K7_R_2(0x1603,0x1683);
    lock = (RFU_K7_REG_2(0x1603,0x1683).muxout == 0 ? false : true);
	return 0;
}

int32_t sp1401_r1c::set_rx_att_019_sw(rx_att_019_t att)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t*)&reg_gpb;
    gpb.rx_att019_sw = (unsigned)att;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_lna_att_sw(rx_lna_att_t lna_att)
{
    uint32_t reg_gpb = 0;
    INT_CHECK(get_mcp23s17_reg(0x13,reg_gpb));
    gpio_b_t gpb = *(gpio_b_t *)&reg_gpb;
    gpb.rx_lnaatt_sw = (unsigned)lna_att;
    reg_gpb = *(uint32_t *)&gpb;
    INT_CHECK(set_mcp23s17_reg(0x13,reg_gpb));
	return 0;
}

int32_t sp1401_r1c::set_rx_att1(double att)
{
    int32_t d0_5 = 63 - (int32_t)(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1820,0x2820);
    RFU_K7_REG_DECLARE_2(0x1821,0x2821);
    RFU_K7_REG_2(0x1821,0x2821).data_word = (unsigned)d0_5;
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
    RFU_K7_W_2(0x1821,0x2821);
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
    RFU_K7_OP_2(0x1820,0x2820);
    RFU_K7_WAIT_IDLE_2(0x1820,0x2820,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_rx_att2(double att)
{
    int32_t d0_5 = 63 - (int32_t)(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x1824,0x2824);
    RFU_K7_REG_DECLARE_2(0x1825,0x2825);
    RFU_K7_REG_2(0x1825,0x2825).data_word = (unsigned)d0_5;
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
    RFU_K7_W_2(0x1825,0x2825);
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
    RFU_K7_OP_2(0x1824,0x2824);
    RFU_K7_WAIT_IDLE_2(0x1824,0x2824,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_rx_att3(double att)
{
    int32_t d0_5 = 63 - (int32_t)(63 * att / 31.5);

    RFU_K7_REG_DECLARE_2(0x182b,0x282b);
    RFU_K7_REG_DECLARE_2(0x182c,0x282c);
    RFU_K7_REG_2(0x182c,0x282c).data_word = (unsigned)d0_5;
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
    RFU_K7_W_2(0x182c,0x282c);
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
    RFU_K7_OP_2(0x182b,0x282b);
    RFU_K7_WAIT_IDLE_2(0x182b,0x282b,0,1000);
	return 0;
}

int32_t sp1401_r1c::set_rx_att(double att1,double att2,double att3)
{
    INT_CHECK(set_rx_att1(att1));
    INT_CHECK(set_rx_att2(att2));
    INT_CHECK(set_rx_att3(att3));
	return 0;
}

int32_t sp1401_r1c::get_temp0_rx_lo2(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(0,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_temp1_rx_lna(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(1,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_temp2_rx_lo1(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(2,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_temp3_rx_pa1(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(3,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_temp4_tx_lo2(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(4,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_temp5_tx_lo1(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(5,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_temp6_tx_pa4(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(6,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_temp7_tx_pa3(double *temp)
{
    uint32_t ad = 0;
    INT_CHECK(get_mcp3208(7,ad));
    *temp = tc1407vol2temp(mcp3208ad2vol(ad));
	return 0;
}

int32_t sp1401_r1c::get_ad7680_tx_if2(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 0;
    gpa.pd_rxif12_txif2 = 1;
    gpa.pd_rxcomb_other = 0;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::get_ad7680_rx_comb(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 1;
    gpa.pd_rxif12_txif2 = 0;
    gpa.pd_rxcomb_other = 1;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::get_ad7680_rx_if1(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 0;
    gpa.pd_rxif12_txif2 = 0;
    gpa.pd_rxcomb_other = 0;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::get_ad7680_rx_if2(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1827,0x2827);

    uint32_t reg_gpa = 0;
    INT_CHECK(get_mcp23s17_reg(0x12,reg_gpa));
    gpio_a_t gpa = *(gpio_a_t *)&reg_gpa;
    RFU_K7_REG_2(0x1827,0x2827).sw = 1;
    gpa.pd_rxif12_txif2 = 0;
    gpa.pd_rxcomb_other = 0;
    reg_gpa = *(uint32_t *)&gpa;
    RFU_K7_W_2(0x1827,0x2827);
    INT_CHECK(set_mcp23s17_reg(0x12,reg_gpa));
    INT_CHECK(get_ad7680(det));
	return 0;
}

int32_t sp1401_r1c::set_mcp23s17_reg(uint32_t addr,uint32_t data)
{
    RFU_K7_REG_DECLARE_2(0x182d,0x282d);
    RFU_K7_REG_DECLARE_2(0x182e,0x282e);

    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_REG_2(0x182d,0x282d).addr = (unsigned)addr;
    RFU_K7_REG_2(0x182d,0x282d).wr = 0;
    RFU_K7_REG_2(0x182e,0x282e).data = (unsigned)data;
    RFU_K7_W_2(0x182d,0x282d);
    RFU_K7_W_2(0x182e,0x282e);
    RFU_K7_OP_2(0x182d,0x282d);
    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
	return 0;
}

int32_t sp1401_r1c::get_mcp23s17_reg(uint32_t addr,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x182d,0x282d);
    RFU_K7_REG_DECLARE_2(0x182f,0x282f);

    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_REG_2(0x182d,0x282d).addr = (unsigned)addr;
    RFU_K7_REG_2(0x182d,0x282d).wr = 1;
    RFU_K7_W_2(0x182d,0x282d);
    RFU_K7_OP_2(0x182d,0x282d);
    RFU_K7_WAIT_IDLE_2(0x182d,0x282d,0,1000);
    RFU_K7_R_2(0x182f,0x282f);
    data = RFU_K7_REG_2(0x182f,0x282f).data;
	return 0;
}

int32_t sp1401_r1c::init_mcp23s17()
{
    INT_CHECK(set_mcp23s17_reg(0,0x00));
    INT_CHECK(set_mcp23s17_reg(1,0x00));
	return 0;
}

int32_t sp1401_r1c::get_mcp3208(uint32_t addr,uint32_t &data)
{
    RFU_K7_REG_DECLARE_2(0x1832,0x2832);
    RFU_K7_REG_DECLARE_2(0x1833,0x2833);

    RFU_K7_REG_2(0x1832,0x2832).addr = (unsigned)addr;
    RFU_K7_OP_2(0x1832,0x2832);
    RFU_K7_WAIT_IDLE_2(0x1832,0x2832,0,1000);
    RFU_K7_R_2(0x1833,0x2833);
    data = (uint32_t)(RFU_K7_REG_2(0x1833,0x2833).ad);
	return 0;
}
