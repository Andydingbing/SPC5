#include "sp9500x_cal_tx_filter_thread.h"
#include "spec.h"
#include "test_data.hpp"
#include "algo_math.hpp"
#include <QFile>

using namespace ns_sp1403;
using namespace ns_sp2406;
using namespace ns_sp9500x;
using namespace NS_SP9500X;

void Q_Cal_TXFilter_Thread::cal()
{
    CAL_THREAD_START("TX Filter",(1));

//    setTempRange(2.0);

    const uint64_t freq_step = FREQ_M(2);

    if (justRebuildCoef != true) {
        THREAD_CHECK_BOX("TX<===>Power Meter");

//        Instr.init();
//        Instr.has_pm();
//        Instr.pm_reset();

        init();
        THREAD_TEST_CANCEL;
        sweepRF_IF_0000_3000(freq_step);
        THREAD_TEST_CANCEL;
        sweepRF_3000_4800(freq_step);
        THREAD_TEST_CANCEL;
        sweepRF_4800_6000(freq_step);
        THREAD_TEST_CANCEL;
        sweepRF_6000_7500(freq_step);
        THREAD_TEST_CANCEL;
        sweepIF_3000_7500(freq_step);
    }
    THREAD_TEST_CANCEL
//    generateFreqResponse();
//    THREAD_TEST_CANCEL
//    generateCoef();
//    THREAD_TEST_CANCEL
//    updateCalFile();

//    if (calParam.justRebuildCoef == true) {
//        SET_PROG_POS(tx_filter_freqs + dl_filter_160M_freqs);
//    }
}

void Q_Cal_TXFilter_Thread::init()
{
    SP2406->set_dl_src(dl_src_t::Disable);
    SP2406->set_dl_src(0,dl_src_t::DDS0);
    SP2406->set_dl_cpri_sr(dl_cpri_sr_t::_12288);
    SP2406->set_dl_lpf_en(true);
    SP2406->set_dl_hbf0_en(true);
    SP2406->set_dl_hbf1_en(true);
    SP2406->set_dl_cf_en(false);
    SP2406->set_dl_jesd_src(dl_jesd_src_t::DUC);
    SP2406->set_dl_pwr_comp(0);

    SP1403->set_io_mode(OUTPUT);

//    double att0 = 0.0;
//    double att1 = 0.0;
//    double att2 = 0.0;
//    double att3 = 0.0;
//    double d_gain = -5.0;

//    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
//        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,d_gain);
//    } else if (RFVer == R1F) {
//        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,d_gain);
//    }

//    SP1401->set_tx_att(att0,att1,att2,att3);
//    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
//    SP1401->set_rx_att_019_sw(r1c::RX_ATT_19);
//    SP1401->set_rx_att(31.0,31.0,31.0);
    msleep(10);
}

