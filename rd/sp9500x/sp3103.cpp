#include "sp3103.h"
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

sp3103::sp3103(uint32_t rfu_idx):
    _rfu_idx(rfu_idx),
    _v9(nullptr),
    _s6(nullptr),
    _is_program_rfu_v9(false),
    _is_program_bbu_v9(false)
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

sp3103 &sp3103::instance(uint32_t rfu_idx)
{ 
    switch (rfu_idx) {
        case 0 : {static sp3103 sp3103_0(0);return sp3103_0;}
        case 1 : {static sp3103 sp3103_1(1);return sp3103_1;}
        default: {static sp3103 reserve(-1);return reserve;}
    }
}

int32_t sp3103::get_ocxo(uint16_t &value)
{
//    cal_file::x9119()->get(value);
    return 0;
}

int32_t sp3103::boot(const bool silent)
{
    list<pci_dev *> sp1403_ctrller;
    list<pci_dev *> sp2406_ctrller;

    SAFE_NEW(_v9,vi_pci_dev);
    SAFE_NEW(_s6,vi_pci_dev);

    _v9->set_descriptor("RFU_V9");
    _s6->set_descriptor("S6");

    if (_s6->init("PXI7::0::INSTR",0x10ee,0x3501)) {
        Log.add_msg("%s may not connected.",_s6->descriptor().c_str());
    }
    if (_v9->init("PXI16::0::INSTR",0x10ee,0x7)) {
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

//        instance_sp1403(i);

        if (!silent) {
//            if (is_connected) {
                INT_CHECK(_sp1403_r1a.at(i)->open_board());
//            }
            INT_CHECK(_sp2406.at(i)->open_board());
            INT_CHECK(_rrh.at(i)->open_board());
        }
    }

    return 0;
}

int32_t sp3103::program_rfu_v9() const
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

int32_t sp3103::program_bbu_v9() const
{
    if (_is_program_bbu_v9 == false) {
        return 0;
    }

    string path = _bit_path_bbu_v9;

    path += "\\BBU\\top.bit";
    return working_sp2406(0)->program(sp2406::BBU,path.c_str());
}

int32_t sp3103::get_rf_port(uint32_t &port) const
{
//    int32_t no = 0;
//    for (int i = 0;i < 4;i ++) {
//        if (m_active.sp1401[i] == true)
//            no ++;
//    }
//    return no;
    return 0;
}

hw_ver_t sp3103::get_rf_ver(uint32_t rf_idx) const
{
    DECL_DYNAMIC_SP1403;

    return hw_ver_t::_from_integral(SP1403->hw_ver());
}

int32_t sp3103::get_sn(char *sn)
{
//    strcpy(sn,m_rfu_info.sn);
    return 0;
}

int32_t sp3103::get_rf_sn(uint32_t rf_idx,char *sn)
{
//    DECL_DYNAMIC_SP1403
//    sp1401->get_sn_major(sn);
    return 0;
}

int32_t sp3103::get_ver(char *ver)
{
    strcpy(ver,"SP2401R1BRFUD160");
    return 0;
}

const char *sp3103::get_driver_ver()
{
//    extern const char *drive_ver;
//    return drive_ver;
    return nullptr;
}

void sp3103::set_tx_en_tc(const uint32_t rf_idx,const bool en)
{
    DECL_DYNAMIC_SP1403;
    SP1403->set_tx_en_tc(en);
    return;
}

int32_t sp3103::set_tx_state(const uint32_t rf_idx,const bool state)
{
//    DECL_DYNAMIC_SP1403
//    SP1403->set_tx_modulator_en(state);
    return 0;
}

int32_t sp3103::set_tx_pwr(const uint32_t rf_idx,const double pwr)
{
    DECL_DYNAMIC_RRH;

    RRH->set_tx_pwr(pwr);

//    uint64_t freq = m_tx_freq[rf_idx];
//    io_mode_t mode = m_io_mode[rf_idx];
//    m_tx_pwr[rf_idx] = double(pwr);
    return 0;
}

int32_t sp3103::set_tx_freq(uint32_t rf_idx,uint64_t freq)
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

int32_t sp3103::get_tx_freq(uint32_t rf_idx,uint64_t &freq)
{
    DECL_DYNAMIC_SP1403;
    DECL_DYNAMIC_RRH;

    uint64_t freq_sub8 = SP1403->tx_freq();
    uint64_t freq_rrh  = RRH->tx_freq();
    freq = (freq_rrh == 0 ? freq_sub8 : freq_rrh);
    return 0;
}

