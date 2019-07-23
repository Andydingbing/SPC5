#include "q_test_rf_thread.h"
#include "q_model_freq_res.h"
#include "algorithm.h"
#include "spec.h"

using namespace std;

void QTestR1CRFThread::run()
{
    RD_TEST_TRY

    TestRFParam *param = static_cast<TestRFParam *>(testParam);

//    quint32 pts = 0;
//    pts += param->isTestTX_PhaseNoise ?  : 0;
//    pts += param->isTestRF_RX ? RF_RXFreqRange.freqs.size() : 0;
//    pts += param->isTestIF_TX ? IF_TX_FILTER_160M_PTS : 0;
//    pts += param->isTestIF_RX ? IF_RX_FILTER_160M_PTS : 0;

//    TEST_THREAD_START("RF",pts);

    if (param->isTestTX_PhaseNoise) {
        TX_PhaseNoise();
    }

//    TEST_THREAD_TEST_CANCEL


    TEST_THREAD_ABOART
    RD_TEST_CATCH
}

void QTestR1CRFThread::TX_PhaseNoise()
{
    Instr.init();
    Instr.has_sa();
    Instr.sa_reset();
    Instr.sa_set_vbw(1000.0,true);
    Instr.sa_set_ref(R1C_TX_BASE_POWER_OP + 20.0);

    quint64 Freq = 0;
    string freqStrOffset;
    string freqStrSpan;
    string freqStrRBW;
    QVector<string> strOffset;
    QVector<string> strSpan;
    QVector<string> strRBW;
    double offset;
    double span;
    double rbw;
    double pwr[2] = { 0.0 };

    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = 0.0;

    bool res = true;

    range_freq_string freqString;
    range_freq<quint64> freqRange;

    spec::test_tx_phase_noise_freq(freqString);
    spec::test_tx_phase_noise_offset(freqStrOffset);
    spec::test_tx_phase_noise_span(freqStrSpan);
    spec::test_tx_phase_noise_rbw(freqStrRBW);

    parse_range_freq_string(freqString,freqRange);
    split_freq_string(freqStrOffset,freq_string_sections(freqStrOffset),strOffset);
    split_freq_string(freqStrSpan,freq_string_sections(freqStrSpan),strSpan);
    split_freq_string(freqStrRBW,freq_string_sections(freqStrRBW),strRBW);

    if (strOffset.size() != strSpan.size() || strSpan.size() != strRBW.size()) {
        RD_THROW sp_rd::runtime_error("Invalid SA configuration");
    }

    tx_phase_noise_test_data data;

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
    SP2401->set_dds1(0.0);
    SP2401->set_duc_dds(0.0);
    SP2401->set_tx_phase_rotate_I(0.0);
    SP2401->set_tx_amplitude_balance(8192,8192);
    SP2401->set_tx_dc_offset(0,0);
    SP2401->set_tx_filter_sw(false);

    SP1401->prepare_tr_tx_phase_noise_test();
    SP1401->tr_tx_phase_noise_test()->set_result("Pass");

    for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
        TEST_THREAD_TEST_CANCEL

        Freq = freqRange.freqs.at(i);
        SP1401->set_tx_freq(Freq);
        msleep(50);

        Instr.sa_set_cf(Freq);
        Instr.sa_set_span(1e6);
        Instr.sa_set_rbw(1000.0,true);
        Instr.sa_set_avg_trace(false,1);
        Instr.sa_set_ref_auto();
        Instr.sa_set_avg_trace(true,20);

        res = true;
        for (quint32 j = 0;j < strOffset.size();j ++) {
            offset = freq_string_to_double(strOffset.at(j));
            span = freq_string_to_double(strSpan.at(j));
            rbw = freq_string_to_double(strRBW.at(j));
            Instr.sa_set_cf(Freq + span / 10.0);
            Instr.sa_set_span(span);
            Instr.sa_set_rbw(rbw,false);
            Instr.sa_sweep_once();
            Instr.sa_set_peak_search(sa::PEAK_HIGHEST);
            Instr.sa_get_marker_pwr(pwr[0]);
            Instr.sa_marker_move(Freq + offset);
            Instr.sa_get_marker_pwr(pwr[1]);
            if (j < ARRAY_SIZE(data.phase_noise)) {
                data.phase_noise[j] = pwr[1] - pwr[0] - 10.0 * log10(rbw);
            }
            if (data.phase_noise[j] > spec::test_tx_phase_noise(Freq,offset)) {
                res = false;
            }
        }
        data.set_result(res ? test_common_data::PASS : test_common_data::FAIL);
        data.set_time();
        SP1401->tr_tx_phase_noise_test()->add(int64_t(Freq),data);
        updateTotalResult(res);

        emit update(QModelIndex(),QModelIndex(),TI_TX_PHASE_NOISE,int(totalResult));
    }
    SP1401->tr_tx_phase_noise_test()->update();
    SP1401->ftp_put_tr_tx_phase_noise_test();
}
