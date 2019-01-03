#include "qcalr1crxattthread.h"
#include "qcalr1crxattdlg.h"

QCalR1CRxAttThread::QCalR1CRxAttThread(CalParam *param, QObject *parent) :
    QCalBaseThread(param,parent),
    sgPwr(-160),
    pmIL(PM_IL)
{

}

void QCalR1CRxAttThread::run()
{
    try {
        QString strFreqStar = calParam.rfFreqStar;
        QString strFreqStop = calParam.rfFreqStop;
        QString strFreqStep = calParam.rfFreqStep;
        quint64 freq[RF_RX_FREQ_PTS_CALLED_R1C];
        quint64 freqStar[16];
        quint64 freqStop[16];
        memset(freq,0,sizeof(freq));
        memset(freqStar,0,sizeof(freqStar));
        memset(freqStop,0,sizeof(freqStop));
        qint32 pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

        CAL_THREAD_START("Rx Att Offset",pts);
        THREAD_CHECK_BOX("Rx<===>Z28<===>Signal Generator");

        CalParam::CalIOMode calMode = calParam.mode;

        sp2401_r1a *pSP2401 = calParam._sp2401;
        sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);

        qint64 adBase = 0;
        qint64 ad = 0;
        qint64 offsetBase = 0;
        qint32 attIdx = 0;
        qint32 ref = 30;
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

        if (calOP(calMode)) {
            CAL_THREAD_TEST_CANCEL

            rx_att_op_table_r1cd::data_f_t data;
            rx_ref_op_table_r1cd::data_m_t dataRefOP;
            rx_ref_op_table_r1cd::rx_state_m_t state;
            QR1CRxAttOPModel *model = (QR1CRxAttOPModel *)(calParam.model_0);

            pSP1401->set_io_mode(OUTPUT);

            for (qint32 i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL

                pSP1401->get_cal_file()->m_rx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_rx_filter(coefReal,coefImag);
                pSP1401->set_rx_freq(freq[i]);

                pSP1401->get_cal_file()->m_rx_ref_op->get_base(freq[i],&dataRefOP);

                //	ref		att0	att1	att2	att3
                //	 30		 -20	 -25	 -15	 -10
                //	 20		 -20	 -15	 -15	 -10
                //	 10		 -20	  -5	 -15	 -10////state[0]
                attIdx = 0;
                offsetBase = 0;
                ajustSG(double(freq[i]),10);
                setRxState(dataRefOP.state[0]);
                getADS5474(pSP1401,adBase);
                for (ref = 30;ref >= 11;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_op->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,10.0 - ref);
                    attIdx ++;
                }

                //	ref		att0	att1	att2	att3
                //	 10		   0	 -25	 -15	 -10////state[1]
                //	  0		   0	 -15	 -15	 -10
                //	-10		   0	  -5	 -15	 -10
                //	-20		   0	  -5	  -5	 -10
                ajustSG(double(freq[i]),0);
                setRxState(dataRefOP.state[1]);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-10.0);
                for (ref = 9;ref >= 0;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_op->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,0.0 - ref);
                    attIdx ++;
                }
                offsetBase = data.offset[attIdx - 1];

                ajustSG(double(freq[i]),-10);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-10.0) + offsetBase;
                for (ref = -1;ref >= -10;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_op->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,-10.0 - ref);
                    attIdx ++;
                }
                offsetBase = data.offset[attIdx - 1];

                ajustSG(double(freq[i]),-19);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-9.0) + offsetBase;
                for (ref = -11;ref >= -19;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_op->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,-19.0 - ref);
                    attIdx ++;
                }

                //	ref		att0	att1	att2	att3
                //	-20		  14	 -10	 -15	 -10////state[2]
                //	-30		  14	   0	 -15	 -10
                //	-40		  14	   0	  -5	 -10
                ajustSG(double(freq[i]),-30);
                setRxState(dataRefOP.state[2]);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-10.0);
                for (ref = -21;ref >= -30;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_op->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,-30.0 - ref);
                    attIdx ++;
                }
                offsetBase = data.offset[attIdx - 1];

                ajustSG(double(freq[i]),-40);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-10.0) + offsetBase;
                for (ref = -31;ref >= -40;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_op->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,-40.0 - ref);
                    attIdx ++;
                }
                data.freq = freq[i];
                pSP1401->get_temp0_rx_lo2(&data.temp[0]);
                pSP1401->get_temp1_rx_lna(&data.temp[1]);
                pSP1401->get_temp2_rx_lo1(&data.temp[2]);
                pSP1401->get_temp3_rx_pa1(&data.temp[3]);
                data.time = getCurTime();

                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                for (qint32 j = 0;j < attIdx;j ++) {
                    if (secCur != secBfr)
                        model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                    model->iterTable(j)->at(secCur)->addOneData();
                }
                secBfr = secCur;

                model->calTable()->replace(i,data);
                pSP1401->get_cal_file()->add(cal_file::RX_ATT_OP,&data);
                emit update(model->index(i * attIdx,0),
                            model->index((i + 1) * attIdx,14),
                            cal_file::RX_ATT_OP,
                            secCur);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_ATT_OP);
        }

        secBfr = -1;
        secCur = 0;

        if (calIO(calMode)) {
            CAL_THREAD_TEST_CANCEL

            rx_att_io_table_r1cd::data_f_t data;
            rx_ref_io_table_r1cd::data_m_t dataRefIO;
            rx_ref_io_table_r1cd::rx_state_m_t state;
            QR1CRxAttIOModel *model = (QR1CRxAttIOModel *)(calParam.model_1);

            pSP1401->set_io_mode(IO);

            for (qint32 i = 0;i < pts;i ++) {
                CAL_THREAD_TEST_CANCEL

                pSP1401->get_cal_file()->m_rx_filter_160m->get(freq[i],&dataFilter);
                dataFilter._2double(coefReal,coefImag);
                pSP2401->set_rx_filter(coefReal,coefImag);
                pSP1401->set_rx_freq(freq[i]);
                pSP1401->get_cal_file()->m_rx_ref_io->get_base(freq[i],&dataRefIO);

                //	ref		att0	att1	att2	att3
                //	 30		 -20	 -20	 -13	 -10
                //	 20		 -20	 -10	 -13	 -10
                //	 10		 -20	  -0	 -13	 -10////state[0]
                attIdx = 0;
                offsetBase = 0;
                ajustSG(double(freq[i]),10);
                setRxState(dataRefIO.state[0]);
                getADS5474(pSP1401,adBase);
                for (ref = 30;ref >= 11;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_io->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,10.0 - ref);
                    attIdx ++;
                }

                //	ref		att0	att1	att2	att3
                //	 10		   0	 -20	 -13	 -10////state[1]
                //	  0		   0	 -10	 -13	 -10
                //	-10		   0	   0	 -13	 -10
                //	-20		   0	   0	  -3	 -10
                ajustSG(double(freq[i]),0);
                setRxState(dataRefIO.state[1]);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-10.0);
                for (ref = 9;ref >= 0;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_io->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,0.0 - ref);
                    attIdx ++;
                }
                offsetBase = data.offset[attIdx - 1];

                ajustSG(double(freq[i]),-10);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-10.0) + offsetBase;
                for (ref = -1;ref >= -10;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_io->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,-10.0 - ref);
                    attIdx ++;
                }
                offsetBase = data.offset[attIdx - 1];

                ajustSG(double(freq[i]),-19);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-9.0) + offsetBase;
                for (ref = -11;ref >= -19;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_io->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,-19.0 - ref);
                    attIdx ++;
                }

                //	ref		att0	att1	att2	att3
                //	-20		  14	  -4	 -13	 -10////state[2]
                //	-24		  14	   0	 -13	 -10
                //	-30		  14	   0	  -7	 -10
                ajustSG(double(freq[i]),-30);
                setRxState(dataRefIO.state[2]);
                getADS5474(pSP1401,adBase);
                adBase = dBc2ad(adBase,-10.0);
                for (ref = -21;ref >= -30;ref --) {
                    CAL_THREAD_TEST_CANCEL
                    pSP1401->get_cal_file()->m_rx_ref_io->get(freq[i],(double)ref,&state);
                    setRxState(state);
                    getADS5474(pSP1401,ad);
                    data.offset[attIdx] = adBase - dBc2ad(ad,-30.0 - ref);
                    attIdx ++;
                }

                data.freq = freq[i];
                pSP1401->get_temp0_rx_lo2(&data.temp[0]);
                pSP1401->get_temp1_rx_lna(&data.temp[1]);
                pSP1401->get_temp2_rx_lo1(&data.temp[2]);
                pSP1401->get_temp3_rx_pa1(&data.temp[3]);
                data.time = getCurTime();

                secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
                for (qint32 j = 0;j < attIdx;j ++) {
                    if (secCur != secBfr)
                        model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                    model->iterTable(j)->at(secCur)->addOneData();
                }
                secBfr = secCur;

                model->calTable()->replace(i,data);
                pSP1401->get_cal_file()->add(cal_file::RX_ATT_IO,&data);
                emit update(model->index(i * attIdx,0),
                            model->index((i + 1) * attIdx,14),
                            cal_file::RX_ATT_IO,
                            secCur);
                SET_PROG_POS(i + 1);
            }
            pSP1401->get_cal_file()->w(cal_file::RX_ATT_IO);
        }
        CAL_THREAD_ABOART
    }
    catch (instr_expt &expt) {
        THREAD_ERROR_BOX(QString("%1").arg(expt.get_expt()));
        CAL_THREAD_ABOART
    }
}

