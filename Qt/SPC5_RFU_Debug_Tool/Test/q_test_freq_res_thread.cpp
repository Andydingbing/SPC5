#include "q_test_freq_res_thread.h"
#include "q_model_freq_res.h"
#include "algorithm.h"
#include "algo_math.h"
#include "spec.h"

void QTestR1CFreqResThread::run()
{
    RD_TEST_TRY

    param = static_cast<TestFreqResParam *>(testParam);

    RF_TXFreqString.star = param->RF_TXFreqStar.toStdString();
    RF_TXFreqString.stop = param->RF_TXFreqStop.toStdString();
    RF_TXFreqString.step = param->RF_TXFreqStep.toStdString();

    RF_RXFreqString.star = param->RF_RXFreqStar.toStdString();
    RF_RXFreqString.stop = param->RF_RXFreqStop.toStdString();
    RF_RXFreqString.step = param->RF_RXFreqStep.toStdString();

    parse_range_freq_string(RF_TXFreqString,RF_TXFreqRange);
    parse_range_freq_string(RF_RXFreqString,RF_RXFreqRange);

    dataRF_TX = dynamic_cast<QwtRF_TX_FreqResData *>(param->plotData_0);
    dataIF_TX = dynamic_cast<QwtIF_TX_FreqResData *>(param->plotData_1);
    dataRF_RX = dynamic_cast<QwtRF_RX_FreqResData *>(param->plotData_2);
    dataIF_RX = dynamic_cast<QwtIF_RX_FreqResData *>(param->plotData_3);

    quint32 pts = 0;

    pts += param->isTestRF_TX ? RF_TXFreqRange.freqs.size() : 0;
    pts += param->isTestRF_RX ? RF_RXFreqRange.freqs.size() : 0;
    pts += param->isTestIF_TX ? IF_TX_FILTER_160M_PTS : 0;
    pts += param->isTestIF_RX ? IF_RX_FILTER_160M_PTS : 0;

    TEST_THREAD_START("TX FreqResponse",pts);

    if (param->isTestRF_TX || param->isTestIF_TX) {
        sweepTX();
    }

    THREAD_TEST_CANCEL

    if (param->isTestRF_RX || param->isTestIF_RX) {
        sweepRX();
    }

    THREAD_ENDED
    RD_TEST_CATCH
}

void QTestR1CFreqResThread::sweepTX()
{
    if (param->isTestRF_TX || param->isTestIF_TX) {
        THREAD_CHECK_BOX("TX<===>Spectrum");
    }

    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_span(1000000.0);
    Instr.sa_set_rbw(1000.0,true);
    Instr.sa_set_vbw(1000.0,true);
    Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0);

    quint64 RF_Freq = 0;
    double IF_Freq = 0.0;
    double dds1 = 20e6;
    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = 0.0;

    freq_res_data data;

    if (RFVer >= R1C && RFVer <= R1E) {
        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,d_gain);
    } else if (RFVer >= R1F) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,d_gain);
    }

    SP1401->set_io_mode(OUTPUT);
    SP1401->set_tx_att(att0,att1,att2,att3);
    SP1401->set_rx_att_019_sw(sp1401::RX_ATT_19);
    SP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
    SP1401->set_rx_att(30.0,30.0,30.0);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_tx_pwr_comp(d_gain);
    SP2401->set_dds1(dds1);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);

    if (param->isTestRF_TX) {
        totalResult = true;
        dataRF_TX->report()->set_result("Pass");

        for (quint32 i = 0;i < RF_TXFreqRange.freqs.size();i ++) {
            THREAD_TEST_CANCEL

            RF_Freq = RF_TXFreqRange.freqs.at(i);
            SP1401->set_tx_freq(RF_Freq);
            Instr.sa_set_cf(RF_Freq + dds1);
            msleep(50);

            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(data.pwr);

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkRF_TX(data);
            dataRF_TX->report()->add(RF_Freq,data);
            pwrBefore = data.pwr;
        }
        dataRF_TX->report()->update();
        SP1401->ftp_put_tr_rf_tx_freq_res_test();
    }

    if (param->isTestIF_TX) {
        totalResult = true;
        dataIF_TX->report()->set_result("Pass");

        RF_Freq = 2000000000;
        Instr.sa_set_cf(RF_Freq);
        Instr.sa_set_span(200e6);
        SP1401->set_tx_freq(RF_Freq);

        for (quint32 i = 0;i < IF_TX_FILTER_160M_PTS;i ++) {
            THREAD_TEST_CANCEL

            IF_Freq = IF_TX_FILTER_160M_FREQ_STAR + i * IF_TX_FILTER_FREQ_STEP;
            SP2401->set_dds1(IF_Freq);
            msleep(50);

            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(data.pwr);

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkIF_TX(data);
            dataIF_TX->report()->add(qint64(IF_Freq),data);
            pwrBefore = data.pwr;
        }
        dataIF_TX->report()->update();
        SP1401->ftp_put_tr_if_tx_freq_res_test();
    }
}

void QTestR1CFreqResThread::checkRF_TX(rf_tx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_rf_tx_freq_res_min() || data.pwr > spec::test_rf_tx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 5.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_rf_tx_freq_res()) {
        totalResult = false;
        dataRF_TX->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_RF_TX_FREQ_RES,int(totalResult));
}

