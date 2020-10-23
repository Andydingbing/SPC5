#include "rd_sp9500x.h"
#include "liblog.h"
#include "global.hpp"
#include "reg_def_sp9500x.h"
#include "sp3103.h"
#include "sleep_common.h"
#include <vector>

#define SAFE_RF_INDEX (RFIndex < 2 ? RFIndex : 1)
#define DECL_DYNAMIC_SP3103 uint32_t rf_idx = (RFIndex < 2 ? 0 : 1);
#define DECL_DYNAMIC_SP2406 sp2406 *SP2406 = SP3103_0.working_sp2406(rf_idx);

using namespace std;
using namespace rd;
using namespace rd::ns_sp2406;
using namespace rd::ns_sp9500x;

static double g_rx_ref[2] = { 0.0,0.0 };

RD_SP9500X_IQ_Capture_Param::RD_SP9500X_IQ_Capture_Param()
{
    TriggerType = 0;
    RadioFrameCondition_X = 0;
    RadioFrameCondition_Y = 0;
    TriggerOffset = 0;
    MeasLength = 0;
    Channel = 0;
    SampleRate = SP9500X_UL_SR_98304;
}

RD_SP9500X_CA_Map::RD_SP9500X_CA_Map()
{

}

int32_t SP9500X_RF_SetBitPath(const char *Path)
{
    SP3103_0.set_program_rfu_v9(string(Path));
    SP3103_0.set_program_bbu_v9(string(Path));
    return 0;
}

int32_t SP9500X_RF_Boot()
{
    Log.en(log_t::RD_LOG_ALL_OFF, true);
    Log.en(log_t::RD_LOG_PROMPT, true);

    SP3103_0.boot(true);
    SP3103_0.program_bbu_v9();
    SP3103_0.program_rfu_v9();
    SP3103_0.boot(false);

    SP9500X_SetFans(120);
    return 0;
}

int32_t SP9500X_RF_GetPortNumber(uint32_t &Ports)
{
    return SP3103_0.get_rf_port(Ports);
}

int32_t SP9500X_RF_GetSerialNumber(const uint32_t RFIndex,char *SerialNumber)
{
    return 0;
}

int32_t SP9500X_RF_SetConnector(const uint32_t RFIndex,const RD_IO_Mode Connector)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_io_mode(rf_idx,io_mode_t(Connector));
}

int32_t SP9500X_RF_SetTxState(const uint32_t RFIndex,const bool State)
{
    return 0;
}

int32_t SP9500X_RF_SetTxPower(const uint32_t RFIndex,const float Power)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_tx_pwr(rf_idx,double(Power));
}

int32_t SP9500X_RF_SetTxFrequency(const uint32_t RFIndex,const uint64_t Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_tx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_GetTxFrequency(const uint32_t RFIndex,uint64_t &Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.get_tx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_SetRxLevel(const uint32_t RFIndex,const double Level)
{
    DECL_DYNAMIC_SP3103;
    INT_CHECK(SP3103_0.set_rx_ref(rf_idx,Level));
    g_rx_ref[SAFE_RF_INDEX] = Level;
    return 0;
}

int32_t SP9500X_RF_GetRxLevel(const uint32_t RFIndex,double &Level)
{
    Level = g_rx_ref[SAFE_RF_INDEX];
    return 0;
}

int32_t SP9500X_RF_SetRxFrequency(const uint32_t RFIndex,const uint64_t Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_rx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_GetRxFrequency(const uint32_t RFIndex,uint64_t &Freq)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.get_rx_freq(rf_idx,Freq);
}

int32_t SP9500X_RF_SetTxCAMap(const uint32_t RFIndex,const RD_SP9500X_CA_Map &Map)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    uint8_t ch = 0;
    dl_src_t src = dl_src_t::CPRI;
    double freq = 0.0;

    for (uint8_t i = 0;i < ARRAY_SIZE(Map.Carrier);++i) {
        ch = Map.Carrier[i].Index;
        src = Map.Carrier[i].Bandwidth == SP9500X_CA_Invalid ? dl_src_t::Disable : dl_src_t::CPRI;
        freq = Map.Carrier[i].Freq /*+ FREQ_M(200) * (ch < 4 ? 1 : -1)*/;

        INT_CHECK(SP2406->set_dl_src(ch,src));
        INT_CHECK(SP2406->set_dl_dds0(ch,freq));
        INT_CHECK(SP2406->set_dl_cpri_map(ch,i));
        INT_CHECK(SP2406->set_dl_hbf0_en(ch,Map.Carrier[i].Bandwidth == SP9500X_CA_100M));
        INT_CHECK(SP2406->set_dl_hbf1_en(ch,Map.Carrier[i].Bandwidth <= SP9500X_CA_200M));
    }
    return 0;
}

int32_t SP9500X_RF_SetRxCAMap(const uint32_t RFIndex,const RD_SP9500X_CA_Map &Map)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    uint8_t ch = 0;
    double freq = 0.0;

    for (uint8_t i = 0;i < ARRAY_SIZE(Map.Carrier);++i) {
        ch = Map.Carrier[i].Index;
        freq = Map.Carrier[i].Freq /*+ FREQ_M(200) * (ch < 4 ? 1 : -1)*/;

        INT_CHECK(SP2406->set_ul_hbf1_en(ch,Map.Carrier[i].Bandwidth <= SP9500X_CA_200M));
        INT_CHECK(SP2406->set_ul_hbf2_en(ch,Map.Carrier[i].Bandwidth == SP9500X_CA_100M));
        INT_CHECK(SP2406->set_ul_ddc1(ch,freq));
        INT_CHECK(SP2406->set_ul_cpri_map(ch,i));
    }
    return 0;
}

