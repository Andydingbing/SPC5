#include "sp9500pro_cal_tx_filter_thread.h"
#include "spec.h"
#include "test_data.hpp"
#include <QFile>

using namespace rd;
using namespace rd::ns_sp1403;
using namespace rd::ns_sp2406;
using namespace rd::ns_sp9500x;
using namespace NS_SP9500Pro;

void Q_Cal_TXFilter_Thread::cal()
{
    CAL_THREAD_START("TX Filter",(1));

//    setTempRange(2.0);

    const uint64_t freq_step = 0;

    if (justRebuildCoef != true) {
        THREAD_CHECK_BOX("TX<===>Power Meter");

        Instr.init();
        Instr.has_pm();
        Instr.pm_reset();

        init();
        THREAD_TEST_CANCEL;
        sweepRF(tx_freq_star,tx_freq_sec0,freq_step,cal_table_t::TX_RF_FR_0);
        THREAD_TEST_CANCEL;
        sweepRF(tx_freq_sec0,tx_freq_sec1,freq_step,cal_table_t::TX_RF_FR_1);
        THREAD_TEST_CANCEL;
        sweepIF();
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
//    SP2406->set_dl_jesd_src(port,dl_jesd_src_t::DDS);
//    SP2406->set_dl_jesd_src_dds(dl_jesd_src_dds_mode_t::Fine);
//    SP2406->set_dl_jesd_src_dds_gain(0.0);
//    SP2406->set_dl_pwr_comp(0);

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

    SP1403->set_io_mode(OUTPUT);

//    SP1401->set_tx_att(att0,att1,att2,att3);
//    SP1401->set_rx_lna_att_sw(r1c::RX_ATT);
//    SP1401->set_rx_att_019_sw(r1c::RX_ATT_19);
//    SP1401->set_rx_att(31.0,31.0,31.0);
    msleep(10);
}

void Q_Cal_TXFilter_Thread::sweepRF(const uint64_t star,
                                    const uint64_t stop,
                                    const uint64_t step,
                                    const int32_t table)
{
    fr_table_t::data_f_t data;
    uint64_t freq = 0;

    for (freq = star - BW_Max;freq < stop + BW_Max;freq += step) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL

        SP1403->set_tx_freq(freq);
        msleep(10);

        Instr.pm_get_pwr(freq,data.pwr);

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

void Q_Cal_TXFilter_Thread::sweepIF()
{
//    tx_if_fr_table::data_f_t data;

    SP1403->set_tx_freq(FREQ_G(2));
    msleep(10);

    for (quint32 i = 0;i < 1;i ++) {
        THREAD_TEST_PAUSE_S
        THREAD_TEST_CANCEL
//        data.freq = dl_filter_160M_freq_star + i * dl_filter_freq_step;
//        SP2401->set_duc_dds(data.freq);
//        msleep(10);

//        Instr.pm_get_pwr(2e9 + data.freq,data.pwr);

//        SP1401->get_tx_avg_temp(tempCur);
//        THREAD_CHECK_TEMP;

//        SP1401->cf()->add(cal_file::TX_IF_FR,&data);
//        qwtData->calTable()->append(data);
//        emit update(QModelIndex(),QModelIndex(),cal_file::TX_IF_FR);

//        SET_PROG_POS(tx_filter_freqs + i + 1);
        THREAD_TEST_PAUSE_E
    }
//    SP1401->cf()->w(cal_file::TX_IF_FR);
//    SP1401->cf()->m_tx_if_fr->save_as("c:\\tx_filter_if_fr.txt");
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
