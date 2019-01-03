#include "liblog.h"
#include "libsp3301.h"
#include "libsp3501.h"
#include "reg_def.h"
#include "rd_rfu_sp9500.h"
#include <vector>

using namespace std;
using namespace sp_rd;
using namespace sp_rd::sp1401;

typedef struct AvailableModule {
    sp3301 *m_pSP3301;	//which rfu does this rf module belong
    uint32_t m_uiRfIdx;	//the rf module index of particular rfu above
}AvailableModule;

vector <AvailableModule> g_Module;

#define DECLARE_DYNAMIC_SP3301						\
    if (RFIndex > g_Module.size())					\
        return -1;									\
    sp3301 *pSP3301 = g_Module[RFIndex].m_pSP3301;	\
    uint32_t uiRfIdx = g_Module[RFIndex].m_uiRfIdx;

int32_t RF_SetBitPath(char *Path_0,char *Path_1)
{
    INT_CHECK(SP3301_2.set_program_bit(true,true,Path_0,Path_1));
    INT_CHECK(SP3301_3.set_program_bit(true,true,Path_0,Path_1));
    return 0;
}

int32_t RF_Boot()
{
    Log.set_enalbe(log_t::l_all_off, true);
    Log.set_enalbe(log_t::l_prompt, true);
    g_Module.clear();
    AvailableModule AvaiModule;
    sp3301::active_t RfuActiveInfo[MAX_RFU];

    SP3301_2.boot();
    SP3301_3.boot();
    SP3301_2.program_bit();
    SP3301_3.program_bit();
    SP3301_2.boot();
    SP3301_3.boot();

    int16_t iOCXO = 0;
    SP3301_2.get_ocxo(iOCXO);
    Log.stdprintf("ocxo : %d\n",iOCXO);
    SP3501.open_board();
    SP3501.vol_9119(iOCXO);

    RfuActiveInfo[2] = SP3301_2.is_actived();
    RfuActiveInfo[3] = SP3301_3.is_actived();

    for (int i = 0;i < MAX_RF;i ++) {
        if (RfuActiveInfo[2].sp1401[i]) {
            AvaiModule.m_pSP3301 = &SP3301_2;
            AvaiModule.m_uiRfIdx = i;
            g_Module.push_back(AvaiModule);
        }
    }
    for (int i = 0;i < MAX_RF;i ++) {
        if (RfuActiveInfo[3].sp1401[i]) {
            AvaiModule.m_pSP3301 = &SP3301_3;
            AvaiModule.m_uiRfIdx = i;
            g_Module.push_back(AvaiModule);
        }
    }
    return 0;
}

int32_t RF_GetRFUNumber(uint32_t &uiRFUNumber)
{
    uiRFUNumber = 0;
    sp3301::rfu_info_t RfuInfo[MAX_RF];
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

    bool bK7_0_Active[MAX_RFU],bK7_1_Active[MAX_RFU],bS6_Active[MAX_RFU];
    for (int32_t i = 0;i < MAX_RFU;i ++) {
        bK7_0_Active[i] = false;
        bK7_1_Active[i] = false;
        bS6_Active[i] = false;
    }

    IniFile.get_config_str_value("Product Form","Form",szProductForm);
    strcpy(szRsrcSection,"RESOURCE");
    strcat(szRsrcSection,szProductForm);
    for (int32_t i = 0;i < MAX_RFU;i ++) {
        sprintf(szDevKey,"RFU%d_K7_0",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].rsrc_name.k7_0);
        sprintf(szDevKey,"RFU%d_K7_1",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].rsrc_name.k7_1);
        sprintf(szDevKey,"RFU%d_S6",i);
        IniFile.get_config_str_value(szRsrcSection,szDevKey,RfuInfo[i].rsrc_name.s6);
    }

    viDev.get_devs(strRsrcRfu);
    for (int32_t i = 0;i < MAX_RFU;i ++) {
        for (iterRsrcRfu = strRsrcRfu.begin();iterRsrcRfu != strRsrcRfu.end();iterRsrcRfu ++ ) {
            if (0 == strcmp(RfuInfo[i].rsrc_name.k7_0,iterRsrcRfu->c_str()))
                bK7_0_Active[i] = true;
            if (0 == strcmp(RfuInfo[i].rsrc_name.k7_1,iterRsrcRfu->c_str()))
                bK7_1_Active[i] = true;
            if (0 == strcmp(RfuInfo[i].rsrc_name.s6,iterRsrcRfu->c_str()))
                bS6_Active[i] = true;
        }
        if (bK7_0_Active[i] == true || bK7_1_Active[i] == true || bS6_Active[i] == true) {
            uiRFUNumber ++;
        }
    }
    return 0;
}

