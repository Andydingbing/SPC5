#include "sp1401_r1f.h"
#include "reg_def.h"
#include "algorithm.h"
#include "sleep_common.h"
#include "algo_chip.h"

using namespace sp_rd;
using namespace sp_rd::sp1401;

int32_t sp1401_r1f::open_board()
{
    double temp = 0.0;
    INT_CHECK(get_temp(0,temp));
    INT_CHECK(get_temp(5,temp));
    INT_CHECK(get_temp(6,temp));
    return sp1401_r1e::open_board();
}

int32_t sp1401_r1f::set_adf5355(lo_t lo,uint64_t freq)
{
    uint32_t reg[13];
    ZERO_ARRAY(reg);

    uint64_t freq_vco = freq;
    if (freq > 6800000000) freq_vco = freq / 2;
    if (freq < 3400000000) freq_vco = freq * 2;

    ns_adf5355::freq_formula_in param_in;
    ns_adf5355::freq_formula_out param_out;

    param_in.freq_vco = freq_vco;
    ns_adf5355::freq_formula(param_in,param_out);
    reg[0] = adf5355para2reg0(param_out._int,0,1);
    reg[1] = adf5355para2reg1(param_out._frac1);
    reg[2] = adf5355para2reg2(param_out._mod2,param_out._frac2);
    reg[4] = 0x30008594;

    if (lo == TX_LO_2) {
        reg[6] = freq < 3400000000 ? 0x3520a476 : 0x3500a476;
    } else {
        reg[6] = freq < 3400000000 ? 0x3520a076 : 0x3500a076;
    }

    DECLARE_R1C_SETLOREG_FUNCTION_MAP

    (this->*set_r1c_lo_reg[lo])(reg[6]);
    reg[4] = reg[4] | (1 << 4);
    (this->*set_r1c_lo_reg[lo])(reg[4]);
    (this->*set_r1c_lo_reg[lo])(reg[2]);
    (this->*set_r1c_lo_reg[lo])(reg[1]);
    reg[0] = reg[0] & uint32_t(~(1 << 21));
    (this->*set_r1c_lo_reg[lo])(reg[0]);
    reg[4] = reg[4] & uint32_t(~(1 << 4));
    (this->*set_r1c_lo_reg[lo])(reg[4]);
    sleep_ms(100);
    reg[0] = reg[0] | (1 << 21);
    (this->*set_r1c_lo_reg[lo])(reg[0]);
    return 0;
}

int32_t sp1401_r1f::tx_freq2lo(
        uint64_t freq,
        uint64_t &lo1,
        uint64_t &lo2,
        uint64_t &lo3,
        tx_filter_t &filter1,
        tx_filter_t &filter2)
{
    lo1 = 925000000;
    lo2 = 6574000000;
    lo3 = freq + lo1 + lo2;
    filter1 = (freq >= 3100000000 ? BPF : LPF);
    filter2 = (freq >= 4700000000 ? BPF : LPF);
    return 0;
}

int32_t sp1401_r1f::get_temp(uint32_t idx,double &temp)
{
    if (idx != 0 && idx != 5 && idx != 6) {
        temp = 0.0;
        return 0;
    }

    RFU_K7_REG_DECLARE_2(0x1839,0x2839);
    RFU_K7_REG_DECLARE_2(0x1840,0x2840);

    int16_t ad = 0;

    RFU_K7_REG_2(0x1839,0x2839).cfg = 1;
    RFU_K7_REG_2(0x1839,0x2839).incc = 7;

    // idx = 0 ---> CH4 RX LNA
    // idx = 5 ---> CH5 TX LO1
    // idx = 6 ---> CH6 TX PA3
    RFU_K7_REG_2(0x1839,0x2839).in = (idx < 5 ? 4 : idx);
    RFU_K7_REG_2(0x1839,0x2839).bw = 0;
    RFU_K7_REG_2(0x1839,0x2839).ref = ns_ad7949::INTERNAL_4_096_V;
    RFU_K7_REG_2(0x1839,0x2839).seq = 0;
    RFU_K7_REG_2(0x1839,0x2839).rb = 1;
    RFU_K7_REG_2(0x1839,0x2839).busy = 1;

    RFU_K7_OP_2(0x1839,0x2839);
    RFU_K7_WAIT_IDLE_2(0x1839,0x2839,0,INT32_MAX);

    RFU_K7_R_2(0x1840,0x2840);
    ad = RFU_K7_REG_2(0x1840,0x2840).ad;
    ad &= 0x1FFF;

    RFU_K7_REG_2(0x1839,0x2839).op = 0;
    RFU_K7_W_2(0x1839,0x2839);

    temp = tc1047_voltage_to_temp(ns_ad7949::ad_to_voltage_mv(ad,ns_ad7949::INTERNAL_4_096_V,3.0));
    return 0;
}
