#include "bb_3308_f.h"
#include "liblog.h"
#include "reg_def_3308_f.h"
#include "reg_def_ad9122.h"
#include "algo_chip_dac.hpp"

using namespace std;
using namespace rd;
using namespace rd::ns_dt3308;
using namespace ns_ad9122;

#ifdef RD_C_MSC
    #pragma data_seg("DT3308_ShareData")
        static bb_dt3308_f *g_bb_dt3308_f(new bb_dt3308_f);
    #pragma data_seg()
    #pragma comment(linker,"/section:DT3308_ShareData,rws")
#elif defined RD_C_GNUC
    static bb_dt3308_f *g_bb_dt3308_f __attribute__((section("DT3308_ShareData"))) = new bb_dt3308_f;
#endif

bb_dt3308_f *bb_dt3308_f::instance()
{
    return g_bb_dt3308_f;
}

int32_t bb_dt3308_f::open_board(pci_dev *k7)
{
    m_k7 = k7;
    _ddr = boost::make_shared<dma_mgr>(m_k7);
    _ddr->init();

    set_dl_en(true);
    set_dl_src(CW);
    return 0;
}

int32_t bb_dt3308_f::set_dl_pwr_comp(int32_t offset)
{
    KUS_REG_DECLARE(0x970101);

    KUS_REG(0x970101).comp = unsigned(32768 * (pow(10.0,offset/1000.0/20.0)/sqrt(2))+0.5);
    KUS_W(0x970101);
    return 0;
}

int32_t bb_dt3308_f::set_dl_en(const bool en)
{
    KUS_REG_DECLARE(0x900000);
    KUS_REG(0x900000).en = en ? 1 : 0;
    KUS_W(0x900000);
    return 0;
}

int32_t bb_dt3308_f::set_dl_src(const dl_src_t src)
{
    KUS_REG_DECLARE(0x900004);
    KUS_REG(0x900004).src = src;
    KUS_W(0x900004);
    return 0;
}

int32_t bb_dt3308_f::set_da_dc_offset_i(const uint16_t i)
{
    AD9122_REG_DECL_0x42;
    AD9122_REG_DECL_0x43;

    AD9122_REG(0x42).i_aux_dac = i & 0xff;
    AD9122_REG(0x43).i_aux_dac = i >> 8;

    INT_CHECK(set_da_reg(0x42,AD9122_REG_DATA(0x42)));
    INT_CHECK(set_da_reg(0x43,AD9122_REG_DATA(0x43)));
    return 0;
}

int32_t bb_dt3308_f::set_da_dc_offset_q(const uint16_t q)
{
    AD9122_REG_DECL_0x46;
    AD9122_REG_DECL_0x47;

    AD9122_REG(0x46).q_aux_dac = q & 0xff;
    AD9122_REG(0x47).q_aux_dac = q >> 8;

    INT_CHECK(set_da_reg(0x46,AD9122_REG_DATA(0x46)));
    INT_CHECK(set_da_reg(0x47,AD9122_REG_DATA(0x47)));
    return 0;
}

int32_t bb_dt3308_f::set_dl_dc_offset(uint16_t i,uint16_t q)
{
    INT_CHECK(set_da_dc_offset_i(i));
    INT_CHECK(set_da_dc_offset_q(q));
    return 0;
}

int32_t bb_dt3308_f::set_da_phase_adj_i(const double angle)
{
    AD9122_REG_DECL_0x38;
    AD9122_REG_DECL_0x39;

    uint16_t reg = 0;
    iq_phase_adj_angel_to_reg(angle,reg);

    AD9122_REG(0x38).i_phase_adj = reg & 0xff;
    AD9122_REG(0x39).i_phase_adj = reg >> 8;

    INT_CHECK(set_da_reg(0x38,AD9122_REG_DATA(0x38)));
    INT_CHECK(set_da_reg(0x39,AD9122_REG_DATA(0x39)));
    return 0;
}

int32_t bb_dt3308_f::set_da_phase_adj_q(const double angle)
{
    AD9122_REG_DECL_0x3a;
    AD9122_REG_DECL_0x3b;

    uint16_t reg = 0;
    iq_phase_adj_angel_to_reg(angle,reg);

    AD9122_REG(0x3a).q_phase_adj = reg & 0xff;
    AD9122_REG(0x3b).q_phase_adj = reg >> 8;

    INT_CHECK(set_da_reg(0x3a,AD9122_REG_DATA(0x3a)));
    INT_CHECK(set_da_reg(0x3b,AD9122_REG_DATA(0x3b)));
    return 0;
}

int32_t bb_dt3308_f::set_dl_phase_adj(double th_i,double th_q)
{
    INT_CHECK(set_da_phase_adj_i(th_i));
    INT_CHECK(set_da_phase_adj_q(th_q));
    return 0;
}

int32_t bb_dt3308_f::set_duc(const double freq)
{
    KUS_REG_DECLARE(0x970103);
    KUS_REG(0x970103).duc = unsigned(double(1 << 28) / double(245760000) * freq);
    KUS_W(0x970103);
    return 0;
}

int32_t bb_dt3308_f::set_ul_pwr_comp(int32_t offset)
{
    KUS_REG_DECLARE(0x970100);

    KUS_REG(0x970100).comp = unsigned(32768 * pow(10,float(offset)/1000/20)+0.5);
    KUS_W(0x970100);
    return 0;
}

int32_t bb_dt3308_f::set_ddc(const double freq)
{
    KUS_REG_DECLARE(0x970102);
    KUS_REG(0x970102).ddc = unsigned(double(1 << 29) / double(491520000) * freq);
    KUS_W(0x970102);
    return 0;
}

int32_t bb_dt3308_f::set_da_reg(uint8_t addr,uint8_t data)
{
    KUS_REG_DECLARE(0x960010);
    KUS_REG(0x960010).addr = addr;
    KUS_REG(0x960010).data = data;
    KUS_W(0x960010);
    return 0;
}

int32_t bb_dt3308_f::set_iq_cap_trig(const iq_cap_trig_t trig)
{
    KUS_REG_DECLARE(0x80000b);
    KUS_R(0x80000b);
    KUS_REG(0x80000b).trig = trig;
    KUS_W(0x80000b);
    return 0;
}

int32_t bb_dt3308_f::set_iq_cap_src(const iq_cap_src_t src)
{
    KUS_REG_DECLARE(0x80000b);
    KUS_R(0x80000b);
    KUS_REG(0x80000b).src = src;
    KUS_W(0x80000b);
    return 0;
}

uint32_t bb_dt3308_f::iq_cap_iq_sr(const iq_cap_src_t src)
{
    switch (src) {
    case JESD :
    case FIR_FLATNESS :
    case DEC_24576 : { return 245760000; }
    case DDC : { return 491520000; }
    case DEC_12288 : { return 122880000; }
    case DEC_6144 : { return 61440000; }
    case DEC_3072 : { return 30720000; }
    case DEC_1536 : { return 15360000; }
    }
    return 0;
}