int32_t RF_GetRFPortNumber(uint32_t &uiRFPortNumber)
{
    uiRFPortNumber = SP3301_2.get_rf_port() + SP3301_3.get_rf_port();
    return 0;
}

int32_t RF_GetRFUSerialNumber(uint32_t RFUIndex,char *SerialNumber)
{
    switch (RFUIndex) {
// 		case 0 : {SP3301_0.get_sn(SerialNumber);break;}
// 		case 1 : {SP3301_1.get_sn(SerialNumber);break;}
        case 2 : {SP3301_2.get_sn(SerialNumber);break;}
        case 3 : {SP3301_3.get_sn(SerialNumber);break;}
//		case 4 : {SP3301_4.get_sn(SerialNumber);break;}
        default:break;
    }
    return 0;
}

int32_t RF_GetRFUVersion(char *version)
{
    SP3301_2.get_ver(version);
    return 0;
}

int32_t RF_SetTxState(uint32_t RFIndex,bool State)
{
    return 0;
}

int32_t RF_SetTxPower(uint32_t RFIndex,float Power)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_tx_pwr(uiRfIdx,Power));
    return 0;
}

int32_t RF_SetTxFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_tx_freq(uiRfIdx,Freq));
    return 0;
}

int32_t RF_SetTxSource(uint32_t RFIndex,SOURCE Source)
{
    DECLARE_DYNAMIC_SP3301;
    sp2401_r1a::da_src_t TxSrc = sp2401_r1a::INTER_FILTER;

    switch (Source) {
        case ARB  : {break;}
        case FPGA : {TxSrc = sp2401_r1a::INTER_FILTER;break;}
        case CW   : {TxSrc = sp2401_r1a::SINGLE_TONE;break;}
        default:break;
    }
    INT_CHECK(pSP3301->rf_set_tx_src(uiRfIdx,TxSrc));
    return 0;
}

int32_t RF_SetSourceFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_src_freq(uiRfIdx,Freq));
    return 0;
}

int32_t RF_LoadARBSource(uint32_t RFIndex,char *filename)
{
//    DECLARE_DYNAMIC_SP3301;
//	INT_CHECK(pSP3301->RF_LoadArb(uiRfIdx,filename));
    return 0;
}

int32_t RF_SetARBEnable(uint32_t RFIndex,bool bState)
{
    return 0;
}

int32_t RF_SetARBTriggerSource(uint32_t RFIndex,ARB_TRIGGERMODE Source)
{
    return 0;
}

int32_t RF_ExcuteARBTrigger(uint32_t RFIndex)
{
    return 0;
}

int32_t RF_SetARBCount(uint32_t RFIndex,int iCnt)
{
    return 0;
}

int32_t RF_SetRxLevel(uint32_t RFIndex,double Level)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_rx_level(uiRfIdx,Level));
    return 0;
}

int32_t RF_SetRxFrequency(uint32_t RFIndex,uint64_t Freq)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_rx_freq(uiRfIdx,Freq));
    return 0;
}

int32_t RF_SetConnector(uint32_t RFIndex,CONNECTOR Connector)
{
    DECLARE_DYNAMIC_SP3301;
    io_mode_t Mode = sp1401::OUTPUT;
    switch (Connector) {
        case ::IO     : {Mode = sp1401::IO;break;}
        case ::OUTPUT : {Mode = sp1401::OUTPUT;break;}
        case ::LOOP   : {Mode = sp1401::LOOP;break;}
        default:break;
    }
    INT_CHECK(pSP3301->rf_set_io_mode(uiRfIdx,Mode));
    return 0;
}

