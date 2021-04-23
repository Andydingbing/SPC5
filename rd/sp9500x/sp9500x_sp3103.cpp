#include "sp9500x_sp3103.h"
#include "reg_def_sp9500x.h"
#include "algo_fit.hpp"
#include "gen_ini_file.hpp"
#include <string>
#include <boost/core/ignore_unused.hpp>

using namespace std;
using namespace rd;
using namespace rd::ns_sp9500x;
using namespace rd::ns_sp1403;
using namespace rd::ns_sp3103;

#define DECL_DYNAMIC_SP1403 ns_sp9500x::sp1403 *SP1403 = _sp1403.at(rf_idx).get();
#define DECL_DYNAMIC_RRH    ns_sp9500x::rrh    *RRH    = _rrh.at(rf_idx).get();
#define DECL_DYNAMIC_SP2406 ns_sp9500x::sp2406 *SP2406 = _sp2406.at(rf_idx).get();

ns_sp9500x::sp3103::sp3103(uint32_t rfu_idx):
    rd::sp3103(rfu_idx)
{
    uint32_t rf_idx = 0;

    for (uint32_t i = 0;i < g_max_rf;i ++) {
        rf_idx = g_max_rf - 1 - i;
        _sp1403_r1a.push_back(boost::make_shared<sp1403_r1a>(rf_idx,rfu_idx));
        _sp1403_r1b.push_back(boost::make_shared<sp1403_r1b>(rf_idx,rfu_idx));
        _rrh.push_back(boost::make_shared<rrh>(rf_idx,rfu_idx));
        _sp1403.push_back(nullptr);
        _sp1403.at(i) = _sp1403_r1a.at(i);

        _sp2406.push_back(boost::make_shared<sp2406>(rf_idx));
    }
}

ns_sp9500x::sp3103 &ns_sp9500x::sp3103::instance(uint32_t rfu_idx)
{ 
    static sp3103 sp3103_0(0);
    static sp3103 sp3103_1(1);
    static sp3103 *g_sp3103[2] = { &sp3103_0, &sp3103_1 };

    return *g_sp3103[rfu_idx];
}

int32_t ns_sp9500x::sp3103::boot(const bool silent)
{
    list<pci_dev *> sp1403_ctrller;
    list<pci_dev *> sp2406_ctrller;

    SAFE_NEW(_v9,vi_pci_dev);
    SAFE_NEW(_s6,vi_pci_dev);

    _v9->set_descriptor("RFU_V9");
    _s6->set_descriptor("S6");

    if (_s6->init("PXI8::0::INSTR",0x10ee,0x3501)) {
        Log.add_msg("%s may not connected.",_s6->descriptor().c_str());
    }
    if (_v9->init("PXI3::0::INSTR",0x10ee,0x7)) {
        Log.add_msg("%s may not connected.",_v9->descriptor().c_str());
    }

    sp2406_ctrller.push_back(_v9);
    sp2406_ctrller.push_back(_s6);
    sp1403_ctrller.push_back(_v9);

    for (uint8_t i = 0;i < g_max_rf;++i) {
        BOOL_CHECK(_sp2406.at(i)->connect(sp2406_ctrller));
        BOOL_CHECK(_sp1403_r1a.at(i)->connect(sp1403_ctrller));
        BOOL_CHECK(_sp1403_r1b.at(i)->connect(sp1403_ctrller));
        BOOL_CHECK(_rrh.at(i)->connect(sp1403_ctrller));

        if (!silent) {
            INT_CHECK(_sp1403_r1a.at(i)->open_board());
            INT_CHECK(_sp2406.at(i)->open_board());
            INT_CHECK(_rrh.at(i)->open_board());
        }
    }

    return 0;
}

int32_t ns_sp9500x::sp3103::program_rfu_v9() const
{
    if (_is_program_rfu_v9 == false) {
        return 0;
    }

    hw_ver_t ver = hw_ver_t::_from_integral(working_sp1403(0)->hw_ver());
    string path = _bit_path_rfu_v9;

    path += "\\RFU\\";
    path += ver._to_string();
    path += "\\top.bit";
    return working_sp2406(0)->program(sp2406::RFU,path.c_str());
}