void QTestR1CFreqResThread::checkIF_TX(if_tx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_if_tx_freq_res_min() || data.pwr > spec::test_if_tx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 3.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_if_tx_freq_res()) {
        totalResult = false;
        dataIF_TX->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_IF_TX_FREQ_RES,int(totalResult));
}

void QTestR1CFreqResThread::sweepRX()
{
    if (param->isTestRF_RX || param->isTestIF_RX) {
        THREAD_CHECK_BOX("RX<===>Signal Generator");
    }

    Instr.init();
    Instr.has_sg();
    Instr.sg_reset();
    Instr.sg_set_en_mod(false);
    Instr.sg_set_en_output(true);

    quint64 RF_Freq = 0;
    double IF_Freq = 0.0;
    qint32 ref = 0;
    qint32 lnaAtt = 0;
    qint32 att019 = 0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    qint64 ad = 0;

    freq_res_data data;

    if (RFVer >= R1C && RFVer <= R1E) {
        rx_ref_op_table_r1cd::guess_base_r1c(1,ref,lnaAtt,att019,att1,att2,att3);
    } else if (RFVer >= R1F) {
        rx_ref_op_table_r1cd::guess_base_r1f(1,ref,lnaAtt,att019,att1,att2);
    }

    SP1401->set_io_mode(OUTPUT);
    SP1401->set_tx_att(30.0,30.0,30.0,30.0);
    SP1401->set_rx_lna_att_sw(sp1401::rx_lna_att_t(lnaAtt));
    SP1401->set_rx_att_019_sw(sp1401::rx_att_019_t(att019));
    SP1401->set_rx_att(att1,att2,att3);
    SP1401->set_pwr_meas_src(basic_sp1401::PWR_MEAS_FREE_RUN,false);
    SP1401->set_pwr_meas_timeout(6553600);
    SP1401->set_pwr_meas_samples(102400);
    SP2401->set_rx_filter_default();

    Instr.sg_set_pl(ref - 5.0);

    if (param->isTestRF_RX) {
        totalResult = true;
        dataRF_RX->report()->set_result("Pass");

        for (quint32 i = 0;i < RF_RXFreqRange.freqs.size();i ++) {
            THREAD_TEST_CANCEL

            RF_Freq = RF_RXFreqRange.freqs.at(i);
            SP1401->set_rx_freq(RF_Freq);
            Instr.sg_set_cw(RF_Freq);
            msleep(200);

            getADS5474(SP1401,ad,1);
            data.pwr = ad_to_dBc(_0dBFS,ad);

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkRF_RX(data);
            dataRF_RX->report()->add(RF_Freq,data);
            pwrBefore = data.pwr;
        }
        dataRF_RX->report()->update();
        SP1401->ftp_put_tr_rf_rx_freq_res_test();
    }

    if (param->isTestIF_RX) {
        totalResult = true;
        dataIF_RX->report()->set_result("Pass");

        RF_Freq = 2000000000;
        SP1401->set_rx_freq(RF_Freq);

        for (quint32 i = 0;i < IF_RX_FILTER_160M_PTS;i ++) {
            THREAD_TEST_CANCEL

            IF_Freq = IF_RX_FILTER_160M_FREQ_STAR + i * IF_RX_FILTER_FREQ_STEP;

            Instr.sg_set_cw(RF_Freq + IF_Freq);
            msleep(100);
            data.pwr = getRXAvgPwr();

            if (i == 0) {
                pwrBefore = data.pwr;
                pwrMin = data.pwr;
                pwrMax = data.pwr;
            } else {
                if (pwrMin > data.pwr)
                    pwrMin = data.pwr;
                if (pwrMax < data.pwr)
                    pwrMax = data.pwr;
            }
            checkIF_RX(data);
            dataIF_RX->report()->add(qint64(IF_Freq),data);
            pwrBefore = data.pwr;
        }
        dataIF_RX->report()->update();
        SP1401->ftp_put_tr_if_rx_freq_res_test();
    }
}

void QTestR1CFreqResThread::checkRF_RX(rf_rx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_rf_rx_freq_res_min() || data.pwr > spec::test_rf_rx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 5.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_rf_rx_freq_res()) {
        totalResult = false;
        dataRF_RX->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_RF_RX_FREQ_RES,int(totalResult));
}

void QTestR1CFreqResThread::checkIF_RX(if_rx_freq_res_test_data &data)
{
    bool result = true;
    if (data.pwr < spec::test_if_rx_freq_res_min() || data.pwr > spec::test_if_rx_freq_res_max())
        result = false;
    if (data.pwr - pwrBefore > 3.0)
        result = false;

    if (pwrMax - pwrMin > spec::test_if_rx_freq_res()) {
        totalResult = false;
        dataIF_RX->report()->set_result("Fail");
    }

    data.set_result(result ? test_common_data::PASS : test_common_data::FAIL);
    data.set_time();
    updateTotalResult(result);

    emit update(QModelIndex(),QModelIndex(),TI_IF_RX_FREQ_RES,int(totalResult));
}