int32_t RF_SetClockSource(uint32_t RFIndex,RFU_CLOCKSOURCE ClockSource)
{
    return 0;
}

int32_t RF_SetTriggerSource(uint32_t RFIndex,RFU_TRIGGERSOURCE TriggerSource)
{
    return 0;
}

int32_t RF_SetTriggerMode(uint32_t RFIndex,TRIGGERMODE TriggerMode)
{
    DECLARE_DYNAMIC_SP3301;
    basic_sp1401::pwr_meas_src_t MeasSrc = basic_sp1401::PWR_MEAS_FREE_RUN;
    switch (TriggerMode) {
        case IF		 : {MeasSrc = basic_sp1401::PWR_MEAS_IF_PWR;break;}
        case FREERUN : {MeasSrc = basic_sp1401::PWR_MEAS_FREE_RUN;break;}
        case MARKER  : {break;}
        default:break;
    }
    INT_CHECK(pSP3301->rf_set_trig_mode(uiRfIdx,MeasSrc));
    return 0;
}

int32_t RF_SetTriggerLevel(uint32_t RFIndex,float TriggerLevel)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_set_trig_level(uiRfIdx,TriggerLevel));
    return 0;
}

int32_t RF_SetTriggerOffset(uint32_t RFIndex,uint32_t Offset)
{
    return 0;
}

int32_t RF_SetCaptureLength(uint32_t RFIndex,uint32_t MLength)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->set_iq_cap_samples(uiRfIdx,MLength));
    return 0;
}

int32_t RF_SetIQDataMap(uint32_t RFIndex,uint16_t *pData)
{
    return 0;
}

int32_t RF_InitPowerMeasure(uint32_t RFIndex)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_init_pwr_meas(uiRfIdx));
    return 0;
}

int32_t RF_AbortPowerMeasure(uint32_t RFIndex)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_abort_pwr_meas(uiRfIdx));
    return 0;
}

int32_t RF_GetMeasProcess(uint32_t RFIndex,PROCESS &Process)
{
    DECLARE_DYNAMIC_SP3301;
    Process = IDLE_Driver;
    basic_sp1401::pwr_meas_state_t MeasState = basic_sp1401::PMS_IDLE;
    INT_CHECK(pSP3301->rf_get_pwr_meas_proc(uiRfIdx,MeasState));
    switch (MeasState) {
        case basic_sp1401::PMS_IDLE     : {Process = IDLE_Driver;break;}
        case basic_sp1401::PMS_WFT      : {Process = WFTrigger_Driver;break;}
        case basic_sp1401::PMS_TT       : {Process = Timeout_Driver;break;}
        case basic_sp1401::PMS_RUNNING  : {Process = RUNNING_Driver;break;}
        case basic_sp1401::PMS_DONE     : {Process = DONE_Driver;break;}
        default:break;
    }
    return 0;
}

int32_t RF_GetMeasResult(uint32_t RFIndex,float &Power,float &Crest)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_get_pwr_meas_result(uiRfIdx,Power,Crest));
    return 0;
}

int32_t RF_InitIQCapture(uint32_t RFIndex)
{
    return 0;
}

int32_t RF_AbortIQCapture(uint32_t RFIndex)
{
    return 0;
}

int32_t RF_GetIQCaptureProcess(uint32_t RFIndex,PROCESS &Process)
{
    return 0;
}

int32_t RF_WarningInfo(uint32_t RFIndex,uint32_t &State)
{
    return 0;
}

int32_t RF_GetTemperature(uint32_t RFIndex,double &TxTemperature,double &RxTemperature)
{
    DECLARE_DYNAMIC_SP3301;
    INT_CHECK(pSP3301->rf_get_temp(uiRfIdx,TxTemperature,RxTemperature));
    return 0;
}