void Q_Cal_TXFilter_Thread::sweepRF_IF_0000_3000(const uint64_t step)
{
    cal_table_t table = cal_table_t::TX_RF_IF_FR_0000_3000;
    tx_rf_if_fr_0000_3000_table_t::data_f_t data;
    uint64_t freq = 0;
    int64_t  freq_dds1[4]  = {FREQ_M(-150),FREQ_M(-50),FREQ_M(50),FREQ_M(150)};
    int64_t  freq_dds1_cur = 0;

    int64_t  freq_if = 0;
    int64_t  freq_if_star = 0;
    int64_t  freq_if_stop = 0;

    uint32_t i = 0;

    BW_Max = FREQ_M(400);

    SP2406->set_dl_dds0(0,0);

    for (freq = tx_freq_star + BW_Max / 2;freq < tx_freq_sec0;freq += step) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        Log.stdprintf("freq = %ull\n",freq);
        msleep(10);

        i = 0;

        for (quint32 i = 0;i < ARRAY_SIZE(freq_dds1);++i) {
            freq_dds1_cur = freq_dds1[i];

            freq_if_star = (freq_dds1_cur == FREQ_M(-150) ? FREQ_M(-53) : FREQ_M(-50));
            freq_if_stop = (freq_dds1_cur == FREQ_M(150)  ? FREQ_M(53)  : FREQ_M(50));

            SP2406->set_dl_dds1(0,freq_dds1_cur);
            for (freq_if = freq_if_star;freq_if <= freq_if_stop;freq_if += step) {
                SP2406->set_dl_src_dds0_freq(freq_if);
                msleep(2);
                ++i;
            }
        }

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
//        SP1403->cal_file()->add(table,&data);

//        emit uiUpdate(0,0,table);

//        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepRF_3000_4800(const uint64_t step)
{
    cal_table_t table = cal_table_t::TX_RF_FR_3000_4800;
    tx_rf_fr_3000_4800_table_t::data_f_t data;
    uint64_t freq = 0;

    BW_Max = FREQ_M(800);

    for (freq = FREQ_M(3000) - BW_Max / 2;freq < FREQ_M(4800) + BW_Max / 2;freq += step) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        SP1403_R1B->set_tx0_sw1(ns_sp9500x::sp1403_r1b::tx_sw1_t::_3000_8000);
        SP1403_R1B->set_tx0_sw2(ns_sp9500x::sp1403_r1b::tx_sw2_t::_3000_6000);
        SP1403_R1B->set_tx0_sw3(ns_sp9500x::sp1403_r1b::tx_sw3_t::_3000_6000);
        SP1403_R1B->set_tx0_sw4(ns_sp9500x::sp1403_r1b::tx_sw4_t::_3000_4800);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pwr[0]);

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
        SP1403->cal_file()->add(table,&data);

        emit uiUpdate(0,0,table);

        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepRF_4800_6000(const uint64_t step)
{
    cal_table_t table = cal_table_t::TX_RF_FR_4800_6000;
    tx_rf_fr_4800_6000_table_t::data_f_t data;
    uint64_t freq = 0;

    BW_Max = FREQ_M(800);

    for (freq = FREQ_M(4800) - BW_Max / 2;freq < FREQ_M(6000) + BW_Max / 2;freq += step) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        SP1403_R1B->set_tx0_sw1(ns_sp9500x::sp1403_r1b::tx_sw1_t::_3000_8000);
        SP1403_R1B->set_tx0_sw2(ns_sp9500x::sp1403_r1b::tx_sw2_t::_3000_6000);
        SP1403_R1B->set_tx0_sw3(ns_sp9500x::sp1403_r1b::tx_sw3_t::_3000_6000);
        SP1403_R1B->set_tx0_sw4(ns_sp9500x::sp1403_r1b::tx_sw4_t::_4800_6000);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pwr[0]);

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
        SP1403->cal_file()->add(table,&data);

        emit uiUpdate(0,0,table);

        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepRF_6000_7500(const uint64_t step)
{
    cal_table_t table = cal_table_t::TX_RF_FR_6000_7500;
    tx_rf_fr_6000_7500_table_t::data_f_t data;
    uint64_t freq = 0;

    BW_Max = FREQ_M(800);

    for (freq = FREQ_M(6000) - BW_Max / 2;freq < FREQ_M(7500) + BW_Max / 2;freq += step) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        SP1403_R1B->set_tx0_sw1(ns_sp9500x::sp1403_r1b::tx_sw1_t::_3000_8000);
        SP1403_R1B->set_tx0_sw2(ns_sp9500x::sp1403_r1b::tx_sw2_t::_6000_8000);
        SP1403_R1B->set_tx0_sw3(ns_sp9500x::sp1403_r1b::tx_sw3_t::_6000_8000);
        SP1403_R1B->set_tx0_sw4(ns_sp9500x::sp1403_r1b::tx_sw4_t::_4800_6000);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pwr[0]);

//        SP1403->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

        data.freq = freq;
        SP1403->cal_file()->add(table,&data);

        emit uiUpdate(0,0,table);

        ADD_PROG_POS(1);
        THREAD_TEST_PAUSE_E
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::sweepIF_3000_7500(const uint64_t step)
{
    cal_table_t table = cal_table_t::TX_IF_FR_3000_7500;
    tx_if_fr_3000_7500_table_t::data_f_t data;
    uint64_t freq_rf = FREQ_G(2);
    int64_t  freq_dds0[4]  = {FREQ_M(-150),FREQ_M(-50),FREQ_M(50),FREQ_M(150)};
    int64_t  freq_dds0_cur = 0;
    int64_t  freq_dds1[2]  = {FREQ_M(-200),FREQ_M(200)};
    int64_t  freq_dds1_cur = 0;

    int64_t  freq_if = 0;
    int64_t  freq_if_star = 0;
    int64_t  freq_if_stop = 0;

    BW_Max = FREQ_M(800);

    SP1403->set_tx_freq(freq_rf);
    msleep(10);

    for (quint32 i = 0;i < ARRAY_SIZE(freq_dds1);++i) {
        freq_dds1_cur = freq_dds1[i];
        SP2406->set_dl_dds1(0,freq_dds1_cur);

        for (quint32 j = 0;j < ARRAY_SIZE(freq_dds0);++j) {
            THREAD_TEST_CANCEL
            freq_dds0_cur = freq_dds0[j];
            SP2406->set_dl_dds0(0,freq_dds0_cur);

            if (freq_dds1_cur == FREQ_M(-200) && freq_dds0_cur == FREQ_M(-150)) {
                freq_if_star = FREQ_M(-53);
            } else {
                freq_if_star = FREQ_M(-50);
            }

            if (freq_dds1_cur == FREQ_M(200)  && freq_dds0_cur == FREQ_M(150)) {
                freq_if_stop = FREQ_M(53);
            } else {
                freq_if_stop = FREQ_M(50);
            }

            for (freq_if = freq_if_star;freq_if <= freq_if_stop;freq_if += step) {
                THREAD_TEST_PAUSE_S
                THREAD_TEST_CANCEL
                SP2406->set_dl_src_dds0_freq(freq_if);

                data.freq = freq_dds0_cur + freq_dds1_cur + freq_if;
                Instr.pm_get_pwr(freq_rf + data.freq,data.pwr[0]);
                SP1403->cal_file()->add(table,&data);

                emit uiUpdate(0,0,table);
                ADD_PROG_POS(1);
                THREAD_TEST_PAUSE_E
            }
        }
    }
    SP1403->cal_file()->persistence(table);
}

void Q_Cal_TXFilter_Thread::generateFreqResponse()
{

}

void Q_Cal_TXFilter_Thread::generateCoef()
{

}

void Q_Cal_TXFilter_Thread::updateCalFile()
{

}

void Q_Cal_TXFilter_Thread::check()
{

}


void Q_Exp_TXFilter_Thread::run()
{

}
