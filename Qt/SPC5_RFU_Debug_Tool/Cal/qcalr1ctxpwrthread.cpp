#include "qcalr1ctxpwrthread.h"
#include "qcalr1ctxpwrdlg.h"

QCalR1CTxPwrThread::QCalR1CTxPwrThread(CalParam *param, QObject *parent) : QCalBaseThread(param,parent)
{

}

void QCalR1CTxPwrThread::run()
{
    try {
        QString strFreqStar = calParam.rfFreqStar;
        QString strFreqStop = calParam.rfFreqStop;
        QString strFreqStep = calParam.rfFreqStep;
        uint64_t freq[RF_TX_FREQ_PTS_CALLED];
        uint64_t freqStar[16];
        uint64_t freqStop[16];
        memset(freq,0,sizeof(freq));
        memset(freqStar,0,sizeof(freqStar));
        memset(freqStop,0,sizeof(freqStop));
        int pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

        CAL_THREAD_START("Tx Power",pts);

        CalParam::CalIOMode calMode = calParam.mode;

        if (calOP(calMode))
            THREAD_CHECK_BOX("Tx<===>Power Meter");
        if (calIO(calMode) && !calOP(calMode))
            THREAD_CHECK_BOX("IO<===>Power Meter");

        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

        double att0 = 0.0;
        double att1 = 0.0;
        double att2 = 0.0;
        double att3 = 0.0;
        double d_gain = -5.0;
        double pwrPM = -100.0;
        double pwrOffset = 0.0;
        int ajustCnt = 0;
        int secBfr = -1;
        int secCur = 0;

        tx_lol_table_r1cd::data_m_t	dataLOL;
        tx_sb_table_r1cd::data_m_t	dataSB;
        tx_filter_160m_table::data_m_t dataFilter;
        double coefReal[TX_FILTER_ORDER_2I] = {0.0};
        double coefImag[TX_FILTER_ORDER_2I] = {0.0};

        Instr.init();
        Instr.has_pm();
        Instr.pm_reset();

        pSP1401->get_cal_file()->set_bw(_160M);
        pSP1401->set_rx_att_019_sw(sp1401::RX_ATT_19);
        pSP1401->set_rx_lna_att_sw(sp1401::RX_ATT);
        pSP1401->set_rx_att(30.0,30.0,30.0);
        pSP2401->set_tx_filter_sw(true);
        pSP2401->set_dds_src(sp2401_r1a::SINGLE_TONE);
        pSP2401->set_dds1(20000000.0);
        pSP2401->set_duc_dds(0.0);

        pSP1401->get_cal_file()->m_tx_sb->get(freq[0],&dataSB);
        pSP1401->get_cal_file()->m_tx_lol->get(freq[0],&dataLOL);
        pSP2401->set_tx_phase_rotate_I((double)(dataSB.th));
        pSP2401->set_tx_amplitude_balance(dataSB.am_i,dataSB.am_q);
        pSP2401->set_tx_dc_offset((uint16_t)(dataLOL.dc_i),(uint16_t)(dataLOL.dc_q));

        tx_pwr_table_r1cd::data_f_t data;

        if (calOP(calMode)) {
            QR1CTxPwrModel *model_op = (QR1CTxPwrModel *)(calParam.model_0);
            pSP1401->set_io_mode(OUTPUT);

            for (int i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL

                pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_tx_filter(coefReal,coefImag);
                pSP1401->set_tx_freq(freq[i]);
                Instr.pm_set_freq((double)(freq[i] + 20000000));

                d_gain = -5.0;
                tx_pwr_op_table_r1cd::guess_base(att0,att1,att2,att3,d_gain);
                pSP2401->set_tx_pwr_comp(d_gain);
                pSP1401->set_tx_att(att0,att1,att2,att3);
                msleep(5);

                Instr.pm_get_pwr((double)freq[i],pwrPM);
                pwrOffset = R1C_TX_BASE_POWER_OP - pwrPM;

                ajustCnt = 0;
                while (abs(pwrOffset) > 0.5) {
                    if (pwrOffset > att0) {		//even if dAtt0 == 0.0,this is still correct
                        att1 -= (pwrOffset - att0);
                        att0 = 0.0;
                        att1 = att1 < 0.0 ? 0.0 : att1;
                    }
                    else {
                        att0 -= pwrOffset;
                    }
                    att0 = att0 > 20.0 ? 20.0 : att0;
                    hmc624(&att0);
                    hmc624(&att1);
                    pSP1401->set_tx_att0(att0);
                    pSP1401->set_tx_att1(att1);
                    msleep(5);

                    Instr.pm_get_pwr((double)freq[i],pwrPM);
                    pwrOffset = R1C_TX_BASE_POWER_OP - pwrPM;

                    if (++ajustCnt > 10)
                        break;
                };
                ajustCnt = 0;
                while (abs(pwrOffset) > 0.1) {
                    d_gain += pwrOffset;
                    pSP2401->set_tx_pwr_comp(d_gain);

                    Instr.pm_get_pwr((double)freq[i],pwrPM);
                    pwrOffset = R1C_TX_BASE_POWER_OP - pwrPM;

                    if (++ajustCnt > 10)
                        break;
                }
                data.freq = freq[i];
                data.d_gain = d_gain;
                data.att0 = att0;
                data.att1 = att1;
                data.att2 = att2;
                data.att3 = att3;
                pSP1401->get_temp4_tx_lo2(&data.temp[0]);
                pSP1401->get_temp5_tx_lo1(&data.temp[1]);
                pSP1401->get_temp6_tx_pa4(&data.temp[2]);
                pSP1401->get_temp7_tx_pa3(&data.temp[3]);
                data.time = getCurTime();
                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                if (secCur != secBfr) {
                    model_op->iterTable()->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                    secBfr = secCur;
                }
                model_op->calTable()->replace(i,data);
                model_op->iterTable()->at(secCur)->addOneData();
                pSP1401->get_cal_file()->add(cal_file::TX_PWR_OP,(tx_pwr_op_table_r1cd::data_f_t *)&data);
                emit update(model_op->index(i,0),model_op->index(i,11),cal_file::TX_PWR_OP,secCur);
                SET_PROG_POS(i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            pSP1401->get_cal_file()->w(cal_file::TX_PWR_OP);
            pSP1401->get_cal_file()->m_tx_pwr_op->save_as("c:\\tx_power_op.txt");
        }

        if (calOP(calMode) && calIO(calMode))
            THREAD_CHECK_BOX("Output Complete,IO<===>Power Meter");

        secBfr = -1;
        secCur = 0;

        if (calIO(calMode)) {
            Instr.pm_reset();
            QR1CTxPwrModel *model_io = (QR1CTxPwrModel *)(calParam.model_1);
            pSP1401->set_io_mode(IO);

            for (int i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_PAUSE_S
                CAL_THREAD_TEST_CANCEL

                pSP1401->get_cal_file()->m_tx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_tx_filter(coefReal,coefImag);
                pSP1401->set_tx_freq(freq[i]);
                Instr.pm_set_freq((double)(freq[i] + 20000000));

                d_gain = -5.0;
                tx_pwr_io_table_r1cd::guess_base(att0,att1,att2,att3,d_gain);
                pSP2401->set_tx_pwr_comp(d_gain);
                pSP1401->set_tx_att(att0,att1,att2,att3);
                msleep(5);

                Instr.pm_get_pwr((double)freq[i],pwrPM);
                pwrOffset = R1C_TX_BASE_POWER_IO - pwrPM;

                ajustCnt = 0;
                while (abs(pwrOffset) > 0.5) {
                    if (pwrOffset > att0) {		//even if dAtt0 == 0.0,this is still correct
                        att1 -= (pwrOffset - att0);
                        att0 = 0.0;
                        att1 = att1 < 0.0 ? 0.0 : att1;
                    }
                    else {
                        att0 -= pwrOffset;
                    }
                    att0 = att0 > 20.0 ? 20.0 : att0;
                    hmc624(&att0);
                    hmc624(&att1);
                    pSP1401->set_tx_att0(att0);
                    pSP1401->set_tx_att1(att1);
                    msleep(5);

                    Instr.pm_get_pwr((double)freq[i],pwrPM);
                    pwrOffset = R1C_TX_BASE_POWER_IO - pwrPM;

                    if (++ajustCnt > 10)
                        break;
                };
                ajustCnt = 0;
                while (abs(pwrOffset) > 0.1) {
                    d_gain += pwrOffset;
                    pSP2401->set_tx_pwr_comp(d_gain);

                    Instr.pm_get_pwr((double)freq[i],pwrPM);
                    pwrOffset = R1C_TX_BASE_POWER_IO - pwrPM;

                    if (++ajustCnt > 10)
                        break;
                }
                data.freq = freq[i];
                data.d_gain = d_gain;
                data.att0 = att0;
                data.att1 = att1;
                data.att2 = att2;
                data.att3 = att3;
                pSP1401->get_temp4_tx_lo2(&data.temp[0]);
                pSP1401->get_temp5_tx_lo1(&data.temp[1]);
                pSP1401->get_temp6_tx_pa4(&data.temp[2]);
                pSP1401->get_temp7_tx_pa3(&data.temp[3]);
                data.time = getCurTime();
                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                if (secCur != secBfr) {
                    model_io->iterTable()->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                    secBfr = secCur;
                }
                model_io->calTable()->replace(i,data);
                model_io->iterTable()->at(secCur)->addOneData();
                pSP1401->get_cal_file()->add(cal_file::TX_PWR_IO,(tx_pwr_io_table_r1cd::data_f_t *)&data);
                emit update(model_io->index(i,0),model_io->index(i,11),cal_file::TX_PWR_IO,secCur);
                SET_PROG_POS(i + 1);
                CAL_THREAD_TEST_PAUSE_E
            }
            pSP1401->get_cal_file()->w(cal_file::TX_PWR_IO);
            pSP1401->get_cal_file()->m_tx_pwr_io->save_as("c:\\tx_power_io.txt");
        }
        CAL_THREAD_ABOART
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}


QExportR1CTxPwrThread::QExportR1CTxPwrThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CTxPwrThread::run()
{
    INIT_PROG("Exporting Tx Power",100);

    QString strFreqStar = calParam.rfFreqStar;
    QString strFreqStop = calParam.rfFreqStop;
    QString strFreqStep = calParam.rfFreqStep;
    CalParam::CalIOMode calMode = calParam.mode;
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    int secBfr = -1;
    int secCur = 0;

    quint64 freq[RF_TX_FREQ_PTS_CALLED];
    quint64 freqStar[16];
    quint64 freqStop[16];
    memset(freq,0,sizeof(freq));
    memset(freqStar,0,sizeof(freqStar));
    memset(freqStop,0,sizeof(freqStop));
    int pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

    tx_pwr_table_r1cd::data_f_t data;

    if (calOP(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::TX_PWR_OP);
        QR1CTxPwrModel *model_op = (QR1CTxPwrModel *)(calParam.model_0);

        for (int32_t i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_tx_pwr_op->get_base(freq[i],&data);
            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            if (secCur != secBfr) {
                model_op->iterTable()->at(secCur)->locate2CalTable(model_op->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_op->calTable()->replace(i,data);
            model_op->iterTable()->at(secCur)->addOneData();
        }

        emit update(model_op->index(0,0),model_op->index(pts,11),cal_file::TX_PWR_OP,secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::TX_PWR_IO);
        QR1CTxPwrModel *model_io = (QR1CTxPwrModel *)(calParam.model_1);

        for (int32_t i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_tx_pwr_io->get_base(freq[i],&data);
            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            if (secCur != secBfr) {
                model_io->iterTable()->at(secCur)->locate2CalTable(model_io->calTable()->begin() + i);
                secBfr = secCur;
            }
            model_io->calTable()->replace(i,data);
            model_io->iterTable()->at(secCur)->addOneData();
        }

        emit update(model_io->index(0,0),model_io->index(pts,11),cal_file::TX_PWR_OP,secCur);
    }

    SET_PROG_POS(100);
    THREAD_ABORT
}