//int32_t sp3103::set_tx_bw(uint32_t rf_idx,bw_t bw)
//{
//    return 0;
//}

//int32_t sp3103::set_tx_src(uint32_t rf_idx,sp2401_r1a::da_src_t src)
//{
////    DECL_DYNAMIC_SP2401
////    INT_CHECK(sp2401->set_dds_src(src));
//    return 0;
//}

int32_t sp3103::arb_load(const uint32_t rf_idx,const ns_arb::src_t &src,const string &arg)
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

    SP9500X_RFU_V9_REG_DECL(0x0190);
    SP9500X_RFU_V9_REG_DECL(0x0191);

    SP9500X_RFU_V9_REG_DECL(0x028a);
    SP9500X_RFU_V9_REG_DECL(0x028b);
    SP9500X_RFU_V9_REG_DECL(0x028c);
    SP9500X_RFU_V9_REG_DECL(0x028d);
    SP9500X_RFU_V9_REG_DECL(0x028e);
    SP9500X_RFU_V9_REG_DECL(0x0291);
    SP9500X_RFU_V9_REG_DECL(0x0292);

    for (iter_arb_seg = arb_param.seg.begin();iter_arb_seg != arb_param.seg.end();++iter_arb_seg) {
        SP9500X_RFU_V9_REG(0x028b).seg     = iter_arb_seg->idx;
        SP9500X_RFU_V9_REG(0x028c).samples = iter_arb_seg->samples;
        SP9500X_RFU_V9_REG(0x0292).sr      = 983040000 / iter_arb_seg->sr;
        SP9500X_RFU_V9_W(0x028b);
        SP9500X_RFU_V9_W(0x028c);
        SP9500X_RFU_V9_W(0x0292);
        SP9500X_RFU_V9_OP(0x028a);
    }

    samples_left = arb_param.samples;
    Log.stdprintf("DMA:Prepare read,total samples:%d\n",samples_left);

    while(samples_left > 0)
    {
        ++cnt;
        samples_transing = samples_left > per_samples_max ? per_samples_max : samples_left;

        SP9500X_RFU_V9_REG(0x0190).addr_low  = (samples_transed * 4) & 0x00000000ffffffff;
        SP9500X_RFU_V9_REG(0x0191).addr_high = (samples_transed * 4) >> 32;
        SP9500X_RFU_V9_W(0x0190);
        SP9500X_RFU_V9_W(0x0191);

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

int32_t sp3103::set_arb_en(const uint32_t rf_idx,const bool en) const
{
    DECL_DYNAMIC_SP2406;
    return en ? SP2406->set_arb_start() : SP2406->set_arb_abort();
}

int32_t sp3103::set_rx_ref(const uint32_t rf_idx,const double ref)
{
    DECL_DYNAMIC_RRH;

    RRH->set_rx_ref(ref);
//    DECL_DYNAMIC_SP2401
//    DECL_RF_VER

//    uint64_t freq = m_rx_freq[rf_idx];
//    io_mode_t mode = m_io_mode[rf_idx];

//    switch (RF_VER) {
//        case R1A : case R1B : {
//            r1a::rx_lna_att_t lna_att = r1a::RX_ATT;
//            double att1 = 0.0;
//            int32_t att2 = 0;
//            int64_t ad_0dbfs = g_0dBFS;

//            CAL_FILE_R1A->m_rx_ref->get(freq,level,mode,ad_0dbfs,lna_att,att1,att2);
//            INT_CHECK(SP1401_R1A->set_rx_lna_att_sw(lna_att));
//            INT_CHECK(SP1401_R1A->set_rx_att(att1,att2));
//            INT_CHECK(sp2401->set_rx_pwr_comp(int32_t(g_0dBFS - ad_0dbfs)));
//            break;
//        }
//        case R1C : case R1D : case R1E : case R1F : {
//            rx_ref_op_table_r1cd::rx_state_m_t rx_state;
//            int32_t offset = 0;
//            const bool en_tc = sp1401->is_rx_en_tc();
//            double temp_call = 0.0;
//            double temp_curr = 0.0;
//            double temp_comp = 0.0;

//            rx_tc_table::data_m_t data_tc;

//            if (mode == OUTPUT) {
//                CAL_FILE_R1C->m_rx_ref_op->get(RF_VER,freq,level,&rx_state);
//                offset = CAL_FILE_R1C->m_rx_att_op->get(RF_VER,freq,level);
//                offset += rx_state.ad_offset;
//            } else if (mode == IO) {
//                CAL_FILE_R1C->m_rx_ref_io->get(RF_VER,freq,level,&rx_state);
//                offset = CAL_FILE_R1C->m_rx_att_io->get(RF_VER,freq,level);
//                offset += rx_state.ad_offset;
//            }

//            if (en_tc && rx_state.lna_att == r1c::RX_LNA) {
//                if (((sp1401_r1c *)sp1401)->get_rx_avg_temp(temp_curr) == true) {
//                    CAL_FILE_R1C->m_rx_tc->get(freq,data_tc);
//                    if (data_tc.order > 0) {
//                        temp_call = rx_state.temp;
//                        temp_curr = temp_curr >= data_tc.temp_star ? temp_curr : data_tc.temp_star;
//                        temp_curr = temp_curr <= data_tc.temp_stop ? temp_curr : data_tc.temp_stop;
//                        temp_comp = polynomial<double>(data_tc.coef,data_tc.order,temp_call) -
//                                    polynomial<double>(data_tc.coef,data_tc.order,temp_curr);
//                        offset = g_0dBFS - dBc_to_ad(g_0dBFS,temp_comp - ad_to_dBc(g_0dBFS,g_0dBFS - offset));
//                    }
//                }
//            }

//            INT_CHECK(SP1401_R1C->set_rx_lna_att_sw(r1c::rx_lna_att_t(rx_state.lna_att)));
//            INT_CHECK(SP1401_R1C->set_rx_att_019_sw(r1c::rx_att_019_t(rx_state.att_019)));
//            INT_CHECK(SP1401_R1C->set_rx_att(rx_state.att1,rx_state.att2,rx_state.att3));
//            INT_CHECK(sp2401->set_rx_pwr_comp(offset));
//            break;
//        }
//        default:break;
//    }
//    m_ref[rf_idx] = level;
    return 0;
}

int32_t sp3103::set_rx_freq(uint32_t rf_idx,uint64_t freq)
{
    DECL_DYNAMIC_SP1403;
    DECL_DYNAMIC_RRH;

    uint64_t freq_sub8 = 0;
    uint64_t freq_rrh  = 0;

    freq_to_sub8_rrh(freq,freq_sub8,freq_rrh);

    INT_CHECK(SP1403->set_rx_freq(freq_sub8));
    INT_CHECK(RRH->set_rx_freq(freq_rrh));

//    uint64_t freq_rf = freq / g_rf_freq_space * g_rf_freq_space;
//    double freq_ddc = -92640000.0 - double(freq - freq_rf);

//    INT_CHECK(SP1403->set_rx_freq(freq_rf));
//    INT_CHECK(sp2401->set_ddc(freq_ddc));
//    m_rx_freq[rf_idx] = freq;
//    INT_CHECK(rf_set_rx_level(rf_idx,m_ref[rf_idx]));
//    INT_CHECK(rf_set_rx_bw(rf_idx,sp1401->get_bw()));
    return 0;
}

int32_t sp3103::get_rx_freq(uint32_t rf_idx,uint64_t &freq)
{
    DECL_DYNAMIC_SP1403;
    DECL_DYNAMIC_RRH;

    uint64_t freq_sub8 = SP1403->rx_freq();
    uint64_t freq_rrh  = RRH->rx_freq();
    freq = (freq_rrh == 0 ? freq_sub8 : (freq_rrh + freq_sub8 - FREQ_M(7500)));
    return 0;
}

//int32_t sp3103::rf_set_rx_bw(uint32_t rf_idx,bw_t bw)
//{
//    DECL_DYNAMIC_SP1403
//    DECL_DYNAMIC_SP2401

//    switch (sp1401->get_hw_ver()) {
//        case R1C : case R1D : case R1E : case R1F : {
//            CAL_FILE_R1C->set_bw(bw);
//            double real[ul_filter_tap] = {0.0};
//            double imag[ul_filter_tap] = {0.0};

//            if (_80M == bw) {
//                rx_filter_80m_table::data_m_t data;

//                CAL_FILE_R1C->m_rx_filter_80m->get(m_rx_freq[rf_idx],&data);
//                data._2double(real,imag);
//                sp2401->set_rx_filter(real,imag);
//            } else if (_160M == bw) {
//                rx_filter_160m_table::data_m_t data;

//                CAL_FILE_R1C->m_rx_filter_160m->get(m_rx_freq[rf_idx],&data);
//                data._2double(real,imag);
//                sp2401->set_rx_filter(real,imag);
//            }
//            return 0;
//        }
//        default:return 0;
//    }
//    return 0;
//}

int32_t sp3103::set_io_mode(uint32_t rf_idx,io_mode_t mode)
{
    DECL_DYNAMIC_SP1403;
    INT_CHECK(SP1403->set_io_mode(mode));
    return 0;
}

int32_t sp3103::get_temp(uint32_t rf_idx,double &tx_temp,double &rx_temp)
{
//    DECL_DYNAMIC_SP1403

//    tx_temp = 0.0;
//    rx_temp = 0.0;
//    switch (sp1401->get_hw_ver()) {
//        case R1A : case R1B : {
//            INT_CHECK(SP1401_R1A->get_tx_temp(tx_temp));
//            INT_CHECK(SP1401_R1A->get_rx_temp(rx_temp));
//            return 0;
//        }
//        case R1C : case R1D : case R1E : {
//            double tx_t[4] = {0.0,0.0,0.0,0.0};
//            double rx_t[4] = {0.0,0.0,0.0,0.0};
//            INT_CHECK(SP1401_R1C->get_temp(4,tx_t[0]));
//            INT_CHECK(SP1401_R1C->get_temp(5,tx_t[1]));
//            INT_CHECK(SP1401_R1C->get_temp(6,tx_t[2]));
//            INT_CHECK(SP1401_R1C->get_temp(7,tx_t[3]));
//            INT_CHECK(SP1401_R1C->get_temp(0,rx_t[0]));
//            INT_CHECK(SP1401_R1C->get_temp(1,rx_t[1]));
//            INT_CHECK(SP1401_R1C->get_temp(2,rx_t[2]));
//            INT_CHECK(SP1401_R1C->get_temp(3,rx_t[3]));

//            for (int32_t i = 0;i < 4;i ++) {
//                tx_temp += tx_t[i];
//                rx_temp += rx_t[i];
//            }
//            tx_temp /= 4.0;
//            rx_temp /= 4.0;
//            return 0;
//        }
//        case R1F : {
//            double tx_t[2] = { 0.0,0.0 };
//            double rx_t = 0.0;
//            INT_CHECK(SP1401_R1F->get_temp(5,tx_t[0]));
//            INT_CHECK(SP1401_R1F->get_temp(6,tx_t[1]));
//            INT_CHECK(SP1401_R1F->get_temp(0,rx_t));

//            tx_temp = tx_t[0];
//            //tx_temp = (tx_t[0] + tx_t[1]) / 2.0;
//            rx_temp = rx_t;
//            return 0;
//        }
//        default:return 0;
//    }
    return 0;
}

int32_t sp3103::instance_sp1403(uint32_t rf_idx)
{
    bool is_connected = false;
    string sn;
    hw_ver_t ver = hw_ver_t::HW_ERROR;

    INT_CHECK(_sp1403.at(rf_idx)->get_sn_major(sn));
    if (true/*sp1403::is_valid_sn(sn)*/) {
        is_connected = true;
//        sp1403::get_hw_ver(sn,ver);
        switch (ver) {
            case hw_ver_t::R1A : {
                _sp1403.at(rf_idx) = _sp1403_r1a.at(rf_idx);
                break;
            }
            case hw_ver_t::R1B : {
                _sp1403.at(rf_idx) = _sp1403_r1b.at(rf_idx);
                break;
            }

//            case R1F : case HW_VER_SP9500_MAX : case HW_ERROR : {
//                m_sp1401->at(rf_idx) = m_sp1401_r1f->at(rf_idx);
//                break;
//            }
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

void sp3103::freq_to_sub8_rrh(const uint64_t freq,uint64_t &sub8,uint64_t &rrh)
{
    if (freq > FREQ_M(7500)) {
        sub8 = FREQ_M(7500);
        rrh  = freq;
    } else {
        sub8 = freq;
        rrh  = 0;
    }
}