int32_t SP9500X_RF_LoadARBSource(const uint32_t RFIndex,const char *Path)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.arb_load(rf_idx,ns_arb::src_t::File,Path);
}

int32_t SP9500X_RF_SetARBEnable(const uint32_t RFIndex,const bool State)
{
    DECL_DYNAMIC_SP3103;
    return SP3103_0.set_arb_en(rf_idx,State);
}

int32_t SP9500X_RF_SetARBCount(const uint32_t RFIndex,const int Count)
{
    return 0;
}

int32_t SP9500X_RF_SetIQCaptureBuffer(const uint32_t RFIndex,int16_t **I,int16_t **Q)
{
//    if (*I == nullptr || *Q == nullptr) {
//        Log.set_last_err("IQ Capture buffer null.");
//        return -1;
//    }

    DECL_DYNAMIC_SP3103;
    *I = SP3103_0.working_sp2406(rf_idx)->ul_sequence()->i();
    *Q = SP3103_0.working_sp2406(rf_idx)->ul_sequence()->q();
    return 0;
}

int32_t SP9500X_RF_SetIQCaptureParams(const uint32_t RFIndex,const RD_SP9500X_IQ_Capture_Param &Param)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    iq_cap_src_ddc_t cap_src = iq_cap_src_ddc_t::CF;
    iq_cap_trig_src_t trig_src = iq_cap_trig_src_t::Frame;

    if (Param.SampleRate == SP9500X_UL_SR_98304) {
        cap_src = iq_cap_src_ddc_t::CF;
    } else if (Param.SampleRate == SP9500X_UL_SR_49152) {
        cap_src = iq_cap_src_ddc_t::HBF0;
    } else if (Param.SampleRate == SP9500X_UL_SR_24576) {
        cap_src = iq_cap_src_ddc_t::HBF1;
    } else {
        cap_src = iq_cap_src_ddc_t::LPF;
    }

    if (Param.TriggerType == 0) {
        trig_src = ns_sp2406::iq_cap_trig_src_t::NextFrame;
    } else if (Param.TriggerType == 1) {
        trig_src = ns_sp2406::iq_cap_trig_src_t::Frame;
    } else {
        trig_src = ns_sp2406::iq_cap_trig_src_t::ModXY;
        INT_CHECK(SP2406->set_iq_cap_trig_frame_x(uint16_t(Param.RadioFrameCondition_X)));
        INT_CHECK(SP2406->set_iq_cap_trig_frame_y(uint16_t(Param.RadioFrameCondition_Y)));
    }

    INT_CHECK(SP2406->set_iq_cap_sr(iq_cap_sr_t::_from_index(Param.SampleRate)));
    INT_CHECK(SP2406->set_iq_cap_src_ddc_ch(uint8_t(Param.Channel)));
    INT_CHECK(SP2406->set_iq_cap_src_ddc(cap_src));

    INT_CHECK(SP2406->set_iq_cap_samples(uint32_t(Param.MeasLength)));
    INT_CHECK(SP2406->set_iq_cap_trig_src(trig_src));
    INT_CHECK(SP2406->set_iq_cap_trig_offset(Param.TriggerOffset));
    return 0;
}

int32_t SP9500X_RF_SetIQCaptureStart(const uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    return SP2406->set_iq_cap_start();
}