void QCalR1CRxAttThread::setRxState(rx_ref_op_table_r1cd::rx_state_m_t state)
{
    ((sp1401_r1c *)(calParam._sp1401))->set_rx_lna_att_sw(sp1401::rx_lna_att_t(state.lna_att));
    ((sp1401_r1c *)(calParam._sp1401))->set_rx_att_019_sw(sp1401::rx_att_019_t(state.att_019));
    ((sp1401_r1c *)(calParam._sp1401))->set_rx_att1(double(state.att1));
    ((sp1401_r1c *)(calParam._sp1401))->set_rx_att2(double(state.att2));
    ((sp1401_r1c *)(calParam._sp1401))->set_rx_att3(double(state.att3));
    calParam._sp2401->set_rx_pwr_comp(state.ad_offset);
    msleep(2);
}

void QCalR1CRxAttThread::ajustSG(double freq,qint32 pwr)
{
    if (pwr == sgPwr)
        return;
    double pmPwr = 0.0;
    Instr.sg_set_cw(freq);
    Instr.sg_set_pl(pwr + pmIL);
    msleep(5);
    Instr.pm_set_freq(freq);
    for (qint32 i = 0;i < 10;i ++) {
        Instr.pm_get_pwr(freq,pmPwr);
        if (abs(pwr - pmPwr) < 0.5)
            break;
        Instr.sg_set_pl(pwr + pmIL + (pwr - pmPwr));
        msleep(5);
        pmIL += (pwr - pmPwr);
    }
    msleep(5);
    sgPwr = pwr;
}


