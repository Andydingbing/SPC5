#include "q_cal_r1c_tx_pwr_thread.h"
#include "q_model_tx_pwr.h"
#include "algorithm.h"

void QCalR1CTXPwrThread::run()
{
    RD_CAL_TRY
    CAL_THREAD_START("TX Power",freqRange.freqs.size());

    if (calOP(calParam.mode)) {
        THREAD_CHECK_BOX("TX<===>Power Meter");
    }
    if (calIO(calParam.mode) && !calOP(calParam.mode)) {
        THREAD_CHECK_BOX("IO<===>Power Meter");
    }

    Instr.init();
    Instr.has_pm();

    init();

    if (calOP(calParam.mode)) {
        cal(OUTPUT);
    }

    if (calOP(calParam.mode) && calIO(calParam.mode)) {
        THREAD_CHECK_BOX("Output Complete,IO<===>Power Meter");
    }

    if (calIO(calParam.mode)) {
        cal(IO);
    }

    CAL_THREAD_ABOART
    RD_CAL_CATCH
}

void QCalR1CTXPwrThread::cal(io_mode_t mode)
{
    QTXPwrModel *model_op = dynamic_cast<QTXPwrModel *>(calParam.model_0);
    QTXPwrModel *model_io = dynamic_cast<QTXPwrModel *>(calParam.model_1);

    quint64 freq = 0;

    tx_pwr_table_r1c::data_f_t data;
    tx_filter_160m_table::data_m_t dataFilter;
    double coefReal[TX_FILTER_ORDER_2I] = {0.0};
    double coefImag[TX_FILTER_ORDER_2I] = {0.0};

    double att0 = 0.0;
    double att1 = 0.0;
    double att2 = 0.0;
    double att3 = 0.0;
    double d_gain = -5.0;
    double target = 0.0;

    int secBfr = -1;
    int secCur = 0;

    target = (mode == OUTPUT ? R1C_TX_BASE_POWER_OP : R1C_TX_BASE_POWER_IO);

    SP1401->set_io_mode(mode);

    Instr.pm_reset();

    for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
        CAL_THREAD_TEST_PAUSE_S
        CAL_THREAD_TEST_CANCEL

        freq = freqRange.freqs.at(i);
        SP1401->cf()->m_tx_filter_160m->get(freq,&dataFilter);
        dataFilter._2double(coefReal,coefImag);

        SP2401->set_tx_filter(coefReal,coefImag);
        SP1401->set_tx_freq(freq);

        Instr.pm_set_freq(double(freq + 20000000));

        if (mode == OUTPUT) {
            initTXBaseStateOP(att0,att1,att2,att3,d_gain);
        } else if (mode == IO) {
            initTXBaseStateIO(att0,att1,att2,att3,d_gain);
        }

        if (is_rf_ver_between(RFVer,R1C,R1E)) {
            coarseTuning(target,att0,att1);
        } else if (is_rf_ver_after(RFVer,R1F)) {
            coarseTuning(target,att1,att0);
        }

        fineTuning(target,d_gain);

        data.freq = freq;
        data.d_gain = d_gain;
        data.att0 = att0;
        data.att1 = att1;
        data.att2 = att2;
        data.att3 = att3;
        SP1401->get_temp(4,data.temp[0]);
        SP1401->get_temp(5,data.temp[1]);
        SP1401->get_temp(6,data.temp[2]);
        SP1401->get_temp(7,data.temp[3]);
        data.time = getCurTime();

        secCur = freq_section(freq,freqRange);

        if (mode == OUTPUT) {
            if (secCur != secBfr) {
                model_op->iterTable()->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_op->calTable()->replace(i,data);
            model_op->iterTable()->at(secCur)->addOneData();
            SP1401->cf()->add(cal_file::TX_PWR_OP,&data);
            emit update(model_op->index(i,0),model_op->index(i,11),cal_file::TX_PWR_OP,secCur);
        } else if (mode == IO) {
            if (secCur != secBfr) {
                model_io->iterTable()->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_io->calTable()->replace(i,data);
            model_io->iterTable()->at(secCur)->addOneData();
            SP1401->cf()->add(cal_file::TX_PWR_IO,&data);
            emit update(model_io->index(i,0),model_io->index(i,11),cal_file::TX_PWR_IO,secCur);
        }

        SET_PROG_POS(i + 1);
        CAL_THREAD_TEST_PAUSE_E
    }

    if (mode == OUTPUT) {
        SP1401->cf()->w(cal_file::TX_PWR_OP);
        SP1401->cf()->m_tx_pwr_op->save_as("c:\\tx_power_op.txt");
    }else if (mode == IO) {
        SP1401->cf()->w(cal_file::TX_PWR_IO);
        SP1401->cf()->m_tx_pwr_io->save_as("c:\\tx_power_io.txt");
    }
}

void QCalR1CTXPwrThread::init()
{
    tx_lol_table_r1cd::data_m_t	dataLOL;
    tx_sb_table_r1cd::data_m_t	dataSB;

    SP1401->cf()->set_bw(_160M);
    SP1401->set_rx_att_019_sw(sp1401::RX_ATT_19);
    SP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
    SP1401->set_rx_att(30.0,30.0,30.0);

    SP2401->set_tx_filter_sw(true);
    SP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
    SP2401->set_dds1(20000000.0);
    SP2401->set_duc_dds(0.0);

    SP1401->cf()->m_tx_sb->get(2000000000,&dataSB);
    SP1401->cf()->m_tx_lol->get(2000000000,&dataLOL);
    SP2401->set_tx_phase_rotate_I((double)(dataSB.th));
    SP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
    SP2401->set_tx_dc_offset(quint16(dataLOL.dc_i),quint16(dataLOL.dc_q));
}

void QCalR1CTXPwrThread::initTXBaseStateOP(double &att0, double &att1, double &att2, double &att3, double &dGain)
{
    att0 = 0.0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;
    dGain = 0.0;

    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
        tx_pwr_op_table_r1c::guess_base_r1c(att0,att1,att2,att3,dGain);
    } else if (RFVer == R1F) {
        tx_pwr_op_table_r1c::guess_base_r1f(att0,att1,att2,dGain);
    }

    SP2401->set_tx_pwr_comp(dGain);
    SP1401->set_tx_att(att0,att1,att2,att3);
    msleep(50);
}