int32_t SP9500X_RF_SetIQCaptureAbort(const uint32_t RFIndex)
{
    DECL_DYNAMIC_SP3103;
    DECL_DYNAMIC_SP2406;

    return SP2406->set_iq_cap_abort();
}

int32_t SP9500X_HardResetChips()
{
    SP9500PRO_S6_REG_DECL(0x0002);

    vi_pci_dev *_s6 = SP3103_0.s6();

    SP9500PRO_S6_OP(0x0002);

    SP9500PRO_S6_REG(0x0002).op = 0;
    SP9500PRO_S6_W(0x0002);
    SP9500PRO_S6_REG(0x0002).op = 1;
    SP9500PRO_S6_W(0x0002);
    sleep_ms(2);
    SP9500PRO_S6_REG(0x0002).op = 0;
    SP9500PRO_S6_W(0x0002);
    return 0;
}

int32_t SP9500X_SoftResetChips()
{
    SP9500PRO_S6_REG_DECL(0x0004);

    vi_pci_dev *_s6 = SP3103_0.s6();

    SP9500PRO_S6_OP(0x0004);
    SP9500PRO_S6_REG(0x0004).op = 0;
    SP9500PRO_S6_W(0x0004);
    return 0;
}

int32_t SP9500X_ZU21_ConfigStatus(bool &done)
{
    SP9500PRO_S6_REG_DECL(0x0120);

    vi_pci_dev *_s6 = SP3103_0.s6();

    SP9500PRO_S6_R(0x0120);
    done = SP9500PRO_S6_REG(0x0120).cfg_done == 1;
    return 0;
}

int32_t SP9500X_JESD_Reset(const RD_SP9500X_JESD jesd)
{
    SP9500PRO_RFU_V9_REG_DECL(0x0440);

    vi_pci_dev *_v9 = SP3103_0.v9();

    SP9500PRO_RFU_V9_REG_DATA(0x0440) = jesd;
    SP9500PRO_RFU_V9_W(0x0440);
    SP9500PRO_RFU_V9_REG_DATA(0x0440) = 0;
    SP9500PRO_RFU_V9_W(0x0440);
    return 0;
}

int32_t SP9500X_AD998X_Status(bool &status)
{
    return SP3103_0.working_sp2406(0)->jesd_status(status);
}


int32_t SP9500X_SetFan(const uint32_t FanIndex,const uint32_t Speed)
{
    SP9500PRO_RFU_V9_REG_DECL(0x0480);
    SP9500PRO_RFU_V9_REG_DECL(0x0481);
    SP9500PRO_RFU_V9_REG_DECL(0x0482);
    SP9500PRO_RFU_V9_REG_DECL(0x0485);
    SP9500PRO_RFU_V9_REG_DECL(0x0486);
    SP9500PRO_RFU_V9_REG_DECL(0x0487);

    vi_pci_dev *_v9 = SP3103_0.v9();

    SP9500PRO_RFU_V9_REG(0x0480).mode = 1;
    SP9500PRO_RFU_V9_REG(0x0485).mode = 1;
    SP9500PRO_RFU_V9_W(0x0480);
    SP9500PRO_RFU_V9_W(0x0485);

    if (FanIndex < 12) {
        SP9500PRO_RFU_V9_REG(0x0482).reg = 0x32 + FanIndex % 4;

        if (FanIndex < 4) {
            SP9500PRO_RFU_V9_REG(0x0482).id = 0x2f;
        } else if (FanIndex < 8) {
            SP9500PRO_RFU_V9_REG(0x0482).id = 0x2e;
        } else {
            SP9500PRO_RFU_V9_REG(0x0482).id = 0x2c;
        }

        SP9500PRO_RFU_V9_REG(0x0482).speed = Speed;
        SP9500PRO_RFU_V9_W(0x0482);
        SP9500PRO_RFU_V9_OP(0x0481);
    } else {
        SP9500PRO_RFU_V9_REG(0x0487).reg = 0x32 + FanIndex % 4;
        SP9500PRO_RFU_V9_REG(0x0487).id  = 0x2f;

        SP9500PRO_RFU_V9_REG(0x0487).speed = Speed;
        SP9500PRO_RFU_V9_W(0x0487);
        SP9500PRO_RFU_V9_OP(0x0486);
    }
    sleep_ms(10);
    return 0;
}

int32_t SP9500X_SetFans(const uint32_t Speed)
{
    for (uint32_t i = 0;i < g_max_fan;++i) {
        INT_CHECK(SP9500X_SetFan(i,Speed));
    }
    return 0;
}