QExportR1CRxAttThread::QExportR1CRxAttThread(CalParam *param, QObject *parent) :
    QExportBaseThread(param,parent)
{

}

void QExportR1CRxAttThread::run()
{
    INIT_PROG("Exporting Rx Att",100);

    QString strFreqStar = calParam.rfFreqStar;
    QString strFreqStop = calParam.rfFreqStop;
    QString strFreqStep = calParam.rfFreqStep;
    quint64 freq[RF_TX_FREQ_PTS_CALLED];
    quint64 freqStar[16];
    quint64 freqStop[16];
    memset(freq,0,sizeof(freq));
    memset(freqStar,0,sizeof(freqStar));
    memset(freqStop,0,sizeof(freqStop));
    qint32 pts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)freq,(int64_t *)freqStar,(int64_t *)freqStop);

    CalParam::CalIOMode calMode = calParam.mode;
    sp1401_r1c *pSP1401 = (sp1401_r1c *)(calParam._sp1401);
    int secBfr = -1;
    int secCur = 0;

    if(calOP(calMode)) {
        rx_att_op_table_r1cd::data_f_t data;
        QR1CRxAttOPModel *model = (QR1CRxAttOPModel *)(calParam.model_0);
        const qint32 offsetCnt = ARRAY_SIZE(data.offset);

        pSP1401->get_cal_file()->map2buf(cal_file::RX_ATT_OP);
        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_rx_att_op->get(freq[i],&data);

            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < offsetCnt;j ++) {
                if (secCur != secBfr)
                    model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                model->iterTable(j)->at(secCur)->addOneData();
            }

            model->calTable()->replace(i,data);
            secBfr = secCur;
        }

        emit update(model->index(0,0),
                    model->index(pts * offsetCnt,14),
                    cal_file::RX_ATT_OP,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        rx_att_io_table_r1cd::data_f_t data;
        QR1CRxAttIOModel *model = (QR1CRxAttIOModel *)(calParam.model_1);
        const qint32 offsetCnt = ARRAY_SIZE(data.offset);

        pSP1401->get_cal_file()->map2buf(cal_file::RX_ATT_IO);
        for (qint32 i = 0;i < pts;i ++) {
            pSP1401->get_cal_file()->m_rx_att_io->get(freq[i],&data);

            secCur = freq_section((int64_t *)freqStar,(int64_t *)freqStop,(int64_t)freq[i]);
            for (qint32 j = 0;j < offsetCnt;j ++) {
                if (secCur != secBfr)
                    model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                model->iterTable(j)->at(secCur)->addOneData();
            }

            model->calTable()->replace(i,data);
            secBfr = secCur;
        }

        emit update(model->index(0,0),
                    model->index(pts * offsetCnt,14),
                    cal_file::RX_ATT_IO,
                    secCur);
    }

    SET_PROG_POS(100);
    THREAD_ABORT
}