void QCalR1CTXPwrThread::initTXBaseStateIO(double &att0, double &att1, double &att2, double &att3, double &dGain)
{
    att0 = 0.0;
    att1 = 0.0;
    att2 = 0.0;
    att3 = 0.0;
    dGain = 0.0;

    if (RFVer == R1C || RFVer == R1D || RFVer == R1E) {
        tx_pwr_io_table_r1c::guess_base_r1c(att0,att1,att2,att3,dGain);
    } else if (RFVer == R1F) {
        tx_pwr_io_table_r1c::guess_base_r1f(att0,att1,att2,dGain);
    }

    SP2401->set_tx_pwr_comp(dGain);
    SP1401->set_tx_att(att0,att1,att2,att3);
    msleep(50);
}

void QCalR1CTXPwrThread::coarseTuning(double target, double &att0, double &att1)
{
    double pwrPM = -100.0;
    double offset = -100.0;
    int count = 0;

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.5) {
        if (offset > att0) {		// Even if att0 == 0.0,this is still correct.
            att1 -= (offset - att0);
            att0 = 0.0;
            att1 = att1 < 0.0 ? 0.0 : att1;
        } else {
            att0 -= offset;
        }
        att0 = att0 > 20.0 ? 20.0 : att0;
        hmc624(&att0);
        hmc624(&att1);

        if (is_rf_ver_between(RFVer,R1C,R1E)) {
            SP1401->set_tx_att0(att0);
            SP1401->set_tx_att1(att1);
        } else if (is_rf_ver_after(RFVer,R1F)) {
            SP1401->set_tx_att0(att1);
            SP1401->set_tx_att1(att0);
        }
        msleep(50);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10)
            break;
    };
}

void QCalR1CTXPwrThread::fineTuning(double target, double &dGain)
{
    double pwrPM = -100.0;
    double offset = -100.0;
    int count = 0;

    Instr.pm_get_pwr(pwrPM);
    offset = target - pwrPM;

    while (abs(offset) > 0.1) {
        dGain += offset;
        SP2401->set_tx_pwr_comp(dGain);

        Instr.pm_get_pwr(pwrPM);
        offset = target - pwrPM;

        if (++count > 10)
            break;
    }
}


void QExpR1CTXPwrThread::run()
{
    INIT_PROG("Exporting Tx Power",100);

    QTXPwrModel *model_op = dynamic_cast<QTXPwrModel *>(calParam.model_0);
    QTXPwrModel *model_io = dynamic_cast<QTXPwrModel *>(calParam.model_1);

    CalIOMode calMode = calParam.mode;
    quint64 freq = 0;
    int secBfr = -1;
    int secCur = 0;

    tx_pwr_table_r1c::data_f_t data;

    if (calOP(calMode)) {
        SP1401->cf()->map2buf(cal_file::TX_PWR_OP);

        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            freq = freqRange.freqs.at(i);
            SP1401->cf()->m_tx_pwr_op->get_base(freq,&data);
            secCur = freq_section(freq,freqRange);
            if (secCur != secBfr) {
                model_op->iterTable()->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_op->calTable()->replace(i,data);
            model_op->iterTable()->at(secCur)->addOneData();
        }

        emit update(model_op->index(0,0),
                    model_op->index(freqRange.freqs.size(),11),
                    cal_file::TX_PWR_OP,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        SP1401->cf()->map2buf(cal_file::TX_PWR_IO);

        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            freq = freqRange.freqs.at(i);
            SP1401->cf()->m_tx_pwr_io->get_base(freq,&data);
            secCur = freq_section(freq,freqRange);
            if (secCur != secBfr) {
                model_io->iterTable()->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_io->calTable()->replace(i,data);
            model_io->iterTable()->at(secCur)->addOneData();
        }

        emit update(model_io->index(0,0),
                    model_io->index(freqRange.freqs.size(),11),
                    cal_file::TX_PWR_OP,
                    secCur);
    }

    SET_PROG_POS(100);
    THREAD_ABORT
}