int32_t ns_sp9500x::sp3103::program_bbu_v9() const
{
    if (_is_program_bbu_v9 == false) {
        return 0;
    }

    string path = _bit_path_bbu_v9;

    path += "\\BBU\\top.bit";
    return working_sp2406(0)->program(sp2406::BBU,path.c_str());
}

hw_ver_t ns_sp9500x::sp3103::get_rf_ver(uint32_t rf_idx) const
{
    DECL_DYNAMIC_SP1403;

    return hw_ver_t::_from_integral(SP1403->hw_ver());
}

int32_t ns_sp9500x::sp3103::set_tx_en_tc(const uint32_t rf_idx,const bool en)
{
    DECL_DYNAMIC_SP1403;
    SP1403->set_tx_en_tc(en);
    return 0;
}

int32_t ns_sp9500x::sp3103::set_tx_state(const uint32_t rf_idx,const bool state)
{
    return 0;
}

int32_t ns_sp9500x::sp3103::set_tx_pwr(const uint32_t rf_idx,const double pwr)
{
    DECL_DYNAMIC_RRH;

    RRH->set_tx_pwr(pwr);
    return 0;
}

int32_t ns_sp9500x::sp3103::set_tx_freq(uint32_t rf_idx,uint64_t freq)
{
    DECL_DYNAMIC_SP1403;
    DECL_DYNAMIC_RRH;

    uint64_t freq_sub8 = 0;
    uint64_t freq_rrh  = 0;

    freq_to_sub8_rrh(freq,freq_sub8,freq_rrh);

    INT_CHECK(SP1403->set_tx_freq(freq_sub8));
    INT_CHECK(RRH->set_tx_freq(freq_rrh));
    return 0;
}

int32_t ns_sp9500x::sp3103::get_tx_freq(uint32_t rf_idx,uint64_t &freq)
{
    DECL_DYNAMIC_SP1403;
    DECL_DYNAMIC_RRH;

    uint64_t freq_sub8 = SP1403->tx_freq();
    uint64_t freq_rrh  = RRH->tx_freq();
    freq = (freq_rrh == 0 ? freq_sub8 : freq_rrh);
    return 0;
}

int32_t ns_sp9500x::sp3103::arb_load(const uint32_t rf_idx,const ns_arb::src_t &src,const string &arg)
{
    DECL_DYNAMIC_SP2406;

    uint64_t samples_left = 0;
    uint64_t samples_transed = 0;
    uint64_t samples_transing = 0;
    uint32_t cnt = 0;

    const uint32_t per_samples_max = SP2406->ddr()->dl_memory()->block_size(0) / 4;

    ns_arb::param_t arb_param;
    list<ns_arb::seg_header_t>::iterator iter_seg_header;
    list<ns_arb::seg_t>::iterator iter_arb_seg;

    SP2406->arb()->set_src(src);
    INT_CHECK(SP2406->arb()->load(arg.c_str(),arb_param));

    iter_seg_header = arb_param.seg_header.begin();
    iter_arb_seg = arb_param.seg.begin();

    INT_CHECK(SP2406->set_arb_abort());
    INT_CHECK(SP2406->set_arb_segs(uint8_t(arb_param.header.segments)));

    RFU_V9_REG_DECL(0x0190);
    RFU_V9_REG_DECL(0x0191);

    RFU_V9_REG_DECL(0x028a);
    RFU_V9_REG_DECL(0x028b);
    RFU_V9_REG_DECL(0x028c);
    RFU_V9_REG_DECL(0x0292);

    for (iter_arb_seg = arb_param.seg.begin();iter_arb_seg != arb_param.seg.end();++iter_arb_seg) {
        RFU_V9_REG(0x028b).seg     = iter_arb_seg->idx;
        RFU_V9_REG(0x028c).samples = iter_arb_seg->samples;
        RFU_V9_REG(0x0292).sr      = 983040000 / iter_arb_seg->sr;
        RFU_V9_W(0x028b);
        RFU_V9_W(0x028c);
        RFU_V9_W(0x0292);
        RFU_V9_OP(0x028a);
    }

    samples_left = arb_param.samples;
    Log.stdprintf("DMA:Prepare read,total samples:%d\n",samples_left);

    while(samples_left > 0)
    {
        ++cnt;
        samples_transing = samples_left > per_samples_max ? per_samples_max : samples_left;

        RFU_V9_REG(0x0190).addr_low  = (samples_transed * 4) & 0x00000000ffffffff;
        RFU_V9_REG(0x0191).addr_high = (samples_transed * 4) >> 32;
        RFU_V9_W(0x0190);
        RFU_V9_W(0x0191);

        SP2406->arb()->produce(0,uint32_t(samples_transing),SP2406->ddr()->dl());

        INT_CHECK(SP2406->ddr()->fpga_r(uint32_t(samples_transing)));

        samples_transed += samples_transing;
        samples_left -= samples_transing;
        Log.stdprintf("DMA:%-4d done,transed:%-8d\r",cnt,samples_transed);
    }

    Log.stdprintf("\n");
    Log.stdprintf("DMA:Read all done\n");
    return 0;
}

