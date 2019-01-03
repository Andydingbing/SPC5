#include "QCalR1CRxRefThread.h"
#include "qcalr1crxrefdlg.h"

QCalR1CRxRefThread::QCalR1CRxRefThread(CalParam *param, QObject *parent) :
    QCalBaseThread(param,parent),
    sgPwr(-160),
    pmIL(PM_IL)
{

}

void QCalR1CRxRefThread::run()
{
    try
    {
        QString strFreqStar = calParam.rfFreqStar;
        QString strFreqStop = calParam.rfFreqStop;
        QString strFreqStep = calParam.rfFreqStep;
        uint64_t freq[RF_RX_FREQ_PTS_CALLED_R1C];
        uint64_t freqStar[16];
        uint64_t freqStop[16];
        memset(freq,0,sizeof(freq));
        memset(freqStar,0,sizeof(freqStar));
        memset(freqStop,0,sizeof(freqStop));
        int pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

        CAL_THREAD_START("Rx Reference",pts);

        CalParam::CalIOMode calMode = calParam.mode;

        THREAD_CHECK_BOX("Rx<===>Z28<===>Signal Generator");

        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

        qint32 lnaAtt = 0;
        qint32 att019 = 0;
        double att1 = 0.0;
        double att2 = 0.0;
        double att3 = 0.0;

        qint32 ref = 30;
        qint64 ad = 0;
        double dBc = 0.0;
        qint32 ajustCnt = 0;
        qint32 secBfr = -1;
        qint32 secCur = 0;

        rx_filter_160m_table::data_m_t dataFilter;
        double coefReal[RX_FILTER_ORDER] = {0.0};
        double coefImag[RX_FILTER_ORDER] = {0.0};

        Instr.init();
        Instr.has_pm();
        Instr.has_sg();
        Instr.pm_reset();
        Instr.sg_reset();
        Instr.sg_set_en_mod(false);
        Instr.sg_set_pl(-60.0);
        Instr.sg_set_en_output(true);

        pSP1401->set_tx_att(30.0,30.0,30.0,30.0);
        pSP1401->set_rx_if_filter_sw(basic_sp1401::_160MHz);
        pSP2401->set_tx_pwr_comp(-5.0);
        pSP2401->set_rx_pwr_comp(0);
        pSP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
        pSP1401->get_cal_file()->set_bw(_160M);

        rx_ref_table_r1cd::data_f_t data;

        if (calOP(calMode)) {
            CAL_THREAD_TEST_CANCEL
            QR1CRxRefModel *modelOP = (QR1CRxRefModel *)(calParam.model_0);
            pSP1401->set_io_mode(OUTPUT);

            for (qint32 i = 0; i < pts ;i ++) {
                CAL_THREAD_TEST_CANCEL
                data.freq = freq[i];

                pSP1401->get_cal_file()->m_rx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_rx_filter(coefReal,coefImag);
                pSP1401->set_rx_freq(freq[i]);

                for (qint32 j = 0;j < ARRAY_SIZE(data.state);j ++) {
                    rx_ref_op_table_r1cd::guess_base(j,ref,lnaAtt,att019,att1,att2,att3);
                    ajustSG((double)freq[i],ref);
                    pSP1401->set_rx_lna_att_sw(sp1401::rx_lna_att_t(lnaAtt));
                    pSP1401->set_rx_att_019_sw(sp1401::rx_att_019_t(att019));
                    pSP1401->set_rx_att(att1,att2,att3);
                    msleep(2);
                    getADS5474(pSP1401,ad,AVERAGE_TIMES);
                    dBc = ad2dBc(_0dBFS,ad);

                    ajustCnt = 0;
                    while (abs(dBc) > 0.5) {
                        if (-dBc > att3) {
                            att2 += att3 + dBc;
                            att3 = 0;
                            att2 = att2 < 0.0 ? 0.0 : att2;
                        }
                        else {
                            att3 += dBc;
                        }
                        att3 = att3 < 0.0 ? 0.0 : att3;
                        att3 = att3 > 31.5 ? 31.5 : att3;
                        hmc624(&att2);
                        hmc624(&att3);
                        pSP1401->set_rx_att2(att2);
                        pSP1401->set_rx_att3(att3);
                        msleep(2);

                        getADS5474(pSP1401,ad,AVERAGE_TIMES);
                        dBc = ad2dBc(_0dBFS,ad);

                        if (++ajustCnt > 10)
                            break;
                    }
                    data.state[j].lna_att = (int16_t)lnaAtt;
                    data.state[j].att_019 = (int16_t)att019;
                    data.state[j].att1 = (float)att1;
                    data.state[j].att2 = (float)att2;
                    data.state[j].att3 = (float)att3;
                    data.state[j].ad_offset = _0dBFS - ad;
                    pSP1401->get_temp0_rx_lo2(&data.state[j].temp[0]);
                    pSP1401->get_temp1_rx_lna(&data.state[j].temp[1]);
                    pSP1401->get_temp2_rx_lo1(&data.state[j].temp[2]);
                    pSP1401->get_temp3_rx_pa1(&data.state[j].temp[3]);

                    secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                    if (secCur != secBfr)
                        modelOP->iterTable(j)->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                    modelOP->iterTable(j)->at(secCur)->addOneData();
                }
                secBfr = secCur;

                data.time = getCurTime();
                modelOP->calTable()->replace(i,data);
                emit update(modelOP->index(i * 3,0),
                            modelOP->index(i * 3,13),
                            cal_file::RX_REF_OP,
                            secCur);
                pSP1401->get_cal_file()->add(cal_file::RX_REF_OP,&data);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_REF_OP);
            pSP1401->get_cal_file()->m_rx_ref_op->save_as("c:\\rx_ref_op.txt");
        }

        secBfr = -1;
        secCur = 0;

        if (calIO(calMode)) {
            Instr.pm_reset();
            Instr.sg_reset();
            Instr.sg_set_en_mod(false);
            Instr.sg_set_pl(-60.0);
            Instr.sg_set_en_output(true);
            QR1CRxRefModel *modelIO = (QR1CRxRefModel *)(calParam.model_1);
            CAL_THREAD_TEST_CANCEL
            pSP1401->set_io_mode(IO);

            for (qint32 i = 0; i < pts ;i ++) {
                CAL_THREAD_TEST_CANCEL
                data.freq = freq[i];

                pSP1401->get_cal_file()->m_rx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_rx_filter(coefReal,coefImag);
                pSP1401->set_rx_freq(freq[i]);

                for (qint32 j = 0;j < ARRAY_SIZE(data.state);j ++) {
                    rx_ref_io_table_r1cd::guess_base(j,ref,lnaAtt,att019,att1,att2,att3);
                    ajustSG((double)freq[i],ref);
                    pSP1401->set_rx_lna_att_sw(sp1401::rx_lna_att_t(lnaAtt));
                    pSP1401->set_rx_att_019_sw(sp1401::rx_att_019_t(att019));
                    pSP1401->set_rx_att(att1,att2,att3);
                    msleep(2);
                    getADS5474(pSP1401,ad,AVERAGE_TIMES);
                    dBc = ad2dBc(_0dBFS,ad);

                    ajustCnt = 0;
                    while (abs(dBc) > 0.5) {
                        if (-dBc > att3) {
                            att2 += att3 + dBc;
                            att3 = 0;
                            att2 = att2 < 0.0 ? 0.0 : att2;
                        }
                        else {
                            att3 += dBc;
                        }
                        att3 = att3 < 0.0 ? 0.0 : att3;
                        att3 = att3 > 31.5 ? 31.5 : att3;
                        hmc624(&att2);
                        hmc624(&att3);
                        pSP1401->set_rx_att2(att2);
                        pSP1401->set_rx_att3(att3);
                        msleep(2);

                        getADS5474(pSP1401,ad,AVERAGE_TIMES);
                        dBc = ad2dBc(_0dBFS,ad);

                        if (++ajustCnt > 10)
                            break;
                    }
                    data.state[j].lna_att = (int16_t)lnaAtt;
                    data.state[j].att_019 = (int16_t)att019;
                    data.state[j].att1 = (float)att1;
                    data.state[j].att2 = (float)att2;
                    data.state[j].att3 = (float)att3;
                    data.state[j].ad_offset = _0dBFS - ad;
                    pSP1401->get_temp0_rx_lo2(&data.state[j].temp[0]);
                    pSP1401->get_temp1_rx_lna(&data.state[j].temp[1]);
                    pSP1401->get_temp2_rx_lo1(&data.state[j].temp[2]);
                    pSP1401->get_temp3_rx_pa1(&data.state[j].temp[3]);

                    secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                    if (secCur != secBfr)
                        modelIO->iterTable(j)->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                    modelIO->iterTable(j)->at(secCur)->addOneData();
                }
                secBfr = secCur;

                data.time = getCurTime();

                modelIO->calTable()->replace(i,data);
                emit update(modelIO->index(i * 3,0),
                            modelIO->index(i * 3,13),
                            cal_file::RX_REF_IO,
                            secCur);

                pSP1401->get_cal_file()->add(cal_file::RX_REF_IO,&data);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_REF_IO);
            pSP1401->get_cal_file()->m_rx_ref_io->save_as("c:\\rx_ref_io.txt");
        }
        CAL_THREAD_ABOART
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}

void QCalR1CRxRefThread::ajustSG(double freq,qint32 pwr)
{
    if (pwr == sgPwr)
        return;
    double pmPwr = 0.0;
    Instr.sg_set_cw(freq);
    Instr.sg_set_pl(pwr + pmIL);
    msleep(2);
    Instr.pm_set_freq(freq);
    for (qint32 i = 0;i < 10;i ++) {
        Instr.pm_get_pwr(freq,pmPwr);
        if (abs(pwr - pmPwr) < 0.05)
            break;
        Instr.sg_set_pl(pwr + pmIL + (pwr - pmPwr));
        msleep(2);
        pmIL += (pwr - pmPwr);
    }
    sgPwr = pwr;
}


QExportR1CRxRefThread::QExportR1CRxRefThread(CalParam *param, QObject *parent) : QExportBaseThread(param,parent)
{

}

void QExportR1CRxRefThread::run()
{
    INIT_PROG("Exporting Rx Reference",100);

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

    rx_ref_table_r1cd::data_f_t data;

    if (calOP(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::RX_REF_OP);
        QR1CRxRefModel *modelOP = (QR1CRxRefModel *)(calParam.model_0);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_rx_ref_op->get_base(freq[i],&data);
            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < ARRAY_SIZE(data.state);j ++) {
                if (secCur != secBfr)
                    modelOP->iterTable(j)->at(secCur)->locate2CalTable(modelOP->calTable()->begin() + i);
                modelOP->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;
            modelOP->calTable()->replace(i,data);
        }

        emit update(modelOP->index(0,0),modelOP->index(pts,13),cal_file::RX_REF_OP,secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        pSP1401->get_cal_file()->map2buf(cal_file::RX_REF_IO);
        QR1CRxRefModel *modelIO = (QR1CRxRefModel *)(calParam.model_1);

        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_rx_ref_io->get_base(freq[i],&data);
            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < ARRAY_SIZE(data.state);j ++) {
                if (secCur != secBfr)
                    modelIO->iterTable(j)->at(secCur)->locate2CalTable(modelIO->calTable()->begin() + i);
                modelIO->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;
            modelIO->calTable()->replace(i,data);
        }

        emit update(modelIO->index(0,0),modelIO->index(pts,13),cal_file::RX_REF_IO,secCur);
    }

    SET_PROG_POS(100);
    THREAD_ABORT
}
