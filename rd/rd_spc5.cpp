#include "rd_spc5.h"
#include "liblog.h"
#include "sp3301.h"
#include "sp3501.h"
#include "reg_def.h"
#include "gen_ini_file.hpp"
#include <vector>

using namespace std;
using namespace rd;
using namespace rd::ns_sp9500;
using namespace rd::ns_sp1401;

struct available_rf_board_t {
    sp3301  *m_sp3301;	// the rfu this rf belongs to
    uint32_t m_rf_idx;	// the rf index
};

static vector<available_rf_board_t> g_rf_board;

#define DECLARE_DYNAMIC_SP3301 \
    if (RFIndex > g_rf_board.size() - 1) { \
        return -1; \
    } \
    sp3301  *pSP3301 = g_rf_board[RFIndex].m_sp3301; \
    uint32_t uiRfIdx = g_rf_board[RFIndex].m_rf_idx;

int32_t SPC5_RF_SetBitPath(char *Path)
{
    INT_CHECK(SP3301_2.set_program_bit(Path));
    INT_CHECK(SP3301_3.set_program_bit(Path));
    return 0;
}

int32_t SPC5_RF_Boot()
{
    Log.en(log_t::RD_LOG_ALL_OFF, true);
    Log.en(log_t::RD_LOG_PROMPT, true);
    g_rf_board.clear();
    available_rf_board_t rf_board;
    sp3301::active_t RfuActiveInfo[g_max_rfu];

    SP3301_2.boot();
    SP3301_3.boot();
    SP3301_2.program_bit();
    SP3301_3.program_bit();
    SP3301_2.boot();
    SP3301_3.boot();

    uint16_t ocxo_value = 0;
    SP3301_2.get_ocxo(ocxo_value);
    Log.stdprintf("ocxo : %d\n",ocxo_value);
    SP3501.open_board();
    SP3501.vol_9119(ocxo_value);

    RfuActiveInfo[2] = SP3301_2.is_actived();
    RfuActiveInfo[3] = SP3301_3.is_actived();

    for (uint32_t i = 0;i < g_max_rf;i ++) {
        if (RfuActiveInfo[2].sp1401[i]) {
            rf_board.m_sp3301 = &SP3301_2;
            rf_board.m_rf_idx = i;
            g_rf_board.push_back(rf_board);
        }
    }
    for (uint32_t i = 0;i < g_max_rf;i ++) {
        if (RfuActiveInfo[3].sp1401[i]) {
            rf_board.m_sp3301 = &SP3301_3;
            rf_board.m_rf_idx = i;
            g_rf_board.push_back(rf_board);
        }
    }
    return 0;
}

int32_t SPC5_RF_GetRFUNumber(uint32_t &uiRFUNumber)
{
    uiRFUNumber = 0;
    sp3301::rfu_info_t RfuInfo[g_max_rf];
    vector<string> strRsrcRfu;
    vector<string>::iterator iterRsrcRfu;
    vi_pci_dev viDev;

    char szProductForm[64];
    char szRsrcSection[64];
    char szDevKey[64];
    memset(szProductForm,0,ARRAY_SIZE(szProductForm));
    memset(szRsrcSection,0,ARRAY_SIZE(szRsrcSection));
    memset(szDevKey,0,ARRAY_SIZE(szDevKey));

    gen_ini_file IniFile(CONFIG_FILE_PATH);

    bool bK7_0_Active[g_max_rfu],bK7_1_Active[g_max_rfu],bS6_Active[g_max_rfu];
    for (int32_t i = 0;i < g_max_rfu;i ++) {
        bK7_0_Active[i] = false;
        bK7_1_Active[i] = false;
        bS6_Active[i] = false;
    }

    IniFile.get_config_str_value("Product Form","Form",szProductForm);
    strcpy(szRsrcSection,"RESOURCE");
    strcat(szRsrcSection,szProductForm);
    for (int32_t i = 0;i < g_max_rfu;i ++) {
        sprintf(szDevKey,"RFU%d_K7_0",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].k7_0);
        sprintf(szDevKey,"RFU%d_K7_1",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].k7_1);
        sprintf(szDevKey,"RFU%d_S6",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].s6);
    }

    viDev.get_devs(strRsrcRfu);
    for (int32_t i = 0;i < g_max_rfu;i ++) {
        for (iterRsrcRfu = strRsrcRfu.begin();iterRsrcRfu != strRsrcRfu.end();iterRsrcRfu ++ ) {
            if (0 == strcmp(RfuInfo[i].k7_0,iterRsrcRfu->c_str()))
                bK7_0_Active[i] = true;
            if (0 == strcmp(RfuInfo[i].k7_1,iterRsrcRfu->c_str()))
                bK7_1_Active[i] = true;
            if (0 == strcmp(RfuInfo[i].s6,iterRsrcRfu->c_str()))
                bS6_Active[i] = true;
        }
        if (bK7_0_Active[i] == true || bK7_1_Active[i] == true || bS6_Active[i] == true) {
            uiRFUNumber ++;
        }
    }
    return 0;
}

int32_t SPC5_RF_GetRFPortNumber(uint32_t &uiRFPortNumber)
{
    uiRFPortNumber = SP3301_2.get_rf_port() + SP3301_3.get_rf_port();
    return 0;
}