int32_t ns_sp9500x::sp3103::set_arb_en(const uint32_t rf_idx,const bool en) const
{
    DECL_DYNAMIC_SP2406;
    return en ? SP2406->set_arb_start() : SP2406->set_arb_abort();
}

int32_t ns_sp9500x::sp3103::set_rx_ref(const uint32_t rf_idx,const double ref)
{
    DECL_DYNAMIC_RRH;

    RRH->set_rx_ref(ref);
    return 0;
}

int32_t ns_sp9500x::sp3103::set_rx_freq(uint32_t rf_idx,uint64_t freq)
{
    DECL_DYNAMIC_SP1403;
    DECL_DYNAMIC_RRH;

    uint64_t freq_sub8 = 0;
    uint64_t freq_rrh  = 0;

    freq_to_sub8_rrh(freq,freq_sub8,freq_rrh);

    INT_CHECK(SP1403->set_rx_freq(freq_sub8));
    INT_CHECK(RRH->set_rx_freq(freq_rrh));
    return 0;
}

int32_t ns_sp9500x::sp3103::get_rx_freq(uint32_t rf_idx,uint64_t &freq)
{
    DECL_DYNAMIC_SP1403;
    DECL_DYNAMIC_RRH;

    uint64_t freq_sub8 = SP1403->rx_freq();
    uint64_t freq_rrh  = RRH->rx_freq();
    freq = (freq_rrh == 0 ? freq_sub8 : (freq_rrh + freq_sub8 - FREQ_M(7500)));
    return 0;
}

int32_t ns_sp9500x::sp3103::set_io_mode(uint32_t rf_idx,io_mode_t mode)
{
    DECL_DYNAMIC_SP1403;
    INT_CHECK(SP1403->set_io_mode(mode));
    return 0;
}

int32_t ns_sp9500x::sp3103::instance_sp1403(uint32_t rf_idx)
{
    bool is_connected = false;
    string sn;
    hw_ver_t ver = hw_ver_t::HW_ERROR;

    INT_CHECK(_sp1403.at(rf_idx)->get_sn_major(sn));
    if (true) {
        is_connected = true;

        switch (ver) {
            case hw_ver_t::R1A : {
                _sp1403.at(rf_idx) = _sp1403_r1a.at(rf_idx);
                break;
            }
            case hw_ver_t::R1B : {
                _sp1403.at(rf_idx) = _sp1403_r1b.at(rf_idx);
                break;
            }
            case hw_ver_t::HW_ERROR :
            case hw_ver_t::HW_VER_MAX : {
                _sp1403.at(rf_idx) = _sp1403_r1b.at(rf_idx);
                break;
            }
        }
    } else {
        if ((is_connected = _sp1403_r1a.at(rf_idx)->is_connected())) {
            _sp1403.at(rf_idx) = _sp1403_r1a.at(rf_idx);
        } else if ((is_connected = _sp1403_r1b.at(rf_idx)->is_connected())) {
            _sp1403.at(rf_idx) = _sp1403_r1b.at(rf_idx);
        } else
            _sp1403.at(rf_idx) = _sp1403_r1b.at(rf_idx);
    }

    return 0;
}