int32_t SPC5_RF_GetRFUSerialNumber(uint32_t RFUIndex,char *SerialNumber)
{
    switch (RFUIndex) {
        case 2 : {SP3301_2.get_sn(SerialNumber);break;}
        case 3 : {SP3301_3.get_sn(SerialNumber);break;}
        default:break;
    }
    return 0;
}

int32_t SPC5_RF_GetRFUVersion(char *version)
{
    SP3301_2.get_ver(version);
    return 0;
}

int32_t SPC5_RF_SetTxPower(uint32_t RFIndex,float Power)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_tx_pwr(uiRfIdx,Power));
    return 0;
}

int32_t SPC5_RF_SetTxFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_tx_freq(uiRfIdx,Freq));
    return 0;
}

int32_t SPC5_RF_SetTxSource(uint32_t RFIndex,RD_TX_Source Source)
{
    DECLARE_DYNAMIC_SP3301;
    sp2401_r1a::da_src_t TxSrc = sp2401_r1a::INTER_FILTER;

    switch (Source) {
        case RD_ARB : { break; }
        case RD_Phy : { TxSrc = sp2401_r1a::INTER_FILTER; break; }
        case RD_CW  : { TxSrc = sp2401_r1a::SINGLE_TONE;  break; }
    }
    INT_CHECK(pSP3301->rf_set_tx_src(uiRfIdx,TxSrc));
    return 0;
}

int32_t SPC5_RF_SetSourceFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_src_freq(uiRfIdx,Freq));
    return 0;
}

int32_t SPC5_RF_SetRxLevel(uint32_t RFIndex,double Level)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_rx_level(uiRfIdx,Level));
    return 0;
}

int32_t SPC5_RF_SetRxFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_rx_freq(uiRfIdx,Freq));
    return 0;
}

int32_t SPC5_RF_SetConnector(uint32_t RFIndex,RD_IO_Mode Connector)
{
    DECLARE_DYNAMIC_SP3301;
    io_mode_t Mode = rd::OUTPUT;
    switch (Connector) {
        case ::RD_IO     : {Mode = rd::IO;break;}
        case ::RD_Output : {Mode = rd::OUTPUT;break;}
        case ::RD_Loop   : {Mode = rd::LOOP;break;}
    }
    INT_CHECK(pSP3301->rf_set_io_mode(uiRfIdx,Mode));
    return 0;
}

int32_t SPC5_RF_SetTriggerMode(uint32_t RFIndex,RD_SPC5_Trigger_Mode TriggerMode)
{
    DECLARE_DYNAMIC_SP3301;
    sp1401::iq_cap_src_t MeasSrc = sp1401::PWR_MEAS_FREE_RUN;
    switch (TriggerMode) {
        case SPC5_IF      : {MeasSrc = sp1401::PWR_MEAS_IF_PWR;break;}
        case SPC5_FreeRun : {MeasSrc = sp1401::PWR_MEAS_FREE_RUN;break;}
        case SPC5_Marker  : {break;}
    }
    INT_CHECK(pSP3301->set_iq_cap_trig_src(uiRfIdx,MeasSrc));
    return 0;
}

int32_t SPC5_RF_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->set_iq_cap_trig_level(uiRfIdx,TriggerLevel));
    return 0;
}

int32_t SPC5_RF_SetCaptureLength(uint32_t RFIndex,uint32_t MLength)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->set_iq_cap_samples(uiRfIdx,MLength));
    return 0;
}

int32_t SPC5_RF_InitPowerMeasure(uint32_t RFIndex)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_init_pwr_meas(uiRfIdx));
    return 0;
}

int32_t SPC5_RF_AbortPowerMeasure(uint32_t RFIndex)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_abort_pwr_meas(uiRfIdx));
    return 0;
}

int32_t SPC5_RF_GetMeasProcess(uint32_t RFIndex, RD_SPC5_Power_Meas_Process &Process)
{
    DECLARE_DYNAMIC_SP3301;
    Process = SPC5_PM_Idle;
    sp1401::pwr_meas_state_t MeasState = sp1401::PMS_IDLE;
    INT_CHECK(pSP3301->rf_get_pwr_meas_proc(uiRfIdx,MeasState));
    switch (MeasState) {
        case sp1401::PMS_IDLE    : {Process = SPC5_PM_Idle;break;}
        case sp1401::PMS_WFT     : {Process = SPC5_PM_Wait;break;}
        case sp1401::PMS_TT      : {Process = SPC5_PM_Timeout;break;}
        case sp1401::PMS_RUNNING : {Process = SPC5_PM_Running;break;}
        case sp1401::PMS_DONE    : {Process = SPC5_PM_Done;break;}
    }
    return 0;
}

int32_t SPC5_RF_GetMeasResult(uint32_t RFIndex,float &Power,float &Crest)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_get_pwr_meas_result(uiRfIdx,Power,Crest));
    return 0;
}

int32_t SPC5_RF_GetTemperature(uint32_t RFIndex,double &TxTemperature,double &RxTemperature)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_get_temp(uiRfIdx,TxTemperature,RxTemperature));
    return 0;
}
